// im\FloorPageRecent.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\FloorPageRecent.h"


// CFloorPageRecent 对话框

IMPLEMENT_DYNAMIC(CFloorPageRecent, CDialog)
CFloorPageRecent::CFloorPageRecent(CWnd* pParent /*=NULL*/)
	: CDialog(CFloorPageRecent::IDD, pParent)
{
}

CFloorPageRecent::~CFloorPageRecent()
{
}

void CFloorPageRecent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_RECENT,m_wndRecentList);

	
}


BEGIN_MESSAGE_MAP(CFloorPageRecent, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CFloorPageRecent 消息处理程序
BOOL CFloorPageRecent::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,0xFFFFFF);
	return FALSE;
}

void CFloorPageRecent::OnPaint()
{
	CPaintDC dc(this); 

	CRect rc;
	GetClientRect(&rc);
	rc.bottom=rc.top+23;

	CResBitmap bmp;
	bmp.LoadBitmap(IDB_HEADER_BACK);
	HorizStretchBlt(&dc,0,0,rc.Width(),&bmp);
}

BOOL CFloorPageRecent::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_btnClear.SetBitmap(IDB_BTN_SEARCH,IDB_BTN_SEARCH);
	g_pRecentListCtrl = &m_wndRecentList;
	return TRUE; 
}

void CFloorPageRecent::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_wndRecentList.m_hWnd )
	{
		CRect rcList(0,0, cx, cy);
		rcList.InflateRect(-2,-2);
		m_wndRecentList.MoveWindow(&rcList);
	}
}

void CFloorPageRecent::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
}



