[@react.component]
let make =
    (
      ~items: list(TodoTypes.item),
      ~toggleDelete: TodoTypes.item => unit,
      ~toggleComplete: TodoTypes.item => unit,
    ) =>
  items
  |> List.map((item: TodoTypes.item) =>
       <div>
         <div> {item.description |> React.string} </div>
         <button onClick={_ => toggleDelete(item)}>
           {"delete" |> React.string}
         </button>
         <button onClick={_ => toggleComplete(item)}>
           {(item.complete ? "readd" : "complete") |> React.string}
         </button>
       </div>
     )
  |> Array.of_list
  |> React.array;