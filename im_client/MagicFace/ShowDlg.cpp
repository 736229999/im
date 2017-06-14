// ShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicFace.h"
#include "ShowDlg.h"


// CShowDlg 对话框

IMPLEMENT_DYNAMIC(CShowDlg, CDialog)

CShowDlg::CShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowDlg::IDD, pParent)
{
	m_bitmap.m_hObject = NULL;
	Create(CShowDlg::IDD,pParent);
}

CShowDlg::~CShowDlg()
{
}

void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CShowDlg 消息处理程序

BOOL CShowDlg::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

BOOL CShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	ModifyStyleEx(0,WS_EX_TOOLWINDOW|WS_EX_LAYERED|WS_EX_TRANSPARENT);
	//ModifyStyle(0,WS_CHILD|WS_VISIBLE);
	SetLayeredWindowAttributes(0xAAAAAA, 255, LWA_COLORKEY);	

	return TRUE;  
}

void CShowDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CShowDlg::OnPaint()
{
	CPaintDC dc(this);

	if ( m_bitmap.m_hObject )
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap *pOld = dcMem.SelectObject(&m_bitmap);

		CRect rc;
		GetClientRect(&rc);

		dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);

		dcMem.SelectObject(pOld);		
	}
}

void CShowDlg::ShowFrame(CWnd *pWnd )
{
	if ( m_bitmap.m_hObject )
		m_bitmap.DeleteObject();

	CDC *pDC = pWnd->GetDC();

	CRect rc ;
	pWnd->GetClientRect(&rc);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	m_bitmap.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());

	CBitmap *pOld = dcMem.SelectObject(&m_bitmap);

	dcMem.BitBlt(0,0,rc.Width(),rc.Height(),pDC,0,0,SRCCOPY);

	dcMem.SelectObject(pOld);

	pWnd->ReleaseDC(pDC);

	Invalidate();		
}
