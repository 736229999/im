// MyPicturesX.cpp : CMyPicturesX µÄÊµÏÖ

#include "stdafx.h"
#include "MyPicturesX.h"
#include ".\mypicturesx.h"
#include "MainDlg.h"

// CMyPicturesX


STDMETHODIMP CMyPicturesX::Show(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_spaceData.SetUserInfo((LPCSTR)g_pUserInfo->GetUserName(),g_pUserInfo->GetUserID());
	CMainDlg::Show();

	return S_OK;
}

STDMETHODIMP CMyPicturesX::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainDlg::Close();

	return S_OK;
}

STDMETHODIMP CMyPicturesX::get_UserInfo(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = (LONG)g_pUserInfo;

	return S_OK;
}

STDMETHODIMP CMyPicturesX::SelectHeadPhotoX(LONG pHeadPhotoX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_pHeadPhoto = (IHeadPhotoX *)pHeadPhotoX;

	return S_OK;
}

STDMETHODIMP CMyPicturesX::SetMessageWndHandle(LONG hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_hMessageWnd = (HWND)hWnd;

	return S_OK;
}
