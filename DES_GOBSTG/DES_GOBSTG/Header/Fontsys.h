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
	static void Init();
	static void HeatUp();
	static void FocusChanged();
	static void Release();

	static void SignUp(BYTE ID, const char * text = NULL, HD3DFONT font=NULL);
	static void Render(BYTE ID, float x, float y, DWORD ucol, DWORD dcol, float shadow = 0, float hext = 0, float vext = 0);

	static int strTranslate(char * dtext, const char * stext);

public:
	static char text[FONTSYSMAX][M_STRMAX];
	static HD3DFONT usingfont[FONTSYSMAX];
	static hgeQuad quad[FONTSYSMAX];
	static HTARGET tar[FONTSYSMAX];
	static int lines[FONTSYSMAX];
	static int changeID;

	static HD3DFONT font;
};

#endif