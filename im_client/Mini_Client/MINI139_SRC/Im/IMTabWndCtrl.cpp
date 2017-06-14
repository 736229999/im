// im\FloorPageIM.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include ".\imtabwndctrl.h"
#include "FloorPageFriend.h"
#include "..\Im\FSDialog.h"

#define TAB_BTN_Height 35

// CFloorPageIM 对话框

IMPLEMENT_DYNAMIC(CIMTabWndCtrl, CDialog)
CIMTabWndCtrl::CIMTabWndCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CIMTabWndCtrl::IDD, pParent)
{
	m_nCurSel     = -1;
	m_nHotTab     = -1;
	m_nPreHotTab  = -1;
	m_bmpTabHot.LoadBitmap(IDB_RECENT_NORMAL);
	m_bmpTabNormal.LoadBitmap(IDB_RECENT_NORMAL/*IDB_RECENT_HOT*/);

	//最近联系人
	m_bmpRecentHot.LoadBitmap(IDB_RECENT_HOT);
	m_bmpRecentNormal.LoadBitmap(IDB_RECENT_NORMAL);
	//好友
	m_bmpFriendHot.LoadBitmap(IDB_FRIEND_HOT);
	m_bmpFriendNormal.LoadBitmap(IDB_FRIEND_NORMAL);
	//群
	m_bmpGroupHot.LoadBitmap(IDB_GROUP_HOT);
	m_bmpGroupNormal.LoadBitmap(IDB_GROUP_NORMAL);

	
	m_bMouseOver  = false;

}

CIMTabWndCtrl::~CIMTabWndCtrl()
{
}

void CIMTabWndCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_GUIDE,m_btnGuide);
}


BEGIN_MESSAGE_MAP(CIMTabWndCtrl, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_STATIC_GUIDE, OnBnClickedButtonGuide)
END_MESSAGE_MAP()


// CFloorPageIM 消息处理程序

BOOL CIMTabWndCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnGuide.SetNormalColor(RGB(207,97,19));


	return TRUE;  
}

BOOL CIMTabWndCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient,0xFFFFFF);
	return FALSE;
}

void CIMTabWndCtrl::OnPaint()
{
	CPaintDC dc(this); 

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(&dc,m_rcTab.Width(),m_rcTab.Height());

	int nSavedDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);

	dcMem.FillSolidRect(&m_rcTab,BACKGND_COLOR);
	dcMem.SelectObject(&g_fontText);
	dcMem.SetBkMode(TRANSPARENT);

	//int nLeft  = m_rcTab.left+1;
	int nTabs  = m_vecTab.size();

	int nLineLeft, nLineRight, nLineTop;

	for ( int nTab=0; nTab<nTabs; nTab++ )
	{
		CSize szText = dcMem.GetTextExtent(m_vecTab[nTab].strTabText );
		CRect rcTab = m_vecTab[nTab].rcTab;
		/*
		rcTab.top    = m_rcTab.top;
		rcTab.bottom = rcTab.top + TAB_BTN_CY;
		rcTab.left   = nLeft ;
		rcTab.right  = nLeft + szText.cx + 20;
		rcTab.top+=2;
		*/

		CBitmap *pbmpBack;

		switch(nTab)
		{
			case 0:
				if( m_nCurSel == nTab)
					pbmpBack = &m_bmpRecentHot;
				else
					pbmpBack = &m_bmpRecentNormal;
				break;
			case 1:
				if( m_nCurSel == nTab)
					pbmpBack = &m_bmpFriendHot;
				else
					pbmpBack = &m_bmpFriendNormal;
				break;
			case 2:
				if( m_nCurSel == nTab)
					pbmpBack = &m_bmpGroupHot;
				else
					pbmpBack = &m_bmpGroupNormal;
				break;
			default:
				ASSERT(0);
				break;
		}

		if ( m_nCurSel == nTab )
		{
			//pbmpBack   = &m_bmpTabHot;
			nLineLeft  = rcTab.left;
			nLineRight = rcTab.right;
			nLineTop   = rcTab.bottom-1;
			//dcMem.SetTextColor(RGB(0,83,165));
			dcMem.FillSolidRect(&rcTab,RGB(248,248,248));
		}
		else
		{
			//pbmpBack=&m_bmpTabNormal;
			//dcMem.SetTextColor(RGB(0,0,0));
		}

		CDC dcBtn;
		dcBtn.CreateCompatibleDC(&dc);
		int nSavedDcBtn = dcBtn.SaveDC();
		dcBtn.SelectObject(pbmpBack);

		BITMAP bi;
		pbmpBack->GetBitmap(&bi);
		int nBtnLeft, nBtnTop;
		nBtnLeft = rcTab.left + (rcTab.right - rcTab.left - bi.bmWidth ) / 2;
		nBtnTop = rcTab.top + (rcTab.bottom - rcTab.top - bi.bmHeight ) / 2;
		dcMem.BitBlt( nBtnLeft, nBtnTop, bi.bmWidth,bi.bmHeight, &dcBtn, 0, 0, SRCCOPY);
		//dcMem.StretchBlt(rcTab.left+3,rcTab.top,rcTab.Width()-6,22,&dcBtn,3,0,bi.bmWidth-6,bi.bmHeight,SRCCOPY);
		//dcMem.BitBlt(rcTab.left+3,rcTab.top,rcTab.Width()-6,22,&dcBtn,3,0,bi.bmWidth-6,bi.bmHeight,SRCCOPY);
		//dcMem.BitBlt(rcTab.right-3,rcTab.top,3,22,&dcBtn,bi.bmWidth-3,0,SRCCOPY);

		dcBtn.RestoreDC(nSavedDcBtn);

		//dcMem.TextOut(rcTab.left+10,rcTab.top+(TAB_BTN_CY-szText.cy)/2,m_vecTab[nTab].strTabText);
		//m_vecTab[nTab].rcTab = rcTab;

		//nLeft = rcTab.right + 1;
	}
/*
	CPen penLine(PS_SOLID,1,RGB(142,155,181));

	dcMem.SelectObject(&penLine);

	dcMem.MoveTo(0,nLineTop);
	dcMem.LineTo(nLineLeft,nLineTop);
	dcMem.MoveTo(nLineRight,nLineTop);
	dcMem.LineTo(m_rcTab.right,nLineTop);
*/
	dc.BitBlt(0,0,m_rcTab.Width(),m_rcTab.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedDC);
}


int CIMTabWndCtrl::TabHitTest(const CPoint &pt)
{
	int nTabs = m_vecTab.size();

	for ( int nTab=0; nTab<nTabs; nTab++ )
	{
		if ( m_vecTab[nTab].rcTab.PtInRect(pt) )
		{
			return nTab;
		}
	}

	return -1;
}

int  CIMTabWndCtrl::AddTab(CWnd *pWnd, CString strTabText, UINT nBitmap)
{
	if ( pWnd->IsWindowVisible() )
		pWnd->ShowWindow(SW_HIDE);

	pWnd->SetParent(this);

	tagTabInfo info;
	info.pPage       = pWnd;
	info.strTabText  = strTabText;
	info.nBitmap     = nBitmap;

	m_vecTab.push_back(info);

	int nItem = m_vecTab.size()-1;

	if ( m_nCurSel == -1 )
		m_nCurSel = nItem;

	return nItem;
}

void CIMTabWndCtrl::SetCurSel(int nSel)
{
	if(nSel >= m_vecTab.size())
		return;
	if ( /*m_nCurSel!=nSel && */m_hWnd )   //每次都刷新，解决第一次登录最近联系人“一片蓝”的BUG
	{
		CRect rc;
		GetClientRect(&rc);

		if ( m_nCurSel!=-1 )
			m_vecTab[m_nCurSel].pPage->ShowWindow(SW_HIDE);

		rc.top += TAB_BTN_Height;
		//rc.top +=2;

		CWnd *pTabToShow = m_vecTab[nSel].pPage;
		if(pTabToShow == NULL)
			return;
		pTabToShow->MoveWindow(&rc);

		if ( !pTabToShow->IsWindowVisible() )
		{
			pTabToShow->ShowWindow(SW_SHOW);
		}
		pTabToShow->BringWindowToTop();


		m_nCurSel=nSel;

		if(m_nCurSel == 2 && g_vecCluster.size() == 0)
		{
			m_btnGuide.SetWindowText("寻找您感兴趣的群");
			rc.bottom = rc.top + 100;
			pTabToShow->MoveWindow(&rc);
			m_btnGuide.ShowWindow(SW_HIDE);
			m_btnGuide.MoveWindow(80,150,120,20);
		}

/*暂时屏蔽群*/
		/*
		if(m_nCurSel == 2 )
		{
			m_btnGuide.SetWindowText("正在装修中，敬请期待");
			pTabToShow->ShowWindow(SW_HIDE);
			m_btnGuide.ShowWindow(SW_SHOW);
			m_btnGuide.MoveWindow(80,150,120,20);
		}
		*/

		else if(m_nCurSel == 0 && g_lsFriend.size() == 0)
		{
			m_btnGuide.SetWindowText("点击添加好友");
			rc.bottom = rc.top + 100;
			pTabToShow->MoveWindow(&rc);
			m_btnGuide.ShowWindow(SW_SHOW);
			m_btnGuide.MoveWindow(85,150,120,20);
		}
		else
		{
			m_btnGuide.ShowWindow(SW_HIDE);

		}
		

		//Invalidate();
		
		InvalidateRect(&m_rcTab);
	}

	
}

int  CIMTabWndCtrl::GetCurSel()
{
	return m_nCurSel;
}

CWnd * CIMTabWndCtrl::GetTabSheet(int nTab)
{
	if ( nTab>-1 && nTab<m_vecTab.size() )
		return m_vecTab[nTab].pPage;
	else
		return NULL;
}


void CIMTabWndCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	int nLeft  = m_rcTab.left;
	int nTabs  = m_vecTab.size();
	int perX = cx / 3;
	for ( int nTab=0; nTab<nTabs; nTab++ )
	{
		m_vecTab[nTab].rcTab.top	 = m_rcTab.top;
		m_vecTab[nTab].rcTab.bottom = m_rcTab.bottom;
		m_vecTab[nTab].rcTab.left	 = nLeft ;
		m_vecTab[nTab].rcTab.right  = nLeft + perX;
		m_vecTab[nTab].rcTab.top += 2;
		nLeft += perX;
	}
	//最后一个横TAB，由于不整除有一点点余的
	if(nTabs > 0)
		m_vecTab[nTabs-1].rcTab.right = m_rcTab.left + cx;

	if ( m_nCurSel!=-1 && m_hWnd )
	{
		CWnd *pTabToShow = m_vecTab[m_nCurSel].pPage;

		CRect rc;
		rc.SetRect(0, TAB_BTN_Height, cx, cy);
		m_rcTab.SetRect(0, 0, cx, TAB_BTN_Height);

		pTabToShow->MoveWindow(&rc);
	}
		

}



void CIMTabWndCtrl::ShowToolTip(const CString& strTip)
{
	if(m_sLastTip == strTip)
		return;
	m_sLastTip = strTip;
	m_sTip = strTip;
	if (m_toolTip.m_hWnd == NULL)
	{
		m_toolTip.Create(this,TTS_ALWAYSTIP);
		// 设置出现提示需要的时间长度
		m_toolTip.SetDelayTime(TTDT_INITIAL,100);
		// 设置提示持续时间
		m_toolTip.SetDelayTime(TTDT_AUTOPOP,10000);
	}
	if (m_toolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_toolTip.AddTool(this, strTip, rc, 1001);
	}
	m_toolTip.UpdateTipText(strTip ,this,1001);

}

BOOL CIMTabWndCtrl::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE
		)
	{
		if(m_toolTip.m_hWnd != NULL)
			m_toolTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}



void CIMTabWndCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//if ( m_bMouseOver==false )
	//{
	//	m_bMouseOver=true;
	//}

	if(TabHitTest(point) == 1 && g_FloorPageFriend != NULL)  //好友	
	{
		ShowToolTip(g_FloorPageFriend->m_sFriendCntInfo);
	}
	else
	{
		ShowToolTip("");
	}
	CDialog::OnMouseMove(nFlags, point);
}



void CIMTabWndCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nSel = TabHitTest(point);

	if ( nSel!=-1 && m_nCurSel!=nSel )
	{
		SetCurSel(nSel);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CIMTabWndCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonUp(nFlags, point);
}

void CIMTabWndCtrl::OnBnClickedButtonGuide()
{
	// TODO: 在此添加控件通知处理程序代码
	//添加好友
	if(m_nCurSel == 0)
	{
		tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg=CAddFriendOrClusterDlg::Show();
		pDlg->SetSearchType(FS_XTREAM);	
	}

	if(m_nCurSel == 2)
	{
		tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg=CAddFriendOrClusterDlg::Show();
		pDlg->SetSearchType(FS_CLUSTER);	
	}

}

