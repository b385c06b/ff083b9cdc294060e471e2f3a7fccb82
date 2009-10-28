// ResourceUnpackDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

// CResourceUnpackDlg �Ի���
class CResourceUnpackDlg : public CDialog
{
// ����
public:
	CResourceUnpackDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESOURCEUNPACK_DIALOG };

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
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedUnpack();
public:
	TCHAR szIniFilename[M_PATHMAX];
	CEdit foldernameText;
	CButton unpackButton;
	afx_msg void OnEnChangeFoldername();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedUnpack2();
};
