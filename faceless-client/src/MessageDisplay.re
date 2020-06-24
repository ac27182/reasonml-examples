open FacelessCore;
open FacelessCore.Types;

module MesageDisplayItem = {
  [@react.component]
  let make = (~textMessage: Types.textMessage, ~clientMessage: bool) => {
    let {id, creationTimestamp, data, authorName} = textMessage;

    let className =
      clientMessage
        ? "message-display-item-container user-message-item"
        : "message-display-item-container";

    <div className>
      <div> {"nam - " ++ authorName |> React.string} </div>
      <div> {"id_ - " ++ id |> React.string} </div>
      <div>
        {"ct_ - "
         ++ (creationTimestamp |> Js.Date.fromFloat |> Js.Date.toISOString)
         |> React.string}
      </div>
      <div className="message"> {"> " ++ data |> React.string} </div>
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
let make = (~textMessages: option(list(textMessage)), ~authorId: string) =>
  <div className="message-display-container">
    {switch (textMessages) {
     | None => <div> {"no messages." |> React.string} </div>
     | Some(m) =>
       m
       |> List.sort((m0: textMessage, m1: textMessage) =>
            sortf(m0.creationTimestamp, m1.creationTimestamp)
          )
       |> List.map(textMessage =>
            <MesageDisplayItem
              textMessage
              clientMessage={textMessage.authorId == authorId}
            />
          )
       |> Array.of_list
       |> React.array
     }}
  </div>;