#ifndef _SPRITEITEMMANAGER_H
#define _SPRITEITEMMANAGER_H

#include "MainDependency.h"
#include "DataStruct.h"

class SpriteItemManager
{
public:
	SpriteItemManager();
	~SpriteItemManager();

	static void Init(HTEXTURE * tex);
	static hgeSprite * CreateSprite(int index);
	static bool CreateSprite(int index, hgeSprite * sprite);
	static int GetIndexByName(const char * spritename);
	static hgeSprite * CreateSpriteByName(const char * spritename);
	static bool CreateSpriteByName(const char * spritename, hgeSprite * sprite);
	static void FreeSprite(hgeSprite * sprite);
	static spriteData * CastSprite(int index);

	static bool SetSprite(int index, hgeSprite * sprite, HTEXTURE * tex);

	static bool ptFace(int index, hgeSprite * sprite, bool enemy = true);
	static bool ptName(int index, hgeSprite * sprite, bool enemy = true);

public:
	static HTEXTURE * tex;

	static int digituiIndex;

	static int faceIndexEnemy;
	static int nameIndexEnemy;
	static int faceIndexPlayer;
	static int nameIndexPlayer;

	static int yesIndex;
	static int noIndex;
	static int cancelIndex;
	static int confirmIndex;
};

#endif