// CustomDlg.h : ͷ�ļ�
//

#pragma once

#include "Export.h"
#include "ConstResource.h"

// CCustomDlg �Ի���
class CCustomDlg : public CDialog
{
// ����
public:
	CCustomDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CUSTOM_DIALOG };

	void SetDisplay();
	void LoadIniValue();
	void LoadDefaultValue();

	void getScreenmodeValue();
	void getRenderskipValue();

	bool checkValid(bool reporterror = true);
	bool saveFile();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	bool havejoy;
	union{
		struct{
			int keyUp;
			int keyDown;
			int keyLeft;
			int keyRight;
			int keyFire;
			int keySpecial;
			int keySlow;
			int keyChange;
			int keyPause;
			int keySkip;
			int keyEnter;
			int keyEscape;
			int keyCapture;
		};
		int keyKey[13];
	};
	union{
		struct{
			int	joyFire;
			int	joySpecial;
			int	joySlow;
			int	joyChange;
			int	joyPause;
		};
		int		joyKey[5];
	};
	int screenmode;
	int renderskip;

	int defaultwindow;
	int windowleftx;
	int windowtopy;
	int windoww;
	int windowh;
	int windowtopmost;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDefaultwindow();
	afx_msg void OnBnClickedTopmost();
	afx_msg void OnBnClickedFullscreen();
	afx_msg void OnBnClickedWindow();
	afx_msg void OnBnClickedReload();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRenderskip0();
	afx_msg void OnBnClickedRenderskip2();
	afx_msg void OnBnClickedRenderskip3();
	afx_msg void OnEnChangeJoyFire();
	afx_msg void OnEnChangeJoySpecial();
	afx_msg void OnEnChangeJoySlow();
	afx_msg void OnEnChangeJoyBorder();
	afx_msg void OnEnChangeJoyPause();
	afx_msg void OnBnClickedCheck1();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
