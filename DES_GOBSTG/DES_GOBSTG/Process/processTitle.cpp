#include "Process.h"
#include "Scripter.h"
#include "Selector.h"
#include "SE.h"

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