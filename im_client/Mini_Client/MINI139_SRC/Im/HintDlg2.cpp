// im\HintDlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\hintdlg2.h"

// CHintDlg2 对话框

IMPLEMENT_DYNAMIC(CHintDlg2, CDialog)
CHintDlg2::CHintDlg2(UINT nHintBitmap , int left, int top,CWnd *pParent)
	: CDialog(CHintDlg2::IDD, pParent )
{
	m_iLeft = left, m_iTop = top;
	m_bitmap.LoadBitmap(nHintBitmap);

	Create(CHintDlg2::IDD,pParent);
	ShowWindow(SW_SHOW);
}

CHintDlg2::~CHintDlg2()
{
	
}

void CHintDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHintDlg2, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHintDlg2 消息处理程序

BOOL CHintDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L|WS_EX_PALETTEWINDOW|0x00080000L);
	//ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L);

	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *_fSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD); 
		_fSetLayeredWindowAttributes SetLayeredWindowAttributes =(_fSetLayeredWindowAttributes)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		
		if(SetLayeredWindowAttributes )
			SetLayeredWindowAttributes(m_hWnd,RGB(0,255,0),0,1);

		FreeLibrary(hInst); 
	}

	BITMAP bi;
	m_bitmap.GetBitmap(&bi);
	MoveWindow(m_iLeft, m_iTop-bi.bmHeight, bi.bmWidth,bi.bmHeight);

	return TRUE; 
}

BOOL CHintDlg2::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CHintDlg2::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CDC dc1;
	dc1.CreateCompatibleDC(&dc);

	CBitmap *pOld1 = dc1.SelectObject(&m_bitmap);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dc1,0,0,SRCCOPY);

	dc1.SelectObject(pOld1);
}
