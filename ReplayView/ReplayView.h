// ReplayView.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CReplayViewApp:
// �йش����ʵ�֣������ ReplayView.cpp
//

class CReplayViewApp : public CWinApp
{
public:
	CReplayViewApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReplayViewApp theApp;
