#include "PlayerBullet.h"
#include "BResource.h"

#define _PBTEX_PLAYERBEGIN		10

void PlayerBullet::Init(HTEXTURE * _tex)
{
	tex = _tex;
	Release();
	pb.init(PLAYERBULLETMAX);

	playerbulletData * item;
	for (int i=0; i<PLAYERBULLETSPRITEMAX; i++)
	{
		item = &(res.playerbulletdata[i]);
		spPlayerBullet[i] = new hgeSprite(tex[item->tex], item->x, item->y, item->w, item->h);
	}
}
