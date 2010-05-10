#include "../Header/Process.h"
#include "../Header/InfoSelect.h"
#include "../Header/Player.h"
#include "../Header/DataConnector.h"
#include "../Header/BGLayer.h"
#include "../Header/Replay.h"
#include "../Header/FrontDisplay.h"
#include "../Header/Data.h"

#define _PCLEAR_FDISP_TIME			240
#define _PCLEAR_FDIPS_CANCELTIME	60

int Process::processClear()
{
	processStart();
	if(gametime == 1)
	{
		frameskip = M_DEFAULT_FRAMESKIP;

		BGLayer::ubg[UFGID_FGPAUSE].exist = true;
		BGLayer::ubg[UFGID_FGPAUSE].changetimer = 0;
		BGLayer::ubg[UFGID_FGPAUSE].flag = FG_PAUSEIN;

		LONGLONG tscore = 0;

		BYTE tstage;
		bool islast = false;
		if(scene - SCLEAR > S1)
		{
			tstage = 0xff;
			islast = true;
		}
		else
			tstage = (scene - SCLEAR) / M_STAGENSCENE;

		musicSlide(1.0f);
		if(islast)
		{
			DataConnector::Clear();
			tscore += Player::p.getAllClearBonusLife();
			tscore += Player::p.getAllClearBonusPower();
		}
		tscore += Player::p.getClearBonusPoint();
		tscore += Player::p.getClearBonusGraze();
		tscore += Player::p.getClearBonusFaith();
		tscore += Player::p.getClearBonusStage(tstage);

		Player::p.nScore += tscore;

		FrontDisplay::fdisp.SetValue(tscore, 0, 0, (!islast) && (!practicemode));
		FrontDisplay::fdisp.SetState(FDISP_NEXTSTAGE, _PCLEAR_FDISP_TIME);
	}

	if (gametime == _PCLEAR_FDIPS_CANCELTIME)
	{
//		musicChange(0, true);
	}

	if(gametime > _PCLEAR_FDISP_TIME /*&& !replaymode*/ || gametime > _PCLEAR_FDIPS_CANCELTIME && hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		FrontDisplay::fdisp.SetState(FDISP_NEXTSTAGE, FDISPSTATE_OFF);
		BGLayer::ubg[UFGID_FGPAUSE].exist = false;

		scene -= SCLEAR;
		InfoSelect::Clear();
		BYTE tpart;
		bool islast = false;

		if(scene > S1)
		{
			tpart = 0xff;
			islast = true;
		}
		else
		{
			if (scene > S1200)
			{
				tpart = RPYPARTMAX - 1;
			}
			else
			{
				tpart = Data::data.getStage(scene) - 1;
			}
		}

		if(!replaymode)
		{
			seed = hge->Timer_GetCurrentSystemTime();
			Replay::rpy.partFill(tpart);
		}
		gametime = 0;
		if (!replaymode)
		{
			if (practicemode)
			{
				state = STATE_OVER;
				return PTURN;
			}
			else if (islast)
			{
				state = STATE_ENDING;
				return PTURN;
			}
		}
		state = STATE_START;
		if(islast)
		{
			scene = S1;
		}
		else
		{
			DataConnector::Try();
		}

		clearPrep();
		if(replaymode && tpart != 0xff)
		{
			seed = Replay::rpy.partinfo[tpart].seed;
		}

		srandt(seed);

		return PTURN;
	}

	return PGO;
}