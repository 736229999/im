
#include "stdafx.h"
#include "LinkResDlg.h"
#include "impublic.h"



IMPLEMENT_DYNAMIC(CLinkResDlg, CXSkinDlg)
CLinkResDlg::CLinkResDlg(CWnd* pMsg, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CLinkResDlg::IDD, pParent)
{
	m_pWndMsg = pMsg;
	Create(CLinkResDlg::IDD,pParent);
	m_bmpLinkRes.LoadBitmap(IDB_LINK_RES);
	m_hBrushWhite = CreateSolidBrush(RGB(255,255,255));
	if(m_hBkGround)
		DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(RGB(255,255,255));
}

CLinkResDlg::~CLinkResDlg()
{
	DeleteObject(m_hBrushWhite);
}

void CLinkResDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDT_LINKURL, m_edtResUrl);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_wndFrame);
}


BEGIN_MESSAGE_MAP(CLinkResDlg, CXSkinDlg)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()




CLinkResDlg* CLinkResDlg::NewDlg(CWnd* pMsg, CWnd* pParent /* = NULL */)
{
	CLinkResDlg* pWnd=new CLinkResDlg(pMsg, pParent);
	return pWnd;
}
// CLinkResDlgDlg 消息处理程序

void CLinkResDlg::OnBnClickedOk()
{
	static CString sUrl = "";
	m_edtResUrl.GetWindowText(sUrl);
	sUrl.Trim();
	if(sUrl == "")
	{
		OnOK();
		return;
	}
	// 检验url是否合法
	if(sUrl.Find("http://")==-1)
	{
		ShowMessage("URL地址不正确");
		return;
	}
	
	sUrl = "[img]" + sUrl + "[/img]";
	if(m_pWndMsg)
	{
		m_pWndMsg->SendMessage(WM_INSERTMSG,0,(LPARAM)(LPCTSTR)sUrl);
	}
	OnOK();
}

void CLinkResDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXSkinDlg::OnShowWindow(bShow, nStatus);

	m_edtResUrl.SetFocus();
}

BOOL CLinkResDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();
	SetDlgStyle(MINCLOSE);
	SetWindowText("添加网络资源");
	// 从剪贴板中获取文本
	CString sUrl;
	// 获取安全窗口句柄
	HWND hWnd = GetSafeHwnd();
	// 打开剪贴板
	::OpenClipboard(NULL); 
	// 获取剪贴板数据句柄
	HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);
	// 返回指定内存区域的当前大小
	DWORD dwLength = GlobalSize(hClipMemory); 
	// 锁定内存
	LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory); 
	// 保存得到的文本数据
	sUrl = CString(lpClipMemory); 
	// 内存解锁
	GlobalUnlock(hClipMemory); 
	// 关闭剪贴板
	::CloseClipboard(); 

	m_edtResUrl.SetWindowText(sUrl);
	
	return TRUE;  

}

void CLinkResDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();
	delete this;
}

void CLinkResDlg::OnPaint()
{
	CPaintDC dc(this);

	CXSkinDlg::OnPaint();

	CWnd *pWnd = GetDlgItem(IDC_BMP_LINKRES);
	
	if ( pWnd )
	{
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		DrawBitmap(&dc,rc.left,rc.top,&m_bmpLinkRes);
	}

}

HBRUSH CLinkResDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if( pWnd->GetDlgCtrlID()==IDC_STATIC_FRAME)
	{
		hbr = m_hBrushWhite;
	}
	else if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBrushWhite;
	}
	return hbr;
}
