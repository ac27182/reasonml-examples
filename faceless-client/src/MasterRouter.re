open FacelessCore;

// let url = ReasonReactRouter.useUrl();
// switch (url.path) {
// | _ => <App webSocketClient userId />
// };

let userId: string = Uuid.v4();
[@react.component]
let make = () => {
  <App userId />;
};