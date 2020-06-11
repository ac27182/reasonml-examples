type textMessage = {
  id: string,
  authorId: string,
  data: string,
  creationTimestamp: int,
};

type channelInfo = {
  id: string,
  displayName: string,
  hidden: bool,
  password: option(string),
  creationTimestamp: int,
};

type user = {
  id: string,
  creationTimestamp: int,
};

type message =
  | ChannelInfoMessage(channelInfo)
  | ChannelInfoListMessage(list(channelInfo))
  | TextMessageMessage(textMessage)
  | TextMessageListMessage(list(textMessage));