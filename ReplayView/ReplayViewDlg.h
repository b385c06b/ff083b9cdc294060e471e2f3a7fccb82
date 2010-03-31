// ReplayViewDlg.h : ͷ�ļ�
//

#pragma once

#include "../DES_GOBSTG/DES_GOBSTG/Header/DataPrinter.h"
#include "Resource.h"
#include "afxwin.h"

// CReplayViewDlg �Ի���
class CReplayViewDlg : public CDialog
{
// ����
public:
	CReplayViewDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REPLAYVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
public:
	BOOL isChecked;
	CButton checkButton;
	CEdit filenameText;
	CEdit mainText;

	TCHAR cFilename[M_PATHMAX];
	TCHAR cOutputFilename[M_PATHMAX];
	afx_msg void OnBnClickedOpen();

	void Release();
	bool DisplayInfo();
	bool InitFile();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
