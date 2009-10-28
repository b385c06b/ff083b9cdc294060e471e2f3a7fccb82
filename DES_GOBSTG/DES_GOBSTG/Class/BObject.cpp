#include "BObject.h"

BObject::BObject()
{
	exist	= false;
	alpha	= 0xff;
	diffuse	= 0xffffff;
	tarID	= 0xff;
}

BObject::~BObject()
{
}

void BObject::action()
{
}

int BObject::rMainAngle(const BObject &aim, float r)
{
	return rMainAngle(aim.x, aim.y, r);
}

int BObject::aMainAngle(const BObject &aim, int a)
{
	return aMainAngle(aim.x, aim.y, a);
}

int BObject::rMainAngle(float destx, float desty, float r)
{
	//r>0 clockwise
	//r<0 counterclockwise
	float dx = destx - x;
	float dy = desty - y;
	float atanA, atanB;

	if(dx == 0)
	{
		if(dy >= 0)
			atanA = M_PI_2;
		else
			atanA = -M_PI_2;
	}
	else
		atanA = atan2f(dy, dx);
//	if(dx < 0)
//		atanA += M_PI;
	if(r == 0)
		atanB = 0;
	else
		atanB = atan2f(r, sqrt(dx*dx + dy*dy));
	return ANGLE(atanA + atanB);
}

int BObject::aMainAngle(float destx, float desty, int a)
{
	//a>0 clockwise
	//a<0 counterclockwise
	float dx = destx - x;
	float dy = desty - y;
	float atanA;
	if(dx == 0)
	{
		if(dy >= 0)
			atanA = M_PI_2;
		else
			atanA = -M_PI_2;
	}
	else
		atanA = atan2f(dy, dx);
//	if(dx < 0)
//		atanA += M_PI;
	return ANGLE(atanA)+a;
}

int BObject::chaseAim(const BObject &aim, int chasetimer)
{
	return chaseAim(aim.x, aim.y, chasetimer);
}

int BObject::chaseAim(float destx, float desty, int chasetimer)
{
	angle = aMainAngle(destx, desty);
	if (chasetimer > 1)
	{
		speed = 2 * DIST(x, y, destx, desty) / chasetimer;
	}
	else
		speed = BOBJ_M_CHASEAIM_SPEEDMIN;
	return chasetimer - 1;
}

bool BObject::checkCollisionCircle(const BObject &aim, float r)
{
	return checkCollisionCircle(aim.x, aim.y, r);
}

bool BObject::checkCollisionCircle(float aimx, float aimy, float r)
{
	if((fabsf(x-aimx)>r)||(fabsf(y-aimy)>r))
		return false;

	if((x-aimx)*(x-aimx)+(y-aimy)*(y-aimy) <= r*r)
		return true;

	return false;
}
bool BObject::checkCollisionBigCircle(const BObject &aim, float r)
{
	return checkCollisionBigCircle(aim.x, aim.y, r);
}

bool BObject::checkCollisionBigCircle(float aimx, float aimy, float r)
{
	if (!checkCollisionSquare(aimx, aimy, r, r))
		return false;
	if (checkCollisionSquare(aimx, aimy, r*M_SQRT1_2, r*M_SQRT1_2))
		return true;
	if((x-aimx)*(x-aimx)+(y-aimy)*(y-aimy) <= r*r)
		return true;
	return false;
}

bool BObject::checkCollisionRect(const BObject &aim, float rectPrep, float rectParal, float rotCos, float rotSin, float rOri /* = 0 */)
{
	return checkCollisionRect(aim.x, aim.y, rectPrep, rectParal, rotCos, rotSin, rOri);
}

bool BObject::checkCollisionRect(float aimx, float aimy, float rectPrep, float rectParal, float rotCos, float rotSin, float rOri)
{
	float longside = rectPrep > rectParal ? rectPrep : rectParal;

	if((fabsf(x-aimx)>longside+rOri)||(fabsf(y-aimy)>longside+rOri))
		return false;

	float newx,newy;
	newx = (float)fabsf((rotSin*(aimx-x)-rotCos*(aimy-y)));
	newy = (float)fabsf((rotCos*(aimx-x)+rotSin*(aimy-y)));

	if (newx <= rectPrep+rOri && newy <= rectParal+rOri)
	{
		return true;
	}

	return false;
}

bool BObject::checkCollisionEllipse(const BObject &aim, float rPrep, float rParal, float rotCos, float rotSin, float rOri /* = 0 */)
{
	return checkCollisionEllipse(aim.x, aim.y, rPrep, rParal, rotCos, rotSin, rOri);
}

bool BObject::checkCollisionEllipse(float aimx, float aimy, float rPrep, float rParal, float rotCos, float rotSin, float rOri)
{
	float longr = rPrep > rParal ? rPrep : rParal;

	if((fabsf(x-aimx)>longr+rOri)||(fabsf(y-aimy)>longr+rOri))
		return false;

	float newx,newy;
	newx = (float)fabsf((rotSin*(aimx-x)-rotCos*(aimy-y)));
	newy = (float)fabsf((rotCos*(aimx-x)+rotSin*(aimy-y)));

	if(newx*newx*(rParal+rOri)*(rParal+rOri)+newy*newy*(rPrep+rOri)*(rPrep+rOri) <= (rPrep+rOri)*(rPrep+rOri)*(rParal+rOri)*(rParal+rOri))
		return true;

	return false;
}

bool BObject::checkCollisionSquare(const BObject &aim, float length, float height)
{
	return checkCollisionSquare(aim.x, aim.y, length, height);
}

bool BObject::checkCollisionSquare(float aimx, float aimy, float length, float height)
{
	if (!height)
	{
		height = length;
	}
	if (fabsf(x - aimx) <= length && fabsf(y - aimy) <= height)
	{
		return true;
	}

	return false;
}