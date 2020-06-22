[@react.component]
let make = (~name: string) => {
  <div className="spinner-container">
    <div className="name-prompt">
      {"my name is " ++ name ++ "_" |> React.string}
    </div>
    <div className="centered-item">
      <div className="elipse rotation1" />
      <div className="elipse rotation2" />
      <div className="elipse rotation3" />
      <div className="elipse rotation4" />
      <div className="elipse rotation5" />
      <div className="elipse rotation6" />
      <div className="elipse rotation7" />
      <input placeholder="enter name..." className="name-input" maxLength=10 />
      <button> {">>=" |> React.string} </button>
    </div>
  </div>;
};