#include "../Header/Enemy.h"
#include "../Header/Player.h"
#include "../Header/SE.h"
#include "../Header/Item.h"
#include "../Header/EffectSp.h"
#include "../Header/Scripter.h"
#include "../Header/Chat.h"
#include "../Header/BossInfo.h"
#include "../Header/Process.h"
#include "../Header/BResource.h"
#include "../Header/FrontDisplay.h"
#include "../Header/SpriteItemManager.h"
#include "../Header/EffectSysIDDefine.h"

#define _DAMAGEZONEMAX	0x10

Enemy Enemy::en[ENEMYMAX];

VectorList<DamageZone> Enemy::dmgz;
HTEXTURE Enemy::texmain;
WORD Enemy::index;
BYTE Enemy::bossflag[ENEMY_BOSSMAX];
BYTE Enemy::spelluptimer[ENEMY_BOSSMAX];
BYTE Enemy::storetimer[ENEMY_BOSSMAX];

Enemy::Enemy()
{
	exist	= false;
	able	= false;
	sprite	= NULL;
	eID		= 0;
	effCollapse.exist = false;
	effShot.exist = false;
}

Enemy::~Enemy()
{
	SpriteItemManager::FreeSprite(&sprite);
}

void Enemy::Init(HTEXTURE _texmain)
{
	texmain = _texmain;
	index = ENEMY_INDEXSTART;
	dmgz.init(_DAMAGEZONEMAX);
	dmgz.clear_item();
}

void Enemy::Action()
{
	PlayerBullet::locked = PBLOCK_LOST;
	for(int i=0;i<ENEMYMAX;i++)
	{
		if(en[i].exist)
		{
			Process::mp.objcount ++;

			DWORD stopflag = Process::mp.GetStopFlag();
			bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_ENEMY);
			if (!binstop)
			{
				en[i].action();
			}
			else
			{
				en[i].actionInStop();
			}
			if(PlayerBullet::locked == PBLOCK_LOST && Enemy::en[i].able)
			{
				if (en[i].x <= M_CLIENT_RIGHT && en[i].x >= M_CLIENT_LEFT
					&& en[i].y <= M_CLIENT_BOTTOM && en[i].y >= M_CLIENT_TOP)
				{
					PlayerBullet::locked = i;
				}
			}
		}
	}
}

void Enemy::ClearAll()
{
	for (int i=0; i<ENEMYMAX; i++)
	{
		en[i].Clear();
	}
	index = ENEMY_INDEXSTART;
}

void Enemy::Clear()
{
	exist = false;
	able = false;
	timer = 0;
}

void Enemy::DamageZoneBuild(float _x, float _y, float _r, float _power)
{
	DamageZone _dmgz;
	_dmgz.x = _x;
	_dmgz.y = _y;
	_dmgz.r = _r;
	_dmgz.power = _power;
	dmgz.push_back(_dmgz);
}

bool Enemy::Build(WORD _eID, BYTE _index, BYTE _tarID, float x, float y, int angle, float speed, BYTE type, float life, int infitimer, DWORD take)
{
	bool rv = false;
	if (_index < ENEMYMAX)
	{
		index = _index;
		rv = true;
	}
	else
	{
		if(type < ENEMY_BOSSTYPEBEGIN)
		{
			for(int j=0;j<ENEMYMAX-ENEMY_INDEXSTART;j++)
			{
				index++;
				if(index == ENEMYMAX || index < ENEMY_INDEXSTART)
				{
					index = ENEMY_INDEXSTART;
				}
				if(!en[index].exist)
				{
					rv = true;
//					i = index;
					break;
				}
			}
		}
	}
	if(!rv)
		return false;
	en[index].valueSet(index, x, y, angle, speed, type, life, infitimer, take);
	en[index].eID = _eID;
	en[index].tarID = _tarID;
	return true;
}

void Enemy::RenderAll()
{
	for (int i=0; i<ENEMYMAX; i++)
	{
		if (en[i].exist)
		{
			en[i].Render();
		}
	}
	for (int i=0; i<ENEMYMAX; i++)
	{
		if (en[i].exist)
		{
			en[i].RenderEffect();
		}
	}
}

void Enemy::Render()
{
	sprite->SetColor(alpha<<24|diffuse);
	sprite->RenderEx(x, y, 0, hscale, vscale);
}

void Enemy::RenderEffect()
{
	if(fadeout && timer && effCollapse.eff)
	{
		effCollapse.Render();
	}
	effShot.Render();
}

void Enemy::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, float _life, int _infitimer, DWORD _take,
					 WORD _ac, float para0, float para1, float para2, float para3)
{
	ID		=	_ID;
	x		=	_x;
	lastx	=	x;
	y		=	_y;
	speed	=	_speed;
	type	=	_type;
	life	=	_life;
	ac		=	_ac;
	angle	=	_angle;
	take	=	_take;
	infitimer = _infitimer;

	timer	=	0;
	damagetimer = 0;
	maxlife	=	life;
	defrate	=	0;
	exist	=	true;
	fadeout	=	false;
	able	=	true;
	damage	=	false;
	hscale	=	1.0f;
	vscale	=	1.0f;
	alpha	=	0xff;
	diffuse	=	0xffffff;
	faceindex = BResource::res.enemydata[type].faceIndex;

	if (type >= ENEMY_BOSSTYPEBEGIN)
	{
		bossflag[ID]		=	0;
		spelluptimer[ID]	=	0;
		storetimer[ID]		=	0;
	}

	eID		=	0;
	tarID	=	0xff;

	aim.x	=	0;
	aim.y	=	0;

	para[0] = para0;
	para[1] = para1;
	para[2] = para2;
	para[3] = para3;

	headangle = -angle;

	enemyData * _enemydata = &(BResource::res.enemydata[type]);
	if (sprite)
	{
		SpriteItemManager::ChangeSprite(_enemydata->siid, sprite);
	}
	else
	{
		sprite = SpriteItemManager::CreateSprite(_enemydata->siid);
	}
	initFrameIndex();
	setFrame(ENEMY_FRAME_STAND);

	effShot.valueSet(EFF_EN_SHOT, *this);
	effShot.Stop();
}

void Enemy::matchAction()
{
	switch(ac)
	{
	case ENAC_NONE:
		break;
	case ENAC_DIRECTSET_XYAS:
		//x，y，angle，speed
		//直接设置（需要连续设置）
		x = para[0];
		y = para[1];
		angle = para[2];
		speed = para[3]/1000.0f;
		break;
	case ENAC_CHASEPLAYER_TFR:
		//作用时间，摩擦力，退出角度
		//靠近主角
		if(timer < para[0])
		{
			angle = aMainAngle(Player::p.x, Player::p.y);
			speed *= para[1]/1000.0f;
		}
		else
		{
			angle = (int)para[2];
		}
		break;
	case ENAC_ATTENUATION_TFER:
		//作用时间，摩擦力，停止时间，退出角度
		//向原方向前进、停止、撤离
		if(timer < para[0])
		{
			speed *= para[1]/1000.0f;
		}
		else if(timer < para[2])
			speed = 0;
		else
		{
			angle = (int)para[3];
			speed += 0.03f;
		}
		break;
	case ENAC_CIRCLE_TXYE:
		//[+-]起始时间，环绕中心x，环绕中心y，终止时间
		//绕圆周行走
		if(para[0] > 0)
		{
			if(timer > (DWORD)para[0] && timer < (DWORD)para[3])
			{
				float tr = DIST(x, y, para[1], para[2]);
				angle += ANGLE(speed / tr);
			}
		}
		else
		{
			if(timer > (DWORD)(-para[0]) && timer < (DWORD)para[3])
			{
				float tr = DIST(x, y, para[1], para[2]);
				angle -= ANGLE(speed / tr);
			}
		}
		break;
	case ENAC_BROKENLINE_CATE:
		//作用时间[计数器]，变角范围，更替周期，终止时间
		//折线行走
		if((int)para[0] && timer < para[3])
		{
			speed = speed * ((int)para[0] - 1) / (int)para[0];
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else if(timer < para[3])
		{
			para[0] = para[2];
			speed = para[2] / 30;
			if(para[1] > 0)
			{
				angle = para[1];
				para[1] = 18000 - para[1];
			}
			else
			{
				angle = 18000 + para[1];
				para[1] = -18000 - para[1];
			}
		}
		else
		{
			speed += 0.03f;
			angle = -9000;
		}
		break;
	case ENAC_STOPANDSTRIKE_TEA:
		//作用时间，停止时间，放弃时间
		//减速至0，后冲向Player
		if(timer < para[0])
		{
			speed -= speed / (para[0] - timer);
		}
		else if(timer < para[1])
		{
			speed = 0;
		}
		else if(timer < para[2])
		{
			speed += 0.06f;
			angle = aMainAngle(Player::p.x, Player::p.y);
		}
		break;

	case ENAC_REPOSITION_T:
		//作用时间[计数器]
		//BOSS出场复位
		if((int)para[0])
		{
			chaseAim(M_ACTIVECLIENT_BOSSX, M_ACTIVECLIENT_BOSSY, para[0]);
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else
		{
			ac = 0;
			speed = 0;
		}
		break;
	case ENAC_OVERPLAYER_CXYT:
		//作用时间[计数器]，目标x，目标y，更替周期
		//在主角上方随机
		if((int)para[0])
		{
			chaseAim(para[1], para[2], para[0]);
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else if(timer % (int)para[3] == 0)
		{
			para[0] = 2 * para[3] > 120 ? 120 : 2 * para[3];
			if(Player::p.x > x)
				para[1] = Player::p.x + randt() % 60;
			else
				para[1] = Player::p.x - randt() % 60;
			if(para[1] < M_ACTIVECLIENT_RIGHT*0.15f)
			{
				if(x <= M_ACTIVECLIENT_RIGHT*0.15f + 8)
					para[1] = M_ACTIVECLIENT_RIGHT*0.15f + 8 + randt() % 50;
				else
					para[1] = M_ACTIVECLIENT_RIGHT*0.15f;
			}
			else if(para[1] > M_ACTIVECLIENT_RIGHT*0.85f)
			{
				if(x >= M_ACTIVECLIENT_RIGHT*0.85f- 8)
					para[1] = M_ACTIVECLIENT_RIGHT*0.85f - 8 - randt() % 50;
				else
					para[1] = M_ACTIVECLIENT_RIGHT*0.85f;
			}
			para[2] = randt() % 80 + M_ACTIVECLIENT_BOSSY - 40;
		}
		break;
	case ENAC_CHASETO_CXY:
		//作用时间[计数器]，目标x，目标y
		//滑步走向点
		if((int)para[0])
		{
			chaseAim(para[1], para[2], para[0]);
			para[0] -= 0.8f;
			para[0] = (int)para[0];
		}
		else
			speed = 0;
		break;
	}
}

void Enemy::setMove(float para0, float para1, float para2, float para3, WORD _ac)
{
	para[0] = para0;
	para[1] = para1;
	para[2] = para2;
	para[3] = para3;
	ac = _ac;
}

void Enemy::updateFrame(BYTE frameenum, int usetimer /* = -1*/)
{
	if (usetimer == -1)
	{
		usetimer = timer;
	}
	if (frameenum != nowstate && (frameenum == ENEMY_FRAME_STAND || frameenum+1 != nowstate))
	{
		setFrame(frameenum);
		return;
	}
	if ((usetimer % ENEMY_ANIMATIONSPEED))
	{
		return;
	}
	enemyData * pdata = &(BResource::res.enemydata[type]);
	frameoffset++;
	BYTE tbyte;
	switch (nowstate)
	{
	case ENEMY_FRAME_STAND:
		if (frameoffset >= pdata->standFrame)
		{
			setFrame(ENEMY_FRAME_STAND);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_STAND) + frameoffset);
		}
		if (pdata->standshake)
		{
			float thsx;
			float thsy;
			sprite->GetHotSpot(&thsx, &thsy);
			sprite->SetHotSpot(thsx, sint(timer*512)*4.8f+thsy-2.4f);
		}
		break;
	case ENEMY_FRAME_RIGHTPRE:
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
			setFrame(ENEMY_FRAME_RIGHT);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_RIGHTPRE) + frameoffset);
		}
		break;
	case ENEMY_FRAME_RIGHT:
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
			setFrame(ENEMY_FRAME_RIGHT);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_RIGHT) + frameoffset);
		}
		break;
	case ENEMY_FRAME_LEFTPRE:
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
			setFrame(ENEMY_FRAME_LEFT);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_LEFTPRE) + frameoffset);
		}
		break;
	case ENEMY_FRAME_LEFT:
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
			setFrame(ENEMY_FRAME_LEFT);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_LEFT) + frameoffset);
		}
		break;
	case ENEMY_FRAME_ATTACKPRE:
		tbyte = pdata->attackPreFrame;
		if (frameoffset >= tbyte)
		{
			setFrame(ENEMY_FRAME_ATTACK);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_ATTACKPRE) + frameoffset);
		}
		break;
	case ENEMY_FRAME_ATTACK:
		tbyte = pdata->attackFrame;
		if (frameoffset >= tbyte)
		{
			setFrame(ENEMY_FRAME_ATTACK);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_ATTACK) + frameoffset);
		}
		break;
	case ENEMY_FRAME_STOREPRE:
		tbyte = pdata->storePreFrame;
		if (frameoffset >= tbyte)
		{
			setFrame(ENEMY_FRAME_STORE);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_STOREPRE) + frameoffset);
		}
		break;
	case ENEMY_FRAME_STORE:
		tbyte = pdata->storeFrame;
		if (frameoffset >= tbyte)
		{
			setFrame(ENEMY_FRAME_STORE);
		}
		else
		{
			setIndexFrame(getFrameIndex(ENEMY_FRAME_STORE) + frameoffset);
		}
		break;
	}
}

void Enemy::updateFrameAsMove()
{
	if(lastx - x > ENEMY_BOSSMOVELIMIT)
	{
		updateFrame(ENEMY_FRAME_LEFTPRE);
	}
	else if(x - lastx > ENEMY_BOSSMOVELIMIT)
	{
		updateFrame(ENEMY_FRAME_RIGHTPRE);
	}
	else
	{
		updateFrame(ENEMY_FRAME_STAND);
	}
}

void Enemy::bossAction()
{
	enemyData * pdata = &(BResource::res.enemydata[type]);
	if(timer < ENEMY_BOSSINFITIMER)
		defrate = 1.0f;
	else if(timer == ENEMY_BOSSINFITIMER)
		defrate = 0;
	if (BossInfo::bossinfo.spellflag & BISF_WAIT)
	{
		defrate = 1.0f;
	}
	if(!bossflag[ID])
	{
		updateFrameAsMove();
	}
	if(bossflag[ID] & BOSS_ATTACK)
	{
		if (!pdata->attackFrame)
		{
			updateFrameAsMove();
		}
		else
		{
			updateFrame(ENEMY_FRAME_ATTACKPRE);
		}
		bossflag[ID] &= ~BOSS_ATTACK;
	}
	if(bossflag[ID] & BOSS_STORE)
	{
		if (!pdata->storeFrame)
		{
			updateFrameAsMove();
		}
		else
		{
			updateFrame(ENEMY_FRAME_STOREPRE);
		}
		storetimer[ID]++;

		if(storetimer[ID] == 1)
		{
			SE::push(SE_BOSS_POWER_1, x);
			FrontDisplay::fdisp.infobody.effBossStore.Stop(true);
			FrontDisplay::fdisp.infobody.effBossStore.Fire();
			FrontDisplay::fdisp.infobody.effBossStore.MoveTo(x, y, 0, true);
		}
		else if(storetimer[ID] == 120)
		{
			storetimer[ID] = 0;
			bossflag[ID] &= ~BOSS_STORE;
		}
	}
	else
	{
		FrontDisplay::fdisp.infobody.effBossStore.Stop();
	}
	FrontDisplay::fdisp.infobody.effBossStore.MoveTo(x, y);
	if(bossflag[ID] & BOSS_SPELLUP)
	{
		if (!pdata->attackFrame)
		{
			updateFrameAsMove();
		}
		else
		{
			updateFrame(ENEMY_FRAME_ATTACKPRE);
		}
		if(spelluptimer[ID] == 0)
		{
			SE::push(SE_BOSS_POWER_2, x);
		}
		else if(spelluptimer[ID] == 180)
		{
			bossflag[ID] &= ~BOSS_SPELLUP;
			spelluptimer[ID] = 0;
		}
		spelluptimer[ID]++;
	}
}

void Enemy::initFrameIndex()
{
	enemyData * pdata = &(BResource::res.enemydata[type]);
	int tfi = 0;
	frameindex[ENEMY_FRAME_STAND] = tfi;

	bool bhr = pdata->rightPreFrame;
	bool bhl = pdata->leftPreFrame;

	tfi += pdata->standFrame;
	frameindex[ENEMY_FRAME_RIGHTPRE] = tfi;
	if (bhr)
	{
		tfi += pdata->rightPreFrame;
	}
	else
	{
		tfi += pdata->leftPreFrame;
	}
	frameindex[ENEMY_FRAME_RIGHT] = tfi;
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
	frameindex[ENEMY_FRAME_LEFTPRE] = tfi;
	if (bhr)
	{
		tfi += pdata->rightPreFrame;
	}
	else
	{
		tfi += pdata->leftPreFrame;
	}
	frameindex[ENEMY_FRAME_LEFT] = tfi;

	if (bhr)
	{
		tfi += pdata->rightFrame;
	}
	else
	{
		tfi += pdata->leftFrame;
	}
	frameindex[ENEMY_FRAME_ATTACKPRE] = tfi;
	tfi += pdata->attackPreFrame;
	frameindex[ENEMY_FRAME_ATTACK] = tfi;
	tfi += pdata->attackFrame;
	frameindex[ENEMY_FRAME_STOREPRE] = tfi;
	tfi += pdata->storePreFrame;
	frameindex[ENEMY_FRAME_STORE] = tfi;
	tfi += pdata->storeFrame;
}

BYTE Enemy::getFrameIndex(BYTE frameenum)
{
	flipx = false;
	enemyData * pdata = &(BResource::res.enemydata[type]);
	if ((frameenum == ENEMY_FRAME_RIGHTPRE || frameenum == ENEMY_FRAME_RIGHT) && (!pdata->rightPreFrame) ||
		(frameenum == ENEMY_FRAME_LEFTPRE || frameenum == ENEMY_FRAME_LEFT) && (!pdata->leftPreFrame))
	{
		flipx = true;
	}
	return frameindex[frameenum];
}

void Enemy::setFrame(BYTE frameenum)
{
	frameoffset = 0;
	setIndexFrame(getFrameIndex(frameenum));
	nowstate = frameenum;
}

void Enemy::setIndexFrame(BYTE index)
{
	enemyData * pdata = &(BResource::res.enemydata[type]);
	SpriteItemManager::ChangeSprite(pdata->siid+index, sprite);
	SpriteItemManager::SetSpriteFlip(sprite, flipx);
}

void Enemy::GetCollisionRect(float * w, float * h)
{
	*w = BResource::res.enemydata[type].collision_w;
	*h = BResource::res.enemydata[type].collision_h;
}

void Enemy::CostLife(float power)
{
	if (!Player::p.bBomb || !(BossInfo::bossinfo.spellflag & BISF_NOBOMBDAMAGE))
	{
		life -= power * (1 - defrate);
	}
	damage = true;
}

bool Enemy::isInRange(float _x, float _y, float _r)
{
	float _tw;
	float _th;
	GetCollisionRect(&_tw, &_th);
	_tw /= 2;
	_th /= 2;
	if (checkCollisionBigCircle(_x - _tw, _y + _th, _r) ||
		checkCollisionBigCircle(_x + _tw, _y + _th, _r) ||
		checkCollisionBigCircle(_x - _tw, _y - _th, _r) ||
		checkCollisionBigCircle(_x + _tw, _y - _th, _r))
	{
		return true;
	}
	return false;
}

void Enemy::actionInStop()
{
	if (!fadeout)
	{
		DoShot();
	}
}

void Enemy::DoShot()
{
	float tw;
	float th;
	GetCollisionRect(&tw, &th);
	if (PlayerBullet::pb.getSize())
	{
		DWORD i = 0;
		DWORD size = PlayerBullet::pb.getSize();
		for (PlayerBullet::pb.toBegin(); i<size; PlayerBullet::pb.toNext(), i++)
		{
			if (PlayerBullet::pb.isValid() && (*PlayerBullet::pb).able)
			{
				if ((*PlayerBullet::pb).isInRange(x, y, tw, th))
				{
					CostLife((*PlayerBullet::pb).power);
				}
			}
		}
	}
	if (dmgz.getSize())
	{
		DWORD i = 0;
		DWORD size = dmgz.getSize();
		for (dmgz.toBegin(); i<size; dmgz.toNext(), i++)
		{
			if (dmgz.isValid())
			{
				DamageZone * tdmg = &(*dmgz);
				if (isInRange(tdmg->x, tdmg->y, tdmg->r))
				{
					CostLife(tdmg->power);
					Player::p.DoPlayerBulletHit();
				}
			}
		}
	}


	if(!damage && life < maxlife / 5 && timer%8<4)
	{
		SE::push(SE_ENEMY_DAMAGE_1, x);

		if(BossInfo::bossinfo.bossable() && type >= ENEMY_BOSSTYPEBEGIN)
			FrontDisplay::fdisp.info.enemyx->SetColor(0xffffffff);
	}

	if(damage && !damagetimer)
	{
		damagetimer++;
	}
	else if(damagetimer > 0)
	{
		if(damagetimer < 8)
			damagetimer++;
		else
			damagetimer = 0;
		if(damagetimer > 0 && damagetimer % 8 < 4)
		{
			alpha = 0x7f;
			diffuse = 0xb40000;
			effShot.Fire();

			if(BossInfo::bossinfo.bossable() && type >= ENEMY_BOSSTYPEBEGIN)
				FrontDisplay::fdisp.info.enemyx->SetColor(0xc0ffffff);

			if(life < maxlife / 5)
			{
				SE::push(SE_ENEMY_DAMAGE_2, x);
			}
			else
				SE::push(SE_ENEMY_DAMAGE_1, x);
		}
		else
		{
			alpha = 0xff;
			diffuse = 0xffffff;
			effShot.Stop();
		}
	}
	if(life < 0)
	{
		WORD tindex = index;
		index = ID;
		Scripter::scr.edefExecute(eID, SCRIPT_CON_POST);
		index = tindex;

		if (life < 0)
		{
			Player::p.GetScoreLife(maxlife, true);

			SE::push(SE_ENEMY_DEAD, x);

			fadeout = true;
			timer = 0;
		}
	}

}

void Enemy::action()
{
	timer++;

	if(infitimer)
	{
		infitimer--;
		defrate = 1.0f;
		if(!infitimer)
			defrate = 0;
	}

	effShot.MoveTo(x, y);
	effShot.action();

	if(!fadeout)
	{
		if((Chat::chatitem.chatting || (BossInfo::bossinfo.bossout())) && type < ENEMY_BOSSTYPEBEGIN)
		{
			life = 0;
			fadeout = true;
			timer = 0;
		}

		if(eID && !Scripter::stopEdefScript)
		{
			WORD tindex = index;
			index = ID;
			Scripter::scr.edefExecute(eID, timer);
			index = tindex;
		}
		matchAction();

		x += speed * cost(angle);
		y += speed * sint(angle);

		if(type >= ENEMY_BOSSTYPEBEGIN)
			bossAction();
		else
		{
			if(lastx - x > ENEMY_ENEMYMOVELIMIT)
			{
				updateFrame(ENEMY_FRAME_LEFTPRE);
			}
			else if(x - lastx > ENEMY_ENEMYMOVELIMIT)
			{
				updateFrame(ENEMY_FRAME_RIGHTPRE);
			}
			else
			{
				updateFrame(ENEMY_FRAME_STAND);
			}
		}

		lastx = x;

		if(tarID != 0xff)
		{
			Target::SetValue(tarID, x, y);
		}

		float tw;
		float th;
		GetCollisionRect(&tw, &th);
		if (!Player::p.bInfi && !Player::p.bBomb && !Player::p.bBorder)
		{
			if (checkCollisionSquare(Player::p.x, Player::p.y, tw, th))
			{
				Player::p.DoShot();
			}
		}
		if(BossInfo::bossinfo.bossable())
		{
			int txdiff = fabsf(Player::p.x - x);
			if(txdiff < ENEMY_BOSSX_FADERANGE)
				FrontDisplay::fdisp.info.enemyx->SetColor(((0x40 + txdiff*2) << 24) | 0xffffff);
			else
				FrontDisplay::fdisp.info.enemyx->SetColor(0x80ffffff);
		}
		DoShot();
		if(x > M_DELETECLIENT_RIGHT || x < M_DELETECLIENT_LEFT || y > M_DELETECLIENT_BOTTOM || y < M_DELETECLIENT_TOP)
			exist = false;
	}
	else
	{
		if(timer == 1)
		{
			giveItem();
			effShot.Stop();
			effCollapse.valueSet(EFF_EN_COLLAPSE, *this);
		}
		else if(timer == 32)
		{
			effCollapse.Stop();
			exist = false;
		}
		effCollapse.action();

		if(type < ENEMY_BOSSTYPEBEGIN)
		{
			hscale	=	timer * 0.05f + 1.0f;
			vscale	=	(32 - timer) * 0.03225f;
			alpha	=	(BYTE)((32 - timer) * 8);
		}
	}

	damage = false;
	able = exist && !fadeout;
}

void Enemy::giveItem()
{
	if(Process::mp.spellmode)
		return;

	bool first = true;
	float aimx;
	float aimy;

	float tempx = x;
	float tempy = y;

	y -= randt()%30;
	if(x > PL_MOVABLE_RIGHT)
		x = PL_MOVABLE_RIGHT;
	else if(x < PL_MOVABLE_LEFT)
		x = PL_MOVABLE_LEFT;

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<(int)((take>>(i*4))&0xf);j++)
		{
			if(!first)
			{
				aimx = (float)((x + randt()%80 - 40));
				if(aimx > 417)
					aimx = 417;
				else if(aimx < 23)
					aimx = 23;
				aimy = (float)(randt()%80 - 240 + y);
				Item::Build(i, x, y, false, 18000 + rMainAngle(aimx, aimy), -sqrt(2 * 0.1f * DIST(x, y, aimx, aimy)));
			}
			else
			{
				Item::Build(i, x, y);
			}
			first = false;
		}
	}
	x = tempx;
	y = tempy;
}
