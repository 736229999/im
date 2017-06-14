// DlgMyArts.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "DlgMyArts.h"
#include ".\dlgmyarts.h"
#include "WriteBlogDlg.h"


// CDlgMyArts 对话框

IMPLEMENT_DYNAMIC(CDlgMyArts, CDialog)
CDlgMyArts::CDlgMyArts(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMyArts::IDD, pParent)
{
}

CDlgMyArts::~CDlgMyArts()
{
}

void CDlgMyArts::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ARTS, m_staticArts);
	DDX_Control(pDX, IDC_STATIC_WEBARTS, m_staticWebArts);
}


BEGIN_MESSAGE_MAP(CDlgMyArts, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgMyArts 消息处理程序

void CDlgMyArts::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	
	CFont  font;
	font.CreatePointFont(90,"宋体");

	CPen  pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,248));

	CRect  rc;
	rc.left=0;
	rc.right=175;
	rc.top=20;
	rc.bottom=120;
	dc.SelectObject(&font);
	dc.SelectObject(&pen);
	dc.SetTextColor(RGB(1,161,255));
	dc.MoveTo(rc.left,rc.top);
	dc.LineTo(5,rc.top);
	dc.TextOut(7,15,"查看");
	CSize  cs=dc.GetTextExtent("查看");
	dc.MoveTo(8+cs.cx,20);
	dc.LineTo(rc.right,20);

	dc.MoveTo(rc.left,rc.top);
	dc.LineTo(rc.left,rc.bottom);
	dc.MoveTo(rc.left,rc.bottom-21);
	dc.LineTo(rc.right,rc.bottom-21);
	dc.MoveTo(rc.right-6,rc.top);
	dc.LineTo(rc.right-6,rc.bottom);
}

HBRUSH CDlgMyArts::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	CBrush  br;
	br.CreateSolidBrush(IM_SPACE_COLOR);
	hbr=(HBRUSH)br.Detach();
	pDC->SetTextColor(RGB(1,161,255));
	pDC->SetBkMode(TRANSPARENT);

	return hbr;
}

BOOL CDlgMyArts::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_staticArts.SetNormalColor(RGB(1,161,255));
	m_staticWebArts.SetNormalColor(RGB(1,161,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgMyArts::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgMyArts::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message==WM_COMMAND)
	{

		GetParent()->PostMessage(WM_COMMAND,wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
