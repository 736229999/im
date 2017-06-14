// im\TabPage1.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\tabpage1.h"


// CTabPage1 对话框

IMPLEMENT_DYNAMIC(CTabPage1, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CTabPage1)

CTabPage1::CTabPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage1::IDD, pParent),
m_pageFriend(this),
m_pageCluster(this)
{
}

CTabPage1::~CTabPage1()
{
}

void CTabPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPage1, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_XT_MESSAGE()
	ON_WM_NCDESTROY()
	SET_HMG_SCROLLBAR(CTabPage1::IDD)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CTabPage1)
	ON_XT_COMMAND(CMD_LOGIN_ACK,OnLoginAck)
END_XT_MESSAGE_MAP()

// CTabPage1 消息处理程序

void CTabPage1::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_wndIMTabCtrl.m_hWnd )
		m_wndIMTabCtrl.MoveWindow(0,0,cx,cy);
	//if ( m_wndFloorCtrl.m_hWnd )
	//	m_wndFloorCtrl.MoveWindow(0,0,cx,cy);
}

BOOL CTabPage1::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc;
	GetClientRect(rc);

	//m_wndFloorCtrl.Create(this,rc);
	/*这个是横着的TAB*/
	m_wndIMTabCtrl.Create(CIMTabWndCtrl::IDD,this);
	m_wndIMTabCtrl.ShowWindow(SW_SHOW);

//	m_pageRecent.Create(CFloorPageRecent::IDD,this);
//	m_pageFriend.Create(this,rc);
//	m_pageCluster.Create(CFloorPageCluster::IDD,this);
//	m_pageAddbook.Create(CFloorPageAddbook::IDD,this);

	m_pageFriend.Create(CFloorPageFriend::IDD,this);
	m_pageRecent.Create(CFloorPageRecent::IDD,this);
	m_pageCluster.Create(CFloorPageCluster::IDD,this);
	m_pageAddbook.Create(CFloorPageAddbook::IDD,this);

	m_wndIMTabCtrl.AddTab(&m_pageRecent,"最近联系人",0);
	m_wndIMTabCtrl.AddTab(&m_pageFriend,"好友",0);
	m_wndIMTabCtrl.AddTab(&m_pageCluster,"群",0);
	m_wndIMTabCtrl.SetCurSel(g_LocalData.GetLastSelFloor());

	g_pProtocol->RegistCmd(CMD_LOGIN_ACK,m_hWnd);

	return TRUE; 
}

void CTabPage1::OnLoginAck(const char *data,WORD wRet)
{
	LPXT_LOGIN_ACK resp=(LPXT_LOGIN_ACK)data;

	//重复响应丢掉
	if (wRet==0 && g_ImUserInfo.status!=XTREAM_OFFLINE )
	{
		return ;
	}

	//if (wRet==0 )
	//{
	//	m_wndRecentList.Load();
	//}
}

BOOL CTabPage1::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CTabPage1::OnNcDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	g_LocalData.SaveCurrentSelFloor(m_wndIMTabCtrl.GetCurSel());

	m_pageFriend.DestroyWindow();
	m_pageCluster.DestroyWindow();
	m_pageAddbook.DestroyWindow();

	CDialog::OnNcDestroy();
}

BOOL CTabPage1::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CTabPage1::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow )
	{
		try
		{
			CWnd *p =m_wndIMTabCtrl.GetTabSheet(m_wndIMTabCtrl.GetCurSel());// m_wndFloorCtrl.GetActivePage()->GetWindow();
			
			if ( p->m_hWnd==m_pageFriend.m_hWnd )
			{
				m_pageFriend.m_wndGroupList.SetFocus();
			}

			if ( p->m_hWnd==m_pageCluster.m_hWnd )
			{
				m_pageCluster.m_wndClusterList.SetFocus();
			}
		}
		catch(...)
		{
		}
	}
}
