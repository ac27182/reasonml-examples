open Relude_Globals;
open Redis;
open Utils;

// hashmap ops
let hset =
    (client: Client.t, ~key: string, ~field: string, ~value: string)
    : IO.t(string, Redis.error) =>
  client |> Redis.hset(~key, ~field, ~value) |> toAsyncIo;

let hget =
    (client: Client.t, ~key: string, ~field: string)
    : IO.t(string, Redis.error) =>
  client |> Redis.hget(~key, ~field) |> toAsyncIo;

let hgetall = (client: Client.t, ~key: string): IO.t('a, 'e) =>
  client |> Redis.hgetall(~key) |> toAsyncIo;

// pubsub ops
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