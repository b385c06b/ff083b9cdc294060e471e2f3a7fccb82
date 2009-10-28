#include "processPrep.h"

int Process::render()
{
	Export::clientSetMatrix(worldx, worldy, worldz);
	if (state == STATE_INIT)
	{
		return renderInit();
	}
	//BGLayer
	for(int i=0;i<BGLAYERMAX;i++)
	{
		if(bg[i].exist)
		{
			bg[i].Render();
		}
	}
	if(bgmask.exist)
	{
		bgmask.Render();
	}

	if(Player::p.exist || state == STATE_CONTINUE)
	{
		if (bossinfo.flag && bossinfo.isSpell())
		{
			fdisp.BossTimeCircleDisplay();
//			bossinfo.RenderTimeCircle();
		}
		if(!Player::p.bBorder)
		{
			for(int i=0;i<GHOSTMAX;i++)
			{
				if(gh[i].exist)
					gh[i].Render();
			}
		}
		for(int i=0;i<ENEMYMAX;i++)
		{
			if(en[i].exist)
				en[i].Render();
		}
		for(int i=0;i<ENEMYMAX;i++)
		{
			if(en[i].exist)
				en[i].RenderEffect();
		}
		if (pb.size)
		{
			DWORD i = 0;
			DWORD size = pb.size;
			for (pb.toBegin(); i<size; pb.toNext(), i++)
			{
				if (pb.isValid())
				{
					(*pb).Render();
				}

			}
		}
		if(Player::p.exist)
			Player::p.Render();

		if(Player::p.exist)
		{
			Player::p.RenderEffect();
		}

		//3D objs
		for(int i=0; i<EFFECTSYSMAX; i++)
		{
			if(effsys[i].exist)
			{
				effsys[i].Render();
			}
		}

		if (be.size)
		{
			DWORD i = 0;
			DWORD size = be.size;
			for (be.toBegin(); i<size; be.toNext(), i++)
			{
				if (be.isValid())
				{
					(*be).Render();
				}
			}
		}

		if (bu.size)
		{
			for (int i=0; i<BULLETTYPEMAX; i++)
			{
				if (Bullet::renderDepth[i].haveType)
				{
					for (bu.toIndex(Bullet::renderDepth[i].startIndex); bu.index != Bullet::renderDepth[i].endIndex; bu.toNext())
					{
						if (bu.isValid() && (*bu).getRenderDepth() == i)
						{
							(*bu).Render();
						}
					}
				}
			}
		}
		if(BossInfo::flag)
		{
			bossinfo.exist = false;
			fdisp.BossInfoDisplay();
		}

		if(Player::p.bBorder)
		{
			for(int i=0;i<GHOSTMAX;i++)
			{
				if(gh[i].exist)
					gh[i].Render();
			}
		}
		if (mi.size)
		{
			DWORD i = 0;
			DWORD size = mi.size;
			for (mi.toBegin(); i<size; mi.toNext(), i++)
			{
				if (mi.isValid())
				{
					(*mi).Render();
				}
			}
		}

		if(Chat::chatting)
		{
			chat.Render();
		}

		if (Item::infofont.size)
		{
			DWORD i = 0;
			DWORD size = Item::infofont.size;
			for (Item::infofont.toBegin(); i<size; Item::infofont.toNext(), i++)
			{
				if (!Item::infofont.isValid())
				{
					continue;
				}
				infoFont * _i = &(*(Item::infofont));
				if(state != STATE_PAUSE)
					_i->timer++;
				fdisp.ItemInfoDisplay(_i);
			}
		}

		DWORD tcolor;
		if(Player::p.x < 170 && Player::p.y > 420)
		{
			tcolor = 0x20ffffff;
		}
		else
		{
			tcolor = 0xc0ffffff;
		}
	}

	for(int i=0;i<FGLAYERMAX;i++)
	{
		if(fg[i].exist)
		{
			fg[i].Render();
		}
	}
	if(fgpause.exist)
	{
		fgpause.Render();
	}
	Selector::Render();
	InfoSelect::Render();

	Export::clientSetMatrix();

	fdisp.PanelDisplay();

	if(Player::p.exist && BossInfo::flag)
	{
		fdisp.EnemyXDisplay();
	}
	return PGO;
}

int Process::renderInit()
{
	if (texInit)
	{
		hgeQuad quad;
		quad.blend = BLEND_DEFAULT;
		quad.tex = texInit;
		quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = 0xffffffff;
		quad.v[0].tx = 0;	quad.v[0].ty = 0;
		quad.v[1].tx = 1;	quad.v[1].ty = 0;
		quad.v[2].tx = 1;	quad.v[2].ty = 1;
		quad.v[3].tx = 0;	quad.v[3].ty = 1;
		quad.v[0].x = M_CLIENT_LEFT;	quad.v[0].y = M_CLIENT_TOP;	quad.v[0].z = 0;
		quad.v[1].x = M_CLIENT_RIGHT;	quad.v[1].y = M_CLIENT_TOP;	quad.v[1].z = 0;
		quad.v[2].x = M_CLIENT_RIGHT;	quad.v[2].y = M_CLIENT_BOTTOM;	quad.v[2].z = 0;
		quad.v[3].x = M_CLIENT_LEFT;	quad.v[3].y = M_CLIENT_BOTTOM;	quad.v[3].z = 0;
		hge->Gfx_RenderQuad(&quad);
	}
	return PGO;
}