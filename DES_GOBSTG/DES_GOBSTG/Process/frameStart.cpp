#include "Process.h"
#include "Player.h"
#include "Data.h"
#include "DataConnector.h"

void Process::frameStart()
{
	if(!(Player::p.flag & PLAYER_SHOT))
	{
		if (!(stopflag & FRAME_STOPFLAG_ENEMYSET))
		{
			time++;
		}
	}
	if(!replaymode)
	{
		playing = true;
		if(!Player::p.ncCont)
		{
			replayIndex++;
			replayframe[replayIndex].input = nowInput;
			Export::rpySetBias(&(replayframe[replayIndex]));
		}
	}
	else if(!Player::p.ncCont)
	{
		replayIndex++;
		nowInput = replayframe[replayIndex].input;
		replayFPS = Export::rpyGetReplayFPS(replayframe[replayIndex]);

		if(nowInput == 0xff)
		{
			replayend = true;
			scene = S1;
			clearPrep();
			hge->Input_SetDIKey(KS_PAUSE);
		}
		else
		{
			if(nowInput & 0x1)
				hge->Input_SetDIKey(KS_LEFT);
			if(nowInput & 0x2)
				hge->Input_SetDIKey(KS_RIGHT);
			if(nowInput & 0x4)
				hge->Input_SetDIKey(KS_UP);
			if(nowInput & 0x8)
				hge->Input_SetDIKey(KS_DOWN);
			if(nowInput & 0x10)
				hge->Input_SetDIKey(KS_FIRE);
			if(nowInput & 0x20)
				hge->Input_SetDIKey(KS_SPECIAL);
			if(nowInput & 0x40)
				hge->Input_SetDIKey(KS_CHANGE);
			if(nowInput & 0x80)
				hge->Input_SetDIKey(KS_SLOW);
		}
	}
	if(time == 1)
	{
		if(data.getSpellNumber(scene) > 0)
		{
			DataConnector::Meet();
		}
	}
	active = true;
}