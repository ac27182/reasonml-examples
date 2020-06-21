// example for michael
// type csPath = (Ws.Server.t, Ws.Client.t);
// let alloc =
//   IO.suspendWithVoid(() => Ws.Server.make({port: 3000}))
//   >>= (
//     server =>
//       IO.suspendWithVoid(() => Ws.Client.make("ws://localhost:3000"))
//       |> IO.map(client => (server, client))
//   )
//   |> IO.mapError(_ => Redis.NoKey);
// let program0 =
//   bootstrapProgram
//   >>= (_ => alloc |> IO.mapError(_ => ()))
//   >>= (
//     tuple => {
//       let (server, client) = tuple;
//       IO.suspend(_ =>
//         server |> Ws.Server.on(~event=`connection_(connectionProgram))
//       )
//       >>= (
//         _ =>
//           IO.suspend(_ =>
//             client |> Ws.Client.on(~event=`message(m => m |> Js.log))
//           )
//       );
//     }
/*   )*/