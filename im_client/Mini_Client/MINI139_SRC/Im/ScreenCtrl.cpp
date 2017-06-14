// ScreenCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCtrl.h"
#include "afxdialogex.h"
#include "RectTrackerEx.h"
#include "DlgSCreenCtrlTool.h"
#include <math.h>
// CScreenCtrl 对话框

IMPLEMENT_DYNAMIC(CScreenCtrl, CDialog)

	CScreenCtrl::CScreenCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCtrl::IDD, pParent)
{
	//屏幕大小
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	m_szScreen.cx = cx; 
	m_szScreen.cy = cy;

	//背景图片
	CWnd *pWnd = GetDesktopWindow();
	CDC *pDC = pParent->GetDC();
	m_bitmapScreen.CreateCompatibleBitmap(pDC,cx,cy);
	m_bitmapBack.CreateCompatibleBitmap(pDC,cx,cy);
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);
	CBitmap *pOld = dcTemp.SelectObject(&m_bitmapScreen);
	dcTemp.BitBlt(0,0,cx,cy,pDC,0,0,SRCCOPY);
	dcTemp.SelectObject(pOld);
	pParent->ReleaseDC(pDC);
	pOld->DeleteObject();
	dcTemp.DeleteDC();
	//选择区域初始化
	m_rect.left = 0;
	m_rect.right = 0;
	m_rect.top = 0;
	m_rect.bottom = 0;
	m_rectOld = m_rect;

	//鼠标按下初始化
	m_bIsBtnDown = FALSE;
	m_bIsBtnDownFirst = FALSE;

	//区域边框初始化
	m_rectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine;
	m_rectTracker.SetTrackerColor(RGB(51,255,255));
	m_rectTracker.SetTrackerType(TRACKER_TYPE_SCREEN);
	m_rectTracker.SetScreenCtrl(this);

	//鼠标状态初始化
	m_nHandle = -1;

	//编辑工具
	m_pDlgSCreenCtrlTool = NULL;

	m_bIsDrawBack = FALSE;
	m_bIsDrawRectangle = FALSE;
	m_bIsDrawEllipse = FALSE;
	m_bMouseDown = FALSE;
	m_bIsDrawArrow = FALSE;
	m_bIsMouseUp = FALSE;
	m_bIsDrawEdit = FALSE;
	m_bEditFlag = FALSE;

	m_dwDrawColor = RGB(255, 0, 0);
	m_nDrawSize = 2;
	m_nFontSize = 1;

	//GDI+初始化
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}

CScreenCtrl::~CScreenCtrl()
{
	if(m_pDlgSCreenCtrlTool != NULL)
	{
		delete m_pDlgSCreenCtrlTool;
		m_pDlgSCreenCtrlTool = NULL;
	}
	for(int nBitmap = 0; nBitmap < m_arrayBitmapAll.GetSize(); nBitmap++)
	{
		CBitmap* pBitmap = m_arrayBitmapAll.GetAt(nBitmap);
		pBitmap->DeleteObject();
	}
	GdiplusShutdown(m_pGdiToken);
}

void CScreenCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_edit);
}


BEGIN_MESSAGE_MAP(CScreenCtrl, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_MOVE()
	ON_MESSAGE(WM_EDIT_PIC_SAVE, OnEditPicSave)
	ON_MESSAGE(WM_DRAW_RECTTRACKER, DrawRectTracker)
END_MESSAGE_MAP()


// CScreenCtrl 消息处理程序


BOOL CScreenCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//窗口置顶，用于按层级获取下面的所有窗口
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	m_vecRc.clear();
	CRect rcScreen;  
	SystemParametersInfo(SPI_GETWORKAREA,   0,   &rcScreen,   0) ;   // 获得工作区大小 

	//获取任务栏的位置
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	CRect rcTask;
	rcTask.left = ::GetSystemMetrics(SM_CXSCREEN) - rcScreen.left;
	rcTask.top = ::GetSystemMetrics(SM_CYSCREEN) - rcScreen.bottom;
	rcTask.right = rcTask.left + ( ::GetSystemMetrics(SM_CXSCREEN)-rcScreen.Width() );
	rcTask.bottom = rcTask.top + ( ::GetSystemMetrics(SM_CYSCREEN)-rcScreen.Height() );
	if(cx == rcScreen.Width())
	{
		rcTask.left = rcScreen.left;
		rcTask.top =  rcScreen.bottom == cy ? 0 : rcScreen.bottom;
		rcTask.right = rcTask.left + rcScreen.Width();
		rcTask.bottom = rcTask.top + ( cy - rcScreen.Height() );
	}
	if(cy == rcScreen.Height())
	{
		rcTask.top = rcScreen.top;
		rcTask.left =  rcScreen.right == cx ? 0 : rcScreen.right;
		rcTask.right = rcTask.left + (cx - rcScreen.Width());
		rcTask.bottom = rcTask.top + rcScreen.Height();
	}
	m_vecRc.push_back(rcTask);

	//往下获取所有的窗口
	CWnd* pWnd = GetNextWindow(GW_HWNDNEXT);
	while (pWnd != NULL)
	{
		CWnd* pWndNew = pWnd;
		CRect rc;
		pWndNew->GetWindowRect(&rc);
		if(pWndNew->IsWindowVisible())
		{
			if(rc.left < 0)
				rc.left = 0;
			if(rc.top < 0)
				rc.top = 0;
			if(rc.right > rcScreen.right)
				rc.right = rcScreen.right;
			if(rc.bottom > rcScreen.bottom)
				rc.bottom = rcScreen.bottom;
			BOOL bExist = FALSE;
			for(int nRc = 0; nRc < m_vecRc.size(); nRc++)
			{
				if(rc == m_vecRc[nRc])
					bExist = TRUE;
			}
			if(!bExist)
				m_vecRc.push_back(rc);
		}
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
	}
	m_vecRc.push_back(rcScreen);

	//最大化固定当前屏幕
	ShowWindow(SW_MAXIMIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CScreenCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//灰色的背景只绘制一次，多次绘制会加深灰色的颜色
	if(!m_bIsDrawBack)
	{
		CDC memdc;
		CBitmap bmp,*pOldBitmap;
		memdc.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
		pOldBitmap=memdc.SelectObject(&bmp);
		memdc.FillSolidRect(0,0,m_szScreen.cx,m_szScreen.cy,RGB(128,128,128));
		BLENDFUNCTION bf;
		bf.BlendOp=AC_SRC_OVER;
		bf.BlendFlags=0;
		bf.SourceConstantAlpha=0x9f;
		bf.AlphaFormat=0;
		pDC->AlphaBlend(0,0,m_szScreen.cx,m_szScreen.cy,&memdc,0,0,m_szScreen.cx,m_szScreen.cy,bf);
		memdc.SelectObject(pOldBitmap);
		pDC->SetBkColor(TRANSPARENT);
		m_bIsDrawBack = TRUE;
		bmp.DeleteObject();
		pOldBitmap->DeleteObject();
		memdc.DeleteDC();
	}
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CScreenCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取鼠标移动的大小
	m_nMoveX = m_nDownX - point.x;
	m_nMoveY = m_nDownY - point.y;

	if(!m_bIsDrawRectangle && !m_bIsDrawEllipse && !m_bIsDrawArrow && !m_bIsDrawEdit)
	{
		//选择窗口区域
		if(!m_bIsBtnDown)
		{
			for(int nRect = 0; nRect < m_vecRc.size(); nRect++)
			{
				CRect rc = m_vecRc[nRect];
				if(rc.PtInRect(point))
				{
					m_rect = rc;
					if(m_rectOld != m_rect)
						Invalidate();
					m_rectOld = m_rect;
					break;
				}
			}
		}
		if(m_pDlgSCreenCtrlTool != NULL && m_pDlgSCreenCtrlTool->IsWindowVisible())
		{
			//m_pDlgSCreenCtrlTool->GetWindowRect(&m_rectTool);
			//m_pDlgSCreenCtrlTool->ShowWindow(SW_HIDE);
		}
		//拖动截屏区域
		if(m_bIsBtnDownFirst)
		{
			if(abs(m_nMoveX) >= 2 || abs(m_nMoveY) >= 2)
			{
				m_rect.left = m_nDownX;
				m_rect.top = m_nDownY;
				m_rect.right = m_nDownX/* - m_nMoveX*/;
				m_rect.bottom = m_nDownY/* - m_nMoveY*/;
				m_bIsBtnDown = TRUE;
				m_bIsBtnDownFirst = FALSE;
				if(m_nMoveX <= 0 && m_nMoveY <= 0)
					m_nHandle = 2;
				else if(m_nMoveX >= 0 && m_nMoveY >= 0)
					m_nHandle = 0;
				else if(m_nMoveX <= 0 && m_nMoveY >= 0)
					m_nHandle = 3;
				else if(m_nMoveX >= 0 && m_nMoveY <= 0)
					m_nHandle = 1;
				m_rectLast = m_rect;
				m_rectToolLast = m_rectTool;
				CDC* pDC = GetDC();
				{
					CDC dcBack;
					dcBack.CreateCompatibleDC(pDC);

					CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

					CDC dcTemp;
					dcTemp.CreateCompatibleDC(pDC);

					CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

					dcBack.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcTemp,0,0,SRCCOPY);

					pDC->BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcBack,0,0,SRCCOPY);

					dcTemp.SelectObject(pOldTemp);
					dcBack.SelectObject(pOldback);

					CDC memdc;
					CBitmap bmp,*pOldBitmap;
					memdc.CreateCompatibleDC(pDC);
					bmp.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
					pOldBitmap=memdc.SelectObject(&bmp);
					memdc.FillSolidRect(0,0,m_szScreen.cx,m_szScreen.cy,RGB(128,128,128));
					BLENDFUNCTION bf;
					bf.BlendOp=AC_SRC_OVER;
					bf.BlendFlags=0;
					bf.SourceConstantAlpha=0x9f;
					bf.AlphaFormat=0;
					pDC->AlphaBlend(0,0,m_szScreen.cx,m_szScreen.cy,&memdc,0,0,m_szScreen.cx,m_szScreen.cy,bf);

					memdc.SelectObject(pOldBitmap);
					pDC->SetBkColor(TRANSPARENT);
					pOldback->DeleteObject();
					pOldTemp->DeleteObject();
					dcBack.DeleteDC();
					dcTemp.DeleteDC();
					memdc.DeleteDC();
				}
				{
					CDC dcBack;
					dcBack.CreateCompatibleDC(pDC);

					CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

					CDC dcTemp;
					dcTemp.CreateCompatibleDC(pDC);

					CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

					dcBack.BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcTemp,m_rect.left,m_rect.top,SRCCOPY);

					pDC->BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcBack,m_rect.left,m_rect.top,SRCCOPY);

					dcTemp.SelectObject(pOldTemp);
					dcBack.SelectObject(pOldback);

					pOldTemp->DeleteObject();
					pOldback->DeleteObject();
					dcBack.DeleteDC();
					dcTemp.DeleteDC();
				}
			}
		}
		//改变截屏区域
		if( m_nHandle != -1)
		{
			if(m_nHandle == /*hitTopLeft*/0)//左上拖动
			{
				if(m_rect.left > m_rectLast.right)//超过右边变为右上
				{
					int nLeft = m_rect.left;
					int nRight = m_rect.right;
					m_rect.left = nRight;
					m_rect.right = nLeft;
					m_rectLast = m_rect;

					// 					nLeft = m_rectTool.left;
					// 					nRight = m_rectTool.right;
					// 					m_rectTool.left = nRight;
					// 					m_rectTool.right = nLeft;
					// 					m_rectToolLast = m_rectTool;

					m_nHandle = 1;
					return;
				}
				if(m_rect.top > m_rectLast.bottom)//超过底部变为左下
				{
					int nTop = m_rect.top;
					int nBottom = m_rect.bottom;
					m_rect.top = nBottom;
					m_rect.bottom = nTop;
					m_rectLast = m_rect;

					// 					nTop = m_rectTool.top;
					// 					nBottom = m_rectTool.bottom;
					// 					m_rectTool.top = nBottom;
					// 					m_rectTool.bottom = nTop;
					// 					m_rectToolLast = m_rectTool;

					m_nHandle = 3;
					return;
				}
				m_rect.left = m_rectLast.left - m_nMoveX;
				m_rect.top = m_rectLast.top - m_nMoveY;
				m_rect.right = m_rectLast.right;
				m_rect.bottom = m_rectLast.bottom;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitTopRight*/1)//右上拖动
			{
				if(m_rect.right < m_rectLast.left)//超过左边变为左上
				{
					int nLeft = m_rect.left;
					int nRight = m_rect.right;
					m_rect.left = nRight;
					m_rect.right = nLeft;
					m_rectLast = m_rect;

					// 					nLeft = m_rectTool.left;
					// 					nRight = m_rectTool.right;
					// 					m_rectTool.left = nRight;
					// 					m_rectTool.right = nLeft;
					// 					m_rectToolLast = m_rectTool;

					m_nHandle = 0;
					return;
				}
				if(m_rect.top > m_rectLast.bottom)//超过下面变为右下
				{
					int nTop = m_rect.top;
					int nBottom = m_rect.bottom;
					m_rect.top = nBottom;
					m_rect.bottom = nTop;
					m_rectLast = m_rect;

					// 					nTop = m_rectTool.top;
					// 					nBottom = m_rectTool.bottom;
					// 					m_rectTool.top = nBottom;
					// 					m_rectTool.bottom = nTop;
					// 					m_rectToolLast = m_rectTool;

					m_nHandle = 2;
					return;
				}
				m_rect.right = m_rectLast.right - m_nMoveX;
				m_rect.top = m_rectLast.top - m_nMoveY;
				m_rect.left = m_rectLast.left;
				m_rect.bottom = m_rectLast.bottom;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitBottomRight*/2)//右下拖动
			{
				if(m_rect.right < m_rectLast.left)//超过左边变为左下
				{
					int nLeft = m_rect.left;
					int nRight = m_rect.right;
					m_rect.left = nRight;
					m_rect.right = nLeft;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 3;
					return;
				}
				if(m_rect.bottom < m_rectLast.top)//超过上面变为右上
				{
					int nTop = m_rect.top;
					int nBottom = m_rect.bottom;
					m_rect.top = nBottom;
					m_rect.bottom = nTop;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 1;
					return;
				}
				m_rect.right = m_rectLast.right - m_nMoveX;
				m_rect.bottom = m_rectLast.bottom - m_nMoveY;
				m_rect.left = m_rectLast.left;
				m_rect.top = m_rectLast.top;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitBottomLeft*/3)//左下拖动
			{
				if(m_rect.left > m_rectLast.right)//超过右边变为右下
				{
					int nLeft = m_rect.left;
					int nRight = m_rect.right;
					m_rect.left = nRight;
					m_rect.right = nLeft;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 2;
					return;
				}
				if(m_rect.bottom < m_rectLast.top)//超过上面变为左上
				{
					int nTop = m_rect.top;
					int nBottom = m_rect.bottom;
					m_rect.top = nBottom;
					m_rect.bottom = nTop;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 0;
					return;
				}
				m_rect.left = m_rectLast.left - m_nMoveX;
				m_rect.bottom = m_rectLast.bottom - m_nMoveY;
				m_rect.right = m_rectLast.right;
				m_rect.top = m_rectLast.top;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitTop*/4)//向上拖动
			{
				if(m_rect.top > m_rectLast.bottom)//超过下面变为向下
				{
					int nTop = m_rect.top;
					int nBottom = m_rect.bottom;
					m_rect.top = nBottom;
					m_rect.bottom = nTop;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 6;
					return;
				}
				m_rect.left = m_rectLast.left;
				m_rect.bottom = m_rectLast.bottom;
				m_rect.right = m_rectLast.right;
				m_rect.top = m_rectLast.top - m_nMoveY;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitRight*/5)//向右拖动
			{
				if(m_rect.right < m_rectLast.left)//超过左边变为向左
				{
					int nLeft = m_rect.left;
					int nRight = m_rect.right;
					m_rect.left = nRight;
					m_rect.right = nLeft;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 7;
					return;
				}
				m_rect.left = m_rectLast.left;
				m_rect.bottom = m_rectLast.bottom;
				m_rect.right = m_rectLast.right - m_nMoveX;
				m_rect.top = m_rectLast.top;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitBottom*/6)//向下拖动
			{
				if(m_rect.bottom < m_rectLast.top)//超过上面变为向上
				{
					int nTop = m_rect.top;
					int nBottom = m_rect.bottom;
					m_rect.top = nBottom;
					m_rect.bottom = nTop;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 4;
					return;
				}
				m_rect.left = m_rectLast.left;
				m_rect.bottom = m_rectLast.bottom - m_nMoveY;
				m_rect.right = m_rectLast.right;
				m_rect.top = m_rectLast.top;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitLeft*/7)//向左拖动
			{
				if(m_rect.left > m_rectLast.right)//超过右边变为向右
				{
					int nLeft = m_rect.left;
					int nRight = m_rect.right;
					m_rect.left = nRight;
					m_rect.right = nLeft;
					m_rectLast = m_rect;
					m_rectToolLast = m_rectTool;
					m_nHandle = 5;
					return;
				}
				m_rect.left = m_rectLast.left - m_nMoveX;
				m_rect.bottom = m_rectLast.bottom;
				m_rect.right = m_rectLast.right;
				m_rect.top = m_rectLast.top;
				m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			}
			if(m_nHandle == /*hitMiddle*/8)//移动截屏区域
			{
				if(m_rectLast.left - m_nMoveX <= 0 || m_rectLast.right - m_nMoveX >= m_szScreen.cx)
				{
					if(m_rectLast.bottom - m_nMoveY <= m_szScreen.cy &&  m_rectLast.top - m_nMoveY >= 0)
					{
						m_rect.bottom = m_rectLast.bottom - m_nMoveY;
						m_rect.top = m_rectLast.top - m_nMoveY;
					}
					m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
				}
				else if(m_rectLast.bottom - m_nMoveY >= m_szScreen.cy ||  m_rectLast.top - m_nMoveY <= 0)
				{
					if(m_rectLast.left - m_nMoveX >= 0 && m_rectLast.right - m_nMoveX <= m_szScreen.cx)
					{
						m_rect.left = m_rectLast.left - m_nMoveX;
						m_rect.right = m_rectLast.right - m_nMoveX;
					}
					m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
				}
				else
				{
					m_rect.left = m_rectLast.left - m_nMoveX;
					m_rect.bottom = m_rectLast.bottom - m_nMoveY;
					m_rect.right = m_rectLast.right - m_nMoveX;
					m_rect.top = m_rectLast.top - m_nMoveY;
					m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
				}
			}
			//左上
			if(m_rectLast.left - m_rect.left < 0 && m_rectLast.top - m_rect.top < 0)
			{
				m_rectRefresh1.left = m_rectLast.left;
				m_rectRefresh1.top = m_rect.top;
				m_rectRefresh1.right = m_rect.left;
				m_rectRefresh1.bottom = m_rectLast.bottom;

				m_rectRefresh2.left = m_rectLast.left;
				m_rectRefresh2.top = m_rectLast.top;
				m_rectRefresh2.right = m_rect.left;
				m_rectRefresh2.bottom = m_rect.top;

				m_rectRefresh3.left = m_rect.left;
				m_rectRefresh3.top = m_rectLast.top;
				m_rectRefresh3.right = m_rectLast.right;
				m_rectRefresh3.bottom = m_rect.top;
			}
			//左
			else if(m_rectLast.left - m_rect.left < 0 && m_rectLast.top - m_rect.top >= 0 && m_rectLast.bottom - m_rect.bottom <= 0)
			{
				m_rectRefresh1.left = m_rectLast.left;
				m_rectRefresh1.top = m_rect.top;
				m_rectRefresh1.right = m_rect.left;
				m_rectRefresh1.bottom = m_rect.bottom;

				m_rectRefresh2.left = 0;
				m_rectRefresh2.top = 0;
				m_rectRefresh2.right = 0;
				m_rectRefresh2.bottom = 0;

				m_rectRefresh3.left = 0;
				m_rectRefresh3.top = 0;
				m_rectRefresh3.right = 0;
				m_rectRefresh3.bottom = 0;
			}
			//左下
			else if(m_rectLast.left - m_rect.left < 0 && m_rectLast.bottom - m_rect.bottom > 0)
			{
				m_rectRefresh1.left = m_rectLast.left;
				m_rectRefresh1.top = m_rectLast.top;
				m_rectRefresh1.right = m_rect.left;
				m_rectRefresh1.bottom = m_rect.bottom;

				m_rectRefresh2.left = m_rectLast.left;
				m_rectRefresh2.top = m_rect.bottom;
				m_rectRefresh2.right = m_rect.left;
				m_rectRefresh2.bottom = m_rectLast.bottom;

				m_rectRefresh3.left = m_rect.left;
				m_rectRefresh3.top = m_rect.bottom;
				m_rectRefresh3.right = m_rectLast.right;
				m_rectRefresh3.bottom = m_rectLast.bottom;
			}
			//下
			else if(m_rectLast.left - m_rect.left >= 0 && m_rectLast.right - m_rect.right <= 0 && m_rectLast.bottom - m_rect.bottom > 0)
			{
				m_rectRefresh1.left = 0;
				m_rectRefresh1.top = 0;
				m_rectRefresh1.right = 0;
				m_rectRefresh1.bottom = 0;

				m_rectRefresh2.left = 0;
				m_rectRefresh2.top = 0;
				m_rectRefresh2.right = 0;
				m_rectRefresh2.bottom = 0;

				m_rectRefresh3.left = m_rect.left;
				m_rectRefresh3.top = m_rect.bottom;
				m_rectRefresh3.right = m_rect.right;
				m_rectRefresh3.bottom = m_rectLast.bottom;
			}
			//右下
			else if(m_rectLast.right - m_rect.right > 0 && m_rectLast.bottom - m_rect.bottom > 0)
			{
				m_rectRefresh1.left = m_rectLast.left;
				m_rectRefresh1.top = m_rect.bottom;
				m_rectRefresh1.right = m_rect.right;
				m_rectRefresh1.bottom = m_rectLast.bottom;

				m_rectRefresh2.left = m_rect.right;
				m_rectRefresh2.top = m_rect.bottom;
				m_rectRefresh2.right = m_rectLast.right;
				m_rectRefresh2.bottom = m_rectLast.bottom;

				m_rectRefresh3.left = m_rect.right;
				m_rectRefresh3.top = m_rectLast.top;
				m_rectRefresh3.right = m_rectLast.right;
				m_rectRefresh3.bottom = m_rect.bottom;
			}
			//右
			else if(m_rectLast.right - m_rect.right > 0 && m_rectLast.bottom - m_rect.bottom <= 0 && m_rectLast.top - m_rect.top >= 0)
			{
				m_rectRefresh1.left = 0;
				m_rectRefresh1.top = 0;
				m_rectRefresh1.right = 0;
				m_rectRefresh1.bottom = 0;

				m_rectRefresh2.left = 0;
				m_rectRefresh2.top = 0;
				m_rectRefresh2.right = 0;
				m_rectRefresh2.bottom = 0;

				m_rectRefresh3.left = m_rect.right;
				m_rectRefresh3.top = m_rect.top;
				m_rectRefresh3.right = m_rectLast.right;
				m_rectRefresh3.bottom = m_rect.bottom;
			}
			//右上
			else if(m_rectLast.right - m_rect.right > 0 && m_rectLast.top - m_rect.top < 0)
			{
				m_rectRefresh1.left = m_rectLast.left;
				m_rectRefresh1.top = m_rectLast.top;
				m_rectRefresh1.right = m_rect.right;
				m_rectRefresh1.bottom = m_rect.top;

				m_rectRefresh2.left = m_rect.right;
				m_rectRefresh2.top = m_rectLast.top;
				m_rectRefresh2.right = m_rectLast.right;
				m_rectRefresh2.bottom = m_rect.top;

				m_rectRefresh3.left = m_rect.right;
				m_rectRefresh3.top = m_rect.top;
				m_rectRefresh3.right = m_rectLast.right;
				m_rectRefresh3.bottom = m_rectLast.bottom;
			}
			//上
			else if(m_rectLast.right - m_rect.right <= 0 && m_rectLast.left - m_rect.left >= 0 && m_rectLast.top - m_rect.top < 0)
			{
				m_rectRefresh1.left = m_rect.left;
				m_rectRefresh1.top = m_rectLast.top;
				m_rectRefresh1.right = m_rect.right;
				m_rectRefresh1.bottom = m_rect.top;

				m_rectRefresh2.left = 0;
				m_rectRefresh2.top = 0;
				m_rectRefresh2.right = 0;
				m_rectRefresh2.bottom = 0;

				m_rectRefresh3.left = 0;
				m_rectRefresh3.top = 0;
				m_rectRefresh3.right = 0;
				m_rectRefresh3.bottom = 0;
			}
			else
			{
				m_rectRefresh1.left = 0;
				m_rectRefresh1.top = 0;
				m_rectRefresh1.right = 0;
				m_rectRefresh1.bottom = 0;

				m_rectRefresh2.left = 0;
				m_rectRefresh2.top = 0;
				m_rectRefresh2.right = 0;
				m_rectRefresh2.bottom = 0;

				m_rectRefresh3.left = 0;
				m_rectRefresh3.top = 0;
				m_rectRefresh3.right = 0;
				m_rectRefresh3.bottom = 0;
			}
			if(m_rectToolLast.left > m_rectTool.left && m_rectToolLast.top > m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectToolLast.left;
				m_rectRefresh4.top = m_rectTool.bottom;
				m_rectRefresh4.right = m_rectTool.right;
				m_rectRefresh4.bottom = m_rectToolLast.bottom;

				m_rectRefresh5.left = m_rectTool.right;
				m_rectRefresh5.top = m_rectTool.bottom;
				m_rectRefresh5.right = m_rectToolLast.right;
				m_rectRefresh5.bottom = m_rectToolLast.bottom;

				m_rectRefresh6.left = m_rectTool.right;
				m_rectRefresh6.top = m_rectToolLast.top;
				m_rectRefresh6.right = m_rectToolLast.right;
				m_rectRefresh6.bottom = m_rectTool.bottom;
			}
			else if(m_rectToolLast.left < m_rectTool.left && m_rectToolLast.top > m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectToolLast.left;
				m_rectRefresh4.top = m_rectToolLast.top;
				m_rectRefresh4.right = m_rectTool.left;
				m_rectRefresh4.bottom = m_rectTool.bottom;

				m_rectRefresh5.left = m_rectToolLast.left;
				m_rectRefresh5.top = m_rectTool.bottom;
				m_rectRefresh5.right = m_rectTool.left;
				m_rectRefresh5.bottom = m_rectToolLast.bottom;

				m_rectRefresh6.left = m_rectTool.left;
				m_rectRefresh6.top = m_rectTool.bottom;
				m_rectRefresh6.right = m_rectToolLast.right;
				m_rectRefresh6.bottom = m_rectToolLast.bottom;
			}
			else if(m_rectToolLast.left > m_rectTool.left && m_rectToolLast.top < m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectToolLast.left;
				m_rectRefresh4.top = m_rectToolLast.top;
				m_rectRefresh4.right = m_rectTool.right;
				m_rectRefresh4.bottom = m_rectTool.top;

				m_rectRefresh5.left = m_rectTool.right;
				m_rectRefresh5.top = m_rectToolLast.top;
				m_rectRefresh5.right = m_rectToolLast.right;
				m_rectRefresh5.bottom = m_rectTool.top;

				m_rectRefresh6.left = m_rectTool.right;
				m_rectRefresh6.top = m_rectTool.top;
				m_rectRefresh6.right = m_rectToolLast.right;
				m_rectRefresh6.bottom = m_rectToolLast.bottom;
			}
			else if(m_rectToolLast.left < m_rectTool.left && m_rectToolLast.top < m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectToolLast.left;
				m_rectRefresh4.top = m_rectTool.top;
				m_rectRefresh4.right = m_rectTool.left;
				m_rectRefresh4.bottom = m_rectToolLast.bottom;

				m_rectRefresh5.left = m_rectToolLast.left;
				m_rectRefresh5.top = m_rectToolLast.top;
				m_rectRefresh5.right = m_rectTool.left;
				m_rectRefresh5.bottom = m_rectTool.top;

				m_rectRefresh6.left = m_rectTool.left;
				m_rectRefresh6.top = m_rectToolLast.top;
				m_rectRefresh6.right = m_rectToolLast.right;
				m_rectRefresh6.bottom = m_rectTool.top;
			}
			else if(m_rectToolLast.left > m_rectTool.left && m_rectToolLast.top == m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectTool.right;
				m_rectRefresh4.top = m_rectTool.top;
				m_rectRefresh4.right = m_rectToolLast.right;
				m_rectRefresh4.bottom = m_rectTool.bottom;
			}
			else if(m_rectToolLast.left < m_rectTool.left && m_rectToolLast.top == m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectToolLast.left;
				m_rectRefresh4.top = m_rectTool.top;
				m_rectRefresh4.right = m_rectTool.left;
				m_rectRefresh4.bottom = m_rectTool.bottom;
			}
			else if(m_rectToolLast.left == m_rectTool.left && m_rectToolLast.top < m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectTool.left;
				m_rectRefresh4.top = m_rectToolLast.top;
				m_rectRefresh4.right = m_rectTool.right;
				m_rectRefresh4.bottom = m_rectTool.top;
			}
			else if(m_rectToolLast.left == m_rectTool.left && m_rectToolLast.top > m_rectTool.top)
			{
				m_rectRefresh4.left = m_rectTool.left;
				m_rectRefresh4.top = m_rectTool.bottom;
				m_rectRefresh4.right = m_rectTool.right;
				m_rectRefresh4.bottom = m_rectToolLast.bottom;
			}
			CDC* pDC = GetDC();
			//重新绘制截屏区域
			{
				CDC dcBack;
				dcBack.CreateCompatibleDC(pDC);

				CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

				CDC dcTemp;
				dcTemp.CreateCompatibleDC(pDC);

				CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

				dcBack.BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcTemp,m_rect.left,m_rect.top,SRCCOPY);

				pDC->BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcBack,m_rect.left,m_rect.top,SRCCOPY);

				dcTemp.SelectObject(pOldTemp);
				dcBack.SelectObject(pOldback);

				pOldTemp->DeleteObject();
				pOldback->DeleteObject();
				dcBack.DeleteDC();
				dcTemp.DeleteDC();
			}
			//重新绘制需要刷新的3个区域
			CDC dcBack;
			dcBack.CreateCompatibleDC(pDC);

			CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

			CDC dcTemp;
			dcTemp.CreateCompatibleDC(pDC);

			CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

			dcBack.BitBlt(m_rectRefresh1.left,m_rectRefresh1.top,m_rectRefresh1.Width(),m_rectRefresh1.Height(),&dcTemp,m_rectRefresh1.left,m_rectRefresh1.top,SRCCOPY);
			pDC->BitBlt(m_rectRefresh1.left,m_rectRefresh1.top,m_rectRefresh1.Width(),m_rectRefresh1.Height(),&dcBack,m_rectRefresh1.left,m_rectRefresh1.top,SRCCOPY);

			dcBack.BitBlt(m_rectRefresh2.left,m_rectRefresh2.top,m_rectRefresh2.Width(),m_rectRefresh2.Height(),&dcTemp,m_rectRefresh2.left,m_rectRefresh2.top,SRCCOPY);
			pDC->BitBlt(m_rectRefresh2.left,m_rectRefresh2.top,m_rectRefresh2.Width(),m_rectRefresh2.Height(),&dcBack,m_rectRefresh2.left,m_rectRefresh2.top,SRCCOPY);

			dcBack.BitBlt(m_rectRefresh3.left,m_rectRefresh3.top,m_rectRefresh3.Width(),m_rectRefresh3.Height(),&dcTemp,m_rectRefresh3.left,m_rectRefresh3.top,SRCCOPY);
			pDC->BitBlt(m_rectRefresh3.left,m_rectRefresh3.top,m_rectRefresh3.Width(),m_rectRefresh3.Height(),&dcBack,m_rectRefresh3.left,m_rectRefresh3.top,SRCCOPY);

			dcBack.BitBlt(m_rectRefresh4.left,m_rectRefresh4.top,m_rectRefresh4.Width(),m_rectRefresh4.Height(),&dcTemp,m_rectRefresh4.left,m_rectRefresh4.top,SRCCOPY);
			pDC->BitBlt(m_rectRefresh4.left,m_rectRefresh4.top,m_rectRefresh4.Width(),m_rectRefresh4.Height(),&dcBack,m_rectRefresh4.left,m_rectRefresh4.top,SRCCOPY);

			dcBack.BitBlt(m_rectRefresh5.left,m_rectRefresh5.top,m_rectRefresh5.Width(),m_rectRefresh5.Height(),&dcTemp,m_rectRefresh5.left,m_rectRefresh5.top,SRCCOPY);
			pDC->BitBlt(m_rectRefresh5.left,m_rectRefresh5.top,m_rectRefresh5.Width(),m_rectRefresh5.Height(),&dcBack,m_rectRefresh5.left,m_rectRefresh5.top,SRCCOPY);

			dcBack.BitBlt(m_rectRefresh6.left,m_rectRefresh6.top,m_rectRefresh6.Width(),m_rectRefresh6.Height(),&dcTemp,m_rectRefresh6.left,m_rectRefresh6.top,SRCCOPY);
			pDC->BitBlt(m_rectRefresh6.left,m_rectRefresh6.top,m_rectRefresh6.Width(),m_rectRefresh6.Height(),&dcBack,m_rectRefresh6.left,m_rectRefresh6.top,SRCCOPY);

			dcTemp.SelectObject(pOldTemp);
			dcBack.SelectObject(pOldback);

			//重新将3个区域绘制为灰色透明
			CDC memdc;
			CBitmap bmp,*pOldBitmap;
			memdc.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
			pOldBitmap=memdc.SelectObject(&bmp);
			memdc.FillSolidRect(0,0,m_szScreen.cx,m_szScreen.cy,RGB(128,128,128));
			BLENDFUNCTION bf;
			bf.BlendOp=AC_SRC_OVER;
			bf.BlendFlags=0;
			bf.SourceConstantAlpha=0x9f;
			bf.AlphaFormat=0;
			pDC->AlphaBlend(m_rectRefresh1.left,m_rectRefresh1.top,m_rectRefresh1.Width(),m_rectRefresh1.Height(),&memdc,m_rectRefresh1.left,m_rectRefresh1.top,m_rectRefresh1.Width(),m_rectRefresh1.Height(),bf);
			pDC->AlphaBlend(m_rectRefresh2.left,m_rectRefresh2.top,m_rectRefresh2.Width(),m_rectRefresh2.Height(),&memdc,m_rectRefresh2.left,m_rectRefresh2.top,m_rectRefresh2.Width(),m_rectRefresh2.Height(),bf);
			pDC->AlphaBlend(m_rectRefresh3.left,m_rectRefresh3.top,m_rectRefresh3.Width(),m_rectRefresh3.Height(),&memdc,m_rectRefresh3.left,m_rectRefresh3.top,m_rectRefresh3.Width(),m_rectRefresh3.Height(),bf);
			pDC->AlphaBlend(m_rectRefresh4.left,m_rectRefresh4.top,m_rectRefresh4.Width(),m_rectRefresh4.Height(),&memdc,m_rectRefresh4.left,m_rectRefresh4.top,m_rectRefresh4.Width(),m_rectRefresh4.Height(),bf);
			pDC->AlphaBlend(m_rectRefresh5.left,m_rectRefresh5.top,m_rectRefresh5.Width(),m_rectRefresh5.Height(),&memdc,m_rectRefresh5.left,m_rectRefresh5.top,m_rectRefresh5.Width(),m_rectRefresh5.Height(),bf);
			pDC->AlphaBlend(m_rectRefresh6.left,m_rectRefresh6.top,m_rectRefresh6.Width(),m_rectRefresh6.Height(),&memdc,m_rectRefresh6.left,m_rectRefresh6.top,m_rectRefresh6.Width(),m_rectRefresh6.Height(),bf);
			memdc.SelectObject(pOldBitmap);

			pOldTemp->DeleteObject();
			pOldback->DeleteObject();
			bmp.DeleteObject();
			pOldBitmap->DeleteObject();
			memdc.DeleteDC();
			dcBack.DeleteDC();
			dcTemp.DeleteDC();

			//截屏工具跟随区域右下角移动
			if(m_pDlgSCreenCtrlTool != NULL && m_pDlgSCreenCtrlTool->IsWindowVisible())
			{
				CRect rcTool;
				m_pDlgSCreenCtrlTool->GetWindowRect(&rcTool);

				int nRight = m_rect.right > m_rect.left ? m_rect.right : m_rect.left;
				int nBottom = m_rect.bottom > m_rect.top ? m_rect.bottom : m_rect.top;
				m_pDlgSCreenCtrlTool->MoveWindow(nRight - rcTool.Width(), nBottom + 5, rcTool.Width(), rcTool.Height());
			}

			m_rectLast = m_rect;
			m_rectToolLast = m_rectTool;
		}
	}
	else
	{
		m_rectTracker.m_bIsDrawPointCross = TRUE;
		m_rectTracker.HitTestHandles(point);
		if( m_bMouseDown )  
		{  
			CDC* pDC = GetDC();
			CPen NewPen(PS_SOLID, /*2*/m_nDrawSize, /*RGB(255,0,0)*/m_dwDrawColor);//设置的大小和颜色  
			pDC->SelectObject(&NewPen);  
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->SetROP2( /*R2_NOT*/R2_NOTXORPEN );//绘制模式 
			//绘制上一次的图形，相当于擦除
			if(m_bIsDrawRectangle)//矩形
				pDC->Rectangle(m_ptStart.x,m_ptStart.y,m_ptOld.x,m_ptOld.y); 
			if(m_bIsDrawEllipse)//圆形
				pDC->Ellipse(m_ptStart.x,m_ptStart.y,m_ptOld.x,m_ptOld.y); 
			if(m_bIsDrawArrow)//箭头
			{
				pDC->MoveTo(m_ptStart.x,m_ptStart.y);
				pDC->LineTo(m_ptOld.x,m_ptOld.y);

				double slopy = 0, cosy = 0, siny = 0;
				double par = 15.0;  //length of Arrow (>)
				slopy = atan2( double( m_ptStart.y - m_ptOld.y ),double( m_ptStart.x - m_ptOld.x ) );
				cosy = cos( slopy );
				siny = sin( slopy ); 

				// 计算出三角箭头的两端点的坐标
				CPoint pt1( m_ptOld.x + int( par * cosy - ( par / 2.0 * siny ) ), 
					m_ptOld.y + int( par * siny + ( par / 2.0 * cosy ) ) );
				CPoint pt2( m_ptOld.x + int( par * cosy + par / 2.0 * siny ),
					m_ptOld.y - int( par / 2.0 * cosy - par * siny ) );

				pDC->MoveTo( m_ptOld );
				pDC->LineTo( (m_pt1Old.x + m_pt2Old.x) / 2 + 1, (m_pt1Old.y + m_pt2Old.y) / 2 + 1);

				CBrush br;
				br.CreateSolidBrush(/*RGB(255,0,0)*/m_dwDrawColor);
				pDC->SelectObject(&br);

				CPoint ptArray[3] = { m_ptOld, m_pt1Old, m_pt2Old };
				pDC->Polygon( ptArray, 3 ); // 画箭头处的三角形

				HRGN hRgn = CreatePolygonRgn( ptArray, 3, ALTERNATE ); // 创建箭头处的三角区域
				if ( hRgn != NULL )
				{
					CBrush brush( /*RGB(255,0,0)*/m_dwDrawColor ); // 填充红色
					CBrush *pOldBrush = pDC->SelectObject( &brush );
					brush.DeleteObject();
					pOldBrush->DeleteObject();
				}
				br.DeleteObject();
			}
			//绘制新的图形
			pDC->MoveTo( m_ptStart ); //回到起点
			//dc.LineTo( point );   //    
			if(m_bIsDrawRectangle)//矩形
				pDC->Rectangle(m_ptStart.x,m_ptStart.y,point.x,point.y);  
			if(m_bIsDrawEllipse)//圆形
				pDC->Ellipse(m_ptStart.x,m_ptStart.y,point.x,point.y); 
			if(m_bIsDrawArrow)//箭头
			{
				pDC->MoveTo(m_ptStart.x,m_ptStart.y);
				pDC->LineTo(point.x,point.y);

				double slopy = 0, cosy = 0, siny = 0;
				double par = 15.0;
				slopy = atan2( double( m_ptStart.y - m_ptOld.y ),double( m_ptStart.x - m_ptOld.x ) );
				cosy = cos( slopy );
				siny = sin( slopy ); 

				// 画起点到终点的直线

				// 计算出三角箭头的两端点的坐标
				CPoint pt1( point.x + int( par * cosy - ( par / 2.0 * siny ) ), 
					point.y + int( par * siny + ( par / 2.0 * cosy ) ) );
				CPoint pt2( point.x + int( par * cosy + par / 2.0 * siny ),
					point.y - int( par / 2.0 * cosy - par * siny ) );

				pDC->MoveTo( point );
				pDC->LineTo( (pt1.x + pt2.x) / 2 + 1, (pt1.y + pt2.y) / 2 + 1);

				CBrush br;
				br.CreateSolidBrush(/*RGB(255,0,0)*/m_dwDrawColor);
				pDC->SelectObject(&br);

				CPoint ptArray[3] = { point, pt1, pt2 };
				pDC->Polygon( ptArray, 3 ); // 画箭头处的三角形

				HRGN hRgn = CreatePolygonRgn( ptArray, 3, ALTERNATE ); // 创建箭头处的三角区域
				if ( hRgn != NULL )
				{
					CBrush brush( /*RGB(255,0,0)*/m_dwDrawColor ); // 填充红色
					CBrush *pOldBrush = pDC->SelectObject( &brush );
					brush.DeleteObject();
					pOldBrush->DeleteObject();
				}
				m_pt1Old = pt1;
				m_pt2Old = pt2;

				br.DeleteObject();
			}
			NewPen.DeleteObject();
			m_ptOld = point; //鼠标当前位置在下一次鼠标移动事件看来就是"旧位置"   
		}
	}
	m_nDownX = point.x;
	m_nDownY = point.y;
	CDialog::OnMouseMove(nFlags, point);
}

HBITMAP CScreenCtrl::CopyBitmap(HBITMAP hSourceHbitmap)
{
	CDC sourceDC;
	CDC destDC;
	sourceDC.CreateCompatibleDC(NULL);
	destDC.CreateCompatibleDC(NULL);
	//The bitmap information.
	BITMAP bm = {0};
	//Get the bitmap information.
	::GetObject(hSourceHbitmap, sizeof(bm), &bm);
	// Create a bitmap to hold the result
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), bm.bmWidth, bm.bmHeight);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(sourceDC.m_hDC, hSourceHbitmap);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(destDC.m_hDC, hbmResult);
	destDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY );

	// Restore DCs
	::SelectObject(sourceDC.m_hDC, hbmOldSource);
	::SelectObject(destDC.m_hDC, hbmOldDest);
	::DeleteObject(sourceDC.m_hDC);
	::DeleteObject(destDC.m_hDC);

	return hbmResult;
}

void CScreenCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	if(m_rect.Width() == 0 || m_rect.Height() == 0)
		return;

	CDC *pDC = GetDC();

	if((!m_bIsBtnDown && !m_bIsDrawRectangle && !m_bIsDrawEllipse && !m_bIsDrawArrow && !m_bIsDrawEdit))
	{
		//重新绘制灰色背景
		CDC dcBack;
		dcBack.CreateCompatibleDC(pDC);

		CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

		CDC dcTemp;
		dcTemp.CreateCompatibleDC(pDC);

		CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

		dcBack.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcTemp,0,0,SRCCOPY);

		pDC->BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcBack,0,0,SRCCOPY);

		dcTemp.SelectObject(pOldTemp);
		dcBack.SelectObject(pOldback);

		CDC memdc;
		CBitmap bmp,*pOldBitmap;
		memdc.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
		pOldBitmap=memdc.SelectObject(&bmp);
		memdc.FillSolidRect(0,0,m_szScreen.cx,m_szScreen.cy,RGB(128,128,128));
		BLENDFUNCTION bf;
		bf.BlendOp=AC_SRC_OVER;
		bf.BlendFlags=0;
		bf.SourceConstantAlpha=0x9f;
		bf.AlphaFormat=0;
		pDC->AlphaBlend(0,0,m_szScreen.cx,m_szScreen.cy,&memdc,0,0,m_szScreen.cx,m_szScreen.cy,bf);

		memdc.SelectObject(pOldBitmap);
		pDC->SetBkColor(TRANSPARENT);
		pOldback->DeleteObject();
		pOldTemp->DeleteObject();
		dcBack.DeleteDC();
		dcTemp.DeleteDC();
		memdc.DeleteDC();
	}

	if(!m_bIsDrawRectangle && !m_bIsDrawEllipse && !m_bIsDrawArrow && !m_bIsDrawEdit)
	{
		//重新绘制截屏区域
		CDC dcBack;
		dcBack.CreateCompatibleDC(pDC);

		CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

		CDC dcTemp;
		dcTemp.CreateCompatibleDC(pDC);

		CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

		dcBack.BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcTemp,m_rect.left,m_rect.top,SRCCOPY);

		pDC->BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcBack,m_rect.left,m_rect.top,SRCCOPY);

		dcTemp.SelectObject(pOldTemp);
		dcBack.SelectObject(pOldback);

		pOldTemp->DeleteObject();
		pOldback->DeleteObject();
		dcBack.DeleteDC();
		dcTemp.DeleteDC();
	}

	if(!m_bIsBtnDown)
	{
		//绘制窗口边框
		CPen NewPen(PS_SOLID, 4, RGB(51,255,255));  
		pDC->SelectObject(&NewPen);  
		pDC->SelectStockObject(NULL_BRUSH);   
		pDC->Rectangle(m_rect.left, m_rect .top
			, m_rect .right, m_rect .bottom);  
		NewPen.DeleteObject();
	}

	if(m_bIsMouseUp)//绘制图形抬起鼠，重新绘制截屏区域，保存图片之后再绘制边框
	{
		//重新绘制截屏区域
		CDC dcBack;
		dcBack.CreateCompatibleDC(pDC);

		CBitmap *pOldback = dcBack.SelectObject(&m_bitmapLast);

		CDC dcTemp;
		dcTemp.CreateCompatibleDC(pDC);

		CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

		dcBack.BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcTemp,m_rect.left,m_rect.top,SRCCOPY);

		pDC->BitBlt(m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),&dcBack,m_rect.left,m_rect.top,SRCCOPY);

		dcTemp.SelectObject(pOldTemp);
		dcBack.SelectObject(pOldback);

		//保存没有绘制边框的图片
		CBitmap bitmapLast;
		m_bitmapLast.DeleteObject();

		m_bitmapLast.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
		CDC MemDC;
		MemDC.DeleteDC();
		MemDC.CreateCompatibleDC(pDC);

		CBitmap *oldbitmap=MemDC.SelectObject(&m_bitmapLast);
		MemDC.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,pDC,0,0,SRCCOPY);
		MemDC.SelectObject(oldbitmap);
		MemDC.DeleteDC();

		//保存图片集合的第一张图片，用于撤销编辑
		CBitmap *newBitmap = CBitmap::FromHandle(CopyBitmap((HBITMAP)m_bitmapLast.m_hObject));
		if(m_arrayBitmap.GetSize() == 0)
			m_arrayBitmap.Add(newBitmap);
		else
			m_arrayBitmap.GetAt(0) = newBitmap;
		m_arrayBitmapAll.Add(newBitmap);

		//绘制边框
		CRect rect;
		m_rectTracker.GetTrueRect(&m_rect);
		pDC->SelectStockObject(NULL_BRUSH);   
		m_rectTracker.Draw(pDC/*, &pen*/);

		oldbitmap->DeleteObject();
		m_bIsMouseUp = FALSE;
		dcBack.DeleteDC();
		dcTemp.DeleteDC();
	}
	if(m_bIsBtnDown)
	{
		//编辑工具跟随移动
		if(m_pDlgSCreenCtrlTool != NULL && m_pDlgSCreenCtrlTool->IsWindowVisible())
		{
			CRect rcTool;
			m_pDlgSCreenCtrlTool->GetWindowRect(&rcTool);
			m_pDlgSCreenCtrlTool->MoveWindow(m_rect.right - rcTool.Width(), m_rect.bottom + 5, rcTool.Width(), rcTool.Height());
		}
	}
}


void CScreenCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bIsDrawRectangle && !m_bIsDrawEllipse && !m_bIsDrawArrow && !m_bIsDrawEdit)
	{
		if(!m_bIsBtnDown)
		{
			//创建编辑工具
			if(m_pDlgSCreenCtrlTool == NULL)
			{
				m_pDlgSCreenCtrlTool = new CDlgSCreenCtrlTool(NULL,this);
				m_pDlgSCreenCtrlTool->Create(CDlgSCreenCtrlTool::IDD, this);
			}
			CRect rcTool;
			m_pDlgSCreenCtrlTool->GetWindowRect(&rcTool);
			m_pDlgSCreenCtrlTool->SetWindowPos(this, m_rect.right - rcTool.Width(), m_rect.bottom + 5, /*m_rect.right*/rcTool.Width(), /*m_rect.bottom + 5 +*/ rcTool.Height(), SWP_SHOWWINDOW);
			m_pDlgSCreenCtrlTool->ShowWindow(SW_HIDE);
			m_bIsBtnDownFirst = TRUE;
			m_nMoveX = 0;
			m_nMoveY = 0;
			m_nDownX = point.x;
			m_nDownY = point.y;
		}
		if(m_bIsBtnDown)
		{
			m_nDownX = point.x;
			m_nDownY = point.y;
			m_nMoveX = 0;
			m_nMoveY = 0;
			m_nHandle = m_rectTracker.HitTestHandles(point);
			m_rectTracker.GetTrueRect(&m_rect);
			m_rectLast = m_rect;
			m_rectToolLast = m_rectTool;

			//按下的时候重新绘制截屏区域，不绘制边框
			CDC* pDC = GetDC();
			CDC memDcLast;
			memDcLast.CreateCompatibleDC(pDC);
			memDcLast.SelectObject(&m_bitmapLast);
			pDC->BitBlt(0, 0, m_szScreen.cx, m_szScreen.cy, &memDcLast, 0, 0, SRCCOPY);
			memDcLast.DeleteDC();
			Invalidate();
		}

		if(m_pDlgSCreenCtrlTool != NULL && m_pDlgSCreenCtrlTool->IsWindowVisible())
		{
			//m_pDlgSCreenCtrlTool->GetWindowRect(&m_rectTool);
			m_pDlgSCreenCtrlTool->ShowWindow(SW_HIDE);
			CRect rcTool;
			m_pDlgSCreenCtrlTool->GetWindowRect(&rcTool);
			CDC* pDC = GetDC();
			CDC dcBack;
			dcBack.CreateCompatibleDC(pDC);

			CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

			CDC dcTemp;
			dcTemp.CreateCompatibleDC(pDC);

			CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

			dcBack.BitBlt(rcTool.left-1,rcTool.top-1,rcTool.Width()+2,rcTool.Height()+2,&dcTemp,rcTool.left-1,rcTool.top-1,SRCCOPY);
			pDC->BitBlt(rcTool.left-1,rcTool.top-1,rcTool.Width()+2,rcTool.Height()+2,&dcBack,rcTool.left-1,rcTool.top-1,SRCCOPY);

			dcTemp.SelectObject(pOldTemp);
			dcBack.SelectObject(pOldback);

			//重新将3个区域绘制为灰色透明
			CDC memdc;
			CBitmap bmp,*pOldBitmap;
			memdc.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
			pOldBitmap=memdc.SelectObject(&bmp);
			memdc.FillSolidRect(0,0,m_szScreen.cx,m_szScreen.cy,RGB(128,128,128));
			BLENDFUNCTION bf;
			bf.BlendOp=AC_SRC_OVER;
			bf.BlendFlags=0;
			bf.SourceConstantAlpha=0x9f;
			bf.AlphaFormat=0;
			pDC->AlphaBlend(rcTool.left-1,rcTool.top-1,rcTool.Width()+2,rcTool.Height()+2,&memdc,rcTool.left-1,rcTool.top-1,rcTool.Width()+2,rcTool.Height()+2,bf);
			memdc.SelectObject(pOldBitmap);

			pOldTemp->DeleteObject();
			pOldback->DeleteObject();
			bmp.DeleteObject();
			pOldBitmap->DeleteObject();
			memdc.DeleteDC();
			dcBack.DeleteDC();
			dcTemp.DeleteDC();
			ReleaseDC(pDC);
		}
	}
	else
	{
		//在截屏区域外无法编辑
		CPoint ptCursor;
		GetCursorPos(&ptCursor);	
		if(!m_rect.PtInRect(ptCursor))
			return;

		if(m_bIsDrawEdit)//文字编辑模式
		{
			if(!m_bEditFlag)//非编辑状态创建透明编辑框
			{
				CRect rect(point.x, point.y, m_rect.right, m_rect.bottom);
				m_edit.m_bitmapLast = CBitmap::FromHandle(CopyBitmap((HBITMAP)m_bitmapLast.m_hObject));
				m_edit.SetParentRect(m_rect);
				m_edit.m_bIsPaint = TRUE;
				m_edit.m_bIsDrawRectangle = TRUE;
				m_edit.m_bIsSavePicture = TRUE;
				m_edit.SetWindowText("");
				m_edit.MoveWindow(&rect);
				m_edit.Invalidate();
				m_edit.ShowWindow(SW_SHOW );
				m_bEditFlag = TRUE;
			}
			else//编辑状态完成一次文字编辑
			{
				m_edit.m_bIsDrawRectangle = FALSE;
				m_edit.Invalidate();
				m_bEditFlag = FALSE;
			}
			//防止编辑工具消失
			SetCapture(); 
			ReleaseCapture();
		}
		else//图形绘制模式
		{
			if(m_bEditFlag)
			{
				m_edit.m_bIsDrawRectangle = FALSE;
				m_edit.Invalidate();
				m_bEditFlag = FALSE;	
			}
			m_bMouseDown = true; 
			m_ptStart = point; 
			m_ptOld = point;  
			m_pt1Old = point;
			m_pt2Old = point;
			SetCapture(); 
			//GetClientRect(&m_rect);
			//ClientToScreen(&m_rect); 
			//ClipCursor(&m_rect);
		} 
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CScreenCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bIsDrawRectangle && !m_bIsDrawEllipse && !m_bIsDrawArrow && !m_bIsDrawEdit)
	{
		if(!m_bIsBtnDown)
		{
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);//设置边框位置

			m_bIsBtnDown = TRUE;

			//绘制背景
			CDC* pDC = GetDC();
			CDC dcBack;
			dcBack.CreateCompatibleDC(pDC);

			CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

			CDC dcTemp;
			dcTemp.CreateCompatibleDC(pDC);

			CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

			dcBack.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcTemp,0,0,SRCCOPY);

			pDC->BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcBack,0,0,SRCCOPY);

			dcTemp.SelectObject(pOldTemp);
			dcBack.SelectObject(pOldback);

			//绘制灰色背景
			CDC memdc;
			CBitmap bmp,*pOldBitmap;
			memdc.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
			pOldBitmap=memdc.SelectObject(&bmp);
			memdc.FillSolidRect(0,0,m_szScreen.cx,m_szScreen.cy,RGB(128,128,128));
			BLENDFUNCTION bf;
			bf.BlendOp=AC_SRC_OVER;
			bf.BlendFlags=0;
			bf.SourceConstantAlpha=0x9f;
			bf.AlphaFormat=0;
			pDC->AlphaBlend(0,0,m_szScreen.cx,m_szScreen.cy,&memdc,0,0,m_szScreen.cx,m_szScreen.cy,bf);

			memdc.SelectObject(pOldBitmap);
			pDC->SetBkColor(TRANSPARENT);
			pOldback->DeleteObject();
			pOldTemp->DeleteObject();
			dcBack.DeleteDC();
			dcTemp.DeleteDC();
			memdc.DeleteDC();
		}
		else
		{
			m_rectTracker.GetTrueRect(&m_rect);
		}
		m_bIsMouseUp = TRUE;
		if(m_pDlgSCreenCtrlTool != NULL)
		{
			CRect rcTool;
			m_pDlgSCreenCtrlTool->GetWindowRect(&rcTool);

			int nRight = m_rect.right > m_rect.left ? m_rect.right : m_rect.left;
			int nBottom = m_rect.bottom > m_rect.top ? m_rect.bottom : m_rect.top;
			m_pDlgSCreenCtrlTool->MoveWindow(nRight - rcTool.Width(), nBottom + 5, rcTool.Width(), rcTool.Height());
			m_pDlgSCreenCtrlTool->ShowWindow(SW_SHOW);
		}
		Invalidate();
		m_bIsBtnDownFirst = FALSE;
		m_nHandle = -1;
	}
	else
	{
		if( m_bMouseDown && !m_bIsDrawEdit)//绘制图形
		{   
			CDC* pDC = GetDC();

			CDC memDcLast;
			memDcLast.CreateCompatibleDC(pDC);
			memDcLast.SelectObject(&m_bitmapLast);
			pDC->BitBlt(0, 0, m_szScreen.cx, m_szScreen.cy, &memDcLast, 0, 0, SRCCOPY);
			memDcLast.DeleteDC();

			m_bMouseDown = false;
			ReleaseCapture(); 
			ClipCursor( NULL ); 
			CPen NewPen(PS_SOLID, m_nDrawSize, m_dwDrawColor);  
			pDC->SelectObject(&NewPen);  
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->SetROP2( R2_NOT ); 
			//绘制旧的图形，相当于擦除
			if(m_bIsDrawRectangle)
				pDC->Rectangle(m_ptStart.x,m_ptStart.y,m_ptOld.x,m_ptOld.y);
			if(m_bIsDrawEllipse)
				pDC->Ellipse(m_ptStart.x,m_ptStart.y,m_ptOld.x,m_ptOld.y);
			if(m_bIsDrawArrow)
			{
				pDC->MoveTo(m_ptStart.x,m_ptStart.y);
				pDC->LineTo(m_ptOld.x,m_ptOld.y);


				double slopy = 0, cosy = 0, siny = 0;
				double par = 15.0;  
				slopy = atan2( double( m_ptStart.y - m_ptOld.y ),double( m_ptStart.x - m_ptOld.x ) );
				cosy = cos( slopy );
				siny = sin( slopy ); 

				// 画起点到终点的直线
				pDC->MoveTo( m_ptStart );
				pDC->LineTo( m_ptOld );

				// 计算出三角箭头的两端点的坐标
				CPoint pt1( m_ptOld.x + int( par * cosy - ( par / 2.0 * siny ) ), 
					m_ptOld.y + int( par * siny + ( par / 2.0 * cosy ) ) );
				CPoint pt2( m_ptOld.x + int( par * cosy + par / 2.0 * siny ),
					m_ptOld.y - int( par / 2.0 * cosy - par * siny ) );

				CPoint ptArray[3] = { m_ptOld, m_pt1Old, m_pt2Old };
				pDC->Polygon( ptArray, 3 ); // 画箭头处的三角形
				HRGN hRgn = CreatePolygonRgn( ptArray, 3, ALTERNATE ); // 创建箭头处的三角区域
				if ( hRgn != NULL )
				{
					CBrush brush( /*RGB(255,0,0)*/m_dwDrawColor ); // 填充红色
					CBrush *pOldBrush = pDC->SelectObject( &brush );
					pDC->FillRgn( CRgn::FromHandle( hRgn ), &brush );
					pDC->SelectObject( pOldBrush );

					DeleteObject( hRgn );
					brush.DeleteObject();
					pOldBrush->DeleteObject();
				}
			}
			//绘制新的图形
			pDC->SetROP2( R2_COPYPEN );  
			if(m_bIsDrawRectangle)
				pDC->Rectangle(m_ptStart.x,m_ptStart.y,point.x,point.y);   
			if(m_bIsDrawEllipse)
				pDC->Ellipse(m_ptStart.x,m_ptStart.y,point.x,point.y);
			if(m_bIsDrawArrow)
			{
				pDC->MoveTo(m_ptStart.x,m_ptStart.y);
				pDC->LineTo(point.x,point.y);

				double slopy = 0, cosy = 0, siny = 0;
				double par = 15.0;  //length of Arrow (>)
				slopy = atan2( double( m_ptStart.y - point.y ),double( m_ptStart.x - point.x ) );
				cosy = cos( slopy );
				siny = sin( slopy ); 

				// 画起点到终点的直线
				pDC->MoveTo( m_ptStart );
				pDC->LineTo( point );

				// 计算出三角箭头的两端点的坐标
				CPoint pt1( point.x + int( par * cosy - ( par / 2.0 * siny ) ), 
					m_ptOld.y + int( par * siny + ( par / 2.0 * cosy ) ) );
				CPoint pt2( point.x + int( par * cosy + par / 2.0 * siny ),
					m_ptOld.y - int( par / 2.0 * cosy - par * siny ) );

				CPoint ptArray[3] = { m_ptOld, pt1, pt2 };
				pDC->Polygon( ptArray, 3 ); // 画箭头处的三角形
				HRGN hRgn = CreatePolygonRgn( ptArray, 3, ALTERNATE ); // 创建箭头处的三角区域
				if ( hRgn != NULL )
				{
					CBrush brush( /*RGB(255,0,0)*/m_dwDrawColor ); // 填充红色
					CBrush *pOldBrush = pDC->SelectObject( &brush );
					pDC->FillRgn( CRgn::FromHandle( hRgn ), &brush );
					pDC->SelectObject( pOldBrush );

					DeleteObject( hRgn );
					brush.DeleteObject();
					pOldBrush->DeleteObject();
				}
			}
			//保存没有绘制边框的图片
			CDC MemDC;
			MemDC.CreateCompatibleDC(pDC);

			CBitmap *oldbitmap=MemDC.SelectObject(&m_bitmapLast);
			MemDC.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,pDC,0,0,SRCCOPY);
			MemDC.SelectObject(oldbitmap);
			MemDC.DeleteDC();
			oldbitmap->DeleteObject();	

			//保存图片集合，用于撤销编辑
			CBitmap *newBitmap = CBitmap::FromHandle(CopyBitmap((HBITMAP)m_bitmapLast.m_hObject));
			m_arrayBitmap.Add(newBitmap);
			m_arrayBitmapAll.Add(newBitmap);
			m_rectTracker.Draw(pDC/*, &pen*/);
			NewPen.DeleteObject();
		} 
	}
	m_nDownX = -1;
	m_nDownY = -1;
	m_nMoveX = 0;
	m_nMoveY = 0;

	CDialog::OnLButtonUp(nFlags, point);
}


void CScreenCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DonePic();
	CDialog::OnLButtonDblClk(nFlags, point);
}


BOOL CScreenCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	if(m_rect.PtInRect(ptCursor))
	{
		m_rectTracker.SetCursor(this, nHitTest);
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CScreenCtrl::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: 在此处添加消息处理程序代码
}

void CScreenCtrl::DonePic()
{
	//获取截屏区域的图片，复制到剪切板
	CBitmap bmpCut ;
	CDC *pDC = GetDC();

	CDC dc1, dc2;
	dc1.CreateCompatibleDC(pDC);
	dc2.CreateCompatibleDC(pDC);

	CRect rc;
	m_rectTracker.GetTrueRect(&rc);

	bmpCut.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	CBitmap *pOld1 = dc1.SelectObject(&bmpCut);
	CBitmap *pOld2 = dc2.SelectObject(&m_bitmapLast);

	if(pOld1 == NULL || pOld2 == NULL)
		return;

	dc1.BitBlt(0,0,rc.Width(),rc.Height(),&dc2,rc.left,rc.top,SRCCOPY);

	CImage image;
	image.Attach((HBITMAP)bmpCut.m_hObject);

	CString str;
	str.Format("%stemp%d.jpg",m_strImageFileCachePath,time(0));
	//str.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));
	m_strImageFile += str;
	image.Save(m_strImageFile);
	image.Detach();

	

	dc1.SelectObject(pOld1);
	//图片复制到剪切板
	if(OpenClipboard())
	{   
		EmptyClipboard();   
		SetClipboardData(CF_BITMAP,bmpCut.m_hObject);   
		CloseClipboard();   
	}   

	ReleaseDC(pDC);
	ReleaseDC(&dc1);
	ReleaseDC(&dc2);
	bmpCut.DeleteObject();
	pOld1->DeleteObject();
	pOld2->DeleteObject();

	CDialog::OnOK();
}

void CScreenCtrl::SavePic(CString strFilePath)
{
	//获取截屏区域的图片，保存至指定的文件夹
	CBitmap bmpCut;
	CDC *pDC = GetDC();

	CDC dc1, dc2;
	dc1.CreateCompatibleDC(pDC);
	dc2.CreateCompatibleDC(pDC);

	CRect rc;
	m_rectTracker.GetTrueRect(&rc);

	bmpCut.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	CBitmap *pOld1 = dc1.SelectObject(&bmpCut);
	CBitmap *pOld2 = dc2.SelectObject(&m_bitmapLast);

	if(pOld1 == NULL || pOld2 == NULL)
		return;

	dc1.BitBlt(0,0,rc.Width(),rc.Height(),&dc2,rc.left,rc.top,SRCCOPY);

	CImage image;
	image.Attach((HBITMAP)bmpCut.m_hObject);

	CString str;
	str = strFilePath + ".jpg";
	image.Save(str);
	image.Detach();

	dc1.SelectObject(pOld1);

	ReleaseDC(pDC);
	ReleaseDC(&dc1);
	ReleaseDC(&dc2);
	bmpCut.DeleteObject();
	pOld1->DeleteObject();
	pOld2->DeleteObject();

	CDialog::OnCancel();
}

LRESULT CScreenCtrl::OnEditPicSave(WPARAM,LPARAM)
{
	//透明编辑框发送消息保存图片，否则虚线边框会同时保存到图片
	CDC *pDC=GetDC();

	//保存图片
	CBitmap bitmapLast;
	m_bitmapLast.DeleteObject();

	m_bitmapLast.CreateCompatibleBitmap(pDC,m_szScreen.cx,m_szScreen.cy);
	CDC MemDC;
	MemDC.DeleteDC();
	MemDC.CreateCompatibleDC(pDC);

	CBitmap *oldbitmap=MemDC.SelectObject(&m_bitmapLast);
	MemDC.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,pDC,0,0,SRCCOPY);
	MemDC.SelectObject(oldbitmap);
	MemDC.DeleteDC();

	//保存图片集合，用于撤销编辑
	CBitmap *newBitmap = CBitmap::FromHandle(CopyBitmap((HBITMAP)m_bitmapLast.m_hObject));
	m_arrayBitmap.Add(newBitmap);
	m_arrayBitmapAll.Add(newBitmap);
	CRect rect;
	m_rectTracker.GetTrueRect(&m_rect);
	pDC->SelectStockObject(NULL_BRUSH);   
	m_rectTracker.Draw(pDC);
	//如果不保存图片则删除图片集合中最后一个图片
	if(!m_edit.m_bIsSavePicture)
	{
		m_arrayBitmap.RemoveAt(m_arrayBitmap.GetSize() - 1);
	}
	m_edit.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CScreenCtrl::DrawRectTracker(WPARAM,LPARAM)
{
	//绘制边框
	CDC *pDC=GetDC();
	CRect rect;
	m_rectTracker.GetTrueRect(&m_rect);
	pDC->SelectStockObject(NULL_BRUSH);   
	m_rectTracker.Draw(pDC/*, &pen*/);
	return 0;
}

BOOL CScreenCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://ESC键关闭所有工具
			if(m_pDlgSCreenCtrlTool && m_pDlgSCreenCtrlTool->IsWindowVisible())
				m_pDlgSCreenCtrlTool->SendMessage(WM_CLOSE);
			if(IsWindowVisible())
				SendMessage(WM_CLOSE);
			break;
		case VK_RETURN://回车键，如果是捕获窗口则选择窗口，如果是编辑区域则完成截屏
			if(!m_bIsDrawRectangle && !m_bIsDrawEllipse && !m_bIsDrawArrow && !m_bIsDrawEdit)
			{
				if(!m_bIsBtnDown)
				{
					CPoint ptCursor;
					GetCursorPos(&ptCursor);
					SendMessage(WM_LBUTTONDOWN, NULL, MAKELPARAM(ptCursor.x, ptCursor.y));
					SendMessage(WM_LBUTTONUP, NULL, MAKELPARAM(ptCursor.x, ptCursor.y));
				}
			}
			else
			{
				DonePic();
				if(m_pDlgSCreenCtrlTool && m_pDlgSCreenCtrlTool->IsWindowVisible())
					m_pDlgSCreenCtrlTool->SendMessage(WM_CLOSE);
				if(IsWindowVisible())
					SendMessage(WM_CLOSE);
			}
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CScreenCtrl::Revoke()
{
	//如果正在文字编辑则取消文字编辑
	if(m_bEditFlag)
	{
		m_edit.SetWindowText("");
		m_edit.m_bIsDrawRectangle = FALSE;
		m_edit.m_bIsSavePicture = FALSE;
		m_edit.Invalidate();
		m_bEditFlag = FALSE;
		return;
	}
	//撤销编辑，绘制图片集合中倒数第二个图片，最后一个图片删除
	int nSize = m_arrayBitmap.GetSize();
	if(nSize <= 1)
	{
		return;
	}
	CBitmap* pBitmap = m_arrayBitmap.GetAt(nSize - 2);

	CDC* pDC = GetDC();
	CDC memDcLast;
	memDcLast.CreateCompatibleDC(pDC);
	memDcLast.SelectObject(pBitmap);
	pDC->BitBlt(0, 0, m_szScreen.cx, m_szScreen.cy, &memDcLast, 0, 0, SRCCOPY);
	memDcLast.DeleteDC();
	m_bitmapLast.m_hObject = CopyBitmap((HBITMAP)pBitmap->m_hObject);
	m_arrayBitmap.RemoveAt(nSize - 1);
	DrawRectTracker(0, 0);//绘制边框
}

void CScreenCtrl::SetDrawColor(DWORD dwColor)
{
	//设置绘制颜色
	m_dwDrawColor = dwColor;
}

void CScreenCtrl::SetDrawSize(int nSize)
{
	//设置绘制宽度
	m_nDrawSize = nSize * 2;
}

void CScreenCtrl::SetFontSize(int nSize)
{
	//设置绘制字体大小
	m_nFontSize = nSize;
}