// insertPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "insertPanel.h"
#include ".\insertpanel.h"
#include  "WriteBlogDlg.h"

 


// CinsertPanel 对话框

IMPLEMENT_DYNAMIC(CinsertPanel, CDialog)
CinsertPanel::CinsertPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CinsertPanel::IDD, pParent)
{
}

CinsertPanel::~CinsertPanel()
{
}

void CinsertPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINK1, m_staticLink);
	DDX_Control(pDX, IDC_BUTTON_LOCALFILE, m_staticPic);
	DDX_Control(pDX, IDC_BUTTON_WEBP, m_statWebPic);
	DDX_Control(pDX, IDC_BUTTON_ARTICLE2, m_buttonMyArts);
	DDX_Control(pDX, IDC_BUTTON_ARTICLE, m_buttonArtsInWeb);
	DDX_Control(pDX, IDC_STATIC_LINK3, m_staticMyArts);
	DDX_Control(pDX, IDC_STATIC_LINK2, m_staticWebArts);
}


BEGIN_MESSAGE_MAP(CinsertPanel, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
//	ON_STN_CLICKED(IDC_BUTTON_WEBP, OnStnClickedButtonWebp)
//	ON_STN_CLICKED(IDC_BUTTON_LOCALFILE, OnStnClickedButtonLocalfile)
//	ON_STN_CLICKED(IDC_STATIC_LINK3, OnStnClickedStaticLink3)
//	ON_STN_CLICKED(IDC_STATIC_LINK2, OnStnClickedStaticLink2)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CinsertPanel 消息处理程序

BOOL CinsertPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(255,255,255));
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CinsertPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	
//	pDC->SetBkColor(IM_SPACE_COLOR);
	
	CBrush  br;
	br.CreateSolidBrush(RGB(255,255,255));
	hbr=(HBRUSH)br.Detach();
	pDC->SetTextColor(RGB(1,161,255));
	pDC->SetBkMode(TRANSPARENT);

	
	return hbr;
}

BOOL CinsertPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	


	//m_staticLink.SetLinkColor(RGB(1,161,255));
	m_staticLink.SetNormalColor(RGB(1,161,255));
	//m_staticPic.SetLinkColor(RGB(1,161,255));
	m_staticPic.SetNormalColor(RGB(1,161,255));

	//m_statWebPic.SetLinkColor(RGB(1,161,255));
	m_statWebPic.SetNormalColor(RGB(1,161,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CinsertPanel::OnStnClickedButtonWebp()
//{
//	// TODO: 在此添加控件通知处理程序代码
////	((CWriteBlogDlg)GetParent()).OnStnClickedButtonWebpic();
//	//GetParent()->SendMessage(WM_LBUTTONDOWN,0,0);
//	
//
//}

//void CinsertPanel::OnStnClickedButtonLocalfile()
//{
//	// TODO: 在此添加控件通知处理程序代码
////	((CWriteBlogDlg)GetParent()).OnStnClickedButtonLocalfile();
//}

//BOOL CinsertPanel::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	
//
//	return CDialog::PreTranslateMessage(pMsg);
//}

//BOOL CinsertPanel::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	
//
//	return CDialog::PreTranslateMessage(pMsg);
//}

//LRESULT CinsertPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	
//		
//	if ( message==WM_COMMAND )
//		GetParent()->PostMessage(message,wParam,lParam);
//
//	return CDialog::WindowProc(message, wParam, lParam);
//}

//void CinsertPanel::OnStnClickedStaticLink3()
//{
//	// TODO: 在此添加控件通知处理程序代码
//     
//}

//void CinsertPanel::OnStnClickedStaticLink2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

void CinsertPanel::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CPaintDC  dc(this);
	CFont  font;
	font.CreatePointFont(90,"宋体");

	CPen  pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,248));

	CRect  rc;
	rc.left=0;
	rc.right=188;
	rc.top=10;
	rc.bottom=110;
	dc.SelectObject(&font);
	dc.SelectObject(&pen);
	dc.SetTextColor(RGB(1,161,255));
	dc.MoveTo(rc.left,rc.top);
	dc.LineTo(5,rc.top);
	dc.TextOut(7,5,"插入");
	CSize  cs=dc.GetTextExtent("插入");
	dc.MoveTo(6+cs.cx,10);
	dc.LineTo(rc.right,10);



	dc.MoveTo(rc.left,rc.top);
	dc.LineTo(rc.left,rc.bottom);
	dc.MoveTo(rc.left,rc.bottom);
	dc.LineTo(rc.right,rc.bottom);
	dc.MoveTo(rc.right,rc.top);
	dc.LineTo(rc.right,rc.bottom);
	CClientDC  cdc(this);
	m_bmpLink.LoadBitmap(IDB_BITMAP_LINK);
	m_bmpLocal.LoadBitmap(IDB_BITMAP_LOCAL);
	m_bmpInternet.LoadBitmap(IDB_BITMAP_INTERNET);
	CDC   dcMem;
	dcMem.CreateCompatibleDC(&cdc);
	dcMem.SelectObject(&m_bmpLocal);
	int nSaveDC=dcMem.SaveDC();
	cdc.BitBlt(4,55,16,16,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSaveDC);

	dcMem.SelectObject(&m_bmpInternet);
	nSaveDC=dcMem.SaveDC();
	cdc.BitBlt(4,85,16,16,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSaveDC);

	dcMem.SelectObject(&m_bmpLink);
	nSaveDC=dcMem.SaveDC();
	cdc.BitBlt(4,25,16,16,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSaveDC);

}

//BOOL CinsertPanel::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//
//	return CDialog::PreTranslateMessage(pMsg);
//}

LRESULT CinsertPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
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
