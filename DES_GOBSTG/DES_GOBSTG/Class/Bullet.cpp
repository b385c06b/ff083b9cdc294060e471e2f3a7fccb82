#include "Bullet.h"
#include "Player.h"
#include "SE.h"
#include "Item.h"
#include "BossInfo.h"
#include "Scripter.h"
#include "Main.h"
#include "BResource.h"

#define _IZEZONEMAX			0x20

RenderDepth Bullet::renderDepth[BULLETTYPEMAX];

Bullet Bullet::_bu;
hgeSprite * Bullet::sp[BULLETTYPECOLORMAX];
VectorList<IzeZone> Bullet::izel;

VectorList<Bullet>bu;
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
		bulletData * tbd = &res.bulletdata[i];
		int tnum = tbd->nRoll;
		if (tnum < 2)
		{
			tnum = res.bulletdata[i].nColor;
		}
		int j=0;
		int index;
		for (j=0; j<tnum; j++)
		{
			index = i*BULLETCOLORMAX+j;
			sp[index] = new hgeSprite(tex, tbd->tex_x + j*(tbd->tex_w), tbd->tex_y, tbd->tex_w, tbd->tex_h);
			sp[index]->SetBlendMode(tbd->blendtype);
			if (res.bulletdata[i].collisiontype != BULLET_COLLISION_ELLIPSE && tbd->collisionSub)
			{
				sp[index]->SetHotSpot((tbd->tex_w)/2.0f, (tbd->tex_h)/2.0f+tbd->collisionSub);
			}
		}
		for (; j<BULLETCOLORMAX; j++)
		{
			index = i*BULLETCOLORMAX+j;
			sp[index] = new hgeSprite(tex, 0, 0, 0, 0);
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
		Bullet::Build(tx, ty, true, tnowangle, speed, type, color, fadeinTime, avoid, 0xff);
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
		Bullet::Build(tx, ty, true, angle + anglefactor * tindex, speed + speedfactor * abs(tindex), type, color, fadeinTime, avoid, 0xff);
	}
}

bool Bullet::Build(float x, float y, bool absolute, int angle, float speed, BYTE type, BYTE color, int fadeinTime, float avoid, BYTE tarID)
{
	Bullet * _tbu = NULL;
	_tbu = bu.push_back();
	index = bu.getEndIndex();
	if (!_tbu->valueSet(index, x, y, absolute, angle, speed, type, color, fadeinTime, avoid, tarID))
	{
		bu.pop(index);
		return false;
	}
	memcpy(_tbu->actionList, _bu.actionList, BULLETACTIONMAX*sizeof(int));
	return true;
}

void Bullet::Release()
{
	bu.clear();
	for(int i=0;i<BULLETTYPECOLORMAX;i++)
	{
		if(sp[i])
			delete sp[i];
		sp[i] = NULL;
	}
}

void Bullet::Render()
{
	int i = type*BULLETCOLORMAX + color;
	sp[i]->SetColor(alpha<<24 | diffuse);
	sp[i]->RenderEx(x, y, ARC(angle+headangle+BULLET_ANGLEOFFSET), hscale);
}

BYTE Bullet::getRenderDepth()
{
	return res.bulletdata[type].renderdepth;
}

void Bullet::matchFadeInColorType()
{
	if( res.bulletdata[type].fadecolor < BULLETCOLORMAX)
	{
		color = res.bulletdata[type].fadecolor;
		type = BULLET_FADEINTYPE;
	}
	else if (res.bulletdata[type].fadecolor == BULLET_FADECOLOR_16)
	{
		type = BULLET_FADEINTYPE;
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
	else if (res.bulletdata[type].fadecolor == BULLET_FADECOLOR_8)
	{
		type = BULLET_FADEINTYPE;
	}
}
void Bullet::matchFadeOutColorType()
{
	if (res.bulletdata[type].fadecolor < BULLETCOLORMAX)
	{
		color = res.bulletdata[type].fadecolor;
		type = BULLET_FADEOUTTYPE;
	}
	else if (res.bulletdata[type].fadecolor == BULLET_FADECOLOR_16)
	{
		type = BULLET_FADEOUTTYPE;
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
	else if (res.bulletdata[type].fadecolor == BULLET_FADECOLOR_8)
	{
		type = BULLET_FADEOUTTYPE;
	}
}

bool Bullet::valueSet(WORD _ID, float _x, float _y, bool absolute, int _angle, float _speed, BYTE _type, BYTE _color, int _fadeinTime, float avoid, BYTE _tarID)
{
	ID			=	_ID;
	x			=	_x;
	y			=	_y;
	type		=	_type;
	if(avoid)
	{
		if(isInRect(avoid, Player::p.x, Player::p.y))
			return false;
	}
	if(absolute)
		angle	=	_angle;
	else
		angle	=	rMainAngle(Player::p, _angle);
	speed		=	_speed;
	oldtype		=	type;
	color		=	_color;
	oldcolor	=	color;
	fadeinTime	=	_fadeinTime;

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
	if (cancelable || bossinfo.flag>=BOSSINFO_COLLAPSE)
	{
		if (izel.size)
		{
			DWORD i = 0;
			DWORD size = izel.size;
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
								scr.eventExecute(SCR_EVENT_BULLETENTERIZE, tize->eventID);
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
	if(!grazed && res.bulletdata[type].collisiontype != BULLET_COLLISION_NONE)
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
	if (!(res.bulletdata[type].nColor % 8))
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
		else if(timer < (DWORD)fadeinTime)
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
		else if(timer == fadeinTime)
		{
			if(oldtype != type)
			{
				hscale -= 0.0625f;
			}
			else
			{
				hscale += 1.0f / fadeinTime;
			}
			type = oldtype;
			color = oldcolor;
			alpha = 0xff;
			SE::push(res.bulletdata[type].seID, x);
		}
		else
		{
			x += xplus;
			y += yplus;

			if (able)
			{
				ChangeAction();

				if (res.bulletdata[type].nRoll && !(timer % BULLET_ANIMATIONSPEED))
				{
					color++;
					if (color >= res.bulletdata[type].nRoll)
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
						type = oldtype;
						color = oldcolor;
						typechangetimer = 0;
						alpha = 0xff;
					}
				}

				DoCollision();
				DoGraze();
			}
		}

		DoIze();
		headangle += SIGN(color) * res.bulletdata[type].nTurnAngle;
		if(tarID != 0xff)
		{
			tar[tarID].x = x;
			tar[tarID].y = y;
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
				color = res.bulletdata[oldtype].bonuscolor;
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

bool Bullet::isInRect(float r, float aimx, float aimy)
{
	bulletData * tbd = &(res.bulletdata[type]);
	switch (tbd->collisiontype)
	{
	case BULLET_COLLISION_NONE:
		return false;
		break;
	case BULLET_COLLISION_CIRCLE: 
		return checkCollisionCircle(aimx, aimy, tbd->collisionMain + r);
		break;
	case BULLET_COLLISION_ELLIPSE: 
		float rotCos;
		float rotSin;
		if (!speed)
		{
			rotCos = xplus / speed;
			rotSin = yplus / speed;
		}
		else
		{
			rotCos = cost(angle);
			rotSin = sint(angle);
		}
		return checkCollisionEllipse(aimx, aimy, tbd->collisionSub, tbd->collisionMain, rotCos, rotSin, r);
		break;
	case BULLET_COLLISION_SQURE: 
		return checkCollisionSquare(aimx, aimy, tbd->collisionMain);
		break;
	}
	return false;
}

void Bullet::ChangeAction()
{
	bool doit = false;
	for(int i=0;i<BULLETACTIONMAX;++i)
	{
		if (actionList[i] < BULA_SPECIALSTART)
		{
			switch (actionList[i])
			{
			case AND:
				if(!doit)
					for(++i;i<BULLETACTIONMAX;++i)
						if(actionList[i] == THEN)
							break;
				doit = false;
				break;
			case OR:
				if(doit)
				{
					for(++i;i<BULLETACTIONMAX;++i)
						if(actionList[i] == THEN)
							break;
				}
				else
					doit = false;
				break;
			case NOT:
				doit = !doit;
				break;
//			case ANDSET:
//				doit = true;
//				break;
			}
		}
		else if (actionList[i] < BUAL_EXECUTESTART)
		{
			switch (actionList[i] & BUALC_FILTER)
			{
			case BULAC_OTHER:
				switch (actionList[i])
				{
				case SECTIONEND:
					i = BULLETACTIONMAX-1;
					break;

				case EVERY:
					doit = true;
					break;
				case EVERYMOD:
					if(timer % (DWORD)actionList[i+1] == 0)
						doit = true;
					++i;
					break;
				}
				break;
			case BULAC_TIMER:
				switch (actionList[i])
				{
				case TIMERGREAT:
					if(timer >= (DWORD)actionList[i+1])
						doit = true;
					++i;
					break;
				case TIMEREQUAL:
					if(timer == (DWORD)actionList[i+1])
						doit = true;
					++i;
					break;
				case TIMERLESS:
					if(timer <= (DWORD)actionList[i+1])
						doit = true;
					++i;
					break;
				case TIMERRANGE:
					if(timer >= (DWORD)actionList[i+1] && timer <= (DWORD)actionList[i+2])
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_TYPE:
				switch (actionList[i])
				{
				case TYPEEQUAL:
					if(type == actionList[i+1])
						doit = true;
					++i;
					break;
				}
				break;
			case BULAC_COLOR:
				switch (actionList[i])
				{
				case COLOREQUAL:
					if(color == actionList[i+1])
						doit = true;
					++i;
					break;
				}
				break;
			case BULAC_ANGLE:
				switch (actionList[i])
				{
				case ANGLEGREAT:
					if(angle >= actionList[i+1])
						doit = true;
					++i;
					break;
				case ANGLEEQUAL:
					if(angle == actionList[i+1])
						doit = true;
					++i;
					break;
				case ANGLELESS:
					if(angle <= actionList[i+1])
						doit = true;
					++i;
					break;
				case ANGLERANGE:
					if(angle >= actionList[i+1] && angle <= actionList[i+2])
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_POS:
				switch (actionList[i])
				{
				case XGREAT:
					if(x >= actionList[i+1])
						doit = true;
					++i;
					break;
				case XLESS:
					if(x <= actionList[i+1])
						doit = true;
					++i;
					break;
				case XRANGE:
					if(x >= actionList[i+1] && x <= actionList[i+2])
						doit = true;
					i+=2;
					break;
				case YGREAT:
					if(y >= actionList[i+1])
						doit = true;
					++i;
					break;
				case YLESS:
					if(y <= actionList[i+1])
						doit = true;
					++i;
					break;
				case YRANGE:
					if(y >= actionList[i+1] && y <= actionList[i+2])
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_VAL:
				switch (actionList[i])
				{
				case VALGREAT:
					if(CAST(scr.d[actionList[i+1]]) >= actionList[i+2])
						doit = true;
					i+=2;
					break;
				case VALEQUAL:
					if(CAST(scr.d[actionList[i+1]]) == actionList[i+2])
						doit = true;
					i+=2;
					break;
				case VALLESS:
					if(CAST(scr.d[actionList[i+1]]) <= actionList[i+2])
						doit = true;
					i+=2;
					break;
				case VALRANGE:
					if(CAST(scr.d[actionList[i+1]]) >= actionList[i+2] && CAST(scr.d[actionList[i+1]]) <= actionList[i+3])
						doit = true;
					i+=3;
					break;
				}
				break;
			case BULAC_SPEED:
				switch (actionList[i])
				{
				case SPEEDGREAT:
					if(speed*BULLETACT_FLOATSCALE >= actionList[i+1])
						doit = true;
					++i;
					break;
				case SPEEDEQUAL:
					if(speed*BULLETACT_FLOATSCALE == actionList[i+1])
						doit = true;
					++i;
					break;
				case SPEEDLESS:
					if(speed*BULLETACT_FLOATSCALE <= actionList[i+1])
						doit = true;
					++i;
					break;
				case SPEEDRANGE:
					if(speed*BULLETACT_FLOATSCALE >= actionList[i+1] && speed*BULLETACT_FLOATSCALE <= actionList[i+2])
						doit = true;
					i+=2;
					break;
				}
				break;
			case BULAC_INDEX:
				switch (actionList[i])
				{
				case INDEXMODGREAT:
					if (index % actionList[i+1] >= actionList[i+2])
					{
						doit = true;
					}
					i+=2;
					break;
				case INDEXMODEQUAL:
					if (index % actionList[i+1] == actionList[i+2])
					{
						doit = true;
					}
					i+=2;
					break;
				case INDEXMODLESS:
					if (index % actionList[i+1] <= actionList[i+2])
					{
						doit = true;
					}
					i+=2;
					break;
				case INDEXMODRANGE:
					if (index % actionList[i+1] >= actionList[i+2] && index % actionList[i+1] <= actionList[i+3])
					{
						doit = true;
					}
					i+=3;
					break;
				}
				break;
			}
		}
		else
		{
			switch (actionList[i] & BULAE_FILTER)
			{
			case BULAE_TYPE:
				switch (actionList[i])
				{
				case TYPESET:
					if(doit)
					{
						oldtype = actionList[i+1];
						typechangetimer = 1;
						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_COLOR:
				switch (actionList[i])
				{
				case COLORSET:
					if(doit)
					{
						color = actionList[i+1];
						oldcolor = color;
						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_ANGLE:
				switch (actionList[i])
				{
				case ANGLESET:
					if(doit)
					{
						angle = actionList[i+1];
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETADD:
					if(doit)
					{
						angle += actionList[i+1];
						if(actionList[i+1] > BULLETACT_ANGLECHANGESE || actionList[i+1] < -BULLETACT_ANGLECHANGESE)
							SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETRMA:
					if(doit)
					{
						angle = rMainAngle(actionList[i+1]*1.0f,actionList[i+2]*1.0f,actionList[i+3]*1.0f);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=3;
					doit = false;
					break;
				case ANGLESETRMAP:
					if(doit)
					{
						angle = rMainAngle(Player::p.x,Player::p.y,actionList[i+1]*1.0f);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETRMAT:
					if(doit)
					{
						angle = rMainAngle(tar[actionList[i+1]].x,tar[actionList[i+1]].y,actionList[i+2]*1.0f);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=2;
					doit = false;
					break;
				case ANGLESETAMA:
					if(doit)
					{
						angle = aMainAngle(actionList[i+1]*1.0f,actionList[i+2]*1.0f,actionList[i+3]);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					i+=3;
					doit = false;
					break;
				case ANGLESETAMAP:
					if(doit)
					{
						angle = aMainAngle(Player::p.x,Player::p.y,actionList[i+1]);
						SE::push(SE_BULLET_CHANGE_1, x);
					}
					++i;
					doit = false;
					break;
				case ANGLESETAMAT:
					if(doit)
					{
						angle = aMainAngle(tar[actionList[i+1]].x,tar[actionList[i+1]].y,actionList[i+2]);
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
				switch (actionList[i])
				{
				case HEADANGLESET:
					if(doit)
						headangle = actionList[i+1];
					++i;
					doit = false;
					break;
				case HEADANGLESETADD:
					if(doit)
						headangle += actionList[i+1];
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_POS:
				switch (actionList[i])
				{
				case XSET:
					if(doit)
					{
						x = (float)actionList[i+1];
						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				case YSET:
					if(doit)
					{
						y = (float)actionList[i+1];
						SE::push(SE_BULLET_CHANGE_2, x);
					}
					++i;
					doit = false;
					break;
				case XSETADD:
					if(doit)
						x += actionList[i+1]/BULLETACT_FLOATSCALE;
					++i;
					doit = false;
					break;
				case YSETADD:
					if(doit)
						y += actionList[i+1]/BULLETACT_FLOATSCALE;
					++i;
					doit = false;
					break;
				case XSETACCADD:
					if(doit && timer > (DWORD)actionList[i+1])
						x += (int)(timer - actionList[i+1]) * actionList[i+2]/BULLETACT_FLOATSCALE;
					i+=2;
					doit = false;
					break;
				case YSETACCADD:
					if(doit && timer > (DWORD)actionList[i+1])
						y += (int)(timer - actionList[i+1]) * actionList[i+2]/BULLETACT_FLOATSCALE;
					i+=2;
					doit = false;
					break;
				}
				break;
			case BULAE_SPEED:
				switch (actionList[i])
				{
				case SPEEDSET:
					if(doit)
					{
						if (speed == 0)
						{
							SE::push(SE_BULLET_CHANGE_1, x);
						}
						speed = actionList[i+1]/BULLETACT_FLOATSCALE;
					}
					++i;
					doit = false;
					break;
				case SPEEDSETADD:
					if(doit)
					{
						if(speed == 0)
							SE::push(SE_BULLET_CHANGE_2, x);
						speed += actionList[i+1]/BULLETACT_FLOATSCALE;
					}
					++i;
					doit = false;
					break;
				}
				break;
			case BULAE_VAL:
				switch (actionList[i])
				{
				case VALSET:
					if(doit)
					{
						scr.SetIntValue(actionList[i+1], actionList[i+2]);
						scr.d[actionList[i+1]].bfloat = false;
					}
					i+=2;
					doit = false;
					break;
				case VALSETADD:
					if(doit)
					{
						scr.SetIntValue(actionList[i+1], scr.GetIntValue(actionList[i+1])+actionList[i+2]);
						scr.d[actionList[i+1]].bfloat = false;
					}
					i+=2;
					doit = false;
					break;
				}
				break;
			case BULAE_OTHER:
				switch (actionList[i])
				{
				case CALLEVENT:
					if (doit)
					{
						scr.eventExecute(actionList[i+1], actionList[i+2]);
					}
					i+=2;
					doit = false;
					break;
				case CHASE:
					if (doit)
					{
						actionList[i+2]--;
						chaseAim(tar[actionList[i+1]].x, tar[actionList[i+1]].y, actionList[i+2]);
					}
					i+=2;
					doit = false;
					break;

				case REMAIN:
					if(doit)
						remain = true;
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
						if (actionList[i+2])
						{
							if (x < M_ACTIVECLIENT_LEFT + actionList[i+1] || x > M_ACTIVECLIENT_RIGHT - actionList[i+1])
							{
								actionList[i+2]--;
								SE::push(SE_BULLET_CHANGE_2, x);
								angle = 18000 - angle;
							}
							if (y < M_ACTIVECLIENT_TOP + actionList[i+1] || y > M_ACTIVECLIENT_BOTTOM - actionList[i+1])
							{
								actionList[i+2]--;
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
}
