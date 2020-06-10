open Jest;
open Expect;
open FacelessCore;
open Relude_Globals;
open Controller;
describe("tests", () => {
  testAsync("testing how to test...", onDone =>
    onDone(expect(1) |> toEqual(1))
  )
});

// dumb tests
describe("inegration tests", () => {
  testAsync("poke redis", onDone => {
    let key = "channels";
    let subscriber = Redis.createClient({port: 6379});

    Js.log("....");
    subscriber
    |> Redis_IO.hgetall(~key)
    |> IO.unsafeRunAsync(
         fun
         | Ok(dict) => {
             let channelInfoList = dict |> dictToChannelInfoList;
             let message = Types.ChannelInfoListMessage(channelInfoList);
             let encodedMessage = message |> Encoders.encodeMessage;
             encodedMessage |> Js.log;
             onDone(expect(1) |> toBe(1));
           }
         | Error(_) => Js.log("e"),
       );
  })
});