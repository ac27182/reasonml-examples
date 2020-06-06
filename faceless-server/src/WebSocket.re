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
    userId: string,
  };

  type channel = {
    name: string,
    password: string,
    protected: bool,
    hidden: bool,
    creationTime: int,
    clientPool: list(client),
  };

  // type state = {clientPool: list(client)};
  type state = {channels: list(channel)};
};

// -- crude testing of implementation
open WsServer;
open WsClient;
open Relude_Globals;

// terrible function, not referentially transparent it will have to do for now
let parser = (path: string): Types.pathDetails => {
  let l = path |> String.sliceToEnd(1) |> String.splitAsList(~delimiter="/");

  {
    channelName: l |> List.at(0) |> Option.getOrThrow,
    userId: l |> List.at(1) |> Option.getOrThrow,
  };
};

let state: Pervasives.ref(Types.state) = ref({channels: []}: Types.state);

let con =
  `connection_(
    (wsClient, request) => {
      // do naive state operations

      let pd = request |> Fetch.Request.url |> parser;
      Js.log(pd.userId ++ " has connected to channel " ++ pd.channelName);

      let currentState = state^;

      // create a new client
      let newClient: Types.client = {id: "xxx", ws: wsClient};

      let channels: list(Types.channel) = currentState.channels;

      let scrutinee0: option(Types.channel) =
        channels |> List.find((c: Types.channel) => c.name == pd.channelName);

      let newState: Types.state =
        switch (scrutinee0) {
        | Some(channel) =>
          let updatedChannel: Types.channel = {
            ...channel,
            clientPool: channel.clientPool |> List.concat([newClient]),
          };

          let ns: Types.state = {
            channels:
              currentState.channels
              |> List.filterNot((c: Types.channel) =>
                   c.name === channel.name
                 )
              |> List.concat([updatedChannel]),
          };
          ns;
        | None =>
          let newChannel: Types.channel = {
            name: pd.channelName,
            password: "",
            protected: false,
            hidden: false,
            creationTime: 0,
            clientPool: [newClient],
          };
          let ns: Types.state = {
            channels: currentState.channels |> List.concat([newChannel]),
          };
          ns;
        };

      state := newState;

      let w =
        wsClient->WsClient.on(
          `message(
            m => {
              let currentState = state^;
              let channelName = pd.channelName;
              let scrutinee1 =
                currentState.channels
                |> List.find((c: Types.channel) => c.name == channelName);

              switch (scrutinee1) {
              | Some(channel) =>
                channel.clientPool
                |> List.forEach(_ => Js.log2(m, channelName))
              | None => "channel not availible" |> Js.log
              };
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
  IO.suspendWithVoid(_ => wsc("ws://localhost:3000/channel0/client0/"))
  |> IO.map(w => w->on(opn));

let io20 =
  IO.suspendWithVoid(_ => wsc("ws://localhost:3000/channel0/client1"))
  |> IO.map(w => w->on(opn));

let io21 =
  IO.suspendWithVoid(_ => wsc("ws://localhost:3000/channel1/client2"))
  |> IO.map(w => w->on(opn));

// sends a single message after a short delay
let i03 =
  io2
  |> IO.withDelayAfter(100)
  |> IO.flatMap(w => IO.suspendWithVoid(_ => w->send("message1")));

// create 5 copies of a client
let f = (w: WsClient.t): list(WsClient.t) => [w, w, w, w, w];

let testProgram =
  io1
  |> IO.flatMap(_ => io2)
  |> IO.flatMap(_ => io20)
  |> IO.withDelayAfter(100)
  |> IO.flatMap(client0 => {
       client0
       |> f
       |> Relude_List.IO.traverse(clientN =>
            IO.suspendWithVoid(_ => clientN->send("> message"))
          )
     })
  |> IO.flatMap(_ => io21)
  |> IO.withDelayAfter(100)
  |> IO.flatMap(client0 => {
       client0
       |> f
       |> Relude_List.IO.traverse(clientN =>
            IO.suspendWithVoid(_ => clientN->send("> message"))
          )
     });

io1
|> IO.unsafeRunAsync(
     fun
     | Ok(_) => Js.log("websocket operational")
     | Error(_) => Js.log("an error occcurred"),
   );

// end of the world
// testProgram
// |> IO.unsafeRunAsync(
//      fun
//      | Ok(_) => Js.log("program ran successfully")
//      | Error(_) => Js.log("an error occcurred"),
//    );

// 1. organise the clients using id's /
// 2. organise the clients into channels /
// 4. make simple ui to connect to a channel
// 5. make the ui consume messages from the socket
// 3. specify message types for transmission

// stops the refmt destroying all of the comments...
let e = ();