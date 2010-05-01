#ifndef _PARSER_H
#define _PARSER_H

#include "Scripter.h"

class _Parser
{
public:
	_Parser();
	~_Parser();

	static void Init(vector<Script>::iterator * it, int varcount);

	static bool IsFloat(int i);
	static void FloatToInt(int i);
	static void IntToFloat(int i);

	static void SetFloat(int i, float fval);
	static void SetInt(int i, int ival);
	static void SetUInt(int i, DWORD uval);
	static float FGet(){return GetFloat(-1);};
	static int IGet(){return GetInt(-1);};
	static DWORD UGet(){return GetUInt(-1);};
	static float GetFloat(int i=-1);
	static int GetInt(int i=-1);
	static DWORD GetUInt(int i=-1);

	static bool PushScript();
	static bool PopScript();

	static bool SD_();
	static bool SDf_();
	static bool ST_();
	static bool CALL_();
	static bool CALLEX_();
	static bool EXECUTE_();

	static bool RETURN_();
	static bool SETSTATE_();
	static bool SETTIME_();
	static bool SETDIFFI_();
	static bool SETCHARA_();
	static bool SETSCENE_();
	static bool SETMODE_();
	static bool STARTPREP_();
	static bool SETKEY_();
	static bool DISABLEALLKEY_();
	static bool SETPUSHEVENT_();
	static bool UPDATEPUSHEVENT_();
	static bool STOPACTION_();
	static bool SETFRAMESKIP_();

	static bool MUSICCHANGE_();
	static bool MUSICSLIDE_();
	static bool SE_();
	static bool SEOFF_();
	static bool HSVTORGB_();
	static bool SETSHAKE_();

	static bool B_();
	static bool A_();
	static bool BC_();
	static bool BL_();
	static bool BUDECANCEL_();
	static bool IZEZONE_();
	static bool BEB_();
	static bool BEVECTOR_();
	static bool BEHOLD_();
	static bool BEPIN_();

	static bool GB_();
	static bool GHSAIM_();
	static bool GHSET_();
	static bool GHCHASE_();
	static bool GHSETLIFE_();
	static bool GHCHANGE_();

	static bool EB_();
	static bool EA_();
	static bool ENSAIM_();
	static bool ENCHASE_();
	static bool ENSETLIFE_();
	static bool ENCHANGE_();
	static bool DAMAGEZONE_();
	static bool BOSSSTORE_();
	static bool BOSSATTACK_();

	static bool RAMA_();
	static bool DIST_();
	static bool CHASEAIM_();
	static bool INTER_();

	static bool BOSSUP_();
	static bool SPELLUP_();
	static bool ACONTROL_();
	static bool BCONTROL_();
	static bool COLLISION_CIRCLE_();
	static bool COLLISION_SQUARE_();

	static bool LOADTEXTURESET_();
	static bool FREETEXTURESET_();
	static bool ITEMBUILD_();

	static bool EFFSETUP_();
	static bool EFFSETUPEX_();
	static bool EFFSETUPCHASE_();
	static bool EFFSTOP_();
	static bool EFFOFF_();
	static bool EFFSET_();
	static bool EFFMOVETO_();

	static bool SETPPOS_();
	static bool SETPLIFE_();
	static bool SETPPOWER_();
	static bool SETPFAITH_();
	static bool SETPPOINT_();
	static bool SETPBBORDER_();
	static bool SETPBSLOW_();
	static bool SETPBINFI_();
	static bool SETPSPEED_();
	static bool SETPSLOWSPEED_();
	static bool SETPSPEEDFACTOR_();
	static bool COLLAPSE_();
	static bool SHOOTPB_();
	static bool BONUSFLAG_();

	static bool BGVALUE_();
	static bool BGVALEX_();
	static bool BGTEXRECT_();
	static bool BGRECT_();
	static bool BGZ_();
	static bool BGSCALE_();
	static bool BGCOLOR_();
	static bool BGMOVE_();
	static bool BGFLAG_();
	static bool BGPARAL_();
	static bool BG4V_();
	static bool BGOFF_();
	static bool BGBLEND_();
	static bool BGSETUP_();

	static bool SELBUILD_();
	static bool SELCLEAR_();
	static bool SELCONFIRM_();
	static bool SELSET_();
	static bool SELFLAG_();
	static bool ISELBUILD_();
	static bool ISELCLEAR_();
	static bool ISELSET_();
	static bool ISELFLAG_();
	static bool ISELCOLOR_();

	static bool IF_();
	static bool ELSE_();
	static bool ELSEIF_();

	static bool LOOP_();
	static bool SKIP_();

	static bool CHATON_();
	static bool CHATOFF_();

	static bool DATAGET_();
	static bool DATAGETf_();
	static bool DATASET_();
	static bool DATASETf_();
	static bool SETFLAG_();
	static bool TRYSTAGE_();
	static bool DEBUG_BREAKPOINT_();

	static bool BUI_();
	static bool BUANGLE_();
	static bool BUSPEED_();
	static bool BUX_();
	static bool BUY_();
	static bool BUat_();
	static bool BUCANCELABLE_();
	static bool BUHAVEGRAY_();
	static bool BEI_();
	static bool BEANGLE_();
	static bool BESPEED_();
	static bool BEX_();
	static bool BEY_();
	static bool BEat_();
	static bool BEHOLDTAR_();
	static bool BEPINTAR_();

	static bool GHX_();
	static bool GHY_();
	static bool GHat_();
	static bool GHI_();
	static bool GHANGLE_();
	static bool GHSPEED_();
	static bool GHAMAP_();
	static bool GHRMAP_();
	static bool GHAIMX_();
	static bool GHAIMY_();
	static bool GHLIFE_();
	static bool GHAC_();

	static bool ENX_();
	static bool ENY_();
	static bool ENat_();
	static bool ENI_();
	static bool ENANGLE_();
	static bool ENSPEED_();
	static bool ENAMAP_();
	static bool ENRMAP_();
	static bool ENAIMX_();
	static bool ENAIMY_();
	static bool ENLIFE_();

	static bool RAND_();
	static bool RANDR_();
	static bool SEED_();
	static bool SINA_();
	static bool COSA_();
	static bool TANA_();
	static bool ASIN2_();
	static bool ACOS2_();
	static bool ATAN2_();
	static bool SQRT_();
	static bool SIGN_();
	static bool ROLL_();

	static bool D_();
	static bool Du_();
	static bool TX_();
	static bool TY_();
	static bool TIME_();
	static bool NOWsharp_();
	static bool NOWat_();
	static bool DIFFI_();
	static bool SNOSTAGE_();
	static bool SNODIFFI_();
	static bool SNOBATTLE_();
	static bool SNOUSER_();
	static bool CHARA_();
	static bool GETSCENE_();
	static bool MODE_();
	static bool REPLAYMODE_();
	static bool FRAMESKIP_();

	static bool CHATI_();
	static bool BOSSFAILED_();
	static bool BOSSFLAG_();
	static bool GETPEID_();
	static bool CHECKKEY_();
	static bool GETFLAG_();
	static bool PLAYERNAME_();
	static bool BOSSNAME_();
	static bool SPELLNAME_();
	static bool SPELLUSERNAME_();
	static bool SPELLUSERENAME_();

	static bool BGSat_();
	static bool BGSI_();

	static bool SELCOMPLETE_();
	static bool SEL_();
	static bool SELFIRSTID_();
	static bool ISELCOMPLETE_();
	static bool ISEL_();
	static bool ISELFIRSTID_();

	static bool PX_();
	static bool PY_();
	static bool PLIFE_();
	static bool PBOMB_();
	static bool PPOWER_();
	static bool PFAITH_();
	static bool PPOINT_();
	static bool PBBORDER_();
	static bool PBSLOW_();
	static bool PBINFI_();
	static bool PSPEED_();
	static bool PSLOWSPEED_();
	static bool PGX_();
	static bool PGY_();
	static bool HAVEPLAYER_();

	bool rv;
	vector<Script>::iterator * it;
	int varcount;
	int indexcount;

	static _Parser _p;
	static _Parser _psaved;
};

#endif