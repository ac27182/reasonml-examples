[%bs.raw {|require("./css/app.css")|}];
open FacelessCore;
open FacelessCore.Types;
open FacelessCore.Webapi;
open Relude_Globals;
open ClientLogic;

// take the path

type action = ClientLogic.appAction;

type state = {
  // un used
  subscribedChannels: list(channelInfo),
  connections: int,
  // in use
  channels: list(channelInfo),
  textMessages: option(list(textMessage)),
  currentChannel: option(channelInfo),
  navigationPannelHidden: bool,
  wsGlobalClient: option(WebSocket.t),
  wsChannelClient: option(WebSocket.t),
};

// state to initialise the application
let initialState = {
  channels: List.empty,
  subscribedChannels: List.empty,
  connections: Int.zero,
  navigationPannelHidden: false,
  currentChannel: None,
  textMessages: None,
  wsGlobalClient: None,
  wsChannelClient: None,
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
  | PopulateTextMessages(textMessages) => {...state, textMessages}
  | ToggleNavigationPannel => {
      ...state,
      navigationPannelHidden: !state.navigationPannelHidden,
    }
  | AddWsGlobalClient(wsGlobalClient) => {...state, wsGlobalClient}
  | AddWsChannelClient(wsChannelClient) => {...state, wsChannelClient}
  | AppendTextMessage(textMessage) => {
      ...state,
      textMessages:
        state.textMessages |> Option.map(l => l |> List.append(textMessage)),
    }
  | AppendChannelInfo(channelInfo) => {
      ...state,
      channels: state.channels |> List.append(channelInfo),
    }
  | _ => state
  };

let messageToAction = (message: Types.message): action =>
  switch (message) {
  | ChannelInfoListMessage(m) => PopulateChannels(m)
  | TextMessageListMessage(m) => PopulateTextMessages(Some(m))
  | TextMessageMessage(m) => AppendTextMessage(m)
  | ChannelInfoMessage(m) => AppendChannelInfo(m)
  | _ => DoNothing
  };

[@react.component]
let make = (~userId: string) => {
  let (state, dispatch: action => unit) =
    React.useReducer(reducer, initialState);
  let url = "ws://localhost:3000/global";

  let {
    connections,
    subscribedChannels,
    currentChannel,
    channels,
    navigationPannelHidden,
    wsGlobalClient,
    wsChannelClient,
    textMessages,
  } = state;

  // adding websocket client
  React.useEffect0(() => {
    AddWsGlobalClient(Some(Webapi.WebSocket.wsbc(url))) |> dispatch;
    Some(() => ());
  });

  // setting state for context
  let value: ContextProvider.appContext = {
    dispatch,
    wsGlobalClient,
    wsChannelClient,
    textMessages,
  };

  // message handler function
  let messageHandler = (messageEvent: WebSocket.messageEvent): unit => {
    "> incoming message" |> Js.log;
    messageEvent |> Js.log;

    messageEvent.data
    |> Js.Json.parseExn
    |> Decoders.decodeMessage
    |> messageToAction
    |> dispatch;
  };

  // adding an event listener to our websocket
  switch (wsGlobalClient) {
  | None => "no websocket passed in..." |> Js.log
  | Some(w) => w |> WebSocket.onmessage(~messageHandler)
  };

  switch (wsChannelClient) {
  | None => "no websocket passed in..." |> Js.log
  | Some(w) => w |> WebSocket.onmessage(~messageHandler)
  };

  state |> Js.log;

  <div className="app-container">
    <div onClick={_ => ToggleNavigationPannel |> dispatch} />
    <Header />
    <ContextProvider value>
      <NavigationPannel navigationPannelHidden channels />
    </ContextProvider>
    {switch (currentChannel) {
     | None => <Spinner />
     | Some(currentChannel) =>
       <ContextProvider value>
         <ChannelPannel currentChannel />
       </ContextProvider>
     }}
  </div>;
};