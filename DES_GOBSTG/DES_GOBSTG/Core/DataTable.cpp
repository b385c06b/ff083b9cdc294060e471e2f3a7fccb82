#include "Data.h"
#include "BResource.h"

bool Data::GetTableFile(BYTE type)
{
	if (!(type & DATA_TABLEHEADER))
	{
		return false;
	}
	getFile(type);
	if (!filename)
	{
		return false;
	}
	FILE * file = fopen(filename, "rb");
	if (!file)
	{
		return false;
	}
	int tgameversion = -1;
	char tsignature[M_STRMAX];
	int tfiletype = -1;

	fscanf(file, "%x%s%x", &tgameversion, tsignature, &tfiletype);
	if (tgameversion != GAME_VERSION || strcmp(tsignature, GAME_SIGNATURE) || tfiletype != type)
	{
		fclose(file);
		return false;
	}

	char buffer[M_STRMAX];
	int tindex;
	int tint[32];
	switch (type)
	{
	case DATA_CUSTOMCONSTFILE:
		res.ReleaseCustomConst();
		res.customconstdata = (customconstData *)malloc(RSIZE_CUSTOMCONST);
		ZeroMemory(res.customconstdata, RSIZE_CUSTOMCONST);
		fscanf(file, "%s%s%s", 
			buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			if (feof(file))
			{
				break;
			}
			customconstData * item = &(res.customconstdata[tindex]);
			fscanf(file, "%s%d", 
				item->name, 
				&(item->value));
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_SPELLDEFINEFILE:
		res.spelldata.clear();
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			if (feof(file))
			{
				break;
			}
			spellData _item;
			fscanf(file, "%d\t%[^\t]%d%d%d%I64d%d%x%d", 
				&(_item.spellnumber), 
				_item.spellname, 
				&(tint[0]), 
				&(tint[1]), 
				&(tint[2]), 
				&(_item.bonus), 
				&(_item.turntoscene), 
				&(tint[3]), 
				&(tint[4]));
			_item.sno = tindex;
			_item.timelimit = tint[0];
			_item.remain = tint[1];
			_item.userID = tint[2];
			_item.spellflag = tint[3];
			_item.battleID = tint[4];
			res.spelldata.push_back(_item);
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_MUSICDEFINEFILE:
		ZeroMemory(res.musdata, RSIZE_MUSIC);
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			musicData * item = &(res.musdata[tindex]);
			if (feof(file))
			{
				break;
			}
			fscanf(file, "%[^\t]%s%d%d%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\r\n]", 
				item->musicname, 
				item->musicfilename, 
				&(item->startpos), 
				&(item->introlength), 
				&(item->alllength), 
				item->explain_1, 
				item->explain_2, 
				item->explain_3, 
				item->explain_4);
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_BULLETDEFINEFILE:
		ZeroMemory(res.bulletdata, RSIZE_BULLET);
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			bulletData * item = &(res.bulletdata[tindex]);
			if (feof(file))
			{
				break;
			}
			fscanf(file, "%d%d%d%d%d%d%d%f%f%d%d%d%d%d%d", 
				&(item->tex_x), 
				&(item->tex_y), 
				&(item->tex_w), 
				&(item->tex_h), 
				&(tint[0]), 
				&(tint[1]), 
				&(tint[2]), 
				&(item->collisionMain), 
				&(item->collisionSub), 
				&(tint[3]),
				&(tint[4]), 
				&(item->nTurnAngle), 
				&(tint[5]), 
				&(tint[6]), 
				&(tint[7]));
			item->nRoll = tint[0];
			item->nColor = tint[1];
			item->collisiontype = tint[2];
			item->fadecolor = tint[3];
			item->bonuscolor = tint[4];
			item->seID = tint[5];
			item->blendtype = tint[6];
			item->renderdepth = tint[7];
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_ENEMYDEFINEFILE:
		ZeroMemory(res.enemydata, RSIZE_ENEMY);
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			enemyData * item = &(res.enemydata[tindex]);
			if (feof(file))
			{
				break;
			}
			fscanf(file, "%d%d%d%d%f%f%d%d%d%d%d%d%d%d%d%d%d%f%f\t%[^\t]\t%[^\r\n]", 
				&(tint[0]), 
				&(tint[1]), 
				&(tint[2]),
				&(tint[3]), 
				&(item->collision_w), 
				&(item->collision_h), 
				&(tint[4]), 
				&(tint[5]), 
				&(item->standshake), 
				&(tint[6]), 
				&(tint[7]),
				&(tint[8]), 
				&(tint[9]),
				&(tint[10]), 
				&(tint[11]),
				&(tint[12]), 
				&(tint[13]), 
				&(item->usetexw),
				&(item->usetexh),
				item->name,
				item->ename);
			item->tex = tint[0];
			item->faceIndex  = tint[1];
			item->tex_nCol = tint[2];
			item->tex_nRow  = tint[3];
			item->startFrame  = tint[4];
			item->standFrame  = tint[5];
			item->rightPreFrame  = tint[6];
			item->rightFrame = tint[7];
			item->leftPreFrame  = tint[8];
			item->leftFrame = tint[9];
			item->attackPreFrame  = tint[10];
			item->attackFrame = tint[11];
			item->storePreFrame  = tint[12];
			item->storeFrame  = tint[13];
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_PLAYERDEFINEFILE:
		ZeroMemory(res.playerdata, RSIZE_PLAYER);
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			playerData * item = &(res.playerdata[tindex]);
			if (feof(file))
			{
				break;
			}
			fscanf(file, "%f%f%f%f%d%d%d%d%d%d%d%d%d%d%d%d%d%d%f%f\t%[^\r\n]", 
				&(item->collision_r), 
				&(item->fastspeed), 
				&(item->slowspeed), 
				&(item->graze_r),
				&(item->bombperpower), 
				&(tint[0]), 
				&(tint[1]), 
				&(tint[2]), 
				&(tint[3]), 
				&(tint[4]), 
				&(tint[5]), 
				&(tint[6]), 
				&(tint[7]), 
				&(tint[8]), 
				&(tint[9]), 
				&(tint[10]), 
				&(tint[11]), 
				&(tint[12]), 
				&(item->usetexw),
				&(item->usetexh),
				(item->name));
			item->shotdelay = tint[0];
			item->borderlast = tint[1];
			item->bomblast = tint[2];
			item->tex = tint[3];
			item->faceIndex = tint[4];
			item->tex_nCol = tint[5];
			item->tex_nRow = tint[6];
			item->startFrame = tint[7];
			item->standFrame = tint[8];
			item->leftPreFrame = tint[9];
			item->leftFrame = tint[10];
			item->rightPreFrame = tint[11];
			item->rightFrame = tint[12];
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_SPRITEDEFINEFILE:
		ZeroMemory(res.spritedata, RSIZE_SPRITE);
		fscanf(file, "%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			if (feof(file))
			{
				break;
			}
			spriteData * item = &(res.spritedata[tindex]);
			fscanf(file, "%s%d%d%d%d%d", 
				item->spritename, 
				&(tint[0]), 
				&(item->tex_x), 
				&(item->tex_y), 
				&(item->tex_w), 
				&(item->tex_h));
			item->tex = tint[0];
			if (feof(file))
			{
				break;
			}
		}
		break;

	case DATA_PLAYERBULLETDEFINE:
		ZeroMemory(res.playerbulletdata, RSIZE_PLAYERBULLET);
		fscanf(file, "%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			if (feof(file))
			{
				break;
			}
			playerbulletData * item = &(res.playerbulletdata[tindex]);
			fscanf(file, "%d%f%f%f%f", 
				&(tint[0]), 
				&(item->x), 
				&(item->y), 
				&(item->w), 
				&(item->h));
			item->tex = tint[0];
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_PLAYERSHOOTDEFINE:
		ZeroMemory(res.playershootdata, RSIZE_PLAYERSHOOT);
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			if (feof(file))
			{
				break;
			}
			playershootData * item = &(res.playershootdata[tindex]);
			fscanf(file, "%d%d%d%x%f%d%d%d%f%f%f%f%f%d", 
				&(tint[0]), 
				&(tint[1]), 
				&(tint[2]), 
				&(tint[3]), 
				&(item->power), 
				&(item->hitonfactor),
				&(tint[4]), 
				&(item->angle), 
				&(item->speed), 
				&(item->scale),
				&(item->xbias), 
				&(item->ybias), 
				&(item->accelspeed), 
				&(tint[5]));
			item->userID = tint[0];
			item->ID = tint[1];
			item->timeMod = tint[2];
			item->flag = tint[3];
			item->arrange = tint[4];
			item->seID = tint[5];
			if (feof(file))
			{
				break;
			}
		}
		break;
	case DATA_PLAYERGHOSTDEFINE:
		ZeroMemory(res.playerghostdata, RSIZE_PLAYERGHOST);
		fscanf(file, "%s%s%s%s%s%s%s%s%s%s", 
			buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);
		while (!feof(file))
		{
			fscanf(file, "%d", &tindex);
			if (feof(file))
			{
				break;
			}
			playerghostData * item = &(res.playerghostdata[tindex]);
			fscanf(file, "%d%f%f%f%x%d%f%d%d", 
				&(item->siID), 
				&(item->xadj), 
				&(item->yadj), 
				&(item->speed), 
				&(tint[0]), 
				&(item->shootangle), 
				&(item->mover), 
				&(item->startangle),
				&(tint[1]));
			item->flag = tint[0];
			item->blend = tint[1];
			if (feof(file))
			{
				break;
			}
		}
		break;
	}
	fclose(file);
	return true;
}
