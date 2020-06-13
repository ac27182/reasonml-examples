[@react.component]
let make = () =>
  <div className="message-input-bar-container">
    <input />
    <button> {">>=" |> React.string} </button>
  </div>;