module Client = {
  type t;

  [@bs.new] [@bs.module] external make: string => t = "ws";

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

  // (very) incomplete list of server options
  type serverOptions = {port: int};

  [@bs.new] [@bs.module "ws"] external make: serverOptions => t = "Server";

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