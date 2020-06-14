open FacelessCore;
open FacelessCore.Types;

module ChannelInfoItem = {
  [@react.component]
  let make = (~channelInfo: Types.channelInfo) => {
    let {id, displayName, hidden, creationTimestamp} = channelInfo;

    let appDispatch = React.useContext(ContextProvider.appContext);

    <div
      className="channel-info-item-container"
      onClick={_ => appDispatch |> ClientLogic.enterChannel(channelInfo)}>
      <div> {"image" |> React.string} </div>
      <div className="f">
        <div> {displayName |> React.string} </div>
        <div> {"id: " ++ id |> React.string} </div>
        <div> {(hidden ? "h" : "v") |> React.string} </div>
        <div>
          {"creation time: "
           ++ (creationTimestamp |> Js.Date.fromFloat |> Js.Date.toDateString)
           |> React.string}
        </div>
        <div>
          {"age: "
           ++ (creationTimestamp |> Js.Date.fromFloat |> Js.Date.toDateString)
           |> React.string}
        </div>
      </div>
    </div>;
  };
};

// let fakeChannelInfo: Types.channelInfo = {
//   id: Uuid.v4(),
//   displayName: "channel-" ++ Uuid.v4(),
//   hidden: false,
//   password: None,
//   creationTimestamp: Js.Date.now(),
// };

[@react.component]
let make = (~channels: list(Types.channelInfo)) =>
  <div className="channel-info-display-container">
    {channels
     |> List.map(channelInfo => <ChannelInfoItem channelInfo />)
     |> Array.of_list
     |> React.array}
  </div>;