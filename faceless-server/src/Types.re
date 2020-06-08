type textMessage = {
  id: string,
  clientId: string,
  creationTimestamp: int,
  data: string,
};

type clientInfo = {
  id: string,
  connectedChannels: list(string),
};

type client = {
  clientInfo,
  connection: WebSocket.Client.t,
};

type channelInfo = {
  displayName: string,
  id: string,
  hidden: bool,
  password: option(string),
  creationTimestamp: int,
};

type message =
  | ChannelInfoMessage(channelInfo)
  | ChannelInfoListMessage(list(channelInfo));

module Encoders = {
  open Json_encode;

  // channel info
  let encodeChannelInfo = (channelInfo: channelInfo) => {
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

  // message
  let encodeMessage = (message: message) => {
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
};
module Decoders = {
  open Json_decode;

  let decodeChannelInfo = (json: Js.Json.t) => {
    let sw = (s: string) =>
      switch (s) {
      | "" => None
      | v => Some(v)
      };

    {
      displayName: json |> field("displayName", string),
      id: json |> field("id", string),
      hidden: json |> field("hidden", bool),
      password: json |> field("password", string) |> sw,
      creationTimestamp: json |> field("creationTimestamp", int),
    };
  };

  let channelInfoDict = (json: Js.Json.t) => json |> dict(decodeChannelInfo);
};

type channel = {channelInfo};

type state = {
  channelPool: list(channel),
  clientPool: list(client),
};