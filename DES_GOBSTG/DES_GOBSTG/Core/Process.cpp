#include "processPrep.h"

replayFrame replayframe[M_SAVEINPUTMAX];

Process mp;

DWORD replayIndex = 0;

float worldx = 0;
float worldy = 0;
float worldz = 0;

int randi = 0;
BYTE tari = 0;

Process::Process()
{
	active		= false;
	stream		= NULL;
	channel		= NULL;
	ZeroMemory(&channelsyncinfo, sizeof(hgeChannelSyncInfo));
	retvalue	= PGO;
	errorcode	= PROC_ERROR_INIFILE;
	for(int i=0;i<TEXMAX;i++)
	{
		tex[i] = NULL;
	}
	keyUp		= RESCONFIGDEFAULT_KEYUP;
	keyDown		= RESCONFIGDEFAULT_KEYDOWN;
	keyLeft		= RESCONFIGDEFAULT_KEYLEFT;
	keyRight	= RESCONFIGDEFAULT_KEYRIGHT;
	keyFire		= RESCONFIGDEFAULT_KEYFIRE;
	keySpecial		= RESCONFIGDEFAULT_KEYSPECIAL;
	keySlow		= RESCONFIGDEFAULT_KEYSLOW;
	keyChange	= RESCONFIGDEFAULT_KEYCHANGE;
	keyPause	= RESCONFIGDEFAULT_KEYPAUSE;
	keySkip		= RESCONFIGDEFAULT_KEYSKIP;
	keyEnter	= RESCONFIGDEFAULT_KEYENTER;
	keyEscape	= RESCONFIGDEFAULT_KEYESCAPE;
	keyCapture	= RESCONFIGDEFAULT_KEYCAPTURE;
	musicID = -1;
	screenmode = 0;

	texInit = NULL;
}

Process::~Process()
{
}

void Process::Realease()
{
	if (!errorcode)
	{
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, keyUp);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, keyDown);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, keyLeft);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, keyRight);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, keyFire);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSPECIAL, keySpecial);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, keySlow);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCHANGE, keyChange);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, keyPause);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, keySkip);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, keyEnter);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, keyEscape);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, keyCapture);

		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, joyFire);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSPECIAL, joySpecial);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, joySlow);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCHANGE, joyChange);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, joyPause);
#ifdef __DEBUG
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_DEBUG_JOYSPEEDUP, debug_joySpeedUp);
#endif

		hge->	Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, bgmvol);
		hge->	Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, sevol);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, screenmode);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_DEFAULTLEVEL, defaultdifflv);
		hge->	Ini_SetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, username);
		if(playing)
			DataConnector::addPlayTime();

		if (!data.bin.empty())
		{
			data.SaveBin();
		}
	}

	//
	Selector::Clear();
	Bullet::Release();
	PlayerBullet::Release();
	Item::Release();
	Fontsys::Release();
	BossInfo::Release();
	chat.Release();
	fdisp.Release();

	for(int i=0;i<TEXMAX;i++)
	{
		if(tex[i])
			hge->Texture_Free(tex[i]);
		tex[i] = NULL;
	}
	if (texInit)
	{
		hge->Texture_Free(texInit);
		texInit = NULL;
	}
	hge->Stream_Free(stream);
}


void Process::musicSlide(float slidetime, int tovol, int pan, float pitch)
{
	if (hge->Channel_IsPlaying(channel))
	{
		hge->Channel_SlideTo(channel, slidetime, tovol < 0 ? bgmvol : tovol, pan, pitch);
	}
}

void Process::SetShake(BYTE round, bool force/* =false */)
{
	if (force || worldshaketimer == 0)
	{
		worldshaketimer = round * 15;
	}
}

void Process::WorldShake()
{
	if (worldshaketimer)
	{
		worldshaketimer--;
		if (!worldshaketimer)
		{
			round = 0;
			worldx = 0;
			worldy = 0;
		}
		else
		{
			int tangle = (round*15-worldshaketimer) * 4800;
			worldx = sint(tangle) * 10;
			worldy = cost(tangle) * 10;
		}
	}
}

void Process::musicChange(BYTE ID, bool force)
{
	if(!ID)
	{
		hge->Channel_RemoveLoop(channel, &channelsyncinfo);
		ZeroMemory(&channelsyncinfo, sizeof(hgeChannelSyncInfo));
		hge->Channel_Stop(channel);
		musicID = -1;
		return;
	}
	if(!hge->Channel_IsPlaying(channel) || musicID != ID-1 || force)
	{
		if (musicID < 0 || strcmp(res.musdata[ID-1].musicfilename, res.musdata[musicID].musicfilename))
		{
			if(stream)
				hge->Stream_Free(stream);
			stream = hge->Stream_Load(res.musdata[ID-1].musicfilename, 0, false);
		}
		musicID = ID-1;
		channelsyncinfo.startPos = res.musdata[musicID].startpos;
		channelsyncinfo.introLength = res.musdata[musicID].introlength;
		channelsyncinfo.allLength = res.musdata[musicID].alllength;
		if (channel)
		{
			musicSlide(0, bgmvol);
			hge->Channel_Stop(channel);
		}
		channel = hge->Stream_Play(stream, true, bgmvol);
		hge->Channel_SetPitch(channel, 1.0f);
		hge->Channel_SetLoop(channel, &channelsyncinfo);
		hge->Channel_SetStartPos(channel, &channelsyncinfo);
	}
}

void Process::SnapShot()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	char snapshotfilename[M_PATHMAX];
	strcpy(snapshotfilename, "");
	sprintf(snapshotfilename, "%s%s_%04d_%02d_%02d_%02d_%02d_%02d_%04d.%s",
		res.resdata.snapshotfoldername,
		SNAPSHOT_PRIFIX,
		systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds,
		SNAPSHOT_EXTENSION);
	hge->System_Snapshot(snapshotfilename);
}

char Process::getInputNowChar(bool wide)
{
	char nowchar = -1;
	if(hge->Input_GetDIKey(KS_FIRE) && hge->Input_GetDIJoy(joyFire))
	{
		hge->Input_SetDIKey(KS_FIRE, false);
	}
	if(hge->Input_GetDIKey(KS_SPECIAL) && hge->Input_GetDIJoy(joySpecial))
	{
		hge->Input_SetDIKey(KS_SPECIAL, false);
	}
	if(hge->Input_GetDIKey(KS_SLOW) && hge->Input_GetDIJoy(joySlow))
	{
		hge->Input_SetDIKey(KS_SLOW, false);
	}
	if(hge->Input_GetDIKey(KS_CHANGE) && hge->Input_GetDIJoy(joyChange))
	{
		hge->Input_SetDIKey(KS_CHANGE, false);
	}
	if(hge->Input_GetDIKey(KS_PAUSE) && hge->Input_GetDIJoy(joyPause))
	{
		hge->Input_SetDIKey(KS_PAUSE, false);
	}

	if(hge->Input_GetDIKey(DIK_1, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD1, DIKEY_DOWN))
	{
		nowchar = '1';
	}
	else if(hge->Input_GetDIKey(DIK_2, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD2, DIKEY_DOWN))
	{
		nowchar = '2';
	}
	else if(hge->Input_GetDIKey(DIK_3, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD3, DIKEY_DOWN))
	{
		nowchar = '3';
	}
	else if(hge->Input_GetDIKey(DIK_4, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD4, DIKEY_DOWN))
	{
		nowchar = '4';
	}
	else if(hge->Input_GetDIKey(DIK_5, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD5, DIKEY_DOWN))
	{
		nowchar = '5';
	}
	else if(hge->Input_GetDIKey(DIK_6, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD6, DIKEY_DOWN))
	{
		nowchar = '6';
	}
	else if(hge->Input_GetDIKey(DIK_7, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD7, DIKEY_DOWN))
	{
		nowchar = '7';
	}
	else if(hge->Input_GetDIKey(DIK_8, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD8, DIKEY_DOWN))
	{
		nowchar = '8';
	}
	else if(hge->Input_GetDIKey(DIK_9, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD9, DIKEY_DOWN))
	{
		nowchar = '9';
	}
	else if(hge->Input_GetDIKey(DIK_0, DIKEY_DOWN) || hge->Input_GetDIKey(DIK_NUMPAD0, DIKEY_DOWN))
	{
		nowchar = '0';
	}
	else if(hge->Input_GetDIKey(DIK_Q, DIKEY_DOWN))
	{
		nowchar = 'q';
	}
	else if(hge->Input_GetDIKey(DIK_W, DIKEY_DOWN))
	{
		nowchar = 'w';
	}
	else if(hge->Input_GetDIKey(DIK_E, DIKEY_DOWN))
	{
		nowchar = 'e';
	}
	else if(hge->Input_GetDIKey(DIK_R, DIKEY_DOWN))
	{
		nowchar = 'r';
	}
	else if(hge->Input_GetDIKey(DIK_T, DIKEY_DOWN))
	{
		nowchar = 't';
	}
	else if(hge->Input_GetDIKey(DIK_Y, DIKEY_DOWN))
	{
		nowchar = 'y';
	}
	else if(hge->Input_GetDIKey(DIK_U, DIKEY_DOWN))
	{
		nowchar = 'u';
	}
	else if(hge->Input_GetDIKey(DIK_I, DIKEY_DOWN))
	{
		nowchar = 'i';
	}
	else if(hge->Input_GetDIKey(DIK_O, DIKEY_DOWN))
	{
		nowchar = 'o';
	}
	else if(hge->Input_GetDIKey(DIK_P, DIKEY_DOWN))
	{
		nowchar = 'p';
	}
	else if(hge->Input_GetDIKey(DIK_A, DIKEY_DOWN))
	{
		nowchar = 'a';
	}
	else if(hge->Input_GetDIKey(DIK_S, DIKEY_DOWN))
	{
		nowchar = 's';
	}
	else if(hge->Input_GetDIKey(DIK_D, DIKEY_DOWN))
	{
		nowchar = 'd';
	}
	else if(hge->Input_GetDIKey(DIK_F, DIKEY_DOWN))
	{
		nowchar = 'f';
	}
	else if(hge->Input_GetDIKey(DIK_G, DIKEY_DOWN))
	{
		nowchar = 'g';
	}
	else if(hge->Input_GetDIKey(DIK_H, DIKEY_DOWN))
	{
		nowchar = 'h';
	}
	else if(hge->Input_GetDIKey(DIK_J, DIKEY_DOWN))
	{
		nowchar = 'j';
	}
	else if(hge->Input_GetDIKey(DIK_K, DIKEY_DOWN))
	{
		nowchar = 'k';
	}
	else if(hge->Input_GetDIKey(DIK_L, DIKEY_DOWN))
	{
		nowchar = 'l';
	}
	else if(hge->Input_GetDIKey(DIK_Z, DIKEY_DOWN))
	{
		nowchar = 'z';
	}
	else if(hge->Input_GetDIKey(DIK_X, DIKEY_DOWN))
	{
		nowchar = 'x';
	}
	else if(hge->Input_GetDIKey(DIK_C, DIKEY_DOWN))
	{
		nowchar = 'c';
	}
	else if(hge->Input_GetDIKey(DIK_V, DIKEY_DOWN))
	{
		nowchar = 'v';
	}
	else if(hge->Input_GetDIKey(DIK_B, DIKEY_DOWN))
	{
		nowchar = 'b';
	}
	else if(hge->Input_GetDIKey(DIK_N, DIKEY_DOWN))
	{
		nowchar = 'n';
	}
	else if(hge->Input_GetDIKey(DIK_M, DIKEY_DOWN))
	{
		nowchar = 'm';
	}
	else if(hge->Input_GetDIKey(DIK_MINUS, DIKEY_DOWN))
	{
		nowchar = '-';
	}

	if(wide)
	{
		if(hge->Input_GetDIKey(DIK_EQUALS, DIKEY_DOWN))
		{
			nowchar = '=';
		}
		else if(hge->Input_GetDIKey(DIK_LBRACKET, DIKEY_DOWN))
		{
			nowchar = '[';
		}
		else if(hge->Input_GetDIKey(DIK_RBRACKET, DIKEY_DOWN))
		{
			nowchar = ']';
		}
		else if(hge->Input_GetDIKey(DIK_SEMICOLON, DIKEY_DOWN))
		{
			nowchar = ';';
		}
		else if(hge->Input_GetDIKey(DIK_APOSTROPHE, DIKEY_DOWN))
		{
			nowchar = '\'';
		}
		else if(hge->Input_GetDIKey(DIK_GRAVE, DIKEY_DOWN))
		{
			nowchar = '`';
		}
		else if(hge->Input_GetDIKey(DIK_BACKSLASH, DIKEY_DOWN))
		{
			nowchar = '\\';
		}
		else if(hge->Input_GetDIKey(DIK_COMMA, DIKEY_DOWN))
		{
			nowchar = ',';
		}
		else if(hge->Input_GetDIKey(DIK_PERIOD, DIKEY_DOWN))
		{
			nowchar = '.';
		}
		else if(hge->Input_GetDIKey(DIK_SLASH, DIKEY_DOWN))
		{
			nowchar = '/';
		}
		else if(hge->Input_GetDIKey(DIK_SPACE, DIKEY_DOWN))
		{
			nowchar = ' ';
		}
		
		else if(hge->Input_GetDIKey(DIK_ADD, DIKEY_DOWN))
		{
			nowchar = '+';
		}
		else if(hge->Input_GetDIKey(DIK_SUBTRACT, DIKEY_DOWN))
		{
			nowchar = '-';
		}
		else if(hge->Input_GetDIKey(DIK_MULTIPLY, DIKEY_DOWN))
		{
			nowchar = '*';
		}
		else if(hge->Input_GetDIKey(DIK_DIVIDE, DIKEY_DOWN))
		{
			nowchar = '/';
		}
		else if(hge->Input_GetDIKey(DIK_DECIMAL, DIKEY_DOWN))
		{
			nowchar = '.';
		}
	}
	if(hge->Input_GetDIKey(DIK_LSHIFT) || hge->Input_GetDIKey(DIK_RSHIFT))
	{
		if(nowchar >= 'a' && nowchar <= 'z')
		{
			nowchar += 'A' - 'a';
		}
		else if(hge->Input_GetDIKey(DIK_MINUS, DIKEY_DOWN))
		{
			nowchar = '_';
		}
	
		if(wide)
		{
			if(nowchar == '1')
			{
				nowchar = '!';
			}
			else if(nowchar == '2')
			{
				nowchar = '@';
			}
			else if(nowchar == '3')
			{
				nowchar = '#';
			}
			else if(nowchar == '4')
			{
				nowchar = '$';
			}
			else if(nowchar == '5')
			{
				nowchar = '%';
			}
			else if(nowchar == '6')
			{
				nowchar = '^';
			}
			else if(nowchar == '7')
			{
				nowchar = '&';
			}
			else if(nowchar == '8')
			{
				nowchar = '*';
			}
			else if(nowchar == '9')
			{
				nowchar = '(';
			}
			else if(nowchar == '0')
			{
				nowchar = ')';
			}
			else if(nowchar == '=')
			{
				nowchar = '+';
			}
			else if(nowchar == '[')
			{
				nowchar = '{';
			}
			else if(nowchar == ']')
			{
				nowchar = '}';
			}
			else if(nowchar == ';')
			{
				nowchar = ':';
			}
			else if(nowchar == '\'')
			{
				nowchar = '\"';
			}
			else if(nowchar == '`')
			{
				nowchar = '~';
			}
			else if(nowchar == '\\')
			{
				nowchar = '|';
			}
			else if(nowchar == ',')
			{
				nowchar = '<';
			}
			else if(nowchar == '.')
			{
				nowchar = '>';
			}
			else if(nowchar == '/')
			{
				nowchar = '?';
			}
		}
	}
	return nowchar;
}