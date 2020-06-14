open ClientLogic;

let defaultValue = (_: appAction): unit => "doing nothing..." |> Js.log; // do nothing

let appContext = React.createContext(defaultValue: appAction => unit);

let makeProps = (~value, ~children, ()) => {
  "value": value,
  "children": children,
};

let make = React.Context.provider(appContext);