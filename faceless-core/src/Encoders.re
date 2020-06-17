open Types;
open Json_encode;
open Utils;

let encodeTimestampAsString = (ts: float): Js.Json.t =>
  ts |> Js.Float.toString |> string;

let encodeChannelInfo = (channelInfo: Types.channelInfo): Js.Json.t => {
  let {displayName, id, password, hidden, creationTimestamp} = channelInfo;

  [
    ("displayName", displayName |> string),
    ("id", id |> string),
    ("hidden", hidden |> bool),
    ("password", password |> string_of_option |> string),
    ("creationTimestamp", creationTimestamp |> encodeTimestampAsString),
  ]
  |> object_;
};

let encodeTextMessage = (textMessage: Types.textMessage): Js.Json.t => {
  let {id, authorId, data, creationTimestamp} = textMessage;

  [
    ("id", id |> string),
    ("authorId", authorId |> string),
    ("data", data |> string),
    ("creationTimestamp", creationTimestamp |> encodeTimestampAsString),
  ]
  |> object_;
};

let encodeCreateChannelRequest =
    (createChannelRequest: Types.createChannelRequest): Js.Json.t => {
  let {displayName, password, hidden} = createChannelRequest;

  [
    ("displayName", displayName |> string),
    ("hidden", hidden |> bool),
    ("password", password |> string_of_option |> string),
  ]
  |> object_;
};

// helper function for encodeMessage
let derivePayloadType = (message: Types.message) =>
  switch (message) {
  | ChannelInfoMessage(_) => "channelInfo" |> string
  | ChannelInfoListMessage(_) => "channelInfoList" |> string
  | TextMessageMessage(_) => "textMessage" |> string
  | TextMessageListMessage(_) => "textMesageList" |> string
  };

// helper function for encodeMessage
let derivePayload = (message: Types.message): Js.Json.t =>
  switch (message) {
  | ChannelInfoMessage(m) => m |> encodeChannelInfo
  | ChannelInfoListMessage(m) => m |> list(encodeChannelInfo)
  | TextMessageMessage(m) => m |> encodeTextMessage
  | TextMessageListMessage(m) => m |> list(encodeTextMessage)
  };

let encodeMessage = (message: Types.message): Js.Json.t =>
  [
    ("payloadType", message |> derivePayloadType),
    ("payload", message |> derivePayload),
  ]
  |> object_;