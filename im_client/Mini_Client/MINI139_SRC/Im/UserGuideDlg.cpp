// im\UserGuideDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\userguidedlg.h"

#include "ContactInviteDlg.h"
#include "FSDialog.h"
#include "P2PSendMsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
#include "MessageHandleWnd.h"
// CUserGuideDlg 对话框
CUserGuideDlg * CUserGuideDlg::pInstance=NULL;

IMPLEMENT_DYNAMIC(CUserGuideDlg, CXSkinDlg)
CUserGuideDlg::CUserGuideDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CUserGuideDlg::IDD, pParent)
{
	m_bmpDlg.LoadBitmap(IDB_USERGUIDE_DLG);
	Create(CUserGuideDlg::IDD,pParent);
}

CUserGuideDlg::~CUserGuideDlg()
{
	pInstance=NULL;
}

void CUserGuideDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC1,m_btn1);
	DDX_Control(pDX,IDC_STATIC2,m_btn2);
	DDX_Control(pDX,IDC_STATIC3,m_btn3);
}


BEGIN_MESSAGE_MAP(CUserGuideDlg, CXSkinDlg)
	ON_STN_CLICKED(IDC_STATIC1, OnStnClickedStatic1)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC2, OnStnClickedStatic2)
	ON_STN_CLICKED(IDC_STATIC3, OnStnClickedStatic3)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CUserGuideDlg 消息处理程序

BOOL CUserGuideDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("第一次使用IEC-iM吧？^^");
	SetDlgStyle(MINCLOSE);

	m_btn1.SetNormalColor(RGB(200,0,200));
	m_btn1.SetLinkColor(RGB(255,0,255));

	m_btn2.SetNormalColor(RGB(200,0,200));
	m_btn2.SetLinkColor(RGB(255,0,255));

	m_btn3.SetNormalColor(RGB(200,0,200));
	m_btn3.SetLinkColor(RGB(255,0,255));

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);



	return TRUE; 
}

HBRUSH CUserGuideDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( pWnd->GetDlgCtrlID()==IDC_STATIC )
	{
		hbr = g_hWhiteBrush;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(155,22,255));
	}

	return hbr;
}

void CUserGuideDlg::Show()
{
	if ( pInstance )
	{
		if ( pInstance->IsIconic() )
			pInstance->ShowWindow(SW_RESTORE);

		pInstance->BringWindowToTop();
	}
	else
	{
		pInstance=new CUserGuideDlg(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
		pInstance->BringWindowToTop();
	}
}

void CUserGuideDlg::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
}

void CUserGuideDlg::OnStnClickedStatic1()
{
	//DestroyWindow();
	SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	CAddFriendOrClusterDlg::Show();
}

void CUserGuideDlg::OnStnClickedStatic2()
{
	//DestroyWindow();
	SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	CContactInviteDlg::Show("","");
}

void CUserGuideDlg::OnStnClickedStatic3()
{
	//DestroyWindow();
	//SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	//if( !g_fiHelper.bBasicInfo )
	//{
	//	XT_GET_FRIEND_BASICINFO_REQ req;
	//	req.type = XT_GET_FRIEND_BASICINFO_REQ::TYPE_OTHER;
	//	req.count = 1;
	//	req.FriendID[0] = HELPER_ID;
	//	g_pProtocol->SendReq(req);
	//}

	//CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(&g_fiHelper);

	//if ( pDlg->m_hWnd==NULL )
	//	pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

	//if ( pDlg->IsWindowVisible()==FALSE )
	//	pDlg->ShowWindow(SW_SHOW);

	//if ( pDlg->IsIconic() )
	//	pDlg->ShowWindow(SW_RESTORE);

	//pDlg->BringWindowToTop();

}

void CUserGuideDlg::OnClose()
{
	DestroyWindow();
}

void CUserGuideDlg::OnDestroy()
{
	CXSkinDlg::OnDestroy();
	delete this;
}

void CUserGuideDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();

	DrawBitmap(&dc,4,28,&m_bmpDlg,RGB(0,255,0));
}

BOOL CUserGuideDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRet = CXSkinDlg::OnEraseBkgnd(pDC);

	CRect rect(210,145,420,242);
	pDC->FillSolidRect(&rect,0xFFFFFF);

	return bRet;
}
