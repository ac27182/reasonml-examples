open FacelessCore;
open FacelessCore.Types;
open ContextProvider;

[@react.component]
let make = (~currentChannel: Types.channelInfo) => {
  let {dispatch} = React.useContext(ContextProvider.appContext);
  <div className="channel-header-container ">
    <div>
      {"> "
       ++ currentChannel.displayName
       ++ " | "
       ++ currentChannel.id
       |> React.string}
    </div>
    <button onClick={_ => dispatch |> ClientLogic.leaveChannel}>
      {"---" |> React.string}
    </button>
  </div>;
};