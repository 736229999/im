// PictureLoadX.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "PictureLoadX.h"

class CPictureLoadXModule : public CAtlDllModuleT< CPictureLoadXModule >
{
public :
	DECLARE_LIBID(LIBID_PictureLoadXLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PICTURELOADX, "{4BAA7427-B500-4807-869F-B4637B2EC12F}")
};

CPictureLoadXModule _AtlModule;

class CPictureLoadXApp : public CWinApp
{
public:

// ��д
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CPictureLoadXApp, CWinApp)
END_MESSAGE_MAP()

CPictureLoadXApp theApp;

BOOL CPictureLoadXApp::InitInstance()
{

    return CWinApp::InitInstance();
}

int CPictureLoadXApp::ExitInstance()
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
