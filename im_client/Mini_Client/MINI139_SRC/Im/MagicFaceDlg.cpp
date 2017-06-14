// im\MagicFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\magicfacedlg.h"
#include "WebProcess.h"

// CMagicFaceDlg 对话框
CMagicFaceDlg * CMagicFaceDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CMagicFaceDlg, CDialog)
CMagicFaceDlg::CMagicFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicFaceDlg::IDD, pParent)
{
	m_iUserId     = 0;
	Create(IDD,pParent);
}

CMagicFaceDlg::~CMagicFaceDlg()
{
	pInstance=NULL;
}

void CMagicFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorer);
}

BEGIN_MESSAGE_MAP(CMagicFaceDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMagicFaceDlg 消息处理程序
void CMagicFaceDlg::Show(int x, int y, uint32 iUserId, HWND hWndSel)
{
	int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	int screen_cy = GetSystemMetrics(SM_CYSCREEN)-25;

	int cx=400, cy=297;

	if ( (x+cx)>screen_cx ) 
		x = screen_cx-cx;

	if ( (y+cy)>screen_cy )
		y = screen_cy-cy;

	if ( pInstance == NULL )
	{
		pInstance = new CMagicFaceDlg(GetDesktopWindow());
		pInstance->m_hFaceSelWnd = hWndSel;
	}
	else
	{
		pInstance->m_hFaceSelWnd = hWndSel;
	}
	
	pInstance->MoveWindow(x,y,cx,cy);	
	pInstance->ShowWindow(SW_SHOW);
	pInstance->BringWindowToTop();

	if ( pInstance->m_iUserId!=iUserId )
	{
		pInstance->m_iUserId=iUserId;

		if(g_pWebConfig)
		{
			CString url = (LPCSTR)g_pWebConfig->GetWebPageUrl(MAGIC_FACE,StrToWideStr(IntToStr(iUserId)));//>GetAspxUrl(atMagicFace,(LPCSTR)IntToStr(iUserId));
			pInstance->m_explorer.Navigate(url,NULL,NULL,NULL,NULL);
		}

	}
}

void CMagicFaceDlg::Destory()
{
	if ( pInstance )
	{
		pInstance->DestroyWindow();
	}
}

void CMagicFaceDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CMagicFaceDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

BOOL CMagicFaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(0,WS_EX_TOOLWINDOW);	
	return TRUE; 
}

void CMagicFaceDlg::OnKillFocus(CWnd* pNewWnd)
{
	ShowWindow(SW_HIDE);
}

BOOL CMagicFaceDlg::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CMagicFaceDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( nState==WA_INACTIVE )
	{
		ShowWindow(SW_HIDE);
	}
}

BEGIN_EVENTSINK_MAP(CMagicFaceDlg, CDialog)
	ON_EVENT(CMagicFaceDlg, IDC_EXPLORER1, 250, BeforeNavigate2Explorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

void CMagicFaceDlg::BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	USES_CONVERSION;
	CString strUrl = W2A(URL->bstrVal);

	strUrl.Trim();

	TRACE(strUrl);
	TRACE("\n");

	if ( strUrl.Right(2)=="))" )
	{
		static CString strSwf;
		static CString strName;

		int pos1=strUrl.Find("((");
		int pos2=strUrl.Find("))");

		if ( pos2>pos1 && pos1>0 )
		{
			int posDot = strUrl.Find(",",pos1);

			if ( posDot>pos1 && posDot<pos2 )
			{
				strName=strUrl.Mid(pos1+2,posDot-pos1-2);
				strSwf =strUrl.Mid(posDot+1,pos2-posDot-1);

				::SendMessage(this->m_hFaceSelWnd,WM_MAGICFACE_NOTIFY,(WPARAM)(LPCSTR)strSwf,(LPARAM)(LPCSTR)strName);
			
				*Cancel = TRUE;

				ShowWindow(SW_HIDE);
			}
		}

	}
}

void CMagicFaceDlg::OnNcPaint()
{
	CDC *pDC = GetWindowDC();
	CRect rc ;
	GetWindowRect(&rc);
	rc.right-=rc.left;
	rc.bottom-=rc.top;
	rc.left = rc.top = 0;
	pDC->Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
	ReleaseDC(pDC);
}

void CMagicFaceDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( this->m_explorer.m_hWnd )
	{
		this->m_explorer.MoveWindow(-19,-48,cx+58,cy+70);
	}
}
