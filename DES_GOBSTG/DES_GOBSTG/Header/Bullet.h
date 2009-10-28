#ifndef _BULLET_H
#define _BULLET_H

#include "BObject.h"


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
#define BULLET_COLLISION_SQURE		3

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

#define THEN -1000000

#define BUAL_CONDITIONSTART	0x0000

#define BUALC_FILTER	0xf0

#define BULAC_OTHER		0x00

#define	SECTIONEND		0x0

#define	EVERY			0x1
#define	EVERYMOD		0x2

#define BULAC_TIMER		0x10

#define	TIMERGREAT		0x11
#define	TIMEREQUAL		0x12
#define	TIMERLESS		0x13
#define	TIMERRANGE		0x14

#define BULAC_TYPE		0x20

#define	TYPEEQUAL		0x21

#define BULAC_COLOR		0x30

#define	COLOREQUAL		0x31

#define BULAC_ANGLE		0x40

#define	ANGLEGREAT		0x41
#define	ANGLEEQUAL		0x42
#define	ANGLELESS		0x43
#define	ANGLERANGE		0x44

#define BULAC_POS		0x50

#define	XGREAT			0x51
#define	XLESS			0x52
#define	XRANGE			0x53
#define	YGREAT			0x58
#define	YLESS			0x59
#define	YRANGE			0x5A

#define BULAC_VAL		0x60

#define VALGREAT		0x61
#define VALEQUAL		0x62
#define VALLESS			0x63
#define VALRANGE		0x64

#define BULAC_SPEED		0x70

#define	SPEEDGREAT		0x71
#define	SPEEDEQUAL		0x72
#define	SPEEDLESS		0x73
#define	SPEEDRANGE		0x74

#define BULAC_INDEX		0x80

#define INDEXMODGREAT	0x81
#define INDEXMODEQUAL	0x82
#define INDEXMODLESS	0x83
#define INDEXMODRANGE	0x84

#define BUAL_EXECUTESTART	0x1000

#define BULAE_FILTER	0xf000

#define BULAE_TYPE		0x1000

#define	TYPESET			0x1100

#define BULAE_COLOR		0x2000

#define	COLORSET		0x2100

#define BULAE_ANGLE		0x3000

#define	ANGLESET		0x3100
#define ANGLESETADD		0x3200
#define	ANGLEADJUST		0x3300
#define	ANGLESETRMA		0x3400
#define	ANGLESETRMAP	0x3500
#define ANGLESETRMAT	0x3600
#define	ANGLESETAMA		0x3700
#define	ANGLESETAMAP	0x3800
#define ANGLESETAMAT	0x3900

#define BULAE_HEADANGLE	0x4000

#define	HEADANGLESET	0x4100
#define	HEADANGLESETADD	0x4200

#define BULAE_POS		0x5000

#define	XSET			0x5100
#define	YSET			0x5200
#define XSETADD			0x5300
#define	YSETADD			0x5400
#define	XSETACCADD		0x5500
#define	YSETACCADD		0x5600

#define BULAE_VAL		0x6000

#define VALSET			0x6100
#define VALSETADD		0x6200

#define BULAE_SPEED		0x7000

#define	SPEEDSET		0x7100
#define	SPEEDSETADD		0x7200

#define BULAE_OTHER		0x8000

#define CALLEVENT		0x8100
#define CHASE			0x8200
#define	REMAIN			0x8300
#define FADEOUT			0x8400
#define FAITHLIZE		0x8500
#define POINTLIZE		0x8600
#define SCORELIZE		0x8700
#define BOUNCE			0x8800

#define BULA_SPECIALSTART	0x00

#define	AND				-0x1
#define	OR				-0x2
#define	NOT				-0x3


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

	bool valueSet(WORD ID, float x, float y, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid = 0, BYTE tarID = 0xff);
	inline void valueSet(WORD ID, const BObject & from, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid = 0, BYTE tarID = 0xff)
	{
		valueSet(ID, from.x, from.y, absolute, angle, speed, type, color, fadeinTime, avoid, tarID);
	}

	static bool Build(float x, float y, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid, BYTE tarID);
	static void BuildCircle(int num, int baseangle, float baser, float x, float y, float speed, BYTE type, BYTE color, int fadeinTime, float avoid);
	static void BuildLine(int num, int baseangle, float space, int baseindex, float x, float y, int angle, float anglefactor, float speed, float speedfactor, BYTE type, BYTE color, int fadeinTime, float avoid);

	void matchFadeInColorType();
	void matchFadeOutColorType();

	void faithlize();
	void pointlize();
	void scorelize();

	bool passedEvent(BYTE eventID);
	void passEvent(BYTE eventID);

	void ChangeAction();

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

	BYTE	type;
	BYTE	oldtype;
	BYTE	color;
	BYTE	oldcolor;
	BYTE	toafter;
	BYTE	typechangetimer;
	BYTE	eventID[BULLET_EVENTMAX];

	static RenderDepth renderDepth[BULLETTYPEMAX];
	static Bullet _bu;
	static hgeSprite * sp[BULLETTYPECOLORMAX];
	static VectorList<IzeZone> izel;
	static HTEXTURE tex;
	static WORD index;
};
extern VectorList<Bullet>bu;

#endif