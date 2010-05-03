#include "../Header/Parser.h"
#include "../Header/Scripter.h"
#include "../Header/ScripterDefines.h"
#include "../Header/keytable.h"
#include "../Header/processPrep.h"
#include "../Header/BulletListActionConst.h"

_ParserFunction _ParserFunction::_p;

_ParserFunction::_ParserFunction()
{
}

_ParserFunction::~_ParserFunction()
{
}

void _ParserFunction::Init(vector<Script>::iterator * pscript, int index)
{
	_p.pscript = pscript;
	_p.index = index;
}

void _ParserFunction::SetFloat(float fval)
{
	_Parser::SetFloat(_p.index, fval);
}

void _ParserFunction::SetInt(int ival)
{
	_Parser::SetInt(_p.index, ival);
}

void _ParserFunction::SetUInt(DWORD uval)
{
	_Parser::SetUInt(_p.index, uval);
}

void _ParserFunction::SetString(char * sval)
{
	_Parser::SetUInt(_p.index, (DWORD)sval, true);
}

float _ParserFunction::FGet()
{
	++(*(_p.pscript));
	float _fret = CFLOAT(Scripter::scr.Value(_p.pscript, _p.index));
	return _fret;
}

int _ParserFunction::IGet()
{
	++(*(_p.pscript));
	int _iret = CINT(Scripter::scr.Value(_p.pscript, _p.index));
	return _iret;
}

DWORD _ParserFunction::UGet()
{
	++(*(_p.pscript));
	DWORD _dret = CUINT(Scripter::scr.Value(_p.pscript, _p.index));
	return _dret;
}

#define _ENTER_PARSE(X)	
#define _LEAVE_PARSE(X) return true;

bool _ParserFunction::BUI_()
{
	_ENTER_PARSE(0);

	int _index = Bullet::index;
	SetInt(_index);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUANGLE_()
{
	_ENTER_PARSE(0);

	int _angle = Bullet::bu[Bullet::index].angle;
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUSPEED_()
{
	_ENTER_PARSE(0);

	float _speed = Bullet::bu[Bullet::index].speed;
	SetFloat(_speed);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUX_()
{
	_ENTER_PARSE(0);

	float _x = Bullet::bu[Bullet::index].x;
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUY_()
{
	_ENTER_PARSE(0);

	float _y = Bullet::bu[Bullet::index].y;
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUat_()
{
	_ENTER_PARSE(0);

	DWORD _timer = Bullet::bu[Bullet::index].timer;
	SetUInt(_timer);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUCANCELABLE_()
{
	_ENTER_PARSE(0);

	bool _cancelable = Bullet::bu[Bullet::index].cancelable;
	SetInt((int)_cancelable);

	_LEAVE_PARSE();
}

bool _ParserFunction::BUHAVEGRAY_()
{
	_ENTER_PARSE(0);

	bool _havegray = Bullet::bu[Bullet::index].HaveGray();
	SetInt((int)_havegray);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEI_()
{
	_ENTER_PARSE(0);

	int _index = Beam::index;
	SetInt(_index);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEANGLE_()
{
	_ENTER_PARSE(0);

	int _angle = Beam::be[Beam::index].angle;
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::BESPEED_()
{
	_ENTER_PARSE(0);

	float _speed = Beam::be[Beam::index].speed;
	SetFloat(_speed);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEX_()
{
	_ENTER_PARSE(0);

	float _x = Beam::be[Beam::index].x;
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEY_()
{
	_ENTER_PARSE(0);

	float _y = Beam::be[Beam::index].y;
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEat_()
{
	_ENTER_PARSE(0);

	DWORD _timer = Beam::be[Beam::index].timer;
	SetUInt(_timer);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEHOLDTAR_()
{
	_ENTER_PARSE(0);

	BYTE _holdtar = Beam::be[Beam::index].holdtar;
	SetInt(_holdtar);

	_LEAVE_PARSE();
}

bool _ParserFunction::BEPINTAR_()
{
	_ENTER_PARSE(0);

	BYTE _pintar = Beam::be[Beam::index].pintar;
	SetInt(_pintar);

	_LEAVE_PARSE();
}


bool _ParserFunction::GHX_()
{
	_ENTER_PARSE(0);

	float _x = Ghost::gh[Ghost::index].x;
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHY_()
{
	_ENTER_PARSE(0);

	float _y = Ghost::gh[Ghost::index].y;
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHat_()
{
	_ENTER_PARSE(0);

	DWORD _timer = Ghost::gh[Ghost::index].timer;
	SetUInt(_timer);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHI_()
{
	_ENTER_PARSE(0);

	int _index = Ghost::index;
	SetInt(_index);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHANGLE_()
{
	_ENTER_PARSE(0);

	int _angle = Ghost::gh[Ghost::index].angle;
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHSPEED_()
{
	_ENTER_PARSE(0);

	float _speed = Ghost::gh[Ghost::index].speed;
	SetFloat(_speed);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHAMAP_()
{
	_ENTER_PARSE(0);

	int _a = IGet();
	int _angle = Ghost::gh[Ghost::index].aMainAngle(Player::p.x, Player::p.y, _a);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHRMAP_()
{
	_ENTER_PARSE(0);

	float _r = FGet();
	int _angle = Ghost::gh[Ghost::index].rMainAngle(Player::p.x, Player::p.y, _r);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHAIMX_()
{
	_ENTER_PARSE(0);

	float _aimx = Ghost::gh[Ghost::index].aim.x;
	SetFloat(_aimx);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHAIMY_()
{
	_ENTER_PARSE(0);

	float _aimy = Ghost::gh[Ghost::index].aim.y;
	SetFloat(_aimy);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHLIFE_()
{
	_ENTER_PARSE(0);

	float _life = Ghost::gh[Ghost::index].life;
	SetFloat(_life);

	_LEAVE_PARSE();
}

bool _ParserFunction::GHAC_()
{
	_ENTER_PARSE(0);

	int _ac = Ghost::gh[Ghost::index].ac;
	SetInt(_ac);

	_LEAVE_PARSE();
}


bool _ParserFunction::ENX_()
{
	_ENTER_PARSE(0);

	float _x = Enemy::en[Enemy::index].x;
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENY_()
{
	_ENTER_PARSE(0);

	float _y = Enemy::en[Enemy::index].y;
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENat_()
{
	_ENTER_PARSE(0);

	DWORD _timer = Enemy::en[Enemy::index].timer;
	SetUInt(_timer);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENI_()
{
	_ENTER_PARSE(0);

	int _index = Enemy::index;
	SetInt(_index);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENANGLE_()
{
	_ENTER_PARSE(0);

	int _angle = Enemy::en[Enemy::index].angle;
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENSPEED_()
{
	_ENTER_PARSE(0);

	float _speed = Enemy::en[Enemy::index].speed;
	SetFloat(_speed);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENAMAP_()
{
	_ENTER_PARSE(0);

	int _a = IGet();
	int _angle = Enemy::en[Enemy::index].aMainAngle(Player::p.x, Player::p.y, _a);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENRMAP_()
{
	_ENTER_PARSE(0);

	float _r = FGet();
	int _angle = Enemy::en[Enemy::index].rMainAngle(Player::p.x, Player::p.y, _r);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENAIMX_()
{
	_ENTER_PARSE(0);

	float _aimx = Enemy::en[Enemy::index].aim.x;
	SetFloat(_aimx);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENAIMY_()
{
	_ENTER_PARSE(0);

	float _aimy = Enemy::en[Enemy::index].aim.y;
	SetFloat(_aimy);

	_LEAVE_PARSE();
}

bool _ParserFunction::ENLIFE_()
{
	_ENTER_PARSE(0);

	float _life = Enemy::en[Enemy::index].life;
	SetFloat(_life);

	_LEAVE_PARSE();
}


bool _ParserFunction::RAND_()
{
	_ENTER_PARSE(0);

	int _rand = randt();
	SetInt(_rand);

	_LEAVE_PARSE();
}

bool _ParserFunction::RANDR_()
{
	_ENTER_PARSE(0);

	int _range = IGet();
	int _rand = randt(-_range, _range);
	SetInt(_rand);

	_LEAVE_PARSE();
}

bool _ParserFunction::SEED_()
{
	_ENTER_PARSE(0);

	int _seed = Process::mp.seed;
	SetInt(_seed);

	_LEAVE_PARSE();
}

bool _ParserFunction::SINA_()
{
	_ENTER_PARSE(0);

	int _angle = IGet();
	float _fsina = sint(_angle);
	SetFloat(_fsina);

	_LEAVE_PARSE();
}

bool _ParserFunction::COSA_()
{
	_ENTER_PARSE(0);

	int _angle = IGet();
	float _fcosa = cost(_angle);
	SetFloat(_fcosa);

	_LEAVE_PARSE();
}

bool _ParserFunction::TANA_()
{
	_ENTER_PARSE(0);

	int _angle = IGet();
	float _ftana = sint(_angle);
	SetFloat(_ftana);

	_LEAVE_PARSE();
}

bool _ParserFunction::ASIN2_()
{
	_ENTER_PARSE(0);

	float _x = FGet();
	float _y = FGet();
	int _angle = asin2t(_x, _y);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::ACOS2_()
{
	_ENTER_PARSE(0);

	float _x = FGet();
	float _y = FGet();
	int _angle = acos2t(_x, _y);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::ATAN2_()
{
	_ENTER_PARSE(0);

	float _x = FGet();
	float _y = FGet();
	int _angle = atan2t(_x, _y);
	SetInt(_angle);

	_LEAVE_PARSE();
}

bool _ParserFunction::SQRT_()
{
	_ENTER_PARSE(0);

	float _x = FGet();
	float _fsqrt = sqrtf(_x);
	SetFloat(_fsqrt);

	_LEAVE_PARSE();
}

bool _ParserFunction::SIGN_()
{
	_ENTER_PARSE(0);

	int _x = IGet();
	int _sign = SIGN(_x);
	SetFloat(_sign);

	_LEAVE_PARSE();
}

bool _ParserFunction::ROLL_()
{
	_ENTER_PARSE(0);

	int _x = IGet();
	int _t = IGet();
	int _roll = ROLL(_x, _t);
	SetFloat(_roll);

	_LEAVE_PARSE();
}

bool _ParserFunction::D_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	if (_Parser::IsFloat(_tdi))
	{
		float _fval = _Parser::GetFloat(_tdi);
		SetFloat(_fval);
	}
	else
	{
		int _ival = _Parser::GetInt(_tdi);
		SetInt(_ival);
	}

	_LEAVE_PARSE();
}

bool _ParserFunction::Du_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	DWORD _dval = _Parser::GetUInt(_tdi);
	SetUInt(_dval);

	_LEAVE_PARSE();
}

bool _ParserFunction::TX_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	float _x = Target::GetX(_tdi);
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::TY_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	float _y = Target::GetY(_tdi);
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::TIME_()
{
	_ENTER_PARSE(0);

	int _time = gametime;
	SetInt(_time);

	_LEAVE_PARSE();
}

bool _ParserFunction::NOWsharp_()
{
	_ENTER_PARSE(0);

	DWORD _nowname = Scripter::scr.nowName;
	SetUInt(_nowname);

	_LEAVE_PARSE();
}

bool _ParserFunction::NOWat_()
{
	_ENTER_PARSE(0);

	DWORD _nowcon = Scripter::scr.nowCon;
	SetUInt(_nowcon);

	_LEAVE_PARSE();
}

bool _ParserFunction::DIFFI_()
{
	_ENTER_PARSE(0);

	BYTE _nowdifflv = Process::mp.nowdifflv;
	SetInt(_nowdifflv);

	_LEAVE_PARSE();
}

bool _ParserFunction::SNOSTAGE_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	int _stage = Data::data.getStage(_sno);
	SetInt(_stage);

	_LEAVE_PARSE();
}

bool _ParserFunction::SNODIFFI_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	int _diffi = Data::data.getDiffi(_sno);
	SetInt(_diffi);

	_LEAVE_PARSE();
}

bool _ParserFunction::SNOBATTLE_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	int _battle = Data::data.getBattle(_sno);
	SetInt(_battle);

	_LEAVE_PARSE();
}

bool _ParserFunction::SNOUSER_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	int _spelluser = Data::data.getSpellUser(_sno);
	SetInt(_spelluser);

	_LEAVE_PARSE();
}

bool _ParserFunction::CHARA_()
{
	_ENTER_PARSE(0);

	int _index = IGet();
	WORD _chara = Player::p.GetChara(_index);
	SetInt(_chara);

	_LEAVE_PARSE();
}

bool _ParserFunction::GETSCENE_()
{
	_ENTER_PARSE(0);

	DWORD _scene = Process::mp.scene;
	SetUInt(_scene);

	_LEAVE_PARSE();
}

bool _ParserFunction::MODE_()
{
	_ENTER_PARSE(0);

	int _iret = M_MODE_NORMAL;
	if (Process::mp.practicemode)
	{
		if (Process::mp.spellmode)
		{
			_iret = M_MODE_SPELL;
		}
		else
		{
			_iret = M_MODE_STAGE;
		}
	}
	SetInt(_iret);

	_LEAVE_PARSE();
}

bool _ParserFunction::REPLAYMODE_()
{
	_ENTER_PARSE(0);

	bool _replaymode = Process::mp.replaymode;
	SetInt((int)_replaymode);

	_LEAVE_PARSE();
}

bool _ParserFunction::FRAMESKIP_()
{
	_ENTER_PARSE(0);

	int _frameskip = Process::mp.frameskip;
	SetInt(_frameskip);

	_LEAVE_PARSE();
}


bool _ParserFunction::CHATI_()
{
	_ENTER_PARSE(0);

	int _chati = Chat::chatitem.chati;
	SetInt(_chati);

	_LEAVE_PARSE();
}

bool _ParserFunction::BOSSFAILED_()
{
	_ENTER_PARSE(0);

	bool _bossfailed = BossInfo::bossinfo.failed;
	SetInt((int)_bossfailed);

	_LEAVE_PARSE();
}

bool _ParserFunction::BOSSFLAG_()
{
	_ENTER_PARSE(0);

	BYTE _bossflag = BossInfo::bossinfo.flag;
	SetInt(_bossflag);

	_LEAVE_PARSE();
}

bool _ParserFunction::GETPEID_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	int _iret = Player::p.nowID;
	if (_tdi >= 0)
	{
		_iret = BResource::bres.playerdata[_tdi].eid;
	}
	SetInt(_iret);

	_LEAVE_PARSE();
}

bool _ParserFunction::CHECKKEY_()
{
	_ENTER_PARSE(0);

	int _key = IGet();
	BYTE _state = IGet();
	int _iret = hge->Input_GetDIKey(_key, _state);
	SetInt(_iret);

	_LEAVE_PARSE();
}

bool _ParserFunction::GETFLAG_()
{
	_ENTER_PARSE(0);

	int _name = IGet();
	int _flag = Data::data.iRead(DATA_BINFILE, DATAS_FLAG, _name, 0);
	SetInt(_flag);

	_LEAVE_PARSE();
}

bool _ParserFunction::PLAYERNAME_()
{
	_ENTER_PARSE(0);

	int _chara = IGet();
	char * _name = Data::data.getPlayerName(_chara);
	SetString(_name);

	_LEAVE_PARSE();
}

bool _ParserFunction::BOSSNAME_()
{
	_ENTER_PARSE(0);

	int _chara = IGet();
	char * _name = Data::data.getEnemyName(_chara);
	SetString(_name);

	_LEAVE_PARSE();
}

bool _ParserFunction::SPELLNAME_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	char * _name = Data::data.getSpellName(_sno);
	SetString(_name);

	_LEAVE_PARSE();
}

bool _ParserFunction::SPELLUSERNAME_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	char * _name = Data::data.getSpellUserName(_sno);
	SetString(_name);

	_LEAVE_PARSE();
}

bool _ParserFunction::SPELLUSERENAME_()
{
	_ENTER_PARSE(0);

	int _sno = IGet();
	char * _name = Data::data.getSpellUserEName(_sno);
	SetString(_name);

	_LEAVE_PARSE();
}


bool _ParserFunction::PX_()
{
	_ENTER_PARSE(0);

	float _x = Player::p.x;
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::PY_()
{
	_ENTER_PARSE(0);

	float _y = Player::p.y;
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::PLIFE_()
{
	_ENTER_PARSE(0);

	BYTE _life = Player::p.nLife;
	SetInt(_life);

	_LEAVE_PARSE();
}

bool _ParserFunction::PBOMB_()
{
	_ENTER_PARSE(0);

	int _bomb = Player::p.nPower / (float)Player::p.bombperpower;
	SetInt(_bomb);

	_LEAVE_PARSE();
}

bool _ParserFunction::PPOWER_()
{
	_ENTER_PARSE(0);

	DWORD _power = Player::p.nPower;
	SetUInt(_power);

	_LEAVE_PARSE();
}

bool _ParserFunction::PFAITH_()
{
	_ENTER_PARSE(0);

	DWORD _faith = Player::p.nFaith;
	SetUInt(_faith);

	_LEAVE_PARSE();
}

bool _ParserFunction::PPOINT_()
{
	_ENTER_PARSE(0);

	DWORD _point = Player::p.nPoint;
	SetUInt(_point);

	_LEAVE_PARSE();
}

bool _ParserFunction::PBBORDER_()
{
	_ENTER_PARSE(0);

	bool _bborder = Player::p.bBorder;
	SetInt((int)_bborder);

	_LEAVE_PARSE();
}

bool _ParserFunction::PBSLOW_()
{
	_ENTER_PARSE(0);

	bool _bslow = Player::p.bSlow;
	SetInt((int)_bslow);

	_LEAVE_PARSE();
}

bool _ParserFunction::PBINFI_()
{
	_ENTER_PARSE(0);

	bool _binfi = Player::p.bInfi;
	SetInt((int)_binfi);

	_LEAVE_PARSE();
}

bool _ParserFunction::PSPEED_()
{
	_ENTER_PARSE(0);

	float _speed = Player::p.speed;
	SetFloat(_speed);

	_LEAVE_PARSE();
}

bool _ParserFunction::PSLOWSPEED_()
{
	_ENTER_PARSE(0);

	float _slowspeed = Player::p.slowspeed;
	SetFloat(_slowspeed);

	_LEAVE_PARSE();
}

bool _ParserFunction::PGX_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	float _x = Player::p.pg[_tdi].x;
	SetFloat(_x);

	_LEAVE_PARSE();
}

bool _ParserFunction::PGY_()
{
	_ENTER_PARSE(0);

	int _tdi = IGet();
	float _y = Player::p.pg[_tdi].y;
	SetFloat(_y);

	_LEAVE_PARSE();
}

bool _ParserFunction::HAVEPLAYER_()
{
	_ENTER_PARSE(0);

	WORD _chara = IGet();
	bool _haveplayer = Player::p.HavePlayer(_chara);
	SetInt((bool)_haveplayer);


	_LEAVE_PARSE();
}

bool _ParserFunction::BGSI_()
{
	_ENTER_PARSE(0);

	WORD _index = BGLayer::setindex;
	SetInt(_index);

	_LEAVE_PARSE();
}

bool _ParserFunction::BGSat_()
{
	_ENTER_PARSE(0);

	DWORD _timer = BGLayer::bglayerset[BGLayer::setindex].timer;
	SetUInt(_timer);

	_LEAVE_PARSE();
}

bool _ParserFunction::SELCOMPLETE_()
{
	_ENTER_PARSE(0);

	bool _complete = Selector::complete;
	SetInt((int)_complete);

	_LEAVE_PARSE();
}

bool _ParserFunction::SEL_()
{
	_ENTER_PARSE(0);

	int _select = Selector::select;
	SetInt(_select);

	_LEAVE_PARSE();
}

bool _ParserFunction::SELFIRSTID_()
{
	_ENTER_PARSE(0);

	int _firstID = Selector::firstID;
	SetInt(_firstID);

	_LEAVE_PARSE();
}

bool _ParserFunction::ISELCOMPLETE_()
{
	_ENTER_PARSE(0);

	bool _complete = InfoSelect::complete;
	SetInt((int)_complete);

	_LEAVE_PARSE();
}

bool _ParserFunction::ISEL_()
{
	_ENTER_PARSE(0);

	int _select = InfoSelect::select;
	SetInt(_select);

	_LEAVE_PARSE();
}

bool _ParserFunction::ISELFIRSTID_()
{
	_ENTER_PARSE(0);

	int _firstID = InfoSelect::firstID;
	SetInt(_firstID);

	_LEAVE_PARSE();
}

