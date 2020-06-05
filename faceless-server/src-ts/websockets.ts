import { uuid } from "uuidv4";
import WebSocket from "ws";
// const ws = require("ws");

export const genConnection = () => {
  const sessionid = uuid();
  const connection = new WebSocket("ws://localhost:3001");

  const connectionf = (websocket: WebSocket) =>
    console.log(`user: ${sessionid} conected`);

  connection.on("open", connectionf);

  connection.send;
};
const connection = new WebSocket("ws://localhost:3001");

export const genWss = (port: number) => {
  const wss = new WebSocket.Server({ port });
  // wss.on("connection",)

  // const connectionf = (ws: WebSocket): void => {
  //   console.log("connection to wss");
  // };

  // wss.on("connection", connectionf);
  wss.on("connection", (ws: WebSocket) => {
    ws.on("message", x => {
      // wss.em;
    });
  });
  // wss.on("")
};
