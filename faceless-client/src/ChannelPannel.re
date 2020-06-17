open FacelessCore;
open FacelessCore.Types;

[%bs.raw {|require("./css/channelPannel.css")|}];

type state = {wsClient: option(Webapi.WebSocket.t)};

let initialState = {wsClient: None};

type action =
  | AddClient(option(Webapi.WebSocket.t));

let reducer = (state, action) =>
  switch (action) {
  | AddClient(w) => {wsClient: w}
  };

[@react.component]
let make = (~currentChannel: Types.channelInfo) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let url: string = "ws://localhost:3000/" ++ currentChannel.id;

  let appContext = React.useContext(ContextProvider.appContext);

  React.useEffect1(
    () => {
      ClientLogic.AddWsChannelClient(Some(Webapi.WebSocket.wsbc(url)))
      |> appContext.dispatch;
      Some(() => ());
    },
    [|currentChannel|],
  );

  state |> Js.log;
  <div className="channel-pannel-container">
    <ChannelHeader currentChannel />
    <MessageDisplay textMessages={appContext.textMessages} />
    <MessageInputBar />
  </div>;
};