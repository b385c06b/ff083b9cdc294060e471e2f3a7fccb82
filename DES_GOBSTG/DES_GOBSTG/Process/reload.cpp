#include "../Header/processPrep.h"

bool Process::reload()
{
	SetCurrentDirectory(hge->Resource_MakePath(""));

	frameskip = M_DEFAULT_FRAMESKIP;

	strcpy(rpyfilename, "");
	Replay::rpy.replayIndex = 0;

	Selector::Clear();
	Player::p.exist = false;

	BGLayer::Init();

	FrontDisplay::fdisp.SetState(FDISP_PANEL, FDISPSTATE_OFF);
	Enemy::ClearAll();
	Ghost::ClearAll();
	EffectSp::ClearAll();
	Target::ClearAll();
	InfoSelect::Clear();
	Chat::chatitem.ClearAll();
	BossInfo::bossinfo.Clear();
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

	Bullet::Init();
	Enemy::Init();

	Ghost::index = 0;
	
	Item::Init();
	Beam::Init();
	PlayerBullet::Init();

	BossInfo::bossinfo.Init();
	InfoQuad::tex = tex[TEX_WHITE];

	FrontDisplay::fdisp.Init();
	Fontsys::fontsys.Init(FrontDisplay::fdisp.info.normalfont);
//	Fontsys::fontsys.font = FrontDisplay::fdisp.info.normalfont;
	//Heatup
	Fontsys::fontsys.HeatUp();

#ifdef __DEBUG
	HGELOG("\nCleared up.\n");
#endif

	SetCurrentDirectory(hge->Resource_MakePath(""));

	return true;
}