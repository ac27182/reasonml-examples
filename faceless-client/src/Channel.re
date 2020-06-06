[@react.component]
let make = (~channel: Client.channel) => {
  <div className="channel-container">
    <div> {channel.name |> React.string} </div>
    <div> {"message part" |> React.string} </div>
    <div> {"send part" |> React.string} </div>
  </div>;
};