// HeadPhoto.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "HeadPhoto.h"

class CHeadPhotoModule : public CAtlDllModuleT< CHeadPhotoModule >
{
public :
	DECLARE_LIBID(LIBID_HeadPhotoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HEADPHOTO, "{142BC1EC-3B0A-4EC2-A8CB-AF19533F7356}")
};

CHeadPhotoModule _AtlModule;

class CHeadPhotoApp : public CWinApp
{
public:

// ��д
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CHeadPhotoApp, CWinApp)
END_MESSAGE_MAP()

CHeadPhotoApp theApp;

BOOL CHeadPhotoApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

    return CWinApp::InitInstance();
}

int CHeadPhotoApp::ExitInstance()
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
