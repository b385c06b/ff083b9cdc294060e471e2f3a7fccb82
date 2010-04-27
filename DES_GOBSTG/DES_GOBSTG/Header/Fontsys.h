#ifndef _FONTSYS_H
#define _FONTSYS_H

#include "MainDependency.h"
#include "Const.h"

#define FONTSYS_CHATUSE			FONTSYSMAX-2
#define FONTSYS_SPELLNAMEUSE	FONTSYSMAX-1

#define FONTSYS_TRANSCHAR		'|'
#define FONTSYS_CONTROLCHAR		'E'
#define FONTSYS_CONTROLSTR		"|E"

#define FONTSYS_TARGETWIDTH		M_CLIENT_WIDTH
#define FONTSYS_TARGETHEIGHT	(M_CLIENT_HEIGHT/2)

#define FONTSYS_CHATTEXTWIDTH	300
#define FONTSYS_CHATTEXTHEIGHT	80

class Fontsys
{
	Fontsys(){};
	~Fontsys(){};

public:
	void Init(HD3DFONT font);
	void HeatUp();
	void FocusChanged();
	void Release();

	void SignUp(BYTE ID, const char * text = NULL, HD3DFONT font=NULL);
	void Render(BYTE ID, float x, float y, DWORD ucol, DWORD dcol, float shadow = 0, float hext = 0, float vext = 0);

	int strTranslate(char * dtext, const char * stext);

public:
	char text[FONTSYSMAX][M_STRMAX];
	HD3DFONT usingfont[FONTSYSMAX];
	hgeQuad quad[FONTSYSMAX];
	HTARGET tar[FONTSYSMAX];
	int lines[FONTSYSMAX];
	static int changeID;

	HD3DFONT font;
	static Fontsys fontsys;
};

#endif