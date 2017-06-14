// MainFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "MainFrame.h"
#include ".\mainframe.h"


// CMainFrame 对话框
CMainFrame * CMainFrame::pInstance = NULL;

IMPLEMENT_DYNAMIC(CMainFrame, CDialog)
CMainFrame::CMainFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CMainFrame::IDD, pParent)
{
	m_pWriteBlogDlg  = NULL;
	m_bStopCmdThread = false;
	Create(CMainFrame::IDD,pParent);
}

CMainFrame::~CMainFrame()
{
	pInstance = NULL;
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainFrame, CDialog)
	ON_WM_NCDESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_INITWRITEBLOGDLG,OnInitWriteBlogDlg)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
END_MESSAGE_MAP()


// CMainFrame 消息处理程序
CMainFrame * CMainFrame::Show()
{
	if ( pInstance==NULL )
	{
		pInstance = new CMainFrame(GetDesktopWindow());
	}
	else
	{
		if ( pInstance->m_pWriteBlogDlg->IsIconic() )
			pInstance->m_pWriteBlogDlg->ShowWindow(SW_RESTORE);

		pInstance->m_pWriteBlogDlg->BringWindowToTop();
	}
	
	return pInstance;
}

void CMainFrame::NewArticle()
{
	tagCmdInfo *pInfo = new tagCmdInfo();
	pInfo->nCmdType=0;
	m_secCmd.Lock();
	m_lstCmd.push_back(pInfo);
	m_secCmd.Unlock();
}

void CMainFrame::EditLocalSavedArticle(const TLocalSavedArticleInfo &info, DWORD dwSavedID)
{
	tagCmdInfo *pInfo = new tagCmdInfo();
	pInfo->nCmdType=2;
	pInfo->pCmdData = new char[sizeof(TLocalSavedArticleInfo)];
	memcpy(pInfo->pCmdData,&info,sizeof(info));
	pInfo->nParam  = dwSavedID;
	m_secCmd.Lock();
	m_lstCmd.push_back(pInfo);
	m_secCmd.Unlock();
}

void CMainFrame::EditWebArticle(const TWebArticleInfo &info)
{
	tagCmdInfo *pInfo = new tagCmdInfo();
	pInfo->nCmdType=1;
	pInfo->pCmdData = new char[sizeof(TWebArticleInfo)];
	memcpy(pInfo->pCmdData,&info,sizeof(info));
	m_secCmd.Lock();
	m_lstCmd.push_back(pInfo);
	m_secCmd.Unlock();
}



UINT CMainFrame::CmdScanProc(LPVOID lpParam)
{
	CMainFrame *pFrame = (CMainFrame *)lpParam;

	while(pFrame->m_pWriteBlogDlg==NULL)
	{
		Sleep(1);
	}

	while(pFrame->m_bStopCmdThread==false)
	{
		if ( pFrame->m_lstCmd.size()>0 )
		{
			pFrame->m_secCmd.Lock();
			list<tagCmdInfo *>::iterator it = pFrame->m_lstCmd.begin();
			tagCmdInfo * pInfo = *(it);
			pFrame->m_lstCmd.pop_front();
			pFrame->m_secCmd.Unlock();
			pFrame->PostMessage(WM_THREAD_NOTIFY,(WPARAM)pInfo);
			//delete pInfo;
		}
		
		Sleep(1);
	}

	return 0;
}

void CMainFrame::Close()
{
	if ( pInstance )
	{
		if ( pInstance->m_pWriteBlogDlg )
			pInstance->m_pWriteBlogDlg->SendMessage(WM_CLOSEWRITEBLOGDLG);
	}
}

void CMainFrame::OnNcDestroy()
{
	m_bStopCmdThread = true;

	

	CDialog::OnNcDestroy();
	delete this;
}

BOOL CMainFrame::OnInitDialog()
{
	CDialog::OnInitDialog();

	AfxBeginThread((AFX_THREADPROC)CmdScanProc,this);

	ShowWindow(SW_HIDE);

	::PostMessage(m_hWnd, WM_INITWRITEBLOGDLG, 0 , 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CMainFrame::OnInitWriteBlogDlg(WPARAM,LPARAM)
{
	m_pWriteBlogDlg = new CWriteBlogDlg(GetDesktopWindow());
	m_pWriteBlogDlg->DoModal();

	delete m_pWriteBlogDlg;
	m_pWriteBlogDlg = NULL;

	DestroyWindow();
	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	tagCmdInfo *pInfo = (tagCmdInfo *)wParam;

	switch(pInfo->nCmdType) {
	case 0://new article
		m_pWriteBlogDlg->NewArticle();
		break;
	case 1:
		{
			TWebArticleInfo *pWebArticle = (TWebArticleInfo *)pInfo->pCmdData;
			m_pWriteBlogDlg->EditWebArticle(*pWebArticle);
		}
		break;
	case 2:
		{
			TLocalSavedArticleInfo *pLocalArticle = (TLocalSavedArticleInfo *)pInfo->pCmdData;
			m_pWriteBlogDlg->EditLocalSavedArticle(*pLocalArticle,pInfo->nParam);
		}
		break;
	default:
		break;
	}

	delete pInfo;

	return 0;
}
