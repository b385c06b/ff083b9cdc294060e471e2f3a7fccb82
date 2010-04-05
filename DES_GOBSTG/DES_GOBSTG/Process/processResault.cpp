#include "../Header/Process.h"
#include "../Header/Scripter.h"
#include "../Header/BResource.h"
#include "../Header/Data.h"
#include "../Header/DataConnector.h"
#include "../Header/InfoSelect.h"
#include "../Header/Selector.h"
#include "../Header/SE.h"
#include "../Header/DataPrinter.h"
#include "../Header/BossInfo.h"
#include "../Header/ConstResource.h"

int Process::processResult()
{
	gametime++;
	if (gametime == 1)
	{
		scr.SetIntValue(SCR_RESERVEBEGIN, -2);
		scr.SetIntValue(SCR_RESERVEBEGIN+1, -1);
		scr.SetIntValue(SCR_RESERVEBEGIN+2, 0);
	}
	scr.controlExecute(STATE_RESULT, gametime);
	int tsec = scr.GetIntValue(SCR_RESERVEBEGIN);
	int tdiff = scr.GetIntValue(SCR_RESERVEBEGIN+1);
	int tsel = scr.GetIntValue(SCR_RESERVEBEGIN+2);

	if(hge->Input_GetDIKey(KS_SPECIAL, DIKEY_DOWN))
	{
		SE::push(SE_SYSTEM_CANCEL);
		if (tsec < 0)
		{
			gametime = 0;
			state = STATE_TITLE;
			return PTURN;
		}
		else
		{
			tsec = -2;
		}
	}
	else if (hge->Input_GetDIKey(DIK_D, DIKEY_DOWN))
	{
		DataPrinter::PrintScore();
	}

	if (tsec == 0)
	{
		if (tdiff < 0)
		{
			InfoSelect::Clear();
			tdiff = Selector::select;
			InfoSelect _ifs[13];
			DWORD sec;
			for (int i=0; i<10; i++)
			{
				sec = Data::data.sLinkType(DATAS_TOP);
				sec = Data::data.sLinkDiff(sec, tdiff);
				sec = Data::data.sLinkNum(sec, i+1);

				strcpy(_ifs[i].info, "");
				_ifs[i].linki("|10002", i+1);
				strcat(_ifs[i].info, ". ");
				strcat(_ifs[i].info, Data::data.sRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_USERNAME), RESCONFIGDEFAULT_USERNAME));
				_ifs[i].linkl("|11224", Data::data.lRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_SCORE), 0));
				BYTE tlaststage = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_LASTSTAGE), 0);
				strcat(_ifs[i].info, "(");
				switch(tlaststage)
				{
				case 1:
					strcat(_ifs[i].info, M_LASTSTAGESTR_1);
					break;
				case 2:
					strcat(_ifs[i].info, M_LASTSTAGESTR_2);
					break;
				case 3:
					strcat(_ifs[i].info, M_LASTSTAGESTR_3);
					break;
				case 4:
					strcat(_ifs[i].info, M_LASTSTAGESTR_4);
					break;
				case 5:
					strcat(_ifs[i].info, M_LASTSTAGESTR_5);
					break;
				case 6:
					strcat(_ifs[i].info, M_LASTSTAGESTR_6);
					break;
				default:
					strcat(_ifs[i].info, M_LASTSTAGESTR_C);
					break;
				}
				strcat(_ifs[i].info, ")");
				_ifs[i].linki("|12933", Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_YEAR), 0));
				strcat(_ifs[i].info, "/");
				_ifs[i].linki("|23436", Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_MONTH), 0));
				strcat(_ifs[i].info, "/");
				_ifs[i].linki("|23739", Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TIME_DAY), 0));
//				_ifs[i].linkf("|24550", 2, data.fRead(DATA_BINFILE, sec, data.nLinkType(DATAN_LOST), 0));

				strcat(_ifs[i].info, "\n��");
				strcat(_ifs[i].info, Data::data.getPlayerName(Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkNum(Data::data.nLinkType(DATAN_CHARA), 1), 0)));
				strcat(_ifs[i].info, "\n��");
				strcat(_ifs[i].info, Data::data.getPlayerName(Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkNum(Data::data.nLinkType(DATAN_CHARA), 2), 0)));
				strcat(_ifs[i].info, "\n��");
				strcat(_ifs[i].info, Data::data.getPlayerName(Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkNum(Data::data.nLinkType(DATAN_CHARA), 3), 0)));

				_ifs[i].valueSet(i, _ifs[i].info, 30, 210+80*i, INFO_GREEN);
				infoselect.push_back(_ifs[i]);
			}

			char buff[M_STRITOAMAX];
			sec = Data::data.sLinkType(DATAS_TOTAL);
			LONGLONG tltotalplaytime = Data::data.lRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_TOTALPLAYTIME), 0) / 10000000;
			int tplayhour = tltotalplaytime / 3600;
			int tplayminute = (tltotalplaytime / 60) % 60;
			int tplaysecond = tltotalplaytime % 60;
			strcpy(_ifs[12].info, "");
			_ifs[12].linki("|10003", tplayhour);
			strcat(_ifs[12].info, ":");
			_ifs[12].linki("|20406", tplayminute);
			strcat(_ifs[12].info, ":");
			_ifs[12].linki("|20709", tplaysecond);

			sec = Data::data.sLinkDiff(sec, tdiff);

			int tplaytime = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_PLAYTIME), 0);
			itoa(tplaytime, _ifs[10].info, 10);

			int tcleartime = Data::data.iRead(DATA_BINFILE, sec, Data::data.nLinkType(DATAN_CLEARTIME), 0);
			itoa(tcleartime, _ifs[11].info, 10);

			for (int i=0; i<3; i++)
			{
				_ifs[10+i].valueSet(10+i, _ifs[10+i].info, M_CLIENT_CENTER_X, 365+i*25, INFO_BLUE, SEL_NONACTIVE | SEL_STAY);
				infoselect.push_back(_ifs[10+i]);
			}

			InfoSelect::Setup(10, 0);
			InfoSelect::SetPageNum(1, 90, 80);
		}
	}
	else if (tsec == 1)
	{
		if (tdiff < 0)
		{
			InfoSelect::Clear();
			tdiff = Selector::select;
			int i = 0;
			for (vector<spellData>::iterator it = BResource::res.spelldata.begin(); it!= BResource::res.spelldata.end(); it++)
			{
				if ((it->spellflag) & BISF_NOTSPELL)
				{
					continue;
				}
				if (it->battleID == tdiff)
				{
					InfoSelect _ifs;

					int sno = it->sno;
					int tnget = 0;
					int tnmeet = 0;
					LONGLONG tmaxbonus = 0;

					tnget = Data::data.nGet(sno, true);
					tnmeet = Data::data.nMeet(sno, true);
					tmaxbonus = Data::data.nHighScore(sno, 0, true, false);

					strcpy(_ifs.info, M_STAGESTR_PRE);
					_ifs.linki("|20206", Data::data.getSpellNumber(sno));
					strcat(_ifs.info, "|008");
					if(Data::data.raGetIndi(sno))
					{
						strcat(_ifs.info, Data::data.getSpellName(sno));
						strcat(_ifs.info, "(");
						switch (Data::data.getDiffi(sno))
						{
						case M_DIFFI_EASY:
							strcat(_ifs.info, M_DIFFISTR_EASY_S);
							break;
						case M_DIFFI_NORMAL:
							strcat(_ifs.info, M_DIFFISTR_NORMAL_S);
							break;
						case M_DIFFI_HARD:
							strcat(_ifs.info, M_DIFFISTR_HARD_S);
							break;
						case M_DIFFI_DESTINY:
							strcat(_ifs.info, M_DIFFISTR_DESTINY_S);
							break;
						case M_DIFFI_EXTRA_1:
						case M_DIFFI_EXTRA_2:
						case M_DIFFI_EXTRA_3:
						case M_DIFFI_EXTRA_4:
						case M_DIFFI_EXTRA_5:
						case M_DIFFI_EXTRA_6:
							strcat(_ifs.info, M_DIFFISTR_EXTRA_S);
							break;
							break;
						}
						strcat(_ifs.info, ")");
						if(tnget)
							_ifs.coltype = INFO_GREEN;
						else
							_ifs.coltype = INFO_YELLOW;
					}
					else
					{
						strcat(_ifs.info, DATA_DEFAULTSTR_WIDE);
						_ifs.coltype = INFO_GRAY;
					}

					strcat(_ifs.info, "\n");
					_ifs.linki("|10610", tnget);
					strcat(_ifs.info, "/");
					_ifs.linki("|11115", tnmeet);
					strcat(_ifs.info, "(Get/Meet)|035MaxBonus:");
					_ifs.linkl("|14252", tmaxbonus);

					_ifs.valueSet(i, _ifs.info, 80, i*40+160, _ifs.coltype, SEL_NULL);
					infoselect.push_back(_ifs);

					i++;
				}
			}
			InfoSelect::Setup(i, 0);
			InfoSelect::SetPageNum(5, 100, 40);
		}
	}

	scr.SetIntValue(SCR_RESERVEBEGIN, tsec);
	scr.SetIntValue(SCR_RESERVEBEGIN+1, tdiff);
	scr.SetIntValue(SCR_RESERVEBEGIN+2, tsel);
	
	return PGO;
}