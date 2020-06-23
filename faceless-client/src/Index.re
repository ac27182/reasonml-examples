[%bs.raw {|require("./css/index.css")|}];

open FacelessCore;

// store userId in local storage
// retrive on load if exists
let userId: string = Uuid.v4();

ReactDOMRe.renderToElementWithId(<App userId />, "root");