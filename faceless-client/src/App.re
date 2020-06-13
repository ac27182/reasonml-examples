[%bs.raw {|require("./app.css")|}];

[@react.component]
let make = () =>
  <div className="app-container">
    <Header />
    <Header />
    <NavigationPannel />
    // <Spinner />
    <ChannelPannel />
  </div>;