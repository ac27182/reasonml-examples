open FacelessCore;
open Redis;
open Relude_Globals;
open Relude_IO;
open Relude_List.IO;
open Logic;
open FacelessCore.Types;

// client availible in the closure if we omit it... strange. thank you ocaml
// not referentially transparent though
// reads through the channel infolist
// gets the channel Id's
// deletes all corresponding message lists
let deleteAllChannels =
    (client: Redis.Client.t, channelInfoList: list(Types.channelInfo)) =>
  channelInfoList
  |> List.map((channel: Types.channelInfo) => channel.id)
  |> traverse(id => client |> Redis_IO.del(~key=id));

// deletes the hmap containing with the channel info
let deleteChannelLedger = (client: Redis.Client.t, _) =>
  client |> Redis_IO.del(~key="channels");

// drains the lhs of an IO and prints a message to the console
let drainWithLog = (~message: string, io: IO.t('a, 'e)) =>
  io |> IO.map(_ => "> " ++ message |> Js.log);

// drains the rhs of an io and returns a unit as an error
let drainError = (io: IO.t('a, 'e)) => io |> IO.mapError(_ => ());

// clears redis of any left over channels or messages
let wipeRedis = (client: Redis.Client.t) =>
  getAllChannels(client)
  >>= deleteAllChannels(client)
  |> drainWithLog(~message="all channels deleted")
  >>= deleteChannelLedger(client)
  |> drainWithLog(~message="redis wiped")
  |> drainError;

// creates a global channel and logs out to the console
let createGlobalChannel = (client: Redis.Client.t, _) =>
  client
  |> createChannel(
       ~channel={
         id: Uuid.v4(),
         displayName: "global",
         hidden: false,
         password: None,
         creationTimestamp: Logic.dateGen(),
       },
     )
  |> drainWithLog(~message="global channel created")
  |> drainError;

// bootstraps the server by wiping redis and creating the global channel for consumption
let bootstrapProgram = (client: Redis.Client.t) =>
  wipeRedis(client)
  >>= createGlobalChannel(client)
  >>= (_ => Redis_IO.quit(client) |> drainError)
  |> drainWithLog(~message="program bootstrapped");

// sends the channel info list to the client
let sendChannelInfoList = (wsClient: Ws.Client.t, redisClient: Redis.Client.t) => {
  getAllChannels(redisClient)
  >>= (
    channels =>
      wsClient
      |> sendMessage(
           ~data=
             Types.ChannelInfoListMessage(channels)
             |> Encoders.encodeMessage
             |> Js.Json.stringify,
         )
  );
};

let subscribeToGlobal =
    (
      wsClient: Ws.Client.t,
      subscriber: Redis.Client.t,
      general: Redis.Client.t,
    ) => {
  // create subscribed client and a getting client...

  let onSubscribe = (_: string, _: int): unit =>
    general
    |> sendChannelInfoList(wsClient)
    |> IO.unsafeRunAsync(
         fun
         | Ok(_v) => "client successfully joined global channel" |> Js.log
         | Error(e) => e |> Js.log,
       );

  let onUnsubscribe = (_: string, _: int): unit => {
    "> client unsubscribed" |> Js.log;
  };

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

  [
    `message(onMessage),
    `subscribe(onSubscribe),
    `unsubscribe(onUnsubscribe),
  ]
  |> traverse(event =>
       IO.suspend(() => subscriber |> Redis.Client.on(~event))
     )
  >>= (_ => subscriber |> Redis_IO.subscribe(~channel="global"));
};

let connectionProgram = (wsClient: Ws.Client.t, f: Fetch.Request.t): unit => {
  f |> Fetch.Request.url |> Js.log;

  subscribeToGlobal(
    wsClient,
    createClient({port: 6379}),
    createClient({port: 6379}),
  )
  |> IO.unsafeRunAsync(
       fun
       | Ok(_) => "connection was successful" |> Js.log
       | Error(_) => "connection was unsuccessful" |> Js.log,
     );
};

// message handler
let websocketMessageHandler = (message: string): unit => {
  message |> Js.log;
  message |> Js.Json.parseExn |> Js.log;
  let decodedMessage = message |> Js.Json.parseExn |> Decoders.decodeMessage;
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
  | TextMessageMessage(message) =>
    redisClient
    |> Redis_IO.publish(
         ~channel="global",
         ~value=message |> Encoders.encodeTextMessage |> Js.Json.stringify,
       )
    |> IO.unsafeRunAsync(
         fun
         | Ok(_) => "connection was successful" |> Js.log
         | Error(_) => "connection was unsuccessful" |> Js.log,
       )
  | _ => "ignore, we dont handle those messages on the server" |> Js.log
  };
};

let sendAllTextMessages =
    (messages: list(Types.textMessage), client: Ws.Client.t) => {
  client
  |> sendMessage(
       ~data=
         Types.TextMessageListMessage(messages)
         |> Encoders.encodeMessage
         |> Js.Json.stringify,
     );
};

let subscribeToChannel =
    (
      wsClient: Ws.Client.t,
      subscriber: Redis.Client.t,
      general: Redis.Client.t,
      channelId: string,
    ) => {
  // begin
  let subscribeHandler = (_: string, _: int): unit =>
    general
    |> getAllTextMessages(~channelId)
    >>= (messages => wsClient |> sendAllTextMessages(messages))
    |> IO.unsafeRunAsync(
         fun
         | Ok(_) => "messages sent down channel" |> Js.log
         | Error(_) => "an error occurred" |> Js.log,
       );

  let redisMessageHandler = (channel: string, message: string): unit =>
    wsClient
    |> Logic.sendMessage(~data=message)
    |> IO.unsafeRunAsync(
         fun
         | Ok(_) => "message sent down channel" |> Js.log
         | Error(_) => "an error occurred" |> Js.log,
       );

  let event0 = `subscribe(subscribeHandler);
  let attachEvent0 =
    IO.suspend(() => subscriber |> Redis.Client.on(~event=event0));

  let event1 = `message(redisMessageHandler);
  let attachEvent1 =
    IO.suspend(() => subscriber |> Redis.Client.on(~event=event1));

  // let event1 = Redis

  let subscribeToChannel =
    subscriber |> Redis_IO.subscribe(~channel=channelId);

  attachEvent0 >>= (_ => attachEvent1) >>= (_ => subscribeToChannel);
};

let connectToWebSocket = (wsClient: Ws.Client.t, req: Fetch.Request.t) => {
  switch (req |> Fetch.Request.url |> Utils.pathParser) {
  | GlobalChannel =>
    subscribeToGlobal(
      wsClient,
      Redis.createClient({port: 6379}),
      Redis.createClient({port: 6379}),
    )
    |> IO.unsafeRunAsync(
         fun
         | Ok(_) => "success" |> Js.log
         | Error(_) => "an error occurred" |> Js.log,
       )
  | MessageChannel(channelId) =>
    let event = `message(websocketMessageHandler);
    let attachEvent = IO.suspend(() => wsClient |> Ws.Client.on(~event));

    attachEvent
    >>= (
      wsClient =>
        subscribeToChannel(
          wsClient,
          Redis.createClient({port: 6379}),
          Redis.createClient({port: 6379}),
          channelId,
        )
    )
    |> IO.unsafeRunAsync(
         fun
         | Ok(v) => v |> Js.log
         | Error(_) => "an error occurred" |> Js.log,
       );
  };
};

// makes websocket server
let makeWsServer =
  IO.suspendWithVoid(() => Ws.Server.make({port: 3000}))
  |> IO.mapError(_ => ());

// websocketMessageHandler

let wsServerConnectionHandler = connectToWebSocket;

// program to run at the end of the world
let program =
  createClient({port: 6379})
  |> bootstrapProgram
  >>= (_ => makeWsServer)
  >>= (
    wsServer => {
      let attachEvent0 =
        IO.suspendWithVoid(() =>
          wsServer
          |> Ws.Server.on(~event=`connection_(wsServerConnectionHandler))
        )
        |> IO.mapError(_ => ());

      attachEvent0;
    }
  );

// handler funtion to run the program
let programHandler = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(_) => "success" |> Js.log
       | Error(_) => "an error occurred" |> Js.log,
     );

program |> programHandler;