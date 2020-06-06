open Relude_Globals;

let l = ["x", "y", "z"];

let s = l |> List.find(i => i == "a");

switch (s) {
| Some(x) => x |> Js.log
| None => "none" |> Js.log
};