open Relude_Globals;

module WsBrowserClient = {
  type t;
  [@bs.new] external wsbc: string => t = "WebSocket";
  [@bs.send] external send: (t, ~message: string) => unit = "send";
};

// master state reducer

type state = {
  channelName: string,
  clientName: string,
  message: string,
  client: option(WsBrowserClient.t),
};

type action =
  | AddClient(option(WsBrowserClient.t), string, string)
  | RemoveClient;

let initialState = {
  channelName: "",
  clientName: "",
  message: "",
  client: None,
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

// helper function
let connect = (i: inputState): option(WsBrowserClient.t) =>
  Some(
    WsBrowserClient.wsbc(
      "ws://localhost:3000/"
      ++ i.newChannelName
      ++ "/"
      ++ i.newClientName
      ++ "/",
    ),
  );

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  let (inputState, inputDispatch) =
    React.useReducer(inputReducer, initialInputState);

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