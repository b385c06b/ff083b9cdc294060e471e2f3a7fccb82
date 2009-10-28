#ifndef _INFOSELECT_H
#define _INFOSELECT_H

#include "MainDependency.h"
#include "Const.h"

//infoselect colortype
#define	INFO_GREEN		0
#define INFO_YELLOW		1
#define INFO_GRAY		2
#define INFO_RED		3
#define INFO_BLUE		4

class InfoSelect
{
public:
	InfoSelect();
	~InfoSelect();

	void valueSet(BYTE ID, const char * info, float x, float y, BYTE coltype, BYTE flag=SEL_NULL);

	static void Build(BYTE ID, const char * info, float x, float y, BYTE coltype, BYTE flag=SEL_NULL);

	static bool SetPageNum(BYTE nPageNum, float fadebegin, float offset, int initshift=0);
	static void shift(int nshift);
	static void Render();

	static void Setup(int nselect, int select, bool updown=true);

	static InfoSelect * GetPointer(int index);

	static void Clear();

	void action();
	void matchSelect();

	void linki(const char * linkcode, int num);
	void linkl(const char * linkcode, LONGLONG num);
	void linkf(const char * linkcode, BYTE tail, float num);

public:
	char info[M_STRMAX*2];
	float x;
	float y;
	DWORD ucol;
	DWORD dcol;

	BYTE coltype;
	BYTE ID;
	BYTE flag;

	static int nPageNum;
	static int firstID;
	static int lastID;
	static float fadebegin;
	static float offset;

	static int nselect;
	static int select;
	static bool updown;

	static bool complete;
	static bool plus;

	static bool avoid;
};

extern list<InfoSelect>infoselect;

#endif