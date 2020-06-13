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

type createChannelRequest = {
  displayName: string,
  hidden: bool,
  password: option(string),
};

type user = {
  id: string,
  creationTimestamp: float,
};

type message =
  | CreateChannelRequestMessage(createChannelRequest)
  | ChannelInfoMessage(channelInfo)
  | ChannelInfoListMessage(list(channelInfo))
  | TextMessageMessage(textMessage)
  | TextMessageListMessage(list(textMessage));