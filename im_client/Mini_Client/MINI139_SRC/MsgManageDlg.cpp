#include "stdafx.h"
#include "resource.h"
#include "MsgManageDlg.h"
#include "FriendMsgManageDlg.h"

CMsgManageDlg *CMsgManageDlg::m_stapInstance = NULL;

IMPLEMENT_DYNAMIC(CMsgManageDlg, CXSkinDlg)

CMsgManageDlg::CMsgManageDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(IDD_DLG_MSGMANAGE, pParent)
{
	//////添加颜色设置///////////////
	SetDlgStyle(CXSkinDlg::ALL);
	m_colorBackground = RGB(235, 235, 235);
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	m_font.CreateFont(
		19,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	// tab设置
	m_MainTab.SetFont(&m_font);
	m_MainTab.AddItem("联系人");
	m_MainTab.AddItem("群");
	m_MainTab.AddItem("系统消息");
	m_MainTab.SetColor(RGB(237, 238, 238), RGB(0,0,0), RGB(77,149,194));
	m_MainTab.SetFrameModol(FRAME_DRAW_BOTTOM);

	m_pDlgFriendMsg = new CMsgManage_Friend_Dlg(this);
}

CMsgManageDlg::~CMsgManageDlg()
{
	if(m_pDlgFriendMsg != NULL)
	{
		delete m_pDlgFriendMsg;
		m_pDlgFriendMsg = NULL;
	}
}

void CMsgManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_TAB, m_MainTab);
}

BEGIN_MESSAGE_MAP(CMsgManageDlg, CXSkinDlg)
	ON_WM_NCDESTROY()
	ON_MESSAGE(WM_MENUCTRL_SEL_CHANGE, OnColumnctrlSelChange)
END_MESSAGE_MAP()

// CMsgDlg 消息处理程序

BOOL CMsgManageDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();
	SetWindowText(_T("消息管理器"));
	m_MainTab.SetCurSel(0);
	InitTabCtrl();
	return TRUE;
}

void CMsgManageDlg::InitTabCtrl()
{
	CRect rc;
	GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rc);
	ScreenToClient(&rc);

	// 创建子窗口
	m_pDlgFriendMsg->Create(IDD_DLG_MSGMANAGE_FRIEND, this);
	m_pDlgFriendMsg->MoveWindow(&rc);

	//默认显示第一个
	m_pDlgFriendMsg->ShowWindow(SW_SHOW);

	//m_SystemCommon.Create(IDD_WIZARD_CLUSTER_SYS_COMMON,this);    //在TAB上创建
	//m_SystemCommon.MoveWindow(&rc);
	//m_SystemSecurity.Create(IDD_WIZARD_CLUSTER_SYS_SECURITY,this);  
	//m_SystemSecurity.MoveWindow(&rc);

	////////默认显示第一个
	//m_SystemCommon.ShowWindow(SW_SHOW);
	//m_SystemSecurity.ShowWindow(SW_HIDE);
}

CMsgManageDlg * CMsgManageDlg::Show()
{
	if ( m_stapInstance == NULL )
	{
		m_stapInstance = new CMsgManageDlg(GetDesktopWindow());
		m_stapInstance->Create(IDD_DLG_MSGMANAGE, GetDesktopWindow());
		m_stapInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if (m_stapInstance->IsIconic())
		{
			m_stapInstance->ShowWindow(SW_RESTORE);
		}

		m_stapInstance->BringWindowToTop();
	}

	return m_stapInstance;
}

// 菜单栏选着改变事件
LRESULT CMsgManageDlg::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CMenuCtrl *pCtrl = (CMenuCtrl*)wParam;
	if(pCtrl == &m_MainTab)
	{
		int nCurSel = HIWORD(lParam);
		if(nCurSel == 0)
		{	// 联系人
			/*m_SystemCommon.ShowWindow(SW_SHOW);
			m_SystemSecurity.ShowWindow(SW_HIDE);*/
		}
		if(nCurSel == 1)
		{	// 群
			/*m_SystemCommon.ShowWindow(SW_HIDE);
			m_SystemSecurity.ShowWindow(SW_SHOW);*/
		}
		if(nCurSel == 3)
		{	// 系统消息
			/*m_SystemCommon.ShowWindow(SW_HIDE);
			m_SystemSecurity.ShowWindow(SW_SHOW);*/
		}
	}

	return 0;
}

void CMsgManageDlg::OnCancel()
{
	CXSkinDlg::OnCancel();
	DestroyWindow();
}

void CMsgManageDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();
	delete this;
	m_stapInstance = NULL;
}