type t;

[@bs.module] external make: t = "uuid";

// no frills uuid
[@bs.module "uuid"] external v4: unit => string = "v4";