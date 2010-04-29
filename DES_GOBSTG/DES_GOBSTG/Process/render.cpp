#include "../Header/processPrep.h"

int Process::render()
{
	Export::clientSetMatrix(worldx, worldy, worldz);
	if (state == STATE_INIT)
	{
		return renderInit();
	}
	//BGLayer
	BGLayer::RenderBG();

	if(Player::p.exist || state == STATE_CONTINUE)
	{
		if (BossInfo::bossinfo.bossable() && BossInfo::bossinfo.isSpell())
		{
			FrontDisplay::fdisp.BossTimeCircleDisplay();
		}
		if(!Player::p.bBorder)
		{
			Ghost::RenderAll();
		}
		Enemy::RenderAll();
		PlayerBullet::RenderAll();
		Player::RenderAll();

		//3D objs
		Effectsys::RenderAll();
		Beam::RenderAll();
#ifdef __DEBUG
//		Beam::Debug_RenderCollision();
#endif
		Bullet::RenderAll();
		if(BossInfo::bossinfo.bossable())
		{
			BossInfo::bossinfo.exist = false;
			FrontDisplay::fdisp.BossInfoDisplay();
		}

		if(Player::p.bBorder)
		{
			Ghost::RenderAll();
		}
		Item::RenderAll();
		Chat::chatitem.Render();


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

	BGLayer::RenderFG();
	BGLayer::RenderFGPause();
	Selector::Render();
	InfoSelect::Render();

	Export::clientSetMatrix();

	FrontDisplay::fdisp.PanelDisplay();

	if(Player::p.exist && BossInfo::bossinfo.bossable())
	{
		FrontDisplay::fdisp.EnemyXDisplay();
	}
	return PGO;
}

int Process::renderInit()
{
	if (texInit.tex)
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