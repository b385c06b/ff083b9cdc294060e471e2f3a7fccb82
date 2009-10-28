// ResourcePackDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define M_PACKEXTENSION ".pck"
#define MAX_PATHNUMBER	16


// CResourcePackDlg �Ի���
class CResourcePackDlg : public CDialog
{
// ����
public:
	CResourcePackDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESOURCEPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton packButton;
	CEdit foldernameText;
	TCHAR foldername[M_PATHMAX];
	TCHAR packname[M_PATHMAX];
	TCHAR filterstr[M_PATHMAX];
	TCHAR szPath[M_PATHMAX];
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedPack();
	afx_msg void OnEnChangeFoldername();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CEdit filteredit;
};
