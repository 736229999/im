// PictureBroadcast.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CPictureBroadcastApp:
// �йش����ʵ�֣������ PictureBroadcast.cpp
//

class CPictureBroadcastApp : public CWinApp
{
public:
	CPictureBroadcastApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPictureBroadcastApp theApp;
