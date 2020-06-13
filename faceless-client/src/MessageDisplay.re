open FacelessCore;
open FacelessCore.Types;

module MesageDisplayItem = {
  [@react.component]
  let make = (~textMessage: Types.textMessage) => {
    let {id, authorId, creationTimestamp, data} = textMessage;
    <div className="message-display-item-container">
      <div> {"messageId: " ++ id |> React.string} </div>
      <div> {"authorId: " ++ authorId |> React.string} </div>
      <div> {"data: " ++ data |> React.string} </div>
      <div>
        {"date: "
         ++ (creationTimestamp |> Js.Date.fromFloat |> Js.Date.toDateString)
         |> React.string}
      </div>
    </div>;
  };
};

let fakeMessage: Types.textMessage = {
  id: Uuid.v4(),
  authorId: Uuid.v4(),
  creationTimestamp: Js.Date.now(),
  data: "> message body",
};

[@react.component]
let make = () =>
  <div className="message-display-container">
    <MesageDisplayItem textMessage=fakeMessage />
    <MesageDisplayItem textMessage=fakeMessage />
    <MesageDisplayItem textMessage=fakeMessage />
    <MesageDisplayItem textMessage=fakeMessage />
    <MesageDisplayItem textMessage=fakeMessage />
    <MesageDisplayItem textMessage=fakeMessage />
  </div>;