import ws from "ws";
import url from "url";

// type R = Record<string, WebSocket>;
const clientPool = {};

interface PathInfo {
  channelName: string;
  clientName: string;
}

const urlParser = (s: string): PathInfo => {
  const arr = s.split("/");
  return {
    channelName: arr[1],
    clientName: arr[2]
  };
};

// create server
const server = new ws.Server({ port: 3000 });

// create connection listner
server.on("connection", (connectedClient: WebSocket, req: Request) => {
  const pathInfo = urlParser(req.url);

  // add client connection to client pool
  clientPool[pathInfo.clientName] = connectedClient;
  console.log(pathInfo.clientName, "connected");

  // todo
  // on connection get message history from redis
  // send to client
  // const messageHistory = [];
  // webSocket.send(messageHistory);

  // on recepit of a message
  // first persist to redis (todo)
  // on success broadcast message to entire client pool
  connectedClient.onmessage = (event: MessageEvent) => {
    Object.values(clientPool).map((w: WebSocket) => w.send(event.data));
  };
});

// test
const client0 = new ws("ws://localhost:3000/channel0/client0");
const client1 = new ws("ws://localhost:3000/channel0/client1");

client0.on("message", m => console.log("recieved message:", m));
client1.on("message", m => console.log("recieved message:", m));

clientPool["client0"] = client0;
clientPool["client1"] = client1;

setTimeout(() => {
  client1.send("hello");
  client1.send("world");
}, 500);
