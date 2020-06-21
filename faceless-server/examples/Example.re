open FacelessCore;
open Relude_Globals;
open Relude_IO;
open Logic;
type csPath = (Ws.Server.t, Ws.Client.t);
let alloc =
  IO.suspendWithVoid(() => Ws.Server.make({port: 3000}))
  >>= (
    server =>
      IO.suspendWithVoid(() => Ws.Client.make("ws://localhost:3000"))
      |> IO.map(client => (server, client))
  );