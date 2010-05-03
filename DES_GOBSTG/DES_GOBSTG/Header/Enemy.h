#ifndef _ENEMY_H
#define _ENEMY_H

#include "BObject.h"
#include "EffectSp.h"
#include "Effectsys.h"
#include "Target.h"

#define ENEMYMAX			0x80

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

#define ENAC_DIRECTSET_XYSOAOOO		0x01
#define ENAC_CHASEPLAYER_OOSFATOO	0x02
#define ENAC_CHASEAIM_XYSOAOCO		0x03
#define ENAC_TURNANGLE_OOOOATOE		0x04

#define ENAC_FADEOUT_OOOOOTOO			0x40

#define ENAC_REPOSITION_OOOOOOCO		0x80
#define ENAC_OVERPLAYER_XYOOOTCE		0x81

#define ENAC_DELAY_OOOOOTOO			0xC0

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
	static void Init();
	static void Action();
	static void ClearAll();
	static void RenderAll();

	static void DamageZoneBuild(float x, float y, float r, float power);

	bool isInRange(float x, float y, float r);

	void Clear();
	void valueSet(WORD ID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer);

	void setTake(DWORD take);
	void setAction(WORD ac=ENAC_NONE, float para_x=0, float para_y=0, float para_speed=0, float para_friction=0, int para_angle=0, int para_time=0, int para_counter=0, int para_endtime=0);

	void Fadeout();

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

	void SetAim(float aimx, float aimy);
	void SetLife(float life, float maxlife);
	void ChangeEID(WORD eID);

public:
	Effectsys	effCollapse;
	Effectsys	effShot;

//	EffectSp	esCircle;

	hgeSprite * sprite;

	union{
		struct{
			float para_x;
			float para_y;
			float para_speed;
			float para_friction;
		};
		float fpara[ENEMY_PARAMAX];
	};
	union{
		struct{
			int para_angle;
			int para_time;
			int para_counter;
			int para_endtime;
		};
		int ipara[ENEMY_PARAMAX];
	};
//	float	para[ENEMY_PARAMAX];
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
	static BYTE bossflag[ENEMY_BOSSMAX];
	static BYTE spelluptimer[ENEMY_BOSSMAX];
	static BYTE storetimer[ENEMY_BOSSMAX];
	static WORD index;

	static Enemy en[ENEMYMAX];
};

#endif