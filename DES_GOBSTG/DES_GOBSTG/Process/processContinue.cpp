#include "../Header/Process.h"
#include "../Header/Bullet.h"
#include "../Header/Player.h"
#include "../Header/Scripter.h"
#include "../Header/BGLayer.h"
#include "../Header/Selector.h"
#include "../Header/Replay.h"

int Process::processContinue()
{
	int tsec;
	if(Player::p.ncCont >= M_PL_CONTINUEMAX || practicemode)
		goto exit;

	if(!Scripter::scr.GetIntValue(SCR_RESERVEBEGIN))
	{
		BGLayer::ubg[UFGID_FGPAUSE].exist = true;
		BGLayer::ubg[UFGID_FGPAUSE].SetFlag(FG_PAUSEIN, FGMT_PAUSE);
		Scripter::scr.SetIntValue(SCR_RESERVEBEGIN+1, M_PL_CONTINUEMAX-Player::p.ncCont);

		hge->Channel_Pause(channel);
	}

	Scripter::scr.controlExecute(STATE_CONTINUE, SCRIPT_CON_INIT);
	//60 sec
	tsec = Scripter::scr.GetIntValue(SCR_RESERVEBEGIN);

	if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
	{
		BGLayer::ubg[UFGID_FGPAUSE].SetFlag(FG_PAUSEOUT, FGMT_PAUSE);
	}
	else if(hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN))
	{
		Selector::select = 1;
	}

	if(tsec == 0x10)
	{
		BGLayer::ubg[UFGID_FGPAUSE].exist = false;
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
		Replay::rpy.partFill(0xff);
		BGLayer::ubg[UFGID_FGPAUSE].exist = false;
//		musicChange(0);
		gametime = 0;
		state = STATE_OVER;
		return PTURN;
	}

	Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, tsec);

	return PGO;
}