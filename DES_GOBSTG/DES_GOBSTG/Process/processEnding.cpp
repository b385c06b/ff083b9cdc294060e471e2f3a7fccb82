#include "../Header/Process.h"
#include "../Header/BGLayer.h"
#include "../Header/Player.h"
#include "../Header/Scripter.h"
#include "../Header/Data.h"
#include "../Header/Selector.h"
#include "../Header/InfoSelect.h"
#include "../Header/FrontDisplay.h"

int Process::processEnding()
{
	gametime++;
	if (gametime == 1)
	{
		BGLayer::KillOtherLayer();

		Player::p.exist = false;
		BGLayer::ubg[UBGID_BGMASK].exist = false;
		FrontDisplay::fdisp.SetState(FDISP_PANEL, FDISPSTATE_OFF);
		musicChange(0, true);

		Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, 0);
		Scripter::scr.SetIntValue(SCR_RESERVEBEGIN+1, 0);
	}
	retvalue = PGO;
	retvalue = Scripter::scr.controlExecute(STATE_ENDING, gametime);
	//pushtimer depth
	int tpushtimer = Scripter::scr.GetIntValue(SCR_RESERVEBEGIN);
	int tdepth = Scripter::scr.GetIntValue(SCR_RESERVEBEGIN+1);

	if (tdepth == 0)
	{
	}
	else if (tdepth == 0xff)
	{
		Selector::Clear();
		InfoSelect::Clear();
		gametime = 0;
		state = STATE_OVER;
		return PTURN;
	}
	if(hge->Input_GetDIKey(KS_FIRE) || hge->Input_GetDIKey(KS_SPECIAL))
	{
		tpushtimer++;
		if(tpushtimer == 120)
		{
			tdepth = 0xff;
		}
	}
	else
	{
		tpushtimer = 0;
	}
	Scripter::scr.SetIntValue(SCR_RESERVEBEGIN, tpushtimer);
	Scripter::scr.SetIntValue(SCR_RESERVEBEGIN+1, tdepth);

	return retvalue;
}