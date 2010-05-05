#include "../Header/EffectSp.h"
#include "../Header/Player.h"
#include "../Header/Enemy.h"
#include "../Header/SpriteItemManager.h"
#include "../Header/FrontDisplayName.h"

BYTE EffectSp::evtype;

EffectSp::EffectSp()
{
	sprite = NULL;
}

EffectSp::~EffectSp()
{
	SpriteItemManager::FreeSprite(&sprite);
}

void EffectSp::ClearAll()
{
}

void EffectSp::Clear()
{
	exist = false;
	timer = 0;
}

void EffectSp::Render()
{
	if (sprite)
	{
		sprite->SetColor((alpha<<24)|diffuse);
		sprite->SetBlendMode(blend);
		SpriteItemManager::RenderSpriteEx(sprite, x, y, ARC(angle+headangle), hscale, vscale);
	}
}

void EffectSp::valueSet(int _siID, float _x, float _y, int _angle, float _speed, bool _onplayer)
{
	siID		= _siID;
	posSet(_x, _y);
	angle		= _angle;
	speed		= _speed;
	onplayer	= _onplayer;

	timer = 0;
	exist = true;
	headangle = 0;

	if(sprite)
	{
		SpriteItemManager::FreeSprite(&sprite);
	}
	sprite = SpriteItemManager::CreateSprite(siID);

	colorSet(0xffffffff, BLEND_DEFAULT);
	scaleSet(1, 0);
/*
	
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
		}*/
	
}

void EffectSp::posSet(float _x, float _y)
{
	x = _x;
	y = _y;
}

void EffectSp::colorSet(DWORD color, int _blend)
{
	diffuse = color & 0xffffff;
	alpha = GETA(color);
	if (_blend >= 0)
	{
		blend = _blend;
	}
	if (blend < 0)
	{
		blend = BLEND_DEFAULT;
	}
}

void EffectSp::scaleSet(float _hscale, float _vscale/* =0 */)
{
	hscale = _hscale;
	vscale = _vscale;
}

float EffectSp::GetWidth()
{
	if (!sprite)
	{
		return 0;
	}
	return sprite->width;
}

float EffectSp::GetHeight()
{
	if (!sprite)
	{
		return 0;
	}
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
	}
}