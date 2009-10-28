#include "DataConnector.h"
#include "Process.h"
#include "Player.h"
#include "Data.h"
#include "Replay.h"

int DataConnector::Insert()
{
	if(mp.replaymode)
		return -1;

	DWORD sec;
	if(mp.spellmode)
	{
		sec = data.sLinkType(DATAS_SPELL);
		sec = data.sLinkNum(sec, rpy.rpyinfo.startscene);

		if(Player::p.nScore > data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPBONUS), 0))
		{
			data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPBONUS), Player::p.nScore);
		}
		return -1;
	}

	if(mp.practicemode)
	{
		sec = data.sLinkType(DATAS_STAGEPRACTICE);
		sec = data.sLinkDiff(sec, rpy.rpyinfo.difflv);
		sec = data.sLinkNum(sec, rpy.rpyinfo.startscene / M_STAGENSCENE);
		if(rpy.rpyinfo.score > data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPSCORE), 0))
		{
			data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TOPSCORE), rpy.rpyinfo.score);
		}
		return -1;
	}

	sec = data.sLinkType(DATAS_TOP);
	sec = data.sLinkDiff(sec, rpy.rpyinfo.difflv);

	for(int i=DATA_NTOP10FILESAVE;i>=0;i--)
	{
		DWORD secbuf = sec;
		secbuf = data.sLinkNum(secbuf, i);
		if(i == 0 || data.lRead(DATA_BINFILE, secbuf, data.nLinkType(DATAN_SCORE), 0) > rpy.rpyinfo.score)
		{
			if(i == DATA_NTOP10FILESAVE)
				return 0;

			for(int j=DATA_NTOP10FILESAVE-1;j>i;j--)
			{
				data.MoveDown(sec, j);
			}
			//
			sec = data.sLinkNum(sec, i+1);

			data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_SCORE), rpy.rpyinfo.score);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_LASTSTAGE), rpy.rpyinfo.laststage);
			data.sWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_USERNAME), rpy.rpyinfo.username);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_YEAR), rpy.rpyinfo.year);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MONTH), rpy.rpyinfo.month);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_DAY), rpy.rpyinfo.day);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_HOUR), rpy.rpyinfo.hour);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TIME_MINUTE), rpy.rpyinfo.minute);
			data.fWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_LOST), rpy.rpyinfo.lost);
			data.fWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_BORDERRATE), rpy.rpyinfo.borderrate);
			data.fWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_FASTRATE), rpy.rpyinfo.fastrate);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_ALLTIME), rpy.rpyinfo.alltime);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_SPELLGET), rpy.rpyinfo.get);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_POINT), rpy.rpyinfo.point);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_FAITH), rpy.rpyinfo.faith);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MISS), rpy.rpyinfo.miss);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_BOMB), rpy.rpyinfo.border);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_CONTINUE), rpy.rpyinfo.cont);
			data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_PAUSE), rpy.rpyinfo.pause);

			DWORD name;
			name = data.nLinkType(DATAN_CHARA);
			for (int j=0; j<M_PL_ONESETPLAYER; j++)
			{
				name = data.nLinkNum(name, j+1);
				data.iWrite(DATA_BINFILE, sec, name, rpy.rpyinfo.usingchara[j]);
			}
			name = data.nLinkType(DATAN_GETSPELL);
			for (int j=0; j<M_GETSPELLMAX; j++)
			{
				data.iWrite(DATA_BINFILE, sec, data.nLinkNum(name, j+1), rpy.rpyinfo.getspell[j]);
			}

			return i+1;
		}
	}
	return 0;
}


//////////////////////////////
//replaymode, p.ID, scene, spellmode
void DataConnector::Meet()
{
	if(mp.replaymode)
		return;

	DWORD sec;
	int t;
	sec = data.sLinkType(DATAS_SPELL);
	sec = data.sLinkNum(sec, mp.scene);
	if(mp.spellmode)
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETPRACTICE), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETPRACTICE), t+1);

		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << 8;
		data.raSetIndi(mp.scene, indi);
	}
	else
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETGAME), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_MEETGAME), t+1);

		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1;
		data.raSetIndi(mp.scene, indi);
	}
}

///////////////////////////
//replaymode, p.ID, scene, spellmode
void DataConnector::Get()
{
	if(mp.replaymode)
		return;

	DWORD sec;
	int t;
	sec = data.sLinkType(DATAS_SPELL);
	sec = data.sLinkNum(sec, mp.scene);
	if(mp.spellmode)
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_GETPRACTICE), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_GETPRACTICE), t+1);
	
		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << 12;
		data.raSetIndi(mp.scene, indi);
	}
	else
	{
		t = data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_GETGAME), 0);
		data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_GETGAME), t+1);
		
		WORD indi = data.raGetIndi(mp.scene);
		indi |= 1 << 4;
		data.raSetIndi(mp.scene, indi);
	}
}

//luchara, scene, spellmode
int DataConnector::nMeet()
{
	return data.nMeet(mp.scene, mp.spellmode);
}

//luchara, scene, spellmode
int DataConnector::nGet()
{
	return data.nGet(mp.scene, mp.spellmode);
}

//replaymode, practicemode, nowdifflv, luchara
void DataConnector::Clear()
{
	if(mp.replaymode || mp.practicemode)
		return;

	DWORD sec;
	sec = data.sLinkType(DATAS_TOTAL);
	sec = data.sLinkDiff(sec, mp.nowdifflv);

	data.iWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_CLEARTIME), data.iRead(DATA_BINFILE, sec, data.nLinkType(DATAN_CLEARTIME), 0)+1);
}

//playtimeStart
void DataConnector::addPlayTime()
{
	DWORD sec;

	SYSTEMTIME tsystime;
	FILETIME tfiletime;
	GetLocalTime(&tsystime);
	SystemTimeToFileTime(&tsystime, &tfiletime);
	LONGLONG playtimeEnd = (((LONGLONG)tfiletime.dwHighDateTime) << 32) | tfiletime.dwLowDateTime;

	sec = data.sLinkType(DATAS_TOTAL);
	if(playtimeEnd > mp.playtimeStart)
		data.lWrite(DATA_BINFILE, sec, data.nLinkType(DATAN_TOTALPLAYTIME), data.lRead(DATA_BINFILE, sec, data.nLinkType(DATAN_TOTALPLAYTIME), 0) + (playtimeEnd - mp.playtimeStart));

	mp.playtimeStart = 0;
}

//spellmode, luchara, scene, nowdifflv, practicemode
LONGLONG DataConnector::nHiScore()
{
	return data.nHighScore(mp.scene, mp.nowdifflv, mp.spellmode, mp.practicemode);
}

//nowdifflv, luchara, scene
int DataConnector::nTryStageTime()
{
	return data.nTryStageTime(data.getStage(mp.scene), mp.nowdifflv);
}

//spellmode, replaymode, practicemode, nowdifflv, luchara, scene
void DataConnector::Try(bool first)
{
	if(mp.spellmode || mp.replaymode)
		return;
	
	DWORD sec;
	DWORD name;
	if(mp.practicemode || !nTryStageTime())
	{
		sec = data.sLinkType(DATAS_STAGEPRACTICE);
		sec = data.sLinkDiff(sec, mp.nowdifflv);
		sec = data.sLinkNum(sec, mp.scene/M_STAGENSCENE);

		name = data.nLinkType(DATAN_TRYTIME);
		if(mp.practicemode)
			data.iWrite(DATA_BINFILE, sec, name, data.iRead(DATA_BINFILE, sec, name, 0)+1);
		else
			data.iWrite(DATA_BINFILE, sec, name, 1);
	}

	sec = data.sLinkType(DATAS_TOTAL);
	sec = data.sLinkDiff(sec, mp.nowdifflv);

	if(mp.practicemode)
	{
		name = data.nLinkType(DATAN_PRACTICETIME);
		data.iWrite(DATA_BINFILE, sec, name, data.iRead(DATA_BINFILE, sec, name, 0)+1);
	}
	else if (first)
	{
		name = data.nLinkType(DATAN_PLAYTIME);
		data.iWrite(DATA_BINFILE, sec, name, data.iRead(DATA_BINFILE, sec, name, 0)+1);
		name = data.nLinkNum(name, Player::p.ID+1);
		data.iWrite(DATA_BINFILE, sec, name, data.iRead(DATA_BINFILE, sec, name, 0)+1);
		name = data.nLinkNum(name, Player::p.ID_sub_1+1);
		data.iWrite(DATA_BINFILE, sec, name, data.iRead(DATA_BINFILE, sec, name, 0)+1);
		name = data.nLinkNum(name, Player::p.ID_sub_2+1);
		data.iWrite(DATA_BINFILE, sec, name, data.iRead(DATA_BINFILE, sec, name, 0)+1);
	}
}