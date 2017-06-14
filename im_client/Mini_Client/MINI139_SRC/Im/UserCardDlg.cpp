// im\UserCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\ImClient.h"
#include "UserCardDlg.h"
#include ".\usercarddlg.h"
#include "webprocess.h"
#include "..\..\..\Common\Common.h"
#include "LocalData.h"
#include "..\MainFrame.h"
using namespace DuiLib;
#define DLG_WIDTH 350
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
extern  CLocalData	g_LocalData;

void CUserCardDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////////获取控件变量
	m_pBtnclose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));
	m_pBtnPic = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindControl("BtnPic"));//头像
	m_pLabelNickName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("nickNameLabel"));
	m_pLabelProfile   = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("ProfileLabel"));

#ifdef _VER_INTERNAL_
	m_pLabelCellphone = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("cellphoneLabel"));
	m_pLabelTel   = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("telLabel"));
	m_pLabelJob = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("jobLabel"));
#else
	m_pLevel = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelSignLabel"));
	m_pLabelLevel = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelLabel"));
	m_pLabelSec   = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelSec"));
	m_pLabelThird = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelThird"));
#endif

#ifdef _CS_PLAT_EX_  ///飞牛客服平台
	m_pLevel->SetVisible(false);	 ////取消"等级"
	m_pLabelLevel->SetVisible(false);
	m_pLabelSec->SetVisible(false);
	m_pLabelThird->SetVisible(false);
#endif

	UpdateBk();
	g_pProtocol->RegistCmd(CMD_CREATE_CLUSTER_ACK,GetHWND());
}


std::tr1::shared_ptr<CUserCardDlg> CUserCardDlg::pInstance = NULL;
std::tr1::shared_ptr<CUserCardDlg> CUserCardDlg::Show(FRIEND_INFO *pFriend, int nLeft, int nTop)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		BringWindowToTop(pInstance->GetHWND());
		pInstance->UpdateFriendInfo(pFriend);
		RECT rcDlg;
		GetWindowRect(pInstance->GetHWND(), &rcDlg);
		::SetWindowPos(pInstance->GetHWND(),NULL,nLeft,nTop,rcDlg.right-rcDlg.left, rcDlg.bottom-rcDlg.top,SWP_SHOWWINDOW);
	}
	else
	{
		pInstance = tr1::shared_ptr<CUserCardDlg>(new CUserCardDlg());
		//
		pInstance->Create(GetDesktopWindow(), _T(""), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, nLeft,nTop,0);
		pInstance->UpdateFriendInfo(pFriend);
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

void CUserCardDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()) && m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,140' dest='0,0,%d,140'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,140' dest='0,0,%d,140' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}
}

// 消息
void CUserCardDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnclose) ////收到消息
		{
			pInstance->ShowWindow(SW_HIDE);
		}
		//else if(msg.pSender == m_pBtnPic) ////收到消息
		//{
		//	OnBnClickedFaceIcon();
		//}
	}
}

void CUserCardDlg::Refresh(FRIEND_INFO *pFriend)
{
	if ( pInstance )
	{
		if ( pFriend->GetId()==pInstance->m_pFriendInfo->GetId() )
		{
			pInstance->UpdateFriendInfo(pFriend);
		}

	}
}


BEGIN_XT_MESSAGE_MAP(CUserCardDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
END_XT_MESSAGE_MAP()


LRESULT CUserCardDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// 处理后台交互消息
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CUserCardDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(GetHWND());
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}


void CUserCardDlg::CloseWindow()
{
	if ( pInstance )
	{
		 pInstance->ShowWindow(SW_HIDE);
	}
}

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
void CUserCardDlg::UpdateFriendInfo(FRIEND_INFO *pFriend)
{

	if(pFriend != NULL && CMainFrame::IsWork(pFriend->GetId()))
	{
		m_pLevel->SetVisible(false);	 ////取消"等级"
		m_pLabelLevel->SetVisible(false);
		m_pLabelSec->SetVisible(false);
		m_pLabelThird->SetVisible(false);
	}
	else
	{
		m_pLevel->SetVisible(true);	 ////取消"等级"
		m_pLabelLevel->SetVisible(true);
		m_pLabelSec->SetVisible(true);
		m_pLabelThird->SetVisible(true);
	}
	m_pFriendInfo = new FRIEND_INFO();
	memset(m_pFriendInfo,'0',sizeof(FRIEND_INFO));
	memcpy(m_pFriendInfo, pFriend, sizeof(FRIEND_INFO));

	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(m_pFriendInfo);
	CString strLocalPath =  g_webUserHeadIconProcess.GetFaceFile(m_pFriendInfo);
	CString strGifFile =  "";
	CString strPath =  m_pFriendInfo->face_url;	
	if(strPath.Find(".gif")!=-1)
	{
		strGifFile = strLocalPath+".gif";////GIF文件
		if ( !FileExist(strGifFile) )////不存在则创建
		{
			CopyFile(strLocalPath,strGifFile,FALSE);
		}
	}
	if ( FileExist(strLocalPath) )
	{
		if ( FileExist(strGifFile) )
		{
			//CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("BkLayout")));
			//pVLhead->SetBkImage("SysBtn\\icon_88x88_bg.png");
			m_pBtnPic->SetBkImage("");
			m_pBtnPic->SetNormalGifFile(strGifFile);
			
			
			//CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("BkLayout")));
			//pVLhead->SetBkImage("SysBtn\\icon_88x88_bg.png");
		}
		else
		{
			string strImg = CutRoundImg(strLocalPath, 88, 88, FALSE);
			m_pBtnPic->SetBkImage(strImg.c_str());
			m_pBtnPic->SetNormalGifFile("");
			//CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("BkLayout")));
			//pVLhead->SetBkImage(szHead);
		}
	}

	m_pLabelNickName->SetText(UserInfoToStr(m_pFriendInfo));

	CString str;
	str.Format("%s",m_pFriendInfo->user_sign);
	////获得签名
	m_pLabelProfile->SetText(str);


	FRIEND_INFO *pRealFriend = FindFriend(m_pFriendInfo->GetId());

	if ( pRealFriend )
	{

#ifdef _VER_INTERNAL_		// 内部版显示手机号
		//更新好友资料
		XT_USERINFO_GET_REQ req;
		req.fid = m_pFriendInfo->GetId();
		g_pProtocol->SendReq(req);
#endif
	}
	
/////////////////////////////////////////////
////显示用户等级
#ifndef _VER_INTERNAL_ ///内部版
  #ifndef _CS_PLAT_EX_  ///飞牛客服平台
	/*m_wndUserLevel.SetOnlineTime(m_pFriendInfo->online_time);*/
	// 等级
	int nNextTime = 0;
	int nLevel = CountLevel(m_pFriendInfo->online_time,nNextTime);
	SetLevelPic(nLevel);////设置图片
	CString strMsg = _T("");
	strMsg.Format("当前在线时长为%d,下一次升级需要时间为%d",m_pFriendInfo->online_time,nNextTime);
	m_pLabelLevel->SetToolTip(strMsg);
	m_pLabelSec->SetToolTip(strMsg);
	m_pLabelThird->SetToolTip(strMsg);
  #endif
#endif

	delete m_pFriendInfo;
	m_pFriendInfo = NULL;
}

void CUserCardDlg::SetLevelPic(int nLevel)
{
#ifndef _VER_INTERNAL_
	switch(nLevel)
	{
	case 1:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 2:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 3:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelThird->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(true);
		}
		break;
	case 4:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 5:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 6:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelThird->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(true);
		}
		break;
	default:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	}
#endif
}

void CUserCardDlg::OnUserInfoAck(const char *data,WORD wRet)
{
#ifdef _VER_INTERNAL_
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==m_pFriendInfo->GetId() )
		{   
			// 解析出职务，分机号
			string strJob, strExt;
			SplitExtAndJob(ack->flash_url, strExt, strJob);

			CString strMobel;
			strMobel.Format("%s", strExt.c_str());
			m_pLabelTel->SetText(strMobel);


			// 职务
			CString sTRJob;
			sTRJob.Format("%s", strJob.c_str());
			m_pLabelJob->SetText(sTRJob);

			// 手机
			strMobel.Format("%s", ack->mobile);
			m_pLabelCellphone->SetText(strMobel);

		}
	}
#endif
}



/*
等级计算公式：
time = (N`2+4*N)*a
a=18
其中N为当前用户等级，time为该等级需要的时间

则当前在线时长time可以计算等级 N = -2 + (√￣(16+time*(4/a)))/2 
*/
int CUserCardDlg::CountLevel(int nTime,int&nNextDelta)
{
	int nLevel;
	float fLevel;
	int nNextTime;	// 到下一等级需要的时间
	fLevel=-2.0+sqrt(16.0+nTime*(4.0/18))/2.0;
	nLevel=fLevel;
	if(nLevel==0)
	{
		nLevel=1;
	}
	if(fLevel >= nLevel)
	{
		nLevel++;
	}
	nNextTime=nLevel*nLevel+4*nLevel;
	nNextTime *= 18;
	nNextDelta=nNextTime-nTime;
	return nLevel;
}


//// 点击头像区域
//void CUserCardDlg::OnBnClickedFaceIcon()
//{
//	// 如果当前显示的是当前登录用户信息，点击头像区域时，向MainFrame发送消息
//	if(m_pFriendInfo->GetId() == g_ImUserInfo.GetId())
//	{
//		::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_FACE_CLICKED, 0, 0);
//	}
//}

CUserCardDlg::~CUserCardDlg()
{
	if(m_pFriendInfo != NULL)
	{
		delete m_pFriendInfo;
		m_pFriendInfo = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
// CUserCardDlg 对话框
//
//IMPLEMENT_DYNAMIC(CUserCardDlg, CDialog)
//CUserCardDlg::CUserCardDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CUserCardDlg::IDD, pParent),
//	m_wndFaceIcon(IDB_PNG_BIG_BROUND_NORMAL, IDB_PNG_BIG_BROUND_NORMAL)
//{
//	m_pFriendInfo = new FRIEND_INFO();
//	m_brushBackground.CreateSolidBrush(IM_SPACE_COLOR);
//	Create(CUserCardDlg::IDD,pParent);
//}
//
//
//void CUserCardDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	
//	DDX_Control(pDX,IDC_STATIC_FACE,m_wndFaceIcon);
//#ifndef _VER_INTERNAL_
//	DDX_Control(pDX,IDC_STATIC_LEVEL,m_wndUserLevel);
//#endif
//	DDX_Control(pDX,IDC_BUTTON_PIC,m_wndUserPic);
//	DDX_Control(pDX,IDC_BTN_BLOG,m_btnHome);
//	DDX_Control(pDX,IDC_STATIC_ART,m_btnArticle);
//}
//
//
//BEGIN_MESSAGE_MAP(CUserCardDlg, CDialog)
//	ON_BN_CLICKED(IDC_BTN_BLOG, OnBnClickedBtnBlog)
//	ON_WM_ERASEBKGND()
//	ON_WM_CTLCOLOR()
//	ON_WM_PAINT()
//	ON_WM_NCDESTROY()
//	ON_WM_TIMER()
//	ON_MESSAGE(WM_WEBPICTURE_NOTIFY,OnWebPictureNotify)
//	ON_STN_CLICKED(IDC_STATIC_ART, OnStnClickedStaticArt)
//	ON_BN_CLICKED(IDC_BUTTON_PIC, OnBnClickedButtonPic)
//	ON_BN_CLICKED(IDC_STATIC_FACE, OnBnClickedFaceIcon)
//	ON_XT_MESSAGE()
//END_MESSAGE_MAP()
//
//BEGIN_XT_MESSAGE_MAP(CUserCardDlg)
//	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
//END_XT_MESSAGE_MAP()
//
//
//LRESULT CUserCardDlg::OnWebPictureNotify(WPARAM wParam, LPARAM lParam)
//{
//	if ( wParam!=1 )
//		return 0;
//
//	CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;
//
//	CBitmap * pBitmap = CBitmapFromFile(result->szLocalFile,40,40);
//
//	if ( pBitmap )
//	{
//		int n=this->m_vecLastestPicture.size();
//
//		for ( int i=0; i<n; i++ )
//		{
//			if ( m_vecLastestPicture[i]->dwPictureID==result->pic_id )
//			{
//				CString strTip;
//				strTip.Format("%s\n%s",m_vecLastestPicture[i]->szTitle,m_vecLastestPicture[i]->szCreateDate);
//				m_wndUserPic.AddBitmap(pBitmap,strTip,m_vecLastestPicture[i]->dwPictureID);
//			}
//		}
//
//	}
//
//	return 0;
//}
//
//void CUserCardDlg::Refresh(FRIEND_INFO *pFriend)
//{
//	if ( pInstance )
//	{
//		if ( pFriend->GetId()==pInstance->m_pFriendInfo->GetId() )
//			pInstance->UpdateFriendInfo(pFriend);
//	}
//}
//
//void CUserCardDlg::Show(FRIEND_INFO *pFriend, int nLeft, int nTop,CRect rcItem)
//{
//	if ( pInstance==NULL )
//	{
//		pInstance= new CUserCardDlg(CWnd::GetDesktopWindow());
//	}
//
//	CRect rcWnd;
//	pInstance->GetWindowRect(&rcWnd);
//
//	pInstance->MoveWindow(nLeft,nTop,rcWnd.Width(),rcWnd.Height());
//	pInstance->UpdateFriendInfo(pFriend);
//	pInstance->ShowWindow(SW_SHOW);
//
//	if ( rcItem.left<nLeft )
//	{
//		pInstance->m_bOffset = true;
//	}
//	else
//	{
//		pInstance->m_bOffset = false;
//	}
//
//	pInstance->m_rcItem = rcItem;
//	pInstance->m_nTimerCount = 0;
//
//	/*if(pInstance->m_pFriendInfo->id == g_ImUserInfo.GetId())
//	{
//		pInstance->m_wndFaceIcon.SetToolTip("详细资料");
//	}
//	else
//	{
//		pInstance->m_wndFaceIcon.SetToolTip("");
//	}*/
//}
//
//void CUserCardDlg::Close()
//{
//	if ( pInstance )
//	{
//		pInstance->DestroyWindow();
//	}
//}
//
//void CUserCardDlg::UpdateFriendInfo(FRIEND_INFO *pFriend)
//{
//	m_vecLastestPicture.clear();
//	m_pLastestArticle=NULL;
//	m_wndUserPic.Clear();
//
//	memcpy(m_pFriendInfo, pFriend, sizeof(FRIEND_INFO));
//
//	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(m_pFriendInfo);
//	
//	Invalidate();
//
//	if ( FileExist(strFaceFile) )
//	{
//		m_wndFaceIcon.SetPicPath(strFaceFile);
//		//m_wndFaceIcon.SetFaceFile(strFaceFile,0);
//	}
//	/*else
//	{
//	m_wndFaceIcon.SetSex(m_pFriendInfo->sex,0);
//	}*/
//
//	CString str;
//	str.Format("昵称: %s",m_pFriendInfo->nickname);
//	SetDlgItemText(IDC_STATIC_NICK,str);
//
//	str.Format("性别: %s",SexToStr(m_pFriendInfo->sex) );
//	SetDlgItemText(IDC_STATIC_SEX,str);
//
//	FRIEND_INFO *pRealFriend = FindFriend(m_pFriendInfo->GetId());
//
//	if ( pRealFriend )
//	{
//		if ( pRealFriend->pUserWebArticleInfo )
//		{
//			sort(pRealFriend->pUserWebArticleInfo->vecArticles.begin(),pRealFriend->pUserWebArticleInfo->vecArticles.end(),
//				TWebArticleInfo::CompareArticleDate);
//
//			if ( pRealFriend->pUserWebArticleInfo->vecArticles.size()>0 )
//				m_pLastestArticle = pRealFriend->pUserWebArticleInfo->vecArticles[0];
//		}
//		
//		if ( pRealFriend->pUserWebPictureInfo )
//		{
//			sort(pRealFriend->pUserWebPictureInfo->vecPics.begin(),pRealFriend->pUserWebPictureInfo->vecPics.end(),
//				TWebPictureInfo::ComparePictureDate);
//			
//			int nPics = pRealFriend->pUserWebPictureInfo->vecPics.size();
//
//			for ( int i=0; i<nPics; i++ )
//			{
//				TWebPictureInfo *pInfo = pRealFriend->pUserWebPictureInfo->vecPics[i];
//				m_vecLastestPicture.push_back(pInfo);
//
//				g_webPictureProcess.GetWebPicture(pFriend->GetId(),*pInfo,40,this);
//
//				if ( i>4 )
//					break;
//			}
//		}
//
//#ifdef _VER_INTERNAL_		// 内部版显示手机号
//		//更新好友资料
//		XT_USERINFO_GET_REQ req;
//		req.fid = m_pFriendInfo->GetId();
//		g_pProtocol->SendReq(req);
//#endif
//	}
//	
//	if ( m_pLastestArticle )
//		this->m_btnArticle.SetWindowText(TrimHtmlCode(m_pLastestArticle->szOutlines));
//	else
//		this->m_btnArticle.SetWindowText(TrimHtmlCode(m_pFriendInfo->description));
//#ifndef _VER_INTERNAL_
//	m_wndUserLevel.SetOnlineTime(m_pFriendInfo->online_time);
//#endif
//	SetDlgItemText(IDC_STATIC_SIGN,m_pFriendInfo->user_sign);
//}
//// CUserCardDlg 消息处理程序
//
//BOOL CUserCardDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
//
//	// 遍历所有子控件，设置控件字体
//	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//	while(hWndChild)  
//	{  
//		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//	}
//	::SendMessage(GetDlgItem(IDC_STATIC_NICK)->GetSafeHwnd(), WM_SETFONT, (WPARAM)g_font13B.GetSafeHandle(), TRUE);
//
//	ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L|WS_EX_PALETTEWINDOW);
//
//	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
//
//	m_btnArticle.SetNormalColor(RGB(0,86,166));
//	m_btnArticle.SetLinkColor(IM_LINE_COLOR);
//	m_btnArticle.SetAutoSize(false);
//	
//	m_btnHome.ShowWindow(SW_HIDE);
//	//m_btnHome.SetBitmap(IDB_FRIEND_BLOG,IDB_FRIEND_BLOG);
//	//m_btnHome.SetToolTip("个人主页");
//#ifndef _VER_INTERNAL_
//	m_wndUserLevel.SetBkColor(IM_SPACE_COLOR);
//#endif
//	SetTimer(0x01,200,NULL);
//
//#ifdef _VER_INTERNAL_		// 内部版显示手机号
//	GetDlgItem(IDC_STATIC_MOBILE)->ShowWindow(SW_SHOW);
//#else
//	GetDlgItem(IDC_STATIC_MOBILE)->ShowWindow(SW_HIDE);
//	CRect rcMobel;
//	GetDlgItem(IDC_STATIC_MOBILE)->GetWindowRect(&rcMobel);
//	ScreenToClient(&rcMobel);
//	CRect rcSign;
//	GetDlgItem(IDC_STATIC_SIGN)->GetWindowRect(&rcSign);
//	ScreenToClient(&rcSign);
//	GetDlgItem(IDC_STATIC_SIGN)->MoveWindow(rcMobel.left, rcMobel.top, rcSign.Width(), rcSign.Height());
//	CRect rcClient;
//	GetWindowRect(&rcClient);
//	CRect rcFace;
//	m_wndFaceIcon.GetWindowRect(&rcFace);
//	ScreenToClient(&rcFace);
//	MoveWindow(rcClient.left, rcClient.top, rcClient.Width(), rcFace.Height()+2*rcFace.top);
//#endif
//
//	{	// 图标居中
//		CRect rcClient;
//		GetWindowRect(&rcClient);
//		CRect rcFace;
//		m_wndFaceIcon.GetWindowRect(&rcFace);
//		ScreenToClient(&rcFace);
//		m_wndFaceIcon.MoveWindow(rcFace.left, (rcClient.Height()-rcFace.Height())/2, rcFace.Width(), rcFace.Height());
//	}
//
//	return TRUE; 
//	// 异常: OCX 属性页应返回 FALSE
//}
//
//void CUserCardDlg::OnBnClickedBtnBlog()
//{
//	if(g_pWebConfig)
//	{
//		MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(m_pFriendInfo->GetUserName().c_str())));
//	}
//	
//}
//
//BOOL CUserCardDlg::OnEraseBkgnd(CDC* pDC)
//{
//	CRect r;
//	GetClientRect(&r);
//	pDC->FillSolidRect(&r,IM_SPACE_COLOR);
//	//pDC->Draw3dRect(&r,IM_LINE_COLOR,IM_LINE_COLOR);
//	return FALSE;
//}
//
//HBRUSH CUserCardDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		pDC->SetTextColor(RGB(255,255,255));
//		hbr = (HBRUSH)m_brushBackground.GetSafeHandle();
//	}
//	return hbr;
//}
//
//void CUserCardDlg::OnPaint()
//{
//	CDialog::OnPaint();
//	/*CPaintDC dc(this);
//
//	CWnd *pWnd = GetDlgItem(IDC_STATIC_SIGN);
//
//	if ( pWnd )
//	{
//	int nSaveDC = dc.SaveDC();
//
//	dc.SetBkMode(TRANSPARENT);
//	dc.SelectObject(&g_font12);
//
//	CRect r;
//	pWnd->GetWindowRect(&r);
//	ScreenToClient(&r);
//	dc.SetTextColor(RGB(255,255,255));
//	dc.DrawText(m_pFriendInfo->user_sign,&r,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
//
//	dc.RestoreDC(nSaveDC);
//	}*/
//}
//
//void CUserCardDlg::OnNcDestroy()
//{
//	g_pProtocol->UnRegistCmd(GetSafeHwnd());
//	CDialog::OnNcDestroy();
//	delete this;
//}
//
//void CUserCardDlg::OnTimer(UINT nIDEvent)
//{
//	//CDialog::OnTimer(nIDEvent);
//
//	if ( nIDEvent==0x01 )
//	{
//		CPoint pt;
//		CRect  rc1, rc2;
//		GetCursorPos(&pt);
//		GetWindowRect(GetHWND(),&rc1);
//
//		if ( m_bOffset==false )
//		{
//			BOOL b1 = rc1.PtInRect(pt);
//			BOOL b2 = m_rcItem.PtInRect(pt);
//
//			if ( b1==FALSE && b2==FALSE )
//			{
//				m_nTimerCount++;
//			}
//			else
//			{
//				m_nTimerCount=0;
//			}
//
//			if ( m_nTimerCount>3 )
//			{
//				Close();
//			}
//		}
//		else
//		{
//			BOOL b1 = rc1.PtInRect(pt);
//			BOOL b2 = m_rcItem.PtInRect(pt);
//
//			if ( b1==FALSE && b2==FALSE )
//			{
//				m_nTimerCount++;
//			}
//			else
//			{
//				m_nTimerCount = 0;
//			}
//
//			if ( m_nTimerCount>1 )
//			{
//				Close();
//			}
//		}
//	}
//}


////
////void CUserCardDlg::OnStnClickedStaticArt()
////{
//	if ( this->m_pLastestArticle )
//	{
//		MiniOpenUrl(m_pLastestArticle->szUrl);
//	}
//}
//
//void CUserCardDlg::OnBnClickedButtonPic()
//{
//int nItem = this->m_wndUserPic.GetCurrentItem();
//	if ( nItem!=-1 )
//	{
//		DWORD dwId = m_wndUserPic.GetItemData(nItem);
//		for ( int i=0; i<m_vecLastestPicture.size(); i++ )
//		{
//			if ( dwId==m_vecLastestPicture[i]->dwPictureID )
//			{
//				return;
//				CString strUrl;
//				strUrl.Format("http://%s.feiniu.com/photo/%u_%u.html",
//					m_pFriendInfo->GetUserName().c_str(),
//					m_vecLastestPicture[i]->nAlbumId,
//					m_vecLastestPicture[i]->dwPictureID);
//
//				MiniOpenUrl(strUrl);
//			}
//		}
//	}
//}
//
//// 点击头像区域
//void CUserCardDlg::OnBnClickedFaceIcon()
//{
//	// 如果当前显示的是当前登录用户信息，点击头像区域时，向MainFrame发送消息
//	if(m_pFriendInfo->GetId() == g_ImUserInfo.GetId())
//	{
//		::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_FACE_CLICKED, 0, 0);
//	}
//}
//
