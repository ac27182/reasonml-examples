[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  switch (url.path) {
  | ["test"] => <ChannelTester />
  | [c] => <App channel={Some(c)} />
  | [] => <App channel=None />
  | _ => <div> {"page not found." |> React.string} </div>
  };
};