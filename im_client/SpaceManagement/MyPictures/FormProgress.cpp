// im\FormProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "FormProgress.h"
#include ".\formprogress.h"


// CFormProgress 对话框
IMPLEMENT_DYNAMIC(CFormProgress, CDialog)
CFormProgress::CFormProgress(HWND hNotifyWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CFormProgress::IDD, pParent)
{
	m_hNotifyWnd = hNotifyWnd;
	Create(IDD,pParent);
}

CFormProgress::~CFormProgress()
{
}

void CFormProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDCANCEL,m_btnCancel);
	DDX_Control(pDX,IDC_STATIC_PROGRESS,m_wndProgress);
}


BEGIN_MESSAGE_MAP(CFormProgress, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFormProgress 消息处理程序

BOOL CFormProgress::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc,RGB(255,255,255));
	pDC->Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	return FALSE;
}

void CFormProgress::OnBnClickedCancel()
{
	::PostMessage(m_hNotifyWnd,WM_CANCEL_NOTIFY,0,0);
}

void CFormProgress::OnDestroy()
{
	CDialog::OnDestroy();
	m_wndProgress.UnLoad();
}

BOOL CFormProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_wndProgress.Load(GetResDLLHandle(),MAKEINTRESOURCE(IDR_GIF_LOGIN),"GIF") )
	{
		m_wndProgress.Draw();
	}

	m_btnCancel.SetFlatStyle(CFlatButton::FS_OFFICE);

	return TRUE; 
}




IMPLEMENT_DYNAMIC(CFormProgress2, CDialog)
CFormProgress2::CFormProgress2(CString strTip, CWnd* pParent /*=NULL*/)
	: CDialog(CFormProgress2::IDD, pParent)
{
	m_strTip = strTip;
	Create(IDD,pParent);
}

CFormProgress2::~CFormProgress2()
{
}

void CFormProgress2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_PROGRESS,m_wndProgress);
}


BEGIN_MESSAGE_MAP(CFormProgress2, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFormProgress 消息处理程序

BOOL CFormProgress2::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	CFont *pOldFont = pDC->SelectObject(GetFont());

	pDC->FillSolidRect(rc,RGB(255,255,255));
	pDC->Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,84,166));

	pDC->SetTextAlign(TA_CENTER);
	pDC->TextOut(rc.Width()/2,7,m_strTip);

	pDC->SelectObject(pOldFont);

	return FALSE;
}

void CFormProgress2::OnDestroy()
{
	CDialog::OnDestroy();
	m_wndProgress.UnLoad();
}

BOOL CFormProgress2::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_wndProgress.Load(GetResDLLHandle(),MAKEINTRESOURCE(IDR_GIF_LOGIN),"GIF") )
	{
		m_wndProgress.Draw();
	}

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	return TRUE; 
}



IMPLEMENT_DYNAMIC(CFormProgress3, CDialog)
CFormProgress3::CFormProgress3(HWND hNotifyWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CFormProgress3::IDD, pParent)
{
	m_hNotifyWnd = hNotifyWnd;
	m_hBrush = CreateSolidBrush(RGB(222,255,211));

	Create(IDD,pParent);
}

CFormProgress3::~CFormProgress3()
{
}

void CFormProgress3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDCANCEL,m_btnCancel);
	DDX_Control(pDX,IDC_STATIC_PROGRESS,m_wndProgress);
	DDX_Control(pDX,IDC_STATIC1,m_lblTip);
}


BEGIN_MESSAGE_MAP(CFormProgress3, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CFormProgress 消息处理程序

BOOL CFormProgress3::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc,RGB(222,255,211));
	pDC->Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	return FALSE;
}

void CFormProgress3::OnBnClickedCancel()
{
	::PostMessage(m_hNotifyWnd,WM_CANCEL_NOTIFY,0,0);
}

void CFormProgress3::OnDestroy()
{
	CDialog::OnDestroy();
	m_wndProgress.UnLoad();
}

BOOL CFormProgress3::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_wndProgress.Load(GetResDLLHandle(),MAKEINTRESOURCE(IDR_GIF_LOGIN),"GIF") )
	{
		m_wndProgress.Draw();
	}

	m_btnCancel.SetFlatStyle(CFlatButton::FS_OFFICE);

	return TRUE; 
}

HBRUSH CFormProgress3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		hbr = m_hBrush;
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}
