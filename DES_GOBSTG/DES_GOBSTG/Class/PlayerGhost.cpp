#include "PlayerGhost.h"
#include "Player.h"
#include "Main.h"
#include "Process.h"
#include "SpriteItemManager.h"
#include "BResource.h"

#define _PLAYERGHOST_ADJUSTTIME	0x10
#define _PLAYERGHOST_LASTINDEX	1

#define _PLAYERGHOST_TRACEEDGEANGLE	200

PlayerGhost::PlayerGhost()
{
	sprite	= NULL;
}

PlayerGhost::~PlayerGhost()
{
	if(sprite)
		SpriteItemManager::FreeSprite(sprite);
	sprite = NULL;
}

void PlayerGhost::ResetValue(bool move/* =false */)
{
	int tid = Player::p.nowID;
	tid *= PLAYERGHOSTMAX * 2;
	if (Player::p.bSlow)
	{
		tid += PLAYERGHOSTMAX;
	}
	for (int i=0; i<PLAYERGHOSTMAX; i++)
	{
		Player::p.pg[i].valueSet(tid+i, move);
	}
}

void PlayerGhost::valueSet(WORD _ID, bool move)
{
	ID		=	_ID;

	timer	=	0;
	exist	=	true;
	headangle =	0;
	alpha	=	0xff;
	diffuse	=	0xffffff;
	scale	=	1.0f;
	yshake	=	0.0f;
	lastchasing	=	false;

	playerghostData * _pgd = &(res.playerghostdata[ID]);
	
	speed	=	_pgd->speed;
	flag	=	_pgd->flag;
	angle	=	_pgd->startangle;
	if (!(flag & PGFLAG_REMAINSHOOTANGLE))
	{
		shootangle	=	_pgd->shootangle;
	}

	if (move)
	{
		x = PL_MERGEPOS_X;
		y = PL_MERGEPOS_Y;
	}

	if(!sprite)
	{
		sprite = SpriteItemManager::CreateSprite(_pgd->siID);
	}
	else
	{
		SpriteItemManager::SetSprite(_pgd->siID, sprite, mp.tex);
	}
}

void PlayerGhost::Render()
{
	sprite->SetColor((alpha<<24)|diffuse);
	sprite->RenderEx(x, y, ARC(headangle), scale);
}

void PlayerGhost::AntiShooter(float aimx, float aimy)
{
	while(shootangle >= 27000)
		shootangle -=36000;
	while(shootangle < -9000)
		shootangle += 36000;
	if(aimx == x)
	{
		if(hge->Input_GetDIKey(KS_UP_MP) && shootangle != 9000)
		{
			if(shootangle > 9000)
			{
				shootangle -= _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle < 9000)
					shootangle = 9000;
			}
			else if(shootangle < 9000)
			{
				shootangle += _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle > 9000)
					shootangle = 9000;
			}
		}
		if(hge->Input_GetDIKey(KS_DOWN_MP) && shootangle != -9000)
		{
			if(shootangle < 9000)
			{
				shootangle -= _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle < -9000)
					shootangle = -9000;
			}
			else if(shootangle > 9000)
			{
				shootangle += _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle > 27000)
					shootangle = -9000;
			}
		}
	}
	while(shootangle >= 18000)
		shootangle -= 36000;
	while(shootangle < -18000)
		shootangle += 36000;
	if(aimy == y)
	{
		if(hge->Input_GetDIKey(KS_LEFT_MP) && shootangle != 0)
		{
			if(shootangle < 0)
			{
				shootangle += _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle > 0)
					shootangle = 0;
			}
			else if(shootangle > 0)
			{
				shootangle -= _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle < 0)
					shootangle = 0;
			}
		}
		if(hge->Input_GetDIKey(KS_RIGHT_MP) && shootangle != 18000)
		{
			if(shootangle > 0)
			{
				shootangle += _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle > 18000)
					shootangle = 18000;
			}
			else if(shootangle < 0)
			{
				shootangle -= _PLAYERGHOST_TRACEEDGEANGLE;
				if(shootangle < -18000)
					shootangle = 18000;
			}
		}
	}
	if((aimx != x || aimy != y) || timer < _PLAYERGHOST_ADJUSTTIME)
		shootangle = aMainAngle(Player::p, 18000);
}

void PlayerGhost::action()
{
	timer++;

	float aimx = x;
	float aimy = y;

	int chasetime = 2;
	bool chasing = false;
	bool shooting = Player::p.flag & PLAYER_SHOOT;

	playerghostData * _pgd = &(res.playerghostdata[ID]);

	if (flag & PGFLAG_TRACE)
	{
		int _tindex = (ID%PLAYERGHOSTMAX+1)*8-1;
		aimx = Player::p.lastmx[_tindex];
		aimy = Player::p.lastmy[_tindex];
	}

	if (flag & PGFLAG_CHASE)
	{
		chasetime = 8;
		BObject * _tobj;
		if (PlayerBullet::GetLockAim(&_tobj) && shooting)
		{
			aimx = _tobj->x - _pgd->xadj;
			aimy = _tobj->y - _pgd->yadj;
			shootangle = _tobj->aMainAngle(x, y, -9000);
			chasing = true;
		}
		else
		{
			aimx = Player::p.lastx[_PLAYERGHOST_LASTINDEX] + _pgd->xadj;
			aimy = Player::p.lasty[_PLAYERGHOST_LASTINDEX] + _pgd->yadj;
		}
	}
	if (chasing != lastchasing)
	{
		timer = 0;
	}

	if (flag & PGFLAG_ABSSTAY)
	{
		aimx = _pgd->xadj;
		aimy = _pgd->yadj;
	}

	if (!(flag & PGFLAG_POSTMASK))
	{
		aimx = Player::p.lastx[_PLAYERGHOST_LASTINDEX];
		aimy = Player::p.lasty[_PLAYERGHOST_LASTINDEX];
	}

	if (_pgd->mover)
	{
		int tangle = _pgd->speed * 100 * timer + _pgd->startangle;
		if (flag & PGFLAG_NEGATIVE)
		{
			tangle = 36000 - tangle;
		}
		float txoffset = cost(tangle) * _pgd->mover;
		float tyoffset = sint(tangle) * _pgd->mover;
		aimx += txoffset;
		aimy += tyoffset;
	}

	if (!(flag & PGFLAG_ABSSTAY) && !(flag & PGFLAG_STAY) && !(flag & PGFLAG_CHASE))
	{
		aimx += _pgd->xadj;
		aimy += _pgd->yadj;
	}
	if(flag & PGFLAG_ANTISHOOTER)
	{
		if (flag & PGFLAG_STAY)
		{
			shootangle = aMainAngle(Player::p, 18000);
		}
		else
		{
			AntiShooter(aimx, aimy);
		}
	}

	if (flag & PGFLAG_ROLL)
	{
		int troll = (ROLL(timer, 72)-36)*_pgd->shootangle;
		if (flag & PGFLAG_NEGATIVE)
		{
			shootangle = troll;
		}
		else
		{
			shootangle = troll;
		}
	}

	if (timer < _PLAYERGHOST_ADJUSTTIME)
	{
		chasetime = _PLAYERGHOST_ADJUSTTIME - timer;
	}

	if (aimx != x || aimy != y)
	{
		chaseAim(aimx, aimy, chasetime);
		x += speed * cost(angle);
		y += speed * sint(angle);
	}
	if (flag & PGFLAG_STANDSHAKE)
	{
		yshake = sint(timer*512)*4.8f-2.4f;
	}
	else
	{
		yshake = 0;
	}
	if (flag & PGFLAG_SCALESHAKE)
	{
		scale = ROLL(timer, 10)/20.0f+0.8f;
	}

	if (flag & PGFLAG_TURN)
	{
		if (flag & PGFLAG_NEGATIVE)
		{
			headangle = -1;
		}
		else
		{
			headangle = 1;
		}
		headangle *= timer * 1000;
	}
	else
	{
		headangle = 0;
	}

	SpriteItemManager::SetSprite(_pgd->siID, sprite, mp.tex);
	if (flag & PGFLAG_SYNCPLAYER)
	{
		float tex_x, tex_y, tex_w, tex_h;
		Player::p.sprite->GetTextureRect(&tex_x, &tex_y, &tex_w, &tex_h);
		if (flag & PGFLAG_CHASE)
		{
			if (chasing)
			{
				headangle = -timer * 3000;
				HTEXTURE ttex = sprite->GetTexture();
				tex_x = hge->Texture_GetWidth(ttex)-tex_w;
				tex_y = hge->Texture_GetHeight(ttex)-tex_h;
			}
		}
		sprite->SetTextureRect(tex_x, tex_y, tex_w, tex_h);
		bool bxflip, byflip;
		Player::p.sprite->GetFlip(&bxflip, &byflip);
		sprite->SetFlip(bxflip, byflip);
	}
	lastchasing = chasing;
	sprite->SetBlendMode(_pgd->blend);
	float hotx = sprite->GetWidth()/2;
	float hoty = sprite->GetHeight()/2;
	if (headangle && yshake)
	{
		hotx += yshake * sint(headangle);
		hoty += yshake * cost(headangle);
	}
	else
	{
		hoty += yshake;
	}
	sprite->SetHotSpot(hotx, hoty);
}
