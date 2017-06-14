// im\UserLevelCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../Resource.h"
#include "UserLevelCtrl.h"
#include ".\userlevelctrl.h"
#include <math.h>

#include "resgdiobj.h"

#define IDC_MYTIP 0x10002

IMPLEMENT_DYNAMIC(CUserLevelCtrl, CStatic)
CUserLevelCtrl::CUserLevelCtrl()
{
	m_nTime = 0;
	// ����ȼ�
	m_nLevel = CountLevel(m_nTime,m_nLeftTime);
	// ����ͼ�����Ҫ�ĸ���
	CountIcon(m_nLevel);
	// ���صȼ�ͼ��
	CResBitmap bmp;
	bmp.LoadBitmap(IDB_BMP_LEVEL);
	m_imglistLevel.Create(LEVEL_ICON_WIDTH,LEVEL_ICON_HEIGHT,ILC_COLOR24|ILC_MASK,0,1);
	m_imglistLevel.Add(&bmp,RGB(0,255,0));
	bmp.DeleteObject();
	// ��������
	m_font.CreateFont(12, 
		0,
		0, 
		90,
		FW_NORMAL,
		0,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		(DEFAULT_PITCH|FF_MODERN),
		"����"
	);

	m_clrBackgournd = RGB(255,255,255);
	m_clrText = RGB(0, 0, 0);
}

CUserLevelCtrl::~CUserLevelCtrl()
{
	m_font.Detach();
}


BEGIN_MESSAGE_MAP(CUserLevelCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



void CUserLevelCtrl::SetOnlineTime(int nTime)
{
	if(m_nTime==nTime)
		return;
	if(nTime<0)
		nTime=0;
	m_nTime=nTime;
	// ����ȼ�
	m_nLevel = CountLevel(m_nTime,m_nLeftTime);
	// ����ͼ�����Ҫ�ĸ���
	CountIcon(m_nLevel);
	// �ػ�
	Invalidate();
}

void CUserLevelCtrl::SetLevel(int nLevel)
{
	if(m_nLevel==nLevel)
	{
		return;
	}
	if(nLevel<0)
		nLevel=0;
	m_nLevel=nLevel;
	m_nTime=(nLevel*nLevel+4*nLevel)*18;
	CountLevel(m_nTime,m_nLeftTime);
	// ����ͼ�����Ҫ�ĸ���
	CountIcon(m_nLevel);
	// �ػ�
	Invalidate();
}

void CUserLevelCtrl::CountIcon(int nLevel)
{
	int nLeft;
	m_nSun=nLevel/(LEVEL_STEP*LEVEL_STEP);
	nLeft=nLevel-m_nSun*(LEVEL_STEP*LEVEL_STEP);
	m_nMoon=nLeft/LEVEL_STEP;
	m_nStar=nLeft-m_nMoon*LEVEL_STEP;
}

void CUserLevelCtrl::RecalcLayerout(CDC* pDC)
{
	CRect rct;
	GetClientRect(&rct);
	CPoint pt(0,0);
	CSize size;
	// �߶Ⱦ��л���
	if(rct.Height()>LEVEL_ICON_HEIGHT)
	{
		pt.y=(rct.Height()-LEVEL_ICON_HEIGHT)/2;
	}
	size.cx=(m_nSun+m_nMoon+m_nStar)*(LEVEL_ICON_WIDTH+1);
	size.cy=LEVEL_ICON_HEIGHT;
	// ��õȼ�ͼ�������
	m_rtIcon.left=pt.x;
	m_rtIcon.top=pt.y;
	m_rtIcon.right=pt.x+size.cx;
	m_rtIcon.bottom=pt.y+size.cy;

	// ����ı���ʾ������
	CString szText;
	CFont* pOldFont=pDC->SelectObject(&m_font);
	m_rtTime.left=0;
	m_rtTime.top=pt.y;
	// ���㵱ǰ����ʱ���ı�
	szText.Format("%dСʱ",m_nTime);
	size=pDC->GetTextExtent(szText);
	// ����ͼ����
	m_rtTime.right=m_rtTime.left+(LEVEL_ICON_WIDTH+1)+size.cx;
	if(size.cy<LEVEL_ICON_HEIGHT)
		size.cy=LEVEL_ICON_HEIGHT;
	m_rtTime.bottom=m_rtTime.top+size.cy;
	// ����ʣ��ʱ���ı�
	m_rtLeft.left=m_rtTime.right+5;
	m_rtLeft.top=m_rtTime.top;
	m_rtLeft.bottom=m_rtTime.bottom;
	szText.Format("%dСʱ",m_nLeftTime);
	size=pDC->GetTextExtent(szText);
	// ����ͼ����
	m_rtLeft.right=m_rtLeft.left+(LEVEL_ICON_WIDTH+1)+size.cx;
	
	// ����˵����ϢӦ���Ҷ���
	int nMoveX=rct.Width()-m_rtTime.Width()-m_rtLeft.Width();
	if(nMoveX>0)
	{
		m_rtTime.OffsetRect(nMoveX,0);
		m_rtLeft.OffsetRect(nMoveX,0);
	}

	pDC->SelectObject(pOldFont);

}

void CUserLevelCtrl::SetBkColor(COLORREF clr)
{
	m_clrBackgournd = clr;
}

void CUserLevelCtrl::OnPaint()
{
	CPaintDC dc(this); 
	CRect rct;
	GetClientRect(&rct);
	dc.FillSolidRect(&rct,m_clrBackgournd);
	int i=0;
	// ���¼�������
	RecalcLayerout(&dc);

	CPoint pt(0,0);
	pt=m_rtIcon.TopLeft();
	// ���ݵȼ�����ͼ��
	// 
	for(i=0;i<m_nSun;i++)
	{
		m_imglistLevel.Draw(&dc,0,pt,ILD_NORMAL);
		pt.x+=LEVEL_ICON_WIDTH+1;
	}
	for(i=0;i<m_nMoon;i++)
	{
		m_imglistLevel.Draw(&dc,1,pt,ILD_NORMAL);
		pt.x+=LEVEL_ICON_WIDTH+1;
	}
	for(i=0;i<m_nStar;i++)
	{
		m_imglistLevel.Draw(&dc,2,pt,ILD_NORMAL);
		pt.x+=LEVEL_ICON_WIDTH+1;
	}

	
	CFont* pOldFont=dc.SelectObject(&m_font);
	CString szText;

	/*����ʵ����ʱ�����Լ������´�����ʱ����zhangmin*/
	/*
	// ��ӡ��ǰ����ʱ��
	dc.SetTextColor(m_clrText);
	pt=m_rtTime.TopLeft();
	szText.Format("%dСʱ",m_nTime);
	m_imglistLevel.Draw(&dc,3,pt,ILD_NORMAL);
	pt.x+=LEVEL_ICON_WIDTH+1;
	// ��ӡ�ı�ʱ�����ı����µ�һ��λ��
	dc.TextOut(pt.x,pt.y+3,szText);
	
	// ��ӡ���´�����ʣ��ʱ��
	pt=m_rtLeft.TopLeft();
	szText.Format("%dСʱ",m_nLeftTime);
	m_imglistLevel.Draw(&dc,3,pt,ILD_NORMAL);
	pt.x+=LEVEL_ICON_WIDTH+1;
	dc.TextOut(pt.x,pt.y+3,szText);
*/
	dc.SelectObject(pOldFont);

}


/*
�ȼ����㹫ʽ��
time = (N`2+4*N)*a
a=18
����NΪ�û��ȼ���timeΪ�õȼ���Ҫ��ʱ��
����֪����ʱ��time���Լ���ȼ� N = -2 + (�̣�(16+time*(4/a)))/2
*/
int CUserLevelCtrl::CountLevel(int nTime,int&nNextDelta)
{
	int nLevel;
	float fLevel;
	int nNextTime;	// ����һ�ȼ���Ҫ��ʱ��
	fLevel=-2.0+sqrt(16.0+nTime*(4.0/18))/2.0;
	nLevel=fLevel;
	if(nLevel==0)
		nLevel=1;
	if(fLevel >= nLevel)
	{
		nLevel++;
	}
	nNextTime=nLevel*nLevel+4*nLevel;
	nNextTime *= 18;
	nNextDelta=nNextTime-nTime;

	return nLevel;
}



void CUserLevelCtrl::ShowToolTip(const CString& strTip)
{
	if(m_sLastTip==strTip)
		return;
	m_sLastTip=strTip;
	if (m_toolTip.m_hWnd == NULL)
	{
		m_toolTip.Create(this,TTS_ALWAYSTIP);
		// ���ó�����ʾ��Ҫ��ʱ�䳤��
		m_toolTip.SetDelayTime(TTDT_INITIAL,100);
		// ������ʾ����ʱ��
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

void CUserLevelCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	
	CString sTip;
	if(m_rtIcon.PtInRect(point))
	{
		sTip.Format("%d��",m_nLevel);
	}
	else if(m_rtTime.PtInRect(point))
	{
		sTip.Format("����ʱ��%dСʱ",m_nTime);
	}
	else if(m_rtLeft.PtInRect(point))
	{
		sTip.Format("���´���������%dСʱ",m_nLeftTime);
	}
	ShowToolTip(sTip);
	
	CStatic::OnMouseMove(nFlags, point);
}



BOOL CUserLevelCtrl::PreTranslateMessage(MSG* pMsg)
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

BOOL CUserLevelCtrl::OnEraseBkgnd(CDC* pDC)
{

	

	return CStatic::OnEraseBkgnd(pDC);
}

void CUserLevelCtrl::SetTextColor(COLORREF clr)
{
	m_clrText = clr;
}

void CUserLevelCtrl::SetFont(CFont &font)
{
	m_font.Detach();
	LOGFONT log;
	font.GetLogFont(&log);
	m_font.CreateFontIndirect(&log);
}