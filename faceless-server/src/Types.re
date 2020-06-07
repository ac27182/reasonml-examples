type textMessage = {
  id: string,
  clientId: string,
  creationTimestamp: int,
  payload: string,
};

type clientInfo = {
  id: string,
  connectedChannels: list(string),
};

type client = {
  clientInfo,
  connection: WebSocket.Client.t,
};

type channelInfo = {
  displayName: string,
  id: string,
  hidden: bool,
  password: option(string),
  creationTimestamp: int,
};

let channelInfoEncoder = (c: channelInfo) =>
  Json.Encode.(
    object_([
      ("displayName", Json.Encode.string(c.displayName)),
      ("id", Json.Encode.string(c.id)),
      ("hidden", Json.Encode.bool(c.hidden)),
      (
        "password",
        Json.Encode.string(
          switch (c.password) {
          | None => ""
          | Some(p) => p
          },
        ),
      ),
      ("creationTimestamp", Json.Encode.int(c.creationTimestamp)),
    ])
  );

type channel = {channelInfo};

type state = {
  channelPool: list(channel),
  clientPool: list(client),
};