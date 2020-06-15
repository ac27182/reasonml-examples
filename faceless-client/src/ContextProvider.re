open ClientLogic;
open FacelessCore.Webapi;
open FacelessCore.Types;

type appContext = {
  dispatch: appAction => unit,
  wsGlobalClient: option(WebSocket.t),
  wsChannelClient: option(WebSocket.t),
  textMessages: option(list(textMessage)),
};

let defaultAppConext: appContext = {
  dispatch: (_: appAction) => ("doing nothing..." |> Js.log: unit),
  wsGlobalClient: None,
  wsChannelClient: None,
  textMessages: None,
};

// let defaultValue = (_: appAction): unit => "doing nothing..." |> Js.log; // do nothing

let appContext = React.createContext(defaultAppConext);

let makeProps = (~value, ~children, ()) => {
  "value": value,
  "children": children,
};

let make = React.Context.provider(appContext);