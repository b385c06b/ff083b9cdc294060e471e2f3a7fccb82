#ifndef _GHOST_H
#define _GHOST_H

#include "BObject.h"
#include "Effectsys.h"
#include "Target.h"

#define GHOSTMAX			0x80

#define GHOST_COLLISIONR	8

#define GHOST_IZER(LIFE, MAXLIFE)	((LIFE) / (MAXLIFE) * 32.0f)

class Ghost : public BObject
{
public:
	Ghost();
	virtual ~Ghost();

	static bool Build(WORD gID, BYTE tarID, BYTE belong, float x, float y, int angle, float speed, BYTE type, float life, int ac);
	static void Action();
	static void ClearAll();
	static void RenderAll();

	void valueSet(WORD ID, float x, float y, int angle, float speed, BYTE type, float life, int ac);
	void valueSet(WORD ID, BYTE belong, int angle, float speed, BYTE type, float life, int ac);
	void Clear();

	void CostLife(float power);

	void DoShot();

	virtual void action();
	void actionInStop();
	void Render();

	void SetAim(float aimx, float aimy);
	void SetValue(int angle, float speed, int ac);
	void SetLife(float life, float maxlife);
	void ChangeGID(WORD gID);

public:

	Effectsys effghost;

	Target aim;

	float	life;
	float	maxlife;
	float	cenx;
	float	ceny;
	bool	half;
	bool	gave;
	int		ac;
	bool	fadeout;
	bool	able;
	bool	damage;
	bool	lastborderstate;

	WORD	gID;

	BYTE	belong;
	BYTE	type;

	static WORD index;
	static Ghost gh[GHOSTMAX];
};

#endif
