open FacelessCore;

let b2s = (b: bool): string => b ? "true" : "false";

[@react.component]
let make =
    (
      ~channels: list(Types.channelInfo),
      ~enterChannel: Types.channelInfo => unit,
    ) => {
  let makePannelItem = (c: Types.channelInfo): React.element => {
    <div className="pannel-item" onClick={_ => enterChannel(c)}>
      <div> {c.displayName |> React.string} </div>
      <div> {"hidden: " ++ (c.hidden |> b2s) |> React.string} </div>
    </div>;
  };

  <div className="pannel">
    <div> {"channels" |> React.string} </div>
    <div>
      <input placeholder="search" />
      <button> {"search" |> React.string} </button>
    </div>
    <div className="pannel-item-wrapper">
      {channels
       |> List.map(c => c |> makePannelItem)
       |> Array.of_list
       |> React.array}
    </div>
  </div>;
};