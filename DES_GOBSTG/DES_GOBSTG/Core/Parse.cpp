#include "../Header/Parser.h"
#include "../Header/Scripter.h"
#include "../Header/ScripterDefines.h"
#include "../Header/keytable.h"
#include "../Header/processPrep.h"
#include "../Header/BulletListActionConst.h"

_Parser _Parser::_p;

_Parser::_Parser()
{
	rv = false;
	it = NULL;
	varcount = 0;
	indexcount = 0;
}

_Parser::~_Parser()
{

}

void _Parser::Init(vector<Script>::iterator * _it, int _varcount)
{
	_p.rv = false;
	_p.it = _it;
	_p.varcount = _varcount;
	_p.indexcount = 0;
}

bool _Parser::IsFloat(int i)
{
	return Scripter::scr.d[i].bfloat;
}

void _Parser::IntToFloat(int i)
{
	CFLOAT(Scripter::scr.d[i].value) = (float)(CINT(Scripter::scr.d[i].value));
}

void _Parser::FloatToInt(int i)
{
	CINT(Scripter::scr.d[i].value) = (float)(CFLOAT(Scripter::scr.d[i].value));
}

void _Parser::SetFloat(int i, float fval)
{
	Scripter::scr.SetFloatValue(i, fval);
}

void _Parser::SetInt(int i, int ival)
{
	Scripter::scr.SetIntValue(i, ival);
}

void _Parser::SetUInt(int i, DWORD uval, bool setfloat)
{
	Scripter::scr.SetDWORDValue(i, uval, setfloat);
}

float _Parser::GetFloat(int i)
{
	if (i < 0)
	{
		i = _p.indexcount;
		++(_p.indexcount);
	}
	return (float)CAST(Scripter::scr.d[i]);
}

int _Parser::GetInt(int i)
{
	if (i < 0)
	{
		i = _p.indexcount;
		++(_p.indexcount);
	}
	return (int)CAST(Scripter::scr.d[i]);
}

DWORD _Parser::GetUInt(int i)
{
	if (i < 0)
	{
		i = _p.indexcount;
		++(_p.indexcount);
	}
	return UCAST(Scripter::scr.d[i]);
}

char * _Parser::GetString(int i)
{
	if (i < 0)
	{
		i = _p.indexcount;
		++(_p.indexcount);
	}
	return Scripter::scr.GetString(i);
}

char * _Parser::GetStringSp(int i)
{
	if (i < 0)
	{
		i = _p.indexcount;
		++(_p.indexcount);
	}
	return Scripter::scr.GetStringSp(i);
}

bool _Parser::PushScript(_Parser ** _ppsaved, vector<Script> ** psaved, int ** idescsaved, TData ** descsaved)
{
	if (!_ppsaved)
	{
		return false;
	}
	if (Scripter::scr.PushScript(_p.varcount, psaved, idescsaved, descsaved))
	{
		*_ppsaved = (_Parser *)malloc(sizeof(_Parser));
		memcpy(*_ppsaved, &_p, sizeof(_Parser));
		return true;
	}
	return false;
}

bool _Parser::PopScript(_Parser ** _ppsaved, vector<Script> ** psaved, int ** idescsaved, TData ** descsaved)
{
	if (!_ppsaved)
	{
		return false;
	}
	if (Scripter::scr.PopScript(_p.varcount, psaved, idescsaved, descsaved))
	{
		memcpy(&_p, *_ppsaved, sizeof(_Parser));
		free(*_ppsaved);
		return true;
	}
	free(*_ppsaved);
	return false;
}

#define _ENTER_PARSE(X)	_p.rv = (X) ? Scripter::scr.Copy(_p.it, (X)) : true;	\
						_p.indexcount = 0;										\
						if (!_p.rv)												\
						{														\
							return false;										\
						}
#define _LEAVE_PARSE(X)	_p.indexcount = 0;	\
						return _p.rv;

bool _Parser::SD_()
{
	_ENTER_PARSE(2);

	if (IsFloat(1))
	{
		FloatToInt(1);
	}
	int _i = IGet();
	int _ival = IGet();
	SetInt(_i, _ival);

	_LEAVE_PARSE();
}

bool _Parser::SDf_()
{
	_ENTER_PARSE(2);

	if (!IsFloat(1))
	{
		IntToFloat(1);
	}
	int _i = IGet();
	float _fval = FGet();
	SetFloat(_i, _fval);

	_LEAVE_PARSE();
}

bool _Parser::ST_()
{
	_ENTER_PARSE(3);

	int _tdi = IGet();
	float _fx = FGet();
	float _fy = FGet();
	Target::SetValue(_tdi, _fx, _fy);

	_LEAVE_PARSE();
}

bool _Parser::CALL_()
{
	_ENTER_PARSE(2);

	_Parser * _psaved = NULL;
	vector<Script> * psaved = NULL;
	int * idescsaved = NULL;
	TData * descsaved = NULL;
	if (!PushScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}
	DWORD _con = UGet();
	DWORD _name = UGet();
	Scripter::scr.functionExecute(_con, _name);
	if (!PopScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}

	_LEAVE_PARSE();
}

bool _Parser::CALLEX_()
{
	_ENTER_PARSE(3);

	_Parser * _psaved = NULL;
	vector<Script> * psaved = NULL;
	int * idescsaved = NULL;
	TData * descsaved = NULL;
	if (!PushScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}
	int _tdi = GetUInt(2);
	if (_tdi)
	{
		_p.rv = Scripter::scr.Copy(_p.it, _tdi, 2);
	}
	DWORD _con = UGet();
	DWORD _name = UGet();
	Scripter::scr.functionExecute(_con, _name);
	if (!PopScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}

	_LEAVE_PARSE();
}

bool _Parser::EXECUTE_()
{
	_ENTER_PARSE(3);

	_Parser * _psaved = NULL;
	vector<Script> * psaved = NULL;
	int * idescsaved = NULL;
	TData * descsaved = NULL;
	if (!PushScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}
	DWORD _typeflag = UGet() + SCR_TYPEBEGIN;
	DWORD _con = UGet();
	DWORD _name = UGet();
	Scripter::scr.Execute(_typeflag, _con, _name);
	if (!PopScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}

	_LEAVE_PARSE();
}


bool _Parser::RETURN_()
{
	_ENTER_PARSE(1);

	int _retvalue = IGet();
	Process::mp.retvalue = _retvalue;

	_LEAVE_PARSE();
}

bool _Parser::SETSTATE_()
{
	_ENTER_PARSE(1);

	int _state = IGet();
	Process::mp.state = _state;

	_LEAVE_PARSE();
}

bool _Parser::SETTIME_()
{
	_ENTER_PARSE(1);

	int _gametime = IGet();
	gametime = _gametime;

	_LEAVE_PARSE();
}

bool _Parser::SETDIFFI_()
{
	_ENTER_PARSE(1);

	int _nowdifflv = IGet();
	Process::mp.SetDiffLv(_nowdifflv);

	_LEAVE_PARSE();
}

bool _Parser::SETCHARA_()
{
	_ENTER_PARSE(3);

	WORD _ID = IGet();
	WORD _ID_sub_1 = IGet();
	WORD _ID_sub_2 = IGet();
	Process::mp.SetChara(_ID, _ID_sub_1, _ID_sub_2);
	Player::p.SetID(_ID, _ID_sub_1, _ID_sub_2);

	_LEAVE_PARSE();
}

bool _Parser::SETSCENE_()
{
	_ENTER_PARSE(1);

	int _scene = IGet();
	Process::mp.scene = _scene;

	_LEAVE_PARSE();
}

bool _Parser::SETMODE_()
{
	_ENTER_PARSE(1);

	int _mode = IGet();
	Process::mp.SetMode(_mode);

	_LEAVE_PARSE();
}

bool _Parser::STARTPREP_()
{
	_ENTER_PARSE(0);

	Process::mp.startPrep(false);

	_LEAVE_PARSE();
}

bool _Parser::SETKEY_()
{
	_ENTER_PARSE(2);

	int _key = IGet();
	bool _bset = IGet();
	hge->Input_SetDIKey(_key, _bset);

	_LEAVE_PARSE();
}

bool _Parser::DISABLEALLKEY_()
{
	_ENTER_PARSE(0);

	hge->Input_SetDIKey(KS_FIRE_MP, false);
	hge->Input_SetDIKey(KS_SPECIAL_MP, false);
	hge->Input_SetDIKey(KS_CHANGE_MP, false);
	hge->Input_SetDIKey(KS_SLOW_MP, false);
	hge->Input_SetDIKey(KS_UP_MP, false);
	hge->Input_SetDIKey(KS_DOWN_MP, false);
	hge->Input_SetDIKey(KS_LEFT_MP, false);
	hge->Input_SetDIKey(KS_RIGHT_MP, false);

	_LEAVE_PARSE();
}

bool _Parser::SETPUSHEVENT_()
{
	_ENTER_PARSE(7);

	BYTE _ID = IGet();
	int _pushkey_1 = IGet();
	int _pushkey_2 = IGet();
	int _pushkey_3 = IGet();
	int _pushkey_4 = IGet();
	int _pushfirst = IGet();
	int _pushrollto = IGet();
	PushKey::SetPushEvent(_ID, _pushkey_1, _pushkey_2, _pushkey_3, _pushkey_4, _pushfirst, _pushrollto);
	_LEAVE_PARSE();
}

bool _Parser::UPDATEPUSHEVENT_()
{
	_ENTER_PARSE(1);

	BYTE _ID = IGet();
	PushKey::UpdatePushEvent(_ID);

	_LEAVE_PARSE();
}

bool _Parser::STOPACTION_()
{
	_ENTER_PARSE(2);

	DWORD _stopflag = UGet();
	int _stoptime = IGet();
	Process::mp.SetStop(_stopflag, _stoptime);

	_LEAVE_PARSE();
}

bool _Parser::SETFRAMESKIP_()
{
	_ENTER_PARSE(1);

	int _frameskip = IGet();
	Process::mp.frameskip = _frameskip;

	_LEAVE_PARSE();
}


bool _Parser::MUSICCHANGE_()
{
	_ENTER_PARSE(2);

	BYTE _ID = IGet();
	bool _force = (bool)IGet();
	Process::mp.musicChange(_ID, _force);

	_LEAVE_PARSE();
}

bool _Parser::MUSICSLIDE_()
{
	_ENTER_PARSE(4);

	float _slidetime = FGet();
	int _tovol = IGet();
	int _pan = IGet();
	float _pitch = FGet();
	Process::mp.musicSlide(_slidetime, _tovol, _pan, _pitch);

	_LEAVE_PARSE();
}

bool _Parser::SE_()
{
	_ENTER_PARSE(2);

	BYTE _type = IGet();
	float _x = FGet();
	SE::push(_type, _x);

	_LEAVE_PARSE();
}

bool _Parser::SEOFF_()
{
	_ENTER_PARSE(1);

	BYTE _type = IGet();
	SE::stop(_type);

	_LEAVE_PARSE();
}

bool _Parser::HSVTORGB_()
{
	_ENTER_PARSE(5);

	float _h = IGet() / 255.0f;
	float _s = IGet() / 255.0f;
	float _v = IGet() / 255.0f;
	float _a = IGet() / 255.0f;
	int _tdi = IGet();
	hgeColorHSV _hsv(_h, _s, _v, _a);
	SetUInt(_tdi, _hsv.GetHWColor());

	_LEAVE_PARSE();
}

bool _Parser::SETSHAKE_()
{
	_ENTER_PARSE(2);

	BYTE _round = IGet();
	bool _force = (bool)IGet();
	Process::mp.SetShake(_round, _force);

	_LEAVE_PARSE();
}


bool _Parser::B_()
{
	_ENTER_PARSE(9);

	float _x = FGet();
	float _y = FGet();
	int _angle = IGet();
	float _speed = FGet();
	BYTE _type = IGet();
	BYTE _color = IGet();
	int _fadeintime = IGet();
	float _avoid = FGet();
	BYTE _tarID = IGet();
	Bullet::Build(_x, _y, _angle, _speed, _type, _color, _fadeintime, _avoid, _tarID);

	_LEAVE_PARSE();
}

bool _Parser::A_()
{
	_ENTER_PARSE(1);

	int _tdi = IGet();
	if(!_tdi)
	{
		Bullet::_bu.actionList[0] = SECTIONEND;
	}
	else if(_p.rv = Scripter::scr.Copy(_p.it, _tdi))
	{
		for(int i=0; i<_tdi; i++)
		{
			Bullet::_bu.actionList[i] = GetInt(i);
		}
		Bullet::_bu.actionList[_tdi] = SECTIONEND;
	}

	_LEAVE_PARSE();
}

bool _Parser::BC_()
{
	_ENTER_PARSE(10);

	int _num = IGet();
	int _baseangle = IGet();
	float _baser = FGet();
	float _x = FGet();
	float _y = FGet();
	float _speed = FGet();
	BYTE _type = IGet();
	BYTE _color = IGet();
	int _fadeintime = IGet();
	float _avoid = FGet();
	Bullet::BuildCircle(_num, _baseangle, _baser, _x, _y, _speed, _type, _color, _fadeintime, _avoid);

	_LEAVE_PARSE();
}

bool _Parser::BL_()
{
	_ENTER_PARSE(14);

	int _num = IGet();
	int _baseangle = IGet();
	float _space = FGet();
	int _baseindex = IGet();
	float _x = FGet();
	float _y = FGet();
	int _angle = IGet();
	float _anglefactor = FGet();
	float _speed = FGet();
	float _speedfactor = FGet();
	BYTE _type = IGet();
	BYTE _color = IGet();
	int _fadeintime = IGet();
	float _avoid = FGet();
	Bullet::BuildLine(_num, _baseangle, _space, _baseindex, _x, _y, _angle, _anglefactor, _speed, _speedfactor, _type, _color, _fadeintime, _avoid);

	_LEAVE_PARSE();
}

bool _Parser::BUDECANCEL_()
{
	_ENTER_PARSE(1);

	int _tdi = IGet();
	if (_tdi >= 0 && _tdi < BULLETMAX)
	{
		if (Bullet::bu.toIndex(_tdi) != VECLST_INDEXERROR)
		{
			(*Bullet::bu).cancelable = false;
		}
	}

	_LEAVE_PARSE();
}

bool _Parser::IZEZONE_()
{
	_ENTER_PARSE(6);

	BYTE _type = IGet();
	float _x = FGet();
	float _y = FGet();
	BYTE _maxtime = IGet();
	float _r = FGet();
	BYTE _eventID = IGet();
	Bullet::IzeBuild(_type, _x, _y, _maxtime, _r, _eventID);

	_LEAVE_PARSE();
}

bool _Parser::BEB_()
{
	_ENTER_PARSE(12);

	float _x = FGet();
	float _y = FGet();
	int _angle = IGet();
	float _speed = FGet();
	BYTE _type = IGet();
	BYTE _color = IGet();
	float _length = FGet();
	float _width = FGet();
	BYTE _flag = IGet();
	int _fadeintime = IGet();
	int _fadeouttime = IGet();
	BYTE _tarID = IGet();
	Beam::Build(_x, _y, _angle, _speed, _type, _color, _length, _width, _flag, _fadeintime, _fadeouttime, _tarID);

	_LEAVE_PARSE();
}

bool _Parser::BEVECTOR_()
{
	_ENTER_PARSE(5);

	int _tdi = IGet();
	float _orix = FGet();
	float _oriy = FGet();
	float _aimx = FGet();
	float _aimy = FGet();
	if (Beam::be.toIndex(_tdi) != VECLST_INDEXERROR)
	{
		if (Beam::be.isInRange() && Beam::be.isValid())
		{
			if ((*Beam::be).able)
			{
				(*Beam::be).SetVector(_orix, _oriy, _aimx, _aimy);
			}
		}
	}

	_LEAVE_PARSE();
}

bool _Parser::BEHOLD_()
{
	_ENTER_PARSE(4);

	int _tdi = IGet();
	BYTE _holdtar = IGet();
	float _holdoffset = FGet();
	int _angle = IGet();
	if (Beam::be.toIndex(_tdi) != VECLST_INDEXERROR)
	{
		if (Beam::be.isInRange() && Beam::be.isValid())
		{
			if ((*Beam::be).able)
			{
				(*Beam::be).SetHold(_holdtar, _holdoffset, _angle);
			}
		}
	}

	_LEAVE_PARSE();
}

bool _Parser::BEPIN_()
{
	_ENTER_PARSE(4);

	int _tdi = IGet();
	BYTE _holdtar = IGet();
	BYTE _pintar = IGet();
	float _holdoffset = FGet();
	if (Beam::be.toIndex(_tdi) != VECLST_INDEXERROR)
	{
		if (Beam::be.isInRange() && Beam::be.isValid())
		{
			if ((*Beam::be).able)
			{
				(*Beam::be).SetHoldPin(_holdtar, _pintar, _holdoffset);
			}

		}
	}

	_LEAVE_PARSE();
}


bool _Parser::GB_()
{
	_ENTER_PARSE(10);

	WORD _gID = IGet();
	BYTE _tarID = IGet();
	BYTE _belong = IGet();
	float _x = FGet();
	float _y = FGet();
	int _angle = IGet();
	float _speed = FGet();
	BYTE _type = IGet();
	float _life = FGet();
	int _ac = IGet();
	Ghost::Build(_gID, _tarID, _belong, _x, _y, _angle, _speed, _type, _life, _ac);

	_LEAVE_PARSE();
}

bool _Parser::GHSAIM_()
{
	_ENTER_PARSE(2);

	float _aimx = FGet();
	float _aimy = FGet();
	Ghost::gh[Ghost::index].SetAim(_aimx, _aimy);

	_LEAVE_PARSE();
}

bool _Parser::GHSET_()
{
	_ENTER_PARSE(3);

	int _angle = IGet();
	float _speed = FGet();
	int _ac = IGet();
	Ghost::gh[Ghost::index].SetValue(_angle, _speed, _ac);

	_LEAVE_PARSE();
}

bool _Parser::GHCHASE_()
{
	_ENTER_PARSE(3);

	float _x = FGet();
	float _y = FGet();
	int _chasetimer = IGet();
	Ghost::gh[Ghost::index].chaseAim(_x, _y, _chasetimer);

	_LEAVE_PARSE();
}

bool _Parser::GHSETLIFE_()
{
	_ENTER_PARSE(2);

	float _life = FGet();
	float _maxlife = FGet();
	Ghost::gh[Ghost::index].SetLife(_life, _maxlife);

	_LEAVE_PARSE();
}

bool _Parser::GHCHANGE_()
{
	_ENTER_PARSE(1);

	WORD _gID = IGet();
	Ghost::gh[Ghost::index].ChangeGID(_gID);

	_LEAVE_PARSE();
}


bool _Parser::EB_()
{
	_ENTER_PARSE(11);

	WORD _eID = IGet();
	BYTE _index = IGet();
	BYTE _tarID = IGet();
	float _x = FGet();
	float _y = FGet();
	int _angle = IGet();
	float _speed = FGet();
	BYTE _type = IGet();
	float _life = FGet();
	int _infitimer = IGet();
	DWORD _take = UGet();
	Enemy::Build(_eID, _index, _tarID, _x, _y, _angle, _speed, _type, _life, _infitimer, _take);

	_LEAVE_PARSE();
}

bool _Parser::EA_()
{
	_ENTER_PARSE(1);

	int _ac = IGet();
	if(_ac != ENAC_NONE)
	{
		if(_p.rv = Scripter::scr.Copy(_p.it, ENEMY_PARAMAX*2))
		{
			_p.indexcount = 0;

			float _fpara[ENEMY_PARAMAX];
			int _ipara[ENEMY_PARAMAX];
			for (int i=0; i<ENEMY_PARAMAX; i++)
			{
				_fpara[i] = FGet();
			}
			for (int i=0; i<ENEMY_PARAMAX; i++)
			{
				_ipara[i] = IGet();
			}

			Enemy::en[Enemy::index].setAction(_ac, _fpara[0], _fpara[1], _fpara[2], _fpara[3], _ipara[0], _ipara[1], _ipara[2], _ipara[3]);
		}
	}
	else
	{
		Enemy::en[Enemy::index].setAction(_ac);
	}

	_LEAVE_PARSE();
}

bool _Parser::ENSAIM_()
{
	_ENTER_PARSE(2);

	float _aimx = FGet();
	float _aimy = FGet();
	Enemy::en[Enemy::index].SetAim(_aimx, _aimy);

	_LEAVE_PARSE();
}

bool _Parser::ENCHASE_()
{
	_ENTER_PARSE(3);

	float _x = FGet();
	float _y = FGet();
	int _chasetimer = IGet();
	Enemy::en[Enemy::index].chaseAim(_x, _y, _chasetimer);

	_LEAVE_PARSE();
}

bool _Parser::ENSETLIFE_()
{
	_ENTER_PARSE(2);

	float _life = FGet();
	float _maxlife = FGet();
	Enemy::en[Enemy::index].SetLife(_life, _maxlife);

	_LEAVE_PARSE();
}

bool _Parser::ENCHANGE_()
{
	_ENTER_PARSE(1);

	WORD _eID = IGet();
	Enemy::en[Enemy::index].ChangeEID(_eID);

	_LEAVE_PARSE();
}

bool _Parser::DAMAGEZONE_()
{
	_ENTER_PARSE(4);

	float _x = FGet();
	float _y = FGet();
	float _r = FGet();
	float _power = FGet();
	Enemy::DamageZoneBuild(_x, _y, _r, _power);

	_LEAVE_PARSE();
}

bool _Parser::BOSSSTORE_()
{
	_ENTER_PARSE(0);

	if (Enemy::index < ENEMY_INDEXSTART)
	{
		Enemy::bossflag[Enemy::index] = BOSS_STORE;
	}

	_LEAVE_PARSE();
}

bool _Parser::BOSSATTACK_()
{
	_ENTER_PARSE(0);

	if (Enemy::index < ENEMY_INDEXSTART)
	{
		Enemy::bossflag[Enemy::index] = BOSS_ATTACK;
	}

	_LEAVE_PARSE();
}


bool _Parser::RAMA_()
{
	_ENTER_PARSE(7);

	bool _brama = (bool)IGet();
	float _orix = FGet();
	float _oriy = FGet();
	float _aimx = FGet();
	float _aimy = FGet();
	int _iret = 0;
	if (_brama)
	{
		float _r = FGet();
		_iret = BObject::RMainAngle(_orix, _oriy, _aimx, _aimy, _r);
	}
	else
	{
		int _a = IGet();
		_iret = BObject::AMainAngle(_orix, _oriy, _aimx, _aimy, _a);
	}
	int _tdi = IGet();
	SetInt(_tdi, _iret);

	_LEAVE_PARSE();
}

bool _Parser::DIST_()
{
	_ENTER_PARSE(5);

	float _orix = FGet();
	float _oriy = FGet();
	float _aimx = FGet();
	float _aimy = FGet();
	int _tdi = IGet();
	SetInt(_tdi, DIST(_orix, _oriy, _aimx, _aimy));

	_LEAVE_PARSE();
}

bool _Parser::CHASEAIM_()
{
	_ENTER_PARSE(3);

	int _oritarID = IGet();
	int _aimtarID = IGet();
	int _chasetimer = IGet();
	BObject::ChaseAim(Target::GetX(_oritarID), Target::GetY(_oritarID), Target::GetX(_aimtarID), Target::GetY(_aimtarID), _chasetimer);
	BObject::UpdateMove();
	Target::SetValue(_oritarID, BObject::GetObject().x, BObject::GetObject().y);

	_LEAVE_PARSE();
}

bool _Parser::INTER_()
{
	_ENTER_PARSE(4);

	float _a = FGet();
	float _b = FGet();
	float _x = FGet();
	int _tdi = IGet();

	SetFloat(_tdi, INTER(_a, _b, _x));

	_LEAVE_PARSE();
}


bool _Parser::LOADTEXTURESET_()
{
	_ENTER_PARSE(1);

	int _texset = IGet();
	Process::mp.LoadTextureSet(_texset);

	_LEAVE_PARSE();
}

bool _Parser::FREETEXTURESET_()
{
	_ENTER_PARSE(1);

	int _texset = IGet();
	Process::mp.FreeTextureSet(_texset);

	_LEAVE_PARSE();
}

bool _Parser::ITEMBUILD_()
{
	_ENTER_PARSE(6);

	WORD _type = IGet();
	float _x = FGet();
	float _y = FGet();
	bool _bDrained = (bool)IGet();
	int _angle = IGet();
	float _speed = FGet();
	Item::Build(_type, _x, _y, _bDrained, _angle, _speed);

	_LEAVE_PARSE();
}


bool _Parser::BOSSUP_()
{
	_ENTER_PARSE(0);

	BossInfo::bossinfo.bossUp();

	_LEAVE_PARSE();
}

bool _Parser::SPELLUP_()
{
	_ENTER_PARSE(3);

	int _tdi = IGet();
	float _x = FGet();
	float _y = FGet();
	Target::SetValue(_tdi, _x, _y);

	_LEAVE_PARSE();
}

bool _Parser::ACONTROL_()
{
	_ENTER_PARSE(2);

	int _tindex = IGet();
	int _tdi = IGet();
	if (_p.rv = Scripter::scr.Copy(_p.it, _tdi))
	{
		_p.indexcount = 0;
		if (_tindex >= 0 && _tindex < BULLETMAX)
		{
			if (Bullet::bu.toIndex(_tindex) != VECLST_INDEXERROR)
			{
				if (Bullet::bu.isInRange() && Bullet::bu.isValid())
				{
					if ((*Bullet::bu).able)
					{
						if(!_tdi)
						{
							(*Bullet::bu).actionList[0] = SECTIONEND;
						}
						else
						{
							for(int j=0;j<_tdi;j++)
							{
								(*Bullet::bu).actionList[j] = IGet();
							}
							(*Bullet::bu).actionList[_tdi] = SECTIONEND;
						}
					}
				}
			}
		}
	}

	_LEAVE_PARSE();
}

bool _Parser::BCONTROL_()
{
	_ENTER_PARSE(2);

	BObject * _tobj = NULL;
	int _ttype = IGet();
	int _tdi = IGet();
	int _tindex = GetInt(_tdi);

	if (_tindex >= 0)
	{
		switch(_ttype)
		{
		case BTYPE_BULLET:
			if (_tindex < BULLETMAX)
			{
				if (Bullet::bu.toIndex(_tindex) != VECLST_INDEXERROR)
				{
					if (Bullet::bu.isInRange() && Bullet::bu.isValid())
					{
						if ((*Bullet::bu).able)
						{
							_tobj = &(*Bullet::bu);
						}
					}
				}
			}
			break;
		case BTYPE_BEAM:
			if (_tindex < BEAMMAX)
			{
				if (Beam::be.toIndex(_tindex) != VECLST_INDEXERROR)
				{
					if (Beam::be.isInRange() && Beam::be.isValid())
					{
						if ((*Beam::be).able)
						{
							_tobj = &(*Beam::be);
						}
					}
				}
			}
			break;
		case BTYPE_ENEMY:
			if (_tindex < ENEMYMAX)
			{
				if(Enemy::en[_tindex].able)
				{
					Enemy::index = _tindex;
					_tobj = &Enemy::en[Enemy::index];
				}
			}
			break;
		case BTYPE_GHOST:
			if (_tindex < GHOSTMAX)
			{
				if(Ghost::gh[_tindex].able)
				{
					Ghost::index = _tindex;
					_tobj = &Ghost::gh[Ghost::index];
				}
			}
			break;
		}
	}

	if (_p.rv = Scripter::scr.Copy(_p.it, 5))
	{
		_p.indexcount = 0;
		if(_tobj)
		{
			_tobj->x = FGet();
			_tobj->y = FGet();
			_tobj->angle = IGet();
			_tobj->headangle = IGet();
			_tobj->speed = FGet();
		}
		else
		{
			SetInt(_tdi, BINDEX_NULL);
		}
	}

	_LEAVE_PARSE();
}

bool _Parser::COLLISION_CIRCLE_()
{
	_ENTER_PARSE(6);

	bool _bret = false;
	float _orix = FGet();
	float _oriy = FGet();
	float _aimx = FGet();
	float _aimy = FGet();
	float _r = FGet();
	int _tdi = IGet();
	_bret = BObject::CheckCollisionCircle(_orix, _oriy, _aimx, _aimy, _r);
	SetInt(_tdi, _bret?1:0);

	_LEAVE_PARSE();
}

bool _Parser::COLLISION_SQUARE_()
{
	_ENTER_PARSE(6);

	bool _bret = false;
	float _orix = FGet();
	float _oriy = FGet();
	float _aimx = FGet();
	float _aimy = FGet();
	float _r = FGet();
	int _tdi = IGet();
	_bret = BObject::CheckCollisionSquare(_orix, _oriy, _aimx, _aimy, _r);
	SetInt(_tdi, _bret?1:0);

	_LEAVE_PARSE();
}


bool _Parser::EFFSETUP_()
{
	_ENTER_PARSE(5);

	int _tdi = IGet();
	WORD _ID = IGet();
	float _x = FGet();
	float _y = FGet();
	int _lifetime = IGet();
	Effectsys::effsys[_tdi].valueSet(_ID, _x, _y, _lifetime);

	_LEAVE_PARSE();
}

bool _Parser::EFFSETUPEX_()
{
	_ENTER_PARSE(10);

	int _tdi = IGet();
	WORD _ID = IGet();
	int _lifetime = IGet();
	float _x = FGet();
	float _y = FGet();
	float _z = FGet();
	BYTE _tarID = IGet();
	int _angle = IGet();
	float _speed = FGet();
	float _zSpeed = FGet();
	Effectsys::effsys[_tdi].valueSet(_ID, _lifetime, _x, _y, _z, _tarID, _angle, _speed, _zSpeed);

	_LEAVE_PARSE();
}

bool _Parser::EFFSETUPCHASE_()
{
	_ENTER_PARSE(8);

	int _tdi = IGet();
	WORD _ID = IGet();
	int _lifetime = IGet();
	float _x = FGet();
	float _y = FGet();
	BYTE _tarID = IGet();
	int _chasetimer = IGet();
	BYTE _taraimID = IGet();
	Effectsys::effsys[_tdi].valueSet(_ID, _lifetime, _x, _y, _tarID, _chasetimer, _taraimID);

	_LEAVE_PARSE();
}

bool _Parser::EFFSTOP_()
{
	_ENTER_PARSE(1);

	int _tdi = IGet();
	if (Effectsys::effsys[_tdi].exist && Effectsys::effsys[_tdi].eff)
	{
		Effectsys::effsys[_tdi].Stop();
	}

	_LEAVE_PARSE();
}

bool _Parser::EFFOFF_()
{
	_ENTER_PARSE(1);

	int _tdi = IGet();
	Effectsys::effsys[_tdi].exist = false;

	_LEAVE_PARSE();
}

bool _Parser::EFFSET_()
{
	_ENTER_PARSE(4);

	int _tdi = IGet();
	int _angle = IGet();
	float _speed = FGet();
	float _zSpeed = FGet();
	Effectsys::effsys[_tdi].SetValue(_angle, _speed, _zSpeed);

	_LEAVE_PARSE();
}

bool _Parser::EFFMOVETO_()
{
	_ENTER_PARSE(5);

	int _tdi = IGet();
	float _x = FGet();
	float _y = FGet();
	float _z = FGet();
	bool _bForce = (bool)IGet();
	if (!_bForce)
	{
		Effectsys::effsys[_tdi].x = _x;
		Effectsys::effsys[_tdi].y = _y;
		Effectsys::effsys[_tdi].z = _z;
	}
	else
	{
		Effectsys::effsys[_tdi].MoveTo(_x, _y, _z, _bForce);
	}

	_LEAVE_PARSE();
}


bool _Parser::SETPPOS_()
{
	_ENTER_PARSE(2);

	float _x = FGet();
	float _y = FGet();
	Player::p.SetPos(_x, _y);

	_LEAVE_PARSE();
}

bool _Parser::SETPLIFE_()
{
	_ENTER_PARSE(1);

	int _nLife = IGet();
	Player::p.SetLife(_nLife);
	Player::p.nLife = _nLife > PL_NPLAYERMAX ? PL_NPLAYERMAX : _nLife;

	_LEAVE_PARSE();
}

bool _Parser::SETPPOWER_()
{
	_ENTER_PARSE(1);

	int _nPower = IGet();
	if (_nPower >= 0)
	{
		Player::p.nPower = _nPower;
	}

	_LEAVE_PARSE();
}

bool _Parser::SETPFAITH_()
{
	_ENTER_PARSE(1);

	int _nFaith = IGet();
	if (_nFaith >= 0)
	{
		Player::p.nFaith = _nFaith;
	}

	_LEAVE_PARSE();
}

bool _Parser::SETPPOINT_()
{
	_ENTER_PARSE(1);

	int _nPoint = IGet();
	if (_nPoint >= 0)
	{
		Player::p.nPoint = _nPoint;
	}

	_LEAVE_PARSE();
}

bool _Parser::SETPBBORDER_()
{
	_ENTER_PARSE(1);

	bool _bBorder = (bool)IGet();
	if (_bBorder ^ Player::p.bBorder)
	{
		Player::p.callBomb(true);
	}

	_LEAVE_PARSE();
}

bool _Parser::SETPBSLOW_()
{
	_ENTER_PARSE(1);

	bool _toslow = (bool)IGet();
	Player::p.callSlowFastChange(_toslow);

	_LEAVE_PARSE();
}

bool _Parser::SETPBINFI_()
{
	_ENTER_PARSE(0);

	Player::p.bInfi = true;

	_LEAVE_PARSE();
}

bool _Parser::SETPSPEED_()
{
	_ENTER_PARSE(1);

	float _speed = FGet();
	Player::p.speed = _speed;

	_LEAVE_PARSE();
}

bool _Parser::SETPSLOWSPEED_()
{
	_ENTER_PARSE(1);

	float _slowspeed = FGet();
	Player::p.slowspeed = _slowspeed;

	_LEAVE_PARSE();
}

bool _Parser::SETPSPEEDFACTOR_()
{
	_ENTER_PARSE(1);

	float _speedfactor = FGet();
	Player::p.speedfactor = _speedfactor;

	_LEAVE_PARSE();
}

bool _Parser::COLLAPSE_()
{
	_ENTER_PARSE(0);

	Player::p.Collapse();

	_LEAVE_PARSE();
}

bool _Parser::SHOOTPB_()
{
	_ENTER_PARSE(1);

	int _shootdataID = IGet();
	PlayerBullet::Build(_shootdataID);

	_LEAVE_PARSE();
}

bool _Parser::BONUSFLAG_()
{
	_ENTER_PARSE(1);

	BYTE _bonusflag = IGet();
	Player::p.bonusflag = _bonusflag;

	_LEAVE_PARSE();
}


bool _Parser::BGVALUE_()
{
	_ENTER_PARSE(7);

	int _tdi = IGet();
	int _siid = IGet();
	float _cenx = FGet();
	float _ceny = FGet();
	float _width = FGet();
	float _height = FGet();
	DWORD _col = UGet();
	BGLayer::ubg[_tdi].valueSet(_siid, _cenx, _ceny, _width, _height, _col);

	_LEAVE_PARSE();
}

bool _Parser::BGVALEX_()
{
	_ENTER_PARSE(16);

	int _tdi = IGet();
	int _siid = IGet();
	float _x = FGet();
	float _y = FGet();
	float _z = FGet();
	float _w = FGet();
	float _h = FGet();
	float _rotx = FGet();
	float _roty = FGet();
	float _rotz = FGet();
	float _paral = FGet();
	float _speed = FGet();
	int _angle = IGet();
	bool _bmove = (bool)IGet();
	bool _brotate = (bool)IGet();
	DWORD _col = UGet();
	BGLayer::ubg[_tdi].valueSet(_siid, _x, _y, _z, _w, _h, _rotx, _roty, _rotz, _paral, _speed, _angle, _bmove, _brotate, _col);

	_LEAVE_PARSE();
}

bool _Parser::BGTEXRECT_()
{
	_ENTER_PARSE(5);

	int _tdi = IGet();
	float _texx = FGet();
	float _texy = FGet();
	float _texw = FGet();
	float _texh = FGet();
	BGLayer::ubg[_tdi].texRectSet(_texx, _texy, _texw, _texh);

	_LEAVE_PARSE();
}

bool _Parser::BGRECT_()
{
	_ENTER_PARSE(9);

	int _tdi = IGet();
	float _x = FGet();
	float _y = FGet();
	float _z = FGet();
	float _w = FGet();
	float _h = FGet();
	float _rotx = FGet();
	float _roty = FGet();
	float _rotz = FGet();
	BGLayer::ubg[_tdi].rectSet(_x, _y, _z, _w, _h, _rotx, _roty, _rotz);

	_LEAVE_PARSE();
}

bool _Parser::BGZ_()
{
	_ENTER_PARSE(5);

	int _tdi = IGet();
	float _z0 = FGet();
	float _z1 = FGet();
	float _z2 = FGet();
	float _z3 = FGet();
	BGLayer::ubg[_tdi].zSet(_z0, _z1, _z2, _z3);

	_LEAVE_PARSE();
}

bool _Parser::BGSCALE_()
{
	_ENTER_PARSE(3);

	int _tdi = IGet();
	float _hscale = FGet();
	float _vscale = FGet();
	BGLayer::ubg[_tdi].scaleSet(_hscale, _vscale);

	_LEAVE_PARSE();
}

bool _Parser::BGCOLOR_()
{
	_ENTER_PARSE(5);

	int _tdi = IGet();
	DWORD _col0 = UGet();
	DWORD _col1 = UGet();
	DWORD _col2 = UGet();
	DWORD _col3 = UGet();
	BGLayer::ubg[_tdi].colorSet(_col0, _col1, _col2, _col3);

	_LEAVE_PARSE();
}

bool _Parser::BGMOVE_()
{
	_ENTER_PARSE(6);

	int _tdi = IGet();
	float _speed = FGet();
	float _zSpeed = FGet();
	int _angle = IGet();
	bool _bmove = (bool)IGet();
	bool _brotate = (bool)IGet();
	BGLayer::ubg[_tdi].moveSet(_speed, _zSpeed, _angle, _bmove, _brotate);

	_LEAVE_PARSE();
}

bool _Parser::BGFLAG_()
{
	_ENTER_PARSE(3);

	int _tdi = IGet();
	BYTE _flag = IGet();
	BYTE _maxtime = IGet();
	BGLayer::ubg[_tdi].SetFlag(_flag, _maxtime);

	_LEAVE_PARSE();
}

bool _Parser::BGPARAL_()
{
	_ENTER_PARSE(2);

	int _tdi = IGet();
	float _paral = FGet();
	BGLayer::ubg[_tdi].parallelogram(_paral);

	_LEAVE_PARSE();
}

bool _Parser::BG4V_()
{
	_ENTER_PARSE(13);

	int _tdi = IGet();
	float _x0 = FGet();
	float _y0 = FGet();
	float _z0 = FGet();
	float _x1 = FGet();
	float _y1 = FGet();
	float _z1 = FGet();
	float _x2 = FGet();
	float _y2 = FGet();
	float _z2 = FGet();
	float _x3 = FGet();
	float _y3 = FGet();
	float _z3 = FGet();
	BGLayer::ubg[_tdi].vertexSet(_x0, _y0, _z0, _x1, _y1, _z1, _x2, _y2, _z2, _x3, _y3, _z3);

	_LEAVE_PARSE();
}

bool _Parser::BGOFF_()
{
	_ENTER_PARSE(1);

	int _tdi = IGet();
	if (_tdi < 0)
	{
		BGLayer::KillOtherLayer();
	}
	else
	{
		BGLayer::ubg[_tdi].exist = false;
	}

	_LEAVE_PARSE();
}

bool _Parser::BGBLEND_()
{
	_ENTER_PARSE(2);

	int _tdi = IGet();
	int _blend = IGet();
	BGLayer::ubg[_tdi].SetBlend(_blend);

	_LEAVE_PARSE();
}

bool _Parser::BGSETUP_()
{
	_ENTER_PARSE(4);

	int _setID = IGet();
	int _sID = IGet();
	bool _bforce = (bool)IGet();
	int _quittime = IGet();

	_Parser * _psaved = NULL;
	vector<Script> * psaved = NULL;
	int * idescsaved = NULL;
	TData * descsaved = NULL;
	if (!PushScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}
	BGLayer::BGLayerSetup(_setID, _sID, _bforce, _quittime);
	if (!PopScript(&_psaved, &psaved, &idescsaved, &descsaved))
	{
		return false;
	}

	_LEAVE_PARSE();
}


bool _Parser::SELBUILD_()
{
	_ENTER_PARSE(16);

	BYTE _ID = IGet();
	int _siid = IGet();
	float _cenx = FGet();
	float _ceny = FGet();
	float _hscale = FGet();
	float _vscale = FGet();
	BYTE _maxtime = IGet();
	float _xadj0 = FGet();
	float _yadj0 = FGet();
	float _xadj1 = FGet();
	float _yadj1 = FGet();
	float _xadj2 = FGet();
	float _yadj2 = FGet();
	float _xadj3 = FGet();
	float _yadj3 = FGet();
	BYTE _flag = IGet();
	Selector::Build(_ID, _siid, _cenx, _ceny, _hscale, _vscale, _maxtime, _xadj0, _yadj0, _xadj1, _yadj1, _xadj2, _yadj2, _xadj3, _yadj3, _flag);

	_LEAVE_PARSE();
}

bool _Parser::SELCLEAR_()
{
	_ENTER_PARSE(0);

	Selector::Clear();

	_LEAVE_PARSE();
}

bool _Parser::SELCONFIRM_()
{
	_ENTER_PARSE(4);

	float _cenx = FGet();
	float _ceny = FGet();
	bool _settrue = (bool)IGet();
	int _tdi = IGet();
	bool _bret = Selector::confirm(_cenx, _ceny, _settrue);
	SetInt(_tdi, (int)_bret);

	_LEAVE_PARSE();
}

bool _Parser::SELSET_()
{
	_ENTER_PARSE(7);

	int _nselect = IGet();
	int _select = IGet();
	bool _updown = (bool)IGet();
	BYTE _nPageNum = IGet();
	float _fadebegin = FGet();
	float _offset = FGet();
	int _initshift = IGet();
	Selector::Setup(_nselect, _select, _updown);
	Selector::SetPageNum(_nPageNum, _fadebegin, _offset, _initshift);

	_LEAVE_PARSE();
}

bool _Parser::SELFLAG_()
{
	_ENTER_PARSE(2);

	int _index = IGet();
	BYTE _flag = IGet();
	Selector * _sel = Selector::GetPointer(_index);
	if (_sel)
	{
		_sel->flag = _flag;
	}

	_LEAVE_PARSE();
}

bool _Parser::ISELBUILD_()
{
	_ENTER_PARSE(6);

	BYTE _ID = IGet();
	char * _info = SSpGet();
	float _x = FGet();
	float _y = FGet();
	BYTE _coltype = IGet();
	BYTE _flag = IGet();
	InfoSelect::Build(_ID, _info, _x, _y, _coltype, _flag);

	_LEAVE_PARSE();
}

bool _Parser::ISELCLEAR_()
{
	_ENTER_PARSE(0);

	InfoSelect::Clear();

	_LEAVE_PARSE();
}

bool _Parser::ISELSET_()
{
	_ENTER_PARSE(7);

	int _nselect = IGet();
	int _select = IGet();
	bool _updown = (bool)IGet();
	BYTE _nPageNum = IGet();
	float _fadebegin = FGet();
	float _offset = FGet();
	int _initshift = IGet();
	InfoSelect::Setup(_nselect, _select, _updown);
	InfoSelect::SetPageNum(_nPageNum, _fadebegin, _offset, _initshift);

	_LEAVE_PARSE();
}

bool _Parser::ISELFLAG_()
{
	_ENTER_PARSE(2);

	int _index = IGet();
	BYTE _flag = IGet();
	InfoSelect * _ifs = InfoSelect::GetPointer(_index);
	if (_ifs)
	{
		_ifs->flag = _flag;
	}

	_LEAVE_PARSE();
}

bool _Parser::ISELCOLOR_()
{
	_ENTER_PARSE(2);

	int _index = IGet();
	BYTE _coltype = IGet();
	InfoSelect * _ifs = InfoSelect::GetPointer(_index);
	if (_ifs)
	{
		_ifs->coltype = _coltype;
	}

	_LEAVE_PARSE();
}

bool _Parser::CHATON_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::CHATOFF_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::DATAGET_()
{
	_ENTER_PARSE(4);

	DWORD _section = UGet();
	DWORD _name = UGet();
	int _defval = IGet();
	int _tdi = IGet();
	int _iret = Data::data.iRead(DATA_BINFILE, _section, _name, _defval);
	SetInt(_tdi, _iret);

	_LEAVE_PARSE();
}

bool _Parser::DATAGETf_()
{
	_ENTER_PARSE(4);

	DWORD _section = UGet();
	DWORD _name = UGet();
	float _defval = FGet();
	int _tdi = IGet();
	float _fret = Data::data.fRead(DATA_BINFILE, _section, _name, _defval);
	SetInt(_tdi, _fret);

	_LEAVE_PARSE();
}

bool _Parser::DATASET_()
{
	_ENTER_PARSE(3);

	DWORD _section = UGet();
	DWORD _name = UGet();
	int _val = IGet();
	Data::data.iWrite(DATA_BINFILE, _section, _name, _val);

	_LEAVE_PARSE();
}

bool _Parser::DATASETf_()
{
	_ENTER_PARSE(3);

	DWORD _section = UGet();
	DWORD _name = UGet();
	float _val = FGet();
	Data::data.fWrite(DATA_BINFILE, _section, _name, _val);

	_LEAVE_PARSE();
}

bool _Parser::SETFLAG_()
{
	_ENTER_PARSE(2);

	DWORD _name = UGet();
	int _val = IGet();
	Data::data.iWrite(DATA_BINFILE, DATAS_FLAG, _name, _val);

	_LEAVE_PARSE();
}

bool _Parser::TRYSTAGE_()
{
	_ENTER_PARSE(0);

	DataConnector::Try();

	_LEAVE_PARSE();
}

bool _Parser::DEBUG_BREAKPOINT_()
{
	_ENTER_PARSE(0);

#ifdef __DEBUG
	Scripter::scr.LogOut();
#endif

	_LEAVE_PARSE();
}