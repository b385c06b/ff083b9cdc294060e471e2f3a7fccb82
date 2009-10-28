/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: resources management
*/


#include "hge_impl.h"

#define NOCRYPT
//#define NOUNCRYPT
#include "ZLIB\unzip.h"
/************************************************************************/
/* This header file is added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
#include "ZLIB\zip.h"


/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
bool CALL HGE_Impl::Resource_AttachPack(const char *filename, int password)
{
	char *szName;
	CResourceList *resItem=res;
	unzFile zip;

	szName=Resource_MakePath(filename);
	strupr(szName);

	while(resItem)
	{
		if(!strcmp(szName,resItem->filename)) return true;
		resItem=resItem->next;
	}
	
	zip=unzOpen(szName);
	if(!zip) return false;
	unzClose(zip);

	resItem=new CResourceList;
	strcpy(resItem->filename, szName);
	if(password)
		resItem->password = password;
	else resItem->password=0;
	resItem->next=res;
	res=resItem;

	return true;
}

/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
bool CALL HGE_Impl::Resource_AddFileInPack(const char * filename, int password, hgeMemoryFile * memfile)
{
	if(!Resource_AttachPack(filename, password))
	{
		return Resource_CreatePack(filename, password, memfile, NULL);
	}

	zipFile zip = zipOpen(Resource_MakePath(filename), APPEND_STATUS_ADDINZIP);

	if(!zip)
		return false;

	if(Z_OK != 
		zipOpenNewFileInZip3(
		zip, memfile->filename, NULL,
		NULL, 0, NULL, 0, NULL,
		Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
		-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
		Resource_GetPSW(password), Resource_GetCRC(memfile->data, memfile->size)
		))
	{
		zipClose(zip, NULL);
		return false;
	}
	if(Z_OK != zipWriteInFileInZip(zip, memfile->data, memfile->size))
	{
		zipCloseFileInZip(zip);
		zipClose(zip, NULL);
		return false;
	}

	zipCloseFileInZip(zip);
	zipClose(zip, NULL);

	return true;
}

bool CALL HGE_Impl::Resource_CreatePack(const char * filename, int password, hgeMemoryFile * first, ...)
{
	Resource_RemovePack(filename);

	va_list ap;
	va_start(ap, first);

	zipFile zip = zipOpen(Resource_MakePath(filename), APPEND_STATUS_CREATE);

	if(!zip)
		return false;

	hgeMemoryFile * vai = first;

	while(vai != NULL)
	{
		if(Z_OK != 
			zipOpenNewFileInZip3(
			zip, vai->filename, NULL,
			NULL, 0, NULL, 0, NULL,
			Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
			-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
			Resource_GetPSW(password), Resource_GetCRC(vai->data, vai->size)
			))
		{
			zipClose(zip, NULL);
			return false;
		}
		if(Z_OK != zipWriteInFileInZip(zip, vai->data, vai->size))
		{
			zipCloseFileInZip(zip);
			zipClose(zip, NULL);
			return false;
		}
		zipCloseFileInZip(zip);

		vai = va_arg(ap, hgeMemoryFile *);
	}

	zipClose(zip, NULL);

	va_end(ap);

	return Resource_AttachPack(filename, password);
}
// end

void CALL HGE_Impl::Resource_RemovePack(const char *filename)
{
	char *szName;
	CResourceList *resItem=res, *resPrev=0;

	szName=Resource_MakePath(filename);
	strupr(szName);

	while(resItem)
	{
		if(!strcmp(szName,resItem->filename))
		{
			if(resPrev) resPrev->next=resItem->next;
			else res=resItem->next;
			delete resItem;
			break;
		}

		resPrev=resItem;
		resItem=resItem->next;
	}
}

void CALL HGE_Impl::Resource_RemoveAllPacks()
{
	CResourceList *resItem=res, *resNextItem;

	while(resItem)
	{
		resNextItem=resItem->next;
		delete resItem;
		resItem=resNextItem;
	}

	res=0;
}


/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
DWORD CALL HGE_Impl::Resource_GetCRC(const BYTE * content, DWORD size)
{
	return crc32(0, (Bytef *)content, size);
}

#ifdef ZLIB_USEPSW
int CALL HGE_Impl::Resource_GetPSW(int psw)
{
	return psw;
}
#else
char * CALL HGE_Impl::Resource_GetPSW(int psw)
{
	static char szPSWBuffer[20];
	strcpy(szPSWBuffer, "PSW");
	for (int i=0; i<4; i++)
	{
		psw = psw * 214013 + 2531011;
		szPSWBuffer[i*4+3] = (psw & 0xFF000000) >> 24;
		szPSWBuffer[i*4+1+3] = (psw & 0x00FF0000) >> 16;
		szPSWBuffer[i*4+2+3] = (psw & 0x0000FF00) >> 8;
		szPSWBuffer[i*4+3+3] = (psw & 0x000000FF);
	}
	szPSWBuffer[19] = 0;
	for (int i=3; i<19; i++)
	{
		if (szPSWBuffer[i] == 0)
		{
			szPSWBuffer[i] = szPSWBuffer[i-1];
		}
	}
	return szPSWBuffer;
	return (char *)psw;
}
#endif

char * CALL HGE_Impl::Resource_GetPackFirstFileName(const char * packfilename)
{
	unzFile zip;
	zip=unzOpen(Resource_MakePath(packfilename));
	if(UNZ_OK == unzGoToFirstFile(zip))
	{
		unzGetCurrentFileInfo(zip, NULL, szPackFirstFilename, sizeof(szPackFirstFilename), NULL, 0, NULL, 0);
		unzClose(zip);
		return szPackFirstFilename;
	}
	unzClose(zip);
	return NULL;
}
// end

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
BYTE * CALL HGE_Impl::Resource_Load(const char *filename, DWORD *size)
{
	static char *res_err="Can't load resource: %s";

	CResourceList *resItem=res;
	char szName[_MAX_PATH];
	char szZipName[_MAX_PATH];
	unzFile zip;
	unz_file_info file_info;
	int done, i;
	BYTE * ptr;
	HANDLE hF;

	if(size)
		*size = 0;

	if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':') goto _fromfile; // skip absolute paths

	// Load from pack
 
	strcpy(szName,filename);
	strupr(szName);
	for(i=0; szName[i]; i++) { if(szName[i]=='/') szName[i]='\\'; }

	while(resItem)
	{
		zip=unzOpen(resItem->filename);
		done=unzGoToFirstFile(zip);
		while(done==UNZ_OK)
		{
			unzGetCurrentFileInfo(zip, &file_info, szZipName, sizeof(szZipName), NULL, 0, NULL, 0);
			strupr(szZipName);
			for(i=0; szZipName[i]; i++)	{ if(szZipName[i]=='/') szZipName[i]='\\'; }
			if(!strcmp(szName,szZipName))
			{
				if(unzOpenCurrentFilePassword(zip, Resource_GetPSW(resItem->password)/* ? resItem->password : 0*/) != UNZ_OK)
				{
					unzClose(zip);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}

				ptr = (BYTE *)malloc(file_info.uncompressed_size);
				if(!ptr)
				{
					unzCloseCurrentFile(zip);
					unzClose(zip);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}

				if(unzReadCurrentFile(zip, ptr, file_info.uncompressed_size) < 0)
				{
					unzCloseCurrentFile(zip);
					unzClose(zip);
					free(ptr);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}
				unzCloseCurrentFile(zip);
				unzClose(zip);
				if(size) *size=file_info.uncompressed_size;
				return ptr;
			}
			
			done=unzGoToNextFile(zip);
		}
		
		unzClose(zip);
		resItem=resItem->next;
	}

	// Load from file
_fromfile:

	hF = CreateFile(Resource_MakePath(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if(hF == INVALID_HANDLE_VALUE)
	{
		sprintf(szName, res_err, filename);
		_PostError(szName);
		return 0;
	}
	file_info.uncompressed_size = GetFileSize(hF, NULL);
	ptr = (BYTE *)malloc(file_info.uncompressed_size);
	if(!ptr)
	{
		CloseHandle(hF);
		sprintf(szName, res_err, filename);
		_PostError(szName);
		return 0;
	}
	if(ReadFile(hF, ptr, file_info.uncompressed_size, &file_info.uncompressed_size, NULL ) == 0)
	{
		CloseHandle(hF);
		free(ptr);
		sprintf(szName, res_err, filename);
		_PostError(szName);
		return 0;
	}
	
	CloseHandle(hF);
	if(size) *size=file_info.uncompressed_size;
	return ptr;
}


void CALL HGE_Impl::Resource_Free(void *res)
{
	if(res) free(res);
}


/************************************************************************/
/* This function is added by h5nc (h5nc@yahoo.com.cn)                   */
/************************************************************************/
char* CALL HGE_Impl::Resource_SetPath(const char *filename)
{
	int i;

	if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':')
		strcpy(szResourcePath, filename);
	
	else
	{
		char szTmp[256];
//		strcpy(szResourcePath, szAppPath);
		strcpy(szTmp, filename);
		while(szTmp[0] == '.' && szTmp[1] == '.')
		{
			i = strlen(szResourcePath) - 1;
			if(szResourcePath[i] == '/' || szResourcePath[i] == '\\')
				szResourcePath[i] = 0;
			for(i = strlen(szResourcePath) - 1; i >= 0; i--)
			{
				if(szResourcePath[i] != '/' && szResourcePath[i] != '\\')
					szResourcePath[i] = 0;
				else
					break;
			}
			for(i = 0; i < (int)(strlen(szTmp) - 2); i++)
			{
				szTmp[i] = szTmp[i+3];
			}
		}
		if(szTmp) strcat(szResourcePath, szTmp);
	}

	for(i=0; szResourcePath[i]; i++) { if(szResourcePath[i]=='/') szResourcePath[i]='\\'; }
	if(szResourcePath[i-1] != '\\')
	{
		szResourcePath[i] = '\\';
		szResourcePath[i+1] = 0;
	}
	return szResourcePath;
}


/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
char* CALL HGE_Impl::Resource_MakePath(const char *filename)
{
	int i;

	if(!filename)
		strcpy(szTmpFilename, szAppPath);
	else if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':')
		strcpy(szTmpFilename, filename);
	
	else
	{
		char szTmp[256];
		strcpy(szTmpFilename, szResourcePath);
		strcpy(szTmp, filename);
		while(szTmp[0] == '.' && szTmp[1] == '.')
		{
			i = strlen(szTmpFilename) - 1;
			if(szTmpFilename[i] == '/' || szTmpFilename[i] == '\\')
				szTmpFilename[i] = 0;
			for(i = strlen(szTmpFilename) - 1; i >= 0; i--)
			{
				if(szTmpFilename[i] != '/' && szTmpFilename[i] != '\\')
					szTmpFilename[i] = 0;
				else
					break;
			}
			for(i = 0; i < (int)(strlen(szTmp) - 2); i++)
			{
				szTmp[i] = szTmp[i+3];
			}
		}
		if(szTmp) strcat(szTmpFilename, szTmp);
	}

	for(i=0; szTmpFilename[i]; i++) { if(szTmpFilename[i]=='/') szTmpFilename[i]='\\'; }
	return szTmpFilename;
}

char* CALL HGE_Impl::Resource_EnumFiles(const char *wildcard)
{
	if(wildcard)
	{
		if(hSearch) { FindClose(hSearch); hSearch=0; }
		hSearch=FindFirstFile(Resource_MakePath(wildcard), &SearchData);
		if(hSearch==INVALID_HANDLE_VALUE) { hSearch=0; return 0; }

		if(!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return SearchData.cFileName;
		else return Resource_EnumFiles();
	}
	else
	{
		if(!hSearch) return 0;
		for(;;)
		{
			if(!FindNextFile(hSearch, &SearchData))	{ FindClose(hSearch); hSearch=0; return 0; }
			if(!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return SearchData.cFileName;
		}
	}
}

char* CALL HGE_Impl::Resource_EnumFolders(const char *wildcard)
{
	if(wildcard)
	{
		if(hSearch) { FindClose(hSearch); hSearch=0; }
		hSearch=FindFirstFile(Resource_MakePath(wildcard), &SearchData);
		if(hSearch==INVALID_HANDLE_VALUE) { hSearch=0; return 0; }

		if((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
		   strcmp(SearchData.cFileName,".") && strcmp(SearchData.cFileName,".."))
				return SearchData.cFileName;
		else return Resource_EnumFolders();
	}
	else
	{
		if(!hSearch) return 0;
		for(;;)
		{
			if(!FindNextFile(hSearch, &SearchData))	{ FindClose(hSearch); hSearch=0; return 0; }
			if((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			   strcmp(SearchData.cFileName,".") && strcmp(SearchData.cFileName,".."))
					return SearchData.cFileName;
		}
	}
}
