#ifndef _EFFECTSP_H
#define _EFFECTSP_H

#include "BObject.h"

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

	void valueSet(BYTE type, float x, float y, int angle, float speed, bool onplayer = true, WORD ID = 0);
	void colorSet(DWORD color);

	void action();
	void Render();

	float GetWidth();
	float GetHeight();

public:
	hgeSprite * sprite;

	bool onplayer;
	BYTE type;

	static BYTE evtype;
};

extern EffectSp es[EFFECTSPMAX];

#endif