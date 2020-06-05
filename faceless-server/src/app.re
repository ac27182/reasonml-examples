module Types = {
  type user = {
    userId: string,
    client: string,
    creationTime: int,
  };

  type dynamicChannel = {
    name: string,
    password: string,
    protected: bool,
    hidden: bool,
    users: list(user),
    creationTime: int,
  };

  type channelName =
    | Channel0
    | Channel1
    | Channel2
    | Channel3;

  type lockedChannel = {
    name: channelName,
    password: string,
    protected: bool,
    hidden: bool,
    users: list(user),
    creationTime: int,
  };

  type message = {
    messageId: string,
    userId: string,
    references: list(string),
    creationTime: int,
  };

  type state = {channels: list(lockedChannel)};
};

module WebSocket = {
  type t;
  module Server = {
    type t;
  };

  // incomplete list of server options
  type serverOptions = {port: int};
};

// example of instantiating a new class directly from the module
[@bs.new] [@bs.module] external webSocket: string => WebSocket.t = "ws";

// example of instansiating a new class nested in a module
[@bs.new] [@bs.module "ws"]
external webSocketServer: WebSocket.serverOptions => WebSocket.Server.t =
  "Server";

// see bs-ws
[@bs.send]
external on:
  (
    WebSocket.t,
    [@bs.string] [
      | `close((int, string) => unit)
      | `message(string => unit)
      | [@bs.as "open"] `open_(unit => unit)
      | `ping(Node.Buffer.t => unit)
      | `pong(Node.Buffer.t => unit)
      | `error(Dom.errorEvent => unit)
    ]
  ) =>
  WebSocket.t =
  "on";

// let options: WebSocket.serverOptions = {port: 3000};
// web socket server
let wss = webSocketServer({port: 3000});

// websocket client

let openEvent = `open_(() => Js.log("connection opened"));

let ws = webSocket("ws://localhost:3000")->on(openEvent);

open Relude_Globals;
open Pervasives;
open BsBastet.Interface;

let throwJSError: unit => int = [%bs.raw
  {| function() { throw new Error("Error from JS"); } |}
];

// let wsIo = suspend(() => ws);

let x =
  IO.suspend(() => 42)
  |> IO.unsafeRunAsync(
       fun
       | Ok(v) => Js.log(v)
       | Error(_) => Js.log("an error occurred"),
     );

// let wsIo =
//   IO.suspend(() => webSocket("ws://localhost:3000"))
//   |> IO.withDelay(100)
//   |> IO.map(w => w->on(openEvent));

// wsIo
// |> IO.unsafeRunAsync(
//      fun
//      | Ok(v) => Js.log(v)
//      | Error(_) => Js.log("an error occurred"),
//    );
type applicationError =
  | ErrorType1
  | ErrorType2;

let iowss =
  IO.suspend(() => webSocketServer({port: 3000}))
  |> IO.mapError(
       fun
       | Ok(v) => v
       | _ => ErrorType1,
     );

open BsBastet.Interface;

module type TransofrmError = {type t('a);};

module TransofrmError:
  TransofrmError with type t('a) = result('a, applicationError) = {
  type t('a) = result('a, applicationError);
};

// let iows = IO.suspend(() => webSocket("ws://localhost:3000")->on(openEvent));

// let ios = [io0];

// |> IO.mapRight(
//      fun
//      | Ok(x) => Js.log(x)
//      | _ => Js.log("failed"),
//    );

let message: Types.message = {
  messageId: "x",
  creationTime: 0,
  userId: "x",
  references: [],
};