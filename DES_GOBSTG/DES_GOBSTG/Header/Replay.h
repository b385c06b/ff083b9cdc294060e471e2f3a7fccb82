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

	static void Free(char * filename);

public:
	partInfo partinfo[RPYPARTMAX];
	replayInfo rpyinfo;
};

extern Replay rpy;

#endif