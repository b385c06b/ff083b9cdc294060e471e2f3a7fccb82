#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/Selector.h"
#include "../Header/Player.h"
#include "../Header/InfoSelect.h"
#include "../Header/SE.h"
#include "../Header/BGLayer.h"
#include "../Header/BossInfo.h"
#include "../Header/FrontDisplay.h"

int Process::processPause()
{
	BYTE tstate = state;
	if(!pauseinit)
	{
		hge->Channel_Pause(channel);

		Scripter::scr.d[SCR_RESERVEBEGIN].bfloat = false;
		Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0x00);
		pauseinit = true;

		if(replaymode)
		{
			if (replayend)
			{
				gametime = 0;
			}
		}
		else if (Player::p.exist && Player::ncPause < 0xff)
		{
			Player::ncPause++;
		}
	}
	if(Scripter::scr.GetIntValue(SCR_RESERVEBEGIN) < 0x100)
		Scripter::scr.controlExecute(STATE_PAUSE, SCRIPT_CON_INIT);

	if(Selector::sel.size() && hge->Input_GetDIKey(KS_SPECIAL, DIKEY_UP))
	{
		Selector::Clear();
		if(Scripter::scr.GetIntValue(SCR_RESERVEBEGIN) == 0x10)
		{
			Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0xff);
			if(replaymode && replayend || spellmode && !replaymode && gametime == 0)
				state = STATE_TITLE;
			else
				state = STATE_START;
		}
		else
			Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0x01);
		SE::push(SE_SYSTEM_CANCEL);
//		state = STATE_START;
	}
	if(hge->Input_GetDIKey(KS_PAUSE, DIKEY_DOWN))
	{
		Selector::Clear();
		Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0xff);
		SE::push(SE_SYSTEM_CANCEL);
		if(replaymode && replayend)
			state = STATE_TITLE;
		else
			state = STATE_START;
	}

	if(Scripter::scr.GetIntValue(SCR_RESERVEBEGIN) == 0xff)
	{
		BGLayer::ubg[UFGID_FGPAUSE].SetFlag(FG_PAUSEOUT, FGMT_PAUSE);
		Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0x100|state);
		state = STATE_PAUSE;
	}
	else if(Scripter::scr.GetIntValue(SCR_RESERVEBEGIN) >= 0x100 && !BGLayer::ubg[UFGID_FGPAUSE].flag)
	{
		BGLayer::ubg[UFGID_FGPAUSE].exist = false;
		pauseinit = false;
		state = Scripter::scr.GetIntValue(SCR_RESERVEBEGIN) & 0xff;
		if(state == STATE_START)
		{
			if(gametime == 0)
			{
				scene = startscene;
				startPrep();
				return PTURN;
			}
			hge->Input_SetDIKey(KS_LEFT, (bool)(saveInput & 0x1));
			hge->Input_SetDIKey(KS_RIGHT, (bool)(saveInput & 0x2));
			hge->Input_SetDIKey(KS_UP, (bool)(saveInput & 0x4));
			hge->Input_SetDIKey(KS_DOWN, (bool)(saveInput & 0x8));
			hge->Input_SetDIKey(KS_FIRE, (bool)(saveInput & 0x10));
			hge->Input_SetDIKey(KS_SPECIAL, (bool)(saveInput & 0x20));
			hge->Input_SetDIKey(KS_CHANGE, (bool)(saveInput & 0x40));
			hge->Input_SetDIKey(KS_SLOW, (bool)(saveInput & 0x80));

			if (musicID >= 0)
			{
				hge->Channel_SetVolume(channel, 0);
				hge->Channel_Resume(channel);
				musicSlide(1.5f, -1);
			}

			return PTURN;
		}
		else if(state == STATE_TITLE)
		{
			gametime = 0;
			if(replaymode)
			{
				BGLayer::KillOtherLayer();
				Selector::Clear();
				InfoSelect::Clear();
				BGLayer::ubg[UBGID_BGMASK].exist = false;
				FrontDisplay::fdisp.SetState(FDISP_PANEL, FDISPSTATE_OFF);
				Player::p.exist = false;
				BossInfo::bossinfo.Clear();
				getInput();
				replaymode = false;
				state = STATE_REPLAY;
				return PTURN;
			}
			if(practicemode || spellmode)
			{
				BGLayer::KillOtherLayer();
				Selector::Clear();
				InfoSelect::Clear();
				BGLayer::ubg[UBGID_BGMASK].exist = false;
				FrontDisplay::fdisp.SetState(FDISP_PANEL, FDISPSTATE_OFF);
				BossInfo::bossinfo.Clear();
				if(spellmode && !replaymode && gametime == 0)
					state = STATE_CONTINUE;
				else
					state = STATE_DIFFICULT_SELECT;
			}
			scene = startscene;
			Player::p.exist = false;
//			startPrep();
			return PTURN;
		}
	}
	return PGO;
}
