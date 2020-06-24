open FacelessCore;
open FacelessCore.Types;
open ContextProvider;

[%bs.raw {|require("./css/channelPannel.css")|}];

[@react.component]
let make = (~currentChannel: Types.channelInfo) => {
  let {textMessages, authorId, dispatch, webSocketPort} =
    React.useContext(ContextProvider.appContext);

  let url: string =
    ClientLogic.makeWsUrl(webSocketPort, "/" ++ currentChannel.id);

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