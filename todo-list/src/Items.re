[@react.component];
let make = (~items: list(string)) => {
  items
  |> List.map(i => <div> {ReasonReact.string(i)} </div>)
  |> Array.of_list
  |> React.array;
};