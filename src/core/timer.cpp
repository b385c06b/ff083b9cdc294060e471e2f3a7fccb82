/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: timer
*/


#include "hge_impl.h"

void CALL HGE_Impl::Timer_GetSystemTime(WORD *wYear, WORD *wMonth, WORD *wDayOfWeek, WORD *wDay, WORD *wHour, WORD *wMinute, WORD *wSecond, WORD *wMilliseconds)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	if (wYear)
	{
		*wYear = systime.wYear;
	}
	if (wMonth)
	{
		*wMonth = systime.wMonth;
	}
	if (wDayOfWeek)
	{
		*wDayOfWeek = systime.wDayOfWeek;
	}
	if (wDay)
	{
		*wDay = systime.wDay;
	}
	if (wHour)
	{
		*wHour = systime.wHour;
	}
	if (wMinute)
	{
		*wMinute = systime.wMinute;
	}
	if (wSecond)
	{
		*wSecond = systime.wSecond;
	}
	if (wMilliseconds)
	{
		*wMilliseconds = systime.wMilliseconds;
	}
}

LONGLONG CALL HGE_Impl::Timer_GetFileTime()
{
	FILETIME filetime;
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	SystemTimeToFileTime(&systime, &filetime);
	return (((ULONGLONG)filetime.dwHighDateTime)<<32)|(filetime.dwLowDateTime);
}

float CALL HGE_Impl::Timer_GetTime()
{
	return fTime;
}

float CALL HGE_Impl::Timer_GetDelta()
{
	return fDeltaTime;
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
float CALL HGE_Impl::Timer_GetFPS(int mod)
{
	if(mod < 1)
		return fFPS;
	else
	{
		static float nowFPS = 0;
		static float fLastTime = 0;
		static DWORD nLastFrameCounter = 0;
		int modbase = nFrameSkip < 2 ? nFrameCounter : (nFrameCounter / nFrameSkip);
		if((modbase) % mod == 1)
		{
			if (nFrameCounter > nLastFrameCounter && fTime > fLastTime)
			{
				nowFPS = 1 / ((fTime - fLastTime) / (nFrameCounter - nLastFrameCounter));
			}
			fLastTime = fTime;
			nLastFrameCounter = nFrameCounter;
		}
		return nowFPS;
	}
}

/************************************************************************/
/* This function is added by h5nc (h5nc@yahoo.com.cn)                   */
/************************************************************************/
float CALL HGE_Impl::Timer_GetWorstFPS(int mod)
{
	if(mod <= 1)
		return fFPS;
	static float worstFPS = HGEFPS_MAX;
	static float worstFPSMod = worstFPS;
	if(worstFPS > fFPS)
		worstFPS = fFPS;
	int modbase = nFrameSkip < 2 ? nFrameCounter : (nFrameCounter / nFrameSkip);
	if((modbase) % mod == 1)
	{
		worstFPSMod = worstFPS;
		worstFPS = HGEFPS_MAX;
	}
	return worstFPSMod;
}
