// ResourceUnpack.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CResourceUnpackApp:
// �йش����ʵ�֣������ ResourceUnpack.cpp
//

class CResourceUnpackApp : public CWinApp
{
public:
	CResourceUnpackApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CResourceUnpackApp theApp;
