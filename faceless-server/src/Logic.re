open FacelessCore;
open Redis;
open Relude_Globals;
open Relude_IO;
open Relude_List.IO;
open FacelessCore.Types;

type environment = {
  redisPort: option(int),
  webSocketPort: option(int),
  redisHost: option(string),
};

// NB
// int of string can still thow an exception if it can't be converted correctly
// need to make int of string return an option(int) for referential transparency
let parseProcess = (process: Node_process.t): environment => {
  let dict: Js_dict.t(string) = process##env;

  let redisPort: option(int) =
    dict->Js.Dict.get("REDIS_PORT") |> Option.map(s => s |> int_of_string);

  let webSocketPort: option(int) =
    dict->Js.Dict.get("WEBSOCKET_PORT") |> Option.map(s => s |> int_of_string);

  let redisHost: option(string) = dict->Js.Dict.get("REDIS_HOST");

  {redisPort, redisHost, webSocketPort};
};

let defaultHandler = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(_) => "success" |> Js.log
       | Error(_) => "an error occurred" |> Js.log,
     );

// helper function to generate a unix timestamp
let genDate = (): float => Js.Date.now();

// helper function to generatae a creation message
let genCreationMessage = (): string =>
  {
    id: Uuid.v4(),
    authorId: "server",
    data: "channel created",
    creationTimestamp: genDate(),
  }
  |> Encoders.encodeTextMessage
  |> Js.Json.stringify;

// drains the lhs of an IO and prints a message to the console
let drainWithLog = (~message: string, io: IO.t('a, 'e)) =>
  io |> IO.map(_ => "> " ++ message |> Js.log);

// drains the rhs of an io and returns a unit as an error
let drainError = (io: IO.t('a, 'e)) => io |> IO.mapError(_ => ());

// redis dict of channel info is a bit of a pain to parse
// we'll use this remporary abstraction to move things along in the mean time
let dictToChannelInfoList = (dict: Js.Dict.t(string)): list(channelInfo) =>
  dict
  |> Js.Dict.values
  |> List.fromArray
  |> List.map(s => s |> Js.Json.parseExn |> Decoders.decodeChannelInfo);

// gets all persisted channels from redis
let getAllChannels =
    (client: Redis.Client.t): IO.t(list(Types.channelInfo), Redis.error) =>
  client
  |> Redis_IO.hgetall(~key="channels")
  |> IO.map((dict: option(Js.Dict.t(string))) =>
       switch (dict) {
       | None => List.empty
       | Some(d) => d |> dictToChannelInfoList
       }
     );

// gets all persisteted text messages for a particular channel
let getAllTextMessages =
    (client: Redis.Client.t, ~channelId: string)
    : IO.t(list(Types.textMessage), Redis.error) =>
  client
  |> Redis_IO.lrange(~key=channelId, ~start=0, ~stop=-1)
  |> IO.map(opt =>
       switch (opt) {
       | None => List.empty
       | Some(list) =>
         list
         |> List.map(i => i |> Js.Json.parseExn |> Decoders.decodeTextMessage)
       }
     );

// by the nature of redis there is no concreate concept of creation or deletion
// for the purposes of the app we'll 'create' a channel be ensuring we have keyspace assigned to a list
// with at least one message
// conversely we will 'delete' a channel, by voiding a list of all messages / just deleting the key
let createChannel = (client: Redis.Client.t, ~channel: Types.channelInfo) =>
  client
  |> Redis_IO.hget(~key="channels", ~field=channel.id)
  >>= (
    response =>
      switch (response) {
      | None =>
        client
        |> Redis_IO.hset(
             ~key="channels",
             ~field=channel.id,
             ~value=channel |> Encoders.encodeChannelInfo |> Js.Json.stringify,
           )
        >>= (
          _ =>
            client
            |> Redis_IO.lpush(~key=channel.id, ~item=genCreationMessage())
        )
      | Some(_) => IO.throw(Redis.FieldAlreadyExists(channel.id))
      }
  );

// deletes a channel from the channel ledger and redis
let deleteChannel = (client: Redis.Client.t, ~channel: Types.channelInfo) =>
  client
  |> Redis_IO.hdel(~key="channels", ~field=channel.id)
  |> IO.flatMap(_ => client |> Redis_IO.del(~key=channel.id));

// sends a message down the websocket channel to a client
let sendMessage = (client: Ws.Client.t, ~data: string) =>
  IO.suspend(() => client |> Ws.Client.send(~data));

// client availible in the closure if we omit it... strange. thank you ocaml
// not referentially transparent though
// reads through the channel info list
// gets the channel Id's
// deletes all corresponding message lists
let deleteAllChannels =
    (client: Redis.Client.t, channelInfoList: list(Types.channelInfo)) =>
  channelInfoList
  |> List.map((channel: Types.channelInfo) => channel.id)
  |> traverse(id => client |> Redis_IO.del(~key=id));

// deletes the hmap containing the channel info
let deleteChannelLedger = (client: Redis.Client.t, _) =>
  client |> Redis_IO.del(~key="channels");

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
         creationTimestamp: genDate(),
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
  let subscribeEvent = (_: string, _: int): unit =>
    general |> sendChannelInfoList(wsClient) |> defaultHandler;

  let unsubscribeEvent = (_: string, _: int): unit =>
    "> client unsubscribed" |> Js.log;

  let messageEvent = (_: string, message: string): unit => {
    wsClient |> sendMessage(~data=message) |> defaultHandler;
  };

  [
    `message(messageEvent),
    `subscribe(subscribeEvent),
    `unsubscribe(unsubscribeEvent),
  ]
  |> traverse(event =>
       IO.suspend(() => subscriber |> Redis.Client.on(~event))
     )
  >>= (_ => subscriber |> Redis_IO.subscribe(~channel="global"));
};

// message handler
let websocketMessageHandler =
    (channelId: string, message: string, ~redisClient: Redis.Client.t): unit => {
  let decodedMessage = message |> Js.Json.parseExn |> Decoders.decodeMessage;
  // let redisClient = createClient(redisConfig);

  switch (decodedMessage) {
  | ChannelInfoMessage(m0) =>
    redisClient
    |> createChannel(~channel=m0)
    >>= (
      _ => redisClient |> Redis_IO.publish(~channel="global", ~value=message)
    )
    |> defaultHandler

  | TextMessageMessage(m1) =>
    redisClient
    |> Redis_IO.lpush(
         ~key=channelId,
         ~item=m1 |> Encoders.encodeTextMessage |> Js.Json.stringify,
       )
    >>= (
      _ => redisClient |> Redis_IO.publish(~channel=channelId, ~value=message)
    )
    |> defaultHandler
  | _ => "ignore, we dont consume those messages on the server" |> Js.log
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
    |> defaultHandler;

  // need to use the channel
  let redisMessageHandler = (channel: string, message: string): unit => {
    wsClient |> sendMessage(~data=message) |> defaultHandler;
  };

  let subscribeEvent = `subscribe(subscribeHandler);

  let messageEvent = `message(redisMessageHandler);

  IO.suspend(() => subscriber |> Redis.Client.on(~event=subscribeEvent))
  >>= (s => IO.suspend(() => s |> Redis.Client.on(~event=messageEvent)))
  >>= (s => s |> Redis_IO.subscribe(~channel=channelId));
};

let wsServerConnectionHandler =
    (
      wsClient: Ws.Client.t,
      req: Fetch.Request.t,
      ~redisConfig: Redis.clientOptions,
    ) => {
  switch (req |> Fetch.Request.url |> Utils.pathParser) {
  | GlobalChannel =>
    let event =
      `message(
        websocketMessageHandler(
          "",
          ~redisClient=Redis.createClient(redisConfig),
        ),
      ); // need to make the channel ID optional
    let attachEvent = IO.suspend(() => wsClient |> Ws.Client.on(~event));

    attachEvent
    >>= (
      wsClient =>
        subscribeToGlobal(
          wsClient,
          Redis.createClient(redisConfig),
          Redis.createClient(redisConfig),
        )
    )
    |> defaultHandler;
  | MessageChannel(channelId) =>
    let event =
      `message(
        websocketMessageHandler(
          channelId,
          ~redisClient=Redis.createClient(redisConfig),
        ),
      );
    let attachEvent = IO.suspend(() => wsClient |> Ws.Client.on(~event));
    attachEvent
    >>= (
      wsClient =>
        subscribeToChannel(
          wsClient,
          Redis.createClient(redisConfig),
          Redis.createClient(redisConfig),
          channelId,
        )
    )
    |> defaultHandler;
  };
};

let connectionProgram =
    (
      wsClient: Ws.Client.t,
      f: Fetch.Request.t,
      redisConfig: Redis.clientOptions,
    )
    : unit => {
  f |> Fetch.Request.url |> Js.log;

  subscribeToGlobal(
    wsClient,
    createClient(redisConfig),
    createClient(redisConfig),
  )
  |> defaultHandler;
};

// makes websocket server
let makeWsServer = (serverOptions: Ws.Server.serverOptions) =>
  IO.suspendWithVoid(() => Ws.Server.make(serverOptions))
  |> IO.mapError(_ => ());