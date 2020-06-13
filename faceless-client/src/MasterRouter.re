[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  switch (url.path) {
  | [] => <App />
  | _ => <div> {"page not found." |> React.string} </div>
  };
};