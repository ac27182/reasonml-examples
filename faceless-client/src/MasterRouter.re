open FacelessCore;
open Relude_Globals;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let webSocketClient: option(Webapi.WebSocket.t) =
    Some(Webapi.WebSocket.wsbc("ws://localhost:3000"));

  let userId: string = Uuid.v4();

  switch (url.path) {
  | _ => <App webSocketClient userId />
  // | _ => <div> {"page not found." |> React.string} </div>
  };
};