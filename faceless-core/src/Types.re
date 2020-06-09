// not needed anymore
// type clientInfo = {
//   id: string,
//   connectedChannels: list(string),
// };

// not needed anymore
// type client = {
//   clientInfo,
//   connection: WebSocket.Client.t,
// };

// not needed anymore
// type state = {
//   channelPool: list(channel),
//   clientPool: list(client),
// };

// not needed anymore
// type channel = {channelInfo};

type textMessage = {
  id: string,
  clientId: string,
  creationTimestamp: int,
  data: string,
};

type channelInfo = {
  displayName: string,
  id: string,
  hidden: bool,
  password: option(string),
  creationTimestamp: int,
};

type message =
  | ChannelInfoMessage(channelInfo)
  | ChannelInfoListMessage(list(channelInfo));