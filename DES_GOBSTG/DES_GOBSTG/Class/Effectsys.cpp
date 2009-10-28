#include "Effectsys.h"
#include "Main.h"
#include "Export.h"
#include "Player.h"

Effectsys effsys[EFFECTSYSMAX];
hgeEffectSystem Effectsys::efftype[EFFECTSYSTYPEMAX];

Effectsys::Effectsys()
{
	eff = NULL;
	exist = false;
}

Effectsys::~Effectsys()
{
	if(eff)
	{
		eff->Stop(true);
		delete eff;
	}
	eff = NULL;
}

bool Effectsys::Init(HTEXTURE * tex, const char * foldername, char name[][M_PATHMAX])
{
	char buffer[M_STRMAX];
	for(int i=0;i<EFFECTSYSTYPEMAX;i++)
	{
		strcpy(buffer, foldername);
		strcat(buffer, strlen(name[i]) ? name[i] : name[0]);

		if(Export::effLoad(buffer, &efftype[i], tex) < 0)
		{
			HGELOG("%Failed in loading Effect System File %s.", HGELOG_ERRSTR, buffer);
			hgeEffectBasicInfo ebi;
			ZeroMemory(&ebi, sizeof(hgeEffectBasicInfo));
			efftype[i].SetBasicInfo(&ebi);
		}
	}
	return true;
}

void Effectsys::valueSet(WORD ID, float x, float y, int lifetime)
{
	valueSet(ID, lifetime, x, y, 0, 0xff, 9000, 0, 0);
}

void Effectsys::valueSet(WORD ID, BObject & owner)
{
	valueSet(ID, owner.x, owner.y);
}

void Effectsys::valueSet(WORD ID, int lifetime, float x, float y, BYTE tarID, int _chasetimer, BYTE _tarAim)
{
	valueSet(ID, lifetime, x, y, 0, tarID, 9000, 0, 0);
	chasetimer = _chasetimer;
	tarAim = _tarAim;
}

void Effectsys::valueSet(WORD _ID, int _lifetime, float _x, float _y, float _z, BYTE _tarID, int _angle, float _speed, float _zSpeed)
{
	ID			= _ID;
	tarID		= _tarID;
	lifetime	= _lifetime;
	x			= _x;
	y			= _y;
	z			= _z;
	angle		= _angle;
	speed		= _speed;
	zSpeed		= _zSpeed;
	headangle	= 0;
	hscale		= 1.0f;
	vscale		= 1.0f;
	timer		= 0;

	chasetimer = 0;

	exist = true;

	if(ID >= EFFECTSYSTYPEMAX)
		ID = 0;

	if(eff)
	{
		eff->Stop(true);
		delete eff;
	}
	eff = new hgeEffectSystem(efftype[ID]);

	MoveTo(x, y, z, true);
	Fire();
}

void Effectsys::Stop(bool bKill /* = false */)
{
	eff->Stop(bKill);
}

void Effectsys::Fire()
{
	eff->Fire();
}

void Effectsys::Render()
{
	eff->Render();
}

void Effectsys::MoveTo(float _x, float _y, float _z, bool bForce)
{
	x = _x;
	y = _y;
	z = _z;
	eff->MoveTo(x, y, z, bForce);
}

void Effectsys::action()
{
	timer++;

	if (lifetime > 0)
	{
		if (timer == lifetime)
		{
			Stop();
		}
		else if (timer == lifetime + EFFSYS_AUTOFADEOUT_TIME)
		{
			Stop(true);
			exist = false;
		}
	}

	if (chasetimer)
	{
		chasetimer--;
		if (tarAim == 0xff)
		{
			chaseAim(Player::p.x, Player::p.y, chasetimer);
		}
		else
		{
			chaseAim(tar[tarAim].x, tar[tarAim].y, chasetimer);
		}
	}

	if (speed)
	{
		x += speed * cost(angle);
		y += speed * sint(angle);
	}
	z += zSpeed;

	MoveTo(x, y, z);
	if (tarID != 0xff)
	{
		tar[tarID].x = x;
		tar[tarID].y = y;
	}
	eff->Update();
}