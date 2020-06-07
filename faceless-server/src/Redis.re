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
external hgetall: (Client.t, ~key: string, callback(string)) => unit =
  "hgetall";

// open Relude_Globals;
// module Redis_IO = {
//   open Client;
//   let hget = (t, ~key: string, ~field: string) => t |> Redis.hget;
// };

// let subscriber = {port: 6379} |> createClient;
// let publisher = {port: 6379} |> createClient;

// let x = subscriber |> hget(~key="x", ~field="x");

// let toAsync =
//     (partialFunction: callback(string) => 'x): Relude.Globals.IO.t('a, 'e) => {
//   IO.async(onDone => {
//     (
//       (e, v) =>
//         switch (e |> Js.Null.toOption) {
//         | Some(e) => Error(e) |> onDone
//         | None => Ok(v) |> onDone
//         }
//     )
//     |> partialFunction
//   });
// };

// let y = x |> toAsync;

// let l0 = (channel, nSubscribers) => Js.log2(channel, nSubscribers);
// let l1 = (channel, nSubscribers) => Js.log2(channel, nSubscribers);
// let l2 = (channel, message) => Js.log2(channel, message);

// subscriber |> Client.on(~event=`subscribe(l0));
// subscriber |> Client.on(~event=`unsubscribe(l1));
// subscriber |> Client.on(~event=`message(l2));

// subscriber |> subscribe(~channel="channel0");
// publisher |> publish(~channel="channel0", ~message="hello world");
// subscriber |> unsubscribe(~channel="channel0");

// subscriber
// |> hset(
//      ~key="channels",
//      ~field="channel0",
//      ~value="{this is de channel information}",
//    );

// subscriber |> hget(~key="channels", ~field="channel0", (_, v) => v |> Js.log);

// subscriber |> hgetall(~key="channels", ~cb=(_, v) => v |> Js.log);

// publisher |> quit;
// subscriber |> quit;
let x = "";