#ifndef _SELECTOR_H
#define _SELECTOR_H

#include "MainDependency.h"
#include "Const.h"

#define SEL_STATEMAX	0x04

#define	SEL_NONE		0x00
#define	SEL_OVER		0x01
#define	SEL_ENTER		0x02
#define	SEL_LEAVE		0x04

#define SEL_INFOMASK	0x0f
#define SEL_STATEMASK	0xf0

#define SELINFO_NONE	0
#define SELINFO_OVER	1
#define SELINFO_ENTER	2
#define SELINFO_LEAVE	3

#define SELOP_EQUAL		1
#define SELOP_SET		2
#define SELOP_UNSET		4

#define SELFADE_UP		1
#define SELFADE_DOWN	2
#define SELFADE_LEFT	4
#define SELFADE_RIGHT	8


struct selinfo
{
	float xadj;
	float yadj;
};

class Selector
{
public:
	Selector();
	~Selector();

	void valueSet(BYTE ID, int siID, float cenx, float ceny, float hscale, float vscale, BYTE maxtime);
	void actionSet(BYTE setflag, float xadj, float yadj);

	static void Build(BYTE ID, int siID, float cenx, float ceny, float hscale, float vscale, BYTE maxtime,
		float xadj0, float yadj0,
		float xadj1, float yadj1,
		float xadj2, float yadj2,
		float xadj3, float yadj3,
		BYTE flag=SEL_NULL);

	static bool confirm(float cenx=M_ACTIVECLIENT_CENTER_X, float ceny=M_ACTIVECLIENT_CENTER_Y, bool settrue=false);
	static void Clear();

	void action();
	void changeState(BYTE state, BYTE op);
	void matchSelect();

	static void Setup(int nselect, int select, bool updown=true);

	static void shift(int nshift);
	static bool SetPageNum(BYTE nPageNum, float fadebegin, float offset, int initshift=0);
	static void Render();

	static Selector * GetPointer(int index);

public:
	selinfo	info[SEL_STATEMAX];

	float	x;
	float	y;
	float	hscale;
	float	vscale;

	hgeSprite * sprite;

	BYTE	ID;
	BYTE	flag;
	BYTE	timer;
	BYTE	maxtime;
	BYTE	alpha;

	static int nselect;
	static int select;
	static bool updown;

	static int sellock;
	static bool complete;
	static bool plus;

	static int nPageNum;
	static int firstID;
	static int lastID;
	static float fadebegin;
	static float offset;

	static bool confirminit;

	static bool avoid;
};

extern list<Selector> sel;

#endif