open Relude_Globals;

let toAsync = (partialFunction: Redis.callback('a) => 'u): IO.t('a, 'e) => {
  IO.async(onDone =>
    (
      (e, v) =>
        switch (e |> Js.Null.toOption) {
        | Some(e) => Error(e) |> onDone
        | None => Ok(v) |> onDone
        }
    )
    |> partialFunction
  );
};

let ioRun = (io: IO.t('a, 'e)): unit =>
  io
  |> IO.unsafeRunAsync(
       fun
       | Ok(v) => v |> Js.log
       | Error(e) => e |> Js.log,
     );