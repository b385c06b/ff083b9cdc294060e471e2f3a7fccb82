/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core system functions
*/

#include "hge_impl.h"


#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))


const char *WINDOW_CLASS_NAME = "HGE__WNDCLASS";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int			nRef=0;
HGE_Impl*	pHGE=0;



BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}


HGE* CALL hgeCreate(int ver)
{
	if(ver==HGE_VERSION)
		return (HGE*)HGE_Impl::_Interface_Get();
	else
		return 0;
}


HGE_Impl* HGE_Impl::_Interface_Get()
{
	if(!pHGE) pHGE=new HGE_Impl();

	nRef++;

	return pHGE;
}


void CALL HGE_Impl::Release()
{
	nRef--;

	if(!nRef)
	{
		if(pHGE->hwnd) pHGE->System_Shutdown();
		Resource_RemoveAllPacks();
		delete pHGE;
		pHGE=0;
	}
}


bool CALL HGE_Impl::System_Initiate()
{
	OSVERSIONINFO	os_ver;
	SYSTEMTIME		tm;
	MEMORYSTATUS	mem_st;
	WNDCLASS		winclass;
	int				width, height;

	// Log system info

	System_Log("HGE Started..\n");

	System_Log("HGE version: %X.%X", HGE_VERSION>>8, HGE_VERSION & 0xFF);
	GetLocalTime(&tm);
	System_Log("Date: %02d.%02d.%d, %02d:%02d:%02d\n", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond);

	System_Log("Application: %s",szWinTitle);
	os_ver.dwOSVersionInfoSize=sizeof(os_ver);
	GetVersionEx(&os_ver);
	System_Log("OS: Windows %ld.%ld.%ld",os_ver.dwMajorVersion,os_ver.dwMinorVersion,os_ver.dwBuildNumber);

	GlobalMemoryStatus(&mem_st);
	System_Log("Memory: %ldK total, %ldK free\n",mem_st.dwTotalPhys/1024L,mem_st.dwAvailPhys/1024L);
	

	// Register window class
	
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hInstance;
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	if(szIcon) winclass.hIcon = LoadIcon(hInstance, szIcon);
	else winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClass(&winclass)) {
		_PostError("Can't register window class");
		return false;
	}

	// Create window

	width=nScreenWidth + GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	height=nScreenHeight + GetSystemMetrics(SM_CYFIXEDFRAME)*2 + GetSystemMetrics(SM_CYCAPTION);


	/************************************************************************/
	/* These parameters are changed by h5nc (h5nc@yahoo.com.cn)             */
	/************************************************************************/
	rectW.left=(GetSystemMetrics(SM_CXSCREEN)-width)/4;
	rectW.top=(GetSystemMetrics(SM_CYSCREEN)-height)/4;
	rectW.right=rectW.left+width;
	rectW.bottom=rectW.top+height;
	styleW=WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

	rectFS.left=(GetSystemMetrics(SM_CXFULLSCREEN)-width)/4;
	rectFS.top=(GetSystemMetrics(SM_CYFULLSCREEN)-height)/4;
	rectFS.right=rectFS.left+width;
	rectFS.bottom=rectFS.top+height;
	styleFS=WS_POPUP|WS_VISIBLE; //WS_POPUP

	if(hwndParent)
	{
		rectW.left=0;
		rectW.top=0;
		rectW.right=nScreenWidth;
		rectW.bottom=nScreenHeight;
		styleW=WS_CHILD|WS_VISIBLE; 
		bWindowed=true;
	}

	if(bWindowed)
		hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, szWinTitle, styleW,
				rectW.left, rectW.top, rectW.right-rectW.left, rectW.bottom-rectW.top,
				hwndParent, NULL, hInstance, NULL);
	else
		hwnd = CreateWindowEx(WS_EX_TOPMOST, WINDOW_CLASS_NAME, szWinTitle, styleFS,
				0, 0, 0, 0,
				NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		_PostError("Can't create window");
		return false;
	}


	ShowWindow(hwnd, SW_SHOW);

	// Init subsystems

	timeBeginPeriod(1);
	Random_Seed();
	_InputInit();

	/************************************************************************/
	/* These lines are added by h5nc (h5nc@yahoo.com.cn)                    */
	/************************************************************************/
	// begin
	haveJoy = true;
	
	ZeroMemory(&joyState, sizeof(DIJOYSTATE));
	ZeroMemory(&lastJoyState, sizeof(DIJOYSTATE));

	switch(_DIInit())
	{
	case ERROR_NOKEYBOARD:
	case ERROR_NOKEYBOARD | ERROR_NOJOYSTICK:
		System_Shutdown();
		return false;
	case ERROR_NOJOYSTICK:
		haveJoy = false;
		break;
	}
	if(!_GfxInit()) { System_Shutdown(); return false; }
	if(!_SoundInit()) { System_Shutdown(); return false; }
	// end
	System_Log("Init done.\n");

	/************************************************************************/
	/* These parameters are changed by h5nc (h5nc@yahoo.com.cn)             */
	/************************************************************************/
	fTime=0.0f;
	fFPS=0;
	nFrameCounter = 0;
	nFrameSkip = 0;
	nRenderSkip = 0;

	return true;
}

void CALL HGE_Impl::System_Shutdown()
{
	System_Log("\nFinishing..");

	timeEndPeriod(1);
	if(hSearch) { FindClose(hSearch); hSearch=0; }
	_ClearQueue();
	_SoundDone();

	/************************************************************************/
	/* Blocks marked with "Yuki" are originally her codes                   */
	/* h5nc (h5nc@yahoo.com.cn) copied these codes under her permission     */
	/************************************************************************/
	/* add by Yuki */
	// begin
	_FontDone();
    // end

	/************************************************************************/
	/* This call is added by h5nc (h5nc@yahoo.com.cn)                       */
	/************************************************************************/
	_DIRelease();

	_GfxDone();

	if(hwnd)
	{
		//ShowWindow(hwnd, SW_HIDE);
		//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		//ShowWindow(hwnd, SW_SHOW);
		DestroyWindow(hwnd);
		hwnd=0;
	}
	if(hInstance) UnregisterClass(WINDOW_CLASS_NAME, hInstance);

	System_Log("The End.");
}

bool CALL HGE_Impl::System_Start()
{
	MSG		msg;

	/************************************************************************/
	/* These parameters are added by h5nc (h5nc@yahoo.com.cn)               */
	/************************************************************************/
	POINT	pt;
	RECT	rc;

	if(!hwnd)
	{
		_PostError("System_Start: System_Initiate wasn't called");
		return false;
	}

	if(!procFrameFunc) {
		_PostError("System_Start: No frame function defined");
		return false;
	}

	bActive=true;

	// MAIN LOOP

	for(;;)
	{
		
		// Process window messages if not in "child mode"
		// (if in "child mode" the parent application will do this for us)

		if(!hwndParent)
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{ 
				if (msg.message == WM_QUIT)	break;
				// TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
		}

		if(IsIconic(hwnd))
			continue;

		// Check if mouse is over HGE window for Input_IsMouseOver

		_UpdateMouse();

		// If HGE window is focused or we have the "don't suspend" state - process the main loop


		/************************************************************************/
		/* These lines are changed by h5nc (h5nc@yahoo.com.cn)                  */
		/* The core algorithm is based on LOVEHINA-AVC's code                   */
		/* h5nc copied his codes with his permission                            */
		/************************************************************************/
		// begin
		GetCursorPos(&pt);
		GetClientRect(hwnd, &rc);
		MapWindowPoints(hwnd, NULL, (LPPOINT)&rc, 2);
		if(bCaptured || (PtInRect(&rc, pt) && WindowFromPoint(pt)==hwnd)) bMouseOver=true;
		else bMouseOver=false;

		if(bActive || bDontSuspend) {
			int DI_retv = 0;
			if(nFrameSkip < 0)
			{
				for(int i=0;i<-nFrameSkip;i++)
				{
					DI_retv = _DIUpdate();
					if(DI_retv & ERROR_NOJOYSTICK)
					{
						haveJoy = false;
						if(DI_retv & ERROR_NOKEYBOARD)
						{
							break;
						}
					}
					if(procFrameFunc())
					{
						_ClearQueue();
						break;
					}
				}
			}
			else if(nFrameSkip < 2 || !(nFrameCounter % nFrameSkip))
			{
				DI_retv = _DIUpdate();
				if(DI_retv & ERROR_NOJOYSTICK)
				{
					haveJoy = false;
				}
				if(procFrameFunc())
				{
					_ClearQueue();
					break;
				}
			}
			if(DI_retv == (ERROR_NOJOYSTICK | ERROR_NOKEYBOARD))
				continue;
			if(nRenderSkip < 2 || !(nFrameCounter % nRenderSkip))
			{
				if(procRenderFunc) procRenderFunc();
			}
			if(hwndParent) break;

				// Clean up input events that were generated by
				// WindowProc and weren't handled by user's code


			int bPriorityRaised = 0;
		    LONGLONG TimeInterval, TimePrecision, NowTime;
			static LONGLONG lastTime = Timer_GetCurrentSystemTime();

			LARGE_INTEGER Frequency;
		    QueryPerformanceFrequency(&Frequency);
			TimePrecision =  Frequency.QuadPart;
			if (nHGEFPS > 0)
			{
				TimeInterval = TimePrecision / nHGEFPS;
			}
			else
			{
				TimeInterval = 0;
			}
   
			if (Timer_GetCurrentSystemTime() - lastTime >= TimeInterval)
		    {
				if (GetThreadPriority(GetCurrentThread()) > THREAD_PRIORITY_NORMAL)
					SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		    }
		    else
		    {
		        for (;;)
		        {
		            if (Timer_GetCurrentSystemTime() - lastTime >= (TimeInterval - (TimePrecision / (1000 * 2))))
		            {
		                if (!bPriorityRaised && GetThreadPriority(GetCurrentThread()) > THREAD_PRIORITY_NORMAL)
		                    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
						for (;;)
						{
						    if (Timer_GetCurrentSystemTime() - lastTime >= TimeInterval)
					            break;
				        }
			            break;
					}
		            else
		            {
		                if (!bPriorityRaised)
		                {
		                    if (GetThreadPriority(GetCurrentThread()) < THREAD_PRIORITY_HIGHEST)
								SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		                    bPriorityRaised = 1;
		                }
		                Sleep(1);
		            }
		        }
		    }

			NowTime = Timer_GetCurrentSystemTime();

		    if (lastTime <= (NowTime - TimeInterval) && lastTime >= (NowTime - (TimeInterval * 2)))
			{
				fDeltaTime = (float)((double)(TimeInterval) / (double)(TimePrecision));
		        lastTime += TimeInterval;
			}
		    else
			{
				fDeltaTime = (float)((double)(NowTime - lastTime) / (double)(TimePrecision));
		        lastTime = NowTime;
			}

			nFrameCounter++;
			fTime += fDeltaTime;
			fFPS = 1.0f / fDeltaTime;
		}
		// end

		// If main loop is suspended - just sleep a bit
		// (though not too much to allow instant window
		// redraw if requested by OS)

		else Sleep(1);
	}

	_ClearQueue();

	bActive=false;

	return true;
}
/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
bool CALL HGE_Impl::System_Set2DMode(float x, float y, float z)
{
	if (z == 0.0f)
	{
		return false;
	}
	b2DMode = true;
	ptfar.x = x;
	ptfar.y = y;
	ptfar.z = z;
	return true;
}

bool CALL HGE_Impl::System_Set3DMode()
{
	b2DMode = false;
	return true;
}

hge3DPoint * CALL HGE_Impl::System_GetFarPoint()
{
	if (!b2DMode)
	{
		return NULL;
	}
	return &ptfar;
}

bool CALL HGE_Impl::System_Is2DMode()
{
	return b2DMode;
}
// end

float CALL HGE_Impl::System_Transform3DPoint(float &x, float &y, float &z)
{
	if (!b2DMode || ptfar.z == 0.0f)
	{
		return 1.0f;
	}
	float scale = (ptfar.z - z) / ptfar.z;
	x = (x - ptfar.x) * scale + ptfar.x;
	y = (y - ptfar.y) * scale + ptfar.y;
	z = 0;
	return scale;
}

void CALL HGE_Impl::System_SetStateBool(hgeBoolState state, bool value)
{
	switch(state)
	{
		case HGE_WINDOWED:		if(VertArray || hwndParent) break;
								if(pD3DDevice && bWindowed != value)
								{
									if(d3dppW.BackBufferFormat==D3DFMT_UNKNOWN || d3dppFS.BackBufferFormat==D3DFMT_UNKNOWN) break;

									/************************************************************************/
									/* Theis cakk is added by h5nc (h5nc@yahoo.com.cn)                      */
									/************************************************************************/
									if(procFocusLostFunc) procFocusLostFunc();

									if(bWindowed) GetWindowRect(hwnd, &rectW);
									bWindowed=value;
									if(bWindowed) d3dpp=&d3dppW;
									else d3dpp=&d3dppFS;

									if(_format_id(d3dpp->BackBufferFormat) < 4) nScreenBPP=16;
									else nScreenBPP=32;

									_GfxRestore();
									_AdjustWindow();

									if(procFocusGainFunc) procFocusGainFunc();
								}
								else bWindowed=value;
								break;

		case HGE_ZBUFFER:		if(!pD3DDevice)	bZBuffer=value;
								break;

		case HGE_TEXTUREFILTER: bTextureFilter=value;
								if(pD3DDevice)
								{
									_render_batch();
									if(bTextureFilter)
									{
//										pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
//										pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
										pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
										pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
									}
									else
									{
//										pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
//										pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);
										pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
										pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
									}
								}
								break;

		case HGE_USESOUND:		if(bUseSound!=value)
								{
									bUseSound=value;
									if(bUseSound && hwnd) _SoundInit();
									if(!bUseSound && hwnd) _SoundDone();
								}
								break;

		case HGE_HIDEMOUSE:		bHideMouse=value; break;

		case HGE_DONTSUSPEND:	bDontSuspend=value; break;

			/************************************************************************/
			/* HGE_SHOWSPLASH case is deleted by h5nc (h5nc@yahoo.com.cn)           */
			/************************************************************************/
	}
}

void CALL HGE_Impl::System_SetStateFunc(hgeFuncState state, hgeCallback value)
{
	switch(state)
	{
		case HGE_FRAMEFUNC:		procFrameFunc=value; break;
		case HGE_RENDERFUNC:	procRenderFunc=value; break;
		case HGE_FOCUSLOSTFUNC:	procFocusLostFunc=value; break;
		case HGE_FOCUSGAINFUNC:	procFocusGainFunc=value; break;
		case HGE_GFXRESTOREFUNC: procGfxRestoreFunc=value; break;
		case HGE_EXITFUNC:		procExitFunc=value; break;
	}
}

void CALL HGE_Impl::System_SetStateHwnd(hgeHwndState state, HWND value)
{
	switch(state)
	{
		case HGE_HWNDPARENT:	if(!hwnd) hwndParent=value; break;
	}
}

void CALL HGE_Impl::System_SetStateInt(hgeIntState state, int value)
{
	switch(state)
	{
		case HGE_SCREENWIDTH:	if(!pD3DDevice) nScreenWidth=value; break;

		case HGE_SCREENHEIGHT:	if(!pD3DDevice) nScreenHeight=value; break;

		case HGE_SCREENBPP:		if(!pD3DDevice) nScreenBPP=value; break;

		case HGE_SAMPLERATE:	if(!hBass) nSampleRate=value;
								break;

		case HGE_FXVOLUME:		nFXVolume=value;
								_SetFXVolume(nFXVolume);
								break;

		case HGE_SAMPLEVOLUME:	nSampleVolume=value;
								_SetSampleVolume(nSampleVolume);
								break;
		case HGE_STREAMVOLUME:	nStreamVolume=value;
								_SetStreamVolume(nStreamVolume);
								break;


		case HGE_FPS:			if(VertArray) break;

								if(pD3DDevice)
								{
									if((nHGEFPS>=0 && value <0) || (nHGEFPS<0 && value>=0))
									{

										/************************************************************************/
										/* These blocks are added by h5nc (h5nc@yahoo.com.cn)                   */
										/************************************************************************/
										if(value==HGEFPS_VSYNC)
										{
//											d3dppW.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
											d3dppW.SwapEffect = D3DSWAPEFFECT_DISCARD;
//											d3dppFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
											d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
										}
										else
										{
											d3dppW.SwapEffect = D3DSWAPEFFECT_DISCARD;
//											d3dppFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
											d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
										}
										if(procFocusLostFunc) procFocusLostFunc();
										_GfxRestore();
										_DIRelease();
										if(procFocusGainFunc) procFocusGainFunc();
										_DIInit();
									}
								}
								nHGEFPS=value;
								break;

								/************************************************************************/
								/* These cases are added by h5nc (h5nc@yahoo.com.cn)                    */
								/************************************************************************/
		case HGE_FRAMESKIP:		nFrameSkip = value;
								break;
		case HGE_RENDERSKIP:	nRenderSkip = value;
								break;
	}
}

void CALL HGE_Impl::System_SetStateString(hgeStringState state, const char *value)
{
	FILE *hf;
	
	switch(state)
	{
		case HGE_ICON:			szIcon=value;
								if(pHGE->hwnd) SetClassLong(pHGE->hwnd, GCL_HICON, (LONG)LoadIcon(pHGE->hInstance, szIcon));
								break;
		case HGE_TITLE:			strcpy(szWinTitle,value);
								if(pHGE->hwnd) SetWindowText(pHGE->hwnd, szWinTitle);
								break;
		case HGE_INIFILE:		if(value) strcpy(szIniFile,Resource_MakePath(value));
								else szIniFile[0]=0;
								break;
		case HGE_LOGFILE:		if(value)
								{
									strcpy(szLogFile,Resource_MakePath(value));
									hf=fopen(szLogFile, "w");
									if(!hf) szLogFile[0]=0;
									else fclose(hf);
								}
								else szLogFile[0]=0;
								break;

								/************************************************************************/
								/* This case is added by h5nc (h5nc@yahoo.com.cn)                       */
								/************************************************************************/
		case HGE_BASSDLLFILE:	if (value)
								{
									strcpy(szBassDllFile,Resource_MakePath(value));
								}
								break;
	}
}

bool CALL HGE_Impl::System_GetStateBool(hgeBoolState state)
{
	switch(state)
	{
		case HGE_WINDOWED:		return bWindowed;
		case HGE_ZBUFFER:		return bZBuffer;
		case HGE_TEXTUREFILTER:	return bTextureFilter;
		case HGE_USESOUND:		return bUseSound;
		case HGE_DONTSUSPEND:	return bDontSuspend;
		case HGE_HIDEMOUSE:		return bHideMouse;

			/************************************************************************/
			/* HGE_SHOWSPLASH case is deleted by h5nc (h5nc@yahoo.com.cn)           */
			/************************************************************************/
	}
	return false;
}

hgeCallback CALL HGE_Impl::System_GetStateFunc(hgeFuncState state)
{
	switch(state)
	{
		case HGE_FRAMEFUNC:		return procFrameFunc;
		case HGE_RENDERFUNC:	return procRenderFunc;
		case HGE_FOCUSLOSTFUNC:	return procFocusLostFunc;
		case HGE_FOCUSGAINFUNC:	return procFocusGainFunc;
		case HGE_EXITFUNC:		return procExitFunc;
	}

	return NULL;
}

HWND CALL HGE_Impl::System_GetStateHwnd(hgeHwndState state)
{
	switch(state)
	{
		case HGE_HWND:			return hwnd;
		case HGE_HWNDPARENT:	return hwndParent;
	}

	return 0;
}

int CALL HGE_Impl::System_GetStateInt(hgeIntState state)
{
	switch(state)
	{
		case HGE_SCREENWIDTH:	return nScreenWidth;
		case HGE_SCREENHEIGHT:	return nScreenHeight;
		case HGE_SCREENBPP:		return nScreenBPP;
		case HGE_SAMPLERATE:	return nSampleRate;
		case HGE_FXVOLUME:		return nFXVolume;
		case HGE_SAMPLEVOLUME:	return nSampleVolume;
		case HGE_STREAMVOLUME:	return nStreamVolume;
		case HGE_FPS:			return nHGEFPS;

			/************************************************************************/
			/* These cases are added by h5nc (h5nc@yahoo.com.cn)                    */
			/************************************************************************/
		case HGE_FRAMECOUNTER:	return nFrameCounter;
		case HGE_FRAMESKIP:		return nFrameSkip;
		case HGE_RENDERSKIP:	return nRenderSkip;
	}

	return 0;
}

const char* CALL HGE_Impl::System_GetStateString(hgeStringState state) {
	switch(state) {
		case HGE_ICON:			return szIcon;
		case HGE_TITLE:			return szWinTitle;
		case HGE_INIFILE:		if(szIniFile[0]) return szIniFile;
								else return 0;
		case HGE_LOGFILE:		if(szLogFile[0]) return szLogFile;
								else return 0;
	}

	return NULL;
}

char* CALL HGE_Impl::System_GetErrorMessage()
{
	return szError;
}

void CALL HGE_Impl::System_Log(const char *szFormat, ...)
{
	FILE *hf = NULL;
	va_list ap;
	
	if(!szLogFile[0]) return;

	hf = fopen(szLogFile, "a");
	if(!hf) return;

	va_start(ap, szFormat);
	vfprintf(hf, szFormat, ap);
	va_end(ap);

	fprintf(hf, "\n");

	fclose(hf);
}

bool CALL HGE_Impl::System_Launch(const char *url)
{
	if((DWORD)ShellExecute(pHGE->hwnd, NULL, url, NULL, NULL, SW_SHOWMAXIMIZED)>32) return true;
	else return false;
}

void CALL HGE_Impl::System_Snapshot(const char *filename)
{
	LPDIRECT3DSURFACE9 pSurf;
	char *shotname, tempname[_MAX_PATH];
	int i;

	if(!filename)
	{
		i=0;
		shotname=Resource_EnumFiles("SnapShot/shot???.bmp");
		while(shotname)
		{
			i++;
			shotname=Resource_EnumFiles();
		}
		sprintf(tempname, "SnapShot/shot%03d.bmp", i);
		filename=Resource_MakePath(tempname);
	}

	if(pD3DDevice)
	{
		//+1st
		pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurf);
		D3DXSaveSurfaceToFile(filename, D3DXIFF_BMP, pSurf, NULL, NULL);
		pSurf->Release();
	}
}

//////// Implementation ////////


HGE_Impl::HGE_Impl()
{
	hInstance=GetModuleHandle(0);
	hwnd=0;

	/************************************************************************/
	/* This parameter is changed by h5nc (h5nc@yahoo.com.cn)                */
	/************************************************************************/
	bActive=true;
	szError[0]=0;

	pD3D=0;
	pD3DDevice=0;
	d3dpp=0;
	pTargets=0;
	pCurTarget=0;
	pScreenSurf=0;
	pScreenDepth=0;
	pVB=0;
	pIB=0;
	VertArray=0;
	textures=0;

	hBass=0;
	bSilent=false;
	streams=0;

	hSearch=0;
	res=0;

	queue=0;
	Char=VKey=Zpos=0;
	Xpos=Ypos=0.0f;
	bMouseOver=false;
	bCaptured=false;

	nHGEFPS=HGEFPS_UNLIMITED;
	fTime=0.0f;
	fDeltaTime=0.0f;
	/************************************************************************/
	/* This parameter is changed by h5nc (h5nc@yahoo.com.cn)                */
	/************************************************************************/
	fFPS=0;

	/************************************************************************/
	/* This parameter is added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	b2DMode = false;
	
	procFrameFunc=0;
	procRenderFunc=0;
	procFocusLostFunc=0;
	procFocusGainFunc=0;
	procGfxRestoreFunc=0;
	procExitFunc=0;
	szIcon=0;
	strcpy(szWinTitle,"HGE");
	nScreenWidth=800;
	nScreenHeight=600;
	nScreenBPP=32;
	bWindowed=false;
	bZBuffer=false;
	bTextureFilter=true;
	szLogFile[0]=0;
	szIniFile[0]=0;

	/************************************************************************/
	/* This parameter is added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	strcpy(szBassDllFile, "BASS.dll");
	bUseSound=true;
	nSampleRate=44100;
	nFXVolume=10000;
	nSampleVolume=10000;
	nStreamVolume=10000;
	bHideMouse=true;
	bDontSuspend=false;
	hwndParent=0;


	GetModuleFileName(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
	
	int i;
	for(i=strlen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
	szAppPath[i+1]=0;
	/************************************************************************/
	/* This parameter is added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	strcpy(szResourcePath, szAppPath);

	/************************************************************************/
	/* This parameter is added by Yuki                                      */
	/************************************************************************/
	/* add by Yuki */
	fontList = 0;
	
}

void HGE_Impl::_PostError(char *error)
{
	System_Log(error);
	strcpy(szError,error);
}

void HGE_Impl::_FocusChange(bool bAct)
{
	bActive=bAct;

	/************************************************************************/
	/* These blocks are modified by h5nc (h5nc@yahoo.com.cn)                */
	/************************************************************************/
	if(bActive) {
		_GfxRestore();
		if(procFocusGainFunc) procFocusGainFunc();
		_DIInit();
	}
	else
	{
		if(procFocusLostFunc) procFocusLostFunc();
		_DIRelease();
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool bActivating;

	switch(msg)
	{	
		case WM_CREATE: 
			return FALSE;
		
		case WM_PAINT:
			if(pHGE->pD3D && pHGE->procRenderFunc && pHGE->bWindowed) pHGE->procRenderFunc();
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return FALSE;

		case WM_ACTIVATE:
			// tricky: we should catch WA_ACTIVE and WA_CLICKACTIVE,
			// but only if HIWORD(wParam) (fMinimized) == FALSE (0)
			bActivating = (LOWORD(wparam) != WA_INACTIVE) && (HIWORD(wparam) == 0);
			if(pHGE->pD3D && pHGE->bActive != bActivating) pHGE->_FocusChange(bActivating);
			return FALSE;

		case WM_SETCURSOR:
			if(pHGE->bActive && LOWORD(lparam)==HTCLIENT && pHGE->bHideMouse) SetCursor(NULL);
			else SetCursor(LoadCursor(NULL, IDC_ARROW));
			return FALSE;

		case WM_SYSKEYDOWN:
			if(wparam==VK_F4)
			{
				if(pHGE->procExitFunc && !pHGE->procExitFunc()) return FALSE;
						return DefWindowProc(hwnd, msg, wparam, lparam);
				}

			/************************************************************************/
			/* This block is deleted by h5nc (h5nc@yahoo.com.cn)                    */
			/************************************************************************/
			/*
			else if(wparam == VK_RETURN)
			{
				pHGE->System_SetState(HGE_WINDOWED, !pHGE->System_GetState(HGE_WINDOWED));
				return FALSE;
			}
			*/
			else
			{
				pHGE->_BuildEvent(INPUT_KEYDOWN, wparam, HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? HGEINP_REPEAT:0, -1, -1);
				return FALSE;
			}

		case WM_KEYDOWN:
			pHGE->_BuildEvent(INPUT_KEYDOWN, wparam, HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? HGEINP_REPEAT:0, -1, -1);
			return FALSE;
		case WM_SYSKEYUP:
			pHGE->_BuildEvent(INPUT_KEYUP, wparam, HIWORD(lparam) & 0xFF, 0, -1, -1);
			return FALSE;
		case WM_KEYUP:
			pHGE->_BuildEvent(INPUT_KEYUP, wparam, HIWORD(lparam) & 0xFF, 0, -1, -1);
			return FALSE;

		case WM_LBUTTONDOWN:
			SetFocus(hwnd);
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_MBUTTONDOWN:
			SetFocus(hwnd);
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_RBUTTONDOWN:
			SetFocus(hwnd);
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_LBUTTONDBLCLK:
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_MBUTTONDBLCLK:
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_RBUTTONDBLCLK:
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_LBUTTONUP:
			pHGE->_BuildEvent(INPUT_MBUTTONUP, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_MBUTTONUP:
			pHGE->_BuildEvent(INPUT_MBUTTONUP, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_RBUTTONUP:
			pHGE->_BuildEvent(INPUT_MBUTTONUP, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_MOUSEMOVE:
			pHGE->_BuildEvent(INPUT_MOUSEMOVE, 0, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case 0x020A: // WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
			pHGE->_BuildEvent(INPUT_MOUSEWHEEL, short(HIWORD(wparam))/120, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_SIZE:
			if(pHGE->pD3D && wparam==SIZE_RESTORED) pHGE->_Resize(LOWORD(lparam), HIWORD(lparam));
			//return FALSE;
			break;

		case WM_SYSCOMMAND:
			if(wparam==SC_CLOSE)
			{
				if(pHGE->procExitFunc && !pHGE->procExitFunc()) return FALSE;
				pHGE->bActive=false;
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}
			break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}





