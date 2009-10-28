#include "BResource.h"
#include "data.h"
#include "ConstResource.h"

BResource res;

BResource::BResource()
{
	ZeroMemory(&resdata, RSIZE_RESOURCE);
	customconstdata = NULL;
}

BResource::~BResource()
{
	ReleaseCustomConst();
}

void BResource::ReleaseCustomConst()
{
	if (customconstdata)
	{
		free(customconstdata);
		customconstdata = NULL;
	}
}

//Scripter::LoadAll
bool BResource::Fill()
{
	DWORD sec;
	DWORD name;

	//resource
	strcpy(resdata.widefontname, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_FONT), data.nLinkType(RESDATAN_FONT), RESDEFAULT_FONTFONT));

	sec = data.sLinkType(RESDATAS_FOLDER);
	for (int i=0; i<M_SCRIPTFOLDERMAX; i++)
	{
		strcpy(resdata.scriptfoldername[i], data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SCRIPTFOLDER_1+i), RESDEFAULT_SCRIPTFOLDER));
		if (!strlen(resdata.scriptfoldername[i]))
		{
			continue;
		}
		if(_access(resdata.scriptfoldername[i], 00) == -1)
		{
			CreateDirectory(resdata.scriptfoldername[i], NULL);
		}
	}
	strcpy(resdata.snapshotfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SNAPSHOTFOLDER), RESDEFAULT_SNAPSHOTFOLDER));
	if(_access(resdata.snapshotfoldername, 00) == -1)
	{
		CreateDirectory(resdata.snapshotfoldername, NULL);
	}
	strcpy(resdata.replayfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_REPLAYFOLDER), RESDEFAULT_REPLAYFOLDER));
	if(_access(resdata.replayfoldername, 00) == -1)
	{
		CreateDirectory(resdata.replayfoldername, NULL);
	}
	strcpy(resdata.datafoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_DATAFOLDER), RESDEFAULT_DATAFOLDER));
	if(_access(resdata.datafoldername, 00) == -1)
	{
		CreateDirectory(resdata.datafoldername, NULL);
	}
	strcpy(resdata.effectsysfoldername, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_EFFECTSYSFOLDER), RESDEFAULT_DATAFOLDER));
	if(_access(resdata.effectsysfoldername, 00) == -1)
	{
		CreateDirectory(resdata.effectsysfoldername, NULL);
	}

	char buffer[M_STRMAX];

	sec = data.sLinkType(RESDATAS_DATAFILE);

	strcpy(resdata.binname, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_BINFILE), RESDEFAULT_DATABIN));
	strcpy(resdata.binfilename, resdata.datafoldername);
	strcat(resdata.binfilename, resdata.binname);
	//copy to data
	//

	strcpy(resdata.rabinname, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SPELLACCESSFILE), RESDEFAULT_DATASPELLACCESS));
	strcpy(resdata.spellaccessfilename, resdata.datafoldername);
	strcat(resdata.spellaccessfilename, resdata.rabinname);
	//copy to data
	//

	strcpy(resdata.scrbinname, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SCRIPTFILE), RESDEFAULT_DATASCRIPT));
	strcpy(resdata.scriptfilename, resdata.datafoldername);
	strcat(resdata.scriptfilename, resdata.scrbinname);
	//copy to data
	//

	strcpy(resdata.customconstfilename, resdata.datafoldername);
	strcat(resdata.customconstfilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_CUSTOMCONSTFILE), RESDEFAULT_DATACUSTOMCONST));
	//copy to data
	//

	strcpy(resdata.spelldefinefilename, resdata.datafoldername);
	strcat(resdata.spelldefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SPELLDEFINEFILE), RESDEFAULT_DATASPELLDEFINE));
	//copy to data
	//

	strcpy(resdata.musicdefinefilename, resdata.datafoldername);
	strcat(resdata.musicdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_MUSICDEFINEFILE), RESDEFAULT_DATAMUSICDEFINE));
	//copy to data
	//

	strcpy(resdata.bulletdefinefilename, resdata.datafoldername);
	strcat(resdata.bulletdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_BULLETDEFINEFILE), RESDEFAULT_DATABULLETDEFINE));
	//copy to data
	//

	strcpy(resdata.enemydefinefilename, resdata.datafoldername);
	strcat(resdata.enemydefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_ENEMYDEFINEFILE), RESDEFAULT_ENEMYDEFINE));
	//copy to data
	//

	strcpy(resdata.playerdefinefilename, resdata.datafoldername);
	strcat(resdata.playerdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_PLAYERDEFINEFILE), RESDEFAULT_DATAPLAYERDEFINE));
	//copy to data
	//

	strcpy(resdata.spritedefinefilename, resdata.datafoldername);
	strcat(resdata.spritedefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_SPRITEDEFINEFILE), RESDEFAULT_DATASPRITEDEFINE));
	//copy to data
	//

	strcpy(resdata.playerbulletdefinefilename, resdata.datafoldername);
	strcat(resdata.playerbulletdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_PLAYERBULLETDEFINEFILE), RESDEFAULT_DATAPLAYERBULLETDEFINE));
	//copy to data
	//

	strcpy(resdata.playershootdefinefilename, resdata.datafoldername);
	strcat(resdata.playershootdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_PLAYERSHOOTDEFINEFILE), RESDEFAULT_DATAPLAYERSHOOTDEFINE));
	//copy to data
	//

	strcpy(resdata.playerghostdefinefilename, resdata.datafoldername);
	strcat(resdata.playerghostdefinefilename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_PLAYERGHOSTDEFINEFILE), RESDEFAULT_DATAPLAYERGHOSTDEFINE));
	//copy to data
	//

	name = data.nLinkType(RESDATAN_TYPE);
	for(int i=0;i<PACKAGEMAX;i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.packagefilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_PACKAGE), name, ""));
	}

	for(int i=0;i<TEXMAX;i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.texfilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_TEXTURE), name, ""));
	}

	for(int i=0;i<SEMAX;i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.sefilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_SE), name, ""));
	}

	for(int i=0; i<EFFECTSYSTYPEMAX; i++)
	{
		name = data.nLinkNum(name, i+1);
		strcpy(resdata.effectsysfilename[i], data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_EFFECTSYS), name, ""));
	}

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_EXTENSION), data.nLinkType(RESDATAN_SCRIPTEXT7), RESDEFAULT_SCRIPTEXT7));
	if(strlen(buffer) > 8)
	{
#ifdef __DEBUG
		HGELOG("%s\nExtension %s for Script File is too long.(Required to be less than 8).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.scriptextensionname7, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_EXTENSION), data.nLinkType(RESDATAN_REPLAYEXT7), RESDEFAULT_REPLAYEXT7));
	if(strlen(buffer) > 8)
	{
#ifdef __DEBUG
		HGELOG("%s\nExtension %s for Replay File is too long.(Required to be less than 8).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replayextensionname7, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYSIGNATURE11), RESDEFAULT_RPYSIGNATURE11));
	if(strlen(buffer) > 12)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Signature %s is too long.(Required to be less than 12.)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaysignature11, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYTEMPSIGN3), RESDEFAULT_RPYTEMPSIGN3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Temp Sign %s is too long.(Required to be less than 4).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaytempsign3, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYCOMPLETESIGN3), RESDEFAULT_RPYCOMPLETESIGN3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Complete Sign %s is too long.(Required to be less than 4).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaycompletesign3, buffer);

	strcpy(buffer, data.sRead(DATA_RESOURCEFILE, data.sLinkType(RESDATAS_REPLAYHEADER), data.nLinkType(RESDATAN_RPYTAG3), RESDEFAULT_RPYTAG3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Tag %s is too long.(Required to be less than 4).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaytag3, buffer);

	GetUIString();
	CopyData();
	return data.GetAllTable();
}

void BResource::GetUIString()
{
	DWORD sec = data.sLinkType(RESDATAS_UISTRING);	
	strcpy(resdata.uistr.filename, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_FILENAME), RESDEFAULT_UISTR_FILENAME));
	strcpy(resdata.uistr.username, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_USERNAME), RESDEFAULT_UISTR_USERNAME));
	strcpy(resdata.uistr.score, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_SCORE), RESDEFAULT_UISTR_SCORE));
	strcpy(resdata.uistr.point, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_POINT), RESDEFAULT_UISTR_POINT));
	strcpy(resdata.uistr.faith, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_FAITH), RESDEFAULT_UISTR_FAITH));
	strcpy(resdata.uistr.alltime, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_ALLTIME), RESDEFAULT_UISTR_ALLTIME));
	strcpy(resdata.uistr.date, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_DATE), RESDEFAULT_UISTR_DATE));
	strcpy(resdata.uistr.lost, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_LOST), RESDEFAULT_UISTR_LOST));
	strcpy(resdata.uistr.borderrate, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_BORDERRATE), RESDEFAULT_UISTR_BORDERRATE));
	strcpy(resdata.uistr.fastrate, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_FASTRATE), RESDEFAULT_UISTR_FASTRATE));
	strcpy(resdata.uistr.modeflag, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MODEFLAG), RESDEFAULT_UISTR_MODEFLAG));
	strcpy(resdata.uistr.mode_none, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MODE_NONE), RESDEFAULT_UISTR_MODE_NONE));
	strcpy(resdata.uistr.mode_practice, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MODE_PRACTICE), RESDEFAULT_UISTR_MODE_PRACTICE));
	strcpy(resdata.uistr.mode_spell, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MODE_SPELL), RESDEFAULT_UISTR_MODE_SPELL));
	strcpy(resdata.uistr.usingchara, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_USINGCHARA), RESDEFAULT_UISTR_USINGCHARA));
//	strcpy(resdata.uistr.maxplayer, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MAXPLAYER), RESDEFAULT_UISTR_MAXPLAYER));
	strcpy(resdata.uistr.misstime, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MISSTIME), RESDEFAULT_UISTR_MISSTIME));
	strcpy(resdata.uistr.bordertime, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_BORDERTIME), RESDEFAULT_UISTR_BORDERTIME));
	strcpy(resdata.uistr.continuetime, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_CONTINUETIME), RESDEFAULT_UISTR_CONTINUETIME));
	strcpy(resdata.uistr.gettime, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_GETTIME), RESDEFAULT_UISTR_GETTIME));
	strcpy(resdata.uistr.pausetime, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_PAUSETIME), RESDEFAULT_UISTR_PAUSETIME));
	strcpy(resdata.uistr.difflv, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_DIFFLV), RESDEFAULT_UISTR_DIFFLV));
	strcpy(resdata.uistr.laststage, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_LASTSTAGE), RESDEFAULT_UISTR_LASTSTAGE));
	strcpy(resdata.uistr.graze, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_GRAZE), RESDEFAULT_UISTR_GRAZE));
	strcpy(resdata.uistr.confirm, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_CONFIRM), RESDEFAULT_UISTR_CONFIRM));

}

bool BResource::Pack(void * pStrdesc, void * pCustomConstData)
{
	if(pStrdesc == NULL || pCustomConstData == NULL)
		return false;

	DWORD size = M_BINHEADER_OFFSET + 
		RSIZE_RESOURCE + 
		RSIZE_STRINGDESC + 
		RSIZE_CUSTOMCONST + 
		RSIZE_MUSIC + 
		RSIZE_BULLET + 
		RSIZE_ENEMY + 
		RSIZE_PLAYER + 
		RSIZE_SPRITE + 
		RSIZE_PLAYERBULLET + 
		RSIZE_PLAYERSHOOT + 
		RSIZE_PLAYERGHOST + 
		sizeof(spellData) * spelldata.size();
	BYTE * content = (BYTE *)malloc(size);
	if(!content)
		return false;

	DWORD offset;

	BYTE * _header = data.CreateMemHeader(DATA_RESOURCEFILE);
	if(!_header)
		return false;
	memcpy(content, _header, M_BINHEADER_OFFSET);
	offset = M_BINHEADER_OFFSET;
	free(_header);

	memcpy(content+offset, &resdata, RSIZE_RESOURCE);
	offset += RSIZE_RESOURCE;
	memcpy(content+offset, pStrdesc, RSIZE_STRINGDESC);
	offset += RSIZE_STRINGDESC;
	memcpy(content+offset, pCustomConstData, RSIZE_CUSTOMCONST);
	offset += RSIZE_CUSTOMCONST;
	memcpy(content+offset, musdata, RSIZE_MUSIC);
	offset += RSIZE_MUSIC;
	memcpy(content+offset, bulletdata, RSIZE_BULLET);
	offset += RSIZE_BULLET;
	memcpy(content+offset, enemydata, RSIZE_ENEMY);
	offset += RSIZE_ENEMY;
	memcpy(content+offset, playerdata, RSIZE_PLAYER);
	offset += RSIZE_PLAYER;
	memcpy(content+offset, spritedata, RSIZE_SPRITE);
	offset += RSIZE_SPRITE;
	memcpy(content+offset, playerbulletdata, RSIZE_PLAYERBULLET);
	offset += RSIZE_PLAYERBULLET;
	memcpy(content+offset, playershootdata, RSIZE_PLAYERSHOOT);
	offset += RSIZE_PLAYERSHOOT;
	memcpy(content+offset, playerghostdata, RSIZE_PLAYERGHOST);
	offset += RSIZE_PLAYERGHOST;
	for(vector<spellData>::iterator i=spelldata.begin(); i!=spelldata.end(); i++)
	{
		memcpy(content+offset, &(*i), sizeof(spellData));
		offset += sizeof(spellData);
	}

	hgeMemoryFile memfile;
	memfile.data = content;
	memfile.filename = data.resbinname;
	memfile.size = size;

	bool ret = false;
	ret = hge->Resource_CreatePack(hge->Resource_MakePath(data.resbinname), data.password, &memfile, NULL);

	free(content);

	return ret;
}

bool BResource::Gain(void * pStrdesc, void * pCustomConstData)
{
	BYTE * content;
	DWORD size;
	bool ret = false;

	hge->Resource_AttachPack(data.resourcefilename, data.password);
	content = hge->Resource_Load(data.resbinname, &size);
	if(content)
	{
		spelldata.clear();
		if(data.CheckMemHeader(content, size, DATA_RESOURCEFILE))
		{
			DWORD offset = M_BINHEADER_OFFSET;
			memcpy(&resdata, content+offset, RSIZE_RESOURCE);
			offset += RSIZE_RESOURCE;
			if(pStrdesc)
				memcpy(pStrdesc, content+offset, RSIZE_STRINGDESC);
			offset += RSIZE_STRINGDESC;
			if(pCustomConstData)
				memcpy(pCustomConstData, content+offset, RSIZE_CUSTOMCONST);
			offset += RSIZE_CUSTOMCONST;
			memcpy(musdata, content+offset, RSIZE_MUSIC);
			offset += RSIZE_MUSIC;
			memcpy(bulletdata, content+offset, RSIZE_BULLET);
			offset += RSIZE_BULLET;
			memcpy(enemydata, content+offset, RSIZE_ENEMY);
			offset += RSIZE_ENEMY;
			memcpy(playerdata, content+offset, RSIZE_PLAYER);
			offset += RSIZE_PLAYER;
			memcpy(spritedata, content+offset, RSIZE_SPRITE);
			offset += RSIZE_SPRITE;
			memcpy(playerbulletdata, content+offset, RSIZE_PLAYERBULLET);
			offset += RSIZE_PLAYERBULLET;
			memcpy(playershootdata, content+offset, RSIZE_PLAYERSHOOT);
			offset += RSIZE_PLAYERSHOOT;
			memcpy(playerghostdata, content+offset, RSIZE_PLAYERGHOST);
			offset += RSIZE_PLAYERGHOST;
			while(offset < size)
			{
				spellData _rdata;
				memcpy(&_rdata, content+offset, sizeof(spellData));
				offset += sizeof(spellData);
				spelldata.push_back(_rdata);
			}
			CopyData();
			if(SetDataFile())
				ret = true;
		}
	}
	hge->Resource_Free(content);

	if(_access(resdata.snapshotfoldername, 00) == -1)
	{
		CreateDirectory(resdata.snapshotfoldername, NULL);
	}
	if(_access(resdata.replayfoldername, 00) == -1)
	{
		CreateDirectory(resdata.replayfoldername, NULL);
	}
	return ret;
}

bool BResource::LoadPackage()
{
	for(int i=0; i<PACKAGEMAX; i++)
	{
		if(strlen(resdata.packagefilename[i]) && !hge->Resource_AttachPack(resdata.packagefilename[i], data.password))
		{
#ifdef __DEBUG
			HGELOG("%s\nFailed in Loading Package File %s.", HGELOG_ERRSTR, resdata.packagefilename[i]);
#endif
			return false;
		}
	}
	return true;
}

void BResource::CopyData()
{
	data.binname = resdata.binname;
	data.binfilename = resdata.binfilename;
	data.rabinname = resdata.rabinname;
	data.spellaccessfilename = resdata.spellaccessfilename;
	data.scrbinname = resdata.scrbinname;
	data.scriptfilename = resdata.scriptfilename;
	if (!data.binmode)
	{
		data.customconstfilename = resdata.customconstfilename;
		data.spelldefinefilename = resdata.spelldefinefilename;
		data.musicdefinefilename = resdata.musicdefinefilename;
		data.bulletdefinefilename = resdata.bulletdefinefilename;
		data.enemydefinefilename = resdata.enemydefinefilename;
		data.playerdefinefilename = resdata.playerdefinefilename;
		data.spritedefinefilename = resdata.spritedefinefilename;
		data.playerbulletdefinefilename = resdata.playerbulletdefinefilename;
		data.playershootdefinefilename = resdata.playershootdefinefilename;
		data.playerghostdefinefilename = resdata.playerghostdefinefilename;
	}
}

bool BResource::SetDataFile()
{
	if(!data.SetFile(data.binfilename, DATA_BINFILE))
		return false;
	if(!data.SetFile(data.spellaccessfilename, DATA_SPELLACCESSFILE))
		return false;
	if(!data.SetFile(data.scriptfilename, DATA_SCRIPTFILE))
		return false;
	if (!data.binmode)
	{
		if(!data.SetFile(data.customconstfilename, DATA_CUSTOMCONSTFILE))
			return false;
		if(!data.SetFile(data.spelldefinefilename, DATA_SPELLDEFINEFILE))
			return false;
		if(!data.SetFile(data.musicdefinefilename, DATA_MUSICDEFINEFILE))
			return false;
		if (!data.SetFile(data.bulletdefinefilename, DATA_BULLETDEFINEFILE))
			return false;
		if (!data.SetFile(data.enemydefinefilename, DATA_ENEMYDEFINEFILE))
			return false;
		if (!data.SetFile(data.playerdefinefilename, DATA_PLAYERDEFINEFILE))
			return false;
		if (!data.SetFile(data.spritedefinefilename, DATA_SPRITEDEFINEFILE))
			return false;
		if (!data.SetFile(data.playerbulletdefinefilename, DATA_PLAYERBULLETDEFINE))
			return false;
		if (!data.SetFile(data.playershootdefinefilename, DATA_PLAYERSHOOTDEFINE))
			return false;
		if (!data.SetFile(data.playerghostdefinefilename, DATA_PLAYERGHOSTDEFINE))
			return false;
	}
	return true;
}
/*
int BResource::SplitString(const char * str)
{
	if (str == NULL)
	{
		return 0;
	}
	for (int k=0; k<M_SCRIPTFOLDERMAX; k++)
	{
		strcpy(scriptfolder[k], "");
	}
	int length = strlen(str);
	int i = 0;
	int j = 0;
	int n = 0;
	while (i <= length)
	{
		if (str[i] == '|' || str[i] == 0)
		{
			scriptfolder[n][j] = 0;
			n++;
			j = 0;
		}
		else
		{
			scriptfolder[n][j] = str[i];
			j++;
		}
		i++;
	}
	return n;
}
*/