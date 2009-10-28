#include "EffectSp.h"
#include "Player.h"
#include "Enemy.h"
#include "SpriteItemManager.h"
#include "FrontDisplayName.h"

BYTE EffectSp::evtype;

EffectSp es[EFFECTSPMAX];

EffectSp::EffectSp()
{
	sprite = NULL;
}

EffectSp::~EffectSp()
{
	if(sprite)
		delete sprite;
	sprite = NULL;
}

void EffectSp::Render()
{
	sprite->SetColor((alpha<<24)|diffuse);
	sprite->RenderEx(x, y, ARC(angle+headangle), hscale, vscale);
}

void EffectSp::valueSet(BYTE _type, float _x, float _y, int _angle, float _speed, bool _onplayer, WORD _ID)
{
	ID			= _ID;
	type		= _type;
	x			= _x;
	y			= _y;
	angle		= _angle;
	speed		= _speed;
	onplayer	= _onplayer;

	timer = 0;
	exist = true;
	hscale = 1;
	vscale = 0;
	headangle = 0;

	if(sprite)
		SpriteItemManager::FreeSprite(sprite);

	colorSet(0xffffff);

	switch(type)
	{
	case EFFECT_PLAYERCHANGE:
		sprite = SpriteItemManager::CreateSpriteByName(SI_PLAYER_SHOTITEM);
		sprite->SetBlendMode(BLEND_ALPHAADD);
		alpha = 0x7f;
		hscale = 3.0f;
		break;
	case EFFECT_PLAYERSHOT:
		sprite = SpriteItemManager::CreateSpriteByName(SI_PLAYER_SHOTITEM);
		alpha = 0xcc;
		hscale = 1.2f;
		break;
	case EFFECT_PLAYERBORDER:
		sprite = SpriteItemManager::CreateSpriteByName(SI_BORDER_CIRCLE);
		alpha = 0xC0;
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERBORDERZONE:
		sprite = SpriteItemManager::CreateSpriteByName(SI_BORDER_CIRCLE);
		alpha = 0x80;
		hscale = 0.0f;
		break;
	case EFFECT_PLAYERPOINT:
		sprite = SpriteItemManager::CreateSpriteByName(SI_PLAYER_POINT);
		alpha = 0xff;
		hscale = 1.0f;
		break;
	case EFFECT_PLAYERCOLLAPSE:
		sprite = SpriteItemManager::CreateSpriteByName(SI_PLAYER_SHOTITEM);
		alpha = 0x7f;
		hscale = 1.0f;
		break;

	case EFFECT_BOMB_CUTIN:
		sprite = SpriteItemManager::CreateSpriteByName(SI_PLAYERFACE_01);
		SpriteItemManager::ptFace(Player::p.nowID, sprite, false);
		hscale = vscale = 1.40625f;
		alpha = 0x7f;
		break;
	}
}

void EffectSp::colorSet(DWORD color)
{
	diffuse = color;
}

float EffectSp::GetWidth()
{
	return sprite->width;
}

float EffectSp::GetHeight()
{
	return sprite->height;
}

void EffectSp::action()
{
	timer++;

	if(onplayer)
	{
		headangle += angle;
		x = Player::p.x;
		y = Player::p.y;
	}
	else
	{
		x += speed * cost(angle);
		y += speed * sint(angle);

		if(type > 0x80)
		{
			if(headangle > 0)
				headangle += ANGLE(speed / 15);
			else
				headangle -= ANGLE(speed / 15);
		}
	}
}