[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  // let websocket = WsBrowserClient.wsbc("ws://localhost:3000/");

  switch (url.path) {
  | ["test"] => <ChannelTester />
  | [c] =>
    <App
      channel={Some(c)}
      websocket={Some(WsBrowserClient.wsbc("ws://localhost:3000/"))}
    />
  | [] =>
    <App
      channel=None
      websocket={Some(WsBrowserClient.wsbc("ws://localhost:3000/"))}
    />
  | _ => <div> {"page not found." |> React.string} </div>
  };
};