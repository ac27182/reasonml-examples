type item = {
  description: string,
  complete: bool,
  id: string,
};

type filter =
  | All
  | Complete
  | Incomplete;

type action =
  | EditNewItem(string)
  | SaveNewItem
  | DeleteItem(item)
  | ToggleComplete(item)
  | Filter(filter)
  | Clear
  | EditFilterQuery(string);

type state = {
  items: list(item),
  newItem: item,
  filter,
  filterQuery: string,
};