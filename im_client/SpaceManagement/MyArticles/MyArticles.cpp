// MyArticles.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "MyArticles.h"

class CMyArticlesModule : public CAtlDllModuleT< CMyArticlesModule >
{
public :
	DECLARE_LIBID(LIBID_MyArticlesLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MYARTICLES, "{7493B605-A089-4095-AB4B-B435EB2B62E1}")
};

CMyArticlesModule _AtlModule;

class CMyArticlesApp : public CWinApp
{
public:

// ��д
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyArticlesApp, CWinApp)
END_MESSAGE_MAP()

CMyArticlesApp theApp;

BOOL CMyArticlesApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();
    return CWinApp::InitInstance();
}

int CMyArticlesApp::ExitInstance()
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
