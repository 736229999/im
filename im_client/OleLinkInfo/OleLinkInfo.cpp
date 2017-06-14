// OleLinkInfo.cpp : COleLinkInfoApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "OleLinkInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COleLinkInfoApp theApp;

const GUID CDECL _tlid = { 0x21DFE841, 0xC6D0, 0x42AF, { 0x8E, 0xF2, 0x14, 0x79, 0x55, 0xFF, 0x87, 0x91 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// COleLinkInfoApp::InitInstance - DLL ��ʼ��

BOOL COleLinkInfoApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// COleLinkInfoApp::ExitInstance - DLL ��ֹ

int COleLinkInfoApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
 	AFX_MANAGE_STATE(_afxModuleAddrThis);
 
 	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
 		return ResultFromScode(SELFREG_E_TYPELIB);
 
 	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
 		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
 	AFX_MANAGE_STATE(_afxModuleAddrThis);
 
 	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
 		return ResultFromScode(SELFREG_E_TYPELIB);
 
 	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
 		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
