#include "../Header/Data.h"
#include "../Header/BResource.h"
#include "../Header/DataTable.h"
#include "../Header/SpriteItemManager.h"

_DataTable _DataTable::datatable;

#define _EOF_DATATABLE	(feof(file))
#define _CHECKEOF_DATATABLE	if (_EOF_DATATABLE)	\
	return false
#define _READSTRINGBUFFERLINE(X)	if (!ReadStringBuffer(X))	\
	return false
#define _BREAKCOMMENTBUFFER	if (!CommentBuffer())	\
	break
#define _INITTINT	ti = -1
#define _LOADTINT	(tint[++ti])
#define _SAVETINT	(&_LOADTINT)

#ifndef __INTEL_COMPILER
#define _DOSWAPTINT	int tbegin = 0;	\
	int tend = ti;	\
	while (tbegin < tend)	\
{	\
	int tswapint = tint[tbegin];	\
	tint[tbegin] = tint[tend];	\
	tint[tend] = tswapint;	\
	++tbegin;	\
	--tend;	\
}
#else
#define _DOSWAPTINT
#endif

_DataTable::_DataTable()
{
}

_DataTable::~_DataTable()
{
}

bool _DataTable::ReadStringBuffer(int nCol)
{
	if (!file)
	{
		return false;
	}
	for (int i=0; i<nCol; i++)
	{
		fscanf(file, "%s", buffer);
		_CHECKEOF_DATATABLE;
	}
	return true;
}

bool _DataTable::CommentBuffer()
{
	if (!file)
	{
		return false;
	}
	fscanf(file, "%s", buffer);
	_CHECKEOF_DATATABLE;
	if (buffer[0] == '#')
	{
		fscanf(file, "%[^\r\n]", bufferlong);
		_CHECKEOF_DATATABLE;
		CommentBuffer();
	}
	return true;
}

void _DataTable::SetFile(FILE * _file)
{
	file = _file;
}

bool _DataTable::DataTableDefine()
{
	_READSTRINGBUFFERLINE(2);

	strcpy(BResource::res.resdata.customconstfilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.customconstfilename, buffer);
	strcpy(BResource::res.resdata.spelldefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.spelldefinefilename, buffer);
	strcpy(BResource::res.resdata.musicdefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.musicdefinefilename, buffer);
	strcpy(BResource::res.resdata.bulletdefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.bulletdefinefilename, buffer);
	strcpy(BResource::res.resdata.enemydefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.enemydefinefilename, buffer);
	strcpy(BResource::res.resdata.playerdefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.playerdefinefilename, buffer);
	strcpy(BResource::res.resdata.spritedefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.spritedefinefilename, buffer);
	strcpy(BResource::res.resdata.playershootdefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.playershootdefinefilename, buffer);
	strcpy(BResource::res.resdata.playerghostdefinefilename, BResource::res.resdata.datafoldername);
	_READSTRINGBUFFERLINE(2);
	strcat(BResource::res.resdata.playerghostdefinefilename, buffer);
	return true;
}

bool _DataTable::PackageTableDefine()
{
	ZeroMemory(BResource::res.resdata.packagefilename, sizeof(char) * PACKAGEMAX * M_PATHMAX);
	_READSTRINGBUFFERLINE(3);
	while (!feof(file))
	{
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		fscanf(file, "%s", BResource::res.resdata.packagefilename[tindex]);
	}
	return true;
}

bool _DataTable::TextureTableDefine()
{
	ZeroMemory(BResource::res.texturedata, RSIZE_TEXTURE);
	_READSTRINGBUFFERLINE(6);
	while (!feof(file))
	{
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		textureData * item = &(BResource::res.texturedata[tindex]);
		fscanf(file, "%s%d%d%d",
			item->texfilename,
			&(item->texset),
			&(item->width),
			&(item->height)
		);
	}
	return true;
}

bool _DataTable::EffectTableDefine()
{
	ZeroMemory(BResource::res.resdata.effectsysfilename, sizeof(char) * EFFECTSYSTYPEMAX * M_PATHMAX);
	_READSTRINGBUFFERLINE(3);
	while (!feof(file))
	{
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		fscanf(file, "%s", BResource::res.resdata.effectsysfilename[tindex]);
	}
	return true;
}

bool _DataTable::SETableDefine()
{
	ZeroMemory(BResource::res.resdata.sefilename, sizeof(char) * SEMAX * M_PATHMAX);
	_READSTRINGBUFFERLINE(3);
	while (!feof(file))
	{
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		fscanf(file, "%s", BResource::res.resdata.sefilename[tindex]);
	}
	return true;
}

bool _DataTable::CustomConstFile()
{
	BResource::res.MallocCustomConst();
	ZeroMemory(BResource::res.customconstdata, RSIZE_CUSTOMCONST);
	_READSTRINGBUFFERLINE(4);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;

		customconstData * item = &(BResource::res.customconstdata[tindex]);
		fscanf(file, "%s%d", 
			item->name, 
			&(item->value));

		_DOSWAPTINT;
		_INITTINT;
	}
	return true;
}

bool _DataTable::SpellDefineFile()
{
	BResource::res.spelldata.clear();
//	ZeroMemory(BResource::res.spelldata, RSIZE_SPELL);
	_READSTRINGBUFFERLINE(11);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
//		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
//		spellData * item = &(BResource::res.spelldata[tindex]);
		spellData spItem;
		BResource::res.spelldata.push_back(spItem);
		spellData * item = &(BResource::res.spelldata.back());

		fscanf(file, "%d%d\t%[^\t]%d%d%d%I64d%d%x%d", 
			&(item->sno), 
			&(item->spellnumber), 
			item->spellname, 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			&(item->bonus), 
			&(item->turntoscene), 
			_SAVETINT, 
			_SAVETINT);

		_DOSWAPTINT;
		_INITTINT;
		item->timelimit = _LOADTINT;
		item->remain = _LOADTINT;
		item->userID = _LOADTINT;
		item->spellflag = _LOADTINT;
		item->battleID = _LOADTINT;
		//		BResource::res.spelldata.push_back(item);
	}
	return true;
}

bool _DataTable::MusicDefineFile()
{
	ZeroMemory(BResource::res.musdata, RSIZE_MUSIC);
	_READSTRINGBUFFERLINE(11);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		musicData * item = &(BResource::res.musdata[tindex]);
		_CHECKEOF_DATATABLE;

		fscanf(file, "\t%[^\t]%s%d%d%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\r\n]", 
			item->musicname, 
			item->musicfilename, 
			&(item->startpos), 
			&(item->introlength), 
			&(item->alllength),
			item->explain_1, 
			item->explain_2, 
			item->explain_3, 
			item->explain_4);

		_DOSWAPTINT;
		_INITTINT;
	}
	return true;
}

bool _DataTable::BulletDefineFile()
{
	ZeroMemory(BResource::res.bulletdata, RSIZE_BULLET);
	_READSTRINGBUFFERLINE(15);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		bulletData * item = &(BResource::res.bulletdata[tindex]);
		_CHECKEOF_DATATABLE;

		fscanf(file, "%s%d%d%d%f%f%d%d%d%d%d%d%x", 
			strbuffer[0], 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT,  
			&(item->collisionMain), 
			&(item->collisionSub), 
			_SAVETINT, 
			_SAVETINT, 
			&(item->nTurnAngle), 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT);

		_DOSWAPTINT;
		_INITTINT;
		item->siid = SpriteItemManager::GetIndexByName(strbuffer[0]);
		item->nRoll = _LOADTINT;
		item->nColor = _LOADTINT;
		item->collisiontype = _LOADTINT;
		item->fadecolor = _LOADTINT;
		item->bonuscolor = _LOADTINT;
		item->seID = _LOADTINT;
		item->blendtype = _LOADTINT;
		item->renderdepth = _LOADTINT;
		item->renderflag = _LOADTINT;
	}
	return true;
}

bool _DataTable::EnemyDefineFile()
{
	ZeroMemory(BResource::res.enemydata, RSIZE_ENEMY);
	_READSTRINGBUFFERLINE(18);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		enemyData * item = &(BResource::res.enemydata[tindex]);
		_CHECKEOF_DATATABLE;

		fscanf(file, "%s%d%f%f%d%d%d%d%d%d%d%d%d%d\t%[^\t]\t%[^\r\n]", 
			strbuffer[0], 
			_SAVETINT, 
			&(item->collision_w), 
			&(item->collision_h), 
			_SAVETINT, 
			&(item->standshake), 
			_SAVETINT, 
			_SAVETINT,
			_SAVETINT, 
			_SAVETINT,
			_SAVETINT, 
			_SAVETINT,
			_SAVETINT, 
			_SAVETINT, 
			item->name,
			item->ename);

		_DOSWAPTINT;
		_INITTINT;
		item->siid = SpriteItemManager::GetIndexByName(strbuffer[0]);
		item->faceIndex  = _LOADTINT;
		item->standFrame  = _LOADTINT;
		item->rightPreFrame  = _LOADTINT;
		item->rightFrame = _LOADTINT;
		item->leftPreFrame  = _LOADTINT;
		item->leftFrame = _LOADTINT;
		item->attackPreFrame  = _LOADTINT;
		item->attackFrame = _LOADTINT;
		item->storePreFrame  = _LOADTINT;
		item->storeFrame  = _LOADTINT;
	}
	return true;
}

bool _DataTable::PlayerDefineFile()
{
	ZeroMemory(BResource::res.playerdata, RSIZE_PLAYER);
	_READSTRINGBUFFERLINE(19);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		playerData * item = &(BResource::res.playerdata[tindex]);
		_CHECKEOF_DATATABLE;

		fscanf(file, "%f%f%f%f%d%d%d%d%s%d%d%d%d%d%d%d\t%[^\r\n]", 
			&(item->collision_r), 
			&(item->fastspeed), 
			&(item->slowspeed), 
			&(item->graze_r),
			&(item->bombperpower), 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			strbuffer[0], 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			_SAVETINT, 
			(item->name));

		_DOSWAPTINT;
		_INITTINT;
		item->shotdelay = _LOADTINT;
		item->borderlast = _LOADTINT;
		item->bomblast = _LOADTINT;
		item->siid = SpriteItemManager::GetIndexByName(strbuffer[0]);
		item->eid = _LOADTINT;
		item->faceIndex = _LOADTINT;
		item->standFrame = _LOADTINT;
		item->leftPreFrame = _LOADTINT;
		item->leftFrame = _LOADTINT;
		item->rightPreFrame = _LOADTINT;
		item->rightFrame = _LOADTINT;
	}
	return true;
}

bool _DataTable::SpriteDefineFile()
{
//	fscanf(file, "%d", &(BResource::res.spritenumber));
//	BResource::res.InitSpriteData();
	ZeroMemory(BResource::res.spritedata, RSIZE_SPRITE);
	_READSTRINGBUFFERLINE(8);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		spriteData * item = &(BResource::res.spritedata[tindex]);

		fscanf(file, "%s%d%d%d%d%d", 
			item->spritename, 
			&(item->tex), 
			&(item->tex_x), 
			&(item->tex_y), 
			&(item->tex_w), 
			&(item->tex_h));

		_DOSWAPTINT;
		_INITTINT;
	}
	return true;
}

bool _DataTable::PlayerShootDefineFile()
{
	ZeroMemory(BResource::res.playershootdata, RSIZE_PLAYERSHOOT);
	_READSTRINGBUFFERLINE(16);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		playershootData * item = &(BResource::res.playershootdata[tindex]);

		fscanf(file, "%d%s%d%x%f%d%d%d%f%f%f%f%f%d", 
			_SAVETINT, 
			strbuffer[0], 
			_SAVETINT, 
			_SAVETINT, 
			&(item->power), 
			&(item->hitonfactor),
			_SAVETINT, 
			&(item->angle), 
			&(item->speed), 
			&(item->scale),
			&(item->xbias), 
			&(item->ybias), 
			&(item->accelspeed), 
			_SAVETINT);

		_DOSWAPTINT;
		_INITTINT;
		item->userID = _LOADTINT;
		item->siid = SpriteItemManager::GetIndexByName(strbuffer[0]);
		item->timeMod = _LOADTINT;
		item->flag = _LOADTINT;
		item->arrange = _LOADTINT;
		item->seID = _LOADTINT;
	}
	return true;
}

bool _DataTable::PlayerGhostDefineFile()
{
	ZeroMemory(BResource::res.playerghostdata, RSIZE_PLAYERGHOST);
	_READSTRINGBUFFERLINE(11);
	while (!feof(file))
	{
		_INITTINT;
		_BREAKCOMMENTBUFFER;
		fscanf(file, "%d", &tindex);
		_CHECKEOF_DATATABLE;
		playerghostData * item = &(BResource::res.playerghostdata[tindex]);

		fscanf(file, "%s%f%f%f%x%d%f%d%d", 
			strbuffer[0], 
			&(item->xadj), 
			&(item->yadj), 
			&(item->speed), 
			_SAVETINT, 
			&(item->shootangle), 
			&(item->mover), 
			&(item->startangle),
			_SAVETINT);

		_DOSWAPTINT;
		_INITTINT;
		item->siID = SpriteItemManager::GetIndexByName(strbuffer[0]);
		item->flag = _LOADTINT;
		item->blend = _LOADTINT;
	}
	return true;
}

FILE * Data::checkTableFile(BYTE type)
{
	getFile(type);
	if (!nowfilename)
	{
		return NULL;
	}
	FILE * file = fopen(nowfilename, "rb");
	if (!file)
	{
		return NULL;
	}
	int tgameversion = -1;
	char tsignature[M_STRMAX];
	int tfiletype = -1;

	fscanf(file, "%x%s%x", &tgameversion, tsignature, &tfiletype);
	if (tgameversion != GAME_VERSION || strcmp(tsignature, GAME_SIGNATURE) || tfiletype != type)
	{
		fclose(file);
		return NULL;
	}
	return file;
}

bool Data::GetTableFile(BYTE type)
{
	if (!(type & DATA_TABLEHEADER))
	{
		return false;
	}
	FILE * file = checkTableFile(type);
	if (file == NULL)
	{
		return false;
	}

	_DataTable::datatable.SetFile(file);

	char buffer[M_STRMAX];
	int tindex;
	int tint[32];
	switch (type)
	{
	case DATA_DATATABLEDEFINE:
		_DataTable::datatable.DataTableDefine();
		break;
	case DATA_PACKAGETABLEDEFINE:
		_DataTable::datatable.PackageTableDefine();
		break;
	case DATA_TEXTURETABLEDEFINE:
		_DataTable::datatable.TextureTableDefine();
		break;
	case DATA_EFFECTTABLEDEFINE:
		_DataTable::datatable.EffectTableDefine();
		break;
	case DATA_SETABLEDEFINE:
		_DataTable::datatable.SETableDefine();
		break;

	case DATA_CUSTOMCONSTFILE:
		_DataTable::datatable.CustomConstFile();
		break;
	case DATA_SPELLDEFINEFILE:
		_DataTable::datatable.SpellDefineFile();
		break;
	case DATA_MUSICDEFINEFILE:
		_DataTable::datatable.MusicDefineFile();
		break;
	case DATA_BULLETDEFINEFILE:
		_DataTable::datatable.BulletDefineFile();
		break;
	case DATA_ENEMYDEFINEFILE:
		_DataTable::datatable.EnemyDefineFile();
		break;
	case DATA_PLAYERDEFINEFILE:
		_DataTable::datatable.PlayerDefineFile();
		break;
	case DATA_SPRITEDEFINEFILE:
		_DataTable::datatable.SpriteDefineFile();
		break;

	case DATA_PLAYERSHOOTDEFINE:
		_DataTable::datatable.PlayerShootDefineFile();
		break;
	case DATA_PLAYERGHOSTDEFINE:
		_DataTable::datatable.PlayerGhostDefineFile();
		break;
	}
	fclose(file);
	return true;
}
