#include "../Header/Player.h"
#include "../Header/PlayerBullet.h"
#include "../Header/SE.h"
#include "../Header/Enemy.h"
#include "../Header/Chat.h"
#include "../Header/Process.h"
#include "../Header/BResource.h"

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
