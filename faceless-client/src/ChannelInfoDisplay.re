open FacelessCore;
open FacelessCore.Types;
open ContextProvider;

module ChannelInfoItem = {
  [@react.component]
  let make = (~channelInfo: Types.channelInfo) => {
    let {id, displayName, hidden, creationTimestamp} = channelInfo;

    let {dispatch} = React.useContext(ContextProvider.appContext);

    <div
      key={Uuid.v4()}
      className="channel-info-item-container"
      onClick={_ => dispatch |> ClientLogic.enterChannel(channelInfo)}>
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

[@react.component]
let make = (~channels: list(Types.channelInfo)) =>
  <div className="channel-info-display-container">
    {channels
     |> List.map(channelInfo => <ChannelInfoItem channelInfo />)
     |> Array.of_list
     |> React.array}
  </div>;