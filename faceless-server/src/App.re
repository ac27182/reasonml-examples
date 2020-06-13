open FacelessCore;
open Redis;
open Relude_Globals;
// open Js;

module Relude_IO_Infix = {
  open Relude_IO;

  let (>>=) = flatMap;
  let (>>) = andThen;
};

let masterChannel: Types.channelInfo = {
  id: "924b48ac-39e1-46ab-bc23-b303f7c29599",
  displayName: "master",
  hidden: false,
  password: None,
  creationTimestamp: 1.0,
};

let slaveChannel: Types.channelInfo = {
  id: "9c5e0ebd-bae9-4d18-a7f4-0426963e35c9",
  displayName: "slave",
  hidden: false,
  password: None,
  creationTimestamp: 1.0,
};

let onConnection = (webSocketClient, _) => {
  // subscribe to the channel channel
  let key = "channels";
  // let field = masterChannel.id;
  let subscriber = Redis.createClient({port: 6379});
  // let sendCurrentChannels: IO.t(unit, Redis.error) =
  //   subscriber
  //   |> Redis_IO.hgetall(~key)
  //   |> IO.map((dict: Js.Dict.t(string))
  //        // json encoded channel info list
  //        =>
  //          Types.ChannelInfoListMessage(dict |> Logic.dictToChannelInfoList)
  //          |> Encoders.encodeMessage
  //          |> Js.Json.stringify
  //        )
  //   |> IO.flatMap(data => webSocketClient |> Logic.sendIo(data));

  let subscribeToChannelChannel: IO.t(string, Redis.error) =
    subscriber |> Redis_IO.subscribe(~channel="global");

  let program0: IO.t(string, Redis.error) =
    IO.suspend(() => "> client connected" |> Js.log)
    // |> IO.flatMap(_ => sendCurrentChannels)
    |> IO.flatMap(_ => subscribeToChannelChannel);

  program0 |> Utils.runAndLogIo;
  // webSocketClient |> Client.on(~event=`message(onMessage));
  // not ideal
  // needed to help the compiler along with the type information
  // find a way to get rid of Js.Json.parseExn...
  //  |> Array.map((value: string) =>
  //       value |> Js.Json.parseExn |> Types.Decoders.channelInfo
  //     )
};

let bootstrap = () => {
  let field = masterChannel.id;
  let value = masterChannel |> Encoders.encodeChannelInfo |> Js.Json.stringify;
  let key = "channels";

  let redisClient = Redis.createClient({port: 6379});

  // set master channel
  let setIo = redisClient |> Redis_IO.hset(~key, ~field, ~value);

  // get master channel
  let getIo = redisClient |> Redis_IO.hget(~key, ~field);

  // flatmap into a single program
  let program = setIo |> IO.flatMap(_ => getIo);

  // log to console
  "> master channel set" |> Js.log;

  program;
};

let webSocketServer = Ws.Server.make({port: 3000});

// run initialisation program
bootstrap() |> Utils.runAndLogIo;

// engage websocket server
webSocketServer |> Ws.Server.on(~event=`connection_(onConnection));