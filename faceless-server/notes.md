# rest api

- post createChannel

# websocket

- register client with server
- on connection get all channels

- register client with channel
- on register get all channel messages

- channel added
- on message channel added
- send clients new channel

- channel removed
- on message channel removed
- inform clients to remove the defunct channel

- connection broken
- on connection broken
- unregister client from clientPool
- unregister client from all channels

- new message sent
- on message newMessage
- update channel messages in state
- send new message to client

# redis pub sub understanding

1. client makes web socket connection
2. client is subscribed to redis pub sub
3. channel is created
4. all clients are given to new channel
