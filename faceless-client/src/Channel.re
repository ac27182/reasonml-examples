open FacelessCore;

[@react.component]
let make = (~channel: Types.channelInfo) => {
  <div className="channel-container">
    <div> {channel.displayName |> React.string} </div>
    <div> {"message part" |> React.string} </div>
    <div> {"send part" |> React.string} </div>
  </div>;
};