#include "InfoSelect.h"
#include "SE.h"
#include "FontSys.h"
#include "Main.h"
#include "Process.h"
#include "PushKey.h"

list<InfoSelect>infoselect;

int InfoSelect::nselect = 0;
int InfoSelect::select = 0;
bool InfoSelect::updown = true;
bool InfoSelect::complete = false;
bool InfoSelect::plus = true;

int InfoSelect::nPageNum = 0;
int InfoSelect::firstID;
int InfoSelect::lastID;
float InfoSelect::fadebegin;
float InfoSelect::offset;

bool InfoSelect::avoid = false;

InfoSelect::InfoSelect()
{
	nPageNum = 0;
}

InfoSelect::~InfoSelect()
{
}

void InfoSelect::linki(const char * linkcode, int num)
{
	if(!info)
		return;

	char buffer[M_STRITOAMAX];
	itoa(num, buffer, 10);
	if(!linkcode)
	{
		strcat(info, buffer);
		return;
	}
	strcat(info, linkcode);
	strcat(info, buffer);
	if(strlen(linkcode) == 6)
		strcat(info, FONTSYS_CONTROLSTR);
}

void InfoSelect::linkl(const char * linkcode, LONGLONG num)
{
	if(!info)
		return;

	char buffer[M_STRMAX];
	_i64toa(num, buffer, 10);
	if(!linkcode)
	{
		strcat(info, buffer);
		return;
	}
	strcat(info, linkcode);
	strcat(info, buffer);
	if(strlen(linkcode) == 6)
		strcat(info, FONTSYS_CONTROLSTR);
}

void InfoSelect::linkf(const char * linkcode, BYTE tail, float num)
{
	if(!info)
		return;

	char buffer[M_STRMAX];
	if(!tail)
	{
		sprintf(buffer, "%f", num);
	}
	else
	{
		char buff[M_STRMAX];
		char tbuf[M_STRITOAMAX];
		strcpy(buff, "%.");
		strcat(buff, itoa(tail, tbuf, 10));
		strcat(buff, "f");
		sprintf(buffer, buff, num);
	}
	if(!linkcode)
	{
		strcat(info, buffer);
		return;
	}
	strcat(info, linkcode);
	strcat(info, buffer);
	if(strlen(linkcode) == 6)
		strcat(info, FONTSYS_CONTROLSTR);
}

void InfoSelect::Clear()
{
	infoselect.clear();
	nPageNum = 0;
}

void InfoSelect::Setup(int _nselect, int _select, bool _updown/* =true */)
{
	nselect = _nselect;
	select = _select;
	updown = _updown;
	PushKey::SetPushEvent(PUSHKEY_ID_ISELUSE, updown?KS_UP_MP:KS_LEFT_MP, updown?KS_DOWN_MP:KS_RIGHT_MP);
}

void InfoSelect::shift(int nshift)
{
	if (!nshift)
	{
		return;
	}
	float moveoffset = offset * nshift;
	for (list<InfoSelect>::iterator it=infoselect.begin(); it!=infoselect.end(); it++)
	{
		if (it->flag & SEL_STAY)
		{
			continue;
		}
		if (updown)
		{
			it->y -= moveoffset;
		}
		else
		{
			it->x -= moveoffset;
		}
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

void InfoSelect::Render()
{
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
	{
		if(InfoSelect::select == i->ID && !(i->flag & SEL_NONACTIVE))
			Fontsys::Render(i->ID, i->x-2, i->y-2, i->ucol, i->dcol, 0.8f, 3, 1);
		else
			Fontsys::Render(i->ID, i->x, i->y, i->ucol, i->dcol, 0.8f);
	}
}

InfoSelect * InfoSelect::GetPointer(int index)
{
	if (index < 0 || index >= infoselect.size())
	{
		return NULL;
	}
	int tindex = 0;
	for (list<InfoSelect>::iterator i=infoselect.begin(); i!=infoselect.end(); i++)
	{
		if (index == tindex)
		{
			return &(*i);
		}
		tindex++;
	}
	return NULL;
}

void InfoSelect::valueSet(BYTE _ID, const char * _info, float _x, float _y, BYTE _coltype, BYTE _flag)
{
	ID = _ID;
	strcpy(info, _info);
	Fontsys::SignUp(ID, info);
	x = _x;
	y = _y;
	coltype = _coltype;
	flag = _flag;
	complete = false;
	plus = true;

	avoid = false;
}

void InfoSelect::Build(BYTE ID, const char * info, float x, float y, BYTE coltype, BYTE flag)
{
	InfoSelect _infoselect;

	_infoselect.valueSet(ID, info, x, y, coltype, flag);
	infoselect.push_back(_infoselect);
}

bool InfoSelect::SetPageNum(BYTE _nPageNum, float _fadebegin, float _offset, int initshift)
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

void InfoSelect::matchSelect()
{
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
	{
		if(i->ID == select && (i->flag & SEL_NONACTIVE))
		{
			list<InfoSelect>::iterator j = i;
			do
			{
				if(plus)
				{
					j++;
					if(j == infoselect.end())
						j++;
				}
				else
				{
					j--;
					if(j == infoselect.end())
						j--;
				}
				if(!(j->flag & SEL_NONACTIVE))
				{
					select = j->ID;
					break;
				}
			}while(j!=i);
			break;
		}
	}
}

void InfoSelect::action()
{
	if(flag & SEL_NONACTIVE)
	{
		/*
		if(select == ID)
		{
			matchSelect();
		}
		*/
		if(flag & SEL_GRAY)
			dcol = 0xce808080;
	}

	int fvID = -1;
	for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
	{
		if(!(i->flag & SEL_NONACTIVE))
		{
			fvID = i->ID;
			break;
		}
	}

	if(ID == fvID)
	{
		if(updown)
		{
			/*
			if(hge->Input_GetDIKey(KS_UP_MP) || hge->Input_GetDIKey(KS_DOWN_MP))
			{
				pushtimer++;
			}
			else
				pushtimer = 0;
			if(pushtimer == M_PUSH_FIRST)
				pushtimer = M_PUSH_ROLLTO;
			if(pushtimer == M_PUSH_ROLLTO)
			{
				hge->Input_SetDIKey(KS_UP_MP, false);
				hge->Input_SetDIKey(KS_DOWN_MP, false);
			}
			*/
			PushKey::UpdatePushEvent(PUSHKEY_ID_ISELUSE);
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
			/*
			if(hge->Input_GetDIKey(KS_LEFT_MP) || hge->Input_GetDIKey(KS_RIGHT_MP))
			{
				pushtimer++;
			}
			else
				pushtimer = 0;
			if(pushtimer == M_PUSH_FIRST)
				pushtimer = M_PUSH_ROLLTO;
			if(pushtimer == M_PUSH_ROLLTO)
			{
				hge->Input_SetDIKey(KS_LEFT_MP, false);
				hge->Input_SetDIKey(KS_RIGHT_MP, false);
			}
			*/
			PushKey::UpdatePushEvent(PUSHKEY_ID_ISELUSE);
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
			for(list<InfoSelect>::iterator i=infoselect.begin();i!=infoselect.end();i++)
			{
				if(i->ID == select)
				{
					if(!(i->flag & SEL_NONACTIVE))
					{
						benter = true;
						break;
					}
				}
			}
			if(benter)
			{
				SE::push(SE_SYSTEM_OK);
				complete = true;
			}
			else
			{
				SE::push(SE_SYSTEM_CANCEL);
			}
		}
		avoid = false;
	}

	if(select == ID && fvID >= 0)
	{
		dcol = 0xff4000ff;
	}
	else
	{ 
		if (!(flag & SEL_GRAY))
		{
			switch(coltype)
			{
			case INFO_GREEN:
				dcol = 0xff0066ff;
				break;
			case INFO_YELLOW:
				dcol = 0xffffff66;
				break;
			case INFO_GRAY:
				dcol = 0xce808080;
				break;
			case INFO_RED:
				dcol = 0xffff3333;
				break;
			case INFO_BLUE:
				dcol = 0xff6600ff;
			}
		}
	}

	ucol = 0xceffffff;
	if (nPageNum && !(flag & SEL_STAY))
	{
		float fadeend = fadebegin + (nPageNum+2) * offset;
		if (updown)
		{
			float fadeoffbegin = fadebegin + offset / 2;
			float fadeoffend = fadeend - offset / 2;
			if (y < fadebegin || y > fadeend)
			{
				ucol = dcol = 0;
			}
			else if (y <= fadeoffbegin)
			{
				ucol = 0;
			}
			else if (y >= fadeoffend)
			{
				dcol = 0;
			}
		}
		else
		{
			if (x < fadebegin || x > fadeend)
			{
				ucol = dcol = 0;
			}
		}
	}
}