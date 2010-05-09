#include "../include/hge.h"
#include "../DES_GOBSTG/DES_GOBSTG/Header/Export.h"
#include "../DES_GOBSTG/DES_GOBSTG/Header/BResource.h"
#include "../DES_GOBSTG/DES_GOBSTG/Header/Data.h"
#include "../include/hgeEffectSystem.h"
#include "EditorRes.h"
#include "EditorUI.h"
//#include "vld.h"

HGE * hge = hgeCreate(HGE_VERSION);

bool FrameFunc()
{
	if(hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;
	if(hge->Input_GetKeyState(HGEK_CTRL) && hge->Input_GetKeyState(HGEK_BACKSPACE))
	{
		if(!eres.Load())
			return true;
		ui.timer = 0;
	}
	ui.Update();

	return false;
}

bool RenderFunc()
{
//	Export::clientSetMatrix();
	
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);

	ui.Render();

	hge->Gfx_EndScene();
	return false;
}

#ifdef __WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
void main()
#endif // __WIN32
{
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

	if (!Export::clientInitial(false, true))
	{
		hge->System_SetState(HGE_TITLE, "HGE Effect System Editor by h5nc");
	}

	if(hge->System_Initiate())
	{
		if(eres.Load())
		{
			hge->System_Start();
		}
	}
	eres.Release();

//	hge->System_Shutdown();
	hge->Release();

#ifdef __WIN32
#ifdef WIN32
#ifdef _DEBUG
	m_dumpMemoryReport();
#endif // _DEBUG
#endif // WIN32
#endif // __WIN32

}