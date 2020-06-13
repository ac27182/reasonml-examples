type error =
  | NoKey
  | FieldAlreadyExists(string);

type callback('a) = (Js.Null.t(error), 'a) => unit;

type clientOptions = {port: int};

module Client = {
  type t;

  [@bs.send]
  external on:
    (
      t,
      ~event: [@bs.string] [
                | `subscribe((string, int) => unit)
                  // channel, message
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

// hashmap
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
  (
    Client.t,
    ~key: string,
    ~field: string,
    callback(Js.Nullable.t(string))
  ) =>
  unit =
  "hget";

[@bs.send]
external hgetall:
  (Client.t, ~key: string, callback(Js.Nullable.t(Js.Dict.t(string)))) =>
  unit =
  "hgetall";

[@bs.send]
external hdel: (Client.t, ~key: string, ~field: string, callback(int)) => unit =
  "hdel";

[@bs.send]
external lrange:
  (
    Client.t,
    ~key: string,
    ~start: int,
    ~stop: int,
    callback(Js.Nullable.t(Js.Array.t(string)))
  ) =>
  unit =
  "lrange";

[@bs.send]
external llen: (Client.t, ~key: string, callback(int)) => unit = "llen";

// ignoring multiple arguments to reduce complexity.
// to use variadic arguments employ the [@bs.variadic] binding
[@bs.send]
external lpush: (Client.t, ~key: string, ~item: string, callback(int)) => unit =
  "lpush";

[@bs.send]
external lpop: (Client.t, ~key: string, callback(string)) => unit = "lpop";

// kv
[@bs.send]
external del: (Client.t, ~key: string, callback(int)) => unit = "del";