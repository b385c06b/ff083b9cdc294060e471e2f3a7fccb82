#include "Process.h"
#include "Bullet.h"
#include "Player.h"
#include "Scripter.h"
#include "BGLayer.h"
#include "Selector.h"
#include "Replay.h"

int Process::processContinue()
{
	if(Player::p.ncCont >= M_PL_CONTINUEMAX || practicemode)
		goto exit;

	if(!scr.GetIntValue(SCR_RESERVEBEGIN))
	{
		fgpause.exist = true;
		fgpause.SetFlag(FG_PAUSEIN, FGMT_PAUSE);
		scr.SetIntValue(SCR_RESERVEBEGIN+1, M_PL_CONTINUEMAX-Player::p.ncCont);

		hge->Channel_Pause(channel);
	}

	scr.controlExecute(STATE_CONTINUE, SCRIPT_CON_INIT);
	//60 sec
	int tsec = scr.GetIntValue(SCR_RESERVEBEGIN);

	if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		fgpause.SetFlag(FG_PAUSEOUT, FGMT_PAUSE);
	}
	else if(hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN))
	{
		Selector::select = 1;
	}

	if(tsec == 0x10)
	{
		fgpause.exist = false;
		if(!spellmode && scene < S1200)
		{
			Player::p.valueSet(mainchara, subchara_1, subchara_2, PL_DEFAULTNPLAYER, true);
			Player::ncCont++;

			Bullet::IzeBuild(BULLETIZE_FADEOUT, Player::p.x, Player::p.y);

			hge->Channel_Resume(channel);
			hge->Channel_SetVolume(channel, 0);
			musicSlide(1.5f, -1);
			state = STATE_START;
			return PTURN;
		}
		else
		{
			scene = startscene;
			startPrep();
			if(spellmode)
			{
				hge->Channel_Resume(channel);
				hge->Channel_SetVolume(channel, 0);
				musicSlide(1.5f, -1);
			}
			state = STATE_START;
			return PTURN;
		}
	}
	else if(tsec == 0x11)
	{
exit:
		rpy.partFill(0xff);
		fgpause.exist = false;
//		musicChange(0);
		time = 0;
		state = STATE_OVER;
		return PTURN;
	}

	scr.SetIntValue(SCR_RESERVEBEGIN, tsec);

	return PGO;
}