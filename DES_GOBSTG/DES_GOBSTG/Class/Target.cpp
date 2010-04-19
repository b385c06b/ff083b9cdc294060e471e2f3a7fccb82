#include "../header/Target.h"

Target Target::tar[TARGETMAX];

void Target::ClearAll()
{
	for (int i=0; i<TARGETMAX; i++)
	{
		SetValue(i, 0, 0);
	}
}

void Target::SetValue(int index, float x, float y)
{
	if (index >=0 && index < TARGETMAX)
	{
		tar[index].x = x;
		tar[index].y = y;
	}
}

bool Target::GetValue(int index, float * x, float * y)
{
	if (x)
	{
		*x = GetX(index);
		if (y)
		{
			*y = GetY(index);
			return true;
		}
	}
	return false;
}

float Target::GetX(int index)
{
	if (index >=0 && index < TARGETMAX)
	{
		return tar[index].x;
	}
	return 0;
}

float Target::GetY(int index)
{
	if (index >=0 && index < TARGETMAX)
	{
		return tar[index].y;
	}
	return 0;
}