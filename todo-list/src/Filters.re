[@react.component]
let make =
    (
      ~toggleFilter,
      ~toggleClear,
      ~editFilterQuery,
      ~filterQuery,
      ~clearFilterQuery,
    ) =>
  <div>
    <div className="filter-query-bar">
      <input
        onChange=editFilterQuery
        value=filterQuery
        placeholder="filterquery"
      />
      <button onClick=clearFilterQuery> {"clear" |> React.string} </button>
    </div>
    <div className="filter-bar">
      <button onClick={_ => TodoTypes.All |> toggleFilter}>
        {"all" |> React.string}
      </button>
      <button onClick={_ => TodoTypes.Complete |> toggleFilter}>
        {"complete" |> React.string}
      </button>
      <button onClick={_ => TodoTypes.Incomplete |> toggleFilter}>
        {"incomplete" |> React.string}
      </button>
      <button onClick=toggleClear> {"clear all" |> React.string} </button>
    </div>
  </div>;