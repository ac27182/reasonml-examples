open FacelessCore;
open FacelessCore.Types;
open ContextProvider;

[%bs.raw {|require("./css/channelPannel.css")|}];

[@react.component]
let make = (~currentChannel: Types.channelInfo) => {
  let url: string = "ws://localhost:3000/" ++ currentChannel.id;

  let {textMessages, authorId, dispatch} =
    React.useContext(ContextProvider.appContext);

  React.useEffect1(
    () => {
      ClientLogic.AddWsChannelClient(Some(Webapi.WebSocket.wsbc(url)))
      |> dispatch;
      Some(() => ());
    },
    [|currentChannel|],
  );

  <div className="channel-pannel-container">
    <ChannelHeader currentChannel />
    <MessageDisplay textMessages authorId />
    <MessageInputBar />
  </div>;
};