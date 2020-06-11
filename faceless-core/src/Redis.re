type error;
type callback('a) = (Js.Null.t(error), 'a) => unit;
type clientOptions = {port: int};

module Client = {
  type t;
  // client listener...
  [@bs.send]
  external on:
    (
      t,
      ~event: [@bs.string] [
                | `subscribe((string, int) => unit)
                | `unsubscribe((string, int) => unit)
                | `message((string, string) => unit)
              ]
    ) =>
    t =
    "on";
};

[@bs.module "redis"]
external createClient: clientOptions => Client.t = "createClient";

[@bs.send] external quit: (Client.t, callback(string)) => unit = "quit";

// pubsub
[@bs.send]
external subscribe: (Client.t, ~channel: string, callback(string)) => unit =
  "subscribe";

[@bs.send]
external unsubscribe: (Client.t, ~channel: string, callback(string)) => unit =
  "unsubscribe";

[@bs.send]
external publish:
  (Client.t, ~channel: string, ~value: string, callback(string)) => unit =
  "publish";

[@bs.send]
external hset:
  (
    Client.t,
    ~key: string,
    ~field: string,
    ~value: string,
    callback(string)
  ) =>
  unit =
  "hset";

[@bs.send]
external hget:
  (Client.t, ~key: string, ~field: string, callback(string)) => unit =
  "hget";

[@bs.send]
external hgetall:
  (Client.t, ~key: string, callback(Js.Dict.t(string))) => unit =
  "hgetall";