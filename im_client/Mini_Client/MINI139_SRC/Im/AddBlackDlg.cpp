// AddBlackDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "AddBlackDlg.h"
#include "afxdialogex.h"
#include "impublic.h"

#include "p2pSendmsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
#include "MessageHandleWnd.h"
#include "..\MainFrame.h"


using namespace DuiLib;

map<int, std::tr1::shared_ptr<CAddBlackDlg>> CAddBlackDlg::g_mapDlg;

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern  void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
extern  CDuiP2PSendMsgDlg * ShowMessageDlg(FRIEND_INFO *pFriend, StruExtraInfo struExtInfo = StruExtraInfo(), BOOL bIsShow = TRUE);
CAddBlackDlg::CAddBlackDlg(int nFriendID)
{
	m_nFriendID = nFriendID;
	m_pFriend = NULL;

	FRIEND_INFO *pFriend = FindFriend(nFriendID);
	if(pFriend != NULL)
	{
		m_pFriend = new FRIEND_INFO();
		memcpy(m_pFriend, pFriend, sizeof(FRIEND_INFO));
	}
	else
	{
		m_pFriend = new FRIEND_INFO();
		m_pFriend->SelfEncrypt();
		m_pFriend->SetId(nFriendID);
	}
}

CAddBlackDlg::~CAddBlackDlg()
{
	if(m_pFriend != NULL)
	{
		delete m_pFriend;
		m_pFriend = NULL;
	}
}

BEGIN_XT_MESSAGE_MAP(CAddBlackDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
END_XT_MESSAGE_MAP()

void CAddBlackDlg::Show(int nFriendID)
{
	if(g_mapDlg.find(nFriendID) != g_mapDlg.end())
	{
		BringWindowToTop(g_mapDlg[nFriendID]->GetHWND());
	}
	else
	{
		tr1::shared_ptr<CAddBlackDlg> pDlg = tr1::shared_ptr<CAddBlackDlg>(new CAddBlackDlg(nFriendID));
		pDlg->Create(GetDesktopWindow(), _T("��Ӻ���"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pDlg->CenterWindow();
		pDlg->ShowWindow(true);
		g_mapDlg[nFriendID] = pDlg;
	}
}

void CAddBlackDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);

	//////////��ȡ�ؼ�����
	m_plabelUser = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("userName"));
	m_pEditName  = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("BackNameEdit"));
	m_pComboGroup= static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl("FriGroupCombo"));

	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));
	m_pBtnOK = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnOK"));
	m_pBtnSend = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnSendMsg"));
	m_pBtnOK->SetEnabled(false);
	m_pBtnSend->SetEnabled(false);
	m_pTabPage = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("page"));
	m_pLbUserInfoLadding = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lblGetUserInfo"));
	m_pLbUserImg = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lblPic"));

	m_pHLVeriInfo = static_cast<DuiLib::CHorizontalLayoutUI*>(m_PaintManager.FindControl("hLVeriInfo"));
	m_pREVeriInfo = static_cast<DuiLib::CRichEditUI*>(m_PaintManager.FindControl("editVeriInfo"));

	m_pEditName->SetMaxChar(8);

	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,GetHWND());
	// ���º�������
	XT_USERINFO_GET_REQ req;
	req.fid = m_nFriendID;
	g_pProtocol->SendReq(req);   

	// 10������Ƿ�ɹ���ȡ��������Ϣ
	SetTimer(GetHWND(), TIMER_CHECK_SUCCESS, 10000, NULL);
	SetTimer(GetHWND(), TIMER_LOADDING, 500, NULL);

	UpdateBk();
}

// ��Ϣ
void CAddBlackDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnClose) ////�ر�
		{
			OnClose();
		}
		else if(msg.pSender == m_pBtnOK) ////ȷ��
		{
			OnOK();
		}
		else if(msg.pSender == m_pBtnSend) ////����
		{
			OnBnClickedSendMsg();
		}
	}
}

void CAddBlackDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_CHECK_SUCCESS:
		{
			KillTimer(GetHWND(), TIMER_LOADDING);
			KillTimer(GetHWND(), nIDEvent);
			if(m_pFriend->GetId() == 0)
			{
				CMessageBox::Model(GetHWND(), "��ȡ������Ϣʧ�ܣ�", IM_CAPTION, MB_OK|MB_ICONERROR);
				Close(IDCANCEL);
			}
		}
		break;
	case TIMER_LOADDING:
		{
			static int sCount = 0;
			sCount++;
			CString strText = "���ڻ�ȡ��������";
			if(sCount%3 == 0)	strText += ".";
			else if(sCount%3 == 1)	strText += "..";
			else if(sCount%3 == 2)	strText += "...";
			m_pLbUserInfoLadding->SetText(strText);
		}
		break;
	default:
		break;
	}
}

LRESULT CAddBlackDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// �����̨������Ϣ
	else if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}
	else if(WM_TIMER == uMsg)
	{	// ʱ����Ϣ
		OnTimer(wParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CAddBlackDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(hWnd);
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);

	map<int, std::tr1::shared_ptr<CAddBlackDlg>>::iterator it = g_mapDlg.find(m_nFriendID);
	if(it != g_mapDlg.end())
	{
		g_mapDlg.erase(it);
	}
}

void CAddBlackDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()))
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, GetHWND());
		}
	}
}

// �������б���
void CAddBlackDlg::UpdateAllBk()
{
	map<int, std::tr1::shared_ptr<CAddBlackDlg>>::iterator it = g_mapDlg.begin();
	for(; it != g_mapDlg.end(); ++it)
	{
		it->second->UpdateBk();
	}
}

void CAddBlackDlg::OnOK()
{
   ////��ţ����֯�ܹ��еĲ������ƶ�
	CString strTxt = _T("");
	int nIdx = -1;
	strTxt = m_pComboGroup->GetText();

	map<int, CString>::iterator it;
	for (it = m_mapGroupInfo.begin(); it != m_mapGroupInfo.end(); ++it)
	{
		if (strcmp(it->second, strTxt) == 0)
		{
			nIdx = it->first;
			break;////����
		}
	}
	if(nIdx == -1)
		return;

	nIdx = g_GroupInfo.FindItemIndex(nIdx);
	if (nIdx > g_GroupInfo.count)
	{
		return;
	}

	FRIEND_INFO *pFriend = FindFriend(m_pFriend->GetId());
	CMainFrame::GetInstance()->m_pFriendList->SetCurSel(pFriend);
	::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_IMFRIEND_LIST_NOTIFY, NM_FL_DRAGDROP, MAKELPARAM(nIdx, 0));
	
	/////�޸ı�ע��
	Sleep(200);
	CString strComName = m_pEditName->GetText();
	strComName.Trim();
		
	if(strComName == m_pFriend->comment_name || strComName.GetLength()  == 0)
	{
	   
	}
	else if (nIdx != 0)// İ���˲������޸ĵ�������
	{
		//�޸��û���ע����
		XT_MOD_FRIEND_COMMNAME_REQ req;
		req.fid = m_pFriend->GetId();
		req.id=g_ImUserInfo.GetId();
		strcpy(req.commName,strComName);
		// ���Ͳ���
		g_pProtocol->SendReq(req);
		//CMainFrame::GetInstance()->SetCommName(m_pFriend,strComName);
	}
	Close(IDOK);
	
}

void CAddBlackDlg::OnClose()
{
	Close(IDCANCEL);
}


void CAddBlackDlg::OnBnClickedSendMsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame::OpenP2PMsgDlg(m_pFriend);
}

void CAddBlackDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==m_pFriend->GetId() )
		{   
			KillTimer(GetHWND(), TIMER_LOADDING);
			KillTimer(GetHWND(), TIMER_CHECK_SUCCESS);
			m_pBtnOK->SetEnabled(true);
			m_pBtnSend->SetEnabled(true);
			m_pTabPage->SelectItem(1);

			// ������ע��
			CString sCName=m_pFriend->comment_name;
			UpdateUserFullInfo(m_pFriend, ack);
			if(!sCName.IsEmpty())
			{
				strncpy(m_pFriend->comment_name,sCName,sizeof(m_pFriend->comment_name)-1);
			}
	        
			m_pEditName->SetText(m_pFriend->comment_name);

			////�û���
			if(strlen(m_pFriend->GetUserName().c_str()) > 0)
			{
				CString strName = UserInfoToStr(m_pFriend);
				m_plabelUser->SetText(strName);
				m_plabelUser->SetToolTip(strName);
			}
			else
			{
				m_plabelUser->SetText("\"�û���δ��ȡ\"");
			}

			////���ѷ�������
			AddToComboBox();

			// ����ͷ��
			CString strLocalPath =  g_webUserHeadIconProcess.GetFaceFile(m_pFriend);
			string strImg = CutRoundImg(strLocalPath, 100, 100, FALSE);
			strLocalPath = strImg.c_str();
			if ( FileExist(strLocalPath) )
			{
				LPCSTR szHead = (LPSTR)(LPCSTR)strLocalPath;
				m_pLbUserImg->SetBkImage(szHead);
			}	

			// �Ƿ���Ҫ��֤
			//m_pHLVeriInfo->SetVisible(m_pFriend->rule == 1);
			m_pHLVeriInfo->SetVisible(false);
		}
	}
}

void  CAddBlackDlg::AddToComboBox()
{
	m_pComboGroup->RemoveAll();//���
	m_mapGroupInfo.clear(); /////���MAP
	for (int i = 0; i<g_GroupInfo.count; i++)//////�޸��ж��߼�����֯�ܹ�������䶯 2014-11-12
	{
		if (g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
		{
			//�ų�İ���˺ͺ�����
			if (g_GroupInfo.group[i].group_index != GROUPID_STRANGER && g_GroupInfo.group[i].group_index != GROUPID_BLACKLIST)
			{
				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
				pListElement->SetAttribute("text",g_GroupInfo.group[i].name);
				pListElement->SetTag(g_GroupInfo.group[i].group_index);
				m_pComboGroup->Add(pListElement);	
				CString str = g_GroupInfo.group[i].name;
				m_mapGroupInfo[g_GroupInfo.group[i].group_index] = str;
			}
		}
	}
	m_pComboGroup->SelectItem(0);	
}

//////////////////////////////////////////////////////////////////////////
// CAddBlackDlg �Ի���
//CAddBlackDlg * CAddBlackDlg::pInstance = NULL;
//////���ݵ�������Ϣ�д���2014-11-24
////IMPLEMENT_DYNAMIC(CAddBlackDlg, CXSkinDlg)
////
////CAddBlackDlg::CAddBlackDlg(FRIEND_INFO* p_Friend,CMainFrame* p_IMPage,CWnd* pParent /*=NULL*/)
////	: CXSkinDlg(CAddBlackDlg::IDD, pParent)
////{
////	//////���ñ�����ɫ//////////////////////////////////////
////	m_colorBackground = RGB(255, 255, 255);
////	m_colorBorder = CHAT_BKG_COLOR;
////	DeleteObject(m_hBkGround);
////	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
////
////	m_bmpLogo.LoadBitmap(IDB_IM_STATUS100);
////	////////////////////////////////////////////////////////
////	pFriend = p_Friend; 
////	pIMPage = p_IMPage;
////
////}
////
////CAddBlackDlg::~CAddBlackDlg()
////{
////	g_pProtocol->UnRegistCmd(m_hWnd);
////	DeleteObject(m_hBkGround);
////}
////
////void CAddBlackDlg::DoDataExchange(CDataExchange* pDX)
////{
////	CXSkinDlg::DoDataExchange(pDX);
////	DDX_Control(pDX, ID_OK, m_btnOK);
////	DDX_Control(pDX, ID_SEND, m_btnSend);
////}
////
////BEGIN_MESSAGE_MAP(CAddBlackDlg, CXSkinDlg)
////	ON_BN_CLICKED(ID_OK, &CAddBlackDlg::OnBnClickedOk)
////	ON_BN_CLICKED(ID_SEND, &CAddBlackDlg::OnBnClickedSendMsg)
////	ON_WM_CTLCOLOR()
////	ON_WM_PAINT()
////	ON_WM_CLOSE()
////	ON_XT_MESSAGE()
////END_MESSAGE_MAP()
////
////BEGIN_XT_MESSAGE_MAP(CAddBlackDlg)
////	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
////END_XT_MESSAGE_MAP()
////
////HBRUSH CAddBlackDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////{
////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
////	if ( nCtlColor==CTLCOLOR_STATIC )
////	{
////		pDC->SetBkMode(TRANSPARENT);
////		hbr = (HBRUSH)m_hBkGround;
////	}
////	return hbr;
////}
////
////void CAddBlackDlg::OnPaint()
////{
////	CPaintDC dc(this); // device context for painting
////	// TODO: �ڴ˴������Ϣ����������
////	// ��Ϊ��ͼ��Ϣ���� CXSkinDlg::OnPaint()
////	CXSkinDlg::OnPaint();
////	CWnd *pWnd = GetDlgItem(IDC_STATIC_LOGO);
////	if ( pWnd )
////	{
////		CRect rc;
////		pWnd->GetWindowRect(rc);
////		ScreenToClient(&rc);
////		DrawBitmap(&dc,rc.left,rc.top,&m_bmpLogo);
////	}
////}
////
////void CAddBlackDlg::PostNcDestroy()
////{
////	CXSkinDlg::PostNcDestroy();
////
////	delete this;
////}
////
////BOOL CAddBlackDlg::OnInitDialog()
////{
////	CXSkinDlg::OnInitDialog();
////	SetWindowText("��Ӻ��ѳɹ�");
////
////	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
////
////	// ���������ӿؼ������ÿؼ�����
////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////	while(hWndChild)  
////	{  
////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////	}
////
////	//���º�������
////	XT_USERINFO_GET_REQ req;
////	req.fid = pFriend->GetId();
////	g_pProtocol->SendReq(req);
////
////	SetDlgStyle(NONE);
////	return true;
////}
////
////void  CAddBlackDlg::AddToComboBox()
////{
////	ClearComboBox();/////�������
////	m_mapGroupInfo.clear();/////���MAP
////	for (int i = 0; i<g_GroupInfo.count; i++)//////�޸��ж��߼�����֯�ܹ�������䶯 2014-11-12
////	{
////		if (g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
////		{
////			//�ų�İ���˺ͺ�����
////			if (g_GroupInfo.group[i].group_index != GROUPID_STRANGER && g_GroupInfo.group[i].group_index != GROUPID_BLACKLIST)
////			{
////				((CComboBox*)GetDlgItem(IDC_COMBO_GROUP))->AddString(g_GroupInfo.group[i].name);
////				CString str = g_GroupInfo.group[i].name;
////				m_mapGroupInfo[i] = str;
////			}
////		}
////		((CComboBox*)GetDlgItem(IDC_COMBO_GROUP))->SetCurSel(0);
////	}
////}
////
////void  CAddBlackDlg::ClearComboBox()
////{
////	((CComboBox*)GetDlgItem(IDC_COMBO_GROUP))->ResetContent();
////}
////
////void CAddBlackDlg::OnBnClickedOk()
////{
////	// TODO: �ڴ���ӿؼ�֪ͨ����������
////	//��ţ����֯�ܹ��еĲ������ƶ�
////	CString strTxt = _T("");
////	int nIdx = 0;
////	((CWnd*)GetDlgItem(IDC_COMBO_GROUP))->GetWindowText(strTxt);
////	map<int, CString>::iterator it;
////	for (it = m_mapGroupInfo.begin(); it != m_mapGroupInfo.end(); ++it)
////	{
////		if (strcmp(it->second, strTxt) == 0)
////		{
////			nIdx = it->first;
////			break;////����
////		}
////	}
////	if (nIdx > g_GroupInfo.count)
////	{
////		return;
////	}
////
////	//��ѡ��Ҫ�ƶ�����ĺ���,�ٷ���Ϣ
////	//pIMPage->m_wndGroupList.SetCurSel(pFriend);
////	::PostMessage(pIMPage->GetHWND(), WM_IMFRIEND_LIST_NOTIFY, NM_FL_DRAGDROP, MAKELPARAM(nIdx, 0));
////
////	CString strComName;
////	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(strComName);
////	strComName.Trim();
////	
////	if(strComName == pFriend->comment_name || strComName.GetLength()  == 0)
////	{
////
////	}else if (pFriend->group_index != 1)// İ���˲������޸ĵ�������
////	{
////		//�޸��û���ע����
////		XT_MOD_FRIEND_COMMNAME_REQ req;
////		req.fid = pFriend->GetId();
////		req.id=g_ImUserInfo.GetId();
////		strcpy(req.commName,strComName);
////		// ���Ͳ���
////		g_pProtocol->SendReq(req);
////		pIMPage->SetCommName(pFriend, strComName);
////	}
////	
////	OnOK();
////
////	DestroyWindow();
////}
////
////
////void CAddBlackDlg::OnBnClickedSendMsg()
////{
////	// TODO: �ڴ���ӿؼ�֪ͨ����������
////	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
////	OnBnClickedOk();
////
////	if ( pDlg->GetHWND()==NULL )
////		//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
////		pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, GetDesktopWindow());
////
////	if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
////		pDlg->ShowWindow(/*SW_SHOW*/true);
////
////	if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
////		pDlg->ShowWindow(/*SW_RESTORE*/true);
////
////	/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
////}
////
////void CAddBlackDlg::OnUserInfoAck(const char *data,WORD wRet)
////{
////	if ( wRet==0 )
////	{
////		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;
////
////		if ( ack->GetId()==pFriend->GetId() )
////		{   
////			// ������ע��
////			CString sCName=pFriend->comment_name;
////			//memcpy(pFriend,ack,sizeof(XT_USER_INFO));
////			UpdateUserFullInfo(pFriend, ack);
////			strncpy(pFriend->comment_name,sCName,sizeof(pFriend->comment_name)-1);
////			
////			if(strlen(pFriend->GetUserName().c_str()) > 0)
////				GetDlgItem(IDC_STATIC_MEMBER)->SetWindowText(pFriend->GetUserName().c_str());
////			else
////				GetDlgItem(IDC_STATIC_MEMBER)->SetWindowText("\"�û���δ��ȡ\"");
////
////			ClearComboBox();
////			AddToComboBox();
////		}
////	}
////}
