type textMessage = {
  id: string,
  authorId: string,
  data: string,
  creationTimestamp: float,
  authorName: string,
};

type channelInfo = {
  id: string,
  displayName: string,
  hidden: bool,
  password: option(string),
  creationTimestamp: float,
};

type message =
  | ChannelInfoMessage(channelInfo)
  | ChannelInfoListMessage(list(channelInfo))
  | TextMessageMessage(textMessage)
  | TextMessageListMessage(list(textMessage));

type channelMultiplexer =
  | GlobalChannel
  | MessageChannel(string);

type environment = {
  redisPort: option(int),
  webSocketPort: option(int),
  redisHost: option(string),
};