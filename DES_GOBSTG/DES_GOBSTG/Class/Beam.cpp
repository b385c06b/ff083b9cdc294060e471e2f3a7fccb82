#include "../Header/Beam.h"
#include "../Header/Bullet.h"
#include "../Header/Player.h"
#include "../Header/SE.h"
#include "../Header/Chat.h"
#include "../Header/BossInfo.h"
#include "../Header/Item.h"
#include "../Header/Main.h"
#include "../Header/Target.h"
#include "../Header/Process.h"
#include "../Header/BResource.h"
#include "../Header/SpriteItemManager.h"

VectorList<Beam> Beam::be;
WORD Beam::index;

Beam::Beam()
{
	able	= false;
}

Beam::~Beam()
{
}

void Beam::Init()
{
	be.init(BEAMMAX);
	index = 0;
}

void Beam::ClearAll()
{
	be.clear_item();
}

void Beam::Action()
{
	if (be.getSize())
	{
		DWORD stopflag = Process::mp.GetStopFlag();
		bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_BEAM);
		if (!binstop)
		{
			DWORD i = 0;
			DWORD size = be.getSize();
			for (be.toBegin(); i<size; be.toNext(), i++)
			{
				if (!be.isValid())
				{
					continue;
				}
				if ((*be).exist)
				{
					Process::mp.objcount ++;

					(*be).action();
				}
				else
				{
					be.pop();
				}
			}
		}
	}
}

int Beam::Build(float x, float y, int angle, float speed, BYTE type, BYTE color, float length, float width, BYTE flag, int fadeintime, int fadeouttime, BYTE tarID)
{
	if (be.getSize() == BEAMMAX)
	{
		return 0;
	}
	Beam * _tbe;
	_tbe = be.push_back();
	index = be.getEndIndex();
	_tbe->valueSet(index, x, y, angle, speed, type, color, length, width, flag, fadeintime, fadeouttime, tarID);
	return index;
}

#ifdef __DEBUG
void Beam::Debug_RenderCollision()
{
	if (be.getSize())
	{
		DWORD i = 0;
		DWORD size = be.getSize();
		for (be.toBegin(); i<size; be.toNext(), i++)
		{
			if (be.isValid())
			{
				for (int j=M_ACTIVECLIENT_LEFT; j<M_ACTIVECLIENT_RIGHT; j++)
				{
					for (int k=M_ACTIVECLIENT_TOP; k<M_ACTIVECLIENT_BOTTOM; k++)
					{
						if ((*be).isInRect(j, k, 0))
						{
							hge->Gfx_RenderLine(j, k, j+1, k, 0xffffff00);
						}
					}
				}
			}
		}
	}
}
#endif

void Beam::RenderAll()
{
	if (be.getSize())
	{
		DWORD i = 0;
		DWORD size = be.getSize();
		for (be.toBegin(); i<size; be.toNext(), i++)
		{
			if (be.isValid())
			{
				(*be).Render();
			}
		}
	}
}

void Beam::Render()
{
	int i = type*BULLETCOLORMAX+color;
	if (Bullet::sprite[i])
	{
		int tblend = Bullet::sprite[i]->GetBlendMode();
		Bullet::sprite[i]->SetBlendMode(BLEND_ALPHAADD);
		Bullet::sprite[i]->SetColor(alpha<<24|diffuse);
		Bullet::sprite[i]->RenderEx(x, y, ARC(angle+headangle+BULLET_ANGLEOFFSET), hscale, vscale);
		Bullet::sprite[i]->SetBlendMode(tblend);
	}
}

void Beam::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, BYTE _color, float _length, float _width, BYTE _flag, int _fadeintime, int _fadeouttime, BYTE _tarID)
{
	ID			=	_ID;
	x			=	_x;
	y			=	_y;
	angle		=	_angle;
	speed		=	_speed;
	type		=	_type;
	color		=	_color;
	length		=	_length;
	width		=	_width;
	flag		=	_flag;
	fadeintime	=	_fadeintime;
	fadeouttime	=	_fadeouttime;

	tarID		=	_tarID;
	holdtar		=	0xff;
	pintar		=	0xff;
	holdoffset	=	0;

	timer			=	0;
	grazetimer		=	0;
	exist			=	true;
	fadeout			=	false;
	able			=	true;
	headangle		=	0;
	alpha			=	0xff;

	lastangle = angle;

	xplus = cost(angle);
	yplus = sint(angle);

	spriteData * spdata = SpriteItemManager::CastSprite(BResource::res.bulletdata[type].siid);
	texw	=	spdata->tex_w;
	texh	=	spdata->tex_h;

	if (!width)
	{
		width = texw;
	}

	hscale = width / texh;
	if(flag & BEAMFLAG_HORIZON)
	{
//		hscale = length / 16.0f;
//		vscale = 0.1f;
		/*
		if (fadeintime > 0)
		{
			hscale = hscale / fadeintime;
		}
		*/
		hscale = 0;
		vscale = length / texh;
		x += length * cost(angle) / 2;
		y += length * sint(angle) / 2;
		speed /= 100.0f;
	}
	else
	{
		hscale = 1;
		vscale = 0;
		xplus *= speed;
		yplus *= speed;
	}
}

void Beam::SetVector(const BObject &ori, const BObject &aim)
{
	SetVector(ori.x, ori.y, aim.x, aim.y);
}

void Beam::SetVector(float orix, float oriy, float aimx, float aimy)
{
	length = DIST(orix, oriy, aimx, aimy) + holdoffset;
	x = (orix + aimx) / 2;
	y = (oriy + aimy) / 2;
	vscale = length / texh;
	angle = aMainAngle(aimx, aimy);
}

void Beam::SetHold(BYTE _holdtar, float _holdoffset)
{
	holdtar = _holdtar;
	holdoffset = _holdoffset;
}

void Beam::SetHold(BYTE _holdtar, BYTE _pintar, float holdoffset)
{
	SetHold(_holdtar, holdoffset);
	pintar = _pintar;
}

void Beam::action()
{
	if(angle != lastangle)
	{
		xplus = speed * cost(angle);
		yplus = speed * sint(angle);
		lastangle = angle;
		lastspeed = speed;
	}
	timer++;

	if(!fadeout)
	{
		if(BossInfo::bossinfo.bossout())
		{
			fadeout = true;
			timer = 0;
		}
		if(timer == 1)
		{
			SE::push(SE_BEAM_1, x);
		}

		if (!Player::p.bInfi)
		{
			if (isInRect(Player::p.x, Player::p.y, Player::p.r))
			{
				Player::p.DoShot();
			}
		}
		if(!(flag & BEAMFLAG_HORIZON))
		{
			if(vscale * texh < length)
			{
				x += xplus / 2;
				y += yplus / 2;
				vscale += speed / texh;
			}
			else if(!(flag & BEAMFLAG_STOP))
			{
				x += xplus;
				y += yplus;
			}
		}
		else
		{
			if(timer < fadeintime)
			{
				hscale += width / texw / fadeintime;
			}
		}
		if ((flag & BEAMFLAG_HORIZON) || (flag & BEAMFLAG_STOP))
		{
			if(timer > (DWORD)fadeouttime)
			{
				fadeout = true;
				timer = 0;
			}
		}
		if (holdtar != 0xff)
		{
			if (pintar != 0xff)
			{
				SetVector(Target::tar[holdtar].x, Target::tar[holdtar].y, Target::tar[pintar].x, Target::tar[pintar].y);
			}
			else
			{
				float factor = (vscale * texh - holdoffset) / speed;
				SetVector(Target::tar[holdtar].x, Target::tar[holdtar].y, Target::tar[holdtar].x + factor * xplus, Target::tar[holdtar].y + factor * yplus);
			}
		}

		if(tarID != 0xff)
		{
			Target::SetValue(tarID, x, y);
		}

		if(Chat::chatitem.IsChatting())
		{
			fadeout = true;
			timer = 0;
		}

		if (!(flag & BEAMFLAG_NOGRAZE))
		{
			if(grazetimer)
			{
				grazetimer++;
				if(grazetimer == 6)
					grazetimer = 0;
			}
			else
			{
				grazetimer++;
				if(isInRect(Player::p.x, Player::p.y, Player::p.graze_r))
				{
					float itemx;
					float itemy;
					float tk = 0;
					if (xplus || yplus)
					{
						tk = - ((x - Player::p.x) * xplus + (y - Player::p.y) * yplus) / (xplus * xplus + yplus * yplus);
						itemx = x + xplus * tk;
						itemy = y + yplus * tk;
					}
					else
					{
						itemx = Player::p.x;
						itemy = Player::p.y;
					}
					Player::p.DoGraze(itemx, itemy);
				}
			}

		}
		float nowlength = vscale * texh;
		float nowwidth = hscale * texw;
		float longside = nowlength > nowwidth ? nowlength : nowwidth;
		if(	x - longside > M_DELETECLIENT_RIGHT ||
			x + longside < M_DELETECLIENT_LEFT ||
			y - longside > M_DELETECLIENT_BOTTOM ||
			y + longside < M_DELETECLIENT_TOP)
			exist = false;
	}
	else
	{
		if(timer == 1)
		{
			SE::push(SE_BEAM_FADEOUT, x);
		}
		if(timer == 32)
			exist = false;
		else
		{
			vscale -= width / texw / 32;//0.03125;
			alpha = (BYTE)(32-timer) * 8 - 1;
		}
	}
	able = exist && !fadeout;
}

bool Beam::isInRect(float aimx, float aimy, float r)
{
	if(vscale < BEAM_INVALIDSCALE)
		return false;

	float _x = x;
	float _y = y;

	float rl = BResource::res.bulletdata[type].collisionMain;
	float rs = BResource::res.bulletdata[type].collisionSub;

	float nowrl = vscale * rl;

	float cx = _x;
	float cy = _y;

	//	nowlength *= 0.3125f;

	float rotCos;
	float rotSin;
	if (!(flag & BEAMFLAG_HORIZON) && speed != 0)
	{
		rotCos = xplus / speed;
		rotSin = yplus / speed;
	}
	else
	{
		rotCos = cost(angle);
		rotSin = sint(angle);
	}
	float nowrs = hscale * rs;
	return CheckCollisionEllipse(_x, _y, aimx, aimy, nowrs, nowrl, rotCos, rotSin, r);
}