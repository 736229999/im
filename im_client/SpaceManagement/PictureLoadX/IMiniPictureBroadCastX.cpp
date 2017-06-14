// IMiniPictureBroadCastX.cpp : CIMiniPictureBroadCastX 的实现

#include "stdafx.h"
#include "IMiniPictureBroadCastX.h"
#include ".\iminipicturebroadcastx.h"
#include "MainDlg.h"


// CIMiniPictureBroadCastX

STDMETHODIMP CIMiniPictureBroadCastX::Show( BSTR strCatchePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_spaceData.SetPath((CString)strCatchePath);
	
	if ( g_vecUserID.size()>0 )
	{
		int nUserCount = g_vecUserID.size();
		
		srand(time(0));

		long *pData ;
		
		try
		{
			pData = new long[nUserCount];
		}
		catch(...)
		{
			pData = NULL;
		}


		if ( pData!=NULL )
		{
			for ( int i=0; i<nUserCount; i++ )
				pData[i]=g_vecUserID[i];

			int n = nUserCount;

			while ( n>0 )
			{
				int nIndex = rand()%n;

				n--;

				for ( int ii=nIndex; ii<n; ii++ )
					pData[ii]=pData[ii+1];

				g_vecUserID[n]=pData[nIndex];
			}

			delete [] pData;
		}

		CString str;
	
		str.Format("%sdata\\%s\\pic0\\",WideStrToStr(strCatchePath),g_strUserName);
		g_strCachePath=str;	
		g_spaceData.SetUserInfo(g_strUserName,g_nUserID);
		
		CMainDlg::Show();
	}
   
	return S_OK;
}

STDMETHODIMP CIMiniPictureBroadCastX::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	CMainDlg::Close();

	return S_OK;
}

STDMETHODIMP CIMiniPictureBroadCastX::AddUser(long nUserID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_vecUserID.push_back(nUserID);
	
	return S_OK;
}

STDMETHODIMP CIMiniPictureBroadCastX::SetWindowTop(BOOL bWndTop)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainDlg::SetWindowTop(bWndTop);
	return S_OK;
}

STDMETHODIMP CIMiniPictureBroadCastX::SetMessageWndHandle(LONG hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_hMessageWnd = (HWND)hWnd;

	return S_OK;
}


STDMETHODIMP CIMiniPictureBroadCastX::SetInterval(int nInterval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(nInterval>=1&&nInterval<=10)
	{
		CMainDlg::SetInterval(nInterval);
	}
	else
	AfxMessageBox("请输入一个1—10之间的整数");

	return S_OK;
}


STDMETHODIMP CIMiniPictureBroadCastX::SetUserInfo(LONG nUserID, BSTR strUserName, BSTR strNickName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_nUserID = nUserID;
	g_strUserName = WideStrToStr(strUserName);
	g_strNickName = WideStrToStr(strNickName);

	return S_OK;
}


STDMETHODIMP CIMiniPictureBroadCastX::EmptyPictures()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(g_strCachePath=="")   return S_OK;
	char   szFilePath[256];
	sprintf(szFilePath,"%s",g_strCachePath);
	CFileFind tempFind; 
	char sTempFileFind[200] ;
	sprintf(sTempFileFind,"%s\\*.*",szFilePath); 
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);  
	while (IsFinded) 
	{
		IsFinded = tempFind.FindNextFile();
		if (!tempFind.IsDots()) 
		{	
			char sFoundFileName[200]; 
			strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 

			if (tempFind.IsDirectory())
			{  
				char sTempDir[200]; 
				sprintf(sTempDir,"%s\\%s",szFilePath,sFoundFileName); 
				DeleteDirectory(sTempDir); 
			} 
			else 
			{  
				char sTempFileName[200]; 
				sprintf(sTempFileName,"%s\\%s",szFilePath,sFoundFileName); 
				DeleteFile(sTempFileName); 
			} 
		}	
	} 
	tempFind.Close(); 
	RemoveDirectory(szFilePath);
	
	return S_OK;
}
