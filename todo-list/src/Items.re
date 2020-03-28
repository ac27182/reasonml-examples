[@react.component]
let make = (~items: list(TodoTypes.item), ~toggleDelete: string => unit) =>
  items
  |> List.mapi((index, item: TodoTypes.item) =>
       <div key={item.id}>
         <div> {item.description |> React.string} </div>
         <button onClick={_ => toggleDelete(item.id)}>
           {"delete" |> React.string}
         </button>
         <button> {"complete" |> React.string} </button>
       </div>
     )
  |> Array.of_list
  |> React.array;