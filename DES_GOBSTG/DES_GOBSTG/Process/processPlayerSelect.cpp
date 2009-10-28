#include "Process.h"
#include "Scripter.h"
#include "Player.h"
#include "InfoSelect.h"
#include "Selector.h"
#include "Data.h"
#include "DataConnector.h"
#include "SE.h"
#include "BResource.h"

int Process::processPlayerSelect()
{
	time++;
	if(time == 1)
	{
		scr.eventExecute(SCR_EVENT_ENTERSTATE, STATE_PLAYER_SELECT);
	}
	retvalue = PGO;
	scr.controlExecute(STATE_PLAYER_SELECT, time);
	return retvalue;
}
