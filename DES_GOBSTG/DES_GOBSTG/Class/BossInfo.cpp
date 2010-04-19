#include "../Header/BossInfo.h"
#include "../Header/Enemy.h"
#include "../Header/Player.h"
#include "../Header/SE.h"
#include "../Header/EffectSp.h"
#include "../Header/Data.h"
#include "../Header/Process.h"
#include "../Header/BGLayer.h"
#include "../Header/Fontsys.h"
#include "../Header/Item.h"
#include "../Header/Ghost.h"
#include "../Header/BResource.h"
#include "../Header/DataConnector.h"
#include "../Header/Scripter.h"
#include "../Header/FrontDisplay.h"
#include "../Header/Bullet.h"

BossInfo BossInfo::bossinfo;
int BossInfo::sno;
int BossInfo::turntoscene;
bool BossInfo::failed;
bool BossInfo::allover;
BYTE BossInfo::flag;
BYTE BossInfo::spellflag;

bool BossInfo::Init()
{
	Release();

	allover = false;

	return true;
}

void BossInfo::Clear()
{
	turntoscene = S0;
	spellflag = BISF_NONE;
	flag = 0;
}

//bossinfo
bool BossInfo::Fill(int _sno)
{
	for(vector<spellData>::iterator i=BResource::res.spelldata.begin(); i!=BResource::res.spelldata.end(); i++)
	{
		if(i->sno == _sno)
		{
			sno = _sno;
			spellflag = i->spellflag;
			strcpy(spellname, i->spellname);
			strcpy(enemyname, Data::data.getEnemyName(i->userID));
			strcpy(enemyename, Data::data.getEnemyEName(i->userID));
			limit = i->timelimit;
			remain = i->remain;
			turntoscene = i->turntoscene;
			bonus = i->bonus;
			maxbonus = bonus;
			return true;
		}
	}
	return false;
}

bool BossInfo::isSpell()
{
	return flag && !(spellflag & BISF_NOTSPELL);
}

void BossInfo::bossUp()
{
	Fill(Process::mp.scene);

	failed	= false;
	lastgraze = Player::p.nGraze;

	flag = BOSSINFO_UP | BOSSINFO_ENABLE;
	timer = 0;
	itemstack = 0;

	if(!(spellflag & BISF_NOTSPELL))
	{
		BGLayer::ubg[UBGID_BGMASK].SetFlag(BG_WHITEFLASH, BGMT_FLASH);
		Enemy::bossflag[ENEMY_MAINBOSSINDEX] = BOSS_SPELLUP;
		Fontsys::SignUp(FONTSYS_SPELLNAMEUSE, spellname, FrontDisplay::fdisp.info.smallfont);
		get = DataConnector::nGet();
		meet = DataConnector::nMeet() - 1;
		if(meet < 0)
			meet = 0;
	}
}

void BossInfo::Release()
{
	flag = 0;
	spellflag = BISF_NONE;
	failed = false;
}

void BossInfo::quit()
{
	if (flag & BOSSINFO_COLLAPSE)
	{
		Player::p.bInfi = false;
	}
	else if (flag & BOSSINFO_TIMEOVER)
	{
		failed = true;
	}
	if(!(spellflag & BISF_NOTSPELL) && Process::mp.spellmode)
	{
		BGLayer::ubg[UBGID_BGMASK].SetFlag(BG_WHITEOUT, BGMT_OUT);
	}
	if(remain)
	{
		Enemy::en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
	}

	Scripter::scr.stageExecute(sno, SCRIPT_CON_POST);

	spellflag = BISF_NONE;
	flag = 0;
	exist = false;

	allover = false;
	Process::mp.frameskip = M_DEFAULT_FRAMESKIP;
}

bool BossInfo::action()
{
	exist = true;

	if(flag < BOSSINFO_COLLAPSE)
	{
		if (Enemy::en[ENEMY_MAINBOSSINDEX].able)
		{
			if(timer == limit * 60)
			{
				flag |= BOSSINFO_TIMEOVER;
				timer = 0;
			}
		}
		else
		{
			flag |= BOSSINFO_COLLAPSE;
			if(!(spellflag & BISF_NOTSPELL) && !failed)
			{
				if(spellflag & BISF_WAIT)
					timer = limit * 30;
				itemstack = (limit * 60 - timer) / 15;
			}
			timer = 0;
		}
	}
	timer++;
	FrontDisplay::fdisp.BossAction();

	if(flag & BOSSINFO_UP)
	{
		if(timer == 160)
			flag &= ~BOSSINFO_UP;
	}
	else if(flag & BOSSINFO_COLLAPSE)
	{
		Player::p.bInfi = true;
		if(timer == 1)
		{
			Scripter::scr.stageExecute(sno, SCRIPT_CON_QUIT);

			if(remain)
			{
				Enemy::en[ENEMY_MAINBOSSINDEX].alpha = 0x7f;
				Enemy::en[ENEMY_MAINBOSSINDEX].defrate = 1.0f;
				Enemy::en[ENEMY_MAINBOSSINDEX].ac = 0;
				Enemy::en[ENEMY_MAINBOSSINDEX].speed = 0;
			}
			Player::p.nScore += bonus;
			if(!(spellflag & BISF_NOTSPELL))
			{
				Bullet::IzeBuild(BULLETIZE_FAITH, Enemy::en[ENEMY_MAINBOSSINDEX].x, Enemy::en[ENEMY_MAINBOSSINDEX].y);
			}
			else
			{
				Bullet::IzeBuild(BULLETIZE_FADEOUT, Enemy::en[ENEMY_MAINBOSSINDEX].x, Enemy::en[ENEMY_MAINBOSSINDEX].y);
				timer = 100;
			}

			if(!failed)
			{
				DataConnector::Get();
			}

			if(!failed && !(spellflag & BISF_NOTSPELL))
			{
				Player::p.getspell[Player::p.ncGet] = Process::mp.scene;
				Player::p.ncGet++;
			}

			if(turntoscene - SCLEAR > S1 && !Process::mp.spellmode)
			{
				allover = true;
			}
		}
		else if(timer == 160)
		{
			quit();
			return true;
		}
		if(Enemy::en[ENEMY_MAINBOSSINDEX].timer == 16 && Enemy::en[ENEMY_MAINBOSSINDEX].life < 0 && !Process::mp.spellmode && remain)
		{
			Enemy::en[ENEMY_MAINBOSSINDEX].exist = true;
			Enemy::en[ENEMY_MAINBOSSINDEX].fadeout = false;
			Enemy::en[ENEMY_MAINBOSSINDEX].life = 0;
			Enemy::en[ENEMY_MAINBOSSINDEX].timer = 0;
			Enemy::en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
		}
	}
	else if(flag & BOSSINFO_TIMEOVER)
	{
		if(timer == 1)
		{
			Scripter::scr.stageExecute(sno, SCRIPT_CON_QUIT);
			SE::push(SE_BOSS_TIMEUP);
			if(remain)
			{
				Enemy::en[ENEMY_MAINBOSSINDEX].defrate = 1.0f;
				Enemy::en[ENEMY_MAINBOSSINDEX].alpha = 0x7f;
			}
			if(!(spellflag & BISF_WAIT))
				failed = true;
			if(!failed && !(spellflag & BISF_NOTSPELL) && (spellflag & BISF_WAIT))
			{
				Player::p.getspell[Player::p.ncGet] = Process::mp.scene;
				Player::p.ncGet++;
			}
			if(spellflag & BISF_NOTSPELL)
			{
				timer = 100;
			}
			Bullet::IzeBuild(BULLETIZE_FADEOUT, Enemy::en[ENEMY_MAINBOSSINDEX].x, Enemy::en[ENEMY_MAINBOSSINDEX].y);
			if((spellflag & BISF_WAIT) || !remain)
			{
				Enemy::en[ENEMY_MAINBOSSINDEX].life = -1;
			}

			if(turntoscene - SCLEAR > S1 && !Process::mp.spellmode)
			{
				allover = true;
			}
		}
		if(timer == 160)
		{
			quit();
			return true;
		}
		if(Enemy::en[ENEMY_MAINBOSSINDEX].timer == 16 && Enemy::en[ENEMY_MAINBOSSINDEX].life < 0 && !Process::mp.spellmode && remain)
		{
			Enemy::en[ENEMY_MAINBOSSINDEX].exist = true;
			Enemy::en[ENEMY_MAINBOSSINDEX].fadeout = false;
			Enemy::en[ENEMY_MAINBOSSINDEX].life = 0;
			Enemy::en[ENEMY_MAINBOSSINDEX].timer = 0;
			Enemy::en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
		}
	}
	if(!(spellflag & BISF_NOTSPELL) && flag < BOSSINFO_COLLAPSE)
	{
		if(!failed && !(spellflag & BISF_WAIT))
		{
			int minus;
			minus = maxbonus / (limit*60);
			minus -= Player::p.GrazeRegain(Player::p.nGraze - lastgraze);
			bonus -= minus;
			if(bonus > maxbonus)
				bonus = maxbonus;
			lastgraze = Player::p.nGraze;
		}
		else if(failed)
			bonus = maxbonus / 100;
		else if(timer == limit * 60 - 1)
			Enemy::en[ENEMY_MAINBOSSINDEX].life = -1;
	}
	if(flag < BOSSINFO_COLLAPSE)
	{
		if(timer >= (limit-10) * 60 && timer % 60 == 0)
			SE::push(SE_BOSS_TIMEOUT);
		if(timer >= (limit-3) * 60 && timer % 60 == 10)
			SE::push(SE_BOSS_TIMEOUT);
	}
	else if(!(spellflag & BISF_NOTSPELL) && !failed)
	{
		if(timer < 90)
		{
			int tthrow = itemstack / (90-timer);
			float tr;
			float tangle;
			if(timer <= 30)
			{
				tr = timer + 40;
				tangle = timer * 1800;
			}
			else if(timer <= 60)
			{
				tr = timer + 20;
				tangle = -timer * 1800 + 12000;
			}
			else
			{
				tr = timer;
				tangle = timer * 1800 - 12000;
			}
			float tx = Enemy::en[ENEMY_MAINBOSSINDEX].x + tr * cost(tangle);
			float ty = Enemy::en[ENEMY_MAINBOSSINDEX].y + tr * sint(tangle);

			FrontDisplay::fdisp.BossMoveItemEffect(tx, ty);
//			effItem.MoveTo(tx, ty);

			for(int i=0;i<tthrow;i++)
			{
				Item::Build(ITEM_SMALLFAITH, tx+randt()%10-5, ty+randt()%10-5, true);
			}
			itemstack -= tthrow;
		}
	}
	return false;
}
