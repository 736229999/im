// SelfInfoDlg1.cpp : 实现文件
//

#include "stdafx.h"
#include "SelfInfoDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "SelfInfoPreviewDlg.h"
#include "RoundFaceCtrl.h"
#include "SelfInfoEditDlg.h"
#include "ModHeadIconDlg.h"
#include "Im\WebProcess.h"

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;

// CSelfInfoDlg 对话框
#define TIMER_REFRESH	0x01			// 更新用户信息
IMPLEMENT_DYNAMIC(CSelfInfoDlg, CXSkinDlg)

CSelfInfoDlg::CSelfInfoDlg(CWnd* pParent)
	: CXSkinDlg(IDD_DLG_SELF_INFO, pParent)
{
	m_colorBackground = RGB(255, 255, 255);
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
	m_hBkCaptionBar = ::CreateSolidBrush(BACKGND_COLOR);
	SetDlgStyle(CXSkinDlg::CLOSE);

	m_pPreViewDlg = NULL;
	m_pEditDlg = NULL;
	m_pFacePic = NULL;
	m_pModHeadDlg = NULL;

	m_staLevel.SetBkColor(BACKGND_COLOR);
	m_staLevel.SetTextColor(RGB(255, 255, 255));
	m_staLevel.SetFont(g_font12B);
}

CSelfInfoDlg::~CSelfInfoDlg()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	if(m_pPreViewDlg != NULL)
	{
		if(::IsWindow(m_pPreViewDlg->GetSafeHwnd()))
		{
			m_pPreViewDlg->DestroyWindow();
		}
		delete m_pPreViewDlg;
		m_pPreViewDlg = NULL;
	}

	if(m_pEditDlg != NULL)
	{
		if(::IsWindow(m_pEditDlg->GetSafeHwnd()))
		{
			m_pEditDlg->DestroyWindow();
		}
		delete m_pEditDlg;
		m_pEditDlg = NULL;
	}

	if(m_pFacePic != NULL)
	{
		if(::IsWindow(m_pFacePic->GetSafeHwnd()))
		{
			m_pFacePic->DestroyWindow();
		}
		delete m_pFacePic;
		m_pFacePic = NULL;
	}

	if(m_pModHeadDlg != NULL)
	{
		if(::IsWindow(m_pModHeadDlg->GetSafeHwnd()))
		{
			m_pModHeadDlg->DestroyWindow();
		}
		delete m_pModHeadDlg;
		m_pModHeadDlg = NULL;
	}
}

void CSelfInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LEVEL, m_staLevel);
}

// CSelfInfoPreviewDlg 消息处理程序
BOOL CSelfInfoDlg::Create()
{
	BOOL bRlt = CXSkinDlg::Create(IDD_DLG_SELF_INFO, m_pParentWnd);

	ShowWindow(SW_SHOW);

	return bRlt;
}

BEGIN_MESSAGE_MAP(CSelfInfoDlg, CXSkinDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SELF_INFO_PREVIEW_DLG_MSG, OnSelMsg_PreviewDlg)
	ON_MESSAGE(WM_SELF_INFO_EDIT_DLG_MSG, OnSelMsg_EditDlg)
	ON_BN_CLICKED(50001, OnBnClickedFace)
	ON_MESSAGE(WM_MOD_HEAD_DLG_MSG, OnModHeadDlgMsg)
	ON_XT_MESSAGE()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CSelfInfoDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK, OnUserInfoModAck)
END_XT_MESSAGE_MAP()


// CSelfInfoDlg 消息处理程序
BOOL CSelfInfoDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	// 头像
	m_pFacePic = new CRountFaceCtrl(IDB_PNG_BIG_BROUND_NORMAL, IDB_PNG_BIG_BROUND_NORMAL);
	m_pFacePic->Create(this, 50001);
	CRect rcPic;
	m_pFacePic->GetWindowRect(&rcPic);
	ScreenToClient(&rcPic);
	CRect rcPicNew;
	rcPicNew.left = 10;
	rcPicNew.right = rcPicNew.left + rcPic.Width();
	rcPicNew.top = (150-rcPic.Height())/2;
	rcPicNew.bottom = rcPicNew.top + rcPic.Height();
	m_pFacePic->MoveWindow(&rcPicNew);
	m_pFacePic->ShowWindow(SW_SHOW);

	// 创建子窗口
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.top += 150;
	rcClient.left += 1;
	rcClient.right -= 1;
	rcClient.bottom -= 1;
	m_pPreViewDlg = new CSelfInfoPreviewDlg(this);
	m_pPreViewDlg->Create();
	m_pPreViewDlg->MoveWindow(rcClient);
	m_pEditDlg = new CSelfInfoEditDlg(this);
	m_pEditDlg->Create();
	m_pEditDlg->MoveWindow(rcClient);
	m_pEditDlg->ShowWindow(SW_HIDE);

	// 用户名设置字体
	GetDlgItem(IDC_STATIC_NAME)->SetFont(&g_font16B);
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(g_ImUserInfo.nickname);

	// 等级
	GetDlgItem(IDC_STATIC_LEVEL2)->SetFont(&g_font12);
	m_staLevel.SetOnlineTime(g_ImUserInfo.online_time);

	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);

	CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)g_ImUserInfo.face_url);
	m_pFacePic->SetPicPath(strLocalPath);

	return TRUE;
}

void CSelfInfoDlg::OnPaint()
{
	CXSkinDlg::OnPaint();
}

BOOL CSelfInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CXSkinDlg::OnEraseBkgnd(pDC);

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.top = TITLE_BAR_CY;
	rcClient.bottom = 150;
	rcClient.left += 1;
	rcClient.right -= 1;
	pDC->FillSolidRect(rcClient,BACKGND_COLOR);	

	return FALSE;
}

// 设置控件颜色
HBRUSH CSelfInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		hbr = m_hBkCaptionBar;
	}

	return hbr;
}

// 预览窗口自定义消息
LRESULT CSelfInfoDlg::OnSelMsg_PreviewDlg(WPARAM wp, LPARAM lp)
{
	if(((UINT)wp) == IDC_BTN_EDIT)
	{
		m_pPreViewDlg->ShowWindow(SW_HIDE);
		m_pEditDlg->ShowWindow(SW_SHOW);
		m_pEditDlg->ShowFriendInfo();
	}
	return 0;
}

// 编辑窗口自定义消息
LRESULT CSelfInfoDlg::OnSelMsg_EditDlg(WPARAM wp, LPARAM lp)
{
	if(((UINT)wp) == IDC_BTN_CANCEL)
	{
		m_pPreViewDlg->ShowWindow(SW_SHOW);
		m_pEditDlg->ShowWindow(SW_HIDE);
	}

	if(((UINT)wp) == IDC_BTN_SAVE)
	{
		m_pPreViewDlg->ShowWindow(SW_SHOW);
		m_pEditDlg->ShowWindow(SW_HIDE);
		m_pPreViewDlg->Refresh();
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText(g_ImUserInfo.nickname);
	}
	return 0;
}

// 修改头像
void CSelfInfoDlg::OnBnClickedFace()
{
// 	if(m_pModHeadDlg != NULL)
// 	{
// 		delete m_pModHeadDlg;
// 		m_pModHeadDlg = NULL;
// 	}

	if(m_pModHeadDlg == NULL)
	{
		m_pModHeadDlg = new CModHeadIconDlg();
		m_pModHeadDlg->Create(this);
	}
	m_pModHeadDlg->ShowWindow(SW_SHOW);
}

// 修改头像窗口的自定义消息
LRESULT CSelfInfoDlg::OnModHeadDlgMsg(WPARAM wP, LPARAM lP)
{
	UINT id = (UINT)wP;
	if(id == IDOK)
	{
		string strHeadUrl = m_pModHeadDlg->GetHeadUrl();
		if(!strHeadUrl.empty())
		{
			FRIEND_INFO user = g_ImUserInfo;
			strcpy(user.face_url, strHeadUrl.c_str());
			g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);
		}

	}

	if(id == IDOK || id == IDCANCEL)
	{
		delete m_pModHeadDlg;
		m_pModHeadDlg = NULL;
	}	

	return 0;
}

// 修改个人资料时后台返回消息
void CSelfInfoDlg::OnUserInfoModAck(const char *data,WORD wRet)
{
	LPXT_USERINFO_MOD_ACK ack = (LPXT_USERINFO_MOD_ACK)data;
	if(wRet == 0)
	{
		SetTimer(TIMER_REFRESH,350,NULL);
	}
}

void CSelfInfoDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==g_ImUserInfo.GetId() )
		{   
			// 更新头像
			//memcpy(&g_ImUserInfo,ack,sizeof(XT_USER_INFO));
			UpdateUserFullInfo(&g_ImUserInfo,ack);
			CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)g_ImUserInfo.face_url);
			m_pFacePic->SetPicPath(strLocalPath);
		}
	}
}

void CSelfInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_REFRESH:
		{
			KillTimer(nIDEvent);
			XT_USERINFO_GET_REQ req;
			req.fid = g_ImUserInfo.GetId();
			g_pProtocol->SendReq(req);
		}
		break;
	default:
		CXSkinDlg::OnTimer(nIDEvent);
	}
}
