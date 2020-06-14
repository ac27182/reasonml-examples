[%bs.raw {|require("./css/navigationPannel.css")|}];
open FacelessCore;
open Relude_Globals;
open ClientLogic;
open ReactEvent;

type state = {
  filterInput: string,
  newChannelInput: string // fill in later
};

let initialState: state = {
  filterInput: String.empty,
  newChannelInput: String.empty,
};

let reducer = (state, action: navigationPannelAction) =>
  switch (action) {
  | FilterChannels(s) => {...state, filterInput: s}
  | ClearFilter => {...state, filterInput: String.empty}
  };

[@react.component]
let make =
    (~navigationPannelHidden: bool, ~channels: list(Types.channelInfo)) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  <div className="navigation-pannel-container">
    <div> {" channels" |> React.string} </div>
    <div className="channel-search-bar-container">
      <input
        placeholder="search for channel"
        value={state.filterInput}
        onChange={event => {
          event->Form.target##value |> Js.log;
          FilterChannels(event->Form.target##value) |> dispatch;
        }}
      />
      <button onClick={_ => ClearFilter |> dispatch}>
        {"clear" |> React.string}
      </button>
    </div>
    <div> {"" |> React.string} </div>
    <ChannelInfoDisplay
      channels={
        channels
        |> List.filter((c: Types.channelInfo) =>
             c.displayName |> String.contains(~search=state.filterInput)
           )
      }
    />
  </div>;
};