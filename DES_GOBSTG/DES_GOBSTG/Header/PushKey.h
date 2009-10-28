#ifndef _PUSHKEY_H
#define _PUSHKEY_H

#include "Const.h"
#include "MainDependency.h"

#define PUSHKEY_IDMAX	4
#define PUSHKEY_KEYCHECKMAX	4

#define PUSHKEY_ID_SELUSE	0
#define PUSHKEY_ID_ISELUSE	1
#define PUSHKEY_ID_UIUSE	2
#define PUSHKEY_ID_GAMEUSE	3

#define PUSHKEY_KEYNULL	-1

struct pushkeyEvent
{
	int timer;
	int pushkey[PUSHKEY_KEYCHECKMAX];
	int pushfirst;
	int pushrollto;
};

class PushKey
{
public:
	static bool SetPushEvent(BYTE ID, int pushkey_1, int pushkey_2=PUSHKEY_KEYNULL, int pushkey_3=PUSHKEY_KEYNULL, int pushkey_4=PUSHKEY_KEYNULL, int pushfirst=M_PUSH_FIRST, int pushrollto=M_PUSH_ROLLTO);
	static bool UpdatePushEvent(BYTE ID);
public:
	static pushkeyEvent pushkeyevent[PUSHKEY_IDMAX];
};

#endif