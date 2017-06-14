// OleLinkInfo.cpp : COleLinkInfoApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "OleLinkInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COleLinkInfoApp theApp;

const GUID CDECL _tlid = { 0x21DFE841, 0xC6D0, 0x42AF, { 0x8E, 0xF2, 0x14, 0x79, 0x55, 0xFF, 0x87, 0x91 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// COleLinkInfoApp::InitInstance - DLL 初始化

BOOL COleLinkInfoApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// COleLinkInfoApp::ExitInstance - DLL 终止

int COleLinkInfoApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
 	AFX_MANAGE_STATE(_afxModuleAddrThis);
 
 	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
 		return ResultFromScode(SELFREG_E_TYPELIB);
 
 	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
 		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
 	AFX_MANAGE_STATE(_afxModuleAddrThis);
 
 	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
 		return ResultFromScode(SELFREG_E_TYPELIB);
 
 	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
 		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
