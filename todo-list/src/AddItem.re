open ReactEvent;

[@react.component]
let make = (~description: string, ~onEdit: Form.t => unit, ~onSave) => {
  <div>
    <input value=description onChange=onEdit />
    <button onClick=onSave disabled={description == ""}>
      {"add item" |> React.string}
    </button>
  </div>;
};