#include "FrontDisplay.h"
#include "SpriteItemManager.h"
#include "BResource.h"
#include "FrontDisplayName.h"
#include "processPrep.h"
#include "BossInfo.h"
#include "Fontsys.h"

FrontDisplay fdisp;

FrontDisplay::FrontDisplay()
{
	ZeroMemory(&panel, sizeof(ftPanelSet));
	ZeroMemory(&info, sizeof(ftInfoSet));
	ZeroMemory(&bignum, sizeof(ftNumSet));
	ZeroMemory(&normalnum, sizeof(ftNumSet));
	ZeroMemory(&smallnum, sizeof(ftNumSet));
	ZeroMemory(&itemnum, sizeof(ftItemNumSet));
	ZeroMemory(&ascii, sizeof(ftAscIISet));
	ZeroMemory(&asciismall, sizeof(ftAscIISet));
	ZeroMemory(&first, sizeof(ftFirstSet));
}

FrontDisplay::~FrontDisplay()
{
}

void FrontDisplay::SetValue(LONGLONG _llval, int _ival, float _fval, bool _bval)
{
	llval = _llval;
	ival = _ival;
	fval = _fval;
	bval = _bval;
}

void FrontDisplay::SetState(BYTE type, BYTE state/* =FDISP_STATE_COUNT */)
{
	switch (type)
	{
	case FDISP_PANEL:
		panelcountup = state;
		break;
	case FDISP_NEXTSTAGE:
		nextstagecount = state;
		break;
	case FDISP_FULLPOWER:
		fullpowercount = state;
		break;
	case FDISP_HISCORE:
		hiscorecount = state;
		break;
	case FDISP_EXTEND:
		extendcount = state;
		break;
	}
}

void FrontDisplay::PanelDisplay()
{
	if (nextstagecount)
	{
		nextstagecount--;
		NextStageDisplay();
	}
	if (panelcountup)
	{
		if (panelcountup < 0xff)
		{
			int tcountup = panelcountup;
			tcountup += 0x08;
			if (tcountup > 0xff)
			{
				tcountup = 0xff;
			}
			panelcountup = tcountup;
			DWORD tcol = (panelcountup<<16)|(panelcountup<<8)|panelcountup|0xff000000;
			panel.left->SetColor(tcol);
			panel.right->SetColor(tcol);
			panel.top->SetColor(tcol);
			panel.bottom->SetColor(tcol);
		}

		if (panelcountup == 0xff)
		{
			BYTE alpha = 0xff;
			DWORD color = 0xffffff;
			if (Player::p.x < 128 && Player::p.y > 420)
			{
				alpha = 0x40;
			}
			color = (alpha<<24)+0xffffff;
			panel.pointindi->SetColor(color);
			panel.pointindi->Render(38, 457);
			float tx, ty, tw, th;
			panel.slot->GetTextureRect(&tx, &ty, &tw, &th);

			panel.slot->SetTextureRect(tx, ty, tw*(Player::p.fPoprate*0.82f+0.18f), th);
			panel.slot->SetHotSpot(0, 0);
			if (Player::p.fPoprate >= 1.0f)
			{
				if (time % 8 < 4)
				{
					color = 0xff3333;
				}
				else
				{
					color = 0xffff33;
				}
			}
			else
			{
				color = 0xffffff;
			}
			color += (alpha<<24);
			panel.slot->SetColor(color);
			panel.slot->Render(44, 438);
			panel.slot->SetTextureRect(tx, ty, tw, th);
//			panel.slotback->SetColor((alpha<<24)+0xffffff);
			panel.slotback->Render(44, 438);

			color = (alpha<<24)+0xffffff;
			if (Player::p.bBorder)
			{
				panel.borderindi->SetColor(color);
				panel.borderindi->RenderEx(80, 448, ARC(Player::p.esBorder.angle+Player::p.esBorder.headangle), Player::p.esBorder.hscale / 2.0f);
			}

			if (info.smalldigitfont)
			{
				info.smalldigitfont->SetColor(color);
				info.smalldigitfont->printf(48, 454, HGETEXT_LEFT | HGETEXT_TOP, "%06d*%.2f", Player::p.nFaith, (int)((Player::p.fPoprate+Player::p.fPopratebase)*100)/100.0f);
				info.smalldigitfont->SetColor(0xffffffff);
			}

		}

		float ledge = panel.left->GetWidth();
		panel.left->Render(M_ACTIVECLIENT_LEFT, M_ACTIVECLIENT_TOP);
		panel.top->Render(M_ACTIVECLIENT_LEFT+ledge, M_ACTIVECLIENT_TOP);
		panel.bottom->Render(M_ACTIVECLIENT_LEFT+ledge, M_ACTIVECLIENT_BOTTOM-panel.bottom->GetHeight());
		panel.right->Render(M_ACTIVECLIENT_RIGHT-ledge, M_ACTIVECLIENT_TOP);

		if (panelcountup == 0xff)
		{
			panel.hiscore->Render(472, 54);
			panel.score->Render(472, 80);
			panel.player->Render(472, 111);
			panel.power->Render(472, 135);
			panel.graze->Render(472, 159);
			if(info.normaldigitfont)
			{
				info.normaldigitfont->SetColor(0xffffffff);
				info.normaldigitfont->printfb(0, 45, 630, 20, HGETEXT_MIDDLE | HGETEXT_RIGHT, "%I64d", (__int64)Player::p.nHiScore);
				info.normaldigitfont->printfb(0, 71, 630, 20, HGETEXT_MIDDLE | HGETEXT_RIGHT, "%I64d", (__int64)Player::p.nScore);

				info.normaldigitfont->printf(520, 126, HGETEXT_LEFT, "%.3f", (float)Player::p.nPower / Player::p.bombperpower);
				info.normaldigitfont->printf(520, 152, HGETEXT_LEFT, "%d", Player::p.nGraze);
				for (int i=0; i<Player::p.nLife; i++)
				{
					panel.lifeindi[5]->Render(530+i*panel.lifeindi[5]->GetWidth(), 110);
				}
			}

		}
	}
	if(info.asciifont)
	{
		if (mp.replaymode)
		{
			info.smalldigitfont->printf(385, 450, 0, "%.2f", mp.replayFPS);
		}
		info.asciifont->printf(
#ifdef __DEBUG
			520,
			410,
			0,
			"%d  %d\n%f\n%f\n%f",
			mp.objcount,
			hge->System_GetState(HGE_FRAMECOUNTER),
			hge->Timer_GetWorstFPS(35)/M_DEFAULT_RENDERSKIP,
			hge->Timer_GetFPS()/M_DEFAULT_RENDERSKIP,
#else
			540,
			450,
			0,
			"%.2f\n%.2f",
			hge->Timer_GetFPS(35)/M_DEFAULT_RENDERSKIP,
#endif
			hge->Timer_GetTime()
			);
#ifdef __DEBUG
		if (Player::p.exist && info.smalldigitfont)
		{
			info.smalldigitfont->printf(8, 465, 0, "%d / %d", mp.scene, time);
		}
#endif // __DEBUG
	}
}

void FrontDisplay::NextStageDisplay()
{
	if (nextstagecount)
	{
		info.stageclear->Render(M_ACTIVECLIENT_CENTER_X, 160);
		if (bval)
		{
			info.nextstage->Render(M_ACTIVECLIENT_CENTER_X, 280);
		}
		info.bossfont->SetColor(0xffffffff);
		info.bossfont->printf(M_ACTIVECLIENT_CENTER_X, 220, HGETEXT_CENTER|HGETEXT_MIDDLE, "%d", llval);
	}
}

void FrontDisplay::BossAction()
{
	BYTE flag = BossInfo::flag;
	WORD timer = bossinfo.timer;
	bool bSpell = bossinfo.isSpell();
	if(flag >= BOSSINFO_COLLAPSE)
	{
		infobody.effBossStore.Stop();
		infobody.effBossCollapse.action();
		infobody.effBossItem.action();
	}
	infobody.effBossStore.action();

	if (flag & BOSSINFO_UP)
	{
		if (timer == 1)
		{
			DWORD col1, col2;
			if(bSpell)
			{
				col1 = 0xce7f0000;
				col2 = 0xceff0000;

				SpriteItemManager::ptFace(en[ENEMY_MAINBOSSINDEX].faceindex, info.cutin);
				info.cutin->SetColor(0x60ffffff);

				infobody.effBossUp.Stop(true);
				infobody.effBossUp.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, 0, true);
				infobody.effBossUp.Fire();
			}
			else
			{
				col1 = 0xcececebd;
				col2 = 0xceffffbd;
			}
			infobody.iqBossBlood.valueSet(0, M_ACTIVECLIENT_CENTER_X, 32,
				60, 30, col1,
				60, 30, col1,
				60, 34, col2,
				60, 34, col2);
		}
		if((infobody.iqBossBlood.quad.v[1].x-60) / 320.0f < en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife)
		{
			infobody.iqBossBlood.quad.v[1].x += 2;
			infobody.iqBossBlood.quad.v[2].x += 2;
		}
		else
		{
			infobody.iqBossBlood.quad.v[1].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
			infobody.iqBossBlood.quad.v[2].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
		}
		//
		if(bSpell)
		{
		/*
			float _tx = en[ENEMY_MAINBOSSINDEX].x;
			float _ty = en[ENEMY_MAINBOSSINDEX].y;
			_tx += (160 - timer) * cost(timer * 400);
			_ty += (160 - timer) * sint(timer * 400);
			infobody.effBossUp.MoveTo(_tx, _ty);
			*/
			infobody.effBossUp.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y);
			infobody.effBossUp.action();
			if(timer == 90)
			{
				SE::push(SE_BOSS_UP, en[ENEMY_MAINBOSSINDEX].x);
				infobody.effBossUp.Stop();
			}
		}
		//
		if (timer > 128)
		{
			info.cutin->SetColor((((160-timer)*3)<<24)+0xffffff);
		}
	}
	else if(flag & BOSSINFO_COLLAPSE)
	{
		if (timer == 1)
		{
			SE::push(SE_BOSS_DEAD, en[ENEMY_MAINBOSSINDEX].x);
			infobody.effBossCollapse.Stop(true);
			infobody.effBossCollapse.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, 0, true);
			infobody.effBossCollapse.Fire();
			if (bSpell)
			{
//				bgmask.flag = BG_REDOUT;
				bgmask.SetFlag(BG_REDOUT, BGMT_OUT);
			}
			mp.SetShake(1);
		}
		else if (timer == 75)
		{
			if (bSpell)
			{
				if (!BossInfo::failed)
				{
					SE::push(SE_BOSS_BONUS);
				}
			}
		}
		else if(timer == 120)
		{
			infobody.effBossCollapse.Stop();
		}
	}
	else if(flag & BOSSINFO_TIMEOVER)
	{
		if(timer == 1)
		{
			if (bSpell)
			{
//				bgmask.flag = BG_WHITEFLASH;
				bgmask.SetFlag(BG_WHITEFLASH, BGMT_FLASH);
			}
		}
	}
	else
	{
		if(en[ENEMY_MAINBOSSINDEX].life > 0)
		{
			infobody.iqBossBlood.quad.v[1].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
			infobody.iqBossBlood.quad.v[2].x = en[ENEMY_MAINBOSSINDEX].life / en[ENEMY_MAINBOSSINDEX].maxlife * 320.0f + 60;
		}
		else
		{
			infobody.iqBossBlood.quad.v[1].x = 60;
			infobody.iqBossBlood.quad.v[2].x = 60;
		}
	}
	if(Player::p.y < 100)
	{
		infobody.iqBossBlood.quad.v[0].col &= (0x7fffffff);
		infobody.iqBossBlood.quad.v[1].col &= (0x7fffffff);
		infobody.iqBossBlood.quad.v[2].col &= (0x7fffffff);
		infobody.iqBossBlood.quad.v[3].col &= (0x7fffffff);

		info.bossspellline->SetColor(0x40ffffff);
		info.spellbonustext->SetColor(0x40ffffff);
		info.spellhistorytext->SetColor(0x40ffffff);
		info.spellfailedtext->SetColor(0x40ffffff);

		info.bossfont->SetColor(0x40ffffff);
		info.bossasciifont->SetColor(0x40ffffff);
	}
	else
	{
		infobody.iqBossBlood.quad.v[0].col |= (0x80<<24);
		infobody.iqBossBlood.quad.v[1].col |= (0x80<<24);
		infobody.iqBossBlood.quad.v[2].col |= (0x80<<24);
		infobody.iqBossBlood.quad.v[3].col |= (0x80<<24);

		info.bossspellline->SetColor(0xffffffff);
		info.spellbonustext->SetColor(0xffffffff);
		info.spellhistorytext->SetColor(0xffffffff);
		info.spellfailedtext->SetColor(0xffffffff);

		info.bossfont->SetColor(0xffffffff);
		info.bossasciifont->SetColor(0xffffffff);
	}

	if(flag >= BOSSINFO_COLLAPSE)
	{
		if(timer == 1)
		{
			infobody.effBossItem.Stop(true);
			infobody.effBossItem.MoveTo(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, 0, true);
			infobody.effBossItem.Fire();
		}
		else if(timer == 90)
		{
			infobody.effBossItem.Stop();
		}
	}
}

void FrontDisplay::BossMoveItemEffect(float x, float y)
{
	infobody.effBossItem.MoveTo(x, y);
}

void FrontDisplay::BossInfoDisplay()
{
	BYTE flag = BossInfo::flag;
	WORD timer = bossinfo.timer;
	bool bSpell = bossinfo.isSpell();
	bool failed = BossInfo::failed;
	DWORD bonus = bossinfo.bonus;
//	exist = false;
	infobody.effBossStore.Render();
	if(flag < BOSSINFO_COLLAPSE)
	{
		info.bossfont->SetScale(1.2f);
		info.bossfont->printf(50, 20, HGETEXT_RIGHT|HGETEXT_MIDDLE, "%d", bossinfo.remain);

		int ttime = bossinfo.limit-timer/60;
		if (ttime < 4)
			info.bossfont->SetColor(0xffff0000);
		else if (ttime < 11)
			info.bossfont->SetColor(0xffff00ff);
		else if (ttime > 99)
		{
			ttime = 99;
		}
		info.bossfont->printf(400, 20, HGETEXT_CENTER|HGETEXT_MIDDLE, "%d", ttime);

		info.bossasciifont->printf(60, 20, HGETEXT_LEFT, "%s", bossinfo.enemyename);

		if(bSpell)
		{
			float yt;
			if(Enemy::bossflag[ENEMY_MAINBOSSINDEX] & BOSS_SPELLUP)
				info.cutin->Render(312, Enemy::spelluptimer[ENEMY_MAINBOSSINDEX]*2.4f);
			if(timer < 30)
			{
				yt = 225;
			}
			else if(timer < 120)
			{
				yt = (120-timer)*2+45;
			}
			else
			{
				yt = 45;
			}

			info.bossspellline->Render(296, yt);

			int tlenth = strlen(bossinfo.spellname);
			float spellnamew = tlenth*8;
			DWORD spellnamealpha = 0xff000000;
			if(Player::p.y < 100)
				spellnamealpha = 0x40000000;
			Fontsys::Render(FONTSYS_SPELLNAMEUSE, 400-spellnamew, yt-5, spellnamealpha+0xffffff, spellnamealpha+0xff0000, 0.4f);

			if (flag & BOSSINFO_UP)
			{
				infobody.effBossUp.Render();
			}

			info.spellbonustext->Render(240, yt+20);
			info.spellhistorytext->Render(345, yt+20);
			if (!failed)
			{
				info.bossasciifont->printf(320, yt+16, HGETEXT_RIGHT, "%09d", bonus);
			}
			else
			{
				info.spellfailedtext->Render(280, yt+20);
			}
			info.bossasciifont->printf(410, yt+16, HGETEXT_RIGHT, "%03d/%03d", bossinfo.get, bossinfo.meet);
		}
		hge->Gfx_RenderQuad(&infobody.iqBossBlood.quad);
	}
	else if(flag & BOSSINFO_COLLAPSE)
	{
		if(bSpell && !failed)
		{
			infobody.effBossCollapse.Render();
			infobody.effBossItem.Render();
			info.getbonus->Render(M_ACTIVECLIENT_CENTER_X, 80);
			info.bossfont->SetColor((((timer*8)%0x100)<<8)+0xffff007f);
		}
		else
		{
			if (failed)
			{
				info.failed->Render(M_ACTIVECLIENT_CENTER_X, 80);
				info.bossfont->SetColor((((timer*8)%0x100)<<16)+0xff00ffff);
			}
		}
		info.bossfont->printf(M_ACTIVECLIENT_CENTER_X, 120, HGETEXT_CENTER|HGETEXT_MIDDLE, "%d", bonus);
	}
}

void FrontDisplay::BossTimeCircleDisplay()
{
	BYTE flag = BossInfo::flag;
	WORD timer = bossinfo.timer;
	BYTE limit = bossinfo.limit;
	if (flag < BOSSINFO_COLLAPSE)
	{
		float scale;
		if (timer >= 88)
		{
			if (timer < 120)
			{
				scale = (timer-88) * 0.1f;
			}
			else
			{
				scale = (timer-60*limit)*0.8f / (30-15*limit);
			}
			info.timecircle->RenderEx(en[ENEMY_MAINBOSSINDEX].x, en[ENEMY_MAINBOSSINDEX].y, timer/15.0f, scale);
		}
	}
}

void FrontDisplay::EnemyXDisplay()
{
	info.enemyx->Render(en[ENEMY_MAINBOSSINDEX].x, 472);
}

void FrontDisplay::ItemInfoDisplay(infoFont * item)
{
	if(item->yellow)
		info.itemfont->SetColor(0xafffff00);
	else
		info.itemfont->SetColor(0xafffffff);
	if(item->timer < 32)
		info.itemfont->printf(item->x, item->y-10-item->timer, HGETEXT_CENTER, "%s", item->cScore);
	else
	{
		Item::infofont.pop();
	}
	if (item->timer == 24 || item->timer == 28)
	{
		for(int i=0;i<(int)strlen(item->cScore);i++)
		{
			item->cScore[i] += 10;
		}
	}
}

bool FrontDisplay::Init()
{
	Release();

	int idx = 0;

	//For SI
	SpriteItemManager::yesIndex = SpriteItemManager::GetIndexByName(SI_CONFIRM_YES);
	SpriteItemManager::noIndex = SpriteItemManager::GetIndexByName(SI_CONFIRM_NO);
	SpriteItemManager::cancelIndex = SpriteItemManager::GetIndexByName(SI_CONFIRM_CANCEL);
	SpriteItemManager::confirmIndex = SpriteItemManager::GetIndexByName(SI_CONFIRM);
	SpriteItemManager::digituiIndex = SpriteItemManager::GetIndexByName(SI_DIGITBIG_0);

	//panel
	panel.left = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LEFT);
	panel.left->SetHotSpot(0, 0);
	panel.right = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_RIGHT);
	panel.right->SetHotSpot(0, 0);
	panel.top = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_TOP);
	panel.top->SetHotSpot(0, 0);
	panel.bottom = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_BOTTOM);
	panel.bottom->SetHotSpot(0, 0);
	panel.hiscore = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_HISCORE);
	panel.score = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_SCORE);
	panel.player = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_PLAYER);
	panel.power = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_POWER);
	panel.graze = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_GRAZE);
	panel.slot = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_SLOT);
	panel.slot->SetHotSpot(0, 0);
	panel.slotback = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_SLOTBACK);
	panel.slotback->SetColor(0x803333ff);
	panel.slotback->SetHotSpot(0, 0);
	panel.lifeindi[0] = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LIFEINDI_0);
	panel.lifeindi[1] = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LIFEINDI_1);
	panel.lifeindi[2] = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LIFEINDI_2);
	panel.lifeindi[3] = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LIFEINDI_3);
	panel.lifeindi[4] = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LIFEINDI_4);
	panel.lifeindi[5] = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_LIFEINDI_5);
	panel.triangleindi = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_TRIANGLEINDI);
	panel.pointindi = SpriteItemManager::CreateSpriteByName(SI_FRONTPANEL_POINT);
	panel.borderindi = SpriteItemManager::CreateSpriteByName(SI_BORDER_CIRCLE);

	//info
	infobody.effBossCollapse.valueSet(EFF_BI_COLLAPSE, 0, 0);
	infobody.effBossCollapse.Stop();
	infobody.effBossItem.valueSet(EFF_BI_ITEM, 0, 0);
	infobody.effBossItem.Stop();
	infobody.effBossUp.valueSet(EFF_BI_BOSSUP, 0, 0);
	infobody.effBossUp.Stop();
	infobody.effBossStore.valueSet(EFF_BI_BOSSSTORE, 0, 0);
	infobody.effBossStore.Stop();
	/************************************************************************/
	infobody.effLoading.valueSet(0, 0, 0);
	infobody.effLoading.Stop();
	/************************************************************************/

	idx = SpriteItemManager::GetIndexByName(SI_FACE_01);
	info.cutin = SpriteItemManager::CreateSprite(idx);
	SpriteItemManager::faceIndexEnemy = idx;
	SpriteItemManager::faceIndexPlayer = SpriteItemManager::GetIndexByName(SI_PLAYERFACE_01);
//	info.cutin = SpriteItemManager::CreateSpriteByName(SI_FACE_01);
	info.plchat_1 = SpriteItemManager::CreateSprite(idx);
	info.plchat_2 = SpriteItemManager::CreateSprite(idx);
	info.plchat_3 = SpriteItemManager::CreateSprite(idx);
	info.enchat_1 = SpriteItemManager::CreateSprite(idx);
	info.enchat_2 = SpriteItemManager::CreateSprite(idx);
	info.enchat_3 = SpriteItemManager::CreateSprite(idx);

	idx = SpriteItemManager::GetIndexByName(SI_NAMECARD_01);
	info.namecard = SpriteItemManager::CreateSprite(idx);
	SpriteItemManager::nameIndexEnemy = idx;
	SpriteItemManager::nameIndexPlayer = SpriteItemManager::GetIndexByName(SI_PLAYERNAMECARD_01);
//	info.namecard = SpriteItemManager::CreateSpriteByName(SI_NAMECARD_01);
	info.bossspellline = SpriteItemManager::CreateSpriteByName(SI_SPELLLINE_BOSS);
	info.playerspellline = SpriteItemManager::CreateSpriteByName(SI_SPELLLINE_PLAYER);
	info.spellbonustext = SpriteItemManager::CreateSpriteByName(SI_SPELLLINE_BONUS);
	info.spellhistorytext = SpriteItemManager::CreateSpriteByName(SI_SPELLLINE_HISTORY);
	info.spellfailedtext = SpriteItemManager::CreateSpriteByName(SI_SPELLLINE_FAILED);
	info.timecircle = SpriteItemManager::CreateSpriteByName(SI_BOSS_TIMECIRCLE);
	info.enemyx = SpriteItemManager::CreateSpriteByName(SI_ENEMY_X);

	info.lifebar = SpriteItemManager::CreateSpriteByName(SI_NULL);
	info.star = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_STAR);
	info.getbonus = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_GETBONUS);
	info.failed = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_FAILED);
	info.stageclear = SpriteItemManager::CreateSpriteByName(SI_STAGE_CLEAR);
	info.nextstage = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_NEXTSTAGE);
	info.fullpower = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_FULLPOWER);
	info.hiscoreget = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_HISCOREGET);
	info.extend = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_EXTEND);
	info.textbox = SpriteItemManager::CreateSpriteByName(SI_FRONT_TEXT);
	info.demo = SpriteItemManager::CreateSpriteByName(SI_FRONTINFO_DEMO);
	info.loading = SpriteItemManager::CreateSpriteByName(SI_LOADING_WORD);

	//num
	idx = SpriteItemManager::GetIndexByName(SI_ASCIIBIG_0);
	for (int i=0; i<10; i++)
	{
		bignum.num[i] = SpriteItemManager::CreateSprite(idx+i);
	}
	bignum.num_add = SpriteItemManager::CreateSpriteByName(SI_ASCIIBIG_X2B);
	bignum.num_sub = SpriteItemManager::CreateSpriteByName(SI_ASCIIBIG_X2D);
	bignum.num_mul = SpriteItemManager::CreateSpriteByName(SI_ASCIIBIG_X2A);
	bignum.num_div = SpriteItemManager::CreateSpriteByName(SI_ASCIIBIG_X2F);
	bignum.num_mod = SpriteItemManager::CreateSpriteByName(SI_ASCIIBIG_X25);
	bignum.num_dot = SpriteItemManager::CreateSpriteByName(SI_ASCIIBIG_X2E);

	idx = SpriteItemManager::GetIndexByName(SI_DIGITNORMAL_0);
	for (int i=0; i<10; i++)
	{
		normalnum.num[i] = SpriteItemManager::CreateSprite(idx+i);
	}
	normalnum.num_div = SpriteItemManager::CreateSpriteByName(SI_DIGITNORMAL_DIV);
	normalnum.num_dot = SpriteItemManager::CreateSpriteByName(SI_DIGITNORMAL_DOT);

	idx = SpriteItemManager::GetIndexByName(SI_DIGITSMALL_0);
	for (int i=0; i<10; i++)
	{
		smallnum.num[i] = SpriteItemManager::CreateSprite(idx+i);
	}
	smallnum.num_add = SpriteItemManager::CreateSpriteByName(SI_DIGITSMALL_ADD);
	smallnum.num_sub = SpriteItemManager::CreateSpriteByName(SI_DIGITSMALL_SUB);
	smallnum.num_mul = SpriteItemManager::CreateSpriteByName(SI_DIGITSMALL_MUL);
	smallnum.num_div = SpriteItemManager::CreateSpriteByName(SI_DIGITSMALL_DIV);
	smallnum.num_mod = SpriteItemManager::CreateSpriteByName(SI_DIGITSMALL_MOD);
	smallnum.num_dot = SpriteItemManager::CreateSpriteByName(SI_DIGITSMALL_DOT);

	idx = SpriteItemManager::GetIndexByName(SI_DIGITCHARACTER_0);
	for (int i=0; i<30; i++)
	{
		itemnum.itemnum[i] = SpriteItemManager::CreateSprite(idx+i);
	}

	//ascii

	idx = SpriteItemManager::GetIndexByName(SI_ASCIIBIG_X20);
	for (int i=0; i<FDISP_ASCII_MAX; i++)
	{
		ascii.ascii[i] = SpriteItemManager::CreateSprite(idx+i);
	}
	idx = SpriteItemManager::GetIndexByName(SI_ASCIISMALL_X20);
	for (int i=0; i<FDISP_ASCIISMALL_MAX; i++)
	{
		asciismall.ascii[i] = SpriteItemManager::CreateSprite(idx+i);
	}

	//first
	first.face = SpriteItemManager::CreateSpriteByName(SI_FACE_01);
	first.namecard = SpriteItemManager::CreateSpriteByName(SI_NAMECARD_01);


	//font
	info.normaldigitfont = new hgeFont();
	for (int i='0'; i<='9'; i++)
	{
		info.normaldigitfont->ChangeSprite(i, normalnum.num[i-'0']);
	}
	info.normaldigitfont->ChangeSprite('/', normalnum.num_div);
	info.normaldigitfont->ChangeSprite('.', normalnum.num_dot);
	info.normaldigitfont->ChangeSprite(' ', ascii.space);

	info.bossfont = new hgeFont();
	for (int i='0'; i<='9'; i++)
	{
		info.bossfont->ChangeSprite(i, normalnum.num[i-'0']);
	}
	info.bossfont->ChangeSprite('/', normalnum.num_div);
	info.bossfont->ChangeSprite('.', normalnum.num_dot);
	info.bossfont->ChangeSprite(' ', ascii.space);

	info.smalldigitfont = new hgeFont();
	for (int i='0'; i<='9'; i++)
	{
		info.smalldigitfont->ChangeSprite(i, smallnum.num[i-'0']);
	}
	info.smalldigitfont->ChangeSprite('+', smallnum.num_add);
	info.smalldigitfont->ChangeSprite('-', smallnum.num_sub);
	info.smalldigitfont->ChangeSprite('*', smallnum.num_mul);
	info.smalldigitfont->ChangeSprite('/', smallnum.num_div);
	info.smalldigitfont->ChangeSprite('%', smallnum.num_mod);
	info.smalldigitfont->ChangeSprite('.', smallnum.num_dot);
	info.smalldigitfont->ChangeSprite(' ', asciismall.space);

	info.itemfont = new hgeFont();
	for (int i='0'; i<30+'0'; i++)
	{
		info.itemfont->ChangeSprite(i, itemnum.itemnum[i-'0']);
	}
	info.itemfont->SetScale(1.2f);

	info.asciifont = new hgeFont();
	for (int i=FDISP_ASCII_BEGIN; i<=FDISP_ASCII_END; i++)
	{
		info.asciifont->ChangeSprite(i, ascii.ascii[i-FDISP_ASCII_BEGIN]);
	}
	info.bossasciifont = new hgeFont();
	for (int i=FDISP_ASCII_BEGIN; i<=FDISP_ASCIISMALL_END; i++)
	{
		info.bossasciifont->ChangeSprite(i, asciismall.ascii[i-FDISP_ASCII_BEGIN]);
	}

	info.normalfont = hge->Font_Load(res.resdata.widefontname, 20);
	info.smallfont = hge->Font_Load(res.resdata.widefontname, 16);

	return true;
}

void FrontDisplay::Release()
{
	SpriteItemManager::FreeSprite(panel.left);
	SpriteItemManager::FreeSprite(panel.top);
	SpriteItemManager::FreeSprite(panel.bottom);
	SpriteItemManager::FreeSprite(panel.right);
	SpriteItemManager::FreeSprite(panel.hiscore);
	SpriteItemManager::FreeSprite(panel.score);
	SpriteItemManager::FreeSprite(panel.player);
	SpriteItemManager::FreeSprite(panel.power);
	SpriteItemManager::FreeSprite(panel.graze);
	SpriteItemManager::FreeSprite(panel.slot);
	SpriteItemManager::FreeSprite(panel.slotback);
	for (int i=0; i<6; i++)
	{
		SpriteItemManager::FreeSprite(panel.lifeindi[i]);
	}
	SpriteItemManager::FreeSprite(panel.triangleindi);
	SpriteItemManager::FreeSprite(panel.pointindi);
	SpriteItemManager::FreeSprite(panel.borderindi);

	if (info.bossfont)
	{
		delete info.bossfont;
		info.bossfont = NULL;
	}
	if (info.normaldigitfont)
	{
		delete info.normaldigitfont;
		info.normaldigitfont = NULL;
	}
	if (info.smalldigitfont)
	{
		delete info.smalldigitfont;
		info.smalldigitfont = NULL;
	}
	if (info.asciifont)
	{
		delete info.asciifont;
		info.asciifont = NULL;
	}
	if (info.bossasciifont)
	{
		delete info.bossasciifont;
		info.bossasciifont = NULL;
	}
	if (info.itemfont)
	{
		delete info.itemfont;
		info.itemfont = NULL;
	}
	if (info.normalfont)
	{
		hge->Font_Free(info.normalfont);
		info.normalfont = NULL;
	}
	if (info.smallfont)
	{
		hge->Font_Free(info.smallfont);
		info.smallfont = NULL;
	}
	SpriteItemManager::FreeSprite(info.cutin);
	SpriteItemManager::FreeSprite(info.namecard);
	SpriteItemManager::FreeSprite(info.plchat_1);
	SpriteItemManager::FreeSprite(info.plchat_2);
	SpriteItemManager::FreeSprite(info.plchat_3);
	SpriteItemManager::FreeSprite(info.enchat_1);
	SpriteItemManager::FreeSprite(info.enchat_2);
	SpriteItemManager::FreeSprite(info.enchat_3);
	SpriteItemManager::FreeSprite(info.bossspellline);
	SpriteItemManager::FreeSprite(info.playerspellline);
	SpriteItemManager::FreeSprite(info.spellbonustext);
	SpriteItemManager::FreeSprite(info.spellhistorytext);
	SpriteItemManager::FreeSprite(info.spellfailedtext);
	SpriteItemManager::FreeSprite(info.timecircle);
	SpriteItemManager::FreeSprite(info.enemyx);
	SpriteItemManager::FreeSprite(info.lifebar);
	SpriteItemManager::FreeSprite(info.star);
	SpriteItemManager::FreeSprite(info.getbonus);
	SpriteItemManager::FreeSprite(info.failed);
	SpriteItemManager::FreeSprite(info.stageclear);
	SpriteItemManager::FreeSprite(info.nextstage);
	SpriteItemManager::FreeSprite(info.fullpower);
	SpriteItemManager::FreeSprite(info.hiscoreget);
	SpriteItemManager::FreeSprite(info.extend);
	SpriteItemManager::FreeSprite(info.textbox);
	SpriteItemManager::FreeSprite(info.demo);
	SpriteItemManager::FreeSprite(info.loading);

	for (int i=0; i<10; i++)
	{
		SpriteItemManager::FreeSprite(bignum.num[i]);
	}
	SpriteItemManager::FreeSprite(bignum.num_add);
	SpriteItemManager::FreeSprite(bignum.num_sub);
	SpriteItemManager::FreeSprite(bignum.num_mul);
	SpriteItemManager::FreeSprite(bignum.num_div);
	SpriteItemManager::FreeSprite(bignum.num_mod);
	SpriteItemManager::FreeSprite(bignum.num_dot);

	for (int i=0; i<10; i++)
	{
		SpriteItemManager::FreeSprite(normalnum.num[i]);
	}
	SpriteItemManager::FreeSprite(normalnum.num_div);
	SpriteItemManager::FreeSprite(normalnum.num_dot);

	for (int i=0; i<10; i++)
	{
		SpriteItemManager::FreeSprite(smallnum.num[i]);
	}
	SpriteItemManager::FreeSprite(smallnum.num_add);
	SpriteItemManager::FreeSprite(smallnum.num_sub);
	SpriteItemManager::FreeSprite(smallnum.num_mul);
	SpriteItemManager::FreeSprite(smallnum.num_div);
	SpriteItemManager::FreeSprite(smallnum.num_mod);
	SpriteItemManager::FreeSprite(smallnum.num_dot);

	for (int i=0; i<30; i++)
	{
		SpriteItemManager::FreeSprite(itemnum.itemnum[i]);
	}

	for (int i=0; i<FDISP_ASCII_MAX; i++)
	{
		SpriteItemManager::FreeSprite(ascii.ascii[i]);
	}
	for (int i=0; i<FDISP_ASCIISMALL_MAX; i++)
	{
		SpriteItemManager::FreeSprite(asciismall.ascii[i]);
	}

	SpriteItemManager::FreeSprite(first.face);
	SpriteItemManager::FreeSprite(first.namecard);

}