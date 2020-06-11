open FacelessCore;
open Relude_Globals;
open Ws;

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
  |> IO.map((dict: Js.Dict.t(string)) => dict |> dictToChannelInfoList);

// gets all persisteted text messages for a particular channel
// let getAllTextMessages =
//     (client: Redis.Client.t, ~channelId: string)
//     : IO.t(list(Types.textMessage), Redis.error) => {
//   client |> Redis_IO
// };

let sendIo = (data: string, client: Ws.Client.t) =>
  IO.suspend(() => client |> Ws.Client.send(~data));