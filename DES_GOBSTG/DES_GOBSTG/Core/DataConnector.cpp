#include "../Header/DataConnector.h"
#include "../Header/Process.h"
#include "../Header/Player.h"
#include "../Header/Data.h"
#include "../Header/Replay.h"

int DataConnector::Insert()
{
	if(mp.replaymode)
		return -1;

	DWORD sec;
	if(mp.spellmode)
	{
		sec = Data::data.sLinkType(DATAS_SPELL);
		sec = Data::data.sLinkNum(sec, rpy.rpyinfo.startscene);

		if(Player::p.nScore > Data::data.lRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOPBONUS), 0))
		{
			Data::data.lWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOPBONUS), Player::p.nScore);
		}
		return -1;
	}

	if(mp.practicemode)
	{
		sec = Data::data.sLinkType(DATAS_STAGEPRACTICE);
		sec = Data::data.sLinkDiff(sec, rpy.rpyinfo.difflv);
		sec = Data::data.sLinkNum(sec, rpy.rpyinfo.startscene / M_STAGENSCENE);
		if(rpy.rpyinfo.score > Data::data.lRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOPSCORE), 0))
		{
			Data::data.lWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOPSCORE), rpy.rpyinfo.score);
		}
		return -1;
	}

	sec = Data::data.sLinkType(DATAS_TOP);
	sec = Data::data.sLinkDiff(sec, rpy.rpyinfo.difflv);

	for(int i=DATA_NTOP10FILESAVE;i>=0;i--)
	{
		DWORD secbuf = sec;
		secbuf = Data::data.sLinkNum(secbuf, i);
		if(i == 0 || Data::data.lRead(DATA_BINFILE, secbuf, Data::data.nLinkType(DATAN_SCORE), 0) > rpy.rpyinfo.score)
		{
			if(i == DATA_NTOP10FILESAVE)
				return 0;

			for(int j=DATA_NTOP10FILESAVE-1;j>i;j--)
			{
				Data::data.MoveDown(sec, j);
			}
			//
			sec = Data::data.sLinkNum(sec, i+1);

			Data::data.lWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_SCORE), rpy.rpyinfo.score);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_LASTSTAGE), rpy.rpyinfo.laststage);
			Data::data.sWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_USERNAME), rpy.rpyinfo.username);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_YEAR), rpy.rpyinfo.year);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_MONTH), rpy.rpyinfo.month);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_DAY), rpy.rpyinfo.day);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_HOUR), rpy.rpyinfo.hour);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_MINUTE), rpy.rpyinfo.minute);
			Data::data.fWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_LOST), rpy.rpyinfo.lost);
			Data::data.fWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_BORDERRATE), rpy.rpyinfo.borderrate);
			Data::data.fWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_FASTRATE), rpy.rpyinfo.fastrate);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_ALLTIME), rpy.rpyinfo.alltime);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_SPELLGET), rpy.rpyinfo.get);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_POINT), rpy.rpyinfo.point);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_FAITH), rpy.rpyinfo.faith);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_MISS), rpy.rpyinfo.miss);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_BOMB), rpy.rpyinfo.border);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_CONTINUE), rpy.rpyinfo.cont);
			Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_PAUSE), rpy.rpyinfo.pause);

			DWORD name;
			name = Data::data.nLinkType(DATAN_CHARA);
			for (int j=0; j<M_PL_ONESETPLAYER; j++)
			{
				name = Data::data.nLinkNum(name, j+1);
				Data::data.iWrite(DATA_BINFILE, sec, name, rpy.rpyinfo.usingchara[j]);
			}
			name = Data::data.nLinkType(DATAN_GETSPELL);
			for (int j=0; j<M_GETSPELLMAX; j++)
			{
				Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkNum(name, j+1), rpy.rpyinfo.getspell[j]);
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
	sec = Data::data.sLinkType(DATAS_SPELL);
	sec = Data::data.sLinkNum(sec, mp.scene);
	if(mp.spellmode)
	{
		t = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_MEETPRACTICE), 0);
		Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_MEETPRACTICE), t+1);

		WORD indi = Data::data.raGetIndi(mp.scene);
		indi |= 1 << 8;
		Data::data.raSetIndi(mp.scene, indi);
	}
	else
	{
		t = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_MEETGAME), 0);
		Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_MEETGAME), t+1);

		WORD indi = Data::data.raGetIndi(mp.scene);
		indi |= 1;
		Data::data.raSetIndi(mp.scene, indi);
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
	sec = Data::data.sLinkType(DATAS_SPELL);
	sec = Data::data.sLinkNum(sec, mp.scene);
	if(mp.spellmode)
	{
		t = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_GETPRACTICE), 0);
		Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_GETPRACTICE), t+1);
	
		WORD indi = Data::data.raGetIndi(mp.scene);
		indi |= 1 << 12;
		Data::data.raSetIndi(mp.scene, indi);
	}
	else
	{
		t = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_GETGAME), 0);
		Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_GETGAME), t+1);
		
		WORD indi = Data::data.raGetIndi(mp.scene);
		indi |= 1 << 4;
		Data::data.raSetIndi(mp.scene, indi);
	}
}

//luchara, scene, spellmode
int DataConnector::nMeet()
{
	return Data::data.nMeet(mp.scene, mp.spellmode);
}

//luchara, scene, spellmode
int DataConnector::nGet()
{
	return Data::data.nGet(mp.scene, mp.spellmode);
}

//replaymode, practicemode, nowdifflv, luchara
void DataConnector::Clear()
{
	if(mp.replaymode || mp.practicemode)
		return;

	DWORD sec;
	sec = Data::data.sLinkType(DATAS_TOTAL);
	sec = Data::data.sLinkDiff(sec, mp.nowdifflv);

	Data::data.iWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_CLEARTIME), Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_CLEARTIME), 0)+1);
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

	sec = Data::data.sLinkType(DATAS_TOTAL);
	if(playtimeEnd > mp.playtimeStart)
		Data::data.lWrite(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOTALPLAYTIME), Data::data.lRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOTALPLAYTIME), 0) + (playtimeEnd - mp.playtimeStart));

	mp.playtimeStart = 0;
}

//spellmode, luchara, scene, nowdifflv, practicemode
LONGLONG DataConnector::nHiScore()
{
	return Data::data.nHighScore(mp.scene, mp.nowdifflv, mp.spellmode, mp.practicemode);
}

//nowdifflv, luchara, scene
int DataConnector::nTryStageTime()
{
	return Data::data.nTryStageTime(Data::data.getStage(mp.scene), mp.nowdifflv);
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
		sec = Data::data.sLinkType(DATAS_STAGEPRACTICE);
		sec = Data::data.sLinkDiff(sec, mp.nowdifflv);
		sec = Data::data.sLinkNum(sec, mp.scene/M_STAGENSCENE);

		name = Data::data.nLinkType(DATAN_TRYTIME);
		if(mp.practicemode)
			Data::data.iWrite(DATA_BINFILE, sec, name, Data::data.iRead(DATA_BINFILE, sec, name, 0)+1);
		else
			Data::data.iWrite(DATA_BINFILE, sec, name, 1);
	}

	sec = Data::data.sLinkType(DATAS_TOTAL);
	sec = Data::data.sLinkDiff(sec, mp.nowdifflv);

	if(mp.practicemode)
	{
		name = Data::data.nLinkType(DATAN_PRACTICETIME);
		Data::data.iWrite(DATA_BINFILE, sec, name, Data::data.iRead(DATA_BINFILE, sec, name, 0)+1);
	}
	else if (first)
	{
		name = Data::data.nLinkType(DATAN_PLAYTIME);
		Data::data.iWrite(DATA_BINFILE, sec, name, Data::data.iRead(DATA_BINFILE, sec, name, 0)+1);
		name = Data::data.nLinkNum(name, Player::p.ID+1);
		Data::data.iWrite(DATA_BINFILE, sec, name, Data::data.iRead(DATA_BINFILE, sec, name, 0)+1);
		name = Data::data.nLinkNum(name, Player::p.ID_sub_1+1);
		Data::data.iWrite(DATA_BINFILE, sec, name, Data::data.iRead(DATA_BINFILE, sec, name, 0)+1);
		name = Data::data.nLinkNum(name, Player::p.ID_sub_2+1);
		Data::data.iWrite(DATA_BINFILE, sec, name, Data::data.iRead(DATA_BINFILE, sec, name, 0)+1);
	}
}