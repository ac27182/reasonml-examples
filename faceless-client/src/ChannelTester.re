open Relude_Globals;
open FacelessCore;
open FacelessCore.Types;
open WsBrowserClient;

// master state reducer

type state = {
  channelName: string,
  clientName: string,
  message: string,
  client: option(WsBrowserClient.t),
  channels: list(Types.channelInfo),
};

type action =
  | AddClient(option(WsBrowserClient.t), string, string)
  | RemoveClient
  | PopulateChannels(list(channelInfo))
  | AddNewChannel(channelInfo);

let initialState = {
  channelName: "",
  clientName: "",
  message: "",
  client: None,
  channels: List.empty,
};

let reducer = (state, action) =>
  switch (action) {
  | AddClient(c, channelName, clientName) => {
      ...state,
      client: c,
      channelName,
      clientName,
    }
  | RemoveClient => {...state, client: None, channelName: "", clientName: ""}
  | PopulateChannels(channels) => {...state, channels}
  | AddNewChannel(channel) => {
      ...state,
      channels: state.channels |> List.append(channel),
    }
  };

// input state reducer
type inputState = {
  newChannelName: string,
  newClientName: string,
  message: string,
};

type inputAction =
  | ChangeChannelName(string)
  | ChangeClientName(string)
  | ChangeMessage(string)
  | SendMessage;

let initialInputState: inputState = {
  newChannelName: "",
  newClientName: "",
  message: "",
};

let inputReducer = (state: inputState, action) => {
  switch (action) {
  | ChangeChannelName(t) => {...state, newChannelName: t}
  | ChangeClientName(t) => {...state, newClientName: t}
  | ChangeMessage(m) => {...state, message: m}
  | SendMessage => {...state, message: ""}
  };
};

module Message_Consumer = {
  // consume the message
  let consume = (message: string): Types.message =>
    message |> Js.Json.parseExn |> Decoders.decodeMessage;

  // choose how to update the state based on the type of message recieved
  let discriminator = (message: Types.message) =>
    switch (message) {
    | ChannelInfoListMessage(m) => PopulateChannels(m)
    | ChannelInfoMessage(m) => AddNewChannel(m)
    };
};

open Message_Consumer;
[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let (inputState, inputDispatch) =
    React.useReducer(inputReducer, initialInputState);

  let websocket = WsBrowserClient.wsbc("ws://localhost:3000/");

  websocket->WsBrowserClient.onmessage(e => {
    let action = e.data |> consume |> discriminator;
    action |> dispatch;
  });

  // helper function

  // let onMessage = (e: WsBrowserClient.MessageEvent.t): unit => {

  // };

  let connect = (i: inputState): option(WsBrowserClient.t) => {
    let c =
      WsBrowserClient.wsbc(
        "ws://localhost:3000/"
        ++ i.newChannelName
        ++ "/"
        ++ i.newClientName
        ++ "/",
      );

    Some(c);
  };

  <main>
    <div>
      {"current channel name: " ++ state.channelName |> React.string}
      <div />
      <div />
      {"current client name: " ++ state.clientName |> React.string}
    </div>
    <div>
      <input
        placeholder="channel name"
        onChange={e =>
          e->ReactEvent.Form.target##value->ChangeChannelName->inputDispatch
        }
      />
      <input
        placeholder="client name"
        onChange={e =>
          e->ReactEvent.Form.target##value->ChangeClientName->inputDispatch
        }
      />
      <button
        onClick={_ =>
          dispatch(
            AddClient(
              connect(inputState),
              inputState.newChannelName,
              inputState.newClientName,
            ),
          )
        }>
        {"connect" |> React.string}
      </button>
    </div>
    <div>
      <input
        placeholder="message"
        value={inputState.message}
        onChange={e =>
          e->ReactEvent.Form.target##value->ChangeMessage->inputDispatch
        }
      />
    </div>
    <button
      disabled={inputState.message === ""}
      onClick={_ => {
        state.client
        |> Option.getOrThrow
        |> WsBrowserClient.send(~message=inputState.message);

        SendMessage |> inputDispatch;
      }}>
      {"send" |> React.string}
    </button>
  </main>;
};