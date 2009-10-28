#include "BossInfo.h"
#include "Enemy.h"
#include "Player.h"
#include "SE.h"
#include "EffectSp.h"
#include "Data.h"
#include "Process.h"
#include "BGLayer.h"
#include "Fontsys.h"
#include "Item.h"
#include "Ghost.h"
#include "BResource.h"
#include "DataConnector.h"
#include "Scripter.h"
#include "FrontDisplay.h"
#include "Bullet.h"

BossInfo bossinfo;
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

void BossInfo::empty()
{
	turntoscene = S0;
	spellflag = BISF_NONE;
	flag = 0;
}

//bossinfo
bool BossInfo::Fill(int _sno)
{
	for(vector<spellData>::iterator i=res.spelldata.begin(); i!=res.spelldata.end(); i++)
	{
		if(i->sno == _sno)
		{
			sno = _sno;
			spellflag = i->spellflag;
			strcpy(spellname, i->spellname);
			strcpy(enemyname, data.getEnemyName(i->userID));
			strcpy(enemyename, data.getEnemyEName(i->userID));
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
	Fill(mp.scene);

	failed	= false;
	lastgraze = Player::p.nGraze;

	flag = BOSSINFO_UP | BOSSINFO_ENABLE;
	timer = 0;
	itemstack = 0;

	if(!(spellflag & BISF_NOTSPELL))
	{
		bgmask.SetFlag(BG_WHITEFLASH, BGMT_FLASH);
		Enemy::bossflag[ENEMY_MAINBOSSINDEX] = BOSS_SPELLUP;
		Fontsys::SignUp(FONTSYS_SPELLNAMEUSE, spellname, fdisp.info.smallfont);
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
	if(!(spellflag & BISF_NOTSPELL) && mp.spellmode)
	{
		bgmask.SetFlag(BG_WHITEOUT, BGMT_OUT);
	}
	if(remain)
	{
		en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
	}

	scr.stageExecute(sno, SCRIPT_CON_POST);

	spellflag = BISF_NONE;
	flag = 0;
	exist = false;

	allover = false;
	mp.frameskip = M_DEFAULT_FRAMESKIP;
}

bool BossInfo::action()
{
	exist = true;

	if(flag < BOSSINFO_COLLAPSE)
	{
		if (en[ENEMY_MAINBOSSINDEX].able)
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
	fdisp.BossAction();

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
			scr.stageExecute(sno, SCRIPT_CON_QUIT);

			if(remain)
			{
				en[ENEMY_MAINBOSSINDEX].alpha = 0x7f;
				en[ENEMY_MAINBOSSINDEX].defrate = 1.0f;
				en[ENEMY_MAINBOSSINDEX].ac = 0;
				en[ENEMY_MAINBOSSINDEX].speed = 0;
			}
			Player::p.nScore += bonus;
			if(!(spellflag & BISF_NOTSPELL))
			{
				Bullet::IzeBuild(BULLETIZE_FAITH, en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y);
			}
			else
			{
				Bullet::IzeBuild(BULLETIZE_FADEOUT, en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y);
				timer = 100;
			}

			if(!failed)
			{
				DataConnector::Get();
			}

			if(!failed && !(spellflag & BISF_NOTSPELL))
			{
				Player::p.getspell[Player::p.ncGet] = mp.scene;
				Player::p.ncGet++;
			}

			if(turntoscene - SCLEAR > S1 && !mp.spellmode)
			{
				allover = true;
			}
		}
		else if(timer == 160)
		{
			quit();
			return true;
		}
		if(en[ENEMY_MAINBOSSINDEX].timer == 16 && en[ENEMY_MAINBOSSINDEX].life < 0 && !mp.spellmode && remain)
		{
			en[ENEMY_MAINBOSSINDEX].exist = true;
			en[ENEMY_MAINBOSSINDEX].fadeout = false;
			en[ENEMY_MAINBOSSINDEX].life = 0;
			en[ENEMY_MAINBOSSINDEX].timer = 0;
			en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
		}
	}
	else if(flag & BOSSINFO_TIMEOVER)
	{
		if(timer == 1)
		{
			scr.stageExecute(sno, SCRIPT_CON_QUIT);
			SE::push(SE_BOSS_TIMEUP);
			if(remain)
			{
				en[ENEMY_MAINBOSSINDEX].defrate = 1.0f;
				en[ENEMY_MAINBOSSINDEX].alpha = 0x7f;
			}
			if(!(spellflag & BISF_WAIT))
				failed = true;
			if(!failed && !(spellflag & BISF_NOTSPELL) && (spellflag & BISF_WAIT))
			{
				Player::p.getspell[Player::p.ncGet] = mp.scene;
				Player::p.ncGet++;
			}
			if(spellflag & BISF_NOTSPELL)
			{
				timer = 100;
			}
			Bullet::IzeBuild(BULLETIZE_FADEOUT, en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y);
			if((spellflag & BISF_WAIT) || !remain)
			{
				en[ENEMY_MAINBOSSINDEX].life = -1;
			}

			if(turntoscene - SCLEAR > S1 && !mp.spellmode)
			{
				allover = true;
			}
		}
		if(timer == 160)
		{
			quit();
			return true;
		}
		if(en[ENEMY_MAINBOSSINDEX].timer == 16 && en[ENEMY_MAINBOSSINDEX].life < 0 && !mp.spellmode && remain)
		{
			en[ENEMY_MAINBOSSINDEX].exist = true;
			en[ENEMY_MAINBOSSINDEX].fadeout = false;
			en[ENEMY_MAINBOSSINDEX].life = 0;
			en[ENEMY_MAINBOSSINDEX].timer = 0;
			en[ENEMY_MAINBOSSINDEX].alpha = 0xff;
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
			en[ENEMY_MAINBOSSINDEX].life = -1;
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
			float tx = en[ENEMY_MAINBOSSINDEX].x + tr * cost(tangle);
			float ty = en[ENEMY_MAINBOSSINDEX].y + tr * sint(tangle);

			fdisp.BossMoveItemEffect(tx, ty);
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
