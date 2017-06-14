// UserAuthInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\userauthinfodlg.h"
#include "..\FriendInfoDlg.h"
#include "SelectGroupdlg.h"
#include "LocalData.h"
#include "..\MainFrame.h"
#include "..\DuiMfRecentList.h"
#include "UserAuthDlg.h"
#include "MessageHandleWnd.h"
#include "..\CSPlatDlg.h"
// CUserAuthInfoDlg �Ի���
vector<CUserAuthInfoDlg *> CUserAuthInfoDlg::m_vecInstance;
vector<uint32> CUserAuthInfoDlg::vecRefuseUser;
bool CUserAuthInfoDlg::s_bInitRefuseUser=false;

IMPLEMENT_DYNAMIC(CUserAuthInfoDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CUserAuthInfoDlg)

CUserAuthInfoDlg::CUserAuthInfoDlg( CWnd* pFloorIMPage, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CUserAuthInfoDlg::IDD, pParent)
{
	ASSERT(pFloorIMPage);
	Create(CUserAuthInfoDlg::IDD,pParent);
	m_hBkGround = CreateSolidBrush(RGB(255,255,255));

	m_pFloorIMPage = pFloorIMPage;
	m_vecInstance.push_back(this);
	//m_nGroupIndex = 0;
	//m_pFriendList=pFriendList;
}

CUserAuthInfoDlg::~CUserAuthInfoDlg()
{
	for ( int i=0; i<m_vecInstance.size(); i++ )
	{
		if ( m_vecInstance[i]==this)
		{
			m_vecInstance.erase(m_vecInstance.begin()+i);
			break;
		}
	}

	DeleteObject(m_hBkGround);
}

void CUserAuthInfoDlg::Close() 
{
	vector<CUserAuthInfoDlg *> vecDlg;

	for ( int i=0; i<m_vecInstance.size(); i++ )
	{
		vecDlg.push_back(m_vecInstance[i]);
	}

	for ( int i=0; i<vecDlg.size(); i++ )
		vecDlg[i]->DestroyWindow();

	g_LocalData.SaveRefuseUsers(vecRefuseUser);
	vecRefuseUser.clear();
	s_bInitRefuseUser = false;
}

void CUserAuthInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_STATIC_FACE,m_wndFaceIcon);
	DDX_Control(pDX,IDC_STATIC_MORE,m_btnMore);
	DDX_Control(pDX, IDC_OK, m_btnOk);
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CHECK_ADD, m_chkAdd);
	DDX_Control(pDX, IDC_EDIT_AUTH, m_edtAuth);
	DDX_Control(pDX, IDC_CHECK_REFUSE, m_chkRefuse);
}


BEGIN_MESSAGE_MAP(CUserAuthInfoDlg, CXSkinDlg)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_WM_CLOSE()
	ON_XT_MESSAGE()
	ON_STN_CLICKED(IDC_STATIC_MORE, OnStnClickedStaticMore)
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	SET_HMG_SCROLLBAR(IDC_EDIT_AUTH)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CUserAuthInfoDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
END_XT_MESSAGE_MAP()


// CUserAuthInfoDlg ��Ϣ�������

void CUserAuthInfoDlg::OnBnClickedOk()
{
	//0x00=����; 0x01=ͬ��; 0x02=�ܾ�
	if ( m_userAuthReq.action==0x00 )// �Է�����������
	{
		CButton * pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
	
		if ( pRadio1->GetCheck() )
		{
			XT_USERAUTH_REQ req;
			req.action = 0x01;//ͬ��
			req.from_id = g_ImUserInfo.GetId();
			req.to_id   = m_userAuthReq.from_id;
			req.groupId = m_userAuthReq.groupId;
			req.send_time = time(NULL);
			strcpy(req.info,"-");
			g_pProtocol->SendReq(req);
			

			// ��ס���ѡ�����Է���Ҫ��֤��ֱ�ӷ�����֤
			CString sAllowIni;
			CString sID;
			sAllowIni.Format("%sdata\\%s\\allow.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
			sID.Format("%d",req.to_id);
			CString sVal = "0";

			//�ӶԷ�Ϊ����
			if ( m_chkAdd.GetCheck() )
			{	
				sVal = "1";
				//����һ��.
				XT_FRIENDGROUP_MOD_REQ req;
				req.action = GROUP_ADD;
				req.uid = g_ImUserInfo.GetId();
				req.group_id = m_userAuthReq.groupId;
				req.fid = m_userAuthReq.from_id;
				g_pProtocol->SendReq(req);	

			}
			::WritePrivateProfileString("AddFriends",sID,sVal,sAllowIni);
		}
		else
		{
			XT_USERAUTH_REQ req;

			req.action = 0x02;//�ܾ�
			req.from_id = g_ImUserInfo.GetId();
			req.to_id   = m_userAuthReq.from_id;
			req.send_time = time(NULL);
			sprintf(req.info,"-");
			g_pProtocol->SendReq(req);

			if ( m_chkRefuse.GetCheck() )
			{
				vecRefuseUser.push_back(req.to_id);
			}
		}

	}
	else if ( m_userAuthReq.action==0x02 )// �Է������˾ܾ�
	{
		CUserAuthReqDlg::Show(&m_friendInfo,m_userAuthReq.groupId);
	}
	else if ( m_userAuthReq.action==0x03 )// ����Ϊ���ѵ�֪ͨ
	{
		XT_FRIENDGROUP_MOD_REQ req;
		req.action = GROUP_ADD;
		req.uid = g_ImUserInfo.GetId();
		req.group_id = m_userAuthReq.groupId;
		req.fid = m_userAuthReq.from_id;
		g_pProtocol->SendReq(req);	
		// ѡ����ѷ���
	}

	DestroyWindow();
}


void CUserAuthInfoDlg::OnBnClickedCancel()
{
	if(m_userAuthReq.action==0x01 || m_userAuthReq.action==0x03)
	{
		// ����õ�ͬ��
		// ѡ����ѷ���
		if(m_pFloorIMPage)
		{
			CSelectGroupDlg::NewDlg(m_friendInfo.GetId(),m_friendInfo.GetUserName().c_str(),m_pFloorIMPage->m_hWnd);
		}
	}

	DestroyWindow();
}

void CUserAuthInfoDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId() == m_userAuthReq.from_id )
		{
			ShowWindow(SW_SHOW);
			FlashWindow(TRUE);

			//memcpy(&m_friendInfo,ack,sizeof(XT_USER_INFO));
			
			UpdateUserFullInfo(&m_friendInfo,ack);
		
			// ���û�и��û����������������ʱ���ݻ�����
			FRIEND_INFO * pUser = GetFriendInfo(m_friendInfo.GetId());

			if(pUser == NULL)
			{
				pUser = new FRIEND_INFO(m_friendInfo);
				pUser->group_index = 1;
				g_lsFriendTemp.push_back(pUser);
			}
			
			switch ( m_userAuthReq.action )
			{
			case 0x00://����
				m_chkAdd.ShowWindow(SW_SHOW);
				if (m_chkRefuse.GetCheck()==0 )
					m_chkAdd.SetCheck(1);
				break;
			case 0x01://ͬ��
				m_btnOk.ShowWindow(SW_HIDE);
				m_btnCancel.SetWindowText("����");
				break;
			case 0x02://�ܾ�
				m_btnOk.SetWindowText("��������");
				m_btnCancel.SetWindowText("����");

				if ( m_friendInfo.rule==0x02 )
				{
					m_btnOk.ShowWindow(SW_HIDE);
				}
				break;
			case 0x03://֪ͨ
				m_btnOk.SetWindowText("��Ϊ����");
				m_btnCancel.SetWindowText("����");
				break;
			}

			ShowUserInfo();
		}
	}	
}

void CUserAuthInfoDlg::PostNcDestroy()
{	
	g_pProtocol->UnRegistCmd(m_hWnd);

	CXSkinDlg::PostNcDestroy();
	delete this;
}

void CUserAuthInfoDlg::ShowUserInfo()
{
	CString str;

	switch(m_userAuthReq.action)
	{
	case 0x00:
		SetWindowText("hi~,��Ҫ����Ϊ����");
		SetDlgItemText(IDC_EDIT_AUTH,m_userAuthReq.info);
		break;
	case 0x01://ͬ��
		str.Format("�����º���..",m_friendInfo.nickname);
		SetWindowText(str);

		str.Format("%s\r\nͨ�����������,�Ѿ��� ���� ���뵽�����б�����\r\n�뷵��",m_friendInfo.nickname);
		SetDlgItemText(IDC_EDIT_AUTH,str);
		break;	
	case 0x02://�ܾ�
		SetWindowText("�㱻�ܾ���");

		if ( m_friendInfo.rule==0x02 )
		{
			str.Format("%s\r\n�ܾ����κ�����Ϊ����!\r\n�뷵��",m_friendInfo.nickname);		
		}
		else
		{
			str.Format("%s\r\n�ܾ����������!\r\n�����·���������Ϣ�򷵻�!",m_friendInfo.nickname);		
		}
		SetDlgItemText(IDC_EDIT_AUTH,str);
		break;	
	case 0x03:
		SetWindowText("���˼���Ϊ����");
		str.Format("\r\n   ���Ѿ��� %s ��Ϊ����",m_friendInfo.nickname);		
		FRIEND_INFO *pFriend = FindFriend(m_friendInfo.GetId());
		if(pFriend != NULL)
		{
			CMainFrame::GetInstance()->GetRecentList()->UpdateUser(pFriend);
			RECENTLIST_OP(UpdateUser(pFriend))
		}
		SetDlgItemText(IDC_EDIT_AUTH,str);
		break;	
	}


	//str.Format("�û���:%s",m_friendInfo.username);
	SetDlgItemText(IDC_STATIC_ID,m_friendInfo.GetUserName().c_str());

	//str.Format("�ǳ�:%s",m_friendInfo.nickname);
	SetDlgItemText(IDC_STATIC_NAME,m_friendInfo.nickname);

	str.Format("�Ա�:%s", SexToStr(m_friendInfo.sex)) ;
	SetDlgItemText(IDC_STATIC_SEX,str);

	m_wndFaceIcon.SetUserInfo(&m_friendInfo);
}

void CUserAuthInfoDlg::SetUserAuthInfo(FRIEND_INFO *pFriend, uint16 action/*0,����1ͨ��,2�ܾ�*/,CString strVerify,uint32 nGroupId)
{
	memcpy(&m_friendInfo,pFriend,sizeof(XT_USER_INFO));

	if ( action==0 )//��������
	{
		if ( s_bInitRefuseUser==false )
		{
			g_LocalData.GetRefuseUsers(vecRefuseUser);
			s_bInitRefuseUser=true;
		}

		int n=vecRefuseUser.size();
		for ( int i=0; i<n; i++ )
		{
			if ( vecRefuseUser[i]==m_friendInfo.GetId() )//���ܾ���
			{
				PostMessage(WM_CLOSE);
				return ;
			}
		}
	}

	FlashWindow(TRUE);
	m_userAuthReq.action = action;
	m_userAuthReq.groupId = nGroupId;

	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	m_chkRefuse.ShowWindow(SW_HIDE);
	m_chkAdd.ShowWindow(SW_HIDE);


	switch ( m_userAuthReq.action )
	{
	case 0x00://����
		m_chkAdd.ShowWindow(SW_SHOW);
		m_chkAdd.SetCheck(1);
		break;
	case 0x01://ͬ��
		m_btnOk.ShowWindow(SW_HIDE);
		m_btnCancel.SetWindowText("����");
		break;
	case 0x02://�ܾ�
		m_btnOk.SetWindowText("��������");
		m_btnCancel.SetWindowText("����");

		if ( m_friendInfo.rule==0x02 )
		{
			m_btnOk.ShowWindow(SW_HIDE);
		}
		break;
	case 0x03://֪ͨ
		m_btnOk.SetWindowText("��Ϊ����");
		m_btnCancel.SetWindowText("����");
		break;
	}

	ShowUserInfo();

	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	m_chkRefuse.ShowWindow(SW_HIDE);
	m_chkAdd.ShowWindow(SW_HIDE);

	if ( this->m_userAuthReq.action==0 )
	{
			int nBtnTop   ;

			CRect rect;

			GetWindowRect(&rect);
			rect.bottom+=18;
			MoveWindow(&rect);

			CWnd * pWnd = GetDlgItem(IDC_STATIC_LINE);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);

			nBtnTop = rect.bottom-3;
			rect.bottom+=18;

			pWnd->MoveWindow(&rect);

			pWnd = GetDlgItem(IDC_RADIO1);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			rect.top = nBtnTop;
			rect.bottom = rect.top+15;
			pWnd->MoveWindow(&rect);
			pWnd->ShowWindow(SW_SHOW);

			((CButton *)pWnd)->SetCheck(1);

			pWnd = GetDlgItem(IDC_RADIO2);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			rect.top = nBtnTop;
			rect.bottom = rect.top+15;
			pWnd->MoveWindow(&rect);
			pWnd->ShowWindow(SW_SHOW);

			m_chkRefuse.GetWindowRect(&rect);
			ScreenToClient(&rect);
			rect.top = nBtnTop;
			rect.bottom = rect.top+15;
			m_chkRefuse.MoveWindow(&rect);
			m_chkAdd.MoveWindow(&rect);	
			m_chkAdd.ShowWindow(SW_SHOW);

			m_btnOk.SetWindowText("ȷ��");
			m_btnCancel.SetWindowText("ȡ��");
			m_btnOk.GetWindowRect(&rect);
			rect.top+=18;
			rect.bottom+=18;
			ScreenToClient(&rect);
			m_btnOk.MoveWindow(&rect);

			m_btnCancel.GetWindowRect(&rect);
			rect.top+=18;
			rect.bottom+=18;
			ScreenToClient(&rect);
			m_btnCancel.MoveWindow(&rect);
	}
}

void CUserAuthInfoDlg::SetUserAuthInfo(uint32 fid, uint16 action/*0,����1ͨ��,2�ܾ�*/,CString strVerify,uint32 nGroupId)
{
	if ( action==0 )//��������
	{
		if ( s_bInitRefuseUser==false )
		{
			g_LocalData.GetRefuseUsers(vecRefuseUser);
			s_bInitRefuseUser=true;
		}

		int n=vecRefuseUser.size();
		for ( int i=0; i<n; i++ )
		{
			if ( vecRefuseUser[i]==fid )//���ܾ���
			{
				PostMessage(WM_CLOSE);
				return ;
			}
		}
	}

	memset(&m_friendInfo,0,sizeof(XT_USER_INFO));
	m_userAuthReq.from_id = fid;
	m_userAuthReq.action  = action;
	m_userAuthReq.groupId = nGroupId;
	sprintf(m_userAuthReq.info,strVerify);
	m_friendInfo.SetId(fid);

	XT_USERINFO_GET_REQ userinfo_req;
	userinfo_req.fid = m_userAuthReq.from_id;
	//userinfo_req.uid = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(userinfo_req);

	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	m_chkRefuse.ShowWindow(SW_HIDE);
	m_chkAdd.ShowWindow(SW_HIDE);

	if ( this->m_userAuthReq.action==0 )
	{
		int nBtnTop   ;

		CRect rect;

		GetWindowRect(&rect);
		rect.bottom+=18;
		MoveWindow(&rect);

		CWnd * pWnd = GetDlgItem(IDC_STATIC_LINE);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		nBtnTop = rect.bottom-3;
		rect.bottom+=18;

		pWnd->MoveWindow(&rect);

		pWnd = GetDlgItem(IDC_RADIO1);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top = nBtnTop;
		rect.bottom = rect.top+15;
		pWnd->MoveWindow(&rect);
		pWnd->ShowWindow(SW_SHOW);

		((CButton *)pWnd)->SetCheck(1);

		pWnd = GetDlgItem(IDC_RADIO2);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top = nBtnTop;
		rect.bottom = rect.top+15;
		pWnd->MoveWindow(&rect);
		pWnd->ShowWindow(SW_SHOW);

		m_chkRefuse.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top = nBtnTop;
		rect.bottom = rect.top+15;
		m_chkRefuse.MoveWindow(&rect);
		m_chkAdd.MoveWindow(&rect);	
		m_chkAdd.ShowWindow(SW_SHOW);

		m_btnOk.SetWindowText("ȷ��");
		m_btnCancel.SetWindowText("ȡ��");
		m_btnOk.GetWindowRect(&rect);
		rect.top+=18;
		rect.bottom+=18;
		ScreenToClient(&rect);
		m_btnOk.MoveWindow(&rect);

		m_btnCancel.GetWindowRect(&rect);
		rect.top+=18;
		rect.bottom+=18;
		ScreenToClient(&rect);
		m_btnCancel.MoveWindow(&rect);
	}
}

void CUserAuthInfoDlg::OnClose()
{
	DestroyWindow();
}

BOOL CUserAuthInfoDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(MINCLOSE);

	m_edtAuth.SetBackColor(RGB(255,233,222));
	m_edtAuth.SetTextColor(RGB(0,0,100));

	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);

	AttachHMGScrollbar(m_edtAuth.m_hWnd);

	ShowUserInfo();

	return TRUE;  
}

void CUserAuthInfoDlg::OnStnClickedStaticMore()
{
	CFriendInfoDlg::NewDlg(&m_friendInfo);
}


HBRUSH CUserAuthInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	if ( m_userAuthReq.action==0 )
	{
		UINT nCtlID = pWnd->GetDlgCtrlID();
		if ( nCtlID ==IDC_CHECK_ADD || nCtlID==IDC_CHECK_REFUSE )
		{
			hbr = g_hWhiteBrush;
		}
	}
	else
	{
		if ( pWnd->GetDlgCtrlID()==IDC_CHECK_ADD )
		{
			hbr = CXSkinDlg::m_hBkGround;
		}
	}

	return hbr;
}

BOOL CUserAuthInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRet = CXSkinDlg::OnEraseBkgnd(pDC);

	CRect rcFrame;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rcFrame);
		ScreenToClient(&rcFrame);

		pDC->FillSolidRect(&rcFrame,RGB(255,255,255));
		pDC->Draw3dRect(&rcFrame,RGB(0,122,255),RGB(0,122,255));
	}

	return bRet;
}

void CUserAuthInfoDlg::OnBnClickedRadio1()
{
	m_chkRefuse.ShowWindow(SW_HIDE);
	m_chkAdd.ShowWindow(SW_SHOW);
}

void CUserAuthInfoDlg::OnBnClickedRadio2()
{
	m_chkAdd.ShowWindow(SW_HIDE);
	m_chkRefuse.ShowWindow(SW_SHOW);
}
