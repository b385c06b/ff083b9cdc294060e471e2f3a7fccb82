#include "processPrep.h"

void Process::clearPrep(bool bclearkey)
{
	Selector::Clear();
	for(int i=0; i<EFFECTSYSMAX; i++)
	{
		effsys[i].exist = false;
	}
	for(int i=0; i<BGLAYERSETMAX; i++)
	{
		BGLayer::set[i].sID = 0;
	}
	BGLayer::setindex = 0;
	for(int i=0;i<BGLAYERMAX;i++)
	{
		bg[i].exist = false;
		bg[i].changetimer = 0;
		bg[i].timer = 0;
		bg[i].flag = 0;
	}
	for(int i=0;i<FGLAYERMAX;i++)
	{
		fg[i].exist = false;
		fg[i].changetimer = 0;
		fg[i].timer = 0;
		fg[i].flag = 0;
	}
	for(int i=0;i<ENEMYMAX;i++)
	{
		en[i].exist = false;
		en[i].able = false;
		en[i].timer = 0;
	}
	Enemy::index = ENEMY_INDEXSTART;
	for(int i=0;i<GHOSTMAX;i++)
	{
		gh[i].exist = false;
		gh[i].able = false;
		gh[i].timer = 0;
	}
	Ghost::index = 0;
	for(int i=0;i<EFFECTSPMAX;i++)
	{
		es[i].exist = false;
		es[i].timer = 0;
	}
	for(int i=0;i<TARGETMAX;i++)
	{
		tar[i].x = 0;
		tar[i].y = 0;
	}
	bu.clear_item();
	Bullet::index = 0;
	Bullet::izel.clear_item();
	ZeroMemory(Bullet::_bu.actionList, sizeof(int) * BULLETACTIONMAX);
//	Bullet::_bu.actionList[0] = 0;
	mi.clear_item();
	pb.clear_item();
	be.clear_item();
	Item::infofont.clear_item();
	InfoSelect::Clear();
	Chat::chatting = false;
	Chat::timer = 0;
	Chat::chati = 0;
	Chat::chatinit = false;
	BossInfo::empty();
	pauseinit = false;

	worldx = 0;
	worldy = 0;
	worldz = 0;

	frameskip = M_DEFAULT_FRAMESKIP;

	Scripter::stopEdefScript = false;

	bgmask.changetimer = 0;
	bgmask.timer = 0;
	bgmask.flag = 0;
	bgmask.exist = false;
	fdisp.SetState(FDISP_PANEL, 1);
	/*
	fgpanel.changetimer = 0;
	fgpanel.timer = 0;
	fgpanel.flag = 0;
	*/
	fgpause.changetimer = 0;
	fgpause.timer = 0;
	fgpause.flag = 0;
	fgpause.exist = false;

	Player::p.ClearSet();

	if(!bclearkey)
		return;

	hge->Input_SetDIKey(KS_UP, false);
	hge->Input_SetDIKey(KS_DOWN, false);
	hge->Input_SetDIKey(KS_LEFT, false);
	hge->Input_SetDIKey(KS_RIGHT, false);
	hge->Input_SetDIKey(KS_FIRE, false);
	hge->Input_SetDIKey(KS_SPECIAL, false);
	hge->Input_SetDIKey(KS_CHANGE, false);
	hge->Input_SetDIKey(KS_SLOW, false);
}

void Process::startPrep(bool callinit)
{
	replayend = false;
	replayIndex = 0;

	SetCurrentDirectory(hge->Resource_MakePath(""));

	if(replaymode)
	{
		rpy.Load(rpyfilename, true);
		BYTE part = 0;
		if(scene < S1200)
			part = data.getStage(scene) - 1;
		/*
		else
			part = RPYPARTMAX - 1;
			*/
		seed = rpy.partinfo[part].seed;
		scene = rpy.partinfo[part].scene;
		mainchara = rpy.rpyinfo.usingchara[0];
		subchara_1 = rpy.rpyinfo.usingchara[1];
		subchara_2 = rpy.rpyinfo.usingchara[2];
		spellmode = rpy.rpyinfo.modeflag & M_RPYMODE_SPELL;
		practicemode = (bool)(rpy.rpyinfo.modeflag & M_RPYMODE_PRACTICE);

		replayIndex = rpy.partinfo[part].offset - 1;
	}
	else
	{
		seed = timeGetTime();
	}
	clearPrep();

	srandt(seed);

	Player::ncCont = 0;
	Player::ncGet = 0;
	Player::ncBorder = 0;
	Player::ncMiss = 0;
	Player::ncPause = 0;

	//set

	startscene = scene;
	nowdifflv = data.getDiffi(scene);
	if(!practicemode)
	{
		Player::p.valueSet(mainchara, subchara_1, subchara_2);
	}
	else if(!spellmode)
	{
		Player::p.valueSet(mainchara, subchara_1, subchara_2, PL_NPLAYERMAX);
	}
	else
	{
		Player::p.valueSet(mainchara, subchara_1, subchara_2, 0);
	}
	Player::p.nHiScore = DataConnector::nHiScore();
	DataConnector::Try(true);

	if(!replaymode)
	{
		ZeroMemory(&rpy.rpyinfo, sizeof(replayInfo));

		//partinfo
		BYTE part = 0;
		if(scene < S1200)
			part = scene / M_STAGENSCENE - 1;
		for(int i=0;i<RPYPARTMAX;i++)
		{
			if(i != part)
			{
				ZeroMemory(&rpy.partinfo[i], sizeof(partInfo));
			}
			else
			{
				rpy.partFill(part);
			}
		}
	}
	else
	{
		BYTE part = 0;
		if(scene < S1200)
			part = data.getStage(scene) - 1;
		if (part)
		{
			Player::p.nScore = rpy.partinfo[part].nowscore;
			Player::p.nPoint = Player::p.nLastPoint = rpy.partinfo[part].nowpoint;
			Player::p.nFaith = Player::p.nLastFaith = rpy.partinfo[part].nowfaith;
			Player::p.nGraze = Player::p.nLastGraze = rpy.partinfo[part].nowgraze;
			Player::p.nPower = rpy.partinfo[part].nowpower;
			Player::p.nNext = Player::p.getnNext();
			Player::p.changePlayerID(rpy.partinfo[part].nowID);
		}
		Player::p.nLife = rpy.partinfo[part].nowplayer;
		if(Player::p.nHiScore < Player::p.nScore)
			Player::p.nHiScore = Player::p.nScore;
	}

	bgmask.valueSetByName(tex, SI_NULL, M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, M_ACTIVECLIENT_WIDTH, M_ACTIVECLIENT_HEIGHT, 0);
	fgpause.valueSetByName(tex, SI_NULL, M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, M_ACTIVECLIENT_WIDTH, M_ACTIVECLIENT_HEIGHT, 0);
	fgpause.exist = false;

	stopflag = 0;
	stoptimer = 0;

	framecounter = 0;
	time = 0;
	alltime = 0;

	if (callinit)
	{
		scr.stageExecute(SCRIPT_CON_POST, scene);
	}
}