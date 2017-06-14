// MyArticlesX.cpp : CMyArticlesX 的实现

#include "stdafx.h"
#include "MyArticlesX.h"
#include "MainDlg.h"
#include ".\myarticlesx.h"

// CMyArticlesX


STDMETHODIMP CMyArticlesX::Show(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_spaceData.SetUserInfo((LPCSTR)g_pUserInfo->GetUserName(),g_pUserInfo->GetUserID());
	CMainDlg::Show();

	return S_OK;
}

STDMETHODIMP CMyArticlesX::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	CMainDlg::Close();

	return S_OK;
}

STDMETHODIMP CMyArticlesX::get_UserInfo(LONG * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    *pVal = (LONG)g_pUserInfo;
	return S_OK;
}

STDMETHODIMP CMyArticlesX::SelectWriteArticleX(LONG pWriteArticleX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pWriteArticle = (IWriteArticleX *)pWriteArticleX;
	return S_OK;
}

STDMETHODIMP CMyArticlesX::OnUploadLocalArticle(LONG nSavedID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	CMainDlg *pDlg = CMainDlg::GetInstance();
	if ( pDlg )
		pDlg->OnUploadedLocalArticle(nSavedID);

	return S_OK;
}

STDMETHODIMP CMyArticlesX::UpdateWebArticleList(LONG nArticleID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainDlg *pDlg = CMainDlg::GetInstance();
	if ( pDlg )
		pDlg->UpdateWebArticleList(nArticleID);

	return S_OK;
}

STDMETHODIMP CMyArticlesX::UpdateLocalArticleList(LONG nSavedID, BSTR strArticleTitle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainDlg *pDlg = CMainDlg::GetInstance();
	if ( pDlg )
		pDlg->UpdateLocalArticleList(nSavedID,WideStrToStr(strArticleTitle));

	return S_OK;
}

STDMETHODIMP CMyArticlesX::SetMessageWndHandle(LONG hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_hMessageWnd = (HWND)hWnd;
	return S_OK;
}
