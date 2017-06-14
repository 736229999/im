// im\UserAuthDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "UserAuthDlg.h"
#include ".\userauthdlg.h"
#include "..\FriendInfoDlg.h"
#include "LocalData.h"
#include "MessageHandleWnd.h"

// CUserAuthReqDlg 对话框
vector<CUserAuthReqDlg *>  CUserAuthReqDlg::m_vecInstance;
uint32  CUserAuthReqDlg::m_preReqId = 0;//上次请求的用户id
CString CUserAuthReqDlg::m_preReqInfo = "";
time_t  CUserAuthReqDlg::m_preReqTime = 0;
#define EDIT_AUTH_MAX 60

IMPLEMENT_DYNAMIC(CUserAuthReqDlg, CXSkinDlg)
CUserAuthReqDlg::CUserAuthReqDlg(FRIEND_INFO *pFriend,CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CUserAuthReqDlg::IDD, pParent)
{
	//////添加颜色设置///////////////
	m_colorBackground = CHAT_BKG_COLOR;
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	m_FriendInfo = *pFriend;
	m_hBkGround  = CreateSolidBrush(RGB(255,255,255));

	m_vecInstance.push_back(this);
	m_nGroupId = 0;
	m_dwSelAuth = 0;
	m_iLenAuth = 0;
}

CUserAuthReqDlg::~CUserAuthReqDlg()
{
	for ( int i=0; i<m_vecInstance.size(); i++ )
	{
		if ( this==m_vecInstance[i] )
		{
			m_vecInstance.erase(m_vecInstance.begin()+i);
			break;
		}
	}

	DeleteObject(m_hBkGround);
}

void CUserAuthReqDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_FACE,m_wndFaceIcon);
	DDX_Control(pDX,IDC_STATIC_MORE,m_btnDetail);
	DDX_Control(pDX, IDC_SEND, m_btnSend);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_AUTH, m_edtAuth);
	DDX_Control(pDX, IDC_CHECK_ALLOW, m_btnAllow);
}


BEGIN_MESSAGE_MAP(CUserAuthReqDlg, CXSkinDlg)
	ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
	ON_STN_CLICKED(IDC_STATIC_MORE, OnStnClickedStaticMore)
	ON_XT_MESSAGE()
	ON_WM_NCDESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_AUTH, &CUserAuthReqDlg::OnChangeEditAuth)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CUserAuthReqDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
END_XT_MESSAGE_MAP()


// CUserAuthReqDlg 消息处理程序

BOOL CUserAuthReqDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("添加好友");
    SetDlgStyle(MINCLOSE);
	SetDlgItemText(IDC_EDIT_AUTH,"你好，交个朋友可以吗？");
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	m_btnAllow.SetCheck(1);

	return TRUE;  
}

void CUserAuthReqDlg::OnBnClickedSend()
{
	XT_USERAUTH_REQ req;
	req.from_id = g_ImUserInfo.GetId();
	req.groupId = m_nGroupId;
	req.to_id   = m_FriendInfo.GetId();
	req.action  = XT_USERAUTH_REQ::ACTION_REQUEST;//请求
	req.send_time = time(NULL);
	
	CString strAuth;
	GetDlgItemText(IDC_EDIT_AUTH,strAuth);
	strcpy(req.info, strAuth);
	//GetDlgItemText(IDC_EDIT_AUTH,req.info,120);
	CString sAllowIni;
	CString sID;

	if ( req.to_id!=m_preReqId ||
		req.info!=m_preReqInfo ||
		(req.send_time-m_preReqTime)>20 )
	{
		g_pProtocol->SendReq(req);
		sAllowIni.Format("%sdata\\%s\\allow.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo.GetUserName().c_str());
		sID.Format("%d",m_FriendInfo.GetId());
		CString sVal = "0";
		if(m_btnAllow.GetCheck())
		{
			sVal = "1";
		}
		::WritePrivateProfileString("AllowFriends",sID,sVal,sAllowIni);
		m_preReqId = req.to_id;
		m_preReqInfo = req.info;
		m_preReqTime = req.send_time;
	}

	DestroyWindow();
}

void CUserAuthReqDlg::OnStnClickedStaticMore()
{
	CFriendInfoDlg::NewDlg(&m_FriendInfo);
	XT_USERINFO_GET_REQ req;
	req.fid = m_FriendInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CUserAuthReqDlg::OnUserInfoGet(const char *data,WORD wRet)
{
	if ( wRet==0 && data )
	{
		//memcpy(&m_FriendInfo,data,sizeof(XT_USER_INFO));
		UpdateUserFullInfo(&m_FriendInfo,(XT_USER_INFO*)data);
		ShowUserInfo();
		ShowWindow(SW_SHOW);
	}
}

void CUserAuthReqDlg::Close()
{
	vector<CUserAuthReqDlg *> vecDlg;

	for ( int i=0; i<m_vecInstance.size(); i++ )
	{
		vecDlg.push_back(m_vecInstance[i]);
	}

	for ( int j=0; j<vecDlg.size(); j++ )
		vecDlg[j]->DestroyWindow();
}

void CUserAuthReqDlg::Show(FRIEND_INFO *pFriend,uint32 nGroupId)
{
	CUserAuthReqDlg *pDlg = new CUserAuthReqDlg(pFriend,GetDesktopWindow());
	pDlg->Create(CUserAuthReqDlg::IDD,GetDesktopWindow());
	pDlg->m_nGroupId = nGroupId;

	/*
	if ( pFriend->bRefreshed==false )
	{
		pDlg->ShowWindow(SW_HIDE);

		XT_USERINFO_GET_REQ req;
		req.fid = pFriend->id;
		//req.uid = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);
	}
	else
	{
	*/
		pDlg->ShowUserInfo();
		pDlg->ShowWindow(SW_SHOW);
	//}
}

void CUserAuthReqDlg::OnNcDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);

	CXSkinDlg::OnNcDestroy();

	delete this;
}

void CUserAuthReqDlg::ShowUserInfo()
{
	CString str;
	//str.Format("用户名:%s",m_FriendInfo.username);
	if ( strcmp(m_FriendInfo.GetUserName().c_str(),"")==0 )
	{
		char username[MAX_USERNAME_LEN+1] = {0};
		sprintf(username,"%u(ID)",m_FriendInfo.GetId());
		m_FriendInfo.SetUserName(username);
	}

	SetDlgItemText(IDC_STATIC_ID,m_FriendInfo.GetUserName().c_str());

	//str.Format("昵称:%s",m_FriendInfo.nickname);
	SetDlgItemText(IDC_STATIC_NAME,m_FriendInfo.nickname);

	str.Format("性别:%s", SexToStr(m_FriendInfo.sex)) ;
	SetDlgItemText(IDC_STATIC_SEX,str);

	CEdit *pWnd = (CEdit *)GetDlgItem(IDC_EDIT_AUTH);
	//pWnd->LimitText(120-1);

	m_wndFaceIcon.SetUserInfo(&m_FriendInfo);
}

HBRUSH CUserAuthReqDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

BOOL CUserAuthReqDlg::OnEraseBkgnd(CDC* pDC)
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

void CUserAuthReqDlg::OnClose()
{
	DestroyWindow();
}

void CUserAuthReqDlg::OnBnClickedCancel()
{
	DestroyWindow();
}


void CUserAuthReqDlg::OnChangeEditAuth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CXSkinDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_AUTH);
	m_dwSelAuth = pEdit->GetSel();//获取当前光标位置
	int maxSize =EDIT_AUTH_MAX;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenAuth =0;//文字个数
	for (int i=0;strName[i];i++) {		
		if (strName[i] < 0){  			   
			tmp.Format("%c%c",strName[i],strName[i+1]);
			allName = allName +tmp;
			i++;   			
		}
		else{
			tmp.Format("%c",strName[i]);
			allName = allName +tmp;
		}
		m_iLenAuth++;
		if (m_iLenAuth>=maxSize){
			if (strName != allName)
			{
				pEdit->SetWindowText(allName);
				pEdit->SetSel(m_dwSelAuth);//群签名//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}	
	// TODO:  在此添加控件通知处理程序代码
}


BOOL CUserAuthReqDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			return TRUE;
			break;

		default:

			if(m_iLenAuth>=EDIT_AUTH_MAX)//身份验证
			{
				CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_AUTH);
				pEdit->SetSel(m_dwSelAuth);//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}

	return CXSkinDlg::PreTranslateMessage(pMsg);
}
