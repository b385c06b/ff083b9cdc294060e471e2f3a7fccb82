#include "../Header/Effectsys.h"
#include "../Header/Main.h"
#include "../Header/Export.h"
#include "../Header/Player.h"
#include "../Header/Target.h"
#include "../Header/Process.h"

Effectsys Effectsys::effsys[EFFECTSYSMAX];
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

void Effectsys::Clear()
{
	exist = false;
}

void Effectsys::Action()
{
	DWORD stopflag = Process::mp.GetStopFlag();
	bool binstop = FRAME_STOPFLAGCHECK_(stopflag, FRAME_STOPFLAG_EFFECTSYS);
	if (!binstop)
	{
		for (int i=0; i<EFFECTSYSMAX; i++)
		{
			if (effsys[i].exist)
			{
				effsys[i].action();
			}
		}
	}
}

void Effectsys::ClearAll()
{
	for(int i=0; i<EFFECTSYSMAX; i++)
	{
		effsys[i].Clear();
	}
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

void Effectsys::RenderAll()
{
	for(int i=0; i<EFFECTSYSMAX; i++)
	{
		if(effsys[i].exist)
		{
			effsys[i].Render();
		}
	}
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
			float tarx;
			float tary;
			Target::GetValue(tarID, &tarx, &tary);
			chaseAim(tarx, tary, chasetimer);
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
		Target::SetValue(tarID, x, y);
	}
	eff->Update();
}