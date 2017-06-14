// im\SMSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include "SMSDlg.h"
#include ".\smsdlg.h"
#include "..\friendinfodlg.h"
#include "localdata.h"
#include "MessageHandleWnd.h"
CSMSDlg * CSMSDlg::pInstance = NULL;
// CSMSDlg 对话框

IMPLEMENT_DYNAMIC(CSMSDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CSMSDlg)

CSMSDlg::CSMSDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CSMSDlg::IDD, pParent)
{
	m_FontSMS.CreateFont(14, 
		0,
		0, 
		90,
		FW_NORMAL,
		0,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		(DEFAULT_PITCH|FF_MODERN),
		"宋体");

	m_bRecvSMS = false;

	Create(CSMSDlg::IDD,pParent);
}

CSMSDlg::~CSMSDlg()
{
	pInstance = NULL;
}

void CSMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_SMS,m_btnHistory);
	DDX_Control(pDX,IDC_STATIC_INFO,m_btnInfo);
	DDX_Control(pDX,IDC_STATIC_HEAD,m_wndFaceCtrl);
	DDX_Control(pDX,IDC_EDIT_SMS,m_edtSMS);
	DDX_Control(pDX,IDOK,m_btnSend);
	DDX_Control(pDX,IDCANCEL,m_btnClose);
	DDX_Control(pDX,IDC_LIST_SMS,m_wndListSMS);
	DDX_Control(pDX,IDC_EDIT_SHOWSMS,m_edtSMSShow);
}


BEGIN_MESSAGE_MAP(CSMSDlg, CXSkinDlg)
	ON_WM_CLOSE()
	SET_HMG_SCROLLBAR(IDC_EDIT_SMS)
	SET_HMG_SCROLLBAR(IDC_LIST_SMS)
	SET_HMG_SCROLLBAR(IDC_EDIT_SHOWSMS)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SMS, OnBnClickedButtonSms)
	ON_STN_CLICKED(IDC_STATIC_INFO, OnStnClickedStaticInfo)
	ON_EN_KILLFOCUS(IDC_EDIT_SMS, OnEnKillfocusEditSms)
	ON_EN_SETFOCUS(IDC_EDIT_SMS, OnEnSetfocusEditSms)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_XT_MESSAGE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SMS, OnLvnItemchangedListSms)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListSms)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CSMSDlg)
	ON_XT_COMMAND(CMD_SMS_ACK,OnSMSAck)
END_XT_MESSAGE_MAP()

// CSMSDlg 消息处理程序
CSMSDlg * CSMSDlg::Show(FRIEND_INFO * pFriend)
{
	if ( pInstance )
	{
		if ( pInstance->m_FriendInfo.GetId()!=pFriend->GetId() )
		{
			pInstance->m_FriendInfo = * pFriend;
			pInstance->Init();
		}	

		if ( pInstance->IsIconic() )
		{
			pInstance->ShowWindow(SW_RESTORE);
		}
	}
	else
	{
		pInstance = new CSMSDlg(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
		pInstance->m_FriendInfo = * pFriend;
		pInstance->Init();
	}

	pInstance->BringWindowToTop();
	return pInstance;	
}

void CSMSDlg::ShowRecvSMS(tagSMSItem si)
{
	m_bRecvSMS = true;
	m_btnSend.SetWindowText("回复");

	m_edtSMS.SetWindowText(si.message);
	m_edtSMS.SetTextColor(RGB(0,0,0));
	m_edtSMS.SetReadOnly(TRUE);

	KillTimer(0x01);

	CString strTitle;
	strTitle.Format("收到%s发给你的手机短信",m_FriendInfo.nickname);
	SetWindowText(strTitle);

	SetDlgItemText(IDC_STATIC_NUM,CString("发信日期: ")+si.szTime);
}

void CSMSDlg::Close()
{
	if ( pInstance )
	{
		pInstance->DestroyWindow();
	}
}

void CSMSDlg::UpdateSMSList()
{
	m_wndListSMS.DeleteAllItems();
	m_smsList.clear();
	g_LocalData.GetMobileSMSList(m_FriendInfo.GetId(),m_smsList);

	int n = m_smsList.size();

	for ( int i=0; i<n; i++ )
	{
		CString strTime = m_smsList[i].szTime;
		strTime.Replace("/","-");

		int nItem = m_wndListSMS.InsertItem(i,strTime);
		if (m_smsList[i].nType==0)
		{
			m_wndListSMS.SetItemText(nItem,1,"发送");
			m_wndListSMS.SetItemText(nItem,2,m_smsList[i].message);

			m_wndListSMS.SetItemBackColor(nItem,0,RGB(255,233,222));
			m_wndListSMS.SetItemBackColor(nItem,1,RGB(255,233,222));
			m_wndListSMS.SetItemBackColor(nItem,2,RGB(255,233,222));
		}
		else
		{
			m_wndListSMS.SetItemText(nItem,1,"接收");
			m_wndListSMS.SetItemText(nItem,2,m_smsList[i].message);

			m_wndListSMS.SetItemBackColor(nItem,0,RGB(222,233,255));
			m_wndListSMS.SetItemBackColor(nItem,1,RGB(222,233,255));
			m_wndListSMS.SetItemBackColor(nItem,2,RGB(222,233,255));
		}
	}
}

void CSMSDlg::Init()
{
	m_wndFaceCtrl.SetUserInfo(&m_FriendInfo);
	
	CString strTitle;
	strTitle.Format("发送手机短信给%s",m_FriendInfo.nickname);

	SetWindowText(strTitle);
	SetDlgItemText(IDC_STATIC_NICKNAME,m_FriendInfo.nickname);
	//SetDlgItemText(IDC_STATIC_MOBILE,CString("绑定手机号： ")+m_FriendInfo.mobile);

	m_edtSMS.Clear();

	m_btnSend.SetFocus();
	OnEnKillfocusEditSms();

	UpdateSMSList();
}

void CSMSDlg::SetStyle(int nStyle)
{
	int left, top;

	CRect rc;
	GetWindowRect(&rc);

	left = rc.left;
	top  = rc.top;

	if ( nStyle==0 )//带历史消息列表模式
	{
		MoveWindow(left,top,344,468);

		CRect rcInvalidate;
		rcInvalidate.left=0;
		rcInvalidate.top   = 248;
		rcInvalidate.right = 344;
		rcInvalidate.bottom=468;

		InvalidateRect(&rcInvalidate);	
	}
	else
	if ( nStyle==1 )
	{
		MoveWindow(left,top,344,250);

		CRect rcInvalidate;
		rcInvalidate.left=0;
		rcInvalidate.top   = 245;
		rcInvalidate.right = 344;
		rcInvalidate.bottom=250;

		InvalidateRect(&rcInvalidate);	
	}
}


void CSMSDlg::OnClose()
{
	DestroyWindow();
}

void CSMSDlg::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);

	CXSkinDlg::OnDestroy();
	delete pInstance;
}

BOOL CSMSDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	m_btnHistory.SetBitmap(IDB_SMS_BTN2,IDB_SMS_BTN2,IDB_SMS_BTN1,IDB_SMS_BTN1);
	AttachHMGScrollbar(m_edtSMS.m_hWnd);
	AttachHMGScrollbar(m_wndListSMS.m_hWnd);
	AttachHMGScrollbar(m_edtSMSShow.m_hWnd);

	SetDlgStyle(MINCLOSE);
	SetStyle(1);

	SetTimer(0x01,500,NULL);

	m_edtSMS.SetFont(&m_FontSMS);

	m_wndListSMS.InsertColumn(0,"日期",LVCFMT_LEFT,110);
	m_wndListSMS.InsertColumn(1,"类型",LVCFMT_LEFT,38);
	m_wndListSMS.InsertColumn(2,"内容",LVCFMT_LEFT,145);
	m_wndListSMS.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_wndListSMS.EnableToolTips(TRUE);

	g_pProtocol->RegistCmd(CMD_SMS_ACK,m_hWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSMSDlg::OnBnClickedCancel()
{
	DestroyWindow();
}

void CSMSDlg::OnBnClickedOk()
{
	if ( m_bRecvSMS )
	{
		m_edtSMS.SetWindowText("");
		m_edtSMS.SetReadOnly(FALSE);
		SetTimer(0x01,500,NULL);
		m_btnSend.SetWindowText("发送");
		m_edtSMS.SetFocus();

		CString strTitle;
		strTitle.Format("发送手机短信给%s",m_FriendInfo.nickname);

		m_bRecvSMS = false;
	}
	else
	{
		CString s;
		m_edtSMS.GetWindowText(s);

		CString sTip;

		if ( s!="\r\n     在这里输入短信内容。\r\n         提示：最多70字。" )
		{
			bool bFindChinese = false;
			int  l = s.GetLength();
			for ( int i=0; i<l; i++ )
			{
				if ( s[i]>125 || s[i]<0 )
				{
					bFindChinese=true;
					break;
				}
			}

			BSTR bstr1 = s.AllocSysString();
			int  nLen  = wcslen(bstr1);

			if ( nLen>70 )
			{
				ShowMessage("对不起，短信内容过长！",MB_OK|MB_ICONINFORMATION);
			}
			else
			{
				KillTimer(0x01);
				SetDlgItemText(IDC_STATIC_NUM,"正在发送手机短信给对方...");
				m_edtSMS.SetReadOnly(TRUE);
				m_btnSend.EnableWindow(FALSE);

				XT_SMS_REQ req;

				req.biz_type = 400;
				strcpy(req.link_id , "");
				strcpy(req.message,(LPCSTR)s);
				strcpy(req.mobile,this->m_FriendInfo.mobile);
				req.recv_id  = m_FriendInfo.GetId();
				req.msg_type = 0;
				
				g_pProtocol->SendReq(req);
			}

		//	::SysFreeString(bstr1);
		}
		else
		{
			ShowMessage("请输入短信内容。",MB_OK|MB_ICONINFORMATION);
		}
	}

}

HBRUSH CSMSDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();


	if ( nCtlColor==CTLCOLOR_STATIC && nID!=IDC_EDIT_SMS && nID!=IDC_EDIT_SHOWSMS )
	{	
		pDC->SetBkMode(TRANSPARENT);
		hbr = g_hBackBrush;

		if ( nID==IDC_STATIC_MOBILE )
		{
			pDC->SetTextColor(RGB(255,0,0));
		}
	}

	return hbr;
}

void CSMSDlg::OnBnClickedButtonSms()
{
	if ( m_btnHistory.IsDown()==false )
	{
		SetStyle(0);
		m_btnHistory.SetDown(true);
	}
	else
	{
		SetStyle(1);
		m_btnHistory.SetDown(false);
	}
}

void CSMSDlg::OnStnClickedStaticInfo()
{
	CFriendInfoDlg::NewDlg(&m_FriendInfo);
}

void CSMSDlg::OnEnKillfocusEditSms()
{
	CString s;
	m_edtSMS.GetWindowText(s);
	s.Trim();

	if ( s=="" )
	{
		m_edtSMS.SetTextColor(0xAAAAAA);
		m_edtSMS.SetWindowText("\r\n     在这里输入短信内容。\r\n         提示：最多70字。");   
	}
}

void CSMSDlg::OnEnSetfocusEditSms()
{
	CString s;
	m_edtSMS.GetWindowText(s);
	s.Trim();

	if ( s=="在这里输入短信内容。\r\n         提示：最多70字。" )
	{
		m_edtSMS.SetTextColor(RGB(0,0,255));
		m_edtSMS.SetWindowText("");   
	}
}

void CSMSDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CString s;
		m_edtSMS.GetWindowText(s);

		CString sTip;

		if ( s!="\r\n     在这里输入短信内容。\r\n         提示：最多70字。" )
		{

			bool bFindChinese = false;
			int  l = s.GetLength();
			for ( int i=0; i<l; i++ )
			{
				if ( s[i]>125 || s[i]<0 )
				{
					bFindChinese=true;
					break;
				}
			}

			BSTR bstr1 = s.AllocSysString();
			int  nLen  = wcslen(bstr1);

			::SysFreeString(bstr1);
		
			sTip.Format("字符数:%d,最多不超过70",nLen);

		}
		else
		{
			sTip = "字符数:0";
		}

		SetDlgItemText(IDC_STATIC_NUM,sTip);
	}
	
	CXSkinDlg::OnTimer(nIDEvent);
}

void CSMSDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	m_wndListSMS.GetWindowRect(&rc);
	ScreenToClient(&rc);

	rc.InflateRect(1,1);
	rc.right++;
	rc.bottom++;
	dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	DrawCaptionBar();
	DrawBorder();
}

void CSMSDlg::OnSMSAck(const char *data, WORD wRet)
{
	m_edtSMS.SetReadOnly(FALSE);
	m_btnSend.EnableWindow(TRUE);
	
	SetTimer(0x01,500,NULL);

	uint8 ok_code = (uint8)data;

	if ( wRet!=0 )
	{
		ShowMessage(g_pProtocol->GetLastError());
	}
	else
	{
		CString strSms;
		m_edtSMS.GetWindowText(strSms);

		tagSMSItem item;
		strcpy(item.message,(LPCSTR)strSms);
		item.nType  = 0;//send


		CTime t = CTime::GetCurrentTime();

		strcpy(item.szTime,(LPCSTR)t.Format("%Y-%m-%d %H:%M:%S"));

		g_LocalData.AddMobileSMS(m_FriendInfo.GetId(),item);

		m_edtSMS.SetWindowText("");
		m_edtSMS.SetFocus();

		if ( ok_code==0 )
            ShowMessage("手机短信发送成功！");
		else
			ShowMessage(g_pProtocol->GetLastError());

		UpdateSMSList();
	}
}

void CSMSDlg::OnBnClickedButtonExport()
{
	CFileDialog dlg(false,"txt","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"文本文件(*.txt)|*.txt||",this);

	if ( dlg.DoModal()==IDOK )
	{
		CString strFile = dlg.GetPathName();

		CStdioFile objFile;

		if ( objFile.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			CString strTitle;
			strTitle.Format("与 %s 的短信记录",m_FriendInfo.nickname);

			objFile.WriteString(strTitle);
			objFile.WriteString("\r\n");
			objFile.WriteString("\r\n");


			for ( int i=0; i<m_smsList.size(); i++ )
			{
				objFile.WriteString(m_smsList[i].szTime);
				objFile.WriteString("\r\n");

				if ( m_smsList[i].nType==0 )
					objFile.WriteString("发送");
				else
					objFile.WriteString("接收");
				objFile.WriteString("\r\n");

				objFile.WriteString(m_smsList[i].message);
				objFile.WriteString("\r\n");
				objFile.WriteString("\r\n");
			}

			objFile.Close();
		}


		ShellExecute(NULL,"open","notepad.exe",strFile,"",SW_SHOW);
	}
}

void CSMSDlg::OnBnClickedButtonClear()
{
	CString strTip;
	strTip.Format("确定要删除你和%s的手机短信纪录么?",m_FriendInfo.nickname);

	if ( CMessageBox::Model(GetSafeHwnd(),strTip,"Mini",MB_YESNO|MB_ICONQUESTION)==IDYES )
	{
		g_LocalData.DeleteSMSFile(m_FriendInfo.GetId());
		UpdateSMSList();
	}
}

void CSMSDlg::OnLvnItemchangedListSms(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	
	int nCount = m_wndListSMS.GetItemCount();

	for ( int nItem=0; nItem<nCount; nItem++ )
	{
		if ( m_wndListSMS.GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED )
		{
			SetDlgItemText(IDC_EDIT_SHOWSMS,m_smsList[nItem].message);
			break;
		}
	}

	*pResult = 0;
}

void CSMSDlg::OnHdnItemclickListSms(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);


	*pResult = 0;
}
