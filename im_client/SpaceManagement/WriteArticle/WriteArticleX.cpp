// WriteArticleX.cpp : CWriteArticleX µÄÊµÏÖ

#include "stdafx.h"
#include "WriteArticleX.h"
#include ".\writearticlex.h"
#include "mainframe.h"

// CWriteArticleX


STDMETHODIMP CWriteArticleX::Show()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_spaceData.SetUserInfo((LPCSTR)g_pUserInfo->GetUserName(),g_pUserInfo->GetUserID());
	CMainFrame::Show();
	return S_OK;
}

STDMETHODIMP CWriteArticleX::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainFrame::Close();
	
	return S_OK;
}

STDMETHODIMP CWriteArticleX::get_UserInfo(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = (LONG)g_pUserInfo;

	return S_OK;
}

STDMETHODIMP CWriteArticleX::SelectMyArticlesX(LONG pMyArticlesX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_pMyArticles = (IMyArticlesX *)pMyArticlesX;

	return S_OK;
}

STDMETHODIMP CWriteArticleX::NewArticle(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainFrame *pDlg = CMainFrame::pInstance;

	if ( pDlg )
	{
		pDlg->NewArticle();
	}

	return S_OK;
}

STDMETHODIMP CWriteArticleX::EditLocalSavedArticle(LONG pLocalSaveArticleInfo, LONG nSavedID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainFrame *pDlg = CMainFrame::pInstance;

	if ( pDlg )
	{
		pDlg->EditLocalSavedArticle(*(TLocalSavedArticleInfo *)pLocalSaveArticleInfo,nSavedID);
	}

	return S_OK;
}

STDMETHODIMP CWriteArticleX::EditWebArticle(LONG pWebArticleInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMainFrame *pDlg = CMainFrame::pInstance;

	if ( pDlg )
	{
	 	pDlg->EditWebArticle(*(TWebArticleInfo *)pWebArticleInfo);
	}

	return S_OK;
}
