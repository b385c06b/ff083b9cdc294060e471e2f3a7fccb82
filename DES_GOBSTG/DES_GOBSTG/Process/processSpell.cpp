#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/Selector.h"
#include "../Header/SE.h"

int Process::processSpell()
{
	gametime++;
	if(gametime == 1)
	{
		Scripter::scr.eventExecute(SCR_EVENT_ENTERSTATE, STATE_SPELL);
	}
	retvalue = PGO;
	Scripter::scr.controlExecute(STATE_SPELL, gametime);
	return retvalue;
}