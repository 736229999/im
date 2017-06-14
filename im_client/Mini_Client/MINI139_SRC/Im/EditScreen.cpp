// EditScreen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EditScreen.h"

#define TRANS_BACK -1
// CEditScreen

IMPLEMENT_DYNAMIC(CEditScreen, CEdit)

CEditScreen::CEditScreen()
{
 	m_dwTextColor = RGB(255, 0, 0);
	m_brush.CreateStockObject(NULL_BRUSH);
	m_bIsDrawRectangle = TRUE;
	m_rect.left = 0;
	m_rect.right = 0;
	m_rect.top = 0;
	m_rect.bottom = 0;
	m_bIsPaint = TRUE;
	m_bIsSavePicture = TRUE;
}

CEditScreen::~CEditScreen()
{
	m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditScreen, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CEditScreen ��Ϣ�������

HBRUSH CEditScreen::CtlColor( CDC* pDC, UINT nCtlColor )
{
	//����������ɫ�ͱ���͸��
	pDC->SetTextColor(m_dwTextColor);
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)m_brush;

}

void CEditScreen::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//�������������Ӧ�����ڵĵ����Ϣ
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if(!m_bIsDrawRectangle)
	{
		if(GetParent() != NULL)
			GetParent()->SendMessage(WM_LBUTTONDOWN, HTCAPTION, MAKELPARAM(ptCursor.x, ptCursor.y));
		return;
	}

	if(m_rect.PtInRect(point))//��������߿�����������Ӧ�༭��ĵ����Ϣ
	{
		CEdit::OnLButtonDown(nFlags, point);
	}
	else//��������߿�����������Ӧ�����ڵĵ����Ϣ
	{
		if(GetParent() != NULL)
			GetParent()->SendMessage(WM_LBUTTONDOWN, HTCAPTION, MAKELPARAM(ptCursor.x, ptCursor.y));
	}
}

BOOL CEditScreen::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return FALSE;
}

void CEditScreen::OnPaint() 
{
	if(m_bIsPaint)//���ֱ༭
	{
		CDC* pDC = GetDC();

		//���Ʊ���ͼƬ
		CRect rc;
		GetWindowRect(&rc);

		CDC memDcLast;
		memDcLast.CreateCompatibleDC(pDC);
		memDcLast.SelectObject(m_bitmapLast);
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &memDcLast, rc.left, rc.top, SRCCOPY);
		memDcLast.DeleteDC();

		
		CDC dc;//���ڼ��������С
		dc.CreateCompatibleDC(pDC);
		dc.SelectObject(g_font14);
		SetFont(&g_font14);//��������
		CString str;
		GetWindowText(str);
		CSize sizeText;
		if(str == "")//�������Ϊ�����ȡһ���ַ��Ĵ�С
		{
			sizeText = dc.GetTextExtent("a");
		}
		else
		{
			sizeText = dc.GetTextExtent(str);
		}
		dc.DeleteDC();
		pDC->SelectObject(GetStockObject(NULL_BRUSH)); 
		pDC->SelectObject(CreatePen(PS_DASHDOT,1,RGB(0,0,0))); 
		CSize size; 
		int lenStrLen;  
		lenStrLen=GetWindowTextLength(); 
		GetTextExtentPoint(::GetDC(m_hWnd),str,lenStrLen,&size); 

		int nWidthMax = (sizeText.cx/*size.cx*/ + rc.left + 5) >= m_rcParent.right ? m_rcParent.right - rc.left : sizeText.cx/*size.cx*/ + 5;
		int nHeightMax = sizeText.cy * GetLineCount() + rc.top >= m_rcParent.bottom ?  m_rcParent.bottom - rc.top : sizeText.cy * GetLineCount();
		pDC->Rectangle(0, 0, nWidthMax, nHeightMax); //�������߱߿�
		//���߱߿�̬�仯
		m_rect.left = rc.left;
		m_rect.top = rc.top;
		m_rect.right = rc.left + nWidthMax;
		m_rect.bottom = rc.top + sizeText.cy * GetLineCount();
		m_bIsPaint = FALSE;
		ShowCaret();//��ʾ���
		if(GetParent() != NULL)//���ƽ����߿�
			::PostMessage(GetParent()->GetSafeHwnd(),WM_DRAW_RECTTRACKER, 0, 0);
		CEdit::OnPaint();
	}
	if(!m_bIsDrawRectangle)//��ɱ༭,���������߱߿�
	{
		CRect rc;
		GetWindowRect(&rc);

		CDC* pDC = GetDC();
		CDC memDcLast;
		memDcLast.CreateCompatibleDC(pDC);
		memDcLast.SelectObject(m_bitmapLast);
		pDC->BitBlt(m_rcParent.left - rc.left, m_rcParent.top - rc.top, m_rcParent.Width(), m_rcParent.Height(), &memDcLast, m_rcParent.left, m_rcParent.top, SRCCOPY);
		memDcLast.DeleteDC();

		CEdit::OnPaint();
		HideCaret();//�������
		if(GetParent() != NULL)
			::PostMessage(GetParent()->GetSafeHwnd(),WM_EDIT_PIC_SAVE, 0, 0);//�����ڱ���ͼƬ
	}
} 

////////////////////////////////////////////////////////////////////////// 
//�ݲ�����ճ�Ͱ����͹��ܼ���2������� 
////////////////////////////////////////////////////////////////////////// 
void CEditScreen::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{ 
	if(m_bIsDrawRectangle)//�༭����
	{
		m_bIsPaint = TRUE;
		CEdit::OnChar(nChar, nRepCnt, nFlags); 
		Invalidate();
	}
}

void CEditScreen::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CEditScreen::OnChange() 
{
}

void CEditScreen::SetParentRect(CRect rcParent)
{
	m_rcParent = rcParent;
}