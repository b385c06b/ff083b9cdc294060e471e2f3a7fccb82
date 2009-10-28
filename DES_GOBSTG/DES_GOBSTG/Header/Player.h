#ifndef _PLAYER_H
#define _PLAYER_H

#include "BObject.h"
#include "EffectSp.h"
#include "PlayerGhost.h"
#include "PlayerBullet.h"
#include "Effectsys.h"

#define PLAYER_FRAME_STAND		0
#define PLAYER_FRAME_LEFTPRE	1
#define PLAYER_FRAME_LEFT		2
#define PLAYER_FRAME_RIGHTPRE	3
#define PLAYER_FRAME_RIGHT		4

#define PLSHOTDELAY_ADD		12
#define PL_NPOPMAX			1200

#define PL_HITONFACTORDEFAULT	50

#define PL_ITEMDRAINY		144

#define PLAYER_FRAME_STATEMAX	5
#define PLAYER_ANIMATIONSPEED	8

#define PLAYER_BORDEROFFPRE	8

#define PL_DEFAULTNPOWER	300000
#define PL_DEFAULTNFAITH	10000
#define PL_DEFAULTNPLAYER	2
#define PL_NPLAYERMAX		7

#define PL_MOVABLEDGE_X		36
#define PL_MOVABLEDGE_Y		24
#define PL_MOVABLE_LEFT		(M_ACTIVECLIENT_LEFT+PL_MOVABLEDGE_X)
#define PL_MOVABLE_RIGHT	(M_ACTIVECLIENT_RIGHT-PL_MOVABLEDGE_X)
#define PL_MOVABLE_TOP		(M_ACTIVECLIENT_TOP+PL_MOVABLEDGE_Y)
#define PL_MOVABLE_BOTTOM	(M_ACTIVECLIENT_BOTTOM-PL_MOVABLEDGE_Y)

#define PL_MERGEPOS_X		M_ACTIVECLIENT_CENTER_X
#define PL_MERGEPOS_Y		(M_ACTIVECLIENT_BOTTOM + 40)

#define PL_SAVELASTMAX		0x20

#define	PLAYER_MERGE				0x1
#define	PLAYER_SHOT					0x2
#define	PLAYER_COLLAPSE				0x4
#define	PLAYER_SHOOT				0x8
#define	PLAYER_BORDER				0x10
#define	PLAYER_BOMB					0x20
#define	PLAYER_SLOWCHANGE			0x40
#define	PLAYER_FASTCHANGE			0x80
#define	PLAYER_PLAYERCHANGE			0x100
#define	PLAYER_GRAZE				0x200

#define PLBONUS_GRAZE	0x01
#define PLBONUS_SHOOT	0x02
#define PLBONUS_TIME	0x04

class Player : public BObject
{
public:
	Player();
	virtual ~Player();

	void valueSet(WORD ID, WORD ID_sub_1, WORD ID_sub_2, BYTE nLife=PL_DEFAULTNPLAYER, bool bContinue = false);
	void ClearSet();
	void UpdatePlayerData();

	virtual void action();

	void RenderEffect();

	bool Merge();
	bool Shot();
	bool Collapse();
	bool Shoot();
	bool Border();
	bool Bomb();
	bool SlowChange();
	bool FastChange();
	bool PlayerChange();
	bool Graze();

	DWORD getnNext();

	void initFrameIndex();
	void setFrame(BYTE frameenum);
	BYTE getFrameIndex(BYTE frameenum);
	void setIndexFrame(BYTE index);
	void updateFrame(BYTE frameenum, int usetimer = -1);

	void callCollapse();
	bool callBomb(bool onlyborder = false);
	void callSlowFastChange(bool toslow);
	void callPlayerChange();

	void _Shoot();
	void _Bomb();

	void bombAction();

	void Render();

	LONGLONG getClearBonusPoint();
	LONGLONG getClearBonusGraze();
	LONGLONG getClearBonusFaith();
	LONGLONG getClearBonusStage(int nstage);
	LONGLONG getAllClearBonusLife();
	LONGLONG getAllClearBonusPower();

	LONGLONG getItemBonus(WORD itemtype);

	void DoGraze(float x, float y);
	void DoPlayerBulletHit(int hitonfactor = PL_HITONFACTORDEFAULT);
	void DoShot();

	bool HavePlayer(WORD ID);

	void changePlayerID(WORD toID, bool moveghost=false);

	float TranslatePower(float bulletpower);

	int GrazeRegain(int grazenum);
	void GetScoreLife(float maxlife, bool isenemy = true);

	void AddPower(int power);

public:
	WORD	ID_sub_1;
	WORD	ID_sub_2;
	WORD	nowID;

	bool	flipx;
	BYTE	frameoffset;
	BYTE	nowstate;

	int getspell[M_GETSPELLMAX];
	float	lastx[PL_SAVELASTMAX];
	float	lasty[PL_SAVELASTMAX];
	float	lastmx[PL_SAVELASTMAX];
	float	lastmy[PL_SAVELASTMAX];

	PlayerGhost pg[PLAYERGHOSTMAX];

	EffectSp	esBorder;
	EffectSp	esBorderZone;
	EffectSp	esChange;
	EffectSp	esShot;
	EffectSp	esPoint;
	EffectSp	esCollapse;

	Effectsys	effGraze;
	Effectsys	effChange;
	Effectsys	effInfi;
	Effectsys	effCollapse;
	Effectsys	effMerge;
	Effectsys	effBorder;
	Effectsys	effBorderOn;
	Effectsys	effBorderOff;

	LONGLONG	nScore;
	LONGLONG	nHiScore;
	DWORD	nPoint;
	DWORD	nGraze;
	DWORD	nFaith;
	DWORD	nNext;
	DWORD	nPower;

	DWORD	nLastPoint;
	DWORD	nLastGraze;
	DWORD	nLastFaith;

	float	lostStack;
	DWORD	borderCounter;
	DWORD	fastCounter;

	float	slowspeed;
	float	speedfactor;
	float	r;
	float	graze_r;
	float	fPoprate;
	float	fPopratebase;

	int		nPop;
	int		bombperpower;

	int		bordergraze;

	bool	bBomb;
	bool	bBorder;
	bool	bSlow;
	bool	bInfi;

	bool	borderShot;

	hgeSprite * sprite;
	BYTE	frameindex[M_PL_ONESETPLAYER][PLAYER_FRAME_STATEMAX];
	BYTE	faceindex[M_PL_ONESETPLAYER];

	WORD	flag;

	WORD	mergetimer;
	WORD	shottimer;
	WORD	collapsetimer;
	WORD	shoottimer;
	WORD	bordertimer;
	WORD	bombtimer;
	WORD	slowtimer;
	WORD	fasttimer;
	WORD	playerchangetimer;

	WORD	borderlast;
	WORD	bomblast;
	BYTE	shotdelay;

	BYTE	nLife;

	BYTE	bonusflag;

	static Player p;
	static	int		ncBorder;
	static	BYTE	ncMiss;
	static	BYTE	ncCont;
	static	BYTE	ncGet;
	static	BYTE	ncPause;

};
#endif