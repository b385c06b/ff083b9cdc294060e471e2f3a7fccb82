#include "processPrep.h"

int Process::processPreInitial()
{
	bool rebuilddone = false;
	if(_access(CONFIG_STR_FILENAME, 00) == -1)
	{
rebuild:
		if (rebuilddone)
		{
			errorcode = PROC_ERROR_INIFILE;
			return PQUIT;
		}
		DeleteFile(CONFIG_STR_FILENAME);

		hge->	Ini_SetInt(data.translateSection(data.sLinkType(DATAS_HEADER)), data.translateName(data.nLinkType(DATAN_GAMEVERSION)), GAME_VERSION);
		hge->	Ini_SetString(data.translateSection(data.sLinkType(DATAS_HEADER)), data.translateName(data.nLinkType(DATAN_SIGNATURE)), GAME_SIGNATURE);

		hge->	Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESOURCEFILE, RESCONFIGDEFAULT_RESOURCEFILE);
		hge->	Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESBINNAME, RESCONFIGDEFAULT_RESBINNAME);
		hge->	Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_PASSWORD, RESCONFIGDEFAULT_PASSWORD);

		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSPECIAL, RESCONFIGDEFAULT_KEYSPECIAL);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCHANGE, RESCONFIGDEFAULT_KEYCHANGE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);

		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSPECIAL, RESCONFIGDEFAULT_JOYSPECIAL);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCHANGE, RESCONFIGDEFAULT_JOYCHANGE);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);

		hge->	Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, RESCONFIGDEFAULT_VOLMUSIC);
		hge->	Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, RESCONFIGDEFAULT_VOLSE);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_DEFAULTLEVEL, RESCONFIGDEFAULT_DEFAULTLV);
		hge->	Ini_SetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, RESCONFIGDEFAULT_USERNAME);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, RESCONFIGDEFAULT_DEFAULTWINDOW);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, RESCONFIGDEFAULT_WINDOWLEFT);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, RESCONFIGDEFAULT_WINDOWTOP);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, RESCONFIGDEFAULT_WINDOWWIDTH);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, RESCONFIGDEFAULT_WINDOWHEIGHT);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, RESCONFIGDEFAULT_WINDOWTOPMOST);
#ifdef __DEBUG
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_DEBUG_JOYSPEEDUP, RESCONFIGDEFAULT_DEBUG_JOYSPEEDUP);
		HGELOG("Succeeded in rebuilding Config File.");

		rebuilddone = true;
#endif
	}

	keyUp			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP);
	keyDown			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN);
	keyLeft			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT);
	keyRight		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT);
	keyFire			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE);
	keySpecial		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSPECIAL, RESCONFIGDEFAULT_KEYSPECIAL);
	keySlow			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW);
	keyChange		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCHANGE, RESCONFIGDEFAULT_KEYCHANGE);
	keyPause		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
	keySkip			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
	keyEnter		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
	keyEscape		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
	keyCapture		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);

	joyFire			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
	joySpecial		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSPECIAL, RESCONFIGDEFAULT_JOYSPECIAL);
	joySlow			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
	joyChange		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCHANGE, RESCONFIGDEFAULT_JOYCHANGE);
	joyPause		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);
#ifdef __DEBUG
	debug_joySpeedUp = hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_DEBUG_JOYSPEEDUP, RESCONFIGDEFAULT_DEBUG_JOYSPEEDUP);
#endif

	bgmvol			= hge->Ini_GetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, RESCONFIGDEFAULT_VOLMUSIC);
	sevol			= hge->Ini_GetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, RESCONFIGDEFAULT_VOLSE);

	screenmode		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
	defaultdifflv	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_DEFAULTLEVEL, RESCONFIGDEFAULT_DEFAULTLV);
	strcpy(username, hge->Ini_GetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, RESCONFIGDEFAULT_USERNAME));
	renderskip		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);

	//

	if(hge->Ini_GetInt(data.translateSection(data.sLinkType(DATAS_HEADER)), data.translateName(data.nLinkType(DATAN_GAMEVERSION)), -1) != GAME_VERSION)
		goto rebuild;
	for (int i=0; i<13; i++)
	{
		if (keyKey[i] < 0 || keyKey[i] >= M_KEYKEYMAX)
		{
			goto rebuild;
		}
	}
	for (int i=0; i<5; i++)
	{
		if(joyKey[i] < 0 || joyKey[i] >= M_JOYKEYMAX)
		{
			goto rebuild;
		}
	}
#ifdef __DEBUG
	if(debug_joySpeedUp < 0 || debug_joySpeedUp >= M_JOYKEYMAX)
		goto rebuild;
#endif
	if(screenmode < 0 || screenmode > 1)
		goto rebuild;
	if(defaultdifflv < 0 || defaultdifflv > M_DIFFI_EXTRA_START)
		goto rebuild;
	if(bgmvol < 0 || bgmvol > 100)
		goto rebuild;
	if(sevol < 0 || sevol > 100)
		goto rebuild;
	if (renderskip < 0 || renderskip > 3)
	{
		goto rebuild;
	}
	else
	{
		if (renderskip == 0)
		{
			renderskip = 1;
		}
		hge->System_SetState(HGE_RENDERSKIP, renderskip * M_DEFAULT_RENDERSKIP);
	}

#ifdef __DEBUG
	HGELOG("Succeeded in gaining data from Config File.");
#endif
	hge->Resource_AttachPack(RESLOADING_PCK, Export::GetPassword());
	texInit = hge->Texture_Load(RESLOADING_TEX);

	return PGO;
}

int Process::processInit()
{
	time++;

	if (time == 1)
	{
		return processPreInitial();
	}

	if (texInit)
	{
		hge->Texture_Free(texInit);
		texInit = NULL;
	}
	hge->Resource_RemovePack(RESLOADING_PCK);

	bool binmode = Export::GetResourceFile();

	data.GetIni();

	if(binmode)
	{
		data.binmode = true;
		scr.binmode = true;
#ifdef __RELEASE
//		hge->System_SetState(HGE_LOGFILE, "");
#endif // __RELEASE
	}
	else
	{
		if(!res.Fill())
		{
#ifdef __DEBUG
			HGELOG("Error in Filling Resource Data.");
#endif
			errorcode = PROC_ERROR_RESOURCE;
			return PQUIT;
		}
		if(!scr.LoadAll())
		{
			errorcode = PROC_ERROR_SCRIPT;
			return PQUIT;
		}
		if(!res.Pack(strdesc, res.customconstdata))
		{
#ifdef __DEBUG
			HGELOG("Error in Packing Resource Data.");
#endif
			errorcode = PROC_ERROR_TRANSLATE;
			return PQUIT;
		}
		if(!res.SetDataFile())
		{
			errorcode = PROC_ERROR_DATA;
			return PQUIT;
		}
	}
	if(!res.Gain(strdesc, binmode?res.customconstdata:NULL))
	{
#ifdef __DEBUG
		HGELOG("Error in Gaining Resource Data.");
#endif
		errorcode = PROC_ERROR_DATA;
		return PQUIT;
	}
	if(scr.binmode && !scr.LoadAll())
	{
		errorcode = PROC_ERROR_SCRIPT;
		return PQUIT;
	}
	if(!data.SetFile(Export::resourcefilename, DATA_RESOURCEFILE))
	{
#ifdef __DEBUG
		HGELOG("Error in Setting Resource File");
#endif
		errorcode = PROC_ERROR_DATA;
		return PQUIT;
	}

	if(!res.LoadPackage())
	{
		errorcode = PROC_ERROR_PACKAGE;
		return PQUIT;
	}

	BGLayer::Init();

	SE::vol = sevol;
	if(!SE::Initial())
	{
		errorcode = PROC_ERROR_SOUND;
		return PQUIT;
	}

	char tnbuffer[M_STRMAX];
	for(int i=1;i<TEXMAX;i++)
	{
		if(tex[i])
			hge->Texture_Free(tex[i]);
		tex[i] = NULL;

		strcpy(tnbuffer, res.resdata.texfilename[i]);
		if(!strlen(tnbuffer))
		{
			strcpy(tnbuffer, res.resdata.texfilename[TEX_WHITE]);
		}

		tex[i] = hge->Texture_Load(tnbuffer);
#ifdef __DEBUG
		if(tex[i] == NULL)
		{
			HGELOG("%s\nFailed in loading Texture File %s.(To be assigned to Index %d).", HGELOG_ERRSTR, tnbuffer, i);
			errorcode = PROC_ERROR_TEXTURE;
			return PQUIT;
		}
		else
		{
			HGELOG("Succeeded in loading Texture File %s.(Assigned to Index %d).", tnbuffer, i);
		}
#endif
	}

	tex[TEX_WORD] = hge->Texture_Load(res.resdata.texfilename[TEX_WORD]);
#ifdef __DEBUG
	if(tex[TEX_WORD] == NULL)
	{
		HGELOG("%s\nFailed in loading Texture File %s.(To be assigned to Index %d).", HGELOG_ERRSTR, res.resdata.texfilename[TEX_WORD], TEX_WORD);
		errorcode = PROC_ERROR_TEXTURE;
		return PQUIT;
	}
	else
	{
		HGELOG("Succeeded in loading Texture File %s.(Assigned to Index %d).", res.resdata.texfilename[TEX_WORD], TEX_WORD);
	}
#endif

	SpriteItemManager::Init(tex);

	Fontsys::Init();
	if(!Effectsys::Init(tex, res.resdata.effectsysfoldername, res.resdata.effectsysfilename))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in Initializing Effectsys.", HGELOG_ERRSTR);
#endif
		return false;
	}

#ifdef __DEBUG
	HGELOG("Succeeded in loading all resources.\n");
#endif

	hge->System_SetState(HGE_WINDOWED, !(bool)screenmode);
	hge->System_SetState(HGE_HIDEMOUSE, (bool)screenmode);

	if(!screenmode)
	{
		Export::clientAdjustWindow();
	}

	chat.Init();

	Selector::Clear();
	InfoSelect::Clear();

	mainchara	= 0;
	subchara_1	= 0;
	subchara_2	= 0;
	nowdifflv	= defaultdifflv;
	randi		= 0;
	errorcode = PROC_ERROR_NONE;
	titleselect = 0;

	replaymode = false;
	practicemode = false;
	spellmode = false;

	playing = false;
	playtimeStart = 0;

	time = 0;
	state = STATE_TITLE;
	return PTURN;
}