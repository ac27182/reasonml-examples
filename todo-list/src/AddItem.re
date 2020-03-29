open ReactEvent;

[@react.component]
let make = (~description: string, ~onEdit: Form.t => unit, ~onSave) =>
  <div className="add-item-bar">
    <input value=description onChange=onEdit />
    <button onClick=onSave disabled={description == ""}>
      {"add item" |> React.string}
    </button>
  </div>;