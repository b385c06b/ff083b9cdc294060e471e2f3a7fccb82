#include "Process.h"
#include "InfoSelect.h"
#include "Player.h"
#include "DataConnector.h"
#include "BGLayer.h"
#include "Replay.h"
#include "FrontDisplay.h"
#include "Data.h"

#define _PCLEAR_FDISP_TIME			240
#define _PCLEAR_FDIPS_CANCELTIME	60

int Process::processClear()
{
	processStart();
	if(time == 1)
	{
		frameskip = M_DEFAULT_FRAMESKIP;

		fgpause.exist = true;
		fgpause.changetimer = 0;
		fgpause.flag = FG_PAUSEIN;

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

		fdisp.SetValue(tscore, 0, 0, (!islast) && (!practicemode));
		fdisp.SetState(FDISP_NEXTSTAGE, _PCLEAR_FDISP_TIME);
	}

	if (time == _PCLEAR_FDIPS_CANCELTIME)
	{
//		musicChange(0, true);
	}

	if(time > _PCLEAR_FDISP_TIME /*&& !replaymode*/ || time > _PCLEAR_FDIPS_CANCELTIME && hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		fdisp.SetState(FDISP_NEXTSTAGE, 0);
		fgpause.exist = false;

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
				tpart = data.getStage(scene) - 1;
			}
		}

		if(!replaymode)
		{
			seed = timeGetTime();
			rpy.partFill(tpart);
		}
		time = 0;
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
			seed = rpy.partinfo[tpart].seed;
		}

		srandt(seed);

		return PTURN;
	}

	return PGO;
}