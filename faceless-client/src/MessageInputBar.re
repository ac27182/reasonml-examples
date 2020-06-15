open Relude_Globals;
open ClientLogic;
open ReactEvent;
open ContextProvider;
open FacelessCore.Webapi;
open FacelessCore.Types;
open FacelessCore;

type state = {messageInput: string};

let initialState: state = {messageInput: String.empty};

let reducer = (_, action: messageInputBarAction) =>
  switch (action) {
  | InputMessage(s) => {messageInput: s}
  | SendMessage => {messageInput: String.empty}
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let {wsChannelClient} = React.useContext(appContext);

  let textMessage: textMessage = {
    id: Uuid.v4(),
    authorId: Uuid.v4(),
    data: state.messageInput,
    creationTimestamp: Js.Date.now(),
  };

  <div className="message-input-bar-container">
    <input
      value={state.messageInput}
      onChange={event => InputMessage(event->Form.target##value) |> dispatch}
    />
    <button
      onClick={_ => {
        switch (wsChannelClient) {
        | None => "no websocket availible" |> Js.log
        | Some(w) =>
          w
          |> WebSocket.send(
               ~message=
                 TextMessageMessage(textMessage)
                 |> Encoders.encodeMessage
                 |> Js.Json.stringify,
             );
          SendMessage |> dispatch;
          TextMessageMessage(textMessage)
          |> Encoders.encodeMessage
          |> Js.Json.stringify
          |> Js.log;
        }
      }}>
      {">>=" |> React.string}
    </button>
  </div>;
};