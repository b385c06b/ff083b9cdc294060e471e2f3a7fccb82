#ifndef _FRONTDISPLAY_H
#define _FRONTDISPLAY_H

#include "MainDependency.h"
#include "Const.h"
#include "Effectsys.h"
#include "InfoQuad.h"
#include "Item.h"

#define FDISP_ASCII_BEGIN		0x20
#define FDISP_ASCII_END			0x81
#define FDISP_ASCIISMALL_END	0x7E
#define FDISP_ASCII_MAX			(FDISP_ASCII_END-FDISP_ASCII_BEGIN+1)
#define FDISP_ASCIISMALL_MAX	(FDISP_ASCIISMALL_END-FDISP_ASCII_BEGIN+1)

struct ftPanelSet 
{
	hgeSprite * left;
	hgeSprite * top;
	hgeSprite * bottom;
	hgeSprite * right;
	hgeSprite * hiscore;
	hgeSprite * score;
	hgeSprite * player;
	hgeSprite * power;
	hgeSprite * graze;
	hgeSprite * slot;
	hgeSprite * slotback;
	hgeSprite * lifeindi[6];
	hgeSprite * triangleindi;
	hgeSprite * pointindi;
	hgeSprite * borderindi;
};

struct ftInfoBody
{
	Effectsys effBossCollapse;
	Effectsys effBossItem;
	Effectsys effBossUp;
	Effectsys effBossStore;

	Effectsys effLoading;

	InfoQuad iqBossBlood;
};

struct ftInfoSet
{
	hgeFont * bossfont;
	hgeFont * bossasciifont;
	hgeFont * normaldigitfont;
	hgeFont * smalldigitfont;
	hgeFont * asciifont;
	hgeFont * itemfont;
	HD3DFONT normalfont;
	HD3DFONT smallfont;

	hgeSprite * cutin;
	hgeSprite * plchat_1;
	hgeSprite * plchat_2;
	hgeSprite * plchat_3;
	hgeSprite * enchat_1;
	hgeSprite * enchat_2;
	hgeSprite * enchat_3;
	hgeSprite * namecard;

	hgeSprite * bossspellline;
	hgeSprite * playerspellline;
	hgeSprite * spellbonustext;
	hgeSprite * spellhistorytext;
	hgeSprite * spellfailedtext;
	hgeSprite * timecircle;
	hgeSprite * enemyx;

	hgeSprite * lifebar;
	hgeSprite * star;
	hgeSprite * getbonus;
	hgeSprite * failed;

	hgeSprite * stageclear;
	hgeSprite * nextstage;
	hgeSprite * fullpower;
	hgeSprite * hiscoreget;
	hgeSprite * extend;
	hgeSprite * textbox;
	hgeSprite * demo;
	hgeSprite * loading;
};

struct ftNumSet 
{
	union{
		struct  
		{
			hgeSprite * num_0;
			hgeSprite * num_1;
			hgeSprite * num_2;
			hgeSprite * num_3;
			hgeSprite * num_4;
			hgeSprite * num_5;
			hgeSprite * num_6;
			hgeSprite * num_7;
			hgeSprite * num_8;
			hgeSprite * num_9;
		};
		hgeSprite * num[10];
	};

	hgeSprite * num_add;
	hgeSprite * num_sub;
	hgeSprite * num_mul;
	hgeSprite * num_div;
	hgeSprite * num_mod;
	hgeSprite * num_dot;
};

struct ftItemNumSet 
{
	union {
		struct {
			hgeSprite * num_0;
			hgeSprite * num_1;
			hgeSprite * num_2;
			hgeSprite * num_3;
			hgeSprite * num_4;
			hgeSprite * num_5;
			hgeSprite * num_6;
			hgeSprite * num_7;
			hgeSprite * num_8;
			hgeSprite * num_9;
			hgeSprite * fadeI_0;
			hgeSprite * fadeI_1;
			hgeSprite * fadeI_2;
			hgeSprite * fadeI_3;
			hgeSprite * fadeI_4;
			hgeSprite * fadeI_5;
			hgeSprite * fadeI_6;
			hgeSprite * fadeI_7;
			hgeSprite * fadeI_8;
			hgeSprite * fadeI_9;
			hgeSprite * fadeII_0;
			hgeSprite * fadeII_1;
			hgeSprite * fadeII_2;
			hgeSprite * fadeII_3;
			hgeSprite * fadeII_4;
			hgeSprite * fadeII_5;
			hgeSprite * fadeII_6;
			hgeSprite * fadeII_7;
			hgeSprite * fadeII_8;
			hgeSprite * fadeII_9;
		};
		hgeSprite * itemnum[30];
	};
};

struct ftAscIISet 
{
	union {
		struct{
			hgeSprite * space;
			hgeSprite * x21;
			hgeSprite * x22;
			hgeSprite * x23;
			hgeSprite * x24;
			hgeSprite * x25;
			hgeSprite * x26;
			hgeSprite * x27;
			hgeSprite * x28;
			hgeSprite * x29;
			hgeSprite * x2A;
			hgeSprite * x2B;
			hgeSprite * x2C;
			hgeSprite * x2D;
			hgeSprite * x2E;
			hgeSprite * x2F;
			hgeSprite * num_0;
			hgeSprite * num_1;
			hgeSprite * num_2;
			hgeSprite * num_3;
			hgeSprite * num_4;
			hgeSprite * num_5;
			hgeSprite * num_6;
			hgeSprite * num_7;
			hgeSprite * num_8;
			hgeSprite * num_9;
			hgeSprite * x3A;
			hgeSprite * x3B;
			hgeSprite * x3C;
			hgeSprite * x3D;
			hgeSprite * x3E;
			hgeSprite * x3F;
			hgeSprite * x40;
			hgeSprite * ch_A;
			hgeSprite * ch_B;
			hgeSprite * ch_C;
			hgeSprite * ch_D;
			hgeSprite * ch_E;
			hgeSprite * ch_F;
			hgeSprite * ch_G;
			hgeSprite * ch_H;
			hgeSprite * ch_I;
			hgeSprite * ch_J;
			hgeSprite * ch_K;
			hgeSprite * ch_L;
			hgeSprite * ch_M;
			hgeSprite * ch_N;
			hgeSprite * ch_O;
			hgeSprite * ch_P;
			hgeSprite * ch_Q;
			hgeSprite * ch_R;
			hgeSprite * ch_S;
			hgeSprite * ch_T;
			hgeSprite * ch_U;
			hgeSprite * ch_V;
			hgeSprite * ch_W;
			hgeSprite * ch_X;
			hgeSprite * ch_Y;
			hgeSprite * ch_Z;
			hgeSprite * x5B;
			hgeSprite * x5C;
			hgeSprite * x5D;
			hgeSprite * x5E;
			hgeSprite * x5F;
			hgeSprite * x60;
			hgeSprite * ch_a;
			hgeSprite * ch_b;
			hgeSprite * ch_c;
			hgeSprite * ch_d;
			hgeSprite * ch_e;
			hgeSprite * ch_f;
			hgeSprite * ch_g;
			hgeSprite * ch_h;
			hgeSprite * ch_i;
			hgeSprite * ch_j;
			hgeSprite * ch_k;
			hgeSprite * ch_l;
			hgeSprite * ch_m;
			hgeSprite * ch_n;
			hgeSprite * ch_o;
			hgeSprite * ch_p;
			hgeSprite * ch_q;
			hgeSprite * ch_r;
			hgeSprite * ch_s;
			hgeSprite * ch_t;
			hgeSprite * ch_u;
			hgeSprite * ch_v;
			hgeSprite * ch_w;
			hgeSprite * ch_x;
			hgeSprite * ch_y;
			hgeSprite * ch_z;
			hgeSprite * x7B;
			hgeSprite * x7C;
			hgeSprite * x7D;
			hgeSprite * x7E;
			hgeSprite * backspace;
			hgeSprite * end;
			hgeSprite * block;
		};
		hgeSprite * ascii[FDISP_ASCII_MAX];
	};
};

struct ftFirstSet 
{
	hgeSprite * face;
	hgeSprite * namecard;
};

#define FDISP_PANEL			0
#define FDISP_NEXTSTAGE		1
#define FDISP_FULLPOWER		2
#define FDISP_HISCORE		3
#define FDISP_EXTEND		4

#define FDISP_STATE_COUNT	120

class FrontDisplay
{
public:
	FrontDisplay();
	~FrontDisplay();

	bool Init();
	void Release();

	void PanelDisplay();
	void InfoDisplay();
	void ItemInfoDisplay(infoFont * item);

	void BossInit();
	void BossAction();
	void BossMoveItemEffect(float x, float y);

	void BossInfoDisplay();
	void BossTimeCircleDisplay();
	void EnemyXDisplay();
	void NextStageDisplay();
	void FullPowerDisplay();
	void HiScoreGetDisplay();
	void ExtendDisplay();
	void DemoDisplay(int rpyindex);
	void LoadingDisplay();

	void SetState(BYTE type, BYTE state=FDISP_STATE_COUNT);
	void SetValue(LONGLONG llval, int ival, float fval, bool bval);

	ftPanelSet panel;
	ftInfoSet info;
	ftNumSet bignum;
	ftNumSet normalnum;
	ftNumSet smallnum;
	ftItemNumSet itemnum;
	ftAscIISet ascii;
	ftAscIISet asciismall;
	ftFirstSet first;
	ftInfoBody infobody;

	LONGLONG llval;
	int ival;
	float fval;
	bool bval;

	BYTE panelcountup;
	BYTE nextstagecount;
	BYTE fullpowercount;
	BYTE hiscorecount;
	BYTE extendcount;
};

extern FrontDisplay fdisp;

#endif