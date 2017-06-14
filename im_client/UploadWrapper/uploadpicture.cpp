// uploadpicture.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "uploadpicture.h"
#include "uploadpictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuploadpictureApp

BEGIN_MESSAGE_MAP(CuploadpictureApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CuploadpictureApp 构造

CuploadpictureApp::CuploadpictureApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CuploadpictureApp 对象

CuploadpictureApp theApp;


// CuploadpictureApp 初始化
#include <TLHELP32.H>

BOOL CuploadpictureApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 检测运行实列
	int nMaxInstanceNum = 2;

	CString strSubKey;
	strSubKey.Format(_T("software\\mini139_uploadPictureX\\shellext"));

	HKEY hKey;
	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		BYTE data[512]={'\0'};
		DWORD cbValue = 512;

		long lngRet = ::RegQueryValueEx(hKey, _T("MaxInstanceNum"), NULL, &dwType, (LPBYTE)data, &cbValue);
		if(lngRet == ERROR_SUCCESS)
		{
			int nNum = atoi((const char *)data);
			if(nNum <= 0)	nMaxInstanceNum = 3;
			else if(nNum >= 100) nMaxInstanceNum= -1;		// 无限制
			else nMaxInstanceNum = nNum;
		}

		DWORD dw = GetLastError();
		::RegCloseKey(hKey); 
	}

	CString strTemp;
	GetModuleFileName(NULL, strTemp.GetBufferSetLength(MAX_PATH), MAX_PATH);

	CString strModuleFilePathName;
	strModuleFilePathName.Format("%s\0", strTemp);

    if(nMaxInstanceNum != -1)
	{	
		HANDLE Snapshot;
		Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	
		PROCESSENTRY32 processListStr;
		processListStr.dwSize = sizeof(PROCESSENTRY32);

		BOOL return_value;
		return_value = Process32First(Snapshot,&processListStr);
		
		int nCount = 0;
		CString strModuleName;
		CString strSelfModeuleName, strSelfModeuleExt, strSelfModuleFileName;
		_splitpath_s(strModuleFilePathName, NULL,0, NULL,0, strSelfModeuleName.GetBufferSetLength(MAX_PATH),MAX_PATH, strSelfModeuleExt.GetBufferSetLength(MAX_PATH),MAX_PATH);
		strSelfModuleFileName.Format("%s%s\0", strSelfModeuleName, strSelfModeuleExt);
		strSelfModuleFileName.MakeLower();
		while(return_value)
		{
			strModuleName.Format("%s\0", processListStr.szExeFile);
			strModuleName.MakeLower();	strModuleName.Trim();

			if(strModuleName.GetLength() > 0 && strModuleName == strSelfModuleFileName)
			{
				++ nCount;
				if(nCount > nMaxInstanceNum)
				{
					return FALSE;
				}
			} // end if
			
			return_value = Process32Next(Snapshot,&processListStr);
		} // end while
		
	} // end if

	SetRegistryKey(_T("mini139_uploadPictureX"));

	g_bShellExt = FALSE;
	CString strCmdLine = GetCommandLine();


	CString strCmdParam(m_lpCmdLine);

	const CString strShellExt("ShellExt"), strReg(_T("/reg"));
	if(strCmdParam.GetLength() > strShellExt.GetLength())
	{
		CString strPre = strCmdParam.Left(strShellExt.GetLength());
		if(strPre == strShellExt)
		{
			g_bShellExt = TRUE;
			g_strParam = strCmdParam.Right(strCmdParam.GetLength() - strShellExt.GetLength());

			::CoInitialize(0);

			CuploadpictureDlg dlg;
			m_pMainWnd = &dlg;
			INT_PTR nResponse = dlg.DoModal();

			
			::CoUninitialize();

			return FALSE;
		}
	}
	else if(strCmdParam.GetLength() >= strReg.GetLength())
	{
		CString strPre = strCmdParam.Left(strReg.GetLength());
		strPre.MakeLower();
	
		// 注册
		if(strPre == strReg)
		{
			HKEY hKey;
			DWORD dwDisposition = REG_CREATED_NEW_KEY;
			if(::RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("software\\mini139_uploadPictureX\\shellext"), 0, NULL, REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS)
			{	
			}

			::RegCloseKey(hKey);
			return FALSE;
		}
	} // end if

	int nPos1 = strCmdLine.Find("(-",0);
	int nPos2 = strCmdLine.Find("-)",0);

	if ( nPos2>nPos1 && nPos1>0 )
	{
		g_strParam = strCmdLine.Mid(nPos1+2,nPos2-nPos1-2);
	}
	else
	{
		return FALSE;
	}
	::CoInitialize(0);

	CuploadpictureDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	::CoUninitialize();

	return FALSE;
}
