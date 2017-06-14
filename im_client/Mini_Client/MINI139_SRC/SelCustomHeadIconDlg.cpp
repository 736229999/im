// SelCustomHeadIconDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelCustomHeadIconDlg.h"
#include "afxdialogex.h"
#include "ModHeadIconDlg.h"
// CSelCustomHeadIconDlg 对话框

IMPLEMENT_DYNAMIC(CSelCustomHeadIconDlg, CXSkinDlg)

CSelCustomHeadIconDlg::CSelCustomHeadIconDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CSelCustomHeadIconDlg::IDD, pParent)
{
	//装载GDI+
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	//区域边框初始化
	m_rectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine;
	m_rectTracker.SetTrackerColor(RGB(0x3F,0xB5,0xFF));
	m_rectTracker.SetTrackerType(TRACKER_TYPE_HEAD);

	m_colorBackground = RGB(255, 255, 255);
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	m_nHandle = -1;
	m_nTrackerRectWidth = 88;
	m_nTrackerRectHeight = 88;
}

CSelCustomHeadIconDlg::~CSelCustomHeadIconDlg()
{
	//卸载GDI+
	GdiplusShutdown(m_pGdiToken);
	m_bitmap.DeleteObject();
}

void CSelCustomHeadIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SELECTPIC, m_btnSelectPic);
	DDX_Control(pDX, IDC_STATIC_PIC, m_staticPic);
}


BEGIN_MESSAGE_MAP(CSelCustomHeadIconDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SELECTPIC, &CSelCustomHeadIconDlg::OnBnClickedBtnSelectpic)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSelCustomHeadIconDlg 消息处理程序


BOOL CSelCustomHeadIconDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CSelCustomHeadIconDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}
	pDC->SetTextColor(0x2E2E2E);
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CSelCustomHeadIconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_STATIC_PICEXT)->SetFont(&g_font12);
	m_btnSelectPic.SetPng(IDB_PNG_SENIOR_BTN_HOVER_L, IDB_PNG_SENIOR_BTN_NORMAL_L, IDB_PNG_SENIOR_BTN_PRESSED_L, IDB_PNG_SENIOR_BTN_PRESSED_L);
	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSelCustomHeadIconDlg::OnBnClickedBtnSelectpic()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sFilter = "头像(*.bmp;*jpg;*png;*.gif)|*.bmp;*jpg;*png;*.gif||";
	CFileDialog fileDlg(TRUE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, 
		sFilter,
		this);
	fileDlg.m_ofn.lpstrTitle = "选择头像文件";
	char szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));
	//打开文件目录改为默认目录
 	//GetCurrentDirectory(sizeof(szBuf), szBuf);
 	//fileDlg.m_ofn.lpstrInitialDir = szBuf;	
	if(IDOK==fileDlg.DoModal())
	{
		m_nTrackerRectWidth = 88;
		m_nTrackerRectHeight = 88;
		CString strRecordInfo;
		CString strFilePath = fileDlg.GetPathName();

		CString strPath,strFileName,strExt;
		m_strFilePath = strFilePath;

		CRect rc;
		m_staticPic.GetWindowRect(&rc);
		ScreenToClient(&rc);
		CRect rcPic;
		m_staticPic.GetClientRect(&rcPic);
		
		m_rect.left = 0;
		m_rect.top = 0;
		CImage imagePic; //使用图片类
		imagePic.Load( m_strFilePath );

		//图片按比例显示
		if(imagePic.GetWidth() > rc.Width() || imagePic.GetHeight() > rc.Height())
		{
			if(imagePic.GetWidth() / (double)rc.Width() > imagePic.GetHeight() / (double)rc.Height())
			{
				m_rect.right = rc.Width();
				m_rect.bottom = imagePic.GetHeight() * (double)rc.Width() / imagePic.GetWidth();
			}
			else if(imagePic.GetWidth() / (double)rc.Width() < imagePic.GetHeight() / (double)rc.Height())
			{
				m_rect.bottom = rc.Height();
				m_rect.right = imagePic.GetWidth() * (double)rc.Height() / imagePic.GetHeight();
			}
			else if(imagePic.GetWidth() / (double)rc.Width() == imagePic.GetHeight() / (double)rc.Height())
			{
				m_rect.right = rc.Width();
				m_rect.bottom = rc.Height();
			}
		}
		else
		{
			m_rect.right = imagePic.GetWidth();
			m_rect.bottom = imagePic.GetHeight();
		}
		m_rectPic.left = 0;
		m_rectPic.top = 0;

		m_rectPic.right = imagePic.GetWidth() > m_rect.Width() ? m_rect.Width() : imagePic.GetWidth();
		m_rectPic.bottom = imagePic.GetHeight() > m_rect.Height() ? m_rect.Height() : imagePic.GetHeight();

		CDC *pDC;
		pDC = m_staticPic.GetDC();
		CBrush brush;
		brush.CreateSolidBrush(RGB(105,105,105));
		
		pDC->FillSolidRect(rcPic, RGB(255,255,255));

		Graphics graphics(pDC->m_hDC);
		char *p=(LPSTR)(LPCSTR)m_strFilePath;

		WCHAR buf[1024];
		CStringW strWide = CT2CW(m_strFilePath); // 将T字符转换成宽字符
		wcscpy(buf, strWide); 

		Image image(buf, FALSE);
		graphics.DrawImage(&image, 0, 0, m_rect.Width(), m_rect.Height());  //绘制图片

		pDC->SelectObject(&brush);
		pDC->DrawEdge(rcPic,EDGE_ETCHED,BF_RECT);

		//绘制框选边框，四边都留有1/20的位置，如果长度小于88则框住全部的位置
		CRect rcTracker;
		if(m_rect.Width() > m_nTrackerRectWidth + m_nTrackerRectWidth / 10)
		{
			if(m_rect.Height() > m_nTrackerRectHeight + m_nTrackerRectHeight / 10)
			{
				rcTracker.left = /*(m_rect.Width() - m_nTrackerRectWidth) / 2*/m_rect.Width() / 20;
				rcTracker.right = /*rcTracker.left + m_nTrackerRectWidth*/m_rect.Width() * 19 / 20;

				rcTracker.top = /*(m_rect.Height() - m_nTrackerRectHeight) / 2*/m_rect.Height() / 20;
				rcTracker.bottom = /*rcTracker.top + m_nTrackerRectHeight*/m_rect.Height() * 19 / 20;
			}
			else
			{
				rcTracker.left = /*(m_rect.Width() - m_rect.Height()) / 2*/m_rect.Width() / 20;
				rcTracker.right = /*rcTracker.left + m_rect.Height()*/m_rect.Width() * 19 / 20;

				rcTracker.top = 1;
				rcTracker.bottom = m_rect.Height();
				m_nTrackerRectHeight = m_rect.Height();
			}
		}
		else if(m_rect.Height() > m_nTrackerRectHeight + m_nTrackerRectHeight / 10)
		{
			if(m_rect.Width() > m_nTrackerRectWidth + m_nTrackerRectWidth / 10)
			{
				rcTracker.left = /*(m_rect.Width() - m_nTrackerRectWidth) / 2*/m_rect.Height() / 20;
				rcTracker.right = /*rcTracker.left + m_nTrackerRectWidth*/m_rect.Width() * 19 / 20;

				rcTracker.top = /*(m_rect.Height() - m_nTrackerRectHeight) / 2*/m_rect.Height() / 20;
				rcTracker.bottom = /*rcTracker.top + m_nTrackerRectHeight*/m_rect.Height() * 19 / 20;
			}
			else
			{

				rcTracker.top = /*(m_rect.Height() - m_rect.Width()) / 2*/m_rect.Height() / 20;
				rcTracker.bottom = /*rcTracker.top + m_rect.Width()*/m_rect.Height() * 19 / 20;

				rcTracker.left = 1;
				rcTracker.right = m_rect.Width();
				m_nTrackerRectWidth = m_rect.Width();
			}
		}
		else
		{
			rcTracker.left = 1;
			rcTracker.right = m_rect.Width();

			rcTracker.top = 1;
			rcTracker.bottom = m_rect.Height();

			m_nTrackerRectWidth = m_rect.Width();
			m_nTrackerRectHeight = m_rect.Height();
		}

		rcTracker.left += rc.left;
		rcTracker.top += rc.top;
		rcTracker.right += rc.left;
		rcTracker.bottom += rc.top;
		m_rectTracker.m_rect.SetRect(rcTracker.left, rcTracker.top, rcTracker.right, rcTracker.bottom);
		m_rect = rcTracker;

		ReleaseDC(pDC);

		pDC = GetDC();

		CBitmap bitmap;
		//bitmap.DeleteObject();
		bitmap.CreateCompatibleBitmap(pDC, rcTracker.Width(), rcTracker.Height());
		CDC memDcInit;
		memDcInit.CreateCompatibleDC(pDC);


		CBitmap *oldmemBitmapInit = memDcInit.SelectObject(&bitmap);
		memDcInit.BitBlt(0, 0, rcTracker.Width(),rcTracker.Height(), pDC, rcTracker.left, rcTracker.top, SRCCOPY);  
		memDcInit.SelectObject(oldmemBitmapInit);
		memDcInit.DeleteDC();

		//框选图片显示到预览框
		GetParent()->SendMessage(WM_PIC_PREVIEW_MSG, (DWORD)&bitmap);

		CRect rcParent;
		GetClientRect(&rcParent);

		m_bitmap.DeleteObject();
		m_bitmap.CreateCompatibleBitmap(pDC, rc.Width()-1, rc.Height()-1);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);


		CBitmap *oldmemBitmap = memDC.SelectObject(&m_bitmap);
		memDC.BitBlt(0, 0, rc.Width()-1,rc.Height()-1, pDC, rc.left, rc.top, SRCCOPY);  
		memDC.SelectObject(oldmemBitmap);

		memDC.DeleteDC();
		oldmemBitmap->DeleteObject();

		//添加白色虚线呈九宫格
		Graphics graphics2(pDC->m_hDC);
		Pen pen(Color(255, 255, 255, 255), 1);
		pen.SetDashStyle(DashStyleDash);//虚线  
		graphics2.DrawLine(&pen, rcTracker.left + rcTracker.Width() / 3, rcTracker.top, rcTracker.left + rcTracker.Width() / 3, rcTracker.bottom);
		graphics2.DrawLine(&pen, rcTracker.left + rcTracker.Width() / 3 * 2, rcTracker.top, rcTracker.left + rcTracker.Width() / 3 * 2, rcTracker.bottom);
		graphics2.DrawLine(&pen, rcTracker.left, rcTracker.top + rcTracker.Height() / 3, rcTracker.right, rcTracker.top + rcTracker.Height() / 3);
		graphics2.DrawLine(&pen, rcTracker.left, rcTracker.top + rcTracker.Height() / 3 * 2, rcTracker.right, rcTracker.top + rcTracker.Height() / 3 * 2);

		CRect rect;
 		m_rectTracker.GetTrueRect(&rect);
		pDC->SelectStockObject(NULL_BRUSH);  
		m_rectTracker.Draw(pDC/*, &pen*/);
		ReleaseDC(pDC);
	}
}

void CSelCustomHeadIconDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_rectTracker.HitTest(point)>=0)
	{
		m_nMoveX = 0;
		m_nMoveY = 0;
		m_nDownX = point.x;
		m_nDownY = point.y;
		m_rectLast = m_rect;

		m_nHandle = m_rectTracker.HitTestHandles(point);
		SetTimer(1, 100, NULL);
	}

	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CSelCustomHeadIconDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pWnd==this && this->m_rectTracker.SetCursor(this,nHitTest))
	{
		return true;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CSelCustomHeadIconDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}


void CSelCustomHeadIconDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nHandle = -1;
	m_nMoveX = 0;
	m_nMoveY = 0;
	m_nDownX = point.x;
	m_nDownY = point.y;
	CDialog::OnLButtonUp(nFlags, point);
}


void CSelCustomHeadIconDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_nHandle == -1)
		return;

	//获取鼠标移动的大小
	m_nMoveX = m_nDownX - point.x;
	m_nMoveY = m_nDownY - point.y;

	CRect rcPic;
	m_staticPic.GetWindowRect(&rcPic);
	ScreenToClient(&rcPic);
	rcPic.right = rcPic.left + m_rectPic.Width();
	rcPic.bottom = rcPic.top + m_rectPic.Height();

	if(m_nHandle == /*hitTopLeft*/0)//左上拖动
	{
		if( m_rectLast.left - m_nMoveX >= rcPic.left+1 && m_rectLast.top - m_nMoveY >= rcPic.top+1 )
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.top = m_rectLast.top - m_nMoveY;
			m_rect.right = m_rectLast.right;
			m_rect.bottom = m_rectLast.bottom;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitTopRight*/1)//右上拖动
	{
		if( m_rectLast.right - m_nMoveX <= rcPic.right-1 && m_rectLast.top - m_nMoveY >= rcPic.top+1 )
		{
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.top = m_rectLast.top - m_nMoveY;
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom;
		}

		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitBottomRight*/2)//右下拖动
	{
		if( m_rectLast.right - m_nMoveX <= rcPic.right-1 && m_rectLast.bottom - m_nMoveY <= rcPic.bottom-1 )
		{
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.left = m_rectLast.left;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitBottomLeft*/3)//左下拖动
	{
		if(m_rectLast.left - m_nMoveX >= rcPic.left+1 && m_rectLast.bottom - m_nMoveY <= rcPic.bottom-1)
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitTop*/4)//向上拖动
	{
		if( m_rectLast.top - m_nMoveY >= rcPic.top+1)
		{
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top - m_nMoveY;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitRight*/5)//向右拖动
	{
		if(m_rectLast.right - m_nMoveX <= rcPic.right-1)
		{
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom;
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitBottom*/6)//向下拖动
	{
		if(m_rectLast.bottom - m_nMoveY <= rcPic.bottom-1)
		{
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitLeft*/7)//向左拖动
	{
		if(m_rectLast.left - m_nMoveX >= rcPic.left+1)
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitMiddle*/8)//移动截屏区域
	{
		if(m_rectLast.left - m_nMoveX <= rcPic.left+1 || m_rectLast.right - m_nMoveX >= rcPic.right-1)
		{
			if(m_rectLast.bottom - m_nMoveY <= rcPic.bottom-1 &&  m_rectLast.top - m_nMoveY >= rcPic.top+1)
			{
				m_rect.bottom = m_rectLast.bottom - m_nMoveY;
				m_rect.top = m_rectLast.top - m_nMoveY;
			}
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
		else if(m_rectLast.bottom - m_nMoveY >= rcPic.bottom-1 ||  m_rectLast.top - m_nMoveY <= rcPic.top+1)
		{
			if(m_rectLast.left - m_nMoveX >= rcPic.left+1 && m_rectLast.right - m_nMoveX <= rcPic.right-1)
			{
				m_rect.left = m_rectLast.left - m_nMoveX;
				m_rect.right = m_rectLast.right - m_nMoveX;
			}
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
		else
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.top = m_rectLast.top - m_nMoveY;
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
	}
	m_rectLast = m_rect;
	
	//刷新背景图片，擦除之前画的框选框
 	Bitmap *pBmp = Bitmap::FromHBITMAP((HBITMAP)m_bitmap.m_hObject, NULL);
 	if(pBmp == NULL)
 		return;
  	Graphics bmpGraphics(pBmp);
  	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
 
 	CRect rc;
 	m_staticPic.GetWindowRect(&rc);
 	ScreenToClient(&rc); //子控件屏幕坐标映射到控件客户区；
 
 	CDC* pDC = GetDC();
 	/*Drawing on DC*/
 	Graphics graphics(pDC->m_hDC);
 	/*Important! Create a CacheBitmap object for quick drawing*/
 	CachedBitmap cachedBmp(pBmp,&graphics);
 	graphics.DrawCachedBitmap(&cachedBmp,rc.left,rc.top);
 
 	CBitmap bitmap;
 	//bitmap.DeleteObject();
 	bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
 	CDC memDC;
 	memDC.CreateCompatibleDC(pDC);
 
 
 	CBitmap *oldmemBitmap = memDC.SelectObject(&bitmap);
 	memDC.BitBlt(0, 0, m_rect.Width(),m_rect.Height(), pDC, m_rect.left, m_rect.top, SRCCOPY);  
 	memDC.SelectObject(oldmemBitmap);
 	memDC.DeleteDC();
 
	//白色虚线，呈九宫格
 	Graphics graphics2(pDC->m_hDC);
 	Pen pen(Color(255, 255, 255, 255), 1);
 	pen.SetDashStyle(DashStyleDash);//虚线  
 	graphics2.DrawLine(&pen, m_rect.left + m_rect.Width() / 3, m_rect.top, m_rect.left + m_rect.Width() / 3, m_rect.bottom);
 	graphics2.DrawLine(&pen, m_rect.left + m_rect.Width() / 3 * 2, m_rect.top, m_rect.left + m_rect.Width() / 3 * 2, m_rect.bottom);
 	graphics2.DrawLine(&pen, m_rect.left, m_rect.top + m_rect.Height() / 3, m_rect.right, m_rect.top + m_rect.Height() / 3);
 	graphics2.DrawLine(&pen, m_rect.left, m_rect.top + m_rect.Height() / 3 * 2, m_rect.right, m_rect.top + m_rect.Height() / 3 * 2);

	m_rectTracker.Draw(pDC);

	//记录鼠标移动到的位置,用于计算下次鼠标移动的距离
 	m_nDownX = point.x;
 	m_nDownY = point.y;
 
 	//框选区域显示到预览框
 	GetParent()->SendMessage(WM_PIC_PREVIEW_MSG, (DWORD)&bitmap);
 
 	ReleaseDC(pDC);
 	oldmemBitmap->DeleteObject();
 	DeleteObject(pBmp);
	delete pBmp;
	pBmp = NULL;
	CDialog::OnMouseMove(nFlags, point);
}

void CSelCustomHeadIconDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//如果鼠标拖动到图片框外则取消鼠标的响应
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	CRect rc;
	m_staticPic.GetWindowRect(rc);
	if(ptCursor.x > rc.right || ptCursor.x < rc.left || ptCursor.y > rc.bottom || ptCursor.y < rc.top)
	{
		m_nMoveX = 0;
		m_nMoveY = 0;
		m_nDownX = -1;
		m_nDownY = -1;
		m_nHandle = -1;
		KillTimer(1);
	}
	CDialog::OnTimer(nIDEvent);
}