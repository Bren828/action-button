# action-button

Action button to confirm for SAMP

Подтверждение действий по нажатию кнопки.\
Для подтверждения нажмите Y или для отмены N

## Reference
* [Installation](https://github.com/Bren828/action-button#installation)
* [Example](https://github.com/Bren828/action-button#example)
* [Functions](https://github.com/Bren828/action-button#functions)
* [Definition](https://github.com/Bren828/action-button#definition)


## Installation

Include in your code and begin using the library:
```pawn
#include <action-button>
```

## Example
```pawn
CMD:test(playerid, params[])
{
    new inviteid;

    if(sscanf(params, "d", inviteid)) 
        return SendClientMessage(playerid, -1, "/test [ID]");

    if(!IsPlayerConnected(inviteid)) 
        return SendClientMessage(playerid,-1, "INVALID");

    CreateActionButton("give_flowers", playerid, inviteid, "You offered flowers", "You were offered flowers", KEY_YES, KEY_NO);

    return 1;
}

ActionButton:give_flowers(playerid, inviteid, response)
{
    if(!response) return 1;

    GivePlayerWeapon(playerid, 14, 1);
    return 1;
}
```

## Functions

#### CreateActionButton(const function[], playerid, inviteid, text_creator[] = "", text_invited[] = "", accept_button = KEY_YES, decline_button = KEY_NO, dialog_caption[] = "", dialog_button1[] = "", dialog_button2[] = "")
> Create action button
> * `function[]` - Function name
> * `playerid` - The ID of the player
> * `inviteid` - Invited Player ID
> * `text_creator[]` - Text in chat
> * `text_invited[]` - Text in chat for invitee
> * `accept_button` - Accept button
> * `decline_button` - Decline button
> * `dialog_caption[]` - **Use dialog.** The title at the top of the dialog
> * `dialog_button1[]` - Dialog button 1
> * `dialog_button2[]` - Dialog button 2
> * Returns (0) on failure or (1) on success

#### CancelActionButton(playerid)
> Cancel invitation
> * `playerid` - The ID of the player
> * Returns (0) on failure or (1) on success

#### GetActionButtonInviteid(playerid)
> Get invited player ID
> * `playerid` - The ID of the player
> * Returns (`INVALID_PLAYER_ID`) on failure or (`ID`) on success

#### SetActionButtonTime(playerid, time)
> Set timeout time
> * `playerid` - The ID of the player
> * `time` - Time

#### GetActionButtonTime(playerid)
> Get timeout time
> * `playerid` - The ID of the player
> * Returns The time in seconds

#### SetActionButtonInviteYourself(playerid, bool:enable)
> Allow me to invite myself
> * `playerid` - The ID of the player
> * `bool:enable` - Status

#### SetActionButtonDistance(playerid, Float:distance)
> Set the distance to the player
> * `playerid` - The ID of the player
> * `Float:distance` - Distance


## Definition

```pawn
#define AB_INVITE_TIMEOUT_TIME   30
#define AB_MAX_DISTANCE   3.0
#define AB_DIALOG_ID   16743		
#define AB_COLOR_MESSAGE   0xFFFFFFAA
#define AB_COLOR_MESSAGE_INFO   0x269BD8AA
#define AB_COLOR_ERROR_MESSAGE   0xAFAFAFAA

```
