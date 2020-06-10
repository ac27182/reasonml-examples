open Relude_Globals;
open FacelessCore;

// redis dict of channel info is a bit of a pain to parse
// we'll use this remporary abstraction to move things along in the mean time
let dictToChannelInfoList = (dict: Js.Dict.t(string)) =>
  dict
  |> Js.Dict.values
  |> List.fromArray
  |> List.map(s => s |> Js.Json.parseExn |> Decoders.decodeChannelInfo);

let sendIo = (data: string, client: WebSocket.Client.t) =>
  IO.suspend(() => client |> WebSocket.Client.send(~data));