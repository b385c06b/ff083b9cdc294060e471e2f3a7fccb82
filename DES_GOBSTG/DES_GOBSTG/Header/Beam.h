#ifndef _BEAM_H
#define _BEAM_H

#include "BObject.h"

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
	virtual ~Beam();

	static void Init();
	static bool Build(float x, float y, int angle, float speed, BYTE type, BYTE color, WORD length, BYTE flag, int fadeouttime, BYTE tarID);

	void valueSet(WORD ID, float x, float y,int angle, float speed, BYTE type, BYTE color, WORD length, BYTE flag, int fadeouttime = 0, BYTE tarID = 0xff);
	bool isInRect(float r, float aimx, float aimy);

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
	int		fadeouttime;

	bool	fadeout;
	bool	able;

	WORD	length;
	BYTE	flag;
	BYTE	type;
	BYTE	color;
	BYTE	grazetimer;
	BYTE	holdtar;
	BYTE	pintar;

	static WORD index;
};

extern VectorList<Beam> be;

#endif