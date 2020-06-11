open FacelessCore;
open FacelessCore.Types;
open Relude_Globals;
open WsBrowserClient;

type state = {
  channels: list(Types.channelInfo),
  connections: int,
  currentChannel: option(Types.channelInfo),
  pannelHidden: bool,
};

type action =
  | ToggleSidePannel
  | EnterChannel(Types.channelInfo)
  | PopulateChannels(list(Types.channelInfo));

let initialState: state = {
  channels: List.empty,
  currentChannel: None,
  connections: 0,
  pannelHidden: false,
};

let reducer = (state, action) =>
  switch (action) {
  | ToggleSidePannel => {...state, pannelHidden: !state.pannelHidden}
  | PopulateChannels(channels) => {
      ...state,
      channels: state.channels |> List.concat(channels),
    } //unimplemented
  | EnterChannel(channel) => {...state, currentChannel: Some(channel)}
  };

let messageToAction = (message: Types.message) => {
  // Js.log(message);
  switch (message) {
  | ChannelInfoListMessage(m) => PopulateChannels(m)
  | ChannelInfoMessage(m) => ToggleSidePannel // unimplemented
  };
};

[@react.component]
let make = (~channel: option(string), ~websocket: option(WsBrowserClient.t)) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let enterChannel = (channel: Types.channelInfo): unit =>
    dispatch(EnterChannel(channel));

  switch (websocket) {
  | None => "no connection passed in" |> Js.log
  | Some(w) =>
    w
    |> WsBrowserClient.onmessage(~f=(event: WsBrowserClient.messageEvent) => {
         let {data} = event;

         data
         |> Js.Json.parseExn
         |> Decoders.decodeMessage
         |> messageToActionf
         |> dispatch;
       })
  };

  <div className="app-grid">
    <SidePannel channels={state.channels} enterChannel />
    {switch (state.currentChannel) {
     | None => <Spinner />
     | Some(c) => <Channel channel=c />
     }}
  </div>;
};