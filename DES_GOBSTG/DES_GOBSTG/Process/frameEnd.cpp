#include "../Header/processPrep.h"

void Process::frameEnd()
{
	objcount = 0;
	if(active)
	{
		framecounter++;

		WorldShake();
		Player::Action();
		Enemy::Action();
		Ghost::Action();

		Enemy::dmgz.clear_item();

		Bullet::Action();
		Beam::Action();
		PlayerBullet::Action();
		Item::Action();

		Scripter::stopEdefScript = false;
		if(BossInfo::bossinfo.flag)
		{
			if(BossInfo::bossinfo.action())
			{
				gametime = 0;
				if(!spellmode)
				{
					scene = BossInfo::bossinfo.turntoscene;
				}
				else
					Player::p.exist = false;
			}
			if(BossInfo::bossinfo.bossout())
				Scripter::stopEdefScript = true;
		}
	}
	if(active || !Player::p.exist && state != STATE_CONTINUE)
	{
		BGLayer::Action(active);
		Effectsys::Action();
	}
	BGLayer::ActionSpecial();
	for(list<Selector>::iterator i = Selector::sel.begin();i != Selector::sel.end(); i++)
	{
		i->action();
	}
	Fontsys::changeID = -1;
	for(list<InfoSelect>::iterator i=InfoSelect::infoselect.begin();i!=InfoSelect::infoselect.end();i++)
	{
		if(!InfoSelect::complete)
		{
			i->action();
			if (Fontsys::changeID < i->ID)
			{
				Fontsys::changeID = i->ID;
			}
		}
		else
		{
			InfoSelect::Clear();
			break;
		}
	}
	if (Player::p.nHiScore < Player::p.nScore)
	{
		Player::p.nHiScore = Player::p.nScore;
	}

	SE::play();
	if (active)
	{
		for (int i=0; i<FRAME_STOPINFOMAX; i++)
		{
			if (stopflag[i])
			{
				stoptimer[i]--;
				if (stoptimer[i] == 0)
				{
					stopflag[i] = 0;
				}
			}
		}
	}
	active = false;
}