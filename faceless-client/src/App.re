let fakeChannelData: list(Client.channel) = [
  {name: "channel0", hidden: false, password: false},
  {name: "channel1", hidden: false, password: false},
  {name: "channel2", hidden: false, password: false},
  {name: "channel3", hidden: false, password: false},
  {name: "channel4", hidden: false, password: false},
  {name: "channel5", hidden: false, password: false},
  {name: "channel6", hidden: false, password: false},
  {name: "channel7", hidden: false, password: false},
  {name: "channel8", hidden: false, password: false},
  {name: "channel9", hidden: false, password: false},
  {name: "channel10", hidden: false, password: false},
  {name: "channel11", hidden: false, password: false},
  {name: "channel12", hidden: false, password: false},
  {name: "channel13", hidden: false, password: false},
];

let fakeChannel: Client.channel = {
  name: "channel0",
  hidden: false,
  password: false,
};

type state = {
  channels: list(Client.channel),
  connections: int,
  currentChannel: option(Client.channel),
  pannelHidden: bool,
};

type action =
  | TogglePannel
  | RemoveChannel(string)
  | CreateChannel(string);

let initialState: state = {
  channels: fakeChannelData,
  currentChannel: None,
  connections: 0,
  pannelHidden: false,
};

let reducer = (state, action) =>
  switch (action) {
  | TogglePannel => {...state, pannelHidden: !state.pannelHidden}
  | CreateChannel(name) =>
    let newChannel: Client.channel = {name, hidden: false, password: false};
    {...state, channels: state.channels |> List.append([newChannel])};
  | RemoveChannel(_) => state //unimplemented
  };

[@react.component]
let make = (~channel: option(string)) => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  <div className="app-grid">
    <SidePannel channels={state.channels} />
    {switch (state.currentChannel) {
     | None => <Spinner />
     | Some(c) => <Channel channel=c />
     }}
  </div>;
};