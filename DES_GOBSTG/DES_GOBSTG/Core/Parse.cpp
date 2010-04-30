#include "../Header/Parser.h"
#include "../Header/Scripter.h"
#include "../Header/ScripterDefines.h"
#include "../Header/keytable.h"
#include "../Header/processPrep.h"
#include "../Header/BulletListActionConst.h"

bool _Parser::rv=false;
vector<Script>::iterator * _Parser::it=NULL;
int _Parser::varcount = 0;

_Parser::_Parser()
{

}

_Parser::~_Parser()
{

}

void _Parser::Init(vector<Script>::iterator * _it, int _varcount)
{
	rv = false;
	it = _it;
	varcount = _varcount;
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

float _Parser::GetFloat(int i)
{
	return (float)CAST(Scripter::scr.d[i]);
}

int _Parser::GetInt(int i)
{
	return (int)CAST(Scripter::scr.d[i]);
}

DWORD _Parser::GetUInt(int i)
{
	return UCAST(Scripter::scr.d[i]);
}

bool _Parser::PushScript()
{
	return Scripter::scr.PushScript(varcount);
}

bool _Parser::PopScript()
{
	return Scripter::scr.PopScript(varcount);
}

#define _ENTER_PARSE(X)	rv = (X) ? Scripter::scr.Copy(it, (X)) : true;	\
						if (!rv)										\
						{												\
							return false;								\
						}
#define _LEAVE_PARSE(X)	return rv;

bool _Parser::SD_()
{
	_ENTER_PARSE(2);

	if (IsFloat(1))
	{
		FloatToInt(1);
	}
	int _i = GetInt(0);
	int _ival = GetInt(1);
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
	int _i = GetInt(0);
	float _fval = GetFloat(1);
	SetFloat(_i, _fval);

	_LEAVE_PARSE();
}

bool _Parser::ST_()
{
	_ENTER_PARSE(3);

	int _tdi = GetInt(0);
	float _fx = GetFloat(1);
	float _fy = GetFloat(2);
	Target::SetValue(_tdi, _fx, _fy);

	_LEAVE_PARSE();
}

bool _Parser::CALL_()
{
	_ENTER_PARSE(2);

	if (!PushScript())
	{
		return false;
	}
	DWORD _con = GetUInt(0);
	DWORD _name = GetUInt(1);
	Scripter::scr.functionExecute(_con, _name);
	if (!PopScript())
	{
		return false;
	}

	_LEAVE_PARSE();
}

bool _Parser::CALLEX_()
{
	_ENTER_PARSE(3);

	if (!PushScript())
	{
		return false;
	}
	int _tdi = GetUInt(2);
	if (_tdi)
	{
		rv = Scripter::scr.Copy(it, _tdi, 2);
	}
	DWORD _con = GetUInt(0);
	DWORD _name = GetUInt(1);
	Scripter::scr.functionExecute(_con, _name);
	if (!PopScript())
	{
		return false;
	}

	_LEAVE_PARSE();
}

bool _Parser::EXECUTE_()
{
	_ENTER_PARSE(3);

	if (!PushScript())
	{
		return false;
	}
	DWORD _typeflag = GetUInt(0) + SCRIPT_CTOFFSET;
	DWORD _con = GetUInt(1);
	DWORD _name = GetUInt(2);
	Scripter::scr.Execute(_typeflag, _con, _name);
	if (!PopScript())
	{
		return false;
	}

	_LEAVE_PARSE();
}


bool _Parser::RETURN_()
{
	_ENTER_PARSE(1);

	int _retvalue = GetInt(0);
	Process::mp.retvalue = _retvalue;

	_LEAVE_PARSE();
}

bool _Parser::SETSTATE_()
{
	_ENTER_PARSE(1);

	int _state = GetInt(0);
	Process::mp.state = _state;

	_LEAVE_PARSE();
}

bool _Parser::SETTIME_()
{
	_ENTER_PARSE(1);

	int _gametime = GetInt(0);
	gametime = _gametime;

	_LEAVE_PARSE();
}

bool _Parser::SETDIFFI_()
{
	_ENTER_PARSE(1);

	int _nowdifflv = GetInt(0);
	Process::mp.SetDiffLv(_nowdifflv);

	_LEAVE_PARSE();
}

bool _Parser::SETCHARA_()
{
	_ENTER_PARSE(3);

	WORD _ID = GetInt(0);
	WORD _ID_sub_1 = GetInt(1);
	WORD _ID_sub_2 = GetInt(2);
	Process::mp.SetChara(_ID, _ID_sub_1, _ID_sub_2);
	Player::p.SetID(_ID, _ID_sub_1, _ID_sub_2);

	_LEAVE_PARSE();
}

bool _Parser::SETSCENE_()
{
	_ENTER_PARSE(1);

	int _scene = GetInt(0);
	Process::mp.scene = _scene;

	_LEAVE_PARSE();
}

bool _Parser::SETMODE_()
{
	_ENTER_PARSE(1);

	int _mode = GetInt(0);
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

	int _key = GetInt(0);
	bool _bset = GetInt(1);
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

	BYTE _ID = GetInt(0);
	int _pushkey_1 = GetInt(1);
	int _pushkey_2 = GetInt(2);
	int _pushkey_3 = GetInt(3);
	int _pushkey_4 = GetInt(4);
	int _pushfirst = GetInt(5);
	int _pushrollto = GetInt(6);
	PushKey::SetPushEvent(_ID, _pushkey_1, _pushkey_2, _pushkey_3, _pushkey_4, _pushfirst, _pushrollto);
	_LEAVE_PARSE();
}

bool _Parser::UPDATEPUSHEVENT_()
{
	_ENTER_PARSE(1);

	BYTE _ID = GetInt(0);
	PushKey::UpdatePushEvent(_ID);

	_LEAVE_PARSE();
}

bool _Parser::STOPACTION_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETFRAMESKIP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::MUSICCHANGE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::MUSICSLIDE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SEOFF_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::HSVTORGB_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETSHAKE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::B_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::A_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BC_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUDECANCEL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::IZEZONE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEB_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEVECTOR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEHOLD_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEPIN_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::GB_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHSAIM_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHSET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHCHASE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHSETLIFE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHCHANGE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::EB_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EA_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENSAIM_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENCHASE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENSETLIFE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENCHANGE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DAMAGEZONE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BOSSSTORE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BOSSATTACK_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::RAMA_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DIST_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::CHASEAIM_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::INTER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::BOSSUP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SPELLUP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ACONTROL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BCONTROL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::COLLISION_CIRCLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::COLLISION_SQUARE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::LOADTEXTURESET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::FREETEXTURESET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ITEMBUILD_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::EFFSETUP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EFFSETUPEX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EFFSETUPCHASE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EFFSTOP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EFFOFF_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EFFSET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::EFFMOVETO_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::SETPPOS_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPLIFE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPPOWER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPFAITH_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPPOINT_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPBBORDER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPBSLOW_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPBINFI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPSLOWSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETPSPEEDFACTOR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::COLLAPSE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SHOOTPB_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BONUSFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::BGVALUE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGVALEX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGTEXRECT_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGRECT_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGZ_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGSCALE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGCOLOR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGMOVE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGPARAL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BG4V_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGOFF_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGBLEND_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGSETUP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::SELBUILD_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SELCLEAR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SELCONFIRM_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SELSET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SELFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELBUILD_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELCLEAR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELSET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELCOLOR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::IF_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ELSE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ELSEIF_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::LOOP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SKIP_()
{
	_ENTER_PARSE(0);
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
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DATAGETf_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DATASET_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DATASETf_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SETFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::TRYSTAGE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DEBUG_BREAKPOINT_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::BUI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUANGLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUat_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUCANCELABLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BUHAVEGRAY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEANGLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BESPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEat_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEHOLDTAR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BEPINTAR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::GHX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHat_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHANGLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHAMAP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHRMAP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHAIMX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHAIMY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHLIFE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GHAC_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::ENX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENat_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENANGLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENAMAP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENRMAP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENAIMX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENAIMY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ENLIFE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::RAND_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::RANDR_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SINA_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::COSA_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::TANA_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ASIN2_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ACOS2_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ATAN2_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SQRT_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SIGN_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ROLL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::D_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::Du_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::TX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::TY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::TIME_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::NOWsharp_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::NOWat_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::DIFFI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SNOSTAGE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SNODIFFI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SNOBATTLE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SNOUSER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::CHARA_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GETSCENE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::MODE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::REPLAYMODE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::FRAMESKIP_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::CHATI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BOSSFAILED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BOSSFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GETPEID_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::CHECKKEY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::GETFLAG_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PLAYERNAME_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BOSSNAME_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SPELLNAME_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SPELLUSERNAME_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SPELLUSERENAME_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::BGSat_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::BGSI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::SELCOMPLETE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SEL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::SELFIRSTID_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELCOMPLETE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISEL_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::ISELFIRSTID_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}


bool _Parser::PX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PLIFE_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PBOMB_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PPOWER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PFAITH_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PPOINT_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PBBORDER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PBSLOW_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PBINFI_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PSLOWSPEED_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PGX_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::PGY_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}

bool _Parser::HAVEPLAYER_()
{
	_ENTER_PARSE(0);
	_LEAVE_PARSE();
}