// WebConfig.cpp : WinMain µÄÊµÏÖ

#include "stdafx.h"
#include "resource.h"
#include "WebConfig.h"

class CWebConfigModule : public CAtlExeModuleT< CWebConfigModule >
{
public :
	DECLARE_LIBID(LIBID_WebConfigLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WEBCONFIG, "{AB60B1BD-555A-49D4-92BE-A9A0999B8A0B}")
};

CWebConfigModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

