// PictureCtrlClose.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureCtrlClose.h"
#include "afxdialogex.h"
#include "PictureCtrl.h"

// CPictureCtrlClose 对话框

IMPLEMENT_DYNAMIC(CPictureCtrlClose, CDialog)

	CPictureCtrlClose::CPictureCtrlClose(CWnd* pParent /*=NULL*/)
	: CDialog(CPictureCtrlClose::IDD, pParent)
{
	m_pPictureCtrl = (CPictureCtrl*)pParent;
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	m_bIsHover = FALSE;
}

CPictureCtrlClose::~CPictureCtrlClose()
{
}

void CPictureCtrlClose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPictureCtrlClose, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CPictureCtrlClose 消息处理程序

BOOL CPictureCtrlClose::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_brush.CreateSolidBrush(RGB(76,76,76));

	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);

	SetLayeredWindowAttributes(RGB(76,76,76),0,LWA_COLORKEY);

	m_pPictureCtrl->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPictureCtrlClose::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//绘制一个半透明的矩形   
	pDC->SetBkColor(TRANSPARENT);
	return CDialog::OnEraseBkgnd(pDC);
}

void CPictureCtrlClose::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_pPictureCtrl->SendMessage(WM_LBUTTONDBLCLK);
	return;
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPictureCtrlClose::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_pPictureCtrl->SendMessage(WM_LBUTTONDOWN);
	m_pPictureCtrl->CloseAll();
	return;
	CDialog::OnLButtonDown(nFlags, point);
}

void CPictureCtrlClose::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pPictureCtrl->SendMessage(WM_MOUSEMOVE);
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CPictureCtrlClose::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * & pImg)  
{  
	HINSTANCE hInst = AfxGetResourceHandle();  
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type  
	if (!hRsrc)  
		return FALSE;  
	// load resource into memory  
	DWORD len = SizeofResource(hInst, hRsrc);  
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
	if (!lpRsrc)  
		return FALSE;  
	// Allocate global memory on which to create stream  
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
	memcpy(pmem,lpRsrc,len);  
	IStream* pstm;  
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
	// load from stream  
	pImg=Gdiplus::Image::FromStream(pstm);  
	// free/release stuff  
	GlobalUnlock(m_hMem);  
	pstm->Release();  
	FreeResource(lpRsrc);  
	return TRUE;  
}  

void CPictureCtrlClose::DrawClose(BOOL bIsHover)
{
	if(bIsHover)
	{
		Image * img = NULL;
		ImageFromIDResource(IDB_PNG_CLOSE_HOVER,  "png",(Image*&)img);
		CDC* pDC = GetDC();
		Graphics graphics( pDC->m_hDC);
		graphics.DrawImage(img, 0,0);
	}
	else
	{
		Image * img = NULL;
		ImageFromIDResource(IDB_PNG_CLOSE_LEAVE,  "png",(Image*&)img);
		CDC* pDC = GetDC();
		if(m_bIsHover != bIsHover)
		{
			CRect rc;
			GetClientRect(&rc);
			pDC->FillRect(rc, &m_brush);
			Graphics graphics( pDC->m_hDC);
			graphics.DrawImage(img, 0,0);
		}
	}
	m_bIsHover = bIsHover;
}

void CPictureCtrlClose::SetPos(CRect rc)
{
	CRect rect;
	GetWindowRect(&rect);
	int nWidth = rect.Width();
	int nHight = rect.Height();
	rect.right = rc.right - 10;
	rect.top = rc.top + 10;
	rect.left = rect.right - nWidth;
	rect.bottom = rect.top + nHight;
	MoveWindow(rect); 
	SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	::SetWindowPos( NULL, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION );
	CDC* pDC = GetDC();
	Graphics graphics( pDC->m_hDC);

	Image * img = NULL;
	ImageFromIDResource(IDB_PNG_CLOSE_LEAVE,  "png",(Image*&)img);
	graphics.DrawImage(img, 0,0);

	m_pPictureCtrl->SetCloseRect(rect);
	ShowWindow(SW_SHOW);
}

HBRUSH CPictureCtrlClose::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	pDC->SetBkMode(TRANSPARENT);
	return   m_brush;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CPictureCtrlClose::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pPictureCtrl->CloseAll();
	return;
	CDialog::OnLButtonUp(nFlags, point);
}
