# action-button

Library for creating interactive player-to-player action confirmation systems.
It allows you to implement custom actions that require another player's consent via key presses or dialog responses within a defined timeout and distance.

Библиотека предназначенная для создания систем подтверждения действий между игроками.
Позволяет реализовать пользовательские действия, требующие согласия другого игрока посредством нажатия клавиш или ответов в диалоговом окне в пределах заданного таймаута и расстояния.\
**Для подтверждения нажмите Y или для отмены N**

## Reference
* [Installation](#installation)
* [Example](#example)
* [Functions](#functions)
* [Definition](#definition)

## Installation

Include in your code and begin using the library:
```pawn
#include <action-button>
```
> **Note:** [mdialog](https://github.com/Open-GTO/mdialog) If included, it will be automatically used for dialogs. 

## Example
```pawn
CMD:test(playerid, params[]) {

    new inviteid;

    if(sscanf(params, "d", inviteid)) {
        return SendClientMessage(playerid, -1, "/test [ID]");
    }

    if(!IsPlayerConnected(inviteid)) {
        return SendClientMessage(playerid,-1, "INVALID");
    }

    CreateActionButton("give_flowers", playerid, inviteid, 
        "You offered flowers", 
        "You were offered flowers", 
        KEY_YES, KEY_NO);

    return 1;
}

ActionButton:give_flowers(playerid, inviteid, response) {
    
    if(!response) {
        return 1;
    }

    GivePlayerWeapon(playerid, 14, 1);
    return 1;
}
```

## Functions

#### CreateActionButton(const function[], playerid, inviteid, messageCreator[] = "", messageInvited[] = "", keyAccept = KEY_YES, keyDecline = KEY_NO, dialogCaption[] = "", dialogButton1[] = "", dialogButton2[] = "")
> Create action button
> * `function[]` - Function name
> * `playerid` - The ID of the player
> * `inviteid` - Invited Player ID
> * `messageCreator[]` - Message for the creator
> * `messageInvited[]` - Message for the invitee
> * `keyAccept` - Key for acceptance
> * `keyDecline` - Key for decline
> * `dialogCaption[]` - **Use dialog.** The title at the top of the dialog
> * `dialogButton1[]` - Dialog button 1
> * `dialogButton2[]` - Dialog button 2
> * Returns (0) on failure or (1) on success

#### CancelActionButton(playerid)
> Cancel invitation
> * `playerid` - The ID of the player
> * Returns (0) on failure or (1) on success

#### GetActionButtonInviteID(playerid)
> Get invited player ID
> * `playerid` - The ID of the player
> * Returns (`INVALID_PLAYER_ID`) on failure or (`ID`) on success

#### SetActionButtonTimeout(playerid, timeout)
> Set timeout time
> * `playerid` - The ID of the player
> * `timeout` - Timeout

#### GetActionButtonTimeout(playerid)
> Get timeout time
> * `playerid` - The ID of the player
> * Returns The time in seconds

#### SetActionButtonSelfInvite(playerid, bool:enable)
> Allows a player to invite themselves
> * `playerid` - The ID of the player
> * `bool:enable` - true to enable, false to disable

#### IsActionButtonSelfInvite(playerid)
> Check if self-invitation is allowed
> * `playerid` - The ID of the player
> * Returns true if self-invitation is allowed, false otherwise

#### SetActionButtonMaxDistance(playerid, Float:distance)
> Set the distance to the player
> * `playerid` - The ID of the player
> * `Float:distance` - Distance

#### GetActionButtonMaxDistance(playerid)
> Get the distance to the player
> * `playerid` - The ID of the player
> * Returns The distance as a float

## Definition

```pawn
#define AB_DEFAULT_TIMEOUT 30
#define AB_MAX_DISTANCE 3.0
#define AB_DIALOG_ID 16743		


#define AB_MESSAGE_COLOR 0xFFFFFFAA
#define AB_HELP_MESSAGE_COLOR 0x269BD8AA
#define AB_ERROR_MESSAGE_COLOR 0xAFAFAFAA


#define AB_TEXT_KEY_HELP "Для подтверждения нажмите {33AA33}~k~~CONVERSATION_YES~ {269BD8}или для отмены {FF0000}~k~~CONVERSATION_NO~"
#define AB_TEXT_PENDING_INCOMING "У вас есть активное предложение, повторите попытку через несколько секунд."
#define AB_TEXT_PENDING_OUTGOING "Вы уже отправили предложение, повторите попытку через несколько секунд."

#define AB_TEXT_ALREADY_INVITED "Игроку уже что-то предложили."
#define AB_TEXT_INVALID_ID "Игрок не найден"
#define AB_TEXT_INVALID_STATE "Игрок в данный момент не активен"
#define AB_TEXT_INVALID_DISTANCE "Игрок далеко от вас"
#define AB_TEXT_TIMEOUT "Время подтверждения вышло"
#define AB_TEXT_ACCEPTED "Вы приняли предложение %s"
#define AB_TEXT_ACCEPTED_BY_PLAYER "Игрок %s принял ваше предложение"
#define AB_TEXT_DECLINED "Вы отказались от предложения %s"
#define AB_TEXT_DECLINED_BY_PLAYER "Игрок %s отказался от вашего предложения"
```
