#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/Player.h"
#include "../Header/InfoSelect.h"
#include "../Header/Selector.h"
#include "../Header/Data.h"
#include "../Header/DataConnector.h"
#include "../Header/SE.h"
#include "../Header/BResource.h"

int Process::processPlayerSelect()
{
	gametime++;
	if(gametime == 1)
	{
		scr.eventExecute(SCR_EVENT_ENTERSTATE, STATE_PLAYER_SELECT);
	}
	retvalue = PGO;
	scr.controlExecute(STATE_PLAYER_SELECT, gametime);
	return retvalue;
}
