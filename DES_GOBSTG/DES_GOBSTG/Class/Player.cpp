#include "Player.h"

#include "Process.h"
#include "BGLayer.h"
#include "SE.h"

#include "PlayerBullet.h"
#include "Item.h"
#include "Ghost.h"
#include "Bullet.h"
#include "Chat.h"
#include "BossInfo.h"
#include "InfoQuad.h"
#include "BResource.h"

Player Player::p;

int Player::ncBorder = 0;
BYTE Player::ncCont = 0;
BYTE Player::ncGet = 0;
BYTE Player::ncMiss = 0;
BYTE Player::ncPause = 0;

#define _PL_ITEMDRAINNPOP	(PL_NPOPMAX * 4 / 5)

#define _PL_HITONSCOREADD	40
#define _PL_POWERADD		1000
#define _PL_BIGPOWERADD		10000
#define _PL_BOMBPOWERADD	100000
#define _PL_FULLPOWERADD	30000

#define _PL_FAITHADD		1000
#define _PL_SMALLFAITHADD	10

#define _PL_NPOPGRAZEADD	100

#define _PL_NPOPCOST		4

#define _PL_EXTENDNPOP		(PL_NPOPMAX + _PL_NPOPCOST*96)

#define _PL_BORDERIZEZONE_TIME	8
#define _PL_BORDERGRAZEMAX		300

#define _PL_BOMBFAITHCOSTRATE	0.75f

#define _PLEXTEND_1		1000
#define _PLEXTEND_2		2500
#define _PLEXTEND_3		5000
#define _PLEXTEND_4		8000
#define _PLEXTEND_5		11000
#define _PLEXTEND_MAX	99999

Player::Player()
{
	effGraze.exist = false;
	effChange.exist = false;
	effInfi.exist = false;
	effCollapse.exist = false;
	effMerge.exist = false;
	effBorder.exist = false;
	effBorderOn.exist = false;
	effBorderOff.exist = false;
	sprite			= NULL;
	nowID			= 0;
	ID_sub_1		= 0;
	ID_sub_2		= 0;
}

Player::~Player()
{
	if(sprite)
		delete sprite;
	sprite = NULL;
}

void Player::initFrameIndex()
{
	WORD _ID;
	for (int i=0; i<M_PL_ONESETPLAYER; i++)
	{
		if (i == 1)
		{
			_ID = ID_sub_1;
		}
		else if (i == 2)
		{
			_ID = ID_sub_2;
		}
		else
		{
			_ID = ID;
		}
		playerData * pdata = &(res.playerdata[_ID]);
		int tfi = pdata->startFrame;
		frameindex[i][PLAYER_FRAME_STAND] = tfi;

		bool bhr = pdata->rightPreFrame;
		bool bhl = pdata->leftPreFrame;

		tfi += pdata->standFrame;
		frameindex[i][PLAYER_FRAME_LEFTPRE] = tfi;
		if (bhr)
		{
			tfi += pdata->rightPreFrame;
		}
		else
		{
			tfi += pdata->leftPreFrame;
		}
		frameindex[i][PLAYER_FRAME_LEFT] = tfi;

		if (bhr)
		{
			tfi += pdata->rightFrame;
		}
		else
		{
			tfi += pdata->leftFrame;
		}
		if (!bhr || !bhl)
		{
			tfi -= pdata->leftPreFrame + pdata->rightPreFrame + pdata->leftFrame + pdata->rightFrame;
		}
		frameindex[i][PLAYER_FRAME_RIGHTPRE] = tfi;
		if (bhr)
		{
			tfi += pdata->rightPreFrame;
		}
		else
		{
			tfi += pdata->leftPreFrame;
		}
		frameindex[i][PLAYER_FRAME_RIGHT] = tfi;
		if (bhr)
		{
			tfi += pdata->rightFrame;
		}
		else
		{
			tfi += pdata->leftFrame;
		}
	}
}

BYTE Player::getFrameIndex(BYTE frameenum)
{
	flipx = false;
	playerData * pdata = &(res.playerdata[nowID]);
	if ((frameenum == PLAYER_FRAME_RIGHTPRE || frameenum == PLAYER_FRAME_RIGHT) && (!pdata->rightPreFrame) ||
		(frameenum == PLAYER_FRAME_LEFTPRE || frameenum == PLAYER_FRAME_LEFT) && (!pdata->leftPreFrame))
	{
		flipx = true;
	}
	int tindex = 0;
	if (nowID == ID_sub_1)
	{
		tindex = 1;
	}
	else if (nowID == ID_sub_2)
	{
		tindex = 2;
	}
	return frameindex[tindex][frameenum];
}

void Player::setFrame(BYTE frameenum)
{
	frameoffset = 0;
	setIndexFrame(getFrameIndex(frameenum));
	nowstate = frameenum;
}

void Player::setIndexFrame(BYTE index)
{
	playerData * pdata = &(res.playerdata[nowID]);
	HTEXTURE nowtex = mp.tex[res.playerdata[nowID].tex];
	sprite->SetTexture(nowtex);
	float tw = pdata->usetexw / (pdata->tex_nCol);
	float th = pdata->usetexh / (pdata->tex_nRow);
	float ltx = tw * (index % (pdata->tex_nCol));
	float lty = th * (index / (pdata->tex_nCol));
	sprite->SetTextureRect(ltx, lty, tw, th);
	sprite->SetFlip(flipx, false);
}


void Player::updateFrame(BYTE frameenum, int usetimer /* = -1*/)
{
	if (usetimer == -1)
	{
		usetimer = timer;
	}
	if (frameenum != nowstate && (frameenum == PLAYER_FRAME_STAND || frameenum+1 != nowstate))
	{
		setFrame(frameenum);
		return;
	}
	if ((usetimer % PLAYER_ANIMATIONSPEED))
	{
		return;
	}
	playerData * pdata = &(res.playerdata[nowID]);
	frameoffset++;
	BYTE tbyte;
	switch (nowstate)
	{
	case PLAYER_FRAME_STAND:
		if (frameoffset >= pdata->standFrame)
		{
			setFrame(PLAYER_FRAME_STAND);
		}
		else
		{
			setIndexFrame(getFrameIndex(PLAYER_FRAME_STAND) + frameoffset);
		}
		break;
	case PLAYER_FRAME_LEFTPRE:
		if (!pdata->leftPreFrame)
		{
			tbyte = pdata->rightPreFrame;
		}
		else
		{
			tbyte = pdata->leftPreFrame;
		}
		if (frameoffset >= tbyte)
		{
			setFrame(PLAYER_FRAME_LEFT);
		}
		else
		{
			setIndexFrame(getFrameIndex(PLAYER_FRAME_LEFTPRE) + frameoffset);
		}
		break;
	case PLAYER_FRAME_LEFT:
		if (!pdata->leftFrame)
		{
			tbyte = pdata->rightFrame;
		}
		else
		{
			tbyte = pdata->leftFrame;
		}
		if (frameoffset >= tbyte)
		{
			setFrame(PLAYER_FRAME_LEFT);
		}
		else
		{
			setIndexFrame(getFrameIndex(PLAYER_FRAME_LEFT) + frameoffset);
		}
		break;
	case PLAYER_FRAME_RIGHTPRE:
		if (!pdata->rightPreFrame)
		{
			tbyte = pdata->leftPreFrame;
		}
		else
		{
			tbyte = pdata->rightPreFrame;
		}
		if (frameoffset >= tbyte)
		{
			setFrame(PLAYER_FRAME_RIGHT);
		}
		else
		{
			setIndexFrame(getFrameIndex(PLAYER_FRAME_RIGHTPRE) + frameoffset);
		}
		break;
	case PLAYER_FRAME_RIGHT:
		if (!pdata->rightFrame)
		{
			tbyte = pdata->leftFrame;
		}
		else
		{
			tbyte = pdata->rightFrame;
		}
		if (frameoffset >= tbyte)
		{
			setFrame(PLAYER_FRAME_RIGHT);
		}
		else
		{
			setIndexFrame(getFrameIndex(PLAYER_FRAME_RIGHT) + frameoffset);
		}
		break;
	}
}


LONGLONG Player::getClearBonusPoint()
{
	LONGLONG ret = 0;
	if (nPoint > nLastPoint)
	{
		ret = (nPoint - nLastPoint) * 5000;
	}
	nLastPoint = nPoint;
	return ret;
}

LONGLONG Player::getClearBonusGraze()
{
	LONGLONG ret = 0;
	if (nGraze > nLastGraze)
	{
		ret = (nGraze - nLastGraze) * 125;
	}
	nLastGraze = nGraze;
	return ret;
}

LONGLONG Player::getClearBonusFaith()
{
	LONGLONG ret = 0;
	
	if (nFaith > nLastFaith)
	{
		ret = (nFaith - nLastFaith) * 50;
	}
	nLastFaith = nFaith;
	
	return ret;
}

LONGLONG Player::getClearBonusStage(int nstage)
{
	return 2500000 * nstage;
}

LONGLONG Player::getAllClearBonusLife()
{
	return 5000000 * nLife;
}

LONGLONG Player::getAllClearBonusPower()
{
	return 3 * nPower;
}

bool Player::HavePlayer(WORD _ID)
{
	if (_ID == ID || _ID == ID_sub_1 || _ID == ID_sub_2)
	{
		return true;
	}
	return false;
}

void Player::ClearSet()
{
	x			=	PL_MERGEPOS_X;
	y			=	PL_MERGEPOS_Y;

	for(int i=0;i<PL_SAVELASTMAX;i++)
	{
		lastx[i] = x;
		lasty[i] = y;
		lastmx[i] = x;
		lastmy[i] = y;
	}
	timer		=	0;
	angle		=	0;
	flag		=	PLAYER_MERGE;
	bBomb		=	false;
	bBorder		=	false;
	bSlow		=	false;
	bInfi		=	true;
	borderShot	=	false;
	hscale		=	1.0f;
	vscale		=	1.0f;
	alpha		=	0xff;

	mergetimer			=	0;
	shottimer			=	0;
	collapsetimer		=	0;
	shoottimer			=	0;
	bordertimer			=	0;
	bombtimer			=	0;
	slowtimer			=	0;
	fasttimer			=	0;
	playerchangetimer	=	0;

	nPop			=	0;
	fPoprate		=	0.0f;
	bordergraze		=	0;

	bonusflag		=	0;

	speedfactor		=	1.0f;

	exist = true;

	if (effGraze.exist)
	{
		effGraze.Stop(true);
		effGraze.MoveTo(x, y, 0, true);
	}
	if (effChange.exist)
	{
		effChange.Stop(true);
		effChange.MoveTo(x, y, 0, true);
	}
	if (effInfi.exist)
	{
		effInfi.Stop(true);
		effInfi.MoveTo(x, y, 0, true);
	}
	if (effCollapse.exist)
	{
		effCollapse.Stop(true);
		effCollapse.MoveTo(x, y, 0, true);
	}
	if (effMerge.exist)
	{
		effMerge.Stop(true);
		effMerge.MoveTo(x, y, 0, true);
	}
	if (effBorder.exist)
	{
		effBorder.Stop(true);
		effBorder.MoveTo(x, y, 0, true);
	}
	if (effBorderOn.exist)
	{
		effBorderOn.Stop(true);
		effBorderOn.MoveTo(x, y, 0, true);
	}
	if (effBorderOff.exist)
	{
		effBorderOff.Stop(true);
		effBorderOff.MoveTo(x, y, 0, true);
	}

	changePlayerID(nowID, true);

	esChange.valueSet(EFFECT_PLAYERCHANGE, x, y, 0, 0);
	esShot.valueSet(EFFECT_PLAYERSHOT, x, y, 0, 0);
	esShot.colorSet(0xff0000);
	esBorder.valueSet(EFFECT_PLAYERBORDER, x, y, 240, 1.5f, true);
	esBorderZone.valueSet(EFFECT_PLAYERBORDERZONE, x, y, 240, -1.5f, true);
	esPoint.valueSet(EFFECT_PLAYERPOINT, x, y, 0, 0);
	esCollapse.valueSet(EFFECT_PLAYERCOLLAPSE, x, y, 160 ,0, false);
}

void Player::UpdatePlayerData()
{
	playerData * pdata = &(res.playerdata[nowID]);
	r = pdata->collision_r;
	speed = pdata->fastspeed;
	slowspeed = pdata->slowspeed;
	graze_r = pdata->graze_r;
	bombperpower = pdata->bombperpower;
	shotdelay = pdata->shotdelay;
	borderlast = pdata->borderlast;
	bomblast = pdata->bomblast;
}

void Player::valueSet(WORD _ID, WORD _ID_sub_1, WORD _ID_sub_2, BYTE _nLife, bool bContinue)
{
	ID			=	_ID;
	ID_sub_1	= _ID_sub_1;
	ID_sub_2	= _ID_sub_2;
	nowID		= ID;
	ClearSet();
	initFrameIndex();
	UpdatePlayerData();

	nLife		=	_nLife;

	nPower		=	PL_DEFAULTNPOWER;
	nGraze		=	0;
	nPoint		=	0;
	nFaith		=	PL_DEFAULTNFAITH;
	nScore		=	0;

	nNext		=	getnNext();

	for(int i=0;i<M_GETSPELLMAX;i++)
		getspell[i] = 0;

	nLastPoint		= 0;
	nLastGraze		= 0;
	nLastFaith	= 0;

	if (!bContinue)
	{
		lostStack		=	0;
		fastCounter		=	0;
		borderCounter	=	0;
	}

	if(!sprite)
		sprite = new hgeSprite(mp.tex[res.playerdata[ID].tex], 0, 0, 0, 0);
	setFrame(PLAYER_FRAME_STAND);

	effGraze.valueSet(EFF_PL_GRAZE, *this);
	effGraze.Stop();
	effChange.valueSet(EFF_PL_CHANGE, *this);
	effChange.Stop();
	effInfi.valueSet(EFF_PL_INFI, *this);
	effInfi.Stop();
	effCollapse.valueSet(EFF_PL_COLLAPSE, *this);
	effCollapse.Stop();
	effMerge.valueSet(EFF_PL_MERGE, *this);
	effMerge.Stop();
	effBorder.valueSet(EFF_PL_BORDER, *this);
	effBorder.Stop();
	effBorderOn.valueSet(EFF_PL_BORDERON, *this);
	effBorderOn.Stop();
	effBorderOff.valueSet(EFF_PL_BORDEROFF, *this);
	effBorderOff.Stop();
}

DWORD Player::getnNext()
{
	if (nPoint < _PLEXTEND_1)
	{
		return _PLEXTEND_1;
	}
	if (nPoint < _PLEXTEND_2)
	{
		return _PLEXTEND_2;
	}
	if (nPoint < _PLEXTEND_3)
	{
		return _PLEXTEND_3;
	}
	if (nPoint < _PLEXTEND_4)
	{
		return _PLEXTEND_4;
	}
	if (nPoint < _PLEXTEND_5)
	{
		return _PLEXTEND_5;
	}
	return _PLEXTEND_MAX;
}

void Player::action()
{
	float nowspeed = 0;
	timer++;

	alpha = 0xff;
	if(timer == 1)
		flag |= PLAYER_MERGE;

	if (nPoint >= nNext)
	{
		SE::push(SE_ITEM_EXTEND, x);
		nLife++;
		if(nLife > PL_NPLAYERMAX)
		{
			nLife = PL_NPLAYERMAX;
			nPower += _PL_BOMBPOWERADD;
		}
		nNext = getnNext();
	}

	//savelast
	if(lastmx[0] != x || lastmy[0] != y)
	{
		for(int i=PL_SAVELASTMAX-1;i>0;i--)
		{
			lastmx[i] = lastmx[i-1];
			lastmy[i] = lastmy[i-1];
		}
		lastmx[0] = x;
		lastmy[0] = y;
	}
	for(int i=PL_SAVELASTMAX-1;i>0;i--)
	{
		lastx[i] = lastx[i-1];
		lasty[i] = lasty[i-1];
	}
	lastx[0] = x;
	lasty[0] = y;

	if (nPop > PL_NPOPMAX)
	{
		fPoprate = 1.0f;
	}
	else
	{
		fPoprate = (float)nPop / PL_NPOPMAX;
	}
	fPopratebase = nGraze / 10000.0f;

	//flag
	if(flag & PLAYER_MERGE)
		if(Merge())
			flag &= ~PLAYER_MERGE;
	if(flag & PLAYER_SHOT)
	{
		bool shotdelaychange = bossinfo.isSpell() && (BossInfo::spellflag & BISF_SHOT);
		if (shotdelaychange)
		{
			shotdelay += PLSHOTDELAY_ADD;
		}
		if(Shot())
		{
			flag &= ~PLAYER_SHOT;
		}
		if (shotdelaychange)
		{
			shotdelay -= PLSHOTDELAY_ADD;
		}
	}
	if(flag & PLAYER_COLLAPSE)
		if(Collapse())
			flag &= ~PLAYER_COLLAPSE;
	if(flag & PLAYER_SLOWCHANGE)
		if(SlowChange())
			flag &= ~PLAYER_SLOWCHANGE;
	if(flag & PLAYER_FASTCHANGE)
		if(FastChange())
			flag &= ~PLAYER_FASTCHANGE;
	if(flag & PLAYER_PLAYERCHANGE)
		if(PlayerChange())
			flag &= ~PLAYER_PLAYERCHANGE;
	if(flag & PLAYER_SHOOT)
		if(Shoot())
			flag &= ~PLAYER_SHOOT;
	if(flag & PLAYER_BOMB)
		if(Bomb())
			flag &= ~PLAYER_BOMB;
	if(flag & PLAYER_BORDER)
		if(Border())
			flag &= ~PLAYER_BORDER;
	if(flag & PLAYER_GRAZE)
		if(Graze())
			flag &= ~PLAYER_GRAZE;


	if (bonusflag & PLBONUS_TIME)
	{
		nScore += 2000;
		bonusflag &= ~PLBONUS_TIME;
	}
	//input
	if(!(flag & PLAYER_SHOT || flag & PLAYER_COLLAPSE))
	{
		if (hge->Input_GetDIKey(KS_SLOW_MP))
		{
			bSlow = true;
			flag &= ~PLAYER_FASTCHANGE;
			if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_DOWN))
			{
				if (!(flag & PLAYER_SLOWCHANGE))
				{
					slowtimer = 0;
					flag |= PLAYER_SLOWCHANGE;
				}
			}
		}
		else
		{
			bSlow = false;
			flag &= ~PLAYER_SLOWCHANGE;
			if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_UP))
			{
				if (!(flag & PLAYER_FASTCHANGE))
				{
					fasttimer = 0;
					flag |= PLAYER_FASTCHANGE;
				}
			}
		}
		if(!chat.chatting && !bInfi)
		{
			nPop -= _PL_NPOPCOST;
			if (nPop > _PL_EXTENDNPOP)
			{
				nPop = _PL_EXTENDNPOP;
			}
			else if (nPop < 0)
			{
				nPop = 0;
			}
		}
		if(bSlow)
		{
			nowspeed = slowspeed;
		}
		else
		{
			nowspeed = speed;
		}
		nowspeed *= speedfactor;
		speedfactor = 1.0f;
		if(!(flag & PLAYER_BORDER || flag & PLAYER_BOMB))
		{
			if(hge->Input_GetDIKey(KS_CHANGE_MP, DIKEY_DOWN))
			{
				if (!(flag & PLAYER_PLAYERCHANGE))
				{
					playerchangetimer = 0;
					flag |= PLAYER_PLAYERCHANGE;
				}
			}
		}
		if((hge->Input_GetDIKey(KS_UP_MP) ^ hge->Input_GetDIKey(KS_DOWN_MP)) &&
			hge->Input_GetDIKey(KS_LEFT_MP) ^ hge->Input_GetDIKey(KS_RIGHT_MP))
			nowspeed *= M_SQUARE_2;
		if(hge->Input_GetDIKey(KS_UP_MP))
			y -= nowspeed;
		if(hge->Input_GetDIKey(KS_DOWN_MP))
			y += nowspeed;
		if(hge->Input_GetDIKey(KS_LEFT_MP))
		{
			updateFrame(PLAYER_FRAME_LEFTPRE);
			x -= nowspeed;
		}
		if(hge->Input_GetDIKey(KS_RIGHT_MP))
		{
			if (!hge->Input_GetDIKey(KS_LEFT_MP))
			{
				updateFrame(PLAYER_FRAME_RIGHTPRE);
			}
			else
			{
				updateFrame(PLAYER_FRAME_STAND);
			}
			x += nowspeed;
		}
		if (!hge->Input_GetDIKey(KS_LEFT_MP) && !hge->Input_GetDIKey(KS_RIGHT_MP))
		{
			updateFrame(PLAYER_FRAME_STAND);
		}
		if(hge->Input_GetDIKey(KS_FIRE_MP) && !Chat::chatting)
			flag |= PLAYER_SHOOT;
	}
	if(hge->Input_GetDIKey(KS_SPECIAL_MP) && !(flag & PLAYER_MERGE))
	{
		callBomb(mp.spellmode);
	}

	if (!(flag & PLAYER_MERGE) || mergetimer >= 32)
	{
		if(x > PL_MOVABLE_RIGHT)
			x = PL_MOVABLE_RIGHT;
		else if(x < PL_MOVABLE_LEFT)
			x = PL_MOVABLE_LEFT;
		if(y > PL_MOVABLE_BOTTOM)
			y = PL_MOVABLE_BOTTOM;
		else if(y < PL_MOVABLE_TOP)
			y = PL_MOVABLE_TOP;
	}

	if (y < PL_ITEMDRAINY)
	{
		if (nPop < _PL_ITEMDRAINNPOP)
		{
			nPop = _PL_ITEMDRAINNPOP;
		}
	}

	if(nScore > nHiScore)
		nHiScore = nScore;

	if (bInfi && timer % 16 < 8)
	{
		diffuse = 0xff99ff;
	}
	else
		diffuse = 0xffffff;

	esChange.action();
	esBorder.action();
	esBorderZone.action();
	esShot.action();
	esPoint.action();
	
	effGraze.MoveTo(x, y);
	effGraze.action();
	effCollapse.action();
	effBorderOn.action();
	effBorderOff.action();

	if(!(flag & PLAYER_GRAZE))
		effGraze.Stop();

	for(int i=0;i<PLAYERGHOSTMAX;i++)
		pg[i].action();

	if(!bSlow)
		fastCounter++;
	if(bBorder)
	{
		borderCounter++;
	}

	bBomb = flag & PLAYER_BOMB;
	bBorder = flag & PLAYER_BORDER;
}

LONGLONG Player::getItemBonus(WORD itemtype)
{
	LONGLONG retscore = 0;
	switch (itemtype)
	{
	case ITEM_POINT:
		nPoint++;
		retscore = nFaith * (fPoprate + fPopratebase);
		if (fPoprate >= 1.0f)
		{
			retscore *= 2;
		}
		break;
	case ITEM_POWER:
		nPower+=_PL_POWERADD;
		retscore = 100;
		break;
	case ITEM_BIGPOWER:
		nPower += _PL_BIGPOWERADD;
		retscore = 500;
		SE::push(SE_ITEM_POWERUP, x);
		break;
	case ITEM_FULL:
		nPower += _PL_FULLPOWERADD;
		Item::ChangeItemID(ITEM_FULL, ITEM_SMALLFAITH);
		SE::push(SE_ITEM_POWERUP, x);
		retscore = 1000;
		break;
	case ITEM_BOMB:
		nPower += _PL_BOMBPOWERADD;
		SE::push(SE_ITEM_EXTEND, x);
		retscore = 1000;
		break;
	case ITEM_EXTEND:
		SE::push(SE_ITEM_EXTEND, x);
		nLife++;
		if(nLife > PL_NPLAYERMAX)
		{
			nLife = PL_NPLAYERMAX;
			nPower += _PL_BOMBPOWERADD;
		}
		retscore = 1000;
		break;
	case ITEM_FAITH:
		nFaith += _PL_FAITHADD;
		retscore = nPower;
		break;
	case ITEM_SMALLFAITH:
		nFaith += _PL_SMALLFAITHADD;
		retscore = nPower / 100;
		break;
	}
	return retscore;
}

void Player::DoGraze(float x, float y)
{
	if(!(flag & (PLAYER_MERGE | PLAYER_SHOT | PLAYER_COLLAPSE)))
	{
		int grazechange = bSlow ? 1 : 2;
		nGraze += grazechange;
		nScore += 200 * grazechange;

		if (flag & PLAYER_BORDER)
		{
			Item::Build(ITEM_SMALLFAITH, x, y, true);
			bordergraze++;
			if (bordergraze > _PL_BORDERGRAZEMAX)
			{
				bordergraze = _PL_BORDERGRAZEMAX;
			}
		}
		nPop += _PL_NPOPGRAZEADD;
		if (nPop > _PL_EXTENDNPOP)
		{
			nPop = _PL_EXTENDNPOP;
		}

		if(bossinfo.isSpell() && !bossinfo.failed && bossinfo.limit)
		{
			bossinfo.bonus += bossinfo.maxbonus/(bossinfo.limit*60);
		}

		flag |= PLAYER_GRAZE;

		if (bonusflag & PLBONUS_GRAZE)
		{
			nScore += 2000;
			bonusflag &= ~PLBONUS_GRAZE;
		}
	}
}

void Player::AddPower(int power)
{
	if (power >= 0 || nPower >= -power)
	{
		nPower += power;
	}
}

void Player::DoPlayerBulletHit(int hitonfactor)
{
	if (!bBorder && !bBomb && (!bossinfo.flag || bossinfo.flag==BOSSINFO_ENABLE))
	{
		AddPower(hitonfactor);
	}
	if (bonusflag & PLBONUS_SHOOT)
	{
		if (hitonfactor > 0)
		{
			nScore += hitonfactor * _PL_HITONSCOREADD;
		}
		bonusflag &= ~PLBONUS_SHOOT;
	}
}

void Player::DoShot()
{
	if (!Player::p.bInfi && !(Player::p.flag & (PLAYER_SHOT | PLAYER_COLLAPSE)))
	{
		if(nPower >= bombperpower)
			flag |= PLAYER_SHOT;
		else
			flag |= PLAYER_COLLAPSE;
	}
}

float Player::TranslatePower(float bulletpower)
{
	return bulletpower * (nPower + 1300000) / 1600000;
}

void Player::GetScoreLife(float maxlife, bool isenemy /* = true */)
{
	if (isenemy)
	{
		nScore += 5000 * maxlife;
	}
	else
	{
		nScore += 3000 * maxlife;
	}
}

int Player::GrazeRegain(int grazenum)
{
	return 200 * grazenum;
}

void Player::callCollapse()
{
	if (flag & PLAYER_COLLAPSE)
	{
		return;
	}
	flag |= PLAYER_COLLAPSE;
	collapsetimer = 0;
}

bool Player::callBomb(bool onlyborder)
{
	if (Chat::chatting || (flag & PLAYER_COLLAPSE) || (flag & PLAYER_BOMB))
	{
		return false;
	}
	if (nPower < bombperpower)
	{
		return false;
	}
	if (onlyborder && (flag & PLAYER_BORDER))
	{
		return true;
	}
	if (!(flag & PLAYER_BORDER) && !(flag & PLAYER_SHOT))
	{
		bordertimer = 0;
		flag |= PLAYER_BORDER;
		return true;
	}
	
	if (bBomb && onlyborder)
	{
		return false;
	}

	if (flag & PLAYER_BORDER)
	{
		if (!hge->Input_GetDIKey(KS_SPECIAL_MP, DIKEY_DOWN) && hge->Input_GetDIKey(KS_SPECIAL_MP))
		{
			return false;
		}
		else
		{
			int _tbt = borderlast - PLAYER_BORDEROFFPRE;
			if (bordertimer < _tbt)
			{
				bordertimer = _tbt;
			}
		}
	}
	flag |= PLAYER_BOMB;
	bInfi = true;
	return true;
}

void Player::callSlowFastChange(bool toslow)
{
	if (toslow)
	{
		hge->Input_SetDIKey(KS_SLOW_MP);
	}
	else
	{
		hge->Input_SetDIKey(KS_SLOW_MP, false);
	}
}

void Player::callPlayerChange()
{
	flag |= PLAYER_PLAYERCHANGE;
	playerchangetimer = 0;
}

bool Player::Merge()
{
	mergetimer++;
	bInfi = true;
	if(mergetimer == 1)
	{
		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
	}
	if(mergetimer < 48)
	{
		flag &= ~PLAYER_SHOOT;
		x = PL_MERGEPOS_X;
		y = PL_MERGEPOS_Y - (mergetimer-16) * 4.5f;
		alpha = (mergetimer-16) * 8;
	}
	else if(mergetimer == 48 && mp.spellmode)
	{
		mergetimer = 208;
	}
	else if(mergetimer > 208 && mergetimer < 240)
	{
		alpha = 0xff;
	}
	else if(mergetimer == 240)
	{
		mergetimer = 0;
		bInfi = false;
		return true;
	}
	return false;
}

bool Player::Shot()
{
	shottimer++;
	if (mp.spellmode)
	{
		shottimer = shotdelay;
	}
	else if(flag & PLAYER_BORDER || flag & PLAYER_BOMB)
	{
		if (flag & PLAYER_BORDER)
		{
			int _tbt = borderlast - PLAYER_BORDEROFFPRE;
			if (bordertimer < _tbt)
			{
				bordertimer = _tbt;
			}
			nFaith *= _PL_BOMBFAITHCOSTRATE;
			borderShot = true;
		}
		shottimer = 0;
		return true;
	}
	
	if(shottimer == 1)
	{
		Item::undrainAll();
		SE::push(SE_PLAYER_SHOT, x);
	}
	else if (shottimer == (8>shotdelay?shotdelay:8) && bossinfo.isSpell() && (BossInfo::spellflag & BISF_BOMB) && !mp.spellmode)
	{
		if (callBomb())
		{
			shottimer = 0;
			return true;
		}
	}
	else if(shottimer == shotdelay)
	{
		shottimer = 0;
		flag |= PLAYER_COLLAPSE;
		return true;
	}

	esShot.hscale = (shotdelay - shottimer) * 4.0f / shotdelay;
	return false;
}

bool Player::Collapse()
{
	collapsetimer++;
	if(collapsetimer == 1)
	{
		Bullet::IzeBuild(BULLETIZE_FADEOUT, x, y, 64);

		nPop = 0;

		if(bossinfo.isSpell())
			BossInfo::failed = true;
		esCollapse.x = x;
		esCollapse.y = y;
		SE::push(SE_PLAYER_DEAD, x);

		float aimx;
		float aimy;
		for(int i=0;i<5;i++)
		{
			aimx = (float)(randt()%360 + 40);
			aimy = (float)(randt()%80 - 40 - (480 - y) / 2);
			Item::Build(nLife ? ITEM_POWER : ITEM_FULL, x, y + 36, false, 18000 + rMainAngle(aimx, aimy), -sqrt(2 * 0.1f * DIST(x, y, aimx, aimy)));
		}
		aimx = (float)(randt()%360 + 40);
		aimy = (float)(randt()%80 - 40 - (480 - y) / 2);
		Item::Build(nLife ? ITEM_BIGPOWER : ITEM_FULL, x, y + 32, false, 18000 + rMainAngle(aimx, aimy), -sqrt(2 * 0.1f * DIST(x, y, aimx, aimy)));

		effCollapse.MoveTo(x, y , 0, true);
		effCollapse.Fire();
	}
	else if(collapsetimer == 64)
	{
		x = PL_MERGEPOS_X;
		y = PL_MERGEPOS_Y;
		for(int i=0;i<PL_SAVELASTMAX;i++)
		{
			lastx[i] = x;
			lasty[i] = y;
			lastmx[i] = x;
			lastmy[i] = y;
		}

		timer = 0;
		collapsetimer = 0;
		vscale = 1.0f;
		flag |= PLAYER_MERGE;
		bInfi = true;
		ncMiss++;
		if(nLife)
		{
			nLife--;
			nPower = PL_DEFAULTNPOWER;
		}
		else
		{
			p.exist = false;
			return true;
		}

		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
		if(bBorder)
		{
			int _tbt = borderlast - PLAYER_BORDEROFFPRE;
			if (bordertimer < _tbt)
			{
				bordertimer = _tbt;
			}
		}

		effCollapse.Stop();

		return true;
	}
	
	esCollapse.hscale = collapsetimer / 1.5f;
	esCollapse.alpha = (BYTE)((WORD)(0xff * collapsetimer) / 0x3f);
	esCollapse.colorSet(0xff0000);

	alpha = (0xff - collapsetimer * 4);
	vscale = (float)(collapsetimer)/40.0f + 1.0f;
	return false;
}

bool Player::Border()
{
	bordertimer++;
	nPop = _PL_EXTENDNPOP;
	if (!(flag & PLAYER_SHOT) && !(flag & PLAYER_COLLAPSE))
	{
		Item::drainAll();
		nScore += nPower / 10000;
	}
	esBorder.hscale = (float)(borderlast-bordertimer) / (float)borderlast;
	esBorderZone.hscale = (float)bordergraze * 2.0f / esBorderZone.GetWidth();
	if(bordertimer == 1)
	{
		ncBorder++;
		bordergraze = 0;
		SE::push(SE_PLAYER_BORDERON, x);
		effBorderOn.MoveTo(x, y, 0, true);
		effBorderOn.Fire();
	}
	else if (bordertimer == borderlast - PLAYER_BORDEROFFPRE + 1)
	{
		effBorderOff.MoveTo(x, y, 0, true);
		effBorderOff.Fire();
		SE::push(SE_PLAYER_BORDEROFF, x);
		bInfi = true;
	}
	else if(bordertimer == borderlast)
	{
		bordertimer = 0;
		AddPower(-bombperpower);

		if (!(flag & PLAYER_BOMB))
		{
			bInfi = false;
			if (borderShot)
			{
				Bullet::IzeBuild(BULLETIZE_FAITH, x, y, IZEZONE_DEFAULTTIME/2);
				borderShot = false;
			}
			else
			{
				Bullet::IzeBuild(BULLETIZE_POINT, x, y, IZEZONE_DEFAULTTIME, bordergraze);
			}
		}
		return true;
	}
	return false;
}

bool Player::Bomb()
{
	bombtimer++;
	_Bomb();
	if(bombtimer == 1)
	{
		nFaith *= _PL_BOMBFAITHCOSTRATE;
		if(bossinfo.isSpell())
			BossInfo::failed = true;
		Item::drainAll();
		bInfi = true;
	}
	
	else if(bombtimer == bomblast)
	{
		bombtimer = 0;
		bInfi = false;
		if(hge->Input_GetDIKey(KS_SLOW_MP))
		{
			flag |= PLAYER_SLOWCHANGE;
			slowtimer = 0;
			flag &= ~PLAYER_FASTCHANGE;
		}
		else
		{
			flag |= PLAYER_FASTCHANGE;
			fasttimer = 0;
			flag &= ~PLAYER_SLOWCHANGE;
		}
		flag &= ~PLAYER_PLAYERCHANGE;

		Bullet::IzeBuild(BULLETIZE_FADEOUT, x, y);

		return true;
	}
	return false;
}

bool Player::SlowChange()
{
	if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_DOWN))
		slowtimer = 0;
	bSlow = true;
	slowtimer++;
	if(slowtimer == 1)
	{
		PlayerGhost::ResetValue();
		SE::push(SE_PLAYER_SLOWON, x);
		for(int i=0;i<PLAYERGHOSTMAX;i++)
		{
			pg[i].timer = 0;
		}
	}
	else if(slowtimer == 16)
	{
		esPoint.alpha = 0xff;
		slowtimer = 0;
		return true;
	}

	esPoint.angle = (24 - slowtimer) * 25;
	esPoint.alpha = slowtimer * 16;
	return false;
}

void Player::changePlayerID(WORD toID, bool moveghost/* =false */)
{
	nowID = toID;
	PlayerGhost::ResetValue(moveghost);
	UpdatePlayerData();
}

bool Player::FastChange()
{
	if(hge->Input_GetDIKey(KS_SLOW_MP, DIKEY_UP))
		fasttimer = 0;
	bSlow = false;
	fasttimer++;
	if(fasttimer == 1)
	{
		PlayerGhost::ResetValue();
		SE::push(SE_PLAYER_SLOWOFF, x);
		for(int i=0;i<PLAYERGHOSTMAX;i++)
		{
			pg[i].timer = 0;
		}
	}
	else if(fasttimer == 16)
	{
		fasttimer = 0;
		return true;
	}
	return false;
}

bool Player::PlayerChange()
{
	if(hge->Input_GetDIKey(KS_CHANGE_MP, DIKEY_DOWN))
		playerchangetimer = 0;
	playerchangetimer++;
	if(playerchangetimer == 1)
	{
		if (nowID == ID)
		{
			changePlayerID(ID_sub_1);
		}
		else if (nowID == ID_sub_1)
		{
			changePlayerID(ID_sub_2);
		}
		else
		{
			changePlayerID(ID);
		}

		SE::push(SE_PLAYER_CHANGE, x);
	}
	else if(playerchangetimer == 16)
	{
		playerchangetimer = 0;
		return true;
	}
	esChange.colorSet(0x3030ff | (((16-playerchangetimer) * 16)<<16));
	return false;
}

bool Player::Graze()
{
	effGraze.Fire();
	SE::push(SE_PLAYER_GRAZE, x);
	return true;
}

void Player::Render()
{
	sprite->SetColor(alpha<<24|diffuse);
	sprite->RenderEx(x, y, 0, hscale, vscale);
}

void Player::RenderEffect()
{
	if (bBorder)
	{
		esBorderZone.Render();
		esBorder.Render();
	}

	effGraze.Render();

	for(int i=0;i<PLAYERGHOSTMAX;i++)
		pg[i].Render();
	if(flag & PLAYER_PLAYERCHANGE)
	{
		esChange.Render();
	}
	if(flag & PLAYER_SHOT)
		esShot.Render();
	if (bBorder)
	{
		effBorderOn.Render();
	}
	effBorderOff.Render();
	if(bSlow)
	{
		esPoint.Render();
		esPoint.headangle = -esPoint.headangle;
		esPoint.Render();
		esPoint.headangle = -esPoint.headangle;
	}
	if(flag & PLAYER_COLLAPSE)
		esCollapse.Render();

	effCollapse.Render();
}