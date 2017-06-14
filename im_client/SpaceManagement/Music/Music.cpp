// Music.cpp : DLL 导出的实现。

#include "stdafx.h"
#include "resource.h"
#include "Music.h"

class CMusicModule : public CAtlDllModuleT< CMusicModule >
{
public :
	DECLARE_LIBID(LIBID_MusicLib)
//	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MUSIC, "{544A5157-22ED-4167-B311-49E400F7492E}")
};

CMusicModule _AtlModule;

class CMusicApp : public CWinApp
{
public:

// 重写
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMusicApp, CWinApp)
END_MESSAGE_MAP()

CMusicApp theApp;

BOOL CMusicApp::InitInstance()
{
    return CWinApp::InitInstance();
}

int CMusicApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}


// 用于确定 DLL 是否可由 OLE 卸载
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// 返回一个类工厂以创建所请求类型的对象
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - 将项添加到系统注册表
STDAPI DllRegisterServer(void)
{
    // 注册对象、类型库和类型库中的所有接口
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - 将项从系统注册表中移除
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
