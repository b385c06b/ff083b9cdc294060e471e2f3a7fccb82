#ifndef _DATASTRUCT_H
#define _DATASTRUCT_H

#include "Const.h"

struct customconstData{
	char name[M_STRMAX];
	int value;
};

struct musicData{
	char musicname[M_STRMAX];
	char musicfilename[M_PATHMAX];
	union{
		struct  
		{
			char explain_1[M_STRMAX];
			char explain_2[M_STRMAX];
			char explain_3[M_STRMAX];
			char explain_4[M_STRMAX];
		};
		char explain[4][M_STRMAX];
	};
	LONGLONG startpos;
	LONGLONG introlength;
	LONGLONG alllength;
};

struct spellData{
	char spellname[M_STRMAX];
	LONGLONG bonus;
	int spellnumber;
	int sno;
	int turntoscene;
	BYTE timelimit;
	BYTE remain;
	BYTE userID;
	BYTE spellflag;
	BYTE battleID;
};

struct uiStringData 
{
	char filename[M_STRMAX];
	char username[M_STRMAX];
	char score[M_STRMAX];
	char point[M_STRMAX];
	char faith[M_STRMAX];
	char alltime[M_STRMAX];
	char date[M_STRMAX];
	char lost[M_STRMAX];
	char borderrate[M_STRMAX];
	char fastrate[M_STRMAX];
	char modeflag[M_STRMAX];
	char mode_none[M_STRMAX];
	char mode_practice[M_STRMAX];
	char mode_spell[M_STRMAX];
	char usingchara[M_STRMAX];
//	char maxplayer[M_STRMAX];
	char misstime[M_STRMAX];
	char bordertime[M_STRMAX];
	char continuetime[M_STRMAX];
	char gettime[M_STRMAX];
	char pausetime[M_STRMAX];
	char difflv[M_STRMAX];
	char laststage[M_STRMAX];
	char graze[M_STRMAX];
	char confirm[M_STRMAX];
};

struct resourceData
{
	//resource file
	char texfilename[TEXMAX][M_PATHMAX];
	char sefilename[SEMAX][M_PATHMAX];
	char effectsysfilename[EFFECTSYSTYPEMAX][M_PATHMAX];
	char packagefilename[PACKAGEMAX][M_PATHMAX];

	//font
	char widefontname[M_STRMAX];

	//datafile

	char binfilename[M_PATHMAX];
	char binname[M_PATHMAX];
	char spellaccessfilename[M_PATHMAX];
	char rabinname[M_STRMAX];
	char scriptfilename[M_PATHMAX];
	char scrbinname[M_PATHMAX];

	char customconstfilename[M_PATHMAX];
	char spelldefinefilename[M_PATHMAX];
	char musicdefinefilename[M_PATHMAX];
	char bulletdefinefilename[M_PATHMAX];
	char enemydefinefilename[M_PATHMAX];
	char playerdefinefilename[M_PATHMAX];
	char spritedefinefilename[M_PATHMAX];
	char playerbulletdefinefilename[M_PATHMAX];
	char playershootdefinefilename[M_PATHMAX];
	char playerghostdefinefilename[M_PATHMAX];

	//folder
	char scriptfoldername[M_SCRIPTFOLDERMAX][M_PATHMAX];
	char snapshotfoldername[M_PATHMAX];
	char replayfoldername[M_PATHMAX];
	char datafoldername[M_PATHMAX];
	char effectsysfoldername[M_PATHMAX];

	//extension
	char scriptextensionname7[8];
	char replayextensionname7[8];

	//replay header
	char replaysignature11[12];
	char replaytempsign3[4];
	char replaycompletesign3[4];
	char replaytag3[4];

	bool binmode;

	uiStringData uistr;
};

struct bulletData 
{
	int nTurnAngle;
	int tex_x;
	int tex_y;
	int tex_w;
	int tex_h;
	float collisionMain;
	float collisionSub;
	BYTE nRoll;
	BYTE nColor;
	BYTE seID;
	BYTE blendtype;
	BYTE collisiontype;
	BYTE fadecolor;
	BYTE bonuscolor;
	BYTE renderdepth;
};

struct enemyData 
{
	char name[M_STRMAX];
	char ename[M_STRMAX];
	float collision_w;
	float collision_h;
	float usetexw;
	float usetexh;
	BYTE tex;
	BYTE faceIndex;
	BYTE tex_nCol;
	BYTE tex_nRow;
	BYTE startFrame;
	BYTE standFrame;
	BYTE rightPreFrame;
	BYTE rightFrame;
	BYTE leftPreFrame;
	BYTE leftFrame;
	BYTE attackPreFrame;
	BYTE attackFrame;
	BYTE storePreFrame;
	BYTE storeFrame;
	bool standshake;
};

struct playerData 
{
	char name[M_STRMAX];
	float collision_r;
	float fastspeed;
	float slowspeed;
	float usetexw;
	float usetexh;
	float graze_r;
	int bombperpower;
	WORD borderlast;
	WORD bomblast;
	BYTE shotdelay;
	BYTE tex;
	BYTE faceIndex;
	BYTE tex_nCol;
	BYTE tex_nRow;
	BYTE startFrame;
	BYTE standFrame;
	BYTE leftPreFrame;
	BYTE leftFrame;
	BYTE rightPreFrame;
	BYTE rightFrame;
};

struct spriteData 
{
	char spritename[M_STRMAX];
	int tex_x;
	int tex_y;
	int tex_w;
	int tex_h;
	BYTE tex;
};

struct playerbulletData 
{
	float x;
	float y;
	float w;
	float h;
	BYTE tex;
};

struct playershootData 
{
	int angle;
	int hitonfactor;
	float xbias;
	float ybias;
	float scale;
	float speed;
	float accelspeed;
	float power;
	WORD flag;
	BYTE userID;
	BYTE seID;
	BYTE timeMod;
	BYTE arrange;
	BYTE ID;
};

struct playerghostData 
{
	int siID;
	int shootangle;
	int startangle;
	float mover;
	float xadj;
	float yadj;
	float speed;
	WORD flag;
	BYTE blend;
};

#endif