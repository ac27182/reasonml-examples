open WebSocket;
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
        wsClient->Client.on(
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
  IO.suspendWithVoid(() => Server.wss({port: 3000}))
  |> IO.map(w => w->Server.on(con))
  |> IO.map(drain);

// creates a client connection
// let io2 =
//   IO.suspendWithVoid(_ => Client.wsc("ws://localhost:3000/channel0/client0/"))
//   |> IO.map(w => w->Client.on(opn));

// let io20 =
//   IO.suspendWithVoid(_ => Client.wsc("ws://localhost:3000/channel0/client1"))
//   |> IO.map(w => w->Client.on(opn));

// let io21 =
//   IO.suspendWithVoid(_ => Client.wsc("ws://localhost:3000/channel1/client2"))
//   |> IO.map(w => w->Client.on(opn));

// sends a single message after a short delay
// let i03 =
//   io2
//   |> IO.withDelayAfter(100)
//   |> IO.flatMap(w => IO.suspendWithVoid(_ => w->Client.send("message1")));

// create 5 copies of a client
// let f = (w: Client.t): list(Client.t) => [w, w, w, w, w];

// sample test program
// let testProgram =
//   io1
//   |> IO.flatMap(_ => io2)
//   |> IO.flatMap(_ => io20)
//   |> IO.withDelayAfter(100)
//   |> IO.flatMap(client0 => {
//        client0
//        |> f
//        |> Relude_List.IO.traverse(clientN =>
//             IO.suspendWithVoid(_ => clientN->Client.send("> message"))
//           )
//      })
//   |> IO.flatMap(_ => io21)
//   |> IO.withDelayAfter(100)
//   |> IO.flatMap(client0 => {
//        client0
//        |> f
//        |> Relude_List.IO.traverse(clientN =>
//             IO.suspendWithVoid(_ => clientN->Client.send("> message"))
//           )
//      });

// execution
io1
|> IO.unsafeRunAsync(
     fun
     | Ok(_) => Js.log("websocket operational")
     | Error(_) => Js.log("an error occcurred"),
   );