import express, { Express } from "express";
import { State, Channel } from "./interfaces";

const state: State = {
  channels: []
};
const serverPort: number = 3000;
const redisPort: number = 6379;
const app: Express = express();

const getPublicChannels = (state: State): Array<Channel> => state.channels;

app.get("/publicChannels", (req, res) => res.json(getPublicChannels(state)));
app.listen(serverPort);
