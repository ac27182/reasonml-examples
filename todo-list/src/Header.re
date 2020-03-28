[@react.component]
let make = () => {
  <div className="header">
    <a className="logo"> {"todo" |> React.string} </a>
  </div>;
};