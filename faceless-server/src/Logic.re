open FacelessCore;
open Relude_Globals;
// open Ws;

// naughty and impure, but pragmatic and harmless
// helper function to wrap around bs-uuid

// helper finction to get a unix timestamp
let dateGen = (): float => Js.Date.now();

// redis dict of channel info is a bit of a pain to parse
// we'll use this remporary abstraction to move things along in the mean time
let dictToChannelInfoList = (dict: Js.Dict.t(string)) =>
  dict
  |> Js.Dict.values
  |> List.fromArray
  |> List.map(s => s |> Js.Json.parseExn |> Decoders.decodeChannelInfo);

// gets all persisted channels from redis
let getAllChannels =
    (client: Redis.Client.t): IO.t(list(Types.channelInfo), Redis.error) =>
  client
  |> Redis_IO.hgetall(~key="channels")
  |> IO.map((dict: option(Js.Dict.t(string))) =>
       switch (dict) {
       | None => List.empty
       | Some(d) => d |> dictToChannelInfoList
       }
     );

// gets all persisteted text messages for a particular channel
let getAllTextMessages =
    (client: Redis.Client.t, ~channelId: string)
    : IO.t(list(Types.textMessage), Redis.error) =>
  client
  |> Redis_IO.lrange(~key=channelId, ~start=0, ~stop=-1)
  |> IO.map(opt =>
       switch (opt) {
       | None => List.empty
       | Some(l) =>
         l
         |> List.map(i => i |> Js.Json.parseExn |> Decoders.decodeTextMessage)
       }
     );

// by the nature of redis there is no concept of creation or deletion
// for the purposes of the app we'll 'create' a channel be ensuring we have list
// with at least one message
// conversely we will 'delete' a channel, by voiding a list of all messages / just deleting the key

let createChannel = (client: Redis.Client.t, ~channel: Types.channelInfo) =>
  client
  |> Redis_IO.hget(~key="channels", ~field=channel.id)
  |> IO.flatMap(response =>
       switch (response) {
       | None =>
         client
         |> Redis_IO.hset(
              ~key="channels",
              ~field=channel.id,
              ~value=
                channel |> Encoders.encodeChannelInfo |> Js.Json.stringify,
            )
         |> IO.flatMap(_ => {
              let creationMessage: Types.textMessage = {
                id: Uuid.v4(),
                authorId: "server",
                data: ">channel created<",
                creationTimestamp: dateGen(),
              };

              let encodedCreationMessage: string =
                creationMessage
                |> Encoders.encodeTextMessage
                |> Js.Json.stringify;

              client
              |> Redis_IO.lpush(~key=channel.id, ~item=encodedCreationMessage);
            })
       | Some(_) => IO.throw(Redis.FieldAlreadyExists(channel.id))
       }
     );

// removes the respective channel from channels hashmap
// deletes the keyspace containing any messages corresponding the the messageId

let deleteChannel = (client: Redis.Client.t, ~channel: Types.channelInfo) =>
  client
  |> Redis_IO.hdel(~key="channels", ~field=channel.id)
  |> IO.flatMap(_ => client |> Redis_IO.del(~key=channel.id));

let sendIo = (data: string, client: Ws.Client.t) =>
  IO.suspend(() => client |> Ws.Client.send(~data));

// small integration
open Redis;
let client = Redis.createClient({port: 6379});
open Types;
let globalChannel: Types.channelInfo = {
  id: Uuid.v4(),
  hidden: false,
  password: None,
  displayName: "global",
  creationTimestamp: dateGen(),
};

let newChannel: Types.channelInfo = {
  id: Uuid.v4(),
  hidden: false,
  password: None,
  displayName: "new-channel",
  creationTimestamp: dateGen(),
};

// client |> Redis_IO.hdel(~key="global", ~field="xxx") |> Utils.runAndLogIo;

// client |> Redis_IO.hgetall(~key="global") |> Utils.runAndLogIo;
// client |> getAllChannels |> Utils.runAndLogIo;

// client
// |> createChannel(~channel=globalChannel)
// |> IO.flatMap(_ => client |> getAllChannels |> IO.map(c => c |> Js.log))
// |> IO.flatMap(_ =>
//      client
//      |> getAllTextMessages(~channelId=globalChannel.id)
//      |> IO.map(c => c |> Js.log)
//    )
// |> Utils.runAndLogIo;
let x = "";