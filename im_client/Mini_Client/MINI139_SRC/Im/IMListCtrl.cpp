#include "StdAfx.h"
#include ".\imlistctrl.h"
#include "resgdiobj.h"

// CIMListCtrl

IMPLEMENT_DYNAMIC(CIMListCtrl, CWnd)
CIMListCtrl::CIMListCtrl()
{
	m_ToolTip.m_hWnd  = NULL;
	m_strPreTip       = "";
}

CIMListCtrl::~CIMListCtrl()
{
}


BEGIN_MESSAGE_MAP(CIMListCtrl, CWnd)
END_MESSAGE_MAP()

void CIMListCtrl::DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp,bool bBlackWhite)
{
	CResBitmap bmp;
	bmp.LoadBitmap(nIDBmp);

	BITMAP bi;
	bmp.GetBitmap(&bi);

	CDC dc;

	CDC *pSafeDC = GetDC();
	dc.CreateCompatibleDC(pSafeDC);
	ReleaseDC(pSafeDC);

	CBitmap *pOldbmp = dc.SelectObject(&bmp);

	TransparentBlt(pDC->m_hDC,x,y,bi.bmWidth,bi.bmHeight,dc.m_hDC,0,0,bi.bmWidth,bi.bmHeight,RGB(0,255,0));

	dc.SelectObject(pOldbmp);
}

void CIMListCtrl::DrawBitmap(CDC *pDC,int nDestX, int nDestY, CBitmap *pBitmap, int nSrcX, int nSrcY, int nWidth, int nHeight, COLORREF clrTrans)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOld=dcMem.SelectObject(pBitmap);

	TransparentBlt(pDC->m_hDC,nDestX,nDestY,nWidth,nHeight,
		dcMem.m_hDC,nSrcX,nSrcY,
		nWidth,
		nHeight,
		clrTrans);

	dcMem.SelectObject(pOld);
	
	dcMem.DeleteDC();
}


void CIMListCtrl::DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, bool bBlackWhite)
{
	BITMAP bi;
	pBitmap->GetBitmap(&bi);


	CDC *pSafeDC = GetDC();

	CDC dcSrc ;
	dcSrc.CreateCompatibleDC(pSafeDC);
	CBitmap *pBitmapSave = dcSrc.SelectObject(pBitmap);


	CDC dcDraw;
	dcDraw.CreateCompatibleDC(pSafeDC);

	CBitmap bmpDraw;
	bmpDraw.CreateCompatibleBitmap(pSafeDC,bi.bmWidth,bi.bmHeight);

	CBitmap *pOldbmp = dcDraw.SelectObject(&bmpDraw);

	dcDraw.BitBlt(0,0,bi.bmWidth,bi.bmHeight,&dcSrc,0,0,SRCCOPY);

	if ( bBlackWhite==true )
	{
		BYTE     R,G,B;
		COLORREF clr  ;
		int      temp ;

		for ( int i=0;i<bi.bmWidth; i++ )
		{
			for ( int j=0;j<bi.bmHeight; j++ )
			{
				clr = dcDraw.GetPixel(i,j);

				R   = clr & 0xFF;
				G   = (clr & 0xFF00)>>8;
				B   = (clr & 0xFF0000)>>16;

				temp  =  ((int)R + (int)G + (int)B)/3;

				dcDraw.SetPixel(i,j,RGB(temp,temp,temp));
			}
		}
	}

	pDC->BitBlt(x,y,bi.bmWidth,bi.bmHeight,&dcDraw,0,0,SRCCOPY);

	dcSrc.SelectObject(pBitmapSave);
	dcDraw.SelectObject(pOldbmp);

	ReleaseDC(pSafeDC);
}

void CIMListCtrl::ShowToolTip(CString strTip)
{
	if ( strTip == m_strPreTip )
	{
		return ;
	}

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_ToolTip.AddTool(this, strTip, rc, 1);
	} // if


	m_ToolTip.UpdateTipText(strTip ,this,1);

	m_strPreTip = strTip ;

	m_ToolTip.Activate(TRUE);
}

void CIMListCtrl::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this,TTS_ALWAYSTIP);
		m_ToolTip.Activate(TRUE);
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 350);
	} 
} 

//nStyle
void CIMListCtrl::DrawPtInRect(const CPoint &ptCursor,const CRect & rcRect,CDC *pDC, int nStyle, bool bMouseDown)
{
	if (rcRect.PtInRect(ptCursor) )
	{
		if ( nStyle==0 )
		{
			if ( bMouseDown )
				pDC->Draw3dRect(&rcRect,RGB(122,122,122),RGB(233,233,233));
			else
				pDC->Draw3dRect(&rcRect,RGB(233,233,233),RGB(122,122,122));
		}
		else
		if ( nStyle==1 )
		{
			CRect rcTemp = rcRect;

			if ( bMouseDown )
			{
				rcTemp.InflateRect(-1,-1);
			}

			CPoint pt(3,3);

			CPen   p(PS_SOLID,1,RGB(0,188,199));
			CPen   *pp = pDC->SelectObject(&p);

			pDC->RoundRect(rcTemp,pt);

			pDC->FillSolidRect(rcTemp.left+1,rcTemp.top+1,
				rcTemp.Width()-2,rcTemp.Height()-2,
					RGB(200,222,255));

			pDC->SelectObject(pp);
		}
	}
}


BOOL CIMListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if ( m_ToolTip.m_hWnd )
		m_ToolTip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

void CIMListCtrl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
}
