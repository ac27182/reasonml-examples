type t;

[@bs.module] external make: t = "uuid";

// no frills uuid, the other one broke.
[@bs.module "uuid"] external v4: unit => string = "v4";