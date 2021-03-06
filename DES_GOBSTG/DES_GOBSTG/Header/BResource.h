#ifndef _BRESOURCE_H
#define _BRESOURCE_H

#include "MainDependency.h"
#include "DataStruct.h"

#define RSIZE_RESOURCE		(sizeof(resourceData))
#define RSIZE_STRINGDESC	(sizeof(char) * M_STRMAX * STRINGDESCMAX)
#define RSIZE_CUSTOMCONST	(sizeof(customconstData) * SCR_CUSTOMCONSTMAX)
#define RSIZE_MUSIC			(sizeof(musicData) * MUSICMAX)
#define RSIZE_BULLET		(sizeof(bulletData) * BULLETTYPEMAX)
#define RSIZE_ENEMY			(sizeof(enemyData) * ENEMYTYPEMAX)
#define RSIZE_PLAYER		(sizeof(playerData) * PLAYERTYPEMAX)
#define RSIZE_SPRITE		(sizeof(spriteData) * SPRITEITEMMAX)
#define RSIZE_PLAYERBULLET	(sizeof(playerbulletData) * PLAYERBULLETSPRITEMAX)
#define RSIZE_PLAYERSHOOT	(sizeof(playershootData) * PLAYERSHOOTTYPEMAX)
#define RSIZE_PLAYERGHOST	(sizeof(playerghostData) * PLAYERGHOSTTYPEMAX)
#define RSIZE_TEXTURE		(sizeof(textureData) * TEXMAX)

class BResource
{
public:

	BResource();
	~BResource();

	void Init();
	void InitTexinfo();
	void Release();

	bool Fill();
	bool Pack(void * pStrdesc, void * pCustomConstName);
	bool Gain(void * pStrdesc = NULL, void * pCustomConstName = NULL);
	void CopyData();
	void GetUIString();
	bool SetDataFile();
	bool LoadPackage();

	bool LoadTextureSet(int texset=-1);
	bool FreeTextureSet(int texset=-1);
	HTEXTURE LoadTexture(int i);

	void ReleaseCustomConst();
	void MallocCustomConst();

	void ReleaseStrDesc();
	void MallocStrDesc(DWORD size);

public:

	musicData musdata[MUSICMAX];
	resourceData resdata;
	vector<spellData> spelldata;
	bulletData bulletdata[BULLETTYPEMAX];
	enemyData enemydata[ENEMYTYPEMAX];
	playerData playerdata[PLAYERTYPEMAX];
	spriteData spritedata[SPRITEITEMMAX];
	playerbulletData playerbulletdata[PLAYERBULLETSPRITEMAX];
	playershootData playershootdata[PLAYERSHOOTTYPEMAX];
	playerghostData playerghostdata[PLAYERGHOSTTYPEMAX];
	customconstData * customconstdata;
	textureData texturedata[TEXMAX];
	stringData * strdesc;

	static BResource bres;

	//texture
	HTEXTURE	tex[TEXMAX];
	hgeTextureInfo texinfo[TEXMAX];
};

extern HGE * hge;

#endif