open FacelessCore;
open Redis;
open Relude_Globals;
open Relude_IO;
open Relude_List.IO;
open Logic;
open FacelessCore.Types;
// open Redis.Client;

// -- topics
// global
// channel/$channelId

let bootstrapProgram = {
  // constants used in the progam
  let globalChannel: Types.channelInfo = {
    id: Uuid.v4(),
    displayName: "global",
    hidden: false,
    password: None,
    creationTimestamp: Logic.dateGen(),
  };
  let clientOptions: Redis.clientOptions = {port: 6379};
  let client = Redis.createClient(clientOptions);
  let channel = globalChannel;

  // clears redis of any left over channels or messages
  let wipeRedis =
    client
    |> getAllChannels
    >>= (
      list =>
        list
        |> List.map((channel: Types.channelInfo) => channel.id)
        |> traverse(id => client |> Redis_IO.del(~key=id))
    )
    |> IO.map(_ => "> redis wiped" |> Js.log);

  // creates a global channel and logs out to the console
  let createGlobalChannel =
    client
    |> createChannel(~channel)
    |> IO.map(_ => "> global channel created" |> Js.log);

  wipeRedis >>= (_ => createGlobalChannel) >>= (_ => client |> Redis_IO.quit);
};

let connectionProgram = (wsClient: Ws.Client.t, _: Fetch.Request.t): unit => {
  let sendChannelsIo = (wsClient: Ws.Client.t, redisClient: Redis.Client.t) =>
    redisClient
    |> getAllChannels
    >>= (
      channels => {
        let data: string =
          Types.ChannelInfoListMessage(channels)
          |> Encoders.encodeMessage
          |> Js.Json.stringify;
        wsClient |> sendMessage(~data);
      }
    );

  let subscribef = (wsClient: Ws.Client.t, redisClient: Redis.Client.t) => {
    let subscriber = redisClient |> Redis_IO.subscribe(~channel="global");
    // | `subscribe((string, int) => unit)
    let onSubscribe = (_: string, _: int): unit =>
      redisClient
      |> sendChannelsIo(wsClient)
      |> IO.unsafeRunAsync(
           fun
           | Ok(_v) => "client successfully joined global channel" |> Js.log
           | Error(_e) =>
             "client failed to subscribe to global channel" |> Js.log,
         );

    // | `unsubscribe((string, int) => unit)
    let onUnsubscribe = (_: string, _: int): unit => {
      "> client unsubscribed" |> Js.log;
    };
    // | `message((string, string) => unit)
    let onMessage = (channel: string, message: string): unit => {
      // broadcast message to subscribed client
      wsClient
      |> Logic.sendMessage(~data=message)
      |> IO.unsafeRunAsync(
           fun
           | Ok(_) => "message successfully sent to client" |> Js.log
           | Error(_) => "message did not send" |> Js.log,
         );
    };
    ();
    subscriber
    >>= (
      _ =>
        [
          `message(onMessage),
          `subscribe(onSubscribe),
          `unsubscribe(onUnsubscribe),
        ]
        |> traverse(event => IO.suspend(() => Redis.Client.on(~event)))
    );
  };

  subscribef(wsClient, createClient({port: 6379}))
  |> IO.unsafeRunAsync(
       fun
       | Ok(_) => "connection was successful" |> Js.log
       | Error(_) => "connection was unsuccessful" |> Js.log,
     );
};

// message handler
let onMessage = (message: string): unit => {
  let decodedMessage: Types.message =
    message |> Js.Json.parseExn |> Decoders.decodeMessage;
  let redisClient = createClient({port: 6379});

  switch (decodedMessage) {
  | CreateChannelRequestMessage(req) =>
    "recieved channel info message, make the channel and broadcast" |> Js.log;
    let newChannel: Types.channelInfo = {
      id: Uuid.v4(),
      displayName: req.displayName,
      hidden: req.hidden,
      password: req.password,
      creationTimestamp: Js.Date.now(),
    };
    redisClient
    |> Logic.createChannel(~channel=newChannel)
    |> IO.unsafeRunAsync(
         fun
         | Ok(_d) => "connection was successful" |> Js.log
         | Error(_e) => "connection was unsuccessful" |> Js.log,
       );
  | TextMessageMessage(_message) =>
    "recieved text message, persist and broadcast" |> Js.log
  | _ => "ignore, we dont handle those messages on the server" |> Js.log
  };
};

type csPath = (Ws.Server.t, Ws.Client.t);

let alloc =
  IO.suspendWithVoid(() => Ws.Server.make({port: 3000}))
  >>= (
    server =>
      IO.suspendWithVoid(() => Ws.Client.make("ws://localhost:3000"))
      |> IO.map(client => (server, client))
  )
  |> IO.mapError(_ => Redis.NoKey);

let program0 =
  bootstrapProgram
  >>= (_ => alloc)
  >>= (
    tuple => {
      let (server, client) = tuple;
      IO.suspend(_ =>
        server |> Ws.Server.on(~event=`connection_(connectionProgram))
      )
      >>= (
        _ =>
          IO.suspend(_ =>
            client |> Ws.Client.on(~event=`message(m => m |> Js.log))
          )
      );
    }
  );
// |> IO.unsafeRunAsync(
//      fun
//      | Ok(_) => "success" |> Js.log
//      | Error(_) => "an error occurred" |> Js.log,
//    );

let resource = IO.suspendWithVoid(() => Ws.Server.make({port: 3000}));

let program =
  resource
  >>= (
    wsServer =>
      IO.suspendWithVoid(() =>
        wsServer |> Ws.Server.on(~event=`connection_(connectionProgram))
      )
  );

program
|> IO.unsafeRunAsync(
     fun
     | Ok(_) => "success" |> Js.log
     | Error(_) => "an error occurred" |> Js.log,
   );