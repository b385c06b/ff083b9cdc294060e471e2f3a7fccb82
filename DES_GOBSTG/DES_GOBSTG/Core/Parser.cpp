#include "../Header/Scripter.h"
#include "../Header/processPrep.h"
#include "../Header/BulletListActionConst.h"
#include "../Header/keytable.h"
#include "../Header/Parser.h"

bool Scripter::PushScript(int varcount)
{
	if (!scriptpushed)
	{
		psaved = pnow;
		if (varcount)
		{
			idescsaved = (int *)malloc(sizeof(int) * varcount);
			descsaved = (TData *)malloc(sizeof(TData) * varcount);
			memcpy(idescsaved, &idesc[SCR_VARBEGIN], sizeof(int) * varcount);
			memcpy(descsaved, &d[SCR_VARBEGIN], sizeof(TData) * varcount);
		}
		scriptpushed = true;
		return true;
	}
	return false;
}

bool Scripter::PopScript(int varcount)
{
	if (scriptpushed)
	{
		if (varcount)
		{
			memcpy(&idesc[SCR_VARBEGIN], idescsaved, sizeof(int) * varcount);
			memcpy(&d[SCR_VARBEGIN], descsaved, sizeof(TData) * varcount);
			free(idescsaved);
			free(descsaved);
		}
		pnow = psaved;
		scriptpushed = false;
		return true;
	}
	return false;
}

bool Scripter::Parse(int varcount)
{
	for(vector<Script>::iterator it=pnow->begin();it!=pnow->end();it++)
	{
		bool rv = false;
		int depth = 0;
		if(!it->value)
			continue;

		_Parser::Init(&it, varcount);
		//Filter Type
		DWORD nowval = it->value;
		switch (nowval & SCRKWMASK_TYPE)
		{
			/************************************************************************/
			/* Action                                                               */
			/************************************************************************/
		case SCRKW_ACTION:
			switch (nowval)
			{
			case SCR_SD:
				rv = _Parser::SD_();
/*
				if(rv = Copy(&it, 2))
				{
					if (d[1].bfloat)
					{
						CINT(d[1].value) = (int)(CFLOAT(d[1].value));
					}
					SetValue(CAST(d[0]), d[1].value, false);
				}*/

				break;
			case SCR_SDF:
				rv = _Parser::SDf_();
/*
				if(rv = Copy(&it, 2))
				{
					if (!d[1].bfloat)
					{
						CFLOAT(d[1].value) = (float)(CINT(d[1].value));
					}
					SetValue(CAST(d[0]), d[1].value, true);
				}*/

				break;
			case SCR_ST:
				rv = _Parser::ST_();
/*
				if(rv = Copy(&it, 3))
				{
					int _tdi = CAST(d[0]);
					Target::SetValue(_tdi, CAST(d[1]), CAST(d[2]));
				}*/

				break;
			case SCR_CALL:
				rv = _Parser::CALL_();
/*
				if(rv = Copy(&it, 2))
				{
					vector<Script> * psaved = pnow;
					int * idescsaved;
					TData * descsaved;
					if (varcount)
					{
						idescsaved = (int *)malloc(sizeof(int) * varcount);
						descsaved = (TData *)malloc(sizeof(TData) * varcount);
						memcpy(idescsaved, &idesc[SCR_VARBEGIN], sizeof(int) * varcount);
						memcpy(descsaved, &d[SCR_VARBEGIN], sizeof(TData) * varcount);
					}
					functionExecute(UCAST(d[0]), UCAST(d[1]));
					if (varcount)
					{
						memcpy(&idesc[SCR_VARBEGIN], idescsaved, sizeof(int) * varcount);
						memcpy(&d[SCR_VARBEGIN], descsaved, sizeof(TData) * varcount);
						free(idescsaved);
						free(descsaved);
					}
					pnow = psaved;
				}*/

				break;
			case SCR_CALLEX:
				rv = _Parser::CALLEX_();
/*
				if (rv = Copy(&it, 3))
				{
					vector<Script> * psaved = pnow;
					int * idescsaved;
					TData * descsaved;
					if (varcount)
					{
						idescsaved = (int *)malloc(sizeof(int) * varcount);
						descsaved = (TData *)malloc(sizeof(TData) * varcount);
						memcpy(idescsaved, &idesc[SCR_VARBEGIN], sizeof(int) * varcount);
						memcpy(descsaved, &d[SCR_VARBEGIN], sizeof(TData) * varcount);
					}
					int _tdi = UCAST(d[2]);
					if (_tdi)
					{
						rv = Copy(&it, _tdi, 2);
					}
					functionExecute(UCAST(d[0]), UCAST(d[1]));
					if (varcount)
					{
						memcpy(&idesc[SCR_VARBEGIN], idescsaved, sizeof(int) * varcount);
						memcpy(&d[SCR_VARBEGIN], descsaved, sizeof(TData) * varcount);
						free(idescsaved);
						free(descsaved);
					}
					pnow = psaved;
				}*/

				break;
			case SCR_EXECUTE:
				rv = _Parser::EXECUTE_();
/*
				if(rv = Copy(&it, 3))
				{
					vector<Script> * psaved = pnow;
					int * idescsaved;
					TData * descsaved;
					if (varcount)
					{
						idescsaved = (int *)malloc(sizeof(int) * varcount);
						descsaved = (TData *)malloc(sizeof(TData) * varcount);
						memcpy(idescsaved, &idesc[SCR_VARBEGIN], sizeof(int) * varcount);
						memcpy(descsaved, &d[SCR_VARBEGIN], sizeof(TData) * varcount);
					}
					Execute(UCAST(d[0]) + SCRIPT_CTOFFSET, UCAST(d[1]), UCAST(d[2]));
					if (varcount)
					{
						memcpy(&idesc[SCR_VARBEGIN], idescsaved, sizeof(int) * varcount);
						memcpy(&d[SCR_VARBEGIN], descsaved, sizeof(TData) * varcount);
						free(idescsaved);
						free(descsaved);
					}
					pnow = psaved;
				}*/

				break;

			case SCR_RETURN:
				rv = _Parser::RETURN_();
				if (rv)
				{
					return true;
				}
/*
				if(rv = Copy(&it, 1))
				{
					Process::mp.retvalue = CAST(d[0]);
					return true;
				}*/

				break;

			case SCR_SETSTATE:
				rv = _Parser::SETSTATE_();
/*
				if(rv = Copy(&it, 1))
				{
					Process::mp.state = CAST(d[0]);
				}*/

				break;
			case SCR_SETTIME:
				rv = _Parser::SETTIME_();
/*
				if(rv = Copy(&it, 1))
				{
					gametime = CAST(d[0]);
				}*/

				break;
			case SCR_SETDIFFI:
				rv = _Parser::SETDIFFI_();
/*
				if(rv = Copy(&it, 1))
				{
					Process::mp.nowdifflv = CAST(d[0]);
					if(Process::mp.nowdifflv < M_DIFFI_EXTRA_START)
						Process::mp.defaultdifflv = Process::mp.nowdifflv;
				}*/

				break;
			case SCR_SETCHARA:
				rv = _Parser::SETCHARA_();
/*
				if(rv = Copy(&it, 3))
				{
					Process::mp.mainchara = CAST(d[0]);
					Process::mp.subchara_1 = CAST(d[1]);
					Process::mp.subchara_2 = CAST(d[2]);
					Player::p.ID = Process::mp.mainchara;
					Player::p.ID_sub_1 = Process::mp.subchara_1;
					Player::p.ID_sub_2 = Process::mp.subchara_2;
				}*/

				break;
			case SCR_SETSCENE:
				rv = _Parser::SETSCENE_();
/*
				if(rv = Copy(&it, 1))
				{
					Process::mp.scene = CAST(d[0]);
				}*/

				break;
			case SCR_SETMODE:
				rv = _Parser::SETMODE_();
/*
				if(rv = Copy(&it, 1))
				{
					int _tdi = CAST(d[0]);
					Process::mp.spellmode = (bool)(_tdi & M_RPYMODE_SPELL);
					Process::mp.practicemode = (bool)(_tdi & M_RPYMODE_PRACTICE);
				}*/

				break;
			case SCR_STARTPREP:
				rv = _Parser::STARTPREP_();
/*
				if (rv = true)
				{
					Process::mp.startPrep(false);
				}*/

				break;

			case SCR_SETKEY:
				rv = _Parser::SETKEY_();
/*
				if (rv = Copy(&it, 2))
				{
					hge->Input_SetDIKey(CAST(d[0]), CAST(d[1]));
				}*/

				break;
			case SCR_DISABLEALLKEY:
				rv = _Parser::DISABLEALLKEY_();
/*
				if (rv = true)
				{
					hge->Input_SetDIKey(KS_FIRE_MP, false);
					hge->Input_SetDIKey(KS_SPECIAL_MP, false);
					hge->Input_SetDIKey(KS_CHANGE_MP, false);
					hge->Input_SetDIKey(KS_SLOW_MP, false);
					hge->Input_SetDIKey(KS_UP_MP, false);
					hge->Input_SetDIKey(KS_DOWN_MP, false);
					hge->Input_SetDIKey(KS_LEFT_MP, false);
					hge->Input_SetDIKey(KS_RIGHT_MP, false);
				}*/

				break;

			case SCR_SETPUSHEVENT:
				rv = _Parser::SETPUSHEVENT_();
/*
				if (rv = Copy(&it, 7))
				{
					PushKey::SetPushEvent(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]));
				}*/

				break;
			case SCR_UPDATEPUSHEVENT:
				rv = _Parser::UPDATEPUSHEVENT_();
/*
				if (rv = Copy(&it, 1))
				{
					PushKey::UpdatePushEvent(CAST(d[0]));
				}*/

				break;

			case SCR_STOPACTION:
				rv = _Parser::STOPACTION_();
/*
				if (rv = Copy(&it, 2))
				{
					Process::mp.SetStop(CAST(d[0]), CAST(d[1]));
				}*/

				break;
			case SCR_SETFRAMESKIP:
				rv = _Parser::SETFRAMESKIP_();
/*
				if (rv = Copy(&it, 1))
				{
					Process::mp.frameskip = CAST(d[0]);
				}*/

				break;

			case SCR_MUSICCHANGE:
				rv = _Parser::MUSICCHANGE_();
/*
				if(rv = Copy(&it, 2))
				{
					Process::mp.musicChange(CAST(d[0]), (bool)(CAST(d[1])));
				}*/

				break;
			case SCR_MUSICSLIDE:
				rv = _Parser::MUSICSLIDE_();
/*
				if (rv = Copy(&it, 4))
				{
					Process::mp.musicSlide(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]));
				}*/

				break;

			case SCR_SE:
				rv = _Parser::SE_();
/*
				if(rv = Copy(&it, 2))
				{
					SE::push(CAST(d[0]), CAST(d[1]));
				}*/

				break;
			case SCR_SEOFF:
				rv = _Parser::SEOFF_();
/*
				if (rv = Copy(&it, 1))
				{
					SE::stop(CAST(d[0]));
				}*/

				break;

			case SCR_HSVTORGB:
				rv = _Parser::HSVTORGB_();
/*
				if (rv = Copy(&it, 5))
				{
					hgeColorHSV _hsv(CAST(d[0]) / 255.0f, CAST(d[1]) / 255.0f, CAST(d[2]) / 255.0f, CAST(d[3]) / 255.0f);
					int _tdi = CAST(d[4]);
					CINT(d[_tdi].value) = _hsv.GetHWColor();
				}*/

				break;

			case SCR_SETSHAKE:
				rv = _Parser::SETSHAKE_();
/*
				if (rv = Copy(&it, 2))
				{
					Process::mp.SetShake(CAST(d[0]), CAST(d[1]));
				}*/

				break;
			}
			break;
			/************************************************************************/
			/* Basic                                                                */
			/************************************************************************/
		case SCRKW_BASIC:
			switch (nowval & SCRKWMASK_CLASS)
			{
				/************************************************************************/
				/* AttackObject                                                         */
				/************************************************************************/
			case SCRKW_CLASS_ATTACKOBJECT:
				switch (nowval)
				{
				case SCR_BUBUILD:
					rv = _Parser::B_();
/*
					if(rv = Copy(&it, 9))
					{
						Bullet::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]));
					}*/

					break;
				case SCR_BUACTIONSET:
					rv = _Parser::A_();
/*
					if(rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if(!_tdi)
						{
							Bullet::_bu.actionList[0] = SECTIONEND;
						}
						else if(rv = Copy(&it, _tdi))
						{
							for(int j=0;j<_tdi;j++)
							{
								Bullet::_bu.actionList[j] = CAST(d[j]);
							}
							Bullet::_bu.actionList[_tdi] = SECTIONEND;
						}
					}*/

					break;
				case SCR_BUBUILDCIRCLE:
					rv = _Parser::BC_();
/*
					if (rv = Copy(&it, 10))
					{
						Bullet::BuildCircle(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]));
					}*/

					break;
				case SCR_BUBUILDLINE:
					rv = _Parser::BL_();
/*
					if (rv = Copy(&it, 14))
					{
						Bullet::BuildLine(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), CAST(d[10]), CAST(d[11]), CAST(d[12]), CAST(d[13]));
					}*/

					break;
				case SCR_BUDECANCEL:
					rv = _Parser::BUDECANCEL_();
/*
					if (rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi >= 0 && _tdi < BULLETMAX)
						{
							if (Bullet::bu.toIndex(_tdi) != VECLST_INDEXERROR)
							{
								(*Bullet::bu).cancelable = false;
							}
						}
					}*/

					break;
				case SCR_IZEZONE:
					rv = _Parser::IZEZONE_();
/*
					if (rv = Copy(&it, 6))
					{
						Bullet::IzeBuild(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]));
					}*/

					break;

				case SCR_BEBUILD:
					rv = _Parser::BEB_();
/*
					if(rv = Copy(&it, 12))
					{
						Beam::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), (CAST(d[7])), CAST(d[8]), CAST(d[9]), CAST(d[10]), CAST(d[11]));
					}*/

					break;
				case SCR_BEVECTOR:
					rv = _Parser::BEVECTOR_();
/*
					if (rv = Copy(&it, 5))
					{
						int _tdi = CAST(d[0]);
						if (Beam::be.toIndex(_tdi) != VECLST_INDEXERROR)
						{
							if (Beam::be.isInRange() && Beam::be.isValid())
							{
								if ((*Beam::be).able)
								{
									(*Beam::be).SetVector(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
								}
							}
						}
					}*/

					break;
				case SCR_BEHOLD:
					rv = _Parser::BEHOLD_();
/*
					if (rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[0]);
						if (Beam::be.toIndex(_tdi) != VECLST_INDEXERROR)
						{
							if (Beam::be.isInRange() && Beam::be.isValid())
							{
								if ((*Beam::be).able)
								{
									(*Beam::be).SetHold(CAST(d[1]), CAST(d[2]));
									(*Beam::be).angle = CAST(d[3]);
								}
							}
						}
					}*/

					break;
				case SCR_BEPIN:
					rv = _Parser::BEPIN_();
/*
					if (rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[0]);
						if (Beam::be.toIndex(_tdi) != VECLST_INDEXERROR)
						{
							if (Beam::be.isInRange() && Beam::be.isValid())
							{
								if ((*Beam::be).able)
								{
									(*Beam::be).SetHoldPin(CAST(d[1]), CAST(d[2]), CAST(d[3]));
								}
							
							}
						}
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Ghost                                                                */
				/************************************************************************/
			case SCRKW_CLASS_GHOST:
				switch (nowval)
				{
				case SCR_GHBUILD:
					rv = _Parser::GB_();
/*
					if(rv = Copy(&it, 10))
					{
						Ghost::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]));
					}*/

					break;
				case SCR_GHSAIM:
					rv = _Parser::GHSAIM_();
/*
					if(rv = Copy(&it, 2))
					{
						Ghost::gh[Ghost::index].aim.x = CAST(d[0]);
						Ghost::gh[Ghost::index].aim.y = CAST(d[1]);
					}*/

					break;
				case SCR_GHSET:
					rv = _Parser::GHSET_();
/*
					if(rv = Copy(&it, 3))
					{
						Ghost::gh[Ghost::index].angle = CAST(d[0]);
						Ghost::gh[Ghost::index].speed = CAST(d[1]);
						Ghost::gh[Ghost::index].ac = CAST(d[2]);
					}*/

					break;
				case SCR_GHCHASE:
					rv = _Parser::GHCHASE_();
/*
					if (rv = Copy(&it, 3))
					{
						Ghost::gh[Ghost::index].chaseAim(CAST(d[0]), CAST(d[1]), CAST(d[2]));
					}*/

					break;
				case SCR_GHSETLIFE:
					rv = _Parser::GHSETLIFE_();
/*
					if (rv = Copy(&it, 2))
					{
						Ghost::gh[Ghost::index].life = CAST(d[0]);
						Ghost::gh[Ghost::index].maxlife = CAST(d[1]);
						if (Ghost::gh[Ghost::index].life > Ghost::gh[Ghost::index].maxlife)
						{
							Ghost::gh[Ghost::index].maxlife = Ghost::gh[Ghost::index].life;
						}
					}*/

					break;
				case SCR_GHCHANGE:
					rv = _Parser::GHCHANGE_();
/*
					if (rv = Copy(&it, 1))
					{
						Ghost::gh[Ghost::index].gID = CAST(d[0]);
						Ghost::gh[Ghost::index].timer = 0;
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Enemy                                                                */
				/************************************************************************/
			case SCRKW_CLASS_ENEMY:
				switch (nowval)
				{
				case SCR_ENBUILD:
					rv = _Parser::EB_();
/*
					if(rv = Copy(&it, 11))
					{
						Enemy::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), UCAST(d[10]));
					}*/

					break;
				case SCR_ENACTIONSET:
					rv = _Parser::EA_();
/*
					if(rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if(_tdi)
						{
							if(rv = Copy(&it, ENEMY_PARAMAX*2))
							{
								Enemy::en[Enemy::index].ac = _tdi;
								int j=0;
								for(; j<ENEMY_PARAMAX; j++)
								{
									Enemy::en[Enemy::index].fpara[j] = CAST(d[j]);
								}
								j = 0;
								for(; j<ENEMY_PARAMAX; j++)
								{
									Enemy::en[Enemy::index].ipara[j] = CAST(d[j+ENEMY_PARAMAX]);
								}
							}
						}
						else
						{
							Enemy::en[Enemy::index].ac = ENAC_NONE;
						}
					}*/

					break;
				case SCR_ENSAIM:
					rv = _Parser::ENSAIM_();
/*
					if(rv = Copy(&it, 2))
					{
						Enemy::en[Enemy::index].aim.x = CAST(d[0]);
						Enemy::en[Enemy::index].aim.y = CAST(d[1]);
					}*/

					break;
				case SCR_ENCHASE:
					rv = _Parser::ENCHASE_();
/*
					if (rv = Copy(&it, 3))
					{
						Enemy::en[Enemy::index].chaseAim(CAST(d[0]), CAST(d[1]), CAST(d[2]));
					}*/

					break;
				case SCR_ENSETLIFE:
					rv = _Parser::ENSETLIFE_();
/*
					if (rv = Copy(&it, 2))
					{
						Enemy::en[Enemy::index].life = CAST(d[0]);
						Enemy::en[Enemy::index].maxlife = CAST(d[1]);
						if (Enemy::en[Enemy::index].life < Enemy::en[Enemy::index].maxlife)
						{
							Enemy::en[Enemy::index].maxlife = Enemy::en[Enemy::index].life;
						}
					}*/

					break;
				case SCR_ENCHANGE:
					rv = _Parser::ENCHANGE_();
/*
					if (rv = Copy(&it, 1))
					{
						Enemy::en[Enemy::index].eID = CAST(d[0]);
						Enemy::en[Enemy::index].timer = 0;
					}*/

					break;

				case SCR_DAMAGEZONE:
					rv = _Parser::DAMAGEZONE_();
/*
					if (rv = Copy(&it, 4))
					{
						Enemy::DamageZoneBuild(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]));
					}*/

					break;
				case SCR_BOSSSTORE:
					rv = _Parser::BOSSSTORE_();
/*
					if (rv = true)
					{
						Enemy::bossflag[Enemy::index] = BOSS_STORE;
					}*/

					break;
				case SCR_BOSSATTACK:
					rv = _Parser::BOSSATTACK_();
/*
					if (rv = true)
					{
						Enemy::bossflag[Enemy::index] = BOSS_ATTACK;
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Math                                                                 */
				/************************************************************************/
			case SCRKW_CLASS_MATH:
				switch (nowval)
				{
				case SCR_RAMA:
					rv = _Parser::RAMA_();
/*
					if(rv = Copy(&it, 7))
					{
						int _tdi = CAST(d[6]);
						BObject _tobj;
						_tobj.x = CAST(d[1]);
						_tobj.y = CAST(d[2]);
						if(CAST(d[0]))
						{
							CINT(d[_tdi].value) = _tobj.rMainAngle(CAST(d[3]), CAST(d[4]), CAST(d[5]));
						}
						else
						{
							CINT(d[_tdi].value) = _tobj.aMainAngle(CAST(d[3]), CAST(d[4]), CAST(d[5]));
						}
						d[_tdi].bfloat = false;
					}*/

					break;
				case SCR_DIST:
					rv = _Parser::DIST_();
/*
					if (rv = Copy(&it, 5))
					{
						int _tdi = CAST(d[4]);
						float _txo = CAST(d[0]);
						float _tyo = CAST(d[1]);
						float _txa = CAST(d[2]);
						float _tya = CAST(d[3]);
						CFLOAT(d[_tdi].value) = DIST(_txo, _tyo, _txa, _tya);
						d[_tdi].bfloat = true;
					}*/

					break;
				case SCR_CHASEAIM:
					rv = _Parser::CHASEAIM_();
/*
					if (rv = Copy(&it, 3))
					{
						int _tdi[2];
						_tdi[0] = CAST(d[0]);
						BObject _obj;
						Target::GetValue(_tdi[0], &(_obj.x), &(_obj.y));
						_tdi[1] = CAST(d[1]);
						_obj.chaseAim(Target::GetX(_tdi[1]), Target::GetY(_tdi[1]), CAST(d[2]));
						_obj.x += cost(_obj.angle) * _obj.speed;
						_obj.y += sint(_obj.angle) * _obj.speed;
						Target::SetValue(_tdi[0], _obj.x, _obj.y);
					}*/

					break;
				case SCR_INTER:
					rv = _Parser::INTER_();
/*
					if (rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[3]);
						CFLOAT(d[_tdi].value) = INTER(CAST(d[0]), CAST(d[1]), CAST(d[2]));
						d[_tdi].bfloat = true;
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Other                                                                */
				/************************************************************************/
			case SCRKW_CLASS_OTHER:
				switch (nowval)
				{
				case SCR_LOADTEXTURESET:
					rv = _Parser::LOADTEXTURESET_();
/*
					if (rv = Copy(&it, 1))
					{
						Process::mp.LoadTextureSet(CAST(d[0]));
					}*/

					break;
				case SCR_FREETEXTURESET:
					rv = _Parser::FREETEXTURESET_();
/*
					if (rv = Copy(&it, 1))
					{
						Process::mp.FreeTextureSet(CAST(d[0]));
					}*/

					break;
				case SCR_ITEMBUILD:
					rv = _Parser::ITEMBUILD_();
/*
					if (rv = Copy(&it, 6))
					{
						Item::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]));
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Special                                                              */
				/************************************************************************/
			case SCRKW_CLASS_SPECIAL:
				switch (nowval)
				{
				case SCR_BOSSUP:
					rv = _Parser::BOSSUP_();
/*
					if(rv = true)
					{
						BossInfo::bossinfo.bossUp();
					}*/

					break;
				case SCR_SPELLUP:
					rv = _Parser::SPELLUP_();
/*
					if(rv = Copy(&it, 3))
					{
						if(Process::mp.spellmode)
						{
							int _tdi = CAST(d[0]);
							Target::SetValue(_tdi, CAST(d[1]), CAST(d[2]));
						}
					}*/

					break;

				case SCR_ACONTROL:
					rv = _Parser::ACONTROL_();
/*
					if (rv = Copy(&it, 2))
					{
						int _tindex = CAST(d[0]);
						int _tdi = CAST(d[1]);
						if (rv = Copy(&it, _tdi))
						{
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
													(*Bullet::bu).actionList[j] = CAST(d[j]);
												}
												(*Bullet::bu).actionList[_tdi] = SECTIONEND;
											}
										}
									}
								}
							}
						}
					}*/

					break;
				case SCR_BCONTROL:
					rv = _Parser::BCONTROL_();
/*
					if(rv = Copy(&it, 2))
					{
						BObject * _tobj = NULL;
						int _ttype = CAST(d[0]);
						int _tdi = CAST(d[1]);
						int _tindex = CAST(d[_tdi]);

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

						if (rv = Copy(&it, 5))
						{
							if(_tobj)
							{
								_tobj->x = CAST(d[0]);
								_tobj->y = CAST(d[1]);
								_tobj->angle = CAST(d[2]);
								_tobj->headangle = CAST(d[3]);
								_tobj->speed = CAST(d[4]);
							}
							else
							{
								CINT(d[_tdi].value) = BINDEX_NULL;
								d[_tdi].bfloat = false;
							}
						}
					}*/

					break;

				case SCR_COLLISION_CIRCLE:
					rv = _Parser::COLLISION_CIRCLE_();
/*
					if (rv = Copy(&it, 6))
					{
						int _tdi = CAST(d[5]);
						BObject _tobj;
						_tobj.x = CAST(d[0]);
						_tobj.y = CAST(d[1]);
						CINT(d[_tdi].value) = _tobj.checkCollisionCircle(CAST(d[2]), CAST(d[3]), CAST(d[4]));
						d[_tdi].bfloat = false;
					}*/

					break;
				case SCR_COLLISION_SQUARE:
					rv = _Parser::COLLISION_SQUARE_();
/*
					if (rv = Copy(&it, 6))
					{
						int _tdi = CAST(d[5]);
						BObject _tobj;
						_tobj.x = CAST(d[0]);
						_tobj.y = CAST(d[1]);
						CINT(d[_tdi].value) = _tobj.checkCollisionSquare(CAST(d[2]), CAST(d[3]), CAST(d[4]));
						d[_tdi].bfloat = false;
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Effect                                                               */
				/************************************************************************/
			case SCRKW_CLASS_EFFECT:
				switch (nowval)
				{
				case SCR_EFFSETUP:
					rv = _Parser::EFFSETUP_();
/*
					if (rv = Copy(&it, 5))
					{
						int _tdi = CAST(d[0]);
						Effectsys::effsys[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
					}*/

					break;
				case SCR_EFFSETUPEX:
					rv = _Parser::EFFSETUPEX_();
/*
					if(rv = Copy(&it, 10))
					{
						int _tdi = CAST(d[0]);
						Effectsys::effsys[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]));
					}*/

					break;
				case SCR_EFFSETUPCHASE:
					rv = _Parser::EFFSETUPCHASE_();
/*
					if (rv = Copy(&it, 8))
					{
						int _tdi = CAST(d[0]);
						Effectsys::effsys[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]));
					}*/

					break;
				case SCR_EFFSTOP:
					rv = _Parser::EFFSTOP_();
/*
					if (rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (Effectsys::effsys[_tdi].exist && Effectsys::effsys[_tdi].eff)
						{
							Effectsys::effsys[_tdi].Stop();
						}
					}*/

					break;
				case SCR_EFFOFF:
					rv = _Parser::EFFOFF_();
/*
					if(rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						Effectsys::effsys[_tdi].exist = false;
					}*/

					break;
				case SCR_EFFSET:
					rv = _Parser::EFFSET_();
/*
					if(rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[0]);
						Effectsys::effsys[_tdi].angle = CAST(d[1]);
						Effectsys::effsys[_tdi].speed = CAST(d[2]);
						Effectsys::effsys[_tdi].zSpeed = CAST(d[3]);
					}*/

					break;
				case SCR_EFFMOVETO:
					rv = _Parser::EFFMOVETO_();
/*
					if(rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[0]);
						Effectsys::effsys[_tdi].x = CAST(d[1]);
						Effectsys::effsys[_tdi].y = CAST(d[2]);
						Effectsys::effsys[_tdi].z = CAST(d[3]);
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Player                                                               */
				/************************************************************************/
			case SCRKW_CLASS_PLAYER:
				switch (nowval)
				{
				case SCR_SETPPOS:
					rv = _Parser::SETPPOS_();
/*
					if (rv = Copy(&it, 2))
					{
						float _tdfx = CAST(d[0]);
						float _tdfy = CAST(d[1]);
						_tdfx = _tdfx < PL_MOVABLE_LEFT ? PL_MOVABLE_LEFT : _tdfx;
						_tdfx = _tdfx > PL_MOVABLE_RIGHT ? PL_MOVABLE_RIGHT : _tdfx;
						_tdfy = _tdfy < PL_MOVABLE_TOP ? PL_MOVABLE_TOP : _tdfy;
						_tdfy = _tdfy > PL_MOVABLE_BOTTOM ? PL_MOVABLE_BOTTOM : _tdfy;
						Player::p.x = _tdfx;
						Player::p.y = _tdfy;
					}*/

					break;
				case SCR_SETPLIFE:
					rv = _Parser::SETPLIFE_();
/*
					if (rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nLife = _tdi > PL_NPLAYERMAX ? PL_NPLAYERMAX : _tdi;
					}*/

					break;
				case SCR_SETPPOWER:
					rv = _Parser::SETPPOWER_();
/*
					if(rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nPower = _tdi;
					}*/

					break;
				case SCR_SETPFAITH:
					rv = _Parser::SETPFAITH_();
/*
					if (rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nFaith = _tdi;
					}*/

					break;
				case SCR_SETPPOINT:
					rv = _Parser::SETPPOINT_();
/*
					if (rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi < 0)
						{
							break;
						}
						Player::p.nPoint = _tdi;
					}*/

					break;
				case SCR_SETPBBORDER:
					rv = _Parser::SETPBBORDER_();
/*
					if (rv = Copy(&it, 1))
					{
						if ((bool)(CAST(d[0])) ^ Player::p.bBorder)
						{
							Player::p.callBomb(true);
						}
					}*/

					break;
				case SCR_SETPBSLOW:
					rv = _Parser::SETPBSLOW_();
/*
					if (rv = Copy(&it, 1))
					{
						Player::p.callSlowFastChange((bool)(CAST(d[0])));
					}*/

					break;
				case SCR_SETPBINFI:
					rv = _Parser::SETPBINFI_();
/*
					if(rv = true)
					{
						Player::p.bInfi = true;
					}*/

					break;
				case SCR_SETPSPEED:
					rv = _Parser::SETPSPEED_();
/*
					if (rv = Copy(&it, 1))
					{
						Player::p.speed = CAST(d[0]);
					}*/

					break;
				case SCR_SETPSLOWSPEED:
					rv = _Parser::SETPSLOWSPEED_();
/*
					if (rv = Copy(&it, 1))
					{
						Player::p.slowspeed = CAST(d[0]);
					}*/

					break;
				case SCR_SETPSPEEDFACTOR:
					rv = _Parser::SETPSPEEDFACTOR_();
/*
					if (rv = Copy(&it, 1))
					{
						Player::p.speedfactor = CAST(d[0]);
					}*/

					break;

				case SCR_COLLAPSE:
					rv = _Parser::COLLAPSE_();
/*
					if (rv = true)
					{
						Player::p.callCollapse();
					}*/

					break;

				case SCR_SHOOTPB:
					rv = _Parser::SHOOTPB_();
/*
					if (rv = Copy(&it, 1))
					{
						PlayerBullet::Build(CAST(d[0]));
					}*/

					break;
				case SCR_BONUSFLAG:
					rv = _Parser::BONUSFLAG_();
/*
					if (rv = Copy(&it, 1))
					{
						Player::p.bonusflag = CAST(d[0]);
					}*/

					break;
				}
				break;
				/************************************************************************/
				/* Layer                                                                */
				/************************************************************************/
			case SCRKW_CLASS_LAYER:
				switch (nowval)
				{
				case SCR_BGVALUE:
					if(rv = Copy(&it, 7))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), UCAST(d[6]));
					}
					break;
				case SCR_BGVALEX:
					if (rv = Copy(&it, 16))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].valueSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), CAST(d[10]), CAST(d[11]), CAST(d[12]), (bool)(CAST(d[13])), (bool)(CAST(d[14])), UCAST(d[15]));
					}
					break;
				case SCR_BGTEXRECT:
					if(rv = Copy(&it, 5))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].texRectSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
					}
					break;
				case SCR_BGRECT:
					if(rv = Copy(&it, 9))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].rectSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]));
					}
					break;
				case SCR_BGZ:
					if(rv = Copy(&it, 5))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].zSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]));
					}
					break;
				case SCR_BGSCALE:
					if(rv = Copy(&it, 3))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].scaleSet(CAST(d[1]), CAST(d[2]));
					}
					break;
				case SCR_BGCOLOR:
					if(rv = Copy(&it, 5))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].colorSet(UCAST(d[1]), UCAST(d[2]), UCAST(d[3]), UCAST(d[4]));
					}
					break;
				case SCR_BGMOVE:
					if(rv = Copy(&it, 6))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].moveSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), (bool)(CAST(d[4])), (bool)(CAST(d[5])));
					}
					break;
				case SCR_BGFLAG:
					if(rv = Copy(&it, 3))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].SetFlag(CAST(d[1]), CAST(d[2]));
					}
					break;
				case SCR_BGPARAL:
					if(rv = Copy(&it, 2))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].parallelogram(CAST(d[1]));
					}
					break;
				case SCR_BG4V:
					if (rv = Copy(&it, 13))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].vertexSet(CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]), CAST(d[7]), CAST(d[8]), CAST(d[9]), CAST(d[10]), CAST(d[11]), CAST(d[12]));
					}
					break;
				case SCR_BGOFF:
					if(rv = Copy(&it, 1))
					{
						int _tdi = CAST(d[0]);
						if (_tdi == -1)
						{
							BGLayer::KillOtherLayer();
						}
						else
						{
							BGLayer::ubg[_tdi].exist = false;
						}
					}
					break;
				case SCR_BGBLEND:
					if (rv = Copy(&it, 2))
					{
						int _tdi = CAST(d[0]);
						BGLayer::ubg[_tdi].SetBlend(CAST(d[1]));
					}
					break;
				case SCR_BGSETUP:
					if(rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[0]);
						if(CAST(d[2]) || BGLayer::bglayerset[_tdi].sID == BGLAYERSET_NONE)
						{
							int _tsID = CAST(d[1]);
							if(BGLayer::bglayerset[_tdi].sID != BGLAYERSET_NONE && BGLayer::bglayerset[_tdi].sID != CAST(d[1]))
							{
								vector<Script> * psaved = pnow;
								int * idescsaved;
								TData * descsaved;
								if (varcount)
								{
									idescsaved = (int *)malloc(sizeof(int) * varcount);
									descsaved = (TData *)malloc(sizeof(TData) * varcount);
									memcpy(idescsaved, &idesc[SCR_VARBEGIN], sizeof(int) * varcount);
									memcpy(descsaved, &d[SCR_VARBEGIN], sizeof(TData) * varcount);
								}
								sceneExecute(BGLayer::bglayerset[_tdi].sID, SCRIPT_CON_POST);
								if (varcount)
								{
									memcpy(&idesc[SCR_VARBEGIN], idescsaved, sizeof(int) * varcount);
									memcpy(&d[SCR_VARBEGIN], descsaved, sizeof(TData) * varcount);
									free(idescsaved);
									free(descsaved);
								}
								pnow = psaved;
							}
							BGLayer::bglayerset[_tdi].sID = _tsID;
							BGLayer::bglayerset[_tdi].quittime = CAST(d[3]);
							BGLayer::bglayerset[_tdi].timer = 0;
						}
					}
					break;
				}
				break;
				/************************************************************************/
				/* Select                                                               */
				/************************************************************************/
			case SCRKW_CLASS_SELECT:
				switch (nowval)
				{
				case SCR_SELBUILD:
					if(rv = Copy(&it, 16))
					{
						Selector::Build(CAST(d[0]), CAST(d[1]), CAST(d[2]), CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]),
							CAST(d[7]), CAST(d[8]),
							CAST(d[9]), CAST(d[10]),
							CAST(d[11]), CAST(d[12]),
							CAST(d[13]), CAST(d[14]),
							(CAST(d[15])));
					}
					break;
				case SCR_SELCLEAR:
					Selector::Clear();
					rv = true;
					break;
				case SCR_SELCONFIRM:
					if(rv = Copy(&it, 4))
					{
						int _tdi = CAST(d[3]);
						CINT(d[_tdi].value) = Selector::confirm(CAST(d[0]), CAST(d[1]), CAST(d[2]));
						d[_tdi].bfloat = false;
					}
					break;
				case SCR_SELSET:
					if(rv = Copy(&it, 7))
					{
						Selector::Setup(CAST(d[0]), CAST(d[1]), (bool)CAST(d[2]));
						Selector::SetPageNum(CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]));
					}
					break;
				case SCR_SELFLAG:
					if (rv = Copy(&it, 2))
					{
						Selector * _sel = Selector::GetPointer(CAST(d[0]));
                        if (_sel)
                        {
							_sel->flag = CAST(d[1]);
                        }
					}
					break;

				case SCR_ISELBUILD:
					if(rv = Copy(&it, 6))
					{
						InfoSelect::Build(CAST(d[0]), GetStringSp(1), CAST(d[2]), CAST(d[3]), CAST(d[4]), (CAST(d[5])));
					}
					break;
				case SCR_ISELCLEAR:
					InfoSelect::Clear();
					rv = true;
					break;
				case SCR_ISELSET:
					if(rv = Copy(&it, 7))
					{
						InfoSelect::Setup(CAST(d[0]), CAST(d[1]), (bool)CAST(d[2]));
						InfoSelect::SetPageNum(CAST(d[3]), CAST(d[4]), CAST(d[5]), CAST(d[6]));
					}
					break;
				case SCR_ISELFLAG:
					if (rv = Copy(&it, 2))
					{
						InfoSelect * _tifs = InfoSelect::GetPointer(CAST(d[0]));
						if (_tifs)
						{
							_tifs->flag = CAST(d[1]);
						}
					}
					break;
				case SCR_ISELCOLOR:
					if (rv = Copy(&it, 2))
					{
						InfoSelect * _tifs = InfoSelect::GetPointer(CAST(d[0]));
						if (_tifs)
						{
							_tifs->coltype = CAST(d[1]);
						}
					}
				}
				break;
			}
			break;
			/************************************************************************/
			/* Control                                                              */
			/************************************************************************/
		case SCRKW_CONTROL:
			switch (nowval)
			{
			case SCR_IF:
nest:
				if(rv = Copy(&it, 1))
				{
					if(!CAST(d[0]))
					{
						while(true)
						{
							++it;
							if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_THEN)
								depth++;
							if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_END)
							{
								depth--;
								if(!depth)
									break;
							}
						}
						++it;
						if(it!=pnow->end())
						{
							if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_ELSE)
								continue;
							else if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_ELSEIF)
								goto nest;
							else
								--it;
						}
						else
							--it;
					}
				}
				break;
			case SCR_ELSE:
			case SCR_ELSEIF:
				if(true)
				{
					while(true)
					{
						++it;
						if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_THEN)
							depth++;
						if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_END)
						{
							depth--;
							if(!depth)
								break;
						}
					}
				}
				rv = true;
				break;
			case SCR_END:
			case SCR_THEN:
				continue;

			case SCR_LOOP:
				if(rv = Copy(&it, 1))
				{
					vector<Script> _while;
					while(true)
					{
						++it;
						if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_THEN)
							depth++;
						else if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_END)
						{
							depth--;
							if(!depth)
								break;
						}
						_while.push_back(*it);
					}
					vector<Script> * psaved = pnow;
					pnow = &_while;

					while(CINT(d[CINT(d[0].value)].value)--)
					{
						int _idesc;
						_idesc = idesc[0];
						if(!Parse(varcount))
							break;
						idesc[0] = _idesc;
					}
					pnow = psaved;
				}
				break;
			case SCR_SKIP:
				return true;
				break;

			case SCR_CHATON:
				if(rv = Copy(&it, 1))
				{
					int _tdi = CAST(d[0]);
					if(_tdi < 0 || _tdi > PLAYERTYPEMAX || Player::p.HavePlayer(_tdi))
					{
						if(!Chat::chatitem.chati)
						{
							if(rv = Copy(&it, 3))
							{
								if(Chat::chatitem.chatOn(CAST(d[0]), CAST(d[1]), CAST(d[2])))
									Chat::chatitem.chati++;
								goto chatout;
							}
						}
						else
						{
							if(Chat::chatitem.chati == 0xff)
							{
								Chat::chatitem.chati = 0;
								goto chatout;
							}
							for(int j=0;j<Chat::chatitem.chati;j++)
							{
								++it;
								++it;
								++it;
							}
							++it;
							if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_CHATOFF)
							{
								if(Chat::chatitem.chatOff())
									Chat::chatitem.chati = 0xff;
								break;
							}
							else
							{
								--it;
								if(rv = Copy(&it, 3))
								{
									if(Chat::chatitem.chat(CAST(d[0]), CAST(d[1]), GetStringSp(2)))
										Chat::chatitem.chati++;
									goto chatout;
								}
							}
						}
					}
					else
					{
chatout:
						while(true)
						{
							++it;
							if(it->type & SCR_TOKEN_COMMAND && it->value == SCR_CHATOFF)
							{
								rv = true;
								break;
							}
						}
					}
				}
				break;
			case SCR_CHATOFF:
				continue;
			}
			break;
			/************************************************************************/
			/* Data                                                                 */
			/************************************************************************/
		case SCRKW_DATA:
			switch (nowval)
			{
			case SCR_DATAGET:
				if (rv = Copy(&it, 4))
				{
					int _tdi = CAST(d[3]);
					CINT(d[_tdi].value) = Data::data.iRead(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
					d[_tdi].bfloat = false;
				}
				break;
			case SCR_DATAGETf:
				if (rv = Copy(&it, 4))
				{
					int _tdi = CAST(d[4]);
					CFLOAT(d[_tdi].value) = Data::data.fRead(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
					d[_tdi].bfloat = true;
				}
				break;
			case SCR_DATASET:
				if (rv = Copy(&it, 3))
				{
					Data::data.iWrite(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
				}
				break;
			case SCR_DATASETf:
				if (rv = Copy(&it, 3))
				{
					Data::data.fWrite(DATA_BINFILE, UCAST(d[0]), UCAST(d[1]), CAST(d[2]));
				}
				break;
			case SCR_SETFLAG:
				if (rv = Copy(&it, 2))
				{
					Data::data.iWrite(DATA_BINFILE, DATAS_FLAG, UCAST(d[0]), CAST(d[1]));
				}
				break;

			case SCR_TRYSTAGE:
				if (rv = true)
				{
					DataConnector::Try();
				}
				break;

			case SCR_DEBUG_BREAKPOINT:
#ifdef __DEBUG
				if(rv = true)
				{
					LogOut();
				}
#endif
				break;
			}
			break;
		}

		if(!rv)
		{
#ifdef __DEBUG
			char strbuffer[M_STRMAX];
			sprintf(strbuffer, "%s\nError in parsing %d. Point to 0x%x.", HGELOG_ERRSTR, it->value, it->pos);
			HGELOG(strbuffer);
			MessageBox(NULL, strbuffer, HGELOG_ERRSTR, MB_OK);
			LogOut();
#endif
			return false;
		}
	}
	return true;
}
