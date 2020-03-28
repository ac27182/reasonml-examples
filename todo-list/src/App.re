open TodoTypes;
type state = {
  items: list(string),
  currentInput: string,
};

let initialState = {items: [], currentInput: ""};

let reducer = (state, action) =>
  switch (action) {
  | Save(description) => {
      ...initialState,
      items: state.items |> List.append([description]),
    }
  | Edit(input) => {...state, currentInput: input}
  };

let l = ["x", "y", "z"];

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  <div>
    <Header />
    <AddItem currentInput={state.currentInput} d=dispatch />
    {state.items
     |> List.map(i => <div> {ReasonReact.string(i)} </div>)
     |> Array.of_list
     |> ReasonReact.array}
  </div>;
};