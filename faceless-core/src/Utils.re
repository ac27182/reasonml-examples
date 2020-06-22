open Relude_Globals;
open Types;

let toAsyncIo = (partialFunction: Redis.callback('a) => 'u): IO.t('a, 'e) => {
  IO.async(onDone =>
    (
      (error, data) =>
        switch (error |> Js.Null.toOption) {
        | Some(e) => Error(e) |> onDone
        | None => Ok(data) |> onDone
        }
    )
    |> partialFunction
  );
};

let runAndLogIo = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(v) => v |> Js.log
       | Error(e) => e |> Js.log,
     );

let option_of_string = (str: string): option(string) =>
  switch (str) {
  | "" => None
  | v => Some(v)
  };

let string_of_option = (opt: option(string)): string =>
  switch (opt) {
  | None => ""
  | Some(p) => p
  };

// let genTextMessage = () => ();
// let genChannelInfo = () => ();
// let genUser = () => ();
// let genMessage = () => ();

let logIo = (message: string) => IO.suspendWithVoid(() => message |> Js.log);

let pathParser = (path: string): channelMultiplexer =>
  switch (path |> String.splitList(~delimiter="/")) {
  | [_, id, ..._] =>
    switch (id) {
    | "global" => GlobalChannel
    | "" => GlobalChannel
    | _ => MessageChannel(id)
    }
  | _ => GlobalChannel
  };

// NB
// int of string can still thow an exception if it can't be converted correctly
// need to make int of string return an option(int) for referential transparency
let parseProcess = (process: Node_process.t): environment => {
  let dict: Js_dict.t(string) = process##env;

  let redisPort: option(int) =
    dict->Js.Dict.get("REDIS_PORT") |> Option.map(s => s |> int_of_string);

  let webSocketPort: option(int) =
    dict->Js.Dict.get("WEBSOCKET_PORT") |> Option.map(s => s |> int_of_string);

  let redisHost: option(string) = dict->Js.Dict.get("REDIS_HOST");

  {redisPort, redisHost, webSocketPort};
};

let defaultHandler = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(_) => "success" |> Js.log
       | Error(_) => "an error occurred" |> Js.log,
     );