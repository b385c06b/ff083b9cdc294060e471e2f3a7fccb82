// ReplayViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "vld.h"
#include "ReplayView.h"
#include "ReplayViewDlg.h"
#include ".\replayviewdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HGE * hge = hgeCreate(HGE_VERSION);


// CReplayViewDlg �Ի���



CReplayViewDlg::CReplayViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplayViewDlg::IDD, pParent)
	, isChecked(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReplayViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECKBOX, isChecked);
	DDX_Control(pDX, IDC_CHECKBOX, checkButton);
	DDX_Control(pDX, IDC_FILENAME, filenameText);
	DDX_Control(pDX, IDC_MAINTEXT, mainText);
}

BEGIN_MESSAGE_MAP(CReplayViewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CReplayViewDlg ��Ϣ�������

BOOL CReplayViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	strcpy(cFilename, "");
	strcpy(cOutputFilename, "");

	CWnd::DragAcceptFiles();

	Export::clientInitial(false, true);
	SetWindowText(hge->System_GetState(HGE_TITLE));
	data.GetIni();
	Export::GetResourceFile(true);
	res.CopyData();
	res.Gain();
	checkButton.SetCheck(BST_CHECKED);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CReplayViewDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CReplayViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReplayViewDlg::Release()
{
	isChecked = checkButton.GetCheck();
	if (isChecked == BST_CHECKED)
	{
		if (strlen(cOutputFilename))
		{
			DeleteFile(cOutputFilename);
		}
	}
}

bool CReplayViewDlg::InitFile()
{
	Release();
	TCHAR foldername[M_PATHMAX];
	TCHAR filename[M_PATHMAX];
	strcpy(foldername, "");
	strcpy(filename, "");
	int i = strlen(cFilename);
	while(i > 0 && cFilename[i] != '/' && cFilename[i] != '\\')
	{
		i--;
	}
	if (i == 0)
	{
		return false;
	}
	i++;
	strncpy(foldername, cFilename, i);
	foldername[i] = 0;
	strcpy(filename, &cFilename[i]);
	if (!DataPrinter::PrintReplayData(foldername, filename))
	{
		return false;
	}
	strcpy(cOutputFilename, cFilename);
	while (cOutputFilename[strlen(cOutputFilename)-1] != '.')
	{
		cOutputFilename[strlen(cOutputFilename)-1] = 0;
	}
	strcat(cOutputFilename, DPS_EXTENSION);
	return DisplayInfo();
}

bool CReplayViewDlg::DisplayInfo()
{
	if (!strlen(cOutputFilename))
	{
		return false;
	}
	FILE * file = fopen(cOutputFilename, "r");
	if (!file)
	{
		return false;
	}

	string str = "";
	TCHAR buffer[M_STRMAX];

	bool bstart = false;
	while(!feof(file))
	{
		if (!bstart)
		{
			str = fgets(buffer, M_STRMAX, file);
			if (str.find(DPS_SECTION_REPLAYINFO) != string::npos)
			{
				str += "\r\n";
				bstart = true;
			}
		}
		else
		{
			str += fgets(buffer, M_STRMAX, file);
			str += "\r\n";
		}
	}

	fclose(file);
	file = NULL;

	SetDlgItemText(IDC_FILENAME, cFilename);
	SetDlgItemText(IDC_MAINTEXT, str.data());
	return true;
}

void CReplayViewDlg::OnDropFiles(HDROP hDropInfo)
{
	DragQueryFile(hDropInfo, -1, cFilename, sizeof(cFilename));
	DragQueryFile(hDropInfo, 0, cFilename, sizeof(cFilename));

	DragFinish(hDropInfo);

	InitFile();
}

void CReplayViewDlg::OnBnClickedOpen()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Replay file (*.rpy) | *.rpy"));

	if(dlg.DoModal() == IDOK)
	{
		strcpy(cFilename, dlg.GetPathName());
		InitFile();
	}
}

void CReplayViewDlg::OnBnClickedCancel()
{
	Release();
	OnCancel();
}

BOOL CReplayViewDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}
