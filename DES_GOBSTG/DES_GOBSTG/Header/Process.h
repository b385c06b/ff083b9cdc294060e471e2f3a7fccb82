#ifndef _PROCESS_H
#define _PROCESS_H

#include "Main.h"
#include "Export.h"
#include "Const.h"
#include "ProcessDefine.h"

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

#define KS_UP_MP		Process::mp.keyUp
#define KS_DOWN_MP		Process::mp.keyDown
#define KS_LEFT_MP		Process::mp.keyLeft
#define KS_RIGHT_MP		Process::mp.keyRight
#define KS_FIRE_MP		Process::mp.keyFire
#define KS_SPECIAL_MP	Process::mp.keySpecial
#define KS_SLOW_MP		Process::mp.keySlow
#define KS_CHANGE_MP	Process::mp.keyChange
#define KS_PAUSE_MP		Process::mp.keyPause
#define KS_SKIP_MP		Process::mp.keySkip
#define KS_ENTER_MP		Process::mp.keyEnter
#define KS_ESCAPE_MP	Process::mp.keyEscape
#define KS_CAPTURE_MP	Process::mp.keyCapture

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

	void	SetStop(DWORD stopflag, int stoptime);
	DWORD	GetStopFlag(int index=-1);

	void	ClearAll();

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

	DWORD	stopflag[FRAME_STOPINFOMAX];
	int		stoptimer[FRAME_STOPINFOMAX];

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

	//

	float worldx;
	float worldy;
	float worldz;

	static Process mp;
};

#endif