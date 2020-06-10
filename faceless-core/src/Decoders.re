open Types;
open Json.Decode;

exception InvalidPayloadType(string);

let decodeChannelInfo = (json: Js.Json.t): Types.channelInfo => {
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

let decodeChannelInfoDict =
    (json: Js.Json.t): Js.Dict.t(FacelessCore.Types.channelInfo) =>
  json |> dict(decodeChannelInfo);

let decodeMessage = (json: Js.Json.t): Types.message => {
  let payloadType: string = json |> field("payloadType", string);

  // naughty, don't raise exceptions. learn a nice way to handle exceptions in the decoder
  switch (payloadType) {
  | "channelInfo" =>
    ChannelInfoMessage(json |> field("payload", decodeChannelInfo))
  | "channelInfoList" =>
    ChannelInfoListMessage(
      json |> field("payload", list(decodeChannelInfo)),
    )
  | p => raise(InvalidPayloadType(p))
  };
};