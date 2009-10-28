#ifndef _OBJECT_H
#define _OBJECT_H

#include "MainDependency.h"
#include "Const.h"
#include "BMath.h"

#define BOBJ_M_CHASEAIM_SPEEDMIN	0.1f

class BObject
{
public:
	BObject();
	virtual ~BObject();

	int rMainAngle(float destx, float desty, float r = 0);
	int aMainAngle(float destx, float desty, int a = 0);
	int rMainAngle(const BObject &aim, float r = 0);
	int aMainAngle(const BObject &aim, int a = 0);

	int chaseAim(float destx, float desty, int chasetimer);
	int chaseAim(const BObject &aim, int chasetimer);

	bool checkCollisionCircle(float aimx, float aimy, float r);
	bool checkCollisionBigCircle(float aimx, float aimy, float r);
	bool checkCollisionEllipse(float aimx, float aimy, float rPrep, float rParal, float rotCos = 0, float rotSin = 1, float rOri = 0);
	bool checkCollisionRect(float aimx, float aimy, float rectPrep, float rectParal, float rotCos = 0, float rotSin = 1, float rOri = 0);
	bool checkCollisionCircle(const BObject &aim, float r);
	bool checkCollisionBigCircle(const BObject &aim, float r);
	bool checkCollisionEllipse(const BObject &aim, float rPrep, float rParal, float rotCos = 0, float rotSin = 1, float rOri = 0);
	bool checkCollisionRect(const BObject &aim, float rectPrep, float rectParal, float rotCos = 0, float rotSin = 1, float rOri = 0);
	bool checkCollisionSquare(float aimx, float aimy, float length, float height = 0);
	bool checkCollisionSquare(const BObject &aim, float length, float height = 0);

	virtual void action();

public:
	float	x;
	float	y;
	int		angle;
	int		headangle;
	float	speed;
	float	vscale;
	float	hscale;
	DWORD	diffuse;
	DWORD	timer;
	bool	exist;

	WORD	ID;
	BYTE	alpha;
	BYTE	tarID;

};

#endif