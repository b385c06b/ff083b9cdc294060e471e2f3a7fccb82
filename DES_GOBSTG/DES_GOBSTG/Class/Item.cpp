#include "../Header/Item.h"
#include "../Header/Player.h"
#include "../Header/Bullet.h"
#include "../Header/SE.h"
#include "../Header/SpriteItemManager.h"
#include "../Header/FrontDisplayName.h"
#include "../Header/Process.h"
#include "../Header/FrontDisplay.h"

hgeSprite * Item::spItem[ITEMSPRITEMAX];

VectorList<Item> Item::mi;
VectorList<infoFont>Item::infofont;

#define _ITEM_GETR				32
#define _ITEM_DRAINY			PL_ITEMDRAINY
#define _ITEM_DRAINDELAY		24
#define _ITEM_DRAINFASTSPEED	8.8f
#define _ITEM_DRAINSLOWSPEED	5.0f
#define _ITEM_FAITHUPSPEED		-1.0f
#define _ITEM_DROPSPEEDMAX		2.8f
#define _ITEM_DROPSPEEDACC		0.1f
#define _ITEM_RETHROWSPEED		-3.0f

Item::Item()
{
}

Item::~Item()
{
}

void Item::Init()
{
	Release();
	mi.init(ITEMMAX);
	infofont.init(ITEMINFOFONTMAX);
	int tidx = SpriteItemManager::GetIndexByName(SI_ITEM_POWER);
	for(int i=0;i<ITEMTYPEMAX;i++)
	{
		spItem[i] = SpriteItemManager::CreateSprite(tidx+i);
	}
	tidx = SpriteItemManager::GetIndexByName(SI_ITEM_POWER_OUT);
	for(int i=0;i<ITEMTYPEMAX;i++)
	{
		spItem[i+ITEMTYPEMAX] = SpriteItemManager::CreateSprite(tidx+i);
	}
}

void Item::ClearAll()
{
	mi.clear_item();
	infofont.clear_item();
}

void Item::valueSet(WORD type, float _x, float _y, bool _bDrained, int _angle, float _speed)
{
	ID			=	type;
	x			=	_x;
	y			=	_y;
	bDrained	=	_bDrained;

	bFast	=	true;

	timer	=	0;
	speed	=	_speed;
	angle	=	_angle;
	headangle =	0;
	hscale	=	1.0f;
	vscale	=	1.0f;
	alpha	=	0xff;
	exist	=	true;
}

void Item::Build(WORD type, float _x, float _y, bool _bDrained /* = false */, int _angle, float _speed)
{
	mi.push_back()->valueSet(type, _x, _y, _bDrained, _angle, _speed);
}

void Item::ChangeItemID(WORD oriID, WORD toID)
{
	DWORD nowindex = mi.index;
	DWORD i = 0;
	DWORD size = mi.getSize();
	for (mi.toBegin(); i<size; mi.toNext(), i++)
	{
		if (!mi.isValid())
		{
			continue;
		}
		if ((*mi).exist)
		{
			if ((*mi).ID == oriID)
			{
				(*mi).valueSet(toID, (*mi).x, (*mi).y);
			}
		}
	}
	mi.index = nowindex;
}

void Item::Release()
{
	for(int i=0;i<ITEMSPRITEMAX;i++)
	{
		if(spItem[i])
		{
			SpriteItemManager::FreeSprite(&spItem[i]);
		}
	}
	mi.clear();
	infofont.clear();
}

void Item::RenderAll()
{
	if (mi.getSize())
	{
		DWORD i = 0;
		DWORD size = mi.getSize();
		for (mi.toBegin(); i<size; mi.toNext(), i++)
		{
			if (mi.isValid())
			{
				(*mi).Render();
			}
		}
	}
	if (infofont.getSize())
	{
		DWORD i = 0;
		DWORD size = infofont.getSize();
		for (infofont.toBegin(); i<size; infofont.toNext(), i++)
		{
			if (!infofont.isValid())
			{
				continue;
			}
			infoFont * _i = &(*(infofont));
			if(Process::mp.state != STATE_PAUSE)
				_i->timer++;
			FrontDisplay::fdisp.ItemInfoDisplay(_i);
		}
	}
}

void Item::Render()
{
	if(y < 0)
	{
		if (spItem[ID + ITEMTYPEMAX])
		{
			spItem[ID + ITEMTYPEMAX]->SetColor((BYTE)((int)y + 0xff)<<24 | 0xffffff);
			SpriteItemManager::RenderSprite(spItem[ID + ITEMTYPEMAX], x, 24);
		}
	}
	if (spItem[ID])
	{
		SpriteItemManager::RenderSpriteEx(spItem[ID], x, y, ARC(headangle));
	}
}

void Item::drainAll()
{
	if (mi.getSize())
	{
		DWORD _index = mi.index;

		DWORD i = 0;
		DWORD size = mi.getSize();
		for (mi.toBegin(); i<size; mi.toNext(), i++)
		{
			if (!mi.isValid())
			{
				continue;
			}
			Item * _i = &(*mi);
			if (_i->exist)
			{
				_i->bDrained = true;
			}
		}
		mi.index = _index;
	}
}

void Item::undrainAll()
{
	if (mi.getSize())
	{
		DWORD _index = mi.index;

		DWORD i = 0;
		DWORD size = mi.getSize();
		for (mi.toBegin(); i<size; mi.toNext(), i++)
		{
			if (!mi.isValid())
			{
				continue;
			}
			Item * _i = &(*mi);
			if (_i->exist && _i->bDrained)
			{
				_i->bDrained = false;
				_i->bFast = true;
				_i->speed = _ITEM_RETHROWSPEED;
				_i->angle = 9000;
			}
		}
		mi.index = _index;
	}
}

void Item::Action()
{
	if (mi.getSize())
	{
		DWORD stopflag = Process::mp.GetStopFlag();
		bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_ITEM);
		if (!binstop)
		{
			DWORD i = 0;
			DWORD size = mi.getSize();
			for (mi.toBegin(); i<size; mi.toNext(), i++)
			{
				if (!mi.isValid())
				{
					continue;
				}
				if ((*mi).exist)
				{
					(*mi).action();
				}
				else
				{
					mi.pop();
				}
			}
		}
	}
}

void Item::action()
{
	if(!bDrained && !(Player::p.flag & PLAYER_COLLAPSE || Player::p.flag & PLAYER_SHOT))
	{
		float rdrain = (Player::p.bSlow) ? 64 : 48;
		if (checkCollisionSquare(Player::p.x, Player::p.y, rdrain))
		{
			bDrained = true;
			bFast = false;
		}
		if(Player::p.y < _ITEM_DRAINY || Player::p.fPoprate >= 1.0f)
		{
			bDrained = true;
		}
	}
	if(bDrained)
	{
		if(timer > _ITEM_DRAINDELAY)
		{
			if(bFast)
				speed = _ITEM_DRAINFASTSPEED;
			else
				speed = _ITEM_DRAINSLOWSPEED;
			float dist = DIST(x, y, Player::p.x, Player::p.y);
			x += speed * (Player::p.x - x) / dist;
			y += speed * (Player::p.y - y) / dist;
		}
		else
		{
			speed = _ITEM_FAITHUPSPEED;
			y += speed;
		}
	}
	else if(speed > 0)
	{
		angle = 9000;
	}

	if (speed > 0)
	{
		headangle = 0;
	}
	else
	{
		headangle += SIGN((int)mi.index) * 600 * speed;
	}

	timer++;
	if(speed < _ITEM_DROPSPEEDMAX)
		speed += _ITEM_DROPSPEEDACC;
	if(!bDrained)
	{
		if(angle != 9000)
		{
			x += speed * cost(angle);
			y += speed * sint(angle);
		}
		else
			y += speed;
	}

	if (checkCollisionSquare(Player::p.x, Player::p.y, _ITEM_GETR)
		&& !(Player::p.flag & PLAYER_COLLAPSE)
		&& !((ID == ITEM_SMALLFAITH || ID == ITEM_FAITH) && timer <= _ITEM_DRAINDELAY))
	{
		DWORD score;

		if(ID != ITEM_BOMB && ID != ITEM_EXTEND)
			SE::push(SE_ITEM_GET, x);
		else
			SE::push(SE_ITEM_EXTEND, x);

		score = Player::p.getItemBonus(ID);

		exist = false;
		Player::p.nScore += score;

		struct infoFont info;
		hge->Math_itoa(score, info.cScore);

		info.timer = 0;
		info.x = x;
		info.y = y;
		if(ID == ITEM_POINT && (Player::p.fPoprate) >= 1.0f)
			info.yellow = true;
		else
			info.yellow = false;
		infofont.push_back(info);
	}
	if(y > M_DELETECLIENT_BOTTOM)
		exist = false;
}