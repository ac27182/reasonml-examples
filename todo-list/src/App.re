open ReactEvent;

Random.init;
// open Belt;

let items0: list(string) = ["do washing", "clean room", "work on project"];

[@react.component]
let make = () => {
  let initialItem: TodoTypes.item = {
    description: "",
    complete: false,
    id: "",
  };

  let initialState: TodoTypes.state = {items: [], newItem: initialItem};

  let initialState0: TodoTypes.state = {
    items: [
      {description: "do laundry", complete: false, id: "item-0"},
      {description: "do work", complete: false, id: "item-1"},
      {description: "make dinner", complete: false, id: "item-2"},
    ],
    newItem: initialItem,
  };

  let reducer =
      (state: TodoTypes.state, action: TodoTypes.action): TodoTypes.state =>
    TodoTypes.(
      switch (action) {
      | EditNewItem(description) => {
          ...state,
          newItem: {
            description,
            complete: false,
            id: "item-" ++ (state.items |> List.length |> string_of_int),
          },
        }
      | SaveNewItem => {
          newItem: initialItem,
          items: List.cons(state.newItem, state.items),
        }
      | DeleteItem(id) => {
          ...state,
          items: state.items |> List.filter(item => item.id != id),
        }
      }
    );

  let (state: TodoTypes.state, dispatch) =
    React.useReducer(reducer, initialState0);

  state |> Js.log;

  let toggleDelete = (id: string) => dispatch(DeleteItem(id));

  <div>
    <div>
      <input
        value={state.newItem.description}
        onChange={event => {
          let s: string = event->Form.target##value;
          dispatch(TodoTypes.EditNewItem(s));
        }}
        placeholder="item description"
      />
      <button onClick={_ => dispatch(SaveNewItem)}>
        {"add item" |> React.string}
      </button>
    </div>
    <Items items={state.items} toggleDelete />
  </div>;
};