#ifndef _BOSSINFO_H
#define _BOSSINFO_H

#include "MainDependency.h"
#include "InfoQuad.h"
#include "Const.h"
#include "Effectsys.h"

#define BOSSINFO_ONMASK		0x0f
#define BOSSINFO_ENABLE		0x01
#define BOSSINFO_UP			0x02

#define BOSSINFO_OUTMASK	0xf0
#define BOSSINFO_COLLAPSE	0x10
#define BOSSINFO_TIMEOVER	0x20

#define BISF_NONE			0x00
#define BISF_SHOT			0x01
#define BISF_BOMB			0x02
#define BISF_WAIT			0x04
#define BISF_NOBOMBDAMAGE	0x10
#define BISF_NOTSPELL		0x80

class BossInfo
{
public:
	BossInfo(){};
	~BossInfo(){};

	void bossUp();
	void bossCollapse();

	bool bossable();
	bool bossout();

	bool action();
	void quit();

	bool Fill(int sno);

	bool isSpell();

	void Clear();
	bool Init();
	void Release();

public:
	char enemyname[M_STRMAX];
	char enemyename[M_STRMAX];
	char spellname[M_STRMAX];
	InfoQuad blood;
	DWORD bonus;
	DWORD maxbonus;
	DWORD lastgraze;
	int get;
	int meet;
	int itemstack;
	bool exist;
	WORD timer;
	BYTE limit;
	BYTE remain;

	bool failed;
	bool allover;
	int sno;
	int turntoscene;

	BYTE flag;
	BYTE spellflag;

	static BossInfo bossinfo;
};


#endif