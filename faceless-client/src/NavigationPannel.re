[%bs.raw {|require("./css/navigationPannel.css")|}];
open FacelessCore;
open Relude_Globals;
open ClientLogic;
open ReactEvent;

type state = {
  filterInput: string,
  newChannelInput: string, // fill in later
  createChannelCollapsed: bool,
};

let initialState: state = {
  filterInput: String.empty,
  newChannelInput: String.empty,
  createChannelCollapsed: true,
};

let reducer = (state, action: navigationPannelAction) =>
  switch (action) {
  | ToggleCollapse => {
      ...state,
      createChannelCollapsed: !state.createChannelCollapsed,
    }
  | FilterChannels(s) => {...state, filterInput: s}
  | ClearFilter => {...state, filterInput: String.empty}
  };

// impure
module ChannelSearchBar = {
  [@react.component]
  let make =
      (
        ~filterInput: string,
        ~clearFilter: ReactEvent.Mouse.t => unit,
        ~filterChannels: ReactEvent.Form.t => unit,
      ) =>
    <div className="channel-search-bar-container">
      // filter bar

        <input
          placeholder="search for channel"
          value=filterInput
          onChange=filterChannels
        />
        <button onClick=clearFilter> {"clear" |> React.string} </button>
      </div>;
};

module CreateChannelBar = {
  [@react.component]
  let make = (~toggleCollapse: 'a => unit, ~createChannelCollapsed: bool) =>
    createChannelCollapsed
      ? <div className="create-channel-contianer-collapsed">
          <button onClick=toggleCollapse>
            {"create new channel" |> React.string}
          </button>
        </div>
      : <div className="create-channel-container-open">
          <input />
          <input />
          <button> {"hidden" |> React.string} </button>
          <button onClick=toggleCollapse> {"cancel" |> React.string} </button>
          <button> {"create" |> React.string} </button>
        </div>;
};

[@react.component]
let make =
    (~navigationPannelHidden: bool, ~channels: list(Types.channelInfo)) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let {filterInput, newChannelInput, createChannelCollapsed} = state;

  // component functions
  let filterChannels = (event: ReactEvent.Form.t): unit =>
    FilterChannels(event->Form.target##value) |> dispatch;

  let clearFilter = _: unit => ClearFilter |> dispatch;

  let toggleCollapse = _: unit => ToggleCollapse |> dispatch;

  let channelListFilter =
      (~filterInput: string, channels: list(Types.channelInfo))
      : list(Types.channelInfo) =>
    channels
    |> List.filter((channel: Types.channelInfo) =>
         channel.displayName
         |> String.contains(~search=filterInput)
         || channel.id
         |> String.contains(~search=filterInput)
       );

  // main component
  <div className="navigation-pannel-container">
    <div> {" channels" |> React.string} </div> // header
    <ChannelSearchBar clearFilter filterChannels filterInput />
    <CreateChannelBar toggleCollapse createChannelCollapsed />
    <ChannelInfoDisplay
      channels={channels |> channelListFilter(~filterInput)}
    />
  </div>;
};