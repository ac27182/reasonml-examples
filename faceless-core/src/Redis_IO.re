open Relude_Globals;
open Redis;
open Utils;

// hashmap ops
let hset =
    (client: Client.t, ~key: string, ~field: string, ~value: string)
    : IO.t(string, Redis.error) =>
  client |> Redis.hset(~key, ~field, ~value) |> toAsync;

let hget =
    (client: Client.t, ~key: string, ~field: string)
    : IO.t(string, Redis.error) =>
  client |> Redis.hget(~key, ~field) |> toAsync;

let hgetall = (client: Client.t, ~key: string): IO.t('a, 'e) =>
  client |> Redis.hgetall(~key) |> toAsync;

// pubsub ops
let subscribe = (client: Client.t, ~channel: string): IO.t('a, 'e) =>
  client |> Redis.subscribe(~channel) |> toAsync;

let unsubscribe = (client: Client.t, ~channel: string): IO.t('a, 'e) =>
  client |> Redis.unsubscribe(~channel) |> toAsync;

let publish =
    (client: Client.t, ~channel: string, ~value: string): IO.t('a, 'e) =>
  client |> Redis.publish(~channel, ~value) |> toAsync;

// client ops
let quit = (client: Client.t): IO.t('a, 'e) =>
  client |> Redis.quit |> toAsync;