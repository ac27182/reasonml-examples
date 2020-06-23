open Types;
open Json_decode;
open Utils;
exception InvalidPayloadType(string);

let decodeTimestampAsFloat = (json: Js.Json.t): float =>
  json |> field("creationTimestamp", string) |> float_of_string;

let decodeChannelInfo = (json: Js.Json.t): Types.channelInfo => {
  {
    displayName: json |> field("displayName", string),
    id: json |> field("id", string),
    hidden: json |> field("hidden", bool),
    password: json |> field("password", string) |> option_of_string,
    creationTimestamp: json |> decodeTimestampAsFloat,
  };
};

let decodeTextMessage = (json: Js.Json.t): Types.textMessage => {
  id: json |> field("id", string),
  authorId: json |> field("authorId", string),
  data: json |> field("data", string),
  creationTimestamp: json |> decodeTimestampAsFloat,
  authorName: json |> field("authorName", string),
};

let decodeMessage = (json: Js.Json.t): Types.message => {
  json |> Js.log;
  switch (json |> field("payloadType", string)) {
  | "channelInfo" =>
    ChannelInfoMessage(json |> field("payload", decodeChannelInfo))

  | "channelInfoList" =>
    ChannelInfoListMessage(
      json |> field("payload", list(decodeChannelInfo)),
    )

  | "textMessage" =>
    TextMessageMessage(json |> field("payload", decodeTextMessage))

  | "textMesageList" =>
    TextMessageListMessage(
      json |> field("payload", list(decodeTextMessage)),
    )
  | p => raise(InvalidPayloadType(p))
  };
};