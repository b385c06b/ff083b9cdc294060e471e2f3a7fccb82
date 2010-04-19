#include "../Header/Process.h"
#include "../Header/Player.h"
#include "../Header/Data.h"
#include "../Header/DataConnector.h"
#include "../Header/Replay.h"

void Process::frameStart()
{
	if(!(Player::p.flag & PLAYER_SHOT))
	{
		DWORD stopflag = GetStopFlag();
		bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_ENEMYSET);
		if (!binstop)
		{
			gametime++;
		}
	}
	if(!replaymode)
	{
		playing = true;
		if(!Player::p.ncCont)
		{
			Replay::rpy.replayIndex++;
			Replay::rpy.replayframe[Replay::rpy.replayIndex].input = nowInput;
			Export::rpySetBias(&(Replay::rpy.replayframe[Replay::rpy.replayIndex]));
		}
	}
	else if(!Player::p.ncCont)
	{
		Replay::rpy.replayIndex++;
		nowInput = Replay::rpy.replayframe[Replay::rpy.replayIndex].input;
		replayFPS = Export::rpyGetReplayFPS(Replay::rpy.replayframe[Replay::rpy.replayIndex]);

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
	if(gametime == 1)
	{
		if(Data::data.getSpellNumber(scene) > 0)
		{
			DataConnector::Meet();
		}
	}
	active = true;
}