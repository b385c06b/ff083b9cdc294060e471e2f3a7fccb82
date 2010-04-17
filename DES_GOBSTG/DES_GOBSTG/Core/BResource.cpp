#include "../Header/BResource.h"
#include "../Header/data.h"
#include "../Header/ConstResource.h"

BResource BResource::res;

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

void BResource::MallocCustomConst()
{
	ReleaseCustomConst();
	customconstdata = (customconstData *)malloc(RSIZE_CUSTOMCONST);
}


//Scripter::LoadAll
bool BResource::Fill()
{
	DWORD sec;
	DWORD name;

	//resource
	strcpy(resdata.widefontname, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_FONT), Data::data.nLinkType(RESDATAN_FONT), RESDEFAULT_FONTFONT));

	sec = Data::data.sLinkType(RESDATAS_FOLDER);
	for (int i=0; i<M_SCRIPTFOLDERMAX; i++)
	{
		strcpy(resdata.scriptfoldername[i], Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_SCRIPTFOLDER_1+i), RESDEFAULT_SCRIPTFOLDER));
		if (!strlen(resdata.scriptfoldername[i]))
		{
			continue;
		}
		if(_access(resdata.scriptfoldername[i], 00) == -1)
		{
			CreateDirectory(resdata.scriptfoldername[i], NULL);
		}
	}
	strcpy(resdata.snapshotfoldername, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_SNAPSHOTFOLDER), RESDEFAULT_SNAPSHOTFOLDER));
	if(_access(resdata.snapshotfoldername, 00) == -1)
	{
		CreateDirectory(resdata.snapshotfoldername, NULL);
	}
	strcpy(resdata.replayfoldername, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_REPLAYFOLDER), RESDEFAULT_REPLAYFOLDER));
	if(_access(resdata.replayfoldername, 00) == -1)
	{
		CreateDirectory(resdata.replayfoldername, NULL);
	}
	strcpy(resdata.datafoldername, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_DATAFOLDER), RESDEFAULT_DATAFOLDER));
	if(_access(resdata.datafoldername, 00) == -1)
	{
		CreateDirectory(resdata.datafoldername, NULL);
	}
	strcpy(resdata.effectsysfoldername, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_EFFECTSYSFOLDER), RESDEFAULT_DATAFOLDER));
	if(_access(resdata.effectsysfoldername, 00) == -1)
	{
		CreateDirectory(resdata.effectsysfoldername, NULL);
	}

	char buffer[M_STRMAX];

	sec = Data::data.sLinkType(RESDATAS_DATAFILE);

	strcpy(resdata.binname, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_BINFILE), RESDEFAULT_DATABIN));
	strcpy(resdata.binfilename, resdata.datafoldername);
	strcat(resdata.binfilename, resdata.binname);
	//copy to data
	//

	strcpy(resdata.rabinname, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_SPELLACCESSFILE), RESDEFAULT_DATASPELLACCESS));
	strcpy(resdata.spellaccessfilename, resdata.datafoldername);
	strcat(resdata.spellaccessfilename, resdata.rabinname);
	//copy to data
	//

	strcpy(resdata.scrbinname, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_SCRIPTFILE), RESDEFAULT_DATASCRIPT));
	strcpy(resdata.scriptfilename, resdata.datafoldername);
	strcat(resdata.scriptfilename, resdata.scrbinname);
	//copy to data
	//

	strcpy(resdata.customconstfilename, resdata.datafoldername);
	strcat(resdata.customconstfilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_CUSTOMCONSTFILE), RESDEFAULT_DATACUSTOMCONST));
	//copy to data
	//

	strcpy(resdata.spelldefinefilename, resdata.datafoldername);
	strcat(resdata.spelldefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_SPELLDEFINEFILE), RESDEFAULT_DATASPELLDEFINE));
	//copy to data
	//

	strcpy(resdata.musicdefinefilename, resdata.datafoldername);
	strcat(resdata.musicdefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_MUSICDEFINEFILE), RESDEFAULT_DATAMUSICDEFINE));
	//copy to data
	//

	strcpy(resdata.bulletdefinefilename, resdata.datafoldername);
	strcat(resdata.bulletdefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_BULLETDEFINEFILE), RESDEFAULT_DATABULLETDEFINE));
	//copy to data
	//

	strcpy(resdata.enemydefinefilename, resdata.datafoldername);
	strcat(resdata.enemydefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_ENEMYDEFINEFILE), RESDEFAULT_ENEMYDEFINE));
	//copy to data
	//

	strcpy(resdata.playerdefinefilename, resdata.datafoldername);
	strcat(resdata.playerdefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_PLAYERDEFINEFILE), RESDEFAULT_DATAPLAYERDEFINE));
	//copy to data
	//

	strcpy(resdata.spritedefinefilename, resdata.datafoldername);
	strcat(resdata.spritedefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_SPRITEDEFINEFILE), RESDEFAULT_DATASPRITEDEFINE));
	//copy to data
	//

	strcpy(resdata.playerbulletdefinefilename, resdata.datafoldername);
	strcat(resdata.playerbulletdefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_PLAYERBULLETDEFINEFILE), RESDEFAULT_DATAPLAYERBULLETDEFINE));
	//copy to data
	//

	strcpy(resdata.playershootdefinefilename, resdata.datafoldername);
	strcat(resdata.playershootdefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_PLAYERSHOOTDEFINEFILE), RESDEFAULT_DATAPLAYERSHOOTDEFINE));
	//copy to data
	//

	strcpy(resdata.playerghostdefinefilename, resdata.datafoldername);
	strcat(resdata.playerghostdefinefilename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_PLAYERGHOSTDEFINEFILE), RESDEFAULT_DATAPLAYERGHOSTDEFINE));
	//copy to data
	//

	name = Data::data.nLinkType(RESDATAN_TYPE);
	for(int i=0;i<PACKAGEMAX;i++)
	{
		name = Data::data.nLinkNum(name, i+1);
		strcpy(resdata.packagefilename[i], Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_PACKAGE), name, ""));
	}

	for(int i=0;i<TEXMAX;i++)
	{
		name = Data::data.nLinkNum(name, i+1);
		strcpy(resdata.texfilename[i], Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_TEXTURE), name, ""));
	}

	for(int i=0;i<SEMAX;i++)
	{
		name = Data::data.nLinkNum(name, i+1);
		strcpy(resdata.sefilename[i], Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_SE), name, ""));
	}

	for(int i=0; i<EFFECTSYSTYPEMAX; i++)
	{
		name = Data::data.nLinkNum(name, i+1);
		strcpy(resdata.effectsysfilename[i], Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_EFFECTSYS), name, ""));
	}

	strcpy(buffer, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_EXTENSION), Data::data.nLinkType(RESDATAN_SCRIPTEXT7), RESDEFAULT_SCRIPTEXT7));
	if(strlen(buffer) > 8)
	{
#ifdef __DEBUG
		HGELOG("%s\nExtension %s for Script File is too long.(Required to be less than 8).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.scriptextensionname7, buffer);

	strcpy(buffer, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_EXTENSION), Data::data.nLinkType(RESDATAN_REPLAYEXT7), RESDEFAULT_REPLAYEXT7));
	if(strlen(buffer) > 8)
	{
#ifdef __DEBUG
		HGELOG("%s\nExtension %s for Replay File is too long.(Required to be less than 8).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replayextensionname7, buffer);

	strcpy(buffer, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_REPLAYHEADER), Data::data.nLinkType(RESDATAN_RPYSIGNATURE11), RESDEFAULT_RPYSIGNATURE11));
	if(strlen(buffer) > 12)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Signature %s is too long.(Required to be less than 12.)", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaysignature11, buffer);

	strcpy(buffer, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_REPLAYHEADER), Data::data.nLinkType(RESDATAN_RPYTEMPSIGN3), RESDEFAULT_RPYTEMPSIGN3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Temp Sign %s is too long.(Required to be less than 4).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaytempsign3, buffer);

	strcpy(buffer, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_REPLAYHEADER), Data::data.nLinkType(RESDATAN_RPYCOMPLETESIGN3), RESDEFAULT_RPYCOMPLETESIGN3));
	if(strlen(buffer) > 4)
	{
#ifdef __DEBUG
		HGELOG("%s\nReplay File Header Complete Sign %s is too long.(Required to be less than 4).", HGELOG_ERRSTR, buffer);
#endif
		return false;
	}
	strcpy(resdata.replaycompletesign3, buffer);

	strcpy(buffer, Data::data.sRead(DATA_RESOURCEFILE, Data::data.sLinkType(RESDATAS_REPLAYHEADER), Data::data.nLinkType(RESDATAN_RPYTAG3), RESDEFAULT_RPYTAG3));
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
	return Data::data.GetAllTable();
}

void BResource::GetUIString()
{
	DWORD sec = Data::data.sLinkType(RESDATAS_UISTRING);	
	strcpy(resdata.uistr.filename, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_FILENAME), RESDEFAULT_UISTR_FILENAME));
	strcpy(resdata.uistr.username, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_USERNAME), RESDEFAULT_UISTR_USERNAME));
	strcpy(resdata.uistr.score, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_SCORE), RESDEFAULT_UISTR_SCORE));
	strcpy(resdata.uistr.point, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_POINT), RESDEFAULT_UISTR_POINT));
	strcpy(resdata.uistr.faith, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_FAITH), RESDEFAULT_UISTR_FAITH));
	strcpy(resdata.uistr.alltime, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_ALLTIME), RESDEFAULT_UISTR_ALLTIME));
	strcpy(resdata.uistr.date, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_DATE), RESDEFAULT_UISTR_DATE));
	strcpy(resdata.uistr.lost, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_LOST), RESDEFAULT_UISTR_LOST));
	strcpy(resdata.uistr.borderrate, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_BORDERRATE), RESDEFAULT_UISTR_BORDERRATE));
	strcpy(resdata.uistr.fastrate, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_FASTRATE), RESDEFAULT_UISTR_FASTRATE));
	strcpy(resdata.uistr.modeflag, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_MODEFLAG), RESDEFAULT_UISTR_MODEFLAG));
	strcpy(resdata.uistr.mode_none, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_MODE_NONE), RESDEFAULT_UISTR_MODE_NONE));
	strcpy(resdata.uistr.mode_practice, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_MODE_PRACTICE), RESDEFAULT_UISTR_MODE_PRACTICE));
	strcpy(resdata.uistr.mode_spell, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_MODE_SPELL), RESDEFAULT_UISTR_MODE_SPELL));
	strcpy(resdata.uistr.usingchara, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_USINGCHARA), RESDEFAULT_UISTR_USINGCHARA));
//	strcpy(resdata.uistr.maxplayer, data.sRead(DATA_RESOURCEFILE, sec, data.nLinkType(RESDATAN_UISTR_MAXPLAYER), RESDEFAULT_UISTR_MAXPLAYER));
	strcpy(resdata.uistr.misstime, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_MISSTIME), RESDEFAULT_UISTR_MISSTIME));
	strcpy(resdata.uistr.bordertime, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_BORDERTIME), RESDEFAULT_UISTR_BORDERTIME));
	strcpy(resdata.uistr.continuetime, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_CONTINUETIME), RESDEFAULT_UISTR_CONTINUETIME));
	strcpy(resdata.uistr.gettime, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_GETTIME), RESDEFAULT_UISTR_GETTIME));
	strcpy(resdata.uistr.pausetime, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_PAUSETIME), RESDEFAULT_UISTR_PAUSETIME));
	strcpy(resdata.uistr.difflv, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_DIFFLV), RESDEFAULT_UISTR_DIFFLV));
	strcpy(resdata.uistr.laststage, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_LASTSTAGE), RESDEFAULT_UISTR_LASTSTAGE));
	strcpy(resdata.uistr.graze, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_GRAZE), RESDEFAULT_UISTR_GRAZE));
	strcpy(resdata.uistr.confirm, Data::data.sRead(DATA_RESOURCEFILE, sec, Data::data.nLinkType(RESDATAN_UISTR_CONFIRM), RESDEFAULT_UISTR_CONFIRM));

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

	BYTE * _header = Data::data.CreateMemHeader(DATA_RESOURCEFILE);
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
	memfile.filename = Data::data.resbinname;
	memfile.size = size;

	bool ret = false;
	ret = hge->Resource_CreatePack(hge->Resource_MakePath(Data::data.resbinname), Data::data.password, &memfile, NULL);

	free(content);

	return ret;
}

bool BResource::Gain(void * pStrdesc, void * pCustomConstData)
{
	BYTE * content;
	DWORD size;
	bool ret = false;

	hge->Resource_AttachPack(Data::data.resourcefilename, Data::data.password);
	content = hge->Resource_Load(Data::data.resbinname, &size);
	if(content)
	{
		spelldata.clear();
		if(Data::data.CheckMemHeader(content, size, DATA_RESOURCEFILE))
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
		if(strlen(resdata.packagefilename[i]) && !hge->Resource_AttachPack(resdata.packagefilename[i], Data::data.password))
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
	Data::data.binname = resdata.binname;
	Data::data.binfilename = resdata.binfilename;
	Data::data.rabinname = resdata.rabinname;
	Data::data.spellaccessfilename = resdata.spellaccessfilename;
	Data::data.scrbinname = resdata.scrbinname;
	Data::data.scriptfilename = resdata.scriptfilename;
	if (!Data::data.binmode)
	{
		Data::data.customconstfilename = resdata.customconstfilename;
		Data::data.spelldefinefilename = resdata.spelldefinefilename;
		Data::data.musicdefinefilename = resdata.musicdefinefilename;
		Data::data.bulletdefinefilename = resdata.bulletdefinefilename;
		Data::data.enemydefinefilename = resdata.enemydefinefilename;
		Data::data.playerdefinefilename = resdata.playerdefinefilename;
		Data::data.spritedefinefilename = resdata.spritedefinefilename;
		Data::data.playerbulletdefinefilename = resdata.playerbulletdefinefilename;
		Data::data.playershootdefinefilename = resdata.playershootdefinefilename;
		Data::data.playerghostdefinefilename = resdata.playerghostdefinefilename;
	}
}

bool BResource::SetDataFile()
{
	if(!Data::data.SetFile(Data::data.binfilename, DATA_BINFILE))
		return false;
	if(!Data::data.SetFile(Data::data.spellaccessfilename, DATA_SPELLACCESSFILE))
		return false;
	if(!Data::data.SetFile(Data::data.scriptfilename, DATA_SCRIPTFILE))
		return false;
	if (!Data::data.binmode)
	{
		if(!Data::data.SetFile(Data::data.customconstfilename, DATA_CUSTOMCONSTFILE))
			return false;
		if(!Data::data.SetFile(Data::data.spelldefinefilename, DATA_SPELLDEFINEFILE))
			return false;
		if(!Data::data.SetFile(Data::data.musicdefinefilename, DATA_MUSICDEFINEFILE))
			return false;
		if (!Data::data.SetFile(Data::data.bulletdefinefilename, DATA_BULLETDEFINEFILE))
			return false;
		if (!Data::data.SetFile(Data::data.enemydefinefilename, DATA_ENEMYDEFINEFILE))
			return false;
		if (!Data::data.SetFile(Data::data.playerdefinefilename, DATA_PLAYERDEFINEFILE))
			return false;
		if (!Data::data.SetFile(Data::data.spritedefinefilename, DATA_SPRITEDEFINEFILE))
			return false;
		if (!Data::data.SetFile(Data::data.playerbulletdefinefilename, DATA_PLAYERBULLETDEFINE))
			return false;
		if (!Data::data.SetFile(Data::data.playershootdefinefilename, DATA_PLAYERSHOOTDEFINE))
			return false;
		if (!Data::data.SetFile(Data::data.playerghostdefinefilename, DATA_PLAYERGHOSTDEFINE))
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