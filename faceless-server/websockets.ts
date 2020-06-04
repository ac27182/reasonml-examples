import { uuid } from "uuidv4";
import WebSocket from "ws";

export const genConnection = () => {
  const sessionid = uuid();
  const connection = new WebSocket("ws://localhost:3001");

  const connectionf = (websocket: WebSocket) =>
    console.log(`user: ${sessionid} conected`);

  connection.on("open", connectionf);

  connection;
};

export const genWss = (port: number) => {
  const wss = new WebSocket.Server({ port });

  const connectionf = (wss: WebSocket): void =>
    console.log("connection to wss");

  wss.on("connection", connectionf);

  wss;
};
