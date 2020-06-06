module Client = {
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

module Server = {
  type t;
  // example of instansiating a new class nested in a module
  [@bs.new] [@bs.module "ws"]
  external wss: Client.serverOptions => t = "Server";

  [@bs.send]
  external on:
    (
      t,
      [@bs.string] [
        | [@bs.as "connection"] `connection_(
            (Client.t, Fetch.Request.t) => unit,
          )
      ]
    ) =>
    t =
    "on";
};