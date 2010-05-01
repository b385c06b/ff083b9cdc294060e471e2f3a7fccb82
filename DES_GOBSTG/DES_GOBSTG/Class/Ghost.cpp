#include "../Header/Ghost.h"
#include "../Header/Player.h"
#include "../Header/Enemy.h"
#include "../Header/Item.h"
#include "../Header/SE.h"
#include "../Header/Bullet.h"
#include "../Header/Scripter.h"
#include "../Header/Chat.h"
#include "../Header/BossInfo.h"
#include "../Header/Main.h"
#include "../Header/EffectSysIDDefine.h"
#include "../Header/Process.h"

Ghost Ghost::gh[GHOSTMAX];

WORD Ghost::index;

#define PB_SHOOTGHOST_ENEMYCOST 0.25f

Ghost::Ghost()
{
	able			= false;
	effghost.exist	= false;
	gID				= 0;
}

Ghost::~Ghost()
{
}

bool Ghost::Build(WORD _gID, BYTE _tarID, BYTE _belong, float x, float y, int angle, float speed, BYTE type, float life, int ac)
{
	bool rv = false;
	for(int i=0;i<GHOSTMAX;i++)
	{
		index++;
		if(index == GHOSTMAX)
		{
			index = 0;
		}
		if(!gh[index].exist)
		{
			rv = true;
			break;
		}
	}
	if(!rv)
		return false;
	gh[index].gID = _gID;
	gh[index].valueSet(index, x, y, angle, speed, type, life, ac);
	gh[index].belong = _belong;
	gh[index].tarID = _tarID;

	return true;
}

void Ghost::Action()
{
	for(int i=0;i<GHOSTMAX;i++)
	{
		if(gh[i].exist)
		{
			Process::mp.objcount ++;

			DWORD stopflag = Process::mp.GetStopFlag();
			bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_GHOST);
			if (!binstop)
			{
				gh[i].action();
			}
			else
			{
				gh[i].actionInStop();
			}
		}
	}
}

void Ghost::ClearAll()
{
	for (int i=0; i<GHOSTMAX; i++)
	{
		gh[i].Clear();
	}
	index = 0;
}

void Ghost::Clear()
{
	exist = false;
	able = false;
	timer = 0;
}

void Ghost::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, float _life, int _ac)
{
	ID		=	_ID;
	x		=	_x;
	y		=	_y;
	type	=	_type;

	SetValue(_angle, _speed, _ac);
	SetLife(_life, _life);

	tarID	=	0xff;

	timer	=	0;
	cenx	=	x;
	ceny	=	y;
	exist	=	true;
	fadeout	=	false;
	able	=	true;
	damage	=	false;
	half	=	false;
	gave	=	false;
	lastborderstate = false;
	hscale	=	1.0f;
	vscale	=	1.0f;
	headangle =	0;
	alpha	=	0xff;
	belong	=	0xff;

	SetAim(0, 0);

	effghost.valueSet(EFF_GH_TYPEBEGIN + type, *this);

	effghost.Fire();
}

void Ghost::CostLife(float power)
{
	life -= power;
	if (life < 0)
	{
		life = maxlife / 4;
	}
	damage = true;
	if (belong < ENEMYMAX)
	{
		Enemy::en[belong].CostLife(power * PB_SHOOTGHOST_ENEMYCOST);
	}
}

void Ghost::valueSet(WORD ID, BYTE _belong, int angle, float speed, BYTE type, float life, int ac)
{
	valueSet(ID, Enemy::en[_belong].x, Enemy::en[_belong].y,angle, speed, type, life, ac);
	belong = _belong;
}

void Ghost::RenderAll()
{
	for(int i=0; i<GHOSTMAX; i++)
	{
		if (gh[i].exist)
		{
			gh[i].Render();
		}
	}
}

void Ghost::Render()
{
	effghost.Render();
}

void Ghost::actionInStop()
{
	if (!fadeout && !half && Player::p.bBorder)
	{
		DoShot();
	}
}

void Ghost::DoShot()
{
	if (PlayerBullet::pb.getSize())
	{
		DWORD i = 0;
		DWORD size = PlayerBullet::pb.getSize();
		for (PlayerBullet::pb.toBegin(); i<size; PlayerBullet::pb.toNext(), i++)
		{
			if (PlayerBullet::pb.isValid() && (*PlayerBullet::pb).able)
			{
				if ((*PlayerBullet::pb).isInRange(x, y, 32))
				{
					CostLife((*PlayerBullet::pb).power);
				}
			}
		}
	}
	if (Enemy::dmgz.getSize())
	{
		DWORD i = 0;
		DWORD size = Enemy::dmgz.getSize();
		for (Enemy::dmgz.toBegin(); i<size; Enemy::dmgz.toNext(), i++)
		{
			if (Enemy::dmgz.isValid())
			{
				DamageZone * tdmg = &(*(Enemy::dmgz));
				if (checkCollisionCircle(tdmg->x, tdmg->y, tdmg->r))
				{
					CostLife(tdmg->power);
					Player::p.DoPlayerBulletHit();
				}
			}
		}
	}
}

void Ghost::SetAim(float aimx, float aimy)
{
	aim.x = aimx;
	aim.y = aimy;
}

void Ghost::SetValue(int _angle, float _speed, int _ac)
{
	angle = _angle;
	speed = _speed;
	ac = _ac;
}

void Ghost::SetLife(float _life, float _maxlife)
{
	life = _life;
	maxlife = _maxlife;
	if (maxlife < life)
	{
		maxlife = life;
	}
}

void Ghost::ChangeGID(WORD _gID)
{
	gID = _gID;
	timer = 0;
}

void Ghost::action()
{
	timer++;

	if(life < maxlife / 2 && life > 0)
	{
		if (!half)
		{
			SE::push(SE_GHOST_HALF, x);
			half = true;
		}
	}

	if(!fadeout && !gave && half && (Player::p.bBorder))
	{
		Item::Build(ITEM_FAITH, x, y, true);

		gave = true;
	}

	if (Player::p.bBorder ^ lastborderstate)
	{
		effghost.valueSet(EFF_GH_TYPEBEGIN + type + (Player::p.bBorder?1:0), *this);
		effghost.Fire();
		lastborderstate ^= true;
	}

	if(!fadeout)
	{
		if(timer == 1)
		{
			SE::push(SE_GHOST_MERGE, x);
		}

		if(Chat::chatitem.chatting || BossInfo::bossinfo.bossout())
		{
			life = 0;
			fadeout = true;
			timer = 0;
		}

		if(gID && !Scripter::stopEdefScript)
		{
			WORD tindex = index;
			index = ID;
			Scripter::scr.edefExecute(gID, timer);
			index = tindex;
		}

		if(Player::p.bBorder)
		{
			if (checkCollisionSquare(Player::p.x, Player::p.y, GHOST_COLLISIONR))
			{
				Player::p.DoShot();
			}
			DoShot();
		}

		if(belong != 0xff)
		{
			if(Enemy::en[belong].exist)
			{
				cenx = Enemy::en[belong].x;
				ceny = Enemy::en[belong].y;
			}
			else
			{
				if(Enemy::en[belong].life >= 0)
				{
					life = 0;
				}
				timer = 0;
				fadeout = true;
			}
		}

		if(life < 0)
		{
			WORD tindex = index;
			index = ID;
			Scripter::scr.edefExecute(gID, SCRIPT_CON_POST);
			index = tindex;

			if (life < 0)
			{
				Player::p.GetScoreLife(maxlife, false);

				fadeout = true;
				timer = 0;
			}
		}
		else if(ac && belong != 0xff)
		{
			angle += ANGLE(speed / ac);

			float tr2 = DIST2(x, y, cenx, ceny);
			float trac2 = ac * ac;
			float tac = fabsf(ac);
			float trac = tac;
			if(tr2 < trac2)
			{
				trac = sqrtf(tr2) + fabsf(speed);
				if(trac > tac)
					trac = tac;
			}
			else if(tr2 > trac2)
			{
				trac = sqrtf(tr2) - fabsf(speed);
				if(trac < tac)
					trac = tac;
			}
			if(ac < 0)
				trac = -trac;

			x = cenx + trac * cost(angle);
			y = ceny + trac * sint(angle);
		}
		else
		{
			x += speed * cost(angle);
			y += speed * sint(angle);
		}

		if(tarID != 0xff)
		{
			Target::SetValue(tarID, x, y);
		}

		effghost.MoveTo(x, y);
		effghost.action();

		if(belong == 0xff)
		{
			if(timer > (DWORD)ac)
			{
				life = 0;
				fadeout = true;
				timer = 0;
			}
		}
	}
	else
	{
		effghost.action();
		if(timer == 32)
		{
			effghost.Stop();
			exist = false;
		}
	}

	damage = false;
	able = exist && !fadeout;
}