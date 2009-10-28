// CustomDlg.cpp : 实现文件
//

#include "vld.h"
#include "stdafx.h"
#include "Custom.h"
#include "CustomDlg.h"
#include ".\customdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STR_MSG_INVALID			"Invalid value!"
#define STR_MSG_KEYUSED			"This key has been used!"
#define STR_MSG_RESET			"Are you sure to reset?"
#define STR_MSG_RESETDEFAULT	"Are you sure to reset to default?"
#define STR_MSG_CLOSE			"Are you sure to close?"

HGE * hge = hgeCreate(HGE_VERSION);

// CCustomDlg 对话框

struct dikvkname
{
	int dik;
	int vk;
	char name[M_STRMAX];
};

struct dikvkname dvn[] = 
{
	{DIK_ESCAPE,	VK_ESCAPE,		"Escape"},
	{DIK_1,			'1',			"1"},
	{DIK_2,			'2',			"2"},
	{DIK_3,			'3',			"3"},
	{DIK_4,			'4',			"4"},
	{DIK_5,			'5',			"5"},
	{DIK_6,			'6',			"6"},
	{DIK_7,			'7',			"7"},
	{DIK_8,			'8',			"8"},
	{DIK_9,			'9',			"9"},
	{DIK_0,			'0',			"0"},
	{DIK_MINUS,		VK_OEM_MINUS,	"-"},
	{DIK_EQUALS,	VK_OEM_PLUS,	"="},
	{DIK_BACK,		VK_BACK,		"BackSpace"},
	{DIK_TAB,		VK_TAB,			"Tab"},
	{DIK_Q,			'Q',			"Q"},
	{DIK_W,			'W',			"W"},
	{DIK_E,			'E',			"E"},
	{DIK_R,			'R',			"R"},
	{DIK_T,			'T',			"T"},
	{DIK_Y,			'Y',			"Y"},
	{DIK_U,			'U',			"U"},
	{DIK_I,			'I',			"I"},
	{DIK_O,			'O',			"O"},
	{DIK_P,			'P',			"P"},
	{DIK_LBRACKET,	VK_OEM_4,		"["},
	{DIK_RBRACKET,	VK_OEM_6,		"]"},

	{DIK_NUMPADENTER,	VK_RETURN,	"NumEnter"},
	{DIK_RETURN,	VK_RETURN,		"Enter"},

	{DIK_LCONTROL,	VK_LCONTROL,	"LControl"},
	{DIK_RCONTROL,	VK_CONTROL,		"RControl"},

	{DIK_A,			'A',			"A"},
	{DIK_S,			'S',			"S"},
	{DIK_D,			'D',			"D"},
	{DIK_F,			'F',			"F"},
	{DIK_G,			'G',			"G"},
	{DIK_H,			'H',			"H"},
	{DIK_J,			'J',			"J"},
	{DIK_K,			'K',			"K"},
	{DIK_L,			'L',			"L"},
	{DIK_SEMICOLON,	VK_OEM_1,		";"},
	{DIK_APOSTROPHE,VK_OEM_7,		"\'"},
	{DIK_GRAVE,		VK_OEM_3,		"`"},
	{DIK_LSHIFT,	VK_LSHIFT,		"LShift"},
	{DIK_BACKSLASH,	VK_OEM_5,		"\\"},
	{DIK_Z,			'Z',			"Z"},
	{DIK_X,			'X',			"X"},
	{DIK_C,			'C',			"C"},
	{DIK_V,			'V',			"V"},
	{DIK_B,			'B',			"B"},
	{DIK_N,			'N',			"N"},
	{DIK_M,			'M',			"M"},
	{DIK_COMMA,		VK_OEM_COMMA,	","},
	{DIK_PERIOD,	VK_OEM_PERIOD,	"."},
	{DIK_SLASH,		VK_OEM_2,		"/"},
	{DIK_RSHIFT,	VK_RSHIFT,		"RShift"},
	{DIK_MULTIPLY,	VK_MULTIPLY,	"Num*"},
	{DIK_LMENU,		VK_LMENU,		"LAlt"},

	{DIK_RMENU,		VK_MENU,		"RAlt"},

	{DIK_SPACE,		VK_SPACE,		"Space"},
	{DIK_CAPITAL,	VK_CAPITAL,		"Caps"},
	{DIK_F1,		VK_F1,			"F1"},
	{DIK_F2,		VK_F2,			"F2"},
	{DIK_F3,		VK_F3,			"F3"},
	{DIK_F4,		VK_F4,			"F4"},
	{DIK_F5,		VK_F5,			"F5"},
	{DIK_F6,		VK_F6,			"F6"},
	{DIK_F7,		VK_F7,			"F7"},
	{DIK_F8,		VK_F8,			"F8"},
	{DIK_F9,		VK_F9,			"F9"},
	{DIK_F10,		VK_F10,			"F10"},

	{DIK_NUMLOCK,	VK_NUMLOCK,		"NumLock"},

	{DIK_SCROLL,	VK_SCROLL,		"Scroll"},
	{DIK_NUMPAD7,	VK_NUMPAD7,		"Num7"},
	{DIK_NUMPAD8,	VK_NUMPAD8,		"Num8"},
	{DIK_NUMPAD9,	VK_NUMPAD9,		"Num9"},
	{DIK_SUBTRACT,	VK_SUBTRACT,	"Num-"},
	{DIK_NUMPAD4,	VK_NUMPAD4,		"Num4"},
	{DIK_NUMPAD5,	VK_NUMPAD5,		"Num5"},
	{DIK_NUMPAD6,	VK_NUMPAD6,		"Num6"},
	{DIK_ADD,		VK_ADD,			"Num+"},
	{DIK_NUMPAD1,	VK_NUMPAD1,		"Num1"},
	{DIK_NUMPAD2,	VK_NUMPAD2,		"Num2"},
	{DIK_NUMPAD3,	VK_NUMPAD3,		"Num3"},
	{DIK_NUMPAD0,	VK_NUMPAD0,		"Num0"},
	{DIK_DECIMAL,	VK_DECIMAL,		"Num."},
	{DIK_OEM_102,	VK_OEM_102,		"OEM_102"},
	{DIK_F11,		VK_F11,			"F11"},
	{DIK_F12,		VK_F12,			"F12"},
	{DIK_F13,		VK_F13,			"F13"},
	{DIK_F14,		VK_F14,			"F14"},
	{DIK_F15,		VK_F15,			"F15"},
	{DIK_KANA,		VK_KANA,		"Kana"},
	{DIK_ABNT_C1,	-1,			"??"},
	{DIK_CONVERT,	VK_CONVERT,		"Convert"},
	{DIK_NOCONVERT,	VK_NONCONVERT,	"NoConvert"},
	{DIK_YEN,		-1,			"??"},
	{DIK_ABNT_C2,	-1,			"??"},
	{DIK_NUMPADEQUALS,	VK_OEM_NEC_EQUAL,	"Num="},
	{DIK_PREVTRACK,	VK_MEDIA_PREV_TRACK,	"Prev"},
	{DIK_AT,		-1,			"??"},
	{DIK_COLON,		-1,			"??"},
	{DIK_UNDERLINE,	-1,			"??"},
	{DIK_KANJI,		VK_KANJI,		"Kanji"},
	{DIK_STOP,		-1,			"??"},
	{DIK_AX,		-1,			"??"},
	{DIK_UNLABELED,	-1,			"??"},
	{DIK_NEXTTRACK,	VK_MEDIA_NEXT_TRACK,	"Next"},

	{DIK_RCONTROL,	VK_RCONTROL,	"RControl"},

	{DIK_RCONTROL,	VK_RCONTROL,	"RControl"},

	{DIK_MUTE,		VK_VOLUME_MUTE,	"Mute"},
	{DIK_CALCULATOR,	-1,		"??"},
	{DIK_PLAYPAUSE,	VK_MEDIA_PLAY_PAUSE,	"PlayPause"},
	{DIK_MEDIASTOP,	VK_MEDIA_STOP,	"MediaStop"},
	{DIK_VOLUMEDOWN,	VK_VOLUME_DOWN,	"VolDown"},
	{DIK_VOLUMEUP,	VK_VOLUME_UP,	"VolUp"},
	{DIK_WEBHOME,	VK_BROWSER_HOME,	"WebHome"},
	{DIK_NUMPADCOMMA,	-1,		"??"},
	{DIK_DIVIDE,	VK_DIVIDE,		"Num/"},
	{DIK_SYSRQ,		VK_SNAPSHOT,	"PrintScreen"},
	{DIK_RMENU,		VK_RMENU,		"RMenu"},
	{DIK_PAUSE,		VK_PAUSE,		"Pause"},
	{DIK_HOME,		VK_HOME,		"Home"},
	{DIK_UP,		VK_UP,			"Up"},
	{DIK_PRIOR,		VK_PRIOR,		"PageUp"},
	{DIK_LEFT,		VK_LEFT,		"Left"},
	{DIK_RIGHT,		VK_RIGHT,		"Right"},
	{DIK_END,		VK_END,			"End"},
	{DIK_DOWN,		VK_DOWN,		"Down"},
	{DIK_NEXT,		VK_NEXT,		"PageDown"},
	{DIK_INSERT,	VK_INSERT,		"Insert"},
	{DIK_DELETE,	VK_DELETE,		"Delete"},
	{DIK_LWIN,		VK_LWIN,		"LWin"},
	{DIK_RWIN,		VK_RWIN,		"RWin"},
	{DIK_APPS,		VK_APPS,		"Apps"},
	{DIK_POWER,		-1,			"??"},
	{DIK_SLEEP,		VK_SLEEP,		"Sleep"},
	{DIK_WAKE,		-1,			"??"},
	{DIK_WEBSEARCH,	VK_BROWSER_SEARCH,	"WebSearch"},
	{DIK_WEBFAVORITES,	VK_BROWSER_FAVORITES,	"WebFavorites"},
	{DIK_WEBREFRESH,	VK_BROWSER_REFRESH,	"WebRefresh"},
	{DIK_WEBSTOP,	VK_BROWSER_STOP,	"WebStop"},
	{DIK_WEBFORWARD,	VK_BROWSER_FORWARD,	"WebForward"},
	{DIK_WEBBACK,	VK_BROWSER_BACK,	"WebBack"},
	{DIK_MYCOMPUTER,	-1,		"??"},
	{DIK_MAIL,		VK_LAUNCH_MAIL,	"Mail"},
	{DIK_MEDIASELECT,	VK_LAUNCH_MEDIA_SELECT,	"MediaSelect"},
	{-1,			-1,			""}
};



CCustomDlg::CCustomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CCustomDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DEFAULTWINDOW, OnBnClickedDefaultwindow)
	ON_BN_CLICKED(IDC_TOPMOST, OnBnClickedTopmost)
	ON_BN_CLICKED(IDC_FULLSCREEN, OnBnClickedFullscreen)
	ON_BN_CLICKED(IDC_WINDOW, OnBnClickedWindow)
	ON_BN_CLICKED(IDC_RELOAD, OnBnClickedReload)
	ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RENDERSKIP_0, OnBnClickedRenderskip0)
	ON_BN_CLICKED(IDC_RENDERSKIP_2, OnBnClickedRenderskip2)
	ON_BN_CLICKED(IDC_RENDERSKIP_3, OnBnClickedRenderskip3)
	ON_EN_CHANGE(IDC_JOY_FIRE, OnEnChangeJoyFire)
	ON_EN_CHANGE(IDC_JOY_SPECIAL, OnEnChangeJoySpecial)
	ON_EN_CHANGE(IDC_JOY_SLOW, OnEnChangeJoySlow)
	ON_EN_CHANGE(IDC_JOY_CHANGE, OnEnChangeJoyBorder)
	ON_EN_CHANGE(IDC_JOY_PAUSE, OnEnChangeJoyPause)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

void CCustomDlg::LoadIniValue()
{
	keyUp			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP);
	keyDown			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN);
	keyLeft			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT);
	keyRight		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT);
	keyFire			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE);
	keySpecial			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSPECIAL, RESCONFIGDEFAULT_KEYSPECIAL);
	keySlow			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW);
	keyChange		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCHANGE, RESCONFIGDEFAULT_KEYCHANGE);
	keyPause		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
	keySkip			= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
	keyEnter		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
	keyEscape		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
	keyCapture		= hge->Ini_GetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);

	joyFire			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
	joySpecial		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSPECIAL, RESCONFIGDEFAULT_JOYSPECIAL);
	joySlow			= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
	joyChange		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCHANGE, RESCONFIGDEFAULT_JOYCHANGE);
	joyPause		= hge->Ini_GetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);

	screenmode		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
	renderskip		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);

	defaultwindow	= hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, RESCONFIGDEFAULT_DEFAULTWINDOW);
	windowleftx		= hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, RESCONFIGDEFAULT_WINDOWLEFT);
	windowtopy		= hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, RESCONFIGDEFAULT_WINDOWTOP);
	windoww			= hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, RESCONFIGDEFAULT_WINDOWWIDTH);
	windowh			= hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, RESCONFIGDEFAULT_WINDOWHEIGHT);
	windowtopmost	= hge->Ini_GetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, RESCONFIGDEFAULT_WINDOWTOPMOST);

	if (!checkValid(false))
	{
		LoadDefaultValue();
		saveFile();
	}
}

void CCustomDlg::LoadDefaultValue()
{
	keyUp			= RESCONFIGDEFAULT_KEYUP;
	keyDown			= RESCONFIGDEFAULT_KEYDOWN;
	keyLeft			= RESCONFIGDEFAULT_KEYLEFT;
	keyRight		= RESCONFIGDEFAULT_KEYRIGHT;
	keyFire			= RESCONFIGDEFAULT_KEYFIRE;
	keySpecial			= RESCONFIGDEFAULT_KEYSPECIAL;
	keySlow			= RESCONFIGDEFAULT_KEYSLOW;
	keyChange		= RESCONFIGDEFAULT_KEYCHANGE;
	keyPause		= RESCONFIGDEFAULT_KEYPAUSE;
	keySkip			= RESCONFIGDEFAULT_KEYSKIP;
	keyEnter		= RESCONFIGDEFAULT_KEYENTER;
	keyEscape		= RESCONFIGDEFAULT_KEYESCAPE;
	keyCapture		= RESCONFIGDEFAULT_KEYCAPTURE;

	joyFire			= RESCONFIGDEFAULT_JOYFIRE;
	joySpecial			= RESCONFIGDEFAULT_JOYSPECIAL;
	joySlow			= RESCONFIGDEFAULT_JOYSLOW;
	joyChange		= RESCONFIGDEFAULT_JOYCHANGE;
	joyPause		= RESCONFIGDEFAULT_JOYPAUSE;

	screenmode		= RESCONFIGDEFAULT_SCREENMODE;
	renderskip		= RESCONFIGDEFAULT_RENDERSKIP;

	defaultwindow	= RESCONFIGDEFAULT_DEFAULTWINDOW;
	windowleftx		= RESCONFIGDEFAULT_WINDOWLEFT;
	windowtopy		= RESCONFIGDEFAULT_WINDOWTOP;
	windoww			= RESCONFIGDEFAULT_WINDOWWIDTH;
	windowh			= RESCONFIGDEFAULT_WINDOWHEIGHT;
	windowtopmost	= RESCONFIGDEFAULT_WINDOWTOPMOST;
}

bool CCustomDlg::checkValid(bool reporterror)
{
	for (int i=0; i<13; i++)
	{
		if (keyKey[i] < 0 || keyKey[i] >= M_KEYKEYMAX)
		{
			if (reporterror)
			{
				MessageBox(STR_MSG_INVALID);
				GetDlgItem(IDC_KEY_UP + i)->SetFocus();
			}
			return false;
		}
		for (int j=i+1; j<13; j++)
		{
			if (keyKey[i] == keyKey[j])
			{
				if (reporterror)
				{
					MessageBox(STR_MSG_KEYUSED);
					GetDlgItem(IDC_KEY_UP + i)->SetFocus();
				}
				return false;
			}
		}
	}
	for (int i=0; i<5; i++)
	{
		if(joyKey[i] < 0 || joyKey[i] >= M_JOYKEYMAX)
		{
			if (reporterror)
			{
				MessageBox(STR_MSG_INVALID);
				GetDlgItem(IDC_JOY_FIRE + i)->SetFocus();
			}
			return false;
		}
		for (int j=i+1; j<5; j++)
		{
			if (joyKey[i] == joyKey[j])
			{
				if (reporterror)
				{
					MessageBox(STR_MSG_KEYUSED);
					GetDlgItem(IDC_JOY_FIRE + i)->SetFocus();
				}
				return false;
			}
		}
	}
	if(screenmode < 0 || screenmode > 1)
		screenmode = 1;
	if (renderskip < 2)
	{
		renderskip = 0;
	}
	else if (renderskip > 3)
	{
		renderskip = 3;
	}
	defaultwindow = defaultwindow ? 1 : 0;
	windowtopmost = windowtopmost ? 1 : 0;
	return true;
}

bool CCustomDlg::saveFile()
{
	if (checkValid())
	{
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYUP, keyUp);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYDOWN, keyDown);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYLEFT, keyLeft);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYRIGHT, keyRight);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYFIRE, keyFire);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSPECIAL, keySpecial);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSLOW, keySlow);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCHANGE, keyChange);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYPAUSE, keyPause);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYSKIP, keySkip);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYENTER, keyEnter);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYESCAPE, keyEscape);
		hge->	Ini_SetInt(RESCONFIGS_KEYSETTING, RESCONFIGN_KEYCAPTURE, keyCapture);

		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYFIRE, joyFire);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSPECIAL, joySpecial);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYSLOW, joySlow);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYCHANGE, joyChange);
		hge->	Ini_SetInt(RESCONFIGS_JOYSETTING, RESCONFIGN_JOYPAUSE, joyPause);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, screenmode);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, renderskip);

		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, defaultwindow);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, windowleftx);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, windowtopy);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, windoww);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, windowh);
		hge->	Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, windowtopmost);

		return true;
	}
	return false;
}

void CCustomDlg::SetDisplay()
{
	TCHAR buffer[M_STRITOAMAX];
	for (int i=0; i<13; i++)
	{
		int j = 0;
		while (dvn[j].dik != -1)
		{
			if (dvn[j].dik == keyKey[i])
			{
				SetDlgItemText(IDC_KEY_UP + i, dvn[j].name);
				break;
			}
			j++;
		}
	}
	for (int i=0; i<5; i++)
	{
		SetDlgItemText(IDC_JOY_FIRE + i, itoa(joyKey[i], buffer, 10));
	}

	CheckRadioButton(IDC_FULLSCREEN, IDC_WINDOW, screenmode ? IDC_FULLSCREEN : IDC_WINDOW);

	if (renderskip)
	{
		CheckRadioButton(IDC_RENDERSKIP_0, IDC_RENDERSKIP_3, IDC_RENDERSKIP_0 + renderskip - 1);
	}
	else
	{
		CheckRadioButton(IDC_RENDERSKIP_0, IDC_RENDERSKIP_3, IDC_RENDERSKIP_0);
	}

	if (defaultwindow)
	{
		CheckDlgButton(IDC_DEFAULTWINDOW, BST_CHECKED);

		GetDlgItem(IDC_TOPMOST)->EnableWindow(FALSE);
		GetDlgItem(IDC_LEFTX)->EnableWindow(FALSE);
		GetDlgItem(IDC_TOPY)->EnableWindow(FALSE);
		GetDlgItem(IDC_WINDOWW)->EnableWindow(FALSE);
		GetDlgItem(IDC_WINDOWH)->EnableWindow(FALSE);
	}
	else
	{
		CheckDlgButton(IDC_DEFAULTWINDOW, BST_UNCHECKED);

		GetDlgItem(IDC_TOPMOST)->EnableWindow(TRUE);
		GetDlgItem(IDC_LEFTX)->EnableWindow(TRUE);
		GetDlgItem(IDC_TOPY)->EnableWindow(TRUE);
		GetDlgItem(IDC_WINDOWW)->EnableWindow(TRUE);
		GetDlgItem(IDC_WINDOWH)->EnableWindow(TRUE);
	}

	CheckDlgButton(IDC_TOPMOST, windowtopmost ? BST_CHECKED : BST_UNCHECKED);
	SetDlgItemText(IDC_LEFTX, itoa(windowleftx, buffer, 10));
	SetDlgItemText(IDC_TOPY, itoa(windowtopy, buffer, 10));
	SetDlgItemText(IDC_WINDOWW, itoa(windoww, buffer, 10));
	SetDlgItemText(IDC_WINDOWH, itoa(windowh, buffer, 10));
}

// CCustomDlg 消息处理程序

BOOL CCustomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	Export::clientInitial(false, true);
	SetWindowText(hge->System_GetState(HGE_TITLE));
	Export::SetIni(true);

	JOYINFO joyinfo;
	UINT wNumDevs, wDeviceID;

	havejoy = true;

	if((wNumDevs = joyGetNumDevs()) == 0)
		havejoy = false;
	if (joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_NOERROR)
	{
		havejoy = false;
	}

	if (!havejoy)
	{
		for (int i=0; i<5; i++)
		{
			GetDlgItem(IDC_JOY_FIRE + i)->EnableWindow(FALSE);
		}
	}

	LoadIniValue();

	SetDisplay();

	SetDlgItemText(IDC_FILENAME, hge->System_GetState(HGE_INIFILE));

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCustomDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CCustomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CCustomDlg::PreTranslateMessage(MSG* pMsg)
{
	CWnd * wndinput[18];
	CWnd * wndnow = GetFocus();
	bool checkinput = false;
	int idcnow;
	int keyid;

	for (int i=0; i<13; i++)
	{
		idcnow = IDC_KEY_UP + i;
		wndinput[i] = GetDlgItem(idcnow);
		if (wndinput[i] == wndnow)
		{
			keyid = i;
			checkinput = true;
			break;
		}
	}

	if (checkinput)
	{
		if (WM_KEYDOWN == pMsg->message || WM_SYSKEYDOWN == pMsg->message)
		{
			int i = 0;
			int lparam = pMsg->lParam;
			int wparam = pMsg->wParam;
			while (dvn[i].dik != -1)
			{
				if (!(lparam & 0x01000000))
				{
					if (dvn[i].dik == ((lparam >> 16) & 0xff))
					{
						break;
					}
				}
				else
				{
					if (dvn[i].vk == wparam)
					{
						break;
					}
				}
				i++;
			}

			CWnd::SetDlgItemText(idcnow, dvn[i].name);
			keyKey[keyid] = dvn[i].dik;

			GetDlgItem(idcnow + 1)->SetFocus();
			SetDisplay();
			return true;
		}
	}
	/*
	else
	{
		for (int i=0; i<5; i++)
		{
			idcnow = IDC_JOY_FIRE + i;
			wndinput[13+i] = GetDlgItem(idcnow);
			if (wndinput[13+i] == wndnow)
			{
				checkinput = true;
				break;
			}
		}
		if (checkinput)
		{
			if (WM_PAINT == pMsg->message)
			{
				if (havejoy)
				{
					JOYINFOEX joyinfoex;
					joyinfoex.dwSize = sizeof(joyinfoex);
					joyinfoex.dwFlags = JOY_RETURNBUTTONS;
					if (joyGetPosEx(JOYSTICKID1, &joyinfoex) == JOYERR_NOERROR)
					{
						if (joyinfoex.dwButtons)
						{
							char buffer[M_STRMAX];
							SetDlgItemText(IDC_JOY_FIRE, itoa(joyinfoex.dwButtonNumber, buffer, 10));
							return true;
						}
					}
				}
			}
		}
	}
	*/

	return false;
}

void CCustomDlg::getScreenmodeValue()
{
	screenmode = (int)(GetCheckedRadioButton(IDC_FULLSCREEN, IDC_WINDOW) == IDC_FULLSCREEN);
}

void CCustomDlg::getRenderskipValue()
{
	switch (GetCheckedRadioButton(IDC_RENDERSKIP_0, IDC_RENDERSKIP_3))
	{
	case IDC_RENDERSKIP_0:
		renderskip = 0;
		break;
	case IDC_RENDERSKIP_2:
		renderskip = 2;
		break;
	case IDC_RENDERSKIP_3:
		renderskip = 3;
		break;
	}
}

void CCustomDlg::OnBnClickedDefaultwindow()
{
	// TODO: 在此添加控件通知处理程序代码
	defaultwindow = IsDlgButtonChecked(IDC_DEFAULTWINDOW);
	SetDisplay();
}

void CCustomDlg::OnBnClickedTopmost()
{
	// TODO: 在此添加控件通知处理程序代码
	windowtopmost = IsDlgButtonChecked(IDC_TOPMOST);
	SetDisplay();
}


void CCustomDlg::OnBnClickedFullscreen()
{
	// TODO: 在此添加控件通知处理程序代码
	getScreenmodeValue();
	SetDisplay();
}


void CCustomDlg::OnBnClickedWindow()
{
	// TODO: 在此添加控件通知处理程序代码
	getScreenmodeValue();
	SetDisplay();
}

void CCustomDlg::OnBnClickedReload()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(STR_MSG_RESET, NULL, MB_OKCANCEL) == IDOK)
	{
		LoadIniValue();
		SetDisplay();
	}
}

void CCustomDlg::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(STR_MSG_RESETDEFAULT, NULL, MB_OKCANCEL) == IDOK)
	{
		LoadDefaultValue();
		SetDisplay();
	}
}

void CCustomDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(STR_MSG_CLOSE, NULL, MB_OKCANCEL) == IDOK)
	{
		OnCancel();
	}
}

void CCustomDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (saveFile())
	{
		OnOK();
	}
}

void CCustomDlg::OnBnClickedRenderskip0()
{
	// TODO: 在此添加控件通知处理程序代码
	getRenderskipValue();
	SetDisplay();
}

void CCustomDlg::OnBnClickedRenderskip2()
{
	// TODO: 在此添加控件通知处理程序代码
	getRenderskipValue();
	SetDisplay();
}

void CCustomDlg::OnBnClickedRenderskip3()
{
	// TODO: 在此添加控件通知处理程序代码
	getRenderskipValue();
	SetDisplay();
}

void CCustomDlg::OnEnChangeJoyFire()
{
	TCHAR buffer[M_STRMAX];
	GetDlgItemText(IDC_JOY_FIRE, buffer, M_STRITOAMAX);
	joyFire = atoi(buffer);
}

void CCustomDlg::OnEnChangeJoySpecial()
{
	TCHAR buffer[M_STRMAX];
	GetDlgItemText(IDC_JOY_SPECIAL, buffer, M_STRITOAMAX);
	joySpecial = atoi(buffer);
}

void CCustomDlg::OnEnChangeJoySlow()
{
	TCHAR buffer[M_STRMAX];
	GetDlgItemText(IDC_JOY_SLOW, buffer, M_STRITOAMAX);
	joySlow = atoi(buffer);
}

void CCustomDlg::OnEnChangeJoyBorder()
{
	TCHAR buffer[M_STRMAX];
	GetDlgItemText(IDC_JOY_CHANGE, buffer, M_STRITOAMAX);
	joyChange = atoi(buffer);
}

void CCustomDlg::OnEnChangeJoyPause()
{
	TCHAR buffer[M_STRMAX];
	GetDlgItemText(IDC_JOY_PAUSE, buffer, M_STRITOAMAX);
	joyPause = atoi(buffer);
}

void CCustomDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR buffer[M_STRMAX];
	GetDlgItemText(IDC_JOY_PAUSE, buffer, M_STRITOAMAX);
	joyPause = atoi(buffer);
}


BOOL CCustomDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}
