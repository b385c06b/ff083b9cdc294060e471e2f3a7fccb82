/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: ini file
*/


#include "hge_impl.h"


void CALL HGE_Impl::Ini_SetInt(const char *section, const char *name, int value, char * inifilename/* =NULL */)
{
	char buf[256];

	char * filename = inifilename;
	if (!filename)
	{
		filename = szIniFile;
	}
	else
	{
		filename = Resource_MakePath(filename);
	}

	if(filename[0]) {
		sprintf(buf,"%d",value);
		WritePrivateProfileString(section, name, buf, filename);
	}
}


int CALL HGE_Impl::Ini_GetInt(const char *section, const char *name, int def_val, char * inifilename/* =NULL */)
{
	char buf[256];

	char * filename = inifilename;
	if (!filename)
	{
		filename = szIniFile;
	}
	else
	{
		filename = Resource_MakePath(filename);
	}

	if(filename[0]) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), filename))
		{ return Math_atoi(buf); }
		else { return def_val; }
	}
	return def_val;
}


void CALL HGE_Impl::Ini_SetFloat(const char *section, const char *name, float value, char * inifilename/* =NULL */)
{
	char buf[256];

	char * filename = inifilename;
	if (!filename)
	{
		filename = szIniFile;
	}
	else
	{
		filename = Resource_MakePath(filename);
	}

	if(filename[0]) {
		sprintf(buf,"%f",value);
		WritePrivateProfileString(section, name, buf, filename);
	}
}


float CALL HGE_Impl::Ini_GetFloat(const char *section, const char *name, float def_val, char * inifilename/* =NULL */)
{
	char buf[256];

	char * filename = inifilename;
	if (!filename)
	{
		filename = szIniFile;
	}
	else
	{
		filename = Resource_MakePath(filename);
	}

	if(filename[0]) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), filename))
		{ return (float)Math_atof(buf); }
		else { return def_val; }
	}
	return def_val;
}


void CALL HGE_Impl::Ini_SetString(const char *section, const char *name, const char *value, char * inifilename/* =NULL */)
{
	char * filename = inifilename;
	if (!filename)
	{
		filename = szIniFile;
	}
	else
	{
		filename = Resource_MakePath(filename);
	}
	if(filename[0]) WritePrivateProfileString(section, name, value, filename);
}


char* CALL HGE_Impl::Ini_GetString(const char *section, const char *name, const char *def_val, char * inifilename/* =NULL */)
{
	char * filename = inifilename;
	if (!filename)
	{
		filename = szIniFile;
	}
	else
	{
		filename = Resource_MakePath(filename);
	}
	if(filename[0]) GetPrivateProfileString(section, name, def_val, szIniString, sizeof(szIniString), filename);
	else strcpy(szIniString, def_val);
	return szIniString;
}

