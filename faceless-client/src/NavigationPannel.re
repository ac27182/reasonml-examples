[%bs.raw {|require("./css/navigationPannel.css")|}];
open FacelessCore;
open Relude_Globals;
open ClientLogic;
open ReactEvent;
open FacelessCore.Types;
type state = {
  filterInput: string,
  createChannelCollapsed: bool,
  newChannelDisplayNameInput: string,
  newChannelPasswordInput: string,
  newChannelHiddenToggle: bool,
};

type action = ClientLogic.navigationPannelAction;

let initialState: state = {
  filterInput: String.empty,
  createChannelCollapsed: true,
  newChannelDisplayNameInput: String.empty,
  newChannelPasswordInput: String.empty,
  newChannelHiddenToggle: false,
};

let reducer = (state, action) =>
  switch (action) {
  | ToggleCollapse => {
      ...state,
      createChannelCollapsed: !state.createChannelCollapsed,
    }
  | FilterChannels(s) => {...state, filterInput: s}
  | ClearFilter => {...state, filterInput: String.empty}
  | HandleNewChannelDisplayNameInput(newChannelDisplayNameInput) => {
      ...state,
      newChannelDisplayNameInput,
    }
  | HandleNewChannelPasswordInput(newChannelPasswordInput) => {
      ...state,
      newChannelPasswordInput,
    }
  | HandleNewChannelHiddenToggle => {
      ...state,
      newChannelHiddenToggle: !state.newChannelHiddenToggle,
    }
  | HandleCreateNewChannel => {
      ...state,
      newChannelDisplayNameInput: String.empty,
      newChannelPasswordInput: String.empty,
      newChannelHiddenToggle: false,
    }
  };

module ChannelSearchBar = {
  [@react.component]
  let make =
      (
        ~filterInput: string,
        ~clearFilter: ReactEvent.Mouse.t => unit,
        ~filterChannels: ReactEvent.Form.t => unit,
      ) =>
    <div className="channel-search-bar-container">
      <input
        placeholder="search for channel"
        value=filterInput
        onChange=filterChannels
      />
      <button onClick=clearFilter> {"_" |> React.string} </button>
    </div>;
};

module NavigationPannelHeader = {
  [@react.component]
  let make = () =>
    <div className="navigation-pannel-header-container">
      {"faceless" |> React.string}
    </div>;
};

module CreateChannelBar = {
  [@react.component]
  let make =
      (
        ~toggleCollapse: 'a => unit,
        ~handleNewChannelDisplayNameInput: ReactEvent.Form.t => unit,
        ~handleNewChannelPasswordInput: ReactEvent.Form.t => unit,
        ~handleNewChannelHiddenToggle: ReactEvent.Mouse.t => unit,
        ~handleCreateNewChannel: ReactEvent.Mouse.t => unit,
        ~createChannelCollapsed: bool,
        ~newChannelDisplayNameInput: string,
        ~newChannelPasswordInput: string,
        ~newChannelHiddenToggle: bool,
      ) =>
    createChannelCollapsed
      ? <div className="create-channel-container-collapsed">
          <button onClick=toggleCollapse>
            {"create new channel" |> React.string}
          </button>
        </div>
      : <div className="create-channel-container-open">
          <input
            placeholder="channel display name"
            onChange=handleNewChannelDisplayNameInput
            value=newChannelDisplayNameInput
            maxLength=15
          />
          // <input
          //   placeholder="channel password"
          //   onChange=handleNewChannelPasswordInput
          //   value=newChannelPasswordInput
          // />
          // <button onClick=handleNewChannelHiddenToggle>
          //   {"hidden" |> React.string}
          // </button>
          <div className="containerx">
            <button onClick=toggleCollapse>
              {"cancel" |> React.string}
            </button>
            <button
              disabled={newChannelDisplayNameInput |> String.length <= 2}
              onClick=handleCreateNewChannel>
              {"create" |> React.string}
            </button>
          </div>
        </div>;
};

[@react.component]
let make =
    (~navigationPannelHidden: bool, ~channels: list(Types.channelInfo)) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  let appContext = React.useContext(ContextProvider.appContext);

  let {
    filterInput,
    newChannelDisplayNameInput,
    newChannelPasswordInput,
    newChannelHiddenToggle,
    createChannelCollapsed,
  } = state;

  // component functions
  let filterChannels = (event: ReactEvent.Form.t): unit =>
    FilterChannels(event->Form.target##value) |> dispatch;

  let clearFilter = _: unit => ClearFilter |> dispatch;

  let toggleCollapse = _: unit => ToggleCollapse |> dispatch;

  // super verbose, we can definately fine a cleaner way to do this late on...
  // these are pretty much the same function
  let handleNewChannelDisplayNameInput = (event: ReactEvent.Form.t): unit => {
    let newInput = event->Form.target##value;
    let regexp = [%re "/^[a-zA-Z0-9_]*$/g"];
    let opt = newInput |> Js.String.match(regexp);
    switch (opt) {
    | None => ()
    | _ => HandleNewChannelDisplayNameInput(newInput) |> dispatch
    };
  };

  let handleNewChannelPasswordInput = (event: ReactEvent.Form.t): unit =>
    HandleNewChannelPasswordInput(event->Form.target##value) |> dispatch;

  let handleNewChannelHiddenToggle = _: unit =>
    HandleNewChannelHiddenToggle |> dispatch;

  let handleCreateNewChannel = _: unit => {
    let m: Types.message =
      ChannelInfoMessage({
        id: Uuid.v4(),
        displayName: newChannelDisplayNameInput,
        hidden: newChannelHiddenToggle,
        password: Some(newChannelPasswordInput),
        creationTimestamp: Js.Date.now(),
      });

    switch (appContext.wsGlobalClient) {
    | Some(w) =>
      w
      |> FacelessCore.Webapi.WebSocket.send(
           ~message=
             m |> FacelessCore.Encoders.encodeMessage |> Js.Json.stringify,
         );
      HandleCreateNewChannel |> dispatch;

    | None => "global websocket unavailible. <handle error>" |> Js.log
    };
  };

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

  <div className="navigation-pannel-container">
    <NavigationPannelHeader />
    <ChannelSearchBar clearFilter filterChannels filterInput />
    <CreateChannelBar
      toggleCollapse
      createChannelCollapsed
      handleNewChannelDisplayNameInput
      handleNewChannelPasswordInput
      handleNewChannelHiddenToggle
      newChannelDisplayNameInput
      newChannelPasswordInput
      newChannelHiddenToggle
      handleCreateNewChannel
    />
    <ChannelInfoDisplay
      channels={channels |> channelListFilter(~filterInput)}
    />
  </div>;
};