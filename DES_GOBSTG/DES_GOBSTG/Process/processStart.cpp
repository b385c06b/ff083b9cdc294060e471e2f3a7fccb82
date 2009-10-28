#include "Process.h"
#include "Scripter.h"
#include "BGLayer.h"
#include "Selector.h"
#include "BGLayer.h"
#include "InfoSelect.h"
#include "Player.h"
#include "SE.h"
#include "BossInfo.h"
#include "FrontDisplay.h"

int Process::processStart()
{
	if (replaymode && (!Player::p.exist || scene == S1))
	{
		replayend = true;
		replaymode = false;
		scene = S1;
		time = 0;
//		musicChange(0, true);
		BGLayer::KillOtherLayer();
		Selector::Clear();
		InfoSelect::Clear();
		bgmask.exist = false;
		fdisp.SetState(FDISP_PANEL, 0);
		fdisp.SetState(FDISP_NEXTSTAGE, 0);
		Player::p.exist = false;
//		getInput();
		state = STATE_REPLAY;
		return PTURN;
	}
	if(!Player::p.exist && !replaymode)
	{
		if(spellmode && time != 0)
		{
			time = 0;
			clearPrep(false);
			Player::p.exist = false;
			hge->Input_SetDIKey(KS_PAUSE);
		}
		else
		{
			scr.SetIntValue(SCR_RESERVEBEGIN, 0);
			state = STATE_CONTINUE;
			return PTURN;
		}
	}
	else
		frameStart();

	if(hge->Input_GetDIKey(KS_PAUSE, DIKEY_DOWN) && !(Player::p.flag & PLAYER_SHOT) && state != STATE_CLEAR && !BossInfo::allover)
	{
		saveInput = nowInput;
		SE::push(SE_SYSTEM_PAUSE);

		fgpause.exist = true;
		fgpause.SetFlag(FG_PAUSEIN, FGMT_PAUSE);

		state = STATE_PAUSE;
	}

	if(scene > S1 && state != STATE_CLEAR)
	{
		time = 0;
		state = STATE_CLEAR;
		return PTURN;
	}

	if(time != lasttime && state != STATE_CLEAR)
	{
		if (!(stopflag & FRAME_STOPFLAG_ENEMYSET))
		{
			scr.stageExecute(scene, time);
		}
	}
	return PGO;
}
