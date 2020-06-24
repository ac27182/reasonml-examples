open FacelessCore.Types;
open FacelessCore.Webapi;

// app actions
type appAction =
  | EnterChannel(channelInfo)
  | LeaveChannnel
  | AddWsGlobalClient(option(WebSocket.t))
  | AddWsChannelClient(option(WebSocket.t))
  | AppendTextMessage(textMessage)
  | AppendChannelInfo(channelInfo)
  | ToggleNavigationPannel
  | PopulateChannels(list(channelInfo))
  | PopulateTextMessages(option(list(textMessage)))
  | HandleNameInputChange(string)
  | SetName(string);

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

// general
let makeWsUrl = (port: int, path: string) =>
  "ws://localhost:" ++ (port |> string_of_int) ++ path;