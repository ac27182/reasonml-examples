open Ws;
open Relude_Globals;

let makeWsClient = (url: string): IO.t(Client.t, 'e) =>
  IO.suspend(() => Client.make(url));

let makeWsServer = (serverOptions: Server.serverOptions): IO.t(Server.t, 'e) =>
  IO.suspend(() => Server.make(serverOptions));

let send = (client: Ws.Client.t, ~data: string) =>
  IO.suspend(() => client |> Ws.Client.send(~data));