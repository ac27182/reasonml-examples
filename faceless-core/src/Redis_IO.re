open Relude_Globals;
open Redis;
open Utils;
// open Relude_List.IO;
open Relude.IO;
// hashmap ops
let hset =
    (client: Client.t, ~key: string, ~field: string, ~value: string)
    : IO.t(string, Redis.error) =>
  client |> Redis.hset(~key, ~field, ~value) |> toAsyncIo;

let hget =
    (client: Client.t, ~key: string, ~field: string)
    : IO.t(option(string), Redis.error) =>
  client
  |> Redis.hget(~key, ~field)
  |> toAsyncIo
  |> IO.map(r => r |> Js.Nullable.toOption);

let hgetall = (client: Client.t, ~key: string): IO.t('a, 'e) =>
  client
  |> Redis.hgetall(~key)
  |> toAsyncIo
  |> IO.map(r => r |> Js.Nullable.toOption);

let hdel = (client: Client.t, ~key: string, ~field: string): IO.t('a, 'e) =>
  client |> Redis.hdel(~key, ~field) |> toAsyncIo;

// list ops
let lrange =
    (client: Client.t, ~key: string, ~start: int, ~stop: int)
    : IO.t(option(list(string)), error) =>
  client
  |> Redis.lrange(~key, ~start, ~stop)
  |> toAsyncIo
  |> IO.map(lhs =>
       lhs |> Js.Nullable.toOption |> Option.map(a => a |> List.fromArray)
     );

let llen = (client: Client.t, ~key: string): IO.t(int, error) =>
  client |> Redis.llen(~key) |> toAsyncIo;

let lpush = (client: Client.t, ~key: string, ~item: string): IO.t(int, error) =>
  client |> Redis.lpush(~key, ~item) |> toAsyncIo;

let lpop = (client: Client.t, ~key: string): IO.t(string, error) =>
  client |> Redis.lpop(~key) |> toAsyncIo;

// pubsub opsopen Relude_List.IO;
let subscribe = (client: Client.t, ~channel: string): IO.t('a, 'e) =>
  client |> Redis.subscribe(~channel) |> toAsyncIo;

let unsubscribe = (client: Client.t, ~channel: string): IO.t('a, 'e) =>
  client |> Redis.unsubscribe(~channel) |> toAsyncIo;

let publish =
    (client: Client.t, ~channel: string, ~value: string): IO.t('a, 'e) =>
  client |> Redis.publish(~channel, ~value) |> toAsyncIo;

// client ops
let quit = (client: Client.t): IO.t('a, 'e) =>
  client |> Redis.quit |> toAsyncIo;

// basic redis
let del = (client: Client.t, ~key: string): IO.t('a, 'e) =>
  client |> Redis.del(~key) |> toAsyncIo;

let createClient1 =
    (clientOptions: Redis.clientOptions): IO.t(Redis.Client.t, 'e) =>
  IO.suspend(() => clientOptions |> Redis.createClient);

// let createClient2 =
//     (clientOptions0: clientOptions, clientOptions1: clientOptions) =>
//   [clientOptions0, clientOptions1]
//   |> traverse(c => c |> createClient1)
//   |> IO.map(l =>
//        switch (l) {
//        | [client0, client1] => (client0, client1)
//        | _ => raise(exception_)
//        }
//      );

let createClient2 =
    (clientOptions0: clientOptions, clientOptions1: clientOptions)
    : IO.t((Redis.Client.t, Redis.Client.t), 'e) =>
  createClient1(clientOptions0)
  >>= (
    client0 =>
      createClient1(clientOptions1)
      >>= (client1 => IO.suspend(() => (client0, client1)))
  );