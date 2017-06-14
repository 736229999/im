// im\ClusterIconCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include ".\clustericonctrl.h"
#include "..\utils\function.h"
#include "..\resource.h"
#include "impublic.h"

// CClusterIconCtrl

#define DEFAULT_ICON_WIDTH	52
#define DEFAULT_ICON_HEIGHT	65
#define VIP_ICON_WIDTH		16
#define VIP_ICON_HEIGHT		16		

#define IDC_MYTIP			0x1001

IMPLEMENT_DYNAMIC(CClusterIconCtrl, CStatic)
CClusterIconCtrl::CClusterIconCtrl()
{
	m_hCursor    = ::LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND));
	// 默认群图标
	m_bitmap.LoadBitmap(IDB_BLOG_LOGO);
	m_bmpVIP.LoadBitmap(IDB_ICON_CVIP);
	m_bVIP = FALSE;
	// 提示
	m_sLastTip = "";
}

CClusterIconCtrl::~CClusterIconCtrl()
{
}


BEGIN_MESSAGE_MAP(CClusterIconCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CClusterIconCtrl 消息处理程序


void CClusterIconCtrl::OnPaint()
{
	CPaintDC dc(this);
	int nSavedDC=dc.SaveDC();
	CRect rcClient;
	GetClientRect(&rcClient);
	
	dc.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	CPen pen(PS_SOLID,1,RGB(0,122,255));
	CPen* pOldPen = dc.SelectObject(&pen);

	// 绘制族徽图像
	CPoint pt;	// 绘制起点
	pt.x=(rcClient.Width()-DEFAULT_ICON_WIDTH)/2;
	pt.y=(rcClient.Height()-DEFAULT_ICON_HEIGHT)/2;
	if(pt.x<0)
		pt.x=0;
	if(pt.y<0)
		pt.y=0;
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);
	CBitmap *pOld=dcTemp.SelectObject(&m_bitmap);
	

	dc.TransparentBlt(pt.x,pt.y,DEFAULT_ICON_WIDTH,DEFAULT_ICON_HEIGHT,&dcTemp,0,0,DEFAULT_ICON_WIDTH,DEFAULT_ICON_HEIGHT,RGB(0,255,0));
	

	// 判断是否绘制vip标志
	if(m_bVIP)
	{
		// 开始位置在群图像的右下侧
		pt.x=rcClient.Width()-VIP_ICON_WIDTH-5;
		pt.y=rcClient.Height()-VIP_ICON_HEIGHT-5;
		dcTemp.SelectObject(&m_bmpVIP);
		dc.TransparentBlt(pt.x,pt.y,VIP_ICON_WIDTH,VIP_ICON_HEIGHT,&dcTemp,0,0,VIP_ICON_WIDTH,VIP_ICON_HEIGHT,RGB(255,255,255));

	}
	

	dc.RoundRect(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom,3,3);
	dc.SelectObject(pOldPen);
	dc.RestoreDC(nSavedDC);
	
}

void CClusterIconCtrl::PreSubclassWindow()
{
	SetClassLong(m_hWnd,    // window handle 
		GCL_HCURSOR,		// change cursor 
		(LONG)m_hCursor);	// new cursor 
	SetCursor((HCURSOR)m_hCursor);


	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_NOTIFY, TRUE);
}

BOOL CClusterIconCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	// 背景色为天蓝色
	pDC->FillSolidRect(&rcClient,RGB(213,233,255));
	return FALSE;
}

void CClusterIconCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_sUrl!="")
	{	
		MiniOpenUrl(m_sUrl);
	}

	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CClusterIconCtrl::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE
		)
	{
		if(m_toolTip.m_hWnd!=NULL)
			m_toolTip.RelayEvent(pMsg);
	}

	return CStatic::PreTranslateMessage(pMsg);
}

void CClusterIconCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	ShowToolTip("点击进入群主页");
	
	CStatic::OnMouseMove(nFlags, point);
}


void CClusterIconCtrl::ShowToolTip(const CString& strTip)
{
	if(m_sLastTip==strTip)
		return;
	m_sLastTip=strTip;
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
		m_toolTip.AddTool(this, strTip, rc, IDC_MYTIP);
	}
	m_toolTip.UpdateTipText(strTip ,this,IDC_MYTIP);

}