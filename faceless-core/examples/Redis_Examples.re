open Redis;

let client = createClient({port: 6379, host: "redis"});

let key = "channel0";

let callback = (_, data) => data |> Js.log;

// get all items from list
// client->lrange(~key, ~start=0, ~stop=-1, callback);

// push item to list
// client->lpush(~key, ~item="value", callback);

// remove item from list
// client->lpop(~key, callback);

open Relude_Globals;

// get key

let channel: Types.channelInfo = {
  id: "xxx",
  displayName: "global",
  hidden: false,
  password: None,
  creationTimestamp: 1.0,
};

// client
// |> Redis_IO.hget(~key="global", ~field="xxx")
// |> IO.flatMap(response =>
//      switch (response) {
//      | None =>
//        client
//        |> Redis_IO.hset(
//             ~key="global",
//             ~field="xxx",
//             ~value=channel |> Encoders.encodeChannelInfo |> Json.stringify,
//           )
//      | Some(v) =>
//        v |> Js.log;
//        IO.throw(FieldAlreadyExists("xxx")); // ordinarily availible in the closure
//      }
//    )
// |> IO.unsafeRunAsync(
//      fun
//      | Ok(v) => v |> Js.log
//      | Error(e) =>
//        switch (e) {
//        | Redis.FieldAlreadyExists(_) => "key already exists" |> Js.log
//        | _ => "some other error..." |> Js.log
//        },
//    );

client
|> Redis_IO.hget(~key="global", ~field="xxx")
|> IO.flatMap(_ =>
     client
     |> Redis_IO.hset(
          ~key="global",
          ~field=channel.id,
          ~value=channel |> Encoders.encodeChannelInfo |> Js.Json.stringify,
        )
   )
// |> IO.flatMap(_ => client |> Redis_IO.del(~key="global"))
// |> IO.flatMap(_ => client |> Redis_IO.hdel(~key="global", ~field="xxx"))
|> IO.flatMap(_ => client |> Redis_IO.hget(~key="global", ~field="xxx"))
|> Utils.runAndLogIo;