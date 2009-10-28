#include "Process.h"
#include "BGLayer.h"
#include "Player.h"
#include "Scripter.h"
#include "Data.h"
#include "Selector.h"
#include "InfoSelect.h"
#include "FrontDisplay.h"

int Process::processEnding()
{
	time++;
	if (time == 1)
	{
		BGLayer::KillOtherLayer();

		Player::p.exist = false;
		bgmask.exist = false;
		fdisp.SetState(FDISP_PANEL, 0);
		musicChange(0, true);

		scr.SetIntValue(SCR_RESERVEBEGIN, 0);
		scr.SetIntValue(SCR_RESERVEBEGIN+1, 0);
	}
	retvalue = PGO;
	retvalue = scr.controlExecute(STATE_ENDING, time);
	//pushtimer depth
	int tpushtimer = scr.GetIntValue(SCR_RESERVEBEGIN);
	int tdepth = scr.GetIntValue(SCR_RESERVEBEGIN+1);

	if (tdepth == 0)
	{
	}
	else if (tdepth == 0xff)
	{
		Selector::Clear();
		InfoSelect::Clear();
		time = 0;
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
	scr.SetIntValue(SCR_RESERVEBEGIN, tpushtimer);
	scr.SetIntValue(SCR_RESERVEBEGIN+1, tdepth);

	return retvalue;
}