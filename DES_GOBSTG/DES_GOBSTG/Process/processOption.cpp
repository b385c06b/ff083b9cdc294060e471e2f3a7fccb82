#include "Process.h"
#include "Scripter.h"
#include "Selector.h"
#include "InfoSelect.h"
#include "SpriteItemManager.h"
#include "FrontDisplayName.h"
#include "SE.h"
#include "PushKey.h"
#include "ConstResource.h"

int Process::processOption()
{
	time++;
	if(time == 1)
	{
		scr.SetIntValue(SCR_RESERVEBEGIN, 0);
		scr.SetIntValue(SCR_RESERVEBEGIN+1, 0);
		PushKey::SetPushEvent(PUSHKEY_ID_UIUSE, KS_LEFT, KS_RIGHT);
	}
	scr.controlExecute(STATE_OPTION, time);
	//-> pushtimer sel depth
	int tsel = scr.GetIntValue(SCR_RESERVEBEGIN);
	int tdepth = scr.GetIntValue(SCR_RESERVEBEGIN+1);

	tsel = Selector::select;
	if(!tdepth)
	{
		if(hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_CANCEL);
			Selector::select = Selector::nselect-1;
		}
		if((hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN) || hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN)) && tsel == Selector::nselect-1)
		{
			SE::push(SE_SYSTEM_CANCEL);
			Selector::Clear();
			time = 0;
			state = STATE_TITLE;
			return PTURN;
		}

		PushKey::UpdatePushEvent(PUSHKEY_ID_UIUSE);

		if(hge->Input_HaveJoy() &&
			(hge->Input_GetDIKey(DIK_TAB, DIKEY_DOWN) ||
			!tsel && (hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN) || hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))))
		{
			SE::push(SE_SYSTEM_OK);
			tdepth = 1;

			for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
			{
				if(i->ID < 0x10)
				{
					i->changeState(SEL_NONACTIVE|SEL_GRAY, SELOP_SET);
					i->ID += 0x10;
				}
				else if(i->ID < 0x20)
				{
					i->changeState(SEL_NONACTIVE|SEL_GRAY, SELOP_UNSET);
					i->ID -= 0x10;
				}
			}
			Selector::Setup(6, 0);
		}
		else if(tsel == 1)
		{
			if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN) ||
				hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN) ||
				hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_OK);
				screenmode = 1 - screenmode;
				hge->System_SetState(HGE_WINDOWED, !(bool)screenmode);
				hge->System_SetState(HGE_HIDEMOUSE, (bool)screenmode);
			}
		}
		else if(tsel == 2 || tsel == 3)
		{
			int tvol;
			if(tsel == 2)
			{
				tvol = bgmvol;
			}
			else
			{
				tvol = sevol;
				if(!hge->Channel_IsPlaying(SE::se[SE_BOSS_TIMEOUT].chn))
					SE::push(SE_BOSS_TIMEOUT);
			}

			if(hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
			{
				tvol += 5;
				if(tvol > 100)
					tvol = 0;
			}
			else if(hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				tvol -= 5;
				if(tvol < 0)
					tvol = 0;
			}
			else if(hge->Input_GetDIKey(KS_RIGHT, DIKEY_DOWN))
			{
				SE::push(SE_SYSTEM_SELECT);
				tvol += 5;
				if(tvol > 100)
					tvol = 100;
			}

			if(tsel == 2)
			{
				bgmvol = tvol;
			}
			else
			{
				sevol = tvol;
			}
		}
		else if(tsel == 4 && hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN))
		{
			joyFire = RESCONFIGDEFAULT_JOYFIRE;
			joySpecial = RESCONFIGDEFAULT_JOYSPECIAL;
			joySlow = RESCONFIGDEFAULT_JOYSLOW;
			joyChange = RESCONFIGDEFAULT_JOYCHANGE;
			joyPause = RESCONFIGDEFAULT_JOYPAUSE;
			screenmode = RESCONFIGDEFAULT_SCREENMODE;
			bgmvol = RESCONFIGDEFAULT_VOLMUSIC;
			sevol = RESCONFIGDEFAULT_VOLSE;
		}
	}
	else
	{
		if (tsel != 5)
		{
			for(int i=0;i<M_JOYKEYMAX;i++)
			{
				if(hge->Input_GetDIJoy(i, DIKEY_DOWN) &&
					!hge->Input_GetDIJoy(JOY_LEFT, DIKEY_DOWN) &&
					!hge->Input_GetDIJoy(JOY_RIGHT, DIKEY_DOWN) &&
					!hge->Input_GetDIJoy(JOY_UP, DIKEY_DOWN) &&
					!hge->Input_GetDIJoy(JOY_DOWN, DIKEY_DOWN))
				{
					SE::push(SE_SYSTEM_OK);
					joyKey[tsel] = i;

					for(int j = 0;j < 5;j++)
					{
						if(j == tsel)
							continue;
						if(joyKey[j] == i)
						{
							for(int k = 0;k < M_JOYKEYMAX;k++)
							{
								if(joyKey[0]!=k && 
									joyKey[1]!=k && 
									joyKey[2]!=k && 
									joyKey[3]!=k && 
									joyKey[4]!=k)
								{
									joyKey[j] = k;
									break;
								}
							}
						}
					}
				}
			}
		}
		if(hge->Input_GetDIKey(KS_ENTER, DIKEY_DOWN) ||
			hge->Input_GetDIKey(DIK_TAB, DIKEY_DOWN) ||
			hge->Input_GetDIKey(KS_LEFT, DIKEY_DOWN) ||
			tsel == 5 && (hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN) || hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN)) ||
			hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN) && !hge->Input_GetDIJoy(joySpecial, DIKEY_DOWN) ||
			hge->Input_GetDIKey(KS_FIRE, DIKEY_DOWN) && !hge->Input_GetDIJoy(joyFire, DIKEY_DOWN))
		{
			SE::push(SE_SYSTEM_OK);
			tdepth = 0;
			for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
			{
				if(i->ID < 0x10)
				{
					i->changeState(SEL_NONACTIVE|SEL_GRAY, SELOP_SET);
					i->ID += 0x10;
				}
				else if(i->ID < 0x20)
				{
					i->changeState(SEL_NONACTIVE|SEL_GRAY, SELOP_UNSET);
					i->ID -= 0x10;
				}
			}
			Selector::Setup(6, 0);

			InfoSelect::Clear();
		}
	}

	SE::vol = sevol;
	hge->Channel_SetVolume(channel, bgmvol);

	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if((i->ID & 0xf0) == 0x80)
		{
			int tjk = joyKey[(i->ID & 0x0f) >> 1];
			if(i->ID & 1)
				SpriteItemManager::SetSprite(SpriteItemManager::digituiIndex+tjk%10, i->sprite, tex);
			else
				SpriteItemManager::SetSprite(SpriteItemManager::digituiIndex+tjk/10, i->sprite, tex);
		}
		else if(i->ID == 0x90)
		{
			if (screenmode)
			{
				SpriteItemManager::SetSprite(SpriteItemManager::GetIndexByName(SI_OPTIONGRAPH_FULL), i->sprite, tex);
			}
			else
			{
				SpriteItemManager::SetSprite(SpriteItemManager::GetIndexByName(SI_OPTIONGRAPH_WINDOW), i->sprite, tex);
			}
		}
		else if(i->ID == 0xA0)
		{
		}
		else if((i->ID & 0xf0) == 0xB0 || (i->ID & 0xf0) == 0xC0)
		{
			int tvol;
			if((i->ID & 0xf0) == 0xB0)
			{
				tvol = bgmvol;
			}
			else
			{
				tvol = sevol;
			}
			switch(i->ID & 0x0f)
			{
			case 0x00:
				if(tvol < 100)
					SpriteItemManager::SetSprite(-1, i->sprite, tex);
				else
					SpriteItemManager::SetSprite(SpriteItemManager::digituiIndex+1, i->sprite, tex);
				break;
			case 0x01:
				if(tvol < 10)
					SpriteItemManager::SetSprite(-1, i->sprite, tex);
				else
					SpriteItemManager::SetSprite(SpriteItemManager::digituiIndex+((tvol / 10) % 10), i->sprite, tex);
				break;
			case 0x02:
				SpriteItemManager::SetSprite(SpriteItemManager::digituiIndex+(tvol % 10), i->sprite, tex);
				break;
			case 0x03:
				SpriteItemManager::SetSprite(SpriteItemManager::digituiIndex+SIDIGITUI_MODPLUS, i->sprite, tex);
				break;
			}
		}

		if(i->ID < 0x10)
		{
			if(i->flag & SEL_ENTER)
			{
				i->changeState(SEL_ENTER, SELOP_UNSET);
				i->changeState(SEL_OVER, SELOP_SET);
			}
			else if(i->flag & SEL_LEAVE)
			{
				i->changeState(SEL_LEAVE, SELOP_UNSET);
				i->changeState(SEL_NONE, SELOP_SET);
			}
		}
		else if (i->ID < 0x20 || !(tdepth ^ ((i->ID & 0xf0) != 0x80)))
		{
			i->changeState(SEL_NONACTIVE|SEL_GRAY, SELOP_EQUAL);
		}
		else
		{
			i->changeState(SEL_NONACTIVE, SELOP_EQUAL);
		}
	}

	scr.SetIntValue(SCR_RESERVEBEGIN, tsel);
	scr.SetIntValue(SCR_RESERVEBEGIN+1, tdepth);

	return PGO;
}