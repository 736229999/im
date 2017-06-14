// HmgUploadPicture.cpp : DLL ������ʵ�֡�


#include "stdafx.h"
#include "resource.h"
#include "UploadPictureX/xskindlg.h"
#include "HmgUploadPicture.h"


class CHmgUploadPictureModule : public CAtlDllModuleT< CHmgUploadPictureModule >
{
public :
	DECLARE_LIBID(LIBID_HmgUploadPictureLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HMGUPLOADPICTURE, "{677468A1-58F7-46F7-8CA2-4FDF5336645E}")
};

CHmgUploadPictureModule _AtlModule;

class CHmgUploadPictureApp : public CWinApp
{
public:

// ��д
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CHmgUploadPictureApp, CWinApp)
END_MESSAGE_MAP()

CHmgUploadPictureApp theApp;

BOOL CHmgUploadPictureApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

	
	CXSkinDlg::InitResource();

    return CWinApp::InitInstance();
}

int CHmgUploadPictureApp::ExitInstance()
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

