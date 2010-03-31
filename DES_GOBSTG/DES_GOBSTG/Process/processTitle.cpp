#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/Selector.h"
#include "../Header/SE.h"

int Process::processTitle()
{
	time++;
	if(time == 1)
	{
		reload();
		Selector::select = titleselect;
	}
	retvalue = PGO;
	scr.controlExecute(STATE_TITLE, time);
	titleselect = Selector::select;
	return retvalue;
}