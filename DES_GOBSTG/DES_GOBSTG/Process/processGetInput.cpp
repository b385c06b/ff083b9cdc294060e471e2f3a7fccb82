#include "Process.h"

int Process::getInput()
{
	if(!(replaymode && !replayend && (state == STATE_START || state == STATE_CLEAR)))
	{
		if(	hge->Input_GetDIJoy(JOY_LEFT))
			hge->Input_SetDIKey(KS_LEFT);
		if(	hge->Input_GetDIJoy(JOY_RIGHT))
			hge->Input_SetDIKey(KS_RIGHT);
		if(	hge->Input_GetDIJoy(JOY_UP))
			hge->Input_SetDIKey(KS_UP);
		if(	hge->Input_GetDIJoy(JOY_DOWN))
			hge->Input_SetDIKey(KS_DOWN);

		if(	hge->Input_GetDIJoy(joyFire))
			hge->Input_SetDIKey(KS_FIRE);
		if(	hge->Input_GetDIJoy(joySpecial))
			hge->Input_SetDIKey(KS_SPECIAL);
		if(	hge->Input_GetDIJoy(joySlow))
			hge->Input_SetDIKey(KS_SLOW);
		if(	hge->Input_GetDIJoy(joyChange))
			hge->Input_SetDIKey(KS_CHANGE);
	}
	else
	{
		hge->Input_SetDIKey(KS_LEFT, false);
		hge->Input_SetDIKey(KS_RIGHT, false);
		hge->Input_SetDIKey(KS_UP, false);
		hge->Input_SetDIKey(KS_DOWN, false);
		hge->Input_SetDIKey(KS_FIRE, false);
		hge->Input_SetDIKey(KS_SPECIAL, false);
		hge->Input_SetDIKey(KS_CHANGE, false);
		hge->Input_SetDIKey(KS_SLOW, false);
	}

	if(	hge->Input_GetDIJoy(joyPause))
		hge->Input_SetDIKey(KS_PAUSE);

	nowInput = 0;
	if(hge->Input_GetDIKey(KS_LEFT))
		nowInput |= 0x1;
	if(hge->Input_GetDIKey(KS_RIGHT))
		nowInput |= 0x2;
	if(hge->Input_GetDIKey(KS_UP))
		nowInput |= 0x4;
	if(hge->Input_GetDIKey(KS_DOWN))
		nowInput |= 0x8;
	if(hge->Input_GetDIKey(KS_FIRE))
		nowInput |= 0x10;
	if(hge->Input_GetDIKey(KS_SPECIAL))
		nowInput |= 0x20;
	if(hge->Input_GetDIKey(KS_CHANGE))
		nowInput |= 0x40;
	if(hge->Input_GetDIKey(KS_SLOW))
		nowInput |= 0x80;

	return PGO;
}