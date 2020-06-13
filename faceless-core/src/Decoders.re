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
    creationTimestamp:
      json |> field("creationTimestamp", int) |> float_of_int,
  };
};

let decodeTextMessage = (json: Js.Json.t): Types.textMessage => {
  id: json |> field("id", string),
  authorId: json |> field("authorId", string),
  data: json |> field("data", string),
  creationTimestamp: json |> field("creationTimestamp", int) |> float_of_int,
};

let decodeCreateChannelRequest = (json: Js.Json.t): Types.createChannelRequest => {
  displayName: json |> field("displayName", string),
  hidden: json |> field("hidden", bool),
  password: json |> field("password", string) |> option_of_string,
};

let decodeMessage = (json: Js.Json.t): Types.message =>
  // naughty, don't raise exceptions. learn a nice way to handle exceptions in the decoder
  // maybe turn this into a bifunctor
  switch (json |> field("payloadType", string)) {
  | "createChannelRequest" =>
    CreateChannelRequestMessage(
      json |> field("payload", decodeCreateChannelRequest),
    )
  | "channelInfo" =>
    ChannelInfoMessage(json |> field("payload", decodeChannelInfo))
  | "channelInfoList" =>
    ChannelInfoListMessage(
      json |> field("payload", list(decodeChannelInfo)),
    )
  | "textMessage" => TextMessageMessage(json |> decodeTextMessage)
  | "textMesageList" =>
    TextMessageListMessage(json |> list(decodeTextMessage))
  | p => raise(InvalidPayloadType(p))
  };