#include "Selector.h"
#include "SE.h"
#include "Main.h"
#include "Process.h"
#include "SpriteItemManager.h"
#include "PushKey.h"

list<Selector> sel;

int Selector::select = 0;
int Selector::sellock = 0;
int Selector::nselect = 0;
bool Selector::updown = true;
bool Selector::complete = false;
bool Selector::plus = true;

int Selector::nPageNum = 0;
int Selector::firstID;
int Selector::lastID;
float Selector::fadebegin;
float Selector::offset;

bool Selector::confirminit = false;

bool Selector::avoid = false;

Selector::Selector()
{
	sprite = NULL;
	complete = false;
	flag	= SEL_NONE;

	nPageNum = 0;
}

Selector::~Selector()
{
}

void Selector::Clear()
{
	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if(i->sprite)
			delete i->sprite;
		i->sprite = NULL;
	}
	sel.clear();
	complete = false;
	confirminit = false;
	plus = true;
}

void Selector::valueSet(BYTE _ID, int siID, float cenx, float ceny, float _hscale, float _vscale, BYTE _maxtime)
{
	ID		=	_ID;
	x		=	cenx;
	y		=	ceny;
	maxtime	=	_maxtime;
	hscale	=	_hscale;
	vscale	=	_vscale;

	timer	=	0;
	flag	=	SEL_NONE;

	if(sprite)
		SpriteItemManager::FreeSprite(sprite);
	sprite = SpriteItemManager::CreateSprite(siID);

	alpha = 0xff;

	for(int i=0;i<SEL_STATEMAX;i++)
	{
		info[i].xadj = x;
		info[i].yadj = y;
	}

	avoid = false;
}

void Selector::actionSet(BYTE setflag, float xadj, float yadj)
{
	int i = SELINFO_NONE;
	switch (setflag)
	{
	case SEL_OVER:
		i = SELINFO_OVER;
		break;
	case SEL_ENTER:
		i = SELINFO_ENTER;
		break;
	case SEL_LEAVE:
		i = SELINFO_LEAVE;
		break;
	}
	info[i].xadj = xadj;
	info[i].yadj = yadj;
}

void Selector::Render()
{
	for(list<Selector>::iterator i = sel.begin();i != sel.end(); i++)
	{
		(i->sprite)->RenderEx(i->x, i->y, 0, i->hscale, i->vscale);
	}
}

Selector * Selector::GetPointer(int index)
{
	if (index < 0 || index >= sel.size())
	{
		return NULL;
	}
	int tindex = 0;
	for (list<Selector>::iterator i=sel.begin(); i!=sel.end(); i++)
	{
		if (index == tindex)
		{
			return &(*i);
		}
		tindex++;
	}
	return NULL;
}

void Selector::Build(BYTE _ID, int siID, float cenx, float ceny, float _hscale, float _vscale, BYTE _maxtime,
		float xadj0, float yadj0,
		float xadj1, float yadj1,
		float xadj2, float yadj2,
		float xadj3, float yadj3,
		BYTE flag)
{
	Selector _sel;
	_sel.valueSet(_ID, siID, cenx, ceny, _hscale, _vscale, _maxtime);
	_sel.actionSet(SEL_NONE, xadj0, yadj0);
	_sel.actionSet(SEL_OVER, xadj1+xadj0, yadj1+yadj0);
	_sel.actionSet(SEL_ENTER, xadj2+xadj0, yadj2+yadj0);
	_sel.actionSet(SEL_LEAVE, xadj3+xadj0, yadj3+yadj0);
	_sel.flag = flag;
	sel.push_back(_sel);
}

bool Selector::SetPageNum(BYTE _nPageNum, float _fadebegin, float _offset, int initshift)
{
	if (nselect < _nPageNum)
	{
		return false;
	}
	nPageNum = _nPageNum;
	fadebegin = _fadebegin;
	offset = _offset;
	firstID = 0;
	lastID = nPageNum - 1;
	shift(initshift);
	return true;
}

void Selector::changeState(BYTE state, BYTE op)
{
	BYTE _tflag = flag;
	switch (op)
	{
	case SELOP_EQUAL:
		flag = state;
		break;
	case SELOP_SET:
		flag |= state;
		break;
	case SELOP_UNSET:
		flag &= ~state;
		break;
	}
	if (flag != _tflag)
	{
		timer = 0;
	}
}

void Selector::shift(int nshift)
{
	if (!nshift)
	{
		return;
	}
	float moveoffset = offset * nshift;
	for (list<Selector>::iterator it=sel.begin(); it!=sel.end(); it++)
	{
		if ((it->flag & SEL_STAY) || (it->flag & SEL_NOSHIFT))
		{
			continue;
		}
		for (int i=0; i<SEL_STATEMAX; i++)
		{
			if (updown)
			{
				it->info[i].yadj -= moveoffset;
			}
			else
			{
				it->info[i].xadj -= moveoffset;
			}
		}
		it->timer = 0;
	}
	firstID += nshift;
	while (firstID >= nselect)
	{
		firstID -= nselect;
	}
	lastID = firstID + (nPageNum-1);
	if (lastID >= nselect)
	{
		lastID = nselect - 1;
		firstID = lastID - (nPageNum-1);
	}
}

bool Selector::confirm(float cenx, float ceny, bool settrue)
{
	if(!confirminit)
	{
		Clear();
		Build(0, SpriteItemManager::yesIndex, cenx, ceny, 1, 0, 8,
			cenx-30, ceny+45,
			0, -4,
			0, 0,
			16, 4);
		Build(1, SpriteItemManager::noIndex, cenx, ceny, 1, 0, 8,
			cenx+30, ceny+45,
			0, -4,
			0, 0,
			16, 4);

		Build(2, SpriteItemManager::confirmIndex, cenx, ceny, 1, 0, 8,
			cenx, ceny,
			0, 0,
			0, 0,
			16, 4,
			true);

		Setup(2, settrue?0:1, false);
		confirminit = true;
	}

	if(hge->Input_GetDIKey(KS_SPECIAL_MP, DIKEY_UP) || (mp.playing && hge->Input_GetDIKey(KS_PAUSE_MP)))
	{
		SE::push(SE_SYSTEM_CANCEL);
		confirminit = false;
	}
	if(complete)
	{
		confirminit = false;
		if(select == 0)
			return true;
	}
	return false;
}

void Selector::matchSelect()
{
	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if(i->ID == select && (i->flag & SEL_NONACTIVE))
		{
			list<Selector>::iterator j = i;
			do
			{
				if(plus)
				{
					j++;
					if(j == sel.end())
						j++;
				}
				else
				{
					j--;
					if(j == sel.end())
						j--;
				}
				if(!((j->flag & SEL_NONACTIVE) || (j->flag & SEL_GRAY)))
				{
					select = j->ID;
					break;
				}
			}while(j!=i);
			break;
		}
	}
}

void Selector::Setup(int _nselect, int _select, bool _updown/* =true */)
{
	nselect = _nselect;
	select = _select;
	updown = _updown;
	PushKey::SetPushEvent(PUSHKEY_ID_SELUSE, updown?KS_UP_MP:KS_LEFT_MP, updown?KS_DOWN_MP:KS_RIGHT_MP);
}

void Selector::action()
{
	bool sub = false;
	if((flag & SEL_GRAY) && !(flag & SEL_NONACTIVE))
	{
		sub = true;
		flag &= ~SEL_GRAY;
	}
	if(!((flag & SEL_INFOMASK)>>1) && ID < nselect)
	{
		if(ID == select)
		{
			changeState(SEL_OVER, SELOP_SET);
		}
		else
		{
			changeState(SEL_OVER, SELOP_UNSET);
		}
	}

	int fvID = -1;
	for(list<Selector>::iterator i=sel.begin();i!=sel.end();i++)
	{
		if(!i->flag)
		{
			fvID = i->ID;
			break;
		}
	}
	if(ID == fvID && !(flag>>1) && !sub)
	{
		if(updown)
		{
			PushKey::UpdatePushEvent(PUSHKEY_ID_SELUSE);
			if(hge->Input_GetDIKey(KS_DOWN_MP, DIKEY_DOWN))
			{
				plus = true;
				SE::push(SE_SYSTEM_SELECT);
				select++;
				if(select == nselect)
					select = 0;
			}
			else if(hge->Input_GetDIKey(KS_UP_MP, DIKEY_DOWN))
			{
				plus = false;
				SE::push(SE_SYSTEM_SELECT);
				select--;
				if(select == -1)
					select = nselect - 1;
			}
		}
		else
		{
			PushKey::UpdatePushEvent(PUSHKEY_ID_SELUSE);
			if(hge->Input_GetDIKey(KS_RIGHT_MP, DIKEY_DOWN))
			{
				plus = true;
				SE::push(SE_SYSTEM_SELECT);
				select++;
				if(select == nselect)
					select = 0;
			}
			else if(hge->Input_GetDIKey(KS_LEFT_MP, DIKEY_DOWN))
			{
				plus = false;
				SE::push(SE_SYSTEM_SELECT);
				select--;
				if(select == -1)
					select = nselect - 1;
			}
		}
		matchSelect();
		if (nPageNum)
		{
			if (select < firstID)
			{
				shift(select - firstID);
			}
			else if (select > lastID)
			{
				shift(select - lastID);
			}
		}
		if(hge->Input_GetDIKey(KS_FIRE_MP, DIKEY_DOWN) && !avoid)
		{
			bool benter = false;
			for(list<Selector>::iterator i = sel.begin(); i != sel.end(); i++)
			{
				if(i->ID == select)
				{
					if(!(i->flag & SEL_NONACTIVE))
					{
						sellock = select;
						benter = true;
						break;
					}
				}
			}
			if(benter)
			{
				SE::push(SE_SYSTEM_OK);
				for(list<Selector>::iterator i = sel.begin(); i != sel.end(); i++)
				{
					if(i->ID == select)
						i->changeState(SEL_ENTER, SELOP_SET);
//						i->flag |= SEL_ENTER;
					else
						i->changeState(SEL_LEAVE, SELOP_SET);
//						i->flag |= SEL_LEAVE;
				}
			}
			else
			{
				SE::push(SE_SYSTEM_CANCEL);
			}
		}
		avoid = false;
	}

	int tindex = SELINFO_NONE;
	switch (flag & SEL_INFOMASK)
	{
	case SEL_OVER:
		tindex = SELINFO_OVER;
		break;
	case SEL_ENTER:
		tindex = SELINFO_ENTER;
		break;
	case SEL_LEAVE:
		tindex = SELINFO_LEAVE;
		break;
	}
	if (timer < maxtime)
	{
		x += (info[tindex].xadj - x) / (maxtime-timer);
		y += (info[tindex].yadj - y) / (maxtime-timer);
		switch (tindex)
		{
		case SELINFO_NONE:
			alpha = 0x30 * (maxtime-timer) / maxtime + 0xcf;
			break;
		case SELINFO_OVER:
			alpha = 0x10 * timer / maxtime + 0xef;
			break;
		case SELINFO_ENTER:
			alpha = 0xff;
			break;
		case SELINFO_LEAVE:
			alpha = 0x80 * (maxtime - timer) / maxtime;
			break;
		}
		timer++;
	}
	else
	{
		x = info[tindex].xadj;
		y = info[tindex].yadj;
		switch (tindex)
		{
		case SELINFO_NONE:
			alpha = ROLL(time, 0x3f) + 0x80;
			break;
		case SELINFO_OVER:
			alpha = ROLL(time, 0x3f) / 0x10 + 0xf0;
			break;
		case SELINFO_ENTER:
			alpha = 0xff;
			break;
		case SELINFO_LEAVE:
			alpha = 0;
			break;
		}
	}
	if(flag & SEL_ENTER)
	{
		select = sellock;
		if(flag & ~SEL_ENTER)
		{
			timer = maxtime;
			changeState(SEL_ENTER, SELOP_EQUAL);
		}
		if(timer == maxtime)
		{
			complete = true;
		}
	}
	if(flag & SEL_LEAVE)
	{
		if(flag & ~SEL_LEAVE)
		{
			timer = 0;
			changeState(SEL_LEAVE, SELOP_EQUAL);
			alpha = 0x80;
		}
	}
	
	if(flag & SEL_NONACTIVE)
	{
		if(flag & SEL_GRAY)
			alpha = 0x40;
		else
			alpha = 0xff;
	}

	//
	BYTE fademode = 0;
	if (nPageNum && !(flag & SEL_STAY))
	{
		float fadeend = fadebegin + (nPageNum+2) * offset;
		float fadeoffbegin = fadebegin + offset / 2;
		float fadeoffend = fadeend - offset / 2;
		if (updown)
		{
			if (y < fadebegin || y > fadeend)
			{
				alpha = 0;
			}
			else if (y <= fadeoffbegin)
			{
				fademode = SELFADE_UP;
			}
			else if (y >= fadeoffend)
			{
				fademode = SELFADE_DOWN;
			}
		}
		else
		{
			if (x < fadebegin || x > fadeend)
			{
				alpha = 0;
			}
			else if (x <= fadeoffbegin)
			{
				fademode = SELFADE_LEFT;
			}
			else if (x >= fadeoffend)
			{
				fademode = SELFADE_RIGHT;
			}
		}
	}

	sprite->SetColor((alpha << 24) | (0xffffff));

	if (fademode)
	{
		switch (fademode)
		{
		case SELFADE_UP:
			sprite->SetColor(0, 0);
			sprite->SetColor(0, 1);
			break;
		case SELFADE_DOWN:
			sprite->SetColor(0, 2);
			sprite->SetColor(0, 3);
			break;
		case SELFADE_LEFT:
			sprite->SetColor(0, 0);
			sprite->SetColor(0, 3);
			break;
		case SELFADE_RIGHT:
			sprite->SetColor(0, 1);
			sprite->SetColor(0, 2);
			break;
		}
	}

	if(sub)
	{
		flag |= SEL_GRAY;
	}
}