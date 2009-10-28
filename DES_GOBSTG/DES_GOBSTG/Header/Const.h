#ifndef _CONST_H
#define _CONST_H

#define GAME_VERSION		0x073
#define GAME_VERSION_STR	"ver.0x073"
#define GAME_SIGNATURE		"h5nc"
#define GAME_TITLE			"ñ|∑Ωèé÷√àˆ"

#define GAMEVERSION_CORE		0x08
#define GAMEVERSION_ALGORITHM	0x04
#define GAMEVERSION_RESOURCE	0x00

#define ARC(X)		((X) * 0.0001745329251994329f)
#define ANGLE(X)	(int)((X) * 5729.577951308232f)

#define DIST(X1, Y1, X2, Y2)	(sqrtf(((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2))))
#define DIST2(X1, Y1, X2, Y2)	(((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2)))
#define SIGN(X)					((((X)&1)<<1)-1)
#define ROLL(X, T)				(((X)%(2*(T)))/(T)*(T) - SIGN((X)%(2*(T))/(T)) * ((X)%(T)))
#define INTER(A, B, X)			(((B)-(A))*(X)+(A))
#define RANDA					((LONG)(randt()) * 36000 / RAND_MAX)

/*
//////////ROTATIONX//////////
//y = y * cosf(rotx) - z * sinf(rotx);
//z = y * sinf(rotx) + z * cosf(rotx);
//////////ROTATIONY//////////
//x = x * cosf(roty) - z * sinf(roty);
//z = x * sinf(roty) + z * cosf(roty);
//////////ROTATIONZ//////////
//x = x * cosf(rotz) - y * sinf(rotz);
//y = x * sinf(rotz) + y * cosf(rotz);
*/
#define ROTATION1(F1, F2, A)	((F1) * cost(A) - (F2) * sint(A))
#define ROTATION2(F1, F2, A)	((F1) * sint(A) + (F2) * cost(A))

#define HGELOG			hge->System_Log
#define HGELOG_ERRSTR	"**********ERROR**********"

typedef struct tagTarget
{
	float x;
	float y;
}Target;

#define BTYPE_BULLET	0x10
#define BTYPE_BEAM		0x20
#define BTYPE_ENEMY		0x30
#define BTYPE_GHOST		0x40

#define BINDEX_NULL		0x100000

#define M_DEFAULT_FPS			60
#define M_DEFAULT_FRAMESKIP		1
#define M_DEFAULT_RENDERSKIP	1
#define M_FRAMESKIP_FASTSKIP	-3
#define M_FRAMESKIP_SLOWSKIP	2

#define M_PROJECTIONMATRIX_OFFSET	0.5f

#define M_CLIENT_LEFT			0.0f
#define M_CLIENT_RIGHT			640.0f
#define M_CLIENT_TOP			0.0f
#define M_CLIENT_BOTTOM			480.0f
#define M_CLIENT_WIDTH			(M_CLIENT_RIGHT-M_CLIENT_LEFT)
#define M_CLIENT_HEIGHT			(M_CLIENT_BOTTOM-M_CLIENT_TOP)
#define M_CLIENT_CENTER_X		((M_CLIENT_LEFT+M_CLIENT_RIGHT)/2)
#define M_CLIENT_CENTER_Y		((M_CLIENT_TOP+M_CLIENT_BOTTOM)/2)

#define M_ACTIVECLIENT_LEFT		0.0f
#define M_ACTIVECLIENT_RIGHT	448.0f
#define M_ACTIVECLIENT_TOP		0.0f
#define M_ACTIVECLIENT_BOTTOM	480.0f
#define M_ACTIVECLIENT_WIDTH	(M_ACTIVECLIENT_RIGHT-M_ACTIVECLIENT_LEFT)
#define M_ACTIVECLIENT_HEIGHT	(M_ACTIVECLIENT_BOTTOM-M_ACTIVECLIENT_TOP)
#define M_ACTIVECLIENT_CENTER_X	((M_ACTIVECLIENT_LEFT+M_ACTIVECLIENT_RIGHT)/2)
#define M_ACTIVECLIENT_CENTER_Y ((M_ACTIVECLIENT_TOP+M_ACTIVECLIENT_BOTTOM)/2)
#define M_ACTIVECLIENT_BOSSX	M_ACTIVECLIENT_CENTER_X
#define M_ACTIVECLIENT_BOSSY	(M_ACTIVECLIENT_CENTER_Y/2)

#define M_DELETEBOLDER			20.0f
#define M_DELETECLIENT_LEFT		(M_ACTIVECLIENT_LEFT-M_DELETEBOLDER)
#define M_DELETECLIENT_RIGHT	(M_ACTIVECLIENT_RIGHT+M_DELETEBOLDER)
#define M_DELETECLIENT_TOP		(M_ACTIVECLIENT_TOP-M_DELETEBOLDER)
#define M_DELETECLIENT_BOTTOM	(M_ACTIVECLIENT_BOTTOM+M_DELETEBOLDER)

#define M_KEYKEYMAX			0x100
#define M_JOYKEYMAX			0x20

#define M_GETSPELLMAX		0x40

#define M_PL_CONTINUEMAX	5

#define M_PL_ONESETPLAYER		3

#define M_NDIFFI		6
#define M_NSTAGEDIFFI	11
#define M_DIFFI_EASY		0
#define M_DIFFI_NORMAL		1
#define M_DIFFI_HARD		2
#define M_DIFFI_DESTINY		3
#define M_DIFFI_EXTRA_START	4
#define M_DIFFI_EXTRA_1		M_DIFFI_EXTRA_START
#define M_DIFFI_EXTRA_2		(M_DIFFI_EXTRA_START+1)
#define M_DIFFI_EXTRA_3		(M_DIFFI_EXTRA_START+2)
#define M_DIFFI_EXTRA_4		(M_DIFFI_EXTRA_START+3)
#define M_DIFFI_EXTRA_5		(M_DIFFI_EXTRA_START+4)
#define M_DIFFI_EXTRA_6		(M_DIFFI_EXTRA_START+5)

#define M_LASTSTAGESTR_1		"1"
#define M_LASTSTAGESTR_2		"2"
#define M_LASTSTAGESTR_3		"3"
#define M_LASTSTAGESTR_4		"4"
#define M_LASTSTAGESTR_5		"5"
#define M_LASTSTAGESTR_6		"6"
#define M_LASTSTAGESTR_EX		"Ex"
#define M_LASTSTAGESTR_C		"C"

#define M_DIFFISTR_EASY		"Easy"
#define M_DIFFISTR_NORMAL	"Normal"
#define M_DIFFISTR_HARD		"Hard"
#define M_DIFFISTR_DESTINY	"Destiny"
#define M_DIFFISTR_EXTRA	"Extra"
#define M_DIFFISTR_EXTRA_1	"Extra_1"
#define M_DIFFISTR_EXTRA_2	"Extra_2"
#define M_DIFFISTR_EXTRA_3	"Extra_3"
#define M_DIFFISTR_EXTRA_4	"Extra_4"
#define M_DIFFISTR_EXTRA_5	"Extra_5"
#define M_DIFFISTR_EXTRA_6	"Extra_6"

#define M_DIFFISTR_EASY_S		"E"
#define M_DIFFISTR_NORMAL_S		"N"
#define M_DIFFISTR_HARD_S		"H"
#define M_DIFFISTR_DESTINY_S	"Des"
#define M_DIFFISTR_EXTRA_S		"Ex"

#define M_STAGESTR_PRE			"S-"
#define M_STAGESTR_STAGEPRE		"Stage "
#define M_REPLAY_SAVEFILEPRE	"DES_"

#define	M_RPYMODE_SPELL		1
#define	M_RPYMODE_PRACTICE	2

#define M_MODE_NORMAL	0
#define M_MODE_STAGE	M_RPYMODE_PRACTICE
#define M_MODE_SPELL	(M_RPYMODE_PRACTICE | M_RPYMODE_SPELL)

#define M_PATHMAX		_MAX_PATH
#define M_STRMAX		0x80
#define M_STRITOAMAX	0x10

#define M_FONTWIDTH		11
#define M_FONTHEIGHT	20

#define M_STAGENSCENE	100

#define M_STAGESCENE_1_E	1
#define M_STAGESCENE_1_N	2
#define M_STAGESCENE_1_H	3
#define M_STAGESCENE_2_E	4
#define M_STAGESCENE_2_N	5
#define M_STAGESCENE_2_H	5
#define M_STAGESCENE_3_E	6
#define M_STAGESCENE_3_N	6
#define M_STAGESCENE_3_H	6
#define M_STAGESCENE_4_E	7
#define M_STAGESCENE_4_N	7
#define M_STAGESCENE_4_H	8
#define M_STAGESCENE_5_E	9
#define M_STAGESCENE_5_N	10
#define M_STAGESCENE_5_H	10
#define M_STAGESCENE_6_E	11
#define M_STAGESCENE_6_N	11
#define M_STAGESCENE_6_H	11

#define M_SQUARE_2			0.70710678f
#define M_SECONDPERFRAME	0.01666667f

#define M_USINGMODE_ERROR	0x00
#define M_USINGMODE_TEXT	0x01
#define M_USINGMODE_BIN		0x02
#define M_USINGMODE_BOTH	(M_USINGMODE_TEXT | M_USINGMODE_BIN)

#define SEL_NULL		0x00
#define SEL_NONACTIVE	0x10
#define SEL_GRAY		0x20
#define SEL_STAY		0x40
#define SEL_NOSHIFT		0x80

#define M_BINHEADER_OFFSET	0x40

#define PLAYERTYPEMAX		0x20

#define SPRITEITEMMAX		0x400

#define TARGETMAX			0x40

#define BULLETMAX			0x3000
#define BULLETTYPEMAX		0x40
#define BULLETCOLORMAX		0x10
#define BULLETTYPECOLORMAX	(BULLETTYPEMAX * BULLETCOLORMAX)
#define ITEMMAX				BULLETMAX
#define	BEAMMAX				0x100

#define PLAYERGHOSTMAX		4
#define PLAYERGHOSTTYPEMAX	(PLAYERGHOSTMAX * PLAYERTYPEMAX * 2)

#define PLAYERBULLETMAX		0x80

#define PLAYERBULLETTYPE		4
#define PLAYERBULLETSPRITEMAX	(PLAYERTYPEMAX * PLAYERBULLETTYPE * 2)

#define PLAYERSHOOTTYPEMAX		(PLAYERTYPEMAX * (PLAYERGHOSTMAX + 2))

#define ENEMYMAX			0x80
#define ENEMYTYPEMAX		0x50
#define GHOSTMAX			0x80

#define GHOSTTYPEMAX		8

#define BGLAYERMAX			0x0C
#define FGLAYERMAX			0x04
#define BGMASKINDEX			0x10
#define FGPAUSEINDEX		0x11
#define UBGLAYERMAX			0x12
#define BGLAYERSETMAX		0x10

#define EFFECTSPMAX			0x80

#define EFFECTSYSMAX		0x100
#define EFFECTSYSTYPEMAX	0x100

#define EFF_NULL				0

#define EFF_PG_EFFECTBEGIN		1
#define EFF_PG_EFFECT_0			1
#define EFF_PG_EFFECT_1			2
#define EFF_PG_EFFECT_2			3
#define EFF_PG_EFFECT_3			4
#define EFF_PG_EFFECT_4			5
#define EFF_PG_EFFECT_5			6
#define EFF_PG_EFFECT_6			7
#define EFF_PG_EFFECT_7			8

#define EFF_PL_GRAZE			10
#define EFF_PL_CHANGE			11
#define EFF_PL_INFI				12
#define EFF_PL_COLLAPSE			13
#define EFF_PL_MERGE			14
#define EFF_PL_BORDER			15
#define EFF_PL_BORDERON			16
#define EFF_PL_BORDEROFF		17

#define EFF_PL_BOMBBEGIN		20
#define EFF_PL_BOMB_0			20
#define EFF_PL_BOMB_1			21
#define EFF_PL_BOMB_2			22
#define EFF_PL_BOMB_3			23
#define EFF_PL_BOMB_4			24
#define EFF_PL_BOMB_5			25
#define EFF_PL_BOMB_6			26
#define EFF_PL_BOMB_7			27

#define EFF_GH_COLLAPSE			30
#define EFF_EN_SHOT				31
#define EFF_EN_COLLAPSE			32
#define EFF_BI_BOSSUP			34
#define EFF_BI_DESBOSSUP		35
#define EFF_BI_COLLAPSE			36
#define EFF_BI_DESCOLLAPSE		37
#define EFF_BI_ITEM				38
#define EFF_BI_BOSSSTORE		39

#define EFF_GH_TYPEBEGIN		40
#define EFF_FREE_TYPEBEGIN		50

#define TEXMAX				0x100

#define TEX_WORD		0
#define TEX_WHITE		1
#define TEX_BULLET		2//7
#define TEX_GHOST		3//8
#define TEX_ENEMY		4//14

#define M_SCRIPTFOLDERMAX	8

#define PACKAGEMAX			0x20
#define MUSICMAX			0x40

#define SEMAX				0x80
#define SE_FREEBEGIN		0x40

#define FONTSYSMAX			0x40

#define EXTENSIONMAX		0x08

#define STRINGDESCMAX		0x400

#define M_SAVEINPUTMAX		0x36000

#ifdef __RELEASE
	#define DEFAULT_RESOURCEPATH	"Resource"
#else
	#define DEFAULT_RESOURCEPATH	"../../Resource"
#endif

#ifdef __RELEASE
	#define DEFAULT_HGEDLLPATH		"Resource/hge.dll"
#else
	#ifdef _DEBUG
		#define DEFAULT_HGEDLLPATH		"../../Resource/Debug/hge_d.dll"
	#else
		#define DEFAULT_HGEDLLPATH		"../../Resource/hge.dll"
	#endif // _DEBUG
#endif // __RELEASE

//pan coefficient
#define M_SE_PAN 0.35f

//pushtimer
#define M_PUSH_FIRST	40
#define M_PUSH_ROLLTO	32
#define M_PUSH_SKIP		5
#define M_NOPUSH_SKIP	240


//script

#define SCRIPT_DATAMAX		0x400000
#define SCR_CUSTOMCONSTMAX	0x400
#define SCR_MAXDESC			0x100


enum{
	S0 = 100,
	S100 = 100,
	S700 = 700,
	S1200 = 1200,
	S1 = 1800,

	SCLEAR = 100000
};

#endif