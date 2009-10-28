#ifndef _PROCESS_H
#define _PROCESS_H

#include "Main.h"
#include "Export.h"
#include "Const.h"

#define KS_UP		keyUp
#define KS_DOWN		keyDown
#define KS_LEFT		keyLeft
#define KS_RIGHT	keyRight
#define KS_FIRE		keyFire
#define KS_SPECIAL	keySpecial
#define KS_SLOW		keySlow
#define KS_CHANGE	keyChange
#define KS_PAUSE	keyPause
#define KS_SKIP		keySkip
#define KS_ENTER	keyEnter
#define KS_ESCAPE	keyEscape
#define KS_CAPTURE	keyCapture

#define KS_UP_MP		mp.keyUp
#define KS_DOWN_MP		mp.keyDown
#define KS_LEFT_MP		mp.keyLeft
#define KS_RIGHT_MP		mp.keyRight
#define KS_FIRE_MP		mp.keyFire
#define KS_SPECIAL_MP	mp.keySpecial
#define KS_SLOW_MP		mp.keySlow
#define KS_CHANGE_MP	mp.keyChange
#define KS_PAUSE_MP		mp.keyPause
#define KS_SKIP_MP		mp.keySkip
#define KS_ENTER_MP		mp.keyEnter
#define KS_ESCAPE_MP	mp.keyEscape
#define KS_CAPTURE_MP	mp.keyCapture

//process return value
enum
{
	PQUIT,
	PGO,
	POK,
	PBACK,
	PTURN,
	PSKIP
};
//state indicator
enum
{
	STATE_START,
	STATE_PAUSE,
	STATE_CONTINUE,
	STATE_CLEAR,
	STATE_ENDING,
	STATE_TITLE,
	STATE_PLAYER_SELECT,
	STATE_DIFFICULT_SELECT,
	STATE_OVER,
	STATE_SPELL,
	STATE_REPLAY,
	STATE_RESULT,
	STATE_OPTION,
	STATE_MUSIC,
	STATE_INIT,
};
//errorcode
enum
{
	PROC_ERROR_NONE,
	PROC_ERROR_INIFILE,
	PROC_ERROR_RESOURCE,
	PROC_ERROR_SCRIPT,
	PROC_ERROR_TRANSLATE,
	PROC_ERROR_DATA,
	PROC_ERROR_PACKAGE,
	PROC_ERROR_SOUND,
	PROC_ERROR_FONT,
	PROC_ERROR_TEXTURE
};

#define FRAME_STOPFLAG_WORLDSHAKE	0x0001
#define FRAME_STOPFLAG_PLAYER		0x0002
#define FRAME_STOPFLAG_PLAYERBULLET	0x0004
#define FRAME_STOPFLAG_ENEMY		0x0008
#define FRAME_STOPFLAG_GHOST		0x0010
#define FRAME_STOPFLAG_BULLET		0x0020
#define FRAME_STOPFLAG_BEAM			0x0040
#define FRAME_STOPFLAG_ITEM			0x0080
#define FRAME_STOPFLAG_LAYER		0x0100
#define FRAME_STOPFLAG_EFFECTSYS	0x0200

#define FRAME_STOPFLAG_PLAYERSET	(FRAME_STOPFLAG_WORLDSHAKE|FRAME_STOPFLAG_PLAYER|FRAME_STOPFLAG_PLAYERBULLET|FRAME_STOPFLAG_LAYER)
#define FRAME_STOPFLAG_ENEMYSET		(FRAME_STOPFLAG_WORLDSHAKE|FRAME_STOPFLAG_ENEMY|FRAME_STOPFLAG_GHOST|FRAME_STOPFLAG_BULLET|FRAME_STOPFLAG_BEAM|FRAME_STOPFLAG_ITEM)

class Process
{
public:
	Process();
	virtual ~Process();

	void	Realease();

	bool	reload();

	int		getInput();
	char	getInputNowChar(bool wide);

	int		processStart();
	int		processPause();
	int		processContinue();
	int		processClear();
	int		processEnding();
	int		processTitle();
	int		processPlayerSelect();
	int		processDifficultSelect();
	int		processOver();
	int		processSpell();
	int		processReplay();
	int		processResult();
	int		processOption();
	int		processMusic();
	int		processInit();
	int		processPreInitial();

	int		frame();
	int		render();
	int		renderInit();

	void	startPrep(bool callinit = true);
	void	clearPrep(bool bclearkey = true);

	void	frameStart();
	void	frameEnd();

	void	playSE(const char * sample_name,int pan = 0);
	void	musicChange(BYTE ID, bool force = false);
	void	musicSlide(float slidetime, int tovol = 0, int pan = -101, float pitch = -1);

	void	SnapShot();

	void	SetShake(BYTE round, bool force=false);
	void	WorldShake();

public:
	union{
		struct{
			int keyUp;
			int keyDown;
			int keyLeft;
			int keyRight;
			int keyFire;
			int keySpecial;
			int keySlow;
			int keyChange;
			int keyPause;
			int keySkip;
			int keyEnter;
			int keyEscape;
			int keyCapture;
		};
		int keyKey[13];
	};
	union{
		struct{
			int	joyFire;
			int	joySpecial;
			int	joySlow;
			int	joyChange;
			int	joyPause;
		};
		int		joyKey[5];
	};
#ifdef __DEBUG
	int debug_joySpeedUp;
#endif

	//texture
	HTEXTURE	tex[TEXMAX];

	//read ini
	char	username[RPYINFO_USERNAMEMAX];
	int		screenmode;
	int		bgmvol;
	int		sevol;
	int		defaultdifflv;

	//replay
	char	rpyfilename[M_STRITOAMAX];
	DWORD	seed;
	int		framecounter;
	int		frameskip;
	int		renderskip;
	bool	replaymode;
	bool	replayend;
	//playtime
	LONGLONG	playtimeStart;
	bool		playing;
	//music
	HSTREAM		stream;
	HCHANNEL	channel;
	int			musicID;
	hgeChannelSyncInfo channelsyncinfo;

	//shake
	int worldshaketimer;
	BYTE round;

	//main
	DWORD lasttime;
	DWORD alltime;
	bool	active;

	DWORD	stopflag;
	int		stoptimer;

	int retvalue;

	DWORD	scene;
	DWORD	startscene;
	DWORD	endscene;
	//practice
	bool	practicemode;
	bool	spellmode;
	//pause return
	bool	pauseinit;
	//Replayinfo
	float	replayFPS;
	//error
	int	errorcode;
	//fps
	int	objcount;

	//indicator
	BYTE	nowdifflv;
	BYTE	mainchara;
	BYTE	subchara_1;
	BYTE	subchara_2;
	BYTE	titleselect;
	BYTE	state;
	//input
	BYTE	nowInput;
	BYTE	saveInput;

	//Init
	HTEXTURE	texInit;
};

extern Process mp;

extern int randi;
extern BYTE tari;

extern replayFrame replayframe[M_SAVEINPUTMAX];
extern DWORD replayIndex;

extern float worldx;
extern float worldy;
extern float worldz;

#endif