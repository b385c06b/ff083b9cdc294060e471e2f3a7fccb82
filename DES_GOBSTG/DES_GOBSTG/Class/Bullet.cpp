#include "../Header/Bullet.h"
#include "../Header/Player.h"
#include "../Header/SE.h"
#include "../Header/Item.h"
#include "../Header/BossInfo.h"
#include "../Header/Scripter.h"
#include "../Header/Main.h"
#include "../Header/BResource.h"
#include "../Header/SpriteItemManager.h"
#include "../Header/Target.h"
#include "../Header/Process.h"
#include "../Header/BulletListActionConst.h"

#define _IZEZONEMAX			0x20

RenderDepth Bullet::renderDepth[BULLETTYPEMAX];

Bullet Bullet::_bu;
hgeSprite * Bullet::sprite[BULLETTYPECOLORMAX];
VectorList<IzeZone> Bullet::izel;

VectorList<Bullet> Bullet::bu;
HTEXTURE Bullet::tex;

WORD Bullet::index;

Bullet::Bullet()
{
	able	= false;
}

Bullet::~Bullet()
{
}

void Bullet::Init(HTEXTURE _tex)
{
	Release();

	ZeroMemory(renderDepth, sizeof(RenderDepth) * BULLETTYPEMAX);
	bu.init(BULLETMAX);
	tex = _tex;
	for (int i=0; i<BULLETTYPEMAX; i++)
	{
		bulletData * tbd = &BResource::res.bulletdata[i];
		int tnum = tbd->nRoll;
		if (tnum < 2)
		{
			tnum = BResource::res.bulletdata[i].nColor;
		}
		int j=0;
		int index;
		for (j=0; j<tnum; j++)
		{
			index = i*BULLETCOLORMAX+j;
			sprite[index] = SpriteItemManager::CreateSprite(tbd->siid+j);
			sprite[index]->SetBlendMode(tbd->blendtype);
			if (BResource::res.bulletdata[i].collisiontype != BULLET_COLLISION_ELLIPSE && tbd->collisionSub)
			{
				float tex_w = SpriteItemManager::GetTexW(tbd->siid+j);
				float tex_h = SpriteItemManager::GetTexH(tbd->siid+j);
				sprite[index]->SetHotSpot((tex_w)/2.0f, (tex_h)/2.0f+tbd->collisionSub);
			}
		}
		for (; j<BULLETCOLORMAX; j++)
		{
			index = i*BULLETCOLORMAX+j;
			sprite[index] = SpriteItemManager::CreateNullSprite();
			SpriteItemManager::SetSpriteData(sprite[index], tex, 0, 0, 0, 0);
		}
	}

	izel.init(_IZEZONEMAX);
	izel.clear_item();

	index = 0;
}

void Bullet::BuildCircle(int num, int baseangle, float baser, float x, float y, float speed, BYTE type, BYTE color, int fadeinTime, float avoid)
{
	if (num <= 0)
	{
		return;
	}
	int anglestep = 36000 / num;
	for (int i=0; i<num; i++)
	{
		int tnowangle = baseangle + i * anglestep;
		float tx = x + cost(tnowangle) * baser;
		float ty = y + sint(tnowangle) * baser;
		Bullet::Build(tx, ty, tnowangle, speed, type, color, fadeinTime, avoid, 0xff);
	}
}

void Bullet::BuildLine(int num, int baseangle, float space, int baseindex, float x, float y, int angle, float anglefactor, float speed, float speedfactor, BYTE type, BYTE color, int fadeinTime, float avoid)
{
	if (num <= 0)
	{
		return;
	}
	for (int i=0; i<num; i++)
	{
		int tindex = i - baseindex;
		float tx = x + tindex * cost(baseangle) * space;
		float ty = y + tindex * sint(baseangle) * space;
		Bullet::Build(tx, ty, angle + anglefactor * tindex, speed + speedfactor * abs(tindex), type, color, fadeinTime, avoid, 0xff);
	}
}

int Bullet::Build(float x, float y, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid, BYTE tarID)
{
	if (bu.getSize() == BULLETMAX)
	{
		return -1;
	}
	Bullet * _tbu = NULL;
	_tbu = bu.push_back();
	index = bu.getEndIndex();
	if (!_tbu->valueSet(index, x, y, angle, speed, type, color, fadeinTime, avoid, tarID))
	{
		bu.pop(index);
		return false;
	}
	memcpy(_tbu->actionList, _bu.actionList, BULLETACTIONMAX*sizeof(int));
	return index;
}

void Bullet::Release()
{
	bu.clear();
	for(int i=0;i<BULLETTYPECOLORMAX;i++)
	{
		SpriteItemManager::FreeSprite(&sprite[i]);
	}
}

void Bullet::Action()
{
	if (bu.getSize())
	{
		ZeroMemory(renderDepth, sizeof(RenderDepth) * BULLETTYPEMAX);
		DWORD i = 0;
		DWORD size = bu.getSize();
		for (bu.toBegin(); i<size; bu.toNext(), i++)
		{
			if (!bu.isValid())
			{
				continue;
			}
			if ((*bu).exist)
			{
				Process::mp.objcount++;

				DWORD stopflag = Process::mp.GetStopFlag();
				bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_BULLET);
				if (!binstop)
				{
					(*bu).action();
				}
				else
				{
					(*bu).actionInStop();
				}
			}
			else
			{
				bu.pop();
			}
		}
	}
	IzelAction();
}

void Bullet::IzelAction()
{
	DWORD stopflag = Process::mp.GetStopFlag();
	bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_BULLET);
	if (!(stopflag & FRAME_STOPFLAG_BULLET))
	{
		if (izel.getSize())
		{
			DWORD i = 0;
			DWORD size = izel.getSize();
			for (izel.toBegin(); i<size; izel.toNext(), i++)
			{
				if (izel.isValid())
				{
					IzeZone * tize = &(*(izel));
					tize->timer++;
					if (tize->timer == tize->maxtime)
					{
						izel.pop();
					}
				}
			}
		}
	}
}

void Bullet::ClearAll()
{
	bu.clear_item();
	index = 0;
	izel.clear_item();
	ZeroMemory(_bu.actionList, sizeof(int) * BULLETACTIONMAX);
}

void Bullet::RenderAll()
{
	if (bu.getSize())
	{
		for (int i=0; i<BULLETTYPEMAX; i++)
		{
			if (renderDepth[i].haveType)
			{
				for (bu.toIndex(renderDepth[i].startIndex); bu.index != renderDepth[i].endIndex; bu.toNext())
				{
					if (bu.isValid() && (*bu).getRenderDepth() == i)
					{
						(*bu).Render();
					}
				}
			}
		}
	}
}

void Bullet::Render()
{
	int i = type*BULLETCOLORMAX + color;
	if (sprite[i])
	{
		sprite[i]->SetColor(alpha<<24 | diffuse);
		sprite[i]->RenderEx(x, y, ARC(angle+headangle+BULLET_ANGLEOFFSET), hscale);
	}
}

BYTE Bullet::getRenderDepth()
{
	return BResource::res.bulletdata[type].renderdepth;
}

void Bullet::matchFadeInColorType()
{
	if( BResource::res.bulletdata[type].fadecolor < BULLETCOLORMAX)
	{
		color = BResource::res.bulletdata[type].fadecolor;
//		type = BULLET_FADEINTYPE;
		changeType(BULLET_FADEINTYPE);
	}
	else if (BResource::res.bulletdata[type].fadecolor == BULLET_FADECOLOR_16)
	{
//		type = BULLET_FADEINTYPE;
		changeType(BULLET_FADEINTYPE);
		if (color == 0)
		{
		}
		else if (color < 9)
		{
			color = (color+1)/2;
		}
		else
		{
			color = (color-9)/3+5;
		}
	}
	else if (BResource::res.bulletdata[type].fadecolor == BULLET_FADECOLOR_8)
	{
		changeType(BULLET_FADEINTYPE);
//		type = BULLET_FADEINTYPE;
	}
}
void Bullet::matchFadeOutColorType()
{
	if (BResource::res.bulletdata[type].fadecolor < BULLETCOLORMAX)
	{
		color = BResource::res.bulletdata[type].fadecolor;
		changeType(BULLET_FADEOUTTYPE);
//		type = BULLET_FADEOUTTYPE;
	}
	else if (BResource::res.bulletdata[type].fadecolor == BULLET_FADECOLOR_16)
	{
		changeType(BULLET_FADEOUTTYPE);
//		type = BULLET_FADEOUTTYPE;
		if (color == 0)
		{
		}
		else if (color < 9)
		{
			color = (color+1)/2;
		}
		else
		{
			color = (color-9)/3+5;
		}
	}
	else if (BResource::res.bulletdata[type].fadecolor == BULLET_FADECOLOR_8)
	{
		changeType(BULLET_FADEOUTTYPE);
//		type = BULLET_FADEOUTTYPE;
	}
}

bool Bullet::valueSet(WORD _ID, float _x, float _y, int _angle, float _speed, BYTE _type, BYTE _color, int _fadeinTime, float avoid, BYTE _tarID)
{
	ID			=	_ID;
	x			=	_x;
	y			=	_y;
	changeType(_type);
//	type		=	_type;
	if(avoid)
	{
		if(isInRect(avoid, Player::p.x, Player::p.y))
			return false;
	}
	angle	=	_angle;
	speed		=	_speed;
	oldtype		=	type;
	color		=	_color;
	oldcolor	=	color;
	fadeinTime	=	_fadeinTime;
	bouncetime	=	0;

	for (int i=0; i<BULLET_EVENTMAX; i++)
	{
		eventID[i] = 0xff;
	}

	tarID	=	_tarID;

	matchFadeInColorType();

	timer			=	0;
	typechangetimer	=	0;
	exist			=	true;
	grazed			=	false;
	fadeout			=	false;
	able			=	true;
	remain			=	false;
	actionList[0]	=	SECTIONEND;
	hscale			=	1.0f;
	headangle		=	0;
	alpha			=	0xff;
	toafter			=	0;
	cancelable		=	true;

	xplus = speed * cost(angle);
	yplus = speed * sint(angle);
	lastx = x;
	lasty = y;
	lastangle = angle;
	lastspeed = speed;

	return true;
}

void Bullet::IzeBuild(BYTE _type, float _x, float _y, BYTE _maxtime, float _r, BYTE _eventID)
{
	IzeZone _ize;
	_ize.x = _x;
	_ize.y = _y;
	_ize.r = _r;
	_ize.maxtime = _maxtime;
	_ize.type = _type;
	_ize.timer = 0;
	_ize.eventID = _eventID;
	izel.push_back(_ize);
}

void Bullet::DoIze()
{
	if (cancelable || BossInfo::bossinfo.bossout())
	{
		if (izel.getSize())
		{
			DWORD i = 0;
			DWORD size = izel.getSize();
			for (izel.toBegin(); i<size; izel.toNext(), i++)
			{
				if (izel.isValid())
				{
					IzeZone * tize = &(*(izel));
					float tor = tize->r;
					if (tize->maxtime > 1)
					{
						if (tize->timer > 0)
						{
							tor *= tize->timer;
						}
						tor /= tize->maxtime;
					}


					if (checkCollisionBigCircle(tize->x, tize->y, tor))
					{
						switch (tize->type)
						{
						case BULLETIZE_FADEOUT:
							fadeout = true;
							timer = 0;
							break;
						case BULLETIZE_FAITH:
							faithlize();
							break;
						case BULLETIZE_POINT:
							pointlize();
							break;
						case BULLETIZE_SCORE:
							scorelize();
							break;
						}
						if (tize->eventID != 0xff)
						{
							if (!passedEvent(tize->eventID))
							{
								Scripter::scr.eventExecute(SCR_EVENT_BULLETENTERIZE, tize->eventID);
								passEvent(tize->eventID);
							}
						}
						break;
					}
				}
			}
		}
	}
}

void Bullet::DoGraze()
{
	if(!grazed && BResource::res.bulletdata[type].collisiontype != BULLET_COLLISION_NONE)
	{
		if((Player::p.x - x) * (Player::p.x - x) + (Player::p.y - y) * (Player::p.y - y) < Player::p.graze_r * Player::p.graze_r)
		{
			Player::p.DoGraze(x, y);
			grazed = true;
		}
	}
}

void Bullet::DoCollision()
{
	if(isInRect(Player::p.r, Player::p.x, Player::p.y))
	{
		if (cancelable)
		{
			fadeout = true;
			timer = 0;
		}
		Player::p.DoShot();
	}
}

void Bullet::DoUpdateRenderDepth()
{
	if (exist)
	{
		BYTE rdtype = getRenderDepth();
		if (!renderDepth[rdtype].haveType)
		{
			renderDepth[rdtype].haveType = true;
			renderDepth[rdtype].startIndex = index;
		}
		renderDepth[rdtype].endIndex = index + 1;
		if (renderDepth[rdtype].endIndex == BULLETMAX)
		{
			renderDepth[rdtype].endIndex = 0;
		}
	}
}

bool Bullet::HaveGray()
{
	if (!(BResource::res.bulletdata[type].nColor % 8))
	{
		return true;
	}
	return false;
}

void Bullet::actionInStop()
{
	index = ID;
	if (!fadeout)
	{
		DoIze();
		if (timer > fadeinTime)
		{
			DoGraze();
		}
	}
	DoUpdateRenderDepth();
}

bool Bullet::passedEvent(BYTE _eventID)
{
	for (int i=0; i<BULLET_EVENTMAX; i++)
	{
		if (eventID[i] == _eventID)
		{
			return true;
		}
		else if (eventID[i] == 0xff)
		{
			return false;
		}
	}
	return false;
}

void Bullet::passEvent(BYTE _eventID)
{
	for (int i=0; i<BULLET_EVENTMAX; i++)
	{
		if (eventID[i] == 0xff)
		{
			eventID[i] = _eventID;
			return;
		}
	}
	eventID[0] = _eventID;
}

void Bullet::changeType(BYTE totype)
{
	if (type != totype)
	{
		type = totype;
	}
}

void Bullet::action()
{
	index = ID;
	if(angle != lastangle || lastspeed == 0)
	{
		xplus = speed * cost(angle);
		yplus = speed * sint(angle);
	}
	else if(speed != lastspeed)
	{
		xplus *= speed / lastspeed;
		yplus *= speed / lastspeed;
	}
	lastangle = angle;
	lastspeed = speed;
	
	timer++;

	lastx = x;
	lasty = y;


	if(!fadeout)
	{
		if(timer == 1 && fadeinTime > 1)
		{
			if(oldtype != type)
			{
				hscale *= (fadeinTime + 15.0f) * 0.0625f;
			}
			else
			{
				hscale /= fadeinTime;
			}
		}
		else if(fadeinTime > 0 && timer < (DWORD)fadeinTime)
		{
			if(oldtype != type)
			{
				hscale -= 0.0625f;
				alpha = 0x80;
			}
			else
			{
				hscale += 1.0f / fadeinTime;
			}
		}
		else if(timer == fadeinTime || fadeinTime < 0)
		{
			if (fadeinTime >= 0)
			{
				if(oldtype != type)
				{
					hscale -= 0.0625f;
				}
				else
				{
					hscale += 1.0f / fadeinTime;
				}
			}
			else
			{
				fadeinTime = 1;
			}
			changeType(oldtype);
			color = oldcolor;
			alpha = 0xff;
			SE::push(BResource::res.bulletdata[type].seID, x);
		}
		else
		{
			if (able)
			{
				DoCollision();
				DoGraze();
			}

			x += xplus;
			y += yplus;

			if (able)
			{
				ChangeAction();

				if (BResource::res.bulletdata[type].nRoll && !(timer % BULLET_ANIMATIONSPEED))
				{
					color++;
					if (color >= BResource::res.bulletdata[type].nRoll)
					{
						color = 0;
					}
				}

				if(typechangetimer)
				{
					if(typechangetimer == 1)
					{
						hscale *= (BULLET_TYPECHANGETIME + 15.0f) * 0.0625f;
						matchFadeInColorType();
					}
					if(typechangetimer < BULLET_TYPECHANGETIME)
					{
						typechangetimer++;
						hscale -= 0.0625f;
						alpha = 0x80;
					}
					else
					{
						hscale -= 0.0625f;
						changeType(oldtype);
//						type = oldtype;
						color = oldcolor;
						typechangetimer = 0;
						alpha = 0xff;
					}
				}
			}
		}

		DoIze();
		headangle += SIGN(color) * BResource::res.bulletdata[type].nTurnAngle;
		if(tarID != 0xff)
		{
			Target::SetValue(tarID, x, y);
		}

		if(!remain)
		{
			if(x > M_DELETECLIENT_RIGHT || x < M_DELETECLIENT_LEFT || y > M_DELETECLIENT_BOTTOM || y < M_DELETECLIENT_TOP)
				exist = false;
		}
	}
	else
	{
		if(timer == 16)
		{
			if(toafter == BULLETIZE_FAITH)
			{
				Item::Build(ITEM_SMALLFAITH, x, y, true);
				exist = false;
			}
			else if (toafter == BULLETIZE_POINT)
			{
				Item::Build(ITEM_POINT, x, y, false);
				exist = false;
			}
			else if(toafter == BULLETIZE_SCORE)
			{
				color = BResource::res.bulletdata[oldtype].bonuscolor;
				switch(color)
				{
				case 0:
					Player::p.nScore += 500;
					break;
				case 1:
					Player::p.nScore += 1000;
					break;
				case 2:
					Player::p.nScore += 2000;
					break;
				case 3:
					Player::p.nScore += 3000;
					break;
				case 4:
					Player::p.nScore += 5000;
					break;
				case 5:
					Player::p.nScore += 8000;
					break;
				}
				type = BULLET_BONUSTYPE;
				alpha = 0xff;
				hscale = 1;
				angle = -BULLET_ANGLEOFFSET;
				headangle = 0;
				timer = 32;
			}
		}
		else if(timer == 32)
		{
			exist = false;
		}
		else if(timer == 96)
			exist = false;
		else if(timer < 32)
		{
			diffuse = 0xffffff;
			matchFadeOutColorType();
			hscale = timer / 60.0f + 0.6f;
			alpha = (BYTE)(32-timer) * 4 - 1;
		}
		else if(timer > 32 && timer < 96)
		{
			if(timer > 64)
				alpha = (BYTE)(96-timer) * 8 - 1;
			y -= 0.2f;
		}
	}
	able = exist && !fadeout;

	DoUpdateRenderDepth();
}

void Bullet::faithlize()
{
	timer = 0;
	fadeout = true;
	able = false;
	toafter = BULLETIZE_FAITH;
}

void Bullet::pointlize()
{
	timer = 0;
	fadeout = true;
	able = false;
	toafter = BULLETIZE_POINT;
}

void Bullet::scorelize()
{
	timer = 0;
	fadeout = true;
	able = false;
	toafter = BULLETIZE_SCORE;
}

bool Bullet::ChangeAction()
{
	bool doit = false;
	/*
	bool conbyval = true;
	bool exebyval = true;
#define _CONACL_(X)	(conbyval ? actionList[i+(X)]: Scripter::scr.GetIntValue(i+(X)))
#define _EXEACL_(X)	(exebyval ? actionList[i+(X)]: Scripter::scr.GetIntValue(i+(X)))
#define _ACL_(X)	(actionList[i+(X)])
#define _SAVECON_(X, V)	(conbyval ? actionList[i+(X)]=(V) : Scripter::scr.SetIntValue(i+(X), V))
#define _SAVEEXE_(X, V)	(exebyval ? actionList[i+(X)]=(V) : Scripter::scr.SetIntValue(i+(X), V))
	*/
	int usingtimer = timer;
#define _CONACL_(X)	(actionList[i+(X)])
#define _EXEACL_(X)	(actionList[i+(X)])
#define _ACL_(X)	(actionList[i+(X)])
#define _SAVECON_(X, V)	(actionList[i+(X)]=(V))
#define _SAVEEXE_(X, V)	(actionList[i+(X)]=(V))
	for(int i=0;i<BULLETACTIONMAX;++i)
	{
		if (actionList[i] < BULA_SPECIALSTART)
		{
			switch (actionList[i])
			{
			case AND:
				if (!doit)
				{
					for (++i; i<BULLETACTIONMAX; ++i)
					{
						if (_ACL_(0) == THEN)
						{
							break;
						}
					}
				}
				else
				{
					doit = false;
				}
				break;
			case OR:
				if (doit)
				{
					for (++i; i<BULLETACTIONMAX; ++i)
					{
						if(_ACL_(0) == THEN)
						{
							break;
						}
					}
				}
				else
				{
					doit = false;
				}
				break;
			case NOT:
				doit = !doit;
				break;
//			case ANDSET:
//				doit = true;
//				break;
			case CONDITIONBYVAL:
//				conbyval = true;
				break;
			case CONDITIONBYINDEX:
//				conbyval = false;
				break;
			case EXECUTEBYVAL:
//				exebyval = true;
				break;
			case EXECUTEBYINDEX:
//				exebyval = false;
				break;
			}
		}
		else if (_ACL_(0) < BULA_EXECUTESTART)
		{
			switch (_ACL_(0) & BULAC_FILTER)
			{
			case BULAC_OTHER:
				switch (_ACL_(0))
				{
				case SECTIONEND:
					i = BULLETACTIONMAX-1;
					break;

				case EVERY:
					doit = true;
					break;
				case EVERYMOD:
					if(usingtimer % _CONACL_(1) == 0)
						doit = true;
					++i;
					break;
				}
				break;
			case BULAC_TIMER:
				switch (actionList[i])
				{
				case TIMERGREAT:
					if(usingtimer >= _CONACL_(1))
						doit = true;
					++i;
					break;
				case TIMEREQUAL:
					if(usingtimer == _CONACL_(1))
						doit = true;
					++i;
					break;
				case TIMERLESS:
					if(usingtimer <= _CONACL_(1))
						doit = true;
					++i;
					break;
				case TIMERRANGE:
					if(usingtimer >= _CONACL_(1) && usingtimer <= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_TYPE:
				switch (_ACL_(0))
				{
				case TYPEEQUAL:
					if(type == _CONACL_(1))
						doit = true;
					++i;
					break;
				}
				break;
			case BULAC_COLOR:
				switch (_ACL_(0))
				{
				case COLOREQUAL:
					if(color == _CONACL_(1))
						doit = true;
					++i;
					break;
				}
				break;
			case BULAC_ANGLE:
				switch (_ACL_(0))
				{
				case ANGLEGREAT:
					if(angle >= _CONACL_(1))
						doit = true;
					++i;
					break;
				case ANGLEEQUAL:
					if(angle == _CONACL_(1))
						doit = true;
					++i;
					break;
				case ANGLELESS:
					if(angle <= _CONACL_(1))
						doit = true;
					++i;
					break;
				case ANGLERANGE:
					if(angle >= _CONACL_(1) && angle <= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_POS:
				switch (_ACL_(0))
				{
				case XGREAT:
					if(x >= _CONACL_(1))
						doit = true;
					++i;
					break;
				case XLESS:
					if(x <= _CONACL_(1))
						doit = true;
					++i;
					break;
				case XRANGE:
					if(x >= _CONACL_(1) && x <= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				case YGREAT:
					if(y >= _CONACL_(1))
						doit = true;
					++i;
					break;
				case YLESS:
					if(y <= _CONACL_(1))
						doit = true;
					++i;
					break;
				case YRANGE:
					if(y >= _CONACL_(1) && y <= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_VAL:
				switch (_ACL_(0))
				{
				case VALGREAT:
					if(CAST(Scripter::scr.d[(int)_CONACL_(1)]) >= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				case VALEQUAL:
					if(CAST(Scripter::scr.d[(int)_CONACL_(1)]) == _CONACL_(2))
						doit = true;
					i+=2;
					break;
				case VALLESS:
					if(CAST(Scripter::scr.d[(int)_CONACL_(1)]) <= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				case VALRANGE:
					if(CAST(Scripter::scr.d[(int)_CONACL_(1)]) >= _CONACL_(2) && CAST(Scripter::scr.d[(int)_CONACL_(1)]) <= _CONACL_(3))
						doit = true;
					i+=3;
					break;
				}
				break;
			case BULAC_SPEED:
				switch (_ACL_(0))
				{
				case SPEEDGREAT:
					if(speed*BULLETACT_FLOATSCALE >= _CONACL_(1))
						doit = true;
					++i;
					break;
				case SPEEDEQUAL:
					if(speed*BULLETACT_FLOATSCALE == _CONACL_(1))
						doit = true;
					++i;
					break;
				case SPEEDLESS:
					if(speed*BULLETACT_FLOATSCALE <= _CONACL_(1))
						doit = true;
					++i;
					break;
				case SPEEDRANGE:
					if(speed*BULLETACT_FLOATSCALE >= _CONACL_(1) && speed*BULLETACT_FLOATSCALE <= _CONACL_(2))
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_INDEX:
				switch (_ACL_(0))
				{
				case INDEXMODGREAT:
					if (index % _CONACL_(1) >= _CONACL_(2))
					{
						doit = true;
					}
					i+=2;
					break;
				case INDEXMODEQUAL:
					if (index % _CONACL_(1) == _CONACL_(2))
					{
						doit = true;
					}
					i+=2;
					break;
				case INDEXMODLESS:
					if (index % _CONACL_(1) <= _CONACL_(2))
					{
						doit = true;
					}
					i+=2;
					break;
				case INDEXMODRANGE:
					if (index % _CONACL_(1) >= _CONACL_(2) && index % _CONACL_(1) <= _CONACL_(3))
					{
						doit = true;
					}
					i+=3;
					break;
				}
				break;
			case BULAC_BOUNCE:
				switch (_ACL_(0))
				{
				case BOUNCEGREAT:
					if (bouncetime >= _CONACL_(1))
					{
						doit = true;
					}
					++i;
					break;
				case BOUNCEEQUAL:
					if (bouncetime == _CONACL_(1))
					{
						doit = true;
					}
					++i;
					break;
				case BOUNCELESS:
					if (bouncetime < _CONACL_(1))
					{
						doit = true;
					}
					++i;
					break;
				case BOUNCERANGE:
					if (bouncetime >= _CONACL_(1) && bouncetime <= _CONACL_(2))
					{
						doit = true;
					}
					i+=2;
					break;
				}
				break;
			}
		}
		else// if (!nextstep)
		{
			switch (_ACL_(0) & BULAE_FILTER)
			{
			case BULAE_TYPE:
				switch (_ACL_(0))
				{
				case TYPESET:
					if(doit)
					{
						if (oldtype != _EXEACL_(1))
						{
							oldtype = _EXEACL_(1);
							typechangetimer = 1;
							SE::push(SE_BULLET_CHANGE_2, x);
						}
					}
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_COLOR:
				switch (_ACL_(0))
				{
				case COLORSET:
					if(doit)
					{
						color = _EXEACL_(1);
						oldcolor = color;
//						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_ANGLE:
				switch (_ACL_(0))
				{
				case ANGLESET:
					if(doit)
					{
						angle = _EXEACL_(1);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETADD:
					if(doit)
					{
						angle += _EXEACL_(1);
						if(_EXEACL_(1) > BULLETACT_ANGLECHANGESE || _EXEACL_(1) < -BULLETACT_ANGLECHANGESE)
							SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETRMA:
					if(doit)
					{
						angle = rMainAngle(_EXEACL_(1)*1.0f, _EXEACL_(2)*1.0f, _EXEACL_(3)*1.0f);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=3;
					doit = false;
					break;
				case ANGLESETRMAP:
					if(doit)
					{
						angle = rMainAngle(Player::p.x, Player::p.y, _EXEACL_(1)*1.0f);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETRMAT:
					if(doit)
					{
						angle = rMainAngle(Target::tar[(int)_EXEACL_(1)].x, Target::tar[(int)_EXEACL_(1)].y, _EXEACL_(2)*1.0f);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=2;
					doit = false;
					break;
				case ANGLESETAMA:
					if(doit)
					{
						angle = aMainAngle(_EXEACL_(1)*1.0f, _EXEACL_(2)*1.0f, _EXEACL_(3));
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=3;
					doit = false;
					break;
				case ANGLESETAMAP:
					if(doit)
					{
						angle = aMainAngle(Player::p.x, Player::p.y, _EXEACL_(1));
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETAMAT:
					if(doit)
					{
						angle = aMainAngle(Target::tar[(int)_EXEACL_(1)].x, Target::tar[(int)_EXEACL_(1)].y, _EXEACL_(2));
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=2;
					doit = false;
					break;
				case ANGLESETRAND:
					if (doit)
					{
						angle = randt(_EXEACL_(1), _EXEACL_(2));
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=2;
					doit = false;
					break;
				case ANGLESETADDRAND:
					if (doit)
					{
						int addangle = randt(_EXEACL_(1), _EXEACL_(2));
						angle += addangle;
						if(addangle > BULLETACT_ANGLECHANGESE || addangle < -BULLETACT_ANGLECHANGESE)
							SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=2;
					doit = false;
					break;
				case ANGLEADJUST:
					if(doit)
						angle = aMainAngle(lastx, lasty) + 18000;
					doit = false;
					break;
				}
				break;
			case BULAE_HEADANGLE:
				switch (_ACL_(0))
				{
				case HEADANGLESET:
					if(doit)
						headangle = _EXEACL_(1);
					++i;
					doit = false;
					break;
				case HEADANGLESETADD:
					if(doit)
						headangle += _EXEACL_(1);
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_POS:
				switch (_ACL_(0))
				{
				case XSET:
					if(doit)
					{
						x = (float)_EXEACL_(1);
						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				case YSET:
					if(doit)
					{
						y = (float)_EXEACL_(1);
						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				case XSETADD:
					if(doit)
						x += _EXEACL_(1)/BULLETACT_FLOATSCALE;
					++i;
					doit = false;
					break;
				case YSETADD:
					if(doit)
						y += _EXEACL_(1)/BULLETACT_FLOATSCALE;
					++i;
					doit = false;
					break;
				case XSETACCADD:
					if(doit && usingtimer > (DWORD)_EXEACL_(1))
						x += (int)(usingtimer - _EXEACL_(1)) * _EXEACL_(2)/BULLETACT_FLOATSCALE;
					i+=2;
					doit = false;
					break;
				case YSETACCADD:
					if(doit && usingtimer > (DWORD)_EXEACL_(1))
						y += (int)(usingtimer - _EXEACL_(1)) * _EXEACL_(2)/BULLETACT_FLOATSCALE;
					i+=2;
					doit = false;
					break;
				}
				break;
			case BULAE_SPEED:
				switch (_ACL_(0))
				{
				case SPEEDSET:
					if(doit)
					{
						if (speed == 0)
						{
							SE::push(SE_BULLET_CHANGE_1, x);
						}
						speed = _EXEACL_(1)/BULLETACT_FLOATSCALE;
					}
					++i;
					doit = false;
					break;
				case SPEEDSETADD:
					if(doit)
					{
						if(speed == 0)
							SE::push(SE_BULLET_CHANGE_2, x);
						speed += _EXEACL_(1)/BULLETACT_FLOATSCALE;
					}
					++i;
					doit = false;
					break;
				case SPEEDSETMUL:
					if(doit)
					{
						speed *= _EXEACL_(1)/BULLETACT_FLOATSCALE;
					}
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_VAL:
				switch (_ACL_(0))
				{
				case VALSET:
					if(doit)
					{
						Scripter::scr.SetIntValue(_EXEACL_(1), _EXEACL_(2));
						Scripter::scr.d[(int)_EXEACL_(1)].bfloat = false;
					}
					i+=2;
					doit = false;
					break;
				case VALSETADD:
					if(doit)
					{
						Scripter::scr.SetIntValue(_EXEACL_(1), Scripter::scr.GetIntValue(_EXEACL_(1))+_EXEACL_(2));
						Scripter::scr.d[(int)_EXEACL_(1)].bfloat = false;
					}
					i+=2;
					doit = false;
					break;
				}
				break;
			case BULAE_OTHER:
				switch (_ACL_(0))
				{
				case CALLEVENT:
					if (doit)
					{
						Scripter::scr.Execute(SCR_EVENT, _EXEACL_(1), _EXEACL_(2));
					}
					i+=2;
					doit = false;
					break;
				case CHASE:
					if (doit)
					{
						_SAVEEXE_(2, chaseAim(Target::tar[(int)_EXEACL_(1)].x, Target::tar[(int)_EXEACL_(1)].y, _EXEACL_(2)));
					}
					i+=2;
					doit = false;
					break;

				case REMAIN:
					if(doit)
					{
						remain = true;
					}
					break;
				case DECANCEL:
					if (doit)
					{
						cancelable = false;
					}
					break;
				case FADEOUT:
					if(doit)
					{
						fadeout = true;
						timer = 0;
					}
					break;
				case FAITHLIZE:
					if(doit)
					{
						faithlize();
					}
					break;
				case POINTLIZE:
					if(doit)
					{
						pointlize();
					}
					break;
				case SCORELIZE:
					if (doit)
					{
						scorelize();
					}
					break;
				case BOUNCE:
					if (doit)
					{
						if (bouncetime < _EXEACL_(2))
						{
							if (x < M_ACTIVECLIENT_LEFT + _EXEACL_(1) || x > M_ACTIVECLIENT_RIGHT - _EXEACL_(1))
							{
//								_SAVEEXE_(2, _EXEACL_(2)-1);
								bouncetime++;
								SE::push(SE_BULLET_CHANGE_2, x);
								angle = 18000 - angle;
							}
							if (y < M_ACTIVECLIENT_TOP + _EXEACL_(1) || y > M_ACTIVECLIENT_BOTTOM - _EXEACL_(1))
							{
//								_SAVEEXE_(2, _EXEACL_(2)-1);
								bouncetime++;
								SE::push(SE_BULLET_CHANGE_2, x);
								angle = -angle;
							}
						}
					}
					i+=2;
					doit = false;
					break;
				case BOUNCELR:
					if (doit)
					{
						if (bouncetime < _EXEACL_(2))
						{
							if (x < M_ACTIVECLIENT_LEFT + _EXEACL_(1) || x > M_ACTIVECLIENT_RIGHT - _EXEACL_(1))
							{
//								_SAVEEXE_(2, _EXEACL_(2)-1);
								bouncetime++;
								SE::push(SE_BULLET_CHANGE_2, x);
								angle = 18000 - angle;
							}
						}
					}
					i+=2;
					doit = false;
					break;
				case BOUNCETB:
					if (doit)
					{
						if (bouncetime < _EXEACL_(2))
						{
							if (y < M_ACTIVECLIENT_TOP + _EXEACL_(1) || y > M_ACTIVECLIENT_BOTTOM - _EXEACL_(1))
							{
//								_SAVEEXE_(2, _EXEACL_(2)-1);
								bouncetime++;
								SE::push(SE_BULLET_CHANGE_2, x);
								angle = -angle;
							}
						}
					}
					i+=2;
					doit = false;
					break;
				}
				break;
			}
		}
	}
	return false;
}

bool Bullet::isInRect(float r, float aimx, float aimy)
{
	bulletData * tbd = &(BResource::res.bulletdata[type]);
	float _x = x;
	float _y = y;
	float collisionfactor = 1.0f;

	float rotCos;
	float rotSin;
	if (tbd->collisiontype ==  BULLET_COLLISION_ELLIPSE || tbd->collisiontype == BULLET_COLLISION_RECT)
	{
		if (speed)
		{
			if (!xplus && !yplus)
			{
				rotCos = cost(angle);
				rotSin = sint(angle);
			}
			else
			{
				rotCos = xplus / speed;
				rotSin = yplus / speed;
			}
		}
		else
		{
			rotCos = cost(angle);
			rotSin = sint(angle);
		}
	}
	switch (tbd->collisiontype)
	{
	case BULLET_COLLISION_NONE:
		return false;
	case BULLET_COLLISION_CIRCLE: 
		return CheckCollisionCircle(_x, _y, aimx, aimy, tbd->collisionMain * collisionfactor + r);
	case BULLET_COLLISION_SQURE: 
		return CheckCollisionSquare(_x, _y, aimx, aimy, tbd->collisionMain * collisionfactor, r);
	case BULLET_COLLISION_ELLIPSE:
		return CheckCollisionEllipse(_x, _y, aimx, aimy, tbd->collisionSub * collisionfactor, tbd->collisionMain * collisionfactor, rotCos, rotSin, r);
	case BULLET_COLLISION_RECT:
		return CheckCollisionRect(_x, _y, aimx, aimy, tbd->collisionSub * collisionfactor, tbd->collisionMain * collisionfactor, rotCos, rotSin, r);
	}
	return false;
}