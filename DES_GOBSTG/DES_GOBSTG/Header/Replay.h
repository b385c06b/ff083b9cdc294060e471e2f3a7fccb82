#ifndef _REPLAY_H
#define _REPLAY_H

#include "Export.h"

class Replay
{
public:
	Replay();
	~Replay();

	void Fill();
	void partFill(BYTE part);
	bool Load(char * filename, bool getInput = false);
	void Save(char * filename);
	bool Check(char * filename);

	void AllocReplayFrame();
	void FreeReplayFrame();

	bool SetBias(replayFrame * _replayframe);
	float GetFPS(replayFrame * _replayframe);
	bool SaveInput(BYTE input, float fps);
	BYTE ReadInput(float * fps=NULL);

	static void Free(char * filename);

	static void Init();
	static void Release();

public:
	partInfo partinfo[RPYPARTMAX];
	replayInfo rpyinfo;

	replayFrame * replayframe;
	DWORD replayIndex;

	static Replay rpy;
};


#endif