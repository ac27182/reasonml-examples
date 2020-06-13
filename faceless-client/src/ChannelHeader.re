open FacelessCore;
open FacelessCore.Types;

[@react.component]
let make = (~channelInfo: Types.channelInfo) =>
  <div className="channel-header-container ">
    {channelInfo.displayName ++ "|" ++ channelInfo.id |> React.string}
  </div>;