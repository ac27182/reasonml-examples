type textMessage = {
  id: string,
  authorId: string,
  data: string,
  creationTimestamp: float,
};

type channelInfo = {
  id: string,
  displayName: string,
  hidden: bool,
  password: option(string),
  creationTimestamp: float,
};

type user = {
  id: string,
  creationTimestamp: float,
};

type message =
  | ChannelInfoMessage(channelInfo)
  | ChannelInfoListMessage(list(channelInfo))
  | TextMessageMessage(textMessage)
  | TextMessageListMessage(list(textMessage));