// ResourcePack.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CResourcePackApp:
// �йش����ʵ�֣������ ResourcePack.cpp
//

class CResourcePackApp : public CWinApp
{
public:
	CResourcePackApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CResourcePackApp theApp;
