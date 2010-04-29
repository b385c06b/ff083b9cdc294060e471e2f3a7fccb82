#include "../Header/SpriteItemManager.h"
#include "../Header/BResource.h"

HTEXTURE * SpriteItemManager::tex;
int SpriteItemManager::nullIndex = 0;
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

void SpriteItemManager::Release()
{
}

HTEXTURE SpriteItemManager::GetTexture(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX || !tex)
	{
		return NULL;
	}
	return tex[BResource::res.spritedata[index].tex];
}

float SpriteItemManager::GetTexX(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::res.spritedata[index].tex_x;
}

float SpriteItemManager::GetTexY(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::res.spritedata[index].tex_y;
}

float SpriteItemManager::GetTexW(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::res.spritedata[index].tex_w;
}

float SpriteItemManager::GetTexH(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::res.spritedata[index].tex_h;
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
		if (!strcmp(spritename, BResource::res.spritedata[i].spritename))
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
		return &(BResource::res.spritedata[index]);
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
		SetSpriteTextureRect(sprite, 0, 0, 0, 0);
		return true;
	}
	spriteData * _sd = CastSprite(index);
	if (!_sd)
	{
		return false;
	}
	SetSpriteData(sprite, tex[_sd->tex], _sd->tex_x, _sd->tex_y, 
		_sd->tex_w < 0 ? hge->Texture_GetWidth(tex[_sd->tex])-_sd->tex_x : _sd->tex_w, 
		_sd->tex_h < 0 ? hge->Texture_GetHeight(tex[_sd->tex])-_sd->tex_y : _sd->tex_h);
	return true;
}

hgeSprite * SpriteItemManager::CreateNullSprite()
{
	hgeSprite * sprite;
	if (!tex)
	{
		return NULL;
	}
	sprite = CreateSprite(nullIndex);
	return sprite;
}

hgeSprite * SpriteItemManager::CreateSprite(int index)
{
	hgeSprite * sprite;
	if (!tex)
	{
		return NULL;
	}
//	sprite = CreateNullSprite();
	sprite = new hgeSprite();
	if (index < 0)
	{
		return sprite;
	}
	SetSpriteData(sprite, tex[BResource::res.spritedata[index].tex], BResource::res.spritedata[index].tex_x, BResource::res.spritedata[index].tex_y, BResource::res.spritedata[index].tex_w, BResource::res.spritedata[index].tex_h);
	return sprite;
}

bool SpriteItemManager::SetSpriteData(hgeSprite * sprite, HTEXTURE _tex, float texx, float texy, float texw, float texh, bool flipx/* =false */, bool flipy/* =false */)
{
	if (!sprite)
	{
		return false;
	}
/*
	if (!_tex.tex)
	{
		_tex = tex[TEX_WHITE];
	}*/

	sprite->SetTexture(_tex);
	if (!SetSpriteTextureRect(sprite, texx, texy, texw, texh))
	{
		return false;
	}
	if (!SetSpriteFlip(sprite, flipx, flipy))
	{
		return false;
	}
	return true;
}

bool SpriteItemManager::SetSpriteTextureRect(hgeSprite * sprite, float texx, float texy, float texw, float texh)
{
	if (!sprite)
	{
		return false;
	}
	sprite->SetTextureRect(texx, texy, texw, texh);
	return true;
}

bool SpriteItemManager::SetSpriteHotSpot(hgeSprite * sprite, float hotx, float hoty)
{
	if (!sprite)
	{
		return false;
	}
	sprite->SetHotSpot(hotx, hoty);
	return true;
}

bool SpriteItemManager::SetSpriteFlip(hgeSprite * sprite, bool flipx/* =false */, bool flipy/* =false */)
{
	if (!sprite)
	{
		return false;
	}
	sprite->SetFlip(flipx, flipy);
	return true;
}

bool SpriteItemManager::CreateSprite(int index, hgeSprite ** sprite)
{
	if (!sprite)
	{
		return false;
	}
	*sprite = CreateSprite(index);
	if (*sprite)
	{
		return true;
	}
	return false;
}

bool SpriteItemManager::ChangeSprite(int index, hgeSprite * sprite)
{
	if (!sprite || index < 0)
	{
		return false;
	}
	SetSpriteData(sprite, tex[BResource::res.spritedata[index].tex], BResource::res.spritedata[index].tex_x, BResource::res.spritedata[index].tex_y, BResource::res.spritedata[index].tex_w, BResource::res.spritedata[index].tex_h);
	return true;
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

bool SpriteItemManager::CreateSpriteByName(const char * spritename, hgeSprite ** sprite)
{
	if (!sprite)
	{
		return false;
	}
	*sprite = CreateSpriteByName(spritename);
	if (*sprite)
	{
		return true;
	}
	return false;
}

void SpriteItemManager::FreeSprite(hgeSprite ** sprite)
{
	if (sprite && *sprite)
	{
		free(*sprite);
		*sprite = NULL;
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
	spriteData * tsd = &(BResource::res.spritedata[index+(enemy?faceIndexEnemy:faceIndexPlayer)]);
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
	spriteData * tsd = &(BResource::res.spritedata[index+(enemy?nameIndexEnemy:nameIndexPlayer)]);
	sprite->SetTexture(tex[tsd->tex]);
	sprite->SetTextureRect(tsd->tex_x, tsd->tex_y, tsd->tex_w, tsd->tex_h);
	return true;
}