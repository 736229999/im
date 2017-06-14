// NearCommCustDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NearCommCustDlg.h"
#include "afxdialogex.h"
#include "FlatDateTimeCtrl.h"
#include "resource.h"
#include "ClientProtocol.h"
#include "..\im\impublic.h"
#include "..\im\P2PSendMsgDlg.h"
#include "..\im\DuiP2PSendMsgDlg.h"
#include "..\im\MsgDlg.h"
#include "..\..\..\Common\Common.h"
#include "SpecifySendDlg.h"
#include "..\im\xtpacket.h"
#include "..\im\MessageHandleWnd.h"
using namespace std;

#define LINE_COUNT 12
extern USER_INFO     g_ImUserInfo;
extern /*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * ShowMessageDlg(FRIEND_INFO *pFriend, StruExtraInfo struExtInfo = StruExtraInfo(), BOOL bIsShow = TRUE);
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
// CNearCommCustDlg dialog
std::tr1::shared_ptr<CNearCommCustDlg> CNearCommCustDlg::pInstance = NULL;
std::tr1::shared_ptr<CNearCommCustDlg> CNearCommCustDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		BringWindowToTop(pInstance->GetHWND());
	    ::ShowWindow(pInstance->GetHWND(),SW_NORMAL);
	}
	else
	{
		pInstance = tr1::shared_ptr<CNearCommCustDlg>(new CNearCommCustDlg());
		pInstance->Create(GetDesktopWindow(), _T("�����ϵ�ͻ�"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

void CNearCommCustDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close();
	}
}

void CNearCommCustDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();

	SetIcon(IDR_MAINFRAME);
	m_bSearch = FALSE;
	m_nCustCount  =  0;		// �ܿͻ���			
	m_nLineChosed = -1;     // ѡ�е���

	//////////��ȡ�ؼ�����
	m_pBtnMin    = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnmin"));
	m_pBtnClose  = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));

	m_pBtnSearch     = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnSearch"));
	m_pBtnStartComm  = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnStartComm"));
	m_pBtnViewRecord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnViewRecord"));

	////��ʼʱ��ͽ���ʱ��
	m_pDateTimeBegin = static_cast<DuiLib::CDateTimeUI*>(m_PaintManager.FindControl("DateTimeBegin"));
	m_pDateTimeEnd   = static_cast<DuiLib::CDateTimeUI*>(m_PaintManager.FindControl("DateTimeEnd"));
		
	m_pDateTimeBegin->SetBkColor(0xFFFFFFFF);
	m_pDateTimeEnd->SetBkColor(0xFFFFFFFF);

	m_pBtnDealTime = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("dealTime"));
	m_pBtnDealTime->SetToolTip("˫���ɰ�ʱ������");

	////���ز�ѯ�б�//////////
	m_listOutput = static_cast<CResultListPeopleUI*>(m_PaintManager.FindControl("result"));

    m_pBtnFirst = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("firstbtn"));
    m_pBtnPre = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("prevbtn"));
	m_pBtnNext = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("nextbtn"));
	m_pBtnLast = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("lastbtn"));

	m_pEditPageNo = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("pageNoEdit"));
	m_pLabelPageAll= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("LabelPageNo"));

	m_pEditPageNo->SetReadOnly(TRUE);
	UpdateBk();

	////////////////////////////////////////////////////////////////////////////////////////////

	g_pProtocol->RegistCmd(CMD_GET_NEAR_COMM_CUST_ACK, GetHWND());
	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,GetHWND());

	OnBnClickedSearch();
	////OnBnClickedSearch();////Ĭ����ʾ��������
}


DuiLib::CControlUI* CNearCommCustDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("ResultListPeopleUI")) == 0)
	{
		return new CResultListPeopleUI(m_PaintManager);
	}

	return NULL;
}

LRESULT CNearCommCustDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
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

// ��Ϣ
void CNearCommCustDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnMin)
		{
			SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);////��С��
		}
		else if(msg.pSender == m_pBtnClose)
		{
			Close(1);////�ر�
		}
		else if(msg.pSender == m_pBtnDealTime)////����  
		{
		   OnChangeDirect();
		}
		else if(msg.pSender == m_pBtnSearch) ////����
		{
			OnBnClickedSearch();
		}
		else if(msg.pSender == m_pBtnStartComm)////�������
		{
			OnBnClickedStartComm();
		}
		else if(msg.pSender == m_pBtnViewRecord)////����Ϣ��¼
		{
			OnBnClickedViewRecord();
		}
		else if(msg.pSender == m_pBtnFirst)////��һҳ
		{
			OnBnClickedFirstPage();
		}
		else if(msg.pSender == m_pBtnPre)  ////��һҳ
		{
			OnBnClickedPrevPage();
		}
		else if(msg.pSender == m_pBtnNext) ////��һҳ
		{
			OnBnClickedNextPage();
		}
		else if(msg.pSender == m_pBtnLast) ////���һҳ
		{
			OnBnClickedLastPage();
		}
		else
		{
			for(int i = 0; i < m_listOutput->GetCount(); ++i)////ѡ��ĳһ�еĵ�һ��
			{
				DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_listOutput->GetItemAt(i));
				DuiLib::CButtonUI *pBtnUserAccount= static_cast<DuiLib::CButtonUI*>(pItem->FindSubControl("userAccount"));

				if(pBtnUserAccount == msg.pSender)
				{
					m_listOutput->SelectItem(i);
		            m_nLineChosed = i; ////��ǰѡ����
					break;
				}
			}
		}
	}
	else
	{
		if (m_listOutput->GetCount()>0)
		{
			SetBtnShow(TRUE);
		}
		else
		{
			SetBtnShow(FALSE);
		}
	}
}

void CNearCommCustDlg::SetBtnShow(BOOL bShow)
{
	m_pBtnFirst->SetEnabled(bShow);
	m_pBtnPre->SetEnabled(bShow);
	m_pBtnNext->SetEnabled(bShow);
	m_pBtnLast->SetEnabled(bShow);
}

void CNearCommCustDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(hWnd);
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}

void CNearCommCustDlg::UpdateBk()
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

BEGIN_XT_MESSAGE_MAP(CNearCommCustDlg)
	// ��ȡ�����ϵ�ͻ�
	ON_XT_COMMAND(CMD_GET_NEAR_COMM_CUST_ACK, OnGetNearCommCust)	
	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK, OnSearchUser)
END_XT_MESSAGE_MAP()


// ��ѯ�����ϵ�ͻ�
void CNearCommCustDlg::OnBnClickedSearch()
{
	m_pBtnSearch->SetEnabled(FALSE);
	ReInitData();
	GetCustCountFromServer();
}

// ��ȡ�����ϵ�ͻ�
void CNearCommCustDlg::OnGetNearCommCust(const char *data, WORD wRet)
{
	LPXT_GET_NEAR_COMM_CUST_ACK msg = (LPXT_GET_NEAR_COMM_CUST_ACK)data;
	m_pBtnSearch->SetEnabled(TRUE);
	if (msg->ret != 0)
	{
		CMessageBox::Model(GetHWND(), "��ȡ�����ϵ�ͻ���Ϣʧ��", IM_CAPTION,  MB_OK | MB_ICONWARNING);
		return;
	}

	if (msg->bMask == XT_GET_NEAR_COMM_CUST_ACK::MK_ALL_CUST_COUNT)
	{	// ��ȡ���ǿͻ���Ŀ
		m_nCustCount = msg->actualCount;
		// ������ҳ��
		UpdatePageCount();
		// ����ҳ��
		if(m_nPageCount == 0)
		{
			SetPageNo(0);
			CMessageBox::Model(GetHWND(), "δ�����������ϵ�ͻ�", IM_CAPTION,  MB_OK);
		}
		else
		{
			SetPageNo(1);
		}
		// ��ȡ�ͻ���Ϣ
		GetPageDataFromServer();
	}

	if (msg->bMask == XT_GET_NEAR_COMM_CUST_ACK::MK_CUR_CUST_COUNT)
	{
		vector<XT_COMMINFO_GET> vecCommInfo;
		for (int i = 0; i < msg->actualCount; ++i)
		{
			XT_COMMINFO_GET &comminfo = msg->CommInfo[i];
			vecCommInfo.push_back(comminfo);
		}

		int nPageNo = GetPageNo();

		if (m_bDirect)
		{
			m_mapArrCommInfo1[nPageNo] = vecCommInfo;
		}
		else
		{
			m_mapArrCommInfo0[nPageNo] = vecCommInfo;
		}

		OutputCustInfo(vecCommInfo);
	}
}

// ǰһҳ
void CNearCommCustDlg::OnBnClickedPrevPage()
{
	// ��ȡҳ��
	CString strPageNo;
	strPageNo = m_pEditPageNo->GetText();
	int nPageNo = atoi(strPageNo);
	if (nPageNo == 1)
	{	// �Ѿ��ǵ�һҳ��
		return;
	}

	// ����ҳ��
	SetPageNo(--nPageNo);

	// �����б�
	ShowCustInfo();
}

// ��һҳ
void CNearCommCustDlg::OnBnClickedNextPage()
{
	// ��ȡҳ��
	CString strPageNo;
	strPageNo = m_pEditPageNo->GetText();
	int nPageNo = atoi(strPageNo);
	if (nPageNo == m_nPageCount)
	{	// �Ѿ������һҳ��
		return;
	}

	// ����ҳ��
	SetPageNo(++nPageNo);

	// �����б�
	ShowCustInfo();
}

// ��һҳ
void CNearCommCustDlg::OnBnClickedFirstPage()
{
	// ��ȡҳ��
	CString strPageNo;
	strPageNo = m_pEditPageNo->GetText();
	int nPageNo = atoi(strPageNo);
	if (nPageNo == 1)
	{	// �Ѿ��ǵ�һҳ��
		return;
	}

	// ����ҳ��
	SetPageNo(1);

	// �����б�
	ShowCustInfo();
}

// ���һҳ
void CNearCommCustDlg::OnBnClickedLastPage()
{
	// ��ȡҳ��
	CString strPageNo;
	strPageNo = m_pEditPageNo->GetText();
	int nPageNo = atoi(strPageNo);
	if (nPageNo == m_nPageCount)
	{	// �Ѿ������һҳ��
		return;
	}

	// ����ҳ��
	SetPageNo(m_nPageCount);

	// �����б�
	ShowCustInfo();
}

// ��ȡ�б���ѡ��ĺ���
FRIEND_INFO* CNearCommCustDlg::GetSelFriend()
{
	int iPos = m_listOutput->GetCurSel();
	DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_listOutput->GetItemAt(iPos));
	if(pItem == NULL)
	{
		return NULL;
	}
	DuiLib::CButtonUI *pBtnName = static_cast<DuiLib::CButtonUI*>(pItem->FindSubControl("userAccount"));
	CString strUserName = pBtnName->GetText();
	if (strUserName != "")
	{
		// ��ȡ�û���Ϣ
		FRIEND_INFO *fi = FindFriend(strUserName);
		return fi;	
	}

	return NULL;
}

// ����
void CNearCommCustDlg::OnBnClickedStartComm()
{
	// ��ȡ�û���Ϣ
	FRIEND_INFO *fi = GetSelFriend();
	if (fi == NULL)
	{
		int iPos = m_listOutput->GetCurSel();
		DuiLib::CListContainerElementUI *pItem = static_cast<DuiLib::CListContainerElementUI*>(m_listOutput->GetItemAt(iPos));
		if(pItem == NULL)
		{
			return;
		}
		DuiLib::CButtonUI *pBtnName = static_cast<DuiLib::CButtonUI*>(pItem->FindSubControl("userAccount"));
		CString strUserName = pBtnName->GetText();
		if (strUserName != "")
		{
			m_bSearch = TRUE;
			XT_XTREAM_SEARCH_REQ req;
			req.search_type = XTREAM_SEARCH_PRECISION;
			strcpy(req.user_name, strUserName);

			g_pProtocol->SendReq(req);
		}
		return;
	}

	// �������촰��
	ShowMessageDlg(fi);
}

void CNearCommCustDlg::OnSearchUser(const char *data, int nRetCode)
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
		// ����������
		nTotal = total;

		for ( i=0; i<count; i++ )
		{
			FRIEND_INFO friendinfo;
			friendinfo.SetGuid("");
			friendinfo.SetPassword("");

			uint32 id = 0;
			rp  >>id;
			friendinfo.SetId(id);
			rp	>>friendinfo.sex
				>>friendinfo.status
				>>friendinfo.rule;
			char username[MAX_USERNAME_LEN+1] = {0};	
			rp	>>username;
			friendinfo.SetUserName(username);
			rp	>>friendinfo.nickname;

			if ( (friendinfo.GetId() != g_ImUserInfo.GetId()) )
			{
				FRIEND_INFO *pFriend = NULL;
				/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg;
				pFriend = ::FindFriend(friendinfo.GetId());
				if( pFriend)
				{
					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
				}
				else
				{
					pFriend = new FRIEND_INFO;
					*pFriend = friendinfo;
					pFriend->group_index = 1;//İ����
					CMainFrame::GetInstance()->AddFriend(pFriend, false);
					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
				}	
				if ( pDlg->GetHWND()==NULL )
				{
					pDlg->NotifyServer_OpenedMsgDlg();
				}

				pDlg->ShowWindow(SW_SHOW);

				if (::IsMinimized(pDlg->GetHWND()) )
				{
					pDlg->ShowWindow(SW_RESTORE);
				}
				::BringWindowToTop(pDlg->GetHWND());
				////::SendMessage(GetParent()->GetSafeHwnd(), WM_SPEC_SEND_DLG_MSG, IDOK, 0);
			}
			else
			{	// ���ܺ��Լ�����
				CMessageBox::Model(GetHWND(),"���ܺ��Լ����죡",0,IDOK);
			}
			break;
		}

	}
	else
	{	// ������ϵ��ʧ��
		CMessageBox::Model(GetHWND(),"δ���ҵ���ϵ�ˣ�",0,IDOK);
	}
}

// ��ȡPageҳ��
int CNearCommCustDlg::GetPageNo()
{
	CString strPageNo;
	//GetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
	strPageNo = m_pEditPageNo->GetText();
	int nPageNo = atoi(strPageNo);
	return nPageNo;
}

// ����Pageҳ
void CNearCommCustDlg::SetPageNo(int nPageNo)
{
	CString strPageNo;
	strPageNo.Format("%d", nPageNo);
	//SetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
	m_pEditPageNo->SetText(strPageNo);
}

// ������ʾ�ͻ���Ϣ
void CNearCommCustDlg::ShowCustInfo()
{
	int nPageNo = GetPageNo();

	// �ȴӻ����ȡ�ͻ�����
	vector<XT_COMMINFO_GET> vecCustInfo;
	if (GetPageData(nPageNo, vecCustInfo))
	{
		OutputCustInfo(vecCustInfo);
	}
	else
	{	// �ӻ����ȡʧ�ܣ��Ĵӷ�������ȡ
		GetPageDataFromServer();
	}
}

// �û��ı�������ʽ
void CNearCommCustDlg::OnChangeDirect()
{
	SetPageNo(1);
	m_bDirect = !m_bDirect;

	ShowCustInfo();
}

// ���³�ʼ�����ݣ�����ʼ��ֹʱ��ı�ʱ�����б���Ŀͻ����ݶ�Ҫ��գ����´ӷ�������ȡ
void CNearCommCustDlg::ReInitData()
{
	m_mapArrCommInfo0.clear();
	m_mapArrCommInfo1.clear();
	ClearCustInfo();
	SetPageNo(1);
	m_bDirect = FALSE;
}

// �ӵ�ǰ�����л�ȡ�ͻ�����
BOOL CNearCommCustDlg::GetPageData(int nPageNo, vector<XT_COMMINFO_GET> &vecCustInfo)
{
	map<int, vector<XT_COMMINFO_GET>> *pCommInfo = (m_bDirect ? &m_mapArrCommInfo1 : &m_mapArrCommInfo0);

	map<int, vector<XT_COMMINFO_GET>>::iterator it = pCommInfo->find(nPageNo);
	if (pCommInfo->find(nPageNo) == pCommInfo->end())
	{	// ���ػ��޻���
		return FALSE;
	}

	vecCustInfo = it->second;
	return TRUE;
}

// �������̨������Ϣ����ȡ�ͻ�����Ŀ
void CNearCommCustDlg::GetCustCountFromServer()
{
	////CString strBgnTime = GetTime(m_dataBgn, m_timeBgn);
	////CString strEndTime = GetTime(m_dataEnd, m_timeEnd);

	SYSTEMTIME  cBgnTime = m_pDateTimeBegin->GetTime();
	cBgnTime.wHour = 0;
	cBgnTime.wSecond = 0;
	cBgnTime.wMilliseconds = 0;
	CString strBgnTime = _T("");
	strBgnTime.Format("%d-%d-%d %d:%d:%d",cBgnTime.wYear,cBgnTime.wMonth,cBgnTime.wDay,0,0,0);

	SYSTEMTIME  cEndTime = m_pDateTimeEnd->GetTime();
	cEndTime.wHour = 23;
	cEndTime.wSecond = 59;
	cEndTime.wMilliseconds = 59;
	CString strEndTime = _T("");
	strEndTime.Format("%d-%d-%d %d:%d:%d",cEndTime.wYear,cEndTime.wMonth,cEndTime.wDay,23,59,59);

	// �ж�ʱ��
	CTime cBgn(cBgnTime);
	CTime cEnd(cEndTime);
	if(cBgn > cEnd)
	{
		CMessageBox::Model(NULL, _T("����ʱ�����ڿ�ʼʱ��"), NULL, 0);
		m_pBtnSearch->SetEnabled(TRUE);
		return;
	}

	// ȥ��̨��ѯ
	XT_GET_NEAR_COMM_CUST_REQ req;
	strcpy(req.szBgnTime, strBgnTime);
	strcpy(req.szEndTime, strEndTime);
	req.iMaxCustCount = 1000;
	req.uiBusinessID = g_ImUserInfo.GetId();
	req.iStartPos = 999;
	req.iNeedCustCount = LINE_COUNT;
	req.bDirect = m_bDirect;
	g_pProtocol->SendReq(req);
}

// ����̨������Ϣ������ǰҳ����
void CNearCommCustDlg::GetPageDataFromServer()
{
	int nPageNo = GetPageNo();
	if(nPageNo == 0)
		return;
	//CString strBgnTime = GetTime(m_dataBgn, m_timeBgn);
	//CString strEndTime = GetTime(m_dataEnd, m_timeEnd);

	SYSTEMTIME  cBgnTime = m_pDateTimeBegin->GetTime();
	CString strBgnTime = _T("");
	strBgnTime.Format("%d-%d-%d %d:%d:%d",cBgnTime.wYear,cBgnTime.wMonth,cBgnTime.wDay,0,0,0);

	SYSTEMTIME  cEndTime = m_pDateTimeEnd->GetTime();
	CString strEndTime = _T("");
	strEndTime.Format("%d-%d-%d %d:%d:%d",cEndTime.wYear,cEndTime.wMonth,cEndTime.wDay,23,59,59);


	// ȥ��̨��ѯ
	XT_GET_NEAR_COMM_CUST_REQ req;
	strcpy(req.szBgnTime, strBgnTime);
	strcpy(req.szEndTime, strEndTime);
	req.iMaxCustCount = 1000;
	req.uiBusinessID = g_ImUserInfo.GetId();
	req.iStartPos = (nPageNo - 1)*LINE_COUNT;
	req.iNeedCustCount = LINE_COUNT;
	req.bDirect = m_bDirect;
	g_pProtocol->SendReq(req);
}

// ����ͻ���Ϣ
void CNearCommCustDlg::OutputCustInfo(const vector<XT_COMMINFO_GET> &vecCustInfo)
{
	ClearCustInfo();
	for (size_t i = 0; i < vecCustInfo.size(); ++i)
	{
		const XT_COMMINFO_GET comminfo = vecCustInfo[i];
		m_listOutput->AddItem(comminfo.szCommAccount,  comminfo.szCommTime, comminfo.szTransInfo, comminfo.szRemarks);
	}
	m_pBtnSearch->SetEnabled(TRUE);
}

// ��ս�����ʾ�Ŀͻ���Ϣ
void CNearCommCustDlg::ClearCustInfo()
{
	m_listOutput->RemoveAll();
}

// ������ҳ��
void CNearCommCustDlg::UpdatePageCount()
{
	m_nPageCount = m_nCustCount / LINE_COUNT;
	if (m_nCustCount % LINE_COUNT > 0)
	{
		m_nPageCount++;
	}
	CString strPageCount;
	strPageCount.Format("/%dҳ", m_nPageCount);
	m_pLabelPageAll->SetText(strPageCount);
}

void CNearCommCustDlg::OnLvnColumnclickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iSubItem == 1)
	{	// �ı�����ʽ
		OnChangeDirect();
	}
	*pResult = 0;
}

// �鿴�����¼
void CNearCommCustDlg::OnBnClickedViewRecord()
{
	// ��ȡ�û���Ϣ
	FRIEND_INFO *fi = GetSelFriend();
	if (fi == NULL)
	{
		CMessageBox::Model(NULL, "�����¼�����ڣ�", NULL, MB_OK);
		return;
	}

	std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
	pDlg->SetDefaultFriend(fi->GetId());
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
CResultListPeopleUI::CResultListPeopleUI(DuiLib::CPaintManagerUI& paint_manager)
	: paint_manager_(paint_manager)
{
	////m_enType = FS_XTREAM;
}

CResultListPeopleUI::~CResultListPeopleUI()
{
}

void CResultListPeopleUI::AddItem(LPCTSTR szName, LPCTSTR szEndTime, LPCTSTR szDealInfo, LPCTSTR szBackInfo)////�˺ţ���ϵʱ�䣬������Ϣ����ע
{
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(dlgBuilder.Create(_T("NearCommCust_ListItem.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
	{
		return;
	}
	DuiLib::CButtonUI *pBtnName = static_cast<DuiLib::CButtonUI*>(pListElement->FindSubControl("userAccount"));
	DuiLib::CLabelUI *pLabelLastTime = static_cast<DuiLib::CLabelUI*>(pListElement->FindSubControl("lastTime"));
	DuiLib::CLabelUI *pLabelDealInfo= static_cast<DuiLib::CLabelUI*>(pListElement->FindSubControl("dealInfo"));
	DuiLib::CLabelUI *pLabelBackInfo= static_cast<DuiLib::CLabelUI*>(pListElement->FindSubControl("backInfo"));

	if(pBtnName == NULL || pLabelLastTime == NULL)
	{
		return;
	}
	pBtnName->SetText(szName);
	pLabelLastTime->SetText(szEndTime);
	pLabelDealInfo->SetText(szDealInfo);
	pLabelBackInfo->SetText(szBackInfo);

	DuiLib::CListUI::Add(pListElement);
}

void CResultListPeopleUI::RemoveAll()
{
	for(int i = 0; i < this->GetCount(); ++i)
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO*)this->GetItemAt(i)->GetTag();
		delete pFriend;
		pFriend = NULL;
	}

	DuiLib::CListUI::RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNAMIC(CNearCommCustDlg, CXSkinDlg)
//IMPLEMENT_HMG_SCROLLBAR(CNearCommCustDlg)
//
//CNearCommCustDlg::CNearCommCustDlg(CWnd* pParent /*=NULL*/)
//          : CXSkinDlg(IDD_DLG_NEAR_COMM_CUST, pParent)
//{
//	//////�����ɫ����///////////////
//	m_colorBackground = RGB(255, 255, 255);
//	m_colorBorder = CHAT_BKG_COLOR;
//	DeleteObject(m_hBkGround);
//	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
//
//	DeleteObject(m_hBkSelfGround);
//	m_hBkSelfGround = ::CreateSolidBrush(RGB(222, 222, 222));	
//	
//	SetDlgStyle(CXSkinDlg::MINCLOSE);
//	Create(IDD_DLG_NEAR_COMM_CUST, pParent);
//	m_nPageCount = 1;
//
//	m_bDirect = FALSE;
//	m_nPageIndex = 1;
//}
//
//CNearCommCustDlg::~CNearCommCustDlg()
//{
//	if(g_pProtocol != NULL)
//	{
//		g_pProtocol->UnRegistCmd(m_hWnd);
//	}
//	
//	if(::IsWindow(GetSafeHwnd()))
//	{
//		DestroyWindow();
//	}
//	DeleteObject(m_hBkSelfGround);
//}
//
//void CNearCommCustDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CXSkinDlg::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_DATE_BGN, m_dataBgn);
//	DDX_Control(pDX, IDC_TIME_BGN, m_timeBgn);
//	DDX_Control(pDX, IDC_DATE_END, m_dataEnd);
//	DDX_Control(pDX, IDC_TIME_END, m_timeEnd);
//	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
//	DDX_Control(pDX, IDC_BTN_START_COMM, m_btnStartComm);
//	DDX_Control(pDX, IDC_BTN_VIEW_CHAT_LOG, m_btnViewChatLog);
//	DDX_Control(pDX, IDC_EDIT_PAGE_NO, m_editPageNo);
//	DDX_Control(pDX, IDC_LIST_OUTPUT, m_listOutput);
//	DDX_Control(pDX, IDC_BTN_FIRST_PAGE, m_btnFirstPage);
//	DDX_Control(pDX, IDC_BTN_PREV_PAGE, m_btnPrevPage);
//	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
//	DDX_Control(pDX, IDC_BTN_LAST_PAGE, m_btnLastPage);
//}
//
//BOOL CNearCommCustDlg::OnInitDialog()
//{
//	CXSkinDlg::OnInitDialog();
//
//	AttachHMGScrollbar(m_listOutput.m_hWnd);
//
//	// ��ʼ��ͼ�갴ť
//	m_btnFirstPage.SetPng(IDB_PNG_FIRST_HOVER, IDB_PNG_FIRST_NORMAL, IDB_PNG_FIRST_PRESSED, IDB_PNG_FIRST_DISABLE);
//	m_btnFirstPage.SetToolTip(_T("��һҳ"));
//	m_btnPrevPage.SetPng(IDB_PNG_PREV_HOVER, IDB_PNG_PREV_NORMAL, IDB_PNG_PREV_PRESSED, IDB_PNG_PREV_DISABLE);
//	m_btnPrevPage.SetToolTip(_T("��һҳ"));
//	m_btnNextPage.SetPng(IDB_PNG_NEXT_HOVER, IDB_PNG_NEXT_NORMAL, IDB_PNG_NEXT_PRESSED, IDB_PNG_NEXT_DISABLE);
//	m_btnNextPage.SetToolTip(_T("��һҳ"));
//	m_btnLastPage.SetPng(IDB_PNG_LAST_HOVER, IDB_PNG_LAST_NORMAL, IDB_PNG_LAST_PRESSED, IDB_PNG_LAST_DISABLE);
//	m_btnLastPage.SetToolTip(_T("���һҳ"));
//
//	// ���ÿؼ�����
//	CWnd* pChild = GetNextWindow(GW_CHILD);
//	while (pChild != NULL)
//	{
//		pChild->SetFont(&g_font12);
//		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
//	}
//
//	// ���ñ༭��߿���ɫ
//	m_editPageNo.SetBorderColor(RGB(145,145,145));
//
//	// ע���̨��Ϣ
//	g_pProtocol->RegistCmd(CMD_GET_NEAR_COMM_CUST_ACK, m_hWnd);
//	
//	// ��ʼ���б�ؼ���4��11��
//	m_listOutput.SetExtendedStyle(LVS_EX_GRIDLINES | m_listOutput.GetExtendedStyle());		// ��ʾ������
//	CRect rcOutput;
//	m_listOutput.GetWindowRect(&rcOutput);
//	int nWidth = rcOutput.Width() / 4 - 1;
//	m_listOutput.InsertColumn(0, "�˺�", LVCFMT_CENTER, nWidth);
//	m_listOutput.InsertColumn(1, "�����ϵʱ��", LVCFMT_CENTER, nWidth);
//	m_listOutput.InsertColumn(2, "������Ϣ", LVCFMT_CENTER, nWidth);
//	m_listOutput.InsertColumn(3, "��ע", LVCFMT_CENTER, nWidth);
//	m_listOutput.SetColumnStatus(1, xcsArrowDown);		// ��������ť
//
//	// ���10��
//	for (int i = 0; i < LINE_COUNT; i++)
//	{
//		m_listOutput.InsertItem(i, "");
//	}
//	
//	// ��ʼ��ʱ��
//	CTime tmCur = CTime::GetCurrentTime();
//	CTime tmBgn(tmCur.GetYear(), 1, 1, 0, 0, 0);
//	m_dataBgn.SetTime(&tmBgn);
//	m_timeBgn.SetTime(&tmBgn);
//	m_dataEnd.SetTime(&tmCur);
//	m_timeEnd.SetTime(&tmCur);
//
//	// ���ò�ѯ��ť��Ӧ���������б�
//	OnBnClickedSearch();
//
//	return TRUE;
//}
//
//BEGIN_MESSAGE_MAP(CNearCommCustDlg, CXSkinDlg)
//	ON_WM_PAINT()
//	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedSearch)
//	ON_BN_CLICKED(IDC_BTN_FIRST_PAGE, OnBnClickedFirstPage)
//	ON_BN_CLICKED(IDC_BTN_LAST_PAGE, OnBnClickedLastPage)
//	ON_BN_CLICKED(IDC_BTN_PREV_PAGE, OnBnClickedPrevPage)
//	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, OnBnClickedNextPage)
//	ON_BN_CLICKED(IDC_BTN_START_COMM, OnBnClickedStartComm)
//	ON_BN_CLICKED(IDC_BTN_VIEW_CHAT_LOG, OnBnClickedViewRecord)
//	ON_XT_MESSAGE()
//	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_OUTPUT, &CNearCommCustDlg::OnLvnColumnclickListOutput)
//	SET_HMG_SCROLLBAR(IDC_LIST_OUTPUT)
//END_MESSAGE_MAP()
////
////BEGIN_XT_MESSAGE_MAP(CNearCommCustDlg)
////	// ��ȡ�����ϵ�ͻ�
////	ON_XT_COMMAND(CMD_GET_NEAR_COMM_CUST_ACK, OnGetNearCommCust)	
////END_XT_MESSAGE_MAP()
////
////
////// CNearCommCustDlg message handlers
////void CNearCommCustDlg::Show()
////{
////	m_staInstance = std::tr1::shared_ptr<CNearCommCustDlg>(new CNearCommCustDlg(GetDesktopWindow()));
////	m_staInstance->CenterWindow(GetDesktopWindow());
////	m_staInstance->SetWindowText("�����ϵ�ͻ�");
////	m_staInstance->ShowWindow(SW_SHOW);
////}
////
////void CNearCommCustDlg::PostNcDestroy()
////{
////	CXSkinDlg::PostNcDestroy();
////}
////
////void CNearCommCustDlg::OnPaint()
////{
////	CPaintDC dc(this);
////	CXSkinDlg::DrawCaptionBar();
////	CXSkinDlg::DrawBorder();
////
////	CRect rc;
////	GetClientRect(&rc);
////
////	// �б��·��İ�ť��
////	CRect rcBtn;
////	m_btnFirstPage.GetWindowRect(&rcBtn);
////	ScreenToClient(&rcBtn);
////	CRect rcBottomBar;
////	rcBottomBar.left = 20;
////	rcBottomBar.right = rc.right - 20;
////	rcBottomBar.top = rcBtn.top - rcBtn.Height()/3;
////	rcBottomBar.bottom = rcBtn.bottom + rcBtn.Height()/3;
////	dc.FillSolidRect(&rcBottomBar, RGB(222, 222, 222));
////
////	// �Ի���ײ��İ�ť��
////	m_btnStartComm.GetWindowRect(&rcBtn);
////	ScreenToClient(&rcBtn);
////	rcBottomBar.left = 1;
////	rcBottomBar.right = rc.right - 1;
////	rcBottomBar.top = rcBtn.top - rcBtn.Height() / 3;
////	rcBottomBar.bottom = rc.bottom - 1;
////	dc.FillSolidRect(&rcBottomBar, RGB(222, 222, 222));
////}
////
////// ��Ӧ�ؼ���ɫ�����¼���������Ʊ���ɫ����ԭ����ɫ������
////HBRUSH CNearCommCustDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////{
////	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
////
////	if (nCtlColor == CTLCOLOR_STATIC)
////	{
////		DWORD ID = GetWindowLong(pWnd->GetSafeHwnd(), GWL_ID);
////		if (ID == IDC_STATIC_PAGE_COUNT || ID == IDC_STATIC_CUR || ID == IDC_STATIC_EXPLAIN)
////		{
////			pDC->SetBkMode(TRANSPARENT);
////			hbr = m_hBkSelfGround;
////		}
////		else
////		{
////			pDC->SetBkMode(TRANSPARENT);
////			hbr = m_hBkGround;
////		}
////	}
////
////	////////////////////////////////////////////////////////////////////
////	return hbr;
////}
////
////// ��ѯ�����ϵ�ͻ�
////void CNearCommCustDlg::OnBnClickedSearch()
////{
////	ReInitData();
////	GetCustCountFromServer();
////}
////
////// ��ȡ�����ϵ�ͻ�
////void CNearCommCustDlg::OnGetNearCommCust(const char *data, WORD wRet)
////{
////	LPXT_GET_NEAR_COMM_CUST_ACK msg = (LPXT_GET_NEAR_COMM_CUST_ACK)data;
////	if (msg->ret != 0)
////	{
////		MessageBox("��ȡ�����ϵ�ͻ���Ϣʧ��");
////		return;
////	}
////	
////	if (msg->bMask == XT_GET_NEAR_COMM_CUST_ACK::MK_ALL_CUST_COUNT)
////	{	// ��ȡ���ǿͻ���Ŀ
////		m_nCustCount = msg->actualCount;
////		// ������ҳ��
////		UpdatePageCount();
////		// ����ҳ��
////		SetPageNo(1);
////		// ��ȡ�ͻ���Ϣ
////		GetPageDataFromServer();
////	}
////
////	if (msg->bMask == XT_GET_NEAR_COMM_CUST_ACK::MK_CUR_CUST_COUNT)
////	{
////		vector<XT_COMMINFO_GET> vecCommInfo;
////		for (int i = 0; i < msg->actualCount; ++i)
////		{
////			XT_COMMINFO_GET &comminfo = msg->CommInfo[i];
////			vecCommInfo.push_back(comminfo);
////		}
////
////		int nPageNo = GetPageNo();
////
////		if (m_bDirect)
////		{
////			m_mapArrCommInfo1[nPageNo] = vecCommInfo;
////		}
////		else
////		{
////			m_mapArrCommInfo0[nPageNo] = vecCommInfo;
////		}
////
////		OutputCustInfo(vecCommInfo);
////	}
////}
////
////// ǰһҳ
////void CNearCommCustDlg::OnBnClickedPrevPage()
////{
////	// ��ȡҳ��
////	CString strPageNo;
////	GetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
////	int nPageNo = atoi(strPageNo);
////	if (nPageNo == 1)
////	{	// �Ѿ��ǵ�һҳ��
////		return;
////	}
////
////	// ����ҳ��
////	SetPageNo(--nPageNo);
////	
////	// �����б�
////	ShowCustInfo();
////}
////
////// ��һҳ
////void CNearCommCustDlg::OnBnClickedNextPage()
////{
////	// ��ȡҳ��
////	CString strPageNo;
////	GetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
////	int nPageNo = atoi(strPageNo);
////	if (nPageNo == m_nPageCount)
////	{	// �Ѿ������һҳ��
////		return;
////	}
////
////	// ����ҳ��
////	SetPageNo(++nPageNo);
////
////	// �����б�
////	ShowCustInfo();
////}
////
////// ��һҳ
////void CNearCommCustDlg::OnBnClickedFirstPage()
////{
////	// ��ȡҳ��
////	CString strPageNo;
////	GetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
////	int nPageNo = atoi(strPageNo);
////	if (nPageNo == 1)
////	{	// �Ѿ��ǵ�һҳ��
////		return;
////	}
////
////	// ����ҳ��
////	SetPageNo(1);
////
////	// �����б�
////	ShowCustInfo();
////}
////
////// ���һҳ
////void CNearCommCustDlg::OnBnClickedLastPage()
////{
////	// ��ȡҳ��
////	CString strPageNo;
////	GetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
////	int nPageNo = atoi(strPageNo);
////	if (nPageNo == m_nPageCount)
////	{	// �Ѿ������һҳ��
////		return;
////	}
////
////	// ����ҳ��
////	SetPageNo(m_nPageCount);
////
////	// �����б�
////	ShowCustInfo();
////}
////
////// ��ȡ�б���ѡ��ĺ���
////FRIEND_INFO* CNearCommCustDlg::GetSelFriend()
////{
////	int nIndex = m_listOutput.GetCurSel();
////	if (nIndex == -1)
////	{
////		return NULL;
////	}
////	CString strUserName = m_listOutput.GetItemText(nIndex, 0);
////
////	// ��ȡ�û���Ϣ
////	FRIEND_INFO *fi = FindFriend(strUserName);
////	return fi;
////}
////
////// ����
////void CNearCommCustDlg::OnBnClickedStartComm()
////{
////	// ��ȡ�û���Ϣ
////	FRIEND_INFO *fi = GetSelFriend();
////	if (fi == NULL)
////	{
////		return;
////	}
////
////	// �������촰��
////	ShowMessageDlg(fi);
////}
////
////// ��ȡ�ؼ�ָʾʱ��
////CString CNearCommCustDlg::GetTime(const CFlatDateTimeCtrl &data, const CFlatDateTimeCtrl &time)
////{
////	// ��ȡʱ��
////	CTime da, ti;
////	data.GetTime(da);
////	time.GetTime(ti);
////
////	CString strTime;
////	strTime.Format("%d-%d-%d %d:%d:%d", da.GetYear(), da.GetMonth(),
////		da.GetDay(), ti.GetHour(), ti.GetMinute(), ti.GetSecond());
////
////	return strTime;
////}
////
////// ��ȡPageҳ��
////int CNearCommCustDlg::GetPageNo()
////{
////	CString strPageNo;
////	GetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
////	int nPageNo = atoi(strPageNo);
////	return nPageNo;
////}
////
////// ����Pageҳ
////void CNearCommCustDlg::SetPageNo(int nPageNo)
////{
////	CString strPageNo;
////	strPageNo.Format("%d", nPageNo);
////	SetDlgItemText(IDC_EDIT_PAGE_NO, strPageNo);
////}
////
////// ������ʾ�ͻ���Ϣ
////void CNearCommCustDlg::ShowCustInfo()
////{
////	int nPageNo = GetPageNo();
////
////	// �ȴӻ����ȡ�ͻ�����
////	vector<XT_COMMINFO_GET> vecCustInfo;
////	if (GetPageData(nPageNo, vecCustInfo))
////	{
////		OutputCustInfo(vecCustInfo);
////	}
////	else
////	{	// �ӻ����ȡʧ�ܣ��Ĵӷ�������ȡ
////		GetPageDataFromServer();
////	}
////}
////
////// �û��ı�������ʽ
////void CNearCommCustDlg::OnChangeDirect()
////{
////	SetPageNo(1);
////	m_bDirect = !m_bDirect;
////
////	ShowCustInfo();
////}
////
////// ���³�ʼ�����ݣ�����ʼ��ֹʱ��ı�ʱ�����б���Ŀͻ����ݶ�Ҫ��գ����´ӷ�������ȡ
////void CNearCommCustDlg::ReInitData()
////{
////	m_mapArrCommInfo0.clear();
////	m_mapArrCommInfo1.clear();
////	ClearCustInfo();
////	SetPageNo(1);
////	m_bDirect = FALSE;
////}
////
////// �ӵ�ǰ�����л�ȡ�ͻ�����
////BOOL CNearCommCustDlg::GetPageData(int nPageNo, vector<XT_COMMINFO_GET> &vecCustInfo)
////{
////	map<int, vector<XT_COMMINFO_GET>> *pCommInfo = (m_bDirect ? &m_mapArrCommInfo1 : &m_mapArrCommInfo0);
////
////	map<int, vector<XT_COMMINFO_GET>>::iterator it = pCommInfo->find(nPageNo);
////	if (pCommInfo->find(nPageNo) == pCommInfo->end())
////	{	// ���ػ��޻���
////		return FALSE;
////	}
////
////	vecCustInfo = it->second;
////	return TRUE;
////}
////
////// �������̨������Ϣ����ȡ�ͻ�����Ŀ
////void CNearCommCustDlg::GetCustCountFromServer()
////{
////	CString strBgnTime = GetTime(m_dataBgn, m_timeBgn);
////	CString strEndTime = GetTime(m_dataEnd, m_timeEnd);
////
////	// ȥ��̨��ѯ
////	XT_GET_NEAR_COMM_CUST_REQ req;
////	strcpy(req.szBgnTime, strBgnTime);
////	strcpy(req.szEndTime, strEndTime);
////	req.iMaxCustCount = 1000;
////	req.uiBusinessID = g_ImUserInfo.GetId();
////	req.iStartPos = 999;
////	req.iNeedCustCount = LINE_COUNT;
////	req.bDirect = m_bDirect;
////	g_pProtocol->SendReq(req);
////}
////
////// ����̨������Ϣ������ǰҳ����
////void CNearCommCustDlg::GetPageDataFromServer()
////{
////	int nPageNo = GetPageNo();
////	CString strBgnTime = GetTime(m_dataBgn, m_timeBgn);
////	CString strEndTime = GetTime(m_dataEnd, m_timeEnd);
////
////	// ȥ��̨��ѯ
////	XT_GET_NEAR_COMM_CUST_REQ req;
////	strcpy(req.szBgnTime, strBgnTime);
////	strcpy(req.szEndTime, strEndTime);
////	req.iMaxCustCount = 1000;
////	req.uiBusinessID = g_ImUserInfo.GetId();
////	req.iStartPos = (nPageNo - 1)*LINE_COUNT;
////	req.iNeedCustCount = LINE_COUNT;
////	req.bDirect = m_bDirect;
////	g_pProtocol->SendReq(req);
////}
////
////// ����ͻ���Ϣ
////void CNearCommCustDlg::OutputCustInfo(const vector<XT_COMMINFO_GET> &vecCustInfo)
////{
////	ClearCustInfo();
////
////	for (size_t i = 0; i < vecCustInfo.size(); ++i)
////	{
////		const XT_COMMINFO_GET &comminfo = vecCustInfo[i];
////		m_listOutput.InsertItem(i, "");
////		m_listOutput.SetItemText(i, 0, comminfo.szCommAccount);				// �˺�
////		m_listOutput.SetItemText(i, 1, comminfo.szCommTime);				// ʱ��
////		m_listOutput.SetItemText(i, 2, comminfo.szTransInfo);				// ������Ϣ
////		m_listOutput.SetItemText(i, 3, comminfo.szRemarks);					// ��ע
////	}
////}
////
////// ��ս�����ʾ�Ŀͻ���Ϣ
////void CNearCommCustDlg::ClearCustInfo()
////{
////	m_listOutput.DeleteAllItems();
////}
////
////// ������ҳ��
////void CNearCommCustDlg::UpdatePageCount()
////{
////	m_nPageCount = m_nCustCount / LINE_COUNT;
////	if (m_nCustCount % LINE_COUNT > 0)
////	{
////		m_nPageCount++;
////	}
////	CString strPageCount;
////	strPageCount.Format("/%dҳ", m_nPageCount);
////	SetDlgItemText(IDC_STATIC_PAGE_COUNT, strPageCount);
////}
////
////void CNearCommCustDlg::OnLvnColumnclickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
////{
////	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
////	if (pNMLV->iSubItem == 1)
////	{	// �ı�����ʽ
////		XColumnStatus status = m_listOutput.GetColumnStatus(1);
////		if (status == xcsArrowDown)
////		{
////			m_listOutput.SetColumnStatus(1, xcsArrowUp);
////		}
////
////		if (status == xcsArrowUp)
////		{
////			m_listOutput.SetColumnStatus(1, xcsArrowDown);
////		}
////
////		OnChangeDirect();
////	}
////	*pResult = 0;
////}
////
////// �鿴�����¼
////void CNearCommCustDlg::OnBnClickedViewRecord()
////{
////	// ��ȡ�û���Ϣ
////	FRIEND_INFO *fi = GetSelFriend();
////	if (fi == NULL)
////	{
////		return;
////	}
////
////	CMsgDlg * pDlg = CMsgDlg::GetInstance();
////	pDlg->BringWindowToTop();
////	pDlg->SetDefaultFriend(fi->GetId());
////}