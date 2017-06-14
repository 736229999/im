#include "stdafx.h"
#include "DuiMfRecentList.h"
#include "im\WebProcess.h"
#include "im\LocalData.h"
#include "im\DuiP2PSendMsgDlg.h"
#include "im\MessageHandleWnd.h"
#include "im\ClusterSendMsgDlg.h"
#include "im\ClusterSettingDlg.h"
#include "im\MsgDlg.h"
#include "im\xtpacket.h"
#include "FriendInfoDlg.h"
#include "MainFrame.h"
#include "im\AddBlackDlg.h"
#include "im\DuiCounsel.h"
#include "im\ClusterInfoDlg.h"
#include "CSPlatDlg.h"

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern CMessageHandleWnd *g_pMessageHandleWnd;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);

extern BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu);

//最近联系人
//IMPLEMENT_DYNAMIC(CDuiRecentList, CListCtrlBase)

CDuiRecentList::CDuiRecentList(HWND hParent, DuiLib::CPaintManagerUI& paint_manager)
	: m_PaintManager(paint_manager)
	, m_hParent(hParent)
{
	m_nMaxCount = MAX_RECENT_USER;
}

CDuiRecentList::~CDuiRecentList()
{
	ClearRecentList();	
}

//
//BEGIN_MESSAGE_MAP(CDuiRecentList, CListCtrlBase)
//	ON_WM_DESTROY()
//	ON_XT_MESSAGE()
//	ON_WM_KILLFOCUS()
//	ON_COMMAND(ID_CLUSTER_TALK,OnClusterTalk)
//	ON_COMMAND(ID_CLUSTER_INFO,OnClusterInfo)
//	ON_COMMAND(ID_CLUSTER_PAGE,OnClusterPage)
//	ON_COMMAND(ID_CLUSTER_MSG,OnClusterMsg)
//	ON_CONTROL_REFLECT(LBN_DBLCLK, OnLbnDblclk)
//	ON_COMMAND(ID_CLEAR_RECENT,OnClearRecent)
//	ON_WM_LBUTTONUP()
//END_MESSAGE_MAP()
//
//BEGIN_XT_MESSAGE_MAP(CDuiRecentList)
//	// 好友详细资料获取
//	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
//	// 好友状态改变通知
//	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnUserStatusInd)
//	ON_XT_COMMAND(CMD_STATUS_GET_ACK,OnStatusGetAck)
//END_XT_MESSAGE_MAP()


void CDuiRecentList::Load()
{
	RemoveAll();
	ClearRecentList();

	g_LocalData.GetRecentFriends(m_flRecent);

	FRIEND_LIST::iterator it;
	int n=0;
	
	for ( it=m_flRecent.begin(); it!=m_flRecent.end(); it++ )
	{
		FRIEND_INFO *pUser = *it;

	//客服工作台中最近联系人不添加买家
#ifdef _CS_PLAT_EX_
		if(CMainFrame::IsBuyer(pUser->GetId()))
			continue;
#endif

		AddItem_Interface((DWORD)pUser);

		// 获取好友状态
		XT_STATUS_GET_REQ req;
		req.id = g_ImUserInfo.GetId();
		req.fid = pUser->GetId();
		g_pProtocol->SendReq(req);
	}
}

void CDuiRecentList::Save()
{
	g_LocalData.SaveRecentFriends(m_flRecent);
}

void CDuiRecentList::GetRealValue(DWORD dwData, FRIEND_INFO *&pFriend, CLUSTER_INFO *&pCluster)
{
	pFriend = NULL;
	pCluster = NULL;

	FRIEND_INFO *pTemp = (FRIEND_INFO*)dwData;
	
	if(pTemp->GetId() == (uint32)-1)
	{
		tagCluster c;
		GetCluster(pTemp,c);
		pCluster = FindClusterInfo(c.cid);
	}
	else
	{
		pFriend = pTemp;
	}
}

void CDuiRecentList::UpdateItem_Interface(DuiLib::CListContainerElementUI *pNode, DWORD dwData)
{
	if(pNode == NULL)
		return;

	DuiLib::CButtonGifUI *pIcon = (DuiLib::CButtonGifUI*)pNode->FindSubControl("lbHeaderIcon");
	DuiLib::CLabelUI *pName = (DuiLib::CLabelUI*)pNode->FindSubControl("lbName");
	DuiLib::CLabelUI *pTime = (DuiLib::CLabelUI*)pNode->FindSubControl("lbTime");
	DuiLib::CLabelUI *pMsg = (DuiLib::CLabelUI*)pNode->FindSubControl("lbMsg");

	FRIEND_INFO *pFriend = NULL;
	CLUSTER_INFO *pCluster = NULL;
	GetRealValue(dwData, pFriend, pCluster);
	if(pFriend == NULL && pCluster == NULL)
		return;

	pNode->SetTag(dwData);

	//最后一次聊天时间显示
	FRIEND_INFO *pTemp = (FRIEND_INFO*)dwData;
	CTime tm(pTemp->reserved);
	int nYear = tm.GetYear();
	int nMonth = tm.GetMonth();
	int nDay = tm.GetDay();

	uint32 now = g_LocalData.GetTimeNow();
	CTime tmNow(now);
	int nYearN = tmNow.GetYear();
	int nMonthN = tmNow.GetMonth();
	int nDayN = tmNow.GetDay();

	CTimeSpan ts(1,0,0,0);
	CTime timeYesterday = tmNow - ts;
	int nYearY = timeYesterday.GetYear();
	int nMonthY = timeYesterday.GetMonth();
	int nDayY = timeYesterday.GetDay();

	CTimeSpan ts2(2,0,0,0);
	CTime timeYesterdayB = tmNow - ts2;
	int nYearYB = timeYesterdayB.GetYear();
	int nMonthYB = timeYesterdayB.GetMonth();
	int nDayYB = timeYesterdayB.GetDay();

	char szText[1024] = {0};
	if(nYear == nYearY && nMonth == nMonthY && nDay == nDayY)
	{
		sprintf(szText," %s", "昨天");
	}
	else if (nYear == nYearYB && nMonth == nMonthYB && nDay == nDayYB)
	{
		sprintf(szText," %s", "前天");
	}
	else
	{
		if(nYear == nYearN)
		{
			if(nMonth == nMonthN && nDay == nDayN)
			{
				sprintf(szText," %s",tm.Format("%H:%M"));
			}
			else
			{
				sprintf(szText," %s",tm.Format("%m-%d"));
			}
		}
		else
		{
			sprintf(szText," %s",tm.Format("%y-%m-%d"));
		}
	}
	/*
	 else if(now- pTemp->reserved > 24*60*60 && now- pTemp->reserved < 24*60*60*2)
	 {
	 sprintf(szText," %s",tm.Format("昨天"));
	 }
	 else
	 {
	 sprintf(szText," %s",tm.Format("%m-%d"));
	 }*/
	pTime->SetText(szText);

	if(pFriend != NULL)
	{	
		// 名字
		CString sName = UserInfoToStr(pFriend);
		pName->SetText(sName);

		// 图标
		CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
		CString strGifFile = "";
		CString strPath =  pFriend->face_url;
		if(strPath.Find(".gif")!=-1)
		{
			strGifFile =  strFile + ".gif";////文件改名
			if ( !FileExist(strGifFile) )////不存在则创建
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
					string strHeadIcon = CutRoundImg(strFile, 40, 40, !bOnline);////是否在线的判断
					pIcon->SetBkImage(strHeadIcon.c_str());
					pIcon->SetNormalGifFile("");
				}
			}
			else
			{
				string strHeadIcon = CutRoundImg(strFile, 40, 40, !bOnline);
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}

		}

		// 最后一次消息记录
		MSG_LIST ml1;
		g_LocalData.GetLastMsgList(pFriend->GetId(),CDataManager::USER,ml1,1);
		POSITION pos = ml1.GetTailPosition();
		char szLsMsg[1400] = {0};
		if(pos != 0)//记录为空
		{
			sprintf(szLsMsg," %s", ml1.GetAt(pos).data);

			CString str = szLsMsg;
			//如果是图片则显示[图片]
			while(str.Find("<image:",0) != -1)
			{
				int nStart = str.Find("<image:",0);
				int nEnd = str.Find(">",0);
				str.Replace(str.Mid(nStart, nEnd - nStart + 1), "[图片]");
			}
			
			strcpy(szLsMsg, str);
		}
		pMsg->SetText(szLsMsg);
	}

	if(pCluster != NULL)
	{
		// 名字
		pName->SetText(pCluster->szFamilyName);
		// 图标
		pIcon->SetBkImage("Sysbtn\\icon_group_40x40.png");
		// 最后一次消息
		pMsg->SetText("");
	}
}

void CDuiRecentList::UpdateUser(FRIEND_INFO *pUser)
{
	int nItem;
	FRIEND_INFO *pItem=NULL;

	DuiLib::CListContainerElementUI *pNode = NULL;
	if (FindUser(pUser->GetId(),&pItem,&nItem, &pNode) && pNode != NULL)
	{
		//客服工作台中最近联系人不添加买家
#ifdef _CS_PLAT_EX_
		if(CMainFrame::IsBuyer(pUser->GetId()))
			return;
#endif
		uint32 reserved = pItem->reserved;		// 记录聊天时间
        memcpy(pItem,pUser,sizeof(FRIEND_INFO));
		pItem->reserved = reserved;
		UpdateItem_Interface(pNode, (DWORD)pItem);

		//Save();		// 不实时保存最近联系人了，因写sqlite 数据文件耗时比以前长
	}
}

void CDuiRecentList::DeleteUser(int nId)
{
	int nItem;
	FRIEND_INFO *pItem=NULL;

	DuiLib::CListContainerElementUI *pNode = NULL;
	if (FindUser(nId,&pItem,&nItem, &pNode) && pNode != NULL)
	{
		 Remove((DuiLib::CListContainerElementUI*)pNode);
		 list<FRIEND_INFO *>::iterator itr = m_flRecent.begin();
		 for(; itr != m_flRecent.end(); itr++)
		 {
			 if((*itr)->GetId() == nId)
			 {
				 m_flRecent.erase(itr);
				 break;
			 }
		 }
	}
}

void CDuiRecentList::UpdateCluster(CLUSTER_INFO *pCluster)
{
	int nItem;
	FRIEND_INFO *pItem=NULL;

	DuiLib::CListContainerElementUI *pNode = NULL;
	if (FindCluster(pCluster->nFamilyID,&pItem,&nItem, &pNode) && pNode != NULL)
	{
		tagCluster c;
		c.cid   = pCluster->nFamilyID;
		strcpy(c.name, pCluster->szFamilyName);
		strcpy(c.domain, pCluster->szFamilyDomain);

		SetCluster(pItem, c);

		UpdateItem_Interface(pNode, (DWORD)pItem);

		//Save();		// 不实时保存最近联系人了，因写sqlite 数据文件耗时比以前长
	}
}

// 界面添加节点（好友或者群）
void CDuiRecentList::AddItem_Interface(DWORD dwData)
{
	FRIEND_INFO *pFriend = NULL;
	CLUSTER_INFO *pCluster = NULL;
	GetRealValue(dwData, pFriend, pCluster);

#ifdef _CS_PLAT_EX_
	if(CMainFrame::IsBuyer(pFriend->GetId()))
		return;
#endif
	if(pFriend == NULL && pCluster == NULL)
		return;

	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(dlgBuilder.Create(_T("RecentPage_Item.xml"), (UINT)0, NULL, &m_PaintManager));
	if (pListElement == NULL)
		return;

	pListElement->SetFixedHeight(46);
	pListElement->SetTag(dwData);

	UpdateItem_Interface(pListElement, dwData);

	AddAt(pListElement, 0);
}

void CDuiRecentList::AddCluster(CLUSTER_INFO *pCluster)
{
	FRIEND_LIST::iterator it;

	int nItem;
	FRIEND_INFO *pItem=NULL;

	tagCluster c;
	c.cid             = pCluster->nFamilyID;
	strcpy(c.domain, pCluster->szFamilyDomain);
	strcpy(c.name,   pCluster->szFamilyName);

	DuiLib::CListContainerElementUI *pNode = NULL;
	bool bFind = FindCluster(pCluster->nFamilyID,&pItem,&nItem,&pNode);
	int nC=0;
	if ( bFind==false && pNode == NULL)
	{
		// 获得当前联系人的个数
		nC = GetCount();
		ASSERT(m_nMaxCount>=0);
		if(nC >= m_nMaxCount)
		{
			DuiLib::CListContainerElementUI *pLastNode = (DuiLib::CListContainerElementUI*)GetItemAt(nC-1);
			FRIEND_INFO *pFriend = (FRIEND_INFO*)pLastNode->GetTag();
			if(pFriend)
			{
				FRIEND_LIST::iterator it,itE;
				itE = m_flRecent.end();
				for(it = m_flRecent.begin();it != itE;it++)
				{
					if(*it == pFriend)
					{
						delete pFriend;
						m_flRecent.erase(it);
						break;
					}
				}
			}
			// 如果超过规定个数，则删除最后一个
			RemoveAt(nC-1);
		}

		FRIEND_INFO *pRecent = new FRIEND_INFO();
		pRecent->SelfEncrypt();
		pRecent->SetId((uint32)-1);
		pRecent->reserved = /*time(0)*/g_LocalData.GetTimeNow();

		SetCluster(pRecent, c);

		m_flRecent.push_back(pRecent);

		AddItem_Interface((DWORD)pRecent);
	}
	else
	{
		pItem->reserved = /*time(0)*/g_LocalData.GetTimeNow();

		SetCluster(pItem, c);
	
		if ( nItem==0 )
		{
			UpdateItem_Interface(pNode, (DWORD)pItem);
		}
		else
		{
			Remove(pNode);
			AddItem_Interface((DWORD)pItem);
		}
	}

	//Save();		// 不实时保存最近联系人了，因写sqlite 数据文件耗时比以前长
}

void CDuiRecentList::AddUser(FRIEND_INFO *pUser)
{
	//客服工作台中最近联系人不添加买家
//#ifdef _CS_PLAT_EX_
//	if(CMainFrame::IsBuyer(pUser->GetId()))
//	{
//		return;
//	}
//#endif
	FRIEND_LIST::iterator it;

	int nItem;
	FRIEND_INFO *pItem=NULL;
	int nC=0;

	DuiLib::CListContainerElementUI *pNode = NULL;
	bool bFind = FindUser(pUser->GetId(),&pItem,&nItem,&pNode);
	bool bIsRecent = false;
	list<FRIEND_INFO *>::iterator itr = m_flRecent.begin();
	for (; itr != m_flRecent.end(); itr++)
	{
		if((*itr)->GetId() == pUser->GetId())
		{
			bIsRecent = true;
			break;
		}
	}
	if ( bFind==false && pNode == NULL && !bIsRecent)
	{
		// 获得当前联系人的个数
		nC = GetCount();
		ASSERT(m_nMaxCount>=0);
		if(nC >= m_nMaxCount)
		{
			DuiLib::CListContainerElementUI *pLastNode = (DuiLib::CListContainerElementUI*)GetItemAt(nC-1);
			FRIEND_INFO *pFriend = (FRIEND_INFO*)pLastNode->GetTag();
			if(pFriend)
			{
				FRIEND_LIST::iterator it,itE;
				itE = m_flRecent.end();
				for(it = m_flRecent.begin();it != itE;it++)
				{
					if(*it == pFriend)
					{
						delete pFriend;
						m_flRecent.erase(it);
						break;
					}
				}
			}
			// 如果超过规定个数，则删除最后一个
			RemoveAt(nC-1);
		}
		FRIEND_INFO *pRecent = new FRIEND_INFO();
		memcpy(pRecent,pUser,sizeof(FRIEND_INFO));

		pRecent->reserved = /*time(0)*/g_LocalData.GetTimeNow();
		m_flRecent.push_back(pRecent);
		
		AddItem_Interface((DWORD)pRecent);
	}
	else
	{
		if(bFind)
		{
			memcpy(pItem,pUser,sizeof(FRIEND_INFO));
			pItem->reserved = /*time(0)*/g_LocalData.GetTimeNow();

			if ( nItem==0 )
			{
				UpdateItem_Interface(pNode, (DWORD)pItem);
			}
			else
			{
				Remove(pNode);
				AddItem_Interface((DWORD)pItem);
			}
		}

	}

	//Save();		// 不实时保存最近联系人了，因写sqlite 数据文件耗时比以前长
}

void CDuiRecentList::OnUserInfoGet(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;
		
		FRIEND_INFO *pUser=NULL;
		int nItem;
		DuiLib::CListContainerElementUI *pNode = NULL;
		FindUser(ack->GetId(),&pUser,&nItem,&pNode);

		if ( pUser )
		{
			UpdateUserFullInfo(pUser,ack);
			FRIEND_INFO f= *pUser;
			//memcpy(&f,ack,sizeof(XT_USER_INFO));
			UpdateUser(&f);
		}
	}
}

void CDuiRecentList::DblClick(int nSel)
{
	if ( nSel>-1 && nSel<GetCount() )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI *)GetItemAt(nSel);
		FRIEND_INFO *pItem = (FRIEND_INFO *)pNode->GetTag();

		if ( pItem->GetId()!=(uint32)-1 )
		{	// 打开聊天窗口
			CMainFrame::OpenP2PMsgDlg(pItem);
		}
		else
		{
			tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

				if ( pDlg->m_hWnd==NULL )
					pDlg->Create(CClusterSendMsgDlg::IDD,CWnd::FromHandle(GetDesktopWindow()));

				pDlg->ShowWindow(SW_SHOW);

				if ( pDlg->IsIconic() )
					pDlg->ShowWindow(SW_RESTORE);

				pDlg->BringWindowToTop();
			}
        }
	}
}

void CDuiRecentList::OnClusterInfo()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI *)GetItemAt(nSel);
		FRIEND_INFO *pItem = (FRIEND_INFO *)pNode->GetTag();

		if ( pItem->GetId()==(uint32)-1 )
		{
            tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			/*	CClusterParamSetting::Show(pCluster);*/
			{
				std::tr1::shared_ptr<CClusterInfoDlg> pWnd=CClusterInfoDlg::NewDlg(pCluster);
				if(pWnd)
				{
					// 向服务器请求群信息
					XT_GET_CINFO_REQ req;
					req.nFamilyID   = pCluster->nFamilyID;
					req.nFamilyProp = PERM_FAMILY;
					g_pProtocol->SendReq(req);
					// 显示群信息
					//pWnd->ShowWindow(false);
					pWnd->ShowWindow(true);
					//pWnd->CenterWindow(/*CWnd::FromHandle(GetDesktopWindow())*/);
				}
			}
		}
	}
}

void CDuiRecentList::OnClusterTalk()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		DblClick(nSel);
	}
}

void CDuiRecentList::OnClusterMsg()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI *)GetItemAt(nSel);
		FRIEND_INFO *pItem = (FRIEND_INFO *)pNode->GetTag();

		if ( pItem->GetId()==(uint32)-1 )
		{
            tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
				pDlg->SetDefaultCluster(pCluster->nFamilyID);
			}
		}
	}
}

void CDuiRecentList::OnUserStatusInd(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;
		FRIEND_INFO *pFriend = NULL;

		int nItem;
		
		DuiLib::CListContainerElementUI *pNode = NULL;
		FindUser(ind->fid,&pFriend,&nItem,&pNode);

		//如果是隐身的话，则状态图标和离开一样
		if ( ind->status==XTREAM_HIDE )
		{
			ind->status=XTREAM_OFFLINE;
		}

		if ( pFriend && pFriend->status != ind->status)
		{
			pFriend->ver    = ind->version;
			pFriend->status = ind->status;

			UpdateItem_Interface(pNode, (DWORD)pFriend);
		}
	}
}


void CDuiRecentList::ClearRecentList()
{
	FRIEND_LIST::iterator it;

	for ( it=m_flRecent.begin(); it!=m_flRecent.end(); it++ )
	{
		delete (*it);
	}

	m_flRecent.clear();
}

bool CDuiRecentList::FindUser(uint32 fid, FRIEND_INFO **ppFriend, int *pnItem, DuiLib::CListContainerElementUI **ppNode)
{
	int count=GetCount();

	for ( int i=0; i<count; i++ )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI*)GetItemAt(i);
		FRIEND_INFO *pUser=(FRIEND_INFO *)(pNode->GetTag());

		if ( pUser->GetId()==fid )
		{
			*pnItem   = i;
			*ppFriend = pUser;
			*ppNode = pNode;
			return true;
		}
	}

	return false;
}

bool CDuiRecentList::FindCluster(uint32 cid, FRIEND_INFO **ppFriend, int *pnItem, DuiLib::CListContainerElementUI **ppNode)
{
	int count=GetCount();

	for ( int i=0; i<count; i++ )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI*)GetItemAt(i);
		FRIEND_INFO *pUser=(FRIEND_INFO *)(pNode->GetTag());

		if ( pUser->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pUser, c);

			if ( c.cid==cid )
			{
				*pnItem	   = i;
				*ppFriend  = pUser; 
				*ppNode = pNode;
				return true;
			}
		}
	}

	return false;
}

void CDuiRecentList::GetCluster(FRIEND_INFO *pFriend, tagCluster & c)
{
	CRecvPacket rp(pFriend->description,sizeof(pFriend->description));
	rp>>c.cid>>c.domain>>c.name;
}


void CDuiRecentList::SetCluster(FRIEND_INFO *pFriend, const tagCluster & c)
{
	CSendPacket sp(pFriend->description);
	sp<<c.cid<<c.domain<<c.name;
}

void CDuiRecentList::OnLbnDblclk()
{
	int nSel = GetCurSel();

	DblClick(nSel);
}


void CDuiRecentList::OnClearRecent()
{
	ClearRecentList();
	RemoveAll();
}

// 获取最近联系人
const FRIEND_LIST* CDuiRecentList::GetRecentFriend()
{
	return &m_flRecent;
}

// 获取好友状态
void CDuiRecentList::OnStatusGetAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;

		if ( ack )
		{
			FRIEND_INFO *pFriend = NULL;
			int nItem;
			DuiLib::CListContainerElementUI *pNode = NULL;
			FindUser(ack->id,&pFriend,&nItem,&pNode);

			if ( pFriend )
			{
				if ( ack->status==XTREAM_HIDE )
				{
					ack->status=XTREAM_OFFLINE;
				}			
				pFriend->ip     = ack->ip;
				pFriend->port   = ack->port;
				pFriend->localIP = ack->lanip;
				pFriend->localPort = ack->lanport;
				pFriend->status = ack->status;
				pFriend->ver    = ack->ver;
				memcpy(pFriend->key,ack->p2pkey,16);
				pFriend->status = ack->status;

				UpdateItem_Interface(pNode, (DWORD)pFriend);
			}
		}
	}
}

void CDuiRecentList::OnFriendInfo()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemAt(nSel)->GetTag();
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			pMainFrame->OnBnClickedFaceIcon();
		}
		else
		{
			FRIEND_INFO* pGroupFriend = GetFriendInfo(pFriend->GetId());
			if(pGroupFriend != NULL)
				CFriendInfoDlg::NewDlg(pGroupFriend);
			else
				CFriendInfoDlg::NewDlg(pFriend);
		}
	}
}

void CDuiRecentList::OnP2PTalk()
{
	int nSel = GetCurSel();
	if (nSel>-1)
	{
		DblClick(nSel);
	}
}

void CDuiRecentList::OnFriendMsg()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemAt(nSel)->GetTag();
		std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
		pDlg->SetDefaultFriend(pFriend->GetId());
	}
}

void CDuiRecentList::OnFriendAdd()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemAt(nSel)->GetTag();

		if ( pFriend )
		{
			CAddBlackDlg::Show(pFriend->GetId());
		}
	}
}

void CDuiRecentList::DoEvent(DuiLib::TEventUI& event)
{
	if( event.Type == DuiLib::UIEVENT_RBUTTONDOWN )
	{
		CPoint pt;
		GetCursorPos(&pt);

		CMenuEx menu, *popmenu;
		menu.LoadMenu(IDR_MENU_TEMPLATE);
		popmenu = menu.GetSubMenu(0);
		popmenu->RemoveMenu(0,MF_BYPOSITION);
		popmenu->AppendMenu(MF_STRING,ID_CLEAR_RECENT,"清空列表");	
		
		DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
		DuiLib::CPoint duiPoint(pt.x,pt.y);
		pMenu->Init(NULL, _T("loginstatus.xml"), pt, &m_PaintManager);
		AppendDuiMenu(pMenu, popmenu);
	}

	CListUI::DoEvent(event);
}