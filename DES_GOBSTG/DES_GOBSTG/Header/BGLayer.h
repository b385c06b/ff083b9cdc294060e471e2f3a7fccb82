#ifndef _BGLAYER_H
#define _BGLAYER_H

#include "MainDependency.h"
#include "Const.h"
#include "BObject.h"

#define BGLAYERMAX			0x0C
#define FGLAYERMAX			0x04

#define BGFGLAYERMAX		(BGLAYERMAX+FGLAYERMAX)
#define UBGLAYERMAX			(BGLAYERMAX+FGLAYERMAX+2)
#define UBGID_BGMASK		(UBGLAYERMAX-2)
#define UFGID_FGPAUSE		(UBGLAYERMAX-1)

#define BGLAYERSETMAX		0x08

#define BG_NONE			0
#define BG_WHITEFLASH	1
#define BG_REDFLASH		2
#define BG_YELLOWFLASH	3

#define BGMT_FLASH		0x20

#define BG_WHITEOUT		11
#define BG_REDOUT		12

#define BGMT_OUT		0x80

#define BG_FADEIN		40
#define BG_FADEINHALF	41
#define BG_FADEOUT		42

#define BGMT_FADE		0x40

#define BG_LIGHTUP			100
#define BG_LIGHTRED			110
#define BG_LIGHTUPNORMAL	120

#define BGMT_LIGHT			0x40

#define FG_PAUSEIN		200
#define FG_PAUSEOUT		210

#define FGMT_PAUSE		0x18

struct BGLayerSet
{
	DWORD timer;
	DWORD quittime;
	WORD sID;
};

#define BGLAYERSET_NONE	0

class BGLayer : public BObject
{
public:
	BGLayer();
	~BGLayer();

	static void Init(HTEXTURE * tex);
	static void KillOtherLayer();
	static void Action(bool active);
	static void ActionSpecial();
	static void RenderBG();
	static void RenderFG();
	static void RenderFGPause();

	void Render();
	void valueSet(int siID, float x, float y, float z, float w, float h, int rotx, int roty, int rotz, float paral, float speed = 0, int angle = 9000, bool move = false, bool rotate = false, DWORD col = 0xffffffff);
	void valueSet(int siID, float cenx, float ceny, float width, float height, DWORD col = 0xffffffff);
	void valueSetByName(const char * spritename, float cenx, float ceny, float width=-1, float height=-1, DWORD col = 0xffffffff);
	void texRectSet(float texx, float texy, float texw, float texh);
	void rectSet(float x, float y, float z, float w, float h, int rotx, int roty, int rotz);
	void zSet(float z0, float z1, float z2, float z3);
	void scaleSet(float hscale, float vscale);
	void colorSet(DWORD col0, DWORD col1, DWORD col2, DWORD col3);
	void moveSet(bool move, bool rotate);
	void parallelogram(float paral);
	void vertexSet(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void SetFlag(BYTE flag, BYTE maxtime);
	void SetBlend(int blend);

	void action();



public:
//	hgeQuad	quad;
	hgeSprite * sprite;
	DWORD	ocol[4];
	DWORD	acol[4];
	bool	move;			//move vertex
	bool	rotate;			//set move to enable
	float	width;
	float	height;
	int		tw;
	int		th;
	BYTE	flag;
	BYTE	changetimer;
	BYTE	mtimer;

	static BGLayer ubg[UBGLAYERMAX];
	static BGLayerSet bglayerset[BGLAYERSETMAX];
	static WORD setindex;

	static HTEXTURE * tex;

};

#endif