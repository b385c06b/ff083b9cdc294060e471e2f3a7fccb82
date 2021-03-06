#include "../Header/SpriteItemManager.h"
#include "../Header/BResource.h"

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

void SpriteItemManager::Init()
{
}

void SpriteItemManager::Release()
{
}

HTEXTURE SpriteItemManager::GetTexture(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return NULL;
	}
	HTEXTURE tex(BResource::bres.spritedata[index].tex, NULL);
	return tex;
}

float SpriteItemManager::GetTexX(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::bres.spritedata[index].tex_x;
}

float SpriteItemManager::GetTexY(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::bres.spritedata[index].tex_y;
}

float SpriteItemManager::GetTexW(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::bres.spritedata[index].tex_w;
}

float SpriteItemManager::GetTexH(int index)
{
	if (index < 0 || index >= SPRITEITEMMAX)
	{
		return 0;
	}
	return BResource::bres.spritedata[index].tex_h;
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
		if (!strcmp(spritename, BResource::bres.spritedata[i].spritename))
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
		return &(BResource::bres.spritedata[index]);
	}
	return NULL;
}

bool SpriteItemManager::SetSprite(int index, hgeSprite * sprite)
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
	SetSpriteData(sprite, BResource::bres.tex[_sd->tex], _sd->tex_x, _sd->tex_y, 
		_sd->tex_w < 0 ? hge->Texture_GetWidth(BResource::bres.tex[_sd->tex])-_sd->tex_x : _sd->tex_w, 
		_sd->tex_h < 0 ? hge->Texture_GetHeight(BResource::bres.tex[_sd->tex])-_sd->tex_y : _sd->tex_h);
	return true;
}

hgeSprite * SpriteItemManager::CreateNullSprite()
{
	hgeSprite * sprite;
	sprite = CreateSprite(nullIndex);
	return sprite;
}

hgeSprite * SpriteItemManager::CreateSprite(int index)
{
	hgeSprite * sprite;
//	sprite = CreateNullSprite();
	sprite = new hgeSprite();
	if (index < 0)
	{
		return sprite;
	}
	HTEXTURE tex(BResource::bres.spritedata[index].tex, NULL);
	SetSpriteData(sprite, tex, BResource::bres.spritedata[index].tex_x, BResource::bres.spritedata[index].tex_y, BResource::bres.spritedata[index].tex_w, BResource::bres.spritedata[index].tex_h);
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
	HTEXTURE tex(BResource::bres.spritedata[index].tex, NULL);
	SetSpriteData(sprite, tex, BResource::bres.spritedata[index].tex_x, BResource::bres.spritedata[index].tex_y, BResource::bres.spritedata[index].tex_w, BResource::bres.spritedata[index].tex_h);
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
	spriteData * tsd = &(BResource::bres.spritedata[index+(enemy?faceIndexEnemy:faceIndexPlayer)]);
	HTEXTURE tex(tsd->tex, NULL);
	sprite->SetTexture(tex);
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
	spriteData * tsd = &(BResource::bres.spritedata[index+(enemy?nameIndexEnemy:nameIndexPlayer)]);
	HTEXTURE tex(tsd->tex, NULL);
	sprite->SetTexture(tex);
	sprite->SetTextureRect(tsd->tex_x, tsd->tex_y, tsd->tex_w, tsd->tex_h);
	return true;
}

bool SpriteItemManager::LoadTextureSetWhenNeeded(HTEXTURE tex)
{
	if (hge->Texture_GetTexture(tex) == NULL)
	{
		BResource::bres.LoadTextureSet(BResource::bres.texturedata[tex.texindex].texset);
		return true;
	}
	return false;
}

void SpriteItemManager::RenderSprite( hgeSprite * sprite, float x, float y )
{
	if (!sprite)
	{
		return;
	}
	LoadTextureSetWhenNeeded(sprite->quad.tex);
	sprite->Render(x, y);
}

void SpriteItemManager::RenderSpriteEx( hgeSprite * sprite, float x, float y, float rot, float hscale/*=1.0f*/, float vscale/*=0.0f*/ )
{
	if (!sprite)
	{
		return;
	}
	LoadTextureSetWhenNeeded(sprite->quad.tex);
	sprite->RenderEx(x, y, rot, hscale, vscale);
}

void SpriteItemManager::FontPrintf( hgeFont * font, float x, float y, int align, const char * str)
{
	if (!font)
	{
		return;
	}
	font->printf(x, y, align, str);
}

void SpriteItemManager::FontPrintfb( hgeFont * font, float x, float y, float w, float h, int align, const char * str)
{
	if (!font)
	{
		return;
	}
	font->printfb(x, y, w, h, align, str);
}

void SpriteItemManager::EffectSystemRender( hgeEffectSystem * eff )
{
	if (!eff)
	{
		return;
	}
	LoadTextureSetWhenNeeded(eff->ebi.tex);
	eff->Render();
}

void SpriteItemManager::RenderQuad( hgeQuad * quad )
{
	if (!quad)
	{
		return;
	}
	LoadTextureSetWhenNeeded(quad->tex);
	hge->Gfx_RenderQuad(quad);
}