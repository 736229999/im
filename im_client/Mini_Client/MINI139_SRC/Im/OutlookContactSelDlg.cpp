// im\OutlookContactSelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\outlookcontactseldlg.h"
#include "..\MainFrame.h"


// COutlookContactSelDlg 对话框
COutlookContactSelDlg * COutlookContactSelDlg::pInstance=NULL;

IMPLEMENT_DYNAMIC(COutlookContactSelDlg, CXSkinDlg)
COutlookContactSelDlg::COutlookContactSelDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(COutlookContactSelDlg::IDD, pParent)
{
	CResBitmap bmp;

	bmp.LoadBitmap(IDB_CONTACT_INVITE);
	m_ImageList.Create(20,20,ILC_COLOR24|ILC_MASK,0,1);
	m_ImageList.Add(&bmp,RGB(0,255,0));
	m_ImageList.SetBkColor(RGB(255,255,255));

	m_nSequenceID = 0;

    Create(COutlookContactSelDlg::IDD,pParent);
}

COutlookContactSelDlg::~COutlookContactSelDlg()
{
	pInstance = NULL;
}

void COutlookContactSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(COutlookContactSelDlg, CXSkinDlg)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_XT_MESSAGE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(COutlookContactSelDlg)
	ON_XT_COMMAND(CMD_FRIENDINVITE_ACK,OnInviteAck)
END_XT_MESSAGE_MAP()

// COutlookContactSelDlg 消息处理程序

BOOL COutlookContactSelDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	CWnd *pWnd = GetDlgItem(IDC_LIST_CONTACTS);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_wndListContacts.Create(this,rect);
	m_wndListContacts.SetDlgCtrlID(0x05400+1);
	m_wndListContacts.SetFolderListStyle(FS_CHECK|FS_IMAGE);
	m_wndListContacts.SetImageList(&m_ImageList);
	m_wndListContacts.SetItemIndent(10);

	m_wndListContacts.AddFolder("Outlook地址簿");

	SetWindowText("邀请使用 Mini");
	SetDlgStyle(MINCLOSE);

	g_pProtocol->RegistCmd(CMD_FRIENDINVITE_ACK,m_hWnd);

	return TRUE;  
}

COutlookContactSelDlg * COutlookContactSelDlg::Show()
{
	if ( pInstance==NULL )
	{
		pInstance = new COutlookContactSelDlg(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
		pInstance->BringWindowToTop();
	}
	else
	{
		if ( pInstance->IsIconic() )
			pInstance->ShowWindow(SW_RESTORE);

		pInstance->BringWindowToTop();
	}

	return pInstance;
}

HBRUSH COutlookContactSelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(IM_LINE_COLOR);
		hbr = m_hBkGround;
	}
	return hbr;
}

void COutlookContactSelDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();

	if ( m_wndListContacts.m_hWnd )
	{
		CRect rect;
		m_wndListContacts.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.InflateRect(1,1);
		dc.Draw3dRect(&rect,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

void COutlookContactSelDlg::AddContact(CString strName, CString strMailAddress)
{
	CString strItem;
	strItem.Format("%s <%s>",strName, strMailAddress);
	int nItem = m_wndListContacts.AddItem(strItem,0,false,0);
	m_mapAddress.insert(map<int,CString>::value_type(nItem,strMailAddress));
}

void COutlookContactSelDlg::OnOK()
{
	int n=m_wndListContacts.GetItemCount();
	int nSel=0;

	m_strEmailAddress="";

	for ( int i=0; i<n; i++ )
	{
		if ( m_wndListContacts.GetItemCheck(i) )
		{
			map<int,CString>::iterator it;
			it=m_mapAddress.find(i);

			CString strEmailAddress;
			
			if ( it!=m_mapAddress.end() )
			{
				strEmailAddress=it->second;
			}

			if ( nSel==0 )
			{
				m_strEmailAddress=strEmailAddress;
			}
			else
			{
				m_strEmailAddress += ",";
				m_strEmailAddress += strEmailAddress;
			}

			nSel++;
		}
	}

	if ( nSel>5 )
	{
		MessageBox("对不起，您最多只能一次邀请5位联系人！而您选中了"+IntToStr(nSel)+"位。","邮件邀请",MB_OK|MB_ICONINFORMATION);
	}
	else
	if ( nSel<1 )
	{
		MessageBox("请至少选择一位联系人。","邮件邀请",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		CString strTitle;
		strTitle = "你好,我是"+CString(g_ImUserInfo.nickname)+",推荐你使用一款不错的软件 - IEC-iM";

		CString strBody;

		strBody  = "Hi! ";
		strBody += "\r\n\r\n";
		strBody += "最近我在iecmall商务网下载了一个软件-IEC-iM，可以一次上传数百上千张图片，速度超快，太爽了！还可以写博客，和好友即时聊天，多人聊天，太好玩了！";
		strBody += "\r\n";
		strBody += "\r\n";
		strBody += "IEC-iM 下载链接： http://iecim.feiniu.com";
		strBody += "\r\n";
		strBody += "\r\n";
		strBody += "你也去下载个吧，上来找我，昵称："+CString(g_ImUserInfo.nickname)+" ^_^";
		strBody += "\r\n";
		strBody += "这儿是我的空间，快去踩踩吧！http://"+CString(g_ImUserInfo.GetUserName().c_str())+".feiniu.com";
		
		char buff[1500];
		LPXT_FRIENDINVITE_REQ req = (LPXT_FRIENDINVITE_REQ)buff;

		strBody = "<html><body bgcolor=#cbebf8><P>" + strBody +"</P></body></html>";

		strBody.Replace("\r\n","<br>");
		strBody.Replace("http://www.feiniu.com","<a href=\"http://iecim.feiniu.com\">http://www.feiniu.com</a>");
		strBody.Replace("请访问www.feiniu.com","请访问<a href=\"http://www.feiniu.com/\">www.feiniu.com</a>");
		
		CString strMySite = "http://"+CString(g_ImUserInfo.GetUserName().c_str())+".feiniu.com";
		CString strLink   = "<a href=\""+strMySite+"\">"+strMySite+"</a>";
		strBody.Replace(strMySite,strLink);

		strncpy(req->emailContent,(char *)(LPCSTR)strBody,sizeof(req->emailContent)-1);
		strncpy(req->emailAddress,(char *)(LPCSTR)m_strEmailAddress,sizeof(req->emailAddress)-1);
		strncpy(req->emailTitle,(char *)(LPCSTR)strTitle,sizeof(req->emailTitle)-1);

		m_nSequenceID = g_pProtocol->GetCurSendSequnceId();

		SetTimer(0x01,350,NULL);
		g_pProtocol->SendReq(*req);

		m_btnOk.EnableWindow(FALSE);
		m_wndListContacts.EnableWindow(FALSE);
	}
}

void COutlookContactSelDlg::OnCancel()
{
	DestroyWindow();
}

void COutlookContactSelDlg::OnClose()
{
	DestroyWindow();
}

void COutlookContactSelDlg::OnInviteAck(const char *data, WORD wRet)
{
	LPXT_FRIENDINVITE_ACK ack = (LPXT_FRIENDINVITE_ACK)data;

	uint32 nAckSeq = g_pProtocol->GetLastAckSequenceId();

	if ( m_nSequenceID!=nAckSeq )
		return ;

	m_btnCancel.EnableWindow(TRUE);
	m_btnOk.EnableWindow(TRUE);
	m_wndListContacts.EnableWindow(TRUE);

	KillTimer(0x01);
	SetDlgItemText(IDC_STATIC_INFO,"");
	
	CString str;

	if ( wRet==0 )
	{
		int n=m_wndListContacts.GetItemCount();
		for ( int i=0; i<n; i++ )
		{
			if ( m_wndListContacts.GetItemCheck(i) )
			{
				m_wndListContacts.SetItemCheck(i,false);
				m_wndListContacts.SetItemImage(i,1);

				map<int,CString>::iterator it;
				it=m_mapAddress.find(i);

				CString strEmailAddress;

				if ( it!=m_mapAddress.end() )
				{
					strEmailAddress=it->second;
				}

				CString strName;

				CString strItemText=m_wndListContacts.GetItemText(i);
				int l=strItemText.GetLength();
				l--;

				while(l>-1 && strItemText[l]!='<' )
				{
					l--;
				}

				strName = strItemText.Left(l);
				strName.Trim();

				/*CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
				pMainWnd->m_pPage1->m_pageAddbook.OnInviteAck(strName,strEmailAddress);*/

			}
		}

		SetDlgItemText(IDC_STATIC_INFO,"邀请邮件发送成功");
		ShowMessage("邀请邮件已成功发送，请耐心等待答复！谢谢．",MB_OK|MB_ICONINFORMATION);
		PostMessage(WM_CLOSE);
	}
	else
	if ( wRet==SERVER_TIMEOUT )
	{
		SetDlgItemText(IDC_STATIC_INFO,"邀请邮件发送超时");
		ShowMessage("邀请邮件发送超时",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		CString strError = g_pProtocol->GetLastError();
		SetDlgItemText(IDC_STATIC_INFO,strError);
		str.Format("邀请邮件成功发送失败，失败原因如下：\n  %s，多试几次看看．",strError);
		ShowMessage(str,MB_OK|MB_ICONINFORMATION);
	}
}


void COutlookContactSelDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		static int i=0;
		static CString strDot="";

		if ( i++>2 )
		{
			i=0;
			strDot="";
		}
		else
		{
			strDot+=".";
		}

		SetDlgItemText(IDC_STATIC_INFO,"正在发送邀请邮件"+strDot);
	}
	
	CXSkinDlg::OnTimer(nIDEvent);
}

void COutlookContactSelDlg::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	CXSkinDlg::OnDestroy();
	delete this;
}
