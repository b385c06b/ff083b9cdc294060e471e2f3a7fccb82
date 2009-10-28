#include "SpriteItemManager.h"
#include "BResource.h"

HTEXTURE * SpriteItemManager::tex;
int SpriteItemManager::digituiIndex = 0;
int SpriteItemManager::faceIndexEnemy = 0;
int SpriteItemManager::nameIndexEnemy = 0;
int SpriteItemManager::faceIndexPlayer = 0;
int SpriteItemManager::nameIndexPlayer = 0;
int SpriteItemManager::yesIndex = 0;
int SpriteItemManager::noIndex = 0;
int SpriteItemManager::cancelIndex = 0;
int SpriteItemManager::confirmIndex = 0;

SpriteItemManager::SpriteItemManager()
{

}

SpriteItemManager::~SpriteItemManager()
{

}

void SpriteItemManager::Init(HTEXTURE * _tex)
{
	tex = _tex;
}

int SpriteItemManager::GetIndexByName(const char * spritename)
{
	if (!spritename)
	{
		return -1;
	}
	if (!strlen(spritename))
	{
		return -1;
	}
	for (int i=0; i<SPRITEITEMMAX; i++)
	{
		if (!strcmp(spritename, res.spritedata[i].spritename))
		{
			return i;
			break;
		}
	}
	return -1;
}

spriteData * SpriteItemManager::CastSprite(int index)
{
	if (index >= 0 && index < SPRITEITEMMAX)
	{
		return &(res.spritedata[index]);
	}
	return NULL;
}

bool SpriteItemManager::SetSprite(int index, hgeSprite * sprite, HTEXTURE * tex)
{
	if (!sprite)
	{
		return false;
	}
	if (index < 0)
	{
		sprite->SetTextureRect(0, 0, 0, 0);
		return true;
	}
	spriteData * _sd = CastSprite(index);
	if (!_sd)
	{
		return false;
	}
	sprite->SetTexture(tex[_sd->tex]);
	sprite->SetTextureRect(_sd->tex_x, _sd->tex_y, 
		_sd->tex_w < 0 ? hge->Texture_GetWidth(tex[_sd->tex])-_sd->tex_x : _sd->tex_w, 
		_sd->tex_h < 0 ? hge->Texture_GetHeight(tex[_sd->tex])-_sd->tex_y : _sd->tex_h);
	return true;
}

hgeSprite * SpriteItemManager::CreateSprite(int index)
{
	hgeSprite * sprite;
	if (index < 0)
	{
		sprite = new hgeSprite(tex[TEX_WHITE], 0, 0, 0, 0);
	}
	else
	{
		sprite = new hgeSprite(tex[res.spritedata[index].tex], res.spritedata[index].tex_x, res.spritedata[index].tex_y, res.spritedata[index].tex_w, res.spritedata[index].tex_h);
	}
	return sprite;
}

bool SpriteItemManager::CreateSprite(int index, hgeSprite * sprite)
{
	sprite = CreateSprite(index);
	if (sprite)
	{
		return true;
	}
	return false;
}

hgeSprite * SpriteItemManager::CreateSpriteByName(const char * spritename)
{
	int index = GetIndexByName(spritename);
	if (index >= 0)
	{
		return CreateSprite(index);
	}
	return NULL;
}

bool SpriteItemManager::CreateSpriteByName(const char * spritename, hgeSprite * sprite)
{
	sprite = CreateSpriteByName(spritename);
	if (sprite)
	{
		return true;
	}
	return false;
}

void SpriteItemManager::FreeSprite(hgeSprite * sprite)
{
	if (sprite)
	{
		free(sprite);
		sprite = NULL;
	}
}

bool SpriteItemManager::ptFace(int index, hgeSprite * sprite, bool enemy)
{
	if (!sprite)
	{
		return false;
	}
	if (index < 0)
	{
		sprite->SetTextureRect(0, 0, 0, 0);
		return true;
	}
	spriteData * tsd = &(res.spritedata[index+(enemy?faceIndexEnemy:faceIndexPlayer)]);
	sprite->SetTexture(tex[tsd->tex]);
	sprite->SetTextureRect(tsd->tex_x, tsd->tex_y, tsd->tex_w, tsd->tex_h);
	return true;
}

bool SpriteItemManager::ptName(int index, hgeSprite * sprite, bool enemy)
{
	if (!sprite)
	{
		return false;
	}
	if (index < 0)
	{
		sprite->SetTextureRect(0, 0, 0, 0);
		return true;
	}
	spriteData * tsd = &(res.spritedata[index+(enemy?nameIndexEnemy:nameIndexPlayer)]);
	sprite->SetTexture(tex[tsd->tex]);
	sprite->SetTextureRect(tsd->tex_x, tsd->tex_y, tsd->tex_w, tsd->tex_h);
	return true;
}