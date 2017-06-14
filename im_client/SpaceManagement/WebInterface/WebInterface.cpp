// WebInterface.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "WebInterface.h"

class CWebInterfaceModule : public CAtlDllModuleT< CWebInterfaceModule >
{
public :
	DECLARE_LIBID(LIBID_WebInterfaceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WEBINTERFACE, "{9CC37B00-6DDB-402F-8B14-B6BF38AC5E0D}")
};

CWebInterfaceModule _AtlModule;

class CWebInterfaceApp : public CWinApp
{
public:

// ��д
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CWebInterfaceApp, CWinApp)
END_MESSAGE_MAP()

CWebInterfaceApp theApp;

BOOL CWebInterfaceApp::InitInstance()
{
    return CWinApp::InitInstance();
}

int CWebInterfaceApp::ExitInstance()
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
