open ReactEvent;

let idGen = (): string => {
  Random.self_init();
  max_int / 2 |> Random.int |> string_of_int;
};

let initialItem: TodoTypes.item = {description: "", complete: false, id: ""};

let initialState: TodoTypes.state = {
  items:
    Belt_List.make(
      100,
      {description: "do this", complete: false, id: idGen()}: TodoTypes.item,
    ),
  newItem: initialItem,
  filter: TodoTypes.All,
  filterQuery: "",
};

[@react.component]
let make = () => {
  let reducer =
      (state: TodoTypes.state, action: TodoTypes.action): TodoTypes.state =>
    TodoTypes.(
      switch (action) {
      | EditNewItem(description) =>
        let newItem = {description, complete: false, id: idGen()};
        {...state, newItem};

      | SaveNewItem =>
        let items = List.cons(state.newItem, state.items);
        {...state, newItem: initialItem, items};

      | DeleteItem(item) =>
        let items = Belt_List.keep(state.items, i => i !== item);
        {...state, items};

      | ToggleComplete(item) =>
        let items =
          state.items
          |> List.map(i =>
               i.id == item.id ? {...i, complete: !i.complete} : i
             );
        {...state, items};

      | Filter(f) => {...state, filter: f}

      | Clear => {...state, items: []}

      | EditFilterQuery(q) => {...state, filterQuery: q}
      }
    );

  let (state: TodoTypes.state, dispatch) =
    React.useReducer(reducer, initialState);

  // we could perhaps partially apply these functions to make them referentially transparent?
  // indeed a lot of these functions do the same shit
  // we could make this more readable ang generic
  let toggleDelete = (item: TodoTypes.item): unit =>
    dispatch(DeleteItem(item));

  let toggleComplete = (item: TodoTypes.item): unit =>
    dispatch(ToggleComplete(item));

  let toggleFilter = (filter: TodoTypes.filter): unit =>
    dispatch(Filter(filter));

  let onEdit = (event: ReactEvent.Form.t): unit => {
    let s: string = event->Form.target##value;
    dispatch(TodoTypes.EditNewItem(s));
  };

  let editFilterQuery = (event: ReactEvent.Form.t): unit => {
    let s: string = event->Form.target##value;
    dispatch(TodoTypes.EditFilterQuery(s));
  };
  let clearFilterQuery = _: unit => dispatch(TodoTypes.EditFilterQuery(""));

  let toggleClear = _: unit => dispatch(Clear);

  let onSave = _: unit => dispatch(SaveNewItem);

  let customFilter = (items: list(TodoTypes.item)): list(TodoTypes.item) =>
    items
    |> List.filter(item =>
         TodoTypes.(
           switch (state.filter) {
           | Complete => item.complete
           | Incomplete => !item.complete
           | All => true
           }
         )
         && item.description
         |> Js.String.includes(state.filterQuery)
       );

  <div className="master-container">
    <Header />
    <AddItem description={state.newItem.description} onEdit onSave />
    <Filters
      toggleFilter
      toggleClear
      editFilterQuery
      clearFilterQuery
      filterQuery={state.filterQuery}
    />
    <div className="items-container">
      {state.items
       |> customFilter
       |> List.map(item => <Item item toggleDelete toggleComplete />)
       |> Array.of_list
       |> React.array}
    </div>
  </div>;
};