// WriteArticle.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "WriteArticle.h"
//#include "MainDlg.h"

class CWriteArticleModule : public CAtlDllModuleT< CWriteArticleModule >
{
public :
	DECLARE_LIBID(LIBID_WriteArticleLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WRITEARTICLE, "{3E7792BA-7550-43AB-9D7D-5C7A0F7DD6E3}")
};

CWriteArticleModule _AtlModule;

class CWriteArticleApp : public CWinApp
{
public:

// ��д
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CWriteArticleApp, CWinApp)
END_MESSAGE_MAP()

CWriteArticleApp theApp;

BOOL CWriteArticleApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

    return CWinApp::InitInstance();
}

int CWriteArticleApp::ExitInstance()
{;
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
