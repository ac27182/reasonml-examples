module WsClient = {
  //types
  type t;

  // incomplete list of server options
  type serverOptions = {port: int};

  type messageEvent = {data: string};

  // example of instantiating a new class directly from the module
  [@bs.new] [@bs.module] external wsc: string => t = "ws";

  [@bs.send]
  external on:
    (
      t,
      [@bs.string] [
        | `message(string => unit)
        | [@bs.as "open"] `open_(unit => unit)
      ]
    ) =>
    t =
    "on";

  [@bs.send] external send: (t, string) => unit = "send";
};

module WsServer = {
  type t;
  // example of instansiating a new class nested in a module
  [@bs.new] [@bs.module "ws"]
  external wss: WsClient.serverOptions => t = "Server";

  [@bs.send]
  external on:
    (
      t,
      [@bs.string] [
        | [@bs.as "connection"] `connection_(
            (WsClient.t, Fetch.Request.t) => unit,
          )
      ]
    ) =>
    t =
    "on";
};

module Types = {
  type client = {
    id: string,
    ws: WsClient.t,
  };

  type pathDetails = {
    channelName: string,
    pathName: string,
  };

  type state = {clientPool: list(client)};
};

// -- crude testing of implementation
open WsServer;
open WsClient;
open Relude_Globals;

let state: Pervasives.ref(Types.state) = ref({clientPool: []}: Types.state);

let con =
  `connection_(
    (wsClient, request) => {
      // do naive state operations
      Js.log("client has connected");
      // Js.log(state);
      let currentState = state^;
      Js.log2("current state:", currentState);
      let newClient: Types.client = {id: "xxx", ws: wsClient};
      let newState: Types.state = {
        clientPool: currentState.clientPool |> List.concat([newClient]),
      };
      state := newState;
      Js.log2("new state:", newState);

      wsClient->WsClient.on(
        `message(
          m => {
            // Js.log2("> message recived", m);
            // Js.log("...");
            let cs = state^;
            cs.clientPool |> List.forEach(_ => Js.log(m));
            Js.log("\n");
          },
        ),
      );
      ();
    },
  );

let drain = _: unit => ();
let opn = `open_(_ => Js.log("connected"));
let drainio = _ => IO.map(drain);

// creates websocket server
let io1 =
  IO.suspendWithVoid(() => wss({port: 3000}))
  |> IO.map(w => w->WsServer.on(con))
  |> IO.map(drain);

// creates a client connection
let io2 =
  IO.suspendWithVoid(_ => wsc("ws://localhost:3000/channel0/client0"))
  |> IO.map(w => w->on(opn));

let io20 =
  IO.suspendWithVoid(_ => wsc("ws://localhost:3000/channel0/client1"))
  |> IO.map(w => w->on(opn));

// sends a single message after a short delay
let i03 =
  io2
  |> IO.withDelayAfter(100)
  |> IO.flatMap(w => IO.suspendWithVoid(_ => w->send("message1")));

// create 5 copies of a client
let f = (w: WsClient.t): list(WsClient.t) => [w, w, w, w, w];

let program =
  io1
  |> IO.flatMap(_ => io2)
  |> IO.flatMap(_ => io20)
  |> IO.withDelayAfter(100)
  |> IO.flatMap(client0 => {
       client0
       |> f
       |> Relude_List.IO.traverse(clientN =>
            IO.suspendWithVoid(_ => clientN->send("msg"))
          )
     });

// end of the world
program
|> IO.unsafeRunAsync(
     fun
     | Ok(_) => Js.log("program ran successfully")
     | Error(_) => Js.log("an error occcurred"),
   );

// stops the refmt destroying all of the comments...
let e = ();