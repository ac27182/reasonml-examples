[%bs.raw {|require("./css/index.css")|}];
open FacelessCore;
module AppLoader = {
  type state = {
    userId: option(string),
    webSocketPort: option(int),
  };

  type action =
    | SetUserId(option(string))
    | SetWebSocketPort(option(int));

  let initialState = {userId: None, webSocketPort: None};

  let reducer = (state: state, action: action) =>
    switch (action) {
    | SetUserId(userId) => {...state, userId}
    | SetWebSocketPort(webSocketPort) => {...state, webSocketPort}
    };

  [@react.component]
  let make = () => {
    let (state, dispatch) = React.useReducer(reducer, initialState);
    let {userId, webSocketPort} = state;

    React.useEffect0(() => {
      switch (Dom_storage.localStorage |> Dom_storage.getItem("userId")) {
      | None => Some(Uuid.v4())->SetUserId |> dispatch
      | Some(uuid) => Some(uuid)->SetUserId |> dispatch
      };

      Some(() => ());
    });

    React.useEffect0(() => {
      let env = Node.Process.process |> Utils.parseProcess;
      switch (env.webSocketPort) {
      | None => Some(3000)->SetWebSocketPort |> dispatch
      | Some(port) => Some(port)->SetWebSocketPort |> dispatch
      };
      Some(() => ());
    });

    React.useEffect1(
      () => {
        switch (userId) {
        | None => ()
        | Some(uuid) =>
          Dom_storage.localStorage |> Dom_storage.setItem("userId", uuid)
        };
        Some(() => ());
      },
      [userId] |> Relude_Array.fromList,
    );

    switch (userId, webSocketPort) {
    | (Some(userId), Some(webSocketPort)) => <App userId webSocketPort />
    | (_, _) => <div> {"something went wrong" |> React.string} </div>
    };
  };
};

ReactDOMRe.renderToElementWithId(<AppLoader />, "root");