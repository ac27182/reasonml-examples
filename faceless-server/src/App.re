open FacelessCore;
open Redis;
open Relude_Globals;
open Relude_IO;
open Logic;

let environment: environment = Node_process.process |> parseProcess;

let redisConfig: Redis.clientOptions = {
  port: environment.redisPort,
  host: environment.redisHost,
};

let websocketConfig: Ws.Server.serverOptions = {
  port: environment.webSocketPort,
};

// program to run at the end of the world
let program =
  createClient(redisConfig)
  |> bootstrapProgram
  >>= (_ => makeWsServer(websocketConfig))
  >>= (
    wsServer => {
      let w =
        wsServer
        |> Ws.Server.on(
             ~event=`connection_(wsServerConnectionHandler(~redisConfig)),
           );

      IO.suspendWithVoid(() => w) |> IO.mapError(_ => ());
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

program |> programHandler;