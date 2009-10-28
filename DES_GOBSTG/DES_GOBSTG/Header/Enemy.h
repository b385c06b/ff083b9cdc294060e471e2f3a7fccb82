#ifndef _ENEMY_H
#define _ENEMY_H

#include "BObject.h"
#include "EffectSp.h"
#include "Effectsys.h"

#define ENEMY_INDEXSTART	4
#define ENEMY_MAINBOSSINDEX	0

#define ENEMY_TEXMAX		4
#define ENEMY_PARAMAX		4

#define ENEMY_BOSSMAX		ENEMY_TEXMAX

#define ENEMY_BOSSTYPEBEGIN	20

#define ENEMY_ANIMATIONSPEED	8

#define ENEMY_BOSSINFITIMER		240
#define ENEMY_BOSSMOVELIMIT		0.01f
#define ENEMY_ENEMYMOVELIMIT	0.2f

#define ENEMY_FRAME_STAND		0
#define ENEMY_FRAME_RIGHTPRE	1
#define ENEMY_FRAME_RIGHT		2
#define ENEMY_FRAME_LEFTPRE		3
#define ENEMY_FRAME_LEFT		4
#define ENEMY_FRAME_ATTACKPRE	5
#define ENEMY_FRAME_ATTACK		6
#define ENEMY_FRAME_STOREPRE	7
#define ENEMY_FRAME_STORE		8

#define ENEMY_FRAME_STATEMAX	9

//#define ENEMY_COLLISIONR		8

#define ENEMY_BOSSX_FADERANGE	32

#define	BOSS_ATTACK		0x1
#define BOSS_SPELLUP	0x2

#define	BOSS_STORE		0x11
#define BOSS_BREAK		0x12
#define BOSS_COLLAPSE	0x13

#define ENAC_NONE			0x00

#define ENAC_DIRECTSET_XYAS		0x01
#define ENAC_CHASEPLAYER_TFR	0x02
#define ENAC_ATTENUATION_TFER	0x03
#define ENAC_CIRCLE_TXYE		0x04
#define ENAC_BROKENLINE_CATE	0x05
#define ENAC_STOPANDSTRIKE_TEA	0x06

#define ENAC_REPOSITION_T		0x80
#define ENAC_OVERPLAYER_CXYT	0x81
#define ENAC_CHASETO_CXY		0x82

struct DamageZone 
{
	float x;
	float y;
	float r;
	float power;
};

class Enemy : public BObject
{
public:
	Enemy();
	virtual ~Enemy();

	static bool Build(WORD eID, BYTE index, BYTE tarID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer, DWORD take);

	static void Init(HTEXTURE texmain);

	static void DamageZoneBuild(float x, float y, float r, float power);

	bool isInRange(float x, float y, float r);

	void valueSet(WORD ID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer, DWORD take,
		WORD ac=0, float para0 = 0, float para1 = 0, float para2 = 0, float para3 = 0);

	void setMove(float para0, float para1, float para2, float para3 = 0, WORD ac = 42);

	void initFrameIndex();
	void setFrame(BYTE frameenum);
	BYTE getFrameIndex(BYTE frameenum);
	void setIndexFrame(BYTE index);
	void updateFrame(BYTE frameenum, int usetimer = -1);
	void updateFrameAsMove();

	void DoShot();

	virtual void action();
	void actionInStop();
	void matchAction();
	void bossAction();

	void GetCollisionRect(float * w, float * h);
	void CostLife(float power);

	void giveItem();

	void Render();
	void RenderEffect();

public:
	Effectsys	effCollapse;
	Effectsys	effShot;

//	EffectSp	esCircle;

	hgeSprite * sprite;

	float	para[ENEMY_PARAMAX];
	Target	aim;
	float	lastx;
	float	life;
	float	defrate;
	DWORD	take;

	bool	fadeout;
	bool	able;
	bool	damage;
	int		infitimer;
	float	maxlife;

	WORD	eID;
	WORD	ac;
	BYTE	type;
	BYTE	damagetimer;

	bool	flipx;
	BYTE	frameoffset;
	BYTE	nowstate;
	BYTE	faceindex;

	BYTE	frameindex[ENEMY_FRAME_STATEMAX];

	static VectorList<DamageZone> dmgz;
	static HTEXTURE texmain;
	static BYTE bossflag[ENEMY_BOSSMAX];
	static BYTE spelluptimer[ENEMY_BOSSMAX];
	static BYTE storetimer[ENEMY_BOSSMAX];
	static WORD index;
};

extern Enemy en[ENEMYMAX];

#endif