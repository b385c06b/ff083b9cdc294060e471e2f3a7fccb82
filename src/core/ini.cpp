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
		_IniWritePrivateProfileString(section, name, buf, filename);
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
		if(_IniGetPrivateProfileString(section, name, "", buf, sizeof(buf), filename))
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
		_IniWritePrivateProfileString(section, name, buf, filename);
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
		if(_IniGetPrivateProfileString(section, name, "", buf, sizeof(buf), filename))
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
	if(filename[0]) _IniWritePrivateProfileString(section, name, value, filename);
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
	if(filename[0]) _IniGetPrivateProfileString(section, name, def_val, szIniString, sizeof(szIniString), filename);
	else strcpy(szIniString, def_val);
	return szIniString;
}

DWORD HGE_Impl::_IniGetPrivateProfileString(const char * appname, const char * keyname, const char * defval, char * retstr, DWORD size, const char * filename)
{
#ifdef __WIN32
	return GetPrivateProfileString(appname, keyname, defval, retstr, size, filename);
#else

	/************************************************************************/
	/* TODO                                                                 */
	/************************************************************************/
	if (!retstr)
	{
		return 0;
	}
	strcpy(retstr, defval);
	return size;

#endif // __WIN32
}

bool HGE_Impl::_IniWritePrivateProfileString(const char * appname, const char * keyname, const char * val, const char * filename)
{
#ifdef __WIN32
	return WritePrivateProfileString(appname, keyname, val, filename);
#else

	/************************************************************************/
	/* TODO                                                                 */
	/************************************************************************/
	return true;

#endif // __WIN32
}