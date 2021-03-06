#ifndef _EFFECTSP_H
#define _EFFECTSP_H

#include "BObject.h"

#define EFFECTSPMAX			0x80

#define	EFFECT_PLAYERCHANGE		0x01
#define	EFFECT_PLAYERSHOT		0x02
#define	EFFECT_PLAYERBORDER		0x03
#define EFFECT_PLAYERBORDERZONE	0x04
#define	EFFECT_PLAYERPOINT		0x05
#define	EFFECT_PLAYERCOLLAPSE	0x06

#define	EFFECT_BOMB_CUTIN		0x20

class EffectSp : public BObject
{
public:
	EffectSp();
	~EffectSp();

	static void ClearAll();
	void Clear();

	void valueSet(int siID, float x, float y, int angle, float speed, bool onplayer = true);
	void colorSet(DWORD color, int blend=-1);
	void scaleSet(float hscale, float vscale=0);
	void posSet(float x, float y);

	void action();
	void Render();

	float GetWidth();
	float GetHeight();

public:
	hgeSprite * sprite;

	int siID;
	bool onplayer;
	BYTE type;

	int blend;

	static BYTE evtype;
};

#endif