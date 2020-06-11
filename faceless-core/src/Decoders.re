open Types;
open Json_decode;
open Utils;
exception InvalidPayloadType(string);

let decodeChannelInfo = (json: Js.Json.t): Types.channelInfo => {
  {
    displayName: json |> field("displayName", string),
    id: json |> field("id", string),
    hidden: json |> field("hidden", bool),
    password: json |> field("password", string) |> option_of_string,
    creationTimestamp: json |> field("creationTimestamp", int),
  };
};

let decodeTextMessage = (json: Js.Json.t): Types.textMessage => {
  id: json |> field("id", string),
  authorId: json |> field("authorId", string),
  data: json |> field("data", string),
  creationTimestamp: json |> field("creationTimestamp", int),
};

// redis dict of channel info is a bit of a pain to parse
// we'll use this remporary abstraction to move things along in the mean time
// let decodeRedisChannelInfoListDict = (dict: Dict.t(string)) =>
//   dict
//   |> Dict.values
//   |> List.fromArray
//   |> List.map(s => s |> Json.parseExn |> decodeChannelInfo);

let decodeMessage = (json: Js.Json.t): Types.message =>
  // naughty, don't raise exceptions. learn a nice way to handle exceptions in the decoder
  // maybe turn this into a bifunctor
  switch (json |> field("payloadType", string)) {
  | "channelInfo" =>
    ChannelInfoMessage(json |> field("payload", decodeChannelInfo))
  | "channelInfoList" =>
    ChannelInfoListMessage(
      json |> field("payload", list(decodeChannelInfo)),
    )
  | p => raise(InvalidPayloadType(p))
  };