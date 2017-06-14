// MyPictures.cpp : DLL 导出的实现。

#include "stdafx.h"
#include "resource.h"
#include "MyPictures.h"

class CMyPicturesModule : public CAtlDllModuleT< CMyPicturesModule >
{
public :
	DECLARE_LIBID(LIBID_MyPicturesLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MYPICTURES, "{C1EE0627-8A0A-48BD-8A03-C983C156318C}")
};

CMyPicturesModule _AtlModule;

class CMyPicturesApp : public CWinApp
{
public:

// 重写
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyPicturesApp, CWinApp)
END_MESSAGE_MAP()

CMyPicturesApp theApp;

BOOL CMyPicturesApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

    return CWinApp::InitInstance();
}

int CMyPicturesApp::ExitInstance()
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
