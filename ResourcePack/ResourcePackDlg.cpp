// ResourcePackDlg.cpp : ʵ���ļ�
//
#include "vld.h"
#include "stdafx.h"
#include "ResourcePack.h"
#include "ResourcePackDlg.h"
#include ".\resourcepackdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STR_MSG_SUCCEED	"Succeeded!"
#define STR_MSG_FAILED	"Failed!"

HGE * hge = hgeCreate(HGE_VERSION);

// CResourcePackDlg �Ի���

CResourcePackDlg::CResourcePackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResourcePackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CResourcePackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDPACK, packButton);
	DDX_Control(pDX, IDC_FOLDERNAME, foldernameText);
	DDX_Control(pDX, IDC_FOLDERNAME2, filteredit);
}

BEGIN_MESSAGE_MAP(CResourcePackDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDPACK, OnBnClickedPack)
	ON_EN_CHANGE(IDC_FOLDERNAME, OnEnChangeFoldername)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CResourcePackDlg ��Ϣ�������

BOOL CResourcePackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	Export::SetIni(true);
	Export::GetPassword();
	packButton.EnableWindow(FALSE);

	strcpy(szPath, "");

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CResourcePackDlg::OnPaint() 
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
HCURSOR CResourcePackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CResourcePackDlg::OnBnClickedOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = m_hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = NULL;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;
	LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);

	if (pidl)
	{
		if (!::SHGetPathFromIDList(pidl, szPath))
			szPath[0]=0; 

		IMalloc * pMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))
		{
			pMalloc-> Free(pidl);
			pMalloc-> Release();
		}
	}

	foldernameText.SetWindowText(szPath);
}

void CResourcePackDlg::OnBnClickedPack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	foldernameText.GetWindowText(szPath, MAX_PATH);

	int i = strlen(szPath) - 1;
	if (szPath[i] == '/' || szPath[i] == '\\')
	{
		szPath[i] = 0;
		i--;
	}
	while (i > 0 && szPath[i] != '/' && szPath[i] != '\\')
	{
		i--;
	}
	if (i == 0)
	{
		return;
	}
	i++;

	TCHAR upperfolder[M_PATHMAX];
	strncpy(upperfolder, szPath, i);
	upperfolder[i] = 0;
	hge->Resource_SetPath(upperfolder);
	SetCurrentDirectory(upperfolder);
	strcpy(foldername, &szPath[i]);
	strcpy(packname, &szPath[i]);
	strcat(packname, M_PACKEXTENSION);

	filteredit.GetWindowText(filterstr, MAX_PATH);

	if (Export::packFolder(packname, foldername, filterstr))
	{
		MessageBox(STR_MSG_SUCCEED);
		OnOK();
	}
	else
	{
		MessageBox(STR_MSG_FAILED);
	}
}

void CResourcePackDlg::OnEnChangeFoldername()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	foldernameText.GetWindowText(szPath, MAX_PATH);
	if (strlen(szPath) == 0)
	{
		packButton.EnableWindow(FALSE);
	}
	else if (!_access(hge->Resource_MakePath(szPath), 00))
	{
		packButton.EnableWindow(TRUE);
	}
	else
	{
		packButton.EnableWindow(FALSE);
	}
}

BOOL CResourcePackDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}
