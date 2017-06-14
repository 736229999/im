// im\MsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\msgdlg.h"
#include "xtpacket.h"
#include "WebProcess.h"
#include "..\MainFrame.h"
#include "..\DuiMfRecentList.h"
#include "DuiOrderSubscibe.h"
#include "DuiOrderNotice.h"
#define IDC_SPLITTER_EW  0x101
#define IDC_SPLITTER_NS  0x102
// CMsgDlg 对话框

#define PAGE_COUNT      13
#define PAGE_COUNT_TRANMSG 12

const DWORD	CLR_ACTIONS = RGB(0,84,166);	// 聊天动作字符颜色：黑色
const DWORD CLR_OBJECT = RGB(255,0,0);	// 聊天对象字符颜色


IMPLEMENT_DYNAMIC(CMsgDlg, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CMsgDlg)

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);


#ifdef _CS_PLAT_
	CString CMsgDlg::m_strSysMsgNames[CMsgDlg::m_nSysTypeCnt] =
	{
		"好友验证消息",
		"群系统消息",
		"哞哞系统消息",
	};
#else
	CString CMsgDlg::m_strSysMsgNames[CMsgDlg::m_nSysTypeCnt] =
	{
		"好友验证消息",
		"群系统消息",
		"哞哞系统消息",
		"交易消息",
		"商品消息",
		"退换货消息",
	};

#endif


CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDlg::IDD, pParent)
{
	m_pMainTab = new CUIMsgManageMainTab();
	m_pLabelList = new CLabelListWnd();
	m_pImgList = new CImgListWnd();
	m_pOrderTreeOrder = new CDuiOrderTree();
	m_pTranMsg = new CDuiOrderTree();
	m_colorBackground = CHAT_BOTTOM_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	m_iPage       = 0;

	m_iPageCount  = 0;
	m_iMessageType = 0;

	m_nSearchFriendId = 0;
	m_nSearchClusterId = 0;

	m_hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

	m_bSearch = FALSE;
	//Create(CMsgDlg::IDD,pParent);
}

CMsgDlg::~CMsgDlg()
{
	if(m_pMainTab != NULL)
	{
		delete m_pMainTab;
		m_pMainTab = NULL;
	}
	if(m_pLabelList != NULL)
	{
		delete m_pLabelList;
		m_pLabelList = NULL;
	}
	if(m_pImgList != NULL)
	{
		delete m_pImgList;
		m_pImgList = NULL;
	}
	if(m_pOrderTreeOrder != NULL)
	{
		delete m_pOrderTreeOrder;
		m_pOrderTreeOrder = NULL;
	}
	if(m_pTranMsg != NULL)
	{
		delete m_pTranMsg;
		m_pTranMsg = NULL;
	}
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RE_HISTORY, m_edtReHistory);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_btnFirst);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_btnLast);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_FIND, m_btnFind);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_COMBO1, m_comboRange);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_lblRange);
	DDX_Control(pDX, IDC_STATIC_KEYWORD, m_lblKeyword);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	ON_WM_NCDESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnBnClickedBtnFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedBtnNext)
	ON_XT_MESSAGE()
	ON_WM_TIMER()
	SET_HMG_SCROLLBAR(IDC_RE_HISTORY)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CMsgDlg::OnBnClickedBtnSearch)
	ON_MESSAGE(WM_MAINTAB_SEL_CHANGE, OnMainTabSelChange)
	ON_MESSAGE(WM_LABELLIST_SEL_CHANGE, OnList1SelChange)
	ON_MESSAGE(WM_IMGLIST_SEL_CHANGE, OnList2SelChange)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CMsgDlg)
	ON_XT_COMMAND(CMD_LOGOUT,OnLoginOut)
	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK,OnUserSearchAck)
	ON_XT_COMMAND(CMD_CLUSTER_SEARCH_ACK,OnClusterSearchAck)
END_XT_MESSAGE_MAP()

// CMsgDlg 消息处理程序

BOOL CMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 遍历所有子控件，设置控件字体
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	CRect rcClient1;
	GetClientRect(&rcClient1);
	m_pMainTab->Create(GetSafeHwnd(), _T("DUIMsgManageMainTab"), UI_WNDSTYLE_CHILD, 0, 0, rcClient1.top, rcClient1.Width(), 46); 
	m_pMainTab->ShowWindow(TRUE); 

	m_pLabelList->Create(GetSafeHwnd(), _T("DUIMsgManageLabelList"), UI_WNDSTYLE_CHILD, 0, 0, rcClient1.top+46, 115, rcClient1.Height()-46); 
	m_pLabelList->ShowWindow(TRUE); 

	m_pImgList->Create(GetSafeHwnd(), _T("DUIMsgManageImgList"), UI_WNDSTYLE_CHILD, 0, 115, rcClient1.top+46, 130, rcClient1.Height()-46); 
	m_pImgList->ShowWindow(TRUE); 

	m_rcRichEditSmall.left = 115 + 130;
	m_rcRichEditSmall.top = rcClient1.top+46;
	m_rcRichEditSmall.right = rcClient1.right;
	m_rcRichEditSmall.bottom = rcClient1.bottom-36;

	m_rcRichEditBig.left = 115;
	m_rcRichEditBig.top = rcClient1.top+46;
	m_rcRichEditBig.right = rcClient1.right;
	m_rcRichEditBig.bottom = rcClient1.bottom-36;

	m_pOrderTreeOrder->Create(GetSafeHwnd(), _T("交易消息"), UI_WNDSTYLE_CHILD, 0, m_rcRichEditBig.left, m_rcRichEditBig.top, m_rcRichEditBig.Width(), m_rcRichEditBig.Height());
	m_pOrderTreeOrder->ShowWindow(false);

	m_pTranMsg->Create(GetSafeHwnd(), _T("转接消息"), UI_WNDSTYLE_CHILD, 0, m_rcRichEditBig.left, m_rcRichEditBig.top, m_rcRichEditBig.Width(), m_rcRichEditBig.Height());
	m_pTranMsg->ShowWindow(false);

	m_btnFirst.SetPng(IDB_PNG_FIRST_HOVER, IDB_PNG_FIRST_NORMAL, IDB_PNG_FIRST_PRESSED, IDB_PNG_FIRST_DISABLE);
	m_btnFirst.SetToolTip(_T("第一页"));
	m_btnPrev.SetPng(IDB_PNG_PREV_HOVER, IDB_PNG_PREV_NORMAL, IDB_PNG_PREV_PRESSED, IDB_PNG_PREV_DISABLE);
	m_btnPrev.SetToolTip(_T("上一页"));
	m_btnNext.SetPng(IDB_PNG_NEXT_HOVER, IDB_PNG_NEXT_NORMAL, IDB_PNG_NEXT_PRESSED, IDB_PNG_NEXT_DISABLE);
	m_btnNext.SetToolTip(_T("下一页"));
	m_btnLast.SetPng(IDB_PNG_LAST_HOVER, IDB_PNG_LAST_NORMAL, IDB_PNG_LAST_PRESSED, IDB_PNG_LAST_DISABLE);
	m_btnLast.SetToolTip(_T("最后一页"));
	m_btnSearch.SetPic(IDB_PNG_SEARCH_PRESSED, IDB_PNG_SEARCH_HOVER, IDB_PNG_SEARCH_PRESSED, IDB_PNG_SEARCH_NORMAL, AfxGetInstanceHandle());
	m_btnSearch.SetToolTip(_T("查找"));
	m_btnFind.SetPng(IDB_PNG_SENIOR_BTN_HOVER, IDB_PNG_SENIOR_BTN_NORMAL, IDB_PNG_SENIOR_BTN_PRESSED, IDB_PNG_SENIOR_BTN_PRESSED);
	m_comboRange.SetBorderColor(0xc8c8c8);
	m_editKeyword.SetBorderColor(0xc8c8c8);

	m_edtReHistory.SetEmotionImageList(&g_imgEmotionList);


	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);
	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_ACK,m_hWnd);
	OnBnClickedBtnFriend();
	AttachHMGScrollbar(m_edtReHistory.m_hWnd);

	DWORD mask =::SendMessage(m_edtReHistory.m_hWnd,EM_GETEVENTMASK, 0, 0);  
	mask = mask | ENM_LINK  | ENM_MOUSEEVENTS | ENM_SCROLLEVENTS |ENM_KEYEVENTS;  
	::SendMessage(m_edtReHistory.m_hWnd,EM_SETEVENTMASK, 0, mask);  
	::SendMessage(m_edtReHistory.m_hWnd,EM_AUTOURLDETECT, true, 0);  
	m_edtReHistory.SetShowLinkInformation(TRUE);

	// 初始化查找范围
	InitComboRange();

	AdjustUI();

	SetSysMsgTabText();
	
	return TRUE; 
}

void CMsgDlg::OnNcDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);

	CDialog::OnNcDestroy();
}

void CMsgDlg::AdjustUI()
{
	CUIMsgManageMainTab::EN_SEL sel = m_pMainTab->GetCurSel();
	if(sel == CUIMsgManageMainTab::en_sel_friend 
		|| sel == CUIMsgManageMainTab::en_sel_cluster)
	{
		m_edtReHistory.MoveWindow(m_rcRichEditSmall);
		m_pLabelList->SetBkColor(0xFFD4D4D4, 0xFFD4D4D4, 0xFFDBDBDB, 0xFFEBEBEB);
		CRect rcBtn;
		m_btnSearch.GetWindowRect(&rcBtn);
		ScreenToClient(&rcBtn);
		int nWidth = rcBtn.Width();
		rcBtn.left = m_rcRichEditSmall.left + 2;
		rcBtn.right = rcBtn.left + nWidth;
		m_btnSearch.MoveWindow(&rcBtn);
	}

	if(sel == CUIMsgManageMainTab::en_sel_sysmsg)
	{
		m_edtReHistory.MoveWindow(m_rcRichEditBig);
		m_pLabelList->SetBkColor(0xFFEBEBEB, 0xFFEBEBEB, 0xFFF7F7F7, 0xFFFFFFFF);
		CRect rcBtn;
		m_btnSearch.GetWindowRect(&rcBtn);
		ScreenToClient(&rcBtn);
		int nWidth = rcBtn.Width();
		rcBtn.left = m_rcRichEditBig.left + 2;
		rcBtn.right = rcBtn.left + nWidth;
		m_btnSearch.MoveWindow(&rcBtn);

		
	}

	// 移动查找相关按钮
	CRect rcBtnSearch;
	m_btnSearch.GetWindowRect(&rcBtnSearch);
	ScreenToClient(&rcBtnSearch);
	UINT arrId[] = {IDC_STATIC_RANGE, IDC_STATIC_KEYWORD, IDC_COMBO1, IDC_EDIT_KEYWORD,IDC_BTN_FIND};
	int nDis = 0;
	for(int i = 0; i < sizeof(arrId)/sizeof(UINT); ++i)
	{
		CRect rcBtn;
		GetDlgItem(arrId[i])->GetWindowRect(&rcBtn);
		ScreenToClient(&rcBtn);
		if(i == 0)
			nDis = rcBtn.left - rcBtnSearch.left;

		rcBtn.left -= nDis;
		rcBtn.right -= nDis; 

		GetDlgItem(arrId[i])->MoveWindow(&rcBtn);
		GetDlgItem(arrId[i])->Invalidate();
	}
}

void CMsgDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

BOOL CMsgDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CMsgDlg::InitFriendTree()
{
	m_pLabelList->RemoveAll();
	int nIndex = m_pLabelList->AddItem("最近联系人");
	m_pLabelList->SetItemData(nIndex, -1);

	for ( int i=0; i<g_GroupInfo.count; i++ )
	{
		int nIndex = m_pLabelList->AddItem(g_GroupInfo.group[i].name);
		if(nIndex != -1)
		{
			m_pLabelList->SetItemData(nIndex, g_GroupInfo.group[i].group_index);
		}
	}
	m_pLabelList->SetCurSel(0);
}

void CMsgDlg::InitClusterTree()
{
	m_pLabelList->RemoveAll();
	m_pLabelList->AddItem("群");
	m_pLabelList->SetCurSel(0);
}

void CMsgDlg::InitSystemTree()
{
	m_pLabelList->RemoveAll();
	for(int i=0; i< m_nSysTypeCnt; i++)
	{
		int nIndex = m_pLabelList->AddItem(m_strSysMsgNames[i]);
		if(nIndex != -1)
		{
			m_pLabelList->SetItemData(nIndex, i);
		}
	}
	m_pLabelList->SetCurSel(0);
}

void CMsgDlg::SetDefaultCluster(uint32 cid)
{
	m_pMainTab->SetCurSel(CUIMsgManageMainTab::en_sel_cluster);

	for(int i = 0; i < m_pImgList->GetItemCount(); ++i)
	{
		CLUSTER_INFO *pCluster = (CLUSTER_INFO*)(m_pImgList->GetItemData(i));
		if(pCluster->nFamilyID == cid)
		{
			m_pImgList->SetCurSel(i);
			break;
		}
	}
}
void CMsgDlg::SetDefaultFriend(uint32 fid)
{
	FRIEND_INFO *pFriend = FindFriend(fid);
	if(pFriend == NULL)
		return;

	BOOL bFind = FALSE;
	m_pMainTab->SetCurSel(CUIMsgManageMainTab::en_sel_friend);
	for(int i = 0; i < m_pLabelList->GetItemCount(); ++i)
	{
		if(pFriend->group_index == m_pLabelList->GetItemData(i))
		{
			m_pLabelList->SetCurSel(i);
			bFind = TRUE;
			break;
		}
	}
	
	if(!bFind)
		return;

	for(int i = 0; i < m_pImgList->GetItemCount(); ++i)
	{
		FRIEND_INFO *pTempFriend = (FRIEND_INFO*)(m_pImgList->GetItemData(i));

		if(pTempFriend->GetId() == pFriend->GetId())
		{
			m_pImgList->SetCurSel(i);
			break;
		}
	}
}

void CMsgDlg::SetOrderPage(int nOrderType)
{
	m_pMainTab->SetCurSel(CUIMsgManageMainTab::en_sel_sysmsg);
	SetSysMsgTabText();
	if(nOrderType == 3)
	{
		m_pLabelList->SetCurSel(5);
	}
	else
	{
		m_pLabelList->SetCurSel(3);
	}
}

void CMsgDlg::SetSystemMsgPage()
{
	m_pMainTab->SetCurSel(CUIMsgManageMainTab::en_sel_sysmsg);
	m_pLabelList->SetCurSel(2);
}

BOOL CMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case 0x0D:
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMsgDlg::AddSystemMsg(const SYSTEM_MSG &msg)
{
	if ( m_edtReHistory.m_hWnd==NULL )
	{
		return ;
	}

	m_edtReHistory.SetSel(-1,-1);
	CHARRANGE cr;

	XT_FONT xf;

	CString strTime;

	CTime  tm(msg.dwTime);
	strTime=tm.Format("%Y.%m.%d %H:%M:%S");

	//好友发给我的信息
	xf.fontColor=RGB(74,123,211);


	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	m_edtReHistory.SetStartIndent(80);
	m_edtReHistory.InsertText(strTime);
	m_edtReHistory.InsertText("\n");

	//m_edtReHistory.InsertLine();

	//m_edtReHistory.InsertText("\n");


	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.GetSel(cr);

	XT_FONT   xfMsg;

	xfMsg.fontColor = RGB(122,100,100);


	CStringList sl;

	m_edtReHistory.SetStartIndent(300);
	m_edtReHistory.InsertText(msg.szMessage);

	//m_edtReHistory.InsertText("\n");
	m_edtReHistory.InsertText("\n");

	m_edtReHistory.SetSel(cr.cpMax,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
}

void CMsgDlg::AddClusterSystemMsg(const CLUSTER_SYSTEM_MSG &msg)
{
	if ( m_edtReHistory.m_hWnd==NULL )
	{
		return ;
	}

	m_edtReHistory.SetSel(-1,-1);
	CHARRANGE cr;

	XT_FONT xf;

	CString strTime;

	CTime  tm(msg.dwTime);
	strTime=tm.Format("%Y.%m.%d %H:%M:%S");

	//好友发给我的信息
	xf.fontColor=RGB(74,123,211);


	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	m_edtReHistory.SetStartIndent(80);
	m_edtReHistory.InsertText(strTime);
	m_edtReHistory.InsertText("\n");

	//m_edtReHistory.InsertLine();

	//m_edtReHistory.InsertText("\n");


	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.GetSel(cr);

	XT_FONT   xfMsg;

	xfMsg.fontColor = RGB(122,100,100);


	CStringList sl;

	m_edtReHistory.SetStartIndent(300);
	CString str = msg.szMessage;
	if (str.Find("邀请您加入群") != -1)
	{
		//CString strLink;
		//strLink.Format("【%d】", msg.nInviteId);
		//m_edtReHistory.InsertLink(strLink);

		//m_edtReHistory.InsertText("邀请您加入群");
		//strLink.Format("【%d】", msg.nClusterId);
		//m_edtReHistory.InsertLink(strLink);


		FRIEND_CLUSTER_LINK friendLinkInfo;
		friendLinkInfo.nType = 1;
		friendLinkInfo.nId = msg.nInviteId;
		strcpy(friendLinkInfo.szName, msg.szNickName);

		FRIEND_INFO* pFriend = FindFriend(msg.nInviteId);
		friendLinkInfo.friendInfo = pFriend;
		if(pFriend == NULL)
		{
			m_bSearch = TRUE;
			XT_XTREAM_SEARCH_REQ req;
			req.startpos = 0;
			req.search_type = 1;
			req.count = 0;
			req.id = msg.nInviteId;	
			g_pProtocol->SendReq(req);
		}
		m_edtReHistory.InsertFriendClusterLink(friendLinkInfo);
		

		m_edtReHistory.InsertText("邀请您加入群");


		FRIEND_CLUSTER_LINK clusterLinkInfo;
		clusterLinkInfo.nType = 2;
		clusterLinkInfo.nId = msg.nClusterId;
		strcpy(clusterLinkInfo.szName, msg.szClusterName);
		m_edtReHistory.InsertFriendClusterLink(clusterLinkInfo);

		XT_CLUSTER_SEARCH_REQ req;
		req.start_pos = 0;
		req.search_type = 0;
		req.nCount = 0;
		req.type_data.nFamilyID = msg.nClusterId;
		//m_nClusterId = msg.nClusterId;
		g_pProtocol->SendReq(req);
	}
	else if (str.Find("申请加入群") != -1)
	{
		//CString strLink;
		//strLink.Format("【%d】", msg.nInviteId);
		//m_edtReHistory.InsertLink(strLink);

		//m_edtReHistory.InsertText("邀请您加入群");
		//strLink.Format("【%d】", msg.nClusterId);
		//m_edtReHistory.InsertLink(strLink);


		FRIEND_CLUSTER_LINK friendLinkInfo;
		friendLinkInfo.nType = 1;
		friendLinkInfo.nId = msg.nInviteId;
		strcpy(friendLinkInfo.szName, msg.szNickName);
		FRIEND_INFO* pFriend = FindFriend(msg.nInviteId);
		friendLinkInfo.friendInfo = pFriend;
		if(pFriend == NULL)
		{
			m_bSearch = TRUE;
			XT_XTREAM_SEARCH_REQ req;
			req.startpos = 0;
			req.search_type = 1;
			req.count = 0;
			req.id = msg.nInviteId;	
			g_pProtocol->SendReq(req);
		}

		m_edtReHistory.InsertFriendClusterLink(friendLinkInfo);

		m_edtReHistory.InsertText("申请加入群");

		//FRIEND_CLUSTER_LINK clusterLinkInfo;
		//clusterLinkInfo.nType = 2;
		//clusterLinkInfo.nId = msg.nClusterId;

		FRIEND_CLUSTER_LINK clusterLinkInfo;
		clusterLinkInfo.nType = 2;
		clusterLinkInfo.nId = msg.nClusterId;
		strcpy(clusterLinkInfo.szName, msg.szClusterName);
		m_edtReHistory.InsertFriendClusterLink(clusterLinkInfo);

		XT_CLUSTER_SEARCH_REQ req;
		req.start_pos = 0;
		req.search_type = 0;
		req.nCount = 0;
		req.type_data.nFamilyID = msg.nClusterId;
		//m_nClusterId = msg.nClusterId;
		g_pProtocol->SendReq(req);
	}
	else
		m_edtReHistory.InsertText(msg.szMessage);
	//m_edtReHistory.SetRawHyperText(_T("<%$link$#100#%>"));	

	//m_edtReHistory.InsertText("\n");
	m_edtReHistory.InsertText("\n");

	m_edtReHistory.SetSel(cr.cpMax,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
}

// 查找用户
void CMsgDlg::OnUserSearchAck(const char *data, int nRetCode)
{
	if(!m_bSearch)
		return;
	m_bSearch = FALSE;
	if ( nRetCode==0 )
	{
		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);

		int i;
		uint16	next_pos;
		uint32	total;
		int		nTotal;
		uint8	count;
		uint8	realCount = 0;
		rp>>count>>next_pos>>total;
		// 总在线人数
		nTotal = total;

		for ( i=0; i<count; i++ )
		{
			FRIEND_INFO* friendinfo = new FRIEND_INFO;

			uint32 id = 0;
			rp  >>id;
			friendinfo->SetId(id);
			rp	>>friendinfo->sex
				>>friendinfo->status
				>>friendinfo->rule;
			char username[MAX_USERNAME_LEN+1] ;
			rp	>>username;
			friendinfo->SetUserName(username);
			rp	>>friendinfo->nickname;

			for(int nLink = 0; nLink < m_edtReHistory.m_vecFriendClusterLink.size(); nLink++)
			{
				if(m_edtReHistory.m_vecFriendClusterLink[nLink].nId == friendinfo->GetId())
					m_edtReHistory.m_vecFriendClusterLink[nLink].friendInfo = friendinfo;

			}
		}

	}
	else
	{	

	}
}

void CMsgDlg::OnClusterSearchAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_CLUSTER_SEARCH_ACK ack = (LPXT_CLUSTER_SEARCH_ACK)data;
		if(ack->nCount == 0)
		{
			FRIEND_CLUSTER_LINK clusterLinkInfo;
			clusterLinkInfo.nType = 2;
			//clusterLinkInfo.nId = m_nClusterId;
		}
		else
		{
			for ( int i=0; i<ack->nCount; i++ )
			{
				XT_CLUSTER_INFO * pInfo = new XT_CLUSTER_INFO();

				pInfo->nFamilyID  = ack->FamilySearchData[i].nFamilyID;
				pInfo->nCreatorID = ack->FamilySearchData[i].nCreaterID;
				pInfo->nJoinFlag  = ack->FamilySearchData[i].nJoinFlag;
				pInfo->nFamilyClass  = ack->FamilySearchData[i].nFamilyClass;
				strcpy(pInfo->szFamilyName,ack->FamilySearchData[i].szFamilyName);
				strcpy(pInfo->szFamilyDomain,ack->FamilySearchData[i].szFamilyDomain);

				FRIEND_CLUSTER_LINK clusterLinkInfo;
				clusterLinkInfo.nType = 2;
				//clusterLinkInfo.nId = m_nClusterId;
				clusterLinkInfo.clusterInfo = pInfo;
				for(int nLink = 0; nLink < m_edtReHistory.m_vecFriendClusterLink.size(); nLink++)
				{
					if(m_edtReHistory.m_vecFriendClusterLink[nLink].nId == pInfo->nFamilyID)
						m_edtReHistory.m_vecFriendClusterLink[nLink].clusterInfo = pInfo;
				}
			}
		}
	}	
}

void CMsgDlg::AddP2PTalkMsg(const XT_MSG &msg)
{
	if ( m_edtReHistory.m_hWnd==NULL )
	{
		return ;
	}

	CString sObj;		// 聊天对象
	CString sAction0;	// 聊天动作0
	CString sAction1;	// 聊天动作1
	CString sAction2;	// 聊天动作2
	CString	sMsg;		// 聊天信息
	bool	bHasAction;	// 消息中是否包含动作信息
	CHARRANGE cr;
	XT_FONT xf;
	// 处理动作消息
	bHasAction=ParseMsg(msg.data,sMsg,sObj,sAction0,sAction1,sAction2);

	CString strTime;
	CTime  tm(msg.send_time);
	strTime=tm.Format(" (%Y.%m.%d  %H:%M:%S)");

	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_edtReHistory.InsertText("\n");
	// 好友发给我的信息
	if ( msg.from_id!=g_ImUserInfo.GetId() )
	{
		xf.fontColor=RGB(96,92,168);
	}
	else
	{
		xf.fontColor=RGB(74,123,211);
	}
	m_edtReHistory.SetStartIndent(80);
	// 打印出消息标题（发话人+动作+对象+日期）
	if(bHasAction)
	{
		xf.fontColor=CLR_ACTIONS;
		m_edtReHistory.InsertText(sAction0,xf);
		xf.fontColor= (msg.from_id == g_ImUserInfo.GetId() ?  RGB(0,128,64) : RGB(22,68,223));
		CString strName = msg.from_nickname;
		FRIEND_INFO *pFriend = ::FindFriend(msg.from_id);
		if(pFriend != NULL)
		{
			strName = UserInfoToStrEx(pFriend);
		}
		m_edtReHistory.InsertText(strName,xf);
		xf.fontColor=CLR_ACTIONS;
		m_edtReHistory.InsertText(sAction1,xf);
		xf.fontColor=CLR_OBJECT;
		m_edtReHistory.InsertText(sObj,xf);
		xf.fontColor=CLR_ACTIONS;
		m_edtReHistory.InsertText(sAction2,xf);
	}	
	else
	{
		xf.fontColor= (msg.from_id == g_ImUserInfo.GetId() ?  RGB(0,128,64) : RGB(22,68,223));
		CString strName = msg.from_nickname;
		FRIEND_INFO *pFriend = ::FindFriend(msg.from_id);
		if(pFriend != NULL)
		{
			strName = UserInfoToStrEx(pFriend);
		}
		m_edtReHistory.InsertText(strName,xf);
	}
	xf.fontColor=RGB(122,122,122);
	m_edtReHistory.InsertText(strTime,xf);
	m_edtReHistory.InsertText(":\n");
	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.GetSel(cr);	

	XT_FONT   xfMsg;
	xfMsg.flags     = msg.fontStyle;
	xfMsg.fontColor = msg.fontColor;

	strcpy(xfMsg.fontName,msg.fontName);
	xfMsg.fontSize  = msg.fontSize;

	CStringList sl;

	m_edtReHistory.SetStartIndent(300);
	if(bHasAction)
		m_edtReHistory.InsertText(sMsg);
	else
	{
		m_edtReHistory.InsertText(msg.data);

		CString strData = msg.data;
		if(strData.Find("文件 \"") != -1 && strData.Find("\" 接收完成。") != -1)
		{
			CHARRANGE cr;

			m_edtReHistory.SetSel(-1,-1);
			m_edtReHistory.GetSel(cr);

			m_edtReHistory.InsertText("\n");
			m_edtReHistory.SetStartIndent(230);

			XT_FONT xf;
			m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_edtReHistory.InsertLink("打开文件");
			m_edtReHistory.InsertText(" / ");
			m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_edtReHistory.InsertLink("打开文件夹");
		}
	}

	m_edtReHistory.SetSel(cr.cpMax,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_edtReHistory.SetSel(-1,-1);
	m_edtReHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
}

void CMsgDlg::OnBnClickedBtnLast()
{
	if ( m_iPage>=(m_iPageCount-1) )
		return ;

	m_iPage=m_iPageCount-1;
	ShowPage(m_iPage);

	m_btnLast.EnableWindow(false);
	m_btnNext.EnableWindow(false);

	if ( m_iPageCount>1 )
	{
		m_btnFirst.EnableWindow(true);
		m_btnPrev.EnableWindow(true);
		m_btnFirst.Invalidate();
		m_btnPrev.Invalidate();
	}
}

void CMsgDlg::OnBnClickedBtnNext()
{
	if ( m_iPage>=(m_iPageCount-1) )
		return ;

	m_iPage++;
	ShowPage(m_iPage);

	if ( m_iPage>=(m_iPageCount-1) )
	{
		m_btnLast.EnableWindow(false);
		m_btnNext.EnableWindow(false);
	}

	if ( m_iPageCount>1 )
	{
		m_btnFirst.EnableWindow(true);
		m_btnPrev.EnableWindow(true);
		m_btnFirst.Invalidate();
		m_btnPrev.Invalidate();
	}	
}
//
//void CMsgDlg::OnBnClickedBtnExport()
//{
//	CFileDialog dlg(false,"txt","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"文本文件(*.txt)|*.txt||",this);
//
//	if ( dlg.DoModal() )
//	{
//		CString strFile = dlg.GetPathName();
//
//		DeleteFile(strFile);
//
//		CStdioFile file;
//
//		if ( file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
//		{
//
//			file.WriteString("--------------------- www.feiniu.com ---------------------\n");
//
//			HTREEITEM hItem = m_wndTree.GetSelectedItem();
//			bool bSysMsg=false;
//
//			if ( hItem )
//			{
//				DWORD dwData = m_wndTree.GetItemData(hItem);
//
//				HTREEITEM hParent = m_wndTree.GetParentItem(hItem);
//
//				if (hParent==NULL)
//				{
//					if ( dwData==TREE_SYSMSG )//系统消息
//					{
//						bSysMsg=true;
//					}
//				}
//			}
//
//			if ( bSysMsg )
//			{
//				for ( int i=0; i<m_systemMsgList.GetCount(); i++ )
//				{
//					POSITION pos = m_systemMsgList.FindIndex(i);
//
//					SYSTEM_MSG &msg = m_systemMsgList.GetAt(pos);
//
//					CString strTime;
//
//					CTime  tm(msg.dwTime);
//					strTime=tm.Format("%Y.%m.%d \\ %H:%M:%S");
//
//					file.WriteString("\n");
//					file.WriteString("---------------------");
//					file.WriteString("\n");
//
//
//					file.WriteString(msg.szMessage);
//					file.WriteString("\n");
//					file.WriteString("\n");
//				}
//			}
//			else
//			{
//				for ( int i=0; i<m_mlRecord.GetCount(); i++ )
//				{
//					POSITION pos = m_mlRecord.FindIndex(i);
//
//					XT_MSG & msg=m_mlRecord.GetAt(pos);
//
//					CString strTitle,strTime;
//
//					CTime  tm(msg.send_time);
//					strTime=tm.Format("%Y.%m.%d \\ %H:%M:%S");
//
//					//好友发给我的信息
//					if ( msg.from_id!=g_ImUserInfo.GetId() )
//					{
//						//FRIEND_INFO *pFriend =::FindFriend(msg.from_id);	
//
//						//if ( pFriend )
//						//{
//						//	strTitle.Format("%s %s",UserInfoToStr(pFriend),strTime);	
//						//}
//						//else
//						//{
//						//	strTitle.Format("%u %s",msg.from_id,strTime);
//						//}
//						strTitle.Format("%s %s",msg.from_nickname,strTime);	
//
//					}
//					else
//					{
//						strTitle.Format("%s %s",UserInfoToStr(&g_ImUserInfo),strTime);	
//					}
//
//
//					file.WriteString("\n");
//					file.WriteString(strTitle);
//					file.WriteString("\n");
//
//
//
//					file.WriteString("\t");
//					file.WriteString(msg.data);
//					file.WriteString("\n");
//					file.WriteString("\n");
//				}
//
//			}
//
//			file.Close();
//
//			ShellExecute(NULL,"open","notepad.exe",strFile,"",SW_SHOW);
//		}
//	}		
//}

//void CMsgDlg::OnBnClickedBtnDel()
//{
//	HTREEITEM hSel = m_wndTree.GetSelectedItem();
//
//	if ( hSel )
//	{
//		HTREEITEM hParent = m_wndTree.GetParentItem(hSel);
//
//		if ( hParent )
//		{
//			DWORD dwData = m_wndTree.GetItemData(hParent);
//
//			if ( dwData==TREE_GROUP )//好友组
//			{
//				FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndTree.GetItemData(hSel);
//
//				CString str;
//				str.Format("确定要清空你和 %s 的聊天记录吗？",pFriend->nickname);
//				if (MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)==IDYES )
//				{
//					g_LocalData.DeleteP2PTalkFile(pFriend->id);
//					m_mlRecord.RemoveAll();
//					InitMsg();
//				}
//			}
//			else
//			if ( dwData==TREE_CLUSTER )//群
//			{
//				CLUSTER_INFO * pCluster = (CLUSTER_INFO *)m_wndTree.GetItemData(hSel);
//				
//				CString str;
//				str.Format("确定要清空群 %s 中的聊天记录吗？",pCluster->szFamilyName);
//				if (MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)==IDYES )
//				{
//					g_LocalData.DeleteClusterTalkFile(pCluster->nFamilyID);
//					m_mlRecord.RemoveAll();
//					InitMsg();
//				}
//			}			
//		}
//		else
//		{
//			if ( m_wndTree.GetItemData(hSel)==TREE_SYSMSG )
//			{
//				if (MessageBox("确定要清空系统消息吗？",IM_CAPTION,MB_YESNO|MB_ICONWARNING)==IDYES )
//				{
//					g_LocalData.ClearSystemMessage(g_ImUserInfo.GetId());
//					LoadSystemMessage();
//				}
//			}
//		}
//
//	}
//}

void CMsgDlg::OnLoginOut(const char *data, WORD wRet)
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSE, 0, 0);
}

void CMsgDlg::ShowFirstPage()
{
	m_iPage=0;
	ShowPage(m_iPage);

	m_btnFirst.EnableWindow(false);
	m_btnPrev.EnableWindow(false);

	if ( m_iPageCount>1 )
	{
		m_btnLast.EnableWindow(true);
		m_btnNext.EnableWindow(true);

		m_btnLast.Invalidate();
		m_btnNext.Invalidate();
	}
}

void CMsgDlg::OnBnClickedBtnFirst()
{
	if ( m_iPage<1 )
		return ;
	else
	{
		m_iPage=0;
		ShowPage(m_iPage);

		m_btnFirst.EnableWindow(false);
		m_btnPrev.EnableWindow(false);

		if ( m_iPageCount>1 )
		{
			m_btnLast.EnableWindow(true);
			m_btnNext.EnableWindow(true);

			m_btnLast.Invalidate();
			m_btnNext.Invalidate();
		}
	}
}

void CMsgDlg::OnBnClickedButtonPrev()
{
	if ( m_iPage<1 )
		return ;
	else
	{
		m_iPage--;
		ShowPage(m_iPage);

		if ( m_iPage<1 )
		{
			m_btnFirst.EnableWindow(false);
			m_btnPrev.EnableWindow(false);
		}

		if ( m_iPageCount>1 )
		{
			m_btnLast.EnableWindow(true);
			m_btnNext.EnableWindow(true);

			m_btnLast.Invalidate();
			m_btnNext.Invalidate();
		}
	}
}

void CMsgDlg::ShowPage(int iPage)
{
	//页数显示
	CString str;
	str.Format("%d/%d 页",iPage+1,m_iPageCount);
	//m_lblInfo.SetWindowText(str,"");

	m_edtReHistory.ClearMsg();

	if ( iPage < 0 || iPage >= m_iPageCount )
		return ;

	if ( m_iMessageType == 0 )
	{
		for ( int i=0; i<PAGE_COUNT; i++ )
		{
			POSITION pos = m_mlRecord.FindIndex(i+iPage*PAGE_COUNT);

			if ( pos==NULL )
				break;
			else
			{
				this->AddP2PTalkMsg(m_mlRecord.GetAt(pos));
			}
		}
	}
	else if ( m_iMessageType == 1 ) //system message
	{

		/*添加到显示*/
		for ( int i=0; i<PAGE_COUNT; i++ )
		{
			POSITION pos = m_mlSysMsgShow.FindIndex(i + iPage*PAGE_COUNT);

			if( pos == NULL )
				break;
			else
			{
				this->AddSystemMsg(m_mlSysMsgShow.GetAt(pos));
			}
		}
	}
	else if ( m_iMessageType == 2 ) //cluster system message
	{
#ifdef _CS_PLAT_EX_
		m_pTranMsg->LoadTranMsg(iPage);
#else
		/*添加到显示*/
		m_edtReHistory.ClearFriendClusterLink();
		for ( int i=0; i<PAGE_COUNT; i++ )
		{
			POSITION pos = m_mlClusterSysMsgShow.FindIndex(i + iPage*PAGE_COUNT);



			if( pos == NULL )
				break;
			else
			{
				this->AddClusterSystemMsg(m_mlClusterSysMsgShow.GetAt(pos));
			}
		}
#endif
	}
	else if ( m_iMessageType == 3)
	{
		m_pOrderTreeOrder->LoadOrderMsg(iPage);
	}
	else if ( m_iMessageType == 4)
	{
		m_pOrderTreeOrder->LoadOrderMsg(iPage, FALSE);
	}

	m_edtReHistory.ScrollToBottom();
}


void CMsgDlg::AdjustPageBtn()
{
	if(m_iPage == 0)
	{
		m_btnFirst.EnableWindow(false);
		m_btnPrev.EnableWindow(false);
	}
	//只有一页
	if ( m_iPageCount < 2 )
	{
		m_btnNext.EnableWindow(false);
		m_btnLast.EnableWindow(false);
	}
	else
	{
		m_btnNext.EnableWindow(true);
		m_btnLast.EnableWindow(true);
	}
}

void CMsgDlg::SetSysMsgTabText()
{
	CString strUnReadCount;
	int nCountTrade = g_LocalData.GetOrderTradeUnReadCount();
	int nCountReturn = g_LocalData.GetOrderReturnUnReadCount();
	int nCount = nCountTrade + nCountReturn;
	if(nCount == 0)
		strUnReadCount= "系统消息";
	else
		strUnReadCount.Format("系统消息(%d)", nCount);

	m_pMainTab->SetSysMsgTabText(strUnReadCount);

	if( m_pMainTab != NULL && m_pMainTab->GetCurSel() == CUIMsgManageMainTab::en_sel_sysmsg && m_pLabelList != NULL)
	{
		DuiLib::CListContainerElementUI* pElementTrade = m_pLabelList->GetItem(3);
		if(pElementTrade != NULL)
		{
			if(nCountTrade == 0)
				strUnReadCount= "交易消息";
			else
				strUnReadCount.Format("交易消息(%d)", nCountTrade);
			pElementTrade->SetText(strUnReadCount);
		}
		DuiLib::CListContainerElementUI* pElementReturn = m_pLabelList->GetItem(5);
		if(pElementReturn != NULL)
		{
			if(nCountReturn == 0)
				strUnReadCount= "退换货消息";
			else
				strUnReadCount.Format("退换货消息(%d)", nCountReturn);
			pElementReturn->SetText(strUnReadCount);
		}
	}
}

void CMsgDlg::AddOrderMsg()
{
	if(m_pMainTab->GetCurSel() == CUIMsgManageMainTab::en_sel_sysmsg)
	{
		int nSel = m_pLabelList->GetCurSel();
		if(nSel == 3)
		{
			//交易消息
			m_iMessageType = 3;
			m_btnSearch.ShowWindow(SW_HIDE);
			m_edtReHistory.ShowWindow(SW_HIDE);
			m_pOrderTreeOrder->ShowWindow(true);
			m_pOrderTreeOrder->LoadOrderMsg();
			m_iPageCount = (g_LocalData.GetOrderMsgTrade().size() / 10) + 1;
			ShowFirstPage();
		}
		else if(nSel == 5)
		{
			//退货消息
			m_iMessageType = 4;
			m_btnSearch.ShowWindow(SW_HIDE);
			m_edtReHistory.ShowWindow(SW_HIDE);
			m_pOrderTreeOrder->ShowWindow(true);
			m_pOrderTreeOrder->LoadOrderMsg(0, FALSE);
			m_iPageCount = (g_LocalData.GetOrderMsgTrade().size() / 10) + 1;
			ShowFirstPage();
		}
	}
}

void CMsgDlg::LoadSystemMsgAll(const CString& sKeyword,MSG_LIST &rstList,CTime tMsgBegin)
{
	CUIMsgManageMainTab::EN_SEL enSel = m_pMainTab->GetCurSel();
	if(enSel != CUIMsgManageMainTab::en_sel_sysmsg)
		return;

	g_LocalData.GetSystemMsgList(g_ImUserInfo.GetId(), m_systemMsgList);

	int nAllSysMsgCnt = 0, nSysMsgShowCnt = 0;
	nAllSysMsgCnt = m_systemMsgList.GetCount();
	m_mlSysMsgShow.RemoveAll();
	for ( int i=0; i < nAllSysMsgCnt; i++ )
	{
		POSITION posMsg = m_systemMsgList.FindIndex(i);
		if(posMsg == NULL)
			break;
		
		SYSTEM_MSG mSysMsg =  m_systemMsgList.GetAt(posMsg);
		if(mSysMsg.dwTime > tMsgBegin.GetTime())////
		{
		    if(SubstrExist(mSysMsg.szMessage,-1,sKeyword))///将系统消息构造成聊天消息供查询显示
			{
               XT_MSG *msg = new XT_MSG();
			   strcpy(msg->fontName,_T("微软雅黑"));//字体类型
			   msg->fontSize  = 10;                  //字体大小
			   msg->data_type = mSysMsg.iType;  //系统消息类型
			   msg->send_time = mSysMsg.dwTime;///发送时间
			   strcpy(msg->data,mSysMsg.szMessage);//发送内容
			   rstList.AddTail(*msg);////加入到系统中
			   delete msg;
			   nSysMsgShowCnt++;
			}	
		}
	}

	/*算页数*/
	m_iPage = 0;
	m_iPageCount = ceil(nSysMsgShowCnt * 1.0 / PAGE_COUNT);
	AdjustPageBtn();
	

	//显示系统消息
	m_iMessageType = 1;
	ShowPage(m_iPage);	
}

void CMsgDlg::LoadSystemMessage()
{
	CUIMsgManageMainTab::EN_SEL enSel = m_pMainTab->GetCurSel();
	int nSel = m_pLabelList->GetCurSel();
	if(enSel != CUIMsgManageMainTab::en_sel_sysmsg || nSel == -1)
		return;

	g_LocalData.GetSystemMsgList(g_ImUserInfo.GetId(), m_systemMsgList);

	int nAllSysMsgCnt = 0, nSysMsgShowCnt = 0;
	nAllSysMsgCnt = m_systemMsgList.GetCount();
	m_mlSysMsgShow.RemoveAll();
	for ( int i=0; i < nAllSysMsgCnt; i++ )
	{
		POSITION posMsg = m_systemMsgList.FindIndex(i);
		if(posMsg == NULL)
			break;

		SYSTEM_MSG mSysMsg =  m_systemMsgList.GetAt(posMsg);
		if(mSysMsg.iType == nSel)
		{
			m_mlSysMsgShow.AddTail(mSysMsg);
			nSysMsgShowCnt++;
		}
	}

	/*算页数*/
	m_iPage = 0;
	m_iPageCount = ceil(nSysMsgShowCnt * 1.0 / PAGE_COUNT);
	AdjustPageBtn();


	//显示系统消息
	m_iMessageType = 1;
	ShowPage(m_iPage);	
}

void CMsgDlg::LoadClusterSystemMessage()
{
	CUIMsgManageMainTab::EN_SEL enSel = m_pMainTab->GetCurSel();
	if(enSel != CUIMsgManageMainTab::en_sel_sysmsg)
		return;

	g_LocalData.GetClusterSystemMsgList(g_ImUserInfo.GetId(), m_clusterSystemMsgList);

	int nAllSysMsgCnt = 0, nSysMsgShowCnt = 0;
	nAllSysMsgCnt = m_clusterSystemMsgList.GetCount();
	m_mlClusterSysMsgShow.RemoveAll();
	for ( int i=0; i < nAllSysMsgCnt; i++ )
	{
		POSITION posMsg = m_clusterSystemMsgList.FindIndex(i);
		if(posMsg == NULL)
			break;

		CLUSTER_SYSTEM_MSG mSysMsg =  m_clusterSystemMsgList.GetAt(posMsg);
		m_mlClusterSysMsgShow.AddTail(mSysMsg);
		nSysMsgShowCnt++;
	}

	/*算页数*/
	m_iPage = 0;
	m_iPageCount = ceil(nSysMsgShowCnt * 1.0 / PAGE_COUNT);
	AdjustPageBtn();


	//显示群系统消息
	m_iMessageType = 2;
	ShowPage(m_iPage);
}

void CMsgDlg::InitMsg()
{
	int nCount = m_mlRecord.GetCount();

	m_iMessageType = 0;

	m_iPage = 0;

    m_iPageCount = nCount/PAGE_COUNT;

	if ( (nCount % PAGE_COUNT)!=0 )
		m_iPageCount++;

	m_btnFirst.EnableWindow(false);
	m_btnPrev.EnableWindow(false);

	if ( m_iPageCount <2 )//只有一页
	{
		m_btnNext.EnableWindow(false);
		m_btnLast.EnableWindow(false);
	}
	else
	{
		m_btnNext.EnableWindow(true);
		m_btnLast.EnableWindow(true);
	}

	ShowPage(m_iPage);	
}


void CMsgDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01)
	{
		KillTimer(0x01);
	}

	CDialog::OnTimer(nIDEvent);
}

void CMsgDlg::OnPaint()
{
	CPaintDC dc(this); 
	CDialog::OnPaint();

	CRect rc;

	if ( this->m_edtReHistory.m_hWnd )
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		m_edtReHistory.GetWindowRect(&rc);
		ScreenToClient(&rc);

		/*rc.InflateRect(1,1);
		rc.right+=1;
		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);*/

		// 列表下方的按钮栏
		CRect rcBottomBar;
		rcBottomBar.left = rc.left;
		rcBottomBar.right = rc.right;
		rcBottomBar.top = rc.bottom;
		rcBottomBar.bottom = rcClient.bottom-1;
		dc.FillSolidRect(&rcBottomBar, 0x00EBEBEB);
	}
}

void CMsgDlg::OnBnClickedBtnFind()
{
	m_mlRecord.RemoveAll();
	m_systemMsgList.RemoveAll();

	UINT id = 0;

	CUIMsgManageMainTab::EN_SEL enSel = m_pMainTab->GetCurSel();
	if(enSel == CUIMsgManageMainTab::en_sel_friend)
	{
		id = m_nSearchFriendId;
		if(id == 0)
			return;
	}
	else if(enSel == CUIMsgManageMainTab::en_sel_cluster)
	{
		id = m_nSearchClusterId;
		if(id == 0)
			return;
	}
	else if(enSel == CUIMsgManageMainTab::en_sel_sysmsg)
	{
	}
	else
	{

	}

	int nIdx=0;
	int i=0;
	CString sKeyword;
	CString sRange;
	m_comboRange.GetWindowText(sRange);
	if(sRange.GetLength()==0)
	{
		//MessageBox("请选择要查询的范围");
		CMessageBox::Prompt("请选择要查询的范围");
		return;
	}
	m_editKeyword.GetWindowText(sKeyword);
	//if(sKeyword.GetLength()==0)
	//{
	//	//MessageBox("请输入要查询的文字");
	//	CMessageBox::Prompt("请输入要查询的文字");
	//	return;
	//}
	nIdx=m_comboRange.GetCurSel();
	if(nIdx<0)
		return;

	FINDRANGE_ITEMDATA ;
	FRIEND_LIST::iterator it;
	MSG_LIST rstList;
	int nPos =(int)m_comboRange.GetItemData(nIdx);
	ASSERT(nPos>=0);
	ASSERT(nPos<m_arrItemData.size());
	//UINT id=m_arrItemData[nPos].id;


	CTime  tMsgBegin = CTime::GetCurrentTime();////时间
	switch(m_arrItemData[nPos].type)
	{
	//case EM_IT_FRIEND:
	//	FindMsg(id,CDataManager::USER,sKeyword,m_mlRecord);
	//	break;
	case EM_IT_ALL:////全部
		tMsgBegin = 0;

		break;

	case EM_IT_1MONTH:////最近一个月
		tMsgBegin -= 1*30*24*60*60;
		break;

	case EM_IT_3MONTH:////最近三个月
		tMsgBegin -= 3*30*24*60*60;
		break;
		
	case EM_IT_1YEAR:////最近一年
		tMsgBegin -= 12*30*24*60*60;
		break;

	//case EM_IT_CLUSTER:
	//	FindMsg(id,CDataManager::CLUSTER,sKeyword,m_mlRecord);
	//	break;
	default:
		;
	};

	if(enSel == CUIMsgManageMainTab::en_sel_friend)
	{
		FindMsg(id,CDataManager::USER,sKeyword,rstList,tMsgBegin);
		m_mlRecord.AddTail(&rstList);
	}
	else if(enSel == CUIMsgManageMainTab::en_sel_cluster)
	{
		FindMsg(id,CDataManager::CLUSTER,sKeyword,rstList,tMsgBegin);
		m_mlRecord.AddTail(&rstList);
	}
	else if(enSel == CUIMsgManageMainTab::en_sel_sysmsg)
	{
		LoadSystemMsgAll(sKeyword,rstList,tMsgBegin);
		m_mlRecord.AddTail(&rstList);
	}
	else
	{

	}

	//for(it=g_lsFriend.begin();it!=g_lsFriend.end();it++)
	//{
	//	FindMsg((*it)->GetId(),CDataManager::USER,sKeyword,rstList,tMsgBegin);
	//	m_mlRecord.AddTail(&rstList);
	//}
	//for(i=0;i<g_vecCluster.size();i++)
	//{
	//	FindMsg(g_vecCluster[i]->nFamilyID,CDataManager::CLUSTER,sKeyword,rstList,tMsgBegin);
	//	m_mlRecord.AddTail(&rstList);
	//}
	//////显示系统消息
	//LoadSystemMsgAll(sKeyword,rstList,tMsgBegin);
	//m_mlRecord.AddTail(&rstList);

	if(m_mlRecord.GetCount()==0)
	{
		
		CMessageBox::Prompt("没有找到相应的记录");
		InitMsg();
		return;
	}
	InitMsg();

	m_btnSearch.SetCheck(0);
	::SendMessage(GetSafeHwnd(),WM_COMMAND,IDC_BTN_SEARCH,BN_CLICKED);
}

void CMsgDlg::FindMsg(int nId,CDataManager::MSG_TYPE idType,const CString& sKeyword,MSG_LIST &rstList,CTime tMsgBegin)
{
	MSG_LIST msgList;
	POSITION pos;
	rstList.RemoveAll();
	int nCount;
	g_LocalData.GetMsgList(nId,idType,msgList);
	nCount=msgList.GetCount();
	for(int i=0;i<nCount;i++)
	{
		pos=msgList.FindIndex(i);
		ASSERT(pos);
		if(pos==NULL)
			break;
		XT_MSG &msg=msgList.GetAt(pos);
		// 只查找聊天信息
		if(msg.data_type==0 && msg.send_time > tMsgBegin.GetTime())
		{
			if(SubstrExist(msg.data,-1,sKeyword) && SubstrExist(msg.data,-1,"<image:") == NULL)
				rstList.AddTail(msg);
		}
	}
}

void CMsgDlg::InitComboRange()
{
	int nIdx=0;
	int i=0;
	FRIEND_LIST::iterator it;
	FINDRANGE_ITEMDATA itData;

	// 清空

	m_arrItemData.clear();
	for (i=0;i < m_comboRange.GetCount();i++)
	{
		m_comboRange.DeleteString( i );
	}

	//////// 添加好友名称

	//////itData.type=EM_IT_FRIEND;
	//////for ( it=g_lsFriend.begin();
	//////	it!=g_lsFriend.end();
	//////	it++ )
	//////{
	//////	CString str;
	//////	FRIEND_INFO *pFriend = (*it);
	//////	str.Format("%s(%s)",pFriend->username,pFriend->nickname);
	//////	nIdx=m_comboRange.AddString(str);
	//////	if(nIdx<0)
	//////		return;
	//////	itData.id=pFriend->id;
	//////	m_arrItemData.push_back(itData);
	//////	// 设置项数据
	//////	m_comboRange.SetItemData(nIdx,m_arrItemData.size()-1);
	//////}

	//////// 添加群名

	//////ASSERT(g_GroupInfo.count>=0);
	//////itData.type=EM_IT_CLUSTER;

	//////for(i=0;i<g_vecCluster.size();i++)
	//////{
	//////	nIdx=m_comboRange.AddString(g_vecCluster[i]->szFamilyName);
	//////	if(nIdx<0)
	//////		return;
	//////	itData.id=g_vecCluster[i]->nFamilyID;
	//////	m_arrItemData.push_back(itData);
	//////	// 设置项数据
	//////	m_comboRange.SetItemData(nIdx,m_arrItemData.size()-1);
	//////}

	//  最近一个月
	itData.type=EM_IT_1MONTH;
	m_arrItemData.push_back(itData);
	nIdx=m_comboRange.InsertString(0,"最近一个月");
	m_comboRange.SetItemData(nIdx,m_arrItemData.size()-1);

	//  最近三个月
	itData.type=EM_IT_3MONTH;
	m_arrItemData.push_back(itData);
	nIdx=m_comboRange.InsertString(1,"最近三个月");
	m_comboRange.SetItemData(nIdx,m_arrItemData.size()-1);

	//  最近一年
	itData.type=EM_IT_1YEAR;
	m_arrItemData.push_back(itData);
	nIdx=m_comboRange.InsertString(2,"最近一年");
	m_comboRange.SetItemData(nIdx,m_arrItemData.size()-1);


	// 添加"全部"项
	itData.type=EM_IT_ALL;
	m_arrItemData.push_back(itData);
	nIdx=m_comboRange.InsertString(3,"全部");
	m_comboRange.SetItemData(nIdx,m_arrItemData.size()-1);

	m_comboRange.SetCurSel(3);
}

bool CMsgDlg::SetRangeSel(FINDRANGE_ITEMDATA rit)
{
	int nC=m_comboRange.GetCount();
	int nIdx;
	ASSERT(nC==m_arrItemData.size());
	for(int i=0;i<nC;i++)
	{
		nIdx=(int)m_comboRange.GetItemData(i);
		if( (m_arrItemData[nIdx].id==rit.id) && 
			(m_arrItemData[nIdx].type==rit.type))
		{
			m_comboRange.SetCurSel(i);
			return true;
		}
	}
	return false;
}

void CMsgDlg::OnBnClickedBtnFriend()
{
	m_edtReHistory.ClearMsg();
	m_mlRecord.RemoveAll();

	m_pImgList->ShowWindow(true);
	InitFriendTree();

	AdjustUI();
}


void CMsgDlg::OnBnClickedBtnCluster()
{
	m_edtReHistory.ClearMsg();
	m_mlRecord.RemoveAll();
	m_pImgList->ShowWindow(true);
	InitClusterTree();

	AdjustUI();
}

///////系统消息，需要重新布局MSG///////////
void CMsgDlg::OnBnClickedBtnSys()
{
	m_edtReHistory.ClearMsg();
	m_mlRecord.RemoveAll();
	m_pImgList->ShowWindow(false);
	InitSystemTree();/////重新布局MSG

	AdjustUI();
	
	LoadSystemMessage();//显示系统消息
}

void CMsgDlg::OnBnClickedBtnSearch()
{
	if(m_btnSearch.GetCheck())
	{
		m_rcRichEditSmall.bottom -= 30;
		m_rcRichEditBig.bottom -= 30;

		m_btnFirst.ShowWindow(SW_HIDE);
		m_btnLast.ShowWindow(SW_HIDE);
		m_btnPrev.ShowWindow(SW_HIDE);
		m_btnNext.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_RANGE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_KEYWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_KEYWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_FIND)->ShowWindow(SW_SHOW);

		AdjustUI();
		Invalidate(FALSE);
	}
	else
	{
		m_rcRichEditSmall.bottom += 30;
		m_rcRichEditBig.bottom += 30;

		m_btnFirst.ShowWindow(SW_SHOW);
		m_btnLast.ShowWindow(SW_SHOW);
		m_btnPrev.ShowWindow(SW_SHOW);
		m_btnNext.ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_RANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KEYWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KEYWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_FIND)->ShowWindow(SW_HIDE);

		AdjustUI();
		Invalidate(FALSE);
	}
}

LRESULT CMsgDlg::OnMainTabSelChange(WPARAM wParam, LPARAM lParam)
{
	switch((CUIMsgManageMainTab::EN_SEL)wParam)
	{
	case CUIMsgManageMainTab::en_sel_friend:
		OnBnClickedBtnFriend();
		break;
	case CUIMsgManageMainTab::en_sel_cluster:
		OnBnClickedBtnCluster();
		break;
	case CUIMsgManageMainTab::en_sel_sysmsg:
		OnBnClickedBtnSys();
		break;
	}

	return 0;
}

LRESULT CMsgDlg::OnList1SelChange(WPARAM wParam, LPARAM lParam)
{
	CUIMsgManageMainTab::EN_SEL enSel = m_pMainTab->GetCurSel();
	if(enSel == CUIMsgManageMainTab::en_null)
		return 0;

	m_pImgList->RemoveAll();
	m_nSearchFriendId = 0;
	m_nSearchClusterId = 0;
	switch(enSel)
	{
	case CUIMsgManageMainTab::en_sel_friend:
		{
			m_btnSearch.ShowWindow(SW_SHOW);
			m_edtReHistory.ShowWindow(SW_SHOW);
			m_pOrderTreeOrder->ShowWindow(false);
			int nSel = m_pLabelList->GetCurSel();
			DWORD nGroupIndex = m_pLabelList->GetItemData(nSel);
			if(nGroupIndex == -1)
			{	// 最近联系人
				const FRIEND_LIST  *pFl = CMainFrame::GetInstance()->GetRecentList()->GetRecentFriend();
				FRIEND_LIST::const_iterator iter = pFl->begin();
				// 循环遍历最近联系人
				for (; iter != pFl->end(); ++iter)
				{
					FRIEND_INFO *pUser = *iter;
					FRIEND_INFO *pFriend = NULL;
					CLUSTER_INFO *pCluster = NULL;
					CDuiRecentList::GetRealValue((DWORD)pUser, pFriend, pCluster);
					if(pFriend == NULL)
						continue;

					if(pUser->nickname[0]==0)///屏蔽群消息
					{
						continue;
					}
					CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(pUser);

					int nIndex = m_pImgList->AddItem(strFaceFile, UserInfoToStr(pUser));
					m_pImgList->SetItemData(nIndex, (DWORD)pUser);
				}
			}
			else
			{
				FRIEND_LIST::iterator it;

				for (it=g_lsFriend.begin(); it!=g_lsFriend.end();it++ )////多组好友信息
				{
					CString str;
					FRIEND_INFO *pFriend = (*it);
					str.Format("%s", UserInfoToStr(pFriend));
					CString strTxt = _T("");
					if( pFriend->group_index == (uint8)nGroupIndex )
					{
						CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
						int nIndex = m_pImgList->AddItem(strFaceFile, str);
						m_pImgList->SetItemData(nIndex, (DWORD)pFriend);
					}
				}
			}
		}
		OnBnClickedBtnLast();
		break;
	case CUIMsgManageMainTab::en_sel_cluster:
		{
			m_btnSearch.ShowWindow(SW_SHOW);
			m_edtReHistory.ShowWindow(SW_SHOW);
			m_pOrderTreeOrder->ShowWindow(false);
			int nSel = m_pLabelList->GetCurSel();
			if(nSel == 0)
			{
				m_edtReHistory.ClearMsg();
				for ( int i=0; i<g_vecCluster.size(); i++ )
				{
					CLUSTER_INFO *pCluster = g_vecCluster[i];
					int nIndex = m_pImgList->AddItem("SysBtn\\icon_group.png", pCluster->szFamilyName);
					m_pImgList->SetItemData(nIndex, (DWORD)pCluster);
				}
			}
			else if(nSel == 1)
			{
				m_edtReHistory.ClearMsg();
				for ( int i=0; i<g_vecClusterExit.size(); i++ )
				{
					CLUSTER_INFO *pCluster = g_vecClusterExit[i];
					int nIndex = m_pImgList->AddItem("", pCluster->szFamilyName);
					m_pImgList->SetItemData(nIndex, (DWORD)pCluster);
				}
			}	
		}
		OnBnClickedBtnLast();
		break;
	case CUIMsgManageMainTab::en_sel_sysmsg:
		{
			m_btnSearch.ShowWindow(SW_SHOW);
			m_edtReHistory.ShowWindow(SW_SHOW);
			m_pOrderTreeOrder->ShowWindow(false);
			SetSysMsgTabText();
			int nSel = m_pLabelList->GetCurSel();
			if(nSel == 1)
			{
				LoadClusterSystemMessage();
				OnBnClickedBtnLast();
			}
#ifdef _CS_PLAT_EX_
			else if(nSel == 2)
			{
				//交易消息
				m_iMessageType = 2;
				m_btnSearch.ShowWindow(SW_HIDE);
				m_edtReHistory.ShowWindow(SW_HIDE);
				m_pTranMsg->ShowWindow(true);
				m_pTranMsg->LoadTranMsg();
				m_iPageCount = (g_LocalData.GetTranMsg().size() / PAGE_COUNT_TRANMSG) + 1;
				ShowFirstPage();
			}
#endif
			else if(nSel == 3)
			{
				//交易消息
				m_iMessageType = 3;
				m_btnSearch.ShowWindow(SW_HIDE);
				m_edtReHistory.ShowWindow(SW_HIDE);
				m_pOrderTreeOrder->ShowWindow(true);
				m_pOrderTreeOrder->LoadOrderMsg();
				m_iPageCount = (g_LocalData.GetOrderMsgTrade().size() / 10) + 1;
				ShowFirstPage();
			}
			else if(nSel == 5)
			{
				//退货消息
				m_iMessageType = 4;
				m_btnSearch.ShowWindow(SW_HIDE);
				m_edtReHistory.ShowWindow(SW_HIDE);
				m_pOrderTreeOrder->ShowWindow(true);
				m_pOrderTreeOrder->LoadOrderMsg(0, FALSE);
				m_iPageCount = (g_LocalData.GetOrderMsgTrade().size() / 10) + 1;
				ShowFirstPage();
			}
			else
			{
				LoadSystemMessage();
				OnBnClickedBtnLast();
			}
		}
		break;
	}

	if(m_pImgList->GetItemCount() == 0)
	{
		m_edtReHistory.ClearMsg();
	}
	m_pImgList->SetCurSel(0);

	return 0;
}

LRESULT CMsgDlg::OnList2SelChange(WPARAM wParam, LPARAM lParam)
{
	CUIMsgManageMainTab::EN_SEL enSel = m_pMainTab->GetCurSel();
	if(enSel == CUIMsgManageMainTab::en_null)
		return 0;

	m_nSearchFriendId = 0;
	m_nSearchClusterId = 0;

	int nIndex = m_pImgList->GetCurSel();
	DWORD dwData = m_pImgList->GetItemData(nIndex);

	switch(enSel)
	{
	case CUIMsgManageMainTab::en_sel_friend:
		{
			FRIEND_INFO * pFriend = (FRIEND_INFO *)dwData;
			m_mlRecord.RemoveAll();
			g_LocalData.GetMsgList(pFriend->GetId(),CDataManager::USER,m_mlRecord);
			m_nSearchFriendId = pFriend->GetId();
			// 更新查询范围复选框的选项
			FINDRANGE_ITEMDATA rit;
			rit.type=EM_IT_FRIEND;
			rit.id=pFriend->GetId();
			SetRangeSel(rit);
			InitMsg();
		}
		break;
	case CUIMsgManageMainTab::en_sel_cluster:
		{
			CLUSTER_INFO * pCluster = (CLUSTER_INFO *)dwData;
			m_mlRecord.RemoveAll();

			//CString strItem = m_wndChildTree.GetItemText(hItem);////选中的组名
			//if(strItem == _T("已退出的群"))
			//{
			//	pCluster->nFamilyID = 106;/////指定群号
			//}
			g_LocalData.GetMsgList(pCluster->nFamilyID,CDataManager::CLUSTER,m_mlRecord);
			m_nSearchClusterId = pCluster->nFamilyID;
			// 更新查询范围复选框的选项
			FINDRANGE_ITEMDATA rit;
			rit.type=EM_IT_CLUSTER;
			rit.id=pCluster->nFamilyID;
			SetRangeSel(rit);
			InitMsg();
		}
		break;
	case CUIMsgManageMainTab::en_sel_sysmsg:
		{
			this->m_edtReHistory.ClearMsg();
			LoadSystemMessage();
		}
		break;
	}
	OnBnClickedBtnLast();
	return 0;
}

HBRUSH CMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nId = pWnd->GetDlgCtrlID();
	if(nId == IDC_STATIC_RANGE
		|| nId == IDC_STATIC_KEYWORD
		|| nId == IDC_EDIT_KEYWORD
		|| nId == IDC_COMBO1)
	{
		pDC->SetTextColor(0x2e2e2e);
	}

	return hbr;
}

void CMsgDlg::SetOrderMsgRead(OrderMsg* pOrderMsg)
{
	if(m_pOrderTreeOrder != NULL && m_pOrderTreeOrder->GetHWND() != NULL)
	{
		m_pOrderTreeOrder->SetOrderMsgRead(pOrderMsg);
	}
}

void CMsgDlg::SetAllOrderMsgRead()
{
	if(m_pOrderTreeOrder != NULL && m_pOrderTreeOrder->GetHWND() != NULL)
	{
		m_pOrderTreeOrder->SetAllOrderMsgRead();
	}
}

DuiLib::UILIB_RESOURCETYPE CUIMsgManageMainTab::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CUIMsgManageMainTab::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::UILIB_RESOURCETYPE CDuiOrderTree::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiOrderTree::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CUIMsgManageMainTab::Notify(DuiLib::TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == "selectchanged")
	{
		if(msg.pSender->GetName() == "friendBtn")
			::SendMessage(::GetParent(m_hWnd), WM_MAINTAB_SEL_CHANGE, en_sel_friend, 0);
		if(msg.pSender->GetName() == "clusterBtn")
			::SendMessage(::GetParent(m_hWnd), WM_MAINTAB_SEL_CHANGE, en_sel_cluster, 0);
		if(msg.pSender->GetName() == "sysMsgBtn")
			::SendMessage(::GetParent(m_hWnd), WM_MAINTAB_SEL_CHANGE, en_sel_sysmsg, 0);
	}
	else if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnRead")
		{
			//标记为已读
			CDuiMsgDlg::GetInstance()->SetAllOrderMsgRead();
			CDuiMsgDlg::GetInstance()->SetSysMsgTabOrderUnReadCount();
			CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(false);
		}
		else if(msg.pSender->GetName() == "btnSubscriber")
		{
			//订单消息订阅
			CDuiOrderSubscibe::Show();
		}
	}
}

void CUIMsgManageMainTab::SetSysMsgTabText(CString strText)
{
	DuiLib::COptionUI* pOptionSysMsg = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("sysMsgBtn"));
	pOptionSysMsg->SetText(strText);
}

void CUIMsgManageMainTab::SetCurSel(EN_SEL en)
{
	DuiLib::COptionUI* pOptionFriend = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("friendBtn"));
	DuiLib::COptionUI* pOptionCluster = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("clusterBtn"));
	DuiLib::COptionUI* pOptionSysMsg = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("sysMsgBtn"));
	if(pOptionFriend != NULL && pOptionCluster != NULL && pOptionSysMsg != NULL) 
	{
		pOptionFriend->Selected(en == en_sel_friend);
		pOptionCluster->Selected(en == en_sel_cluster);
		pOptionSysMsg->Selected(en == en_sel_sysmsg);
	}
}

CUIMsgManageMainTab::EN_SEL CUIMsgManageMainTab::GetCurSel()
{
	DuiLib::COptionUI* pOptionFriend = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("friendBtn"));
	DuiLib::COptionUI* pOptionCluster = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("clusterBtn"));
	DuiLib::COptionUI* pOptionSysMsg = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("sysMsgBtn"));
	if(pOptionFriend != NULL && pOptionFriend->IsSelected()) 
	{
		return CUIMsgManageMainTab::en_sel_friend;
	}
	if(pOptionCluster != NULL && pOptionCluster->IsSelected()) 
	{
		return CUIMsgManageMainTab::en_sel_cluster;
	}
	if(pOptionSysMsg != NULL && pOptionSysMsg->IsSelected()) 
	{
		return CUIMsgManageMainTab::en_sel_sysmsg;
	}

	return CUIMsgManageMainTab::en_null;
}

DuiLib::UILIB_RESOURCETYPE CLabelListWnd::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CLabelListWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CLabelListWnd::Notify(DuiLib::TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{	
		::SendMessage(::GetParent(m_hWnd), WM_LABELLIST_SEL_CHANGE, m_pList->GetCurSel(), 0);
	}
}

void CLabelListWnd::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pList = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl("listLabel"));
}

int CLabelListWnd::AddItem(const char *szItem)
{
	DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;  
	pListElement->SetText(szItem);
	pListElement->SetFixedHeight(32);
	m_pList->Add(pListElement);
	m_vecData.push_back(0);
	return m_pList->GetCount()-1;
}

void CLabelListWnd::RemoveAll()
{
	m_vecData.clear();
	m_pList->RemoveAll();
}

DWORD CLabelListWnd::GetItemData(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_vecData.size())
	{
		return -1;
	}

	return m_vecData[nIndex];
}

void CLabelListWnd::SetItemData(int nIndex, DWORD data)
{
	if(nIndex < 0 || nIndex >= m_vecData.size())
	{
		return;
	}

	m_vecData[nIndex] = data;
}

int CLabelListWnd::GetItemCount()
{
	return m_pList->GetCount();
}

void CLabelListWnd::SetCurSel(int nIndex)
{
	m_pList->SelectItem(nIndex);
}

int CLabelListWnd::GetCurSel()
{
	return m_pList->GetCurSel();
}

DuiLib::CListContainerElementUI* CLabelListWnd::GetItem(int nIndex)
{
	return (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nIndex);
}

void CLabelListWnd::SetBkColor(DWORD dwBkColor, DWORD dwItemBkColor, DWORD dwHotItemBkColor, DWORD dwSelectedItemBkColor)
{
	m_pList->SetBkColor(dwBkColor);
	m_pList->SetItemBkColor(dwItemBkColor);
	m_pList->SetHotItemBkColor(dwHotItemBkColor);
	m_pList->SetSelectedItemBkColor(dwSelectedItemBkColor);
}


CImgListUI::CImgListUI(DuiLib::CPaintManagerUI& paint_manager)
	: paint_manager_(paint_manager)
{
}

CImgListUI::~CImgListUI()
{
}

bool CImgListUI::Add(LPCTSTR szImgPath, LPCTSTR szText)
{
	DuiLib::CListContainerElementUI* pListElement = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() ) {
		pListElement = static_cast<DuiLib::CListContainerElementUI*>(m_dlgBuilder.Create(_T("MsgManageImgListItem.xml"), (UINT)0, NULL, &paint_manager_));
	}
	else {
		pListElement = static_cast<DuiLib::CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return false;

	DuiLib::CLabelUI *pNickName = static_cast<DuiLib::CLabelUI*>(pListElement->FindSubControl("nickname"));
	DuiLib::CButtonGifUI *pHead = static_cast<DuiLib::CButtonGifUI*>(pListElement->FindSubControl("HeadPic"));
	if(pNickName == NULL)
		return false;

	CString strFile;
	string strOutImg = szImgPath;
	CString strGifFile =  strOutImg.c_str();
	strGifFile = strGifFile+ ".gif";////文件改名
	if(FileExist(szImgPath))
	{
		//if ( FileExist(strGifFile) )
		//{
		///*	strFile.Format("file='%s' dest='4,2,32,30'", strGifFile);*/
		//	pHead->SetBkImage("");
		//	pHead->SetNormalGifFile(strGifFile);
		//	pHead->SetToolTip(szText);
		//	pNickName->SetText(szText);
		//}
		//else
		//{
			
			strOutImg = CutRoundImg(szImgPath, 28, 28);
			/*strFile.Format("file='%s' dest='4,2,32,30'", strOutImg.c_str());*/
			pHead->SetBkImage(strOutImg.c_str());
			pHead->SetNormalGifFile("");
			pHead->SetToolTip(szText);
			pNickName->SetText(szText);
			
		//}	
	}
	else
	{
		CString strClusterPic = szImgPath;
		if(strClusterPic == "SysBtn\\icon_group.png")////群消息记录
		{
			pHead->SetBkImage(strClusterPic);
			pHead->SetNormalGifFile("");
			pHead->SetToolTip(szText);
			pNickName->SetText(szText);
		}
	}
	return CListUI::Add(pListElement);
}

void CImgListUI::RemoveAll()
{
	CListUI::RemoveAll();
}

DuiLib::UILIB_RESOURCETYPE CImgListWnd::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CImgListWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CImgListWnd::Notify(DuiLib::TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{	
		::SendMessage(::GetParent(m_hWnd), WM_IMGLIST_SEL_CHANGE, m_pList->GetCurSel(), 0);
	}
}

void CImgListWnd::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pList = static_cast<CImgListUI*>(m_PaintManager.FindControl("listImg"));
}

int CImgListWnd::AddItem(LPCTSTR szImgPath, LPCTSTR szText)
{
	if(!m_pList->Add(szImgPath, szText))
		return -1;

	m_vecData.push_back(0);
	return m_pList->GetCount()-1;
}

void CImgListWnd::RemoveAll()
{
	m_vecData.clear();
	m_pList->RemoveAll();
}

DWORD CImgListWnd::GetItemData(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_vecData.size())
	{
		return -1;
	}

	return m_vecData[nIndex];
}

void CImgListWnd::SetItemData(int nIndex, DWORD data)
{
	if(nIndex < 0 || nIndex >= m_vecData.size())
	{
		return;
	}

	m_vecData[nIndex] = data;
}

DuiLib::CControlUI* CImgListWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("ImgList")) == 0)
	{
		return new CImgListUI(m_PaintManager);
	}

	return NULL;
}

int CImgListWnd::GetItemCount()
{
	return m_pList->GetCount();
}

void CImgListWnd::SetCurSel(int nIndex)
{
	m_pList->SelectItem(nIndex);
}

int CImgListWnd::GetCurSel()
{
	return m_pList->GetCurSel();
}

void CUIMsgManageMainTab::InitWindow()
{
	m_pBtnRead = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnRead"));
	m_pBtnSubscriber = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnSubscriber"));
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) 
	m_pBtnRead->SetVisible(true);
	m_pBtnSubscriber->SetVisible(true);
#endif
}

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
std::tr1::shared_ptr<CDuiMsgDlg> CDuiMsgDlg::pInstance = NULL;

std::tr1::shared_ptr<CDuiMsgDlg> CDuiMsgDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiMsgDlg>(new CDuiMsgDlg());
		pInstance->Create(GetDesktopWindow(), _T("消息管理"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	//消息管理器如果有未读订单则打开订单的页面
	if(g_LocalData.GetOrderTradeUnReadCount() > 0)
		pInstance->ShowOrderPage(0);
	else if(g_LocalData.GetOrderTradeUnReadCount() > 0)
		pInstance->ShowOrderPage(3);

	return pInstance;
}

void CDuiMsgDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close();
	}
}

CDuiMsgDlg::CDuiMsgDlg()
{
	m_pMsgDlg = NULL;
}

CDuiMsgDlg::~CDuiMsgDlg()
{
	if(m_pMsgDlg != NULL)
	{
		delete m_pMsgDlg;
		m_pMsgDlg = NULL;
	}
}

DuiLib::UILIB_RESOURCETYPE CDuiMsgDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiMsgDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CDuiMsgDlg::InitWindow()
{
	WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();
}

DuiLib::CControlUI* CDuiMsgDlg::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, _T("OldMsgDlg")) == 0)
	{
		CWndUI  *pUI  = new CWndUI;      
		RECT rc;
		GetWindowRect(GetHWND(), &rc);
		rc.top = 26;
		m_pMsgDlg = new CMsgDlg(CWnd::FromHandle(GetHWND()));
		m_pMsgDlg->Create(CMsgDlg::IDD, CWnd::FromHandle(GetHWND()));
		m_pMsgDlg->ShowWindow(SW_SHOW);
		pUI->Attach(m_pMsgDlg->GetSafeHwnd());         
		return pUI;
	}
	if (_tcsicmp(pstrClassName, _T("ButtonGif")) == 0)
	{
		return	new DuiLib::CButtonGifUI();
	}
	return NULL;
}

void CDuiMsgDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

void CDuiMsgDlg::SetDefaultFriend(uint32 fid)
{
	m_pMsgDlg->SetDefaultFriend(fid);
}

void CDuiMsgDlg::SetDefaultCluster(uint32 cid)
{
	m_pMsgDlg->SetDefaultCluster(cid);
}

void CDuiMsgDlg::OnFinalMessage( HWND hWnd )
{
	m_pMsgDlg->DestroyWindow();
	delete m_pMsgDlg;
	m_pMsgDlg = NULL;
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}

LRESULT CDuiMsgDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg) return 0;
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CDuiMsgDlg::SetOrderMsgRead(OrderMsg* pOrderMsg)
{
	if(m_pMsgDlg != NULL && ::IsWindow(m_pMsgDlg->GetSafeHwnd()))
	{
		m_pMsgDlg->SetOrderMsgRead(pOrderMsg);
		SetSysMsgTabOrderUnReadCount();
	}
}
void CDuiMsgDlg::SetAllOrderMsgRead()
{
	if(m_pMsgDlg != NULL && ::IsWindow(m_pMsgDlg->GetSafeHwnd()))
	{
		m_pMsgDlg->SetAllOrderMsgRead();
	}
}

void CDuiMsgDlg::ShowOrderPage(int nOrderType)
{
	if(m_pMsgDlg != NULL)
	{
		m_pMsgDlg->SetOrderPage(nOrderType);
	}
}

void CDuiMsgDlg::ShowSystemMsgPage()
{
	if(m_pMsgDlg != NULL)
	{
		m_pMsgDlg->SetSystemMsgPage();
	}
}

void CDuiMsgDlg::SetSysMsgTabOrderUnReadCount()
{
	if(m_pMsgDlg != NULL)
	{	
		m_pMsgDlg->SetSysMsgTabText();
	}
}

void CDuiMsgDlg::AddOrderMsg()
{
	SetSysMsgTabOrderUnReadCount();
	m_pMsgDlg->AddOrderMsg();
}

// 消息
void CDuiOrderTree::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		std::map<DuiLib::CButtonUI*, DuiLib::CVerticalLayoutUI*>::iterator itr = m_mapBtnOrderToVLOrderContent.begin();
		for (; itr != m_mapBtnOrderToVLOrderContent.end(); itr++)
		{
			DuiLib::CButtonUI* pBtn = itr->first;
			if(pBtn == msg.pSender || m_mapBtnOrderToTime[pBtn] == msg.pSender)
			{
				OrderMsg* pOrderMsg = (OrderMsg*)pBtn->GetTag();
				if(!pOrderMsg->bIsRead)
				{
					//标记为已读
					pOrderMsg->bIsRead = true;
					pBtn->SetFont(0);
					m_mapBtnOrderToTime[pBtn]->SetFont(0);
					g_LocalData.SetOrderMsgRead(pOrderMsg->orderUuid);
					//订单消息订阅窗口
					if(CDuiOrderNotice::GetInstance() != NULL && CDuiOrderNotice::GetInstance()->GetHWND() != NULL && IsWindowVisible(CDuiOrderNotice::GetInstance()->GetHWND()))
					{
						CDuiOrderNotice::GetInstance()->SetOrderMsgRead(pOrderMsg);
					}
					//如果未读订单为空则取消红点
					BOOL bIsAllRead = TRUE;
					for(int nOrder = 0; nOrder < g_LocalData.GetOrderMsgTrade().size(); nOrder++)
					{
						OrderMsg* pOrderMsgTrade = g_LocalData.GetOrderMsgTrade()[nOrder];
						if(!pOrderMsgTrade->bIsRead)
						{
							bIsAllRead = FALSE;
							break;
						}
					}
					for(int nOrder = 0; nOrder < g_LocalData.GetOrderMsgReturn().size(); nOrder++)
					{
						OrderMsg* pOrderMsgTrade = g_LocalData.GetOrderMsgReturn()[nOrder];
						if(!pOrderMsgTrade->bIsRead)
						{
							bIsAllRead = FALSE;
							break;
						}
					}
					if(bIsAllRead)
					{
						CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(false);
					}
					CDuiMsgDlg::GetInstance()->SetSysMsgTabOrderUnReadCount();
				}

				m_mapBtnOrderToVLOrderContent[pBtn]->SetVisible(!m_mapBtnOrderToVLOrderContent[pBtn]->IsVisible());
			}
		}
		m_vecBtnOrderDetails;
		for(int nOrderDetails = 0; nOrderDetails < m_vecBtnOrderDetails.size(); nOrderDetails++)
		{
			DuiLib::CButtonUI* pBtnOrderDetails = m_vecBtnOrderDetails[nOrderDetails];
			if(pBtnOrderDetails == msg.pSender)
			{
				OrderMsg* pOrderMsg = (OrderMsg*)pBtnOrderDetails->GetTag();
				if(pOrderMsg != NULL)
				{
					if(pOrderMsg->orderType == 0 || pOrderMsg->orderType == 1)
					{
						string orderPackNum = pOrderMsg->orderPackNum;
						CString strDetailUrl;
						strDetailUrl.Format("%sshowStatus=delivery&id=%s", ORDER_TRADE_URL, orderPackNum.c_str());
						OpenHtmlPage(strDetailUrl);
					}
					else if(pOrderMsg->orderType == 2)
					{
						OpenHtmlPage(ORDER_COMMENT_URL);
					}
					else if(pOrderMsg->orderType == 3)
					{
						string orderPackNum = pOrderMsg->orderPackNum;
						CString strDetailUrl;
						strDetailUrl.Format("%sopt=audit&id=%s", ORDER_RETURN_URL, orderPackNum.c_str());
						OpenHtmlPage(strDetailUrl);
					}
				}
			}
		}
	}
}
void CDuiOrderTree::LoadOrderMsg(int nPage, BOOL bIsOrderTrade)
{
	DuiLib::CVerticalLayoutUI* pVLOrderMsg = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl("VLOrderMsg"));
	pVLOrderMsg->RemoveAll();
	m_mapBtnOrderToTime.clear();
	m_mapBtnOrderToVLOrderContent.clear();
	m_vecBtnOrderDetails.clear();
	pVLOrderMsg->EnableScrollBar();

	std::vector<OrderMsg*> vecOrderMsg;
	if(bIsOrderTrade)
		vecOrderMsg = g_LocalData.GetOrderMsgTrade();
	else
		vecOrderMsg = g_LocalData.GetOrderMsgReturn();
	std::vector<OrderMsg*> vecOrderMsgNew;

	int nMax = vecOrderMsg.size() > (nPage+1) * 10 ? (nPage+1) * 10 : vecOrderMsg.size();
	for(int nOrder = nPage * 10; nOrder < nMax; nOrder++)
	{
		vecOrderMsgNew.push_back(vecOrderMsg[nOrder]);
	}

	CString strTimeOld = "";
	for(int nOrder = 0; nOrder < vecOrderMsgNew.size(); nOrder++)
	{
		OrderMsg* pOrder = vecOrderMsgNew[nOrder];
		CString strTime = "";
		strTime = pOrder->orderTime.c_str();
		strTime = strTime.Left(10);
		if(strTimeOld != strTime)
		{
			strTimeOld = strTime;
			//画时间
			DuiLib::CHorizontalLayoutUI* pHLTime = new DuiLib::CHorizontalLayoutUI();
			pHLTime->SetFixedHeight(40);
			DuiLib::CHorizontalLayoutUI* pHLTimeLeft = new DuiLib::CHorizontalLayoutUI();
			DuiLib::CLabelUI* pLabelLeft = new DuiLib::CLabelUI();
			CRect rc(10,20,10,0);
			pLabelLeft->SetPadding(rc);
			pLabelLeft->SetFixedHeight(1);
			pLabelLeft->SetBkColor(0xFFC2C2C2);
			pHLTimeLeft->Add(pLabelLeft);
			DuiLib::CHorizontalLayoutUI* pHLTimeMid = new DuiLib::CHorizontalLayoutUI();
			pHLTimeMid->SetFixedWidth(65);
			DuiLib::CHorizontalLayoutUI* pHLTimeRight = new DuiLib::CHorizontalLayoutUI();
			DuiLib::CLabelUI* pLabelRight = new DuiLib::CLabelUI();
			pLabelRight->SetPadding(rc);
			pLabelRight->SetFixedHeight(1);
			pLabelRight->SetBkColor(0xFFC2C2C2);
			pHLTimeRight->Add(pLabelRight);
			pHLTime->Add(pHLTimeLeft);
			pHLTime->Add(pHLTimeMid);
			pHLTime->Add(pHLTimeRight);
			DuiLib::CLabelUI* pLabel = new DuiLib::CLabelUI();
			pLabel->SetAttribute("align", "center");
			pLabel->SetText(strTime);
			pHLTimeMid->Add(pLabel);
			pVLOrderMsg->Add(pHLTime);
		}
		//画订单
		DuiLib::CHorizontalLayoutUI* pHlTitle = new DuiLib::CHorizontalLayoutUI();
		pHlTitle->SetFixedHeight(30);
		DuiLib::CButtonUI* pButtonOrder = new DuiLib::CButtonUI();
		
		CString strBtnOrder;
		CString strOrderType;
		if(pOrder->orderType == 0)
			strOrderType = "【新订单】";
		else if(pOrder->orderType == 1)
			strOrderType = "【买家已付款】";
		else if(pOrder->orderType == 2)
			strOrderType = "【买家已评价】";
		else if(pOrder->orderType == 3)
			strOrderType = "【退货申请】";
		OrderItem* pOrderItem = pOrder->vecOrderItem[0];
		strBtnOrder.Format("%s (%s) %s", strOrderType, pOrder->buyerName.c_str(), pOrderItem->itemName.c_str());
		pButtonOrder->SetText(/*"买家已评价 (熊宝宝) 可口可乐大瓶装"*/strBtnOrder);
		pButtonOrder->SetAttribute("textcolor", "#FF2E2E2E");
		pButtonOrder->SetAttribute("hottextcolor", "#FF4D95C2");
		pButtonOrder->SetAttribute("align", "left");
		pButtonOrder->SetAttribute("padding", "15,0,0,0");

		DuiLib::CButtonUI* pButtonTime = new DuiLib::CButtonUI();
		pButtonTime->SetText(/*"2015-12-8 08:45:34"*/pOrder->orderTime.c_str());
		pButtonTime->SetAttribute("textcolor", "#FF2E2E2E");
		pButtonTime->SetAttribute("hottextcolor", "#FF4D95C2");
		pButtonTime->SetAttribute("align", "left");
		pButtonTime->SetFixedWidth(130);

		if(!pOrder->bIsRead)
		{
			pButtonOrder->SetFont(1);
			pButtonTime->SetFont(1);
		}

		pHlTitle->Add(pButtonOrder);
		pHlTitle->Add(pButtonTime);

		m_mapBtnOrderToTime[pButtonOrder] = pButtonTime;

		pVLOrderMsg->Add(pHlTitle);

		DuiLib::CVerticalLayoutUI* pVLOrderContent = new DuiLib::CVerticalLayoutUI();
		pVLOrderContent->SetVisible(false);

		pVLOrderContent->SetAttribute("padding", "10,0,10,10");
		pVLOrderContent->SetAttribute("bordersize", "1");
		pVLOrderContent->SetAttribute("bordercolor", "#FFDCDCDC");
		pVLOrderContent->SetAttribute("bkcolor", "#FFF3F3F3");

		//所有订单都有标题和买家ID
		DuiLib::CLabelUI* pLabelTitle = new DuiLib::CLabelUI();
		pLabelTitle->SetAttribute("textcolor", "#FF2E2E2E");
		pLabelTitle->SetAttribute("padding", "10,5,0,0");
		CString strTitle;
		strTitle.Format("标题：%s", pOrderItem->itemName.c_str());
		pLabelTitle->SetText(/*_T("标题：可口可乐大瓶装原味2L瓶")*/strTitle);
		pVLOrderContent->Add(pLabelTitle);

		DuiLib::CLabelUI* pLabelBuyerName = new DuiLib::CLabelUI();
		pLabelBuyerName->SetAttribute("textcolor", "#FF2E2E2E");
		pLabelBuyerName->SetAttribute("padding", "10,1,0,0");
		CString strBuyerName;
		strBuyerName.Format("买家ID：%s", pOrder->buyerName.c_str());
		pLabelBuyerName->SetText(strBuyerName);
		pVLOrderContent->Add(pLabelBuyerName);

		if(pOrder->orderType == 0 || pOrder->orderType == 1)
		{
			pVLOrderContent->SetFixedHeight(150);

			DuiLib::CLabelUI* pLabelStandard = new DuiLib::CLabelUI();
			pLabelStandard->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelStandard->SetAttribute("padding", "10,1,0,0");
			CString strStandard;
			strStandard.Format("商品规格：%s", pOrderItem->itemStandard.c_str());
			pLabelStandard->SetText(strStandard);
			pVLOrderContent->Add(pLabelStandard);

			DuiLib::CLabelUI* pLabelItemCount = new DuiLib::CLabelUI();
			pLabelItemCount->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelItemCount->SetAttribute("padding", "10,1,0,0");
			CString strItemCount;
			strItemCount.Format("商品数量：%d", /*pOrder->vecOrderItem.size()*/pOrderItem->itemCount);
			pLabelItemCount->SetText(strItemCount);
			pVLOrderContent->Add(pLabelItemCount);

 			DuiLib::CLabelUI* pLabelPrice = new DuiLib::CLabelUI();
 			pLabelPrice->SetAttribute("textcolor", "#FF2E2E2E");
 			pLabelPrice->SetAttribute("padding", "10,1,0,0");
 			CString strPrice;
 			strPrice.Format("订单金额：%lg元 （含运费：%lg元）", pOrder->orderPrice + pOrder->orderFreight, pOrder->orderFreight);
 			pLabelPrice->SetText(strPrice);
 			pVLOrderContent->Add(pLabelPrice);
		}
		else if(pOrder->orderType == 2)
		{
			pVLOrderContent->SetFixedHeight(100);
			
			DuiLib::CLabelUI* pLabelContent = new DuiLib::CLabelUI();
			pLabelContent->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelContent->SetAttribute("padding", "10,1,0,0");
			CString strContent;
			strContent.Format("评价内容：%s", pOrder->orderContent.c_str());
			int nCount = 0;
			CString strContentLimit;
			for(int nIndex = 0; nIndex < strContent.GetLength(); nIndex++)
			{
				if(nCount >= 25)
				{
					strContentLimit += "...";
					break;
				}
				BYTE bChar = strContent.GetAt(nIndex);
				strContentLimit += bChar;
				if(bChar > 127)
				{
					nIndex++;
					strContentLimit += strContent.GetAt(nIndex);
				}
				nCount++;
			}
			pLabelContent->SetText(strContentLimit);
			pVLOrderContent->Add(pLabelContent);
		}
		else if(pOrder->orderType == 3)
		{
			pVLOrderContent->SetFixedHeight(180);

			DuiLib::CLabelUI* pLabelStandard = new DuiLib::CLabelUI();
			pLabelStandard->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelStandard->SetAttribute("padding", "10,1,0,0");
			CString strStandard;
			strStandard.Format("商品规格：%s", pOrderItem->itemStandard.c_str());
			pLabelStandard->SetText(strStandard);
			pVLOrderContent->Add(pLabelStandard);

			DuiLib::CLabelUI* pLabelCount = new DuiLib::CLabelUI();
			pLabelCount->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelCount->SetAttribute("padding", "10,1,0,0");
			CString strCount;
			strCount.Format("退货数量：%d件", pOrderItem->itemCount);
			pLabelCount->SetText(strCount);
			pVLOrderContent->Add(pLabelCount);

			DuiLib::CLabelUI* pLabelPrice = new DuiLib::CLabelUI();
			pLabelPrice->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelPrice->SetAttribute("padding", "10,1,0,0");
			CString strPrice;
			strPrice.Format("退货金额：%lg元", pOrderItem->itemPrice);
			pLabelPrice->SetText(strPrice);
			pVLOrderContent->Add(pLabelPrice);

			DuiLib::CLabelUI* pLabelReason = new DuiLib::CLabelUI();
			pLabelReason->SetAttribute("textcolor", "#FF2E2E2E");
			pLabelReason->SetAttribute("padding", "10,1,0,0");
			CString strReason;
			strReason.Format("退货原因：%s", pOrder->orderContent.c_str());
			pLabelReason->SetText(strReason);
			pVLOrderContent->Add(pLabelReason);
		}

		//所有订单都有查看详情
		DuiLib::CButtonUI* pBtnDetails = new DuiLib::CButtonUI();
		pBtnDetails->SetAttribute("padding", "460,0,0,0");
		pBtnDetails->SetAttribute("textcolor", "#FF75B9EC");
		pBtnDetails->SetAttribute("hottextcolor", "#FF75B9EC");
		pBtnDetails->SetText("查看详情＞");
		pBtnDetails->SetTag((DWORD)pOrder);
		m_vecBtnOrderDetails.push_back(pBtnDetails);
		pVLOrderContent->Add(pBtnDetails);

		pVLOrderMsg->Add(pVLOrderContent);

		pButtonOrder->SetTag((DWORD)pOrder);
		m_mapBtnOrderToVLOrderContent[pButtonOrder] = pVLOrderContent;
	}
}

void CDuiOrderTree::LoadTranMsg(int nPage)
{
	DuiLib::CVerticalLayoutUI* pVLOrderMsg = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl("VLOrderMsg"));
	pVLOrderMsg->RemoveAll();
	//m_mapBtnOrderToTime.clear();
	//m_mapBtnOrderToVLOrderContent.clear();
	//m_vecBtnOrderDetails.clear();
	pVLOrderMsg->EnableScrollBar();

	std::vector<TranMsg*> vecTranMsg;
	vecTranMsg = g_LocalData.GetTranMsg();
	std::vector<TranMsg*> vecTranMsgNew;

	int nMax = vecTranMsg.size() > (nPage+1) * PAGE_COUNT_TRANMSG ? (nPage+1) * PAGE_COUNT_TRANMSG : vecTranMsg.size();
	for(int nOrder = nPage * PAGE_COUNT_TRANMSG; nOrder < nMax; nOrder++)
	{
		vecTranMsgNew.push_back(vecTranMsg[nOrder]);
	}


	for(int nTran = 0; nTran < vecTranMsgNew.size(); nTran++)
	{
		TranMsg* pTranMsg = vecTranMsgNew[nTran];
		CString strMsg = pTranMsg->strSystemMsg.c_str();
		int nCusterBefore = strMsg.Find(" ");
		int nCusterAfter = strMsg.ReverseFind(' ');
		if(nCusterBefore != -1)
		{
			strMsg.Insert(nCusterAfter, "</b>");
			strMsg.Insert(nCusterBefore+1, "<b>");
		}

		CString strTimeAll,strDate, strTime;
		CTime  tm(pTranMsg->time);
		strTime=tm.Format("%H:%M:%S");
		strDate=tm.Format("%Y.%m.%d");
		strTimeAll.Format("%s %s", strDate,strTime);

		DuiLib::CHorizontalLayoutUI* pHlTran = new DuiLib::CHorizontalLayoutUI();
		pHlTran->SetFixedHeight(30);

		DuiLib::CLabelUI* pLabelMsg = new DuiLib::CLabelUI;
		pLabelMsg->SetText(strMsg);
		pLabelMsg->SetAttribute("padding", "15,0,0,0");
		pLabelMsg->SetAttribute("showhtml", "true");
		pLabelMsg->SetAttribute("textcolor", "#FF2E2E2E");
		pHlTran->Add(pLabelMsg);
		//DuiLib::CHorizontalLayoutUI* pHlMid = new DuiLib::CHorizontalLayoutUI;
		//pHlTran->Add(pHlMid);
		DuiLib::CLabelUI* pLabelTime = new DuiLib::CLabelUI;
		pLabelTime->SetFixedWidth(130);
		pLabelTime->SetText(strTimeAll);
		pLabelTime->SetAttribute("textcolor", "#FF2E2E2E");
		pHlTran->Add(pLabelTime);
		pVLOrderMsg->Add(pHlTran);
	}
	//{
	//	OrderMsg* pOrder = vecOrderMsgNew[nOrder];
	//	CString strTime = "";
	//	strTime = pOrder->orderTime.c_str();
	//	strTime = strTime.Left(10);
	//	if(strTimeOld != strTime)
	//	{
	//		strTimeOld = strTime;
	//		//画时间
	//		DuiLib::CHorizontalLayoutUI* pHLTime = new DuiLib::CHorizontalLayoutUI();
	//		pHLTime->SetFixedHeight(40);
	//		DuiLib::CHorizontalLayoutUI* pHLTimeLeft = new DuiLib::CHorizontalLayoutUI();
	//		DuiLib::CLabelUI* pLabelLeft = new DuiLib::CLabelUI();
	//		CRect rc(10,20,10,0);
	//		pLabelLeft->SetPadding(rc);
	//		pLabelLeft->SetFixedHeight(1);
	//		pLabelLeft->SetBkColor(0xFFC2C2C2);
	//		pHLTimeLeft->Add(pLabelLeft);
	//		DuiLib::CHorizontalLayoutUI* pHLTimeMid = new DuiLib::CHorizontalLayoutUI();
	//		pHLTimeMid->SetFixedWidth(65);
	//		DuiLib::CHorizontalLayoutUI* pHLTimeRight = new DuiLib::CHorizontalLayoutUI();
	//		DuiLib::CLabelUI* pLabelRight = new DuiLib::CLabelUI();
	//		pLabelRight->SetPadding(rc);
	//		pLabelRight->SetFixedHeight(1);
	//		pLabelRight->SetBkColor(0xFFC2C2C2);
	//		pHLTimeRight->Add(pLabelRight);
	//		pHLTime->Add(pHLTimeLeft);
	//		pHLTime->Add(pHLTimeMid);
	//		pHLTime->Add(pHLTimeRight);
	//		DuiLib::CLabelUI* pLabel = new DuiLib::CLabelUI();
	//		pLabel->SetAttribute("align", "center");
	//		pLabel->SetText(strTime);
	//		pHLTimeMid->Add(pLabel);
	//		pVLOrderMsg->Add(pHLTime);
	//	}
	//	//画订单
	//	DuiLib::CHorizontalLayoutUI* pHlTitle = new DuiLib::CHorizontalLayoutUI();
	//	pHlTitle->SetFixedHeight(30);
	//	DuiLib::CButtonUI* pButtonOrder = new DuiLib::CButtonUI();

	//	CString strBtnOrder;
	//	CString strOrderType;
	//	if(pOrder->orderType == 0)
	//		strOrderType = "【新订单】";
	//	else if(pOrder->orderType == 1)
	//		strOrderType = "【买家已付款】";
	//	else if(pOrder->orderType == 2)
	//		strOrderType = "【买家已评价】";
	//	else if(pOrder->orderType == 3)
	//		strOrderType = "【退货申请】";
	//	OrderItem* pOrderItem = pOrder->vecOrderItem[0];
	//	strBtnOrder.Format("%s (%s) %s", strOrderType, pOrder->buyerName.c_str(), pOrderItem->itemName.c_str());
	//	pButtonOrder->SetText(/*"买家已评价 (熊宝宝) 可口可乐大瓶装"*/strBtnOrder);
	//	pButtonOrder->SetAttribute("textcolor", "#FF2E2E2E");
	//	pButtonOrder->SetAttribute("hottextcolor", "#FF4D95C2");
	//	pButtonOrder->SetAttribute("align", "left");
	//	pButtonOrder->SetAttribute("padding", "15,0,0,0");

	//	DuiLib::CButtonUI* pButtonTime = new DuiLib::CButtonUI();
	//	pButtonTime->SetText(/*"2015-12-8 08:45:34"*/pOrder->orderTime.c_str());
	//	pButtonTime->SetAttribute("textcolor", "#FF2E2E2E");
	//	pButtonTime->SetAttribute("hottextcolor", "#FF4D95C2");
	//	pButtonTime->SetAttribute("align", "left");
	//	pButtonTime->SetFixedWidth(130);

	//	if(!pOrder->bIsRead)
	//	{
	//		pButtonOrder->SetFont(1);
	//		pButtonTime->SetFont(1);
	//	}

	//	pHlTitle->Add(pButtonOrder);
	//	pHlTitle->Add(pButtonTime);

	//	m_mapBtnOrderToTime[pButtonOrder] = pButtonTime;

	//	pVLOrderMsg->Add(pHlTitle);

	//	DuiLib::CVerticalLayoutUI* pVLOrderContent = new DuiLib::CVerticalLayoutUI();
	//	pVLOrderContent->SetVisible(false);

	//	pVLOrderContent->SetAttribute("padding", "10,0,10,10");
	//	pVLOrderContent->SetAttribute("bordersize", "1");
	//	pVLOrderContent->SetAttribute("bordercolor", "#FFDCDCDC");
	//	pVLOrderContent->SetAttribute("bkcolor", "#FFF3F3F3");

	//	//所有订单都有标题和买家ID
	//	DuiLib::CLabelUI* pLabelTitle = new DuiLib::CLabelUI();
	//	pLabelTitle->SetAttribute("textcolor", "#FF2E2E2E");
	//	pLabelTitle->SetAttribute("padding", "10,5,0,0");
	//	CString strTitle;
	//	strTitle.Format("标题：%s", pOrderItem->itemName.c_str());
	//	pLabelTitle->SetText(/*_T("标题：可口可乐大瓶装原味2L瓶")*/strTitle);
	//	pVLOrderContent->Add(pLabelTitle);

	//	DuiLib::CLabelUI* pLabelBuyerName = new DuiLib::CLabelUI();
	//	pLabelBuyerName->SetAttribute("textcolor", "#FF2E2E2E");
	//	pLabelBuyerName->SetAttribute("padding", "10,1,0,0");
	//	CString strBuyerName;
	//	strBuyerName.Format("买家ID：%s", pOrder->buyerName.c_str());
	//	pLabelBuyerName->SetText(strBuyerName);
	//	pVLOrderContent->Add(pLabelBuyerName);

	//	if(pOrder->orderType == 0 || pOrder->orderType == 1)
	//	{
	//		pVLOrderContent->SetFixedHeight(150);

	//		DuiLib::CLabelUI* pLabelStandard = new DuiLib::CLabelUI();
	//		pLabelStandard->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelStandard->SetAttribute("padding", "10,1,0,0");
	//		CString strStandard;
	//		strStandard.Format("商品规格：%s", pOrderItem->itemStandard.c_str());
	//		pLabelStandard->SetText(strStandard);
	//		pVLOrderContent->Add(pLabelStandard);

	//		DuiLib::CLabelUI* pLabelItemCount = new DuiLib::CLabelUI();
	//		pLabelItemCount->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelItemCount->SetAttribute("padding", "10,1,0,0");
	//		CString strItemCount;
	//		strItemCount.Format("商品数量：%d", /*pOrder->vecOrderItem.size()*/pOrderItem->itemCount);
	//		pLabelItemCount->SetText(strItemCount);
	//		pVLOrderContent->Add(pLabelItemCount);

	//		DuiLib::CLabelUI* pLabelPrice = new DuiLib::CLabelUI();
	//		pLabelPrice->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelPrice->SetAttribute("padding", "10,1,0,0");
	//		CString strPrice;
	//		strPrice.Format("订单金额：%lg元 （含运费：%lg元）", pOrder->orderPrice + pOrder->orderFreight, pOrder->orderFreight);
	//		pLabelPrice->SetText(strPrice);
	//		pVLOrderContent->Add(pLabelPrice);
	//	}
	//	else if(pOrder->orderType == 2)
	//	{
	//		pVLOrderContent->SetFixedHeight(100);

	//		DuiLib::CLabelUI* pLabelContent = new DuiLib::CLabelUI();
	//		pLabelContent->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelContent->SetAttribute("padding", "10,1,0,0");
	//		CString strContent;
	//		strContent.Format("评价内容：%s", pOrder->orderContent.c_str());
	//		int nCount = 0;
	//		CString strContentLimit;
	//		for(int nIndex = 0; nIndex < strContent.GetLength(); nIndex++)
	//		{
	//			if(nCount >= 25)
	//			{
	//				strContentLimit += "...";
	//				break;
	//			}
	//			BYTE bChar = strContent.GetAt(nIndex);
	//			strContentLimit += bChar;
	//			if(bChar > 127)
	//			{
	//				nIndex++;
	//				strContentLimit += strContent.GetAt(nIndex);
	//			}
	//			nCount++;
	//		}
	//		pLabelContent->SetText(strContentLimit);
	//		pVLOrderContent->Add(pLabelContent);
	//	}
	//	else if(pOrder->orderType == 3)
	//	{
	//		pVLOrderContent->SetFixedHeight(180);

	//		DuiLib::CLabelUI* pLabelStandard = new DuiLib::CLabelUI();
	//		pLabelStandard->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelStandard->SetAttribute("padding", "10,1,0,0");
	//		CString strStandard;
	//		strStandard.Format("商品规格：%s", pOrderItem->itemStandard.c_str());
	//		pLabelStandard->SetText(strStandard);
	//		pVLOrderContent->Add(pLabelStandard);

	//		DuiLib::CLabelUI* pLabelCount = new DuiLib::CLabelUI();
	//		pLabelCount->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelCount->SetAttribute("padding", "10,1,0,0");
	//		CString strCount;
	//		strCount.Format("退货数量：%d件", pOrderItem->itemCount);
	//		pLabelCount->SetText(strCount);
	//		pVLOrderContent->Add(pLabelCount);

	//		DuiLib::CLabelUI* pLabelPrice = new DuiLib::CLabelUI();
	//		pLabelPrice->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelPrice->SetAttribute("padding", "10,1,0,0");
	//		CString strPrice;
	//		strPrice.Format("退货金额：%lg元", pOrderItem->itemPrice);
	//		pLabelPrice->SetText(strPrice);
	//		pVLOrderContent->Add(pLabelPrice);

	//		DuiLib::CLabelUI* pLabelReason = new DuiLib::CLabelUI();
	//		pLabelReason->SetAttribute("textcolor", "#FF2E2E2E");
	//		pLabelReason->SetAttribute("padding", "10,1,0,0");
	//		CString strReason;
	//		strReason.Format("退货原因：%s", pOrder->orderContent.c_str());
	//		pLabelReason->SetText(strReason);
	//		pVLOrderContent->Add(pLabelReason);
	//	}

	//	//所有订单都有查看详情
	//	DuiLib::CButtonUI* pBtnDetails = new DuiLib::CButtonUI();
	//	pBtnDetails->SetAttribute("padding", "460,0,0,0");
	//	pBtnDetails->SetAttribute("textcolor", "#FF75B9EC");
	//	pBtnDetails->SetAttribute("hottextcolor", "#FF75B9EC");
	//	pBtnDetails->SetText("查看详情＞");
	//	pBtnDetails->SetTag((DWORD)pOrder);
	//	m_vecBtnOrderDetails.push_back(pBtnDetails);
	//	pVLOrderContent->Add(pBtnDetails);

	//	pVLOrderMsg->Add(pVLOrderContent);

	//	pButtonOrder->SetTag((DWORD)pOrder);
	//	m_mapBtnOrderToVLOrderContent[pButtonOrder] = pVLOrderContent;
	//}
}

void CDuiOrderTree::SetOrderMsgRead(OrderMsg* pOrderMsg)
{
	std::map<DuiLib::CButtonUI*, DuiLib::CVerticalLayoutUI*>::iterator it = m_mapBtnOrderToVLOrderContent.begin();
	for(; it != m_mapBtnOrderToVLOrderContent.end(); ++it)
	{
		DuiLib::CButtonUI* btn = it->first;
		if((DWORD)btn->GetTag() == (DWORD)pOrderMsg)
		{
			btn->SetFont(0);
			std::map<DuiLib::CButtonUI*, DuiLib::CButtonUI*>::iterator it_find = m_mapBtnOrderToTime.find(btn);
			if(it_find != m_mapBtnOrderToTime.end())
			{
				it_find->second->SetFont(0);
			}
			return;
		}
	}
}

void CDuiOrderTree::SetAllOrderMsgRead()
{
	std::map<DuiLib::CButtonUI*, DuiLib::CVerticalLayoutUI*>::iterator it = m_mapBtnOrderToVLOrderContent.begin();
	for(; it != m_mapBtnOrderToVLOrderContent.end(); ++it)
	{
		DuiLib::CButtonUI* btn = it->first;
		OrderMsg* pOrderMsg = (OrderMsg*)btn->GetTag();
		if(!pOrderMsg->bIsRead)
		{
			pOrderMsg->bIsRead = true;
			btn->SetFont(0);
			std::map<DuiLib::CButtonUI*, DuiLib::CButtonUI*>::iterator it_find = m_mapBtnOrderToTime.find(btn);
			if(it_find != m_mapBtnOrderToTime.end())
			{
				it_find->second->SetFont(0);
			}
		}
	}
}

LRESULT CDuiOrderTree::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//// 禁止聊天窗口位于顶端最大化，或者左右边缘时停靠
	//if(WM_EXITSIZEMOVE == uMsg)////拖动停止时设置为正常大小显示
	//{
	//    ShowWindow(SW_NORMAL);
	//}
	if(WM_MOUSEMOVE == uMsg)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		std::map<DuiLib::CButtonUI*, DuiLib::CButtonUI*>::iterator itrOrder = m_mapBtnOrderToTime.begin();
		for(; itrOrder != m_mapBtnOrderToTime.end(); itrOrder++)
		{
			DuiLib::CButtonUI* pBtnOrder = itrOrder->first;
			DuiLib::CButtonUI* pBtnTime = itrOrder->second;
			CRect rcOrder = pBtnOrder->GetPos();
			CRect rcTime = pBtnTime->GetPos();

			if(rcOrder.PtInRect(pt))
			{
				pBtnTime->SetAttribute("textcolor", "#FF4D95C2");
			}
			else if(rcTime.PtInRect(pt))
			{
				pBtnOrder->SetAttribute("textcolor", "#FF4D95C2");
			}
			else
			{
				pBtnTime->SetAttribute("textcolor", "#FF2E2E2E");
				pBtnOrder->SetAttribute("textcolor", "#FF2E2E2E");
			}
		}
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}