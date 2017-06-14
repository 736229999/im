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
extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////����δ����Ϣ�ĺ���ID��δ������
extern BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu);
extern CLocalData g_LocalData;

BEGIN_XT_MESSAGE_MAP(CDuiCSPlatDlg)
	// �޸ĺ��ѱ�ע
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

		// ��������
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
		// ��ȡת�Ӷ���
		int nIndex = m_pList->GetCurSel();
		DuiLib::CListLabelElementUI* pItem = static_cast<DuiLib::CListLabelElementUI*>(m_pList->GetItemAt(nIndex));
		int nFriendID = pItem->GetTag();	// ����ID
		FRIEND_INFO *pFriend = GetFriendInfo(nFriendID);
		m_pList->RemoveAt(nIndex);
		if(pFriend == NULL)
			return;

		// �����˷����ѽӴ���������
		XT_RECEPTE_CUST_REQ req;
		req.idBuyer = (uint32)nFriendID;
		req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_OFFER;	// �����Ӵ�
		req.teamId = (uint32)100;
		g_pProtocol->SendReq(req);

		CMainFrame::OpenP2PMsgDlg(pFriend);
		CloseDlg();

		CDuiCSPlatDlg::DelWaitFriend(nFriendID);
	}
}

// ���ݺ���IDˢ���б�
void CCSPlatQueuDlg::UpdateList(vector<int> &vecQueu)
{
	// ��������
	int nCount = (int)vecQueu.size();
	for(int i = 0; i < nCount; ++i)
	{
		int nFriendID = vecQueu[i];
		AddItem(nFriendID);
	}

	m_pList->SelectItem(0);

	// ���¸ı䴰�ڴ�С
	nCount = m_pList->GetCount();
	int nHeight = nCount > 10 ? 10*33 : nCount*33;
	CRect rc;
	GetWindowRect(GetHWND(), &rc);
	MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), nHeight, TRUE);
}

// ��ӵ�Ԫ
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

// ��ӵȴ������еĺ���
void CCSPlatQueuDlg::AddWaitFriend(int nFriendID)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->AddItem(nFriendID);

		// ���¸ı䴰�ڴ�С
		int nCount = pInstance->m_pList->GetCount();
		int nHeight = nCount > 10 ? 10*33 : nCount*33;
		CRect rc;
		GetWindowRect(pInstance->GetHWND(), &rc);
		MoveWindow(pInstance->GetHWND(), rc.left, rc.top, rc.Width(), nHeight, TRUE);
	}
}

// ɾ���ȴ������еĺ���
void CCSPlatQueuDlg::DelWaitFriend(int nFriendID)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		int nCount = pInstance->m_pList->GetCount();
		for(int i = 0; i < nCount; ++i)
		{
			DuiLib::CListLabelElementUI* pItem = static_cast<DuiLib::CListLabelElementUI*>(pInstance->m_pList->GetItemAt(i));
			int nFriendIDTemp = pItem->GetTag();	// ����ID
			if(nFriendIDTemp == nFriendID)
			{
				pInstance->m_pList->RemoveAt(i);
				break;
			}				
		}

		// ���¸ı䴰�ڴ�С
		nCount = pInstance->m_pList->GetCount();
		int nHeight = nCount > 10 ? 10*33 : nCount*33;
		CRect rc;
		GetWindowRect(pInstance->GetHWND(), &rc);
		MoveWindow(pInstance->GetHWND(), rc.left, rc.top, rc.Width(), nHeight, TRUE);
	}
}
*/

static long s_SearchEditStartChar = 0;
std::vector<int> CDuiCSPlatDlg::g_vecWaitFriendID;		// �ȴ�����

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
	{	// ��������Ϣ
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

			// ���������»�ȡ����
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
	{	// ��Ԫѡ��
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

	m_labelUnMsgCust = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelUnMsgCust"));//�ͻ�δ����Ϣ���
	m_labelUnMsgTeam = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelUnMsgTeam"));//�ҵ��Ŷ�δ����Ϣ���

	DuiLib::COptionUI* pOptionCust = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("custBtn"));

	m_MenuCheckInfo[_T("MenuAll")] = true;
	m_bIsUpdate = FALSE;

	// ���������ϵ������
	CDuiRecentList *pMfRecentList = CMainFrame::GetInstance()->GetRecentList();
	int nCount = pMfRecentList->GetCount();
	for(int i = nCount-1; i >= 0; i--)
	{
		DWORD dwData = pMfRecentList->GetItemAt(i)->GetTag();
		m_pRecentList->AddItem_Interface(dwData);
	}

	// �����ҵ��Ŷ��б�
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
	if(msg.sType == _T("itemselect"))//ѡ����ѯ���еĺ���
	{
		if( m_pCounselList == (DuiLib::CListUI *)msg.pSender )
		{
#ifdef _CS_PLAT_EX_
			int index = ((DuiLib::CListUI *)msg.pSender)->GetCurSel();
			if(index >= 0)
			{
				//�򿪿ͻ������촰�����������ͷ������촰��
				for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
				{
					if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg]->GetHWND() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] != m_vecP2PSendMsgDlg[index])
					{
						g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg]->ShowWindow(false);
					}
				}
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index]);////�ֶ��򿪵���Ϣ
			}
#else
			int index = ((DuiLib::CListUI *)msg.pSender)->GetCurSel();
			if(index >= 0)
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index]);////�ֶ��򿪵���Ϣ
#endif
		}
	}
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "custBtn")
		{
			m_pLayoutPage->SelectItem(0);
			m_labelUnMsgCust->SetVisible(false);//ѡ��ͻ���ǩ�����ʧ
		}
		else if(msg.pSender->GetName() == "recentBtn")
		{
			m_pLayoutPage->SelectItem(1);
		}
		else if(msg.pSender->GetName() == "teamBtn")
		{
			m_pLayoutPage->SelectItem(2);
			m_labelUnMsgTeam->SetVisible(false);//ѡ���ҵ��Ŷӱ�ǩ�����ʧ
		}
		else if(msg.pSender->GetName() == "btnReception")
		{
			// �����˷����ѽӴ���������
			XT_RECEPTE_CUST_REQ req;
			req.idBuyer = 0;
			req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER;	// �����Ӵ�
			req.teamId = 0;
			g_pProtocol->SendReq(req);
		}
		else if(msg.pSender->GetName() == _T("btnMarkeing")) //����˰���֮����ʾ����
		{
			DuiLib::CListContainerElementUI* pListElement = NULL;
			pListElement = (DuiLib::CListContainerElementUI*)msg.pSender->GetParent();//((DuiLib::CListUI *)msg.pSender->GetParent())->GetItemAt(index);

			DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
			DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

			pBtnMarkeing->SetVisible(false);
			pBtnMarked->SetVisible(true);
		}
		else if(msg.pSender->GetName() == _T("btnMarked")) //����˺���֮����ʾ����
		{
			DuiLib::CListContainerElementUI* pListElement = NULL;
			pListElement = (DuiLib::CListContainerElementUI*)msg.pSender->GetParent();//((DuiLib::CListUI *)msg.pSender->GetParent())->GetItemAt(index);

			DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
			DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));
			pBtnMarkeing->SetVisible(true);
			pBtnMarked->SetVisible(false);
		}
		else if(msg.pSender->GetName() == _T("btnClose")) //����ر�ͼ��ر�һ����ѯ��������
		{
			CMainFrame::GetInstance()->LockDlg();////��ס�Ի���
			DuiLib::CLabelUI* pListElement = NULL;
			pListElement = (DuiLib::CLabelUI*)msg.pSender->GetParent();
			DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));

			///////////////////////////////////////////////////////
			/////���ж��Ƿ��з���С��û�б��棬���ж��Ƿ���δ����Ϣ
			DuiLib::CControlUI* pC = pListElement->GetParent();
			DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
			int nIndex = pElement->GetIndex();

			FRIEND_INFO* pFriend = m_vecFriend[nIndex];////��ǰ�Ự������Ϣ
			extern map<DWORD,DWORD>  g_mapServiceLog;////�������ID��״̬����
			////g_mapServiceLog.clear();
			DWORD dwServiceLogCnt = g_mapServiceLog.size();
			if(dwServiceLogCnt == 0)//������Ҵ��ڶ�û�б������С��
			{
				//���������
				if(pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))
				{
					std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
					pWnd->ShowModal();
					if(pWnd->m_bClickedBtnOK)
					{
                        ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nIndex]);////ѡ�е�ǰ�Ự����
					}
					return ;
				}
				else//���治����ң���ֱ�Ӽ��δ����Ϣ
				{
					if(pBtnRemind->IsVisible())////�ر�ѡ�е�һ�������,��δ����Ϣ
					{
						std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
						pWnd->ShowModal();
						if(pWnd->m_bUnReadMsgShow)///ȷ�Ϲر�		
						{
							DuiLib::CControlUI* pC = pListElement->GetParent();
							DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
							int nIndex = pElement->GetIndex();

							CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
							int nUnReadCnt = _ttoi(strTxt);
							FRIEND_INFO* pFriend = m_vecFriend[nIndex];
							CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////����δ����Ϣ����д�ļ�
							m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
							CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
						}	
						else 
						{
							CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
							return;
						}
					}
					else ////��ǰ������δ����Ϣ��ֱ�ӹر�
					{
						//////////////////////////////////////////////////////
						DuiLib::CControlUI* pC = pListElement->GetParent();
						DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
						int nIndex = pElement->GetIndex();
						/////////////////
						m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
						CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
					}
				}
			}
			else //����ұ������������߲������ķ���С�ǣ��ж��Ƿ�Ϊ��ǰ���
			{
				bool bUncompleted = FALSE;
				//���������
				if( pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))
				{
					DWORD dwFid = 0;
					DWORD dwStatus = 0;
					map<DWORD,DWORD>::iterator it = g_mapServiceLog.find(pFriend->GetId());///�����Ƿ��е�ǰ���ѽڵ�
					if(it!=g_mapServiceLog.end())//�ҵ���
					{
						dwFid    = it->first;
						dwStatus = it->second;
						if(dwStatus == UNCOMPLETED_SERVICELOG)//δ��������
						{
							FRIEND_INFO *pFriend = ::FindFriend(dwFid);//�ҵ�������Ϣ
							if(pFriend!= NULL)//��ǰ��ѯ���ڱ����˲������ķ���С��
							{
								bUncompleted = FALSE;
								std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());
								pWnd->ShowModal();
								////�л�����ǰ����Ҳ�ҳ
								DuiLib::CControlUI* pC = pListElement->GetParent();
								DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
								int nIndex = pElement->GetIndex();
								ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nIndex]);////ѡ�е�ǰ�Ự����
								return ;
							}
						}
						else///�ѱ������С�ǣ��ټ���Ƿ���δ����Ϣ
						{
							///////����С�Ǳ�������
							/*std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(SAVED_SERVICELOG,GetHWND());
							pWnd->ShowModal();
							if(pWnd->m_bClickedBtnOK)///��ǰ��ѯ���ڷ���С���ѱ��棬��ȷ�Ϲرգ��ټ��δ����Ϣ*/
							{
								if(pBtnRemind->IsVisible())////�ر�ѡ�е�һ�������,��δ����Ϣ
								{
									std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
									pWnd->ShowModal();
									if(pWnd->m_bUnReadMsgShow)///ȷ�Ϲر�		
									{
										DuiLib::CControlUI* pC = pListElement->GetParent();
										DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
										int nIndex = pElement->GetIndex();

										CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
										int nUnReadCnt = _ttoi(strTxt);
										FRIEND_INFO* pFriend = m_vecFriend[nIndex];
										CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////����δ����Ϣ����д�ļ�
										m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
										CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
									}	
									else 
									{
										CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
										return;
									}
								}
								else ////��ǰ������δ����Ϣ��ֱ�ӹر�
								{
									//////////////////////////////////////////////////////
									DuiLib::CControlUI* pC = pListElement->GetParent();
									DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
									int nIndex = pElement->GetIndex();

									m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
									CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
								}
							}
							//else//ȡ��
							//{
							//	return ;
							//}
						}
					}
					else//���ǵ�ǰ��ұ���ķ���С��
					{
						//��ʾ��ǰ���û�������С��
						std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
						pWnd->ShowModal();
						////�л�����ǰ����Ҳ�ҳ
						DuiLib::CControlUI* pC = pListElement->GetParent();
						DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
						int nIndex = pElement->GetIndex();
						ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nIndex]);////ѡ�е�ǰ�Ự����

						return ;
					}
				}
				else////���治�����,ֱ���ж�δ����Ϣ
				{
					if(pBtnRemind->IsVisible())////�ر�ѡ�е�һ�������,��δ����Ϣ
					{
						std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
						pWnd->ShowModal();
						if(pWnd->m_bUnReadMsgShow)///ȷ�Ϲر�		
						{
							DuiLib::CControlUI* pC = pListElement->GetParent();
							DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
							int nIndex = pElement->GetIndex();

							CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
							int nUnReadCnt = _ttoi(strTxt);
							FRIEND_INFO* pFriend = m_vecFriend[nIndex];
							CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////����δ����Ϣ����д�ļ�
							m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
							CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
						}	
						else 
						{
							CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
							return;
						}
					}
					else ////��ǰ������δ����Ϣ��ֱ�ӹر�
					{
						//////////////////////////////////////////////////////
						DuiLib::CControlUI* pC = pListElement->GetParent();
						DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC;
						int nIndex = pElement->GetIndex();

						/////��δ����Ϣ
						//FRIEND_INFO* pFriend = m_vecFriend[nIndex];
						//map<FRIEND_INFO *,UINT>::iterator it;
						//it = g_mapMsgUnRead.find(pFriend);
						//if ( it!=g_mapMsgUnRead.end() )
						//{
						//	g_mapMsgUnRead.erase(it);////����ϴα���ĵ�ǰ���ڵ�δ����Ϣ
						//}
						/////////////////
						m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
						CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
					}
				}
			}
		}
		else if(msg.pSender->GetName() == _T("custMenuBtn")) //����ر�ͼ��ر�һ����ѯ��������
		{
			//������˵�
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
		{	// ˫�������ϵ��
			m_pRecentList->OnLbnDblclk();
		}
		else if(msg.pSender->GetParent()->GetParent()->GetName() == "lsFriend")
		{
			OnSendMsg();
		}
	}
	else if(_tcsicmp(msg.sType, DUI_MSGTYPE_RBUTTONDOWN) == 0)
	{	// �Ҽ����
		if(strcmp(msg.pSender->GetClass(), "TreeNodeUI") == 0)
		{
			if(m_pLayoutPage->GetCurSel() == 2)
			{
				FL_ITEM_POS pos=m_pFriendList->GetCurSel();
				int nGroup = pos.nGroupID;
				if(nGroup == -1)//�������Ժ����б����Ϣ������
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

				// ���ݲ�ͬ�������ʾ�����˵�������

				// �жϵ�ǰ�Ƿ���ѡ�е���
				if ( nGroup > -1 )
				{
					if ( nItem > -1 ) //���ѽڵ�
					{
						CString strItem;

						CString strGroup = m_pFriendList->GetGroupName(nGroup);
						if(strGroup != "İ����" && strGroup!= "������")
						{
							if(pFriend->GetId() != g_ImUserInfo.GetId())
							{
								strItem.Format("���ͼ�ʱ��Ϣ");
								popmenu->AppendMenu(MF_STRING,ID_P2PSENDMSG,strItem);
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							}

							strItem.Format("�޸ı�ע��");
							popmenu->AppendMenu(MF_STRING,ID_MODF_COMMNAME,strItem);

							if(pFriend->GetId() != g_ImUserInfo.GetId())
							{
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							}

							//��ţ����֯�ܹ��еĲ������ƶ�
							if(nGroup < FEINIU_GROUP_ID && nGroup > -1)
							{
								strItem.Format("ɾ����ϵ��");
								popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,strItem);
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");

								CMenu menuGroup;
								menuGroup.CreatePopupMenu();
								//��ţ����֯�ܹ��еĲ������ƶ�
								for(int i=0; i<g_GroupInfo.count;i++)//////�޸��ж��߼�����֯�ܹ�������䶯 2014-11-12
								{
									if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
									{
										//İ���˺ͺ�����ȥ��
										if(/*g_GroupInfo.group[i].group_index!=1&&*/g_GroupInfo.group[i].group_index!=2)
										{
											menuGroup.AppendMenu(MF_STRING,ID_GROUP_CHOOSE_BEG + i,g_GroupInfo.group[i].name);
										}
									}
								}
								popmenu->AppendMenu(MF_POPUP, (UINT)menuGroup.m_hMenu, "�Ѻ����ƶ���...");
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");


								strItem.Format("�ƶ���������");
								popmenu->AppendMenu(MF_STRING,ID_REMOVE_BLACKLIST,strItem);
								//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							}


							if(pFriend->GetId() != g_ImUserInfo.GetId())
							{
								strItem.Format("�鿴����");
								popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,strItem);
							}
						}
						else
						{
							popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,"�Ӹ���ɾ��");
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");

							popmenu->AppendMenu(MF_STRING,ID_ADD_BLACKLIST,"��Ϊ����");
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");

							popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,"�鿴����");
						}
					}
					else//��ڵ�
					{
						if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
						{
							CString strGroup = m_pFriendList->GetGroupName(nGroup);

							if (nGroup == MY_TEAM_GROUP_ID)
							{	// �ҵ��Ŷӷ���ֻ��"����ԱȺ����Ϣ"
								popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "����ԱȺ����Ϣ");
							}
							else
							{
								if (strGroup != "İ����" && strGroup != "������")
								{
									popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "����ԱȺ����Ϣ");
									//��ţ����֯�ܹ��еĲ������ƶ�
									if (nGroup < 100 && nGroup > -1)
									{
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_ADDGROUP, "�����");
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_ADD_CGROUP, "�������");
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_ADD_FRIEND, "��Ӻ���");

										//�ҵĺ��Ѳ�����༭
										if (nGroup != 0)
										{
											//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
											popmenu->AppendMenu(MF_STRING, ID_RENAME_GROUP, "��������");
											//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
											popmenu->AppendMenu(MF_STRING, ID_DELGROUP, "ɾ����");
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

			// ͬʱ�޸�edit�ؼ��ı���ɫ
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

// ͨ���ؼ��ֲ��Һ���
void CDuiCSPlatDlg::SearchFriend(const char *pKey, vector<int> &vecMeetFriend)
{
	vecMeetFriend.clear();
	if(pKey == NULL || strcmp(pKey, "") == 0)
	{
		return;
	}

	// ������ѡtabҳ��������Ӧ����
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
			{	// ����
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
	{	// �ҵ��Ŷ�
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

// ���ҹؼ��ָı�
void CDuiCSPlatDlg::SearchTextChange(const char *pKey)
{
	//if(strcmp(pKey, "") == 0)
	//{
	//	if(m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
	//		m_pSearchListWnd->Close(IDOK);

	//	// ���������»�ȡ����
	//	SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);

	//	return;
	//}

	// ���ҳ��ĺ���
	vector<int> vecMeetFriend;
	SearchFriend(pKey, vecMeetFriend);

	// �����б�
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

	// �����б��λ��
	CRect rcList;
	GetWindowRect(m_pSearchListWnd->GetHWND(), &rcList);
	
	CRect rc = pEdit->GetPos();
	CPoint pt(rc.left-24, rc.bottom+3);
	ClientToScreen(GetHWND(), &pt);
	::MoveWindow(m_pSearchListWnd->GetHWND(), pt.x, pt.y, rc.Width()+24, rcList.Height(), TRUE);
	m_pSearchListWnd->ShowWindow(true);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);

	// ���������»�ȡ����
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

//������ʾ�����б�
void CDuiCSPlatDlg::UpdateTeamList()
{
	//�������group ������Item
	m_pFriendList->DeleteAllItems();

	//groupInfo >> groupList�� groupID��ൽ500
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

		//�븸���ȴ���
		do
		{
			dequeId.push_front(nSubGroupId);
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			if(nSubGroupIndex != -1)
			{
				nSubGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			}
			else//�Ҳ�������
			{
				dequeId.clear();
				break;
			}
		}while(nSubGroupId != -1 && !bGroupAdd[nSubGroupId] && dequeId.size()<10);//�����з���

		while( !dequeId.empty())
		{
			nSubGroupId = dequeId.front();
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			nParentGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			m_pFriendList->AddSubGroup(nParentGroupId, nSubGroupId, g_GroupInfo.group[nSubGroupIndex].name);
			//��״̬
			m_pFriendList->SetGroupState(nSubGroupId,(FLS_STATE)g_GroupInfo.group[nSubGroupIndex].group_state);
			bGroupAdd[nSubGroupId] = true;
			dequeId.pop_front();
		}
	}

	//������Ϣ>> groupList
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

		// �Ƿ��б�ע����
		CString sName;
		if(pFriend->comment_name[0] == 0)
			sName = pFriend->nickname;
		else
			sName = pFriend->comment_name;

		m_pFriendList->AddItem(pFriend->group_index, sName, (DWORD)(pFriend));
		m_pFriendList->SetItemUmMsg(pFriend, false);//���һ������֮������������Ѻ͸��ڵ�ĺ���λ��
	}

	//���ں�������
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
		SetFriendUnMsg(pFriend, false);//�����촰�ڶ�Ӧ�ĺ����ʧ
		CMainFrame::OpenP2PMsgDlg(pFriend);		
	}
	else if(pos.nGroupID != -1)////չ����
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
				str.Format("�� \"%s\" ��������",(pUser->nickname));
				popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,str);	

				str.Format("�鿴 \"%s\" ����ϸ����",(pUser->nickname));
				popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,str);	

				if ( FindFriend(pUser->GetId()) )
				{
					str.Format("�� \"%s\" �������¼",(pUser->nickname));
					popmenu->AppendMenu(MF_STRING,ID_FRIEND_MSG,str);	
				}
				//if(pUser->group_index == 1)
				//{	// İ����
				//	str.Format("��Ҫ�� \"%s\" Ϊ����",(pUser->nickname));
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
			str.Format("����Ⱥ \"%s\" ��������",pCluster->szFamilyName);
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_TALK,str);	

			str.Format("�鿴Ⱥ \"%s\" ����",pCluster->szFamilyName);
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_INFO,str);	

			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSG,"��ʷ�����¼");	

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

		if ( *strMenuName == _T("MenuAll")) //��ʾȫ����ѯ��
		{
			ShowAllCounselDlg();
			m_MenuCheckInfo[_T("MenuAll")] = true;
			m_MenuCheckInfo[_T("MenuPorblem")] = false;
		}
		else if ( *strMenuName == _T("MenuPorblem")) //��ʾ��������ѯ��
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
	else if (uMsg == WM_LBUTTONDOWN)//�����ѯ�����������ö�
	{
		CRect rcCounsel;
		::GetWindowRect(*this, &rcCounsel);
		//û��ѡ����ѯ����Ĭ��ѡ�е�һ����ѯ��
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

//����Ա����Ϣ
void CDuiCSPlatDlg::OnSendGroupMessage()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	FL_GROUP_INFO *pGroup = m_pFriendList->GetGroupData(nGroup);
	if(pGroup == NULL)
		return;

	// ͬʱ������ĺ���Ҳ��ӵ����촰��
	vector<FL_GROUP_INFO*> vecSubGroup;
	pGroup->GetSubGroup(vecSubGroup);
	int nGroupIndex = pGroup->dwData;
	FRIEND_LIST::iterator it,itE;
	it=g_lsFriend.begin();
	itE=g_lsFriend.end();
	// ���ѡ�����İ���˺ͺ������򲻴���
	if(nGroupIndex == 1 || nGroupIndex == 2)
	{
		return ;
	}
	// �ҵ������ڵĺ���
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

	//���Ѹ�Ϊ�ڴ��еĺ���
	FRIEND_INFO* pRealFriend = FindFriend(pFriend->GetId());
	m_vecFriend.push_back(pRealFriend != NULL ? pRealFriend : pFriend);

	DuiLib::CDuiString str;
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder builder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(builder.Create(_T("DuiCSPlatDlgListElement.xml"), (UINT)0, NULL));

	//�����ѯ��Ԫʱ����ߺ��������ɫ
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

	pBtnMarkeing->SetVisible(true);//��һ�������ʾ����
	pListElement->SetTag(pFriend->GetId());
	m_pCounselList->Add(pListElement);
	//m_vecListElement.push_back(pListElement);

	m_PaintManager.SetDefaultFont("΢���ź�", 12, false, false, false);
	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	m_PaintManager.AddFont("΢���ź�", 12, true, false, false);

	DuiLib::CButtonGifUI* pIcon = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("lbHeaderIcon")));
	CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	CString strGifFile =  pFriend->face_url;
	if(strGifFile.Find(".gif") != -1)
	{
		strGifFile =  strFile + ".gif";
		if ( !FileExist(strGifFile) )///������ʱ����
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
		if ( FileExist(strGifFile))/////GIF����
		{
			if(bOnline)
			{
				pIcon->SetBkImage("");
				pIcon->SetNormalGifFile(strGifFile);
			}
			else//������
			{
				string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////�Ƿ����ߵ��ж�
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}
		}
		else///��GIF
		{
			string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////�Ƿ����ߵ��ж�
			pIcon->SetBkImage(strHeadIcon.c_str());
			pIcon->SetNormalGifFile("");
		}
	}

	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//����18��������ʾ...
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
	//�ж�������ظ�
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
	//	m_vecFriend.insert(itrFriendInsert, pFriendChange);////������ǰ

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
	//	m_vecP2PSendMsgDlg.insert(itrDlgInsert, pDlgChange);////�Ի���������ǰ
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

	//���Ϊ��ǰ�������ң�δ����Ϣ���ۼ�
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
		m_vecFriend.insert(itrFriendInsert, pFriendChange);////������ǰ

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
		m_vecP2PSendMsgDlg.insert(itrDlgInsert, pDlgChange);////�Ի���������ǰ
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
	//�ж��ǲ��ǻظ�,����ǣ���δ����Ϣ����
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

	//�ж��Ƿ�Ϊ�ظ���ѯ
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
				if(tm < 172800)//����һ����¼�Ա����������������Ϊ�ظ���ѯ
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
	if(nUnReadNum==0)////����δ����Ϣ
	{
		//δ����Ϣ���99��
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
		if(!pBtnRemind->IsVisible())///���ɼ�
		{
			m_mapFriendCount[pFriend] = 1;
			pListElement->SetDrawBkColor(NULL);
		}
#else
		//�ҵ��ŶӴ򿪵�ʱ��pBtnRemind�ж�Ϊ���ص����Լ���bIsRemind�ж�
		if(!pBtnRemind->IsVisible() && !bIsRemind)
		{
			m_mapFriendCount[pFriend] = 1;
			pListElement->SetDrawBkColor(NULL);
		}
#endif

		//����һ������ʾ��������ʾ��������ɫû�и���ȫ����λ��
		//if((DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(m_pCounselList->GetCurSel()) != pListElement)
		//{
		//	pListElement->SetVisible(false);
		//	pListElement->SetVisible(true);
		//}
	}
	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
// 	m_PaintManager.SetDefaultFont("΢���ź�", 12, false, false, false);//Ĭ������
// 	m_PaintManager.AddFont("΢���ź�", 12, true, false, false);//�������

	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//����18��������ʾ...
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
		//δѡ�е����������
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
			//	//����ǵ�һ�δ򿪿ͻ����촰�������ҳ��֮�ƶ����м�
			//	pDlg->CenterWindow();
			//	CRect rc;
			//	GetWindowRect(pDlg->GetHWND(), &rc);
			//	::MoveWindow(CDuiCSPlatDlg::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
			//}
			SetTimer(pDlg->GetHWND(), TIMER_EDIT_ENDDOWN, 100, NULL);
			m_pDlgSelectP2PSendMsgDlg = pDlg;
			g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;//���õ�ǰ��ʾ�����촰��
			//pList->SelectItem(nDlg);

			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nDlg);
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)
			{
				if(CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindow(CDuiCSPlatDlg::GetInstance()->GetHWND()))
				{
					if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->GetHWND() != NULL)
						CDuiCSPlatDlg::GetInstance()->Close();////�ر���ѯ��
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
				if(nCntUnRead == 0)////û��δ����Ϣ
				{
					pBtnMessage->SetVisible(false);
					pBtnTransfer->SetVisible(false);
					pBtnRepeat->SetVisible(false);
					pBtnRemind->SetVisible(false);

					DuiLib::CControlUI* pListItem = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("listitem")));
					pListElement->SetDrawBkColor(NULL);
					//m_nRemindCount = 1;
					//ѡ�����촰��ʱ��Ϣ����Ϊ1
					m_mapFriendCount[m_vecFriend[nDlg] ] = 1;
				}
			}
			if(m_pCounselList->GetCurSel() != nDlg)
			{
				m_bIsUpdate = TRUE;
				m_pCounselList->SelectItem(nDlg);
			}

			//���õ�ǰ��������
			XT_MONITOR_SET_BUYER_REQ req;
			req.idSeller = g_ImUserInfo.GetId();
			req.idBuyer = pDlg->GetFriendInfo()->GetId();
			g_pProtocol->SendReq(req);

			//tabҳѡ���û�
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
			//�ر�һ����ѯ����ʾ��һ����ѯ��
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
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)//���д��ڶ��ر��������ҳ
			{
				ShowWindow(false);
			}
			break;
		}
	}
}

void CDuiCSPlatDlg::MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent)
{
	//���д��ڴ�С�仯����ֹѡ�����������ʱ��С��ԭ
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
		//���IDһ��������б��еĺ����滻Ϊ�ڴ��еĺ���
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
	m_PaintManager.SetDefaultFont("΢���ź�", 12, false, false, false);//Ĭ������
	m_PaintManager.AddFont("΢���ź�", 12, true, false, false);//�������

	//����ͷ����ɫ
	DuiLib::CButtonGifUI* pIcon = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("lbHeaderIcon")));
	CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	CString strGifFile =  pFriend->face_url;
	if(strGifFile.Find(".gif") != -1)
	{
		strGifFile =  strFile + ".gif";
		if ( !FileExist(strGifFile) )///������ʱ����
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
		if ( FileExist(strGifFile))/////GIF����
		{
			if(bOnline)
			{
				pIcon->SetBkImage("");
				pIcon->SetNormalGifFile(strGifFile);
			}
			else//������
			{
				string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////�Ƿ����ߵ��ж�
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}
		}
		else///��GIF
		{
			string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////�Ƿ����ߵ��ж�
			pIcon->SetBkImage(strHeadIcon.c_str());
			pIcon->SetNormalGifFile("");
		}
	}

	DuiLib::CDuiString str;
	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//����18��������ʾ...
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

//�жϴ���Ĵ��ں͵�ǰѡ�еĴ����Ƿ�һ��
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
	//û��ѡ����ѯ����Ĭ��ѡ�е�һ����ѯ��
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

		//���ԡ��ظ���ѯ��δ����Ϣ��ת����Ϣ�������ʾ������
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

// ��ӵȴ������еĺ���
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

// ɾ���ȴ������еĺ���
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

// ��յȴ����У��ٿͻ������µ�¼�ɹ������
void CDuiCSPlatDlg::ClearWaitFriend()
{
	g_vecWaitFriendID.clear();
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->UpdateWaitLabel();
	}
}
/*
// ��ȡ�ȴ��������ʼ�ĺ���
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
////���µȴ����б�ǩ
void CDuiCSPlatDlg::UpdateWaitLabel()
{
	DuiLib::CButtonUI *pBtnQueu = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnReception"));
	int nCsqueueCount = g_LocalData.GetCsQueueCount();
	pBtnQueu->SetEnabled(nCsqueueCount);

	DuiLib::CLabelUI *pLblQueu = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lbQueu"));
	CString strText;

	strText.Format("%d���Ŷ���", /*g_vecWaitFriendID.size()*/nCsqueueCount);
	if(g_vecWaitFriendID.size() > 999)
	{
		strText = "999+���Ŷ���";
	}
	pLblQueu->SetText(strText);

	pLblQueu->SetVisible(nCsqueueCount != 0);
}
//#else
//���µȴ����б�ǩ
//void CDuiCSPlatDlg::UpdateWaitLabel()
//{
//	DuiLib::CButtonUI *pBtnQueu = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnReception"));
//	pBtnQueu->SetEnabled(g_vecWaitFriendID.size() != 0);
//
//	DuiLib::CLabelUI *pLblQueu = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lbQueu"));
//	CString strText;
//	strText.Format("%d���Ŷ���", g_vecWaitFriendID.size());
//	if(g_vecWaitFriendID.size() > 999)
//	{
//		strText = "999+���Ŷ���";
//	}
//	pLblQueu->SetText(strText);
//
//	pLblQueu->SetVisible(g_vecWaitFriendID.size() != 0);
//}
//#endif

////�ر��������촰��,�鿴����δ����Ϣ
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

			CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
			int nUnReadCnt = _ttoi(strTxt);
			FRIEND_INFO* pFriend = m_vecFriend[nElement];
			g_mapMsgUnRead[pFriend] = nUnReadCnt;////�洢��ǰ�����δ����Ϣ
		}
	}
#ifdef _CS_PLAT_EX_
	//��ȡ�ҵ��Ŷ�δ����Ϣ
	for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
		//��ֹ�ڴ��л�ȡ���յĺ���
		FRIEND_INFO* pFriend = GetFriendInfo(pDlg->GetFriendInfo()->GetId()) != NULL ? GetFriendInfo(pDlg->GetFriendInfo()->GetId()) : pDlg->GetFriendInfo();
		if(!CMainFrame::IsBuyer(pFriend->GetId()))
		{
			int nCount = m_pFriendList->GetCount();
			if(m_pFriendList->GetFriendUnMsgVisable(pFriend))
			{
				g_mapMsgUnRead[pFriend] = 0;////�洢��ǰ�����δ����Ϣ
				bUnReadMsg = TRUE;
			}
		}
	}
#endif
	if(!bUnReadMsg)////��ǰ���д��ھ�û��δ����Ϣ
	{
		g_mapMsgUnRead.clear();////ɾ����ʶ
	}
	return bUnReadMsg;
}

////�����رյ�ǰ�Ự
////��ѯ��رյ�ǰ���촰��,�鿴����δ����Ϣ
BOOL CDuiCSPlatDlg::IsCurrentPopMsgWarn(int &nUnRead,int nFriendID)
{	
	BOOL bUnReadMsg = FALSE;
	nUnRead = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		FRIEND_INFO* pFriend = m_vecFriend[nElement];
		if(pFriend != NULL && pFriend->GetId() == nFriendID)////�ҵ���ǰ�ỰID
		{
			if(pBtnRemind->IsVisible())//��δ����Ϣ
			{
				bUnReadMsg = TRUE;
				nUnRead++;

				CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
				int nUnReadCnt = _ttoi(strTxt);
				g_mapMsgUnRead[pFriend] = nUnReadCnt;////�洢��ǰ�����δ����Ϣ
			}
			break;////����ѭ��
		}
	}
	if(!bUnReadMsg)////��ǰ���д��ھ�û��δ����Ϣ
	{
		g_mapMsgUnRead.clear();////ɾ����ʶ
	}
	return bUnReadMsg;
}

//�򿪿ͷ����촰��ʱ�ͻ��б�����Ϊ-1
void CDuiCSPlatDlg::SetListCurselNull()
{
	m_pCounselList->SelectItem(-1);
}

//��ʾ�����ؿͻ���ǩ�ĺ��
void CDuiCSPlatDlg::SetCustUnMsgVisable(bool bIsVisable)
{
	if(m_pLayoutPage->GetCurSel() != 0)
		m_labelUnMsgCust->SetVisible(bIsVisable);
}

//��ʾ�������ҵ��Ŷӱ�ǩ�ĺ��
void CDuiCSPlatDlg::SetTeamUnMsgVisable(bool bIsVisable)
{
	if(m_pLayoutPage->GetCurSel() != 2)
		m_labelUnMsgTeam->SetVisible(bIsVisable);
}

//�����ҵ��Ŷ��к��ѵĺ��
void CDuiCSPlatDlg::SetFriendUnMsg(FRIEND_INFO* pFriend, bool bIsUnMsg)
{
	m_pFriendList->SetItemUmMsg(pFriend, bIsUnMsg);
	if(bIsUnMsg)
	{
		SetTeamUnMsgVisable(bIsUnMsg);
	}
}

////�رնԻ���ťʱ�����ݺ���ID�б�ԱȲ��Ҷ�Ӧ��δ�������С�ǵĴ���
BOOL CDuiCSPlatDlg::CheckUnSavedServiceLog()
{
	BOOL bFind   = FALSE;
	int  nCnt = 0;
	extern map<DWORD,DWORD>  g_mapServiceLog;////�������ID��״̬����
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//������ѯ��
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(pBtnRemind->IsVisible())
		{
			CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼����
			nCnt = _ttoi(strTxt);//���δ����Ϣ��
		}
		FRIEND_INFO* pFriend = m_vecFriend[nElement];
		map<DWORD,DWORD>::iterator it;
		it = g_mapServiceLog.find(pFriend->GetId());
		if (it==g_mapServiceLog.end())//����ķ���С���б����Ҳ���(״̬Ϊ1����������״̬Ϊ2���治����)
		{	
			extern map<DWORD,CString> g_mapServiceURL;
			if(g_mapServiceURL.size() == 0)//û�пյ����GUID
			{
				bFind = TRUE;
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///ȷ�Ϲر�,��ת��δ�������С�ǵĵ�һ��ҳ��
				{
					CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//����ID�����ĶԻ���
					if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))//����Һ����ҵĶԻ���ǰչʾ
					{
						ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//�л�����ҳ��
					}
				}
				break;
			}
			else///����ҵ�GUIDΪ�գ��ٲ����ǲ��ǵ�ǰ���
			{
				map<DWORD,CString>::iterator iter;
				iter = g_mapServiceURL.find(pFriend->GetId());
				if (iter!=g_mapServiceURL.end())///�ǵ�ǰ��ҵ�GUIDΪ��
				{
					bFind = FALSE;//������һ�α���
					m_vecP2PSendMsgDlg[nElement]->SendMessage(WM_CLOSE_ONE);//�ȹرյ�ǰ�Ự
				}
				else //��ǰ��ҵ�GUID��Ϊ��
				{
	                bFind = TRUE;///��Ҷ�Ӧ��GUID��Ϊ�գ�Ҫ�ȱ���
					std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
					pWnd->ShowModal();
					if(pWnd->m_bClickedBtnOK)///ȷ�Ϲر�,��ת��δ�������С�ǵĵ�һ��ҳ��
					{
						CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//����ID�����ĶԻ���
						if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))//����Һ����ҵĶԻ���ǰչʾ
						{
							ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//�л�����ҳ��
						}
					} 
					break;//���ҵ���δ�����ҳ�棬ֱ����ת
				}
			}	
		}
		else///�ҵ��˵���״̬Ϊ2,�������治����
		{
			if(it->second == UNCOMPLETED_SERVICELOG)
			{
				bFind = TRUE;
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///ȷ�Ϲر�,��ת��δ���������ĵ�һ��ҳ��
				{
					CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//����ID�����ĶԻ���
					if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))//����Һ����ҵĶԻ���ǰչʾ
					{
						ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//�л�����ҳ��
					}
				}
				break;
			}
		}
	}
	return bFind;
}


////�رնԻ���ťʱ�����ݺ���ID���Ҷ�Ӧ��δ������������С�ǵĴ���
BOOL CDuiCSPlatDlg::SelectUnCompletedServiceLog(FRIEND_INFO *pFriend)
{
	BOOL bFind = FALSE;
	int  nCnt = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//������ѯ��
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(pBtnRemind->IsVisible())
		{
			CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼����
			nCnt = _ttoi(strTxt);//���δ����Ϣ��
		}
		FRIEND_INFO* pCurFriend = m_vecFriend[nElement];
		if (pFriend->GetId() == pCurFriend->GetId())//δ���������ķ���С��
		{				
			bFind = TRUE;
			std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());//δ��������
			pWnd->ShowModal();
			if(pWnd->m_bClickedBtnOK)///ȷ�Ϲر�,��ת��δ���������ĵ�һ��ҳ��
			{
				CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pCurFriend,StruExtraInfo(),nCnt);//����ID�����ĶԻ���
				if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))
				{
					ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//�л�����ҳ��
				}
			}
			break;
		}
	}
	return bFind;
}

//�˳������޸��û�ʱ������Ƿ��пͷ�����ҵ����촰��
BOOL CDuiCSPlatDlg::CheckBuyerTalkingExist()
{
	BOOL bFind = FALSE;
	int  nCnt = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//������ѯ��
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		FRIEND_INFO* pFriend = m_vecFriend[nElement];
		if(pFriend->GetGuid().length() > 0 && CMainFrame::IsBuyer(pFriend->GetId()))  //���������
		{
			bFind = TRUE;
			CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nCnt);//����ID�����ĶԻ���
			if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()))//�пͷ�����ҵĶԻ���ǰչʾ
			{
				ShowSelectP2PSendMsgDlg(pDlg/*,nCnt*/);//�л�����ҳ��
				break;
			}
		}
	}
	return bFind;
}

//����ͷ����������Ĵ�����Ŀ
int CDuiCSPlatDlg::CalcBuyerTalkingWnd()
{
	int  nCnt = 0;
	for(int nElement = 0; nElement <m_pCounselList->GetCount(); nElement++)//������ѯ��
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pCounselList->GetItemAt(nElement);
		DuiLib::CButtonUI* pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(m_vecFriend[nElement] != NULL && nElement < m_vecFriend.size())
		{
			FRIEND_INFO* pFriend = m_vecFriend[nElement];
			if(pFriend->GetGuid().length()>0 && TRUE == CMainFrame::IsBuyer(pFriend->GetId()))  //���������
			{
				nCnt++;
			}
		}

	}
	return nCnt;
}

//tabҳѡ���ҵ��Ŷ�
void CDuiCSPlatDlg::SetLayoutPageTeam()
{
	m_pLayoutPage->SelectItem(2);
	DuiLib::COptionUI* pTeamBtn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("teamBtn"));
	pTeamBtn->Selected(true);
}

//tabҳѡ���û�
void CDuiCSPlatDlg::SetLayoutPageCust()
{
	m_pLayoutPage->SelectItem(0);
	DuiLib::COptionUI* pCustBtn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("custBtn"));
	pCustBtn->Selected(true);
}
//��ȡ���֮ǰ��λ��
CRect CDuiCSPlatDlg::GetPreRc()
{
	return m_preRc;
}

//�������֮ǰ��λ��
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
