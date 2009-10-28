#include "Player.h"
#include "PlayerBullet.h"
#include "SE.h"
#include "Enemy.h"
#include "Chat.h"
#include "Process.h"
#include "BResource.h"

void Player::_Shoot()
{
	playershootData * item;
	for (int i=0; i<PLAYERSHOOTTYPEMAX; i++)
	{
		item = &(res.playershootdata[i]);
		if (item->timeMod)
		{
			if (item->userID == nowID && item->timeMod && (item->timeMod == 1 || !(shoottimer % item->timeMod)))
			{
				PlayerBullet::Build(i);
			}
		}
	}
}

bool Player::Shoot()
{
	if(flag & PLAYER_SHOT || Chat::chatting)
	{
		shoottimer = 0;
		return true;
	}

	_Shoot();
	shoottimer++;
//
	if(!hge->Input_GetDIKey(KS_FIRE_MP) && shoottimer % 24 == 0)
	{
		shoottimer = 0;
		return true;
	}
	return false;
}
