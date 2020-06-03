import redis, { RedisClient } from "redis";

// const subscriber0 = redis.createClient();
// const subscriber1 = redis.createClient();
// const publisher0 = redis.createClient();

// const channel = "channel0";

// subscriber0.on("subscribe", (channel, count) => {
//   console.log("> subscriber0 joining", channel);
//   publisher0.publish(channel, "message0");
//   publisher0.publish(channel, "message1");
//   publisher0.publish(channel, "message2");
// });

// subscriber1.on("subscribe", (channel, count) => {
//   console.log("> subscriber1 joining", channel);
// });

// subscriber1.on("message", (c, m) => {
//   console.log(m);
// });

// const shutdown = () => {
//   subscriber0.unsubscribe();
//   subscriber1.unsubscribe();
//   subscriber0.quit();
//   subscriber1.quit();
//   publisher0.quit();
// };

// subscriber0.subscribe(channel);

// setTimeout(() => {
//   subscriber1.subscribe(channel);
// }, 5000);

// shutdown();

const client = redis.createClient();

// client.set("channel/0", "message0");
// client.set("channel/1", "message1");
// client.set("channel/2", "message2");

// client.scan("channel", (e, d) => (e ? console.log(e) : console.log(d)));

interface message {
  id: String;
  payload: String;
  timestamp: number;
}

const genMessage = (): string =>
  JSON.stringify({
    id: "xxx",
    payload: "hello",
    timestamp: Date.now()
  });

// const m: message = { id: "x", payload: "hello" };
// const s: string = JSON.stringify(m);

client.hset("channel0", "0", "message0");
client.hset("channel0", "1", "message1");
client.hset("channel0", "2", "message2");

client.lpush("list0", genMessage());
client.lpush("list0", genMessage());
client.lpush("list0", genMessage());

client.lrange("list0", 0, -1, (e, d) => console.log(d));
client.del("list0");

// client.multi().rpush;

// blindings to make

// RedisClient
// lpush
// lrange
// del
// publish
// subscribe
// quit

// Multi
// rpush

interface State {
  channels: Array<Channel>;
}

interface User {
  userId: string;
  client: RedisClient;
  creationTime: number;
}

interface Channel {
  name: string;
  password: string;
  protected: boolean;
  hidden: boolean;
  users: Array<User>;
  creationTime: number;
}

interface Message {
  messageId: string;
  userId: String;
  references: Array<string>;
  creationTime: string;
}
