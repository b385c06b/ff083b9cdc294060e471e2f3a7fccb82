#include "processPrep.h"

void Process::frameEnd()
{
	objcount = 0;
	if(active)
	{
		framecounter++;
		float lost = (hge->Timer_GetDelta() - 1/60.0f) * 100 * 60.0f;
		if(lost < 0)
			lost = 0;
		if(lost > 100)
			lost = 100;
		Player::p.lostStack += lost;

		if (!(stopflag & FRAME_STOPFLAG_WORLDSHAKE))
		{
			WorldShake();
		}

		if(Player::p.exist && state != STATE_CLEAR)
		{
			alltime++;
			if (!(stopflag & FRAME_STOPFLAG_PLAYER))
			{
				Player::p.action();
			}
		}
		PlayerBullet::locked = PBLOCK_LOST;
		for(int i=0;i<ENEMYMAX;i++)
		{
			if(en[i].exist)
			{
				objcount ++;

				if (!(stopflag & FRAME_STOPFLAG_ENEMY))
				{
					en[i].action();
				}
				else
				{
					en[i].actionInStop();
				}
				if(PlayerBullet::locked == PBLOCK_LOST && en[i].able)
				{
					if (en[i].x <= M_CLIENT_RIGHT && en[i].x >= M_CLIENT_LEFT
						&& en[i].y <= M_CLIENT_BOTTOM && en[i].y >= M_CLIENT_TOP)
					{
						PlayerBullet::locked = i;
					}
				}
			}
		}
		for(int i=0;i<GHOSTMAX;i++)
		{
			if(gh[i].exist)
			{
				objcount ++;

				if (!(stopflag & FRAME_STOPFLAG_GHOST))
				{
					gh[i].action();
				}
				else
				{
					gh[i].actionInStop();
				}
				/*
				if(Player::p.bBorder && PlayerBullet::locked == PBLOCK_LOST && gh[i].able && !gh[i].half)
				{
					if (gh[i].x <= M_CLIENT_RIGHT && gh[i].x >= M_CLIENT_LEFT
						&& gh[i].y <= M_CLIENT_BOTTOM && gh[i].y >= M_CLIENT_TOP)
					{
						PlayerBullet::locked = i + PBLOCK_GHOST;
					}
				}
				*/
			}
		}
		Enemy::dmgz.clear_item();
		if (bu.size)
		{
			ZeroMemory(Bullet::renderDepth, sizeof(RenderDepth) * BULLETTYPEMAX);
			DWORD i = 0;
			DWORD size = bu.size;
			for (bu.toBegin(); i<size; bu.toNext(), i++)
			{
				if (!bu.isValid())
				{
					continue;
				}
				if ((*bu).exist)
				{
					objcount++;

					if (!(stopflag & FRAME_STOPFLAG_BULLET))
					{
						(*bu).action();
					}
					else
					{
						(*bu).actionInStop();
					}
				}
				else
				{
					bu.pop();
				}
			}
		}
		if (!(stopflag & FRAME_STOPFLAG_BULLET))
		{
			if (Bullet::izel.size)
			{
				DWORD i = 0;
				DWORD size = Bullet::izel.size;
				for (Bullet::izel.toBegin(); i<size; Bullet::izel.toNext(), i++)
				{
					if (Bullet::izel.isValid())
					{
						IzeZone * tize = &(*(Bullet::izel));
						tize->timer++;
						if (tize->timer == tize->maxtime)
						{
							Bullet::izel.pop();
						}
					}
				}
			}
		}
		if (be.size)
		{
			if (!(stopflag & FRAME_STOPFLAG_BEAM))
			{
				DWORD i = 0;
				DWORD size = be.size;
				for (be.toBegin(); i<size; be.toNext(), i++)
				{
					if (!be.isValid())
					{
						continue;
					}
					if ((*be).exist)
					{
						objcount ++;

						(*be).action();
					}
					else
					{
						be.pop();
					}
				}
			}
		}
		if (pb.size)
		{
			if (!(stopflag & FRAME_STOPFLAG_PLAYERBULLET))
			{
				DWORD i = 0;
				DWORD size = pb.size;
				for (pb.toBegin(); i<size; pb.toNext(), i++)
				{
					if (!pb.isValid())
					{
						continue;
					}
					if ((*pb).exist)
					{
						(*pb).action();
					}
					else
					{
						pb.pop();
					}
				}
			}
		}
		if (mi.size)
		{
			if (!(stopflag & FRAME_STOPFLAG_ITEM))
			{
				DWORD i = 0;
				DWORD size = mi.size;
				for (mi.toBegin(); i<size; mi.toNext(), i++)
				{
					if (!mi.isValid())
					{
						continue;
					}
					if ((*mi).exist)
					{
						(*mi).action();
					}
					else
					{
						mi.pop();
					}
				}
			}
		}

		Scripter::stopEdefScript = false;
		if(BossInfo::flag)
		{
			if(bossinfo.action())
			{
				time = 0;
				if(!spellmode)
				{
					scene = BossInfo::turntoscene;
				}
				else
					Player::p.exist = false;
			}
			if(BossInfo::flag >= BOSSINFO_COLLAPSE)
				Scripter::stopEdefScript = true;
		}
	}
	if(active || !Player::p.exist && state != STATE_CONTINUE)
	{
		if (!(stopflag & FRAME_STOPFLAG_LAYER))
		{
			if(active)
			{
				for(int i=0; i<BGLAYERSETMAX; i++)
				{
					if(BGLayer::set[i].sID != 0)
					{
						BGLayer::set[i].timer++;
						BGLayer::setindex = i;

						if (BGLayer::set[i].timer < BGLayer::set[i].quittime)
						{
							scr.sceneExecute(BGLayer::set[i].sID, BGLayer::set[i].timer);
						}
						else if (BGLayer::set[i].timer == BGLayer::set[i].quittime)
						{
							scr.sceneExecute(BGLayer::set[i].sID, SCRIPT_CON_QUIT);
						}
					}
				}
			}

			for(int i=0; i<BGLAYERMAX; i++)
			{
				if(bg[i].exist)
					bg[i].action();
			}
			for(int i=0;i<FGLAYERMAX;i++)
			{
				if(fg[i].exist)
					fg[i].action();
			}
		}


		if (!(stopflag & FRAME_STOPFLAG_EFFECTSYS))
		{
			for(int i=0; i<EFFECTSYSMAX; i++)
			{
				if(effsys[i].exist)
					effsys[i].action();
			}
		}
	}
	if(fgpause.exist)
		fgpause.action();
	if(bgmask.exist)
		bgmask.action();
	for(list<Selector>::iterator i = sel.begin();i != sel.end(); i++)
	{
		i->action();
	}
	Fontsys::changeID = -1;
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
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
	active = false;
}