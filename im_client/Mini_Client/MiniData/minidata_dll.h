// minidata_dll.h : minidata_dll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// Cminidata_dllApp
// �йش���ʵ�ֵ���Ϣ������� minidata_dll.cpp
//

class Cminidata_dllApp : public CWinApp
{
public:
	Cminidata_dllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
