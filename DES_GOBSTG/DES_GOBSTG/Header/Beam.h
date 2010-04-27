#ifndef _BEAM_H
#define _BEAM_H

#include "BObject.h"

#define	BEAMMAX				0x100

#define BEAM_INVALIDSCALE	0.5f

#define BEAMFLAG_NONE		0x00
#define BEAMFLAG_HORIZON	0x01
#define BEAMFLAG_STOP		0x02
#define BEAMFLAG_HS			(BEAMFLAG_HORIZON + BEAMFLAG_STOP)
#define BEAMFLAG_NOGRAZE	0x10

class Beam : public BObject
{
public:
	Beam();
	~Beam();

	static void Init();
	static int Build(float x, float y, int angle, float speed, BYTE type, BYTE color, float length, float width, BYTE flag, int fadeintime, int fadeouttime, BYTE tarID);
	static void Action();
	static void ClearAll();
	static void RenderAll();

#ifdef __DEBUG
	static void Debug_RenderCollision();
#endif

	void valueSet(WORD ID, float x, float y,int angle, float speed, BYTE type, BYTE color, float length, float width, BYTE flag, int fadeintime, int fadeouttime = 0, BYTE tarID = 0xff);
	bool isInRect(float aimx, float aimy, float r);

	void SetVector(const BObject &ori, const BObject &aim);
	void SetVector(float orix, float oriy, float aimx, float aimy);

	void SetHold(BYTE holdtar, float holdoffset);
	void SetHold(BYTE holdtar, BYTE pintar, float holdoffset);

	virtual void action();

	void Render();

public:
	float	xplus;
	float	yplus;
	float	lastspeed;
	float	holdoffset;
	int		lastangle;
	int		fadeintime;
	int		fadeouttime;

	bool	fadeout;
	bool	able;

	float	length;
	float	width;

	float	texw;
	float	texh;

	BYTE	flag;
	BYTE	type;
	BYTE	color;
	BYTE	grazetimer;
	BYTE	holdtar;
	BYTE	pintar;

	static WORD index;
	static VectorList<Beam> be;
};

#endif