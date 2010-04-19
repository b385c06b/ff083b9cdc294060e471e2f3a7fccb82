#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/BGLayer.h"
#include "../Header/Selector.h"
#include "../Header/BGLayer.h"
#include "../Header/InfoSelect.h"
#include "../Header/Player.h"
#include "../Header/SE.h"
#include "../Header/BossInfo.h"
#include "../Header/FrontDisplay.h"

int Process::processStart()
{
	if (replaymode && (!Player::p.exist || scene == S1))
	{
		replayend = true;
		replaymode = false;
		scene = S1;
		gametime = 0;
//		musicChange(0, true);
		BGLayer::KillOtherLayer();
		Selector::Clear();
		InfoSelect::Clear();
		BGLayer::ubg[UBGID_BGMASK].exist = false;
		FrontDisplay::fdisp.SetState(FDISP_PANEL, FDISPSTATE_OFF);
		FrontDisplay::fdisp.SetState(FDISP_NEXTSTAGE, FDISPSTATE_OFF);
		Player::p.exist = false;
//		getInput();
		state = STATE_REPLAY;
		return PTURN;
	}
	if(!Player::p.exist && !replaymode)
	{
		if(spellmode && gametime != 0)
		{
			gametime = 0;
			clearPrep(false);
			Player::p.exist = false;
			hge->Input_SetDIKey(KS_PAUSE);
		}
		else
		{
			Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0);
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

		BGLayer::ubg[UFGID_FGPAUSE].exist = true;
		BGLayer::ubg[UFGID_FGPAUSE].SetFlag(FG_PAUSEIN, FGMT_PAUSE);

		state = STATE_PAUSE;
	}

	if(scene > S1 && state != STATE_CLEAR)
	{
		gametime = 0;
		state = STATE_CLEAR;
		return PTURN;
	}

	if(gametime != lasttime && state != STATE_CLEAR)
	{
		DWORD stopflag = GetStopFlag();
		bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_ENEMYSET);
		if (!binstop)
		{
			Scripter::scr.stageExecute(scene, gametime);
		}
	}
	return PGO;
}
