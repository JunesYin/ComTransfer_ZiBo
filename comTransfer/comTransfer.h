
// comTransfer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcomTransferApp:
// �йش����ʵ�֣������ comTransfer.cpp
//

class CcomTransferApp : public CWinApp
{
public:
	CcomTransferApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcomTransferApp theApp;