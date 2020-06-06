let b2s = (b: bool): string => b ? "true" : "false";

let makePannelItem = (c: Client.channel): React.element => {
  <div className="pannel-item">
    <div> {c.name |> React.string} </div>
    <div> {"hidden: " ++ (c.hidden |> b2s) |> React.string} </div>
    <div> {"protected: " ++ (c.password |> b2s) |> React.string} </div>
  </div>;
};

[@react.component]
let make = (~channels: list(Client.channel)) => {
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