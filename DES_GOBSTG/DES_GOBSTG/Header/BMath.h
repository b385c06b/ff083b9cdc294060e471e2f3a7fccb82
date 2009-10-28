#ifndef _BMATH_H
#define _BMATH_H

#ifdef	__INTEL_COMPILER
	#ifdef __USE_INTEL_MATH_LAB
		#include <mathimf.h>
	#endif
#endif

#include <math.h>

#define asin2t(X, Y)	ANGLE(asinf((float)(X)/(float)(Y)))
#define acos2t(X, Y)	ANGLE(acosf((float)(X)/(float)(Y)))
#define atan2t(X, Y)	ANGLE(atan2f(X, Y))

#include "Const.h"

void srandt(int seed);
int randt();

	#ifdef BUSE_MATHTABLE

		struct mathTable
		{
			int angle;
			float sin;
			float tan;
		};

		float sint(int angle);
		float cost(int angle);
		float tant(int angle);

		extern mathTable mathtable[];

	#else

		#define sint(X)		sinf(ARC(X))
		#define cost(X)		cosf(ARC(X))
		#define tant(X)		tanf(ARC(X))


	#endif

#endif