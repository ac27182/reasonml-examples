open ClientLogic;
open FacelessCore.Webapi;
open FacelessCore.Types;
open Relude_Globals;
type appContext = {
  dispatch: appAction => unit,
  wsGlobalClient: option(WebSocket.t),
  wsChannelClient: option(WebSocket.t),
  textMessages: option(list(textMessage)),
  authorId: string,
  authorName: string,
  webSocketPort: int,
};

let defaultAppConext: appContext = {
  dispatch: (_: appAction) => ("doing nothing..." |> Js.log: unit),
  wsGlobalClient: None,
  wsChannelClient: None,
  textMessages: None,
  // nb String.empty => undefined
  authorId: "",
  authorName: "",
  webSocketPort: 3000,
};

// let defaultValue = (_: appAction): unit => "doing nothing..." |> Js.log; // do nothing

let appContext = React.createContext(defaultAppConext);

let makeProps = (~value, ~children, ()) => {
  "value": value,
  "children": children,
};

let make = React.Context.provider(appContext);