open Types;
open Json.Encode;

let encodeChannelInfo = (channelInfo: Types.channelInfo): Js.Json.t => {
  let {displayName, id, password, hidden, creationTimestamp} = channelInfo;

  let sw = (o: option(string)) =>
    switch (o) {
    | None => ""
    | Some(p) => p
    };

  [
    ("displayName", displayName |> string),
    ("id", id |> string),
    ("hidden", hidden |> bool),
    ("password", password |> sw |> string),
    ("creationTimestamp", creationTimestamp |> int),
  ]
  |> object_;
};

let encodeMessage = (message: Types.message): Js.Json.t => {
  let payloadType =
    switch (message) {
    | ChannelInfoMessage(_) => "channelInfo" |> string
    | ChannelInfoListMessage(_) => "channelInfoList" |> string
    };

  let payload =
    switch (message) {
    | ChannelInfoListMessage(m) => m |> list(encodeChannelInfo)
    | ChannelInfoMessage(m) => m |> encodeChannelInfo
    };

  [("payloadType", payloadType), ("payload", payload)] |> object_;
};