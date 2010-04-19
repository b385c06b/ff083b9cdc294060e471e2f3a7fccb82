#ifndef _TARGET_H
#define _TARGET_H
#include "Const.h"

#define TARGETMAX			0x40

class Target
{
public:
	static void SetValue(int index, float x, float y);
	static bool GetValue(int index, float * x, float * y);
	static float GetX(int index);
	static float GetY(int index);
	static void ClearAll();

public:
	float x;
	float y;

public:
	static Target tar[TARGETMAX];
};

#endif