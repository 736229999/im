// HmgUploadPictureX.cpp : CHmgUploadPictureX 的实现

#include "stdafx.h"

#include "fmtstring.h"
#include "HmgUploadPictureX.h"
#include "UploadPictureX/uploadingdlg.h"
CUpLoadPhotoDlg *g_dlg = NULL;

// CHmgUploadPictureX


STDMETHODIMP CHmgUploadPictureX::QueryVersion(BSTR strVersion, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	long nCurrentVersion = 1009;
	long nVersion        = 0;

	USES_CONVERSION;
	nVersion = atoi(W2A(strVersion));

	if(nVersion <=nCurrentVersion )	strResult.Format(_T("1"));
	else	strResult.Format(_T("0"));

	* retVal = strResult.AllocSysString();
                                           
	return S_OK;
}

STDMETHODIMP CHmgUploadPictureX::UploadPicShellExt(BSTR strParamInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strParam = WideStrToStr(strParamInfo);

	if(_access(strParam, 06) == -1)	return S_OK;

	CString strFmtFileString;
	

	try
	{
		CFile file;
		CFileException ex;

		if(!file.Open(strParam, CFile::modeRead, &ex))
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);

			return S_OK;
		}

		int nFileLength = file.GetLength();
		file.Read(strFmtFileString.GetBufferSetLength(nFileLength), nFileLength);
		file.Close();
	}
	catch( ... )
	{
		// ...
	}
	
	if(strFmtFileString.GetLength() < 3)	return S_OK;

	const int nAutoDeleteTempFiles = AfxGetApp() ->GetProfileInt(_T("1.0"), _T("AutoDeleteTempFiles"), 1);
	if(nAutoDeleteTempFiles > 0)
	{
		::DeleteFile(strParam);
	}


	if(!g_AppConfigInfo.LoadConfigInfo() || !g_AppConfigInfo.IsConfigOK())
	{
		strParam.Format("读取配置文件出错,请重新配置图片上传文件信息!\t");
		MessageBox(NULL,strParam, "图片上传", MB_ICONERROR);		

		return S_OK;
	}

	g_AppConfigInfo.bAutoDeleteTempFiles = nAutoDeleteTempFiles > 0 ? TRUE : FALSE;
	

	CString strFilePathName, strFileName, strFileDrv, strFileDir, strFileExt;
	CFmtString fmtString(strFmtFileString.AllocSysString());

	_lstUpFileINFO lstUploadFiles;
	while(fmtString.m_str.length() > 0 && fmtString.End())
	{
		_bstr_t bstrFilePathName = (_bstr_t) fmtString;
		strFilePathName.Format("%s\0", (char *)bstrFilePathName);
		
		_splitpath(strFilePathName, strFileDrv.GetBufferSetLength(MAX_PATH), strFileDir.GetBufferSetLength(MAX_PATH), 
			 strFileName.GetBufferSetLength(MAX_PATH),
			strFileExt.GetBufferSetLength(MAX_PATH));

		tagUpFileInfo upFileInfo;

		upFileInfo.bIsChanged = FALSE;
		
		upFileInfo.strDescription	= strFileName;
		upFileInfo.strFileDir		.Format("%s%s", strFileDrv, strFileDir);
		upFileInfo.strFileName		.Format("%s%s\0", strFileName , strFileExt);
		
		upFileInfo.strFilePathName	= strFilePathName;
		upFileInfo.strSourceFileName = upFileInfo.strFileName;
		upFileInfo.strSourceFilePathName = strFilePathName;
	
		lstUploadFiles.push_back(upFileInfo);
	} // end while

	
	DWORD gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput; 
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	CUploadingDlg UploadingDlg(lstUploadFiles,  CWnd::GetDesktopWindow());
	
	CString strWindowText;
	strWindowText.Format("上传图片到%s个人博客中...", g_AppConfigInfo.userinfo.strUserName);

	UploadingDlg.m_strWindowText = strWindowText;
	g_AppConfigInfo.bCalledByShellExt = TRUE;
	UploadingDlg.DoModal();
	
	GdiplusShutdown(gdiplusToken);

	return S_OK;
}

STDMETHODIMP CHmgUploadPictureX::BringToTop(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( g_dlg )
		g_dlg->PostMessage(WM_BRINGTOTOP_MSG,0,0 );

	return S_OK;
}

STDMETHODIMP CHmgUploadPictureX::ShowUploadDlg(BSTR strParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput; 
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	CUpLoadPhotoDlg dlg;
	g_dlg = &dlg;
	dlg.SetParamInfo(WideStrToStr(strParam));
	dlg.DoModal();
	
	GdiplusShutdown(gdiplusToken);

	return S_OK;
}
