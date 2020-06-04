import redis, { RedisClient } from 'redis'
import { uuid } from 'uuidv4'

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

// const client = redis.createClient()

// client.set("channel/0", "message0");
// client.set("channel/1", "message1");
// client.set("channel/2", "message2");

// client.scan("channel", (e, d) => (e ? console.log(e) : console.log(d)));

// const m: message = { id: "x", payload: "hello" };
// const s: string = JSON.stringify(m);

// client.hset('channel0', '0', 'message0')
// client.hset('channel0', '1', 'message1')
// client.hset('channel0', '2', 'message2')

// client.lpush('list0', genMessage())
// client.lpush('list0', genMessage())
// client.lpush('list0', genMessage())

// client.lrange('list0', 0, -1, (e, d) => console.log(d))
// client.del('list0')

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
	channels: Array<Channel>
}

interface User {
	userId: string
	client: RedisClient
	creationTime: number
}

interface Channel {
	name: string
	password: string
	protected: boolean
	hidden: boolean
	users: Array<User>
	creationTime: number
}

interface Message {
	messageId: string
	userId: String
	references: Array<string>
	creationTime: number
}

const createNewChannel = (): Channel => ({
	name: uuid(),
	password: '',
	protected: false,
	hidden: false,
	users: [],
	creationTime: Date.now(),
})

const createMessage = (): Message => ({
	messageId: uuid(),
	userId: uuid(),
	references: [],
	creationTime: Date.now(),
})

const createUser = (): User => ({
	userId: uuid(),
	client: redis.createClient(),
	creationTime: Date.now(),
})

const initialState = (): State => ({
	channels: [],
})

const addNewChannel = (
	currentState: State,
	channel: Channel,
): [State, State] => [
	currentState,
	{
		channels: currentState.channels.concat([channel]),
	},
]

const subscribeUser = (channel: Channel, user: User): Channel => {
	return {
		...channel,
		users: channel.users.concat([user]),
	}
}

const channel = createNewChannel()
console.log(channel)
const user = createUser()
const channel0 = subscribeUser(channel, user)
console.log(channel0)

import express from 'express'
import http from 'http'
import socket from 'socket.io'
// import WebSocket from 'ws'
import websocket from 'websocket'

const server = http.createServer()
const wss = new websocket.server({ httpServer: server })

wss.on('connect', () => console.log('connected'))

server.listen('3000')
// const app = express()
// const server = http.createServer(app)
// const io = socket(server)

// io.on('connection', () => console.log('connected'))

// server.listen(3000)

// const websocket = new WebSocket.Server({
// 	port: 3000,
// })

// const server = http.createServer()

// websocket.on('connection', () => console.log('connected'))

// websocket.onopen = (e: Event) => console.log('open')
