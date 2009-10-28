#include "Chat.h"
#include "BGLayer.h"
#include "Player.h"
#include "Bullet.h"
#include "Fontsys.h"
#include "Main.h"
#include "Process.h"
#include "SpriteItemManager.h"
#include "FrontDisplay.h"
#include "FrontDisplayName.h"
#include "Enemy.h"

#define _CHAT_LINECHARACTER	34

Chat chat;

bool Chat::chatting;
bool Chat::chatinit;
BYTE Chat::timer;
BYTE Chat::chati;

Chat::Chat()
{
	col = 0x00000000;
	chatting = false;
	chatinit = false;
	timer = 0;
	pushtimer = 0xff;

	for (int i=0; i<CHATTERMAX; i++)
	{
		chatter[i] = NULL;
	}
}

Chat::~Chat()
{
}

void Chat::Init()
{
	Release();
	left = SpriteItemManager::CreateSprite(-1);
	right = SpriteItemManager::CreateSprite(-1);
	leftname = SpriteItemManager::CreateSprite(-1);
	rightname = SpriteItemManager::CreateSprite(-1);
	textbox = SpriteItemManager::CreateSpriteByName(SI_FRONT_TEXT);
}

void Chat::Release()
{
	for (int i=0; i<CHATTERMAX; i++)
	{
		if (chatter[i])
		{
			SpriteItemManager::FreeSprite(chatter[i]);
		}
	}
}

void Chat::Render()
{
	for(int i = 0; i < CHATTERMAX-1; i++)
	{
		chatter[i]->RenderEx(x[i], y[i], 0, 0.8f);
	}
	textbox->RenderEx(x[CHATTER_TEXTBOX], y[CHATTER_TEXTBOX], ARC(9000), 1.5f, 1.0f);
	Fontsys::Render(FONTSYS_CHATUSE, M_ACTIVECLIENT_LEFT+70, M_ACTIVECLIENT_CENTER_Y+138, 0xffffffff, col, 0.4f);
}

bool Chat::chat(BYTE ID, BYTE chatsprite, const char * _text)
{
	timer++;
	if(!chatting)
	{
		timer = 0;
		return true;
	}
	Player::p.bInfi = true;

	if(hge->Input_GetDIKey(KS_FIRE_MP))
	{
		if(pushtimer < M_PUSH_FIRST)
			pushtimer++;
		else if(pushtimer == M_PUSH_FIRST)
			pushtimer = M_PUSH_FIRST - M_PUSH_SKIP;
	}
	else
	{
		pushtimer = 0;
	}

	if(hge->Input_GetDIKey(KS_FIRE_MP, DIKEY_DOWN) || pushtimer == M_PUSH_FIRST || timer == M_NOPUSH_SKIP)
	{
		chatinit = false;
		timer = 0;
		return true;
	}

	if(chatinit)
	{
		return false;
	}

	if (timer == 1)
	{
		if(chatsprite & CHATSPRITE_LEFT)
		{
			left->SetColor(0xffffffff);
			right->SetColor(0x80ffffff);
			textbox->SetColor(0xff3333ff);
			SpriteItemManager::ptFace(ID, left, false);
			left->SetFlip(chatsprite & CHATSPRITE_LEFTFLIP, false);
			SpriteItemManager::ptName(ID, leftname, false);
		}
		else
		{
			ID -= ENEMY_BOSSTYPEBEGIN;
			left->SetColor(0x80ffffff);
			right->SetColor(0xffffffff);
			textbox->SetColor(0xffff3333);
			SpriteItemManager::ptFace(ID, right, true);
			right->SetFlip(chatsprite & CHATSPRITE_RIGHTFLIP, false);
			SpriteItemManager::ptName(ID, rightname, true);
		}
		int line = 0;
		int tlength = strlen(_text);
		for(WORD i=0;i<tlength+1;i++)
		{
			if(i!=0 && !(i%_CHAT_LINECHARACTER) && i < tlength-1)
			{
				text[i+line] = '\n';
				line++;
			}
			text[i+line] = _text[i];
		}
		Fontsys::SignUp(FONTSYS_CHATUSE, text, fdisp.info.smallfont);
		if(chatsprite & CHATSPRITE_LEFT)
		{
			col = 0xff6699ff;
		}
		else
		{
			col = 0xffff6666;
		}
	}
	else if (timer == 16)
	{
		chatinit = true;
	}
	else
	{
		if (chatsprite & CHATSPRITE_LEFT)
		{
			x[CHATTER_LEFT] = INTER(x[CHATTER_LEFT], M_ACTIVECLIENT_CENTER_X-80, 1.0/(16-timer));
			x[CHATTER_RIGHT] = INTER(x[CHATTER_RIGHT], M_ACTIVECLIENT_CENTER_X+132, 1.0/(16-timer));
		}
		else
		{
			x[CHATTER_LEFT] = INTER(x[CHATTER_LEFT], M_ACTIVECLIENT_CENTER_X-132, 1.0/(16-timer));
			x[CHATTER_RIGHT] = INTER(x[CHATTER_RIGHT], M_ACTIVECLIENT_CENTER_X+80, 1.0/(16-timer));
		}
	}
	return false;
}

bool Chat::chatOn(BYTE leftID, BYTE rightID, BYTE chatsprite)
{
	if(Player::p.bBomb || Player::p.bBorder)
		return false;
	timer++;

	Player::p.bInfi = true;
	chatinit = false;

	rightID -= ENEMY_BOSSTYPEBEGIN;
	
	if(timer == 1)
	{
		pushtimer = 0xff;

		strcpy(text, "");
		Fontsys::SignUp(FONTSYS_CHATUSE, text);
		
		Bullet::IzeBuild(BULLETIZE_FAITH, Player::p.x, Player::p.y);

		chatting = true;
		SpriteItemManager::SetSprite(-1, leftname, mp.tex);
		SpriteItemManager::SetSprite(-1, rightname, mp.tex);
		if (leftID != 0xff)
		{
			SpriteItemManager::ptFace(leftID, left, false);
			left->SetFlip(chatsprite & CHATSPRITE_LEFTFLIP, false);
		}
		else
		{
			SpriteItemManager::ptFace(-1, left, false);
		}
		if (rightID != 0xff)
		{
			SpriteItemManager::ptFace(rightID, right, true);
			right->SetFlip(chatsprite & CHATSPRITE_RIGHTFLIP, false);
		}
		else
		{
			SpriteItemManager::ptFace(-1, right, true);
		}
		if (chatsprite & CHATSPRITE_LEFT)
		{
			textbox->SetColor(0xff3333ff);
		}
		else
		{
			textbox->SetColor(0xffff3333);
		}
	}
	if(timer <= 36)
	{
		x[CHATTER_LEFT] = M_ACTIVECLIENT_CENTER_X-492+timer*10;
		y[CHATTER_LEFT] = M_ACTIVECLIENT_CENTER_Y+80;
		x[CHATTER_RIGHT] = M_ACTIVECLIENT_CENTER_X+492-timer*10;
		y[CHATTER_RIGHT] = M_ACTIVECLIENT_CENTER_Y+80;
		if(chatsprite & CHATSPRITE_LEFT)
		{
			right->SetColor(0x80ffffff);
		}
		else
		{
			left->SetColor(0x80ffffff);
		}
		x[CHATTER_TEXTBOX] = M_ACTIVECLIENT_CENTER_X;
		y[CHATTER_TEXTBOX] = M_ACTIVECLIENT_BOTTOM+72-timer*4;
	}
	else
	{
		if (leftID != 0xff)
		{
			x[CHATTER_LEFTNAME] = M_ACTIVECLIENT_CENTER_X-90;
			y[CHATTER_LEFTNAME] = M_ACTIVECLIENT_CENTER_Y+108;
			SpriteItemManager::ptName(leftID, leftname, false);
		}
		if (rightID != 0xff)
		{
			x[CHATTER_RIGHTNAME] = M_ACTIVECLIENT_CENTER_X+90;
			y[CHATTER_RIGHTNAME] = M_ACTIVECLIENT_CENTER_Y+108;
			SpriteItemManager::ptName(rightID, rightname, true);
		}
		/*
		match(CHATTER_LEFTNAME, left);
		match(CHATTER_RIGHTNAME, right);
		*/
		timer = 0;
		return true;
	}
	return false;
}

bool Chat::chatOff()
{
	timer++;

	Player::p.bInfi = true;

	if(timer == 1)
	{
		strcpy(text, "");
		Fontsys::SignUp(FONTSYS_CHATUSE, text);
		SpriteItemManager::ptName(-1, leftname);
		SpriteItemManager::ptName(-1, rightname);
	}

	if(timer <= 32)
	{
		x[CHATTER_LEFT] -= timer * 15;
		x[CHATTER_RIGHT] += timer * 15;
		y[CHATTER_TEXTBOX] += timer * 4;
	}
	else
	{
		Player::p.bInfi = false;
		timer = 0;
		chatting = false;
		return true;
	}
	return false;
}