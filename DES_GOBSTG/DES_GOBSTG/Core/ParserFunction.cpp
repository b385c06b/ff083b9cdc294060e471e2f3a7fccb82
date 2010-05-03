#include "../Header/Scripter.h"
#include "../Header/processPrep.h"
#include "../Header/keytable.h"
#include "../Header/Parser.h"

void * Scripter::Value(vector<Script>::iterator * p, int i, BYTE force)
{
	if((*p)->type & SCR_TOKEN_VALUE)
	{
		if(force & SCR_FORCE_INT)
		{
			d[i].bfloat = false;
			if((*p)->type & SCR_TOKEN_FLOAT)
			{
				idesc[i] = (int)CFLOAT(&((*p)->value));
			}
			else
			{
				idesc[i] = (int)CINT(&((*p)->value));
			}
		}
		else if(force & SCR_FORCE_FLOAT)
		{
			d[i].bfloat = true;
			if((*p)->type & SCR_TOKEN_FLOAT)
			{
				fdesc[i] = (float)CFLOAT(&((*p)->value));
			}
			else
			{
				fdesc[i] = (float)CINT(&((*p)->value));
			}
		}
		else
		{
			if((*p)->type & SCR_TOKEN_FLOAT)
			{
				d[i].bfloat = true;
				fdesc[i] = CFLOAT(&((*p)->value));
			}
			else
			{
				d[i].bfloat = false;
				idesc[i] = CINT(&((*p)->value));
			}
		}
	}
	else if((*p)->type & SCR_TOKEN_HANDLE)
	{
		d[i].bfloat = false;
		idesc[i] = (*p)->value;
	}
	else if((*p)->type & SCR_TOKEN_VARIABLE)
	{
		int _tdi = (*p)->value;
		d[i].bfloat = d[_tdi].bfloat;
		if(d[i].bfloat)
			fdesc[i] = CFLOAT(d[_tdi].value);
		else
			idesc[i] = CINT(d[_tdi].value);
	}
	else
	{
		_ParserFunction::Init(p, i);

		int _tdi;
		float _tdf;

		DWORD nowval = (*p)->value;
		switch(nowval & SCRKWMASK_CLASS)
		{
			/************************************************************************/
			/* AttackObject                                                         */
			/************************************************************************/
		case SCRKW_CLASS_ATTACKOBJECT:
			switch (nowval)
			{
			case SCR_BUI:
				_ParserFunction::BUI_();
/*
				idesc[i] = Bullet::index;
				d[i].bfloat = false;*/

				break;
			case SCR_BUANGLE:
				_ParserFunction::BUANGLE_();
/*
				idesc[i] = Bullet::bu[Bullet::index].angle;
				d[i].bfloat = false;*/

				break;
			case SCR_BUSPEED:
				_ParserFunction::BUSPEED_();
/*
				fdesc[i] = Bullet::bu[Bullet::index].speed;
				d[i].bfloat = true;*/

				break;
			case SCR_BUX:
				_ParserFunction::BUX_();
/*
				fdesc[i] = Bullet::bu[Bullet::index].x;
				d[i].bfloat = true;*/

				break;
			case SCR_BUY:
				_ParserFunction::BUY_();
/*
				fdesc[i] = Bullet::bu[Bullet::index].y;
				d[i].bfloat = true;*/

				break;
			case SCR_BUTIMER:
				_ParserFunction::BUat_();
/*
				idesc[i] = Bullet::bu[Bullet::index].timer;
				d[i].bfloat = false;*/

				break;
			case SCR_BUCANCELABLE:
				_ParserFunction::BUCANCELABLE_();	
/*
				idesc[i] = Bullet::bu[Bullet::index].cancelable;
				d[i].bfloat = false;*/

				break;
			case SCR_BUHAVEGRAY:
				_ParserFunction::BUHAVEGRAY_();
/*
				idesc[i] = (int)Bullet::bu[Bullet::index].HaveGray();
				d[i].bfloat = false;*/

				break;

			case SCR_BEI:
				_ParserFunction::BEI_();
/*
				idesc[i] = Beam::index;
				d[i].bfloat = false;*/

				break;
			case SCR_BEANGLE:
				_ParserFunction::BEANGLE_();
/*
				idesc[i] = Beam::be[Beam::index].angle;
				d[i].bfloat = false;*/

				break;
			case SCR_BESPEED:
				_ParserFunction::BESPEED_();
/*
				fdesc[i] = Beam::be[Beam::index].speed;
				d[i].bfloat = true;*/

				break;
			case SCR_BEX:
				_ParserFunction::BEX_();
/*
				fdesc[i] = Beam::be[Beam::index].x;
				d[i].bfloat = true;*/

				break;
			case SCR_BEY:
				_ParserFunction::BEY_();
/*
				fdesc[i] = Beam::be[Beam::index].y;
				d[i].bfloat = true;*/

				break;
			case SCR_BETIMER:
				_ParserFunction::BEat_();
/*
				idesc[i] = Beam::be[Beam::index].timer;
				d[i].bfloat = false;*/

				break;
			case SCR_BEHOLDTAR:
				_ParserFunction::BEHOLDTAR_();
/*
				idesc[i] = Beam::be[Beam::index].holdtar;
				d[i].bfloat = false;*/

				break;
			case SCR_BEPINTAR:
				_ParserFunction::BEPINTAR_();
/*
				idesc[i] = Beam::be[Beam::index].pintar;
				d[i].bfloat = false;*/

				break;
			}
			break;
			/************************************************************************/
			/* Ghost                                                                */
			/************************************************************************/
		case SCRKW_CLASS_GHOST:
			switch (nowval)
			{
			case SCR_GHX:
				_ParserFunction::GHX_();
/*
				fdesc[i] = Ghost::gh[Ghost::index].x;
				d[i].bfloat = true;*/

				break;
			case SCR_GHY:
				_ParserFunction::GHY_();
/*
				fdesc[i] = Ghost::gh[Ghost::index].y;
				d[i].bfloat = true;*/

				break;
			case SCR_GHTIMER:
				_ParserFunction::GHat_();
/*
				idesc[i] = Ghost::gh[Ghost::index].timer;
				d[i].bfloat = false;*/

				break;
			case SCR_GHI:
				_ParserFunction::GHI_();
/*
				idesc[i] = Ghost::index;
				d[i].bfloat = false;*/

				break;
			case SCR_GHANGLE:
				_ParserFunction::GHANGLE_();
/*
				idesc[i] = Ghost::gh[Ghost::index].angle;
				d[i].bfloat = false;*/

				break;
			case SCR_GHSPEED:
				_ParserFunction::GHSPEED_();
/*
				fdesc[i] = Ghost::gh[Ghost::index].speed;
				d[i].bfloat = true;*/

				break;
			case SCR_GHAMAP:
				_ParserFunction::GHAMAP_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Ghost::gh[Ghost::index].aMainAngle(Player::p.x, Player::p.y, _tdi);
				d[i].bfloat = false;*/

				break;
			case SCR_GHRMAP:
				_ParserFunction::GHRMAP_();
/*
				++(*p);
				_tdf = CFLOAT(Value(&(*p), i, 0));
				idesc[i] = Ghost::gh[Ghost::index].rMainAngle(Player::p.x, Player::p.y, _tdf);
				d[i].bfloat = false;*/

				break;
			case SCR_GHAIMX:
				_ParserFunction::GHAIMX_();
/*
				fdesc[i] = Ghost::gh[Ghost::index].aim.x;
				d[i].bfloat = true;*/

				break;
			case SCR_GHAIMY:
				_ParserFunction::GHAIMY_();
/*
				fdesc[i] = Ghost::gh[Ghost::index].aim.y;
				d[i].bfloat = true;*/

				break;
			case SCR_GHLIFE:
				_ParserFunction::GHLIFE_();
/*
				fdesc[i] = Ghost::gh[Ghost::index].life;
				d[i].bfloat = true;*/

				break;
			case SCR_GHAC:
				_ParserFunction::GHAC_();
/*
				idesc[i] = Ghost::gh[Ghost::index].ac;
				d[i].bfloat = false;*/

				break;
			}
			break;
			/************************************************************************/
			/* Enemy                                                                */
			/************************************************************************/
		case SCRKW_CLASS_ENEMY:
			switch (nowval)
			{
			case SCR_ENX:
				_ParserFunction::ENX_();
/*
				fdesc[i] = Enemy::en[Enemy::index].x;
				d[i].bfloat = true;*/

				break;
			case SCR_ENY:
				_ParserFunction::ENY_();
/*
				fdesc[i] = Enemy::en[Enemy::index].y;
				d[i].bfloat = true;*/

				break;
			case SCR_ENTIMER:
				_ParserFunction::ENat_();
/*
				idesc[i] = Enemy::en[Enemy::index].timer;
				d[i].bfloat = false;*/

				break;
			case SCR_ENI:
				_ParserFunction::ENI_();
/*
				idesc[i] = Enemy::index;
				d[i].bfloat = false;*/

				break;
			case SCR_ENANGLE:
				_ParserFunction::ENANGLE_();
/*
				idesc[i] = Enemy::en[Enemy::index].angle;
				d[i].bfloat = false;*/

				break;
			case SCR_ENSPEED:
				_ParserFunction::ENSPEED_();
/*
				fdesc[i] = Enemy::en[Enemy::index].speed;
				d[i].bfloat = true;*/

				break;
			case SCR_ENAMAP:
				_ParserFunction::ENAMAP_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Enemy::en[Enemy::index].aMainAngle(Player::p.x, Player::p.y, _tdi);
				d[i].bfloat = false;*/

				break;
			case SCR_ENRMAP:
				_ParserFunction::ENRMAP_();
/*
				++(*p);
				_tdf = CFLOAT(Value(&(*p), i, 0));
				idesc[i] = Enemy::en[Enemy::index].rMainAngle(Player::p.x, Player::p.y, _tdf);
				d[i].bfloat = false;*/

				break;
			case SCR_ENAIMX:
				_ParserFunction::ENAIMX_();
/*
				fdesc[i] = Enemy::en[Enemy::index].aim.x;
				d[i].bfloat = true;*/

				break;
			case SCR_ENAIMY:
				_ParserFunction::ENAIMY_();
/*
				fdesc[i] = Enemy::en[Enemy::index].aim.y;
				d[i].bfloat = true;*/

				break;
			case SCR_ENLIFE:
				_ParserFunction::ENLIFE_();
/*
				fdesc[i] = Enemy::en[Enemy::index].life;
				d[i].bfloat = true;*/

				break;
			}
			break;
			/************************************************************************/
			/* Math                                                                 */
			/************************************************************************/
		case SCRKW_CLASS_MATH:
			switch (nowval)
			{
			case SCR_RAND:
				_ParserFunction::RAND_();
/*
				idesc[i] = randt();
				d[i].bfloat = false;*/

				break;
			case SCR_RANDR:
				_ParserFunction::RANDR_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = randt() % (_tdi * 2 + 1) - _tdi;
				d[i].bfloat = false;*/

				break;
			case SCR_SEED:
				_ParserFunction::SEED_();
/*
				idesc[i] = Process::mp.seed;
				d[i].bfloat = false;*/

				break;
			case SCR_SINA:
				_ParserFunction::SINA_();
/*
				++(*p);
				fdesc[i] = sint(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = true;*/

				break;
			case SCR_COSA:
				_ParserFunction::COSA_();
/*
				++(*p);
				fdesc[i] = cost(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = true;*/

				break;
			case SCR_TANA:
				_ParserFunction::TANA_();
/*
				++(*p);
				fdesc[i] = tant(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = true;*/

				break;
			case SCR_ASIN2:
				_ParserFunction::ASIN2_();
/*
				++(*p);
				_tdf = CFLOAT((Value(&(*p), i, 0)));
				++(*p);
				idesc[i] = asin2t(_tdf, CFLOAT(Value(&(*p), i, 0)));
				d[i].bfloat = false;*/

				break;
			case SCR_ACOS2:
				_ParserFunction::ACOS2_();
/*
				++(*p);
				_tdf = CFLOAT((Value(&(*p), i, 0)));
				++(*p);
				idesc[i] = acos2t(_tdf, CFLOAT(Value(&(*p), i, 0)));
				d[i].bfloat = false;*/

				break;
			case SCR_ATAN2:
				_ParserFunction::ATAN2_();
/*
				++(*p);
				_tdf = CFLOAT((Value(&(*p), i, 0)));
				++(*p);
				idesc[i] = atan2t(_tdf, CFLOAT(Value(&(*p), i, 0)));
				d[i].bfloat = false;*/

				break;
			case SCR_SQRT:
				_ParserFunction::SQRT_();
/*
				++(*p);
				fdesc[i] = sqrtf(CFLOAT((Value(&(*p), i, 0))));
				d[i].bfloat = true;*/

				break;
			case SCR_SIGN:
				_ParserFunction::SIGN_();
/*
				++(*p);
				idesc[i] = SIGN(CINT(Value(&(*p), i, 0)));
				d[i].bfloat = false;*/

				break;
			case SCR_ROLL:
				_ParserFunction::ROLL_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				++(*p);
				idesc[i] = ROLL(_tdi, CINT(Value(&(*p), i, 0)));
				d[i].bfloat = false;*/

				break;
			}
			break;
			/************************************************************************/
			/* Other                                                                */
			/************************************************************************/
		case SCRKW_CLASS_OTHER:
			switch (nowval)
			{
			case SCR_D:
				_ParserFunction::D_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				if(d[_tdi].bfloat)
					fdesc[i] = CAST(d[_tdi]);
				else
					idesc[i] = CAST(d[_tdi]);
				d[i].bfloat = d[_tdi].bfloat;*/

				break;
			case SCR_DU:
				_ParserFunction::Du_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = UCAST(d[_tdi]);
				d[i].bfloat = false;*/

				break;
			case SCR_TX:
				_ParserFunction::TX_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				fdesc[i] = Target::GetX(_tdi);
				d[i].bfloat = true;*/

				break;
			case SCR_TY:
				_ParserFunction::TY_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				fdesc[i] = Target::GetY(_tdi);
				d[i].bfloat = true;*/

				break;
			case SCR_TIME:
				_ParserFunction::TIME_();
/*
				idesc[i] = gametime;
				d[i].bfloat = false;*/

				break;
			case SCR_NOWNAME:
				_ParserFunction::NOWsharp_();
/*
				idesc[i] = nowName;
				d[i].bfloat = false;*/

				break;
			case SCR_NOWCON:
				_ParserFunction::NOWat_();
/*
				idesc[i] = nowCon;
				d[i].bfloat = false;*/

				break;

			case SCR_DIFFI:
				_ParserFunction::DIFFI_();
/*
				idesc[i] = Process::mp.nowdifflv;
				d[i].bfloat = false;*/

				break;
			case SCR_SNOSTAGE:
				_ParserFunction::SNOSTAGE_();
/*
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Data::data.getStage(_tdi);
				d[i].bfloat = false;*/

				break;
			case SCR_SNODIFFI:
				_ParserFunction::SNODIFFI_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Data::data.getDiffi(_tdi);
				d[i].bfloat = false;*/

				break;
			case SCR_SNOBATTLE:
				_ParserFunction::SNOBATTLE_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Data::data.getBattle(_tdi);
				d[i].bfloat = false;*/

				break;
			case SCR_SNOUSER:
				_ParserFunction::SNOUSER_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Data::data.getSpellUser(_tdi);
				d[i].bfloat = false;*/

				break;
			case SCR_CHARA:
				_ParserFunction::CHARA_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				if (_tdi == 0)
				{
					idesc[i] = Player::p.ID;
				}
				else if (_tdi == 1)
				{
					idesc[i] = Player::p.ID_sub_1;
				}
				else if (_tdi == 2)
				{
					idesc[i] = Player::p.ID_sub_2;
				}
				else
				{
					idesc[i] = Player::p.nowID;
				}
				d[i].bfloat = false;*/

				break;
			case SCR_GETSCENE:
				_ParserFunction::GETSCENE_();
/*
				idesc[i] = Process::mp.scene;
				d[i].bfloat = false;*/

				break;
			case SCR_MODE:
				_ParserFunction::MODE_();
/*
				if (Process::mp.practicemode)
				{
					if (Process::mp.spellmode)
					{
						idesc[i] = M_MODE_SPELL;
					}
					else
					{
						idesc[i] = M_MODE_STAGE;
					}
				}
				else
				{
					idesc[i] = M_MODE_NORMAL;
				}
				d[i].bfloat = false;*/

				break;
			case SCR_REPLAYMODE:
				_ParserFunction::REPLAYMODE_();
/*
				idesc[i] = Process::mp.replaymode;
				d[i].bfloat = false;*/

				break;
			case SCR_FRAMESKIP:
				_ParserFunction::FRAMESKIP_();
/*
				idesc[i] = Process::mp.frameskip;
				d[i].bfloat = false;*/

				break;
			}
			break;
			/************************************************************************/
			/* Special                                                              */
			/************************************************************************/
		case SCRKW_CLASS_SPECIAL:
			switch (nowval)
			{
			case SCR_CHATI:
				_ParserFunction::CHATI_();
/*
				idesc[i] = Chat::chatitem.chati;
				d[i].bfloat = false;*/

				break;

			case SCR_BOSSFAILED:
				_ParserFunction::BOSSFAILED_();
/*
				idesc[i] = BossInfo::bossinfo.failed;
				d[i].bfloat = false;*/

				break;
			case SCR_BOSSFLAG:
				_ParserFunction::BOSSFLAG_();
/*
				idesc[i] = BossInfo::bossinfo.flag;
				d[i].bfloat = false;*/

				break;

			case SCR_GETPEID:
				_ParserFunction::GETPEID_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				if (_tdi < 0)
				{
					_tdi = Player::p.nowID;
				}
				idesc[i] = BResource::res.playerdata[_tdi].eid;
				d[i].bfloat = false;*/

				break;

			case SCR_CHECKKEY:
				_ParserFunction::CHECKKEY_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				++(*p);
				idesc[i] = hge->Input_GetDIKey(_tdi, CINT(Value(&(*p), i, 0)));
				d[i].bfloat = false;*/

				break;

			case SCR_GETFLAG:
				_ParserFunction::GETFLAG_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Data::data.iRead(DATA_BINFILE, DATAS_FLAG, _tdi, 0);
				d[i].bfloat = false;*/

				break;

			case SCR_PLAYERNAME:
				_ParserFunction::PLAYERNAME_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				CUINT(&idesc[i]) = (DWORD)Data::data.getPlayerName(_tdi);
				d[i].bfloat = true;/ *Specialized for Handle* /*/

				break;
			case SCR_BOSSNAME:
				_ParserFunction::BOSSNAME_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				CUINT(&idesc[i]) = (DWORD)Data::data.getEnemyName(_tdi);
				d[i].bfloat = true;/ *Specialized for Handle* /*/

				break;
			case SCR_SPELLNAME:
				_ParserFunction::SPELLNAME_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				CUINT(&idesc[i]) = (DWORD)Data::data.getSpellName(_tdi);
				d[i].bfloat = true;/ *Specialized for Handle* /*/

				break;
			case SCR_SPELLUSERNAME:
				_ParserFunction::SPELLUSERNAME_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				CUINT(&idesc[i]) = (DWORD)Data::data.getSpellUserName(_tdi);
				d[i].bfloat = true;/ *Specialized for Handle* /*/

				break;
			case SCR_SPELLUSERENAME:
				_ParserFunction::SPELLUSERENAME_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				CUINT(&idesc[i]) = (DWORD)Data::data.getSpellUserEName(_tdi);
				d[i].bfloat = true;/ *Specialized for Handle* /*/

				break;
			}
			break;
			/************************************************************************/
			/* Effect                                                               */
			/************************************************************************/
		case SCRKW_CLASS_EFFECT:
			switch (nowval)
			{
			}
			break;
			/************************************************************************/
			/* Player                                                               */
			/************************************************************************/
		case SCRKW_CLASS_PLAYER:
			switch (nowval)
			{
			case SCR_PX:
				_ParserFunction::PX_();
/*
				fdesc[i] = Player::p.x;
				d[i].bfloat = true;*/

				break;
			case SCR_PY:
				_ParserFunction::PY_();
/*
				fdesc[i] = Player::p.y;
				d[i].bfloat = true;*/

				break;
			case SCR_PLIFE:
				_ParserFunction::PLIFE_();
/*
				idesc[i] = Player::p.nLife;
				d[i].bfloat = false;*/

				break;
			case SCR_PBOMB:
				_ParserFunction::PBOMB_();
/*
				idesc[i] = (float)Player::p.nPower / Player::p.bombperpower;
				d[i].bfloat = false;*/

				break;
			case SCR_PPOWER:
				_ParserFunction::PPOWER_();
/*
				idesc[i] = Player::p.nPower;
				d[i].bfloat = false;*/

				break;
			case SCR_PFAITH:
				_ParserFunction::PFAITH_();
/*
				idesc[i] = Player::p.nFaith;
				d[i].bfloat =false;*/

				break;
			case SCR_PPOINT:
				_ParserFunction::PPOINT_();
/*
				idesc[i] = Player::p.nPoint;
				d[i].bfloat = false;*/

				break;
			case SCR_PBBORDER:
				_ParserFunction::PBBORDER_();
/*
				idesc[i] = (int)(Player::p.bBorder);
				d[i].bfloat = false;*/

				break;
			case SCR_PBSLOW:
				_ParserFunction::PBSLOW_();
/*
				idesc[i] = (int)(Player::p.bSlow);
				d[i].bfloat = false;*/

				break;
			case SCR_PBINFI:
				_ParserFunction::PBINFI_();
/*
				idesc[i] = (int)(Player::p.bInfi);
				d[i].bfloat = false;*/

				break;
			case SCR_PSPEED:
				_ParserFunction::PSPEED_();
/*
				fdesc[i] = Player::p.speed;
				d[i].bfloat = true;*/

				break;
			case SCR_PSLOWSPEED:
				_ParserFunction::PSLOWSPEED_();
/*
				fdesc[i] = Player::p.slowspeed;
				d[i].bfloat = true;*/

				break;
			case SCR_PGX:
				_ParserFunction::PGX_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				fdesc[i] = Player::p.pg[_tdi].x;
				d[i].bfloat = true;*/

				break;
			case SCR_PGY:
				_ParserFunction::PGY_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				fdesc[i] = Player::p.pg[_tdi].y;
				d[i].bfloat = true;*/

				break;
			case SCR_HAVEPLAYER:
				_ParserFunction::HAVEPLAYER_();
/*
				++(*p);
				_tdi = CINT(Value(&(*p), i, 0));
				idesc[i] = Player::p.HavePlayer(_tdi);
				d[i].bfloat = false;*/

				break;
			}
			break;
			/************************************************************************/
			/* Layer                                                                */
			/************************************************************************/
		case SCRKW_CLASS_LAYER:
			switch (nowval)
			{
			case SCR_BGSINDEX:
				_ParserFunction::BGSI_();
/*
				idesc[i] = BGLayer::setindex;
				d[i].bfloat = false;*/

				break;
			case SCR_BGSTIMER:
				_ParserFunction::BGSat_();
/*
				idesc[i] = BGLayer::bglayerset[BGLayer::setindex].timer;
				d[i].bfloat = false;*/

				break;
			}
			break;
			/************************************************************************/
			/* Select                                                               */
			/************************************************************************/
		case SCRKW_CLASS_SELECT:
			switch (nowval)
			{
			case SCR_SELCOMPLETE:
				_ParserFunction::SELCOMPLETE_();
/*
				idesc[i] = Selector::complete;
				d[i].bfloat = false;*/

				break;
			case SCR_SEL:
				_ParserFunction::SEL_();
/*
				idesc[i] = Selector::select;
				d[i].bfloat = false;*/

				break;
			case SCR_SELFIRSTID:
				_ParserFunction::SELFIRSTID_();
/*
				idesc[i] = Selector::firstID;
				d[i].bfloat = false;*/

				break;
			case SCR_ISELCOMPLETE:
				_ParserFunction::ISELCOMPLETE_();
/*
				idesc[i] = InfoSelect::complete;
				d[i].bfloat = false;*/

				break;
			case SCR_ISEL:
				_ParserFunction::ISEL_();
/*
				idesc[i] = InfoSelect::select;
				d[i].bfloat = false;*/

				break;
			case SCR_ISELFIRSTID:
				_ParserFunction::ISELFIRSTID_();
/*
				idesc[i] = InfoSelect::firstID;
				d[i].bfloat = false;*/

				break;
			}
			break;
		}
	}

	if(d[i].bfloat)
		return &fdesc[i];
	return &idesc[i];
}