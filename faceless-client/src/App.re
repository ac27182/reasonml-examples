[%bs.raw {|require("./css/app.css")|}];
open FacelessCore;
open FacelessCore.Types;
open FacelessCore.Webapi;
open Relude_Globals;
open ClientLogic;
open Relude_Option;
open ReactEvent;

type action = ClientLogic.appAction;

type state = {
  channels: list(channelInfo),
  textMessages: option(list(textMessage)),
  currentChannel: option(channelInfo),
  navigationPannelHidden: bool,
  wsGlobalClient: option(WebSocket.t),
  wsChannelClient: option(WebSocket.t),
  name: string,
  nameInput: string,
};

let initialState = {
  channels: List.empty,
  navigationPannelHidden: false,
  currentChannel: None,
  textMessages: None,
  wsGlobalClient: None,
  wsChannelClient: None,
  name: "",
  nameInput: "",
};

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
  | HandleNameInputChange(nameInput) => {...state, nameInput}
  | SetName(name) => {...state, name, nameInput: ""}
  };

let messageToAction = (message: Types.message): action =>
  switch (message) {
  | ChannelInfoListMessage(m) => PopulateChannels(m)
  | TextMessageListMessage(m) => PopulateTextMessages(Some(m))
  | TextMessageMessage(m) => AppendTextMessage(m)
  | ChannelInfoMessage(m) => AppendChannelInfo(m)
  };

[@react.component]
let make = (~userId: string) => {
  let (state, dispatch: action => unit) =
    React.useReducer(reducer, initialState);

  let {
    currentChannel,
    channels,
    navigationPannelHidden,
    wsGlobalClient,
    wsChannelClient,
    textMessages,
    name,
    nameInput,
  } = state;

  // message handler function
  let messageHandler = (messageEvent: WebSocket.messageEvent): unit =>
    messageEvent.data
    |> Js.Json.parseExn
    |> Decoders.decodeMessage
    |> messageToAction
    |> dispatch;

  React.useEffect0(() => {
    let url = "ws://localhost:3000/global";
    AddWsGlobalClient(Webapi.WebSocket.wsbc(url) |> Option.some) |> dispatch;
    Some(() => ());
  });

  React.useEffect1(
    () => {
      switch (wsGlobalClient) {
      | None => "no websocket passed in..." |> Js.log
      | Some(w) => w |> WebSocket.onmessage(~messageHandler)
      };
      // need to close down the websocket connection
      Some(() => ());
    },
    [wsGlobalClient] |> Array.fromList,
  );

  React.useEffect1(
    () => {
      switch (wsChannelClient) {
      | None => "no websocket passed in..." |> Js.log
      | Some(w) => w |> WebSocket.onmessage(~messageHandler)
      };
      Some(() => ());
    },
    [wsChannelClient] |> Array.fromList,
  );

  // setting state for context
  let value: ContextProvider.appContext = {
    dispatch,
    wsGlobalClient,
    wsChannelClient,
    textMessages,
    authorId: userId,
  };

  let handleNameInputChange = (event: ReactEvent.Form.t): unit =>
    HandleNameInputChange(event->Form.target##value) |> dispatch;

  let handleSetName = (_: ReactEvent.Mouse.t): unit =>
    SetName(nameInput) |> dispatch;

  <div className="app-container">
    <ContextProvider value>
      <NavigationPannel navigationPannelHidden channels />
    </ContextProvider>
    {switch (currentChannel) {
     | None => <Spinner name nameInput handleNameInputChange handleSetName />
     | Some(currentChannel) =>
       <ContextProvider value>
         <ChannelPannel currentChannel />
       </ContextProvider>
     }}
  </div>;
};