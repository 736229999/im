// im\VersionInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "versioninfodlg.h"
#include "VersionUpdatgeDlg.h"
#include ".\versioninfodlg.h"
#include "..\MainFrame.h"

// CVersionInfoDlg 对话框

IMPLEMENT_DYNAMIC(CVersionInfoDlg, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CVersionInfoDlg)

CVersionInfoDlg::CVersionInfoDlg(int nVersionFlag,CString strVersionInfoUrl,CWnd* pParent)
	: CDialog(CVersionInfoDlg::IDD, pParent)
{
	m_nVersionFlag			= nVersionFlag;
	m_strVersionInfoUrl		= strVersionInfoUrl;
	Create(IDD,pParent);
}

CVersionInfoDlg::~CVersionInfoDlg()
{
}

void CVersionInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_INFO,m_edtInfo);
}


BEGIN_MESSAGE_MAP(CVersionInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	SET_HMG_SCROLLBAR(IDC_EDIT_INFO)
END_MESSAGE_MAP()


// CVersionInfoDlg 消息处理程序

void CVersionInfoDlg::OnBnClickedOk()
{
	//OpenHtmlPage(g_strUpdateInfo);
	/*
	CVersionUpdatgeDlg *pDlg = new CVersionUpdatgeDlg(12,(LPCSTR)m_strVersionInfoUrl);
	pDlg->ShowWindow(SW_SHOW);
	*/
	CVersionUpdateDlgDui::Show(12, m_strVersionInfoUrl);
	
	DestroyWindow();
	/*
	if ( theApp.m_pMainWnd )
		theApp.m_pMainWnd->DestroyWindow();
		*/

/*	if ( m_nVersionFlag==3 || m_nVersionFlag==2 )
	{
		if ( m_nVersionFlag==3 )
		{		
			XT_LOGOUT req;
			req.id = g_ImUserInfo.GetId();
			req.authData = 0;

			g_pProtocol->SendReq(req);
		}

		CString strParams;
		UINT nVersion = CIMProtocol::s_nCurVer;
		SetCurrentDirectory(g_LocalData.GetPath());
		strParams.Format("miniupdate.exe \"%s\" %u",(LPCSTR)m_strVersionInfoUrl,CIMProtocol::s_nCurVer);
	
		DestroyWindow();
		
		if ( theApp.m_pMainWnd )
			theApp.m_pMainWnd->DestroyWindow();
		
		WinExec(strParams,SW_SHOW);
	}
	*/
}

void CVersionInfoDlg::OnBnClickedCancel()
{

	WORD wVKIgnore = 1;
	g_LocalData.SaveSoftUpdateIgnoreConfig(wVKIgnore);

	DestroyWindow();
	

/*
	if ( m_nVersionFlag==2 )
	{
		theApp.m_pMainWnd->DestroyWindow();
	}
*/
}

void CVersionInfoDlg::OnClose()
{
	DestroyWindow();

	if ( m_nVersionFlag==2 )
	{
		theApp.m_pMainWnd->DestroyWindow();
	}
}

void CVersionInfoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	delete this;
}

BOOL CVersionInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	if ( m_nVersionFlag == 3 )
	{
		m_strTip = "检测到哞哞新版本，是否下载最新版升级？";
	}

	if ( m_nVersionFlag == 2 )
	{
		CMainFrame *pFrame = (CMainFrame *)(theApp.m_pMainWnd);
	
		pFrame->ShowWindow(SW_MINIMIZE);
		pFrame->ShowWindow(SW_HIDE);
		pFrame->RemoveTaskIcon();

		m_strTip = "哞哞发布了新版本，请下载最新版本并安装使用。";
	}

	GetDlgItem(IDC_STATIC_TEXTBOX)->ShowWindow(SW_HIDE);

	
	//AttachHMGScrollbar(m_edtInfo.m_hWnd);
	//m_edtInfo.ShowWindow(SW_HIDE);
	m_edtInfo.SetWindowText(g_strUpdateInfo);

	return TRUE; 
}

void CVersionInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CWnd * pWnd = GetDlgItem(IDC_STATIC_TEXTBOX);

	if ( pWnd )
	{
		CRect rcText ;
		pWnd->GetWindowRect(&rcText);
		ScreenToClient(&rcText);

		CFont *pFont = GetFont();

		int nSaveDC = dc.SaveDC();

		dc.SelectObject(pFont);
		dc.SetBkMode(TRANSPARENT);
		//dc.DrawText(m_strTip,rcText,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_CALCRECT);
		dc.DrawText(m_strTip,rcText,DT_WORDBREAK);
	
		dc.RestoreDC(nSaveDC);
	}
}
