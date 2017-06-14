// MainDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "MyArticles.h"
#include ".\MainDlg.h"

#include "WebDataProcess.h"
#include "WebInterfaceWrapper.h"

bool CompareArticleDate(const TWebArticleInfo *p1, const TWebArticleInfo *p2)
{
	if (p1->dwCreateDate>p2->dwCreateDate)
		return true;
	else
		return false;
}

void RemoveHtmlAttribute(CString &strHtml, CString strAttribute)
{

}


CMainDlg * CMainDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CMainDlg, CXSkinDlg)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
: CXSkinDlg(CMainDlg::IDD, pParent),
m_myArticles(g_pUserInfo->GetUserID())
{
	m_nSplitterPos = 230;
	m_iTabBtn      = -1;
	m_bUpdatingMyAritles = false;

	m_hThread = NULL;
	m_pFormProgress     = NULL;

	m_bAttachTopSide      = false;
	m_dwFocusWebArticleID = 0;

	Create(CMainDlg::IDD,pParent);
}

CMainDlg::~CMainDlg()
{
	pInstance = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_WEB, m_wndExploreWeb);
	DDX_Control(pDX, IDC_EXPLORER_LOCAL, m_wndExploreLocal);
	DDX_Control(pDX, IDC_LIST_LOCAL, m_wndListLocal);
	DDX_Control(pDX, IDC_LIST_WEB, m_wndListWeb);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BUTTON_POST, m_btnPost);
	DDX_Control(pDX, IDC_BUTTON_BACKUP, m_btnBackup);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_comboSearch);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_edtKey);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_CHECK_TOP,m_btnCheckTop);
	DDX_Control(pDX, IDC_COMBO_VIEW,m_comboView);
}


BEGIN_MESSAGE_MAP(CMainDlg, CXSkinDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_MESSAGE(WM_WEBDATA_NOTIFY, OnWebDataNotify)
	ON_BN_CLICKED(IDC_BUTTON_POST, OnBnClickedButtonPost)
	ON_NOTIFY(NM_CLICK, IDC_LIST_WEB, OnListWebClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WEB, OnListWebDblClick)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_WEB, OnListWebColumnClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WEB, OnListWebItemChanged)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOCAL, OnListLocalClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOCAL, OnListLocalDblClick)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_LOCAL, OnListLocalColumnClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCAL, OnListLocalItemChanged)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnBnClickedButtonNew)
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnCancelPubArticle)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_BACKUP, OnBnClickedButtonBackup)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnBnClickedButtonSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW, OnCbnSelchangeComboView)
	ON_BN_CLICKED(IDC_CHECK_TOP, OnBnClickedCheckTop)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WEB, OnNMRclickList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LOCAL, OnNMRclickList)
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序
BOOL CMainDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	EnableHelp(true);

	m_myArticles.uID = g_pUserInfo->GetUserID();

	SetTitleIcon(GetResDLLHandle(),IDI_FRIEND_LOG_ENTRY);

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_SPLITTER);
	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER);
	pWnd->ShowWindow(SW_HIDE);

	m_rcTabBtn1.SetRect(3,62,84,84);
	m_rcTabBtn2.SetRect(87,62,200,84);

	m_btnDelete.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_btnDelete.SetBitmap(IDB_BTN_DEL,IDB_BTN_DEL);
	m_btnDelete.SetToolTip("删除文章");
	m_btnEdit.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_btnEdit.SetBitmap(IDB_BTN_MODIFY,IDB_BTN_MODIFY);
	m_btnEdit.SetToolTip("修改文章");

	m_comboSearch.AddString("标题");
	m_comboSearch.AddString("内容");
	//	m_comboSearch.AddString("作者");
	m_comboSearch.SetCurSel(0);


	ComboInitViewProp(&m_comboView);
	m_comboView.SetCurSel(2);

	m_wndSplitter.SetButton(true);

	CString strTitle;
	strTitle.Format("我的文章 - %s",(LPCSTR)g_pUserInfo->GetNickName());
	SetWindowText(strTitle);
	SetTitleIcon(GetResDLLHandle(),IDI_MY_ARTICLE);

	InitListCtrls();

	AdjustUI();

	m_strDefaultFile = g_spaceData.GetPath()+"Data\\default.htm";

	InitLocalArticles();
	InitWebArticles();

	m_bUpdatingMyAritles = true;
	ShowStatusTip("正在与网站同步数据");

	SetActiveTab(0);

	this->m_wndExploreLocal.SetHelpUrl("http://mini.woku.com/help/read-article.html");
	this->m_wndExploreWeb.SetHelpUrl("http://mini.woku.com/help/read-article.html");

	SetMinMaxInfo(680,300,0,0);

	return TRUE;  
}

CMainDlg * CMainDlg::Show()
{
	if ( pInstance==NULL )
	{
		pInstance = new CMainDlg(GetDesktopWindow());
		pInstance->CenterWindow(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if (pInstance->IsIconic())
		{
			pInstance->ShowWindow(SW_RESTORE);
		}
	}

	pInstance->BringWindowToTop();

	return pInstance;
}

void CMainDlg::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
}


void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CXSkinDlg::OnMouseMove(nFlags, point);
}

void CMainDlg::OnUploadedLocalArticle(DWORD dwSavedID)
{
	int nCount = m_wndListLocal.GetItemCount();

	bool bFind = false;

	for ( int i=0; i<nCount; i++)
	{
		if ( m_wndListLocal.GetItemData(i)==dwSavedID )
		{
			m_wndListLocal.DeleteItem(i);
			m_wndExploreLocal.Navigate(m_strDefaultFile,NULL,NULL,NULL,NULL);
			break;
		}
	}

	//	UpdateLocalListSkin();	
}

void CMainDlg::UpdateWebArticleList(DWORD dwFocusArticleID)
{
	if ( m_bUpdatingMyAritles==false )
	{
		if (dwFocusArticleID==0)
			g_webDataProcess.GetWebArticleList(UIntToStr(g_pUserInfo->GetUserID()),this->m_myArticles.szLastUpdateDate,"",this);
		else
			g_webDataProcess.GetWebArticleList(UIntToStr(g_pUserInfo->GetUserID()),"","",this);

		ShowStatusTip("与网站同步数据...");

		m_dwFocusWebArticleID = dwFocusArticleID;
		m_bUpdatingMyAritles  = true;
	}
}

void CMainDlg::UpdateLocalArticleList(DWORD dwSavedID, CString strTitle)
{
	int nCount = m_wndListLocal.GetItemCount();

	bool bFind = false;

	for ( int i=0; i<nCount; i++)
	{
		if ( m_wndListLocal.GetItemData(i)==dwSavedID )
		{
			m_wndListLocal.SetItemText(i,0,strTitle);
			bFind = true;
			break;
		}
	}

	if ( bFind==false )
	{
		int item = m_wndListLocal.InsertItem(0,strTitle);
		CTime tm(dwSavedID);
		m_wndListLocal.SetItemData(item,dwSavedID);
		m_wndListLocal.SetItemText(item,1,tm.Format("%Y-%m-%d %H:%M:%S"));
		m_wndListLocal.SetItemImage(item,0,0);
		m_wndListLocal.SetCurSel(item);

		if ( item!=-1 )
		{
			SetTimer(0x03,100,NULL);//my local article
			UpdateToolState();
		}
		//		UpdateLocalListSkin();
	}
}

void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iPreBtn = m_iTabBtn;

	if ( m_rcTabBtn1.PtInRect(point) && iPreBtn!=0 )
	{
		SetActiveTab(0);
		ReDrawTabBtns();
	}

	if ( m_rcTabBtn2.PtInRect(point) && iPreBtn!=1 )
	{
		SetActiveTab(1);
		ReDrawTabBtns();
	}

	CXSkinDlg::OnLButtonDown(nFlags, point);
}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CXSkinDlg::OnLButtonUp(nFlags, point);
}

void CMainDlg::InitWebArticles()
{
	g_spaceData.LoadUserWebArticles(g_pUserInfo->GetUserID(),m_myArticles);
	sort(m_myArticles.vecArticles.begin(),m_myArticles.vecArticles.end(),CompareArticleDate);

	PutArticlesToListWeb(m_myArticles.vecArticles);
	g_webDataProcess.GetWebArticleList(UIntToStr(g_pUserInfo->GetUserID()),m_myArticles.szLastUpdateDate,"",this);   
	m_wndExploreWeb.Navigate(m_strDefaultFile,NULL,NULL,NULL,NULL);
}

//void CMainDlg::UpdateLocalListSkin()
//{
//	int n = m_wndListLocal.GetItemCount();
//
//	COLORREF crText = RGB(2,85,165);
//
//	for ( int i=0; i<n; i++ )
//	{
//		COLORREF crBack;
//		crBack = i%2 ?  RGB(241,249,252) : 0xFFFFFF;
//		m_wndListLocal.SetItemTextColor(i,0,crText);
//		m_wndListLocal.SetItemBackColor(i,0,crBack);
//		m_wndListLocal.SetItemTextColor(i,1,crText);
//		m_wndListLocal.SetItemBackColor(i,1,crBack);
//	}
//
//}

void CMainDlg::UpdateToolState()
{
	BOOL bEnable ;

	if (m_iTabBtn==0)//web
	{
		if ( m_wndListWeb.GetCurSel()>-1 )
		{
			bEnable=TRUE;
		}
		else
		{
			bEnable=FALSE;
		}	
	}
	else
	{
		if ( m_wndListLocal.GetCurSel()>-1 )
		{
			bEnable=TRUE;
		}
		else
		{
			bEnable=FALSE;
		}	
	}

	m_btnEdit.EnableWindow(bEnable);
	m_btnDelete.EnableWindow(bEnable);

	m_btnCheckTop.EnableWindow(bEnable);
	m_comboView.EnableWindow(bEnable);

	m_btnEdit.Invalidate();
	m_btnDelete.Invalidate();

	if ( bEnable==FALSE )
	{
		if ( this->m_iTabBtn==0 )
			m_wndExploreWeb.Navigate(m_strDefaultFile,NULL,NULL,NULL,NULL);
		else
			m_wndExploreLocal.Navigate(m_strDefaultFile,NULL,NULL,NULL,NULL);
	}
}

void CMainDlg::InitLocalArticles()
{
	TSavedArticleList l;
	g_spaceData.LoadMySavedArticleList(l);

	m_wndListLocal.LockWindowUpdate();

	int n = l.GetCount();

	for ( int i=0; i<n; i++ )
	{
		TSavedArticleItem info = l.GetAt( l.FindIndex(i) );

		int item = m_wndListLocal.InsertItem(0,info.szTitle);
		CTime tm(info.dwSavedID);
		m_wndListLocal.SetItemText(item,1,tm.Format("%Y-%m-%d %H:%M:%S"));
		m_wndListLocal.SetItemImage(item,0,0);
		m_wndListLocal.SetItemData(item,info.dwSavedID);
	}

	//	UpdateLocalListSkin();

	m_wndExploreLocal.Navigate(m_strDefaultFile,NULL,NULL,NULL,NULL);
	m_wndListLocal.UnlockWindowUpdate();
}

void CMainDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		static CString strDot="";
		static int     iCount=0;

		if ( iCount++<2 )
		{
			strDot=strDot+".";
		}
		else
		{
			iCount=0;
			strDot="";
		}

		SetDlgItemText(IDC_STATIC_TIP,m_strTip+strDot);
	}

	if ( nIDEvent==0x02 )
	{
		KillTimer(nIDEvent);

		int nItem = m_wndListWeb.GetCurSel();

		if ( nItem!=-1 )
		{
			int nArticleID = m_wndListWeb.GetItemData(nItem);
			int n = m_myArticles.FindArticle(nArticleID);

			if ( n!=-1 )
				ShowWebArticle(m_myArticles.vecArticles[n]);
		}

		UpdateToolState();
	}

	if ( nIDEvent==0x03 )
	{		
		KillTimer(nIDEvent);

		int nItem = m_wndListLocal.GetCurSel();

		if ( nItem!=-1 )
		{
			g_spaceData.LoadMySavedArticle(m_currendLocalArticle,m_wndListLocal.GetItemData(nItem));

			CStdioFile f;
			CString str = g_spaceData.GetMySavedArticlePath()+"~~temp.htm";
			DeleteFile(str);
			if ( f.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
			{
				f.WriteString(m_currendLocalArticle.strContent);
				f.Close();
				m_wndExploreLocal.Navigate((LPCSTR)str,NULL,NULL,NULL,NULL);
			}

			ComboSelViewProp(&m_comboView,m_currendLocalArticle.dwViewType);
			m_btnCheckTop.SetCheck(m_currendLocalArticle.dwTopFlag);
		}

		UpdateToolState();

	}

	CXSkinDlg::OnTimer(nIDEvent);
}

void CMainDlg::DrawTabBtn(CDC *pDC, const CRect &rcTab, const CString &sText, bool bBold)
{
	CDC		dcMem;
	CBitmap bmMem;

	int nCaptionHeight = 22;

	dcMem.CreateCompatibleDC(pDC);
	bmMem.CreateCompatibleBitmap(pDC,rcTab.Width(),rcTab.Height());

	int	nSaveMemDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	int nSavedTempDC = dcTemp.SaveDC();

	dcTemp.SelectObject(&m_bmpTitleLeft);
	dcMem.BitBlt(0,0,4,22,&dcTemp,0,0,SRCCOPY);

	dcTemp.SelectObject(&m_bmpTitleRight);
	dcMem.BitBlt(rcTab.Width()-4,0,4,22,&dcTemp,0,0,SRCCOPY);

	dcTemp.RestoreDC(nSavedTempDC);

	Cx_Strech_Blt(&dcMem,4,0,rcTab.Width()-8,&m_bmpTitleMid);

	dcMem.SetTextAlign(TA_CENTER);
	dcMem.SetBkMode(TRANSPARENT);

	CFont font;

	font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		bBold? FW_BOLD: FW_NORMAL, // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	dcMem.SelectObject(&font);

	if ( bBold )
		dcMem.SetTextColor( RGB(244,101,33));
	else
		dcMem.SetTextColor(0);

	dcMem.TextOut( rcTab.Width()/2,5,sText);

	pDC->BitBlt(rcTab.left,rcTab.top,rcTab.Width(),rcTab.Height(),
		&dcMem,0,0,SRCCOPY);

	dcMem.RestoreDC(nSaveMemDC);

}

BOOL CMainDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( wParam==IDC_SPLITTER )
	{
		SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;

		if  (pHdr->hdr.code==0 )
		{
			if ( m_bAttachTopSide==false )
			{
				m_bAttachTopSide = true;
				m_nPreSplitterPos = m_nSplitterPos;
				m_nSplitterPos = 125;
				AdjustUI();
			}
			else
			{
				m_nSplitterPos = m_nPreSplitterPos;
				AdjustUI();
				m_bAttachTopSide = false;
			}
		}
		else
		{
			CRect rc;
			GetClientRect(&rc);
			int cy = rc.Height();

			//	m_bAttachTopSide = false;

			m_nSplitterPos+=pHdr->delta;

			if ( m_nSplitterPos<125 )
				m_nSplitterPos = 125;

			if ( m_nSplitterPos>(cy-100) )
				m_nSplitterPos = cy - 100;

			AdjustUI();	
		}
	}

	return CXSkinDlg::OnNotify(wParam, lParam, pResult);
}

BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc			;
	GetClientRect(&rc)	;

	CRgn rgn;
	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);

	RemoveRgnOfWnd(rgn,&m_wndListWeb);
	RemoveRgnOfWnd(rgn,&m_wndExploreWeb);

	pDC->SelectObject(&rgn);
	pDC->FillSolidRect(&rc,IM_SPACE_COLOR);


	CWnd *	pWnd	;
	CRect	rc1		;

	pWnd = GetDlgItem(IDC_STATIC_ETCHED1);

	if (pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		pDC->Draw3dRect(&rc1,RGB(118,188,214),0xFFFFFF);
	}


	pWnd = GetDlgItem(IDC_STATIC_ETCHED2);

	if (pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		pDC->Draw3dRect(&rc1,RGB(118,188,214),0xFFFFFF);
	}

	return FALSE;
}

void CMainDlg::AdjustUI()
{
	if ( m_wndListWeb.m_hWnd==NULL || m_wndListLocal.m_hWnd==NULL )
		return ;

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();

	int nTopList = m_rcTabBtn1.bottom+1;
	int nCyList  = m_nSplitterPos - nTopList-2;

	CWnd * pWnd ;
	CRect rc1;

	pWnd = GetDlgItem(IDC_STATIC_ETCHED1);
	pWnd->GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	rc1.left   = 3;
	rc1.right  = cx-3;
	pWnd->MoveWindow(&rc1);

	m_wndListWeb.MoveWindow(5,nTopList,cx-10,nCyList-2);
	m_wndListLocal.MoveWindow(5,nTopList,cx-10,nCyList-2);

	m_wndSplitter.MoveWindow(5,m_nSplitterPos-2,cx-10,8);

	rc1.top =  m_nSplitterPos+6;
	rc1.bottom = rc1.top+2;
	pWnd = GetDlgItem(IDC_STATIC_ETCHED2);
	pWnd->MoveWindow(&rc1);


	int nTopTool    = m_nSplitterPos + 7;
	m_btnEdit.MoveWindow(5,nTopTool+2,85,22);
	m_btnDelete.MoveWindow(95,nTopTool+2,85,22);
	GetDlgItem(IDC_STATIC1)->MoveWindow(cx-300,nTopTool+8,55,18);
	m_comboView.MoveWindow(cx-245,nTopTool+4,165,123);
	m_btnCheckTop.MoveWindow(cx-65,nTopTool+5,63,20);
	//GetDlgItem(IDC_STATIC2)->MoveWindow(255,nTopTool+4,90,18);

	int nTopExplore = m_nSplitterPos+33;
	int nCyExplore  = cy - m_nSplitterPos-40;
	m_wndExploreWeb.MoveWindow(5,nTopExplore,cx-10,nCyExplore);
	m_wndExploreLocal.MoveWindow(5,nTopExplore,cx-10,nCyExplore);

	CRgn rgn;
	GetRepaintRgn(rgn);
	InvalidateRgn(&rgn);

	if ( m_wndListWeb.IsWindowVisible() )
	{
		m_wndListWeb.Invalidate();
	}

	if ( m_wndListLocal.IsWindowVisible() )
	{
		m_wndListLocal.Invalidate();
	}

}

void CMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 680;//最小宽度
	lpMMI->ptMinTrackSize.y = 300;//最小高度

	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); 

	CString str;
	str.Format("已发表(%d)",m_wndListWeb.GetItemCount());
	DrawTabBtn(&dc,m_rcTabBtn1,str,m_iTabBtn==0);

	str.Format("未发表/草稿(%d)",m_wndListLocal.GetItemCount());
	DrawTabBtn(&dc,m_rcTabBtn2,str,m_iTabBtn==1);

	DrawCaptionBar();
	DrawBorder();
}

void CMainDlg::OnClose()
{
	DestroyWindow();
}

void CMainDlg::OnDestroy()
{
	if ( m_pFormProgress )
	{
		m_pFormProgress->DestroyWindow();
		delete m_pFormProgress;
		m_pFormProgress = NULL;
	}

	CXSkinDlg::OnDestroy();
	delete this;
}

HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		hbr = brushBack;
		pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	if ( nType!=SIZE_MINIMIZED )
	{
		AdjustUI();
	}
}


void CMainDlg::GetRepaintRgn(CRgn &rgn)
{
	CRect rc;
	GetClientRect(&rc);

	rgn.CreateRectRgn(rc.left,55,rc.right,rc.bottom);

	RemoveRgnOfWnd(rgn,&m_wndListWeb);
	RemoveRgnOfWnd(rgn,&m_wndExploreWeb);
	RemoveRgnOfWnd(rgn,&m_btnDelete);
	RemoveRgnOfWnd(rgn,&m_btnEdit);
	RemoveRgnOfWnd(rgn,&m_comboView);
	RemoveRgnOfWnd(rgn,&m_btnCheckTop);
	RemoveRgnOfWnd(rgn,GetDlgItem(IDC_STATIC1));

	CRgn rgn1;
	rgn1.CreateRectRgn(m_rcTabBtn1.left,m_rcTabBtn1.top,m_rcTabBtn1.right,m_rcTabBtn1.bottom);

	rgn.CombineRgn( &rgn, &rgn1, RGN_DIFF);

	CRgn rgn2;
	rgn2.CreateRectRgn(m_rcTabBtn2.left,m_rcTabBtn2.top,m_rcTabBtn2.right,m_rcTabBtn2.bottom);

	rgn.CombineRgn( &rgn, &rgn2, RGN_DIFF);
}


void CMainDlg::SetActiveTab(int iTab)
{
	if ( iTab!=m_iTabBtn )
	{
		if ( iTab==0 )//web
		{
			m_wndListLocal.ShowWindow(SW_HIDE);			
			m_wndListWeb.ShowWindow(SW_SHOW);
			::ShowWindow(m_wndExploreLocal.m_hWnd,SW_HIDE);
			::ShowWindow(m_wndExploreWeb.m_hWnd,SW_SHOW);
			m_btnPost.SetWindowText("同步更新");
			m_btnPost.SetToolTip("和 woku.com同步数据");

			int nSel = m_wndListWeb.GetCurSel();

			if ( nSel==-1 )
			{
				if ( m_wndListWeb.GetItemCount()>0 )
				{
					m_wndListWeb.SetCurSel(0);
					SetTimer(0x02,100,NULL);
				}

				UpdateToolState();				
			}
		}
		else
		{
			m_wndListWeb.ShowWindow(SW_HIDE);
			m_wndListLocal.ShowWindow(SW_SHOW);
			::ShowWindow(m_wndExploreWeb.m_hWnd,SW_HIDE);
			::ShowWindow(m_wndExploreLocal.m_hWnd,SW_SHOW);
			m_btnPost.SetWindowText("发表文章");
			m_btnPost.SetToolTip("将文章发布到 woku.com");

			int nSel = m_wndListLocal.GetCurSel();

			if ( nSel==-1 )
			{
				if ( m_wndListLocal.GetItemCount()>0 )
				{
					m_wndListLocal.SetCurSel(0);
					SetTimer(0x03,100,NULL);
				}

				UpdateToolState();	
			}
		}

		m_iTabBtn = iTab;

		UpdateToolState();
	}
}

void CMainDlg::ReDrawTabBtns()
{
	InvalidateRect(&m_rcTabBtn1);
	InvalidateRect(&m_rcTabBtn2);
}

void CMainDlg::InitListCtrls()
{
	CResBitmap bmp;
	bmp.LoadBitmap(IDB_ARTICLE);

	m_ImageList.Create(16,18,ILC_COLOR24|ILC_MASK,0,1);
	m_ImageList.Add(&bmp,RGB(0,255,0));

	m_wndListLocal.SetImageList(&m_ImageList,LVSIL_SMALL);

	m_wndListLocal.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndListLocal.InsertColumn(0,"标题",LVCFMT_LEFT,320);
	m_wndListLocal.InsertColumn(1,"创建时间",LVCFMT_LEFT,190);

	m_wndListWeb.SetImageList(&m_ImageList,LVSIL_SMALL);
	m_wndListWeb.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndListWeb.InsertColumn(0,"标题",LVCFMT_LEFT,320);
	m_wndListWeb.InsertColumn(1,"发布渠道",LVCFMT_LEFT,150);
	m_wndListWeb.InsertColumn(2,"发布时间",LVCFMT_LEFT,190);
}

void CMainDlg::ShowStatusTip(CString str)
{
	m_strTip = str;
	SetTimer(0x01,350,NULL);
}

LRESULT CMainDlg::OnWebDataNotify(WPARAM wParam, LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;

	KillTimer(0x01);
	SetDlgItemText(IDC_STATIC_TIP,"");

	m_bUpdatingMyAritles = false;

	bool bModified = false;

	m_myArticles.Lock();

	if ( wParam == 1)//文章列表
	{
		vector<TWebArticleInfo *> info;
		CString strServerDate;
		ParaseWebArticles(result->strWebResult,info,strServerDate);

		strncpy(m_myArticles.szLastUpdateDate,(LPCSTR)strServerDate,sizeof(m_myArticles.szLastUpdateDate)-1);

		int n = info.size();

		for ( int i=0; i<n; i++ )
		{
			int index = m_myArticles.FindArticle(info[i]->dwArticleID);
			if ( index!=-1 )
			{
				delete m_myArticles.vecArticles[index];
				m_myArticles.vecArticles[index] = info[i];	
				bModified = true;
			}
			else
			{
				m_myArticles.vecArticles.push_back(info[i]);
				bModified = true;
			}
		}

		sort(m_myArticles.vecArticles.begin(),m_myArticles.vecArticles.end(),CompareArticleDate);

		if ( bModified)
		{
			m_wndListWeb.DeleteAllItems();
			PutArticlesToListWeb(m_myArticles.vecArticles);

			int nItems = m_wndListWeb.GetItemCount(); 
			if ( nItems>0 )
			{
				if ( m_dwFocusWebArticleID==0 )
				{
					m_wndListWeb.SetCurSel(0);
				}
				else
				{
					for ( int nItem=0; nItem<nItems; nItem++ )
					{
						if ( m_wndListWeb.GetItemData(nItem)==m_dwFocusWebArticleID )
						{
							m_wndListWeb.SetCurSel(nItem);
							break;
						}
					}
				}
			}


			this->UpdateToolState();
		}

		g_spaceData.SaveUserWebArticles(m_myArticles);		

		m_dwFocusWebArticleID = 0;

	}

	m_myArticles.Unlock();

	return 0;
}

void CMainDlg::PutArticlesToListWeb(vector<TWebArticleInfo *> &info )
{
	int n = info.size();

	m_wndListWeb.LockWindowUpdate();

	COLORREF clrText = RGB(2,85,165);

	for ( int i=0; i<n; i++ )
	{
		COLORREF  clrBack;
		int item ;

		clrBack = i%2 ?  RGB(241,249,252) : 0xFFFFFF;

		item = m_wndListWeb.InsertItem(i,info[i]->szTitle,clrText,clrBack);
		m_wndListWeb.SetItemText(item,1,WebDataFrom2Str(info[i]->dwFrom),clrText,clrBack);
		m_wndListWeb.SetItemText(item,2,info[i]->szCreateDate,clrText, clrBack);
		m_wndListWeb.SetItemImage(item,0,0);
		m_wndListWeb.SetItemData(item,info[i]->dwArticleID);
	}

	m_wndListWeb.UnlockWindowUpdate();
}

void CMainDlg::OnBnClickedButtonPost()
{
	if ( m_iTabBtn==0 )//同步更新
	{
		if ( m_bUpdatingMyAritles==false )
		{
			g_webDataProcess.GetWebArticleList(UIntToStr(g_pUserInfo->GetUserID()),"","",this);
			ShowStatusTip("正在和网站进行同步更新");
			m_bUpdatingMyAritles = true;
		}
	}
	else
		if ( m_iTabBtn==1 )//发表文章
		{
			POSITION pos = m_wndListLocal.GetFirstSelectedItemPosition();
			int nSel = -1;
			if ( pos!=NULL )
				nSel = m_wndListLocal.GetNextSelectedItem(pos);

			if ( nSel!=-1 )
			{
				this->LockOperation();

				g_spaceData.LoadMySavedArticle(m_currendLocalArticle,m_wndListLocal.GetItemData(nSel));

				DWORD dwThreadID = 0;
				m_hThread  = CreateThread(NULL,0,
					(LPTHREAD_START_ROUTINE)PubArticleProc,
					0,
					0,
					&dwThreadID);

				m_pFormProgress = new CFormProgress3(m_hWnd,this);
				m_pFormProgress->ShowWindow(SW_SHOW);
				m_pFormProgress->CenterWindow(this);

				CString strTip ;
				strTip.Format("正在发表文章:%s...",this->m_currendLocalArticle.strTitle);
				m_pFormProgress->SetTip(strTip);
			}
			else
			{
				if( 0 == m_wndListLocal.GetItemCount() )
				{
					ShowMessage("您没有需要发表的文章！");
				}
				else
				{
					ShowMessage("对不起，请先选中您要发表的文章！");
				}
			}
		}
}

void CMainDlg::OnListWebClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	TRACE(_T("in CXListCtrlTestDlg::OnClick:  %d, %d\n"), nItem, nSubItem);

	*pResult = 0;
}

void CMainDlg::OnListWebDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem    = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;

		this->OnBnClickedButtonEdit();
	}

	*pResult = 0;
}

void CMainDlg::OnListWebColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;


		SortList(m_wndListWeb,nSubItem);
		//m_wndListLog.SetColumnStatus(nSubItem,xcsArrowUp);
	}
	TRACE(_T("in CXListCtrlTestDlg::OnColumnClick:  %d, %d\n"), nItem, nSubItem);

	*pResult = 0;
}

void CMainDlg::OnListWebItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;

	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;

	}

	if (pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED|LVIS_SELECTED)))
	{
		if ( nItem!=-1 )
		{
			SetTimer(0x02,100,NULL);//my web article
		}
	}

	*pResult = 0;
}



void CMainDlg::OnListLocalClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	TRACE(_T("in CXListCtrlTestDlg::OnClick:  %d, %d\n"), nItem, nSubItem);

	*pResult = 0;
}

void CMainDlg::OnListLocalDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem    = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;

		this->OnBnClickedButtonEdit();
	}

	*pResult = 0;
}

void CMainDlg::OnListLocalColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;


		SortList(m_wndListLocal,nSubItem);
		//m_wndListLog.SetColumnStatus(nSubItem,xcsArrowUp);
	}
	TRACE(_T("in CXListCtrlTestDlg::OnColumnClick:  %d, %d\n"), nItem, nSubItem);

	*pResult = 0;
}

void CMainDlg::OnListLocalItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;

	}

	if (pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED|LVIS_SELECTED)))
	{
		if ( nItem!=-1 )
		{
			SetTimer(0x03,100,NULL);//my local article
		}
	}

	*pResult = 0;
}

void CMainDlg::SortList(CXListCtrl &wndList,int nColumn, bool bAscend)
{
	int nItems      = wndList.GetItemCount();
	int nColumns    = wndList.GetColumns();
	XColumnStatus s = wndList.GetColumnStatus(nColumn);

	int nCols = wndList.GetColumns();

	int col;

	for ( col=0; col<nCols; col++ )
	{
		if ( col!=nColumn )
		{
			wndList.SetColumnStatus(col,xcsNormal);
		}
	}

	if ( (wndList.GetColumnStatus(col)==xcsArrowUp && bAscend) ||
		(wndList.GetColumnStatus(col)==xcsArrowDown && bAscend==false ) )
	{
		return ;
	}

	QuickSortList(wndList,nColumns, nColumn, 0,nItems-1,bAscend);//

	if (bAscend)
		wndList.SetColumnStatus(nColumn,xcsArrowUp);
	else
		wndList.SetColumnStatus(nColumn,xcsArrowDown);
}

void CMainDlg::SortList(CXListCtrl &wndList,int nColumn)
{
	int nItems      = wndList.GetItemCount();
	int nColumns    = wndList.GetColumns();
	XColumnStatus s = wndList.GetColumnStatus(nColumn);

	int nCols = wndList.GetColumns();

	for ( int col=0; col<nCols; col++ )
	{
		if ( col!=nColumn )
		{
			wndList.SetColumnStatus(col,xcsNormal);
		}
	}

	if ( s==xcsNormal || s==xcsArrowDown )
	{
		QuickSortList(wndList,nColumns, nColumn, 0,nItems-1,true);//

		if ( wndList.m_hWnd==m_wndListWeb.m_hWnd && nColumn==1 )
		{
			int iBegin=0;
			CString strKey;
			if (nItems>0)
				strKey=wndList.GetItemText(iBegin,1);

			int nItem=0;

			for ( nItem=0; nItem<nItems; nItem++ )
			{
				if ( wndList.GetItemText(nItem,1)!=strKey )
				{
					QuickSortList(wndList,nColumns,2,iBegin,nItem-1,true);
					iBegin = nItem;
					strKey = wndList.GetItemText(nItem,1); 
				}
			}

			QuickSortList(wndList,nColumns,2,iBegin,nItem-1,true);			

		}

		wndList.SetColumnStatus(nColumn,xcsArrowUp);
	}
	else
		if (s==xcsArrowUp )
		{
			QuickSortList(wndList,nColumns, nColumn, 0,nItems-1,false);//

			if ( wndList.m_hWnd==m_wndListWeb.m_hWnd && nColumn==1 )
			{
				int iBegin=0;
				CString strKey;
				if (nItems>0)
					strKey=wndList.GetItemText(iBegin,1);

				int nItem=0;

				for ( nItem=0; nItem<nItems; nItem++ )
				{
					if ( wndList.GetItemText(nItem,1)!=strKey )
					{
						QuickSortList(wndList,nColumns,2,iBegin,nItem-1,false);
						iBegin = nItem;
						strKey = wndList.GetItemText(nItem,1); 
					}
				}

				QuickSortList(wndList,nColumns,2,iBegin,nItem-1,false);
			}

			wndList.SetColumnStatus(nColumn,xcsArrowDown);
		}	
}


void  CMainDlg::QuickSortList(CXListCtrl &wndList, int nColumnCount, int nColumn, int iLo, int iHi, bool bAscend)
{
	if ( wndList.m_hWnd==this->m_wndListLocal.m_hWnd )
	{
		if ( iLo<iHi )
		{
			char szKey[255];
			strcpy(szKey,(LPCSTR)wndList.GetItemText(iLo,nColumn));

			int lo = iLo;
			int hi = iHi;

			while (lo<hi)
			{
				char szHi[255];
				strcpy(szHi,(LPCSTR)wndList.GetItemText(hi,nColumn));

				if ( bAscend )
				{
					while ( lo<hi && strcmp(szHi,szKey)>=0 )
					{
						--hi;
						strcpy(szHi,(LPCSTR)wndList.GetItemText(hi,nColumn));
					}

					wndList.SwapLine(lo,hi,nColumnCount);

					char szLo[255];
					strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));

					while ( lo<hi && strcmp(szLo,szKey)<0)
					{
						++lo;
						strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));
					}

					wndList.SwapLine(lo,hi,nColumnCount);
				}
				else
				{
					while ( lo<hi && strcmp(szHi,szKey)<=0 )
					{
						--hi;
						strcpy(szHi,(LPCSTR)wndList.GetItemText(hi,nColumn));
					}

					wndList.SwapLine(lo,hi,nColumnCount);

					char szLo[255];
					strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));

					while ( lo<hi && strcmp(szLo,szKey)>0)
					{
						++lo;
						strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));
					}

					wndList.SwapLine(lo,hi,nColumnCount);
				}
			}

			QuickSortList(wndList,nColumnCount,nColumn,iLo,lo-1,bAscend);
			QuickSortList(wndList,nColumnCount,nColumn,lo+1,iHi,bAscend);
		}
	}



	if ( wndList.m_hWnd == this->m_wndListWeb.m_hWnd )
	{
		if ( iLo<iHi )
		{
			if ( nColumn==2 )
			{
				DWORD dwKey = wndList.GetItemData(iLo);

				int lo = iLo;
				int hi = iHi;

				while (lo<hi)
				{
					DWORD dwHi= wndList.GetItemData(hi);

					if ( bAscend )
					{
						while ( lo<hi && dwHi>=dwKey )
						{
							--hi;
							dwHi = wndList.GetItemData(hi);
						}

						wndList.SwapLine(lo,hi,nColumnCount);

						DWORD dwLo = wndList.GetItemData(lo);

						while ( lo<hi && dwLo<dwKey)
						{
							++lo;
							dwLo = wndList.GetItemData(lo);
						}

						wndList.SwapLine(lo,hi,nColumnCount);
					}
					else
					{
						while ( lo<hi && dwHi<=dwKey )
						{
							--hi;
							dwHi = wndList.GetItemData(hi);
						}

						wndList.SwapLine(lo,hi,nColumnCount);

						DWORD dwLo = wndList.GetItemData(lo);

						while ( lo<hi && dwLo>dwKey)
						{
							++lo;
							dwLo = wndList.GetItemData(lo);
						}

						wndList.SwapLine(lo,hi,nColumnCount);
					}
				}

				QuickSortList(wndList,nColumnCount,nColumn,iLo,lo-1,bAscend);
				QuickSortList(wndList,nColumnCount,nColumn,lo+1,iHi,bAscend);
			}
			else
			{
				char szKey[255];
				strcpy(szKey,(LPCSTR)wndList.GetItemText(iLo,nColumn));

				int lo = iLo;
				int hi = iHi;

				while (lo<hi)
				{
					char szHi[255];
					strcpy(szHi,(LPCSTR)wndList.GetItemText(hi,nColumn));

					if ( bAscend )
					{
						while ( lo<hi && strcmp(szHi,szKey)>=0 )
						{
							--hi;
							strcpy(szHi,(LPCSTR)wndList.GetItemText(hi,nColumn));
						}

						wndList.SwapLine(lo,hi,nColumnCount);

						char szLo[255];
						strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));

						while ( lo<hi && strcmp(szLo,szKey)<0)
						{
							++lo;
							strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));
						}

						wndList.SwapLine(lo,hi,nColumnCount);
					}
					else
					{
						while ( lo<hi && strcmp(szHi,szKey)<=0 )
						{
							--hi;
							strcpy(szHi,(LPCSTR)wndList.GetItemText(hi,nColumn));
						}

						wndList.SwapLine(lo,hi,nColumnCount);

						char szLo[255];
						strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));

						while ( lo<hi && strcmp(szLo,szKey)>0)
						{
							++lo;
							strcpy(szLo,(LPCSTR)wndList.GetItemText(lo,nColumn));
						}

						wndList.SwapLine(lo,hi,nColumnCount);
					}
				}

				QuickSortList(wndList,nColumnCount,nColumn,iLo,lo-1,bAscend);
				QuickSortList(wndList,nColumnCount,nColumn,lo+1,iHi,bAscend);
			}
		}
	}
}

void CMainDlg::ShowWebArticle(const TWebArticleInfo * article)
{
	CString str = g_spaceData.GetPath();
	str = str + "~~temp.htm";
	WriteArticlePreviewFile( *article,(LPCSTR)g_pUserInfo->GetNickName(),str);
	ComboSelViewProp(&m_comboView,article->dwViewType);
	m_btnCheckTop.SetCheck(article->dwTopFlag);

	m_wndExploreWeb.Navigate(str,NULL,NULL,NULL,NULL);

	//str.Format("文章查看权限: %s",WebViewProp2Str(article.dwViewType));
	//SetDlgItemText(IDC_STATIC1,str);

	//str.Format("置顶: %s",article.dwTopFlag? "是的" : "不是");
	//SetDlgItemText(IDC_STATIC2,str);
}

void CMainDlg::OnBnClickedButtonDelete()
{
	if ( m_iTabBtn==1 )//未发表
	{
		POSITION pos = m_wndListLocal.GetFirstSelectedItemPosition();

		while (pos!=NULL )
		{
			int nItem = m_wndListLocal.GetNextSelectedItem(pos);

			if ( nItem!=-1 )
			{
				DWORD dwSavedID = m_wndListLocal.GetItemData(nItem);
				g_spaceData.DeleteSavedArticle(dwSavedID);
				m_wndListLocal.DeleteItem(nItem);
			}

			pos = m_wndListLocal.GetFirstSelectedItemPosition();
		}

		this->UpdateToolState();
	}
	else
		if ( m_iTabBtn==0 )
		{
			CWaitCursor wait;
			CString strIDs ="";

			int nSel = 0;

			POSITION pos = m_wndListWeb.GetFirstSelectedItemPosition();

			while (pos!=NULL)
			{
				if ( strIDs!="" )
				{
					strIDs = strIDs+",";
				}

				int nItem = m_wndListWeb.GetNextSelectedItem(pos);
				strIDs = strIDs+UIntToStr(m_wndListWeb.GetItemData(nItem));//article id

				nSel++;
			}

			if ( strIDs!="" )
			{

				if ( MessageBox("你真的要删除这些文章,并且连网站上相应的文章也一起删除掉吗?","删除文章",MB_YESNO|MB_ICONQUESTION)==IDYES )
				{
					//CZoneLog log((LPCSTR)g_pWebConfig->GetServiceUrl(wtZoneLog,g_pUserInfo->GetUserName()));
					//ServiceResponse ret;

					CWebInterfaceWrapper objWI;
					objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());

					CString strError ;
					bool bOk = objWI.DeleteArticle(strIDs,strError);		
					strError.Trim();

					if ( bOk )
					{
						POSITION pos = m_wndListWeb.GetFirstSelectedItemPosition();

						while ( pos )
						{
							int nItem = m_wndListWeb.GetNextSelectedItem(pos);
							if ( nItem!=-1 )
							{
								DWORD dwArticleID = m_wndListWeb.GetItemData(nItem);
								int iIndex = m_myArticles.FindArticle(dwArticleID);
								if ( iIndex!=-1 )
								{
									delete m_myArticles.vecArticles[iIndex];
									m_myArticles.vecArticles.erase(m_myArticles.vecArticles.begin()+iIndex);
								}

								m_wndListWeb.DeleteItem(nItem);
								pos = m_wndListWeb.GetFirstSelectedItemPosition();
							}
							else
							{
								break;
							}
						}

						g_spaceData.SaveUserWebArticles(m_myArticles);

						UpdateToolState();

						while ( nSel>0 )
						{
							nSel--;
						}
					}
					else
					{
						if ( strError=="" )
							strError = "删除失败，可能服务器繁忙，请尝试多次看看．";

						ShowMessage(strError);
					}
				}
			}
		}

		UpdateToolState();
}

void CMainDlg::OnBnClickedButtonEdit()
{
	if ( m_iTabBtn==1 )
	{
		int nItem = m_wndListLocal.GetCurSel();

		if ( nItem!=-1 )
		{
			DWORD dwSavedID = m_wndListLocal.GetItemData(nItem);
			g_spaceData.LoadMySavedArticle(m_currendLocalArticle,dwSavedID);

			if ( g_pWriteArticle )
			{
				g_pWriteArticle->Show();
				g_pWriteArticle->EditLocalSavedArticle((LONG)&m_currendLocalArticle,m_wndListLocal.GetItemData(nItem));
			}
		}
	}
	else
		if ( m_iTabBtn==0 )
		{
			int nItem = m_wndListWeb.GetCurSel();

			if ( nItem!=-1 )
			{		
				int nID = m_myArticles.FindArticle(m_wndListWeb.GetItemData(nItem));

				if (nID!=-1)
				{
					if ( g_pWriteArticle )
					{
						g_pWriteArticle->Show();
						g_pWriteArticle->EditWebArticle((LONG)m_myArticles.vecArticles[nID]);
					}
				}
			}
		}
}

void CMainDlg::OnBnClickedButtonNew()
{
	if ( g_pWriteArticle )
	{
		g_pWriteArticle->Show();
		g_pWriteArticle->NewArticle();
	}
}

LRESULT CMainDlg::OnCancelPubArticle(WPARAM wParam, LPARAM lParam)
{
	UnLockOperation();

	if ( m_hThread )
	{
		try
		{
			TerminateThread(m_hThread,0);
			CloseHandle(m_hThread);
		}
		catch(...)
		{
		}

		m_hThread= NULL;
	}

	if ( m_pFormProgress )
	{
		m_pFormProgress->DestroyWindow();
		delete m_pFormProgress;
		m_pFormProgress=NULL;
	}

	return 0;
}

UINT CMainDlg::WebArticleBackupProc(LPVOID p)
{
	::CoInitialize(0);

	CString strBackupPath = (LPCSTR)p;

	pInstance->m_myArticles.Lock();

	int n = pInstance->m_myArticles.vecArticles.size();

	//IWebConfigXPtr pWebConfig;
	//pWebConfig.CreateInstance(CLSID_WebConfigX);
	//CMiniLog log((LPCSTR)pWebConfig->GetServiceUrl(wtMiniLog,g_pUserInfo->GetUserName()));
	//pWebConfig.Release();

	for ( int i=0; i<n; i++ )
	{
		CComBSTR ret = L"";
		CString strContent;

		CString strHtmlFile;
		char    szTitle[1024];
		strncpy(szTitle,pInstance->m_myArticles.vecArticles[i]->szTitle,sizeof(szTitle)-1);

		int ii,l;

		l = strlen(szTitle);
		for ( ii=0; ii<l; ii++ )
		{
			char c = szTitle[ii];
			if ( c=='\\' ||
				c=='/' ||
				c==':' ||
				c=='?' ||
				c=='<' ||
				c=='>' ||
				c=='|' ||
				c=='\"' )
				szTitle[i]='_';
		}

		strHtmlFile  = strBackupPath;
		strHtmlFile += szTitle;
		strHtmlFile += "(";
		strHtmlFile += IntToStr(pInstance->m_myArticles.vecArticles[i]->dwArticleID);
		strHtmlFile += ").html";
		//strHtmlFile.Format("%s%s(%u).html",strBackupPath,szTitle,);


		CWebInterfaceWrapper objWI;
		objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());

		strContent = objWI.GetArticleContent(pInstance->m_myArticles.vecArticles[i]->dwArticleID);

		CStringList slImages;
		::GetHtmlImageList(strContent,slImages);

		int nn= slImages.GetCount();

		CString strImagePath  = strBackupPath+pInstance->m_myArticles.vecArticles[i]->szTitle+"\\";

		if ( nn>0 )
		{
			CreateDirectory(strImagePath,NULL);
		}

		for ( ii=0; ii<nn; ii++ )
		{
			CString strPicUrl = slImages.GetAt( slImages.FindIndex(ii) );

			CString strFileName = GetWebFileName(strPicUrl);
			strFileName.Trim();

			CString strLocalImageFile = strImagePath + strFileName;

			if ( strFileName!="" && ::DownLoadWebFile(strPicUrl,strLocalImageFile) )
			{
				CString strRelativeFilePath;
				strRelativeFilePath.Format(".\\%s\\%s",pInstance->m_myArticles.vecArticles[i]->szTitle,strFileName);

				strContent.Replace(strPicUrl,strRelativeFilePath);
			}
		}

		CStdioFile f;
		if ( f.Open(strHtmlFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			f.WriteString(strContent);
			f.Close();
		}


		pInstance->PostMessage(WM_THREAD_NOTIFY,1,i+1);
	}

	pInstance->m_myArticles.Unlock();

	::CoUninitialize();

	CloseHandle(pInstance->m_hThread);
	pInstance->m_hThread=NULL;

	pInstance->SendMessage(WM_THREAD_NOTIFY,2,(LPARAM)(LPCSTR)strBackupPath);
	return 0;
}

UINT CMainDlg::PubArticleProc(LPVOID p)
{
	TLocalSavedArticleInfo info = pInstance->m_currendLocalArticle;

	CStringList slImages ;
	GetHtmlImageList(info.strContent,slImages);

	int n= slImages.GetCount();

	CString strErrorInfo = "";

	for ( int i=0; i<n; i++ )
	{
		CString strLocalFile = slImages.GetAt( slImages.FindIndex(i) );
		if ( strLocalFile.Find("://")==-1 )
		{
			CString strPicUrl, strError;

			CWebInterfaceWrapper objWI;
			objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());

			if ( objWI.UploadPicture(strLocalFile, "", "", "", info.dwViewType, 2, strPicUrl, strError) )
			{
				strPicUrl = "http://"+strPicUrl;
				info.strContent.Replace(strLocalFile,strPicUrl);
			}
			else
			{
				strErrorInfo = strErrorInfo + strError;
				strErrorInfo = strErrorInfo + "\n";
			}
		}
	}

	long    nReturnArticleID;

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
	
	nReturnArticleID = objWI.CreateArticle( 0,//create new file
											0,
											info.strTitle,
											info.strContent,
											info.dwDirID,
											info.dwViewType,
											info.strTag,
											info.dwTopFlag,
											strErrorInfo);

	CloseHandle(pInstance->m_hThread);
	pInstance->m_hThread=NULL;

	strErrorInfo.Trim();

	if ( strErrorInfo=="" )
		pInstance->PostMessage(WM_THREAD_NOTIFY,0,0);
	else
		pInstance->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)(LPCSTR)strErrorInfo);

	return 0;
}

void CMainDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

LRESULT CMainDlg::OnThreadNotify(WPARAM wParam , LPARAM lParam)
{
	if ( wParam==0 )
	{
		if ( lParam==0 )//发表成功
		{
			POSITION pos = m_wndListLocal.GetFirstSelectedItemPosition();
			int nSel = -1;
			if ( pos!=NULL )
			{
				nSel = m_wndListLocal.GetNextSelectedItem(pos);

				if ( nSel!=-1 )
				{
					g_spaceData.DeleteSavedArticle(m_wndListLocal.GetItemData(nSel));
					m_wndListLocal.DeleteItem(nSel);
				}

				nSel=-1;

				pos =  m_wndListLocal.GetFirstSelectedItemPosition();
				if ( pos!=NULL )
					nSel = m_wndListLocal.GetNextSelectedItem(pos);
			}

			if ( nSel!=-1 )
			{
				this->LockOperation();

				g_spaceData.LoadMySavedArticle(m_currendLocalArticle,m_wndListLocal.GetItemData(nSel));

				DWORD dwThreadID = 0;
				m_hThread  = CreateThread(NULL,0,
					(LPTHREAD_START_ROUTINE)PubArticleProc,
					0,
					0,
					&dwThreadID);


				CString strTip ;
				strTip.Format("正在发表文章:%s...",this->m_currendLocalArticle.strTitle);
				m_pFormProgress->SetTip(strTip);
			}
			else//upload finished
			{
				this->UnLockOperation();

				if ( m_pFormProgress )
				{
					m_pFormProgress->DestroyWindow();
					delete m_pFormProgress;
					m_pFormProgress = NULL;
				}

				if ( m_bUpdatingMyAritles==false )
				{
					g_webDataProcess.GetWebArticleList(UIntToStr(g_pUserInfo->GetUserID()),this->m_myArticles.szLastUpdateDate,"",this);
					ShowStatusTip("与网站同步数据...");
					m_bUpdatingMyAritles = true;
				}

				UpdateToolState();
			}
		}
		else//失败,or exceptions
		{
			this->UnLockOperation();

			if ( m_pFormProgress )
			{
				m_pFormProgress->DestroyWindow();
				delete m_pFormProgress;
				m_pFormProgress = NULL;
			}

			UpdateToolState();

			CString strError;
			strError.Format("文章发表出现错误，可能是服务器繁忙．稍后多试几次看看．^^　\n错误原因:\n%s",
				(LPCSTR)lParam);
			ShowMessage(strError);
		}
	}
	else
		if ( wParam==1 )
		{
			if ( m_pFormProgress )
			{
				CString str;
				str.Format("正在导出文章(%d/%d)...", lParam, m_myArticles.vecArticles.size() );
				m_pFormProgress->SetTip(str);
			}
		}
		else
			if ( wParam==2 )
			{
				this->UnLockOperation();

				if ( m_pFormProgress )
				{
					m_pFormProgress->DestroyWindow();
					delete m_pFormProgress;
					m_pFormProgress = NULL;
				}


				::ShellExecute(NULL,"explore","","",(LPCSTR)lParam,SW_SHOWMAXIMIZED);
			}

			return 0;
}

void CMainDlg::OnBnClickedButtonSearch()
{
	int iSearchType		= m_comboSearch.GetCurSel();

	CString strKeyWord ;
	m_edtKey.GetWindowText(strKeyWord);

	strKeyWord.Trim();

	if( 0== strKeyWord.GetLength())
	{
		MessageBox("关键字不能为空","提示");
		return;
	}

	strKeyWord.MakeLower();



	bool bFind = false;

	if ( m_iTabBtn==0 )//web 
	{
		int n=m_wndListWeb.GetItemCount();

		CString strData;

		for ( int i=0; i<n; i++ )
		{
			strData = "";

			switch(iSearchType)
			{
			case 0://标题
				strData = m_wndListWeb.GetItemText(i,0);
				break;
			case 1://内容
				{
					DWORD nID = m_myArticles.FindArticle( m_wndListWeb.GetItemData(i) );

					if ( nID!=-1 )
					{
						strData = m_myArticles.vecArticles[nID]->szOutlines;
					}
				}
				break;
			case 2://作者
				strData = m_wndListWeb.GetItemText(i,1);
				break;
			}

			strData.MakeLower();

			if ( strData.Find(strKeyWord)>-1 )
			{
				bFind = true;
				m_wndListWeb.SetCurSel(i);
				if ( IDYES!=MessageBox("找到一条记录．\n按“是”继续查找下一条记录，按“否”停止查找。","查找",MB_YESNO|MB_ICONINFORMATION) )
				{
					break;
				}
			}			
		}
	}
	else
		if ( m_iTabBtn==1 )//未发表
		{
		}

		if ( bFind==false )
		{
			ShowMessage("没有找到相关的记录");
		}
}

void CMainDlg::OnBnClickedButtonBackup()
{
	TZoneSetting setting;
	g_spaceData.GetZoneSetting(setting);

	if ( _access(setting.szBackupPath,0)==-1 )
	{
		MessageBox("你好像没有设置备份路径或者备份路径不可用！你先设置好备份路径再导出你的文章，谢谢！","温馨提示",MB_OK|MB_ICONINFORMATION);
		OnBnClickedButtonSetting();
		return ;
	}

	static CString strBackupPath="";

	CTime tm=CTime::GetCurrentTime();


	LockOperation();


	if ( m_iTabBtn==0 )//备份ｗｅｂ
	{
		strBackupPath.Format("%s\\%s_已经发表的文章(%s)\\",setting.szBackupPath,(LPCSTR)g_pUserInfo->GetUserName(),tm.Format("%Y-%m-%d"));

		CreateDirectory(strBackupPath,NULL);

		DWORD dwThreadID = 0;
		m_hThread  = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)WebArticleBackupProc,
			(LPVOID)(LPCSTR)strBackupPath,
			0,
			&dwThreadID);

		m_pFormProgress = new CFormProgress3(m_hWnd,this);
		m_pFormProgress->ShowWindow(SW_SHOW);
		m_pFormProgress->CenterWindow(this);

		m_pFormProgress->SetTip("正在导出,可能需要很长时间,不如喝杯咖啡吧...");
	}
	else
	{
		strBackupPath.Format("%s\\%s_备份_草稿(%s)\\",setting.szBackupPath,(LPCSTR)g_pUserInfo->GetUserName(),tm.Format("%Y-%m-%d"));

		CreateDirectory(strBackupPath,NULL);

		TSavedArticleList ar;
		g_spaceData.LoadMySavedArticleList(ar);

		int n = ar.GetCount();
		CString strFileName;

		for ( int i=0; i<n; i++ )
		{
			TSavedArticleItem item = ar.GetAt( ar.FindIndex(i) );

			CString strSavedDate ;
			CTime tmSaved(item.dwSavedID);
			strSavedDate = tmSaved.Format("%Y-%m-%d-%H-%M-%S");

			TLocalSavedArticleInfo info;

			CString strImagePath ;
			strImagePath.Format("%s%s.files\\",strBackupPath,strSavedDate);
			CreateDirectory(strImagePath,NULL);

			if ( g_spaceData.LoadMySavedArticle(info, item.dwSavedID) )
			{
				CStringList slImages;

				GetHtmlImageList(info.strContent,slImages);

				int nn=slImages.GetCount();

				for ( int ii=0; ii<nn; ii++ )
				{
					CString strImageFile = slImages.GetAt( slImages.FindIndex(ii) );
					CString strFileName  = GetFileNameFromPath(strImageFile);
					if ( strImageFile.Find("://")==-1 )
					{
						CopyFile(strImageFile,strImagePath+strFileName,FALSE);
					}

					CString strNewImage;
					strNewImage.Format(".\\%s.files\\%s",strSavedDate,strFileName);

					info.strContent.Replace(strImageFile,strNewImage);
				}

				CString strHtmlFile;
				strHtmlFile.Format("%s%s.html",strBackupPath,strSavedDate);
				CStdioFile f;
				if ( f.Open(strHtmlFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
				{
					f.WriteString(info.strContent);
					f.Close();
				}
			}
		}

		UnLockOperation();

		::ShellExecute(NULL,"explore","","",strBackupPath,SW_SHOWMAXIMIZED);
	}

	//	ShowMessage(strBackupPath);
}
void CMainDlg::OnBnClickedButtonSetting()
{
	if ( g_hMessageWnd )
	{
		::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_MyArticles,0);
	}
//	CSAConfig::Show(3);
}

void CMainDlg::OnCbnSelchangeComboView()
{
	int nViewProp  = ComboGetViewProp(&m_comboView);
	USES_CONVERSION;

	if ( m_iTabBtn==0 )//已发表
	{
		CWaitCursor w;
		int nItem = m_wndListWeb.GetCurSel();

		if ( nItem!=-1 )
		{
			int nArticleID = m_wndListWeb.GetItemData(nItem);
			int n = m_myArticles.FindArticle(nArticleID);

			if ( n!=-1 )
			{		
				CWebInterfaceWrapper objWI;
				CString              strError;

				objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
				objWI.SetArticleViewProp(nArticleID,nViewProp,strError);

				m_myArticles.vecArticles[n]->dwViewType = nViewProp;
			}
		}
	}
	else
		if ( m_iTabBtn==1 )
		{
			int nItem = m_wndListLocal.GetCurSel();

			if ( nItem!=-1 )
			{
				m_currendLocalArticle.dwViewType = nViewProp;
				g_spaceData.SaveMySavedArticle(m_currendLocalArticle,m_wndListLocal.GetItemData(nItem));
			}
		}
}

void CMainDlg::OnBnClickedCheckTop()
{
	int nTop  = m_btnCheckTop.GetCheck();

	USES_CONVERSION;

	if ( m_iTabBtn==0 )//已发表
	{
		CWaitCursor w;
		int nItem = m_wndListWeb.GetCurSel();

		if ( nItem!=-1 )
		{
			int nArticleID = m_wndListWeb.GetItemData(nItem);
			int n = m_myArticles.FindArticle(nArticleID);

			if ( n!=-1 )
			{				
				CWebInterfaceWrapper objWI;
				CString              strError;

				objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
				objWI.SetArticleTop(nArticleID,nTop,strError);
			
				m_myArticles.vecArticles[n]->dwViewType = nTop;
			}
		}
	}
	else
		if ( m_iTabBtn==1 )
		{
			int nItem = m_wndListLocal.GetCurSel();

			if ( nItem!=-1 )
			{
				m_currendLocalArticle.dwTopFlag = nTop;
				g_spaceData.SaveMySavedArticle(m_currendLocalArticle,m_wndListLocal.GetItemData(nItem));
			}
		}
}

void CMainDlg::OnStnClickedStaticDownload()
{
	///*if ( m_bUpdatingMyAritles==false )
	//{
	//	CDateSpanSelectDlg dlg("下载\\更新指定时间段内发表的文章",this);
	//	if ( IDOK==dlg.DoModal() )
	//	{
	//		CString strBegin,strEnd;
	//		dlg.GetDateSpan(strBegin,strEnd);
	//		g_webDataProcess.GetWebArticleList(UIntToStr(g_ImUserInfo.id),(LPCSTR)strBegin,(LPCSTR)strEnd,this);
	//		ShowStatusTip("正在更新");
	//		m_bUpdatingMyAritles = true;
	//	}
	//}	
	//else
	//{
	//	ShowMessage("正在更新你的文章，请稍候再下载...");
	//}*/
}

void CMainDlg::OnNMRclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenuEx menu, *popmenu;

	menu.LoadMenu(IDR_MENU_TEMPLATE);
	popmenu = menu.GetSubMenu(0);
	popmenu->RemoveMenu(0,MF_BYPOSITION);

	CPoint pt;
	GetCursorPos(&pt);
	popmenu->AppendMenu(MF_STRING,IDC_BUTTON_EDIT,"编辑");
	popmenu->AppendMenu(MF_STRING,IDC_BUTTON_DELETE,"删除");

	popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXSkinDlg::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

BEGIN_EVENTSINK_MAP(CMainDlg, CXSkinDlg)
	ON_EVENT(CMainDlg, IDC_EXPLORER_LOCAL, 250, BeforeNavigate2ExplorerLocal, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CMainDlg, IDC_EXPLORER_WEB, 250, BeforeNavigate2ExplorerWeb, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

void CMainDlg::BeforeNavigate2ExplorerLocal(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	USES_CONVERSION;
	CString strUrl = W2A(URL->bstrVal);

	strUrl.Trim();
	strUrl.MakeLower();

	int iPos = strUrl.Find("id:");

	if ( iPos>-1 )
	{	
		*Cancel = TRUE;
	}
	else
	{
		strUrl.Replace("139.com","woku.com");

		if ( strUrl.Find("woku.com/article")>-1 )
		{
			*Cancel = TRUE;
			MiniOpenUrl(strUrl);
		}
	}
}

void CMainDlg::BeforeNavigate2ExplorerWeb(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	USES_CONVERSION;
	CString strUrl = W2A(URL->bstrVal);

	strUrl.Trim();
	strUrl.MakeLower();

	int iPos = strUrl.Find("id:");

	if ( iPos>-1 )
	{	
		CString strUrl;
		strUrl.Format("http://%s.woku.com",(LPCSTR)g_pUserInfo->GetUserName());
		MiniOpenUrl(strUrl);
		*Cancel = TRUE;
	}
	else
	{
		strUrl.Replace("139.com","woku.com");

		if ( strUrl.Find("woku.com/article")>-1 )
		{
			*Cancel = TRUE;
			MiniOpenUrl(strUrl);
		}
	}
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CXSkinDlg::PreTranslateMessage(pMsg);
}

LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	TRACE("%x\n",message);
	return CXSkinDlg::WindowProc(message,wParam,lParam);
}