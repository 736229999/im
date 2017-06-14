// im\139MsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include "139MsgDlg.h"
#include ".\139msgdlg.h"


// C139MsgDlg 对话框
C139MsgDlg * C139MsgDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(C139MsgDlg, CXSkinDlg)
C139MsgDlg::C139MsgDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(C139MsgDlg::IDD, pParent)
{
	m_nCurrentLink = -1;
	Create(IDD,pParent);
}

C139MsgDlg::~C139MsgDlg()
{
	pInstance = NULL;
}

void C139MsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_EXPLORER1, m_wndExplore);
}


BEGIN_MESSAGE_MAP(C139MsgDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
END_MESSAGE_MAP()


// C139MsgDlg 消息处理程序
BOOL C139MsgDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(CLOSE);
	SetWindowText("公告");

	CRect rect;
	m_wndExplore.GetWindowRect(&rect);
	ScreenToClient(&rect);

	rect.InflateRect(-2,-2);

	m_wndFrame.Create("",WS_VISIBLE,rect,this);

	m_wndExplore.SetParent(&m_wndFrame);
	m_wndExplore.MoveWindow(0,0,234,233);
	m_wndExplore.MoveWindow(-2,-2,rect.Width()+4,rect.Height()+4);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;  
}

void C139MsgDlg::OnPaint()
{
	CPaintDC dc(this);
	CXSkinDlg::OnPaint();

	if ( m_wndFrame.m_hWnd )
	{
		CRect rc;
		m_wndFrame.GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.InflateRect(1,1);
		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

void C139MsgDlg::Show(CString strMessage)
{
	if ( pInstance==NULL )
	{
		pInstance = new C139MsgDlg(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
		pInstance->SetBullitinLinks(strMessage);
	}
}

void C139MsgDlg::Close()
{
	if ( pInstance )
	{
		pInstance->DestroyWindow();
	}
}

void C139MsgDlg::OnClose()
{
	DestroyWindow();
}

void C139MsgDlg::OnDestroy()
{
	CXSkinDlg::OnDestroy();
	delete this;
}

void C139MsgDlg::SetBullitinLinks(CString strLinks)
{
	int nPos1 = 0;
	int nPos2 = -1;

	while(1)
	{
		nPos2 = strLinks.Find("#",nPos1);

		if ( nPos2!=-1 )
		{
			m_slBullitinLink.AddTail(strLinks.Mid(nPos1,nPos2-nPos1));
			nPos1 = nPos2+1;
		}
		else
		{
			m_slBullitinLink.AddTail(strLinks.Mid(nPos1,strLinks.GetLength()-nPos1));
			break;
		}
	}

	ShowBullitin(0);
	m_nCurrentLink = 0;

}

void C139MsgDlg::ShowBullitin(int nIndex)
{
	CString strUrl = m_slBullitinLink.GetAt( m_slBullitinLink.FindIndex(nIndex) );
	m_wndExplore.Navigate(strUrl,NULL,NULL,NULL,NULL );


	m_btnPrev.EnableWindow(TRUE);
	m_btnNext.EnableWindow(TRUE);

	if ( nIndex==0 )
	{
		m_btnPrev.EnableWindow(FALSE);
	}

	if ( nIndex==(m_slBullitinLink.GetCount()-1) )
	{
		m_btnNext.EnableWindow(FALSE);
	}

}
void C139MsgDlg::OnBnClickedClose()
{
	DestroyWindow();
}

void C139MsgDlg::OnBnClickedNext()
{
	m_nCurrentLink++;
	if ( m_nCurrentLink<m_slBullitinLink.GetCount() )
	{
		ShowBullitin(m_nCurrentLink);
	}
	else
	{
		m_nCurrentLink--;
	}
}

void C139MsgDlg::OnBnClickedPrev()
{
	m_nCurrentLink--;
	if ( m_nCurrentLink>-1)
	{
		ShowBullitin(m_nCurrentLink);
	}
	else
	{
		m_nCurrentLink++;
	}
}
BEGIN_EVENTSINK_MAP(C139MsgDlg, CXSkinDlg)
	ON_EVENT(C139MsgDlg, IDC_EXPLORER1, 252, NavigateComplete2Explorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void C139MsgDlg::NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	if ( URL )
	{
	//	USES_CONVERSION;
	//	ShowMessage(W2A(URL->bstrVal));
	}
}
