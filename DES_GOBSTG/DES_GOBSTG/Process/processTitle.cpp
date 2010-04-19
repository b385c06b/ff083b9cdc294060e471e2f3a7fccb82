#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/Selector.h"
#include "../Header/SE.h"

int Process::processTitle()
{
	gametime++;
	if(gametime == 1)
	{
		reload();
		Selector::select = titleselect;
	}
	retvalue = PGO;
	Scripter::scr.controlExecute(STATE_TITLE, gametime);
	titleselect = Selector::select;
	return retvalue;
}