open FacelessCore;
open FacelessCore.Types;

module MesageDisplayItem = {
  [@react.component]
  let make = (~textMessage: Types.textMessage) => {
    let {id, authorId, creationTimestamp, data} = textMessage;
    <div key={Uuid.v4()} className="message-display-item-container">
      <div> {"id_ - " ++ id |> React.string} </div>
      <div> {"ida - " ++ authorId |> React.string} </div>
      <div>
        {"ct_ - " ++ (creationTimestamp |> Js.Float.toString) |> React.string}
      </div>
      <div> {"msg - " ++ data |> React.string} </div>
    </div>;
  };
};

let sortf = (i0: float, i1: float): int =>
  switch (i0 < i1, i0 == i1) {
  | (_, true) => 0
  | (true, _) => 1
  | (_, _) => (-1)
  };

[@react.component]
let make = (~textMessages: option(list(textMessage))) =>
  <div className="message-display-container">
    {switch (textMessages) {
     | None => <div> {"no messages." |> React.string} </div>
     | Some(m) =>
       m
       |> List.sort((m0:textMessage, m1:textMessage) =>
            sortf(m0.creationTimestamp, m1.creationTimestamp)
          )
       |> List.map(textMessage => <MesageDisplayItem textMessage />)
       |> Array.of_list
       |> React.array
     }}
  </div>;