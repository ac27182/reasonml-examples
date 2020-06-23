open Types;
open Jest;
open Expect;

// might as well save the hassle of having encoder / decoder tests and write generic testing function
let roundTripAssertion =
    (
      encoder: 'a => Js.Json.t,
      decoder: Js.Json.t => 'a,
      data: 'a,
      json: Js.Json.t,
    ) => {
  let encodedData = data |> encoder;
  let decodedData = encodedData |> decoder;

  let decodedJson = json |> decoder;
  let encodedJson = decodedJson |> encoder;

  let expectedTuple = (encodedData, decodedData, encodedJson, decodedJson);
  let actualTuple = (json, data, json, data);

  expect(expectedTuple) |> toEqual(actualTuple);
};

describe("channelInfo", () => {
  test("successful json encoding and decoding", () => {
    let testJson: Js.Json.t =
      {js|
      {
        "displayName": "global",
        "id": "xxxx-xxxx-xxxx-xxxx",
        "hidden": false,
        "password": "",
        "creationTimestamp": "1"
      }
    |js}
      |> Js.Json.parseExn;

    let testData = {
      displayName: "global",
      id: "xxxx-xxxx-xxxx-xxxx",
      hidden: false,
      password: None,
      creationTimestamp: 1.0,
    };

    roundTripAssertion(
      Encoders.encodeChannelInfo,
      Decoders.decodeChannelInfo,
      testData,
      testJson,
    );
  });

  describe("textMessage", () => {
    test("successful json encoding and decoding", () => {
      let testJson: Js.Json.t =
        {js|
      {
        "id": "xxxx-xxxx-xxxx-xxxx",
        "authorId": "yyyy-yyyy-yyyy-yyyy",
        "data": "hello world",
        "creationTimestamp": "2",
        "authorName": "mrRobot"
      }
    |js}
        |> Js.Json.parseExn;

      let testData = {
        id: "xxxx-xxxx-xxxx-xxxx",
        authorId: "yyyy-yyyy-yyyy-yyyy",
        data: "hello world",
        creationTimestamp: 2.0,
        authorName: "mrRobot",
      };

      roundTripAssertion(
        Encoders.encodeTextMessage,
        Decoders.decodeTextMessage,
        testData,
        testJson,
      );
    })
  });

  describe("message", () => {
    describe("channelInfoMessage", () => {
      test("successful json encoding and decoding", () => {
        let testJson: Js.Json.t =
          {js|
            {
              "payloadType": "channelInfo",
              "payload": {
                "displayName": "global",
                "id": "xxxx-xxxx-xxxx-xxxx",
                "hidden": false,
                "password": "",
                "creationTimestamp": "1"
              }
            }
          |js}
          |> Js.Json.parseExn;

        let testData =
          ChannelInfoMessage({
            displayName: "global",
            id: "xxxx-xxxx-xxxx-xxxx",
            hidden: false,
            password: None,
            creationTimestamp: 1.0,
          });

        roundTripAssertion(
          Encoders.encodeMessage,
          Decoders.decodeMessage,
          testData,
          testJson,
        );
      })
    });

    describe("channelInfoListMessage", () => {
      test("successful json encoding and decoding", () => {
        let testJson: Js.Json.t =
          {js|
          {
            "payloadType": "channelInfoList",
            "payload": [
              {
                "displayName": "global",
                "id": "xxxx-xxxx-xxxx-xxxx",
                "hidden": false,
                "password": "",
                "creationTimestamp": "1"
              },
              {
                "displayName": "global0",
                "id": "xxxx-xxxx-xxxx-yyyy",
                "hidden": false,
                "password": "",
                "creationTimestamp": "1"
              }
            ]
          }
          |js}
          |> Js.Json.parseExn;

        let testData =
          ChannelInfoListMessage([
            {
              displayName: "global",
              id: "xxxx-xxxx-xxxx-xxxx",
              hidden: false,
              password: None,
              creationTimestamp: 1.0,
            },
            {
              displayName: "global0",
              id: "xxxx-xxxx-xxxx-yyyy",
              hidden: false,
              password: None,
              creationTimestamp: 1.0,
            },
          ]);

        roundTripAssertion(
          Encoders.encodeMessage,
          Decoders.decodeMessage,
          testData,
          testJson,
        );
      })
    });
  });
});