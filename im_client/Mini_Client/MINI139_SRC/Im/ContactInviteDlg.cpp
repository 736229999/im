// im\ContactInviteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include "ContactInviteDlg.h"
#include ".\contactinvitedlg.h"
#include "..\MainFrame.h"

#define HINT_STR  "<请输入email地址,多个地址用逗号\",\"隔开>"

vector<CContactInviteDlg *> CContactInviteDlg::vecInstance;
// CContactInviteDlg 对话框

IMPLEMENT_DYNAMIC(CContactInviteDlg, CXSkinDlg)
CContactInviteDlg::CContactInviteDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CContactInviteDlg::IDD, pParent)
{
	vecInstance.push_back(this);
	m_bCanEditTo = false;
	Create(CContactInviteDlg::IDD,pParent);
}

CContactInviteDlg::~CContactInviteDlg()
{
	int n=vecInstance.size();
	for ( int i=0; i<n; i++ )
	{
		if ( vecInstance[i]==this )
		{
			vecInstance.erase(vecInstance.begin()+i);
			break;
		}
	}
}

void CContactInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TO, m_edtTo);
	DDX_Control(pDX, IDC_EDIT_BODY, m_edtMail);
	DDX_Control(pDX, IDC_INVITE, m_btnInvite);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_edtTitle);
}


BEGIN_MESSAGE_MAP(CContactInviteDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_XT_MESSAGE()
	ON_BN_CLICKED(IDC_INVITE, OnBnClickedInvite)
	ON_WM_TIMER()
	ON_EN_SETFOCUS(IDC_EDIT_TO, OnEnSetfocusEditTo)
	ON_EN_KILLFOCUS(IDC_EDIT_TO, OnEnKillfocusEditTo)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CContactInviteDlg)
	ON_XT_COMMAND(CMD_FRIENDINVITE_ACK,OnInviteAck)
END_XT_MESSAGE_MAP()
// CContactInviteDlg 消息处理程序

BOOL CContactInviteDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(MINCLOSE);
	SetWindowText("邀请联系人使用 Mini");

	EnableHelp(true);

	m_edtMail.LimitText(450);
	m_edtTitle.LimitText(100);

	g_pProtocol->RegistCmd(CMD_FRIENDINVITE_ACK,m_hWnd);

	return TRUE;  
}

void CContactInviteDlg::SetEmail(CString strEmail, CString strDisplayName)
{
	CString strBody;

	/*if ( strEmail=="" )
	{
		m_bCanEditTo=true;

		m_edtTo.SetWindowText(HINT_STR);
		m_edtTo.SetTextColor(0xAAAAAA);

		SetWindowText("发送邀请邮件" );
	}
	else
	{
		m_bCanEditTo=false;

		m_edtTo.SetTextColor(IM_LINE_COLOR);
		m_edtTo.SetReadOnly(TRUE);

		m_edtTo.SetWindowText(strEmail);

		SetWindowText("邀请　"+strDisplayName+" 一起来用 哞哞" );
	}

	m_strEmail       = strEmail;
	m_strDisplayName = strDisplayName;

	m_edtTitle.SetWindowText(strDisplayName+"你好,我是"+CString(g_ImUserInfo.nickname)+",推荐你使用一款不错的软件 - 哞哞");

	strBody  = "Hi! ";
	strBody += "\r\n\r\n";
	strBody += "最近我在feiniu.com移动个人空间下载了一个软件-IECiM，可以一次上传数百上千张图片，速度超快，太爽了！还可以写博客，和好友即时聊天，多人聊天，太好玩了！";
	strBody += "\r\n";
	strBody += "\r\n";
	strBody += "IEC-iM下载链接： http://iecim.feiniu.com";
	strBody += "\r\n";
	strBody += "\r\n";
	strBody += "你也去下载个吧，上来找我，昵称："+CString(g_ImUserInfo.nickname)+" ^_^";
	strBody += "\r\n";
	strBody += "这儿是我的空间，快去踩踩吧！http://"+CString(g_ImUserInfo.GetUserName().c_str())+".feiniu.com";

	
	m_edtMail.SetWindowText(strBody);*/

	m_edtMail.SetFocus();
	m_edtMail.SetSel(-1,-1);
}

void CContactInviteDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

void CContactInviteDlg::Show(CString strEmail,CString strDisplayName)
{
	bool bExist = false;
	int  n = vecInstance.size();

	CContactInviteDlg * pDlg ;

	for ( int i=0; i<n; i++ )
	{
		if ( vecInstance[i]->m_strEmail==strEmail && vecInstance[i]->m_strDisplayName==strDisplayName )
		{
			bExist = true;
			pDlg   = vecInstance[i];
			break;
		}
	}

	if ( bExist )
	{
		if ( pDlg->IsIconic() )
			pDlg->ShowWindow(SW_RESTORE);

		pDlg->BringWindowToTop();
	}
	else
	{
		pDlg = new CContactInviteDlg(GetDesktopWindow());
		pDlg->ShowWindow(SW_SHOW);
		pDlg->SetEmail(strEmail,strDisplayName);
	}
}

void CContactInviteDlg::Close()
{
	int i,n=vecInstance.size();
	vector<CContactInviteDlg *> vecTemp;
	for ( i=0; i<n; i++ )
	{
		vecTemp.push_back(vecInstance[i]);
	}

	for ( i=0; i<n; i++ )
	{
		vecTemp[i]->DestroyWindow();
	}
}

void CContactInviteDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();

	CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE);

	if ( pWnd && pWnd->m_hWnd )
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		dc.FillSolidRect(&rc,0xFFFFFF);
		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

HBRUSH CContactInviteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nCtlID  = pWnd->GetDlgCtrlID();

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);

		if ( nCtlID==IDC_STATIC1 )
		{
			pDC->SetTextColor(RGB(247,148,28));
		}

		if ( nCtlID==IDC_STATIC_INFO )
		{
			hbr = m_hBkGround;
		}
		else
		{
			hbr = g_hWhiteBrush;
		}
	}

	
	return hbr;
}

BOOL CContactInviteDlg::PreTranslateMessage(MSG* pMsg)
{
	if  ( pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case VK_ESCAPE:
			return TRUE;
			break;
		}
	}

	return CXSkinDlg::PreTranslateMessage(pMsg);
}

void CContactInviteDlg::OnClose()
{
	DestroyWindow();
}

void CContactInviteDlg::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	CXSkinDlg::OnDestroy();
	delete this;	
}

void CContactInviteDlg::OnBnClickedInvite()
{
	char buff[1500];
	LPXT_FRIENDINVITE_REQ req = (LPXT_FRIENDINVITE_REQ)buff;

	CString strEmailContent;
	m_edtMail.GetWindowText(strEmailContent);

	if ( m_bCanEditTo )
	{
		m_edtTo.GetWindowText(m_strEmail);

		m_strEmail.Trim();

		if ( m_strEmail=="" || m_strEmail==HINT_STR )
		{
			ShowMessage("请输入联系人地址！谢谢。");
			return ;
		}

		m_strEmail.Replace("，",",");

		int iPos   = m_strEmail.Find(",");
		int iBegin = 0;

		CString strError, strSubEmail;

		while ( iPos>-1 )
		{
			strSubEmail = m_strEmail.Mid(iBegin,iPos-iBegin);

			if ( CheckEmail(strSubEmail,strError)==false )
			{
				ShowMessage("请输入合法的Email地址,多个地址用逗号\",\"隔开,谢谢");	
				return ;
			}

			iBegin = iPos+1;
			iPos = m_strEmail.Find(",",iBegin); 
		}
		
		strSubEmail = m_strEmail.Mid(iBegin,m_strEmail.GetLength()-iBegin);
		
		if ( CheckEmail(strSubEmail,strError)==false )
		{
			ShowMessage("请输入合法的Email地址,多个地址用逗号\",\"隔开,谢谢");
			return ;
		}
	}



	m_edtTitle.GetWindowText(req->emailTitle,sizeof(req->emailTitle)-1);

	strEmailContent = "<html><body bgcolor=#cbebf8><P>" + strEmailContent +"</P></body></html>";

	strEmailContent.Replace("\r\n","<br>");
	strEmailContent.Replace("www.feiniu.com","<a href=\"http://iecim.feiniu.com\">www.feiniu.com</a>");
	strEmailContent.Replace("请访问www.feiniu.com","请访问<a href=\"www.feiniu.com\">www.feiniu.com</a>");

	CString strMySite = "http://"+CString(g_ImUserInfo.GetUserName().c_str())+".feiniu.com";
	CString strLink   = "<a href=\""+strMySite+"\">"+strMySite+"</a>";
	strEmailContent.Replace(strMySite,strLink);


	//
	strncpy(req->emailContent,(char *)(LPCSTR)strEmailContent,sizeof(req->emailContent)-1);
	strncpy(req->emailAddress,(char *)(LPCSTR)m_strEmail,sizeof(req->emailAddress)-1);

	
	m_nSequenceID = g_pProtocol->GetCurSendSequnceId();
	g_pProtocol->SendReq(*req);

	m_btnInvite.EnableWindow(FALSE);

	SetTimer(0x01,350,NULL);
}

void CContactInviteDlg::OnInviteAck(const char *data, WORD wRet)
{
	LPXT_FRIENDINVITE_ACK ack = (LPXT_FRIENDINVITE_ACK)data;

	uint32 nAckSeq = g_pProtocol->GetLastAckSequenceId();

	if ( m_nSequenceID!=nAckSeq )
		return ;

	m_btnInvite.EnableWindow(TRUE);
	KillTimer(0x01);
	SetDlgItemText(IDC_STATIC_INFO,"");
	
	CString str;

	if ( wRet==0 )
	{
		SetDlgItemText(IDC_STATIC_INFO,"邀请邮件发送成功");

		if ( m_strDisplayName=="" )
			str.Format("邀请邮件发送成功!请耐心等待对方的回复！谢谢。");
		else
        	str.Format("邀请邮件成功发送到　%s ,请耐心等待　%s 给您的答复！谢谢．",m_strEmail,m_strDisplayName);
		
		ShowMessage(str,MB_OK|MB_ICONINFORMATION);

		/*CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
		pMainWnd->m_pPage1->m_pageAddbook.OnInviteAck(m_strDisplayName,m_strEmail);*/

		PostMessage(WM_CLOSE);
	}
	else
	if ( wRet==SERVER_TIMEOUT )
	{
		SetDlgItemText(IDC_STATIC_INFO,"邀请邮件发送超时");
		str.Format("邀请邮件成功发送到　%s 超时，多试几次看看．",m_strEmail);
		ShowMessage(str,MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		CString strError = g_pProtocol->GetLastError();
		SetDlgItemText(IDC_STATIC_INFO,strError);
		str.Format("邀请邮件成功发送到　%s 失败，失败原因如下：\n  %s，多试几次看看．",m_strEmail,strError);
		ShowMessage(str,MB_OK|MB_ICONINFORMATION);
	}
}

void CContactInviteDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		static CString strDot="";
		static int i=0;

		if ( i++>2 )
		{
			strDot="";
			i=0;
		}
		else
		{
			strDot+=".";
		}

		if ( m_strDisplayName=="" )
			SetDlgItemText(IDC_STATIC_INFO,"正在发送邮件"+strDot);
		else
			SetDlgItemText(IDC_STATIC_INFO,"正在邀请 "+m_strDisplayName+" 一起使用IEC-iM"+strDot);
	}

	CXSkinDlg::OnTimer(nIDEvent);
}

void CContactInviteDlg::OnEnSetfocusEditTo()
{
	if ( m_bCanEditTo )
	{
		CString s;
		m_edtTo.GetWindowText(s);
		s.Trim();

		if ( s==HINT_STR )
		{
			m_edtTo.SetWindowText("");
			m_edtTo.SetTextColor(0);
		}
	}
}

void CContactInviteDlg::OnEnKillfocusEditTo()
{
	if ( m_bCanEditTo )
	{
		CString s;
		m_edtTo.GetWindowText(s);
		s.Trim();

		if ( s=="" )
		{
			m_edtTo.SetWindowText(HINT_STR);
			m_edtTo.SetTextColor(0xAAAAAA);
		}
	}
}
