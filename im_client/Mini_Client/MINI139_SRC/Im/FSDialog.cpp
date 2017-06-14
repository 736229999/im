// FSDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\fsdialog.h"
#include "UserAuthDlg.h"
#include "userauthInfodlg.h"
#include "xtpacket.h"
#include "..\FriendInfoDlg.h"
#include "ClusterInfoDlg.h"
#include "AddBlackDlg.h"
#include "MessageHandleWnd.h"
//#define _TEMP_DEBUG
#ifdef _TEMP_DEBUG
#include "clusterinfodlg.h"
#endif 

#define SEARCH_COUNT_ONE_TIME 5		// һ�β�����Ŀ

#define TIMER_SEARCH 0x01

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
///////////�����Ƿ��ں�������////////////////////////////
BOOL isBlackListExists(uint32 FriendID)
{
	map<int,int> mapGroup;
	bool bFlag = FALSE;
	bool bRet  = FALSE;
	FRIEND_LIST::iterator it;
	for(it=g_lsFriend.begin();it!=g_lsFriend.end();it++)
	{

		FRIEND_INFO *pFriend = (*it);
		if(pFriend->GetId() ==  FriendID)////�ҵ�����ID
		{
			int nGroupCount = g_GroupInfo.count;
			int nGroup = 0;
			int nIdx = 0;
			if(!bFlag)
			{
				bFlag = TRUE;
				for ( nGroup = 0; nGroup< nGroupCount; nGroup++ )
				{
					nIdx = g_GroupInfo.group[nGroup].group_index;
					mapGroup[nIdx] = nGroup;
				}
			}
			/////////////���������Ϣ////////////////////
			int nFolder = mapGroup[pFriend->group_index];
			CString strGroup = _T("");
			strGroup.Format(_T("%s"),g_GroupInfo.group[nFolder].name) ;
			if(strGroup == _T("������"))//// ���Ѵ����ں�������
			{
				bRet =  TRUE;
				break;
			}
		}
	}
	return bRet;
}


std::tr1::shared_ptr<CAddFriendOrClusterDlg> CAddFriendOrClusterDlg::pInstance = NULL;

BEGIN_XT_MESSAGE_MAP(CAddFriendOrClusterDlg)
	// �������ߺ��ѽ��
	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK,OnFriendSearchAck)
	// ��������Ⱥ���
	ON_XT_COMMAND(CMD_CLUSTER_SEARCH_ACK,OnClusterSearchAck)
END_XT_MESSAGE_MAP()

LRESULT CAddFriendOrClusterDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// �����̨������Ϣ
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

// �鿴��ϵ������
void CAddFriendOrClusterDlg::ViewFriendInfo(int nIndex)
{
	DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_pResultList->GetItemAt(nIndex));
	if(pItem == NULL)
		return;

	FRIEND_INFO *pFriend = (FRIEND_INFO*)(pItem->GetTag());
	if (pFriend != NULL)
	{
		CFriendInfoDlg::NewDlg(pFriend);
	}
}

// �鿴Ⱥ����
void CAddFriendOrClusterDlg::ViewClusterInfo(int nIndex)
{
	DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_pResultCList->GetItemAt(nIndex));
	if(pItem == NULL)
		return;

	XT_CLUSTER_INFO *pCluster = (XT_CLUSTER_INFO*)(pItem->GetTag());
	if (pCluster != NULL)
	{
		std::tr1::shared_ptr<CClusterInfoDlg> pWnd=CClusterInfoDlg::NewDlg(pCluster);
		if(pWnd)
		{
			// �����������Ⱥ��Ϣ
			XT_GET_CINFO_REQ req;
			req.nFamilyID   = pCluster->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			g_pProtocol->SendReq(req);
			// ��ʾȺ��Ϣ
			//pWnd->ShowWindow(false);
			pWnd->ShowWindow(true);
			//pWnd->CenterWindow(/*CWnd::FromHandle(GetDesktopWindow())*/);
		}
	}
}

// ��Ϣ
void CAddFriendOrClusterDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnNext)
		{
			int nSel = m_pTabLayout->GetCurSel();
			if(nSel == 0)
			{	// ���Һ���
				SearchFriend();
			}

			if(nSel == 1)
			{	// ��Ϊ����
				AddFiend();
			}

			if(nSel == 2)
			{	// ����Ⱥ
				SearchCluster();
			}

			if(nSel == 3)
			{	// ����Ⱥ
				AddCluster();
			}
		}
		else if(msg.pSender == m_pBtnBack)
		{
			if(m_pTabLayout->GetCurSel() == 1)
			{
				m_pTabLayout->SelectItem(0);
			}
			if(m_pTabLayout->GetCurSel() == 3)
			{
				m_pTabLayout->SelectItem(2);
			}

			m_pBtnBack->SetVisible(false);
			m_pBtnNext->SetText("����");
		}
		else if(msg.pSender == m_pBtnFirstPage)
		{	// ��һҳ
			m_nCurPageNo = 0;
			RefreshPageBtn();
			Search();
		}
		else if(msg.pSender == m_pBtnPrePage || msg.pSender == m_pBtnPrePageC)
		{	// ��һҳ
			if(m_nCurPageNo > 0)
			{
				m_nCurPageNo--;
			}
			RefreshPageBtn();
			Search();
		}
		else if(msg.pSender == m_pBtnNextPage || msg.pSender == m_pBtnNextPageC)
		{	// ��һҳ
			if(m_nCurPageNo < m_nPageCount-1)
			{
				m_nCurPageNo++;
			}
			RefreshPageBtn();
			Search();
		}
		else if(msg.pSender == m_pBtnLastPage)
		{	// ���һҳ
			m_nCurPageNo = m_nPageCount-1;
			RefreshPageBtn();
			Search();
		}
		else
		{
			for(int i = 0; i < m_pResultList->GetCount(); ++i)
			{
				DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_pResultList->GetItemAt(i));
				DuiLib::CButtonUI *pBtnName = static_cast<DuiLib::CButtonUI*>(pItem->FindSubControl("name"));

				if(pBtnName == msg.pSender)
				{
					m_pResultList->SelectItem(i);
					ViewFriendInfo(i);
					break;
				}
			}

			for(int i = 0; i < m_pResultCList->GetCount(); ++i)
			{
				DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_pResultCList->GetItemAt(i));
				DuiLib::CButtonUI *pBtnName = static_cast<DuiLib::CButtonUI*>(pItem->FindSubControl("name"));

				if(pBtnName == msg.pSender)
				{
					m_pResultCList->SelectItem(i);
					ViewClusterInfo(i);
					break;
				}
			}
		}
	}
	if(_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{	
		DuiLib::COptionUI *pOption = static_cast<DuiLib::COptionUI*>(msg.pSender);
		string strGroup = pOption->GetGroup();
		if(strGroup == "mainTab")
		{	//  tabѡ��ı�
			if(m_pOptionXtream->IsSelected())
			{
				m_pTabLayout->SelectItem(0);
			}
			else
			{
				m_pTabLayout->SelectItem(2);
			}
			m_pBtnBack->SetVisible(false);
			m_pBtnNext->SetText("����");
		}
		if(strGroup == "SearchFriendTypeSel")
		{	// ���Ѳ���ѡ��
			m_pEditFriendName->SetVisible(m_pOptionFriendPreciseSearch->IsSelected());
			m_pLabelFriendName->SetVisible(m_pOptionFriendPreciseSearch->IsSelected());
		}
	}
}

void CAddFriendOrClusterDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(m_hDlgWnd);
	m_pResultList->RemoveAll();
	m_pResultList->RemoveAll();
	m_mapReqTick.clear();
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}

void CAddFriendOrClusterDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	m_pResultList = static_cast<CResultListUI*>(m_PaintManager.FindControl("result"));
	m_pResultList->SetFsType(FS_XTREAM);
	m_pEditFriendName = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("friendname"));
	m_pLabelPageNo = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("pageNo"));
	m_pEditclusterSearchValue = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("clusterSearchValue"));
	m_pResultCList = static_cast<CResultListUI*>(m_PaintManager.FindControl("resultC"));
	m_pResultCList->SetFsType(FS_CLUSTER);

	m_pBtnBack = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("backbtn"));
	m_pBtnNext = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("searchbtn"));
	m_pClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("closebtn"));

	m_pTabLayout = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));
	m_pOptionXtream = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("addfriendBtn"));
	m_pOptionCluster = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("addClusterBtn"));
	m_pOptionFriendPreciseSearch = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("checkboxPreciseSearch"));
	m_pOptionClusterID = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("checkboxClusterID"));

	m_pLabelFriendName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelFriendName"));

	m_pLabelPageCount = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("LabelPageNo"));
	m_pLabelPageNoC = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelPageNoC"));

	m_pBtnFirstPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("firstbtn"));
	m_pBtnPrePage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("prevbtn"));
	m_pBtnNextPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("nextbtn"));
	m_pBtnLastPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("lastbtn"));
	m_pBtnPrePageC = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("prevbtnC"));
	m_pBtnNextPageC = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("nextbtnC"));

	m_pEditClusterSearchValue = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("clusterSearchValue"));

#ifdef _CS_PLAT_EX_
	m_pOptionXtream->SetVisible(false); ////���/����Ⱥ
	m_pTabLayout->SelectItem(2);
#endif
	
	UpdateBk();

	m_pBtnBack->SetVisible(false);
	m_pBtnNext->SetText("����");

	m_hDlgWnd = GetHWND();
	// ע����Ϣ
	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK, m_hDlgWnd);
	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_REQ, m_hDlgWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_ACK, m_hDlgWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_REQ, m_hDlgWnd);

	m_nLastReqSeq = 0;
}

DuiLib::CControlUI* CAddFriendOrClusterDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("ResultList")) == 0)
	{
		return new CResultListUI(m_PaintManager);
	}

	return NULL;
}

// ���Һ���
void CAddFriendOrClusterDlg::SearchFriend()
{
	m_nCurPageNo = 0;

	m_pBtnNext->SetText("��Ϊ����");
	m_pBtnBack->SetVisible(true);
	m_pTabLayout->SelectItem(1);

	RefreshPageBtn();

	if(!Search())
	{
		m_pBtnNext->SetText("����");
		m_pBtnBack->SetVisible(false);
		m_pTabLayout->SelectItem(0);
	}
}

// ����Ⱥ
void CAddFriendOrClusterDlg::SearchCluster()
{
	m_nCurPageNo = 0;
	m_nPageCount = 9999;

	m_pBtnNext->SetText("����Ⱥ");
	m_pBtnBack->SetVisible(true);
	m_pTabLayout->SelectItem(3);

	RefreshPageBtn();

	if(!Search())
	{
		m_pBtnNext->SetText("����");
		m_pBtnBack->SetVisible(false);
		m_pTabLayout->SelectItem(2);
	}
}

// ��Ϊ����
void CAddFriendOrClusterDlg::AddFiend()
{
	int nCurSel = m_pResultList->GetCurSel();
	if(nCurSel < 0)
		return;

	FRIEND_INFO *pFriend = (FRIEND_INFO*)(m_pResultList->GetItemAt(nCurSel)->GetTag());

	if ( pFriend )
	{
		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("���Ѿ����Լ��������ˣ�");
			return ;
		}

		FRIEND_INFO * pRealFriend = FindFriend(pFriend->GetId());
		CString str  = _T("");
		BOOL    bRet =  FALSE;
		if ( pRealFriend && 
			(pRealFriend->group_index != GROUPID_STRANGER) && (pRealFriend->group_index != GROUPID_BLACKLIST) )
		{
			str.Format("%s �Ѿ�����ĺ����б��У������ظ���ӣ�" , UserInfoToStr(pRealFriend));
			CMessageBox::Model(GetHWND(), str, "���|����", MB_OK|MB_ICONWARNING);
			return ;
		}
		else
		{
			str = _T("����ϵ���ں�������,��Ϊ����,���Ӻ�������ɾ��");
			bRet = isBlackListExists(pFriend->GetId());
			if (bRet)
			{
				if(IDNO==CMessageBox::Model(NULL,str,NULL,MB_YESNO))
					return;
			}

			CAddBlackDlg::Show(pFriend->GetId());

			//if ( pFriend->rule==2 )
			//{
			//	bRet = isBlackListExists(pFriend->GetId());
			//	if (bRet)
			//	{

			//		if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
			//		{
			//			CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(CWnd::FromHandle(GetHWND()), CWnd::FromHandle(GetDesktopWindow()));
			//			// ������ӵ�������0��
			//			pDlg->SetUserAuthInfo(pFriend, 2, "",0);
			//			pDlg->ShowWindow(SW_SHOW);
			//			return;
			//		}

			//	}
			//	else
			//	{
			//		CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(CWnd::FromHandle(GetHWND()), CWnd::FromHandle(GetDesktopWindow()));
			//		// ������ӵ�������0��
			//		pDlg->SetUserAuthInfo(pFriend, 2, "",0);
			//		pDlg->ShowWindow(SW_SHOW);
			//		return;
			//	}

			//}
			//else  if ( pFriend->rule==1 )
			//{
			//	bRet = isBlackListExists(pFriend->GetId());
			//	if (bRet)
			//	{
			//		if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
			//		{
			//			pFriend->bRefreshed=true;
			//			// ������ӵ�������0��
			//			CUserAuthReqDlg::Show(pFriend,0);
			//			return ;
			//		}
			//	}
			//	else
			//	{
			//		pFriend->bRefreshed=true;
			//		// ������ӵ�������0��
			//		CUserAuthReqDlg::Show(pFriend,0);
			//		return ;
			//	}
			//}
			//bRet = isBlackListExists(pFriend->GetId());
			//if (bRet)
			//{
			//	if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
			//	{
			//		map<uint32,DWORD>::iterator it = m_mapReqTick.find(pFriend->GetId());
			//		bool bReq = false;

			//		if ( it==m_mapReqTick.end() )
			//		{
			//			m_mapReqTick.insert(map<uint32,DWORD>::value_type(pFriend->GetId(),GetTickCount()));
			//			bReq=true;
			//		}
			//		else
			//		{
			//			DWORD dwTick = GetTickCount();

			//			if ( (dwTick-it->second)>24000 )//24��
			//			{
			//				it->second = dwTick;
			//				bReq = true;
			//			}
			//		}

			//		if ( bReq )
			//		{
			//			XT_FRIENDGROUP_MOD_REQ req;

			//			req.action = GROUP_ADD;
			//			req.uid    = g_ImUserInfo.GetId();
			//			req.fid    = pFriend->GetId();
			//			req.group_id = 0;

			//			g_pProtocol->SendReq(req);

			//			RegUserOperation(0x00c1);
			//		}
			//	}
			}
		//	else
		//	{
		//		map<uint32,DWORD>::iterator it = m_mapReqTick.find(pFriend->GetId());
		//		bool bReq = false;

		//		if ( it==m_mapReqTick.end() )
		//		{
		//			m_mapReqTick.insert(map<uint32,DWORD>::value_type(pFriend->GetId(),GetTickCount()));
		//			bReq=true;
		//		}
		//		else
		//		{
		//			DWORD dwTick = GetTickCount();

		//			if ( (dwTick-it->second)>24000 )//24��
		//			{
		//				it->second = dwTick;
		//				bReq = true;
		//			}
		//		}

		//		if ( bReq )
		//		{

		//			//amin ��Ӻ���ѡ�����
		//			XT_FRIENDGROUP_MOD_REQ req;

		//			req.action = GROUP_ADD;
		//			req.uid    = g_ImUserInfo.GetId();
		//			req.fid    = pFriend->GetId();
		//			req.group_id = 0;

		//			g_pProtocol->SendReq(req);

		//			RegUserOperation(0x00c1);
		//		}
		//	}
		//} 
	}
	else
	{
		ShowMessage("��Ӳ��ҽ����ѡ��һ���û�!");
	}  
}

// ����Ⱥ
void CAddFriendOrClusterDlg::AddCluster()
{
	int nCurSel = m_pResultCList->GetCurSel();
	if(nCurSel < 0)
		return;

	XT_CLUSTER_INFO *pCluster = (XT_CLUSTER_INFO*)(m_pResultCList->GetItemAt(nCurSel)->GetTag());

	if ( pCluster==NULL )
	{
		ShowMessage("��Ӳ��ҽ����ѡ��һ��Ⱥ!");
		return ;
	}

	if ( pCluster->nCreatorID == g_ImUserInfo.GetId() )
	{
		ShowMessage("�������Լ�������Ⱥ��:)");
		return ;
	}
	for(int i=0;i<g_vecCluster.size();i++)
	{
		if(pCluster->nFamilyID == g_vecCluster[i]->nFamilyID)
		{
			ShowMessage("���Ѿ��Ǹ�Ⱥ�ĳ�Ա��:)");
			return ;
		}
	}

	if ( pCluster->nJoinFlag!=2 )
	{
		XT_CLUSTER_APPLY_REQ req;
		req.nAction     = 0; //�������Ⱥ
		req.nFamilyID = pCluster->nFamilyID;
		req.nFamilyProp = PERM_FAMILY;
		req.nUserID = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);
	}
	else
	{
		CString str;
		str.Format("Ⱥ��%s(%u) �ܾ��³�Ա���룡",pCluster->szFamilyName,pCluster->nFamilyID);
		ShowMessage(str,MB_OK|MB_ICONINFORMATION);
	}
}

void CAddFriendOrClusterDlg::OnFriendSearchAck(const char *data,WORD wRet)
{
	if ( wRet==SERVER_TIMEOUT )
	{
		ShowMessage("��������æ�����ҳ�ʱ��");
		return;
	}

	if ( wRet==0 )
	{
		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);

		//ȷ��������Ӧ�������һ�η��������Ӧ��Ӧ�������֮ǰ������Ӧ������
		if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() )
		{
			return;
		}

		int i;
		uint16	next_pos;
		uint32	total;
		int		nTotal;
		uint8	count;
		uint8	realCount = 0;
		rp>>count>>next_pos>>total;
		// ����������
		nTotal = total;
		if(nTotal == 0)
		{
			nTotal = count;
		}

		// ����ҳ��
		CString strLabelPageNo;
		m_nPageCount = nTotal/SEARCH_COUNT_ONE_TIME;
		if(nTotal%SEARCH_COUNT_ONE_TIME > 0)
		{
			m_nPageCount++;
		}
		strLabelPageNo.Format("/%dҳ", m_nPageCount);
		m_pLabelPageCount->SetText(strLabelPageNo);

		// ����б�
		m_pResultList->RemoveAll();

		for ( i=0; i<count; i++ )
		{
			FRIEND_INFO * pFriend = new FRIEND_INFO();
			pFriend->SelfEncrypt();
			uint32 id = 0;
			rp  >>id;
			pFriend->SetId(id);
			rp	>>pFriend->sex
				>>pFriend->status
				>>pFriend->rule;
			char username[MAX_USERNAME_LEN+1] = {0};
			rp	>>username;
			pFriend->SetUserName(username);
			rp	>>pFriend->nickname;
			if(pFriend->status == FRIEND_INFO::HIDE)
			{
				pFriend->status = FRIEND_INFO::OFFLINE;
			}
			
			m_pResultList->AddItem(pFriend->GetUserName().c_str(), pFriend->nickname, SexToStr(pFriend->sex), (DWORD)pFriend);
		}
		// ˢ�·�ҳ��ť״̬
		RefreshPageBtn();
	}
}

void CAddFriendOrClusterDlg::OnClusterSearchAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		//ȷ��������Ӧ�������һ�η��������Ӧ��Ӧ�������֮ǰ������Ӧ������
		if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() ){
			return;
		}

		m_pResultCList->RemoveAll();

		LPXT_CLUSTER_SEARCH_ACK ack = (LPXT_CLUSTER_SEARCH_ACK)data;
		m_pBtnNextPageC->SetEnabled(ack->next_pos != 0xffff);		// �ж��Ƿ������һҳ
		for ( int i=0; i<ack->nCount; i++ )
		{
			if(ack->FamilySearchData[i].nFamilyID == 0 || ack->FamilySearchData[i].nCreaterID == 0)
			{
				m_pBtnNextPageC->SetEnabled(false);
				break;
			}
			XT_CLUSTER_INFO * pInfo = new XT_CLUSTER_INFO();

			pInfo->nFamilyID  = ack->FamilySearchData[i].nFamilyID;
			pInfo->nCreatorID = ack->FamilySearchData[i].nCreaterID;
			pInfo->nJoinFlag  = ack->FamilySearchData[i].nJoinFlag;
			pInfo->nFamilyClass  = ack->FamilySearchData[i].nFamilyClass;
			strcpy(pInfo->szFamilyName,ack->FamilySearchData[i].szFamilyName);
			strcpy(pInfo->szFamilyDomain,ack->FamilySearchData[i].szFamilyDomain);
			pInfo->nNumberLimit  = ack->FamilySearchData[i].nMemCount;///����Ⱥ��Ա����

			CString strNumber;
			strNumber.Format("%d", pInfo->nNumberLimit);

			CString strClusterType;		// Ⱥ�ȼ�
			if (FAMILY_COMMON == pInfo->nFamilyType) //Ⱥ�ȼ�
			{
				strClusterType = "��ͨȺ";
			}
			if (FAMILY_VIP   == pInfo->nFamilyType)
			{
				strClusterType = "VIPȺ";
			}

			m_pResultCList->AddItem(pInfo->szFamilyName, strNumber, strClusterType, (DWORD)pInfo);
		}
	}
}

// ˢ��ҳ�水ť״̬
void CAddFriendOrClusterDlg::RefreshPageBtn()
{
	if(m_pTabLayout->GetCurSel() == 1)
	{	// ������ϵ��
		m_pBtnFirstPage->SetEnabled(!(m_nCurPageNo == 0));
		m_pBtnPrePage->SetEnabled(!(m_nCurPageNo == 0));
		m_pBtnNextPage->SetEnabled(!(m_nCurPageNo == m_nPageCount-1));
		m_pBtnLastPage->SetEnabled(!(m_nCurPageNo == m_nPageCount-1));

		CString strPageNo; 
		strPageNo.Format("%d", m_nCurPageNo+1);
		m_pLabelPageNo->SetText(strPageNo);
	}

	if(m_pTabLayout->GetCurSel() == 3)
	{	// ����Ⱥ
		//m_pBtnFirstPageC->SetEnabled(!(m_nCurPageNo == 0));
		m_pBtnPrePageC->SetEnabled(!(m_nCurPageNo == 0));
		//m_pBtnNextPageC->SetEnabled(!(m_nCurPageNo == m_nPageCount-1));
		//m_pBtnLastPageC->SetEnabled(!(m_nCurPageNo == m_nPageCount-1));

		CString strPageNo; 
		strPageNo.Format("%d", m_nCurPageNo+1);
		m_pLabelPageNoC->SetText(strPageNo);
	}
}

// ���ң���ҳ��ť���ʱ���ң�
BOOL CAddFriendOrClusterDlg::Search()
{
	if(m_pTabLayout->GetCurSel() == 1)
	{	// ������ϵ��
		if(m_pOptionFriendPreciseSearch->IsSelected())
		{	// ��ȷ���ң��жϻ�Ա���Ƿ�����
			CString strNickName = m_pEditFriendName->GetText();
			if(strNickName.Trim().IsEmpty())
			{
				CMessageBox::Prompt("�������Ա����");
				return FALSE;
			}
		}

		// ��������
		XT_XTREAM_SEARCH_REQ req;
		if(m_pOptionFriendPreciseSearch->IsSelected())
		{	// ��ȷ����
			req.search_type = XTREAM_SEARCH_PRECISION;
		}
		else
		{	// ��˭������
			req.search_type = XTREAM_SEARCH_ONLINE;
		}

		req.count = SEARCH_COUNT_ONE_TIME;

		if(m_pOptionFriendPreciseSearch->IsSelected())
		{
			string strNickName = m_pEditFriendName->GetText();
			strcpy(req.nick_name, strNickName.c_str());
			CString strError = "";
			if ( ValidateString(strNickName.c_str(),strError)==false )
			{
				CMessageBox::Prompt("�û������󣡴���ԭ��:"+strError);
				return FALSE;
			}
		}

		req.startpos = m_nCurPageNo*SEARCH_COUNT_ONE_TIME;
		g_pProtocol->SendReq(req);
		m_nLastReqSeq = g_pProtocol->GetCurSendSequnceId()-1;
	}
	if(m_pTabLayout->GetCurSel() == 3)
	{
		XT_CLUSTER_SEARCH_REQ req;
		if(m_pOptionClusterID->IsSelected())
		{
			req.search_type = CLUSTER_SEARCH_ID;
			CString str = m_pEditClusterSearchValue->GetText();
			str.Trim();

			if (str=="")
			{
				CMessageBox::Prompt("Ⱥ�Ų���Ϊ�գ�");
				return FALSE;
			}

			req.type_data.nFamilyID=atol((LPCSTR)str);
		}
		else
		{
			req.search_type = CLUSTER_SEARCH_NAME;
			CString str = m_pEditClusterSearchValue->GetText();
			str.Trim();

			if ( str=="" )
			{
				CMessageBox::Prompt("Ⱥ���Ʋ���Ϊ��!");
				return FALSE;
			}

			strcpy(req.type_data.szFamilyName,str);
		}
		req.start_pos = m_nCurPageNo*SEARCH_COUNT_ONE_TIME;
		req.nCount = SEARCH_COUNT_ONE_TIME;
		g_pProtocol->SendReq(req);
		m_nLastReqSeq = g_pProtocol->GetCurSendSequnceId()-1;
	}

	return TRUE;
}

std::tr1::shared_ptr<CAddFriendOrClusterDlg> CAddFriendOrClusterDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CAddFriendOrClusterDlg>(new CAddFriendOrClusterDlg());
		pInstance->Create(GetDesktopWindow(), _T("���|����"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

void CAddFriendOrClusterDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close();
	}
}

// ѡ��������
void CAddFriendOrClusterDlg::SetSearchType(FS_TYPE enType)
{
	if(enType == FS_XTREAM)
	{
		m_pOptionXtream->Selected(true);
	}
	else
	{
		m_pOptionCluster->Selected(true);
	}
}

void CAddFriendOrClusterDlg::UpdateBk()
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////
CResultListUI::CResultListUI(DuiLib::CPaintManagerUI& paint_manager)
	: paint_manager_(paint_manager)
{
	m_enType = FS_XTREAM;
}

CResultListUI::~CResultListUI()
{
}

void CResultListUI::AddItem(LPCTSTR szName, LPCTSTR szNickName, LPCTSTR szSex, DWORD tag)
{
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(dlgBuilder.Create(_T("AddFriendOrCluster_ListItem.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return;

	DuiLib::CButtonUI *pBtnName = static_cast<DuiLib::CButtonUI*>(pListElement->FindSubControl("name"));
	DuiLib::CLabelUI *pLabelNickName = static_cast<DuiLib::CLabelUI*>(pListElement->FindSubControl("nickname"));
	DuiLib::CLabelUI *pLabelSex= static_cast<DuiLib::CLabelUI*>(pListElement->FindSubControl("sex"));
	if(pBtnName == NULL || pLabelNickName == NULL || pLabelSex == NULL)
		return;

	pBtnName->SetText(szName);
	pLabelNickName->SetText(szNickName);
	pLabelSex->SetText(szSex);

	pListElement->SetTag(tag);		

	DuiLib::CListUI::Add(pListElement);
}

void CResultListUI::RemoveAll()
{
	for(int i = 0; i < this->GetCount(); ++i)
	{
		if(m_enType == FS_XTREAM)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)this->GetItemAt(i)->GetTag();
			delete pFriend;
			pFriend = NULL;
		}

		if(m_enType == FS_CLUSTER)
		{
			XT_CLUSTER_INFO * pInfo = (XT_CLUSTER_INFO*)this->GetItemAt(i)->GetTag();
			delete pInfo;
			pInfo = NULL;
		}
	}

	DuiLib::CListUI::RemoveAll();
}

//
//std::tr1::shared_ptr<CFSDialog> CFSDialog::pInstance = NULL;
//// CFSDialog �Ի���
//
//IMPLEMENT_DYNAMIC(CFSDialog, CXSkinDlg)
//CFSDialog::CFSDialog(CWnd* pParent /*=NULL*/)
//	: CXSkinDlg(CFSDialog::IDD, pParent)
//{
//	//////�����ɫ����///////////////
//	m_colorBackground = CHAT_BOTTOM_COLOR;
//	m_colorBorder = CHAT_BKG_COLOR;
//	DeleteObject(m_hBkGround);
//	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
//
//	m_pPageResult = NULL;
//	m_pPageSelect = NULL;
//
//	m_nDefault = 0;
//	m_nPage  = 0;
//	m_fsType = FS_XTREAM;
//	m_bmpLogo.LoadBitmap(IDB_SEARCH);
//	m_bSearch = FALSE;
//}
//
//CFSDialog::~CFSDialog()
//{
//	if(::IsWindow(GetSafeHwnd()))
//	{
//		DestroyWindow();
//	}
//	m_mapReqTick.clear();
//
//	delete m_pPageResult ;
//	delete m_pPageSelect ;
//}
//
//void CFSDialog::DoDataExchange(CDataExchange* pDX)
//{
//	CXSkinDlg::DoDataExchange(pDX);
//
//	DDX_Control(pDX,IDC_STATIC_PAGE,m_wndPage);
//	DDX_Control(pDX,IDC_BTN_PREV,m_btnPrev);
//	DDX_Control(pDX,IDC_BTN_CLOSE,m_btnClose);
//	DDX_Control(pDX,IDC_BTN_SEARCH,m_btnSearch);
//	DDX_Control(pDX,IDC_BTN_ADD,m_btnAdd);
//	DDX_Control(pDX, IDC_TAB_GROUP, m_ctlTabGroup);
//}
//
//
//BEGIN_MESSAGE_MAP(CFSDialog, CXSkinDlg)
//	ON_WM_NCDESTROY()
//	ON_MESSAGE(DM_GETDEFID, OnGetDefID)
//	ON_BN_CLICKED(IDC_BTN_SEARCH,OnBnClickedBtnSearch)
//	ON_BN_CLICKED(IDC_BTN_ADD,OnBnClickedBtnAdd)
//	ON_BN_CLICKED(IDC_BTN_PREV,OnBnClickedBtnPrev)
//	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBnClickedBtnClose)
//	ON_WM_TIMER()
//	ON_XT_MESSAGE()
//	ON_WM_CLOSE()
//	ON_WM_CTLCOLOR()
//	ON_WM_PAINT()
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_GROUP, &CFSDialog::OnTcnSelchangeTabGroup)
//END_MESSAGE_MAP()
//
//BEGIN_XT_MESSAGE_MAP(CFSDialog)
//	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK,OnEndSearch)
//	ON_XT_COMMAND(CMD_XTREAM_SEARCH_REQ,OnBeginSearch)
//	ON_XT_COMMAND(CMD_CLUSTER_SEARCH_ACK,OnEndSearch)
//	ON_XT_COMMAND(CMD_CLUSTER_SEARCH_REQ,OnBeginSearch)
//
//
//END_XT_MESSAGE_MAP()
//
//
//
//LRESULT CFSDialog::OnGetDefID(WPARAM wp, LPARAM lp) 
//{
//	
//	return MAKELONG(m_nDefault,DC_HASDEFID);// 
//} 
//
//void CFSDialog::OnHelpClick()
//{
//	OpenHtmlPage("www.feiniu.com");
//}
//
//BOOL CFSDialog::OnInitDialog()
//{
//	CXSkinDlg::OnInitDialog();
//	/*m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ACC_FINDFRIEND)); 
//	ASSERT(m_hAccel); */
//
//
//	// ���������ӿؼ������ÿؼ�����
//	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//	while(hWndChild)  
//	{  
//		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//	}
//
//	EnableHelp(true);
//
//	SetDlgStyle(MINCLOSE);
//	SetWindowText("���|����");
//	
//	CRect rcTab;
//	GetClientRect(&rcTab);
//	rcTab.top = 25 + 2;
//
//	m_ctlTabGroup.MoveWindow(rcTab);
//
//	CRect rcPage;
//	m_wndPage.GetClientRect(&rcPage);
//
//	m_pPageSelect = new CFSPageSelect(this,&m_wndPage);
///*
//	rcPage.top+=25;
//	rcPage.left = 0;
//	rcPage.bottom-=2;
//	rcPage.right-=2;
//	*/
//	m_pPageSelect->MoveWindow(&rcPage);
//	m_pPageSelect->ShowWindow(SW_SHOW);
//
//	m_pPageResult = new CFSPageResult(&m_wndPage);
//	m_pPageResult->MoveWindow(&rcPage);
//	m_pPageResult->ShowWindow(SW_HIDE);
//
//	m_nPage = 0;
//	ShowPage(0);
//
//	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_REQ,m_hWnd);
//	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,m_hWnd);
//	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_ACK,m_hWnd);
//	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_REQ,m_hWnd);
//
//	this->CenterWindow(GetDesktopWindow());
//
//	if ( g_ImUserInfo.status==XTREAM_OFFLINE )
//	{
//		m_btnSearch.ShowWindow(SW_HIDE);
//	}
//	/////////////////////////////////////////
//	m_ctlTabGroup.DeleteAllItems();
//	////////����TABҳ//////////////////////////////////////////
//	InitTabCtrl();
//
//	CAddFriendOrClusterDlg duiFrame;
//	duiFrame.Create(GetSafeHwnd(), _T("���|����"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
//	duiFrame.CenterWindow();
//	if(0 == duiFrame.ShowModal())
//	{
//
//	}
//
//	return TRUE;
//}
//// CFSDialog ��Ϣ�������
//void CFSDialog::OnNcDestroy()
//{
//	if(g_pProtocol != NULL)
//	{
//		g_pProtocol->UnRegistCmd(m_hWnd);
//	}
//
//	CXSkinDlg::OnNcDestroy();
//
//	m_pPageSelect->DestroyWindow();
//	m_pPageResult->DestroyWindow();
//}
//
//void CFSDialog::ShowPage(int nPage)
//{
//	if ( nPage==0 )
//	{
//		m_nDefault = IDC_BTN_SEARCH;
//		// ��ʾ��ҳ������ѡ��ҳ�棩
//		m_btnSearch.ShowWindow(SW_SHOW);
//		m_btnPrev.ShowWindow(SW_HIDE);
//		m_btnAdd.ShowWindow(SW_HIDE);
//		// ����ѡ��ҳ����ʾ����ѯ���ҳ������
//		m_pPageSelect->ShowWindow(SW_SHOW);
//		m_pPageResult->ShowWindow(SW_HIDE);
//		SetDlgItemText(IDC_STATIC_INFO,"");
//	}
//
//	if ( nPage==1 )
//	{
//		m_nDefault = 0;
//		// ��ʾ��ѯ���ҳ��
//		m_btnSearch.ShowWindow(SW_HIDE);
//		m_btnPrev.ShowWindow(SW_SHOW);
//		m_btnAdd.ShowWindow(SW_SHOW);
//
//		if ( m_pPageSelect->GetSearchType()==FS_XTREAM )
//		{
//			m_btnAdd.SetWindowText("��Ϊ����");
//		}
//		else
//		{
//			m_btnAdd.SetWindowText("����Ⱥ");
//		}
//
//		m_pPageSelect->ShowWindow(SW_HIDE);
//		m_pPageResult->ShowWindow(SW_SHOW);
//	}
//}
//
//void CFSDialog::OnBnClickedBtnSearch()
//{
//#ifdef _TEMP_DEBUG
//	 CLUSTER_INFO*pC=new CLUSTER_INFO();
//	CClusterInfoDlg* pWnd=CClusterInfoDlg::NewDlg(pC);
//	if(pWnd)
//	{	
//		pWnd->ShowWindow(SW_SHOW);
//		pWnd->CenterWindow(GetDesktopWindow());
//	}
//	delete pC;
//#endif
//	if ( m_pPageSelect->GetSearchType()==FS_XTREAM )//����xtream�û�
//	{
//		XT_XTREAM_SEARCH_REQ req;
//		if ( m_pPageSelect->GetSearchReq(req) )
//		{
//			req.startpos = 0;
//			m_bSearch = TRUE;
//			g_pProtocol->SendReq(req);
//			m_pPageResult->SetLastReqSet(g_pProtocol->GetCurSendSequnceId()-1);
//			m_pPageResult->SetSearchReq(req);
//
//			m_nPage = 1;
//			ShowPage(1);
//		}
//	}
//	else/////����Ⱥ
//	{
//		XT_CLUSTER_SEARCH_REQ req;
//
//		if ( m_pPageSelect->GetSearchReq(req) )
//		{
//			req.start_pos = 0;
//			m_bSearch = TRUE;
//			g_pProtocol->SendReq(req);
//			m_pPageResult->SetLastReqSet(g_pProtocol->GetCurSendSequnceId()-1);
//			m_pPageResult->SetSearchReq(req);
//
//			m_nPage = 1;
//			ShowPage(1);
//		}
//	}
//
//	/*if ( m_pPage1->GetSearchReq(req) )
//	{
//		g_pProtocol->SendReq(req);
//		m_pPageResult->SetSearchReq(req);
//
//		m_nPage=1;
//		ShowPage(1);
//	}*/
//}
//
//std::tr1::shared_ptr<CFSDialog> CFSDialog::Show()
//{
//	pInstance = tr1::shared_ptr<CFSDialog>(new CFSDialog(GetDesktopWindow()));
//	pInstance->Create(CFSDialog::IDD,GetDesktopWindow());
//	pInstance->ShowWindow(SW_SHOW);
//
//	return pInstance;
//}
//
//void CFSDialog::OnBnClickedBtnAdd()
//{
//	// ����ѡ��ҳ���ѡ����ȷ������Ⱥ������Ӻ���
//	if ( m_pPageSelect->GetSearchType()==FS_XTREAM )
//	{
//		// ��Ӻ���
//
//		FRIEND_INFO *pFriend = m_pPageResult->GetSelUser();
//
//		if ( pFriend )
//		{
//			if ( pFriend->GetId()==g_ImUserInfo.GetId() )
//			{
//				ShowMessage("���Ѿ����Լ��������ˣ�");
//				return ;
//			}
//
//			FRIEND_INFO * pRealFriend = FindFriend(pFriend->GetId());
//			CString str  = _T("");
//			BOOL    bRet =  FALSE;
//			if ( pRealFriend && 
//				(pRealFriend->group_index != GROUPID_STRANGER) && (pRealFriend->group_index != GROUPID_BLACKLIST) )
//			{
//				str.Format("%s �Ѿ�����ĺ����б��У�\r\n�����ظ���ӣ�" , UserInfoToStr(pRealFriend));
//			    /////ShowMessage(str);
//				////MessageBox(str);
//                MessageBox(str,IM_CAPTION,MB_OK|MB_ICONWARNING);
//				return ;
//			}
//			else
//			{
//				str = _T("����ϵ���ں�������,��Ϊ����,���Ӻ�������ɾ��");
//				if ( pFriend->rule==2 )
//				{
//				    bRet = isBlackListExists(pFriend->GetId());
//					if (bRet)
//					{
//						
//						if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
//						{
//	                       CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(this,GetDesktopWindow());
//					       // ������ӵ�������0��
//					       pDlg->SetUserAuthInfo(pFriend, 2, "",0);
//					       pDlg->ShowWindow(SW_SHOW);
//					       return;
//						}
//
//					}
//					else
//					{
//						CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(this,GetDesktopWindow());
//						// ������ӵ�������0��
//						pDlg->SetUserAuthInfo(pFriend, 2, "",0);
//						pDlg->ShowWindow(SW_SHOW);
//						return;
//					}
//				
//				}
//				else  if ( pFriend->rule==1 )
//				{
//					bRet = isBlackListExists(pFriend->GetId());
//					if (bRet)
//					{
//						if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
//						{
//	                        pFriend->bRefreshed=true;
//				        	// ������ӵ�������0��
//				         	CUserAuthReqDlg::Show(pFriend,0);
//					        return ;
//						}
//					}
//					else
//					{
//						pFriend->bRefreshed=true;
//						// ������ӵ�������0��
//						CUserAuthReqDlg::Show(pFriend,0);
//						return ;
//					}
//				}
//				bRet = isBlackListExists(pFriend->GetId());
//				if (bRet)
//				{
//					if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
//					{
//						map<uint32,DWORD>::iterator it = m_mapReqTick.find(pFriend->GetId());
//						bool bReq = false;
//
//						if ( it==m_mapReqTick.end() )
//						{
//							m_mapReqTick.insert(map<uint32,DWORD>::value_type(pFriend->GetId(),GetTickCount()));
//							bReq=true;
//						}
//						else
//						{
//							DWORD dwTick = GetTickCount();
//
//							if ( (dwTick-it->second)>24000 )//24��
//							{
//								it->second = dwTick;
//								bReq = true;
//							}
//						}
//
//						if ( bReq )
//						{
//							XT_FRIENDGROUP_MOD_REQ req;
//
//							req.action = GROUP_ADD;
//							req.uid    = g_ImUserInfo.GetId();
//							req.fid    = pFriend->GetId();
//							req.group_id = 0;
//
//							g_pProtocol->SendReq(req);
//
//							RegUserOperation(0x00c1);
//						}
//					}
//				}
//				else
//				{
//					map<uint32,DWORD>::iterator it = m_mapReqTick.find(pFriend->GetId());
//					bool bReq = false;
//
//					if ( it==m_mapReqTick.end() )
//					{
//						m_mapReqTick.insert(map<uint32,DWORD>::value_type(pFriend->GetId(),GetTickCount()));
//						bReq=true;
//					}
//					else
//					{
//						DWORD dwTick = GetTickCount();
//
//						if ( (dwTick-it->second)>24000 )//24��
//						{
//							it->second = dwTick;
//							bReq = true;
//						}
//					}
//
//					if ( bReq )
//					{
//
//						//amin ��Ӻ���ѡ�����
//						XT_FRIENDGROUP_MOD_REQ req;
//
//						req.action = GROUP_ADD;
//						req.uid    = g_ImUserInfo.GetId();
//						req.fid    = pFriend->GetId();
//						req.group_id = 0;
//
//						g_pProtocol->SendReq(req);
//
//						RegUserOperation(0x00c1);
//					}
//				}
//		    } 
//       }
//	   else
//	  {
//		    ShowMessage("��Ӳ��ҽ����ѡ��һ���û�!");
//	  }  
//	}
//	else
//	{
//		// ����Ⱥ
//		XT_CLUSTER_INFO *pCluster = m_pPageResult->GetSelCluster();
//
//		if ( pCluster==NULL )
//		{
//			ShowMessage("��Ӳ��ҽ����ѡ��һ��Ⱥ!");
//			return ;
//		}
//
//		if ( pCluster->nCreatorID == g_ImUserInfo.GetId() )
//		{
//			ShowMessage("�������Լ�������Ⱥ��:)");
//			return ;
//		}
//		for(int i=0;i<g_vecCluster.size();i++)
//		{
//			if(pCluster->nFamilyID == g_vecCluster[i]->nFamilyID)
//			{
//				ShowMessage("���Ѿ��Ǹ�Ⱥ�ĳ�Ա��:)");
//				return ;
//			}
//		}
//		
//
//		if ( pCluster->nJoinFlag!=2 )
//		{
//			XT_CLUSTER_APPLY_REQ req;
//			req.nAction     = 0; //�������Ⱥ
//			req.nFamilyID = pCluster->nFamilyID;
//			req.nFamilyProp = PERM_FAMILY;
//			req.nUserID = g_ImUserInfo.GetId();
//			g_pProtocol->SendReq(req);
//		}
//		else
//		{
//			CString str;
//			str.Format("Ⱥ��%s(%u) �ܾ��³�Ա���룡",pCluster->szFamilyName,pCluster->nFamilyID);
//			ShowMessage(str,MB_OK|MB_ICONINFORMATION);
//		}
//	}
//}
//
//void CFSDialog::OnBnClickedBtnPrev()
//{
//	m_nPage=0;
//	m_pPageResult->PreFind();
//	ShowPage(0);
//}
//
//void CFSDialog::OnBnClickedBtnClose()
//{
//	DestroyWindow();
//}
//
//void CFSDialog::OnBeginSearch(const char *data,WORD wRet)
//{
//	SetTimer(TIMER_SEARCH,300,NULL);
//
//	m_btnAdd.ShowWindow(SW_HIDE);
//	m_btnPrev.EnableWindow(FALSE);
//	m_pPageResult->EnableWindow(FALSE);
//}
//
//void CFSDialog::OnEndSearch(const char *data,WORD wRet)
//{
//	if(!m_bSearch)
//		return;
//	m_bSearch = FALSE;
//	KillTimer(TIMER_SEARCH);
//	CString sRst;
//	int nC=m_pPageResult->GetResultCount();
//	if(nC==0)
//		sRst="û���ҵ���ؼ�¼";
//	else
//		sRst.Format("��ǰ���ҵ�%d����¼",nC);
//	SetDlgItemText(IDC_STATIC_INFO,sRst);
//	m_btnAdd.EnableWindow(TRUE);
//	m_btnPrev.EnableWindow(TRUE);
//	m_pPageResult->EnableWindow(TRUE);
//
//	if ( wRet==SERVER_TIMEOUT )
//	{
//		ShowMessage("��������æ�����ҳ�ʱ��");
//	}
//}
//
//void CFSDialog::OnTimer(UINT nIDEvent)
//{
//	switch(nIDEvent)
//	{
//	case TIMER_SEARCH:
//		{
//			static CString strDot=".";
//			static int count=0;
//
//			if ( count++>3 )
//			{
//				count=0;
//				strDot="";
//			}
//
//			strDot += ".";
//
//			SetDlgItemText(IDC_STATIC_INFO,"������,��ȴ�"+strDot);
//		}
//		break;
//	default:
//		break;
//
//	}
//
//	CXSkinDlg::OnTimer(nIDEvent);
//}
//
//void CFSDialog::SetSearchType(FS_TYPE type)
//{
//	m_fsType=type;//0 p2p search, 1 cluster search
//	m_pPageSelect->SetSearchType(m_fsType);
//}
//
//BOOL CFSDialog::PreTranslateMessage(MSG* pMsg)
//{
//	//if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST) 
//	//{	
//	//	if (m_hAccel && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) 
//	//		return TRUE; 
//	//} 
//
//	if ( pMsg->message==WM_KEYDOWN )
//	{
//		switch (pMsg->wParam)
//		{
//		//case 0x0D:
//		case VK_ESCAPE:
//			DestroyWindow();
//			return TRUE;
//		default:
//			break;
//		}
//	}
//
//	return CDialog::PreTranslateMessage(pMsg);
//}
//
//void CFSDialog::OnClose()
//{
//	DestroyWindow();
//}
//
//HBRUSH CFSDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if ( nCtlColor == CTLCOLOR_STATIC )
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		hbr = m_hBkGround;
//	}
//
//	return hbr;
//}
//
//void CFSDialog::OnPaint()
//{
//	CPaintDC dc(this); 
//	CXSkinDlg::OnPaint();
//
//	//CWnd *pWnd = GetDlgItem(IDC_STATIC_LOGO);
//
//	//if ( pWnd )
//	//{
//	//	CRect rc;
//	//	pWnd->GetWindowRect(rc);
//	//	ScreenToClient(&rc);
//
//	//	DrawBitmap(&dc,rc.left,rc.top,&m_bmpLogo);
//	//}
//
//}
//
//void  CFSDialog::InitTabCtrl()
//{
//	m_ctlTabGroup.DeleteAllItems();
//	////////����TABҳ//////////////////////////////////////////
//	TCITEM   item;   
//	item.mask   =   TCIF_TEXT;  
//
//	item.pszText   =  _T("�����ϵ��") ;    
//	m_ctlTabGroup.InsertItem   (0,&item);   //�����һҳ
//	
//	item.pszText   =_T("����Ⱥ");      
//	m_ctlTabGroup.InsertItem   (1,&item);  
//}
//
//void CFSDialog::OnTcnSelchangeTabGroup(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//if ( m_nPage == 0 )
//	{
//		switch(m_ctlTabGroup.GetCurSel())  //�õ���ǰҳѡ��   
//		{     
//			case 0:  
//			{
//				SetSearchType(FS_XTREAM);	
//				break;	
//			}
//			case  1:  
//			{
//				SetSearchType(FS_CLUSTER);
//				break; 
//			}
//			default:
//				break;
//			}   
//	}
//	/*����ʾ��һҳ*/
//	ShowPage(0);
//	*pResult = 0;
//}