
// NetClientDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNetClientDemoApp:
// �йش����ʵ�֣������ NetClientDemo.cpp
//

class CNetClientDemoApp : public CWinApp
{
public:
	CNetClientDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetClientDemoApp theApp;