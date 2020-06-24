open FacelessCore;
open FacelessCore.Types;
open ContextProvider;

module Ticker = {
  type state = {currentTime: float};

  let initialState = {currentTime: Js.Date.now()};

  type action =
    | UpdateTime;

  let reducer = (_, action) =>
    switch (action) {
    | UpdateTime => {currentTime: Js.Date.now()}
    };

  [@react.component]
  let make = (~epoch: float) => {
    let (state, dispatch) = React.useReducer(reducer, initialState);

    React.useEffect0(() => {
      let iid = Js.Global.setInterval(() => UpdateTime |> dispatch, 200);
      Some(() => Js.Global.clearInterval(iid));
    });

    <div>
      {"age - "
       ++ (state.currentTime -. epoch |> Js.Float.toString)
       |> React.string}
    </div>;
  };
};

module ChannelInfoItem = {
  [@react.component]
  let make = (~channelInfo: Types.channelInfo) => {
    let {id, displayName, creationTimestamp} = channelInfo;

    let {dispatch} = React.useContext(ContextProvider.appContext);

    <div
      key={Uuid.v4()}
      className="channel-info-item-container"
      onClick={_ => dispatch |> ClientLogic.enterChannel(channelInfo)}>
      <div>
        <div className="channel-info-item-name">
          {displayName |> React.string}
        </div>
        <div> {"id_  - " ++ id |> React.string} </div>
        <div>
          {"ct_ - "
           ++ (creationTimestamp |> Js.Date.fromFloat |> Js.Date.toISOString)
           |> React.string}
        </div>
        <Ticker epoch=creationTimestamp />
      </div>
    </div>;
  };
};

[@react.component]
let make = (~channels: list(Types.channelInfo)) =>
  <div className="channel-info-display-container">
    {channels
     |> List.map(channelInfo => <ChannelInfoItem channelInfo />)
     |> Array.of_list
     |> React.array}
  </div>;