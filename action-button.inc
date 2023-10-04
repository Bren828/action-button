/*
*
*
*
*   ---------------------------------------------------
*
*	Creater - @Bren828 					
*									4 October 2023
*
*   Version: 1.0
*
*   ---------------------------------------------------
*
*
*	https://github.com/Bren828/action-button
*
*
*
* Create action button
*    	CreateActionButton(const function[], playerid, inviteid, const text_creator[] = "", const text_invited[] = "", accept_button = KEY_YES, decline_button = KEY_NO, const dialog_caption[] = "", const dialog_button1[] = "", const dialog_button2[] = "")
*    	Returns (0) on failure or (1) on success
*
*   
* Cancel invitation
*		CancelActionButton(playerid)
*    	Returns (0) on failure or (1) on success
*
*   
* Get invited player ID
*		GetActionButtonInviteid(playerid)
*    	Returns (INVALID_PLAYER_ID) on failure or (ID) on success
*
*   
* Set the distance to the player
*		SetActionButtonTime(playerid, time)
*
*   
* Get timeout time
*		GetActionButtonTime(playerid)
*    	Returns: The time in seconds
*
*   
* Allow to invite yourself
*		SetActionButtonInviteYourself(playerid, bool:enable)
*
*   
* Set the distance to the player
*		SetActionButtonDistance(playerid, Float:distance)
*
*
*/

#if defined _action_button_included 
	#endinput
#endif
#define _action_button_included


#if !defined AB_INVITE_TIMEOUT_TIME
	#define AB_INVITE_TIMEOUT_TIME 		30
#endif

#if !defined AB_MAX_DISTANCE
	#define AB_MAX_DISTANCE 			3.0
#endif

#if !defined AB_DIALOG_ID
	#define AB_DIALOG_ID 				16743		
#endif


#if !defined AB_COLOR_MESSAGE
	#define AB_COLOR_MESSAGE      		0xFFFFFFAA
#endif

#if !defined AB_COLOR_MESSAGE_INFO
	#define AB_COLOR_MESSAGE_INFO      	0x269BD8AA
#endif

#if !defined AB_COLOR_ERROR_MESSAGE
	#define AB_COLOR_ERROR_MESSAGE      0xAFAFAFAA
#endif


#if !defined AB_TEXT_INFO
	static AB_TEXT_INFO[] = "Для подтверждения нажмите {33AA33}~k~~CONVERSATION_YES~ {269BD8}или для отмены {FF0000}~k~~CONVERSATION_NO~";
#endif

#if !defined AB_TEXT_ACTIVE_INVITE
	static AB_TEXT_ACTIVE_INVITE[] = "У вас есть активное предложение, повторите попытку через несколько секунд.";
#endif

#if !defined AB_TEXT_ACTIVE_INVITE2
	static AB_TEXT_ACTIVE_INVITE2[] = "Вы уже отправили предложение, повторите попытку через несколько секунд.";
#endif

#if !defined AB_TEXT_ALREADY_INVITED
	static AB_TEXT_ALREADY_INVITED[] = "Игроку уже что-то предложили.";
#endif

#if !defined AB_TEXT_INVALID_ID
	static AB_TEXT_INVALID_ID[] = "Игрок не найден";
#endif

#if !defined AB_TEXT_INVALID_STATE
	static AB_TEXT_INVALID_STATE[] = "Игрок в данный момент не активен";
#endif

#if !defined AB_TEXT_INVALID_DISTANCE
	static AB_TEXT_INVALID_DISTANCE[] = "Игрок далеко от вас";
#endif

#if !defined AB_TEXT_TIME_OVER
	static AB_TEXT_TIME_OVER[] = "Время подтверждения вышло";
#endif

#if !defined AB_TEXT_ACCEPTED
	static AB_TEXT_ACCEPTED[] = "Вы приняли предложение %s";
#endif

#if !defined AB_TEXT_ACCEPTED_BY_PLAYER
	static AB_TEXT_ACCEPTED_BY_PLAYER[] = "Игрок %s принял ваше предложение";
#endif

#if !defined AB_TEXT_DECLINE
	static AB_TEXT_DECLINE[] = "Вы отказались от предложения %s";
#endif

#if !defined AB_TEXT_DECLINE_BY_PLAYER
	static AB_TEXT_DECLINE_BY_PLAYER[] = "Игрок %s отказался от вашего предложения";
#endif


#define ActionButton:%0(%1) forward ab_%0(%1); public ab_%0(%1)

#define AB ActionButton
#define PressButton ActionButton


enum ACTION_BUTTON_EN
{
	bool:ab_Creator,
	bool:ab_Invited,
	ab_InviteID,
	ab_Time,
	ab_Function[32],
	bool:ab_EnableInvitingYourself,
	Float:ab_Distance,
	ab_AcceptButton,
	ab_DeclineButton,
	ab_TimeTimeout
}
static action_button[MAX_PLAYERS][ACTION_BUTTON_EN];



stock CreateActionButton(const function[], playerid, inviteid, const text_creator[] = "", const text_invited[] = "", accept_button = KEY_YES, decline_button = KEY_NO, const dialog_caption[] = "", const dialog_button1[] = "", const dialog_button2[] = "")
{
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_Time] >= gettime())
	{
		if(action_button[playerid][ab_Invited] == true) SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_ACTIVE_INVITE);
		else SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_ACTIVE_INVITE2);
		
		return 0;
	}

	if(!IsPlayerConnected(inviteid) || action_button[playerid][ab_EnableInvitingYourself] == false && playerid == inviteid) 
	{
		SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_INVALID_ID);
    	return 0;
	}

	if(!(GetPlayerState(inviteid) >= 1 && GetPlayerState(inviteid) <= 3)) 
	{
		SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_INVALID_STATE);
		return 0;
	}

	if(action_button[inviteid][ab_InviteID] != INVALID_PLAYER_ID && action_button[inviteid][ab_Time] >= gettime()) 
	{
		SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_ALREADY_INVITED);
    	return 0;
	}

	new Float:Pos[3];
    GetPlayerPos(inviteid, Pos[0], Pos[1], Pos[2]);

    if(!IsPlayerInRangeOfPoint(playerid, action_button[playerid][ab_Distance], Pos[0], Pos[1], Pos[2]) || 
		GetPlayerInterior(playerid) != GetPlayerInterior(inviteid) || 
		GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(inviteid)
	) 
	{
		SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_INVALID_DISTANCE);
		return 0;
	}

	action_button[playerid][ab_Creator] = true;
	action_button[inviteid][ab_Invited] = true;

	action_button[playerid][ab_InviteID] = inviteid;
	action_button[inviteid][ab_InviteID] = playerid;

	action_button[playerid][ab_Time] = gettime() + action_button[playerid][ab_TimeTimeout];
	action_button[inviteid][ab_Time] = gettime() + action_button[playerid][ab_TimeTimeout];

	format(action_button[inviteid][ab_Function], 32, "ab_%s", function);

	action_button[inviteid][ab_AcceptButton] = accept_button;
	action_button[inviteid][ab_DeclineButton] = decline_button;


	if(strlen(text_creator) >= 1) SendClientMessage(playerid, AB_COLOR_MESSAGE, text_creator);

	if(strlen(dialog_caption) >= 1)
	{
		#if defined _mdialog_included
			Dialog_Open(inviteid, Dialog:_Button_Action_Dialog, DIALOG_STYLE_MSGBOX, dialog_caption, text_invited, dialog_button1, dialog_button2);
		#else 
			ShowPlayerDialog(playerid, AB_DIALOG_ID, DIALOG_STYLE_MSGBOX, dialog_caption, text_invited, dialog_button1, dialog_button2);
		#endif
		return 1;
	}

	if(strlen(text_invited) >= 1) SendClientMessage(inviteid, AB_COLOR_MESSAGE, text_invited);

	if(strlen(AB_TEXT_INFO) >= 1) SendClientMessage(inviteid, AB_COLOR_MESSAGE_INFO, AB_TEXT_INFO);
	
	return 1;
}


stock CancelActionButton(playerid)
{
	if(action_button[playerid][ab_InviteID] == INVALID_PLAYER_ID) return 0;

	new inviteid = action_button[playerid][ab_InviteID];

	action_button[playerid][ab_InviteID] = INVALID_PLAYER_ID;
	action_button[playerid][ab_Function][0] = '\0';
	action_button[playerid][ab_Creator] = false;
	action_button[playerid][ab_Invited] = false;
	action_button[playerid][ab_Time] = 0;

	if(inviteid != INVALID_PLAYER_ID && action_button[inviteid][ab_InviteID] != INVALID_PLAYER_ID && action_button[inviteid][ab_InviteID] == playerid)
	{
		action_button[inviteid][ab_InviteID] = INVALID_PLAYER_ID; 
		action_button[inviteid][ab_Function][0] = '\0';
		action_button[inviteid][ab_Creator] = false;
		action_button[inviteid][ab_Invited] = false;
		action_button[inviteid][ab_Time] = 0;
	}

	return 1;
}


stock GetActionButtonInviteid(playerid)
{
	if(action_button[playerid][ab_Creator] == true && action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_Time] >= gettime()) 
	{
		return action_button[playerid][ab_InviteID];
	}

	return INVALID_PLAYER_ID;
}


stock SetActionButtonTime(playerid, time)
{
	action_button[playerid][ab_TimeTimeout] = time;
	return 1;
}


stock GetActionButtonTime(playerid)
{
	new time;
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_Time] >= gettime()) 
	{
		time = action_button[playerid][ab_Time]-gettime();
	}
	if(time < 0) time = 0;

	return time;
}


stock SetActionButtonInviteYourself(playerid, bool:enable)
{
	action_button[playerid][ab_EnableInvitingYourself] = enable;
	return 1;
}


stock SetActionButtonDistance(playerid, Float:distance)
{
	action_button[playerid][ab_Distance] = distance;
	return 1;
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

		if(action_button[playerid][ab_Invited] == true)
		{
			if(!response) return _ActionButtonResponse(playerid, action_button[playerid][ab_DeclineButton]);

			_ActionButtonResponse(playerid, action_button[playerid][ab_AcceptButton]);
			return 1;
		}
	
	#if !defined _mdialog_included
	}
	#endif

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnDialogResponse_Button_Action
		OnDialogResponse_Button_Action(playerid, dialogid, response, listitem, inputtext);
	#endif
	return 1;
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
	action_button[playerid][ab_Creator] = false;
	action_button[playerid][ab_Invited] = false;
	action_button[playerid][ab_Time] = 0;
	action_button[playerid][ab_EnableInvitingYourself] = false;
	action_button[playerid][ab_Distance] = AB_MAX_DISTANCE;
	action_button[playerid][ab_TimeTimeout] = AB_INVITE_TIMEOUT_TIME;

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerConnect_ActionButton
        OnPlayerConnect_ActionButton(playerid);
    #endif
    return 1;
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
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID) CancelActionButton(playerid);

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerDisconnect_ActionButton
        OnPlayerDisconnect_ActionButton(playerid, reason);
    #endif
    return 1;
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
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID) CancelActionButton(playerid);

    #if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPlayerDeath_ActionButton
        OnPlayerDeath_ActionButton(playerid, killerid, reason);
    #endif
    return 1;
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
	hook OOnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#else
	public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
#endif
{
	if(action_button[playerid][ab_InviteID] != INVALID_PLAYER_ID && action_button[playerid][ab_Invited] == true && (newkeys & action_button[playerid][ab_AcceptButton] || newkeys & action_button[playerid][ab_DeclineButton]))
	{
		_ActionButtonResponse(playerid, newkeys);
		return 1;
	}

	#if !defined _inc_y_hooks && !defined _INC_y_hooks && defined OnPKeyStateC_ActionButton
        OnPKeyStateC_ActionButton(playerid, newkeys, oldkeys);
    #endif
    return 1;
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


static stock _ActionButtonResponse(playerid, newkeys)
{
	new inviteid = action_button[playerid][ab_InviteID], 
		Float:x,
		Float:y,
		Float:z;

	if(inviteid == INVALID_PLAYER_ID || action_button[inviteid][ab_InviteID] != playerid || !IsPlayerConnected(inviteid)) 
	{
		CancelActionButton(playerid);
		SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_INVALID_ID);
		return 1;
	}

	if(action_button[playerid][ab_Time] <= gettime()) 
	{
		CancelActionButton(playerid);
		return SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_TIME_OVER);
	}

	if(!(GetPlayerState(playerid) >= 1 && GetPlayerState(playerid) <= 3) || !(GetPlayerState(inviteid) >= 1 && GetPlayerState(inviteid) <= 3)) 
	{
		CancelActionButton(playerid);
		return SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_INVALID_STATE);
	}

	GetPlayerPos(inviteid, x, y, z);

	if(!IsPlayerInRangeOfPoint(playerid, action_button[inviteid][ab_Distance], x, y, z) || 
		GetPlayerInterior(playerid) != GetPlayerInterior(inviteid) || 
		GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(inviteid)) 
	{
		CancelActionButton(playerid);
		return SendClientMessage(playerid, AB_COLOR_ERROR_MESSAGE, AB_TEXT_INVALID_DISTANCE);
	}

	new string[144], 
		name[MAX_PLAYER_NAME],
		name_invite[MAX_PLAYER_NAME];

	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerName(inviteid, name_invite, MAX_PLAYER_NAME);

	if(newkeys & action_button[playerid][ab_AcceptButton])
	{
		if(strlen(AB_TEXT_ACCEPTED) >= 1)
		{
			format(string, sizeof(string), AB_TEXT_ACCEPTED, name_invite);
			SendClientMessage(playerid, AB_COLOR_MESSAGE, string);
		}

		if(strlen(AB_TEXT_ACCEPTED_BY_PLAYER) >= 1)
		{
			format(string, sizeof(string), AB_TEXT_ACCEPTED_BY_PLAYER, name);
			SendClientMessage(inviteid, AB_COLOR_MESSAGE, string);
		}

		CallLocalFunction(action_button[playerid][ab_Function], "ddd", playerid, inviteid, 1);
	}
	else 
	{
		if(strlen(AB_TEXT_DECLINE) >= 1)
		{
			format(string, sizeof(string), AB_TEXT_DECLINE, name_invite);
			SendClientMessage(playerid, AB_COLOR_MESSAGE, string);
		}

		if(strlen(AB_TEXT_DECLINE_BY_PLAYER) >= 1)
		{
			format(string, sizeof(string), AB_TEXT_DECLINE_BY_PLAYER, name);
			SendClientMessage(inviteid, AB_COLOR_MESSAGE, string);
		}

		CallLocalFunction(action_button[playerid][ab_Function], "ddd", playerid, inviteid, 0);
	}

	CancelActionButton(inviteid);
	return 1;
}