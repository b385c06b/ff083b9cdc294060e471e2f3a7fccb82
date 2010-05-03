#include "../Header/Keytable.h"
#include "../Header/Scripter.h"
#include "../Header/Bullet.h"
#include "../Header/InfoSelect.h"
#include "../Header/SE.h"
#include "../Header/Process.h"
#include "../Header/BGLayer.h"
#include "../Header/Data.h"
#include "../Header/Selector.h"
#include "../Header/PushKey.h"
#include "../Header/Beam.h"
#include "../Header/Player.h"
#include "../Header/Item.h"
#include "../Header/Enemy.h"
#include "../Header/Chat.h"
#include "../Header/BulletListActionConst.h"

scrKeyWord scrKeyTable[] =
{
	{"DES_TH_TOB_SCRIPTFILE",	SCR_SCRIPTTAG},

	{"CONTROLFILE",				SCR_CONTROL},
	{"STAGEFILE",				SCR_STAGE},
	{"EDEFFILE",				SCR_EDEF},
	{"SCENEFILE",				SCR_SCENE},
	{"FUNCTIONFILE",			SCR_FUNCTION},
	{"EVENTFILE",				SCR_EVENT},

	{"#",						SCR_FILENAME},
	{"@",						SCR_BLOCKCON},

	/************************************************************************/
	/* Action                                                               */
	/************************************************************************/
	{"SD",						SCR_SD},
	{"SDf",						SCR_SDF},
	{"ST",						SCR_ST},
	{"CALL",					SCR_CALL},
	{"CALLEX",					SCR_CALLEX},
	{"EXECUTE",					SCR_EXECUTE},

	{"RETURN",					SCR_RETURN},
	{"SETSTATE",				SCR_SETSTATE},
	{"SETTIME",					SCR_SETTIME},
	{"SETDIFFI",				SCR_SETDIFFI},
	{"SETCHARA",				SCR_SETCHARA},
	{"SETSCENE",				SCR_SETSCENE},
	{"SETMODE",					SCR_SETMODE},
	{"STARTPREP",				SCR_STARTPREP},
	{"SETKEY",					SCR_SETKEY},
	{"DISABLEALLKEY",			SCR_DISABLEALLKEY},
	{"SETPUSHEVENT",			SCR_SETPUSHEVENT},
	{"UPDATEPUSHEVENT",			SCR_UPDATEPUSHEVENT},
	{"STOPACTION",				SCR_STOPACTION},
	{"SETFRAMESKIP",			SCR_SETFRAMESKIP},

	{"MUSICCHANGE",				SCR_MUSICCHANGE},
	{"MUSICSLIDE",				SCR_MUSICSLIDE},
	{"SE",						SCR_SE},
	{"SEOFF",					SCR_SEOFF},
	{"HSVTORGB",				SCR_HSVTORGB},
	{"SETSHAKE",				SCR_SETSHAKE},

	/************************************************************************/
	/* Basic                                                                */
	/************************************************************************/
	{"B",						SCR_BUBUILD},
	{"A",						SCR_BUACTIONSET},
	{"BC",						SCR_BUBUILDCIRCLE},
	{"BL",						SCR_BUBUILDLINE},
	{"BUDECANCEL",				SCR_BUDECANCEL},
	{"IZEZONE",					SCR_IZEZONE},
	{"BEB",						SCR_BEBUILD},
	{"BEVECTOR",				SCR_BEVECTOR},
	{"BEHOLD",					SCR_BEHOLD},
	{"BEPIN",					SCR_BEPIN},

	{"GB",						SCR_GHBUILD},
	{"GHSAIM",					SCR_GHSAIM},
	{"GHSET",					SCR_GHSET},
	{"GHCHASE",					SCR_GHCHASE},
	{"GHSETLIFE",				SCR_GHSETLIFE},
	{"GHCHANGE",				SCR_GHCHANGE},

	{"EB",						SCR_ENBUILD},
	{"EA",						SCR_ENACTIONSET},
	{"ENSAIM",					SCR_ENSAIM},
	{"ENCHASE",					SCR_ENCHASE},
	{"ENSETLIFE",				SCR_ENSETLIFE},
	{"ENCHANGE",				SCR_ENCHANGE},
	{"DAMAGEZONE",				SCR_DAMAGEZONE},
	{"BOSSSTORE",				SCR_BOSSSTORE},
	{"BOSSATTACK",				SCR_BOSSATTACK},

	{"RAMA",					SCR_RAMA},
	{"DIST",					SCR_DIST},
	{"CHASEAIM",				SCR_CHASEAIM},
	{"INTER",					SCR_INTER},

	{"BOSSUP",					SCR_BOSSUP},
	{"SPELLUP",					SCR_SPELLUP},
	{"ACONTROL",				SCR_ACONTROL},
	{"BCONTROL",				SCR_BCONTROL},
	{"COLLISION_CIRCLE",		SCR_COLLISION_CIRCLE},
	{"COLLISION_SQUARE",		SCR_COLLISION_SQUARE},

	{"LOADTEXTURESET",			SCR_LOADTEXTURESET},
	{"FREETEXTURESET",			SCR_FREETEXTURESET},
	{"ITEMBUILD",				SCR_ITEMBUILD},

	{"EFFSETUP",				SCR_EFFSETUP},
	{"EFFSETUPEX",				SCR_EFFSETUPEX},
	{"EFFSETUPCHASE",			SCR_EFFSETUPCHASE},
	{"EFFSTOP",					SCR_EFFSTOP},
	{"EFFOFF",					SCR_EFFOFF},
	{"EFFSET",					SCR_EFFSET},
	{"EFFMOVETO",				SCR_EFFMOVETO},

	{"SETPPOS",					SCR_SETPPOS},
	{"SETPLIFE",				SCR_SETPLIFE},
	{"SETPPOWER",				SCR_SETPPOWER},
	{"SETPFAITH",				SCR_SETPFAITH},
	{"SETPPOINT",				SCR_SETPPOINT},
	{"SETPBBORDER",				SCR_SETPBBORDER},
	{"SETPBSLOW",				SCR_SETPBSLOW},
	{"SETPBINFI",				SCR_SETPBINFI},
	{"SETPSPEED",				SCR_SETPSPEED},
	{"SETPSLOWSPEED",			SCR_SETPSLOWSPEED},
	{"SETPSPEEDFACTOR",			SCR_SETPSPEEDFACTOR},
	{"COLLAPSE",				SCR_COLLAPSE},
	{"SHOOTPB",					SCR_SHOOTPB},
	{"BONUSFLAG",				SCR_BONUSFLAG},

	{"BGVALUE",					SCR_BGVALUE},
	{"BGVALEX",					SCR_BGVALEX},
	{"BGTEXRECT",				SCR_BGTEXRECT},
	{"BGRECT",					SCR_BGRECT},
	{"BGZ",						SCR_BGZ},
	{"BGSCALE",					SCR_BGSCALE},
	{"BGCOLOR",					SCR_BGCOLOR},
	{"BGMOVE",					SCR_BGMOVE},
	{"BGFLAG",					SCR_BGFLAG},
	{"BGPARAL",					SCR_BGPARAL},
	{"BG4V",					SCR_BG4V},
	{"BGOFF",					SCR_BGOFF},
	{"BGBLEND",					SCR_BGBLEND},
	{"BGSETUP",					SCR_BGSETUP},

	{"SELBUILD",				SCR_SELBUILD},
	{"SELCLEAR",				SCR_SELCLEAR},
	{"SELCONFIRM",				SCR_SELCONFIRM},
	{"SELSET",					SCR_SELSET},
	{"SELFLAG",					SCR_SELFLAG},
	{"ISELBUILD",				SCR_ISELBUILD},
	{"ISELCLEAR",				SCR_ISELCLEAR},
	{"ISELSET",					SCR_ISELSET},
	{"ISELFLAG",				SCR_ISELFLAG},
	{"ISELCOLOR",				SCR_ISELCOLOR},

	/************************************************************************/
	/* Control                                                              */
	/************************************************************************/
	{"IF",						SCR_IF},
	{"ELSE",					SCR_ELSE},
	{"ELSEIF",					SCR_ELSEIF},
	{"{",						SCR_THEN},
	{"}",						SCR_END},
	{"LOOP",					SCR_LOOP},
	{"SKIP",					SCR_SKIP},

	{"CHATON",					SCR_CHATON},
	{"CHATOFF",					SCR_CHATOFF},

	/************************************************************************/
	/* Data                                                                 */
	/************************************************************************/
	{"DATAGET",					SCR_DATAGET},
	{"DATAGETf",				SCR_DATAGETf},
	{"DATASET",					SCR_DATASET},
	{"DATASETf",				SCR_DATASETf},
	{"SETFLAG",					SCR_SETFLAG},
	{"TRYSTAGE",				SCR_TRYSTAGE},
	{"DEBUG_BREAKPOINT",		SCR_DEBUG_BREAKPOINT},

	/************************************************************************/
	/* Expression                                                           */
	/************************************************************************/
	{"+",						SCR_ADD_INT},
	{"-",						SCR_SUB_INT},
	{"*",						SCR_MUL_INT},
	{"/",						SCR_DIV_INT},
	{"+f",						SCR_ADD_FLOAT},
	{"-f",						SCR_SUB_FLOAT},
	{"*f",						SCR_MUL_FLOAT},
	{"/f",						SCR_DIV_FLOAT},
	{"+u",						SCR_ADD_UINT},
	{"-u",						SCR_SUB_UINT},
	{"*u",						SCR_MUL_UINT},
	{"/u",						SCR_DIV_UINT},
	{"%",						SCR_MOD},
	{"%u",						SCR_MOD_UINT},

	{"~",						SCR_INVERSE},
	{"!",						SCR_NOT},
	{">",						SCR_GREAT_INT},
	{"<",						SCR_LESS_INT},
	{">f",						SCR_GREAT_FLOAT},
	{"<f",						SCR_LESS_FLOAT},
	{">u",						SCR_GREAT_UINT},
	{"<u",						SCR_LESS_UINT},

	/************************************************************************/
	/* Function                                                             */
	/************************************************************************/
	{"BUI",						SCR_BUI},
	{"BUANGLE",					SCR_BUANGLE},
	{"BUSPEED",					SCR_BUSPEED},
	{"BUX",						SCR_BUX},
	{"BUY",						SCR_BUY},
	{"BU@",						SCR_BUTIMER},
	{"BUCANCELABLE",			SCR_BUCANCELABLE},
	{"BUHAVEGRAY",				SCR_BUHAVEGRAY},
	{"BEI",						SCR_BEI},
	{"BEANGLE",					SCR_BEANGLE},
	{"BESPEED",					SCR_BESPEED},
	{"BEX",						SCR_BEX},
	{"BEY",						SCR_BEY},
	{"BE@",						SCR_BETIMER},
	{"BEHOLDTAR",				SCR_BEHOLDTAR},
	{"BEPINTAR",				SCR_BEPINTAR},

	{"GHX",						SCR_GHX},
	{"GHY",						SCR_GHY},
	{"GH@",						SCR_GHTIMER},
	{"GHI",						SCR_GHI},
	{"GHANGLE",					SCR_GHANGLE},
	{"GHSPEED",					SCR_GHSPEED},
	{"GHAMAP",					SCR_GHAMAP},
	{"GHRMAP",					SCR_GHRMAP},
	{"GHAIMX",					SCR_GHAIMX},
	{"GHAIMY",					SCR_GHAIMY},
	{"GHLIFE",					SCR_GHLIFE},
	{"GHAC",					SCR_GHAC},

	{"ENX",						SCR_ENX},
	{"ENY",						SCR_ENY},
	{"EN@",						SCR_ENTIMER},
	{"ENI",						SCR_ENI},
	{"ENANGLE",					SCR_ENANGLE},
	{"ENSPEED",					SCR_ENSPEED},
	{"ENAMAP",					SCR_ENAMAP},
	{"ENRMAP",					SCR_ENRMAP},
	{"ENAIMX",					SCR_ENAIMX},
	{"ENAIMY",					SCR_ENAIMY},
	{"ENLIFE",					SCR_ENLIFE},

	{"RAND",					SCR_RAND},
	{"RANDR",					SCR_RANDR},
	{"SEED",					SCR_SEED},
	{"SINA",					SCR_SINA},
	{"COSA",					SCR_COSA},
	{"TANA",					SCR_TANA},
	{"ASIN2",					SCR_ASIN2},
	{"ACOS2",					SCR_ACOS2},
	{"ATAN2",					SCR_ATAN2},
	{"SQRT",					SCR_SQRT},
	{"SIGN",					SCR_SIGN},
	{"ROLL",					SCR_ROLL},

	{"D",						SCR_D},
	{"Du",						SCR_DU},
	{"TX",						SCR_TX},
	{"TY",						SCR_TY},
	{"TIME",					SCR_TIME},
	{"NOW#",					SCR_NOWNAME},
	{"NOW@",					SCR_NOWCON},
	{"DIFFI",					SCR_DIFFI},
	{"SNOSTAGE",				SCR_SNOSTAGE},
	{"SNODIFFI",				SCR_SNODIFFI},
	{"SNOBATTLE",				SCR_SNOBATTLE},
	{"SNOUSER",					SCR_SNOUSER},
	{"CHARA",					SCR_CHARA},
	{"GETSCENE",				SCR_GETSCENE},
	{"MODE",					SCR_MODE},
	{"REPLAYMODE",				SCR_REPLAYMODE},
	{"FRAMESKIP",				SCR_SETFRAMESKIP},

	{"CHATI",					SCR_CHATI},
	{"BOSSFAILED",				SCR_BOSSFAILED},
	{"BOSSFLAG",				SCR_BOSSFLAG},
	{"GETPEID",					SCR_GETPEID},
	{"CHECKKEY",				SCR_CHECKKEY},
	{"GETFLAG",					SCR_GETFLAG},
	{"PLAYERNAME",				SCR_PLAYERNAME},
	{"BOSSNAME",				SCR_BOSSNAME},
	{"SPELLNAME",				SCR_SPELLNAME},
	{"SPELLUSERNAME",			SCR_SPELLUSERNAME},
	{"SPELLUSERENAME",			SCR_SPELLUSERENAME},

	{"BGSI",					SCR_BGSINDEX},
	{"BGS@",					SCR_BGSTIMER},

	{"SELCOMPLETE",				SCR_SELCOMPLETE},
	{"SEL",						SCR_SEL},
	{"SELFIRSTID",				SCR_SELFIRSTID},
	{"ISELCOMPLETE",			SCR_ISELCOMPLETE},
	{"ISEL",					SCR_ISEL},
	{"ISELFIRSTID",				SCR_ISELFIRSTID},

	{"PX",						SCR_PX},
	{"PY",						SCR_PY},
	{"PLIFE",					SCR_PLIFE},
	{"PBOMB",					SCR_PBOMB},
	{"PPOWER",					SCR_PPOWER},
	{"PFAITH",					SCR_PFAITH},
	{"PPOINT",					SCR_PPOINT},
	{"PBBORDER",				SCR_PBBORDER},
	{"PBSLOW",					SCR_PBSLOW},
	{"PBINFI",					SCR_PBINFI},
	{"PSPEED",					SCR_PSPEED},
	{"PSLOWSPEED",				SCR_PSLOWSPEED},
	{"PGX",						SCR_PGX},
	{"PGY",						SCR_PGY},
	{"HAVEPLAYER",				SCR_HAVEPLAYER},

	{SCR_CONST_STR,				SCR_CONST},

	{"true",					true},
	{"false",					false},

	{"STATE_START",				STATE_START},
	{"STATE_PAUSE",				STATE_PAUSE},
	{"STATE_CONTINUE",			STATE_CONTINUE},
	{"STATE_CLEAR",				STATE_CLEAR},
	{"STATE_ENDING",			STATE_ENDING},
	{"STATE_TITLE",				STATE_TITLE},
	{"STATE_PLAYER_SELECT",		STATE_PLAYER_SELECT},
	{"STATE_DIFFICULT_SELECT",	STATE_DIFFICULT_SELECT},
	{"STATE_OVER",				STATE_OVER},
	{"STATE_SPELL",				STATE_SPELL},
	{"STATE_REPLAY",			STATE_REPLAY},
	{"STATE_RESULT",			STATE_RESULT},
	{"STATE_MUSIC",				STATE_MUSIC},
	{"STATE_OPTION",			STATE_OPTION},
	{"STATE_INIT",				STATE_INIT},

	{"PQUIT",					PQUIT},
	{"PGO",						PGO},
	{"POK",						POK},
	{"PBACK",					PBACK},
	{"PTURN",					PTURN},
	{"PSKIP",					PSKIP},

	{"THEN",					THEN},
	{"SECTIONEND",				SECTIONEND},
	{"EVERY",					EVERY},
	{"EVERYMOD",				EVERYMOD},
	{"TIMERGREAT",				TIMERGREAT},
	{"TIMEREQUAL",				TIMEREQUAL},
	{"TIMERLESS",				TIMERLESS},
	{"TIMERRANGE",				TIMERRANGE},
	{"TYPEEQUAL",				TYPEEQUAL},
	{"COLOREQUAL",				COLOREQUAL},
	{"ANGLEGREAT",				ANGLEGREAT},
	{"ANGLEEQUAL",				ANGLEEQUAL},
	{"ANGLELESS",				ANGLELESS},
	{"ANGLERANGE",				ANGLERANGE},
	{"XGREAT",					XGREAT},
	{"XLESS",					XLESS},
	{"XRANGE",					XRANGE},
	{"YGREAT",					YGREAT},
	{"YLESS",					YLESS},
	{"YRANGE",					YRANGE},
	{"VALGREAT",				VALGREAT},
	{"VALEQUAL",				VALEQUAL},
	{"VALLESS",					VALLESS},
	{"VALRANGE",				VALRANGE},
	{"SPEEDGREAT",				SPEEDGREAT},
	{"SPEEDEQUAL",				SPEEDEQUAL},
	{"SPEEDLESS",				SPEEDLESS},
	{"SPEEDRANGE",				SPEEDRANGE},
	{"INDEXMODGREAT",			INDEXMODGREAT},
	{"INDEXMODEQUAL",			INDEXMODEQUAL},
	{"INDEXMODLESS",			INDEXMODLESS},
	{"INDEXMODRANGE",			INDEXMODRANGE},
	{"BOUNCEGREAT",				BOUNCEGREAT},
	{"BOUNCEEQUAL",				BOUNCEEQUAL},
	{"BOUNCELESS",				BOUNCELESS},
	{"BOUNCERANGE",				BOUNCERANGE},
	{"TYPESET",					TYPESET},
	{"COLORSET",				COLORSET},
	{"ANGLESET",				ANGLESET},
	{"ANGLESETADD",				ANGLESETADD},
	{"ANGLEADJUST",				ANGLEADJUST},
	{"HEADANGLESET",			HEADANGLESET},
	{"HEADANGLESETADD",			HEADANGLESETADD},
	{"ANGLESETRMA",				ANGLESETRMA},
	{"ANGLESETRMAP",			ANGLESETRMAP},
	{"ANGLESETRMAT",			ANGLESETRMAT},
	{"ANGLESETAMA",				ANGLESETAMA},
	{"ANGLESETAMAP",			ANGLESETAMAP},
	{"ANGLESETAMAT",			ANGLESETAMAT},
	{"ANGLESETRAND",			ANGLESETRAND},
	{"ANGLESETADDRAND",			ANGLESETADDRAND},
	{"XSET",					XSET},
	{"YSET",					YSET},
	{"XSETADD",					XSETADD},
	{"YSETADD",					YSETADD},
	{"XSETACCADD",				XSETACCADD},
	{"YSETACCADD",				YSETACCADD},
	{"VALSET",					VALSET},
	{"VALSETADD",				VALSETADD},
	{"SPEEDSET",				SPEEDSET},
	{"SPEEDSETADD",				SPEEDSETADD},
	{"SPEEDSETMUL",				SPEEDSETMUL},
	{"CALLEVENT",				CALLEVENT},
	{"CHASE",					CHASE},
	{"AND",						AND},
	{"OR",						OR},
	{"NOT",						NOT},
	{"CONDITIONBYVAL",			CONDITIONBYVAL},
	{"CONDITIONBYINDEX",		CONDITIONBYINDEX},
	{"EXECUTEBYVAL",			EXECUTEBYVAL},
	{"EXECUTEBYINDEX",			EXECUTEBYINDEX},
	{"REMAIN",					REMAIN},
	{"DECANCEL",				DECANCEL},
	{"FADEOUT",					FADEOUT},
	{"BOUNCE",					BOUNCE},
	{"BOUNCELR",				BOUNCELR},
	{"BOUNCETB",				BOUNCETB},
	{"FAITHLIZE",				FAITHLIZE},
	{"POINTLIZE",				POINTLIZE},
	{"SCORELIZE",				SCORELIZE},

	{"ENAC_NONE",					ENAC_NONE},
	{"ENAC_DIRECTSET_XYSOAOOO",		ENAC_DIRECTSET_XYSOAOOO},
	{"ENAC_CHASEPLAYER_OOSFATOO",	ENAC_CHASEPLAYER_OOSFATOO},
	{"ENAC_CHASEAIM_XYSOAOCO",		ENAC_CHASEAIM_XYSOAOCO},
	{"ENAC_TURNANGLE_OOOOATOE",		ENAC_TURNANGLE_OOOOATOE},
	{"ENAC_FADEOUT_OOOOOTOO",		ENAC_FADEOUT_OOOOOTOO},
	{"ENAC_REPOSITION_OOOOOOCO",	ENAC_REPOSITION_OOOOOOCO},
	{"ENAC_OVERPLAYER_XYOOOTCE",	ENAC_OVERPLAYER_XYOOOTCE},
	{"ENAC_DELAY_OOOOOTOO",			ENAC_DELAY_OOOOOTOO},

	{"INFO_GREEN",				INFO_GREEN},
	{"INFO_YELLOW",				INFO_YELLOW},
	{"INFO_GRAY",				INFO_GRAY},
	{"INFO_RED",				INFO_RED},
	{"INFO_BLUE",				INFO_BLUE},

	{"ITEM_POWER",				ITEM_POWER},
	{"ITEM_POINT",				ITEM_POINT},
	{"ITEM_FAITH",				ITEM_FAITH},
	{"ITEM_BIGPOWER",			ITEM_BIGPOWER},
	{"ITEM_BOMB",				ITEM_BOMB},
	{"ITEM_FULL",				ITEM_FULL},
	{"ITEM_EXTEND",				ITEM_EXTEND},
	{"ITEM_SMALLFAITH",			ITEM_SMALLFAITH},

	{"ITEM_STARTSPEED",			ITEM_STARTSPEED},

	{"BGMASK",					UBGID_BGMASK},
	{"FGPAUSE",					UFGID_FGPAUSE},

	{"BG_NONE",					BG_NONE},
	{"BG_WHITEFLASH",			BG_WHITEFLASH},
	{"BG_REDFLASH",				BG_REDFLASH},
	{"BG_YELLOWFLASH",			BG_YELLOWFLASH},
	{"BG_WHITEOUT",				BG_WHITEOUT},
	{"BG_REDOUT",				BG_REDOUT},
	{"BG_FADEIN",				BG_FADEIN},
	{"BG_FADEINHALF",			BG_FADEINHALF},
	{"BG_FADEOUT",				BG_FADEOUT},
	{"BG_LIGHTUP",				BG_LIGHTUP},
	{"BG_LIGHTUPNORMAL",		BG_LIGHTUPNORMAL},
	{"BG_LIGHTRED",				BG_LIGHTRED},
	{"BG_PAUSEIN",				FG_PAUSEIN},
	{"BG_PAUSEOUT",				FG_PAUSEOUT},

	{"CS_LNN",					CHATSPRITE_LEFT},
	{"CS_RNN",					CHATSPRITE_RIGHT},
	{"CS_LFN",					(CHATSPRITE_LEFT|CHATSPRITE_LEFTFLIP)},
	{"CS_RFN",					(CHATSPRITE_RIGHT|CHATSPRITE_LEFTFLIP)},
	{"CS_LNF",					(CHATSPRITE_LEFT|CHATSPRITE_RIGHTFLIP)},
	{"CS_RNF",					(CHATSPRITE_RIGHT|CHATSPRITE_RIGHTFLIP)},
	{"CS_LFF",					(CHATSPRITE_LEFT|CHATSPRITE_LEFTFLIP|CHATSPRITE_RIGHTFLIP)},
	{"CS_RFF",					(CHATSPRITE_RIGHT|CHATSPRITE_LEFTFLIP|CHATSPRITE_RIGHTFLIP)},

	{"BLEND_COLORADD",			BLEND_COLORADD},
	{"BLEND_COLORMUL",			BLEND_COLORMUL},
	{"BLEND_ALPHABLEND",		BLEND_ALPHABLEND},
	{"BLEND_ALPHAADD",			BLEND_ALPHAADD},
	{"BLEND_ZWRITE",			BLEND_ZWRITE},
	{"BLEND_NOZWRITE",			BLEND_NOZWRITE},
	{"BLEND_DEFAULT",			BLEND_DEFAULT},
	{"BLEND_DEFAULT_Z",			BLEND_DEFAULT_Z},

	{"BGMT_FLASH",				BGMT_FLASH},
	{"BGMT_OUT",				BGMT_OUT},
	{"BGMT_FADE",				BGMT_FADE},
	{"BGMT_LIGHT",				BGMT_LIGHT},
	{"FGMT_PAUSE",				FGMT_PAUSE},

	{"BEAMFLAG_NONE",			BEAMFLAG_NONE},
	{"BEAMFLAG_HORIZON",		BEAMFLAG_HORIZON},
	{"BEAMFLAG_STOP",			BEAMFLAG_STOP},
	{"BEAMFLAG_HS",				BEAMFLAG_HS},
	{"BEAMFLAG_NOGRAZE",		BEAMFLAG_NOGRAZE},

	{"SE_DEFAULT",				SE_DEFAULT},
	{"SE_PLAYER_GRAZE",			SE_PLAYER_GRAZE},
	{"SE_PLAYER_SHOT",			SE_PLAYER_SHOT},
	{"SE_PLAYER_DEAD",			SE_PLAYER_DEAD},
	{"SE_PLAYER_SLOWON",		SE_PLAYER_SLOWON},
	{"SE_PLAYER_SLOWOFF",		SE_PLAYER_SLOWOFF},
	{"SE_PLAYER_CHANGE",		SE_PLAYER_CHANGE},
	{"SE_PLAYER_BORDERON",		SE_PLAYER_BORDERON},
	{"SE_PLAYER_BORDEROFF",		SE_PLAYER_BORDEROFF},
	{"SE_PLAYER_EXPLODE",		SE_PLAYER_EXPLODE},
	{"SE_BULLET_CHANGE_1",		SE_BULLET_CHANGE_1},
	{"SE_BULLET_CHANGE_2",		SE_BULLET_CHANGE_2},
	{"SE_BULLET_BOUND",			SE_BULLET_BOUND},
	{"SE_BULLET_FADEOUT",		SE_BULLET_FADEOUT},
	{"SE_BEAM_1",				SE_BEAM_1},
	{"SE_BEAM_2",				SE_BEAM_2},
	{"SE_BEAM_REFLECT",			SE_BEAM_REFLECT},
	{"SE_BEAM_FADEOUT",			SE_BEAM_FADEOUT},
	{"SE_BOSS_UP",				SE_BOSS_UP},
	{"SE_BOSS_DEAD",			SE_BOSS_DEAD},
	{"SE_BOSS_TIMEOUT",			SE_BOSS_TIMEOUT},
	{"SE_BOSS_TIMEUP",			SE_BOSS_TIMEUP},
	{"SE_BOSS_KIRA",			SE_BOSS_KIRA},
	{"SE_BOSS_POWER_1",			SE_BOSS_POWER_1},
	{"SE_BOSS_POWER_2",			SE_BOSS_POWER_2},
	{"SE_BOSS_BONUS",			SE_BOSS_BONUS},
	{"SE_ENEMY_DAMAGE_1",		SE_ENEMY_DAMAGE_1},
	{"SE_ENEMY_DAMAGE_2",		SE_ENEMY_DAMAGE_2},
	{"SE_ENEMY_DEAD",			SE_ENEMY_DEAD},
	{"SE_GHOST_DEAD",			SE_GHOST_DEAD},
	{"SE_GHOST_MERGE",			SE_GHOST_MERGE},
	{"SE_GHOST_HALF",			SE_GHOST_HALF},
	{"SE_ITEM_GET",				SE_ITEM_GET},
	{"SE_ITEM_EXTEND",			SE_ITEM_EXTEND},
	{"SE_ITEM_POWERUP",			SE_ITEM_POWERUP},
	{"SE_SYSTEM_OK",			SE_SYSTEM_OK},
	{"SE_SYSTEM_CANCEL",		SE_SYSTEM_CANCEL},
	{"SE_SYSTEM_SELECT",		SE_SYSTEM_SELECT},
	{"SE_SYSTEM_PAUSE",			SE_SYSTEM_PAUSE},

	{"MAXDESC",					SCR_MAXDESC},
	{"VARBEGIN",				SCR_VARBEGIN},
	{"FREEBEGIN",				SCR_FREEBEGIN},
	{"RESERVEBEGIN",			SCR_RESERVEBEGIN},

	{"INIT@",					SCRIPT_CON_INIT},
	{"POST@",					SCRIPT_CON_POST},
	{"QUIT@",					SCRIPT_CON_QUIT},

	{"BTYPE_BULLET",			BTYPE_BULLET},
	{"BTYPE_BEAM",				BTYPE_BEAM},
	{"BTYPE_ENEMY",				BTYPE_ENEMY},
	{"BTYPE_GHOST",				BTYPE_GHOST},

	{"CT_CONTROL",				SCR_CTCONTROL},
	{"CT_STAGE",				SCR_CTSTAGE},
	{"CT_EDEF",					SCR_CTEDEF},
	{"CT_SCENE",				SCR_CTSCENE},
	{"CT_FUNCTION",				SCR_CTFUNCTION},

	{"KEY_PRESSED",				DIKEY_PRESSED},
	{"KEY_DOWN",				DIKEY_DOWN},
	{"KEY_UP",					DIKEY_UP},

	{"STOPFLAG_WORLDSHAKE",		FRAME_STOPFLAG_WORLDSHAKE},
	{"STOPFLAG_PLAYER",			FRAME_STOPFLAG_PLAYER},
	{"STOPFLAG_PLAYERBULLET",	FRAME_STOPFLAG_PLAYERBULLET},
	{"STOPFLAG_ENEMY",			FRAME_STOPFLAG_ENEMY},
	{"STOPFLAG_GHOST",			FRAME_STOPFLAG_GHOST},
	{"STOPFLAG_BULLET",			FRAME_STOPFLAG_BULLET},
	{"STOPFLAG_BEAM",			FRAME_STOPFLAG_BEAM},
	{"STOPFLAG_ITEM",			FRAME_STOPFLAG_ITEM},
	{"STOPFLAG_LAYER",			FRAME_STOPFLAG_LAYER},
	{"STOPFLAG_EFFECTSYS",		FRAME_STOPFLAG_EFFECTSYS},
	{"STOPFLAG_PLAYERSET",		FRAME_STOPFLAG_PLAYERSET},
	{"STOPFLAG_ENEMYSET",		FRAME_STOPFLAG_ENEMYSET},

	{"PLBONUS_GRAZE",			PLBONUS_GRAZE},
	{"PLBONUS_SHOOT",			PLBONUS_SHOOT},
	{"PLBONUS_TIME",			PLBONUS_TIME},

	{"IZEZONE_DEFAULTTIME",		IZEZONE_DEFAULTTIME},
	{"IZEOVERZONE",				BULLET_IZEOVERZONE},
	{"IZE_EVENT",				BULLETIZE_EVENT},
	{"IZE_FADEOUT",				BULLETIZE_FADEOUT},
	{"IZE_FAITH",				BULLETIZE_FAITH},
	{"IZE_POINT",				BULLETIZE_POINT},
	{"IZE_SCORE",				BULLETIZE_SCORE},

	{"MODE_NORMAL",				M_MODE_NORMAL},
	{"MODE_STAGE",				M_MODE_STAGE},
	{"MODE_SPELL",				M_MODE_SPELL},

	{"PUSH_FIRST",				M_PUSH_FIRST},
	{"PUSH_ROLLTO",				M_PUSH_ROLLTO},
	{"PUSH_SKIP",				M_PUSH_SKIP},
	{"PUSHID_UIUSE",			PUSHKEY_ID_UIUSE},
	{"PUSHID_GAMEUSE",			PUSHKEY_ID_GAMEUSE},

	{"SEL_NULL",				SEL_NULL},
	{"SEL_NONACTIVE",			SEL_NONACTIVE},
	{"SEL_GRAY",				SEL_GRAY},
	{"SEL_STAY",				SEL_STAY},
	{"SEL_NOSHIFT",				SEL_NOSHIFT},

	{"TotalCenterX",			M_CLIENT_CENTER_X},
	{"TotalCenterY",			M_CLIENT_CENTER_Y},
	{"TotalW",					M_CLIENT_WIDTH},
	{"TotalH",					M_CLIENT_HEIGHT},
	{"CenterX",					M_ACTIVECLIENT_CENTER_X},
	{"CenterY",					M_ACTIVECLIENT_CENTER_Y},
	{"CenterW",					M_ACTIVECLIENT_WIDTH},
	{"CenterH",					M_ACTIVECLIENT_HEIGHT},
	{"CenterBossX",				M_ACTIVECLIENT_BOSSX},
	{"CenterBossY",				M_ACTIVECLIENT_BOSSY},

	{"EVENT_ENTERSTATE",	SCR_EVENT_ENTERSTATE},
	{"EVENT_LEAVESTATE",	SCR_EVENT_LEAVESTATE},

	{"EVENT_PLAYERBOMB",		SCR_EVENT_PLAYERBOMB},

	{"EVENT_BULLETENTERIZE",	SCR_EVENT_BULLETENTERIZE},

	{SCR_KEYSTATE_STR,			SCR_KEYSTATE},

	{"KS_UP",					(DWORD)&(Process::mp.keyUp)},
	{"KS_DOWN",					(DWORD)&(Process::mp.keyDown)},
	{"KS_LEFT",					(DWORD)&(Process::mp.keyLeft)},
	{"KS_RIGHT",				(DWORD)&(Process::mp.keyRight)},
	{"KS_FIRE",					(DWORD)&(Process::mp.keyFire)},
	{"KS_SPECIAL",				(DWORD)&(Process::mp.keySpecial)},
	{"KS_SLOW",					(DWORD)&(Process::mp.keySlow)},
	{"KS_CHANGE",				(DWORD)&(Process::mp.keyChange)},
	{"KS_PAUSE",				(DWORD)&(Process::mp.keyPause)},
	{"KS_SKIP",					(DWORD)&(Process::mp.keySkip)},
	{"KS_ENTER",				(DWORD)&(Process::mp.keyEnter)},
	{"KS_ESCAPE",				(DWORD)&(Process::mp.keyEscape)},
	{"KS_CAPTURE",				(DWORD)&(Process::mp.keyCapture)},

	{SCR_NULL_STR,				SCR_NULL},
};