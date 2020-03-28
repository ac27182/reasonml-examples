open ReactEvent;

// open Belt;

let items0: list(string) = ["do washing", "clean room", "work on project"];

let idGen = (): string => {
  Random.self_init();
  max_int / 2 |> Random.int |> string_of_int;
};

[@react.component]
let make = () => {
  let initialItem: TodoTypes.item = {
    description: "",
    complete: false,
    id: "",
  };

  let initialState: TodoTypes.state = {items: [], newItem: initialItem};

  let reducer =
      (state: TodoTypes.state, action: TodoTypes.action): TodoTypes.state =>
    TodoTypes.(
      switch (action) {
      | EditNewItem(description) => {
          ...state,
          newItem: {
            description,
            complete: false,
            id: idGen(),
          },
        }
      | SaveNewItem => {
          newItem: initialItem,
          items: List.cons(state.newItem, state.items),
        }
      | DeleteItem(item) =>
        let xs = Belt_List.keep(state.items, i => i !== item);
        {...state, items: xs};
      | ToggleComplete(item) =>
        let xs =
          state.items
          |> List.map(i =>
               i.id == item.id ? {...i, complete: !i.complete} : i
             );
        {...state, items: xs};
      }
    );

  let (state: TodoTypes.state, dispatch) =
    React.useReducer(reducer, initialState);

  let toggleDelete = (item: TodoTypes.item) => dispatch(DeleteItem(item));

  let toggleComplete = (item: TodoTypes.item) =>
    dispatch(ToggleComplete(item));

  let onEdit = event => {
    let s: string = event->Form.target##value;
    dispatch(TodoTypes.EditNewItem(s));
  };

  let onSave = _ => dispatch(SaveNewItem);

  <div>
    <AddItem description={state.newItem.description} onEdit onSave />
    <Items items={state.items} toggleDelete toggleComplete />
    <div>
      <button> {"all" |> React.string} </button>
      <button> {"complete" |> React.string} </button>
      <button> {"incomplete" |> React.string} </button>
      <input placeholder="fuzzy search" />
      <button> {"clear" |> React.string} </button>
      <button> {"clear all" |> React.string} </button>
    </div>
  </div>;
};