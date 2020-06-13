module WebSocket = {
  type t;
  type messageEvent = {
    data: string,
    timeStamp: int,
    origin: string,
  };

  [@bs.new] external wsbc: string => t = "WebSocket";
  [@bs.send] external send: (t, ~message: string) => unit = "send";

  // js classes are just fancy objects
  // to do the message property we just have to the following

  // [@bs.set] external onopen
  // [@bs.set] external onerror
  // [@bs.set] external onclose
  [@bs.set]
  external onmessage: (t, ~f: messageEvent => unit) => unit = "onmessage";
};