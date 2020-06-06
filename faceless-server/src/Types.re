type client = {
  id: string,
  ws: WebSocket.Client.t,
};

type pathDetails = {
  channelName: string,
  userId: string,
};

type channel = {
  name: string,
  password: string,
  protected: bool,
  hidden: bool,
  creationTime: int,
  clientPool: list(client),
};

type state = {channels: list(channel)};