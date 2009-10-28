#include "Export.h"
#include "ConstResource.h"

char Export::resourcefilename[M_PATHMAX];
char Export::resbinname[M_PATHMAX];
partInfo Export::partinfo[RPYPARTMAX];
replayInfo Export::rpyinfo;
int Export::password = 0;

Export::Export()
{
}

Export::~Export()
{
}

void Export::Release()
{
	hge->Release();
}

bool Export::clientInitial(bool usesound /* = false */, bool extuse /* = false */)
{
	hge->Resource_SetPath(DEFAULT_RESOURCEPATH);
	char respath[_MAX_PATH];
	strcpy(respath, hge->Resource_MakePath(""));
	if(_access(respath, 00) == -1)
	{
		CreateDirectory(respath, NULL);
	}
	SetCurrentDirectory(hge->Resource_MakePath(""));
	
	hge->System_SetState(HGE_FPS, M_DEFAULT_FPS);
	hge->System_SetState(HGE_FRAMESKIP, M_DEFAULT_FRAMESKIP);
	hge->System_SetState(HGE_RENDERSKIP, M_DEFAULT_RENDERSKIP);

	hge->System_SetState(HGE_WINDOWED, true);

	hge->System_SetState(HGE_SCREENWIDTH, M_CLIENT_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, M_CLIENT_HEIGHT);
	hge->System_SetState(HGE_ZBUFFER, true);

	hge->System_SetState(HGE_DONTSUSPEND, true);
	hge->System_SetState(HGE_USESOUND, usesound);
	hge->System_SetState(HGE_HIDEMOUSE, false);

	SetCurrentDirectory(hge->Resource_MakePath(""));
	bool bret = SetIni(extuse);
	if (bret)
	{
		char strtitle[M_STRMAX];
		strcpy(strtitle, GAME_TITLE);
		strcat(strtitle, "  ");
		strcat(strtitle, GAME_VERSION_STR);
		hge->System_SetState(HGE_TITLE, strtitle);
		hge->System_SetState(HGE_LOGFILE, LOG_STR_FILENAME);
	}

	if (!hge->Ini_GetInt(RESCONFIGS_SYSTEM, RESCONFIGN_USE3DMODE, RESCONFIGDEFAULT_USE3DMODE))
	{
		clientSet2DMode();
	}
	return bret;
}

void Export::clientSetMatrix(float _worldx, float _worldy, float _worldz)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, _worldx, _worldy, _worldz);	
	hge->Gfx_SetTransform( D3DTS_WORLD, &matWorld );

	if (hge->System_Is2DMode())
	{
		return;
	}
	/*
	D3DXMATRIX matView;
	D3DXVECTOR3 vEyePt( M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, M_ACTIVECLIENT_HEIGHT/2);
	D3DXVECTOR3 vLookatPt( M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, 0 );
	D3DXVECTOR3 vUpVec( 0.0f, -1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	*/

	D3DXMATRIX matView(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		-M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_Y, M_ACTIVECLIENT_HEIGHT/2.0f, 1.0f
		);
	
	hge->Gfx_SetTransform( D3DTS_VIEW, &matView );

	/*
	D3DXMATRIX matProj;
	D3DXMATRIX tmat;
	D3DXMatrixIdentity(&matProj);
	D3DXMatrixTranslation(&tmat, -0.5f, 0.5f, 0.0f);
	D3DXMatrixMultiply(&matProj, &matProj, &tmat);
	D3DXMatrixPerspectiveOffCenterLH( &tmat, -M_ACTIVECLIENT_CENTER_X, M_ACTIVECLIENT_CENTER_X+(M_CLIENT_WIDTH-M_ACTIVECLIENT_WIDTH), -M_CLIENT_CENTER_Y, M_CLIENT_CENTER_Y, M_CLIENT_HEIGHT/2, 0 );
	D3DXMatrixMultiply(&matProj, &matProj, &tmat);
	*/

	
	D3DXMATRIX matProj(
		M_CLIENT_HEIGHT/M_CLIENT_WIDTH, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		(M_ACTIVECLIENT_WIDTH-M_CLIENT_WIDTH)/M_CLIENT_WIDTH, 0.0f, 0.0f, 1.0f,
		-M_PROJECTIONMATRIX_OFFSET/*-M_PROJECTIONMATRIX_OFFSET*(M_CLIENT_HEIGHT/M_CLIENT_HEIGHT)*/, M_PROJECTIONMATRIX_OFFSET, 0.0f, -0.55f
		);

	hge->Gfx_SetTransform(D3DTS_PROJECTION, &matProj);
	
}

bool Export::clientSet2DMode(float x/* =M_ACTIVECLIENT_CENTER_X */, float y/* =M_ACTIVECLIENT_CENTER_Y */, float z/* =M_ACTIVECLIENT_CENTER_Y */)
{
	hge3DPoint ptfar;
	ptfar.x = x;
	ptfar.y = y;
	ptfar.z = z;
	return hge->System_Set2DMode(ptfar);
}

bool Export::clientSet3DMode()
{
	return hge->System_Set3DMode();
}

bool Export::SetIni(bool extuse)
{
	if (extuse && _access(hge->Resource_MakePath(CONFIG_STR_FILENAME), 00) == -1)
	{
		hge->System_SetState(HGE_INIFILE, CONFIG_STR_DEFAULTFILENAME);
		return false;
	}
	hge->System_SetState(HGE_INIFILE, CONFIG_STR_FILENAME);
	return true;
}

void Export::clientAdjustWindow()
{
	if (!hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, RESCONFIGDEFAULT_DEFAULTWINDOW))
	{
		int windowleft = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, RESCONFIGDEFAULT_WINDOWLEFT);
		int windowtop = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, RESCONFIGDEFAULT_WINDOWTOP);
		int windowwidth = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, RESCONFIGDEFAULT_WINDOWWIDTH);
		int windowheight = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, RESCONFIGDEFAULT_WINDOWHEIGHT);
		HWND windowtopmost = hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, RESCONFIGDEFAULT_WINDOWTOPMOST) ? HWND_TOPMOST: HWND_NOTOPMOST;
		SetWindowPos(hge->System_GetState(HGE_HWND), windowtopmost, windowleft, windowtop, windowwidth, windowheight, SWP_FRAMECHANGED);
	}
}

bool Export::GetResourceFile(bool readbin)
{
	strcpy(resourcefilename, hge->Ini_GetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESOURCEFILE, RESCONFIGDEFAULT_RESOURCEFILE));
	strcpy(resbinname, hge->Ini_GetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESBINNAME, RESCONFIGDEFAULT_RESBINNAME));
	if(strlen(resourcefilename) && !readbin)
	{
		strcpy(resourcefilename, hge->Resource_MakePath(resourcefilename));
		return false;
	}
	else
		strcpy(resourcefilename, resbinname);
	return true;
}

int Export::GetPassword()
{
	char szPassword[M_STRMAX];
	strcpy(szPassword, hge->Ini_GetString(RESCONFIGS_RESOURCE, RESCONFIGN_PASSWORD, RESCONFIGDEFAULT_PASSWORD));
	password = hge->Resource_GetCRC((BYTE *)szPassword, strlen(szPassword));
	return password;
}

bool Export::rpyLoad(const char * filename, replayInfo * _rpyinfo, partInfo * _partinfo, replayFrame * _replayframe)
{
	bool ret = false;

	BYTE * content;
	DWORD _size;

	hge->Resource_AttachPack(filename, password ^ REPLAYPASSWORD_XORMAGICNUM);
	content = hge->Resource_Load(hge->Resource_GetPackFirstFileName(filename), &_size);
	if(content)
	{
		memcpy(_rpyinfo ? _rpyinfo : &rpyinfo, content+RPYOFFSET_RPYINFO, RPYSIZE_RPYINFO);
		memcpy(_partinfo ? _partinfo : partinfo, content+RPYOFFSET_PARTINFO, RPYSIZE_PARTINFO*RPYPARTMAX);
		if (_replayframe)
		{
			memcpy(_replayframe, content+RPYOFFSET_INPUTDATA, _size-RPYOFFSET_INPUTDATA);
		}
		ret = true;
	}
	hge->Resource_Free(content);
	return ret;
}

bool Export::rpyFree(const char * filename)
{
	if (!filename)
	{
		return false;
	}
	hge->Resource_RemovePack(filename);
	return true;
}

bool Export::rpySetBias(replayFrame * _replayframe)
{
	if (!_replayframe)
	{
		return false;
	}

	float _bias = hge->Timer_GetFPS() - 60.0f;

	_replayframe->bias = 0;
	if (_bias < 0)
	{
		_replayframe->bias |= 0x80;
	}
	_replayframe->bias |= ((BYTE)(sqrtf(fabsf(_bias) * 256.0f)) & 0x7f);
	return true;
}

float Export::rpyGetReplayFPS(replayFrame _replayframe)
{
	int _bias = _replayframe.bias & 0x7f;
	_bias *= _bias;
	if (_replayframe.bias & 0x80)
	{
		_bias = -_bias;
	}
	return (_bias / 256.0f + 60.0f);
}

bool Export::packFile(const char * zipname, const char * filename)
{
	bool ret = false;
	BYTE * _content;
	DWORD _size;
	_content = hge->Resource_Load(filename, &_size);
	if(_content)
	{
		hgeMemoryFile memfile;
		memfile.data = _content;
		memfile.filename = (char *)filename;
		memfile.size = _size;
		if(hge->Resource_CreatePack(zipname, password, &memfile, NULL))
			ret = true;
		hge->Resource_Free(_content);
	}
	return ret;
}

bool Export::packFolder(const char * zipname, const char * foldername, const char * filterstr, int * initcount)
{
	bool ret = true;
	WIN32_FIND_DATA SearchData;

	char buffer[_MAX_PATH];
	strcpy(buffer, foldername);
	if(buffer[strlen(buffer)-1]!='\\' && buffer[strlen(buffer)-1]!='/')
	{
		strcat(buffer, "\\");
	}
	strcat(buffer, "*");

	HANDLE hSearch = FindFirstFile(buffer, &SearchData);

	char filenamebuffer[M_STRMAX];
	char filter[M_STRITOAMAX][M_STRMAX];
	ZeroMemory(filter, sizeof(char) * M_STRITOAMAX * M_STRMAX);
	int filtercount = 0;

	int tfiltercount = 0;
	for (int i=0; i<strlen(filterstr); i++)
	{
		if (filtercount >= M_STRITOAMAX)
		{
			break;
		}
		if (filterstr[i] == ';')
		{
			filtercount++;
			tfiltercount = 0;
			continue;
		}
		if (isspace(filterstr[i]) || filterstr[i] == '*')
		{
			continue;
		}
		filter[filtercount][tfiltercount] = tolower(filterstr[i]);
		if (filter[filtercount][tfiltercount] == '/')
		{
			filter[filtercount][tfiltercount] = '\\';
		}
		tfiltercount++;
	}

#ifdef __UNPACK
	int packagenum = 0;
	char sec[M_STRMAX];
	char name[M_STRMAX];
	char inifilename[M_STRMAX];
	char packnamebuffer[M_STRMAX];
	int typecount = 0;
	if (initcount)
	{
		typecount = *initcount;
	}

	strcpy(inifilename, hge->Resource_MakePath(UNPACK_INIFILENAME));
	strcpy(name, UNPACK_PACKNAME);
	while (true)
	{
		sprintf(sec, "%s%d", UNPACK_SECTION, packagenum);
		GetPrivateProfileString(sec, name, "", packnamebuffer, M_STRMAX, inifilename);
		if (strlen(packnamebuffer) == 0)
		{
			break;
		}
		if (!strcmp(packnamebuffer, zipname))
		{
			while (true)
			{
				sprintf(name, "%s%d", UNPACK_TYPE, typecount);
				GetPrivateProfileString(sec, name, "", packnamebuffer, M_STRMAX, inifilename);
				if (!strlen(packnamebuffer))
				{
					break;
				}
				if (initcount && !(*initcount))
				{
					WritePrivateProfileString(sec, name, "", inifilename);
				}
				typecount++;
			}
			break;
		}
		packagenum++;
	}
	if (initcount)
	{
		typecount = *initcount;
	}
	else
	{
		typecount = 0;
	}

#endif

	while(hSearch != INVALID_HANDLE_VALUE)
	{
		bool filtered = false;
		strcpy(filenamebuffer, SearchData.cFileName);
		for (int i=0; i<strlen(filenamebuffer); i++)
		{
			filenamebuffer[i] = tolower(filenamebuffer[i]);
			if (filenamebuffer[i] == '/')
			{
				filenamebuffer[i] = '\\';
			}
		}
		for (int i=0; i<M_STRITOAMAX; i++)
		{
			if (!strlen(filter[i]))
			{
				break;
			}
			string tstr = filenamebuffer;
			if (tstr.find(filter[i]) != string::npos)
			{
				filtered = true;
				break;
			}
		}
		if (!filtered)
		{
			if((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				strcmp(filenamebuffer,".") && strcmp(filenamebuffer,".."))
			{
				strcpy(buffer, foldername);
				if(buffer[strlen(buffer)-1]!='\\' && buffer[strlen(buffer)-1]!='/')
				{
					strcat(buffer, "\\");
				}
				strcat(buffer, filenamebuffer);
				if(!packFolder(zipname, buffer, filterstr 
#ifdef __UNPACK	
					, &typecount
#endif
					))
				{
					ret = false;
					break;
				}
				strcpy(buffer, foldername);
			}
			else if(!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				strcpy(buffer, foldername);
				if(buffer[strlen(buffer)-1]!='\\' && buffer[strlen(buffer)-1]!='/')
				{
					strcat(buffer, "\\");
				}
				strcat(buffer, filenamebuffer);
				BYTE * _content;
				DWORD _size;
				_content = hge->Resource_Load(buffer, &_size);
				if(_content)
				{
					hgeMemoryFile memfile;
					memfile.data = _content;
					memfile.filename = buffer;
					memfile.size = _size;
					if(!hge->Resource_AddFileInPack(zipname, password, &memfile))
					{
						ret = false;
						break;
					}
#ifdef __UNPACK
					sprintf(name, "%s%d", UNPACK_TYPE, typecount);
					WritePrivateProfileString(sec, name, buffer, inifilename);
					typecount++;
#endif
					hge->Resource_Free(_content);
				}
			}
		}
		if(!FindNextFile(hSearch, &SearchData))
			break;
	}
	FindClose(hSearch);
#ifdef __UNPACK
	if (ret)
	{
		if (initcount)
		{
			*initcount = typecount;
		}
		strcpy(name, UNPACK_PACKNAME);
		WritePrivateProfileString(sec, name, zipname, inifilename);
	}
#endif
	return ret;
}

bool Export::effSave(const char * filename, hgeEffectSystem * eff, int texnum)
{
	if (!eff || texnum < 0)
	{
		return false;
	}
	return eff->Save(filename, texnum);
}

int Export::effLoad(const char * filename, hgeEffectSystem * eff, HTEXTURE * tex)
{
	if(!eff)
		return -1;
	return eff->Load(filename, 0, tex);
}

#ifdef __UNPACK
bool Export::unpackFile(const char * zipname, const char * filename)
{
	bool ret = false;
	BYTE * _content;
	DWORD _size;

	if (!hge->Resource_AttachPack(zipname, password))
	{
		return false;
	}

	_content = hge->Resource_Load(filename, &_size);
	if(_content)
	{
		char pathbuffer[M_STRMAX];
		for (int i=0; i<strlen(filename); i++)
		{
			if (filename[i] == '/' || filename[i] == '\\')
			{
				strncpy(pathbuffer, filename, i);
				pathbuffer[i] = 0;

				if (!_access(hge->Resource_MakePath(pathbuffer), 00))
				{
					continue;
				}
				CreateDirectory(hge->Resource_MakePath(pathbuffer), NULL);
			}
		}
		FILE * file = fopen(hge->Resource_MakePath(filename), "wb");
		if (!file)
		{
			return false;
		}
		fwrite(_content, _size, 1, file);
		fclose(file);
		hge->Resource_Free(_content);
		ret = true;
	}
	return ret;
}

bool Export::unpackFromIni(const char * inifilename)
{
	char fullinifilename[M_STRMAX];
	char zipname[M_STRMAX];

	strcpy(fullinifilename, hge->Resource_MakePath(inifilename));
	char sec[M_STRMAX];
	char name[M_STRMAX];

	int packagecount=0;
	while (true)
	{
		sprintf(sec, "%s%d", UNPACK_SECTION, packagecount);
		strcpy(name, UNPACK_PACKNAME);
		GetPrivateProfileString(sec, name, "", zipname, M_STRMAX, fullinifilename);
		if (!strlen(zipname))
		{
			if (packagecount > 0)
			{
				return true;
			}
			return false;
		}
		if (!hge->Resource_AttachPack(zipname, password))
		{
			return false;
		}
		packagecount++;

		char eachfilename[M_STRMAX];
		int typecount=0;
		while (true)
		{
			sprintf(name, "%s%d", UNPACK_TYPE, typecount);
			GetPrivateProfileString(sec, name, "", eachfilename, M_STRMAX, fullinifilename);
			typecount++;
			if (!strcmp(eachfilename, ""))
			{
				break;
			}
			if (!unpackFile(zipname, eachfilename))
			{
				return false;
			}
		}
	}
	return true;
}
#endif
