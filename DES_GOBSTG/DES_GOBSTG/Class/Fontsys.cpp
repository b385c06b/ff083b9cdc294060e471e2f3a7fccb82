#include "../Header/Fontsys.h"
#include "../Header/Main.h"
#include "../Header/Scripter.h"
#include "../Header/Data.h"
#include "../Header/SpriteItemManager.h"
#include "../Header/Player.h"
#include "../Header/BResource.h"

Fontsys Fontsys::fontsys;
//int Fontsys::changeID = -1;

Fontsys::Fontsys()
{
	font = NULL;
}

Fontsys::~Fontsys()
{
}

void Fontsys::Release()
{
	if (font)
	{
		delete font;
		font = NULL;
	}
/*
	for(int i=0; i<FONTSYSMAX; i++)
	{
		if(tar[i])
			hge->Target_Free(tar[i]);
		tar[i] = NULL;
	}*/

}

void Fontsys::Init(HD3DFONT _d3dfont)
{
	d3dfont = _d3dfont;
	Release();
//	changeID = -1;
}

void Fontsys::DoHeatUpBuffer( const char* str, list<int> * charcodelist )
{
	int nowindex = 0;
	int len = strlen(str);
	if (len)
	{
		while (nowindex < len)
		{
			int i=*(unsigned char*)(&(str[nowindex]));
			nowindex++;
			if (i > 0x7f && nowindex < len)
			{
				i = i << 8;
				i = i | (*(unsigned char*)(&(str[nowindex])));
				nowindex++;
			}
			else
			{
				continue;
			}
			bool tocontinue = false;
			list<int>::iterator it=charcodelist->begin();
			for (; it!=charcodelist->end(); it++)
			{
				if (*it == i)
				{
					tocontinue = true;
					break;
				}
				else if (*it > i)
				{
					break;
				}
			}
			if (tocontinue)
			{
				continue;
			}
			int _charcode = i;
			charcodelist->_Insert(it, _charcode);
//			charcodelist->push_back(_charcode);
		}
	}
}

void Fontsys::HeatUp(bool rebuildfont)
{
	if (rebuildfont)
	{
#ifdef __WIN32
		if (!d3dfont)
		{
			return;
		}

		list<int> charcodelist;

		for (int i=0; i<STRINGDESCMAX; i++)
		{
			DoHeatUpBuffer(Scripter::scr.strdesc[i], &charcodelist);
		}
		for (int i=0; i<PLAYERTYPEMAX; i++)
		{
			DoHeatUpBuffer(BResource::bres.playerdata[i].name, &charcodelist);
		}
		for (int i=0; i<ENEMYTYPEMAX; i++)
		{
			DoHeatUpBuffer(BResource::bres.enemydata[i].name, &charcodelist);
			DoHeatUpBuffer(BResource::bres.enemydata[i].ename, &charcodelist);
		}
		for (int i=0; i<BResource::bres.spelldata.size(); i++)
		{
			DoHeatUpBuffer(BResource::bres.spelldata[i].spellname, &charcodelist);
		}
		for (int i=0; i<MUSICMAX; i++)
		{
			DoHeatUpBuffer(BResource::bres.musdata[i].musicname, &charcodelist);
			DoHeatUpBuffer(BResource::bres.musdata[i].explain_1, &charcodelist);
			DoHeatUpBuffer(BResource::bres.musdata[i].explain_2, &charcodelist);
			DoHeatUpBuffer(BResource::bres.musdata[i].explain_3, &charcodelist);
			DoHeatUpBuffer(BResource::bres.musdata[i].explain_4, &charcodelist);
		}

		int * charcodes = (int *)malloc(charcodelist.size() * sizeof(int));
		int i = 0;
		for (list<int>::iterator it=charcodelist.begin(); it!=charcodelist.end(); it++, i++)
		{
			charcodes[i] = *it;
		}
		if (hgeFont::CreateFontFileByInfo(charcodes, i, BResource::bres.resdata.fontfilename, d3dfont))
		{
		}
		free(charcodes);
#endif // __WIN32
	}

	if (font)
	{
		delete font;
	}
	font = new hgeFont(BResource::bres.resdata.fontfilename);

	/*
	for (int i=0; i<scr.strdescIndex; i+=(i+1))
	{
		SignUp(FONTSYS_CHATUSE, strdesc[i]);
	}
	*/
	for (int i=0; i<PLAYERTYPEMAX; i++)
	{
		SignUp(i%FONTSYSMAX, Data::data.getPlayerName(i), 1.25f);
	}
	for (int i=0; i<ENEMYTYPEMAX; i++)
	{
		SignUp(i%FONTSYSMAX, Data::data.getEnemyName(i), 1.25f);
	}
}

void Fontsys::FocusChanged()
{
/*
	Release();
	SignUp(FONTSYS_CHATUSE, NULL, fontscale[FONTSYS_CHATUSE]);
	SignUp(FONTSYS_SPELLNAMEUSE, NULL, fontscale[FONTSYS_SPELLNAMEUSE]);
	if (changeID >= 0)
	{
		for (int i=0; i<=changeID; i++)
		{
			SignUp(i, NULL, fontscale[i]);
		}
	}*/

}

int Fontsys::strTranslate(char * dtext, const char * stext)
{
	int ret = 1;
	int j=0;
	strcpy(dtext, "");
	for(int i=0;i<(int)strlen(stext);i++)
	{
		if (stext[i] == '\n')
		{
			ret++;
		}
		if (stext[i]<0)
		{
			char twchar[3];
			twchar[0] = stext[i];
			i++;
			j+=2;
			twchar[1] = stext[i];
			twchar[2] = 0;
			strcat(dtext, twchar);
			continue;
		}
		if(stext[i] == FONTSYS_TRANSCHAR)
		{
			i++;
			if(stext[i] != FONTSYS_TRANSCHAR)
			{
				int strcodecon = stext[i] - '0';
				if(!strcodecon)
				{
					int strcodeval = (stext[i+1] - '0') * 10 +	(stext[i+2] - '0');
					while(j < strcodeval)
					{
						strcat(dtext, " ");
						j++;
					}
					i += 2;
					continue;
				}
				else
				{
					int strcodeval1 = (stext[i+1] - '0') * 10 +	(stext[i+2] - '0');
					int strcodeval2 = (stext[i+3] - '0') * 10 +	(stext[i+4] - '0');
					while(j < strcodeval1)
					{
						strcat(dtext, " ");
						j++;
					}
					i += 4;
					char buffer[32];
					int k = 0;
					while(!(stext[i+1] == FONTSYS_TRANSCHAR && stext[i+2] == FONTSYS_CONTROLCHAR))
					{
						i++;
						buffer[k] = stext[i];
						k++;
					}
					buffer[k] = 0;
					for(int l=k;l<strcodeval2-strcodeval1;l++)
					{
						if(strcodecon == 2)
							strcat(dtext, "0");
						else
							strcat(dtext, " ");
						j++;
					}
					strcat(dtext, buffer);
					i += 2;
					j += k;
					continue;
				}
			}
		}
		char tbuff[2];
		tbuff[0] = stext[i];
		tbuff[1] = 0;
		strcat(dtext, tbuff);
		j++;
	}
	return ret;
}

void Fontsys::SignUp(BYTE ID, const char * _text, float scale)
{
	if (_text != NULL)
	{
		strTranslate(text[ID], _text);
	}
	fontscale[ID] = scale;
/*
	if (!font)
	{
		return;
	}
	
		HTEXTURE tex = NULL;
	
		if (_text != NULL)
		{
			lines[ID] = strTranslate(text[ID], _text);
		}
		if (!tar[ID])
		{
			tar[ID] = hge->Target_Create(FONTSYS_TARGETWIDTH, FONTSYS_TARGETHEIGHT, false);
		}
	
		fontscale[ID] = scale;
		font->SetScale(scale);
	
		if (hge->Gfx_BeginScene(tar[ID]))
		{
			hge->Gfx_Clear(0x00000000);
			font->Render(0, 0, HGETEXT_LEFT|HGETEXT_TOP, text[ID]);
			hge->Gfx_EndScene();
			tex = hge->Target_GetTexture(tar[ID]);
	//		hge->Texture_Save(tex, "C:\\1.png", D3DXIFF_PNG);
		}
	//	hge->Gfx_RenderTextToTarget(&tex, tar[ID], _font, text[ID], 0, 0, FONTSYS_TARGETWIDTH, FONTSYS_TARGETHEIGHT);
	
		float h = lines[ID] * M_FONTHEIGHT;
	
		quad[ID].tex = tex;
		quad[ID].blend = BLEND_DEFAULT;
		quad[ID].v[0].tx = 0;	quad[ID].v[0].ty = 0;
		quad[ID].v[1].tx = 1;	quad[ID].v[1].ty = 0;
		quad[ID].v[2].tx = 1;	quad[ID].v[2].ty = h / FONTSYS_TARGETHEIGHT;
		quad[ID].v[3].tx = 0;	quad[ID].v[3].ty = h / FONTSYS_TARGETHEIGHT;
		quad[ID].v[0].z = quad[ID].v[1].z = quad[ID].v[2].z = quad[ID].v[3].z = 0;
		quad[ID].v[0].x = 0;					quad[ID].v[0].y = 0;
		quad[ID].v[1].x = FONTSYS_TARGETWIDTH;	quad[ID].v[1].y = 0;
		quad[ID].v[2].x = FONTSYS_TARGETWIDTH;	quad[ID].v[2].y = h;
		quad[ID].v[3].x = 0;					quad[ID].v[3].y = h;*/
	
}

void Fontsys::Render(BYTE ID, float x, float y, DWORD ucol, DWORD dcol, float shadow, float hext, float vext)
{
	if (shadow)
	{
		Render(ID, x+shadow, y+shadow, ucol&0xff000000, dcol&0xff000000, false, hext, vext);
//		Render(ID, x+shadow, y+shadow, ucol, dcol, false, hext, vext);
//		Render(ID, x+shadow, y-shadow, ucol, dcol, false, hext, vext);
//		Render(ID, x-shadow, y-shadow, ucol, dcol, false, hext, vext);
//		Render(ID, x-shadow, y+shadow, ucol, dcol, false, hext, vext);
	}
	font->SetScale(fontscale[ID]);
	font->SetColor(ucol, ucol, dcol, dcol);
	font->Render(x, y, HGETEXT_LEFT|HGETEXT_TOP, text[ID]);
/*
	if (!quad->tex.tex)
	{
		return;
	}
	if (shadow)
	{
		Render(ID, x+shadow*2, y+shadow*2, ucol&0xff000000, dcol&0xff000000, false, hext, vext);
		Render(ID, x+shadow, y+shadow, ucol, dcol, false, hext, vext);
		Render(ID, x+shadow, y-shadow, ucol, dcol, false, hext, vext);
		Render(ID, x-shadow, y-shadow, ucol, dcol, false, hext, vext);
		Render(ID, x-shadow, y+shadow, ucol, dcol, false, hext, vext);
	}
	float w = quad[ID].v[1].x - quad[ID].v[0].x;
	float h = quad[ID].v[2].y - quad[ID].v[0].y;

	quad[ID].v[0].col = quad[ID].v[1].col = ucol;
	quad[ID].v[2].col = quad[ID].v[3].col = dcol;

	quad[ID].v[0].x = x - hext;		quad[ID].v[0].y = y - vext;
	quad[ID].v[1].x = x + w + hext;	quad[ID].v[1].y = y - vext;
	quad[ID].v[2].x = x + w + hext;	quad[ID].v[2].y = y + h + vext;
	quad[ID].v[3].x = x - hext;		quad[ID].v[3].y = y + h + vext;

	SpriteItemManager::RenderQuad(&quad[ID]);

	quad[ID].v[0].x = x;		quad[ID].v[0].y = y;
	quad[ID].v[1].x = x + w;	quad[ID].v[1].y = y;
	quad[ID].v[2].x = x + w;	quad[ID].v[2].y = y + h;
	quad[ID].v[3].x = x;		quad[ID].v[3].y = y + h;*/

}