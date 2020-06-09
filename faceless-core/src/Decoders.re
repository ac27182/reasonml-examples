open Types;
open Json.Decode;

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