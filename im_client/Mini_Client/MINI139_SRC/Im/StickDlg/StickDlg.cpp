
#include "stdafx.h"
#include "StickDlg.h"
#include "StickMainDlg.h"
#include ".\stickdlg.h"


IMPLEMENT_DYNAMIC(CStickDlg, _BaseStickDlg)

CStickDlg::CStickDlg(UINT nIDTemplate,CStickMainDlg * pStickMain, CWnd * pParent /* = NULL */)
	: _BaseStickDlg(nIDTemplate,pParent),m_rctLastSize(0,0,10,10)
{
	ASSERT(pStickMain);
	m_pStickMain = pStickMain;
	m_pStickMain->AddStickDlg(this);
	m_ptDown.x = -1;
	m_ptDown.y = -1;
	m_bSizeStart = FALSE;
	m_bMoveStart = FALSE;
	m_rctLastMove = m_rctLastSize;
}

CStickDlg::~CStickDlg()
{
}

void CStickDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

int CStickDlg::PlaceWnd(PLACE_STICK_WND nCmdPlace)
{
	ASSERT(m_pStickMain);
	// 由主窗体来控制位置
	if(m_pStickMain)
		return	m_pStickMain->PlaceSubWnd(this,nCmdPlace);
	else
		return -1;
}

int CStickDlg::ShowWnd(SHOW_STICK_WND nCmdShow)
{
	ASSERT(m_pStickMain);
	// 由主窗体来控制显示
	if(m_pStickMain)
	{
		return m_pStickMain->ShowSubWnd(this,nCmdShow);
	}
	else
		return -1;
}


BEGIN_MESSAGE_MAP(CStickDlg, _BaseStickDlg)
	ON_WM_MOVING()
	ON_WM_SIZING()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()



void CStickDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	__super::OnMoving(fwSide, pRect);

	if(m_pStickMain)
	{
		if(m_ptDown.x == -1 || m_ptDown.y == -1)
		{
			GetCursorPos(&m_ptDown);
		}
		m_pStickMain->OnMovingSubWnd(this,pRect,m_ptDown);
	}
	if(m_bMoveStart == FALSE)
	{
		GetWindowRect(&m_rctLastMove);
		m_bMoveStart = TRUE;
	}
}

void CStickDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	__super::OnSizing(fwSide, pRect);
	
	if(m_pStickMain)
	{
		m_pStickMain->OnSizingSubWnd(this,pRect);
	}
	if(m_bSizeStart == FALSE)
	{
		GetWindowRect(&m_rctLastSize); 
		m_bSizeStart = TRUE;
	}
}

void CStickDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	m_ptDown = point;
	__super::OnNcLButtonDown(nHitTest, point);
}

void CStickDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch(nID)
	{
	case SC_MAXIMIZE:
		ShowWnd(SSW_HIDE);
		break;
	case SC_MINIMIZE:
		ShowWnd(SSW_HIDE);
		break;
	case SC_RESTORE:
		__super::OnSysCommand(nID, lParam);
		ShowWnd(SSW_ATTACH);
		return;
	case SC_CLOSE:
		ShowWnd(SSW_CLOSE);
		break;
	};

	__super::OnSysCommand(nID, lParam);
}

void CStickDlg::OnClose()
{
	ShowWnd(SSW_CLOSE);

	__super::OnClose();
}

BOOL CStickDlg::DestroyWindow()
{
	if ( m_hWnd )
		ShowWnd(SSW_CLOSE);
	return __super::DestroyWindow();
}

void CStickDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_pStickMain)
	{
		m_pStickMain->OnSizeSubWnd(this);
	}
	if(m_bSizeStart)
	{
		// 开始新的Size
		m_bSizeStart = FALSE;
	}
	__super::OnSize(nType, cx, cy);

}

void CStickDlg::OnMove(int x, int y)
{
	if(m_pStickMain)
	{
		m_pStickMain->OnMoveSubWnd(this);
	}
	if(m_bMoveStart == TRUE)
	{
		// 开始新的Move
		m_bMoveStart = FALSE;
	}
	__super::OnMove(x, y);
	
}

void CStickDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	
	
	//if(nState == WA_CLICKACTIVE)
	//{
	//	/*if(m_pStickMain)
	//	{
	//		m_pStickMain->OnActiveSub(this);
	//	}*/
	//	//m_pStickMain->BringWindowToTop();
	//}
	__super::OnActivate(nState, pWndOther, bMinimized);
}
