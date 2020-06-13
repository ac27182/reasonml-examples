[%bs.raw {|require("./navigationPannel.css")|}];

[@react.component]
let make = () =>
  <div className="navigation-pannel-container">
    <div> {" channels" |> React.string} </div>
    <div className="channel-search-bar-container">
      <input placeholder="search for channel" />
    </div>
    <div> {"add" |> React.string} </div>
    <ChannelInfoDisplay />
  </div>;