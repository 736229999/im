// MagicFace.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMagicFaceApp:
// �йش����ʵ�֣������ MagicFace.cpp
//

class CMagicFaceApp : public CWinApp
{
public:
	CMagicFaceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMagicFaceApp theApp;