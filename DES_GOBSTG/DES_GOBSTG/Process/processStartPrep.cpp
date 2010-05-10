#include "../Header/processPrep.h"

void Process::clearPrep(bool bclearkey)
{
	ClearAll();

	BGLayer::ubg[UBGID_BGMASK].valueSetByName(SI_WHITE, M_CLIENT_CENTER_X, M_CLIENT_CENTER_Y, M_CLIENT_WIDTH, M_CLIENT_HEIGHT, 0);
	BGLayer::ubg[UFGID_FGPAUSE].valueSetByName(SI_WHITE, M_CLIENT_CENTER_X, M_CLIENT_CENTER_Y, M_CLIENT_WIDTH, M_CLIENT_HEIGHT, 0);

	FrontDisplay::fdisp.SetState(FDISP_PANEL, FDISPSTATE_ON);

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
	Replay::rpy.replayIndex = 0;

	hge->Resource_SetCurrentDirectory("");

	if(replaymode)
	{
		Replay::rpy.Load(rpyfilename, true);
		BYTE part = 0;
		if(scene < S1200)
			part = Data::data.getStage(scene) - 1;
		/*
		else
			part = RPYPARTMAX - 1;
			*/
		seed = Replay::rpy.partinfo[part].seed;
		scene = Replay::rpy.partinfo[part].scene;
		SetChara(Replay::rpy.rpyinfo.usingchara[0], Replay::rpy.rpyinfo.usingchara[1], Replay::rpy.rpyinfo.usingchara[2]);
		SetMode(Replay::rpy.rpyinfo.modeflag);

		Replay::rpy.replayIndex = Replay::rpy.partinfo[part].offset - 1;
	}
	else
	{
		seed = hge->Timer_GetCurrentSystemTime();
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
	nowdifflv = Data::data.getDiffi(scene);
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
		ZeroMemory(&Replay::rpy.rpyinfo, sizeof(replayInfo));

		//partinfo
		BYTE part = 0;
		if(scene < S1200)
			part = scene / M_STAGENSCENE - 1;
		for(int i=0;i<RPYPARTMAX;i++)
		{
			if(i != part)
			{
				ZeroMemory(&Replay::rpy.partinfo[i], sizeof(partInfo));
			}
			else
			{
				Replay::rpy.partFill(part);
			}
		}
	}
	else
	{
		BYTE part = 0;
		if(scene < S1200)
			part = Data::data.getStage(scene) - 1;
		if (part)
		{
			Player::p.nScore = Replay::rpy.partinfo[part].nowscore;
			Player::p.nPoint = Player::p.nLastPoint = Replay::rpy.partinfo[part].nowpoint;
			Player::p.nFaith = Player::p.nLastFaith = Replay::rpy.partinfo[part].nowfaith;
			Player::p.nGraze = Player::p.nLastGraze = Replay::rpy.partinfo[part].nowgraze;
			Player::p.nPower = Replay::rpy.partinfo[part].nowpower;
			Player::p.nNext = Player::p.getnNext();
			Player::p.changePlayerID(Replay::rpy.partinfo[part].nowID);
		}
		Player::p.nLife = Replay::rpy.partinfo[part].nowplayer;
		if(Player::p.nHiScore < Player::p.nScore)
			Player::p.nHiScore = Player::p.nScore;
	}

	BGLayer::ubg[UBGID_BGMASK].valueSetByName(SI_NULL, M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, M_ACTIVECLIENT_WIDTH, M_ACTIVECLIENT_HEIGHT, 0);
	BGLayer::ubg[UFGID_FGPAUSE].valueSetByName(SI_NULL, M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, M_ACTIVECLIENT_WIDTH, M_ACTIVECLIENT_HEIGHT, 0);
	BGLayer::ubg[UFGID_FGPAUSE].exist = false;

	framecounter = 0;
	gametime = 0;
	alltime = 0;

	if (callinit)
	{
		Scripter::scr.stageExecute(SCRIPT_CON_POST, scene);
	}
}