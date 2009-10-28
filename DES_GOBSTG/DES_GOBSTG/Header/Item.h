#ifndef _ITEM_H
#define _ITEM_H

#include "BObject.h"

class Player;

#define ITEMINFOFONTMAX		0x40

#define ITEMTYPEMAX			0x08
#define ITEMSPRITEMAX		ITEMTYPEMAX * 2

#define ITEM_STARTSPEED		-5

#define	ITEM_POWER			0x0
#define	ITEM_POINT			0x1
#define	ITEM_FAITH			0x2
#define	ITEM_BIGPOWER		0x3
#define	ITEM_BOMB			0x4
#define	ITEM_FULL			0x5
#define	ITEM_EXTEND			0x6
#define	ITEM_SMALLFAITH		0x7

struct infoFont
{
	char	cScore[M_STRITOAMAX];
	float	x;
	float	y;
	bool	yellow;
	BYTE	timer;
};

class Item : public BObject
{
public:
	Item();
	virtual ~Item();

	static void Init();

	static void Build(WORD type, float x, float y, bool bDrained = false, int angle = 9000, float speed = ITEM_STARTSPEED);

	void valueSet(WORD type, float x, float y, bool bDrained = false, int angle = 9000, float speed = ITEM_STARTSPEED);

	static void ChangeItemID(WORD oriID, WORD toID);
	static void Release();

	void Render();

	virtual void action();

	static void undrainAll();
	static void drainAll();

public:
	bool bDrained;
	bool bFast;

	static hgeSprite *spItem[ITEMSPRITEMAX];
	static VectorList<infoFont>infofont;
};

extern VectorList<Item>mi;

#endif