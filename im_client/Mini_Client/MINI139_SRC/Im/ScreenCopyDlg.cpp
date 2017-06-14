// im\ScreenCopyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\screencopydlg.h"
#include "localdata.h"

#include <atlimage.h>

// CScreenCopyDlg 对话框
IMPLEMENT_DYNAMIC(CScreenCopyHintDlg, CDialog)

void DrawRectangle(CDC *pDC,int x1, int y1, int x2, int y2,COLORREF clr=0, int nDrawMode=R2_NOTXORPEN)
{
	CPen pen(PS_DOT,1,clr);
	CPen *pOld=pDC->SelectObject(&pen);
	int nRp2 = pDC->SetROP2(nDrawMode);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x1,y2);
	pDC->LineTo(x2,y2);
	pDC->LineTo(x2,y1);
	pDC->LineTo(x1,y1);
	pDC->SelectObject(pOld);
	pDC->SetROP2(nRp2);
}

CScreenCopyHintDlg::CScreenCopyHintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCopyDlg::IDD, pParent)
{
}

CScreenCopyHintDlg::~CScreenCopyHintDlg()
{
}

void CScreenCopyHintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenCopyHintDlg, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CScreenCopyHintDlg::SetPointColor(const CPoint &pt, COLORREF color)
{
}


BOOL CScreenCopyHintDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

IMPLEMENT_DYNAMIC(CScreenCopyDlg, CDialog)
CScreenCopyDlg::CScreenCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCopyDlg::IDD, pParent)
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

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

	m_szScreen.cx = cx; 
	m_szScreen.cy = cy;

	m_ptOrigin.x = m_ptOrigin.y = m_ptNow.x = m_ptNow.y = 0;

	m_bitmapTip.LoadBitmap(IDB_SCREEN_CUT_TIP);
	m_bitmapTip.GetBitmap(&m_biTip);
	m_ptTip.x = m_ptTip.y = 5;
	m_iTipHeight = 0;

	m_bMouseDown = FALSE;
}

CScreenCopyDlg::~CScreenCopyDlg()
{
}

void CScreenCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreenCopyDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CScreenCopyDlg 消息处理程序

BOOL CScreenCopyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ShowWindow(SW_MAXIMIZE);
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	SetTimer(0x01,1,NULL);

	return TRUE;  
}

BOOL CScreenCopyDlg::OnEraseBkgnd(CDC* pDC)
{
	CDC dcBack;
	dcBack.CreateCompatibleDC(pDC);

	CBitmap *pOldback = dcBack.SelectObject(&m_bitmapBack);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	CBitmap *pOldTemp=dcTemp.SelectObject(&m_bitmapScreen);

	dcBack.BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcTemp,0,0,SRCCOPY);

	DrawRectangle(&dcBack,m_ptOrigin.x,m_ptOrigin.y,m_ptNow.x,m_ptNow.y);

    dcTemp.SelectObject(&m_bitmapTip);

	if ( this->m_iTipHeight<m_biTip.bmHeight )
	{
		dcBack.BitBlt(m_ptTip.x,m_ptTip.y,m_biTip.bmWidth,m_iTipHeight,&dcTemp,0,m_biTip.bmHeight-m_iTipHeight,SRCCOPY);
	}
	else
	{
		dcBack.BitBlt(m_ptTip.x,m_ptTip.y,m_biTip.bmWidth,m_biTip.bmHeight,&dcTemp,0,0,SRCCOPY);
	}

	pDC->BitBlt(0,0,m_szScreen.cx,m_szScreen.cy,&dcBack,0,0,SRCCOPY);

	dcTemp.SelectObject(pOldTemp);
	dcBack.SelectObject(pOldback);

	return FALSE;
}

void CScreenCopyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	KillTimer(0x02);
	m_bMouseDown=TRUE;
	m_ptOrigin = m_ptNow = point ;

	Invalidate();

	CDialog::OnLButtonDown(nFlags, point);
}

void CScreenCopyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetTimer(0x02,33,NULL);
	m_bMouseDown=FALSE;
}

void CScreenCopyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

	CRect rc;
	rc.SetRect(m_ptTip.x,
		m_ptTip.y,
		m_biTip.bmWidth+m_ptTip.x,
		m_biTip.bmHeight+m_ptTip.y);

	bool bInvalide=false;

	if ( rc.PtInRect(point) )
	{
		bInvalide = true;
		
		if ( m_ptTip.x==5 )
		{
			CRect rcClient;
			GetClientRect(&rc);

			m_ptTip.x = rc.right-5-m_biTip.bmWidth;
		}
		else
		{
			m_ptTip.x=5;
		}
	}

	if ( m_bMouseDown )
	{
		m_ptNow=point;
		bInvalide = true;
	}

	if ( bInvalide )
		Invalidate();	
}

void CScreenCopyDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CScreenCopyDlg::OnOK()
{
	CRect rcCut;
	int x1 = m_ptOrigin.x, x2 = m_ptNow.x;
	int y1 = m_ptOrigin.y, y2 = m_ptNow.y;

	if ( y2<y1 )
	{
		y2+=y1;
		y1=y2-y1;
		y2=y2-y1;
	}

	if ( x2<x1 )
	{
		x2+=x1;
		x1=x2-x1;
		x2=x2-x1;
	}

	rcCut.SetRect(x1,y1,x2,y2);

	if ( rcCut.IsRectEmpty()==FALSE )
	{
		CBitmap bmpCut ;
		CDC *pDC = GetDC();

		CDC dc1, dc2;
		dc1.CreateCompatibleDC(pDC);
		dc2.CreateCompatibleDC(pDC);

		bmpCut.CreateCompatibleBitmap(pDC,rcCut.Width(),rcCut.Height());
		CBitmap *pOld1 = dc1.SelectObject(&bmpCut);

		CBitmap *pOld2 = dc2.SelectObject(&m_bitmapScreen);

		dc1.BitBlt(0,0,rcCut.Width(),rcCut.Height(),&dc2,rcCut.left,rcCut.top,SRCCOPY);
			
	
		CImage image;
		image.Attach((HBITMAP)bmpCut.m_hObject);

		CString str;
		str.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));
		image.Save(str);
		image.Detach();
	
		dc1.SelectObject(pOld1);
		dc2.SelectObject(pOld2);

		ReleaseDC(pDC);

		m_strImageFile = str;

		CDialog::OnOK();
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CScreenCopyDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent )
	{
	case 0x01:
		{
			CRect rc;
			rc.SetRect(m_ptTip.x,
				m_ptTip.y,
				m_biTip.bmWidth+m_ptTip.x,
				m_biTip.bmHeight+m_ptTip.y);

			if ( m_iTipHeight<m_biTip.bmHeight )
			{
				m_iTipHeight+=3;
			}
			else
			{
				KillTimer(0x01);
			}

			InvalidateRect(&rc);
		}
		break;
	case 0x02:
		{
			static bool bFlag=false;

			CDC *pDC = GetDC();

			if ( bFlag )
				DrawRectangle(pDC,m_ptOrigin.x,m_ptOrigin.y,m_ptNow.x,m_ptNow.y, 0xFFFFFF );
			else
				DrawRectangle(pDC,m_ptOrigin.x,m_ptOrigin.y,m_ptNow.x,m_ptNow.y, 0x00CCFF);
			bFlag = !bFlag;

			ReleaseDC(pDC);
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}