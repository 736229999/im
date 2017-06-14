// im\P2PUserInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\p2puserpanel.h"
#include "..\FriendInfoDlg.h"
#include "SAConfig.h"
#include "webprocess.h"
#include "MessageHandleWnd.h"
// CP2PUserPanel 对话框

CUserPictureWnd::CUserPictureWnd( )
{
	m_nHotItem = -1;
	m_bOver = false;
}

BEGIN_MESSAGE_MAP(CUserPictureWnd, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CUserPictureWnd::OnEraseBkgnd(CDC *pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,IM_SPACE_COLOR);

	return FALSE;
}


void CUserPictureWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_bOver==false )
	{
		SetTimer(0x01,100,NULL);
		m_bOver=true;
	}

	int nHotItem = HitTest(point);

	if ( m_nHotItem!=nHotItem )
	{
		m_nHotItem = nHotItem;
		Invalidate();

		if ( nHotItem!=-1 )
		{
			ShowToolTip(m_vecItem[nHotItem]->strTip);
		}
	}

	CButton::OnMouseMove(nFlags, point);
}

void CUserPictureWnd::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		CRect r;
		GetCursorPos(&pt);
		GetWindowRect(&r);

		if ( r.PtInRect(pt)==FALSE )
		{
			KillTimer(0x01);
			m_nHotItem=-1;
			m_bOver=false;
			Invalidate();
		}
	}

	CButton::OnTimer(nIDEvent);
}


void CUserPictureWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_nHotItem = HitTest(point);
	CWnd::OnLButtonDown(nFlags,point);
}

void CUserPictureWnd::ShowToolTip(CString strTip)
{
	if ( strTip == m_strPreTip )
	{
		return ;
	}

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_ToolTip.AddTool(this, strTip, rc, 1);
	} // if


	m_ToolTip.UpdateTipText(strTip ,this,1);

	m_strPreTip = strTip ;

	m_ToolTip.Activate(TRUE);
}

void CUserPictureWnd::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this,TTS_ALWAYSTIP);
		m_ToolTip.Activate(TRUE);
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 350);
	} 
} 

void CUserPictureWnd::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rcDraw = lpDrawItemStruct->rcItem;

	int n = m_vecItem.size();

	for ( int i=0; i<n; i++ )
	{
		CRect rcItem;
		rcItem.left   = i*(40+9);
		rcItem.top    = 1;
		rcItem.right  = rcItem.left + 40;
		rcItem.bottom = rcItem.top  + 40;

		::DrawBitmap(&dc,rcItem.left,rcItem.top,m_vecItem[i]->pBitmap,0);

		if ( i==m_nHotItem )
			dc.Draw3dRect(&rcItem,RGB(255,255,255),RGB(255,255,255));
		else
			dc.Draw3dRect(&rcItem,IM_LINE_COLOR,IM_LINE_COLOR);

		m_vecItem[i]->rcItem = rcItem;
	}
}

int CUserPictureWnd::HitTest(CPoint pt)
{
	int n = m_vecItem.size();

	for ( int i=0; i<n; i++ )
	{
		if ( m_vecItem[i]->rcItem.PtInRect(pt) )
		{
			return i;
		}
	}

	return -1;
}

void CUserPictureWnd::Clear()
{
	for ( int i=0; i<m_vecItem.size(); i++ )
	{
		delete m_vecItem[i]->pBitmap;
		delete m_vecItem[i];
	}

	m_vecItem.clear();
}

void CUserPictureWnd::AddBitmap(CBitmap *pBitmap, CString strTip, DWORD dwData)
{
	tagItem * item = new tagItem();

	item->dwData  = dwData;
	item->pBitmap = pBitmap;
	item->strTip  = strTip;

	m_vecItem.push_back(item);
	
	sort(m_vecItem.begin(),m_vecItem.end(),tagItem::Compare);

	if ( m_hWnd )
		Invalidate();
}

IMPLEMENT_DYNAMIC(CP2PUserPanel, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CP2PUserPanel)

CP2PUserPanel::CP2PUserPanel(FRIEND_INFO *pFriend,CWnd* pParent /*=NULL*/)
	: CDialog(CP2PUserPanel::IDD, pParent)
{
	m_pFriendInfo     = pFriend;
	m_userId		  = (uint32)-1;
	m_pLastestArticle = NULL;
	m_nInitWebDataOk  = 0;

	try
	{
		m_pWndAvatar = new CShockwaveflash();
	}
	catch(...)
	{
		m_pWndAvatar = NULL;
	}
}

CP2PUserPanel::~CP2PUserPanel()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}

	if ( m_pWndAvatar )
	{
		m_pWndAvatar->DestroyWindow();
		delete m_pWndAvatar;
		m_pWndAvatar = NULL;
	}
}

void CP2PUserPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BTN_ADDFRIEND,m_btnAddFriend);
	DDX_Control(pDX,IDC_STATIC_ART,m_btnArticle);
	DDX_Control(pDX, IDC_BUTTON_PIC, m_wndUserPic);
	//DDX_Control(pDX,IDC_STATIC_AVATAR,m_wndAvatarFrame);
	//DDX_Control(pDX,IDC_EXPLORER_AVATAR, m_wndAvatar);
}


BEGIN_MESSAGE_MAP(CP2PUserPanel, CDialog)
	ON_XT_MESSAGE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ADDFRIEND, OnBnClickedBtnAddfriend)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_MESSAGE(WM_WEBFLASH_NOTIFY,OnWebFlashNotify)
	SET_HMG_SCROLLBAR(IDC_EDIT_DESCRIPTION)	
	ON_WM_SIZE()
	ON_MESSAGE(WM_WEBDATA_NOTIFY,OnWebDataNotify)
	ON_MESSAGE(WM_WEBPICTURE_NOTIFY,OnWebPictureNotify)
	ON_STN_CLICKED(IDC_STATIC_ART, OnStnClickedStaticArt)
	ON_BN_CLICKED(IDC_BUTTON_PIC, OnBnClickedButtonPic)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CP2PUserPanel)
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK,OnUserInfoModified)
END_XT_MESSAGE_MAP()


// CP2PUserPanel 消息处理程序
void CP2PUserPanel::UpdateFriendInfo()
{
	return;
	if ( m_pFriendInfo )
	{
		CString strAvatarUrl;
		//strAvatarUrl.Format("http://%s.feiniu.com/avatar/mini_display.php",m_pFriendInfo->username);
		//VARIANT var;
	//	var.bstrVal = strAvatarUrl.AllocSysString();
		//var.vt = VT_BSTR;
		strAvatarUrl.Format("http://%s.feiniu.com/avatar/display.swf",m_pFriendInfo->GetUserName().c_str());
	//	m_wndAvatar.Navigate2(strAvatarUrl.AllocSysString(),NULL,NULL,NULL,NULL);
		//g_webFlashProcess.GetUserFlashShow(m_pFriendInfo,this);

		if ( m_pWndAvatar )
		{
			m_pWndAvatar->put_Movie(strAvatarUrl);
			m_pWndAvatar->Play();
		}
	}
}

BOOL CP2PUserPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect(0,0,10,10);

	if ( m_pWndAvatar->Create("ShockwaveFlash.Object",WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rect,this,1203) )
	{
		m_pWndAvatar->ShowWindow(SW_SHOW);
	}
	else
	{
		delete m_pWndAvatar;
		m_pWndAvatar = NULL;
	}

	//m_wndAvatar.SetParent(&m_wndAvatarFrame);
//	m_wndAvatar.EnableRButton(false);

	m_btnArticle.SetAutoSize(false);
	m_btnArticle.SetNormalColor(RGB(0,84,166));
	m_btnArticle.SetLinkColor(IM_LINE_COLOR);
	m_btnArticle.ShowWindow(SW_HIDE);

	m_btnAddFriend.SetAutoSize(false);
	m_btnAddFriend.SetNormalColor(RGB(0,84,166));
	m_btnAddFriend.SetLinkColor(IM_LINE_COLOR);

	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);

	FRIEND_INFO *pFriend = FindFriend(m_pFriendInfo->GetId());

	if ( pFriend )
	{
		if ( pFriend->pUserWebArticleInfo==NULL )
		{
			pFriend->pUserWebArticleInfo = new TUserWebArticleInfo(pFriend->GetId());
			g_spaceData.LoadUserWebArticles(m_pFriendInfo->GetId(),*pFriend->pUserWebArticleInfo);
		}
		
		if ( pFriend->pUserWebPictureInfo==NULL )
		{
			pFriend->pUserWebPictureInfo = new TUserWebPictureInfo(pFriend->GetId());
			g_spaceData.LoadUserWebPictures(pFriend->GetId(),*pFriend->pUserWebPictureInfo);
		}

		g_webDataProcess.GetWebPictureList(IntToStr(pFriend->GetId()),pFriend->pUserWebPictureInfo->szLastUpdateDate,"",this);
		g_webDataProcess.GetWebArticleList(IntToStr(pFriend->GetId()),pFriend->pUserWebArticleInfo->szLastUpdateDate,"",this);
	}

	UpdateFriendInfo();

	return TRUE; 
}

void CP2PUserPanel::OnStnClickedStaticFriend()
{	
}

void CP2PUserPanel::OnBnClickedButtonMore()
{
}


LRESULT CP2PUserPanel::OnWebPictureNotify(WPARAM wParam,LPARAM lParam)
{
	if ( wParam!=1 )
		return 0;

	CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;

	CBitmap * pBitmap = CBitmapFromFile(result->szLocalFile,40,40);

	if ( pBitmap )
	{
		int n=this->m_vecLastestPicture.size();

		for ( int i=0; i<n; i++ )
		{
			if ( m_vecLastestPicture[i]->dwPictureID==result->pic_id )
			{
				CString strTip;
				strTip.Format("%s\n%s",m_vecLastestPicture[i]->szTitle,m_vecLastestPicture[i]->szCreateDate);
				m_wndUserPic.AddBitmap(pBitmap,strTip,m_vecLastestPicture[i]->dwPictureID);
			}
		}

	}
	return 0;
}

LRESULT CP2PUserPanel::OnWebDataNotify(WPARAM wParam,LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;

	uint32 fid = atol((LPCSTR)result->strID);

	if ( fid!=m_pFriendInfo->GetId() )
	{
		return 1;
	}

	FRIEND_INFO *pFriend  = FindFriend(fid);

	if ( pFriend )		
	{
		if ( wParam==1)
		{
			m_nInitWebDataOk++;

			vector<TWebArticleInfo *> info;
			CString strServerDate;

			try
			{
				ParaseWebArticles(result->strWebResult,info,strServerDate,100);
			}
			catch(...)
			{

			}

			int n = info.size();

			TUserWebArticleInfo * u = pFriend->pUserWebArticleInfo;
			n--;

			if (u)
			{
				while(n>-1)
				{
					strncpy(u->szLastUpdateDate,(LPCSTR)strServerDate,sizeof(u->szLastUpdateDate)-1);

					if ( u->Push(info[n])==false )
					{
						delete info[n];
					}	

					n--;
				}

				sort(u->vecArticles.begin(),u->vecArticles.end(),TWebArticleInfo::CompareArticleDate);

				if ( u->vecArticles.size()>0 )
					m_pLastestArticle = u->vecArticles[0];

			//	g_spaceData.SaveUserWebArticles(*u);
			}

			info.clear();
		}
		else
		{	
			m_nInitWebDataOk++;

			vector<TWebPictureInfo *> info;
			CString strServerDate;
			
			try
			{
				ParaseWebPictures(result->strWebResult,info,strServerDate,100);
			}
			catch(...)
			{
			}

			int n = info.size();

			n--;

			TUserWebPictureInfo * u = pFriend->pUserWebPictureInfo;

			if (u)
			{		
				strncpy(u->szLastUpdateDate,(LPCSTR)strServerDate,sizeof(u->szLastUpdateDate)-1);

				while(n>-1)
				{
					if ( u->Push(info[n])==false )
					{
						delete info[n];
					}
									
					n--;
				}

				sort(u->vecPics.begin(),u->vecPics.end(),TWebPictureInfo::ComparePictureDate);

				int nPics = u->vecPics.size();
				for ( int i=0; i<nPics; i++ )
				{
					m_vecLastestPicture.push_back(u->vecPics[i]);
					if ( i>1 )
						break;
				}

			//	g_spaceData.SaveUserWebPictures(*u);
			}
		}

	}

	if ( m_nInitWebDataOk==2 )
	{
		SetTimer(0x01,10,NULL);
	}

	return 0;
}

void CP2PUserPanel::OnUserInfoModified(const char *data,WORD wRet)
{
	LPXT_USERINFO_MOD_ACK resp = (LPXT_USERINFO_MOD_ACK)data;

	if ( wRet==0 )
	{
	}	
}

void CP2PUserPanel::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();

	CRect rcAvatar;
	rcAvatar.left = (cx-145)/2;
	rcAvatar.top  = 1;
	rcAvatar.right = rcAvatar.left+145;
	rcAvatar.bottom = rcAvatar.top+250;
	rcAvatar.InflateRect(1,1);

	dc.Draw3dRect(&rcAvatar,IM_LINE_COLOR,IM_LINE_COLOR);

	if ( this->m_pWndAvatar==NULL )
	{
		rcAvatar.InflateRect(-5,-5);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText("由于你没有安装Flash控件，无法显示对方的闪秀",rcAvatar,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
	}
}

void CP2PUserPanel::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	CDialog::OnDestroy();
}

void CP2PUserPanel::OnBnClickedBtnAddfriend()
{
	CFriendInfoDlg::NewDlg(m_pFriendInfo);
}

BOOL CP2PUserPanel::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);

	CRgn rgn;
	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);

	//if ( m_pWndAvatar && m_pWndAvatar->m_hWnd )
	//{
	//	CRect rc1;
	//	m_pWndAvatar->GetWindowRect(&rc1);
	//	ScreenToClient(&rc1);

	//	CRgn rgn1;
	//	rgn1.CreateRectRgn(rc1.left,rc1.top,rc1.right,rc1.bottom);

	//	rgn.CombineRgn(&rgn,&rgn1,RGN_DIFF);

	//	pDC->SelectObject(&rgn);
	//}

	//if ( m_wndAvatarFrame.m_hWnd )
	//{
	//	CRect rc1;
	//	m_wndAvatarFrame.GetWindowRect(&rc1);
	//	ScreenToClient(&rc1);

	//	CRgn rgn1;
	//	rgn1.CreateRectRgn(rc1.left,rc1.top,rc1.right,rc1.bottom);

	//	rgn.CombineRgn(&rgn,&rgn1,RGN_DIFF);

	//	pDC->SelectObject(&rgn);
	//}


	pDC->FillSolidRect(rc,IM_SPACE_COLOR);

	return TRUE;
}

void CP2PUserPanel::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		int n=m_vecLastestPicture.size();

		int nFlag=0;

		for ( int i=0; i<n; i++ )
		{
			::g_webPictureProcess.GetWebPicture(this->m_pFriendInfo->GetId(),
				*m_vecLastestPicture[i],
				40,this);
		}

		if ( n==0 )
		{
			CRect rc;
			m_btnArticle.GetWindowRect(&rc);
			ScreenToClient(&rc);

			rc.top    -= 43;
			rc.bottom -= 43;
			m_btnArticle.MoveWindow(&rc);

		}
		else
		{
			nFlag++;
			m_wndUserPic.ShowWindow(SW_SHOW);
		}

		if ( m_pLastestArticle )
		{
			nFlag++;
			m_btnArticle.SetWindowText(TrimHtmlCode(m_pLastestArticle->szOutlines));
			m_btnArticle.ShowWindow(SW_SHOW);
		}

		KillTimer(0x01);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CP2PUserPanel::PreTranslateMessage(MSG* pMsg)
{
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
		case VK_ESCAPE:
			GetParent()->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		default:
			;
		}  
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CP2PUserPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	AdjustUI();
}

void CP2PUserPanel::AdjustUI()
{
	if ( m_hWnd )
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		int cx = rcClient.Width();
		int cy = rcClient.Height();

		//if ( m_wndAvatarFrame.m_hWnd )
		//{
		//	m_wndAvatarFrame.MoveWindow((cx-145)/2,1,145,250);
		//	m_wndAvatar.MoveWindow(-2,-2,169,274);
		//}

		if ( m_pWndAvatar && m_pWndAvatar->m_hWnd )
			m_pWndAvatar->MoveWindow((cx-145)/2,1,145,250);

		//if ( m_edtDescription.m_hWnd )
		//	m_edtDescription.MoveWindow(2,280,cx-2,cy-285);
	}
}

LRESULT CP2PUserPanel::OnWebFlashNotify(WPARAM wParam, LPARAM lParam)
{
	//if ( m_pWndAvatar )
	//{
	//	m_pWndAvatar->put_Movie((LPCSTR)lParam);
	//	m_pWndAvatar->Play();
	//}

	return 0;
}

void CP2PUserPanel::OnStnClickedStaticArt()
{
	if ( m_pLastestArticle )
	{
		MiniOpenUrl(m_pLastestArticle->szUrl);
	}
}


BOOL CUserPictureWnd::PreTranslateMessage(MSG* pMsg)
{
	if ( m_ToolTip.m_hWnd )
		m_ToolTip.RelayEvent(pMsg);

	return CButton::PreTranslateMessage(pMsg);
}

void CP2PUserPanel::OnBnClickedButtonPic()
{
	int nItem = this->m_wndUserPic.GetCurrentItem();
	if ( nItem!=-1 )
	{
		DWORD dwId = m_wndUserPic.GetItemData(nItem);
		for ( int i=0; i<m_vecLastestPicture.size(); i++ )
		{
			if ( dwId==m_vecLastestPicture[i]->dwPictureID )
			{
				return;
				CString strUrl;
				strUrl.Format("http://%s.feiniu.com/photo/%u_%u.html",
					m_pFriendInfo->GetUserName().c_str(),
					m_vecLastestPicture[i]->nAlbumId,
					m_vecLastestPicture[i]->dwPictureID);

				MiniOpenUrl(strUrl);
			}
		}
	}
}
