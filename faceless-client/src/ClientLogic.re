open FacelessCore.Types;
open FacelessCore.Webapi;

// app actions
type appAction =
  // component state actions
  | DoNothing // testing out the context provider
  | EnterChannel(channelInfo)
  | SetCurrentChannel(option(channelInfo))
  | LeaveChannnel
  | AddWsGlobalClient(option(WebSocket.t))
  | AddWsChannelClient(option(WebSocket.t))
  | AppendTextMessage(textMessage)
  | AppendChannelInfo(channelInfo)
  | ToggleNavigationPannel
  | SwitchChannel(channelInfo)
  | DisconnectChannel(channelInfo)
  | PopulateChannels(list(channelInfo)) /* | ToggleNavigationPanne*/ /* | UpdateConnection*/
  | PopulateTextMessages(option(list(textMessage)));
// | AddNewChannel
// | AddSubscribedChannel
// | RemoveSubscribedChannel

// app functions
let enterChannel =
    (channelInfo: channelInfo, dispatch: appAction => unit): unit =>
  EnterChannel(channelInfo) |> dispatch;

let leaveChannel = (dispatch: appAction => unit): unit =>
  LeaveChannnel |> dispatch;

// navigationPannelActions

type navigationPannelAction =
  | HandleNewChannelDisplayNameInput(string)
  | HandleNewChannelPasswordInput(string)
  | HandleNewChannelHiddenToggle
  | HandleCreateNewChannel
  | ToggleCollapse
  | FilterChannels(string)
  | ClearFilter;

// messageInputBarActitions
type messageInputBarAction =
  | InputMessage(string)
  | SendMessage;