module Types = {
  type user = {
    userId: string,
    client: string,
    creationTime: int,
  };

  type dynamicChannel = {
    name: string,
    password: string,
    protected: bool,
    hidden: bool,
    users: list(user),
    creationTime: int,
  };

  type channelName =
    | Channel0
    | Channel1
    | Channel2
    | Channel3;

  type lockedChannel = {
    name: channelName,
    password: string,
    protected: bool,
    hidden: bool,
    users: list(user),
    creationTime: int,
  };

  type message = {
    messageId: string,
    userId: string,
    references: list(string),
    creationTime: int,
  };

  type state = {channels: list(lockedChannel)};
};