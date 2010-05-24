#include "../Header/Replay.h"
#include "../Header/Process.h"
#include "../Header/Player.h"
#include "../Header/Data.h"
#include "../Header/BResource.h"

Replay Replay::rpy;

Replay::Replay()
{
	ZeroMemory(&rpyinfo, sizeof(replayInfo));
	replayframe = NULL;
}

Replay::~Replay()
{
	FreeReplayFrame();
}

void Replay::Init()
{
	rpy.AllocReplayFrame();
}

void Replay::Release()
{
	rpy.FreeReplayFrame();
}

void Replay::AllocReplayFrame()
{
	FreeReplayFrame();
#ifdef __WIN32
	replayframe = (replayFrame *)malloc(sizeof(replayFrame) * M_SAVEINPUTMAX);
#endif
}

void Replay::FreeReplayFrame()
{
	if (replayframe)
	{
		free(replayframe);
	}
}

void Replay::Free(char * filename)
{
	if (filename)
	{
		Export::rpyFree(filename);
	}
}

void Replay::Fill()
{
	WORD wYear;
	WORD wMonth;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	hge->Timer_GetSystemTime(&wYear, &wMonth, NULL, &wDay, &wHour, &wMinute);

	rpyinfo.modeflag = (Process::mp.spellmode?M_RPYMODE_SPELL:0)|(Process::mp.practicemode?M_RPYMODE_PRACTICE:0);

	rpyinfo.usingchara[0] = Process::mp.mainchara;
	rpyinfo.usingchara[1] = Process::mp.subchara_1;
	rpyinfo.usingchara[2] = Process::mp.subchara_2;

	rpyinfo.startscene = Process::mp.startscene;
	rpyinfo.endscene = Process::mp.endscene;

	rpyinfo.alltime = Process::mp.alltime;
	rpyinfo.year = wYear;
	rpyinfo.month = wMonth;
	rpyinfo.day = wDay;
	rpyinfo.hour = wHour;
	rpyinfo.minute = wMinute;

	rpyinfo.score = Player::p.nScore;
	rpyinfo.miss = Player::p.ncMiss;
	rpyinfo.border = Player::p.ncBorder;
	rpyinfo.cont = Player::p.ncCont;
	rpyinfo.get = Player::p.ncGet;
	rpyinfo.pause = Player::p.ncPause;
	rpyinfo.point = Player::p.nPoint;
	rpyinfo.faith = Player::p.nFaith;
	strcpy(rpyinfo.username, Process::mp.username);

	rpyinfo.lost = Player::lostStack / Process::mp.framecounter;
	rpyinfo.borderrate = (float)Player::p.borderCounter / Process::mp.alltime;
	rpyinfo.fastrate = (float)Player::p.fastCounter / Process::mp.alltime;

	rpyinfo.difflv = Process::mp.nowdifflv;
	if(rpyinfo.endscene == S1)
		rpyinfo.laststage = 0xff;
	else
		rpyinfo.laststage = Data::data.getStage(rpyinfo.endscene) - 1;

	for (int i=0; i<M_GETSPELLMAX; i++)
	{
		rpyinfo.getspell[i] = Player::p.getspell[i];
	}
	rpyinfo.offset = replayIndex;
}

void Replay::partFill(BYTE part)
{
	if (part < RPYPARTMAX)
	{
		partinfo[part].offset = replayIndex + 1;
		partinfo[part].scene = Process::mp.scene;
		partinfo[part].seed = Process::mp.seed;
		partinfo[part].nowplayer = Player::p.nLife;
		partinfo[part].nowpower = Player::p.nPower;
	}
	else
		part = 0;
	partinfo[part].nowfaith = Player::p.nFaith;
	partinfo[part].nowpoint = Player::p.nPoint;
	partinfo[part].nowgraze = Player::p.nGraze;
	partinfo[part].nowscore = Player::p.nScore;
	partinfo[part].nowID = Player::p.nowID;
}

bool Replay::Check(char * filename)
{
	BYTE * content;
	bool ret = false;

	char treplayfilename[M_PATHMAX];
	strcpy(treplayfilename, BResource::bres.resdata.replayfoldername);
	strcat(treplayfilename, filename);
	hge->Resource_AttachPack(treplayfilename, Data::data.password ^ REPLAYPASSWORD_XORMAGICNUM);
	content = hge->Resource_Load(hge->Resource_GetPackFirstFileName(treplayfilename));
	hge->Resource_RemovePack(treplayfilename);
	if(content)
	{
		if(strcmp((char *)(content + RPYOFFSET_SIGNATURE), BResource::bres.resdata.replaysignature11))
			goto exit;
		if(*(DWORD *)(content + RPYOFFSET_VERSION) != GAME_VERSION)
			goto exit;
		if(strcmp((char *)(content + RPYOFFSET_COMPLETESIGN), BResource::bres.resdata.replaycompletesign3))
			goto exit;
		ret = true;
	}
exit:
	hge->Resource_Free(content);
	return ret;
}

bool Replay::Load(char * filename, bool getInput)
{
	bool ret = false;
	if(Check(filename))
	{
        char treplayfilename[M_PATHMAX];
		strcpy(treplayfilename, BResource::bres.resdata.replayfoldername);
		strcat(treplayfilename, filename);
		if (getInput)
		{
			AllocReplayFrame();
		}
		ret = Export::rpyLoad(treplayfilename, &rpyinfo, partinfo, getInput ? replayframe : NULL);
		if (getInput)
		{
			replayIndex = 0;
		}
	}
	return ret;
}

void Replay::Save(char * filename)
{
	if(!filename)
		return;

	char buffer[M_STRITOAMAX];

	DWORD _size = RPYOFFSET_INPUTDATA + (replayIndex + 1) * RPYSIZE_FRAME;
	BYTE * _rpydata = (BYTE *)malloc(_size);
	DWORD tdw;
	memcpy(_rpydata + RPYOFFSET_SIGNATURE, BResource::bres.resdata.replaysignature11, RPYSIZE_SIGNATURE);
	tdw = GAME_VERSION;
	memcpy(_rpydata + RPYOFFSET_VERSION, &tdw, RPYSIZE_VERSION);
	memcpy(_rpydata + RPYOFFSET_COMPLETESIGN, BResource::bres.resdata.replaycompletesign3, RPYSIZE_COMPLETESIGN);
	memcpy(_rpydata + RPYOFFSET_TAG, BResource::bres.resdata.replaytag3, RPYSIZE_TAG);
	tdw = RPYOFFSET_PARTINFO;
	memcpy(_rpydata + RPYOFFSET_INFOOFFSET, &tdw, RPYSIZE_INFOOFFSET);
	strcpy(buffer, "");
	memcpy(_rpydata + RPYOFFSET_APPEND, buffer, RPYSIZE_APPEND);
	memcpy(_rpydata + RPYOFFSET_RPYINFO, &rpyinfo, RPYSIZE_RPYINFO);
	memcpy(_rpydata + RPYOFFSET_PARTINFO, partinfo, RPYSIZE_PARTINFO * RPYPARTMAX);
	memcpy(_rpydata + RPYOFFSET_INPUTDATA, replayframe, replayIndex * RPYSIZE_FRAME);

	replayFrame buff;
	buff.bias = 0;
	buff.input = 0xff;
	memcpy(_rpydata + _size-sizeof(replayFrame), &buff, sizeof(replayFrame));

	char treplayfilename[M_PATHMAX];
	strcpy(treplayfilename, BResource::bres.resdata.replayfoldername);
	strcat(treplayfilename, filename);

	char crcfilename[M_PATHMAX];
	strcpy(crcfilename, filename);
	strcat(crcfilename, hge->Math_itoa(hge->Resource_GetCRC(_rpydata, _size), buffer));
	hgeMemoryFile memfile;
	memfile.filename = crcfilename;
	memfile.data = _rpydata;
	memfile.size = _size;

	hge->Resource_CreatePack(treplayfilename, Data::data.password ^ REPLAYPASSWORD_XORMAGICNUM, &memfile, NULL);

	free(_rpydata);
}

bool Replay::SetBias(replayFrame * _replayframe)
{
	return Export::rpySetBias(_replayframe);
}

float Replay::GetFPS(replayFrame * _replayframe)
{
	return Export::rpyGetReplayFPS(_replayframe);
}

bool Replay::SaveInput(BYTE input, float fps)
{
	if (!replayframe || replayIndex >= M_SAVEINPUTMAX)
	{
		return false;
	}

	replayIndex++;
	replayframe[replayIndex].input = input;
	return SetBias(&(replayframe[replayIndex]));
}

BYTE Replay::ReadInput(float * fps/* =NULL */)
{
	if (!replayframe || replayIndex >= M_SAVEINPUTMAX)
	{
		return false;
	}

	replayIndex++;
	if (fps)
	{
		*fps = GetFPS(&replayframe[replayIndex]);
	}
	return replayframe[replayIndex].input;
}