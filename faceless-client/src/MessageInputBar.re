open ClientLogic;
open ReactEvent;
open ContextProvider;
open FacelessCore;
open Webapi;
open Types;
open Relude_Globals;

type state = {messageInput: string};

let initialState: state = {messageInput: ""};

let reducer = (_, action: messageInputBarAction) =>
  switch (action) {
  | InputMessage(s) => {messageInput: s}
  | SendMessage => {messageInput: ""}
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  let {wsChannelClient, authorId, authorName} = React.useContext(appContext);
  let {messageInput} = state;

  <div className="message-input-bar-container">
    <input
      value=messageInput
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
                 TextMessageMessage({
                   id: Uuid.v4(),
                   authorId,
                   data: messageInput,
                   creationTimestamp: Js.Date.now(),
                   authorName,
                 })
                 |> Encoders.encodeMessage
                 |> Js.Json.stringify,
             );
          SendMessage |> dispatch;
        }
      }}>
      {">>=" |> React.string}
    </button>
  </div>;
};