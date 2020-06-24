module WebSocket = {
  type t;
  type messageEvent = {
    data: string,
    timeStamp: int,
    origin: string,
  };

  [@bs.new] external wsbc: string => t = "WebSocket";
  [@bs.send] external send: (t, ~message: string) => unit = "send";
  [@bs.send] external close: t => unit = "close";

  // [@bs.set] external onopen
  // [@bs.set] external onerror
  // [@bs.set] external onclose
  [@bs.set]
  external onmessage: (t, ~messageHandler: messageEvent => unit) => unit =
    "onmessage";
};