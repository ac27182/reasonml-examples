open Utils;
open Jest;
open Expect;
open Types;

describe("pathParser", () => {
  test("", () => {
    let actualResult = "" |> pathParser;
    let expectedResult = GlobalChannel;
    expect(actualResult) |> toBe(expectedResult);
  });

  test("/", () => {
    let actualResult = "/" |> pathParser;
    let expectedResult = GlobalChannel;
    expect(actualResult) |> toBe(expectedResult);
  });

  test("/xxxx", () => {
    let actualResult = "/xxxx" |> pathParser;
    let expectedResult = MessageChannel("xxxx");
    expect(actualResult) |> toEqual(expectedResult);
  });

  test("/xxxx/yyyy/zzzz", () => {
    let actualResult = "/xxxx/yyyy/zzzz" |> pathParser;
    let expectedResult = MessageChannel("xxxx");
    expect(actualResult) |> toEqual(expectedResult);
  });
});