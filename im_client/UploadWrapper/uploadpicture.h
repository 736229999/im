// uploadpicture.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CuploadpictureApp:
// �йش����ʵ�֣������ uploadpicture.cpp
//

class CuploadpictureApp : public CWinApp
{
public:
	CuploadpictureApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CuploadpictureApp theApp;
