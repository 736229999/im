// Music.cpp : DLL ������ʵ�֡�

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

// ��д
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


// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
    // ע��������Ϳ�����Ϳ��е����нӿ�
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
