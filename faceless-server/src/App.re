// open WebSocket;
open Types;
open Redis;
open Relude_IO;

let masterChannel: Types.channelInfo = {
  id: "924b48ac-39e1-46ab-bc23-b303f7c29599",
  displayName: "master",
  hidden: false,
  password: None,
  creationTimestamp: 1,
};

let con = (webSocketClient, request) => {
  // subscribe to channel channel

  let subscriber = Redis.createClient({port: 6379});
  // subscriber |> Redis.subscribe(~channel="global");

  let key = "channels";
  let field = masterChannel.id;
  ();
  // subscriber |> Redis.hget(~key, ~field, ~cb);
};

let bootstrap = () => {
  let field = masterChannel.id;
  let value = masterChannel |> Types.channelInfoEncoder |> Json.stringify;
  let key = "channels";

  let redisClient = Redis.createClient({port: 6379});

  // set master channel
  let setIo = redisClient |> Redis_IO.hset(~key, ~field, ~value);

  // get master channel
  let getIo = redisClient |> Redis_IO.hget(~key, ~field);

  // flatmap into a single program
  let program = setIo >>= (_ => getIo);

  program;
};

let webSocketServer = WebSocket.Server.wss({port: 3000});

// run initialisation program
bootstrap() |> Utils.ioRun;

// engage websocket server
webSocketServer |> WebSocket.Server.on(~event=`connection_(con));