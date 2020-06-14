open FacelessCore.Types;

// app actions
type appAction =
  // component state actions
  | DoNothing //testing out the context provider
  | EnterChannel(channelInfo)
  | LeaveChannnel
  | SwitchChannel(channelInfo)
  | DisconnectChannel(channelInfo)
  | PopulateChannels(list(channelInfo)) /* | ToggleNavigationPanne*/ /* | UpdateConnection*/;
// | AddNewChannel
// | AddSubscribedChannel
// | RemoveSubscribedChannel

// passable functions
let enterChannel =
    (channelInfo: channelInfo, dispatch: appAction => unit): unit =>
  EnterChannel(channelInfo) |> dispatch;

let leaveChannel = (dispatch: appAction => unit): unit =>
  LeaveChannnel |> dispatch;

// navigationPannelActions

type navigationPannelAction =
  | FilterChannels(string)
  | ClearFilter;