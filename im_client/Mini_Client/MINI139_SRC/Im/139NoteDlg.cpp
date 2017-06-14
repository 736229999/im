// im\139NoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include ".\139notedlg.h"
#include "resgdiobj.h"
#include "localdata.h"
#include "UserAuthInfoDlg.h"
#include "UserAuthDlg.h"
#include "..\MainFrame.h"

#define   IDM_CLEAR  1001
#define   IDM_DELETE 1002

static const COLORREF COLOR_READ=RGB(166,0,0);

IMPLEMENT_DYNAMIC(C139NoteEditCtrl,CRichEditCtrl)

C139NoteEditCtrl::C139NoteEditCtrl()
{
	m_pXFont = NULL;
}

C139NoteEditCtrl::~C139NoteEditCtrl()
{
	if ( m_pXFont )
		delete m_pXFont;
}

BEGIN_MESSAGE_MAP(C139NoteEditCtrl, CRichEditCtrl)
	ON_NOTIFY_REFLECT(EN_LINK, OnEnLink)
END_MESSAGE_MAP()

//设置文本块缩进
void C139NoteEditCtrl::SetStartIndent(int nIndent)
{
	PARAFORMAT2 pf;

	GetParaFormat(pf);

	pf.dwMask = 0;
	pf.dwMask |= PFM_ALIGNMENT;
	pf.wAlignment = PFA_LEFT;
	pf.dwMask |= PFM_RIGHTINDENT;
	pf.dwMask |= PFM_STARTINDENT;
	pf.dwMask |= PFM_OFFSET;

	int nRight=0;
	int nFirst=0;
	int nLeft=nIndent;

	pf.dxRightIndent = nRight;
	pf.dxOffset = -nFirst;
	pf.dxStartIndent = nLeft + nFirst;

	this->SetParaFormat(pf);
}

void C139NoteEditCtrl::InsertText(CString text,const XT_FONT& xf)
{
//	int iPos;

	CHARRANGE cr;

	CHARRANGE crBefore;
	GetSel(crBefore);

	ReplaceSel(text);

	SetSel(-1,-1);
	CHARRANGE crEnd;
	GetSel(crEnd);

	cr.cpMin = crBefore.cpMin;
	cr.cpMax = crEnd.cpMin;
	//cr.cpMax = cr.cpMax+crEnd.cpMax-crBefore.cpMax;
	SetSel(cr);
	SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	SetSel(cr.cpMax,cr.cpMax);
}

void C139NoteEditCtrl::InsertText(CString text )
{
	ReplaceSel(text);
}

void C139NoteEditCtrl::SetXFont(const XT_FONT &xf)
{
	CHARFORMAT2 cf=CRichEditCtrlEx::XFont2CharFormat(xf);

	CHARRANGE crPrev;
	GetSel(crPrev);
	SetSel(0,-1);
	SetSelectionCharFormat(cf);
	SetSel(crPrev);

	if ( m_pXFont!=NULL )
		delete m_pXFont;

	m_pXFont = new XT_FONT;
	memcpy(m_pXFont,&xf,sizeof(XT_FONT));


	SetDefaultCharFormat(cf);
}

void C139NoteEditCtrl::SetXFont(const XT_FONT &xf, long nStart, long nEnd)
{
	CHARRANGE crPrev;
	GetSel(crPrev);
	SetSel(nStart,nEnd);
	SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	SetSel(crPrev);
}

CHARRANGE C139NoteEditCtrl::InsertLink(CString strTextLink)
{
	CHARFORMAT cfPrev;
	cfPrev.dwMask=0xFFFFFFFF;
	GetDefaultCharFormat(cfPrev);

	SetSel(-1, -1);

	InsertText("");

	CHARRANGE cr;
	GetSel(cr);


	InsertText(strTextLink);
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_LINK | CFM_PROTECTED;
	cf.dwEffects = CFE_LINK | CFE_PROTECTED;

	int ansi_len = strTextLink.GetLength();

	WCHAR wszText[255];
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)strTextLink,-1, wszText,255);

	int w_len =
	wcslen(wszText);

	cr.cpMin = cr.cpMax ;
	cr.cpMax = cr.cpMin + w_len ;

	InsertText(" ");

	SetSel(cr.cpMin,cr.cpMax);
	SetSelectionCharFormat(cf);

	SetSel(cr.cpMax+1, -1);
	cf.dwMask=0xFFFFFFFF;
	SetSelectionCharFormat(cfPrev);

	return cr ;
}

void C139NoteEditCtrl::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
	ENLINK *link = reinterpret_cast<ENLINK *>(pNMHDR);

	if (link->msg == WM_LBUTTONUP) {
		CHARRANGE cr;
		GetSel(cr);
		SetSel(link->chrg);

		static CString url = "";
		
		url = GetSelText();
		url.Trim();
		SetSel(cr);

		url.MakeLower();

		if ( url.Find("://")>-1 || url.Find("http")>-1 || url.Find(".")>-1 )
		{
			MiniOpenUrl(url);
		}
		else
		{
			if ( url=="这里" )
			{
				FRIEND_INFO *pFriend = FindFriend(m_senderId);

				if ( pFriend )
				{
					if ( pFriend->GetId()==g_ImUserInfo.GetId() )
					{
						ShowMessage("您已经是自己的朋友了！");
						return ;
					}
					else
					if  ( (pFriend->group_index != GROUPID_STRANGER) && (pFriend->group_index != GROUPID_BLACKLIST) )
					{
						CString str;
						str.Format("%s 已经在你的好友中！" , UserInfoToStr(pFriend));
						ShowMessage(str);
						return ;
					}
				
					if ( pFriend->rule==2 )
					{
						CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(this,GetDesktopWindow());
						// 请求添加到好友组0中
						pDlg->SetUserAuthInfo(pFriend, 2, "",0);
						pDlg->ShowWindow(SW_SHOW);
						return;
					}
					else
					if ( pFriend->rule==1 )
					{
						pFriend->bRefreshed=true;
						// 请求添加到好友组0中
						CUserAuthReqDlg::Show(pFriend,0);
						return ;
					}
				
				}
				else
				{
					XT_FRIENDGROUP_MOD_REQ req;

					req.action = GROUP_ADD;
					req.uid    = g_ImUserInfo.GetId();
					req.fid    = m_senderId;
					req.group_id = 0;

					g_pProtocol->SendReq(req);

					RegUserOperation(0x00c1);

					CString strInfo;
					strInfo.Format("好友添加请求已经发送出去！ 请等待服务器的返回！");

					ShowMessage(strInfo);
				}
			}
		}
	}
	*pResult = 0;
}

static bool compareR(const TWebNotificationInfo * info1, const TWebNotificationInfo * info2 )
{
	if ( info1->is_read < info2->is_read )
	{
		return true;
	}
	else
	{
		return false;
	}
}

static bool compareN(const TWebNotificationInfo * info1, const TWebNotificationInfo * info2 )
{
	if ( info1->dwCreateDate>info2->dwCreateDate )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// C139NoteDlg 对话框
C139NoteDlg * C139NoteDlg::pInstance = NULL;
vector<TWebNotificationInfo *> C139NoteDlg::vecNotification;
int C139NoteDlg::m_nFreshMessage=0;

IMPLEMENT_DYNAMIC(C139NoteDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(C139NoteDlg)

C139NoteDlg::C139NoteDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(C139NoteDlg::IDD, pParent)
{	
	Create(C139NoteDlg::IDD,pParent);
}

C139NoteDlg::~C139NoteDlg()
{
	pInstance = NULL;
}

void C139NoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_RE_MESSAGE, m_reMessage);
	DDX_Control(pDX, IDC_HISTORY, m_btnHistory);
	DDX_Control(pDX, IDC_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
}


BEGIN_MESSAGE_MAP(C139NoteDlg, CXSkinDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HISTORY, OnBnClickedHistory)
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnListItemChanged)
	SET_HMG_SCROLLBAR(IDC_RE_MESSAGE)
	SET_HMG_SCROLLBAR(IDC_LIST1)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_WM_PAINT()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnNMRclickList1)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnLvnKeydownList1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// C139NoteDlg 消息处理程序

BOOL C139NoteDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetTitleIcon(GetResDLLHandle(),IDI_139NOTE);
	SetDlgStyle(MINCLOSE);

	//消息330x145
	m_wndList.InsertColumn(0,"日期",LVCFMT_LEFT,108);
	m_wndList.InsertColumn(1,"类型",LVCFMT_LEFT,60);
	m_wndList.InsertColumn(2,"内容",LVCFMT_LEFT,227);
	m_wndList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_btnHistory.SetBitmap(IDB_NOTE_BTN1,IDB_NOTE_BTN1,IDB_NOTE_BTN2,IDB_NOTE_BTN2);
//	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	AttachHMGScrollbar(this->m_wndList.m_hWnd);
	AttachHMGScrollbar(this->m_reMessage.m_hWnd);

	SetStyle(1);

	return TRUE; 
}

void C139NoteDlg::ResetContent()
{
	if ( pInstance )
	{
		pInstance->m_wndList.DeleteAllItems();
	}

	for ( int i=0; i<vecNotification.size(); i++ )
	{
		delete vecNotification[i];
	}
	vecNotification.clear();

}

void C139NoteDlg::Show()
{
	if ( pInstance==NULL )
	{
		pInstance = new C139NoteDlg(GetDesktopWindow());
		pInstance->CenterWindow(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if (pInstance->IsIconic())
		{
			pInstance->ShowWindow(SW_RESTORE);
		}

		pInstance->BringWindowToTop();
	}

	pInstance->InitContent();
}

void C139NoteDlg::Close()
{
	g_LocalData.SaveWebNotifications(vecNotification);

	for ( int i=0; i<vecNotification.size(); i++ )
	{
		delete vecNotification[i];
	}
	vecNotification.clear();

	if ( pInstance )
	{
		pInstance->DestroyWindow();
	}
}

void C139NoteDlg::OnClose()
{
	DestroyWindow();
}

void C139NoteDlg::OnDestroy()
{	
	::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_139WEB_NOTIFY,2,0);

	CXSkinDlg::OnDestroy();
	delete pInstance;
}

void C139NoteDlg::SetStyle(int nStyle)
{
	int left, top;

	CRect rc;
	GetWindowRect(&rc);

	left = rc.left;
	top  = rc.top;

	if ( nStyle==0 )//带历史消息列表模式
	{
		MoveWindow(left,top,420,428);

		CRect rcInvalidate;
		rcInvalidate.left=0;
		rcInvalidate.top = 235;
		rcInvalidate.right=420;
		rcInvalidate.bottom=428;

		InvalidateRect(&rcInvalidate);	

	}
	else
	if ( nStyle==1 )
	{
		MoveWindow(left,top,420,244);
	}
}

LRESULT C139NoteDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	InitContent();
	return 0;
}

void C139NoteDlg::DisplaySysMsg(const TWebNotificationInfo *info)
{
	XT_FONT		xf;
	CHARRANGE	cr;

	m_reMessage.InsertText(info->szTitle);

	m_reMessage.SetSel(-1,-1);
	m_reMessage.GetSel(cr);

	m_reMessage.InsertText("\n");

	xf.fontColor = RGB(80,80,80);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);

	m_reMessage.SetStartIndent(330);

	m_reMessage.InsertText(TrimHtmlCode(info->szContent));

	m_reMessage.GetSel(cr);
	m_reMessage.InsertText("\n\n");
}

void C139NoteDlg::DisplayFriendMsg(const TWebNotificationInfo *info)
{
	XT_FONT		xf;
	CHARRANGE	cr;

	CString str;
	str.Format("%s 发给你的站内消息:",info->szNickName);		
	m_reMessage.InsertText(str);

	m_reMessage.SetSel(-1,-1);
	m_reMessage.GetSel(cr);

	m_reMessage.InsertText("\n");

	xf.fontColor = RGB(80,80,80);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);

	m_reMessage.SetStartIndent(330);

	m_reMessage.InsertText(TrimHtmlCode(info->szContent));

	m_reMessage.GetSel(cr);
	m_reMessage.InsertText("\n\n");

	xf.fontColor = RGB(0,0,0);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);
	m_reMessage.SetStartIndent(230);
	str.Format("点击这里查看 %s 的个人主页:\n",info->szNickName);
	m_reMessage.InsertText(str);
	str.Format("http://%s.feiniu.com",info->szUserName);
	m_reMessage.InsertLink(str);
}

void C139NoteDlg::DisplayMsgBox(const TWebNotificationInfo *info)
{
	XT_FONT		xf;
	CHARRANGE	cr;
//2--加为好友 3--加入群邀请  4--客服消息	5--申请加为好友
	switch(info->sub_type)
	{
	case 0://站内消息
		{
			CString strTitle;
			strTitle.Format("\"%s\" 发给你的站内消息:",info->szNickName);
			m_reMessage.InsertText(strTitle);
			m_reMessage.SetSel(-1,-1);
			m_reMessage.GetSel(cr);

			m_reMessage.InsertText("\n");

			xf.fontColor = RGB(80,80,80);
			m_reMessage.SetXFont(xf,cr.cpMax,-1);

			m_reMessage.SetStartIndent(330);

			m_reMessage.InsertText(TrimHtmlCode(info->szContent));

			m_reMessage.GetSel(cr);
			m_reMessage.InsertText("\n\n");
			

			m_reMessage.SetStartIndent(230);

			xf.fontColor = RGB(0,0,0);
			m_reMessage.SetXFont(xf,cr.cpMax,-1);

			m_reMessage.ReplaceSel("收件箱: ");

			CString strReplay;
			//if ( info->user_id )
			//	strReplay.Format("http://%s.feiniu.com/myzone/message/write.php?uid=%u",g_ImUserInfo.GetUserName().c_str(),info->user_id);
			//else
			//	strReplay.Format("http://%s.feiniu.com/myzone/message/write.php",g_ImUserInfo.GetUserName().c_str());
			strReplay.Format("http://%s.feiniu.com/myzone/message/in.php",g_ImUserInfo.GetUserName().c_str());
            	
			m_reMessage.ReplaceSel(strReplay);
		}
		break;
	case 2://加好友
		{
			CString strTitle;
			strTitle.Format("\"%s\" 已经加你为好友了",info->szNickName);
			m_reMessage.InsertText(strTitle);
			m_reMessage.SetSel(-1,-1);
			m_reMessage.GetSel(cr);

			m_reMessage.InsertText("\n");


			m_reMessage.SetStartIndent(230);

			xf.fontColor = RGB(0,0,0);
			m_reMessage.SetXFont(xf,cr.cpMax,-1);

			if(g_pWebConfig)
			{
				m_reMessage.ReplaceSel("主页:");
				m_reMessage.ReplaceSel((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(info->szUserName)));
			}

		}
		break;
	default:
		{
			CString strTitle;
			strTitle.Format("%s:",info->szNickName);
			m_reMessage.InsertText(strTitle);
			m_reMessage.SetSel(-1,-1);
			m_reMessage.GetSel(cr);

			m_reMessage.InsertText("\n");

			xf.fontColor = RGB(80,80,80);
			m_reMessage.SetXFont(xf,cr.cpMax,-1);

			m_reMessage.SetStartIndent(330);

			m_reMessage.InsertText(TrimHtmlCode(info->szContent));

			m_reMessage.GetSel(cr);
			m_reMessage.InsertText("\n\n");

		
			m_reMessage.SetStartIndent(230);

			xf.fontColor = RGB(0,0,0);
			m_reMessage.SetXFont(xf,cr.cpMax,-1);

			m_reMessage.ReplaceSel("收件箱: ");

			CString strReplay;
			strReplay.Format("http://%s.feiniu.com/myzone/message/in.php",g_ImUserInfo.GetUserName().c_str());
            	
			m_reMessage.ReplaceSel(strReplay);
		}
		break;
	}

}

void C139NoteDlg::DisplayMainPageComment(const TWebNotificationInfo *info)
{
	XT_FONT		xf;
	CHARRANGE	cr;

	CString str;

	bool bWokuUser=false;

	if ( strcmp(info->szUserName,"")==0 || strcmp(info->szNickName,"")==0 )
		bWokuUser=true;

	if ( bWokuUser==false )
		str.Format("\"%s\"对你的个人主页留言:",info->szNickName);
	else
		str = "宝润商友对你的个人主页留言:";

	m_reMessage.InsertText(str);	

	xf.fontColor = RGB(80,80,80);
	m_reMessage.GetSel(cr);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);
	m_reMessage.SetStartIndent(460);


	m_reMessage.ReplaceSel(TrimHtmlCode(info->szContent));
	m_reMessage.ReplaceSel("\n\n");

	xf.fontColor = RGB(0,0,0);
	m_reMessage.GetSel(cr);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);

	m_reMessage.SetStartIndent(230);
	m_reMessage.ReplaceSel("点击这里查看:");
	m_reMessage.ReplaceSel("\n");
	m_reMessage.SetStartIndent(230);
	if(g_pWebConfig)
	{
		m_reMessage.ReplaceSel((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(g_ImUserInfo.GetUserName().c_str())));
	}

}

void C139NoteDlg::DisplayBlogContentComment(const TWebNotificationInfo *info)
{
	XT_FONT		xf;
	CHARRANGE	cr;

	CString strTitle;
	CString strBlogUrl;
	CString strContentType;

	switch(info->sub_type)//0文章评论，1图片评论，3音频评论，4视频评论
	{
	case 0:
		strContentType = "文章";
		strBlogUrl.Format("http://%s.feiniu.com/article/%u.html",g_ImUserInfo.GetUserName().c_str(),info->log_id);
		break;
	case 1:
		strContentType = "图片";
		strBlogUrl.Format("http://%s.feiniu.com/photo/%u.html",g_ImUserInfo.GetUserName().c_str(),info->log_id);
		break;
	case 2://相册评论
		strContentType = "相册";
		strBlogUrl.Format("http://%s.feiniu.com/album/%u.html",g_ImUserInfo.GetUserName().c_str(),info->log_id);
		break;
	case 3://audio
		strContentType = "歌曲";
		strBlogUrl.Format("http://%s.feiniu.com/audio/%u.html",g_ImUserInfo.GetUserName().c_str(),info->log_id);
		break;
	case 4:
		strContentType = "视频";
		strBlogUrl.Format("http://%s.feiniu.com/video/%u.html",g_ImUserInfo.GetUserName().c_str(),info->log_id);
		break;
	}

	bool bWokuUser=false;

	if ( strcmp(info->szUserName,"")==0 || strcmp(info->szNickName,"")==0 )
		bWokuUser=true;

	if ( bWokuUser==false )
		strTitle.Format("\"%s\"对你的%s发表了评论:",info->szNickName,strContentType);
	else
		strTitle.Format("宝润商友对你的%s发表了评论:",info->szNickName,strContentType);


	m_reMessage.InsertText(strTitle);	

	xf.fontColor = RGB(80,80,80);
	m_reMessage.GetSel(cr);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);
	m_reMessage.SetStartIndent(460);


	m_reMessage.ReplaceSel(TrimHtmlCode(info->szContent));
	m_reMessage.ReplaceSel("\n\n");

	xf.fontColor = RGB(0,0,0);
	m_reMessage.GetSel(cr);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);

	m_reMessage.SetStartIndent(230);
	m_reMessage.ReplaceSel("点击这里查看:");
	m_reMessage.ReplaceSel("\n");
	m_reMessage.SetStartIndent(230);
	m_reMessage.ReplaceSel(strBlogUrl);
}


void C139NoteDlg::ShowNotification(const TWebNotificationInfo *info)
{
	XT_FONT xf;

	CHARRANGE cr;

	m_reMessage.SetWindowText("");

	CString strDate;

	strDate.Format("%d、%s",this->m_wndList.GetCurSel()+1,info->szCreateDate);

	xf.fontColor = RGB(0,0,0);

	m_reMessage.SetSel(-1,-1);
	m_reMessage.GetSel(cr);

	m_reMessage.SetXFont(xf,cr.cpMax,-1);

	m_reMessage.SetStartIndent(80);

	m_reMessage.InsertText(strDate+"\n");

	m_reMessage.SetSel(-1,-1);
	m_reMessage.GetSel(cr);

	xf.fontSize-=6;
	m_reMessage.SetXFont(xf,cr.cpMax,-1);
	m_reMessage.InsertText("\n");

	xf.fontSize+=6;

	m_reMessage.SetSel(-1,-1);
	m_reMessage.GetSel(cr);

	xf.fontColor = RGB(0,0,0);
	m_reMessage.SetXFont(xf,cr.cpMax,-1);
	m_reMessage.SetStartIndent(230);

	CString str;

	switch(info->type)
	{
	case XT_WEB_MSG::SYS_MSG:
		DisplaySysMsg(info);
		break;
	case XT_WEB_MSG::USER_MSG_BOX:	
		DisplayMsgBox(info);		
		break;
	case XT_WEB_MSG::FRIEND_MSG:
		DisplayFriendMsg(info);
		break;
	case XT_WEB_MSG::MAINPAGE_COMMENT:
		DisplayMainPageComment(info);
		break;
	case XT_WEB_MSG::CONTENT_COMMENT:
		DisplayBlogContentComment(info);
		break;
	default:
		str.Format("%s :",info->szNickName);		
		m_reMessage.InsertText(str);

		m_reMessage.SetSel(-1,-1);
		m_reMessage.GetSel(cr);

		m_reMessage.InsertText("\n\n");

		m_reMessage.SetStartIndent(230);
		m_reMessage.InsertText(TrimHtmlCode(info->szContent));
		break;
	}

	m_btnPrev.EnableWindow(TRUE);
	m_btnNext.EnableWindow(TRUE);

	int nSel = m_wndList.GetCurSel();

	if ( nSel==0 )
	{
		m_btnPrev.EnableWindow(FALSE);
	}

	if ( nSel==(vecNotification.size()-1) )
	{
		m_btnNext.EnableWindow(FALSE);			
	}
	
	
	if ( info->is_read==0 )
	{
		((TWebNotificationInfo *)info)->is_read=1;
		m_nFreshMessage--;
	}


	m_btnPrev.Invalidate();
	m_btnNext.Invalidate();
}

void C139NoteDlg::OnBnClickedHistory()
{
	if ( m_btnHistory.IsDown()==false )
	{
		SetStyle(0);


		m_wndList.SetFocus();
		m_btnHistory.SetDown(true);

	
	}
	else
	{
		SetStyle(1);
		m_btnHistory.SetDown(false);
	}
}

void C139NoteDlg::OnBnClickedClose()
{
	DestroyWindow();
}

void C139NoteDlg::OnBnClickedNext()
{
	int nCount = m_wndList.GetItemCount();
	int nSel   = m_wndList.GetCurSel();

	if ( nCount>0 )
	{
		if ( nSel==-1 )
		{
			m_wndList.SetCurSel(0);
		}
		else
		{
			nSel++;

			int nTemp    = nSel  ;
			bool bHasNew = false ;

			while ( nSel<nCount) 
			{
				//m_wndList.SetCurSel(nSel);
				if ( vecNotification[nSel]->is_read==0 )
				{
					m_wndList.SetCurSel(nSel);
					bHasNew = true;
					break;
				}
				else                    
					nSel ++;
			}

			if ( bHasNew==false )
			{
				if ( nTemp<nCount )
					m_wndList.SetCurSel(nTemp);
			}
		}
	}
}

void C139NoteDlg::OnBnClickedPrev()
{
	int nCount = m_wndList.GetItemCount();
	int nSel   = m_wndList.GetCurSel();

	if ( nCount>0 )
	{
		if ( nSel==-1 )
		{
			m_wndList.SetCurSel(0);
		}
		else
		{
			nSel--;
			if ( nSel>-1) 
				m_wndList.SetCurSel(nSel);
		}
	}
}

void C139NoteDlg::InitList()
{
	int count = vecNotification.size();

	for ( int i=0; i<count; i++ )
	{
		COLORREF clrText, clrBack;

		clrText = RGB(0,84,166);
		clrBack = i%2 ? RGB(255,255,255) : RGB(241,249,252);

		int nItem = m_wndList.InsertItem(i,vecNotification[i]->szCreateDate);

		CString strSysType = "未知消息";

		switch(vecNotification[i]->type)
		{
		case XT_WEB_MSG::SYS_MSG:
			strSysType = "系统消息";
			break;
		case XT_WEB_MSG::USER_MSG_BOX:	
			{
				switch(vecNotification[i]->sub_type)// 3--加入群邀请  4--客服消息	5--申请加为好友
				{
				case 0:
					strSysType = "站内消息";
					break;
				case 2:
					strSysType = "加好友";
					break;
				case 3:
					strSysType = "群邀请";
					break;
				case 4:
					strSysType = "客服消息";
					break;
				default:
					strSysType = "收件箱";
					break;
				}
			}
			break;
		case XT_WEB_MSG::FRIEND_MSG:
			strSysType = "站内消息";	
			break;
		case XT_WEB_MSG::MAINPAGE_COMMENT:
			strSysType = "主页留言";	
			break;
		case XT_WEB_MSG::CONTENT_COMMENT:
			{
				switch(vecNotification[i]->sub_type)//0文章评论，1图片评论，3音频评论，4视频评论
				{
				case 0:
					strSysType = "文章评论";
					break;
				case 1:
					strSysType = "图片评论";
					break;
				case 2://相册评论
					strSysType = "相册评论";
					break;
				case 3://audio
					strSysType = "歌曲评论";
					break;
				case 4:
					strSysType = "视频评论";
					break;
				}
			}
			break;
		default:
			break;
		}

		m_wndList.SetItemText(nItem,1, " "+strSysType,clrText, clrBack );
		m_wndList.SetItemText(nItem,2,CString(" ")+TrimHtmlCode(vecNotification[i]->szContent),clrText,clrBack);

		if ( vecNotification[i]->is_read )
		{
			m_wndList.SetItemTextColor(nItem,0,COLOR_READ);
			m_wndList.SetItemTextColor(nItem,1,COLOR_READ);
			m_wndList.SetItemTextColor(nItem,2,COLOR_READ);		
		}
		
	}
}

void C139NoteDlg::OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = -1;
	int nSubItem = -1;

	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;
	}

	if (pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED|LVIS_SELECTED)))
	{
		TWebNotificationInfo * info = vecNotification[nItem];

		if ( info->is_read==0 )
		{
			m_wndList.SetItemTextColor(nItem,0,COLOR_READ);
			m_wndList.SetItemTextColor(nItem,1,COLOR_READ);
			m_wndList.SetItemTextColor(nItem,2,COLOR_READ);	
		}

		ShowNotification(info);
	}

	*pResult = 0;
}


void C139NoteEditCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CRichEditCtrl::PreSubclassWindow();

	SetAutoURLDetect(TRUE);
	SetEventMask( GetEventMask() | ENM_LINK );
}

void C139NoteDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();

	if ( m_wndList.m_hWnd )
	{
		CRect rc;
		m_wndList.GetWindowRect(&rc);

		ScreenToClient(&rc);
		rc.InflateRect(2,2);
		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}

	if ( m_reMessage.m_hWnd )
	{
		CRect rc;
		m_reMessage.GetWindowRect(&rc);

		ScreenToClient(&rc);

		rc.InflateRect(2,2);
		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

void C139NoteDlg::InitContent()
{
	int n = vecNotification.size();

	int nUnRead=0;

	for ( int i=0; i<n; i++ )
	{
		if ( vecNotification[i]->is_read==0 )
			nUnRead++;
	}

	sort(vecNotification.begin(),vecNotification.end(),compareR);

	sort(vecNotification.begin(),vecNotification.begin()+nUnRead,compareN);
	sort(vecNotification.begin()+nUnRead,vecNotification.end(),compareN);

	CString strTitle;
	strTitle.Format("消息中心 - %d条消息,未读%d条",vecNotification.size(),m_nFreshMessage);
	SetWindowText(strTitle);

	m_wndList.DeleteAllItems();

	InitList();

	n    = vecNotification.size();
	int nSel = 0;

	for ( int i=0; i<n; i++ )
	{
		if ( vecNotification[i]->is_read==0 )//not read
		{
			nSel = i;
			break;
		}
	}

	SetTimer(0x01,100,NULL);
}

void C139NoteDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenuEx menu, *popmenu;

	menu.LoadMenu(IDR_MENU_TEMPLATE);
	popmenu = menu.GetSubMenu(0);
	popmenu->RemoveMenu(0,MF_BYPOSITION);

	CPoint point;
	GetCursorPos(&point);

	popmenu->AppendMenu(MF_STRING,IDM_DELETE,"删除");
    popmenu->AppendMenu(MF_STRING,IDM_CLEAR,"清空所有的消息");
		
	popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);

	*pResult = 0;
}

BOOL C139NoteDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message==WM_COMMAND )
	{
		switch(pMsg->wParam)
		{
		case IDM_DELETE:
			{
				int nSel = m_wndList.GetCurSel();

				if ( nSel!=-1 )
				{
					TWebNotificationInfo *pInfo = vecNotification[nSel];
					vecNotification.erase(vecNotification.begin()+nSel);
					m_wndList.DeleteItem(nSel);

					delete pInfo;

					if ( nSel<m_wndList.GetItemCount() )
					{
						m_wndList.SetCurSel(nSel);
					}
				}

				g_LocalData.SaveWebNotifications(vecNotification);
			}
			break;
		case IDM_CLEAR:
			{
				if ( CMessageBox::Model(GetSafeHwnd(),"确定要删除所有的消息吗？","消息中心",MB_YESNO )==IDYES )
				{
					m_nFreshMessage=0;
					ResetContent();
					g_LocalData.SaveWebNotifications(vecNotification);
				}
			}
			break;
		}
	}

	return CXSkinDlg::PreTranslateMessage(pMsg);
}

void C139NoteDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CXSkinDlg::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

void C139NoteDlg::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (KEYDOWN(VK_DELETE) )
	{
		int nSel = m_wndList.GetCurSel();

		if ( nSel!=-1 )
		{
			TWebNotificationInfo *pInfo = vecNotification[nSel];
			vecNotification.erase(vecNotification.begin()+nSel);
			m_wndList.DeleteItem(nSel);

			delete pInfo;

			if ( nSel<m_wndList.GetItemCount() )
			{
				m_wndList.SetCurSel(nSel);
			}

			g_LocalData.SaveWebNotifications(vecNotification);
		}
	}
	*pResult = 0;
}

void C139NoteDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		KillTimer(0x01);
		if ( m_wndList.GetItemCount()>0 )
			m_wndList.SetCurSel(0);
	}

	CXSkinDlg::OnTimer(nIDEvent);
}
