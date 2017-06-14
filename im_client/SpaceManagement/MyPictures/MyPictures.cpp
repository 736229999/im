// MyPictures.cpp : DLL ������ʵ�֡�

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

// ��д
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
