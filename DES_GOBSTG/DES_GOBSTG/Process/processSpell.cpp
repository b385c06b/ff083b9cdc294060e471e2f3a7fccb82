#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/Selector.h"
#include "../Header/SE.h"

int Process::processSpell()
{
	time++;
	if(time == 1)
	{
		scr.eventExecute(SCR_EVENT_ENTERSTATE, STATE_SPELL);
	}
	retvalue = PGO;
	scr.controlExecute(STATE_SPELL, time);
	return retvalue;
}