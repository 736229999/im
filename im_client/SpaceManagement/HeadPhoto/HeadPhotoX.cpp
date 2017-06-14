// HeadPhotoX.cpp : CHeadPhotoX µÄÊµÏÖ

#include "stdafx.h"
#include "HeadPhotoX.h"
#include ".\headphotox.h"

#include "maindlg.h"
// CHeadPhotoX


STDMETHODIMP CHeadPhotoX::Show(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_spaceData.SetUserInfo((LPCSTR)g_pUserInfo->GetUserName(),g_pUserInfo->GetUserID());
	CMainDlg::Show();

	return S_OK;
}

STDMETHODIMP CHeadPhotoX::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainDlg::Close();

	return S_OK;
}

STDMETHODIMP CHeadPhotoX::GetUserInfo(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal =(LONG)g_pUserInfo;

	return S_OK;
}

STDMETHODIMP CHeadPhotoX::SetMessageWndHandle(LONG hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_hMessageWnd = (HWND)hWnd;
	return S_OK;
}
