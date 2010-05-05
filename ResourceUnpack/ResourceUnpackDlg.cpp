// ResourceUnpackDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResourceUnpack.h"
#include "ResourceUnpackDlg.h"
#include ".\resourceunpackdlg.h"

#define STR_MSG_SUCCEED	"Succeeded!"
#define STR_MSG_FAILED	"Failed!"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HGE * hge = hgeCreate(HGE_VERSION);

// CResourceUnpackDlg �Ի���



CResourceUnpackDlg::CResourceUnpackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResourceUnpackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CResourceUnpackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOLDERNAME, foldernameText);
	DDX_Control(pDX, IDC_UNPACK, unpackButton);
}

BEGIN_MESSAGE_MAP(CResourceUnpackDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDC_UNPACK, OnBnClickedUnpack)
	ON_EN_CHANGE(IDC_FOLDERNAME, OnEnChangeFoldername)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_UNPACK2, OnBnClickedUnpack2)
END_MESSAGE_MAP()


// CResourceUnpackDlg ��Ϣ�������

BOOL CResourceUnpackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	Export::SetIni(true);
	hge->System_SetState(HGE_LOGFILE, LOG_STR_FILENAME);
	Export::GetPassword();
	unpackButton.EnableWindow(FALSE);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CResourceUnpackDlg::OnPaint() 
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
HCURSOR CResourceUnpackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CResourceUnpackDlg::OnBnClickedOpen()
{
	CFileDialog fdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ini Files (*.ini)|*.ini||");
	CString filename;
	if (fdlg.DoModal() == IDOK)
	{
		filename = fdlg.GetPathName();
	}
	else
	{
		return;
	}
	strcpy(szIniFilename, filename);
	if (!(strlen(szIniFilename)))
	{
		return;
	}

	foldernameText.SetWindowText(szIniFilename);

}

void CResourceUnpackDlg::OnBnClickedUnpack()
{
	foldernameText.GetWindowText(szIniFilename, M_STRMAX);

	int i = strlen(szIniFilename) - 1;
	while (i > 0 && szIniFilename[i] != '/' && szIniFilename[i] != '\\')
	{
		i--;
	}
	if (i == 0)
	{
		return;
	}
	i++;

	TCHAR upperfolder[M_PATHMAX];
	strncpy(upperfolder, szIniFilename, i);
	upperfolder[i] = 0;
	hge->Resource_SetPath(upperfolder);
	hge->Resource_SetCurrentDirectory(upperfolder);

	if (Export::unpackFromIni(szIniFilename))
	{
		MessageBox(STR_MSG_SUCCEED);
		OnOK();
	}
	else
	{
		MessageBox(STR_MSG_FAILED);
	}
}

void CResourceUnpackDlg::OnEnChangeFoldername()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	foldernameText.GetWindowText(szIniFilename, MAX_PATH);
	if (!strlen(szIniFilename))
	{
		unpackButton.EnableWindow(FALSE);
	}
	else if (!hge->Resource_AccessFile(szIniFilename))
	{
		unpackButton.EnableWindow(TRUE);
	}
	else
	{
		unpackButton.EnableWindow(FALSE);
	}
}

BOOL CResourceUnpackDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}

void CResourceUnpackDlg::OnBnClickedUnpack2()
{
	OnCancel();
}
