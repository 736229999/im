// ClusterSecurity.cpp : 实现文件
//

#include "stdafx.h"
#include "ClusterSecurity.h"
#include "afxdialogex.h"
#include "..\IMClient.h"

// CClusterSecurity 对话框

IMPLEMENT_DYNAMIC(CClusterSecurity, CDialog)

CClusterSecurity::CClusterSecurity(CWnd* pParent /*=NULL*/)
	: CDialog(CClusterSecurity::IDD, pParent)
{

}

CClusterSecurity::~CClusterSecurity()
{
}

void CClusterSecurity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_ALL, m_btnRadioAll);
	DDX_Control(pDX, IDC_RADIO_VERIFY, m_btnRadioVerify);
	DDX_Control(pDX, IDC_RADIO_NONE, m_btnRadioNone);
}


BEGIN_MESSAGE_MAP(CClusterSecurity, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CClusterSecurity 消息处理程序
BOOL CClusterSecurity::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 遍历所有子控件，设置控件字体
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	return TRUE;
}

HBRUSH CClusterSecurity::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nId = pWnd->GetDlgCtrlID();

		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
		{
			
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = g_hWhiteBrush;
		}

		if ( nId==IDC_STATIC_TIP || nId==IDC_STATIC_TIP2 )
		{
			pDC->SetTextColor(RGB(74,123,211));
		}

	}

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}


int  CClusterSecurity::GetClusterRadioBtnChecked()
{
	int nAuth =0;
	if ( m_btnRadioAll.GetCheck() )
	{
		nAuth=0;
	}
	else  if ( m_btnRadioVerify.GetCheck() )
	{
		nAuth=1;
	}
	else  if ( m_btnRadioNone.GetCheck() )
	{
		nAuth=2;
	}

	return nAuth;
}


BOOL CClusterSecurity::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			return TRUE;
			break;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
