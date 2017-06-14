#include "stdafx.h"
#include "CSPlatDlg.h"
#include "im\WebProcess.h"
#include "TabWndSearch.h"
#include "SearchListWnd.h"
#include "MainFrame.h"
#include "DuiMfFriendList.h"
#include "DuiMfRecentList.h"
#include "im\DuiP2PSendMsgDlg.h"
#include "im\MessageHandleWnd.h"
#include "im\DuiCounsel.h"
#include "im\ModfCommNameDlg.h"
#include "GroupSendDlg.h"
#include "im\DuiCounselNone.h"
#include "im\IMProtocol.h"
#include "CExitMsgWarning.h"
#include "CheckServiceLogWarn.h"
using namespace std;

extern CMessageHandleWnd *g_pMessageHandleWnd;
extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////保存未读消息的好友ID和未读条数
extern BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu);
extern CLocalData g_LocalData;

BEGIN_XT_MESSAGE_MAP(CDuiCSPlatDlg)
	// 修改好友备注
	ON_XT_COMMAND(CMD_MOD_FCNAME_ACK,OnModFCNameAck)
END_XT_MESSAGE_MAP()
/*
std::tr1::shared_ptr<CCSPlatQueuDlg> CCSPlatQueuDlg::pInstance = NULL;
std::tr1::shared_ptr<CCSPlatQueuDlg> CCSPlatQueuDlg::Show(HWND hWnd, POINT pt, vector<int> vecWaitFriendID)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = std::tr1::shared_ptr<CCSPlatQueuDlg>(new CCSPlatQueuDlg());
		pInstance->Create(hWnd, _T(""), WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST, pt.x, pt.y, 210, 32, NULL);
		HWND hWndParent = pInstance->m_hWnd;
		while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);

		// 加载数据
		pInstance->UpdateList(vecWaitFriendID);

		::ShowWindow(pInstance->m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}

	return pInstance;
}

void CCSPlatQueuDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close(IDOK);
	}
}

CCSPlatQueuDlg::CCSPlatQueuDlg()
{
}

CCSPlatQueuDlg::~CCSPlatQueuDlg()
{
}

DuiLib::UILIB_RESOURCETYPE CCSPlatQueuDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LRESULT CCSPlatQueuDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(uMsg == WM_KILLFOCUS)
	{
		CloseDlg();
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CCSPlatQueuDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CCSPlatQueuDlg::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);

	m_pList = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl("lsCust"));
}

void CCSPlatQueuDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "itemactivate")
	{
		// 获取转接对象
		int nIndex = m_pList->GetCurSel();
		DuiLib::CListLabelElementUI* pItem = static_cast<DuiLib::CListLabelElementUI*>(m_pList->GetItemAt(nIndex));
		int nFriendID = pItem->GetTag();	// 好友ID
		FRIEND_INFO *pFriend = GetFriendInfo(nFriendID);
		m_pList->RemoveAt(nIndex);
		if(pFriend == NULL)
			return;

		// 向服务端发送已接待好友请求
		XT_RECEPTE_CUST_REQ req;
		req.idBuyer = (uint32)nFriendID;
		req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_OFFER;	// 主动接待
		req.teamId = (uint32)100;
		g_pProtocol->SendReq(req);

		CMainFrame::OpenP2PMsgDlg(pFriend);
		CloseDlg();

		CDuiCSPlatDlg::DelWaitFriend(nFriendID);
	}
}

// 根据好友ID刷新列表
void CCSPlatQueuDlg::UpdateList(vector<int> &vecQueu)
{
	// 加载数据
	int nCount = (int)vecQueu.size();
	for(int i = 0; i < nCount; ++i)
	{
		int nFriendID = vecQueu[i];
		AddItem(nFriendID);
	}

	m_pList->SelectItem(0);

	// 重新改变窗口大小
	nCount = m_pList->GetCount();
	int nHeight = nCount > 10 ? 10*33 : nCount*33;
	CRect rc;
	GetWindowRect(GetHWND(), &rc);
	MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), nHeight, TRUE);
}

// 添加单元
void CCSPlatQueuDlg::AddItem(int nFriendID)
{
	FRIEND_INFO *pFriend = ::FindFriend(nFriendID);
	if(pFriend == NULL)
		return;

	DuiLib::CListLabelElementUI *pItem = new DuiLib::CListLabelElementUI();
	pItem->SetFixedHeight(33);
	pItem->SetText(UserInfoToStr(pFriend));
	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	string strOutImg = CutRoundImg(strFaceFile, 30, 30);
	CString strImgFmt;
	strImgFmt.Format("file='%s' dest='10,1,40,31'", strOutImg.c_str());
	pItem->SetBkImage(strImgFmt);

	m_pList->Add(pItem);

	pItem->SetTag(nFriendID);
}

// 添加等待队列中的好友
void CCSPlatQueuDlg::AddWaitFriend(int nFriendID)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->AddItem(nFriendID);

		// 重新改变窗口大小
		int nCount = pInstance->m_pList->GetCount();
		int nHeight = nCount > 10 ? 10*33 : nCount*33;
		CRect rc;
		GetWindowRect(pInstance->GetHWND(), &rc);
		MoveWindow(pInstance->GetHWND(), rc.left, rc.top, rc.Width(), nHeight, TRUE);
	}
}

// 删除等待队列中的好友
void CCSPlatQueuDlg::DelWaitFriend(int nFriendID)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		int nCount = pInstance->m_pList->GetCount();
		for(int i = 0; i < nCount; ++i)
		{
			DuiLib::CListLabelElementUI* pItem = static_cast<DuiLib::CListLabelElementUI*>(pInstance->m_pList->GetItemAt(i));
			int nFriendIDTemp = pItem->GetTag();	// 好友ID
			if(nFriendIDTemp == nFriendID)
			{
				pInstance->m_pList->RemoveAt(i);
				break;
			}				
		}

		// 重新改变窗口大小
		nCount = pInstance->m_pList->GetCount();
		int nHeight = nCount > 10 ? 10*33 : nCount*33;
		CRect rc;
		GetWindowRect(pInstance->GetHWND(), &rc);
		MoveWindow(pInstance->GetHWND(), rc.left, rc.top, rc.Width(), nHeight, TRUE);
	}
}
*/

static long s_SearchEditStartChar = 0;
std::vector<int> CDuiCSPlatDlg::g_vecWaitFriendID;		// 等待队列

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

std::tr1::shared_ptr<CDuiCSPlatDlg> CDuiCSPlatDlg::pInstance = NULL;
std::tr1::shared_ptr<CDuiCSPlatDlg> CDuiCSPlatDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = std::tr1::shared_ptr<CDuiCSPlatDlg>(new CDuiCSPlatDlg());
		pInstance->Create(GetDesktopWindow(), _T(""), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW /*| WS_EX_TOPMOST*/, 512,281,210,520);
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

void CDuiCSPlatDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close(IDOK);
	}
}

CDuiCSPlatDlg::CDuiCSPlatDlg()
{
	m_pSearchListWnd = NULL;
}

CDuiCSPlatDlg::~CDuiCSPlatDlg()
{
}

DuiLib::UILIB_RESOURCETYPE CDuiCSPlatDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LRESULT CDuiCSPlatDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(WM_TIMER == uMsg)	OnTimer(wParam);
	else if(WM_MOUSEWHEEL == uMsg)
	{	// 鼠标滚轮消息
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(m_PaintManager.GetFocus() == pEdit && m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
		{
			m_pSearchListWnd->HandleMessage(uMsg, wParam, lParam);
		}
	}
	else if(WM_KEYDOWN == uMsg && (wParam == VK_UP || wParam == VK_DOWN))
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(m_PaintManager.GetFocus() == pEdit && m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
		{
			m_pSearchListWnd->HandleMessage(uMsg, wParam, lParam);

			// 搜索框重新获取焦点
			m_PaintManager.SetFocus(NULL);
			SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);
		}
	}
	else if(WM_LBUTTONUP == uMsg)
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		DuiLib::CControlUI *pFocus = m_PaintManager.GetFocus();
		if(pEdit != pFocus && m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
		{
			m_pSearchListWnd->Close(IDOK);
			pEdit->SetText("");
		}
	}
	else if(WM_ACTIVATE == uMsg && WA_INACTIVE == wParam)
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()) && lParam != (LPARAM)m_pSearchListWnd->GetHWND())
		{
			m_pSearchListWnd->Close(IDOK);
			pEdit->SetText("");
		}
	}
	else if(WM_MSG_ITEMCLICK == uMsg)
	{	// 单元选择
		int nIdx = wParam;
		int nFriendID = m_pSearchListWnd->GetItemData(nIdx);
		m_pSearchListWnd->Close(IDOK);
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		pEdit->SetText("");
		FRIEND_INFO *pFriend = ::FindFriend(nFriendID);
		if(pFriend != NULL)
		{
			CMainFrame::OpenP2PMsgDlg(pFriend);
		}
	}
	else if(WM_XT_CMD_NOTIFY == uMsg)	
	{
		OnXtMessage(wParam, lParam);
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CDuiCSPlatDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CDuiCSPlatDlg::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();

	m_pLayoutPage = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("mainTab"));
	m_pRecentList = static_cast<CDuiRecentList*>(m_PaintManager.FindControl("lsRecent"));
	m_pFriendList = static_cast<CDuiFriendList*>(m_PaintManager.FindControl("lsFriend"));
	m_pCounselList = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl("lsCounsel"));

	m_labelUnMsgCust = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelUnMsgCust"));//客户未读消息红点
	m_labelUnMsgTeam = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelUnMsgTeam"));//我的团队未读消息红点

	DuiLib::COptionUI* pOptionCust = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("custBtn"));

	m_MenuCheckInfo[_T("MenuAll")] = true;
	m_bIsUpdate = FALSE;

	// 加载最近联系人名单
	CDuiRecentList *pMfRecentList = CMainFrame::GetInstance()->GetRecentList();
	int nCount = pMfRecentList->GetCount();
	for(int i = nCount-1; i >= 0; i--)
	{
		DWORD dwData = pMfRecentList->GetItemAt(i)->GetTag();
		m_pRecentList->AddItem_Interface(dwData);
	}

	// 更新我的团队列表
	UpdateTeamList();

	UpdateWaitLabel();

	g_pProtocol->RegistCmd(CMD_MOD_FCNAME_ACK,m_hWnd);
}

DuiLib::CControlUI* CDuiCSPlatDlg::CreateControl(LPCTSTR pstrClass)
{
	if(_tcsicmp(pstrClass, _T("RecentList")) == 0)
	{
		return new CDuiRecentList(GetHWND(), m_PaintManager);
	}
	else if (_tcsicmp(pstrClass, _T("TeamTreeView")) == 0)
	{
		return new CDuiFriendList(GetHWND(), m_PaintManager);
	}
	return NULL;
}

void CDuiCSPlatDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == _T("itemselect"))//选择咨询框中的好友
	{
		if( m_pCounselList == (DuiLib::CListUI *)msg.pSender )
		{
#ifdef _CS_PLAT_EX_
			int index = ((DuiLib::CListUI *)msg.pSender)->GetCurSel();
			if(index >= 0)
			{
				//打开客户的聊天窗口隐藏其他客服的聊天窗口
				for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
				{
					if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg]->GetHWND() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] != m_vecP2PSendMsgDlg[index])
					{
						g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg]->ShowWindow(false);
					}
				}
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index]);////手动打开的消息
			}
#else
			int index = ((DuiLib::CListUI *)msg.pSender)->GetCurSel();
			if(index >= 0)
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index]);////手动打开的消息
#endif
		}
	}
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "custBtn")
		{
			m_pLayoutPage->SelectItem(0);
			m_labelUnMsgCust->SetVisible(false);//选择客户标签红点消失
		}
		else if(msg.pSender->GetName() == "recentBtn")
		{
			m_pLayoutPage->SelectItem(1);
		}
		else if(msg.pSender->GetName() == "teamBtn")
		{
			m_pLayoutPage->SelectItem(2);
			m_labelUnMsgTeam->SetVisible(false);//选择我的团队标签红点消失
		}
		else if(msg.pSender->GetName() == "btnReception")
		{
			// 向服务端发送已接待好友请求
			XT_RECEPTE_CUST_REQ req;
			req.idBuyer = 0;
			req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER;	// 主动接待
			req.teamId = 0;
			g_pProtocol->SendReq(req);
		}
		else if(msg.pSender->GetName() == _T("btnMarkeing")) //点击了白旗之后显示红旗
		{
			DuiLib::CListContainerElementUI* pListElement = NULL;
			pListElement = (DuiLib::CListContainerElementUI*)msg.pSender->GetParent();//((DuiLib::CListUI *)msg.pSender->GetParent())->GetItemAt(index);

			DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
			DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

			pBtnMarkeing->SetVisible(false);
			pBtnMarked->SetVisible(true);
		}
		else if(msg.pSender->GetName() == _T("btnMarked")) //点击了红旗之后显示白旗
		{
			DuiLib::CListContainerElementUI* pListElement = NULL;
			pListElement = (DuiLib::CListContainerElementUI*)msg.pSender->GetParent();//((DuiLib::CListUI *)msg.pSender->GetParent())->GetItemAt(index);

			DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
			DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));
			pBtnMarkeing->SetVisible(true);
			pBtnMarked->SetVisible(false);
		}
		else if(msg.pSender->GetName() == _T("btnClose")) //点击关闭图标关闭一个咨询框和聊天框
		{
			CMainFrame::GetInstance()->LockDlg();////锁住对话框
			DuiLib::CLabelUI* pListElement = NULL;
			pListElement = (DuiLib::CLabelUI*)msg.pSender->GetParent();
			DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));

			///////////////////////////////////////////////////////
			/////先判断是否有服务小记没有保存，再判断是否有未读消息
			DuiLib::CControlUI* pC = pListElement->GetParent();
			DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
			int nIndex = pElement->GetIndex();

			FRIEND_INFO* pFriend = m_vecFriend[nIndex];////当前会话好友信息
			extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
			////g_mapServiceLog.clear();
			DWORD dwServiceLogCnt = g_mapServiceLog.size();
			if(dwServiceLogCnt == 0)//所有买家窗口都没有保存服务小记
			{
				//对面是买家
				if(pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))
				{
					std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
					pWnd->ShowModal();
					if(pWnd->m_bClickedBtnOK)
					{
                        ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nIndex]);////选中当前会话窗口
					}
					return ;
				}
				else//对面不是买家，则直接检查未读消息
				{
					if(pBtnRemind->IsVisible())////关闭选中的一个聊天框,有未读消息
					{
						std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
						pWnd->ShowModal();
						if(pWnd->m_bUnReadMsgShow)///确认关闭		
						{
							DuiLib::CControlUI* pC = pListElement->GetParent();
							DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
							int nIndex = pElement->GetIndex();

							CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录，写入临时log;
							int nUnReadCnt = _ttoi(strTxt);
							FRIEND_INFO* pFriend = m_vecFriend[nIndex];
							CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////更新未读消息，不写文件
							m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
							CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
						}	
						else 
						{
							CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
							return;
						}
					}
					else ////当前窗口无未读消息，直接关闭
					{
						//////////////////////////////////////////////////////
						DuiLib::CControlUI* pC = pListElement->GetParent();
						DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
						int nIndex = pElement->GetIndex();
						/////////////////
						m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
						CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
					}
				}
			}
			else //有买家保存了完整或者不完整的服务小记，判断是否为当前买家
			{
				bool bUncompleted = FALSE;
				//对面是买家
				if( pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))
				{
					DWORD dwFid = 0;
					DWORD dwStatus = 0;
					map<DWORD,DWORD>::iterator it = g_mapServiceLog.find(pFriend->GetId());///查找是否含有当前好友节点
					if(it!=g_mapServiceLog.end())//找到了
					{
						dwFid    = it->first;
						dwStatus = it->second;
						if(dwStatus == UNCOMPLETED_SERVICELOG)//未保存完整
						{
							FRIEND_INFO *pFriend = ::FindFriend(dwFid);//找到好友信息
							if(pFriend!= NULL)//当前咨询窗口保存了不完整的服务小记
							{
								bUncompleted = FALSE;
								std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());
								pWnd->ShowModal();
								////切换到当前买家右侧页
								DuiLib::CControlUI* pC = pListElement->GetParent();
								DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
								int nIndex = pElement->GetIndex();
								ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nIndex]);////选中当前会话窗口
								return ;
							}
						}
						else///已保存服务小记，再检查是否有未读消息
						{
							///////服务小记保存完整
							/*std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(SAVED_SERVICELOG,GetHWND());
							pWnd->ShowModal();
							if(pWnd->m_bClickedBtnOK)///当前咨询窗口服务小记已保存，点确认关闭，再检测未读消息*/
							{
								if(pBtnRemind->IsVisible())////关闭选中的一个聊天框,有未读消息
								{
									std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
									pWnd->ShowModal();
									if(pWnd->m_bUnReadMsgShow)///确认关闭		
									{
										DuiLib::CControlUI* pC = pListElement->GetParent();
										DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
										int nIndex = pElement->GetIndex();

										CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录，写入临时log;
										int nUnReadCnt = _ttoi(strTxt);
										FRIEND_INFO* pFriend = m_vecFriend[nIndex];
										CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////更新未读消息，不写文件
										m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
										CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
									}	
									else 
									{
										CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
										return;
									}
								}
								else ////当前窗口无未读消息，直接关闭
								{
									//////////////////////////////////////////////////////
									DuiLib::CControlUI* pC = pListElement->GetParent();
									DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
									int nIndex = pElement->GetIndex();

									m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
									CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
								}
							}
							//else//取消
							//{
							//	return ;
							//}
						}
					}
					else//不是当前买家保存的服务小记
					{
						//提示当前买家没保存服务小记
						std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
						pWnd->ShowModal();
						////切换到当前买家右侧页
						DuiLib::CControlUI* pC = pListElement->GetParent();
						DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
						int nIndex = pElement->GetIndex();
						ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nIndex]);////选中当前会话窗口

						return ;
					}
				}
				else////对面不是买家,直接判断未读消息
				{
					if(pBtnRemind->IsVisible())////关闭选中的一个聊天框,有未读消息
					{
						std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
						pWnd->ShowModal();
						if(pWnd->m_bUnReadMsgShow)///确认关闭		
						{
							DuiLib::CControlUI* pC = pListElement->GetParent();
							DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
							int nIndex = pElement->GetIndex();

							CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录，写入临时log;
							int nUnReadCnt = _ttoi(strTxt);
							FRIEND_INFO* pFriend = m_vecFriend[nIndex];
							CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////更新未读消息，不写文件
							m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
							CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
						}	
						else 
						{
							CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
							return;
						}
					}
					else ////当前窗口无未读消息，直接关闭
					{
						//////////////////////////////////////////////////////
						DuiLib::CControlUI* pC = pListElement->GetParent();
						DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
						int nIndex = pElement->GetIndex();

						/////无未读消息
						//FRIEND_INFO* pFriend = m_vecFriend[nIndex];
						//map<FRIEND_INFO *,UINT>::iterator it;
						//it = g_mapMsgUnRead.find(pFriend);
						//if ( it!=g_mapMsgUnRead.end() )
						//{
						//	g_mapMsgUnRead.erase(it);////清除上次保存的当前窗口的未读消息
						//}
						/////////////////
						m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
						CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
					}
				}
			}
		}
		else if(msg.pSender->GetName() == _T("custMenuBtn")) //点击关闭图标关闭一个咨询框和聊天框
		{
			//待处理菜单
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint point = msg.ptMouse;
			ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("DuiCounselMenu.xml"), point, &m_PaintManager, &m_MenuCheckInfo);
		}
	}
	else if(msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{
		if(msg.pSender->GetName() == "editView")
		{
			SearchTextChange(msg.pSender->GetText().GetData());			
		}
	}
	else if(msg.sType == DUI_MSGTYPE_RETURN)
	{
		if(msg.pSender->GetName() == "editView")
		{
			if(m_pSearchListWnd != NULL)
				::PostMessage(GetHWND(), WM_MSG_ITEMCLICK, m_pSearchListWnd->GetCurSel(), 0);
		}
	}
	else if(msg.sType == "itemactivate")
	{
		if(msg.pSender->GetParent()->GetParent()->GetName() == "lsRecent")
		{	// 双击最近联系人
			m_pRecentList->OnLbnDblclk();
		}
		else if(msg.pSender->GetParent()->GetParent()->GetName() == "lsFriend")
		{
			OnSendMsg();
		}
	}
	else if(_tcsicmp(msg.sType, DUI_MSGTYPE_RBUTTONDOWN) == 0)
	{	// 右键点击
		if(strcmp(msg.pSender->GetClass(), "TreeNodeUI") == 0)
		{
			if(m_pLayoutPage->GetCurSel() == 2)
			{
				FL_ITEM_POS pos=m_pFriendList->GetCurSel();
				int nGroup = pos.nGroupID;
				if(nGroup == -1)//不是来自好友列表的消息不处理
					return;
				int nItem  = pos.nItem;

				FRIEND_INFO * pFriend = NULL;

				if ( nGroup>-1 && nItem>-1 )
					pFriend=(FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);

				CMenuEx menu, *popmenu;

				menu.LoadMenu(IDR_MENU_TEMPLATE);
				popmenu = menu.GetSubMenu(0);
				popmenu->RemoveMenu(0,MF_BYPOSITION);

				CPoint point;
				GetCursorPos(&point);

				TRACE("NM_FL_RBUTTONUP\n");

				// 根据不同情况来显示弹出菜单的内容

				// 判断当前是否有选中的组
				if ( nGroup > -1 )
				{
					if ( nItem > -1 ) //好友节点
					{
						CString strItem;

						CString strGroup = m_pFriendList->GetGroupName(nGroup);
						if(strGroup != "陌生人" && strGroup!= "黑名单")
						{
							if(pFriend->GetId() != g_ImUserInfo.GetId())
							{
								strItem.Format("发送即时消息");
								popmenu->AppendMenu(MF_STRING,ID_P2PSENDMSG,strItem);
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							}

							strItem.Format("修改备注名");
							popmenu->AppendMenu(MF_STRING,ID_MODF_COMMNAME,strItem);

							if(pFriend->GetId() != g_ImUserInfo.GetId())
							{
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							}

							//飞牛网组织架构中的不允许移动
							if(nGroup < FEINIU_GROUP_ID && nGroup > -1)
							{
								strItem.Format("删除联系人");
								popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,strItem);
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");

								CMenu menuGroup;
								menuGroup.CreatePopupMenu();
								//飞牛网组织架构中的不允许移动
								for(int i=0; i<g_GroupInfo.count;i++)//////修改判断逻辑，组织架构不允许变动 2014-11-12
								{
									if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
									{
										//陌生人和黑名单去掉
										if(/*g_GroupInfo.group[i].group_index!=1&&*/g_GroupInfo.group[i].group_index!=2)
										{
											menuGroup.AppendMenu(MF_STRING,ID_GROUP_CHOOSE_BEG + i,g_GroupInfo.group[i].name);
										}
									}
								}
								popmenu->AppendMenu(MF_POPUP, (UINT)menuGroup.m_hMenu, "把好友移动到...");
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");


								strItem.Format("移动至黑名单");
								popmenu->AppendMenu(MF_STRING,ID_REMOVE_BLACKLIST,strItem);
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							}


							if(pFriend->GetId() != g_ImUserInfo.GetId())
							{
								strItem.Format("查看资料");
								popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,strItem);
							}
						}
						else
						{
							popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,"从该组删除");
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");

							popmenu->AppendMenu(MF_STRING,ID_ADD_BLACKLIST,"加为好友");
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");

							popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,"查看资料");
						}
					}
					else//组节点
					{
						if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
						{
							CString strGroup = m_pFriendList->GetGroupName(nGroup);

							if (nGroup == MY_TEAM_GROUP_ID)
							{	// 我的团队分组只有"向组员群发消息"
								popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "向组员群发消息");
							}
							else
							{
								if (strGroup != "陌生人" && strGroup != "黑名单")
								{
									popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "向组员群发消息");
									//飞牛网组织架构中的不允许移动
									if (nGroup < 100 && nGroup > -1)
									{
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_ADDGROUP, "添加组");
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_ADD_CGROUP, "添加子组");
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_ADD_FRIEND, "添加好友");

										//我的好友不允许编辑
										if (nGroup != 0)
										{
											//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
											popmenu->AppendMenu(MF_STRING, ID_RENAME_GROUP, "重命名组");
											//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
											popmenu->AppendMenu(MF_STRING, ID_DELGROUP, "删除组");
										}
									}
								}
							}					
						}
					}
				}
				if(popmenu->GetMenuItemCount() > 0)
				{
					DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
					DuiLib::CPoint duiPoint(point.x,point.y);
					//ClientToScreen(m_hWnd, &point);
					pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
					AppendDuiMenu(pMenu, popmenu);
				}			
			}
		}
		else if(strcmp(msg.pSender->GetClass(), "ListContainerElementUI") == 0)
		{
			if(m_pLayoutPage->GetCurSel() == 1)
			{
				OnRecentListRButtonDown();
			}
		}
	}
}

void CDuiCSPlatDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI *)pInstance->m_PaintManager.FindControl("editView");
		if (background != NULL && pEdit != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;
			if(!bSolid)
			{
				if(strBkImage == "BKImage\\1.png")	strBkImage  = "BKImage\\c1.png";
				else if(strBkImage == "BKImage\\2.png") strBkImage = "BKImage\\c13.png";
				else if(strBkImage == "BKImage\\3.png") strBkImage = "BKImage\\c14.png";
				else if(strBkImage == "BKImage\\4.png") strBkImage = "BKImage\\c15.png";
				else if(strBkImage == "BKImage\\5.png") strBkImage = "BKImage\\c16.png";
				else if(strBkImage == "BKImage\\6.png") strBkImage = "BKImage\\c17.png";
				else if(strBkImage == "BKImage\\7.png") strBkImage = "BKImage\\c18.png";
				else if(strBkImage == "BKImage\\8.png") strBkImage = "BKImage\\c19.png";
				else if(strBkImage == "BKImage\\9.png") strBkImage = "BKImage\\c20.png";
				else if(strBkImage == "BKImage\\10.png") strBkImage = "BKImage\\c21.png";
				else strBkImage = "BKImage\\c1.png";
			}
			RECT rc;
			GetWindowRect(pInstance->GetHWND(), &rc);

			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18' dest='0,0,%d,%d'"), strBkImage.c_str(), rc.right-rc.left, background->GetFixedHeight());
			background->SetBkImage(szBuf);

			// 同时修改edit控件的背景色
			map<string, DWORD> mapEditBk;
			mapEditBk["BKImage\\c1.png"] = RGB(204,166,106);
			mapEditBk["BKImage\\c2.png"] = RGB(170,150,41);
			mapEditBk["BKImage\\c3.png"] = RGB(106,97,224);
			mapEditBk["BKImage\\c4.png"] = RGB(149,123,233);
			mapEditBk["BKImage\\c5.png"] = RGB(174,124,190);
			mapEditBk["BKImage\\c6.png"] = RGB(202,109,131);
			mapEditBk["BKImage\\c7.png"] = RGB(175,118,116);
			mapEditBk["BKImage\\c8.png"] = RGB(141,172,59);
			mapEditBk["BKImage\\c9.png"] = RGB(180,181,100);
			mapEditBk["BKImage\\c10.png"] = RGB(194,164,136);
			mapEditBk["BKImage\\c11.png"] = RGB(106,106,148);
			mapEditBk["BKImage\\c12.png"] = RGB(111,132,153);
			mapEditBk["BKImage\\c13.png"] = RGB(87,82,63);
			mapEditBk["BKImage\\c14.png"] = RGB(192,174,217);
			mapEditBk["BKImage\\c15.png"] = RGB(152,125,90);
			mapEditBk["BKImage\\c16.png"] = RGB(75,125,101);
			mapEditBk["BKImage\\c17.png"] = RGB(164,193,202);
			mapEditBk["BKImage\\c18.png"] = RGB(204,169,103);
			mapEditBk["BKImage\\c19.png"] = RGB(180,192,137);
			mapEditBk["BKImage\\c20.png"] = RGB(123,151,191);
			mapEditBk["BKImage\\c21.png"] = RGB(87,153,119);

			pEdit->SetNativeEditBkColor(mapEditBk[strBkImage]);
		}
	}
}

// 通过关键字查找好友
void CDuiCSPlatDlg::SearchFriend(const char *pKey, vector<int> &vecMeetFriend)
{
	vecMeetFriend.clear();
	if(pKey == NULL || strcmp(pKey, "") == 0)
	{
		return;
	}

	// 根据所选tab页，查找相应好友
	int nSel = m_pLayoutPage->GetCurSel();
	if(nSel == 0)
	{
		for(int nElement = 0; nElement < m_pCounselList->GetCount(); nElement++)
		{
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
			int nFriendID = pListElement->GetTag();
			FRIEND_INFO *pFriend = ::FindFriend(nFriendID);
			if ( pFriend != NULL )
			{	
				CString strName = UserInfoToStr(pFriend);
				BOOL bMeet = CTabWndSearch::KeySearch(pKey, strName);

				if(bMeet)
				{
					vecMeetFriend.push_back(nFriendID);
				}
			}
		}
	}
	else if(nSel == 1)
	{
		int nCount = m_pRecentList->GetCount();
		for(int i = 0; i < nCount; ++i)
		{
			DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI *)m_pRecentList->GetItemAt(i);
			FRIEND_INFO *pItem = NULL;
			CLUSTER_INFO *pCluster = NULL;
			CDuiRecentList::GetRealValue(pNode->GetTag(), pItem, pCluster);

			if ( pItem != NULL )
			{	// 好友
				CString strName = UserInfoToStr(pItem);
				BOOL bMeet = CTabWndSearch::KeySearch(pKey, strName);

				if(bMeet)
				{
					vecMeetFriend.push_back(pItem->GetId());
				}
			}
		}
	}
	else
	{	// 我的团队
		int nCount = m_pFriendList->GetCount();
		for(int i = 0; i < nCount; ++i)
		{
			DuiLib::CTreeNodeUI *pItem = (DuiLib::CTreeNodeUI *)m_pFriendList->GetItemAt(i);
			if(!m_pFriendList->IsGroupItem(pItem))
			{
				FRIEND_INFO *pFriend = (FRIEND_INFO*)pItem->GetTag();
				CString strName = UserInfoToStr(pFriend);
				BOOL bMeet = CTabWndSearch::KeySearch(pKey, strName);

				if(bMeet)
				{
					vecMeetFriend.push_back(pFriend->GetId());
				}
			}
		}
	}
}

// 查找关键字改变
void CDuiCSPlatDlg::SearchTextChange(const char *pKey)
{
	//if(strcmp(pKey, "") == 0)
	//{
	//	if(m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
	//		m_pSearchListWnd->Close(IDOK);

	//	// 搜索框重新获取焦点
	//	SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);

	//	return;
	//}

	// 查找出的好友
	vector<int> vecMeetFriend;
	SearchFriend(pKey, vecMeetFriend);

	// 弹出列表
	DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
	long nEndChar = 0;
	pEdit->GetSel(s_SearchEditStartChar, nEndChar);

	if(m_pSearchListWnd == NULL || !::IsWindow(m_pSearchListWnd->GetHWND()))
	{
		m_pSearchListWnd = std::tr1::shared_ptr<CSearchListWnd>(new CSearchListWnd(vecMeetFriend));
		m_pSearchListWnd->Create(m_PaintManager.GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW /*| WS_EX_TOPMOST*/, DuiLib::CDuiRect());
	}
	else
	{
		m_pSearchListWnd->Update(vecMeetFriend);
	}
	m_PaintManager.SetFocus(NULL);
	// HACK: Don't deselect the parent's caption
	HWND hWndParent = m_pSearchListWnd->GetHWND();
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);

	// 计算列表的位置
	CRect rcList;
	GetWindowRect(m_pSearchListWnd->GetHWND(), &rcList);
	
	CRect rc = pEdit->GetPos();
	CPoint pt(rc.left-24, rc.bottom+3);
	ClientToScreen(GetHWND(), &pt);
	::MoveWindow(m_pSearchListWnd->GetHWND(), pt.x, pt.y, rc.Width()+24, rcList.Height(), TRUE);
	m_pSearchListWnd->ShowWindow(true);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);

	// 搜索框重新获取焦点
	SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);
}

void CDuiCSPlatDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent) 
	{
	case TIMER_SERCHEDIT_SETFOCUS:
		{
			DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
			KillTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS);
			m_PaintManager.SetFocus(pEdit);
			pEdit->SetSel(s_SearchEditStartChar,s_SearchEditStartChar);
		}
		break;
	default:
		break;
	}
}

//更新显示好友列表
void CDuiCSPlatDlg::UpdateTeamList()
{
	//清空所有group 和组中Item
	m_pFriendList->DeleteAllItems();

	//groupInfo >> groupList， groupID最多到500
	bool bGroupAdd[500];
	memset(bGroupAdd, 0, sizeof(bGroupAdd));

	for ( int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId, nParentGroupId, nSubGroupIndex;
		deque<int> dequeId;

		nSubGroupId = g_GroupInfo.group[i].group_index;
		nParentGroupId = g_GroupInfo.group[i].group_father;

		if(nSubGroupId < FEINIU_GROUP_ID)
		{
			continue;
		}

		//须父组先创建
		do
		{
			dequeId.push_front(nSubGroupId);
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			if(nSubGroupIndex != -1)
			{
				nSubGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			}
			else//找不到父组
			{
				dequeId.clear();
				break;
			}
		}while(nSubGroupId != -1 && !bGroupAdd[nSubGroupId] && dequeId.size()<10);//这里有风险

		while( !dequeId.empty())
		{
			nSubGroupId = dequeId.front();
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			nParentGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			m_pFriendList->AddSubGroup(nParentGroupId, nSubGroupId, g_GroupInfo.group[nSubGroupIndex].name);
			//组状态
			m_pFriendList->SetGroupState(nSubGroupId,(FLS_STATE)g_GroupInfo.group[nSubGroupIndex].group_state);
			bGroupAdd[nSubGroupId] = true;
			dequeId.pop_front();
		}
	}

	//好友信息>> groupList
	FRIEND_LIST::iterator it;
	FRIEND_INFO * pFriend ;
	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
	{
		pFriend = (*it);
		if(pFriend == NULL)
		{
			ASSERT(pFriend);
			continue;
		}

		int nGroupID = pFriend->group_index;
		if(nGroupID < FEINIU_GROUP_ID)
		{
			continue;
		}

		// 是否有备注名称
		CString sName;
		if(pFriend->comment_name[0] == 0)
			sName = pFriend->nickname;
		else
			sName = pFriend->comment_name;

		m_pFriendList->AddItem(pFriend->group_index, sName, (DWORD)(pFriend));
		m_pFriendList->SetItemUmMsg(pFriend, false);//添加一个好友之后设置这个好友和父节点的红点的位置
	}

	//组内好友排序
	for(int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId = g_GroupInfo.group[i].group_index;
		if(nSubGroupId < FEINIU_GROUP_ID)
		{
			continue;
		}

		m_pFriendList->SortGroupByState(nSubGroupId);
	}
}

void CDuiCSPlatDlg::OnSendMsg()
{
	FL_ITEM_POS pos = m_pFriendList->GetCurSel();

	if ( pos.nItem!=-1 && pos.nGroupID!=-1 )
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);
		SetFriendUnMsg(pFriend, false);//打开聊天窗口对应的红点消失
		CMainFrame::OpenP2PMsgDlg(pFriend);		
	}
	else if(pos.nGroupID != -1)////展开组
	{
		BOOL bExpand = m_pFriendList->IsExpand(pos.nGroupID);
		m_pFriendList->ExpandGroup(pos.nGroupID, !bExpand);
	}
}

void CDuiCSPlatDlg::OnRecentListRButtonDown()
{
	CPoint pt;
	GetCursorPos(&pt);

	int nSel = m_pRecentList->GetCurSel();
	if ( nSel>-1 && nSel<m_pRecentList->GetCount() )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI *)m_pRecentList->GetItemAt(nSel);
		FRIEND_INFO *pItem = NULL;
		CLUSTER_INFO *pCluster = NULL;
		CDuiRecentList::GetRealValue(pNode->GetTag(), pItem, pCluster);

		if ( pItem != NULL )
		{
			CMenuEx menu, *popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CString str;

			FRIEND_INFO *pUser = (FRIEND_INFO *)pItem;

			CPoint pt;
			GetCursorPos(&pt);

			if ( pUser->GetId()!=g_ImUserInfo.GetId() )
			{
				str.Format("与 \"%s\" 单独聊天",(pUser->nickname));
				popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,str);	

				str.Format("查看 \"%s\" 的详细资料",(pUser->nickname));
				popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,str);	

				if ( FindFriend(pUser->GetId()) )
				{
					str.Format("和 \"%s\" 的聊天记录",(pUser->nickname));
					popmenu->AppendMenu(MF_STRING,ID_FRIEND_MSG,str);	
				}
				//if(pUser->group_index == 1)
				//{	// 陌生人
				//	str.Format("我要加 \"%s\" 为好友",(pUser->nickname));
				//	popmenu->AppendMenu(MF_STRING,ID_FRIEND_ADD,str);	
				//}
			}

			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint duiPoint(pt.x,pt.y);
			pMenu->Init(NULL, _T("loginstatus.xml"), pt, &m_PaintManager);
			AppendDuiMenu(pMenu, popmenu);
		}
		if(pCluster != NULL)
		{
			CMenuEx menu, *popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CString str;
			str.Format("进入群 \"%s\" 的聊天室",pCluster->szFamilyName);
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_TALK,str);	

			str.Format("查看群 \"%s\" 资料",pCluster->szFamilyName);
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_INFO,str);	

			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSG,"历史聊天记录");	

			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint duiPoint(pt.x,pt.y);
			pMenu->Init(NULL, _T("loginstatus.xml"), pt, &m_PaintManager);
			AppendDuiMenu(pMenu, popmenu);
		}
	}
}

LRESULT CDuiCSPlatDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
{  
	if (uMsg == WM_MENUCLICK)  
	{ 
		DuiLib::CDuiString *strMenuName = (DuiLib::CDuiString*)wParam;
		BOOL bChecked = (BOOL)lParam;		 
		int nID = atoi(strMenuName->GetData());

		switch (nID)
		{
		case ID_P2PSENDMSG:	OnSendMsg();	break;
		case ID_FRIENDINFO:	OnFriendInfo();	break;
		case ID_MODF_COMMNAME:	OnModfCommName();	break;
		case ID_SEND_GROUP_MESSAGE:	OnSendGroupMessage();	break;
		case ID_SEND_P2PMSG: {m_pRecentList->OnP2PTalk();	}	break;
		case ID_FRIEND_INFO: {m_pRecentList->OnFriendInfo();	}	break;
		case ID_FRIEND_MSG: {m_pRecentList->OnFriendMsg();	}	break;
		case ID_FRIEND_ADD: {m_pRecentList->OnFriendAdd();	}	break;
		case ID_CLUSTER_TALK: {m_pRecentList->OnClusterTalk();	}	break;
		case ID_CLUSTER_INFO: {m_pRecentList->OnClusterInfo();	}	break;
		case ID_CLUSTER_MSG: {m_pRecentList->OnClusterMsg();	}	break;
		case ID_CLEAR_RECENT: {m_pRecentList->OnClearRecent();	CMainFrame::GetInstance()->GetRecentList()->OnClearRecent();}	break;
		default:	break;
		}

		if ( *strMenuName == _T("MenuAll")) //显示全部咨询框
		{
			ShowAllCounselDlg();
			m_MenuCheckInfo[_T("MenuAll")] = true;
			m_MenuCheckInfo[_T("MenuPorblem")] = false;
		}
		else if ( *strMenuName == _T("MenuPorblem")) //显示待处理咨询框
		{
			BOOL bIsExist = FALSE;
			for(int nElement = 0; nElement < /*m_vecListElement.size()*/m_pCounselList->GetCount(); nElement++)
			{
				DuiLib::CListContainerElementUI* pListElement = /*m_vecListElement[nElement];*/(DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
				DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));
				if(pBtnMarked->IsVisible())
				{
					bIsExist = TRUE;
					break;
				}
			}
			if(!bIsExist)
			{
				CDuiCounselNone::NewDlg();
				m_MenuCheckInfo[_T("MenuAll")] = true;
				m_MenuCheckInfo[_T("MenuPorblem")] = false;
			}
			else
			{
				ShowCounselingDlg();
				m_MenuCheckInfo[_T("MenuAll")] = false;
				m_MenuCheckInfo[_T("MenuPorblem")] = true;
			}
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)//点击咨询框聊天框跟随置顶
	{
		CRect rcCounsel;
		::GetWindowRect(*this, &rcCounsel);
		//没有选中咨询框则默认选中第一个咨询框
#ifdef _CS_PLAT_EX_
		if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 0)
		{
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL)
			{
				CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg; 
				//if(pDlg != NULL && pDlg->GetHWND() != NULL)
				//{
				//	CRect rcDlg;
				//	::GetWindowRect(pDlg->GetHWND(), rcDlg);
				//	pDlg->ShowWindow(SW_SHOW);
				//	::SetWindowPos(pDlg->GetHWND(),*this, rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
				//	::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(), TRUE);
				//}
			}
		}
#else
		if(m_vecP2PSendMsgDlg.size() > 0)
		{
			/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = NULL; 
			BOOL bIsDlgShow = FALSE;
			if(m_pCounselList->GetCurSel() == -1)
			{
				for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
				{
					pDlg = m_vecP2PSendMsgDlg[nDlg];
					if(::IsWindowVisible(pDlg->GetHWND()))
					{
						bIsDlgShow = TRUE;
						::BringWindowToTop(pDlg->GetHWND());
						break;
					}
				}
				if(!bIsDlgShow)
				{
					pDlg = m_vecP2PSendMsgDlg[0];
				}
			}
			else if( m_pCounselList->GetCurSel() >= 0 )
			{
				pDlg = m_vecP2PSendMsgDlg[m_pCounselList->GetCurSel()];
			}
			if(pDlg != NULL)
			{
				CRect rcDlg;
				/*pDlg->*/::GetWindowRect(pDlg->GetHWND(), rcDlg);
				pDlg->ShowWindow(SW_SHOW);
				::SetWindowPos(pDlg->GetHWND(),*this, rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
				/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(), TRUE);
			}
		}
#endif
	}

	bHandled = false;  
	return DuiLib::WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

void CDuiCSPlatDlg::OnFriendInfo()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);

	CFriendInfoDlg::NewDlg(pFriend);
}

void CDuiCSPlatDlg::OnModfCommName()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}
	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	if(pFriend==NULL)
		return;

	tr1::shared_ptr<CModfCommNameDlg> pDlg = CModfCommNameDlg::Show(pFriend);
	pDlg->CenterWindow();
	pDlg->ShowWindow(SW_SHOW);
}

//向组员发消息
void CDuiCSPlatDlg::OnSendGroupMessage()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	FL_GROUP_INFO *pGroup = m_pFriendList->GetGroupData(nGroup);
	if(pGroup == NULL)
		return;

	// 同时将子组的好友也添加到聊天窗口
	vector<FL_GROUP_INFO*> vecSubGroup;
	pGroup->GetSubGroup(vecSubGroup);
	int nGroupIndex = pGroup->dwData;
	FRIEND_LIST::iterator it,itE;
	it=g_lsFriend.begin();
	itE=g_lsFriend.end();
	// 如果选择的是陌生人和黑名单则不处理
	if(nGroupIndex == 1 || nGroupIndex == 2)
	{
		return ;
	}
	// 找到该组内的好友
	DuiLib::CTreeNodeUI *pGroupNode = m_pFriendList->FindGroupNode(pGroup->dwData);
	vector<int> vecFriendID;
	int nChildCount = pGroupNode->GetCountChild();
	for(int i = 0; i < nChildCount; ++i)
	{
		DuiLib::CTreeNodeUI *pChildNode = pGroupNode->GetChildNode(i);
		if(!m_pFriendList->IsGroupItem(pChildNode))
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)pChildNode->GetTag();
			if(pFriend->GetId() != g_ImUserInfo.GetId())
			{
				vecFriendID.push_back(pFriend->GetId());
			}
		}
	}

	CGroupSendDlg::Show(vecFriendID);
}

void CDuiCSPlatDlg::AddFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind, bool bBtnMarkeing, bool bBtnMarked)
{
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		if(m_vecFriend[nFriend] == pFriend)
			return;
	}

	//好友改为内存中的好友
	FRIEND_INFO* pRealFriend = FindFriend(pFriend->GetId());
	m_vecFriend.push_back(pRealFriend != NULL ? pRealFriend : pFriend);

	DuiLib::CDuiString str;
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder builder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(builder.Create(_T("DuiCSPlatDlgListElement.xml"), (UINT)0, NULL));

	//添加咨询框单元时画左边和下面的颜色
	//DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
	//CString strBkImage = background->GetBkImage();
	//DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
	//pLineItem->SetBkImage(strBkImage);

	DuiLib::CButtonUI* pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnClose")));
	pBtnClose->SetVisible(false);
	vector<DuiLib::CButtonUI*> vecMousemoveBtn;
	vecMousemoveBtn.push_back(pBtnClose);
	pListElement->SetMousemoveBtnVisible(vecMousemoveBtn);

	DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
	DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
	DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
	DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
	DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
	DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
	DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

	pBtnMessage->SetVisible(bIsMessage);
	pBtnTransfer->SetVisible(bIsTransfer);
	pBtnRepeat->SetVisible(bIsRepeat);
	pBtnReply->SetVisible(bIsReply);
	pBtnRemind->SetVisible(bIsRemind);

	pBtnMarkeing->SetVisible(false);
	pBtnMarked->SetVisible(false);

	pBtnMarkeing->SetVisible(true);//第一次添加显示白旗
	pListElement->SetTag(pFriend->GetId());
	m_pCounselList->Add(pListElement);
	//m_vecListElement.push_back(pListElement);

	m_PaintManager.SetDefaultFont("微软雅黑", 12, false, false, false);
	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	m_PaintManager.AddFont("微软雅黑", 12, true, false, false);

	DuiLib::CButtonGifUI* pIcon = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("lbHeaderIcon")));
	CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	CString strGifFile =  pFriend->face_url;
	if(strGifFile.Find(".gif") != -1)
	{
		strGifFile =  strFile + ".gif";
		if ( !FileExist(strGifFile) )///不存在时创建
		{
			CopyFile(strFile,strGifFile,FALSE);
		}
	}
	if ( FileExist(strFile) )
	{
		BOOL bOnline = TRUE;
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			bOnline = TRUE;
		}
		else if(pFriend->status == XTREAM_OFFLINE || pFriend->status == XTREAM_HIDE)
		{
			bOnline = FALSE;
		}
		if ( FileExist(strGifFile))/////GIF在线
		{
			if(bOnline)
			{
				pIcon->SetBkImage("");
				pIcon->SetNormalGifFile(strGifFile);
			}
			else//不在线
			{
				string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////是否在线的判断
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}
		}
		else///非GIF
		{
			string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////是否在线的判断
			pIcon->SetBkImage(strHeadIcon.c_str());
			pIcon->SetNormalGifFile("");
		}
	}

	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//超过18个字则显示...
// 		if(str.GetLength() >= 16)
// 		{
// 			BYTE bChar = str.GetAt(12);
// 			if(bChar > 127)
// 				str = str.Left(12) + "...";
// 			else
// 				str = str.Left(13) + "...";
// 		}
		nick_name->SetFont(0);
		nick_name->SetText(str);
	}

	m_mapFriendCount[pFriend] = 1;

	if(m_MenuCheckInfo[_T("MenuPorblem")])
	{
		pListElement->SetVisible(false);
	}
}

void CDuiCSPlatDlg::AddP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg)
{
	//判断聊天框重复
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
	{
		if(m_vecP2PSendMsgDlg[nDlg] == pDlg)
			return;
	}
	m_vecP2PSendMsgDlg.push_back(pDlg);
}

void CDuiCSPlatDlg::SetFriendListElementTranPre(FRIEND_INFO* pFriend, bool bIsTranPre)
{
	int nIndex = -1;
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		if(m_vecFriend[nFriend] == pFriend)
		{
			nIndex = nFriend;
			break;
		}
	}
	if(nIndex == -1)
	{
		return;
	}

	DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nIndex);
//#ifndef _CS_PLAT_EX_
//	if(m_pCounselList->GetCount() == 1)
//	{
//		m_pCounselList->SelectItem(0);
//	}
//#endif
	//if(nIndex != 0)
	//{
	//	for(int nElement = nIndex; nElement > 0; nElement--)
	//	{
	//		m_pCounselList->SetItemIndex(((DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement)), nElement-1);
	//	}

	//	//pList->SetItemIndex(pListElement, 0);

	//	FRIEND_INFO* pFriendChange = m_vecFriend[nIndex];
	//	vector<FRIEND_INFO*>::iterator itrFriendDel = m_vecFriend.begin();
	//	for(; itrFriendDel != m_vecFriend.end(); itrFriendDel++)
	//	{
	//		if(*itrFriendDel == pFriendChange)
	//		{
	//			m_vecFriend.erase(itrFriendDel);
	//			break;
	//		}
	//	}
	//	vector<FRIEND_INFO*>::iterator itrFriendInsert = m_vecFriend.begin();
	//	m_vecFriend.insert(itrFriendInsert, pFriendChange);////置于最前

	//	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgChange = m_vecP2PSendMsgDlg[nIndex];
	//	vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itrDlgDel = m_vecP2PSendMsgDlg.begin();
	//	for(; itrDlgDel != m_vecP2PSendMsgDlg.end(); itrDlgDel++)
	//	{
	//		if(*itrDlgDel == pDlgChange)
	//		{
	//			m_vecP2PSendMsgDlg.erase(itrDlgDel);
	//			break;
	//		}
	//	}
	//	vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itrDlgInsert = m_vecP2PSendMsgDlg.begin();
	//	m_vecP2PSendMsgDlg.insert(itrDlgInsert, pDlgChange);////对话框置于最前
	//}

	DuiLib::CLabelUI* pLabelNickname = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	DuiLib::CHorizontalLayoutUI* pHLBtn = static_cast<DuiLib::CHorizontalLayoutUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("HLBtn")));
	int nWidthBtn = pHLBtn->GetFixedWidth();
	int nWidthNickname = pLabelNickname->GetFixedWidth();
	
	DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
	DuiLib::CButtonUI* pBtnTranPre = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTranPre")));

	if(bIsTranPre && !pBtnTranPre->IsVisible() && !pBtnTransfer->IsVisible())
	{
		nWidthBtn += 20;
		nWidthNickname -= 20;
	}
	
	if(pBtnTransfer->IsVisible())
	{
		pBtnTransfer->SetVisible(false);
	}

	pHLBtn->SetFixedWidth(nWidthBtn);
	pLabelNickname->SetFixedWidth(nWidthNickname);

	
	pBtnTranPre->SetVisible(bIsTranPre);
}

void CDuiCSPlatDlg::SetFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind, int nUnReadNum)
{
	int nIndex = -1;
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		if(m_vecFriend[nFriend] == pFriend)
		{
			nIndex = nFriend;
			break;
		}
	}
	if(nIndex == -1)
	{
		return;
	}

	if(nUnReadNum > 0)
	{
		m_mapFriendCount[pFriend] = nUnReadNum + 1;
	}

	//如果为当前聊天的买家，未读消息不累加
	BOOL bIsSelectDlg = FALSE;
	if(g_pMessageHandleWnd != NULL && (g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1 || g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetFriendInfo()->GetId() == pFriend->GetId()) )
	{
		//m_mapFriendCount[pFriend] = 1;
		bIsSelectDlg = TRUE;
	}

	DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nIndex);
#ifndef _CS_PLAT_EX_
	if(m_pCounselList->GetCount() == 1)
	{
		m_pCounselList->SelectItem(0);
	}
#endif
	if(nIndex != 0)
	{
		for(int nElement = nIndex; nElement > 0; nElement--)
		{
			m_pCounselList->SetItemIndex(((DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement)), nElement-1);
		}

		//pList->SetItemIndex(pListElement, 0);

		FRIEND_INFO* pFriendChange = m_vecFriend[nIndex];
		vector<FRIEND_INFO*>::iterator itrFriendDel = m_vecFriend.begin();
		for(; itrFriendDel != m_vecFriend.end(); itrFriendDel++)
		{
			if(*itrFriendDel == pFriendChange)
			{
				m_vecFriend.erase(itrFriendDel);
				break;
			}
		}
		vector<FRIEND_INFO*>::iterator itrFriendInsert = m_vecFriend.begin();
		m_vecFriend.insert(itrFriendInsert, pFriendChange);////置于最前

		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgChange = m_vecP2PSendMsgDlg[nIndex];
		vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itrDlgDel = m_vecP2PSendMsgDlg.begin();
		for(; itrDlgDel != m_vecP2PSendMsgDlg.end(); itrDlgDel++)
		{
			if(*itrDlgDel == pDlgChange)
			{
				m_vecP2PSendMsgDlg.erase(itrDlgDel);
				break;
			}
		}
		vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itrDlgInsert = m_vecP2PSendMsgDlg.begin();
		m_vecP2PSendMsgDlg.insert(itrDlgInsert, pDlgChange);////对话框置于最前
	}
	
	DuiLib::CLabelUI* pLabelNickname = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
 	DuiLib::CHorizontalLayoutUI* pHLBtn = static_cast<DuiLib::CHorizontalLayoutUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("HLBtn")));
 	int nWidthBtn = 5;
	int nWidthNickname = 141;
	if(bIsReply)
	{
		nWidthBtn += 20;
		nWidthNickname -= 20;
	}
	if(bIsTransfer)
	{
		nWidthBtn += 20;
		nWidthNickname -= 20;
	}
	if(bIsRepeat)
	{
		nWidthBtn += 20;
		nWidthNickname -= 20;
	}
	if(bIsMessage)
	{
		nWidthBtn += 20;
		nWidthNickname -= 20;
	}
	if((bIsRemind || nUnReadNum > 0) && !bIsSelectDlg)
	{
		nWidthBtn += 20;
		nWidthNickname -= 20;
	}
	pHLBtn->SetFixedWidth(nWidthBtn);
	pLabelNickname->SetFixedWidth(nWidthNickname);


	if(!bIsReply && !bIsTransfer && !bIsRepeat && !bIsMessage&& !bIsRemind&&nUnReadNum==0)
	{
		DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
		pBtnReply->SetVisible(false);
		return;
	}

	DuiLib::CDuiString str;



	DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
	DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
	DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
	DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
	DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
	DuiLib::CButtonUI* pBtnTranPre = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTranPre")));

	pBtnMessage->SetVisible(bIsMessage);
	pBtnTransfer->SetVisible(bIsTransfer);
	pBtnRepeat->SetVisible(bIsRepeat);
	//判断是不是回复,如果是，则未读消息重置
	if(bIsReply /*&& m_mapFriendCount[pFriend] > 1*/)
	{
		pBtnReply->SetVisible(true);
		pBtnRepeat->SetVisible(false);
	}
	else
	{
		if(nUnReadNum>0)
		{
			pBtnRemind->SetVisible(true);
			CString strTxt = _T("");
			strTxt.Format(_T("%d"),nUnReadNum);
			pBtnRemind->SetText(strTxt);
			pListElement->SetDrawBkColor(0xFFFEE8C0);
		}
		else
		{
			pBtnReply->SetVisible(false);
		}
	}

	//判断是否为重复咨询
	if(!bIsTransfer && !bIsReply)
	{
		BOOL bExist = FALSE;
		MSG_LIST mlRecordAll;
		g_LocalData.GetMsgList(pFriend->GetId(), CDataManager::USER, mlRecordAll);
		int nCountAll = mlRecordAll.GetCount();

		for(int nCount = nCountAll-1; nCount > 0; nCount--)
		{
			POSITION pos = mlRecordAll.FindIndex(nCount);
			ASSERT(pos);
			if(pos == NULL)
				break;
			XT_MSG &msg = mlRecordAll.GetAt(pos);
			if(msg.from_id == pFriend->GetId())
			{
				POSITION posLast = mlRecordAll.FindIndex(nCountAll-1);
				XT_MSG &msgLast = mlRecordAll.GetAt(posLast);

				uint32 tm = msgLast.send_time - msg.send_time;
				if(tm < 172800)//和上一条记录对比如果在两天以内则为重复咨询
				{
					bIsRepeat = true;
					int nWidthBtn = pHLBtn->GetFixedWidth();
					int nWidthNickname = pLabelNickname->GetFixedWidth();
					nWidthBtn += 20;
					nWidthNickname -= 20;
					pHLBtn->SetFixedWidth(nWidthBtn);
					pLabelNickname->SetFixedWidth(nWidthNickname);
					break;
				}
				break;
			}
		}
	}
	pBtnRepeat->SetVisible(bIsRepeat);
	if(nUnReadNum==0)////不是未读消息
	{
		//未读消息最多99条
		pBtnRemind->SetVisible(bIsRemind);
		CString strCount;
		if(m_mapFriendCount[pFriend] >= 99)
			strCount = "99+";
		else
			strCount.Format("%d", m_mapFriendCount[pFriend]++);
		pBtnRemind->SetText(strCount);
		if(bIsSelectDlg)
		{
			m_mapFriendCount[pFriend] = 1;
			pBtnRemind->SetVisible(false);
		}

		DuiLib::CControlUI* pListItem = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("listitem")));
		pListElement->SetDrawBkColor(0xFFFEE8C0);
#ifndef _CS_PLAT_EX_
		if(!pBtnRemind->IsVisible())///不可见
		{
			m_mapFriendCount[pFriend] = 1;
			pListElement->SetDrawBkColor(NULL);
		}
#else
		//我的团队打开的时候pBtnRemind判断为隐藏的所以加上bIsRemind判断
		if(!pBtnRemind->IsVisible() && !bIsRemind)
		{
			m_mapFriendCount[pFriend] = 1;
			pListElement->SetDrawBkColor(NULL);
		}
#endif

		//隐藏一下再显示，否则显示出来的颜色没有覆盖全部的位置
		//if((DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(m_pCounselList->GetCurSel()) != pListElement)
		//{
		//	pListElement->SetVisible(false);
		//	pListElement->SetVisible(true);
		//}
	}
	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
// 	m_PaintManager.SetDefaultFont("微软雅黑", 12, false, false, false);//默认字体
// 	m_PaintManager.AddFont("微软雅黑", 12, true, false, false);//添加字体

	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//超过18个字则显示...
// 		if(str.GetLength() >= 16)
// 		{
// 			BYTE bChar = str.GetAt(12);
// 			if(bChar > 127)
// 				str = str.Left(12) + "...";
// 			else
// 				str = str.Left(13) + "...";
// 		}
		nick_name->SetFont(0);
		nick_name->SetText(str);
	}
}

void CDuiCSPlatDlg::MoveP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg)
{
	CRect rcCounsel;
	::GetWindowRect(GetHWND(), &rcCounsel);

	CRect rcDlg;
	/*pDlg->*/::GetWindowRect(pDlg->GetHWND(), rcDlg);
	/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcCounsel.Height(), TRUE);
}

void CDuiCSPlatDlg::ShowSelectP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgSelect,int nCntUnRead)
{
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size();  nDlg++)
	{
		//未选中的聊天框都隐藏
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = m_vecP2PSendMsgDlg[nDlg];
		if(pDlgSelect != pDlg)
		{
			//DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
			//CString strBkImage = background->GetBkImage();

			::ShowWindow(pDlg->GetHWND(), SW_HIDE);
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nDlg);
			//DuiLib::CControlUI* pSelectItemColor = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("selectitem")));

			//pSelectItemColor->SetBkImage(strBkImage);
			//pSelectItemColor->SetBkColor(NULL);

			//DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
			//pLineItem->SetBkImage(strBkImage);
		}
		else
		{
			BOOL bIsIconic = FALSE;

			for(int nDlgIconic = 0; nDlgIconic < m_vecP2PSendMsgDlg.size(); nDlgIconic++)
			{
				if(/*m_vecP2PSendMsgDlg[nDlgIconic]->IsIconic()*/::IsMinimized(m_vecP2PSendMsgDlg[nDlgIconic]->GetHWND()))
				{
					bIsIconic = TRUE;
					::ShowWindow(m_vecP2PSendMsgDlg[nDlgIconic]->GetHWND(), SW_RESTORE);
					/*m_vecP2PSendMsgDlg[nDlgIconic]->*/::BringWindowToTop(m_vecP2PSendMsgDlg[nDlgIconic]->GetHWND());
					break;
				}
			}

			CRect rcCounsel;
			::GetWindowRect(*this, &rcCounsel);
			CRect rcDlg;
			/*pDlg->*/::GetWindowRect(pDlg->GetHWND(), rcDlg);
			pDlg->ShowWindow(SW_SHOW);
			/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(), TRUE);
			//if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1 && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
			//{
			//	//如果是第一次打开客户聊天窗口则左侧页随之移动到中间
			//	pDlg->CenterWindow();
			//	CRect rc;
			//	GetWindowRect(pDlg->GetHWND(), &rc);
			//	::MoveWindow(CDuiCSPlatDlg::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
			//}
			SetTimer(pDlg->GetHWND(), TIMER_EDIT_ENDDOWN, 100, NULL);
			m_pDlgSelectP2PSendMsgDlg = pDlg;
			g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;//设置当前显示的聊天窗口
			//pList->SelectItem(nDlg);

			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nDlg);
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)
			{
				if(CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindow(CDuiCSPlatDlg::GetInstance()->GetHWND()))
				{
					if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->GetHWND() != NULL)
						CDuiCSPlatDlg::GetInstance()->Close();////关闭咨询框
				}
				return;
			}

			DuiLib::CDuiString str;

			DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
			DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
			DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
			DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
			DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));

			//DuiLib::CControlUI* pSelectItemColor = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("selectitem")));
			//DWORD dwColor = 0xFFCF4C5A;
			//pSelectItemColor->SetBkImage(_T(""));
			//pSelectItemColor->SetBkColor(dwColor);

			//DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
			//CString strBkImage = background->GetBkImage();
			//DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
			//pLineItem->SetBkImage(strBkImage);

			if(m_bIsUpdate)
			{
				m_bIsUpdate = FALSE;
			}
			else
			{
				if(nCntUnRead == 0)////没有未读消息
				{
					pBtnMessage->SetVisible(false);
					pBtnTransfer->SetVisible(false);
					pBtnRepeat->SetVisible(false);
					pBtnRemind->SetVisible(false);

					DuiLib::CControlUI* pListItem = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("listitem")));
					pListElement->SetDrawBkColor(NULL);
					//m_nRemindCount = 1;
					//选中聊天窗口时消息数变为1
					m_mapFriendCount[m_vecFriend[nDlg] ] = 1;
				}
			}
			if(m_pCounselList->GetCurSel() != nDlg)
			{
				m_bIsUpdate = TRUE;
				m_pCounselList->SelectItem(nDlg);
			}

			//设置当前聊天的买家
			XT_MONITOR_SET_BUYER_REQ req;
			req.idSeller = g_ImUserInfo.GetId();
			req.idBuyer = pDlg->GetFriendInfo()->GetId();
			g_pProtocol->SendReq(req);

			//tab页选择用户
			SetLayoutPageCust();
		}
	}
}

void CDuiCSPlatDlg::DeleteP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg)
{
	vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itr = m_vecP2PSendMsgDlg.begin();
	vector<FRIEND_INFO*>::iterator itrFriend = m_vecFriend.begin();
	//vector<DuiLib::CListContainerElementUI*>::iterator itrElement = m_vecListElement.begin();
	for( int nDlg = 0;
		itr != m_vecP2PSendMsgDlg.end(), nDlg < m_vecP2PSendMsgDlg.size(), itrFriend != m_vecFriend.end() /*, itrElement != m_vecListElement.end()*/;
		itr++, itrFriend++/*,itrElement++*/, nDlg++)
	{
		if(*itr == pDlg)
		{
			/*BOOL bExist = FALSE;
			map<FRIEND_INFO*, int>::iterator itrCount = m_mapFriendCount.begin();
			for(; itrCount != m_mapFriendCount.end(); itrCount++)
			{
				if(itrCount->first == *itrFriend)
				{
					itrCount->second++;
					bExist = TRUE;
					break;
				}
			}
			if(!bExist)
			{
				m_mapFriendCount.insert(make_pair(*itrFriend, 1));
			}*/

			m_vecP2PSendMsgDlg.erase(itr);
			m_vecFriend.erase(itrFriend);
			//m_vecListElement.erase(itrElement);
			m_pCounselList->RemoveAt(nDlg);
			//关闭一个咨询框显示上一个咨询框
			if(/*pDlg != NULL && pDlg->IsIconic() &&*/ nDlg != 0)
			{
				m_bIsUpdate = TRUE;
				m_pCounselList->SelectItem(nDlg-1);
				//m_vecP2PSendMsgDlg[nDlg-1]->ShowWindow(SW_SHOW/*NORMAL*/);
				//m_vecP2PSendMsgDlg[nDlg-1]->CenterWindow();
			}
			else
			{
				// 				if(m_vecListElement.size() > 0)
				// 				{
				// 					m_bIsUpdate = TRUE;
				// 					pList->SelectItem(nDlg);
				// 				}
			}
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)//所有窗口都关闭隐藏左侧页
			{
				ShowWindow(false);
			}
			break;
		}
	}
}

void CDuiCSPlatDlg::MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent)
{
	//所有窗口大小变化，防止选择其他聊天框时大小还原
	for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
		if(pDlgCurrent != pDlg)
		{
			CRect rcCurrent;
			/*pDlgCurrent->*/::GetWindowRect(pDlgCurrent->GetHWND(), &rcCurrent);
			/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCurrent.left, rcCurrent.top, rcCurrent.Width(), rcCurrent.Height(), TRUE);
		}
	}
}

void CDuiCSPlatDlg::SetAllWindowMax(BOOL bIsMax)
{
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
	{
		CDuiP2PSendMsgDlg* pDlg = m_vecP2PSendMsgDlg[nDlg];
		pDlg->SetWindowMax(bIsMax);
	}
}

void CDuiCSPlatDlg::UpdateFriendListElement(FRIEND_INFO* pFriend)
{
	int nIndex = -1;
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		//如果ID一样则好友列表中的好友替换为内存中的好友
		if(m_vecFriend[nFriend]->GetId() == pFriend->GetId())
		{
			m_vecFriend[nFriend] = pFriend;
			nIndex = nFriend;
			break;
		}
	}
	if(nIndex == -1)
	{
		return;
	}
	DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nIndex);

	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	m_PaintManager.SetDefaultFont("微软雅黑", 12, false, false, false);//默认字体
	m_PaintManager.AddFont("微软雅黑", 12, true, false, false);//添加字体

	//更新头像颜色
	DuiLib::CButtonGifUI* pIcon = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("lbHeaderIcon")));
	CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	CString strGifFile =  pFriend->face_url;
	if(strGifFile.Find(".gif") != -1)
	{
		strGifFile =  strFile + ".gif";
		if ( !FileExist(strGifFile) )///不存在时创建
		{
			CopyFile(strFile,strGifFile,FALSE);
		}
	}
	if ( FileExist(strFile) )
	{
		BOOL bOnline = TRUE;
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			bOnline = TRUE;
		}
		else if(pFriend->status == XTREAM_OFFLINE || pFriend->status == XTREAM_HIDE)
		{
			bOnline = FALSE;
		}
		if ( FileExist(strGifFile))/////GIF在线
		{
			if(bOnline)
			{
				pIcon->SetBkImage("");
				pIcon->SetNormalGifFile(strGifFile);
			}
			else//不在线
			{
				string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////是否在线的判断
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}
		}
		else///非GIF
		{
			string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////是否在线的判断
			pIcon->SetBkImage(strHeadIcon.c_str());
			pIcon->SetNormalGifFile("");
		}
	}

	DuiLib::CDuiString str;
	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//超过18个字则显示...
// 		if(str.GetLength() >= 16)
// 		{
// 			BYTE bChar = str.GetAt(12);
// 			if(bChar > 127)
// 				str = str.Left(12) + "...";
// 			else
// 				str = str.Left(13) + "...";
// 		}
		nick_name->SetFont(0);
		nick_name->SetText(str);
	}
}

//判断传入的窗口和当前选中的窗口是否一致
BOOL CDuiCSPlatDlg::IsCurrentShowDlg(CDuiP2PSendMsgDlg* pDlg)
{
	if(m_pCounselList != NULL)
	{
		for (int nElement = 0; nElement < m_pCounselList->GetCount(); nElement++)
		{
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
			if(pListElement->IsSelected() && m_vecP2PSendMsgDlg[nElement] == pDlg)
				return TRUE;
		}
	}
	return FALSE;
}

void CDuiCSPlatDlg::ShowAllCounselDlg()
{
	for(int nElement = 0; nElement < /*m_vecListElement.size()*/m_pCounselList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = /*m_vecListElement[nElement]*/(DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt( nElement );
		pListElement->SetVisible(true);
	}
	//没有选中咨询框则默认选中第一个咨询框
	if( m_pCounselList->GetCurSel() == -1 && m_vecP2PSendMsgDlg.size() > 0 )
	{
		for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
		{
			if(::IsWindowVisible(m_vecP2PSendMsgDlg[nDlg]->GetHWND()))
			{
				m_bIsUpdate = TRUE;
				m_pCounselList->SelectItem(nDlg);
			}
		}
	}
}

void CDuiCSPlatDlg::ShowCounselingDlg()
{
	for(int nElement = 0; nElement < /*m_vecListElement.size()*/m_pCounselList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = /*m_vecListElement[nElement];*/(DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);

		DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
		DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
		DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
		DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

		//留言、重复咨询、未读消息、转接消息、红旗表示待处理
		if(!(/*pBtnMessage->IsVisible() || pBtnRepeat->IsVisible() || / *pBtnReply->IsVisible()* /pBtnRemind->IsVisible() || pBtnTransfer->IsVisible() ||*/ pBtnMarked->IsVisible()))
			pListElement->SetVisible(false);
	}
	DuiLib::CListContainerElementUI* pListSelect = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt( m_pCounselList->GetCurSel() );

	if( pListSelect == NULL )
	{
		for(int nElement = 0; nElement < m_pCounselList->GetCount(); nElement++)
		{
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);//m_vecListElement[nElement];
			if(pListElement->IsVisible())
			{
				m_bIsUpdate = TRUE;
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nElement]);
				break;
			}
		}
	}
}

void CDuiCSPlatDlg::OnModFCNameAck(const char* data,WORD wRet)
{
	XT_MOD_FRIEND_COMMNAME_ACK *pAck=(XT_MOD_FRIEND_COMMNAME_ACK*)data;
	if(wRet==0)
	{
		ModifyName(pAck);
	}
}

void CDuiCSPlatDlg::ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck)
{
	FRIEND_LIST::iterator fIt;

	for ( fIt=g_lsFriend.begin();
		fIt!=g_lsFriend.end();
		fIt++ )
	{
		FRIEND_INFO *p=(*fIt);
		if ( p->GetId()==pAck->fid )
		{
			strcpy(p->comment_name,pAck->commName);
			break;
		}
	}	

	vector<FL_GROUP_INFO *> vecGroup;
	int nGroupCount = m_pFriendList->GetAllGroup(vecGroup);

	for(int i = 0; i < nGroupCount; ++i)
	{
		FL_GROUP_INFO *group = vecGroup[i];
		int nGroup = group->dwData;
		int nItems = group->vecItem.size();

		for ( int nItem=0; nItem<nItems; nItem++)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);

			if ( pAck->fid==pFriend->GetId())
			{
				m_pFriendList->SetItemText(nGroup,nItem,UserInfoToStr(pFriend));
				break;
			}
		}
	}
}

void CDuiCSPlatDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(hWnd);
}

// 添加等待队列中的好友
void CDuiCSPlatDlg::AddWaitFriend(int nFriendID)
{
	if(std::find(g_vecWaitFriendID.begin(), g_vecWaitFriendID.end(), nFriendID) == g_vecWaitFriendID.end())
	{
		g_vecWaitFriendID.push_back(nFriendID);

		//CCSPlatQueuDlg::AddWaitFriend(nFriendID);

		if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
		{
			pInstance->UpdateWaitLabel();
		}
	}
}

void CDuiCSPlatDlg::UpdateWaitFriend()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->UpdateWaitLabel();
	}
}

// 删除等待队列中的好友
void CDuiCSPlatDlg::DelWaitFriend(int nFriendID)
{
	vector<int>::iterator it = std::find(g_vecWaitFriendID.begin(), g_vecWaitFriendID.end(), nFriendID);
	if(it != g_vecWaitFriendID.end())
	{
		g_vecWaitFriendID.erase(it);
		
		//CCSPlatQueuDlg::DelWaitFriend(nFriendID);

		if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
		{
			pInstance->UpdateWaitLabel();
		}
	}
}

// 清空等待队列，再客户端重新登录成功后调用
void CDuiCSPlatDlg::ClearWaitFriend()
{
	g_vecWaitFriendID.clear();
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->UpdateWaitLabel();
	}
}
/*
// 获取等待队列中最开始的好友
int CDuiCSPlatDlg::GetFirstWaitBuyer()
{
	if(g_vecWaitFriendID.size() > 0)
	{
		return g_vecWaitFriendID[0];
	}

	return -1;
}
*/
//#if !defined(_VER_PREVIEW_) || !defined(_VER_ONLINE_)
////更新等待队列标签
void CDuiCSPlatDlg::UpdateWaitLabel()
{
	DuiLib::CButtonUI *pBtnQueu = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnReception"));
	int nCsqueueCount = g_LocalData.GetCsQueueCount();
	pBtnQueu->SetEnabled(nCsqueueCount);

	DuiLib::CLabelUI *pLblQueu = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lbQueu"));
	CString strText;

	strText.Format("%d人排队中", /*g_vecWaitFriendID.size()*/nCsqueueCount);
	if(g_vecWaitFriendID.size() > 999)
	{
		strText = "999+人排队中";
	}
	pLblQueu->SetText(strText);

	pLblQueu->SetVisible(nCsqueueCount != 0);
}
//#else
//更新等待队列标签
//void CDuiCSPlatDlg::UpdateWaitLabel()
//{
//	DuiLib::CButtonUI *pBtnQueu = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnReception"));
//	pBtnQueu->SetEnabled(g_vecWaitFriendID.size() != 0);
//
//	DuiLib::CLabelUI *pLblQueu = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lbQueu"));
//	CString strText;
//	strText.Format("%d人排队中", g_vecWaitFriendID.size());
//	if(g_vecWaitFriendID.size() > 999)
//	{
//		strText = "999+人排队中";
//	}
//	pLblQueu->SetText(strText);
//
//	pLblQueu->SetVisible(g_vecWaitFriendID.size() != 0);
//}
//#endif

////关闭所有聊天窗口,查看有无未读消息
BOOL CDuiCSPlatDlg::IsPopMsgWarn(int &nUnRead)
{	
	BOOL bUnReadMsg = FALSE;
	nUnRead = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(pBtnRemind->IsVisible())
		{
			bUnReadMsg = TRUE;
			nUnRead++;

			CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录，写入临时log;
			int nUnReadCnt = _ttoi(strTxt);
			FRIEND_INFO* pFriend = m_vecFriend[nElement];
			g_mapMsgUnRead[pFriend] = nUnReadCnt;////存储当前聊天的未读消息
		}
	}
#ifdef _CS_PLAT_EX_
	//获取我的团队未读消息
	for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
		//防止内存中获取到空的好友
		FRIEND_INFO* pFriend = GetFriendInfo(pDlg->GetFriendInfo()->GetId()) != NULL ? GetFriendInfo(pDlg->GetFriendInfo()->GetId()) : pDlg->GetFriendInfo();
		if(!CMainFrame::IsBuyer(pFriend->GetId()))
		{
			int nCount = m_pFriendList->GetCount();
			if(m_pFriendList->GetFriendUnMsgVisable(pFriend))
			{
				g_mapMsgUnRead[pFriend] = 0;////存储当前聊天的未读消息
				bUnReadMsg = TRUE;
			}
		}
	}
#endif
	if(!bUnReadMsg)////当前所有窗口均没有未读消息
	{
		g_mapMsgUnRead.clear();////删除标识
	}
	return bUnReadMsg;
}

////新增关闭当前会话
////咨询框关闭当前聊天窗口,查看有无未读消息
BOOL CDuiCSPlatDlg::IsCurrentPopMsgWarn(int &nUnRead,int nFriendID)
{	
	BOOL bUnReadMsg = FALSE;
	nUnRead = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		FRIEND_INFO* pFriend = m_vecFriend[nElement];
		if(pFriend != NULL && pFriend->GetId() == nFriendID)////找到当前会话ID
		{
			if(pBtnRemind->IsVisible())//有未读消息
			{
				bUnReadMsg = TRUE;
				nUnRead++;

				CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录，写入临时log;
				int nUnReadCnt = _ttoi(strTxt);
				g_mapMsgUnRead[pFriend] = nUnReadCnt;////存储当前聊天的未读消息
			}
			break;////跳出循环
		}
	}
	if(!bUnReadMsg)////当前所有窗口均没有未读消息
	{
		g_mapMsgUnRead.clear();////删除标识
	}
	return bUnReadMsg;
}

//打开客服聊天窗口时客户列表设置为-1
void CDuiCSPlatDlg::SetListCurselNull()
{
	m_pCounselList->SelectItem(-1);
}

//显示或隐藏客户标签的红点
void CDuiCSPlatDlg::SetCustUnMsgVisable(bool bIsVisable)
{
	if(m_pLayoutPage->GetCurSel() != 0)
		m_labelUnMsgCust->SetVisible(bIsVisable);
}

//显示或隐藏我的团队标签的红点
void CDuiCSPlatDlg::SetTeamUnMsgVisable(bool bIsVisable)
{
	if(m_pLayoutPage->GetCurSel() != 2)
		m_labelUnMsgTeam->SetVisible(bIsVisable);
}

//设置我的团队中好友的红点
void CDuiCSPlatDlg::SetFriendUnMsg(FRIEND_INFO* pFriend, bool bIsUnMsg)
{
	m_pFriendList->SetItemUmMsg(pFriend, bIsUnMsg);
	if(bIsUnMsg)
	{
		SetTeamUnMsgVisable(bIsUnMsg);
	}
}

////关闭对话框按钮时，根据好友ID列表对比查找对应的未保存服务小记的窗口
BOOL CDuiCSPlatDlg::CheckUnSavedServiceLog()
{
	BOOL bFind   = FALSE;
	int  nCnt = 0;
	extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//遍历咨询框
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(pBtnRemind->IsVisible())
		{
			CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录条数
			nCnt = _ttoi(strTxt);//标记未读消息数
		}
		FRIEND_INFO* pFriend = m_vecFriend[nElement];
		map<DWORD,DWORD>::iterator it;
		it = g_mapServiceLog.find(pFriend->GetId());
		if (it==g_mapServiceLog.end())//保存的服务小记列表中找不到(状态为1保存完整，状态为2保存不完整)
		{	
			extern map<DWORD,CString> g_mapServiceURL;
			if(g_mapServiceURL.size() == 0)//没有空的买家GUID
			{
				bFind = TRUE;
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///确认关闭,跳转到未保存服务小记的第一个页面
				{
					CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//查找ID关联的对话框
					if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))//有买家和卖家的对话最前展示
					{
						ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//切换到本页面
					}
				}
				break;
			}
			else///有买家的GUID为空，再查找是不是当前买家
			{
				map<DWORD,CString>::iterator iter;
				iter = g_mapServiceURL.find(pFriend->GetId());
				if (iter!=g_mapServiceURL.end())///是当前买家的GUID为空
				{
					bFind = FALSE;//继续下一次遍历
					m_vecP2PSendMsgDlg[nElement]->SendMessage(WM_CLOSE_ONE);//先关闭当前会话
				}
				else //当前买家的GUID不为空
				{
	                bFind = TRUE;///买家对应的GUID不为空，要先保存
					std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
					pWnd->ShowModal();
					if(pWnd->m_bClickedBtnOK)///确认关闭,跳转到未保存服务小记的第一个页面
					{
						CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//查找ID关联的对话框
						if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))//有买家和卖家的对话最前展示
						{
							ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//切换到本页面
						}
					} 
					break;//查找到了未保存的页面，直接跳转
				}
			}	
		}
		else///找到了但是状态为2,表明保存不完整
		{
			if(it->second == UNCOMPLETED_SERVICELOG)
			{
				bFind = TRUE;
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///确认关闭,跳转到未保存完整的第一个页面
				{
					CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//查找ID关联的对话框
					if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))//有买家和卖家的对话最前展示
					{
						ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//切换到本页面
					}
				}
				break;
			}
		}
	}
	return bFind;
}


////关闭对话框按钮时，根据好友ID查找对应的未保存完整服务小记的窗口
BOOL CDuiCSPlatDlg::SelectUnCompletedServiceLog(FRIEND_INFO *pFriend)
{
	BOOL bFind = FALSE;
	int  nCnt = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//遍历咨询框
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(pBtnRemind->IsVisible())
		{
			CString strTxt = pBtnRemind->GetText();////获取本次对话的未读消息记录条数
			nCnt = _ttoi(strTxt);//标记未读消息数
		}
		FRIEND_INFO* pCurFriend = m_vecFriend[nElement];
		if (pFriend->GetId() == pCurFriend->GetId())//未保存完整的服务小记
		{				
			bFind = TRUE;
			std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());//未保存完整
			pWnd->ShowModal();
			if(pWnd->m_bClickedBtnOK)///确认关闭,跳转到未保存完整的第一个页面
			{
				CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pCurFriend,StruExtraInfo(),nCnt);//查找ID关联的对话框
				if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))
				{
					ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//切换到本页面
				}
			}
			break;
		}
	}
	return bFind;
}

//退出或者修改用户时，检测是否有客服对买家的聊天窗口
BOOL CDuiCSPlatDlg::CheckBuyerTalkingExist()
{
	BOOL bFind = FALSE;
	int  nCnt = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//遍历咨询框
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		FRIEND_INFO* pFriend = m_vecFriend[nElement];
		if(pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))  //对面是买家
		{
			bFind = TRUE;
			CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//查找ID关联的对话框
			if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()))//有客服和买家的对话最前展示
			{
				ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//切换到本页面
				break;
			}
		}
	}
	return bFind;
}

//计算客服和买家聊天的窗口数目
int CDuiCSPlatDlg::CalcBuyerTalkingWnd()
{
	int  nCnt = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//遍历咨询框
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(m_vecFriend[nElement] != NULL && nElement < m_vecFriend.size())
		{
			FRIEND_INFO* pFriend = m_vecFriend[nElement];
			if(pFriend->GetGuid().length()>0 && TRUE == CMainFrame::IsBuyer(pFriend->GetId()))  //对面是买家
			{
				nCnt++;
			}
		}

	}
	return nCnt;
}

//tab页选择我的团队
void CDuiCSPlatDlg::SetLayoutPageTeam()
{
	m_pLayoutPage->SelectItem(2);
	DuiLib::COptionUI* pTeamBtn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("teamBtn"));
	pTeamBtn->Selected(true);
}

//tab页选择用户
void CDuiCSPlatDlg::SetLayoutPageCust()
{
	m_pLayoutPage->SelectItem(0);
	DuiLib::COptionUI* pCustBtn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("custBtn"));
	pCustBtn->Selected(true);
}
//获取最大化之前的位置
CRect CDuiCSPlatDlg::GetPreRc()
{
	return m_preRc;
}

//设置最大化之前的位置
void CDuiCSPlatDlg::SetPreRc(CDuiP2PSendMsgDlg* pDlg)
{
	CRect rc;
	if(pDlg != NULL)
	{
		::GetWindowRect(pDlg->GetHWND(), &rc);
	}
	m_preRc = rc;
}

void CDuiCSPlatDlg::UpdateFriendList(FRIEND_INFO *pFriend)
{
	m_pFriendList->UpdateItemImage(pFriend);
}
