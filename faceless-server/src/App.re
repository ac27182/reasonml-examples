open FacelessCore;
open FacelessCore.Ws;
open Redis;
open Relude_Globals;
open Relude_IO;

open Logic;
open Types;
open Utils;

let environment: environment = Node_process.process |> parseProcess;

let redisConfig: Redis.clientOptions = {
  port: environment.redisPort,
  host: environment.redisHost,
};

let webSocketConfig: Ws.Server.serverOptions = {
  port: environment.webSocketPort,
};

// program to run at the end of the world
let program =
    (
      redisConfig: Redis.clientOptions,
      webSocketConfig: Ws.Server.serverOptions,
    ) =>
  redisConfig
  |> Redis_IO.createClient1
  >>= (client => client |> bootstrapProgram)
  >>= (_ => Ws_IO.makeWsServer(webSocketConfig))
  >>= (
    wsServer => {
      let event = wsServerConnectionHandler(redisConfig)->`connection_;

      IO.suspend(() => wsServer)
      >>= (server => IO.suspend(() => server |> Server.on(~event)));
    }
  );

// handler funtion which runs the program at the end of the world
let programHandler = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(_) => "> program operational" |> Js.log
       | Error(e) => {
           "> an error occurred" |> Js.log;
           e |> Js.log;
         },
     );

program(redisConfig, webSocketConfig) |> programHandler;