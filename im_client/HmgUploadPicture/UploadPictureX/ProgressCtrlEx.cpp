// ProgressCtrlEx.cpp : implementation file
//

#include "..\stdafx.h"
#include "..\HmgUploadPicture.h"
#include "ProgressCtrlEx.h"


// CProgressCtrlEx

IMPLEMENT_DYNAMIC(CProgressCtrlEx, CProgressCtrl)

CProgressCtrlEx::CProgressCtrlEx()
{
	m_nLower = 0;
	m_nUpper = MAX_SPACE_SIZE;	// MB

	m_nCurrentPosition = 0;
	m_nCurrentPosition2 = 0;
		
	m_nCurrentPositionWidth1 = m_nCurrentPositionWidth2 = 0;

	nPos = -1;
	m_bFirstEntry = TRUE;
}

CProgressCtrlEx::~CProgressCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CProgressCtrlEx, CProgressCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CProgressCtrlEx message handlers

int CProgressCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	MessageBox("CProgressCtrlEx::OnCreat");

	if (CProgressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CProgressCtrlEx::Init()
{
	m_wndToolTip.Create(this, TTS_ALWAYSTIP);
	m_wndToolTip.Activate(TRUE);

	EnableToolTips();

	m_bkBitmap.LoadBitmap(IDB_PROGRESSBAR_GREEN);
	m_foreBitmap.LoadBitmap(IDB_PROGRESSBAR_RED);

	m_Bitmap_Green.LoadBitmap(IDB_PROGRESSBAR_V2_GREEN);
	m_Bitmap_Blue.LoadBitmap(IDB_PROGRESSBAR_V2_BLUE);
	m_Bitmap_Gray.LoadBitmap(IDB_PROGRESSBAR_V2_GRAY);

	m_Bitmap_LeftLine.LoadBitmap(IDB_PROGRESSBAR_V2_LEFTLINE);
	m_Bitmap_RightLine.LoadBitmap(IDB_PROGRESSBAR_V2_RIGHTLINE);

	SetRange(0, 100);

	m_rcCurrentPosition1.SetRectEmpty();	m_rcCurrentPosition2.SetRectEmpty();	m_rcCurrentPosition3.SetRectEmpty();
}
void CProgressCtrlEx::SetToolTipText(const CString & strToolTipText, const int offset)
{
	if(strToolTipText.GetLength() < 1)	return ;

	CRect rc;
	GetClientRect(&rc);

	if(m_wndToolTip.GetToolCount() == 0)
	{
		m_wndToolTip.AddTool(this, strToolTipText, &rc, 1);
	}

	m_wndToolTip.UpdateTipText(strToolTipText, this, 1);
	m_wndToolTip.Activate(TRUE);
}


BOOL CProgressCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	if(m_wndToolTip.GetSafeHwnd())	m_wndToolTip.RelayEvent(pMsg);

	return CProgressCtrl::PreTranslateMessage(pMsg);
}

BOOL CProgressCtrlEx::OnEraseBkgnd(CDC* pDC)
{

	CRect rc;
	GetClientRect(&rc);

	CRect rcTmp(rc);
	CPen pen(PS_SOLID, 1, RGB(155,155,155));
	CPen *pOldPen = pDC ->SelectObject(&pen);
	pDC ->Rectangle(rcTmp);
	
//	CBrush br;
//	br.CreatePatternBrush(&m_Bitmap_Gray);
	
//	rc.DeflateRect(CSize(3, 3));
//	pDC ->FillRect(&rc, &br);

	return FALSE;
}

void CProgressCtrlEx::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CProgressCtrl::OnNcPaint() for painting messages
}

void CProgressCtrlEx::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CBrush br;
	int nWidth = 0;
	
//	rc.left += 3;
	rc.DeflateRect(CSize(3, 3));
	
	m_nCurrentPositionWidth1 = m_nCurrentPositionWidth2 = 0;

	CRect rcTmpPos;
	if(m_nCurrentPosition > 0.0)
	{
		if(br.GetSafeHandle())	br.DeleteObject();
		br.CreatePatternBrush(&m_Bitmap_Blue);
	
		nWidth = GetPosWidth(m_nCurrentPosition, rc);
		m_nCurrentPositionWidth1 = nWidth;

		rcTmpPos = rc;
		rcTmpPos.right = rcTmpPos.left + nWidth;
		m_rcCurrentPosition1 = rcTmpPos;
		dc.FillRect(&rcTmpPos, &br);
	}

	if(m_nCurrentPosition2 > 0.0)
	{
		if(br.GetSafeHandle())	br.DeleteObject();
		br.CreatePatternBrush(&m_Bitmap_Green);

		nWidth = GetPosWidth(m_nCurrentPosition2, rc);
		m_nCurrentPositionWidth2 = nWidth;

		m_nCurrentPositionWidth2 -= m_nCurrentPositionWidth1;

		rcTmpPos = rc;
		rcTmpPos.left = (m_nCurrentPositionWidth1 == 0 ? rc.left : m_nCurrentPositionWidth1 + rc.left);
		rcTmpPos.right = rcTmpPos.left + m_nCurrentPositionWidth2;

		m_rcCurrentPosition2 = rcTmpPos;
		dc.FillRect(&rcTmpPos, &br);
	}

	if(br.GetSafeHandle())	br.DeleteObject();
	br.CreatePatternBrush(&m_Bitmap_Gray);

//	nWidth = GetPosWidth(0, rc);

	rcTmpPos = rc;
	rcTmpPos.left = (m_nCurrentPositionWidth2 > 0 ?  m_rcCurrentPosition2.right : m_rcCurrentPosition1.right);
	if(rcTmpPos.left == 0)	rcTmpPos.left = rc.left;

	m_rcCurrentPosition3 = rcTmpPos;
	dc.FillRect(&rcTmpPos, &br);
}

int CProgressCtrlEx::GetPosWidth(const float nPos, const CRect & rc)
{
	float fp = 100.0f;

	fp *= (float)(nPos - m_nLower); 
	fp /= (float)(m_nUpper-m_nLower); 

	float maxWidth = 0.0;
	maxWidth = ((float)nPos/(float)m_nUpper * (float)rc.right);

//	float	fltmaxWidth =	(maxWidth * 10 + 0.5) / 10;

	int n = Round(maxWidth, 0);

	return n - 1;
//	return (int)fltmaxWidth; 
}

void CProgressCtrlEx::SetCurPos(const float nPos)
{
	m_nCurrentPosition = nPos;
	m_nCurrentPositionWidth1 = m_nCurrentPositionWidth2 = 0;
	m_rcCurrentPosition1.SetRectEmpty();	m_rcCurrentPosition2.SetRectEmpty();	m_rcCurrentPosition3.SetRectEmpty();

	float fp = 100.0f; 

	fp *= (float)(m_nCurrentPosition - m_nLower); 
	fp /= (float)(m_nUpper-m_nLower); 

	CString strText;
	strText.Format("当前已占用:%.2f%%,\r\n剩余空间为:%dMB\0.", fp, _get_remain_space_size());
	
	SetToolTipText(strText);

	Invalidate();
}

void CProgressCtrlEx::SetCurPos2(const float nPos2)
{
	m_nCurrentPosition2 = nPos2;
	m_nCurrentPositionWidth1 =	m_nCurrentPositionWidth2 = 0;
	m_rcCurrentPosition1.SetRectEmpty();	m_rcCurrentPosition2.SetRectEmpty();	m_rcCurrentPosition3.SetRectEmpty();

	Invalidate();
}	
void CProgressCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	if(m_bFirstEntry)
	{
		m_bFirstEntry = FALSE;	
		SetTimer(1, 10, NULL);
	}

//	ScreenToClient(&rc);
	CPoint pt(point); 	// ScreenToClient(&pt);

	TRACE("CProgressCtrlEx::OnMouseMove nPos = %d.\n", nPos);

	CString strText;
	if(!m_rcCurrentPosition1.IsRectEmpty() && m_rcCurrentPosition1.PtInRect(pt)) // && nPos != 2)
	{
		nPos = 2;
		strText.Format("当前已占用服务器空间为%.2fMB.", m_nCurrentPosition);
		SetToolTipText(strText, 5);

		CProgressCtrl::OnMouseMove(nFlags, point);	return ;
	}
	
	if(!m_rcCurrentPosition2.IsRectEmpty() && m_rcCurrentPosition2.PtInRect(pt)) // && nPos != 1)
	{
		nPos = 1;
		strText.Format("将要占用%.2fMB.", m_nCurrentPosition2);
		SetToolTipText(strText, m_nCurrentPositionWidth1 + 2);

		CProgressCtrl::OnMouseMove(nFlags, point);	return ;
	}
	
	if(m_rcCurrentPosition3.PtInRect(pt) && nPos != 0)
	{
		int offset = 0;

		offset = (m_nCurrentPositionWidth2 == 0 ? m_nCurrentPositionWidth1 + 2 : m_nCurrentPositionWidth2 + 2);

		nPos = 0;
		strText.Format("当前剩余空间为%dMB.", _get_remain_space_size());
		SetToolTipText(strText, offset);
		CProgressCtrl::OnMouseMove(nFlags, point);	return ;
	}
	
	nPos = -1;

	CProgressCtrl::OnMouseMove(nFlags, point);
}

void CProgressCtrlEx::OnTimer(UINT_PTR nIDEvent)
{
	CPoint pt;
	GetCursorPos(&pt);

	CRect rc;
	GetWindowRect(&rc);

	if(!rc.PtInRect(pt))
	{
		nPos = -1;
		m_bFirstEntry = TRUE;

		KillTimer(1);
	}

	CProgressCtrl::OnTimer(nIDEvent);
}
