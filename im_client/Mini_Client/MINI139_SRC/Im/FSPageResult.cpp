//// im\FSPage2.cpp : ʵ���ļ�
////
//
#include "stdafx.h"
//#include "..\IMClient.h"
//#include ".\fspageresult.h"
//#include "xtpacket.h"
//#include "..\FriendInfoDlg.h"
//#include "ClusterInfoDlg.h"
//
//#include "..\controls\skinctrl.h"
//#include "MessageBox.h"
//#ifdef _DEBUG
//
////#define _TEMP_DEBUG
//#include <Windows.h>
//
//#endif
//
//// CFSPageResult �Ի���
//#define  LIST_HEADER_WIDTH 103
//IMPLEMENT_DYNAMIC(CFSPageResult, CDialog)
////IMPLEMENT_IM_SB(CFSPageResult)
//
//CFSPageResult::CFSPageResult(CWnd* pParent /*=NULL*/)
//	: CDialog(CFSPageResult::IDD, pParent)
//{
//	Create(IDD,pParent);
//	m_pCurXtreamPage  = NULL;
//	m_pCurClusterPage = NULL;
//	m_hBrushBackGround = CreateSolidBrush(RGB(255,255,255));
//	m_hBrushDisable = CreateSolidBrush(RGB(200,200,200));
//
//	m_nLastReqSeq = 0;
//}
//
//CFSPageResult::~CFSPageResult()
//{
//	ClearXtreamPages();
//	ClearClusterPages();
//
//	DeleteObject(m_hBrushBackGround);
//	DeleteObject(m_hBrushDisable);
//}
//
//void CFSPageResult::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_LIST_FRIEND, m_wndListFriend);
//	DDX_Control(pDX,IDC_STATIC_NEXT,m_btnNext);
//	DDX_Control(pDX,IDC_STATIC_PREV,m_btnPre);
//	DDX_Control(pDX,IDC_STATIC_USERINFO,m_btnUserInfo);
//	DDX_Control(pDX, IDC_LIST_CLUSTER, m_wndListCluster);
//}
//
//
//BEGIN_MESSAGE_MAP(CFSPageResult, CDialog)
//	ON_WM_CTLCOLOR()
//	ON_WM_NCDESTROY()
//	ON_XT_MESSAGE()
//	ON_STN_CLICKED(IDC_STATIC_USERINFO, OnStnClickedStaticUserinfo)
//	ON_STN_CLICKED(IDC_STATIC_PREV, OnStnClickedStaticPrev)
//	ON_STN_CLICKED(IDC_STATIC_NEXT, OnStnClickedStaticNext)
//	//SET_IM_SB(IDC_LIST_FRIEND )
//	//SET_IM_SB(IDC_LIST_CLUSTER )
//	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
//	ON_WM_SHOWWINDOW()
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLUSTER, &CFSPageResult::OnLvnItemchangedListCluster)
//END_MESSAGE_MAP()
//
//BEGIN_XT_MESSAGE_MAP(CFSPageResult)
//	// �������ߺ��ѽ��
//	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK,OnFriendSearchAck)
//	// ��������Ⱥ���
//	ON_XT_COMMAND(CMD_CLUSTER_SEARCH_ACK,OnClusterSearchAck)
//END_XT_MESSAGE_MAP()
//
//
//// CFSPageResult ��Ϣ�������
//
//BOOL CFSPageResult::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	CRect rt;
//	m_wndListFriend.GetClientRect(&rt);
//	int nW=rt.Width();
//	nW-=210;
//	if(nW<30)
//	{
//		nW=30;
//	}
//	m_wndListFriend.InsertColumn(0,"�û���",LVCFMT_LEFT,LIST_HEADER_WIDTH);
//	m_wndListFriend.InsertColumn(1,"�ǳ�",LVCFMT_LEFT,LIST_HEADER_WIDTH);
//	m_wndListFriend.InsertColumn(2,"�Ա�",LVCFMT_LEFT,nW);
//	//m_wndListFriend.InsertColumn(3,"����",LVCFMT_LEFT,80);
//
//	m_wndListFriend.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
//	m_wndListFriend.SetImageList(&g_imgStatus24,LVSIL_SMALL);
//
//	m_wndListCluster.GetClientRect(&rt);
//	nW=rt.Width();
//	nW-=100;
//	if(nW<30)
//	{
//		nW=30;
//	}
//	m_wndListCluster.InsertColumn(0,"Ⱥ����",LVCFMT_LEFT,LIST_HEADER_WIDTH);
//	m_wndListCluster.InsertColumn(1,"Ⱥ��Ա",LVCFMT_LEFT,LIST_HEADER_WIDTH);
//	m_wndListCluster.InsertColumn(2,"Ⱥ�ȼ�",LVCFMT_LEFT,LIST_HEADER_WIDTH);
//	m_wndListCluster.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
//
//	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,m_hWnd);
//	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_ACK,m_hWnd);
//	
////	AttachXtScrollbar(m_wndListFriend.m_hWnd);
////	AttachXtScrollbar(m_wndListCluster.m_hWnd);
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// �쳣: OCX ����ҳӦ���� FALSE
//}
//
//void CFSPageResult::ClearClusterPages()
//{
//	for ( int i=0; i<m_vecClusterPages.size(); i++ )
//	{
//		delete (tagClusterPage *)m_vecClusterPages[i];
//	}
//
//	m_vecClusterPages.clear();
//}
//
//void CFSPageResult::ClearXtreamPages()
//{
//	for ( int i=0; i<m_vecXtreamPages.size(); i++ )
//	{
//		delete (tagXtreamPage *)m_vecXtreamPages[i];
//	}
//
//	m_vecXtreamPages.clear();
//}
//
//void CFSPageResult::SetSearchReq(const XT_CLUSTER_SEARCH_REQ &req)
//{
//	m_clusterReq = req;
//	m_pCurClusterPage = NULL;
//
//	m_wndListFriend.ShowWindow(SW_HIDE);
//	m_wndListCluster.ShowWindow(SW_SHOW);
//	m_btnUserInfo.ShowWindow(SW_SHOW);
//
//	m_wndListCluster.DeleteAllItems();
//
//	ClearClusterPages();
//
//	m_resultType = RT_CLUSTER;
//}
//
//void CFSPageResult::SetSearchReq(const XT_XTREAM_SEARCH_REQ &req)
//{
//	m_xtreamReq  = req;
//	m_pCurXtreamPage   = NULL;
//
//	m_wndListCluster.ShowWindow(SW_HIDE);
//	m_wndListFriend.ShowWindow(SW_SHOW);
//
//	m_btnUserInfo.ShowWindow(SW_SHOW);
//
//	m_wndListFriend.DeleteAllItems();
//
//	ClearXtreamPages();
//
//	m_resultType = RT_XTREAM;
//}
//
//HBRUSH CFSPageResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if ( nCtlColor==CTLCOLOR_STATIC  )
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		hbr = g_hWhiteBrush;
//	}
//
//	if ( nCtlColor==CTLCOLOR_DLG  )
//	{
//		hbr = g_hWhiteBrush;
//	}
//
//	return hbr;
//}
//
//void CFSPageResult::OnClusterSearchAck(const char *data,WORD wRet)
//{
//	if ( wRet==0 )
//	{
////		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
////
////		int i;
////
////		uint16	next_pos;
////		uint8	count;
////
////		/*Cluster No Ⱥ��	4 Bytes, Big Endian unsigned long
////			Creator Ⱥ�����ߺ���	4 Bytes, Big Endian unsigned long
////			��֤��ʽ	1 Byte. 0x00������Ҫ��֤��0x01������֤��0x02����������ӡ�
////			Ⱥ����	1 Byte Ĭ��ѡ�ͬѧ¼������ᡢͬ��¼���������ˡ�����
////			NameȺ��	Max 20 Bytes(Optional), Text ,end with \0. 
////
////*/
////		rp>>next_pos>>count;
////
////		for ( i=0; i<m_vecClusterPages.size(); i++ )
////		{
////			if ( m_vecClusterPages[i]->next_pos==next_pos &&
////				m_vecClusterPages[i]->Size()==count )
////			{
////				//�ظ��İ�������
////				return ;
////			}
////		}
//
//		//ȷ��������Ӧ�������һ�η��������Ӧ��Ӧ�������֮ǰ������Ӧ������
//		if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() ){
//			return;
//		}
//		
//		LPXT_CLUSTER_SEARCH_ACK ack = (LPXT_CLUSTER_SEARCH_ACK)data;
//
//		if(ack->nCount == 0)
//		{
//			m_btnUserInfo.EnableWindow(FALSE);
//		}
//		else
//		{
//			m_btnUserInfo.EnableWindow(TRUE);
//		}
//		tagClusterPage *pPage  = new tagClusterPage;
//		pPage->next_pos = ack->next_pos;
//		for ( int i=0; i<ack->nCount; i++ )
//		{
//			XT_CLUSTER_INFO * pInfo = new XT_CLUSTER_INFO();
//
//			pInfo->nFamilyID  = ack->FamilySearchData[i].nFamilyID;
//			pInfo->nCreatorID = ack->FamilySearchData[i].nCreaterID;
//			pInfo->nJoinFlag  = ack->FamilySearchData[i].nJoinFlag;
//			pInfo->nFamilyClass  = ack->FamilySearchData[i].nFamilyClass;
//			strcpy(pInfo->szFamilyName,ack->FamilySearchData[i].szFamilyName);
//			strcpy(pInfo->szFamilyDomain,ack->FamilySearchData[i].szFamilyDomain);
//			pInfo->nNumberLimit  = ack->FamilySearchData[i].nMemCount;///����Ⱥ��Ա����
//			pPage->AddFamily(pInfo);
//			//pPage->vecResults.push_back(pInfo);
//		}
//
//		m_vecClusterPages.push_back(pPage);
//		m_pCurClusterPage = pPage;
//		ShowResult(pPage);
//
//		if ( ack->next_pos!=0xFFFF )
//		{
//			m_btnNext.EnableWindow(TRUE);
//		}
//		else
//		{
//			m_btnNext.EnableWindow(FALSE);
//		}
//
//		if ( m_vecClusterPages.size()>1 )
//		{
//			m_btnPre.EnableWindow(TRUE);
//		}
//		else{
//          m_btnNext.EnableWindow(FALSE);		
//		}
//	}
//	else
//	{
//		m_btnPre.EnableWindow(FALSE);
//		m_btnNext.EnableWindow(FALSE);
//		m_btnUserInfo.EnableWindow(FALSE);
//		GetDlgItem(IDC_STATIC_PAGE)->EnableWindow(FALSE);
//
//		if ( wRet!=0xFFFF )
//		{
//			//MessageBox(data);
//			CMessageBox::Prompt(data);
//		}
//	}	
//}
//
//void CFSPageResult::OnFriendSearchAck(const char *data,WORD wRet)
//{
//	if ( wRet==0 )
//	{
//		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
//
//		//ȷ��������Ӧ�������һ�η��������Ӧ��Ӧ�������֮ǰ������Ӧ������
//		if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() )
//		{
//			return;
//		}
//
//		int i;
//		uint16	next_pos;
//		uint32	total;
//		int		nTotal;
//		uint8	count;
//		uint8	realCount = 0;
//		rp>>count>>next_pos>>total;
//		// ����������
//		nTotal = total;
//		/*
//		for ( i=0; i<m_vecXtreamPages.size(); i++ )
//		{
//			if ( m_vecXtreamPages[i]->next_pos == next_pos &&
//				 m_vecXtreamPages[i]->Size() == count )
//			{
//				//�ظ��İ�������
//				return ;
//			}
//		}
//		*/
//		m_btnUserInfo.EnableWindow(FALSE);
//		tagXtreamPage *pPage  = new tagXtreamPage;
//		pPage->next_pos = next_pos;
//		for ( i=0; i<count; i++ )
//		{
//			FRIEND_INFO * pFriend = new FRIEND_INFO();
//
//			uint32 id = 0;
//			rp  >>id;
//			pFriend->SetId(id);
//			rp	>>pFriend->sex
//				>>pFriend->status
//				>>pFriend->rule;
//			char username[MAX_USERNAME_LEN+1] = {0};
//			rp	>>username;
//			pFriend->SetUserName(username);
//			rp	>>pFriend->nickname;
//			if ( (pFriend->GetId() != g_ImUserInfo.GetId()) )
//			{
//				if(pFriend->status == FRIEND_INFO::HIDE)
//				{
//					pFriend->status = FRIEND_INFO::OFFLINE;
//				}
//				if(pPage->IsIDExist(pFriend->GetId()))
//				{
//					delete pFriend;
//				}
//				else
//				{
//					pPage->AddFriend(pFriend);
//					m_btnUserInfo.EnableWindow(TRUE);
//				}
//			}
//			else
//			{
//				// ��ȥ�Լ�
//				nTotal--;
//				delete pFriend;
//			}
//		}
//
//		m_vecXtreamPages.push_back(pPage);
//		// ��ǰ���ҳ
//		m_pCurXtreamPage = pPage;
//        ShowResult(pPage);
//		if(  GetResultCount() < nTotal )
//		{
//			if(count >= nTotal)
//				m_btnNext.EnableWindow(FALSE);// ��ֹ�������˵Ĵ���
//			else
//				m_btnNext.EnableWindow(TRUE);
//		}
//		else
//		{
//			m_btnNext.EnableWindow(FALSE);
//		}
//
//		if ( m_vecXtreamPages.size()>1 )
//		{
//			m_btnPre.EnableWindow(TRUE);
//		}
//	}
//	else
//	{
//		m_btnPre.EnableWindow(FALSE);
//		m_btnNext.EnableWindow(FALSE);
//		m_btnUserInfo.EnableWindow(FALSE);
//		GetDlgItem(IDC_STATIC_PAGE)->EnableWindow(FALSE);
//	}
//}
//
//void CFSPageResult::ShowResult(tagClusterPage * pPage)
//{
//	if (m_wndListCluster.GetItemCount()>0)
//	{
//        m_wndListCluster.DeleteAllItems();
//	}
//	CString strItem = _T("");
//	for ( int i=0; i<pPage->Size(); i++ )
//	{
//		XT_CLUSTER_INFO *pInfo = pPage->GetResult()[i];
//		strItem.Format("%s",pInfo->szFamilyName);//�˴���Ⱥ���滻��Ⱥ��
//		if(strItem== _T(""))///Ⱥ��Ϊ�յ����
//		{
//			return ;
//		}
//		m_wndListCluster.InsertItem(i,strItem/*,-1*/);
//		 //////////////////////////////////////////////////////////////////////
//		//�ӱ��ز���Ⱥ����Ϣ
//		//CLUSTER_INFO * pCluster = FindClusterInfo(pInfo->nFamilyID);
//		//if(pCluster!=NULL)/////��������ǰȺ����Ϣ
//		//{
//		//	strItem.Format("%u",pCluster->Count());  //Ⱥ��Ա��///��ǰȺ��Ա��
//		//	m_wndListCluster.SetItemText(i,1,strItem);
//		//}
//	    //////////////////////////////////////////////////////////////////////
//
//	    if(pInfo->nNumberLimit>0)//���������ص�Ⱥ��Ա��
//		{
//	       strItem.Format("%u",pInfo->nNumberLimit);  
//		   m_wndListCluster.SetItemText(i,1,strItem);
//		}
//	
//		if (FAMILY_COMMON == pInfo->nFamilyType) //Ⱥ�ȼ�
//		{
//			strItem.Format("��ͨȺ");
//			m_wndListCluster.SetItemText(i,2,strItem);
//		}
//		if (FAMILY_VIP   == pInfo->nFamilyType)
//		{
//			strItem.Format("VIPȺ");
//			m_wndListCluster.SetItemText(i,2,strItem);
//		}
//		m_wndListCluster.SetItemData(i,(DWORD)pInfo);
//	}
//}
//
//void CFSPageResult::ShowResult(tagXtreamPage *pPage)
//{
//	m_wndListFriend.DeleteAllItems();
//	CString strItem;
//
//	for ( int i=0; i<pPage->Size(); i++ )
//	{
//		FRIEND_INFO *pFriend = pPage->GetResult()[i];
//		m_wndListFriend.InsertItem(i,"");
//		m_wndListFriend.SetItemImage(i,0,GetDefaultImage(pFriend->sex,pFriend->status));
//		m_wndListFriend.SetItemText(i,0,pFriend->GetUserName().c_str());
//		m_wndListFriend.SetItemText(i,1,pFriend->nickname);
//		m_wndListFriend.SetItemText(i,2,SexToStr(pFriend->sex));
//		m_wndListFriend.SetItemData(i,(DWORD)pFriend);
//	}
//}
//
//void CFSPageResult::OnNcDestroy()
//{
//	if(g_pProtocol != NULL)
//		g_pProtocol->UnRegistCmd(m_hWnd);
//	CDialog::OnNcDestroy();	
//}
//
//XT_CLUSTER_INFO * CFSPageResult::GetSelCluster()
//{
//	POSITION pos = m_wndListCluster.GetFirstSelectedItemPosition();
//	if (pos)
//	{
//		int nIndex = m_wndListCluster.GetNextSelectedItem(pos);
//		return (XT_CLUSTER_INFO *)m_wndListCluster.GetItemData(nIndex);
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
//FRIEND_INFO * CFSPageResult::GetSelUser()
//{
//	POSITION pos = m_wndListFriend.GetFirstSelectedItemPosition();
//	if (pos)
//	{
//		int nIndex = m_wndListFriend.GetNextSelectedItem(pos);
//		return (FRIEND_INFO *)m_wndListFriend.GetItemData(nIndex);
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
//int CFSPageResult::GetResultCount()
//{
//	int iC=0;
//	int i;
//	if(m_resultType==RT_XTREAM)
//	{
//		for(i=0;i<m_vecXtreamPages.size();i++)
//		{
//			iC+=m_vecXtreamPages[i]->Size();
//		}
//		
//	}
//	else//Ⱥ��Ϣ
//	{	
//		for(i=0;i<m_vecClusterPages.size();i++)
//		{
//			iC+=m_vecClusterPages[i]->Size();
//		}
//	}
//	return iC;
//}
//
//void CFSPageResult::OnStnClickedStaticUserinfo()
//{
//	if ( m_resultType==RT_XTREAM )
//	{
//		// �鿴������Ϣ
//		FRIEND_INFO *pFriend = GetSelUser();
//
//        if ( pFriend )
//		{
//			XT_USERINFO_GET_REQ req;
//			req.fid = pFriend->GetId();
//			//req.uid = g_ImUserInfo.GetId();
//			g_pProtocol->SendReq(req);
//
//			CFriendInfoDlg::Show(pFriend);
//		}
//		else
//		{
//			CMessageBox::Prompt("����ѡ�����");
//		}
//	}
//	else////�鿴Ⱥ��Ϣ
//	{
//		XT_CLUSTER_INFO *pCluster= GetSelCluster();
//		if ( pCluster )
//		{
//			CClusterInfoDlg* pWnd=CClusterInfoDlg::NewDlg(pCluster);
//			if(pWnd)
//			{
//				// �����������Ⱥ��Ϣ
//				XT_GET_CINFO_REQ req;
//				req.nFamilyID   = pCluster->nFamilyID;
//				req.nFamilyProp = PERM_FAMILY;
//				g_pProtocol->SendReq(req);
//				// ��ʾȺ��Ϣ
//				pWnd->ShowWindow(SW_SHOW);
//				pWnd->CenterWindow(GetDesktopWindow());
//			}
//		}
//		else
//		{
//			CMessageBox::Prompt("����ѡ�����ѯ��Ⱥ");
//		}
//
//	}
//}
//
//void CFSPageResult::OnStnClickedStaticPrev()
//{
//	int count = 0;
//	
//	if ( m_resultType==RT_XTREAM )
//	{
//		count = m_vecXtreamPages.size();
//		for ( int i=0; i<count;  i++ )
//		{
//			if ( m_pCurXtreamPage==m_vecXtreamPages[i] )
//			{
//				if ( i>0 )
//				{
//					m_pCurXtreamPage = m_vecXtreamPages[i-1];
//					ShowResult(m_pCurXtreamPage);
//					m_btnNext.EnableWindow(TRUE);
//
//					if ( i==1 )
//					{
//						m_btnPre.EnableWindow(FALSE);
//					}
//
//					CString str;
//					str.Format("  ��%dҳ",i);
//					SetDlgItemText(IDC_STATIC_PAGE,str);
//				}
//				break;
//			}
//		}
//	}
//	else
//	{
//		count = m_vecClusterPages.size();
//		for ( int i=0; i<count;  i++ )
//		{
//			if ( m_pCurClusterPage==m_vecClusterPages[i] )
//			{
//				if ( i>0 )
//				{
//					m_pCurClusterPage = m_vecClusterPages[i-1];
//					ShowResult(m_pCurClusterPage);
//					m_btnNext.EnableWindow(TRUE);
//
//					if ( i==1 )
//					{
//						m_btnPre.EnableWindow(FALSE);
//					}
//				}
//
//				CString str;
//				str.Format("  ��%dҳ",i);
//				SetDlgItemText(IDC_STATIC_PAGE,str);
//
//				break;
//			}
//		}
//	}
//}
//
//void CFSPageResult::OnStnClickedStaticNext()
//{
//	int count=0;
//	CString str;
//
//	if ( m_resultType==RT_XTREAM )
//	{
//		count = m_vecXtreamPages.size();
//		for ( int i=0; i<count;  i++ )
//		{
//			if ( m_pCurXtreamPage==m_vecXtreamPages[i] )
//			{
//				if ( i<(count-1) )
//				{
//					m_pCurXtreamPage = m_vecXtreamPages[i+1];
//					ShowResult(m_pCurXtreamPage);
//					m_btnPre.EnableWindow(TRUE);
//										
//					if ( i==(count-2) && m_pCurXtreamPage->next_pos==0xFFFF )
//					{
//						m_btnNext.EnableWindow(FALSE);
//					}
//				}
//				else//i==count-1
//				{
//					if ( m_pCurXtreamPage->next_pos!=0xFFFF )
//					{
//						g_pProtocol->SendReq(m_xtreamReq);
//						SetLastReqSet(g_pProtocol->GetCurSendSequnceId()-1);
//					}
//				}
//
//				str.Format("  ��%dҳ",i+2);
//				SetDlgItemText(IDC_STATIC_PAGE,str);
//
//				break;
//			}
//		}
//	}
//	else
//	{
//		count = m_vecClusterPages.size();
//		for ( int i=0; i<count;  i++ )
//		{
//			if ( m_pCurClusterPage==m_vecClusterPages[i] )
//			{
//				if ( i<(count-1) )
//				{
//					m_pCurClusterPage = m_vecClusterPages[i+1];
//					ShowResult(m_pCurClusterPage);
//					m_btnPre.EnableWindow(TRUE);
//
//					if ( i==(count-2) && m_pCurClusterPage->next_pos==0xFFFF )
//					{
//						m_btnNext.EnableWindow(FALSE);
//					}
//				}
//				else//i==count-1
//				{
//					if ( m_pCurClusterPage->next_pos!=0xFFFF )
//					{
//						m_clusterReq.start_pos = m_pCurClusterPage->next_pos;
//						g_pProtocol->SendReq(m_clusterReq);
//						SetLastReqSet(g_pProtocol->GetCurSendSequnceId()-1);
//					}
//				}
//
//				str.Format("  ��%dҳ",i+2);
//				SetDlgItemText(IDC_STATIC_PAGE,str);
//
//				break;
//			}
//		}
//	}
//}
//
//BOOL CFSPageResult::PreTranslateMessage(MSG* pMsg)
//{
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		switch (pMsg->wParam)
//		{
//		case VK_ESCAPE:
//		case 0x0D:
//			return TRUE;
//		default:
//			break;
//		}
//	}
//
//	return CDialog::PreTranslateMessage(pMsg);
//}
//
//void CFSPageResult::OnPaint()
//{
//	CPaintDC dc(this); 
//
//	CRect rc;
//	if ( m_wndListCluster.IsWindowVisible() )
//	{
//		m_wndListCluster.GetWindowRect(rc);
//	}
//	else
//	{
//		m_wndListFriend.GetWindowRect(rc);
//	}
//
//	ScreenToClient(&rc);
//	rc.InflateRect(1,1);
//	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
//
//}
//
//BOOL CFSPageResult::OnEraseBkgnd(CDC* pDC)
//{
//	CRect rc;
//	GetClientRect(rc);
//	pDC->FillSolidRect(&rc,0xFFFFFF);
//	return FALSE;
//}
//
//void CFSPageResult::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CDialog::OnShowWindow(bShow, nStatus);
//
//	static CString str="";
//
//	CSAFrame *pFrame=(CSAFrame *)GetParent();
//
//	if ( bShow )
//	{
//		str = pFrame->GetWindowText();
//		pFrame->SetWindowText("�����Ƿ��������û�");
//		pFrame->Invalidate(this);
//	}
//	else
//	{
//		pFrame->SetWindowText(str);
//		pFrame->Invalidate(this);
//	}
//}
//
//void CFSPageResult::PreFind()
//{
//	m_btnNext.EnableWindow(FALSE);
//	m_btnPre.EnableWindow(FALSE);
//	SetDlgItemText(IDC_STATIC_PAGE,"  ��1ҳ");
//}
//
//
//
//void CFSPageResult::OnLvnItemchangedListCluster(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}
