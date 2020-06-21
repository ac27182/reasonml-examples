open FacelessCore;
open FacelessCore.Ws;
open FacelessCore.Ws_IO;
open Redis;
open Relude_Globals;
open Relude_IO;
open Relude_List.IO;

open Logic;

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
  createClient(redisConfig)
  |> bootstrapProgram
  >>= (_ => Ws_IO.makeWsServer(webSocketConfig))
  // make redis client: subscriber
  // make redis client: general
  >>= (
    wsServer => {
      let connectionEvent =
        wsServerConnectionHandler(redisConfig)->`connection_;

      IO.suspend(() => wsServer)
      >>= (
        server =>
          IO.suspend(() => server |> Server.on(~event=connectionEvent))
      );
    }
  );

// handler funtion which runs the program at the end of the world
let programHandler = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(v) => v |> Js.log
       | Error(e) => {
           "> an error occurred" |> Js.log;
           e |> Js.log;
         },
     );

program |> programHandler;