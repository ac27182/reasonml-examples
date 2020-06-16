open FacelessCore;
open FacelessCore.Types;
open ContextProvider;

[@react.component]
let make = (~currentChannel: Types.channelInfo) => {
  let {dispatch} = React.useContext(ContextProvider.appContext);
  <div className="channel-header-container ">
    <div> {"image" |> React.string} </div>
    <div> {currentChannel.displayName |> React.string} </div>
    <div> {currentChannel.id |> React.string} </div>
    <button onClick={_ => dispatch |> ClientLogic.leaveChannel}>
      {"disconnect" |> React.string}
    </button>
  </div>;
};