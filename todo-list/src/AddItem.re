open ReactEvent;
open TodoTypes;

[@react.component]
let make = (~d, ~currentInput) => {
  <div>
    <input
      value=currentInput
      type_="text"
      maxLength=100
      placeholder="item goes here"
      onChange={event => {
        let v = event->Form.target##value;
        d(Edit(v));
      }}
    />
    <button onClick={_ => d(Save(currentInput))}>
      {"add item" |> React.string}
    </button>
  </div>;
};