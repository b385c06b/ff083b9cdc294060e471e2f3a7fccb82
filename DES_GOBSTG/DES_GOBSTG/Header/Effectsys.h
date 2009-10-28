#ifndef _EFFECTSYS_H
#define _EFFECTSYS_H

#include "MainDependency.h"
#include "Const.h"
#include "BObject.h"

#define EFFSYS_AUTOFADEOUT_TIME	120

class Effectsys : public BObject
{
public:
	Effectsys();
	virtual ~Effectsys();

	static bool Init(HTEXTURE * tex, const char * foldername, char name[][M_PATHMAX]);

	void valueSet(WORD ID, float x, float y, int lifetime = -1);
	void valueSet(WORD ID, int lifetime, float x, float y, float z, BYTE tarID, int angle, float speed, float zSpeed);
	void valueSet(WORD ID, int lifetime, float x, float y, BYTE tarID, int chasetimer, BYTE tarAim);
	void valueSet(WORD ID, BObject & owner);

	void MoveTo(float x, float y, float z = 0, bool bForce = false);
	void Fire();
	void Stop(bool bKill = false);
	void action();

	void Render();

public:
	hgeEffectSystem * eff;

	int lifetime;
	int chasetimer;
	float z;
	float zSpeed;

	BYTE tarAim;

	static hgeEffectSystem efftype[EFFECTSYSTYPEMAX];
};

extern Effectsys effsys[EFFECTSYSMAX];

#endif