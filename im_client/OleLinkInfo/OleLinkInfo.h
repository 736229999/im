#pragma once

// OleLinkInfo.h : OleLinkInfo.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������


// COleLinkInfoApp : �й�ʵ�ֵ���Ϣ������� OleLinkInfo.cpp��

class COleLinkInfoApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

