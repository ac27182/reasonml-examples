[@react.component]
let make =
    (
      ~item: TodoTypes.item,
      ~toggleDelete: TodoTypes.item => unit,
      ~toggleComplete: TodoTypes.item => unit,
    ) =>
  <div>
    <div> {item.description |> React.string} </div>
    <button onClick={_ => item |> toggleDelete}>
      {"delete" |> React.string}
    </button>
    <button onClick={_ => item |> toggleComplete}>
      {(item.complete ? "un-check" : "check") |> React.string}
    </button>
  </div>;