// PopupHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PopupHistoryDlg.h"
#include "afxdialogex.h"


const UINT32 PAGE_COUNT = 30;
const DWORD	CLR_ACTIONS = RGB(0,84,166);	// 聊天动作字符颜色：黑色
const DWORD CLR_OBJECT = RGB(255,0,0);	// 聊天对象字符颜色
const UINT32 SYS_PIC_NUM= 53;////系统表情个数

// CPopupHistoryDlg dialog

IMPLEMENT_DYNAMIC(CPopupHistoryDlg, CDialogEx)
IMPLEMENT_HMG_SCROLLBAR(CPopupHistoryDlg)

CPopupHistoryDlg::CPopupHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupHistoryDlg::IDD, pParent)
	, m_wndToolBar(this)
	, m_nToolBarHight(0)
	, m_nToolBarWidth(0)
	, m_nFindToolBarHight(0)
	, m_bInitial(false)
	, m_iPageAll(0)
	, m_iPageAllTotal(0)
	, m_pFriendInfo(nullptr)
	, m_pClusterInfo(nullptr)
{
	m_font.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	m_hBkGround = ::CreateSolidBrush(RGB(235, 235, 235));
	m_closeTab.SetBackgroundColor(RGB(255, 255, 255));
	m_closeTab.SetFrameModol(FRAME_DRAW_BOTTOM);

	// tab设置
	m_colCtrl.SetBackgroundColor(RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255));
	m_colCtrl.SetItemTextColor(0x4f4f4f, 0xc2954d, 0x4f4f4f);
	m_colCtrl.DrawUnderline(TRUE);
	m_colCtrl.SetDirectMode(CColumnCtrl::EN_X_DIRE);
	m_colCtrl.SetFont(&m_font);
	m_colCtrl.SetItemWidth(60);
}

CPopupHistoryDlg::~CPopupHistoryDlg()
{
	DeleteObject(m_hBkGround);
}

void CPopupHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_FIND, m_btnFind);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_COMBO1, m_comboRange);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_lblRange);
	DDX_Control(pDX, IDC_STATIC_KEYWORD, m_lblKeyword);
	DDX_Control(pDX, IDC_STATIC_CLOSE_TAB, m_closeTab);
	DDX_Control(pDX, IDC_STATIC_MSG_RANGE, m_colCtrl);
}


BEGIN_MESSAGE_MAP(CPopupHistoryDlg, CDialog)
	ON_WM_SIZE()
	/*ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_INFO, &CPopupHistoryDlg::OnTcnSelchangeTabInfo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_HISTORY, &CPopupHistoryDlg::OnTcnSelchangeHistoryTabInfo)*/
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_MESSAGE(WM_SHOW_HISTORY_FIND_TOOL, &ShowFindToolBar)
	ON_MESSAGE(WM_HISTORY_PAGE, &ShowHistoryFirst)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnTabBtnCtrlSelChange)
	SET_HMG_SCROLLBAR(IDC_EDIT_HISTORY_ALL)
	SET_HMG_SCROLLBAR(IDC_EDIT_HISTORY_PIC)
	SET_HMG_SCROLLBAR(IDC_EDIT_HISTORY_FILE)
	ON_MESSAGE(WM_CLOSETABCTRL_CLOSE,OnCloseTabCtrlClose)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPopupHistoryDlg message handlers
void CPopupHistoryDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInitial)
	{
		return;
	}

	AdjustUI();
}


BOOL CPopupHistoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置控件字体
	CWnd* pChild = GetNextWindow(GW_CHILD);
	while (pChild != NULL)
	{
		pChild->SetFont(&g_font12);
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}
	/*if (m_pClusterInfo == nullptr) {
		m_closeTab.AddItem("资料");
	}*/
	m_closeTab.AddItem("消息记录");
	m_closeTab.SetCurSel(0);

	InitUserInfoTab();
	InitHistoryTab();

	CRect rc;
	GetWindowRect(rc);
	rc.right = rc.left + m_nToolBarWidth;
	MoveWindow(rc);


	m_comboRange.AddString("最近一个月");
	m_comboRange.AddString("最近三个月");
	m_comboRange.AddString("最近一年");
	m_comboRange.AddString("全部");
	m_comboRange.SetCurSel(3);

	m_bIsFindToolBarOn = false;
	m_bIsSearchMsg = false;
	m_bInitial = true;

	m_btnFind.SetPng(IDB_PNG_SENIOR_BTN_HOVER, IDB_PNG_SENIOR_BTN_NORMAL, IDB_PNG_SENIOR_BTN_PRESSED, IDB_PNG_SENIOR_BTN_PRESSED);
	m_btnFind.SetButtonData(SW_SHOW);
	m_comboRange.SetBorderColor(RGB(222, 222, 222));
	m_editKeyword.SetBorderColor(RGB(222, 222, 222));
	ShowFindToolBar(0,0);
	return TRUE;
}

void CPopupHistoryDlg::UpdateEditRect(CRect &rcParent)
{
	CRect rc;
	rc.left = rcParent.right - 388;
	rc.right = rcParent.right - 1;
	rc.top = rcParent.top + 150;
	rc.bottom = rcParent.bottom - 40;
	m_editHistoryAll.MoveWindow(rc);
}
void CPopupHistoryDlg::InitUserInfoTab()
{

}

void CPopupHistoryDlg::InitHistoryTab()
{
	m_colCtrl.AddItem("全部");
	m_colCtrl.AddItem("图片", "", TRUE);
	if (m_pClusterInfo == nullptr) 
	{
		m_colCtrl.AddItem("文件", "", TRUE);
	}
	
	//m_editHistoryAll
	m_editHistoryAll.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_READONLY, CRect(), this, IDC_EDIT_HISTORY_ALL);
	m_editHistoryAll.ShowWindow(SW_SHOW);
	m_editHistoryAll.SetEmotionImageList(&g_imgEmotionList);
	AttachHMGScrollbar(m_editHistoryAll.m_hWnd);

	DWORD mask =::SendMessage(m_editHistoryAll.m_hWnd,EM_GETEVENTMASK, 0, 0);  
	mask = mask | ENM_LINK  | ENM_MOUSEEVENTS | ENM_SCROLLEVENTS |ENM_KEYEVENTS;  
	::SendMessage(m_editHistoryAll.m_hWnd,EM_SETEVENTMASK, 0, mask);  
	::SendMessage(m_editHistoryAll.m_hWnd,EM_AUTOURLDETECT, true, 0);  
	m_editHistoryAll.SetShowLinkInformation(TRUE);

	//m_editHistoryPic
	m_editHistoryPic.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_READONLY, CRect(), this, IDC_EDIT_HISTORY_PIC);
	m_editHistoryPic.ShowWindow(SW_HIDE);
	m_editHistoryPic.SetEmotionImageList(&g_imgEmotionList);
	AttachHMGScrollbar(m_editHistoryPic.m_hWnd);


	//m_editHistoryfile
	if (m_pClusterInfo == nullptr)
	{ 
	  m_editHistoryFile.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_READONLY , CRect(), this, IDC_EDIT_HISTORY_FILE);
	  m_editHistoryFile.ShowWindow(SW_HIDE);
	  m_editHistoryFile.SetEmotionImageList(&g_imgEmotionList);
	  AttachHMGScrollbar(m_editHistoryFile.m_hWnd);
   }

	m_wndToolBar.Create(CHistoryToolBar::IDD, this);
	CRect rcToolBar;
	m_wndToolBar.GetWindowRect(rcToolBar);
	m_nToolBarHight = rcToolBar.Height();
	m_nToolBarWidth = rcToolBar.Width();
	m_wndToolBar.ShowWindow(SW_SHOW);


	CRect rcFindToolBar;
	m_btnFind.GetWindowRect(rcFindToolBar);
	m_nFindToolBarHight = rcFindToolBar.Height() + 4;

	m_colCtrl.SetCurSel(SEL_ALL);
}

void CPopupHistoryDlg::AdjustUI()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcCloseTab;
	m_closeTab.GetWindowRect(&rcCloseTab);
	int nHeight = rcCloseTab.Height();
	rcCloseTab.top = 0;
	rcCloseTab.left = 1;
	rcCloseTab.right = rc.right;
	rcCloseTab.bottom = nHeight;
	m_closeTab.MoveWindow(&rcCloseTab);

	rcCloseTab.top = rcCloseTab.bottom;
	rcCloseTab.bottom = rcCloseTab.top + /*nHeight*/33;
	m_colCtrl.MoveWindow(&rcCloseTab);

	/*移动工具按钮栏*/
	CRect rcToolBar;
	rcToolBar = rc;
	rcToolBar.top = rcToolBar.bottom - m_nToolBarHight;
	m_wndToolBar.MoveWindow(rcToolBar);
	m_wndToolBar.ShowWindow(SW_SHOW);

	AdjustFindToolBar();
}

void CPopupHistoryDlg::AdjustFindToolBar()
{
	CRect rcTabBtn;
	m_colCtrl.GetWindowRect(&rcTabBtn);
	ScreenToClient(&rcTabBtn);
	CRect rcToolBar;
	m_wndToolBar.GetWindowRect(&rcToolBar);
	ScreenToClient(&rcToolBar);
	CRect rcHistory;
	rcHistory.left = 1;
	rcHistory.right = rcTabBtn.right - 1;
	rcHistory.top = rcTabBtn.bottom + 1;
	rcHistory.bottom = rcToolBar.top;

	if (m_btnFind.GetButtonData() == SW_SHOW)
	{
		rcHistory.bottom -= m_nFindToolBarHight;
		m_editHistoryAll.MoveWindow(rcHistory);
		m_editHistoryPic.MoveWindow(rcHistory);
		if (m_pClusterInfo == nullptr)
		{ 
			m_editHistoryFile.MoveWindow(rcHistory);
		}
	
		CRect rcTemp;
		m_lblRange.GetWindowRect(&rcTemp);
		ScreenToClient(&rcTemp);
		int nHeight = rcTemp.Height();
		rcTemp.top = rcHistory.bottom + (m_nFindToolBarHight-nHeight)/2;
		rcTemp.bottom = rcTemp.top + nHeight;
		m_lblRange.MoveWindow(rcTemp);

		m_comboRange.GetWindowRect(&rcTemp);
		ScreenToClient(&rcTemp);
		nHeight = rcTemp.Height();
		rcTemp.top = rcHistory.bottom + (m_nFindToolBarHight-nHeight)/2;
		rcTemp.bottom = rcTemp.top + nHeight;
		m_comboRange.MoveWindow(rcTemp);

		m_lblKeyword.GetWindowRect(&rcTemp);
		ScreenToClient(&rcTemp);
		nHeight = rcTemp.Height();
		rcTemp.top = rcHistory.bottom + (m_nFindToolBarHight-nHeight)/2;
		rcTemp.bottom = rcTemp.top + nHeight;
		m_lblKeyword.MoveWindow(rcTemp);

		m_editKeyword.GetWindowRect(&rcTemp);
		ScreenToClient(&rcTemp);
		nHeight = rcTemp.Height();
		rcTemp.top = rcHistory.bottom + (m_nFindToolBarHight-nHeight)/2;
		rcTemp.bottom = rcTemp.top + nHeight;
		m_editKeyword.MoveWindow(rcTemp);

		m_btnFind.GetWindowRect(&rcTemp);
		ScreenToClient(&rcTemp);
		nHeight = rcTemp.Height();
		rcTemp.top = rcHistory.bottom + (m_nFindToolBarHight-nHeight)/2;
		rcTemp.bottom = rcTemp.top + nHeight;
		m_btnFind.MoveWindow(rcTemp);


		//m_btnRet.GetWindowRect(&rcTemp);////返回
		//ScreenToClient(&rcTemp);
		//nHeight = rcTemp.Height();
		//rcTemp.top = rcHistory.bottom + (m_nFindToolBarHight-nHeight)/2;
		//rcTemp.bottom = rcTemp.top + nHeight;
		//m_btnRet.MoveWindow(rcTemp);
	}
	else
	{
		m_editHistoryAll.MoveWindow(rcHistory);
		m_editHistoryPic.MoveWindow(rcHistory);
		if (m_pClusterInfo == nullptr)
		{
			m_editHistoryFile.MoveWindow(rcHistory);
		}
	}
}

void CPopupHistoryDlg::AdjustBottomToolBar(INT32 iCurPage, INT32 iAllPage)
{
	if (iCurPage <= 0)
	{
		m_wndToolBar.m_btnFirst.EnableWindow(FALSE);
		m_wndToolBar.m_btnPrev.EnableWindow(FALSE);
	}
	else
	{
		m_wndToolBar.m_btnFirst.EnableWindow(TRUE);
		m_wndToolBar.m_btnPrev.EnableWindow(TRUE);
	}

	if  (iCurPage >= iAllPage - 1)
	{
		m_wndToolBar.m_btnLast.EnableWindow(FALSE);
		m_wndToolBar.m_btnNext.EnableWindow(FALSE);
	}
	else
	{
		m_wndToolBar.m_btnLast.EnableWindow(TRUE);
		m_wndToolBar.m_btnNext.EnableWindow(TRUE);
	}

	m_wndToolBar.m_btnFirst.Invalidate();
	m_wndToolBar.m_btnPrev.Invalidate();
	m_wndToolBar.m_btnLast.Invalidate();
	m_wndToolBar.m_btnNext.Invalidate();
	
	m_wndToolBar.SetPage(iAllPage, iCurPage + 1);
}

//void CPopupHistoryDlg::OnTcnSelchangeTabInfo(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//
//	int iCurSel = m_wndTabCtrl.GetCurSel(); 
//	switch (iCurSel)
//	{
//		case SEL_INFO:
//			{
//				m_wndHistoryTabCtrl.ShowWindow(SW_HIDE);
//				m_wndToolBar.ShowWindow(SW_HIDE);
//			}
//			break;
//				
//		case SEL_HISTORY:
//			{
//				m_wndHistoryTabCtrl.ShowWindow(SW_SHOW);
//				m_wndToolBar.ShowWindow(SW_SHOW);
//
//			}
//			break;
//
//		default:
//			break;
//		}
//
//}

// tab栏选着改变事件
LRESULT CPopupHistoryDlg::OnTabBtnCtrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	if(pCtrl == &m_colCtrl)
	{
		int nCurSel = HIWORD(lParam);

		switch (nCurSel)
		{
		case SEL_ALL:
			{
				m_editHistoryAll.ShowWindow(SW_SHOW);
				m_editHistoryAll.ScrollToBottom();
				m_editHistoryPic.ShowWindow(SW_HIDE);
				if (m_pClusterInfo == nullptr)
				{
					m_editHistoryFile.ShowWindow(SW_HIDE);
				}
				if(m_bIsFindToolBarOn)
				{
					m_btnFind.ShowWindow(SW_SHOW);
					m_editKeyword.ShowWindow(SW_SHOW);
					m_comboRange.ShowWindow(SW_SHOW);
					m_lblRange.ShowWindow(SW_SHOW);
					m_lblKeyword.ShowWindow(SW_SHOW);
					//m_btnRet.ShowWindow(SW_SHOW);////返回

					ShowFindToolBar(0, 0);
				}
				m_wndToolBar.ShowWindow(SW_SHOW);
			}
			break;
		case SEL_PIC:
			{
				m_editHistoryAll.ShowWindow(SW_HIDE);
				m_editHistoryPic.ShowWindow(SW_SHOW);
				m_editHistoryPic.ScrollToBottom();
				if (m_pClusterInfo == nullptr)
				{
					m_editHistoryFile.ShowWindow(SW_HIDE);
				}

				m_btnFind.SetButtonData(SW_HIDE);
				m_btnFind.ShowWindow(SW_HIDE);
				m_editKeyword.ShowWindow(SW_HIDE);
				m_comboRange.ShowWindow(SW_HIDE);
				m_lblRange.ShowWindow(SW_HIDE);
				m_lblKeyword.ShowWindow(SW_HIDE);
				m_wndToolBar.ShowWindow(SW_HIDE);
			    ////m_btnRet.SetButtonData(SW_HIDE);////返回
				//m_btnRet.ShowWindow(SW_HIDE)/*;*/
			}
			break;
		case SEL_FILE:
			{
				if (m_pClusterInfo == nullptr)
				{
					m_editHistoryAll.ShowWindow(SW_HIDE);
					m_editHistoryPic.ShowWindow(SW_HIDE);
					m_editHistoryFile.ShowWindow(SW_SHOW);
					m_editHistoryFile.ScrollToBottom();
				}
				m_btnFind.SetButtonData(SW_HIDE);
				m_btnFind.ShowWindow(SW_HIDE);
				m_editKeyword.ShowWindow(SW_HIDE);
				m_comboRange.ShowWindow(SW_HIDE);
				m_lblRange.ShowWindow(SW_HIDE);
				m_lblKeyword.ShowWindow(SW_HIDE);
				m_wndToolBar.ShowWindow(SW_HIDE);
				//m_btnRet.SetButtonData(SW_HIDE);////返回
				//m_btnRet.ShowWindow(SW_HIDE);
			}
			break;

		default:
			break;
		}

		AdjustFindToolBar();
	}

	return 0;
}

LRESULT CPopupHistoryDlg::ShowFindToolBar(WPARAM, LPARAM)
{
	CRect rcHistoryAll;
	m_editHistoryAll.GetClientRect(rcHistoryAll);

	if (m_btnFind.GetButtonData() == SW_SHOW)
	{
		m_lblRange.ShowWindow(SW_HIDE);
		m_comboRange.ShowWindow(SW_HIDE);
		m_lblKeyword.ShowWindow(SW_HIDE);
		m_editKeyword.ShowWindow(SW_HIDE);
		m_btnFind.SetButtonData(SW_HIDE);
		m_btnFind.ShowWindow(SW_HIDE);
		//m_btnRet.SetButtonData(SW_HIDE);////返回
		//m_btnRet.ShowWindow(SW_HIDE);
		m_bIsFindToolBarOn = false;
		m_bIsSearchMsg = false;
		//回到全部消息
		MsgAllToPage(m_iPageAll, m_iPageAllTotal, m_mlRecordAll);
	}
	else
	{
		m_lblRange.ShowWindow(SW_SHOW);
		m_comboRange.ShowWindow(SW_SHOW);
		m_lblKeyword.ShowWindow(SW_SHOW);
		m_editKeyword.ShowWindow(SW_SHOW);
 		m_btnFind.SetButtonData(SW_SHOW);
		m_btnFind.ShowWindow(SW_SHOW);
		//m_btnRet.SetButtonData(SW_SHOW);////返回
		//m_btnRet.ShowWindow(SW_SHOW);

		m_bIsFindToolBarOn = true;
	}

	AdjustFindToolBar();
	return 0;
}

void CPopupHistoryDlg::SetFriedInfo(FRIEND_INFO* fi)
{
	m_pFriendInfo = fi;
}

void CPopupHistoryDlg::SetClusterInfo(CLUSTER_INFO* ci)
{
	m_pClusterInfo = ci;
}

void CPopupHistoryDlg::LoadAllMsg()
{
	m_mlRecordAll.RemoveAll();
	//从文件获取消息记录
	if (m_pFriendInfo != nullptr)
	{
		g_LocalData.GetMsgList(m_pFriendInfo->GetId(), CDataManager::USER, m_mlRecordAll);
	}
	else if(m_pClusterInfo != nullptr)
	{
		g_LocalData.GetMsgList(m_pClusterInfo->nFamilyID, CDataManager::CLUSTER, m_mlRecordAll);
	}
	m_iPageAllTotal = ceil(m_mlRecordAll.GetCount() * 1.0 / PAGE_COUNT);
	m_iPageAll = m_iPageAllTotal - 1;
	//加载到页面，并定位到第最后页
	MsgAllToPage(m_iPageAll,m_iPageAllTotal,m_mlRecordAll);
	MsgPicFileUpdate();
}


void CPopupHistoryDlg::UpdateSearchMsg(const CString& sKeyword, CTime tMsgBegin)
{
	m_mlRecordSearch.RemoveAll();
	int nMsgCnt = m_mlRecordAll.GetCount();
	
	for(int i=0; i < nMsgCnt; i++)
	{
		POSITION pos = m_mlRecordAll.FindIndex(i);
		ASSERT(pos);
		if(pos == NULL)
			break;
		XT_MSG &msg = m_mlRecordAll.GetAt(pos);
		// 只查找聊天信息
		if(msg.data_type==0 && msg.send_time > tMsgBegin.GetTime())
		{
			if(SubstrExist(msg.data,-1,sKeyword) && !SubstrExist(msg.data, -1, "<image:"))
				m_mlRecordSearch.AddTail(msg);
		}
	}

	m_iPageSearchTotal = ceil(m_mlRecordSearch.GetCount() * 1.0 / PAGE_COUNT);
	m_iPageSearch = 0;
}

LRESULT CPopupHistoryDlg::ShowHistoryFirst(WPARAM wParam, LPARAM lParam)
{
	CHistoryToolBar::PAGE_TPYE ePageType = (CHistoryToolBar::PAGE_TPYE) wParam;

	int iCurPage = 0, iCurPageTotal = 0;

	if(m_bIsSearchMsg)
	{
		iCurPage = m_iPageSearch;
		iCurPageTotal = m_iPageSearchTotal;
	}
	else
	{
		iCurPage = m_iPageAll;
		iCurPageTotal = m_iPageAllTotal;
	}

	switch(ePageType)
	{
		case CHistoryToolBar::PAGE_FIRST:
			iCurPage = 0;
			break;
		case CHistoryToolBar::PAGE_LAST:
			iCurPage = m_iPageAllTotal - 1;
			break;
		case CHistoryToolBar::PAGE_NEXT:
			if(iCurPage < m_iPageAllTotal) iCurPage++;
			break;
		case CHistoryToolBar::PAGE_PREV:
			if(iCurPage > 0) iCurPage--;
			break;
		default:
			break;
	}

	ASSERT( iCurPage >= 0 && iCurPage < m_iPageAllTotal);
	ASSERT(m_colCtrl.GetCurSel() == SEL_ALL);


	if(!m_bIsSearchMsg)
	{
		m_iPageAll = iCurPage;
		MsgAllToPage(m_iPageAll, m_iPageAllTotal, m_mlRecordAll);
	}
	else
	{
		m_iPageSearch = iCurPage;
		MsgAllToPage(m_iPageSearch, m_iPageSearchTotal, m_mlRecordAll);
	}

	return 0;
}

/*全部消息记录，分页显示*/
void CPopupHistoryDlg::MsgAllToPage(INT32 iCurPage, INT32 iAllPage, const MSG_LIST &mlRecord)
{
	if (m_editHistoryAll.m_hWnd == NULL )
		return;

	m_editHistoryAll.ClearMsg();

	if (iCurPage < 0 || iCurPage > iAllPage)
	{
		return;
	}

	for ( int i = 0; i < PAGE_COUNT; i++ )
	{
		POSITION pos = mlRecord.FindIndex(iCurPage * PAGE_COUNT + i);

		if (pos != NULL)
		{
			ShowTalkMsgAll(mlRecord.GetAt(pos));
		}
		else
		{
			break;
		}
	}

	AdjustBottomToolBar(iCurPage, iAllPage);

	m_editHistoryAll.ScrollToBottom();

}

/*图片和文件一页显示，不分页*/
void CPopupHistoryDlg::MsgPicFileUpdate()
{
	if (m_editHistoryPic.m_hWnd == NULL )
	{
		return;
	}
	
	if(m_pClusterInfo == nullptr )
	{
		if(m_editHistoryFile.m_hWnd == NULL)
		{
			return;
		}
	}

	m_editHistoryPic.ClearMsg();
	if(m_pClusterInfo == nullptr)
		m_editHistoryFile.ClearMsg();

	int iMsgCnt = m_mlRecordAll.GetCount();
	for ( int i = 0; i < iMsgCnt; i++ )
	{
		POSITION pos = m_mlRecordAll.FindIndex(i);
		if (pos != NULL)
		{
			ShowTalkMsgPic(m_mlRecordAll.GetAt(pos));
			if(m_pClusterInfo == nullptr)
				ShowTalkMsgFile(m_mlRecordAll.GetAt(pos));
		}
		else
		{
			break;
		}
	}

	m_editHistoryAll.ScrollToBottom();
	if(m_pClusterInfo == nullptr)
		m_editHistoryFile.ScrollToBottom();
}

void CPopupHistoryDlg::ShowTalkMsgAll(const XT_MSG &msg)
{
	if (m_editHistoryAll.m_hWnd == NULL)
	{
		return;
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
	bHasAction = ParseMsg(msg.data, sMsg, sObj, sAction0, sAction1, sAction2);

	CString strTime;
	CTime  tm(msg.send_time);
	strTime = tm.Format(" (%Y.%m.%d  %H:%M:%S)");

	m_editHistoryAll.SetSel(-1, -1);
	m_editHistoryAll.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_editHistoryAll.InsertText("\n");
	// 好友发给我的信息
	if (msg.from_id != g_ImUserInfo.GetId())
	{
		xf.fontColor = RGB(0x3E, 0x9F, 0xDB);
	}
	else
	{
		xf.fontColor = RGB(0xD3, 0x48, 0x28);
	}
	m_editHistoryAll.SetStartIndent(80);
	// 打印出消息标题（发话人+动作+对象+日期）
	if (bHasAction)
	{
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryAll.InsertText(sAction0, xf);
		xf.fontColor= (msg.from_id == g_ImUserInfo.GetId() ?  RGB(0,128,64) : RGB(22,68,223));
		CString strName = msg.from_nickname;
		FRIEND_INFO *pFriend = ::FindFriend(msg.from_id);
		if(pFriend != NULL)
		{
			strName = UserInfoToStrEx(pFriend);
		}
		m_editHistoryAll.InsertText(strName, xf);
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryAll.InsertText(sAction1, xf);
		xf.fontColor = CLR_OBJECT;
		m_editHistoryAll.InsertText(sObj, xf);
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryAll.InsertText(sAction2, xf);
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
		m_editHistoryAll.InsertText(strName, xf);
	}
	xf.fontColor = RGB(122, 122, 122);
	m_editHistoryAll.InsertText(strTime, xf);
	m_editHistoryAll.InsertText(":\n");
	m_editHistoryAll.SetSel(-1, -1);
	m_editHistoryAll.GetSel(cr);

	XT_FONT   xfMsg;
	xfMsg.flags = msg.fontStyle;
	xfMsg.fontColor = msg.fontColor;

	strcpy(xfMsg.fontName, msg.fontName);
	xfMsg.fontSize = msg.fontSize;

	CStringList sl;

	m_editHistoryAll.SetStartIndent(300);
	if (bHasAction)
		m_editHistoryAll.InsertText(sMsg);
	else
	{
		m_editHistoryAll.InsertText(msg.data);
		CString strData = msg.data;
		if(strData.Find("文件 \"") != -1 && strData.Find("\" 接收完成。") != -1)
		{
			CHARRANGE cr;

			m_editHistoryAll.SetSel(-1,-1);
			m_editHistoryAll.GetSel(cr);

			m_editHistoryAll.InsertText("\n");
			m_editHistoryAll.SetStartIndent(230);

			XT_FONT xf;
			m_editHistoryAll.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_editHistoryAll.InsertLink("打开文件");
			m_editHistoryAll.InsertText(" / ");
			m_editHistoryAll.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_editHistoryAll.InsertLink("打开文件夹");
		}
	}

	m_editHistoryAll.SetSel(cr.cpMax, -1);
	m_editHistoryAll.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_editHistoryAll.SetSel(-1, -1);
	m_editHistoryAll.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
}

void CPopupHistoryDlg::ShowTalkMsgPic(const XT_MSG &msg)
{
	if (m_editHistoryPic.m_hWnd == NULL)
	{
		return;
	}

	CString sObj;		// 聊天对象
	CString sAction0;	// 聊天动作0
	CString sAction1;	// 聊天动作1
	CString sAction2;	// 聊天动作2
	CString	sMsg;		// 聊天信息	
	CString strTxt = _T("");
	bool	bHasAction;	// 消息中是否包含动作信息
	CHARRANGE cr;
	XT_FONT xf;
	// 处理动作消息
	bHasAction = ParseMsg(msg.data, sMsg, sObj, sAction0, sAction1, sAction2);

	strTxt = msg.data;
	if (-1 == strTxt.Find(_T("<image:")))////图片的标识找不到
	{
		return;
	}
	else
	{
		for(int i = 0;i<SYS_PIC_NUM;i++)
		{
		   CString strPic = GetGifName(i).c_str();
		   if(0 == strcmp(strTxt,strPic))
		   {
               return;////系统表情不插入图片中
		   }
		}
	}
	CString strTime;
	CTime  tm(msg.send_time);
	strTime = tm.Format(" (%Y.%m.%d  %H:%M:%S)");

	m_editHistoryPic.SetSel(-1, -1);
	m_editHistoryPic.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_editHistoryPic.InsertText("\n");
	// 好友发给我的信息
	if (msg.from_id != g_ImUserInfo.GetId())
	{
		xf.fontColor = RGB(0x3E, 0x9F, 0xDB);
	}
	else
	{
		xf.fontColor = RGB(0xD3, 0x48, 0x28);
	}
	m_editHistoryPic.SetStartIndent(80);
	// 打印出消息标题（发话人+动作+对象+日期）
	if (bHasAction)
	{
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryPic.InsertText(sAction0, xf);
		xf.fontColor= (msg.from_id == g_ImUserInfo.GetId() ?  RGB(0,128,64) : RGB(22,68,223));
		CString strName = msg.from_nickname;
		FRIEND_INFO *pFriend = ::FindFriend(msg.from_id);
		if(pFriend != NULL)
		{
			strName = UserInfoToStrEx(pFriend);
		}
		m_editHistoryPic.InsertText(strName, xf);
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryPic.InsertText(sAction1, xf);
		xf.fontColor = CLR_OBJECT;
		m_editHistoryPic.InsertText(sObj, xf);
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryPic.InsertText(sAction2, xf);
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
		m_editHistoryPic.InsertText(strName, xf);
	}
	xf.fontColor = RGB(122, 122, 122);
	m_editHistoryPic.InsertText(strTime, xf);
	m_editHistoryPic.InsertText(":\n");
	m_editHistoryPic.SetSel(-1, -1);
	m_editHistoryPic.GetSel(cr);

	XT_FONT   xfMsg;
	xfMsg.flags = msg.fontStyle;
	xfMsg.fontColor = msg.fontColor;

	strcpy(xfMsg.fontName, msg.fontName);
	xfMsg.fontSize = msg.fontSize;

	CStringList sl;

	m_editHistoryPic.SetStartIndent(300);
	if (bHasAction)
	{
	    m_editHistoryPic.InsertText(sMsg);
    }
	else
	{
		m_editHistoryPic.InsertText(msg.data);
	}
	m_editHistoryPic.SetSel(cr.cpMax, -1);
	m_editHistoryPic.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_editHistoryPic.SetSel(-1, -1);
	m_editHistoryPic.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
}

void CPopupHistoryDlg::ShowTalkMsgFile(const XT_MSG &msg)
{
	if (m_editHistoryFile.m_hWnd == NULL)
	{
		return;
	}

	CString sObj;		// 聊天对象
	CString sAction0;	// 聊天动作0
	CString sAction1;	// 聊天动作1
	CString sAction2;	// 聊天动作2
	CString	sMsg;		// 聊天信息
	CString strTxt = _T("");
	bool	bHasAction;	// 消息中是否包含动作信息
	CHARRANGE cr;
	XT_FONT xf;
	// 处理动作消息
	bHasAction = ParseMsg(msg.data, sMsg, sObj, sAction0, sAction1, sAction2);


	strTxt = msg.data;
	if (-1 == strTxt.Find(_T("文件 \"")))////接收文件的标识找不到
	{
		return;
	}
	CString strTime;
	CTime  tm(msg.send_time);
	strTime = tm.Format(" (%Y.%m.%d  %H:%M:%S)");

	m_editHistoryFile.SetSel(-1, -1);
	m_editHistoryFile.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_editHistoryFile.InsertText("\n");
	// 好友发给我的信息
	if (msg.from_id != g_ImUserInfo.GetId())
	{
		xf.fontColor = RGB(0x3E, 0x9F, 0xDB);
	}
	else
	{
		xf.fontColor = RGB(0xD3, 0x48, 0x28);
	}
	m_editHistoryFile.SetStartIndent(80);
	// 打印出消息标题（发话人+动作+对象+日期）
	if (bHasAction)
	{
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryFile.InsertText(sAction0, xf);
		xf.fontColor= (msg.from_id == g_ImUserInfo.GetId() ?  RGB(0,128,64) : RGB(22,68,223));
		CString strName = msg.from_nickname;
		FRIEND_INFO *pFriend = ::FindFriend(msg.from_id);
		if(pFriend != NULL)
		{
			strName = UserInfoToStrEx(pFriend);
		}
		m_editHistoryFile.InsertText(strName, xf);
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryFile.InsertText(sAction1, xf);
		xf.fontColor = CLR_OBJECT;
		m_editHistoryFile.InsertText(sObj, xf);
		xf.fontColor = CLR_ACTIONS;
		m_editHistoryFile.InsertText(sAction2, xf);
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
		m_editHistoryFile.InsertText(strName, xf);
	}
	xf.fontColor = RGB(122, 122, 122);
	m_editHistoryFile.InsertText(strTime, xf);
	m_editHistoryFile.InsertText(":\n");
	m_editHistoryFile.SetSel(-1, -1);
	m_editHistoryFile.GetSel(cr);

	XT_FONT   xfMsg;
	xfMsg.flags = msg.fontStyle;
	xfMsg.fontColor = msg.fontColor;

	strcpy(xfMsg.fontName, msg.fontName);
	xfMsg.fontSize = msg.fontSize;

	CStringList sl;

	m_editHistoryFile.SetStartIndent(300);
	if (bHasAction)
		m_editHistoryFile.InsertText(sMsg);
	else
	{
		m_editHistoryFile.InsertText(msg.data);

		CString strData = msg.data;
		if(strData.Find("文件 \"") != -1 && strData.Find("\" 接收完成。") != -1)
		{
			CHARRANGE cr;

			m_editHistoryFile.SetSel(-1,-1);
			m_editHistoryFile.GetSel(cr);

			m_editHistoryFile.InsertText("\n");
			m_editHistoryFile.SetStartIndent(230);

			XT_FONT xf;
			m_editHistoryFile.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_editHistoryFile.InsertLink("打开文件");
			m_editHistoryFile.InsertText(" / ");
			m_editHistoryFile.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_editHistoryFile.InsertLink("打开文件夹");
		}
	}

	m_editHistoryFile.SetSel(cr.cpMax, -1);
	m_editHistoryFile.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_editHistoryFile.SetSel(-1, -1);
	m_editHistoryFile.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
}

void CPopupHistoryDlg::OnBnClickedBtnFind()
{

	CString sKeyword;
	m_editKeyword.GetWindowText(sKeyword);
	/*if(sKeyword.GetLength()==0)
	{
		CMessageBox::Prompt("请输入要查询的文字");
		return;
	}*/

	CTime  tMsgBegin = CTime::GetCurrentTime();
	switch(m_comboRange.GetCurSel())
	{
		case 0:/*最近一个月*/
			tMsgBegin -= 1*30*24*60*60;
			break;
		case 1:/*最近三个月*/
			tMsgBegin -= 3*30*24*60*60;
			break;
		case 2:/*最近一年*/
			tMsgBegin -= 12*30*24*60*60;
			break;
		case 3:/*全部*/
			tMsgBegin = 0;
			break;
		default:
			break;
	}

	UpdateSearchMsg(sKeyword, tMsgBegin);
	m_bIsSearchMsg = true;
	MsgAllToPage(m_iPageSearch, m_iPageSearchTotal, m_mlRecordSearch);

	m_wndToolBar.m_btnSearch.SetCheck(0);
	m_wndToolBar.OnBnClickedButtonHistoryFind();
}

BOOL CPopupHistoryDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	// 设置背景色
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,RGB(255, 255, 255));	

	// 绘制边框
	CPen pen(PS_SOLID, 1, RGB(222, 222, 222));
	CPen *oldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rcClient.left, rcClient.top);
	pDC->LineTo(rcClient.left, rcClient.bottom);
	pDC->SelectObject(oldPen);

	return FALSE;
}

LRESULT CPopupHistoryDlg::OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam)
{
	if(GetParent() != NULL)
	{
		::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSETABCTRL_CLOSE, NULL, NULL);
	}
	return S_OK;
}

void CPopupHistoryDlg::OnBnClickedBtnRet()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnFind.SetButtonData(SW_SHOW);
	ShowFindToolBar(0, 0);
}

BOOL CPopupHistoryDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN )
	{
		switch (pMsg->wParam)  
		{ 
		case VK_ESCAPE:
		case VK_RETURN : 
			{
				return TRUE;		// 不处理回车和esc
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPopupHistoryDlg::OnPaint()
{
	CPaintDC dc(this); 
	CDialog::OnPaint();

	if (m_btnFind.GetButtonData() == SW_SHOW)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		rcClient.top = rcClient.bottom - 80;
		dc.FillSolidRect(rcClient, 0x00EBEBEB);	
	}
}