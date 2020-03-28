type item = {
  description: string,
  complete: bool,
  id: string,
};

type state = {
  items: list(item),
  newItem: item,
};

type action =
  | EditNewItem(string)
  | SaveNewItem
  | DeleteItem(item)
  | ToggleComplete(item);