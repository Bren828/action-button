/*
*
*
*
*   ---------------------------------------------------
*
*	Created by Bren828					
*									6 May 2026
*
*   Version: 1.2
*
*   ---------------------------------------------------
*
*
*	https://github.com/Bren828/action-button
*
*
*/

#if defined _action_button_included 
	#endinput
#endif
#define _action_button_included


#if !defined AB_DEFAULT_TIMEOUT
	#define AB_DEFAULT_TIMEOUT 30
#endif

#if !defined AB_MAX_DISTANCE
	#define AB_MAX_DISTANCE 3.0
#endif

#if !defined AB_DIALOG_ID
	#define AB_DIALOG_ID 16743		
#endif


#if !defined AB_MESSAGE_COLOR
	#define AB_MESSAGE_COLOR 0xFFFFFFAA
#endif

#if !defined AB_HELP_MESSAGE_COLOR
	#define AB_HELP_MESSAGE_COLOR 0x269BD8AA
#endif

#if !defined AB_ERROR_MESSAGE_COLOR
	#define AB_ERROR_MESSAGE_COLOR 0xAFAFAFAA
#endif


#if !defined AB_TEXT_KEY_HELP
	#define AB_TEXT_KEY_HELP "Для подтверждения нажмите {33AA33}~k~~CONVERSATION_YES~ {269BD8}или для отмены {FF0000}~k~~CONVERSATION_NO~"
#endif

#if !defined AB_TEXT_PENDING_INCOMING
	#define AB_TEXT_PENDING_INCOMING "У вас есть активное предложение, повторите попытку через несколько секунд."
#endif

#if !defined AB_TEXT_PENDING_OUTGOING
	#define AB_TEXT_PENDING_OUTGOING "Вы уже отправили предложение, повторите попытку через несколько секунд."
#endif

#if !defined AB_TEXT_ALREADY_INVITED
	#define AB_TEXT_ALREADY_INVITED "Игроку уже что-то предложили."
#endif

#if !defined AB_TEXT_INVALID_ID
	#define AB_TEXT_INVALID_ID "Игрок не найден"
#endif

#if !defined AB_TEXT_INVALID_STATE
	#define AB_TEXT_INVALID_STATE "Игрок в данный момент не активен"
#endif

#if !defined AB_TEXT_INVALID_DISTANCE
	#define AB_TEXT_INVALID_DISTANCE "Игрок далеко от вас"
#endif

#if !defined AB_TEXT_TIMEOUT
	#define AB_TEXT_TIMEOUT "Время подтверждения вышло"
#endif

#if !defined AB_TEXT_ACCEPTED
	#define AB_TEXT_ACCEPTED "Вы приняли предложение %s"
#endif

#if !defined AB_TEXT_ACCEPTED_BY_PLAYER
	#define AB_TEXT_ACCEPTED_BY_PLAYER "Игрок %s принял ваше предложение"
#endif

#if !defined AB_TEXT_DECLINED
	#define AB_TEXT_DECLINED "Вы отказались от предложения %s"
#endif

#if !defined AB_TEXT_DECLINED_BY_PLAYER
	#define AB_TEXT_DECLINED_BY_PLAYER "Игрок %s отказался от вашего предложения"
#endif


#define ActionButton:%0(%1) \ 
	forward ab_%0(%1); \ 
	public ab_%0(%1)

#define AB ActionButton
#define PressButton ActionButton


enum E_ACTION_BUTTON {
	bool:ab_IsCreator,
	bool:ab_IsInvited,
	ab_InviteID,
	ab_ExpireTime,
	ab_Function[32],
	bool:ab_SelfInvite,
	Float:ab_MaxDistance,
	ab_KeyAccept,
	ab_KeyDecline,
	ab_Timeout
}

static action_button[MAX_PLAYERS][E_ACTION_BUTTON];



stock CreateActionButton(const function[], playerid, inviteid, const messageCreator[] = "", const messageInvited[] = "", keyAccept = KEY_YES, keyDecline = KEY_NO, const dialogCaption[] = "", const dialogButton1[] = "", const dialogButton2[] = "") {

	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_ExpireTime] >= gettime()) {
		if(action_button[playerid][ab_IsInvited]) {
			SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_PENDING_INCOMING);
		}
		else {
			SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_PENDING_OUTGOING);
		}
		return 0;
	}

	if(!IsPlayerConnected(inviteid) || !action_button[playerid][ab_SelfInvite] && playerid == inviteid) {
		SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_INVALID_ID);
    	return 0;
	}

	if(!(GetPlayerState(inviteid) >= 1 && GetPlayerState(inviteid) <= 3)) {
		SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_INVALID_STATE);
		return 0;
	}

	if(action_button[inviteid][ab_InviteID] != INVALID_PLAYER_ID && action_button[inviteid][ab_ExpireTime] >= gettime()) {
		SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_ALREADY_INVITED);
    	return 0;
	}

	new Float:x, 
		Float:y,
		Float:z;

    GetPlayerPos(inviteid, x, y, z);

    if(!IsPlayerInRangeOfPoint(playerid, action_button[playerid][ab_MaxDistance], x, y, z) || GetPlayerInterior(playerid) != GetPlayerInterior(inviteid) || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(inviteid)) {
		SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_INVALID_DISTANCE);
		return 0;
	}

	action_button[playerid][ab_IsCreator] = true;
	action_button[inviteid][ab_IsInvited] = true;

	action_button[playerid][ab_InviteID] = inviteid;
	action_button[inviteid][ab_InviteID] = playerid;

	action_button[playerid][ab_ExpireTime] = gettime() + action_button[playerid][ab_Timeout];
	action_button[inviteid][ab_ExpireTime] = gettime() + action_button[playerid][ab_Timeout];

	format(action_button[inviteid][ab_Function], 32, "ab_%s", function);

	if(keyAccept == keyDecline) {
		keyDecline ++;
	}

	action_button[inviteid][ab_KeyAccept] = keyAccept;
	action_button[inviteid][ab_KeyDecline] = keyDecline;

	if(strlen(messageCreator) >= 1) {
		SendClientMessage(playerid, AB_MESSAGE_COLOR, messageCreator);
	}

	if(strlen(dialogCaption) >= 1) {
		#if defined _mdialog_included
			Dialog_Open(inviteid, Dialog:_Button_Action_Dialog, DIALOG_STYLE_MSGBOX, dialogCaption, messageInvited, dialogButton1, dialogButton2);
		#else 
			ShowPlayerDialog(inviteid, AB_DIALOG_ID, DIALOG_STYLE_MSGBOX, dialogCaption, messageInvited, dialogButton1, dialogButton2);
		#endif
		return 1;
	}

	if(strlen(messageInvited) >= 1) {
		SendClientMessage(inviteid, AB_MESSAGE_COLOR, messageInvited);
	}

	if(strlen(AB_TEXT_KEY_HELP) >= 1) {
		SendClientMessage(inviteid, AB_HELP_MESSAGE_COLOR, AB_TEXT_KEY_HELP);
	}
	return 1;
}


stock CancelActionButton(playerid) {

	new inviteid = action_button[playerid][ab_InviteID];

	if(inviteid == INVALID_PLAYER_ID) {
		return 0;
	}

	action_button[playerid][ab_InviteID] = INVALID_PLAYER_ID;
	action_button[playerid][ab_Function][0] = '\0';
	action_button[playerid][ab_IsCreator] = false;
	action_button[playerid][ab_IsInvited] = false;
	action_button[playerid][ab_ExpireTime] = 0;

	if(inviteid != INVALID_PLAYER_ID && action_button[inviteid][ab_InviteID] != INVALID_PLAYER_ID && action_button[inviteid][ab_InviteID] == playerid) {
		action_button[inviteid][ab_InviteID] = INVALID_PLAYER_ID; 
		action_button[inviteid][ab_Function][0] = '\0';
		action_button[inviteid][ab_IsCreator] = false;
		action_button[inviteid][ab_IsInvited] = false;
		action_button[inviteid][ab_ExpireTime] = 0;
	}

	return 1;
}


stock GetActionButtonInviteID(playerid) {

	if(action_button[playerid][ab_IsCreator] && action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_ExpireTime] >= gettime()) {
		return action_button[playerid][ab_InviteID];
	}

	return INVALID_PLAYER_ID;
}


stock SetActionButtonTimeout(playerid, timeout) {
	action_button[playerid][ab_Timeout] = timeout;
	return 1;
}


stock GetActionButtonTimeout(playerid) {

	new time;

	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_ExpireTime] >= gettime()) {
		time = action_button[playerid][ab_ExpireTime] - gettime();
	}

	if(time < 0) {
		time = 0;
	}

	return time;
}


stock SetActionButtonSelfInvite(playerid, bool:enable) {
	action_button[playerid][ab_SelfInvite] = enable;
	return 1;
}


stock IsActionButtonSelfInvite(playerid) {
	return action_button[playerid][ab_SelfInvite];
}


stock SetActionButtonMaxDistance(playerid, Float:distance) {
	action_button[playerid][ab_MaxDistance] = distance;
	return 1;
}


stock Float:GetActionButtonMaxDistance(playerid) {
	return action_button[playerid][ab_MaxDistance];
}



#if defined _mdialog_included
    DialogResponse:_Button_Action_Dialog(playerid, response, listitem, inputtext[])
#else
	#if defined _inc_y_hooks || defined _INC_y_hooks
		hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
	#else
		public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
	#endif
#endif
{
	#if !defined _mdialog_included
	if(dialogid == AB_DIALOG_ID)
	{
	#endif

		if(action_button[playerid][ab_IsInvited]) {
			
			if(!response) {
				_ActionButtonResponse(playerid, action_button[playerid][ab_KeyDecline]);
				return 1;
			}

			_ActionButtonResponse(playerid, action_button[playerid][ab_KeyAccept]);
			return 1;
		}
	
	#if !defined _mdialog_included
	}
	#endif

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnDialogResponse_Button_Action
		return OnDialogResponse_Button_Action(playerid, dialogid, response, listitem, inputtext);
	#else 
		return 1;
	#endif
}

#if !defined _mdialog_included && !defined _inc_y_hooks && !defined _INC_y_hooks
    #if defined _ALS_OnDialogResponse
        #undef OnDialogResponse
    #else
        #define _ALS_OnDialogResponse
    #endif
    #define OnDialogResponse OnDialogResponse_Button_Action
    #if defined OnDialogResponse_Button_Action
        forward OnDialogResponse_Button_Action(playerid, dialogid, response, listitem, inputtext[]);
    #endif 
#endif



#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnPlayerConnect(playerid)
#else
	public OnPlayerConnect(playerid)
#endif
{
	action_button[playerid][ab_InviteID] = INVALID_PLAYER_ID; 
	action_button[playerid][ab_Function][0] = '\0';
	action_button[playerid][ab_IsCreator] = false;
	action_button[playerid][ab_IsInvited] = false;
	action_button[playerid][ab_ExpireTime] = 0;
	action_button[playerid][ab_SelfInvite] = false;
	action_button[playerid][ab_MaxDistance] = AB_MAX_DISTANCE;
	action_button[playerid][ab_Timeout] = AB_DEFAULT_TIMEOUT;

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerConnect_ActionButton
        return OnPlayerConnect_ActionButton(playerid);
	#else 
   		return 1;
    #endif
}

#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect OnPlayerConnect_ActionButton
	#if defined OnPlayerConnect_ActionButton
		forward OnPlayerConnect_ActionButton(playerid);
	#endif
#endif


#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnPlayerDisconnect(playerid, reason)
#else
	public OnPlayerDisconnect(playerid, reason)
#endif
{
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID) {
		CancelActionButton(playerid);
	}

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerDisconnect_ActionButton
        return OnPlayerDisconnect_ActionButton(playerid, reason);
	#else 
    	return 1;
    #endif
}
#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect OnPlayerDisconnect_ActionButton
	#if defined OnPlayerDisconnect_ActionButton
		forward OnPlayerDisconnect_ActionButton(playerid, reason);
	#endif
#endif


#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnPlayerDeath(playerid, killerid, reason)
#else
	public OnPlayerDeath(playerid, killerid, reason)
#endif
{
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID) {
		CancelActionButton(playerid);
	}

    #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerDeath_ActionButton
        return OnPlayerDeath_ActionButton(playerid, killerid, reason);
	#else 
    	return 1;
    #endif
}
#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnPlayerDeath
		#undef OnPlayerDeath
	#else
		#define _ALS_OnPlayerDeath
	#endif
	#define OnPlayerDeath OnPlayerDeath_ActionButton
	#if defined OnPlayerDeath_ActionButton
		forward OnPlayerDeath_ActionButton(playerid, killerid, reason);
	#endif
#endif


#if defined _inc_y_hooks || defined _INC_y_hooks
	hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#else
	public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#endif
{
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_IsInvited] && 
	  (newkeys & action_button[playerid][ab_KeyAccept] || newkeys & action_button[playerid][ab_KeyDecline])) {
		_ActionButtonResponse(playerid, newkeys);
		return 1;
	}

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPKeyStateC_ActionButton
        return OnPKeyStateC_ActionButton(playerid, newkeys, oldkeys);
	#else 
   		return 1;
    #endif
}
#if !defined _inc_y_hooks && !defined _INC_y_hooks
	#if defined _ALS_OnPlayerKeyStateChange
		#undef OnPlayerKeyStateChange
	#else
		#define _ALS_OnPlayerKeyStateChange
	#endif
	#define OnPlayerKeyStateChange OnPKeyStateC_ActionButton
	#if defined OnPKeyStateC_ActionButton
		forward OnPKeyStateC_ActionButton(playerid, newkeys, oldkeys);
	#endif
#endif


static stock _ActionButtonResponse(playerid, newkeys) {

	new inviteid = action_button[playerid][ab_InviteID];

	if(inviteid == INVALID_PLAYER_ID || action_button[inviteid][ab_InviteID] != playerid || !IsPlayerConnected(inviteid)) {
		CancelActionButton(playerid);
		SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_INVALID_ID);
		return 1;
	}

	if(action_button[playerid][ab_ExpireTime] <= gettime()) {
		CancelActionButton(playerid);
		return SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_TIMEOUT);
	}

	if(!(GetPlayerState(playerid) >= 1 && GetPlayerState(playerid) <= 3) || !(GetPlayerState(inviteid) >= 1 && GetPlayerState(inviteid) <= 3)) {
		CancelActionButton(playerid);
		return SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_INVALID_STATE);
	}

	new Float:x,
		Float:y,
		Float:z;

	GetPlayerPos(inviteid, x, y, z);

	if(!IsPlayerInRangeOfPoint(playerid, action_button[inviteid][ab_MaxDistance], x, y, z) || GetPlayerInterior(playerid) != GetPlayerInterior(inviteid) || GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(inviteid)) {
		CancelActionButton(playerid);
		SendClientMessage(playerid, AB_ERROR_MESSAGE_COLOR, AB_TEXT_INVALID_DISTANCE);
		return 1;
	}

	new string[144];

	new name[MAX_PLAYER_NAME],
		nameInvite[MAX_PLAYER_NAME];

	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerName(inviteid, nameInvite, MAX_PLAYER_NAME);

	if(newkeys & action_button[playerid][ab_KeyAccept]) {

		if(strlen(AB_TEXT_ACCEPTED) >= 1) {
			format(string, sizeof(string), AB_TEXT_ACCEPTED, nameInvite);
			SendClientMessage(playerid, AB_MESSAGE_COLOR, string);
		}

		if(strlen(AB_TEXT_ACCEPTED_BY_PLAYER) >= 1) {
			format(string, sizeof(string), AB_TEXT_ACCEPTED_BY_PLAYER, name);
			SendClientMessage(inviteid, AB_MESSAGE_COLOR, string);
		}

		CallLocalFunction(action_button[playerid][ab_Function], "ddd", playerid, inviteid, 1);
	}
	else {

		if(strlen(AB_TEXT_DECLINED) >= 1) {
			format(string, sizeof(string), AB_TEXT_DECLINED, nameInvite);
			SendClientMessage(playerid, AB_MESSAGE_COLOR, string);
		}

		if(strlen(AB_TEXT_DECLINED_BY_PLAYER) >= 1) {
			format(string, sizeof(string), AB_TEXT_DECLINED_BY_PLAYER, name);
			SendClientMessage(inviteid, AB_MESSAGE_COLOR, string);
		}

		CallLocalFunction(action_button[playerid][ab_Function], "ddd", playerid, inviteid, 0);
	}

	CancelActionButton(inviteid);
	return 1;
}
