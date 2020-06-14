open FacelessCore;
open FacelessCore.Types;

[@react.component]
let make = (~currentChannel: Types.channelInfo) => {
  let dispatch = React.useContext(ContextProvider.appContext);
  <div className="channel-header-container ">
    <div>
      {currentChannel.displayName ++ "|" ++ currentChannel.id |> React.string}
    </div>
    <button onClick={_ => dispatch |> ClientLogic.leaveChannel}>
      {"minimise" |> React.string}
    </button>
    <button> {"disconnect" |> React.string} </button>
  </div>;
};