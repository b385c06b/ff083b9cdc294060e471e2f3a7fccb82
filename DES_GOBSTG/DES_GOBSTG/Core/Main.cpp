#include "vld.h"

#include "Process.h"
#include "Fontsys.h"

#include "Main.h"

char strdesc[STRINGDESCMAX][M_STRMAX*2];

Target tar[TARGETMAX];

HGE *hge = NULL;

int time = 0;

DWORD strdescIndex = 0;

bool RenderFunc()
{
//	Export::clientSetMatrix(worldx, worldy, worldz);

	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);

	mp.render();
	hge->Gfx_EndScene();
	
	return false;
}

bool FrameFunc()
{
	if (hge->Input_GetDIKey(KS_ESCAPE_MP))
		return true;
	if(hge->Input_GetDIKey(KS_CAPTURE_MP, DIKEY_DOWN))
	{
		mp.SnapShot();
	}

	if(mp.frame() == PQUIT)
		return true;

	return false;
}

bool FocusGainFunc()
{
	Fontsys::FocusChanged();
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);
	
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_FOCUSGAINFUNC, FocusGainFunc);

	Export::clientInitial(true);

	if(hge->System_Initiate())
	{
		time = 0;
		mp.state = STATE_INIT;
		hge->System_Start();
	}

	//
	mp.Realease();

//	hge->System_Shutdown();
	hge->Release();
	
	return 0;
}