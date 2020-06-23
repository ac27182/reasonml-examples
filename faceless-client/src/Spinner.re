[@react.component]
let make =
    (
      ~name: string,
      ~nameInput: string,
      ~handleNameInputChange: ReactEvent.Form.t => unit,
      ~handleSetName: ReactEvent.Mouse.t => unit,
    ) => {
  // let (state, dispatch) = React.useReducer(reducer, initialState);
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
      <div className="container">
        <input
          value=nameInput
          onChange=handleNameInputChange
          placeholder="enter name..."
          className="name-input"
          maxLength=10
        />
        <button className="name-button" onClick=handleSetName>
          {">>=" |> React.string}
        </button>
      </div>
    </div>
  </div>;
};