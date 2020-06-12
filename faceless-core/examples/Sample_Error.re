// // BEGINNING
// open Utils;
// open Ws;
// open Ws.Server;
// open Relude_Globals;
// let f0 = () => Ws.Server.make({port: 3000});
// // Error: The type of this expression,
// //        Relude_Globals.IO.t(FacelessCore.Ws.Server.t, '_weak1),
// //        contains type variables that cannot be generalized
// let io1 = f0 |> IO.suspend;
// // END
/* --- */