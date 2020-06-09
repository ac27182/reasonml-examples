module Client = {
  type t;

  // incomplete list of server options
  type serverOptions = {port: int};

  type messageEvent = {data: string};

  [@bs.new] [@bs.module] external wsc: string => t = "ws";

  [@bs.send]
  external on:
    (
      t,
      ~event: [@bs.string] [
                | `message(string => unit)
                | [@bs.as "open"] `open_(unit => unit)
              ]
    ) =>
    t =
    "on";

  [@bs.send] external send: (t, ~data: string) => unit = "send";
};

module Server = {
  type t;

  [@bs.new] [@bs.module "ws"]
  external wss: Client.serverOptions => t = "Server";

  [@bs.send]
  external on:
    (
      t,
      ~event: [@bs.string] [
                | [@bs.as "connection"] `connection_(
                    (Client.t, Fetch.Request.t) => unit,
                  )
              ]
    ) =>
    t =
    "on";
};