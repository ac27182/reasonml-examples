open Relude_Globals;

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