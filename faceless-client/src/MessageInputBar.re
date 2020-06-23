open Relude_Globals;
open ClientLogic;
open ReactEvent;
open ContextProvider;
open FacelessCore.Webapi;
open FacelessCore;
open FacelessCore.Types;
// open Relude_Globals.Array;

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

  // React.useEffect1(() => {}, [messageInput]|> Array.fromList);

  let textMessage: textMessage = {
    id: Uuid.v4(),
    authorId,
    data: messageInput,
    creationTimestamp: Js.Date.now(),
    authorName,
  };

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
                 TextMessageMessage(textMessage)
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