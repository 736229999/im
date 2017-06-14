// im\FaceIconCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "FaceIconCtrl.h"
#include "impublic.h"
#include ".\faceiconctrl.h"
#include "webprocess.h"
#include "SAConfig.h"
// CFaceIconCtrl

IMPLEMENT_DYNAMIC(CFaceIconCtrl, CStatic)
CFaceIconCtrl::CFaceIconCtrl()
{
	m_nSex       = 0;
	m_bMouseOver = false;
	m_bMouseDown = false;
	m_nUserId	 = 0;
	m_bitmap     = NULL;
	m_faceType   = fDefault;
	m_hCursor    = ::LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND));
}

CFaceIconCtrl::~CFaceIconCtrl()
{
	if ( m_bitmap )
		delete m_bitmap;


	m_bitmap= NULL;
}


BEGIN_MESSAGE_MAP(CFaceIconCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CFaceIconCtrl 消息处理程序


void CFaceIconCtrl::OnPaint()
{
	CPaintDC dc(this); 

	int nSavedDC = dc.SaveDC();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = rcClient.Width(), cy = rcClient.Height();

	dc.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	
	CPen pen(PS_SOLID,3,RGB(255,255,255));
	CPen* pOldPen = dc.SelectObject(&pen);
	
	POINT pt;
	pt.x=(rcClient.Width()-52)/2;
	pt.y=(rcClient.Height()-52)/2;
/*
	CRgn rgn1;
	rgn1.CreateRoundRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom,3,3);
	dc.SelectObject(&rgn1);
	*/
	switch ( m_faceType )
	{
		case fDefault://客户端自定头像
			//g_imgStatus52.Draw(&dc,GetDefaultImage(m_nSex,m_nStatus),pt,ILD_NORMAL);

			g_ImgFaceFrame.Draw(dc, pt.x, pt.y, 52,52);
			g_ImgIcon.Draw(dc,pt.x, pt.y, 52, 52);
			break;	
		case fUser://
			CDC dcTemp;
			dcTemp.CreateCompatibleDC(&dc);

			if ( m_bitmap )
			{
				CBitmap * pOld=dcTemp.SelectObject(m_bitmap);			
				dc.BitBlt(0,0,cx,cy,&dcTemp,0,0,SRCCOPY);
				dcTemp.SelectObject(pOld);
			}
			else
			{
				//g_imgStatus52.Draw(&dc,GetDefaultImage(m_nSex,XTREAM_ONLINE),pt,ILD_NORMAL);
				g_ImgFaceFrame.Draw(dc, pt.x, pt.y, 52,52);
				g_ImgIcon.Draw(dc,pt.x, pt.y, 52, 52);
			}
/* 头像不关联状态
			if ( m_nStatus==XTREAM_OFFLINE )
			{
				BYTE     r, g, b , a;
				COLORREF color   ;

				for ( int x=0; x<cx; x++ )
				{
					for ( int y=0; y<cy; y++ )
					{
						color = dc.GetPixel(x,y);

						r=color&0xFF;
						g=(color&0xFF00)>>8;
						b=(color&0xFF0000)>>16;

						a = ((int)r+(int)g+(int)b)/3;

						dc.SetPixel(x,y,RGB(a,a,a) );
					}
				}
			}
			else if ( m_nStatus==XTREAM_LEAVE )
			{
				CResBitmap bmpLeave;
				bmpLeave.LoadBitmap(IDB_STATUS_FOOT40);

				CBitmap *pOld=dcTemp.SelectObject(&bmpLeave);

				dc.BitBlt(cx-19,cy-11,18,9,&dcTemp,0,0,SRCCOPY);
				dcTemp.SelectObject(pOld);
			}
			else if ( m_nStatus==XTREAM_HIDE )
			{
				CResBitmap bmpLeave;
				bmpLeave.LoadBitmap(IDB_STATUS_FOOT40);

				CBitmap *pOld=dcTemp.SelectObject(&bmpLeave);

				dc.BitBlt(cx-19,cy-11,18,9,&dcTemp,0,9,SRCCOPY);
				dcTemp.SelectObject(pOld);
			}
*/		
		break;	
	}
/*
	CRgn rgn2;
	rgn2.CreateRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom);
	dc.SelectObject(rgn2);
	dc.RoundRect(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom,3,3);
*/
	dc.SelectObject(pOldPen);
	dc.RestoreDC(nSavedDC);
}

void CFaceIconCtrl::SetFaceFile(CString strFile, int status)
{
	CRect rc;
	GetClientRect(rc);
	
	CBitmap * pBitmap = ::CBitmapFromFile(strFile,rc.Width(),rc.Height());

	if ( pBitmap )
	{	
		if ( m_bitmap )
			delete m_bitmap;

		m_bitmap=pBitmap ;

		m_faceType = fUser;
		m_nStatus   = status;

		Invalidate();
	}
}

void CFaceIconCtrl::SetSex(int sex, int status)
{
	m_nSex    = sex;
	m_nStatus = status;

	if ( m_bitmap )
	{
		delete m_bitmap;
		m_bitmap = NULL;
	}

	m_faceType= fUser;

	if (m_hWnd)
		Invalidate();
}

void CFaceIconCtrl::SetUserInfo(const FRIEND_INFO *pUser)
{
	CString str ;
	str = g_webUserHeadIconProcess.GetFaceFile(pUser);

	m_nUserId=((FRIEND_INFO*)pUser)->GetId();
	if ( FileExist(str) )
		SetFaceFile(str,pUser->status);
	else
		SetSex(pUser->sex,pUser->status);
}
//
//void CFaceIconCtrl::SetFace(int face, int status)
//{
//	m_nFaceId = face;
//	m_nStatus = status;
//
//	m_nFaceType=0;
//
//	if (m_hWnd)
//		Invalidate();
//}

void CFaceIconCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_NOTIFY, TRUE);
	//SetClassLong(m_hWnd,    // window handle 
	//	GCL_HCURSOR,		// change cursor 
	//	(LONG)m_hCursor);	// new cursor 
	//SetCursor((HCURSOR)m_hCursor);
}

void CFaceIconCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseDown=true;
	CStatic::OnLButtonDown(nFlags, point);
}

void CFaceIconCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMouseDown=false;
//	Invalidate();
	CStatic::OnLButtonUp(nFlags, point);
}

void CFaceIconCtrl::OnTimer(UINT nIDEvent)
{	
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		GetCursorPos(&pt);

		CRect rc;
		GetWindowRect(&rc);

		if ( !rc.PtInRect(pt) )
		{
			m_bMouseOver=false;
			m_bMouseDown=false;
			Invalidate();
			KillTimer(0x01);
		}
	}

	CStatic::OnTimer(nIDEvent);
}

void CFaceIconCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	if ( m_bMouseOver==false )
	{
		m_bMouseOver=true;
		Invalidate();
		SetTimer(0x01,10,NULL);
	}
	*/
	//::SetCursor(m_hCursor);
	ShowToolTip(m_sTip);
    CStatic::OnMouseMove(nFlags, point);

	CWnd *pWnd = GetParent();
	if(pWnd != NULL)
	{
		ClientToScreen(&point);
		pWnd->ScreenToClient(&point);
		::SendMessage(pWnd->GetSafeHwnd(), WM_MOUSEMOVE, nFlags, MAKELONG(point.x, point.y));
	}
}

BOOL CFaceIconCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	//pDC->FillSolidRect(&rcClient,BACKGND_COLOR);

	return FALSE;
}

void CFaceIconCtrl::ShowToolTip(const CString& strTip)
{
	if(m_sLastTip==strTip)
		return;
	m_sLastTip=strTip;
	m_sTip=strTip;
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
BOOL CFaceIconCtrl::PreTranslateMessage(MSG* pMsg)
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


BOOL CFaceIconCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(m_hCursor);
	return FALSE;
	//return CStatic::OnSetCursor(pWnd, nHitTest, message);
}
