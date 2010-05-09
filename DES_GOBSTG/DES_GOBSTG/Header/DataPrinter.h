#ifndef _DATAPRINTER_H
#define _DATAPRINTER_H

#include "MainDependency.h"
#include "Export.h"

#define DPS_SCOREFILENAME	"Score"
#define DPS_EXTENSION		"txt"

#define DPS_SECTION_HEADER		"[Header]\n"
#define DPS_SECTION_TOP10		"[Top10]\n"
#define DPS_SECTION_SPELL		"[SpellCard]\n"
#define DPS_SECTION_TOTAL		"[Total]\n"

#define DPS_SECTION_REPLAYINFO	"[ReplayData]\n"
#define DPS_SECTION_PARTNFO		"[BattleData]\n"
#define DPS_SECTION_REPLAYFRAME	"[ReplayFrame]\n"

#define DPS_COMMENT				"# "
#define DPS_RETURN				"\n"
#define DPS_TABLE_1				"\t"
#define DPS_TABLE_2				"\t\t"
#define DPS_TABLE_3				"\t\t\t"
#define DPS_TABLE_4				"\t\t\t\t"
#define DPS_LEFTANGLEBRACKET	"<"
#define DPS_RIGHTANGLEBRACKET	">"
#define DPS_LEFTBRACKET			"("
#define DPS_RIGHTBRACKET		")"
#define DPS_SLASH				"/"
#define DPS_SPACE				" "
#define DPS_COLON				":"
#define DPS_PERCENT				"%"

#define DPS_SPELLNAME_NFORMAT	40

#define DPS_HEAD_TITLE			"ӛ�h�ƥ����Ȱ� (Dumped Score File)"
#define DPS_HEAD_EXPLAIN_1		"���Υե�����ϡ��F�ڤ�ӛ�h�����פ�����Ǥ��� (This file shows your game data.)"
#define DPS_HEAD_EXPLAIN_2		"���Υե���������ݤ������Ƥ⥲�`��ˤϷ�ӳ����ޤ��� (Changings in this file will not affect game data.)"
#define DPS_HEAD_EXPLAIN_3		"���Υե���������ɤ����ä�����ܞ�d�����ꡢ�e�Υե��`�ޥåȤˤ��Ƥ☋���ޤ��� (You can post these data to anywhere you want.)"
#define DPS_HEAD_TIMESTAMPSIGN	"Time-stamp: "

#define DPS_COMMENTDOUBLELINE	"================================="
#define DPS_COMMENTLINE			"---------------------------------"

#define DPS_TITLE_TOP			"���ӛ�hһ�E (Top 10 Data)"
#define DPS_TITLE_SPELL			"���ڥ륫�`��һ�E		��ͨ��`�ɣ�ȡ��/���飩	������`�ɣ�ȡ��/���飩	(��٥� �ޥå����ܩ`�ʥ�)"
#define DPS_TITLE_SPELL_EX		"SpellCard Data			NormalMode(Get/Meet)	PracticeMode(Get/Meet)	(level	MaxBonus)"
#define DPS_TITLE_TOTAL			"��������״�Bһ�E (Other Data)"

#define DPS_TITLE_REPLAYINFO	"ӛ�h�ǩ`�� (Replay Data)"
#define DPS_TITLE_PARTINFO		"�Хȥ�ǩ`�� (Battle Data)"
#define DPS_TITLE_REPLAYFRAME	"�������Υǩ`�� (Other Data)"

#define DPS_TAG_TOPNUM			"No."
#define DPS_TAG_SPELLPRIFIX		"Spell-"
#define DPS_TAG_PART			"Stage"

#define DPS_TOTAL_RUNTIME		"�t���ӕr�g (Total Run Time)"
#define DPS_TOTAL_GAMETIME		"�t�ץ쥤�r�g (Total Play Time)"
#define DPS_TOTAL_PLAYTIME		"�ץ쥤���� (Total Play Count)"
#define DPS_TOTAL_PLAYTIME_ALL		"Total"
#define DPS_TOTAL_PLAYTIME_ALLCHARA	"ȫ���˹���Ӌ (All Charactors)"
#define DPS_TOTAL_CLEARTIME		"���ꥢ���� (Total Clear Count)"
#define DPS_TOTAL_PRACTICETIME	"�ץ饯�ƥ��� (Total Practice Count)"

#define DPS_TOP_TAGSTR_SPELLLIST				"ȡ�å��ڥ륫�`��һ�E		(�tȡ�û���/�t��������)"
#define DPS_TOP_TAGSTR_SPELLLIST_EX				"Get SpellCard List		(Total Get Count / Total Meet Count)"
#define DPS_RPYINFO_TAGSTR_SPELLLIST			"ȡ�å��ڥ륫�`��һ�E (Get SpellCard List)"
#define DPS_REPLAYFRAME_TAGSTR_AVERAGE			"�tƽ��FPS (Averange FPS)"
#define DPS_REPLAYFRAME_TAGSTR_TOTAL_AVERAGE	"300�ե�`�ऴ��ƽ��FPS (Averange FPS of every 300 frames)"
#define DPS_REPLAYFRAME_AVERAGE_INTERVAL		300

class DataPrinter
{
public:
	DataPrinter();
	~DataPrinter();

	static bool PrintScore();
	static bool WriteString(string * str, FILE * hFile);
	static bool PrintReplayData(const char * foldername, const char * filename);
	static void getHeader();

	static string str;
	static replayInfo rpyinfo;
	static partInfo partinfo[RPYPARTMAX];
	static replayFrame rpyframe[M_SAVEINPUTMAX];
};

extern HGE * hge;

#endif