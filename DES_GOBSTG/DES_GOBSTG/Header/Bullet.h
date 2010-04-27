#ifndef _BULLET_H
#define _BULLET_H

#include "BObject.h"

#define BULLETMAX			0x3000
#define BULLETCOLORMAX		0x10
#define BULLETTYPECOLORMAX	(BULLETTYPEMAX * BULLETCOLORMAX)

#define BULLETACTIONMAX		0x20
#define BULLETIZELISTMAX	0x04

#define IZEZONE_DEFAULTTIME	0x20

#define BULLET_FADEINTYPE	(BULLETTYPEMAX - 3)
#define BULLET_FADEOUTTYPE	(BULLETTYPEMAX - 2)
#define BULLET_BONUSTYPE	(BULLETTYPEMAX - 1)
#define BULLET_ANGLEOFFSET	9000

#define BULLET_FADECOLOR_16	160
#define BULLET_FADECOLOR_8	80

#define BULLET_COLLISION_NONE		0
#define BULLET_COLLISION_CIRCLE		1
#define BULLET_COLLISION_ELLIPSE	2
#define BULLET_COLLISION_RECT		3
#define BULLET_COLLISION_SQURE		4

#define BULLET_ANIMATIONSPEED	4

#define BULLET_TYPECHANGETIME 16

#define BULLETIZE_EVENT		0
#define BULLETIZE_FADEOUT	1
#define BULLETIZE_FAITH		2
#define BULLETIZE_SCORE		3
#define BULLETIZE_POINT		4
#define BULLET_IZEOVERZONE	((M_ACTIVECLIENT_WIDTH + M_ACTIVECLIENT_HEIGHT) / 2)
#define BULLET_EVENTMAX		4

#define BULLETACT_FLOATSCALE	100.0f
#define BULLETACT_ANGLECHANGESE	3000

typedef struct tagIzeZone{
	float	x;
	float	y;
	float	r;
	BYTE	timer;
	BYTE	maxtime;
	BYTE	type;
	BYTE	eventID;
}IzeZone;

typedef struct tagRenderDepth{
	int startIndex;
	int endIndex;
	bool haveType;
}RenderDepth;

class Bullet : public BObject
{
public:
	Bullet();
	virtual ~Bullet();

	static void Init(HTEXTURE tex);
	static void Release();
	static void Action();
	static void IzelAction();
	static void ClearAll();
	static void RenderAll();
	void Render();

	virtual void action();
	void actionInStop();

	void DoIze();
	void DoCollision();
	void DoGraze();
	void DoUpdateRenderDepth();

	bool HaveGray();

	BYTE getRenderDepth();

	bool isInRect(float r,float aimx,float aimy);
	static void IzeBuild(BYTE type, float x, float y, BYTE maxtime=IZEZONE_DEFAULTTIME, float r=BULLET_IZEOVERZONE, BYTE eventID=0xff);

	bool valueSet(WORD ID, float x, float y, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid = 0, BYTE tarID = 0xff);
	void changeType(BYTE totype);

	static int Build(float x, float y, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid, BYTE tarID);
	static void BuildCircle(int num, int baseangle, float baser, float x, float y, float speed, BYTE type, BYTE color, int fadeinTime, float avoid);
	static void BuildLine(int num, int baseangle, float space, int baseindex, float x, float y, int angle, float anglefactor, float speed, float speedfactor, BYTE type, BYTE color, int fadeinTime, float avoid);

	void matchFadeInColorType();
	void matchFadeOutColorType();

	void faithlize();
	void pointlize();
	void scorelize();

	bool passedEvent(BYTE eventID);
	void passEvent(BYTE eventID);

	bool ChangeAction();

public:
	int		actionList[BULLETACTIONMAX];

	float	xplus;
	float	yplus;
	float	lastx;
	float	lasty;
	float	lastspeed;
	int		lastangle;
	int		fadeinTime;

	bool	fadeout;
	bool	able;
	bool	grazed;
	bool	remain;
	bool	cancelable;

	BYTE	bouncetime;

	BYTE	type;
	BYTE	oldtype;
	BYTE	color;
	BYTE	oldcolor;
	BYTE	toafter;
	BYTE	typechangetimer;
	BYTE	eventID[BULLET_EVENTMAX];

	static RenderDepth renderDepth[BULLETTYPEMAX];
	static Bullet _bu;
	static hgeSprite * sprite[BULLETTYPECOLORMAX];
	static VectorList<IzeZone> izel;
	static HTEXTURE tex;
	static WORD index;

	static VectorList<Bullet>bu;
};

#endif