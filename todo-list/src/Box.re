open ReactEvent;

type state = {name: string};

let initialState = {name: ""};

let reducer = (_, action: string) => {name: action};

[@react.component]
let make = (): ReasonReact.reactElement => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  <div>
    <input
      onChange={event => event->Form.target##value |> dispatch}
      value={state.name}
    />
    <button onClick={_ => "" |> dispatch}> {"clear" |> React.string} </button>
    <div> {state.name |> React.string} </div>
  </div>;
};