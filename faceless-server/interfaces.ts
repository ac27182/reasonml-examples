import { RedisClient } from "redis";

export interface State {
  channels: Array<Channel>;
}

export interface User {
  userId: string;
  client: RedisClient;
  creationTime: number;
}

export interface Channel {
  name: string;
  password: string;
  protected: boolean;
  hidden: boolean;
  users: Array<User>;
  creationTime: number;
}

export interface Message {
  messageId: string;
  userId: String;
  references: Array<string>;
  creationTime: number;
}
