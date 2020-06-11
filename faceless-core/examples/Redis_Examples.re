open Redis;

let client = createClient({port: 6379});

let key = "channel0";

let callback = (_, data) => data |> Js.log;

// get all items from list
client->lrange(~key, ~start=0, ~stop=-1, callback);

// push item to list
// client->lpush(~key, ~item="value", callback);

// remove item from list
client->lpop(~key, callback);