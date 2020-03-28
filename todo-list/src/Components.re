// open ReactEvent;
 // open ReactDOMRe;
 // module Box2 = {
 //   // examples of typed styles
 //   let s0 = Style.make(~color="#444444", ~fontSize="68px", ());
 //   let s1 =
 //     Style.make(~border="solid", ~borderWidth="1px", ~padding="10px", ());
 //   type state = {name: string};
 //   let initialState = {name: ""};
 //   let reducer = (_, action: string) => {name: action};
 //   [@react.component]
 //   let make = (): ReasonReact.reactElement => {
 //     let (state, dispatch) = React.useReducer(reducer, initialState);
 //     <div style=s1>
 //       <input
 //         onChange={event => event->Form.target##value |> dispatch}
 //         value={state.name}
 //       />
 //       <button onClick={_ => "" |> dispatch}>
 //         {"clear" |> React.string}
 //       </button>
 //       <div> {state.name |> React.string} </div>
 //     </div>;
 //   };
 /* }*/