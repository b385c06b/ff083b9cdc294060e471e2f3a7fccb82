#include "processPrep.h"

bool Process::reload()
{
	SetCurrentDirectory(hge->Resource_MakePath(""));

	frameskip = M_DEFAULT_FRAMESKIP;

	strcpy(rpyfilename, "");
	replayIndex = 0;

	Selector::Clear();
	Player::p.exist = false;
	for(int i=0; i<BGLAYERSETMAX; i++)
	{
		BGLayer::set[i].sID = 0;
	}
	BGLayer::KillOtherLayer();
	bgmask.exist = false;
	fgpause.exist = false;
	fdisp.SetState(FDISP_PANEL, 0);
	for(int i=0;i<ENEMYMAX;i++)
	{
		en[i].exist = false;
		en[i].able = false;
	}
	for(int i=0;i<GHOSTMAX;i++)
	{
		gh[i].exist = false;
		gh[i].able = false;
	}	
	for(int i=0;i<EFFECTSPMAX;i++)
	{
		es[i].exist = false;
	}
	for(int i=0;i<TARGETMAX;i++)
	{
		tar[i].x = 0;
		tar[i].y = 0;
	}
	InfoSelect::Clear();
	Chat::chatting = false;
	Chat::timer = 0;
	Chat::chati = 0;
	Chat::chatinit = false;
	BossInfo::empty();
	Player::ncCont = 0;
	Player::ncGet = 0;
	Player::ncBorder = 0;
	Player::ncMiss = 0;
	Player::ncPause = 0;
	pauseinit = false;

	practicemode = false;
	spellmode = false;
	replaymode = false;
	replayFPS = 0;

	scene = S100;

	worldx = 0;
	worldy = 0;
	worldz = 0;
	worldshaketimer = 0;

	Bullet::Init(tex[TEX_BULLET]);
	Enemy::Init(tex[TEX_ENEMY]);

	Ghost::index = 0;
	
	Item::Init();
	Beam::Init();
	PlayerBullet::Init(tex);

	BossInfo::Init();
	InfoQuad::tex = tex[TEX_WHITE];

	fdisp.Init();
	Fontsys::font = fdisp.info.normalfont;
	//Heatup
	Fontsys::HeatUp();

#ifdef __DEBUG
	HGELOG("\nCleared up.\n");
#endif

	SetCurrentDirectory(hge->Resource_MakePath(""));

	return true;
}