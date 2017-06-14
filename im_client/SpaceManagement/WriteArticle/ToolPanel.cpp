// ToolPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "ToolPanel.h"
#include ".\toolpanel.h"
#include "WriteBlogDlg.h"


// CToolPanel �Ի���

IMPLEMENT_DYNAMIC(CToolPanel, CDialog)
CToolPanel::CToolPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CToolPanel::IDD, pParent)
{

}

CToolPanel::~CToolPanel()
{
}

void CToolPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OPENF, m_staticOpen);
	DDX_Control(pDX, IDC_BUTTONF, m_delButton);
	DDX_Control(pDX, IDC_STATIC_OPENF, m_staticOpen);
	DDX_Control(pDX, IDC_STATIC_OPENS, m_staticOpen2);
	DDX_Control(pDX, IDC_BUTTONS, m_delButton2);
	DDX_Control(pDX, IDC_STATIC_OPENT, m_staticOpen3);
	DDX_Control(pDX, IDC_BUTTONT, m_delButton3);
	DDX_Control(pDX, IDC_STATIC_MORE, m_StaticMore);
	
	DDX_Control(pDX, IDC_STATIC_VIEWSPACE, m_staticViewSpace);
	
}


BEGIN_MESSAGE_MAP(CToolPanel, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CToolPanel ��Ϣ�������

BOOL CToolPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(255,255,255));
	
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CToolPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	DrawFrameOpen(&dc);
	
	
}
void CToolPanel::DrawFrameOpen(CPaintDC * dc)
{
	CRect  rcClient;
	GetClientRect(&rcClient);

	
	
	
	CPen  pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,248));
    
	CFont  font;
	font.CreatePointFont(90,"����");
	
	CRect  rc;
	rc.left=2;
	rc.right=rcClient.right-5;
	rc.top=100;
	rc.bottom=215;
	dc->SelectObject(&font);
	dc->SelectObject(&pen);
	dc->SetTextColor(RGB(1,161,255));
	dc->MoveTo(rc.left,rc.top);
	dc->LineTo(7,rc.top);
	dc->TextOut(9,rc.top-6,"�򿪲ݸ�");
	CSize  cs=dc->GetTextExtent("�򿪲ݸ�");
	dc->MoveTo(9+cs.cx,rc.top);
	dc->LineTo(rc.right,rc.top);



	dc->MoveTo(rc.left,rc.top);
	dc->LineTo(rc.left,rc.bottom);
	dc->MoveTo(rc.left,rc.bottom);
	dc->LineTo(rc.right,rc.bottom);
	dc->MoveTo(rc.right,rc.top);
	dc->LineTo(rc.right,rc.bottom+1);


	rc.left=2;
	rc.right=rcClient.right-5;
	rc.top=10;
	rc.bottom=80;


	dc->MoveTo(rc.left,rc.top);
	dc->LineTo(7,rc.top);
	dc->TextOut(8,rc.top-6,"����");
	CSize  cs1=dc->GetTextExtent("����");
	dc->MoveTo(8+cs1.cx,rc.top);
	dc->LineTo(rc.right,rc.top);



	dc->MoveTo(rc.left,rc.top);
	dc->LineTo(rc.left,rc.bottom);
	dc->MoveTo(rc.left,rc.bottom);
	dc->LineTo(rc.right,rc.bottom);
	dc->MoveTo(rc.right,rc.top);
	dc->LineTo(rc.right,rc.bottom+1);

	
	CClientDC  cdc(this);
	//CResBitmap   bmp;
	m_resBmp.LoadBitmap(IDB_BITMAP_OPEN);
	CDC   dcMem;
	dcMem.CreateCompatibleDC(&cdc);
	dcMem.SelectObject(&m_resBmp);

	int nSaveDC=dcMem.SaveDC();
   

	cdc.BitBlt(4,185,16,16,&dcMem,0,0,SRCCOPY);
	
	dcMem.RestoreDC(nSaveDC);












	


}

void CToolPanel::AdjustUI()
{

	CRect rc,rcClient;
	GetClientRect(&rcClient);

	


	rc.left=2;
	rc.right=rcClient.right;
	rc.top=10;
	rc.bottom=110;
	m_DlgMyArts.MoveWindow(&rc,FALSE);


	
	rc.top=220;
	rc.bottom=380;
	m_insertDlg.MoveWindow(&rc,FALSE);

	
}

BOOL CToolPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_staticOpen.SetNormalColor(RGB(1,161,255));
	m_staticOpen2.SetNormalColor(RGB(1,161,255));
	m_staticOpen3.SetNormalColor(RGB(1,161,255));
	m_StaticMore.SetNormalColor(RGB(1,161,255));

	

//	m_staticMySpace.SetNormalColor(RGB(1,161,255));
	m_staticViewSpace.SetNormalColor(RGB(1,161,255));
	
	m_delButton.SetTextColor(RGB(1,161,255));
	m_delButton2.SetTextColor(RGB(1,161,255));
	m_delButton3.SetTextColor(RGB(1,161,255));
	


	m_insertDlg.Create(CinsertPanel::IDD,this);
	m_insertDlg.ShowWindow(SW_SHOW);

	m_DlgMyArts.Create(CDlgMyArts::IDD,this);
	m_DlgMyArts.ShowWindow(SW_HIDE);
	
	AdjustUI();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CToolPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//if(nCtlColor==CTLCOLOR_STATIC)
	   
	    //pDC->SetBkColor(RGB(255,255,255));
		
		CBrush  br;
    	br.CreateSolidBrush(RGB(255,255,255));
     	hbr=(HBRUSH)br.Detach();
		pDC->SetTextColor(RGB(1,161,255));

		pDC->SetBkMode(TRANSPARENT);
	
	return hbr;
}

void CToolPanel::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: �ڴ˴������Ϣ����������
	
}

LRESULT CToolPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message==WM_COMMAND)
	{
		
			GetParent()->PostMessage(WM_COMMAND,wParam,lParam);
	}
	if (message==WM_LBUTTONDOWN||message==WM_MBUTTONDOWN||message==WM_RBUTTONDOWN)
	{
		GetParent()->PostMessage(WM_LBUTTONDOWN,wParam,lParam);
	}
	

	return CDialog::WindowProc(message, wParam, lParam);
}
