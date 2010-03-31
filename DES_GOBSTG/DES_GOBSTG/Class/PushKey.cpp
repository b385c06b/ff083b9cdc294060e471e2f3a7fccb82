#include "../Header/PushKey.h"
#include "../Header/BResource.h"

pushkeyEvent PushKey::pushkeyevent[PUSHKEY_IDMAX];

bool PushKey::SetPushEvent(BYTE ID, int pushkey_1, int pushkey_2/* =PUSHKEY_KEYNULL */, int pushkey_3/* =PUSHKEY_KEYNULL */, int pushkey_4/* =PUSHKEY_KEYNULL */, int pushfirst/* =M_PUSH_FIRST */, int pushrollto/* =M_PUSH_ROLLTO */)
{
	if (ID >= PUSHKEY_IDMAX)
	{
		return false;
	}
	pushkeyevent[ID].pushkey[0] = pushkey_1;
	pushkeyevent[ID].pushkey[1] = pushkey_2;
	pushkeyevent[ID].pushkey[2] = pushkey_3;
	pushkeyevent[ID].pushkey[3] = pushkey_4;
	pushkeyevent[ID].pushfirst = pushfirst;
	pushkeyevent[ID].pushrollto = pushrollto;
	pushkeyevent[ID].timer = 0;
	return true;
}

bool PushKey::UpdatePushEvent(BYTE ID)
{
	if (ID >= PUSHKEY_IDMAX)
	{
		return false;
	}
	pushkeyEvent * item = &(pushkeyevent[ID]);
	bool pushed = false;

	for (int i=0; i<PUSHKEY_KEYCHECKMAX; i++)
	{
		if (item->pushkey[i] != PUSHKEY_KEYNULL)
		{
			if (hge->Input_GetDIKey(item->pushkey[i]))
			{
				pushed = true;
				break;
			}
		}
	}
	if (pushed)
	{
		item->timer++;
		if (item->timer == item->pushfirst)
		{
			item->timer = item->pushrollto;
		}
		if (item->timer == item->pushrollto)
		{
			for (int i=0; i<PUSHKEY_KEYCHECKMAX; i++)
			{
				if (item->pushkey[i] != PUSHKEY_KEYNULL)
				{
					hge->Input_SetDIKey(item->pushkey[i], false);
				}
			}
			return true;
		}
	}
	else
	{
		item->timer = 0;
	}
	return false;
}