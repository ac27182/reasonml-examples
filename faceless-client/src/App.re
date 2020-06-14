[%bs.raw {|require("./css/app.css")|}];
open FacelessCore;
open FacelessCore.Types;
open FacelessCore.Webapi;
open Relude_Globals;
open ClientLogic;

type action = ClientLogic.appAction;

type state = {
  channels: list(channelInfo),
  connections: int,
  currentChannel: option(channelInfo),
  subscribedChannels: list(channelInfo),
  navigationPannelHidden: bool,
};

// state to initialise the application
let initialState = {
  channels: List.empty,
  subscribedChannels: List.empty,
  currentChannel: None,
  connections: Int.zero,
  navigationPannelHidden: false,
};

// component reducer
let reducer = (state: state, action: action) =>
  switch (action) {
  | EnterChannel(channelInfo) => {
      ...state,
      currentChannel: Some(channelInfo),
    }
  | LeaveChannnel => {...state, currentChannel: None}
  | PopulateChannels(channelInfoList) => {
      ...state,
      channels: channelInfoList,
    }
  | _ => state
  };

let messageToAction = (message: Types.message): action =>
  switch (message) {
  | ChannelInfoListMessage(m) => PopulateChannels(m)
  | _ => DoNothing
  };

[@react.component]
let make = (~webSocketClient: option(WebSocket.t), ~userId: string) => {
  let (state, dispatch: action => unit) =
    React.useReducer(reducer, initialState);

  let {
    channels,
    subscribedChannels,
    currentChannel,
    connections,
    navigationPannelHidden,
  } = state;

  let messageHandler = (messageEvent: WebSocket.messageEvent): unit => {
    messageEvent.data
    |> Js.Json.parseExn
    |> Decoders.decodeMessage
    |> messageToAction
    |> dispatch;
  };

  switch (webSocketClient) {
  | None => "no websocket passed in..." |> Js.log
  | Some(w) => w |> WebSocket.onmessage(~messageHandler)
  };

  "userId: " ++ userId |> Js.log;

  <div className="app-container">
    <Header />
    <Header />
    <ContextProvider value=dispatch>
      <NavigationPannel navigationPannelHidden channels />
    </ContextProvider>
    {switch (currentChannel) {
     | None => <Spinner />
     | Some(currentChannel) =>
       <ContextProvider value=dispatch>
         <ChannelPannel currentChannel />
       </ContextProvider>
     }}
  </div>;
};