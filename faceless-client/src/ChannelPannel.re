open FacelessCore;
open FacelessCore.Types;

[%bs.raw {|require("./channelPannel.css")|}];

let fakeChannelInfo: Types.channelInfo = {
  id: Uuid.v4(),
  displayName: "channel-" ++ Uuid.v4(),
  hidden: false,
  password: None,
  creationTimestamp: Js.Date.now(),
};

[@react.component]
let make = () =>
  <div className="channel-pannel-container">
    <ChannelHeader channelInfo=fakeChannelInfo />
    <MessageDisplay />
    <MessageInputBar />
  </div>;