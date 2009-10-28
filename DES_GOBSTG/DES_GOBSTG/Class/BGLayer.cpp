#include "BGLayer.h"
#include "Main.h"
#include "SpriteItemManager.h"
#include "BResource.h"

BGLayer * ubg[UBGLAYERMAX];
BGLayerSet BGLayer::set[BGLAYERSETMAX];
BGLayer bg[BGLAYERMAX];
BGLayer fg[FGLAYERMAX];

BGLayer bgmask;
BGLayer fgpause;

WORD BGLayer::setindex = 0;

BGLayer::BGLayer()
{
	exist	= false;
	sprite	= NULL;
}

BGLayer::~BGLayer()
{
	if (sprite)
	{
		delete sprite;
		sprite = NULL;
	}
}

void BGLayer::Init()
{
	for(int i=0;i<BGLAYERMAX;i++)
	{
		ubg[i] = & bg[i];
	}
	for(int i=0;i<FGLAYERMAX;i++)
	{
		ubg[i+BGLAYERMAX] = & fg[i];
	}
	ubg[BGMASKINDEX] = & bgmask;
	ubg[FGPAUSEINDEX] = & fgpause;

	for(int i=0; i<BGLAYERSETMAX; i++)
	{
		set[i].sID = 0;
		set[i].timer = 0;
	}
	setindex = 0;
}

void BGLayer::KillOtherLayer()
{
	for (int i=0; i<BGLAYERMAX; i++)
	{
		bg[i].exist = false;
	}
	for (int i=0; i<FGLAYERMAX; i++)
	{
		fg[i].exist = false;
	}
}

void BGLayer::SetBlend(int blend)
{
	sprite->SetBlendMode(blend);
}

void BGLayer::valueSetByName(HTEXTURE * tex, const char * spritename, float cenx, float ceny, float width, float height, DWORD col)
{
	valueSet(tex, SpriteItemManager::GetIndexByName(spritename), cenx, ceny, width, height, col);
}

void BGLayer::valueSet(HTEXTURE * tex, int siID, float cenx, float ceny, float width, float height, DWORD col)
{
	valueSet(tex, siID, cenx - (width>0?width/2:0), ceny - (height>0?height/2:0), 0, width, height, 0, 0, 0, 0, 0, 9000, false, false, col);
}

void BGLayer::valueSet(HTEXTURE * _tex, int siID, float x, float y, float z, float w, float h, int rotx, int roty, int rotz, float paral, float _speed /* = 0 */, int _angle /* = 9000 */, bool _move /* = false */, bool _rotate /* = false */, DWORD col /* = 0xffffffff */)
{
	speed	=	_speed;
	angle	=	_angle;
	hscale	=	1.0f;
	vscale	=	1.0f;
	flag	=	BG_NONE;
	timer	=	0;
	changetimer	=	0;

	spriteData * _sd = SpriteItemManager::CastSprite(siID);
	HTEXTURE tex = _tex[_sd->tex];


	float tx = (float)_sd->tex_x;
	float ty = (float)_sd->tex_y;
	tw = (float)_sd->tex_w;
	th = (float)_sd->tex_h;
	if (tw < 0 || th < 0)
	{
		tw = hge->Texture_GetWidth(tex) - tx;
		th = hge->Texture_GetHeight(tex) - ty;
	}
	if (w < 0)
	{
		w = tw;
		x -= w / 2;
	}
	if (h < 0)
	{
		h = th;
		y -= h / 2;
	}
	width	=	w;
	height	=	h;

	if (sprite)
	{
//		delete sprite;
		sprite->SetTexture(tex);
		sprite->SetTextureRect(tx, ty, tw, th);
	}
	else
	{
		sprite = new hgeSprite(tex, tx, ty, tw, th);
	}

	sprite->SetBlendMode(BLEND_DEFAULT);

	rectSet(x, y, z, w, h, rotx, roty, rotz);
	parallelogram(paral);
	sprite->SetColor(col);
	for (int i=0; i<4; i++)
	{
		ocol[i] = col;
	}


	exist = true;
	move = _move;
	rotate = _rotate;
}

void BGLayer::texRectSet(float texx, float texy, float texw, float texh)
{
	sprite->SetTextureRect(texx, texy, texw, texh);
	tw = texw;
	th = texh;
}

void BGLayer::scaleSet(float _hscale, float _vscale)
{
	hscale *= _hscale;
	vscale *= _vscale;
}

void BGLayer::zSet(float z0, float z1, float z2, float z3)
{
	sprite->SetZ(z0, z1, z2, z3);
}

void BGLayer::colorSet(DWORD col0, DWORD col1, DWORD col2, DWORD col3)
{
	sprite->SetColor(col0, col1, col2, col3);
	
	ocol[0] = col0;
	ocol[1] = col1;
	ocol[2] = col2;
	ocol[3] = col3;
}

void BGLayer::moveSet(bool _move, bool _rotate)
{
	move = _move;
	rotate = _rotate;
}

void BGLayer::rectSet(float _x, float _y, float z, float w, float h, int rotx, int roty, int rotz)
{
	if (hge->System_Is2DMode())
	{
		x = _x + width / 2;
		y = _y + height / 2;
		hscale = w / tw;
		vscale = h / th;
		return;
	}

	float wx = w, wy = 0, wz = 0;
	float hx = 0, hy = h, hz = 0;

	if (rotx)
	{
		float _hy = hy;
		hy = ROTATION1(_hy, 0, rotx);
		hz = ROTATION2(_hy, 0, rotx);
	}
	if (roty)
	{
		float _wx = wx;
		float _wz = wz;
		wx = ROTATION1(_wx, _wz, roty);
		wz = ROTATION2(_wx, _wz, roty);
	}
	if (rotz)
	{
		float _wx = wx;
		float _wy = wy;
		wx = ROTATION1(_wx, _wy, rotz);
		wy = ROTATION2(_wx, _wy, rotz);
		float _hx = hx;
		float _hy = hy;
		hx = ROTATION1(_hx, _hy, rotz);
		hy = ROTATION2(_hx, _hy, rotz);
	}

	
	sprite->quad.v[0].x	= _x;			sprite->quad.v[0].y = _y;			sprite->quad.v[0].z = z;
	sprite->quad.v[1].x	= _x + wx;		sprite->quad.v[1].y = _y + wy;		sprite->quad.v[1].z = z + wz;
	sprite->quad.v[2].x	= _x + wx + hx;	sprite->quad.v[2].y = _y + wy + hy;	sprite->quad.v[2].z = z + wz + hz;
	sprite->quad.v[3].x	= _x + hx;		sprite->quad.v[3].y = _y + hy;		sprite->quad.v[3].z = z + hz;
	
}

void BGLayer::parallelogram(float paral)
{
	if (hge->System_Is2DMode())
	{
		return;
	}
	sprite->quad.v[2].x += paral;
	sprite->quad.v[3].x += paral;
}

void BGLayer::vertexSet(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	if (hge->System_Is2DMode())
	{
		hscale = (x1-x0) / width;
		vscale = (y2-y1) / height;
		return;
	}
	sprite->quad.v[0].x = x0;	sprite->quad.v[0].y = y0;	sprite->quad.v[0].z = z0;
	sprite->quad.v[1].x = x1;	sprite->quad.v[1].y = y1;	sprite->quad.v[1].z = z1;
	sprite->quad.v[2].x = x2;	sprite->quad.v[2].y = y2;	sprite->quad.v[2].z = z2;
	sprite->quad.v[3].x = x3;	sprite->quad.v[3].y = y3;	sprite->quad.v[3].z = z3;
	
}

void BGLayer::SetFlag(BYTE _flag, BYTE maxtime)
{
	flag = _flag;
	mtimer = maxtime;
	changetimer = 0;
}

void BGLayer::Render()
{
	if (hge->System_Is2DMode())
	{
		sprite->RenderEx(x, y, rotate?ARC(angle-9000):0, hscale, vscale);
	}
	else
	{
		hge->Gfx_RenderQuad(&(sprite->quad));
	}
}

void BGLayer::action()
{
	timer++;

	float costa = cost(angle);
	float sinta = sint(angle);

	if(!move)
	{
		//roll the layer
		if(angle!=9000)
		{
			float xt = speed * costa;
			float yt = speed * sinta;

			sprite->quad.v[0].tx -= xt;	sprite->quad.v[0].ty -= yt;
			sprite->quad.v[1].tx -= xt;	sprite->quad.v[1].ty -= yt;
			sprite->quad.v[2].tx -= xt;	sprite->quad.v[2].ty -= yt;
			sprite->quad.v[3].tx -= xt;	sprite->quad.v[3].ty -= yt;
		}
		else if(speed != 0)
		{
			sprite->quad.v[0].ty -= speed;
			sprite->quad.v[1].ty -= speed;
			sprite->quad.v[2].ty -= speed;
			sprite->quad.v[3].ty -= speed;
		}
	}
	else if(!rotate)
	{
		//move the layer
		float xt = speed * costa;
		float yt = speed * sinta;

		if (hge->System_Is2DMode())
		{
			x += xt;
			y += yt;
		}
		else
		{
			sprite->quad.v[0].x += xt;	sprite->quad.v[0].y += yt;
			sprite->quad.v[1].x += xt;	sprite->quad.v[1].y += yt;
			sprite->quad.v[2].x += xt;	sprite->quad.v[2].y += yt;
			sprite->quad.v[3].x += xt;	sprite->quad.v[3].y += yt;
		}

	}
	else
	{
		if (!hge->System_Is2DMode())
		{
			//rotate the layer for XY-plane
			float x = ((sprite->quad.v[0].x + sprite->quad.v[2].x) / 2);
			float y = ((sprite->quad.v[0].y + sprite->quad.v[2].y) / 2);
					
			sprite->quad.v[0].x  = (-width/2)*costa - (-height/2)*sinta + x;
			sprite->quad.v[0].y  = (-width/2)*sinta + (-height/2)*costa + y;	

			sprite->quad.v[1].x  = (width/2)*costa - (-height/2)*sinta + x;
			sprite->quad.v[1].y  = (width/2)*sinta + (-height/2)*costa + y;	

			sprite->quad.v[2].x  = (width/2)*costa - (height/2)*sinta + x;
			sprite->quad.v[2].y  = (width/2)*sinta + (height/2)*costa + y;	

			sprite->quad.v[3].x  = (-width/2)*costa - (height/2)*sinta + x;
			sprite->quad.v[3].y  = (-width/2)*sinta + (height/2)*costa + y;	
		}
		if(angle > 0)
			angle += (int)(speed*100);
		else
			angle -= (int)(speed*100);
	}
	//set diffuse color for special usage
	if(flag)
	{
		changetimer++;
		if(changetimer == 1)
		{
			switch(flag)
			{
			case BG_WHITEFLASH:
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xc0ffffff;
				}
				break;
			case BG_REDFLASH:
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xc0ff0000;
				}
				break;
			case BG_YELLOWFLASH:
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xc0ffff00;
				}
				break;

			case BG_WHITEOUT:
				acol[0] = acol[1] = 0xffffffff;
				acol[2] = acol[3] = 0x80ffffff;
				break;
			case BG_REDOUT:
				acol[0] = acol[1] = 0xffff0000;
				acol[2] = acol[3] = 0x80ff0000;
				break;

			case BG_FADEIN:
				for (int i=0; i<4; i++)
				{
					acol[i] = ocol[i];
					ocol[i] = (0xff<<24)+(ocol[i]&0xffffff);
				}
				break;
			case BG_FADEINHALF:
				for (int i=0; i<4; i++)
				{
					acol[i] = ocol[i];
					ocol[i] = (0x80<<24)+(ocol[i]&0xffffff);
				}
				break;
			case BG_FADEOUT:
				for (int i=0; i<4; i++)
				{
					acol[i] = ocol[i];
					ocol[i] = (0x00<<24)+(ocol[i]&0xffffff);
				}
				break;

			case BG_LIGHTUP:
				for(int i=0;i<4;i++)
				{
					acol[i] = 0x00000000;
					ocol[i] = 0xffffffff;
				}
				break;
			case BG_LIGHTUPNORMAL:
				for (int i=0; i<4; i++)
				{
					acol[i] = 0xffffffff;
				}
				break;
			case BG_LIGHTRED:
				for(int i=0;i<4;i++)
				{
					acol[i] = 0xffff0000;
					ocol[i] = 0xffffffff;
				}
				break;

			case FG_PAUSEIN:
				for(int i=0;i<4;i++)
				{
					acol[i] = ocol[i];
					ocol[i] = 0xf0000000;
				}
				break;
			case FG_PAUSEOUT:
				for(int i=0;i<4;i++)
				{
					acol[i] = ocol[i];
					ocol[i] = 0x00000000;
				}
				break;
			}
		}
		
		if(changetimer < mtimer)
		{
			for(int i=0;i<4;i++)
			{
				int toa = GETA(ocol[i]);
				int tor = GETR(ocol[i]);
				int tog = GETG(ocol[i]);
				int tob = GETB(ocol[i]);
				int taa = GETA(acol[i]);
				int tar = GETR(acol[i]);
				int tag = GETG(acol[i]);
				int tab = GETB(acol[i]);

				sprite->SetColor(ARGB(
					(toa-taa)*changetimer/mtimer + taa,
					(tor-tar)*changetimer/mtimer + tar,
					(tog-tag)*changetimer/mtimer + tag,
					(tob-tab)*changetimer/mtimer + tab
					), i);
			}
		}
		if(changetimer == mtimer)
		{
			sprite->SetColor(ocol[0], ocol[1], ocol[2], ocol[3]);
			changetimer = 0;
			flag = BG_NONE;
		}
	}
}