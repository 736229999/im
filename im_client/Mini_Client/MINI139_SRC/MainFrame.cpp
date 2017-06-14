#include "stdafx.h"
#include "MainFrame.h"

#include "IMClient.h"

// CMainFrame 对话框
#include "im\saconfig.h"
#include "im\fsdialog.h"
#include "im\LoginDlg.h"
#include "im\VersionInfoDlg.h"
#include "im\sysmsgdlg.h"
#include "im\aboutboxdlg.h"
#include "im\dialogclusterAuth.h"
#include "im\userauthdlg.h"
#include "im\dailogclustercheck.h"
#include "im\userauthinfodlg.h"
#include "im\userauthdlg.h"
#include "im\messagehandlewnd.h"
#include "im\xtpacket.h"
#include "im\hintdlg.h"
#include "im\dialogshowhelp.h"
#include "im\contactinvitedlg.h"
#include "im\userguidedlg.h"
#include "im\AddEmotionDlg.h"
#include "im\139msgdlg.h"
#include "im\139NoteDlg.h"
#include "im\ChangeAccount.h"
#include "im\ClusterCreateDlg.h"
#include "im\selffacedlg.h"
#include "im\xtpacket.h"
#include "im\smsdlg.h"
#include "SystemSettingDlg.h"
#include "..\Im\MsgDlg.h"
#include "..\Im\FSDialog.h"
#include "imtest\TestDialog.h"
#include "ImTest\ImTest.h"
#include "NearCommCustDlg.h"
#include "MsgManageDlg.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "SpecifySendDlg.h"
#include "Im/UserCardDlg.h"
#include "tone.h"
#include "SelfInfoDlg.h"
#include "im\ScreenCtrl.h"
#include "im\DuiCounsel.h"
#include "im\DuiGuidePage.h"
#include "SkinPickerDialog.h"
#include "DuiMfFriendList.h"
#include "DuiMfClusterList.h"
#include "..\..\Common\Common.h"
#include "DuiSelfInfoDlg.h"
#include "DuiMfRecentList.h"
#include "im\UserCardDlg.h"
#include "im\ModfCommNameDlg.h"
#include "CExitMsgWarning.h"
#include "GroupSendDlg.h"
#include "Im\LocalData.h"
#include "CModPSW.h"
#include "CheckServiceLogWarn.h"
#include "CefWebUI.h"
#include "cef_task.h"
#include "cef_runnable.h"
#ifdef _CS_PLAT_EX_
#include "CSPlatDlg.h"
#endif
#include "improtocol.pb.h"
#include "CSPlatDlg.h"
#include "Im\DuiOrderNotice.h"
#include "im\DuiPicture.h"

//////异常记录
#include <Windows.h>
//#include <DbgHelp.h>
//// 添加对dbghelp.lib的编译依赖
//#pragma comment(lib, "dbghelp.lib")
extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////保存未读消息的好友ID和未读条数
//
//void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
//{
//	// 创建Dump文件
//	//
//	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	// Dump信息
//	//
//	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
//	dumpInfo.ExceptionPointers = pException;
//	dumpInfo.ThreadId = GetCurrentThreadId();
//	dumpInfo.ClientPointers = TRUE;
//
//	// 写入Dump文件内容
//	//
//	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
//	CloseHandle(hDumpFile);
//}
//////
//LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
//{	
//	// 这里弹出一个错误对话框并退出程序
//	CString strDump = _T("Test.dmp");
//	CreateDumpFile(strDump, pException);
//
//	FatalAppExit(-1,  _T("*** Unhandled Exception! ***"));
//	return EXCEPTION_EXECUTE_HANDLER;
//}

static long s_SearchEditStartChar = 0;
DuiLib::CMenuElementUI* AppendDuiSubMenu(CMenu *pSubMenu)
{
	DuiLib::CMenuElementUI* pSubFlatMuen = new DuiLib::CMenuElementUI;  
	int nMenuCount = pSubMenu->GetMenuItemCount();
	for(int i = 0; i < nMenuCount; ++i)
	{
		UINT nId = pSubMenu->GetMenuItemID(i);
		CString strMenuText;
		pSubMenu->GetMenuString(i, strMenuText, MF_BYPOSITION);
		CMenu *pTempSubMenu = pSubMenu->GetSubMenu(i);
		if(pTempSubMenu == NULL)
		{
			if(nId == 0 && strMenuText.IsEmpty())
			{
				DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
				pNew->SetLineType();
				pSubFlatMuen->Add(pNew);  
			}
			else
			{
				DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
				CString strName;
				strName.Format("%d", nId);
				pNew->SetName(strName);
				pNew->SetFixedHeight(24);
				pNew->SetText(strMenuText);  
				pSubFlatMuen->Add(pNew);  
			}
		}
		else
		{
			DuiLib::CMenuElementUI *pTempMenu = AppendDuiSubMenu(pSubMenu);
			pTempMenu->SetFixedHeight(24);
			pTempMenu->SetText(strMenuText);
			pTempMenu->SetShowExplandIcon(true);
			pSubFlatMuen->Add(pTempMenu);
		}
	}
	return pSubFlatMuen;
}

BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu)
{
	if(pMenu == NULL || pMenu->GetSafeHmenu() == NULL)
	{
		return FALSE;
	}

	DuiLib::CMenuUI* rootMenu = pDuiMenu->GetMenuUI();  
	if (rootMenu == NULL)  
	{  
		return FALSE;
	}

	rootMenu->RemoveAll();
	int nMenuCount = pMenu->GetMenuItemCount();
	for(int i = 0; i < nMenuCount; ++i)
	{
		UINT nId = pMenu->GetMenuItemID(i);
		CString strMenuText;
		pMenu->GetMenuString(i, strMenuText, MF_BYPOSITION);
		CMenu *pSubMenu = pMenu->GetSubMenu(i);
		if(pSubMenu == NULL)
		{
			if(nId == 0 && strMenuText.IsEmpty())
			{
				DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
				pNew->SetLineType();
				rootMenu->Add(pNew);  
			}
			else
			{
				DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
				CString strName;
				strName.Format("%d", nId);
				pNew->SetName(strName);
				pNew->SetFixedHeight(24);
				pNew->SetText(strMenuText);  
				rootMenu->Add(pNew);  
			}
		}
		else
		{
			DuiLib::CMenuElementUI *pTempMenu = AppendDuiSubMenu(pSubMenu);
			pTempMenu->SetFixedHeight(24);
			pTempMenu->SetText(strMenuText);
			pTempMenu->SetShowExplandIcon(true);
			rootMenu->Add(pTempMenu);
		}
	}

	//pDuiMenu->ResizeSubMenu();
	pDuiMenu->ResizeMenu();	

	return TRUE;
}

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);

static DWORD dwLoginReqTick = 0;

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));//这个消息是系统开始菜单，任务栏创建时发出的
bool g_bFirstLogin = false;

#define STATUS_NOT_LOGIN 0
#define STATUS_LOGINING  1
#define STATUS_LOGIN_SUCCESS 2

int			CMainFrame::s_nLastHeight = 0;
int			CMainFrame::g_bNetworkOK = TRUE;

static CString                  g_strMini139UpdateFile = "";

// 打开聊天窗口的消息响应
#define WM_OPEN_CUST_MSGDLG          WM_USER+0x128

UINT Mini139AutoUpdateProc(LPVOID p)//后台自动更新线程
{
    CString strVersionUrl = (LPCSTR)p;

	::CoInitialize(0);

	if ( DownloadMini139(strVersionUrl,g_strMini139UpdateFile) )
	{
		::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_THREAD_NOTIFY,0,0);
	}
	
	::CoUninitialize();

	return 0;
}

void InitUserInfoX(IUserInfoX *pUserInfoX,USER_INFO *pUser)
{
	pUserInfoX->Age     = pUser->age;
	pUserInfoX->Sex     = pUser->sex;
	pUserInfoX->Email   = pUser->email;
	pUserInfoX->FaceUrl = pUser->face_url;
	pUserInfoX->MessageCount = pUser->message_count;
	pUserInfoX->NickName     = pUser->nickname;
	pUserInfoX->UserID       = pUser->GetId();
	pUserInfoX->UserName     = pUser->GetUserName().c_str();
	pUserInfoX->Password     = pUser->GetPassword().c_str();
	pUserInfoX->GoldMoney    = pUser->gold_money;
}

CMainFrame::CMainFrame()
{
	m_nUnReadCnt    = 0;
	m_bLogined      = false;
	m_nLoginStatus  = STATUS_NOT_LOGIN;
	m_nUserStatus   = g_ImUserInfo.bHideLogin? XTREAM_HIDE : XTREAM_ONLINE;

	m_nifData.hIcon		=	 NULL;	
	m_nWakeupVK         =     0;
	m_hMini139UpdateThread  = NULL;
	m_hMiniWebSession       = NULL;
	m_hNotifyWebUserSign	= NULL;

	m_bHide = FALSE;
	m_alignType = ALIGN_NONE ;
	m_bForceShow = TRUE;
	m_nScreenW = GetSystemMetrics(SM_CXSCREEN);
	m_hAccel	   = LoadAccelerators(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME_HOTKEY));

	m_pLastAfter = NULL;

	memset(m_szExternPic,0,sizeof(m_szExternPic));

	m_bHeartTimeOut = false;
	m_bFriendWnd   = TRUE;///默认显示好友页面
#ifdef _BUSINESS_VESION
	m_bHasUpdateNearCommCust = false;	// 是否已经更新了后台“最近联系客户”	
#endif

	m_nHangupIndex = 1;
	m_nNumThenHangup = -1;

	m_pSecifySendDlg = NULL;
	m_pSelfInfoDlg = NULL;
	m_pDuiGuidePage = NULL;
	m_bNeedSort = false; 
	m_nLastReqSeq = 0;
	m_bExit = FALSE;
}

CMainFrame::~CMainFrame()
{
	
}

BEGIN_XT_MESSAGE_MAP(CMainFrame)
	// 修改用户资料
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK,OnUserInfoModified)
	// 修改在线状态
	ON_XT_COMMAND(CMD_STATUS_MOD_ACK,OnStatusModAck)
	// 登出
	ON_XT_COMMAND(CMD_LOGOUT,OnLogOut)
	// 收到服务器消息
	ON_XT_COMMAND(CMD_SERVERMSG_IND,OnRecvSvrMsg)
	// 获取用户详细信息
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
	// 获取好友列表
	ON_XT_COMMAND(CMD_FRIENDLIST_ACK,OnFriendListAck)
	//获取用户状态
	ON_XT_COMMAND(CMD_GET_USER_STATUS_ACK,OnGetUserStatusAck)
	// 发生网络错误
	ON_XT_COMMAND(CMD_NETWORK_ERR,OnNetWorkErr)
	// web通知
	ON_XT_COMMAND(CMD_WEB_IND,OnWebInd)
	
#ifdef _BUSINESS_VESION
	// 更新最近联系客户应答
	ON_XT_COMMAND(CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK, OnUpdateNearCommCustInfoAck)
	ON_XT_COMMAND(CMD_CUSTSER_TRAN_REQ_IND, OnRecvCustSerTranReqInd)
#endif
	ON_XT_COMMAND(CMD_FORCE_OFFLINE_CS_IND, OnForceOfflineCsInd)
	// 用户验证
	ON_XT_COMMAND(CMD_USERAUTH_IND,OnUserAuthInd)
	// 验证请求
	ON_XT_COMMAND(CMD_USERAUTH_ACK,OnUserAuthAck)
	// 修改好友分组
	ON_XT_COMMAND(CMD_FRIENDGROUP_MOD_ACK,OnFriendGroupModAck)
	// 心跳
	ON_XT_COMMAND(CMD_DIRECTORY_ACK,OnHeartBeat)
	// 好友状态改变通知
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnFriendStatusChange)
	// 收到单聊消息
	ON_XT_COMMAND(CMD_P2PMSG_RECV,OnRecvP2PMsg)
	// p2p聊天应答
	ON_XT_COMMAND(CMD_P2PMSG_ACK,OnP2PMsgAck)
	// 获取分组信息
	ON_XT_COMMAND(CMD_GROUPINFO_GET_ACK,OnGroupInfoGet)
	ON_XT_COMMAND(CMD_STATUS_GET_ACK,OnStatusGetAck)
	// 收到群聊天消息
	ON_XT_COMMAND(CMD_CLUSTERMSG_RECV,OnRecvClusterMsg)
	// 修改组
	ON_XT_COMMAND(CMD_GROUPINFO_MOD_ACK,OnGroupModAck)
	// 修改好友备注
	ON_XT_COMMAND(CMD_MOD_FCNAME_ACK,OnModFCNameAck)
	// 获取好友基本资料
	ON_XT_COMMAND(CMD_GET_FRIEND_BASICINFO_ACK,OnBasicInfoAck)
	// 接收到p2p测试
	ON_XT_COMMAND(CMD_P2P_TEST_REQ,OnRecvP2pTest)
	// 接受到返回的p2p测试应答
	ON_XT_COMMAND(CMD_P2P_TEST_ACK,OnRecvP2pTestAck)
	// 好友要求删除自己
	ON_XT_COMMAND(XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL,OnDelFriendReq)
	// 发送文件通知              
	ON_XT_COMMAND(CMD_SEND_FILE_REQ,OnRecvSendFileReq)
	// 更新好友信息
	ON_XT_COMMAND(CMD_FRIEND_INFO_IND, OnFriendInfoInd)
	ON_XT_COMMAND(CMD_GET_CLIST_ACK,OnClusterListAck)
	// 群基本信息获取
	ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnClusterInfoAck)
	// 设置群成员
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	ON_XT_COMMAND(CMD_CLUSTER_APPLY_ACK,OnClusterApplyAck)
	// 收到群验证消息
	ON_XT_COMMAND(CMD_CLUSTER_AUTH_IND,OnClusterAuthInd)
	// 群成员变动通知
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	//邀请好友入群通知
	ON_XT_COMMAND(CMD_CLUSTER_INVITE_IND,OnClusterInviteInd);
	//好友是否入群通知
	ON_XT_COMMAND(CMD_CLUSTER_AGREE_IND,OnClusterAgreeInd);
	// 群解散通知
	ON_XT_COMMAND(CMD_CDISMISS_IND,OnClusterDismissInd)
	ON_XT_COMMAND(CMD_SET_CINFO_ACK,OnSetClusterInfoAck)

	//获取群名片信息
	ON_XT_COMMAND(CMD_GET_PROFILE_NAME_ACK, OnGetCMProfileAck)
	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK, OnSearchUser)
	// 获取群成员列表
	ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnClusterMembersAck)
#ifdef _CS_PLAT_EX_
	ON_XT_COMMAND(CMD_WAITQUEU_CHANGE, OnWaitQueuChange)
	ON_XT_COMMAND(CMD_RECEPTE_CUST_ACK, OnRecepteCustAck)
	ON_XT_COMMAND(CMD_BUYER_OFFLINE_IND, OnBuyOfflineInd)
#endif
	// 登录应答
	ON_XT_COMMAND(CMD_LOGIN_ACK,OnLoginAck)	// 用于网络断开重新登录
#ifdef _BUSINESS_VESION
	ON_XT_COMMAND(CMD_GETMSGREMIND_ACK, OnGetOrderSubscibe)
#endif
END_XT_MESSAGE_MAP()

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	// 关闭其他窗口
	LastOpera();

	if(m_hAccel)
	{
		DestroyAcceleratorTable(m_hAccel);
	}

	if(m_nifData.hIcon )
	{
		DestroyIcon(m_nifData.hIcon);
		m_nifData.hIcon = NULL;
	}

	m_pipeCallMe.Close();

	g_pProtocol->UnRegistCmd(hWnd);
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);

	if(m_pDuiGuidePage != NULL)
	{
		delete m_pDuiGuidePage;
		m_pDuiGuidePage = NULL;
	}

	FRIEND_LIST::iterator itr;
	itr = g_lsFriend.begin();
	while(itr != g_lsFriend.end())
	{
		FRIEND_INFO* pFriend = *itr;
		if(pFriend != NULL)
		{
			delete pFriend;
			pFriend = NULL;
		}
		g_lsFriend.erase(itr);
		itr = g_lsFriend.begin();
	}

	if(m_bExit)
	{
		::PostQuitMessage(0);
	}
}

std::tr1::shared_ptr<CMainFrame> CMainFrame::pInstance = NULL;
std::tr1::shared_ptr<CMainFrame> CMainFrame::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CMainFrame>(new CMainFrame());
#ifdef _CS_PLAT_EX_
		pInstance->Create(GetDesktopWindow(), FEINIU_CS_PLAT, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
#elif defined _WORK_EX_
		pInstance->Create(GetDesktopWindow(), CLIENT_NAME_WORK, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
#else
		pInstance->Create(GetDesktopWindow(), CLIENT_NAME_SELLER, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
#endif

		
		CRect rcSave;
		g_LocalData.GetMainFramePostion(rcSave);
		::SetWindowPos(pInstance->GetHWND(), NULL, rcSave.left, rcSave.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		pInstance->ShowWindow(true);
		//登录成功后初始化默认表情
		LoadCustomFace();

		//cookie中设置token
		pInstance->SetCookieToken();
#ifdef _CS_PLAT_EX_
		g_LocalData.LoadFastWordInfo();//调用接口获取快捷短语
		g_LocalData.LoadTranMsg();//获取转接消息
		//获取分组信息
//#if !defined(_VER_PREVIEW_) || !defined(_VER_ONLINE_)
		pInstance->GetCsTeamInfo();
#endif

#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		//数据库读取订单消息
		g_LocalData.ReadOrderMsg();
		//获取订单订阅
		pInstance->GetOrderSubscibe();
		//获取离线订单订单
		pInstance->GetOrderMsgOffline();
#endif
		//读取快捷短语
		g_LocalData.ReadFastWordInfo();
//只有商家版才显示右侧页
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		//登录成功显示新版特性
		pInstance->ShowGuiPage();
#endif
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		g_pProtocol->GetOfflineMsgReq();	
#endif
		SetTimer(pInstance->GetHWND(), TIMER_SEND_MSG_QUEUE,1000,NULL);
		
		//暂时在登录的时候设置一次文件目录防止更改用户时无法获取聊天记录
		//CString strModulePath =  g_LocalData.GetPath();
		//g_spaceData.SetPath(strModulePath);
		//g_LocalData.SetPath(strModulePath);

		//登录成功，是否显示上次未读消息
		//加载默认配置
		WORD nExitWarn = 0;
		g_LocalData.GetLoginUnReadMsgCfg(nExitWarn);
		if(nExitWarn!=0)//弹出上次退出时的未读消息提醒
		{
			g_LocalData.GetUnReadWndCnt(nExitWarn);//再次登录哞哞时
			{
				if(nExitWarn!=0)////打开之前的未读消息
				{
					//CExitMsgWarningDlg::Show(LOGIN_AGAIN,pInstance->GetHWND());
					std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(LOGIN_AGAIN,pInstance->GetHWND());
					pWnd->ShowModal();
				}
			}
		}	
#ifdef _CS_PLAT_EX_
		//客服工作台未处理的用户
		vector<FRIEND_INFO*> vecFriend;
		g_LocalData.GetCurrentBuyer(vecFriend);
		if(vecFriend.size() > 0)
		{
			CMessageBox::Model(NULL, _T("检测到有未处理的买家,点击确定将打开未处理的会话！"), IM_CAPTION, MB_OK);
			//打开未处理的聊天窗口
			for(int nFriend = 0; nFriend < vecFriend.size(); nFriend++)
			{
				if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 0)
				{
					FRIEND_INFO* pFriend = vecFriend[nFriend];
					BOOL bIsExist = FALSE;
					for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
					{
						CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];

						if(pFriend->GetId() == pDlg->GetFriendInfo()->GetId())
						{
							bIsExist = TRUE;
							pDlg->UpdateUrl();
						}
					}
					if(!bIsExist)
					{
						CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
						pDlg->ShowWindow(false);
					}
				}
				else
				{
					FRIEND_INFO* pFriend = vecFriend[nFriend];
					CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
					if(nFriend == 0)
					{
						if(CDuiCSPlatDlg::GetInstance() != NULL)
						{
							CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
						}
					}
					else
					{
						pDlg->ShowWindow(false);
					}
				}
			}
		}
#endif
   }
	return pInstance;
}

void CMainFrame::ShowGuiPage()
{
	if(g_LocalData.ReadGuidePageSetting("GuidePage1500"))
	{
		vector<CString> vecPage;
		vecPage.push_back("GuidePage2200\\page1.jpg");
		vecPage.push_back("GuidePage2200\\page2.jpg");

		DuiLib::CPaintManagerUI::SetInstance(AfxGetInstanceHandle());                    // 指定duilib的实例
		DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());    // 指定duilib资源的路径，这里指定为和exe同目录
		if(m_pDuiGuidePage == NULL)
		{
			m_pDuiGuidePage = new CDuiGuidePage;
			m_pDuiGuidePage->SetPage(vecPage);
			m_pDuiGuidePage->Create(NULL, _T("新版特性"), NULL, NULL/*WS_EX_STATICEDGE | WS_EX_APPWINDOW*/, 0, 0, 700, 496);
		}
		m_pDuiGuidePage->CenterWindow();
		m_pDuiGuidePage->ShowWindow(true);


		g_LocalData.SaveGuidePageSetting("GuidePage1500");
	}
}

void CMainFrame::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()) && pInstance->m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(pInstance->GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(pInstance->GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,116' dest='0,0,%d,116'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,116' dest='0,0,%d,116' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}

		// 设置page页前景图片
		DuiLib::COptionUI *pRecentPage = static_cast<DuiLib::COptionUI*>(pInstance->m_PaintManager.FindControl("recentBtn"));
		DuiLib::COptionUI *pFriendPage = static_cast<DuiLib::COptionUI*>(pInstance->m_PaintManager.FindControl("friendBtn"));
		DuiLib::COptionUI *pClusterPage = static_cast<DuiLib::COptionUI*>(pInstance->m_PaintManager.FindControl("clusterBtn"));
		CRect rc = pRecentPage->GetPos();
		if (rc.Width() == 0)
		{
			return;
		}
		TCHAR szBuf[MAX_PATH] = {0};
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='SysBtn\\historical_record_normal-.png' dest='%d,%d,%d,%d'"),
			(rc.Width()-20)/2, (rc.Height()-20)/2, (rc.Width()-20)/2+20, (rc.Height()-20)/2+20);
		pRecentPage->SetForeImage(szBuf);
		memset(szBuf, 0, MAX_PATH);
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='SysBtn\\historical_record_pressed.png' dest='%d,%d,%d,%d'"),
			(rc.Width()-20)/2, (rc.Height()-20)/2, (rc.Width()-20)/2+20, (rc.Height()-20)/2+20);
		pRecentPage->SetSelectedForedImage(szBuf);

		rc = pFriendPage->GetPos();
		memset(szBuf, 0, MAX_PATH);
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='SysBtn\\friends_normal.png' dest='%d,%d,%d,%d'"),
			(rc.Width()-20)/2, (rc.Height()-20)/2, (rc.Width()-20)/2+20, (rc.Height()-20)/2+20);
		pFriendPage->SetForeImage(szBuf);
		memset(szBuf, 0, MAX_PATH);
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='SysBtn\\friends_pressed.png' dest='%d,%d,%d,%d'"),
			(rc.Width()-20)/2, (rc.Height()-20)/2, (rc.Width()-20)/2+20, (rc.Height()-20)/2+20);
		pFriendPage->SetSelectedForedImage(szBuf);

		rc = pClusterPage->GetPos();
		memset(szBuf, 0, MAX_PATH);
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='SysBtn\\group_normal.png' dest='%d,%d,%d,%d'"),
			(rc.Width()-20)/2, (rc.Height()-20)/2, (rc.Width()-20)/2+20, (rc.Height()-20)/2+20);
		pClusterPage->SetForeImage(szBuf);
		memset(szBuf, 0, MAX_PATH);
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='SysBtn\\group_pressed.png' dest='%d,%d,%d,%d'"),
			(rc.Width()-20)/2, (rc.Height()-20)/2, (rc.Width()-20)/2+20, (rc.Height()-20)/2+20);
		pClusterPage->SetSelectedForedImage(szBuf);

		// 更新名字宽度
		GetWindowRect(pInstance->GetHWND(), &rc);
		int nMaxWidth = rc.Width() - 100;
		int nWidth = pInstance->m_plbUserName->CalcRealWidth();
		if(nWidth > nMaxWidth)
			nWidth = nMaxWidth;
		pInstance->m_plbUserName->SetFixedWidth(nWidth);
	}
}

void CMainFrame::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->CLOSE();
	}
}

LRESULT CMainFrame::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg) return 0;
	else if(WM_XT_CMD_NOTIFY == uMsg)	OnXtMessage(wParam, lParam);	
	else if(WM_TIMER == uMsg)	OnTimer(wParam);
	else if(WM_IMFRIEND_LIST_NOTIFY == uMsg)	OnFriendListNotify(wParam, lParam);
	else if(WM_TRAYICON == uMsg)	OnTaskIconClick(wParam, lParam);
	else if(WM_CHECKVERSION == uMsg)	OnCheckVersion(wParam, lParam);
	else if(WM_NETWORK_ERROR == uMsg)	OnNetWorkError(wParam, lParam);
	else if(WM_LOGIN_FAILED == uMsg)	OnLoginFailed(wParam, lParam);
	else if(WM_IM_OK == uMsg)	OnImOk(wParam, lParam);
	else if(WM_USER_HEAD_NOTIFY == uMsg)	OnWebIconDownload(wParam, lParam);
	else if(WM_HEART_TIMEOUT == uMsg)	OnHeartTimeOut(wParam, lParam);
	else if(WM_TRAYICON_UPDATE == uMsg)	OnTrayIconUpdate(wParam, lParam);
	else if(WM_THREAD_NOTIFY == uMsg)	OnThreadNotify(wParam, lParam);
	else if(WM_HINTDLG_CLICK == uMsg)	OnHintDlgClick(wParam, lParam);
	else if(WM_WEB_CALLME == uMsg)	OnCallMe(wParam, lParam);
	else if(WM_WEBPICTURE_NOTIFY == uMsg)	OnWebPictureNotify(wParam, lParam);
	else if(WM_IME_SETCONTEXT == uMsg)	OnIMESetContext(wParam, lParam);
	else if(WM_EXIT_MAINFRM == uMsg)	OnExitMainFrm(wParam, lParam);
	else if(WM_OPEN_SELLER_WIN == uMsg)	OpenSellerMsgWin(wParam, lParam);
	else if(WM_OPEN_WORK_WIN == uMsg)	OpenWorkMsgWin(wParam, lParam);
	else if(WM_OPEN_CS_INSERT_MSG_WIN == uMsg)	OpenCsInsertMsgWin(wParam, lParam);
	else if(WM_HOTKEY == uMsg)	OnHotKey(wParam, lParam);
	else if(WM_ADD_EMOTION == uMsg)	OnAddEmotion(wParam, lParam);
	else if(WM_COPY_URL == uMsg)	OnCopyUrl(wParam, lParam);
	else if(WM_SYSTEM_SETTING_MSG == uMsg)	OnSelfMsgSystemSettingDlgMsg(wParam, lParam);
	else if(WM_SPEC_SEND_DLG_MSG == uMsg)	OnSpecSendDlgMsg(wParam, lParam);
	else if(WM_FACE_CLICKED == uMsg)	OnUserCardDlgFaceClicked(wParam, lParam);
	else if(WM_MOD_BACKNAME == uMsg)	OnModBackName(wParam, lParam);
#ifdef _BUSINESS_VESION
	else if(WM_OPEN_CUST_MSGDLG == uMsg)	OnOpenCustMsgDlg(wParam, lParam);
	else if(WM_OPEN_ORDER_MSG_DLG == uMsg)	OnOpenOrderMsgDlg(wParam, lParam);
#endif
	else if(WM_IMOPERATION == uMsg)	OnImOperation(wParam, lParam);

	else if(WM_KEYDOWN == uMsg)
	{
		switch (wParam)
		{
		case VK_RETURN:
			{
				if (m_pEditSearchValue->IsFocused())
				{	// 按回车键打开聊天窗口
					OpenCharWnd();
				}
			}
		
		default:
			break;
		}
	}
	else if(WM_SIZE == uMsg)
	{
		UpdateBk();
	}
	else if(WM_MOVING == uMsg)
	{
		RECT *pRc = (RECT *)lParam;
		OnMoving(0, pRc);
	}
	else if(WM_MOVE == uMsg)
	{
		int xPos = (int)(short) LOWORD(lParam);   // horizontal position 
		int yPos = (int)(short) HIWORD(lParam);   // vertical position 
		OnMove(xPos, yPos);
	}
	else if(WM_SYSCOMMAND == uMsg)
	{
		if(wParam == SC_CLOSE)
		{
			ShowWindow(false);
			return 0;
		}
	}
	else if(WM_POWERBROADCAST == uMsg)
	{	
		if(wParam == PBT_APMRESUMEAUTOMATIC)
		{	// 从休眠中恢复
#if _DEBUG
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			CString sTime;
			sTime.Format("\n\n%d:%d:%d 恢复\n\n",tm.wHour,tm.wMinute,tm.wSecond);
			TRACE(sTime);
#endif
			SetTimer(GetHWND(), TIMER_RELOGIN,3000,NULL);
		}
		else if(wParam == PBT_APMSUSPEND)
		{	// 即将进入休眠
#if _DEBUG
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			CString sTime;
			sTime.Format("\n\n%d:%d:%d 休眠\n\n",tm.wHour,tm.wMinute,tm.wSecond);
			TRACE(sTime);
#endif
			m_bDisconnectFlag = true;
			m_nPreOnlineStatus = g_ImUserInfo.status;

			g_ImUserInfo.status = XTREAM_OFFLINE;
			m_bLogined=false;
			m_nLoginStatus=0;

			XT_LOGOUT req;
			req.id = g_ImUserInfo.GetId();
			req.authData = 0;
			g_pProtocol->SendReq(req);

			SetTaskIconTip("与服务器失去联系");
			ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);

			g_pProtocol->Stop();

			KillTimer(GetHWND(), TIMER_BEAT);			
		}
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CMainFrame::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	// 主窗口不显示在任务栏中
	::SetWindowPos(GetHWND(), CWnd::wndTop.GetSafeHwnd(),0,0,0,0,NULL);
	DWORD dwStyle = ::GetWindowLong(GetHWND(), GWL_EXSTYLE);
	DWORD dwNewStyle = (dwStyle & ~WS_EX_APPWINDOW) | WS_EX_TOOLWINDOW;
	::SetWindowLong(GetHWND(), GWL_EXSTYLE, dwNewStyle); 

	SetIcon(IDR_MAINFRAME);

	m_pLabelUmMsg = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelUnMsg"));
	m_pFriendList = static_cast<CDuiFriendList*>(m_PaintManager.FindControl("lsFriend"));
	m_pFriendPage = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("friendBtn"));
	m_plbUserName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lbUserName"));
	m_pbtnStatus = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnStatus"));
	m_pEditSign = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("editSign"));
	m_pEditSign->SetMaxChar(50);
	m_pUserImg = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl("userimg"));
	m_pHead = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindControl("btnModSelfInfo"));////头像

	m_pLayoutSearch = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabSearch"));
	m_pEditSearchView = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("editView"));
	m_pBtnClearSearch = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnCancelSearch"));
	m_pEditSearchValue = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("editSearchValue"));

	m_pLayoutPage = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("mainTab"));
	m_pClusterList = static_cast<CDuiClusterList*>(m_PaintManager.FindControl("lsGroup"));

	m_pSearchList = static_cast<CSearchListUI*>(m_PaintManager.FindControl("searchlist"));
	m_pTab = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));
	m_pBtnTempSession = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnTempSession"));

	m_pLayoutSearcePage = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabPagesAndSearchList"));
	m_pTitle = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("title"));
	m_pBtnModSelfInfo = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindControl("btnModSelfInfo"));
	m_pRecentList = static_cast<CDuiRecentList*>(m_PaintManager.FindControl("lsRecent"));
	m_pbtnAddCluster  = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnAddFriend")); 
	
#ifdef _CS_PLAT_EX_
	m_pbtnAddCluster->SetToolTip("添加/查找群");
	m_pbtnSellerHome = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnSellerHome")); 
	m_pbtnSellerHome->SetVisible(false);

	//客服工作台不显示最近联系人图标
	m_pbtnNearCust = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnNearCust"));
	m_pbtnNearCust->SetVisible(false);
#endif

#ifdef _WORK_EX_
	DuiLib::CButtonUI * pBtnHelp = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("helpbtn")); 
	pBtnHelp->SetVisible(false);

	m_pbtnSellerHome = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnSellerHome")); 
	m_pbtnSellerHome->SetVisible(false);

	//客服工作台不显示最近联系人图标
	m_pbtnNearCust = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnNearCust"));
	m_pbtnNearCust->SetVisible(false);
#endif
	m_bExit = FALSE;
	m_pScreenCtrl = NULL;
	UpdateBk();
	/////加载热键
	SetWndHotKey();
	g_pMessageHandleWnd->SetNotifyIconData(&m_nifData);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SERVERMSG_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIENDLIST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_USER_STATUS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_NETWORK_ERR,m_hWnd);
	g_pProtocol->RegistCmd(CMD_WEB_IND,m_hWnd);
#ifdef _BUSINESS_VESION
	g_pProtocol->RegistCmd(CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK, m_hWnd);
	g_pProtocol->RegistCmd(CMD_CUSTSER_TRAN_REQ_IND, m_hWnd);
#endif
	g_pProtocol->RegistCmd(CMD_FORCE_OFFLINE_CS_IND, m_hWnd);
	g_pProtocol->RegistCmd(CMD_GROUPINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERAUTH_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERAUTH_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIENDGROUP_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_DIRECTORY_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2PMSG_RECV,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2PMSG_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTERMSG_RECV,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GROUPINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MOD_FCNAME_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_FRIEND_BASICINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2P_TEST_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2P_TEST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SEND_FILE_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIEND_INFO_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CLIST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_APPLY_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_AUTH_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CMEMBERS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_INVITE_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_AGREE_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CDISMISS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_PROFILE_NAME_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CMEMBERS_ACK,m_hWnd);
#ifdef _CS_PLAT_EX_
	g_pProtocol->RegistCmd(CMD_WAITQUEU_CHANGE,m_hWnd);
	g_pProtocol->RegistCmd(CMD_RECEPTE_CUST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_BUYER_OFFLINE_IND,m_hWnd);
#endif
	g_pProtocol->RegistCmd(CMD_LOGIN_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GETMSGREMIND_ACK, m_hWnd);

	m_nScreenW = GetSystemMetrics(SM_CXSCREEN);
	m_nScreenH = GetSystemMetrics(SM_CYSCREEN);
	// 设置接触边缘时自动隐藏
	WORD nHide = 0;
	g_LocalData.GetHideWinConfig(nHide);//是否自动隐藏
	if(nHide!=0)
	{
		SetTimer(GetHWND(), TIMER_HIDE,AUOT_CHECK_TIME,NULL);
	}
	else
	{
		KillTimer(GetHWND(), TIMER_HIDE);
	}
	//////////////////////////////////////////
#ifdef _BUSINESS_VESION
  #ifdef _CS_PLAT_EX_
	m_pTitle->SetText(FEINIU_CS_PLAT);    //飞牛客服平台
  #elif defined _WORK_EX_
	m_pTitle->SetText(CLIENT_NAME_WORK);//工作版
  #else
	m_pTitle->SetText(CLIENT_NAME_SELLER);//商家版
  #endif	
#else
	m_pTitle->SetText(CLIENT_NAME_BUYER); //买家版
#endif	

	AddTaskIcon();
	
	// 更新个人信息
	m_plbUserName->SetText(UserInfoToStr(&g_ImUserInfo));
	switch(g_ImUserInfo.status)
	{
	case XTREAM_LEAVE:
		{
			m_pbtnStatus->SetBkImage("FriendStatus\\status_leave.png");
		}
		break;
	case XTREAM_BUSY:
		{
			m_pbtnStatus->SetBkImage("FriendStatus\\status_busy.png");
		}
		break;
	case XTREAM_HIDE:
		{
			m_pbtnStatus->SetBkImage("FriendStatus\\status_stealth.png");
		}
	case XTREAM_ONLINE:
	case XTREAM_HANGUP:
		{
			m_pbtnStatus->SetBkImage("FriendStatus\\status_online.png");
		}
		break;
	case XTREAM_OFFLINE:
		{
			m_pbtnStatus->SetBkImage("FriendStatus\\status_offline.png");
		}
		break;
	}

	CString strSign = g_ImUserInfo.user_sign;
	if(!strSign.IsEmpty())
	{
		m_pEditSign->SetText(strSign);
	}

	CString strFile = g_webUserHeadIconProcess.GetFaceFile(&g_ImUserInfo);
	CString strGifFile =  "";
	CString strPath =  g_ImUserInfo.face_url;
	if(strPath.Find(".gif")!=-1)
	{
		strGifFile =  strFile + ".gif";////文件改名
		if ( !FileExist(strGifFile) )////不存在则创建
		{
			CopyFile(strFile,strGifFile,FALSE);
		}
	}
	if ( FileExist(strFile) )
	{
		if ( FileExist(strGifFile) )
		{
			m_pHead->SetBkImage("");
			m_pHead->SetNormalGifFile(strGifFile);
		}
		else
		{
			string strHeadIcon = CutRoundImg(strFile, 50, 50);
			m_pHead->SetBkImage(strHeadIcon.c_str());
			m_pHead->SetNormalGifFile("");
		}
	}
	m_bIsHangupSelect = false;

	// 刷新列表
	InitializeIm();
	StartHeartBeart();
	m_dwPrevBeatTick = GetTickCount();
	LoadLoacalClusterList();

	////空闲时状态切换
	g_LocalData.GetWakeupConfig(m_nWakeupVK,m_nStatus);//时间和状态
	if(m_nWakeupVK  > 0)
	{
		SetTimer(GetHWND(), TIMER_WAKEUP ,m_nWakeupVK*60*1000,NULL);///毫秒触发
	}

	CRect rc;
	g_LocalData.GetMainFramePostion(rc);

	CMainFrame::s_nLastHeight = rc.Height();
	MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
	ShowWindow(true);
	// 确定窗体停靠方式
	NearSide();

	g_hMainWnd = GetHWND();
	theApp.m_pMainWnd = CWnd::FromHandle(GetHWND());
/*
	// 开启线程检测网络连接
	unsigned threadID = 0;
	_beginthreadex(NULL, 0, ThreadCheckNetwork, NULL, 0, &threadID);
*/
}

DuiLib::CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendTreeView")) == 0)
	{
		return new CDuiFriendList(GetHWND(), m_PaintManager);
	}
	else if (_tcsicmp(pstrClass, _T("ClusterTreeView")) == 0)
	{
		return new CDuiClusterList(GetHWND(), m_PaintManager);
	}
	else if (_tcsicmp(pstrClass, _T("SearchList")) == 0)
	{
		return new CSearchListUI(m_PaintManager);
	}
	else if(_tcsicmp(pstrClass, _T("RecentList")) == 0)
	{
		return new CDuiRecentList(GetHWND(), m_PaintManager);
	}
	else if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
	{
		return	new DuiLib::CButtonGifUI();
	}
	return NULL;
}

// 消息
void CMainFrame::Notify(DuiLib::TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("setfocus")) == 0)
	{
		if(msg.pSender == m_pEditSearchView)
		{
			s_SearchEditStartChar = 0;
			m_pLayoutSearch->SelectItem(1);
			SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);
		}
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender->GetName() == "btnCancelSearch")
		{
			m_pLayoutSearch->SelectItem(0);
			m_pLayoutSearcePage->SelectItem(0);
			m_pEditSearchValue->SetText("");
			m_pFriendList->SetFocus();
		}
		else if(msg.pSender->GetName() == "btnStatus")
		{	// 修改状态 改为离线也可以点开菜单手动上线
			//if(XTREAM_OFFLINE == g_ImUserInfo.status)
			//{	// 如果当前状态为离线，取消点击操作
			//	return;
			//}
			RECT rcBtn = msg.pSender->GetPos();
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint point(rcBtn.left,rcBtn.bottom+5);
			ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
		}
		else if(msg.pSender->GetName() == "recentBtn")
		{
			m_bFriendWnd = FALSE;
			m_pLayoutPage->SelectItem(0);
		}
		else if(msg.pSender->GetName() == "friendBtn")
		{
			m_bFriendWnd = TRUE;
			m_pLayoutPage->SelectItem(1);
		}
		else if(msg.pSender->GetName() == "clusterBtn")
		{
			m_bFriendWnd = FALSE;
			m_pLayoutPage->SelectItem(2);
		}
		else if(msg.pSender->GetName() == "helpbtn")
		{
			////修改帮助文档下载链接
 #if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
 #ifdef _CS_PLAT_EX_
 			OpenHtmlPage("http://im.feiniu.com/doc/help-cs.docx");
 #else
 			OpenHtmlPage("http://im.feiniu.com/doc/help-moumou.pdf"); 
 #endif
  			
 #else
 #ifdef _CS_PLAT_EX_
 			OpenHtmlPage("http://messagecenter.beta.fn.com/imindex/doc/help-cs.docx"); 
 #else
 			OpenHtmlPage("http://messagecenter.beta.fn.com/imindex/doc/help-moumou.pdf"); 
 #endif
 #endif
		}
		else if(msg.pSender->GetName() == "skinbtn")
		{
			CSkinPickerDialog::Show();
		}
		else if(msg.pSender->GetName() == "btnSysSetting")
		{
			OnBnClickedBtnSysSet();
		}
		else if(msg.pSender->GetName() == "btnMenu")
		{
			OnBnClickedBtnSysMenu();
		}
		else if(msg.pSender->GetName() == "btnMsgManage")
		{
			OnBnClickedBtnSysMsg();
		}
		else if(msg.pSender->GetName() == "btnAddFriend")
		{
			OnBnClickedBtnSysAdd();
		}
		else if(msg.pSender->GetName() == "btnNearCust")
		{
			CNearCommCustDlg::Show();
		}
		else if(msg.pSender->GetName() == "btnSellerHome")
		{
#ifdef _VER_PREVIEW_
			OpenHtmlPage("http://merchant.feiniu.com");	
#elif _VER_ONLINE_
			OpenHtmlPage("http://merchant.feiniu.com");
#else	//beta环境
			OpenHtmlPage("http://merchant.beta.fn.com");
#endif
		}
		else if(msg.pSender == m_pBtnTempSession)
		{
			string strText = m_pBtnTempSession->GetText().GetData();
			int nBgnIndex = strText.find_first_of('"');
			int nEndIndex = strText.find_last_of('"');
			string strFind = strText.substr(nBgnIndex+1, nEndIndex-nBgnIndex);
			XT_XTREAM_SEARCH_REQ req;
			req.search_type = XTREAM_SEARCH_PRECISION;
			strcpy(req.user_name, strFind.c_str());

			g_pProtocol->SendReq(req);
			m_nLastReqSeq = g_pProtocol->GetCurSendSequnceId()-1;
		}
		else if(msg.pSender->GetName() == "btnModSelfInfo")
		{
			OnBnClickedFaceIcon();
		}
		else if(msg.pSender->GetName() == "btnCreateCluster")
		{
			OnClusterCreate();
		}
		else if(msg.pSender->GetName() == "closebtn")
		{
			//ShowWindow(false);
			OnMenuExit();
			return;
		}
		else if(msg.pSender->GetName() == "minbtn")
		{
			ShowWindow(false);
			return;
		}
	}
	else if(_tcsicmp(msg.sType, DUI_MSGTYPE_RBUTTONDOWN) == 0)
	{	// 右键点击
		if(strcmp(msg.pSender->GetClass(), "TreeNodeUI") == 0)
		{
			if(m_pLayoutPage->GetCurSel() == 1)
			{
				OnFriendListNotify(NM_FL_RBUTTONUP, msg.lParam);
			}
			else if(m_pLayoutPage->GetCurSel() == 2)
			{
				OnGroupListNotify(NM_CL_RBUTTONUP, msg.lParam);
			}
		}
		else if(strcmp(msg.pSender->GetClass(), "ListContainerElementUI") == 0)
		{
			if(m_pLayoutPage->GetCurSel() == 0)
			{
				OnRecentListRButtonDown();
			}
		}
	}
	else if(_tcsicmp(msg.sType, "textchanged") == 0)
	{
		long nEndChar = 0;
		m_pEditSearchValue->GetSel(s_SearchEditStartChar, nEndChar);
		if(msg.pSender == m_pEditSearchValue)
		{
			CString strItem = m_pEditSearchValue->GetText().GetData();
			if(strItem == _T(""))
			{
				m_pLayoutSearcePage->SelectItem(0);
				m_pEditSearchView->SetFocus();
			}
			else
			{
				m_pLayoutSearcePage->SelectItem(1);
				ShowSearchList(strItem);
				SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);
			}
		}
	}
	else if(_tcsicmp(msg.sType, "killfocus") == 0)
	{
		if(msg.pSender != NULL && strcmp(msg.pSender->GetName(), "editSign") == 0)
		{
			UpdateUserSign();
		}
		if(msg.pSender != NULL && msg.pSender == m_pEditSearchValue)
		{
			//m_pLayoutSearcePage->SelectItem(0);
		}
	}
	else if(_tcsicmp(msg.sType, "itemactivate") == 0)
	{
		if(strcmp(msg.pSender->GetClass(), "TreeNodeUI") == 0)
		{
			if(m_pLayoutPage->GetCurSel() == 1)
			{
				OnSendMsg();
			}
			else if(m_pLayoutPage->GetCurSel() == 2)
			{
				OnSendClusterMsg();
			}
		}
		if(strcmp(msg.pSender->GetClass(), "ListContainerElementUI") == 0)
		{
			if(m_pLayoutSearch->GetCurSel() == 1)
			{	// 查找
				OpenCharWnd();
			}
			else if(m_pLayoutPage->GetCurSel() == 0)
			{
				m_pRecentList->OnLbnDblclk();
			}			
		}
	}
	else if(_tcsicmp(msg.sType, DUI_MSTTYPE_MOUSEHOVER) == 0)
	{
		if(_tcsicmp(msg.pSender->GetClass(), "LabelUI") == 0)
		{
			DuiLib::CLabelUI *pLabel = (DuiLib::CLabelUI*)msg.pSender;
			if((pLabel->GetName() == "labelfill") &&m_bFriendWnd) ///仅在好友页面中显示
			{
				DuiLib::CControlUI *pParent = pLabel->GetParent()->GetParent()->GetParent()->GetParent();
				RECT rcPos = pParent->GetPos();
				POINT ptLT;
				ptLT.x = rcPos.left;
				ptLT.y = rcPos.top;
				RECT rcMain;
				GetWindowRect(GetHWND(), &rcMain);
				ClientToScreen(GetHWND(), &ptLT);
				POINT ptRT = ptLT;
				ptRT.x = rcMain.right;
				FRIEND_INFO *pFriend = (FRIEND_INFO*)pParent->GetTag();

				int nLeft = ptLT.x-290;

				if ( nLeft<0 )
				{	
					nLeft = ptRT.x + 10;
				}

				CUserCardDlg::Show(pFriend, nLeft, ptRT.y);
			}
		}
		if(_tcsicmp(msg.pSender->GetClass(), "ButtonGifUI") == 0)
		{
			DuiLib::CButtonGifUI *pButton = (DuiLib::CButtonGifUI*)msg.pSender;
			if(pButton->GetName() == "btnModSelfInfo")
			{
				RECT rcPos = pButton->GetPos();
				POINT ptLT;
				ptLT.x = rcPos.left;
				ptLT.y = rcPos.top;
				RECT rcMain;
				GetWindowRect(GetHWND(), &rcMain);
				ClientToScreen(GetHWND(), &ptLT);
				int nTop = ptLT.y;
				ptLT.x = rcMain.left;
				POINT ptRT = ptLT;
				ptRT.x = rcMain.right;
				int nLeft = ptLT.x-290;

				if ( nLeft<0 )
				{	
					nLeft = ptRT.x + 10;
				}

				CUserCardDlg::Show(&g_ImUserInfo, nLeft, ptRT.y);
			}
		}
	}
	else if(_tcsicmp(msg.sType, DUI_MSGTYPE_RETURN) == 0)
	{
		if(m_pEditSign->IsFocused())
		{
			m_pFriendList->SetFocus();
		}
	}
    else if(_tcsicmp(msg.sType, DUI_MSGTYPE_MOUSELEAVE) == 0)
	{
		if(_tcsicmp(msg.pSender->GetClass(), "LabelUI") == 0)
		{
			DuiLib::CLabelUI *pLabel = (DuiLib::CLabelUI*)msg.pSender;
			if(pLabel->GetName() == "labelfill")
			{
				CUserCardDlg::CloseWindow();
			}
		}
		if(_tcsicmp(msg.pSender->GetClass(), "ButtonGifUI") == 0)
		{
			DuiLib::CButtonGifUI *pButton = (DuiLib::CButtonGifUI*)msg.pSender;
			if(pButton->GetName() == "lbHeaderIcon" || pButton->GetName() == "btnModSelfInfo")
			{
				CUserCardDlg::CloseWindow();
			}
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

LRESULT CMainFrame::OnWebIconDownload(WPARAM w,LPARAM l)
{
	char * szFileIcon = (char *)l;
	uint32 fid        = w;
	CString strGifFile =  szFileIcon;
	strGifFile = strGifFile + ".gif";////文件改名
	if ( w==g_ImUserInfo.GetId() )
	{
		if ( szFileIcon )
		{
			if ( FileExist(strGifFile) )
			{
				m_pHead->SetBkImage("");
				m_pHead->SetNormalGifFile(strGifFile);
			}
			else
			{
				string strHeadIcon = CutRoundImg(szFileIcon, 50, 50);
				m_pHead->SetBkImage(strHeadIcon.c_str());
				m_pHead->SetNormalGifFile("");
			}
		}
	}

	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent) 
	{
	case TIMER_USER_CARD:
		{
			KillTimer(GetHWND(), TIMER_USER_CARD);
			CUserCardDlg::GetInstance()->CloseWindow();
		}
		break;
	case TIMER_BEAT:
		{
			static int timer=0;
			static int operation=0;

			//if(timer%10 == 0 && timer != 0)
			//{	// 10次后发送修改状态请求，已让服务端更新全局服务器状态
			//	XT_STATUS_MOD_REQ req;
			//	req.id     = g_ImUserInfo.GetId();
			//	req.status = g_ImUserInfo.status;

			//	g_pProtocol->SendReq(req);
			//}
			if ( ++timer>30 )
			{
				ImHeartBeat();
				timer=0;
			}

			if ( ++operation>600 )//
			{
				ImSaveUserOperations();
				operation = 0;
			}

			CheckHideTick();
			
			if(m_dwPrevBeatTick < CIMProtocol::s_nHeartBeat)
			{
				m_dwPrevBeatTick = CIMProtocol::s_nHeartBeat;
			}

			int nSpan = GetTickCount() - m_dwPrevBeatTick;
			TRACE("间隔 %dms", nSpan);
			if ( /*g_bNetworkOK &&*/ nSpan > 90000)	
			{	// 心跳超时，网络没断
				m_bDisconnectFlag = true;
				m_nPreOnlineStatus = g_ImUserInfo.status;

				g_ImUserInfo.status = XTREAM_OFFLINE;
				m_bLogined=false;
				m_nLoginStatus=0;

				// 服务器断连时，不灰图标
				// 判断客户端网络状态
				if(InternetCheckConnection("https://www.baidu.com",FLAG_ICC_FORCE_CONNECTION ,0))
				{	// 网络正常
					//SetTaskIconTip("与服务器失去联系");
					//ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);
				}
				else
				{
					SetTaskIconTip("与服务器失去联系");
					ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);
				}				

				g_pProtocol->Stop();

				KillTimer(GetHWND(), TIMER_BEAT);
				SetTimer(GetHWND(), TIMER_RELOGIN,3000,NULL);
#ifdef _CS_PLAT_EX_
				g_pMessageHandleWnd->CloseAllMonitor();
#endif
				//theApp.m_pMainWnd->PostMessage(WM_HEART_TIMEOUT,0,0);
			}
			/*
			if( !g_bNetworkOK )
			{	// 如果网络断开了，隔1s检测一次网络，网络重新连上时，重新登录
				m_bDisconnectFlag = true;
				m_nPreOnlineStatus = g_ImUserInfo.status;

				g_ImUserInfo.status = XTREAM_OFFLINE;
				m_bLogined=false;
				m_nLoginStatus=0;

				SetTaskIconTip("网络断开，请检查您的网络");
				ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);

				g_pProtocol->Stop();

				KillTimer(GetHWND(), TIMER_BEAT);
				SetTimer(GetHWND(), TIMER_CHECK_NETWORD,1000,NULL);				
			}
			*/
		break;
		}
	case TIMER_RELOGIN:
		{
			KillTimer(GetHWND(), TIMER_RELOGIN);
			if ( g_ImUserInfo.status==XTREAM_OFFLINE )
			{
				ChangeUserStatus(m_nPreOnlineStatus);
			}
		}
		break;
/*
	case TIMER_CHECK_NETWORD:
		{	// 网络正常连接时，重新连接
			if ( g_ImUserInfo.status==XTREAM_OFFLINE && g_bNetworkOK )
			{
				ChangeUserStatus(m_nPreOnlineStatus);
				KillTimer(GetHWND(), TIMER_CHECK_NETWORD);
			}
		}
		break;
*/
	case TIMER_SORTLIST:
		{
			KillTimer(GetHWND(), TIMER_SORTLIST);
			UpdateFriendList();
		}
		break;
	case TIMER_INVALIDATELIST:
		{
			KillTimer(GetHWND(), TIMER_INVALIDATELIST);
			//m_wndGroupList.InvalidateList();
		}
		break;
	case TIMER_EDITGOUP:
		{
			KillTimer(GetHWND(), TIMER_EDITGOUP);
			FL_ITEM_POS posItemSelected = m_pFriendList->GetCurSel();
			m_pFriendList->ShowEditBox(posItemSelected.nGroupID);
		}
		break;
	case TIMER_SORTGROUP:
		KillTimer(GetHWND(), TIMER_SORTGROUP);
		SortGroupList();
		break;
	case TIMER_SERCHEDIT_SETFOCUS:
		{
			KillTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS);
			m_pEditSearchValue->SetFocus();
			m_pEditSearchValue->SetSel(s_SearchEditStartChar,s_SearchEditStartChar);
		}
		break;
	case TIMER_LOGIN:
		{
			static BYTE uStatus =0;
			ShowTaskIcon(g_ImUserInfo.sex,uStatus);
			//	m_wndFaceIcon.SetFaceId(g_ImUserInfo.face,uStatus);
			if (++uStatus>3 )
				uStatus=0;
		}
		break;
	case TIMER_LBUTTONUP:
		{
			//		BringWindowToTop();
			//		SetForegroundWindow();

			CPoint point;
			GetCursorPos(&point);

			CMenuEx *popmenu,menu;
			menu.LoadMenu(IDR_LEFT_CLICK);

			CImageList	il;

			CResBitmap bm;	
			il.Create(16, 16, ILC_COLOR24|ILC_MASK, 0, 1);
			bm.LoadBitmap(IDB_WINXP_ICON);    
			il.Add(&bm,RGB(255,255,255));
			bm.DeleteObject();

			menu.SetImageList(&il);

			static UINT nMenuImageIndex[] =
			{   
				ID_MENU_ONLINE,
				ID_MENU_BUSY,
				ID_MENU_LEAVE,
				ID_MENU_HIDE,	

			};

			menu.SetImageIndex(nMenuImageIndex,sizeof(nMenuImageIndex)/sizeof(UINT));

			popmenu = (CMenuEx *) menu.GetSubMenu(0);

			switch (g_ImUserInfo.status)
			{
			case XTREAM_OFFLINE:
				popmenu->CheckMenuItem(ID_MENU_OFFLINE,MF_CHECKED);
				break;
			case XTREAM_HIDE:
				popmenu->CheckMenuItem(ID_MENU_HIDE,MF_CHECKED);
				break;
			case XTREAM_LEAVE:
				popmenu->CheckMenuItem(ID_MENU_LEAVE,MF_CHECKED);
				break;
			case XTREAM_ONLINE:
				popmenu->CheckMenuItem(ID_MENU_ONLINE,MF_CHECKED);
				break;
			}

			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint duiPoint(point.x,point.y);
			pMenu->Init(NULL, _T("loginstatus.xml"), duiPoint, &m_PaintManager, NULL, DuiLib::eMenuAlignment_Left | DuiLib::eMenuAlignment_Bottom);
			AppendDuiMenu(pMenu, menu.GetSubMenu(0));

			KillTimer(GetHWND(), TIMER_LBUTTONUP);
		}
		break;
	case TIMER_DELAY:
		m_nLoginStatus=2;
		KillTimer(GetHWND(), TIMER_DELAY);
		break;

	case TIMER_HIDE:
		{
			POINT           pt;
			CRect            rc;
			GetCursorPos(&pt);
			GetWindowRect(GetHWND(), &rc);
			rc.InflateRect(5,5);
			// 若鼠标不在窗体内,隐藏窗体.
			if (!PtInRect(&rc, pt)) 
			{
				// 如果没有隐藏且没有强制要求显示则隐藏窗体,注意如果当前窗体没有停靠
				// 则窗体不设置为置顶
				if(!m_bHide && !m_bForceShow)
				{
					CWnd *pWnd = CWnd::GetFocus();//隐藏或显示主界面时当前窗口焦点不变
					HideSide(TRUE, &CWnd::wndTopMost);
					if(pWnd != NULL)
						pWnd->SetFocus();
				}
			}
			else
			{
				if(m_bHide)
				{
					// 显示窗体
					CWnd *pWnd = CWnd::GetFocus();//隐藏或显示主界面时当前窗口焦点不变
					HideSide(FALSE,&CWnd::wndNoTopMost);
					if(pWnd != NULL)
						pWnd->SetFocus();
				}
				else if(m_bForceShow)
				{
					// 双击任务栏图标的时候首先让窗体置顶，随后再让它NoTopMost
					SetWindowPos(GetHWND(), CWnd::wndNoTopMost.GetSafeHwnd(),0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				}
				// 如果鼠标移动到窗体上就不要求强制显示窗体，以实现鼠标移出窗体后窗体自动隐藏
				m_bForceShow = FALSE;
			}
		}		
		break;
	case TIMER_SHOW_PCAST:
		{
			FRIEND_LIST::iterator it;

			try
			{		
				//g_pMiniPictureBroadCast->Close();

				//for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
				//{
				//	g_pMiniPictureBroadCast->AddUser( (*it)->id );
				//}

				//CString strCatchePath = g_LocalData.GetPath()+"data\\MiniPictureBroadCast\\";
				//CreateDirectory(strCatchePath,NULL);
				//g_pMiniPictureBroadCast->Show(_bstr_t((LPCSTR)strCatchePath));
			}
			catch(...)
			{
			}

			KillTimer(GetHWND(), nIDEvent);
			break;
		}
	case TIMER_DISCONNECT:
		{	// 断开与后台连接
			KillTimer(GetHWND(), TIMER_DISCONNECT);
			g_pProtocol->Stop();
		}
		break;

	case TIMER_WAKEUP:///空闲时切换状态
		{
			LASTINPUTINFO lpi;
			lpi.cbSize = sizeof(lpi);
			GetLastInputInfo(&lpi);
			WORD nTime     = lpi.dwTime;///空闲时间
			WORD nWakeTime = m_nWakeupVK*60*1000;
			if(nTime > nWakeTime)
			{
				////状态切换
				if(0 == m_nStatus)    //离开
				{
					OnMenuLeave();
				}
				else if(1 == m_nStatus)//忙碌
				{
					OnMenuBusy();
				}
			}
		}
		break;
#ifdef _BUSINESS_VESION
	case TIMER_MOD_HANGUP_STATE:
		{
			KillTimer(GetHWND(), nIDEvent);
			XT_MOD_HANGUP_STATE_REQ req;
			req.uUserId = g_ImUserInfo.GetId();
			//挂起状态变化之后所有聊天窗口的挂起按钮状态需要随之变化
			if(!m_bIsHangupSelect)//没有手动挂起
			{
				if(m_nNumThenHangup == -1)
				{	// 取消挂起
					req.uHangupState = 0;

					for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
					{
						CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
						if(pDlg != NULL && pDlg->GetHWND() != NULL)
						{
							pDlg->SetHangupState(false);
						}
					}
				}
				else
				{
					// 获取陌生人窗口数量
					int nStrangerNum = g_pMessageHandleWnd->GetStrangerMsgDlgCount();
					if(nStrangerNum >= m_nNumThenHangup)
					{	// 挂起
						req.uHangupState = 1;
						for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
						{
							CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
							if(pDlg != NULL && pDlg->GetHWND() != NULL)
							{
								pDlg->SetHangupState(true);
							}
						}
					}
					else
					{	// 陌生人聊天窗口减少时，取消挂起
						req.uHangupState = 0;
						for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
						{
							CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
							if(pDlg != NULL && pDlg->GetHWND() != NULL)
							{
								pDlg->SetHangupState(false);
							}
						}
					}
				}
			}
			else//手动挂起
			{
				req.uHangupState = 1;
				for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
				{
					CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
					if(pDlg != NULL && pDlg->GetHWND() != NULL)
					{
						pDlg->SetHangupState(true);
					}
				}
			}
			g_pProtocol->SendReq(req);
		}
		break;
#endif

	case TIMER_SHOW_USERCARD:
		{
			/*	KillTimer(GetHWND(), nIDEvent);

			CPoint   point;
			GetCursorPos(&point);

			CRect rc;
			m_wndFaceIcon.GetWindowRect(rc);

			if(rc.PtInRect(point))
			{
			static std::tr1::shared_ptr<CUserCardDlg> pInstance = CUserCardDlg::GetInstance();
			if(pInstance == NULL)
			{
			int nLeft = rc.left-240;

			if ( nLeft<0 )
			{	
			CRect rcDlg;
			GetWindowRect(rcDlg);
			nLeft = rcDlg.right + 10;
			}

			CUserCardDlg::Show(&g_ImUserInfo,nLeft,rc.top,rc);
			}
			}*/
		}
		break;
	case TIMER_CLOSE:
		{	// 15s内没传输完毕最近联系客户数据，强制关闭程序
			KillTimer(GetHWND(), TIMER_CLOSE);
			#ifdef _BUSINESS_VESION
				if(!m_bHasUpdateNearCommCust)
				{
					m_bHasUpdateNearCommCust = true;
					CLOSE();
				}         
            #endif
		}
		break;
	case TIMER_SEND_MSG_QUEUE:
		{
			SetSendMsgQueue();
		}
		break;
	default:
		if(m_bNeedSort)
		{
			try
			{
			}
			catch(...)
			{
			}
		}
		break;
	}
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  
{  
	if (uMsg == WM_MENUCLICK)  
	{ 
		DuiLib::CDuiString *strMenuName = (DuiLib::CDuiString*)wParam;
		int nID = atoi(strMenuName->GetData());

		if ( *strMenuName == _T("MenuOnline")) 
		{
			nID = ID_MENU_ONLINE;
		}
		else if ( *strMenuName == _T("MenuBusy")) 
		{
			nID = ID_MENU_BUSY;
		}
		else if ( *strMenuName == _T("MenuLeave")) 
		{
			nID = ID_MENU_LEAVE;
		}
		else if ( *strMenuName == _T("MenuStealth")) 
		{
			nID = ID_MENU_HIDE;
		}

		switch (nID)
		{
		case ID_P2PSENDMSG:	OnSendMsg();	break;
		case ID_FRIENDINFO:	OnFriendInfo();	break;
		case ID_FRIENDALBUM:	OnFriendAlbum();	break;
		case ID_FRIENDARTICLE:	OnFriendArticle();	break;
		case ID_FRIENDBLOG:	OnFriendBlog();	break;
		case ID_DELFRIEND:	OnDelFriend();	break;
		case ID_ADDGROUP:	OnAddGroup();	break;
		case ID_DELGROUP:	OnDelGroup();	break;
		case ID_RENAMEGROUP:	OnRenameGroup();	break;
		case ID_MSGHISTORY:	OnMsgHistory();	break;
		case ID_SEARCH:	OnSearchXtream();	break;
		case ID_REFRESH_FRIENDLIST:	OnRefreshFriendList();	break;
		case ID_SHOWALL:	OnShowAll();	break;
		case ID_HIDEOFFLINE:	OnHideOffline();	break;
		case ID_MODF_COMMNAME:	OnModfCommName();	break;
		case ID_CREATE_TEMPCLUSTER:	OnCreateTempCluster();	break;
		case ID_SEND_GROUP_MESSAGE:	OnSendGroupMessage();	break;
		case ID_ADD_CGROUP:	OnAddSubGroup();	break;
		case ID_ADD_FRIEND:	OnBnClickedButtonAdd();	break;
		case ID_RENAME_GROUP:	OnRenameGroup();	break;
		case ID_REMOVE_BLACKLIST:	OnRemoveBlackList();	break;
		case ID_ADD_BLACKLIST:	OnAddBlackList();	break;
		case ID_MENU_MOD_PSW:	OnMenuModPSW();	break;
		case ID_MENU_CHANGEUSER:	OnMenuChangeUser();	break;
		case ID_MENU_EXIT:	OnMenuExit();	break;
		case ID_MENU_SPEC_USER:	OnSpecifySend();	break;
		case ID_MENU_UPDATE:	OnSoftUpdate();	break;
		case ID_CLUSTER_CREATE:	OnClusterCreate();	break;
		case ID_CLUSTER_FRIENDINFO:	OnFriendInfo();	break;
		case ID_CLUSTER_SENDMSG:	OnSendClusterMsg();	break;
		case ID_CLUSTER_MODIFY:	OnClusterInfo();	break;
		case ID_CLUSTER_DISMISS:	OnClusterDismiss();	break;
		case ID_CLUSTER_QUIT:	OnQuitCluster();	break;
		case ID_CLUSTER_MSGHISTORY:	OnClusterMsgHistory();	break;
		case ID_SEARCH_CLUSTER:	OnSearchCluster();	break;
		case IDC_BUTTON_ADD:	OnSearchCluster();	break;
		case ID_CINFO_REFRESH:	OnCInfoRefresh();	break;
		case ID_CLIST_REFRESH:	OnCListRefresh();	break;
		case ID_MENU_HOME:	OnBnClickedBtnHome();	break;
		case ID_MENU_SEARCH_FRIEND:	OnSearchFriend();	break;
		case ID_MENU_CREATE_FAMILY:	OnCreateFamily();	break;
		case ID_MENU_SEARCH_FAMILY:	OnSearchFamily();	break;
		case ID_MENU_CONFIG:	OnMenuConfig();	break;
		case ID_MENU_REGISTER_P2P:	OnMenuRegisterP2p();	break;		
		case ID_MENU_ONLINE:	OnMenuOnline();	break;		
		case ID_MENU_ABOUT:	OnMenuAbout();	break;	
		case ID_MENU_LEAVE:	OnMenuLeave();	break;	
		case ID_MENU_HIDE:	OnMenuHide();	break;	
		case ID_MENU_BUSY:	OnMenuBusy();	break;	
		case ID_MENU_OFFLINE:	OnMenuOffline();	break;
		case ID_MENU_UPLOAD_PIC:	OnUploadPic();	break;
		case ID_MY_HOME:	OnBnClickedBtnHome();	break;
		case ID_SEND_P2PMSG: {m_pRecentList->OnP2PTalk();	}	break;
		case ID_FRIEND_INFO: {m_pRecentList->OnFriendInfo();	}	break;
		case ID_FRIEND_MSG: {m_pRecentList->OnFriendMsg();	}	break;
		case ID_FRIEND_ADD: {m_pRecentList->OnFriendAdd();	}	break;
		case ID_CLUSTER_TALK: {m_pRecentList->OnClusterTalk();	}	break;
		case ID_CLUSTER_INFO: {m_pRecentList->OnClusterInfo();	}	break;
		case ID_CLUSTER_MSG: {m_pRecentList->OnClusterMsg();	}	break;
		case ID_CLEAR_RECENT: {m_pRecentList->OnClearRecent();	RECENTLIST_OP(OnClearRecent())}	break;
		case ID_CLUSTER_REFUSE: OnClusterRefuse(); break;
		default:	break;
		}
		if(nID >= ID_GROUP_CHOOSE_BEG && nID <= ID_GROUP_CHOOSE_END)
		{
			OnGroupChoose(nID);
		}
	}

	bHandled = false;  
	//return 0; 
	return DuiLib::WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

static WORD g_oldHide = 0;				// 记录原有设置是否为“自动隐藏”	
void CMainFrame::OnBnClickedBtnSysSet()
{
	g_LocalData.GetHideWinConfig(g_oldHide);//是否自动隐藏

	CSystemSettingDlg::Show();
}


void CMainFrame::OnBnClickedBtnSysMsg()
{
	CDuiMsgDlg::Show();
}


void CMainFrame::OnBnClickedBtnSysAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddFriendOrClusterDlg::Show();
}


void CMainFrame::OnBnClickedBtnSysMenu()
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu;	
	menu.LoadMenu(IDR_MAIN_MENU);//弹出式菜单

#ifdef _CS_PLAT_EX_
	menu.GetSubMenu(0)->RemoveMenu(0, MF_BYPOSITION);
#endif

	DuiLib::CButtonUI *pbtnSysMenu = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnMenu"));
	RECT rcBtn = pbtnSysMenu->GetPos();
	POINT pt;
	pt.x = rcBtn.left;
	pt.y = rcBtn.top-2;
	ClientToScreen(GetHWND(), &pt);

	DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
	DuiLib::CPoint duiPoint(pt.x,pt.y);
	pMenu->Init(NULL, _T("loginstatus.xml"), duiPoint, &m_PaintManager, NULL, DuiLib::eMenuAlignment_Left | DuiLib::eMenuAlignment_Bottom);

	AppendDuiMenu(pMenu, menu.GetSubMenu(0));
}

void CMainFrame::OnMenuModPSW()
{
#ifndef _CS_PLAT_EX_
	#ifdef _WORK_EX_
		OpenHtmlPage(WORK_FORGET_PSW_URL);
	#elif defined _BUSINESS_VESION
		OpenHtmlPage(SELLER_FORGET_PSW_URL);
	#else
		OpenHtmlPage(BUYER_FORGET_PSW_URL);
	#endif
		//ShowWindow(false);
#else
	std::tr1::shared_ptr<CModPassWordDlg> pWnd = CModPassWordDlg::Show();
	pWnd->ShowModal();
#endif
}

void CMainFrame::OnMenuChangeUser()////切换用户
{
	////是否关闭了未读消息窗口
	BOOL bFlagUnReadWin    = FALSE;//未关闭所有窗口时统计的未读消息
	BOOL bFlagUnReadAllWin = FALSE;//关闭当前所有窗口时统计的未读消息
	if(g_mapMsgUnRead.size()>0)////关闭对话框时有未读消息
	{
		bFlagUnReadWin = TRUE;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////检测未保存的服务小记
#ifdef _CS_PLAT_EX_  //飞牛客服工作台(先判断是否保存服务小记，再判断是否有未读消息)
	BOOL bIsHint = FALSE;
	if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() >= 2)
	{	
		bIsHint = TRUE;
	}
	if(bIsHint)//有多个咨询框
	{
		CMainFrame::GetInstance()->LockDlg();////锁住对话框
		extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
		DWORD dwServiceLogCnt = g_mapServiceLog.size();
		int nBuyerWndCnt = CDuiCSPlatDlg::GetInstance()->CalcBuyerTalkingWnd();//和买家聊天的窗口数
		if(nBuyerWndCnt == 0)//对面没有买家，直接判断未读消息
		{
			////判断当前所有的咨询框是否有未读消息
			if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
			{
				int nCnt = 0;
				BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
				if(bUnRead) ////有未读消息
				{
					bFlagUnReadAllWin = TRUE;
				}
			}
		}
		else  //对面有买家
		{
			if(dwServiceLogCnt!= nBuyerWndCnt)//对面有买家，且有窗口右侧页没点服务小记的“保存”按钮
			{
				/////和咨询的好友ID进行对比,切换到第一个未保存服务小记的窗口页面
				BOOL bFind = FALSE;
				bFind = CDuiCSPlatDlg::GetInstance()->CheckUnSavedServiceLog();
				if(bFind)
				{
					return ;
				}	
			}
			else//两者相等，再检查是否有未保存完整的服务小记
			{
				bool bUncompleted = FALSE;
				DWORD dwFid = 0;
				DWORD dwStatus = 0;
				map<DWORD,DWORD>::iterator it = g_mapServiceLog.begin();
				for(;it!=g_mapServiceLog.end();it++)
				{
					dwFid    = it->first;
					dwStatus = it->second;

					if(dwStatus == UNCOMPLETED_SERVICELOG)//未保存完整
					{
						FRIEND_INFO *pFriend = ::FindFriend(dwFid);//找到好友信息
						if(pFriend!= NULL)
						{
							bUncompleted = CDuiCSPlatDlg::GetInstance()->SelectUnCompletedServiceLog(pFriend);
							break;
						}
					}							
				}
				if(bUncompleted)
				{
					return ;
				}
				else////全都保存了完整的服务小记，再检查是否有未读消息
				{
					////判断当前所有的咨询框是否有未读消息
					if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
					{
						int nCnt = 0;
						BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
						if(bUnRead) ////有未读消息
						{
							bFlagUnReadAllWin = TRUE;
						}
					}
				}
			}
		}
	}
	if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 1)////只有一个咨询框
	{
		extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
		DWORD dwServiceLogCnt = g_mapServiceLog.size();
		if(dwServiceLogCnt == 0)//未保存服务小记
		{
			//对面是买家
			if(CDuiCSPlatDlg::GetInstance()->CheckBuyerTalkingExist())
			{
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				return ;
			}
			else //对方不是买家,检查未读消息
			{
				////判断当前所有的咨询框是否有未读消息
				if(NULL != CDuiCSPlatDlg::GetInstance())////有聊天咨询框打开
				{
					int nCnt = 0;
					BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
					if(bUnRead) ////有未读消息
					{
						bFlagUnReadAllWin = TRUE;
					}
				}
			}
		}
		else if(dwServiceLogCnt == 1)//保存了服务小记或者不完整的服务小记
		{
			bool bUncompleted = FALSE;
			DWORD dwFid = 0;
			DWORD dwStatus = 0;
			map<DWORD,DWORD>::iterator it = g_mapServiceLog.begin();
			for(;it!=g_mapServiceLog.end();it++)
			{
				dwFid    = it->first;
				dwStatus = it->second;
				if(dwStatus == UNCOMPLETED_SERVICELOG)//未保存完整
				{
					FRIEND_INFO *pFriend = ::FindFriend(dwFid);//找到好友信息
					if(pFriend!= NULL)
					{
						bUncompleted = CDuiCSPlatDlg::GetInstance()->SelectUnCompletedServiceLog(pFriend);
						break;
					}
				}		
			}							
			if(bUncompleted)
			{
				return ;
			}
			else//保存了完整的服务小记，再判断是否有未读消息 
			{
				//////////////////////////////////////////////////
				/*std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(SAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///当前咨询窗口服务小记已保存，点确认关闭，再检测未读消息*/
				{
					////判断当前所有的咨询框是否有未读消息
					if(NULL != CDuiCSPlatDlg::GetInstance())////有聊天咨询框打开
					{
						int nCnt = 0;
						BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
						if(bUnRead) ////有未读消息
						{
							bFlagUnReadAllWin = TRUE;
						}
					}
				}
			}
		}
	}
#else
	////判断当前所有的咨询框是否有未读消息
	if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
	{
		int nCnt = 0;
		BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
		if(bUnRead) ////有未读消息
		{
			bFlagUnReadAllWin = TRUE;
		}
	}
#endif

	if(bFlagUnReadWin || bFlagUnReadAllWin)////检测到未读消息，弹出提示框
	{
		int n = g_mapMsgUnRead.size();
		std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(MOD_USER,GetHWND());
		pWnd->ShowModal();
	}
	else  ////没有未读消息
	{

		WORD nExitWarn = 0;
		g_LocalData.SaveExitMsgWarnConfig(nExitWarn);

		WORD nUnReadWnd =0;
		g_LocalData.SaveUnReadWndCnt(nUnReadWnd);////设置未读为0

		g_ImUserInfo.bAutoLogin = false;
		SaveLoginAccount();

		//得到当前程序的路径；
		char strPath[100]; 
		GetModuleFileName(NULL,strPath,100);
		//创建守护进程，在启用新进程成功后发送WM_QUIT消息结束原来的进程；
		STARTUPINFO startInfo;
		PROCESS_INFORMATION processInfo;
		ZeroMemory(&startInfo, sizeof(STARTUPINFO));
		startInfo.cb = sizeof(STARTUPINFO);
		if(CreateProcess(NULL,(LPTSTR)(LPCTSTR)strPath, NULL, NULL,FALSE, 0, NULL, NULL, &startInfo, &processInfo))
		{
			CloseDlg();
		}

		//CloseDlg();
		//CLoginDlg::Show();
	}
	//CString str;
	//str.Format("确定要注销当前用户吗?");
	//if (MessageBox(GetHWND(), str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) == IDYES)
	//{
	//	XT_LOGOUT lg;
	//	lg.id = g_ImUserInfo.GetId();
	//	lg.authData = 0;
	//	m_sLastUser = g_ImUserInfo.GetUserName().c_str();
	//	g_pProtocol->SendReq(lg);    //注销上个用户
	//	CLoginDlg dlg(GetDesktopWindow());
	//	dlg.EnableAutoLoginFunction(false);

	//	if (IDOK==dlg.DoModal())
	//	{
	//		// 跟新登陆状态图标
	//		UpdateStatusIco();

	//		dlg.GetUserInfo(g_ImUserInfo);
	//		g_pProtocol->SendReq(dlg.m_LoginReq);//登录新用户

	//		m_bLogined = false;	

	//		if(g_pHeadPhoto != NULL)
	//		{
	//			g_pHeadPhoto->Close();
	//		}
	//		if(g_pWriteArticle != NULL)
	//		{
	//			g_pWriteArticle->Close();
	//		}

	//		////CSAConfig::Close();
	//		CloseUploadPicDlg();
	//		if(g_pMyPictures != NULL)
	//		{
	//			g_pMyPictures->Close();
	//		}
	//		if(g_pMyArticles != NULL)
	//		{
	//			g_pMyArticles->Close();
	//		}

	//		::ClearFriendList();
	//		m_pFriendList->DeleteAllItems();
	//		::ClearClusterList();
	//		m_pPage1->m_pageCluster.m_wndClusterList.DeleteAllItems();
	//	}
	//}
}

void CMainFrame::OnMenuExit()
{
	SaveLoginAccount();
	m_bExit = TRUE;
	BOOL bFlagUnReadWin    = FALSE;//未关闭所有窗口时统计的未读消息
	BOOL bFlagUnReadAllWin = FALSE;//关闭当前所有窗口时统计的未读消息
	if(g_mapMsgUnRead.size()>0)    //关闭对话框时有未读消息
	{
		bFlagUnReadWin = TRUE;
	}
	////直接退出时，判断是否有未读消息咨询框
#ifdef _CS_PLAT_EX_  //飞牛客服工作台(先判断是否保存服务小记，再判断是否有未读消息)
	BOOL bIsHint = FALSE;
	if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() >= 2)
	{	
		bIsHint = TRUE;
	}
	if(bIsHint)//有多个咨询框
	{
		CMainFrame::GetInstance()->LockDlg();////锁住对话框
		extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
		DWORD dwServiceLogCnt = g_mapServiceLog.size();
		int nBuyerWndCnt = CDuiCSPlatDlg::GetInstance()->CalcBuyerTalkingWnd();//和买家聊天的窗口数
		if(nBuyerWndCnt == 0)//对面没有买家，直接判断未读消息
		{
			////判断当前所有的咨询框是否有未读消息
			if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
			{
				if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
				{
					int nCnt = 0;
					BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
					if(bUnRead) ////有未读消息
					{
						bFlagUnReadAllWin = TRUE;
					}
				}
			}
		}
		else  //对面有买家
		{
			if(dwServiceLogCnt!= nBuyerWndCnt)//对面有买家，且有窗口右侧页没点服务小记的“保存”按钮
			{
				/////和咨询的好友ID进行对比,切换到第一个未保存服务小记的窗口页面
				BOOL bFind = FALSE;
				bFind = CDuiCSPlatDlg::GetInstance()->CheckUnSavedServiceLog();
				if(bFind)
				{
					return ;
				}	
			}
			else//两者相等，再检查是否有未保存完整的服务小记
			{
				bool bUncompleted = FALSE;
				DWORD dwFid = 0;
				DWORD dwStatus = 0;
				map<DWORD,DWORD>::iterator it = g_mapServiceLog.begin();
				for(;it!=g_mapServiceLog.end();it++)
				{
					dwFid    = it->first;
					dwStatus = it->second;

					if(dwStatus == UNCOMPLETED_SERVICELOG)//未保存完整
					{
						FRIEND_INFO *pFriend = ::FindFriend(dwFid);//找到好友信息
						if(pFriend!= NULL)
						{
							bUncompleted = CDuiCSPlatDlg::GetInstance()->SelectUnCompletedServiceLog(pFriend);
							break;
						}
					}							
				}
				if(bUncompleted)
				{
					return ;
				}
				else////全都保存了完整的服务小记，再检查是否有未读消息
				{
					////判断当前所有的咨询框是否有未读消息
					if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
					{
						int nCnt = 0;
						BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
						if(bUnRead) ////有未读消息
						{
							bFlagUnReadAllWin = TRUE;
						}
					}
				}
			}
		}
	}
	if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 1)////只有一个咨询框
	{
		extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
		DWORD dwServiceLogCnt = g_mapServiceLog.size();
		if(dwServiceLogCnt == 0)//未保存服务小记
		{
			//对方是买家
			if(CDuiCSPlatDlg::GetInstance()->CheckBuyerTalkingExist())
			{
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				return ;
			}
			else //对方不是买家,没有服务小记，直接检查未读消息
			{
				if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
				{
					int nCnt = 0;
					BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
					if(bUnRead) ////有未读消息
					{
						bFlagUnReadAllWin = TRUE;
					}
				}
			}
		}
		else if(dwServiceLogCnt == 1)//保存了服务小记或者不完整的服务小记
		{
			bool bUncompleted = FALSE;
			DWORD dwFid = 0;
			DWORD dwStatus = 0;
			map<DWORD,DWORD>::iterator it = g_mapServiceLog.begin();
			for(;it!=g_mapServiceLog.end();it++)
			{
				dwFid    = it->first;
				dwStatus = it->second;
				if(dwStatus == UNCOMPLETED_SERVICELOG)//未保存完整
				{
					FRIEND_INFO *pFriend = ::FindFriend(dwFid);//找到好友信息
					if(pFriend!= NULL)
					{
						bUncompleted = CDuiCSPlatDlg::GetInstance()->SelectUnCompletedServiceLog(pFriend);
						break;
					}
				}		
			}							
			if(bUncompleted)
			{
				return ;
			}
			else//服务小记全都保存完整，再判断是否有未读消息 
			{
				//////////////////////////////////////////////////
				/*std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(SAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///当前咨询窗口服务小记已保存，点确认关闭，再检测未读消息*/
				{
					if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
					{
						int nCnt = 0;
						BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
						if(bUnRead) ////有未读消息
						{
							bFlagUnReadAllWin = TRUE;
						}
					}
				}
			}
		}
	}
#else
	if(NULL != CDuiCounsel::GetInstance())////有聊天咨询框打开
	{
		int nCnt = 0;
		BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nCnt);    ////更新g_mapMsgUnRead
		if(bUnRead) ////有未读消息
		{
			bFlagUnReadAllWin = TRUE;
		}
	}
#endif
	if(bFlagUnReadWin || bFlagUnReadAllWin)////检测到未读消息，弹出提示框
	{
		std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(EXIT_APP,GetHWND());
		pWnd->ShowModal();
	}
	else ////无未读消息,配置下次登录不提醒
	{
		WORD nExitWarn = 0;
		g_LocalData.SaveExitMsgWarnConfig(nExitWarn);

		WORD nUnReadWnd =0;
		g_LocalData.SaveUnReadWndCnt(nUnReadWnd);////设置未读为0
		ExitApp();
	}
}

void CMainFrame::ExitApp()
{
	WORD nWarn;
	g_LocalData.GetExitWarnConfig(nWarn); //退出告警,并且可以更改配置
	if(nWarn!=0) // 弹出框
	{
		// 弹出后确认再关闭	
		CExitWarningDlg::Show();
		return;
	}
	else// 直接退出
	{	
		CLOSE();
	}
}

void CMainFrame::SearchListAddFriend(LPCTSTR szImgPath, LPCTSTR szText, LPCTSTR szRemarks, DWORD tag, BOOL bUser /*= TRUE*/, int nHeight /*= 32*/)
{
	m_pSearchList->AddFriend(szImgPath, szText, szRemarks, tag, bUser, nHeight);
}

void CMainFrame::SearchListAddGroup(LPCTSTR szText, int nHeight /*= 32*/)
{
	m_pSearchList->AddGroup(szText, nHeight);
}

void CMainFrame::SearchListRemoveAll()
{
	m_pSearchList->RemoveAll();
	SIZE sz;
	sz.cx = 0;
	sz.cy = 0;
	m_pSearchList->SetScrollPos(sz);
}

int CMainFrame::SearchListGetCount()
{
	return m_pSearchList->GetCount();
}

void CMainFrame::SearchListSetCurSel(int nIndex)
{
	m_pSearchList->SelectItem(nIndex);
}

void CMainFrame::SearchListSelPage(int nIndex)
{
	m_pTab->SelectItem(nIndex);
}

void CMainFrame::SetTempSessionBtnText(LPCTSTR pText)
{
	m_pBtnTempSession->SetText(pText);
}

// 根据选择项，打开聊天窗口
void CMainFrame::OpenCharWnd()
{
	// 获取转接对象
	int nIndex = m_pSearchList->GetCurSel();
	DuiLib::CListContainerElementUI* pItem = static_cast<DuiLib::CListContainerElementUI*>(m_pSearchList->GetItemAt(nIndex));
	if(pItem == NULL)
		return;
	DuiLib::CControlUI *pCtrl = pItem->FindSubControl("nickname");
	if(pCtrl == NULL)
		return;

	BOOL bUser = pCtrl->GetTag();
	DWORD dWord = pItem->GetTag();	// 好友或者群指针
	if(bUser)
	{	// 打开好友聊天窗口
		FRIEND_INFO *pFriend = (FRIEND_INFO*)dWord;
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			CMessageBox::Model(GetHWND(), _T("不能和自己聊天"), IM_CAPTION, MB_OK);
			return;
		}
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg;
		pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

		if ( pDlg->GetHWND()==NULL )
		{
			//pDlg->Create(CP2PSendMsgDlg::IDD, CWnd::FromHandle(::GetDesktopWindow()));
			pDlg->NotifyServer_OpenedMsgDlg();
		}

		pDlg->ShowWindow(SW_SHOW);
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
		{
			CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pFriend, false, true, false, false, false);
			CDuiCSPlatDlg::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
#elif defined _WORK_EX_
		if(CDuiCounsel::GetInstance() != NULL)
		{
			CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, true, false, false, false);
			CDuiCounsel::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())) )
		{
			CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, true, false, false, false);
			CDuiCounsel::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
#endif
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;
		if ( ::IsMinimized(pDlg->GetHWND()) )
			pDlg->ShowWindow(SW_RESTORE);

		::BringWindowToTop(pDlg->GetHWND());
	}
	else
	{	// 打开群聊天窗口
		CLUSTER_INFO *pCluster = (CLUSTER_INFO*)dWord; 
		CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

		if ( pDlg->m_hWnd==NULL )
			pDlg->Create(CClusterSendMsgDlg::IDD,CWnd::FromHandle(::GetDesktopWindow()));

		pDlg->ShowWindow(SW_SHOW);

		if ( pDlg->IsIconic() )
			pDlg->ShowWindow(SW_RESTORE);

		pDlg->BringWindowToTop();
	}
}

// 是否显示临时会话按钮
void CMainFrame::ShowTempSessionBtn(BOOL bShow)
{
	m_pBtnTempSession->SetVisible(bShow);
}

void CMainFrame::UpdateSearchList(CLUSTER_INFO * pCluster,CString strItem)
{
	SearchListSelPage(0);
	SearchListRemoveAll();
	m_mapCluster.clear();////清空上次查询
	m_mapFriend.clear(); ////清空上次查询
	m_mapTeamFriend.clear();

	map<int,int>      m_mapGroup;
	int bFlag = FALSE;
	int    i = 0;
	int nCnt = 0;
	int nFolder = 0;
	int nItemIndex = 0;
	////bool bTail;
	CString strTxt  = _T("");
	CString strCluster  = _T("");
	int  n = 0;
	FRIEND_LIST::iterator it;
	/////////////////////////////////////////////////////////////////
	//////群组中查找
	nCnt =   g_vecCluster.size();
	CLUSTER_INFO *pTempCluster = NULL;
	for(i=0;i<nCnt;i++)
	{
		pTempCluster = g_vecCluster[i];
		if(pTempCluster->nFamilyID == pCluster->nFamilyID )
		{
			int nMemberCnt  =   pCluster->Count();
			for(int j = 0;j<nMemberCnt;j++)
			{
				n=0;
				FRIEND_INFO *pMember = &pCluster->members[j];
				IsKeyMatch(strItem,pMember,NULL);////在好友中查找
			}
			break;
		}
	}

	// 添加一空行，不让第一行无法响应双击事件，暂时没找到原因，先特殊处理
	if(!m_mapTeamFriend.empty() || !m_mapFriend.empty())
	{
		SearchListAddGroup("", 15);
	}

	// 我的团队
	map<FRIEND_INFO*,CString>::iterator itMapTeamFriend;
	for(itMapTeamFriend= m_mapTeamFriend.begin();itMapTeamFriend != m_mapTeamFriend.end();itMapTeamFriend++)
	{
		CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(itMapTeamFriend->first);
		CString strGifFile =  "";
		CString strPath =  itMapTeamFriend->first->face_url;
		if(strPath.Find(".gif")!=-1)
		{
			strGifFile =  strFaceFile + ".gif";////文件改名
			if ( !FileExist(strGifFile) )////不存在则创建
			{
				CopyFile(strFaceFile,strGifFile,FALSE);
			}
		}
		if ( FileExist(strGifFile) )
		{
			string strOutImg = strGifFile.GetBuffer(strGifFile.GetLength());
		    SearchListAddFriend(strOutImg.c_str(), itMapTeamFriend->second, itMapTeamFriend->first->user_sign, (DWORD)itMapTeamFriend->first, TRUE, 26);
		}
		else
		{
		    string strOutImg = CutRoundImg(strFaceFile, 24, 24, FALSE);
		    SearchListAddFriend(strOutImg.c_str(), itMapTeamFriend->second, itMapTeamFriend->first->user_sign, (DWORD)itMapTeamFriend->first, TRUE, 26);
		}
	}

	////查询好友分组
	map<FRIEND_INFO*,CString>::iterator itMapFriend;
	for(itMapFriend= m_mapFriend.begin();itMapFriend != m_mapFriend.end();itMapFriend++)
	{
		CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(itMapFriend->first);
		CString strGifFile =  "";
		CString strPath =  itMapFriend->first->face_url;
		if(strPath.Find(".gif")!=-1)
		{
			strGifFile =  strFaceFile + ".gif";////文件改名
			if ( !FileExist(strGifFile) )////不存在则创建
			{
				CopyFile(strFaceFile,strGifFile,FALSE);
			}
		}
		if ( FileExist(strGifFile) )
		{
			string strOutImg = strGifFile.GetBuffer(strGifFile.GetLength());
			SearchListAddFriend(strOutImg.c_str(), itMapFriend->second, itMapFriend->first->user_sign, (DWORD)itMapFriend->first, TRUE, 26);
		}
		else
		{
		    string strOutImg = CutRoundImg(strFaceFile, 24, 24, FALSE);
		    SearchListAddFriend(strOutImg.c_str(), itMapFriend->second, itMapFriend->first->user_sign, (DWORD)itMapFriend->first, TRUE, 26);
		}
	}

	// 没有查找到好友
	if(SearchListGetCount() == 0)
	{	
		SearchListSelPage(1);
	}
}

// 查找用户
void CMainFrame::OnSearchUser(const char *data, int nRetCode)
{
	if ( nRetCode==0)
	{
		//确保回来的应答是最后一次发的请求对应的应答，如果是之前的请求应答则丢弃
		if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() )
		{
			return;
		}

		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);

		int i;
		uint16	next_pos;
		uint32	total;
		int		nTotal;
		uint8	count;
		uint8	realCount = 0;
		rp>>count>>next_pos>>total;
		// 总在线人数
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
			char username[MAX_USERNAME_LEN+1]  = {0};
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
					pFriend->group_index = 1;//陌生人
					AddFriend(pFriend, false);
					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
				}	
				if ( pDlg->GetHWND()==NULL )
					//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
					pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

				if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
					pDlg->ShowWindow(/*SW_SHOW*/true);

				if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
					pDlg->ShowWindow(/*SW_RESTORE*/true);

				/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
			}
			else
			{	// 不能和自己聊天
				CMessageBox::Model(GetHWND(), _T("不能和自己聊天!"), IM_CAPTION, MB_OK);
				//m_strSendName.SetFocus();
			}
			break;
		}

	}
	else
	{	// 查找联系人失败
		CMessageBox::Model(GetHWND(), _T("未查找到联系人!"), IM_CAPTION, MB_OK);
		//m_editName.SetFocus();
	}

	//m_btnOK.EnableWindow(TRUE);
	//Invalidate(TRUE);
}

////////////////////////////////////////////////////////
////根据拼音联想搜索，规则如下:
    //1.支持从字符串任意位置开始匹配
	//2.支持拼音首字母连续匹配（HHL ---黄鹤楼)
	//3.支持拼音全拼连续匹配
	//4.支持前面任意字符全拼加后面首字母连续匹配(HUANGH/HEL/LOU)
	//5.支持汉字部分匹配
/////////////////////////////////////////////////////////////////
void CMainFrame::ShowSearchList(CString &strKey)
{
	m_mapGroup.clear();  ////清空上次查询
	m_mapCluster.clear();////清空上次查询
	m_mapFriend.clear(); ////清空上次查询
	m_mapTeamFriend.clear();
	SearchListSelPage(0);
	SearchListRemoveAll();
	map<int,int>      mapGroup;
	int bFlag = FALSE;
	int    i = 0;
	int nFolder = 0;
	int nItemIndex = 0;
	CString strTxt  = _T("");
	CString strCluster  = _T("");
	int  n = 0;
	FRIEND_LIST::iterator it;
	/////////////////////////////////////////////////////////////
	//////群中查找
	int nCnt =   g_vecCluster.size();
	CLUSTER_INFO *pCluster = NULL;
	for(i=0;i<nCnt;i++)
	{
		pCluster = g_vecCluster[i];
		//strCluster.Format("%s",pCluster->szFamilyName) ;   ////群名模糊匹配（1）
		IsKeyMatch(strKey,NULL,pCluster);                  ////在群名中查找

		////int nMemberCnt  =   pCluster->Count();
		////for(int j = 0;j<nMemberCnt;j++)
		////{
		////	//n=0;
		////	FRIEND_INFO *pMember = &pCluster->members[j];
		////	//strTxt = UserInfoToStr(pMember);                ////群组成员匹配（2）
		////	IsKeyMatch(strKey,pMember,pCluster);       ////查找是否有汉字，然后进行比对。
		////}
	}
	///////////////////////////////////////////////////////////////////////////////
	//////好友中查找
	for(it=g_lsFriend.begin();it!=g_lsFriend.end();it++)    ////好友组成员匹配（3）
	{
		FRIEND_INFO *pFriend = (*it);
#ifdef _CS_PLAT_EX_
		if(IsBuyer(pFriend->GetId()))
			continue;
#endif
#ifdef _CS_PLAT_EX_
		if(IsBuyer(pFriend->GetId()))
			continue;
#endif
		IsKeyMatch(strKey,pFriend,NULL);////在好友中查找
	}
/*
	////查询群友
	map<FRIEND_INFO*,CString>::iterator itMapCluster;
	for(itMapCluster= m_mapCluster.begin();itMapCluster != m_mapCluster.end();itMapCluster++)
	{
		m_listboxSearch.InsertString(0,itMapCluster->second);
        m_listboxSearch.SetItemData(0,(DWORD)itMapCluster->first);
	}
	//if(m_mapCluster.size()>0)
	//{
	//	m_listboxSearch.InsertString(0,_T("群组成员:"));
	//}
*/
	// 我的团队
	if(!m_mapTeamFriend.empty())
	{
		SearchListAddGroup("我的团队");
	}
	map<FRIEND_INFO*,CString>::iterator itMapTeamFriend;
	for(itMapTeamFriend= m_mapTeamFriend.begin();itMapTeamFriend != m_mapTeamFriend.end();itMapTeamFriend++)
	{
		CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(itMapTeamFriend->first);
		CString strGifFile =  "";
		CString strPath =  itMapTeamFriend->first->face_url;
		if(strPath.Find(".gif")!=-1)
		{
			strGifFile =  strFaceFile + ".gif";////文件改名
			if ( !FileExist(strGifFile) )////不存在则创建
			{
				CopyFile(strFaceFile,strGifFile,FALSE);
			}
		}
		if ( FileExist(strGifFile) )
		{
			string strOutImg = strGifFile.GetBuffer(strGifFile.GetLength());
			SearchListAddFriend(strOutImg.c_str(), itMapTeamFriend->second, itMapTeamFriend->first->user_sign, (DWORD)itMapTeamFriend->first);
		}
		else
		{
			string strOutImg = CutRoundImg(strFaceFile, 30, 30, FALSE);
			SearchListAddFriend(strOutImg.c_str(), itMapTeamFriend->second, itMapTeamFriend->first->user_sign, (DWORD)itMapTeamFriend->first);
		}
	}

	////查询好友分组
	if(!m_mapFriend.empty())
	{
		SearchListAddGroup("联系人");
	}
	map<FRIEND_INFO*,CString>::iterator itMapFriend;
	for(itMapFriend= m_mapFriend.begin();itMapFriend != m_mapFriend.end();itMapFriend++)
	{
		CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(itMapFriend->first);
		CString strGifFile =  "";
		CString strPath =  itMapFriend->first->face_url;
		if(strPath.Find(".gif")!=-1)
		{
			strGifFile =  strFaceFile + ".gif";////文件改名
			if ( !FileExist(strGifFile) )////不存在则创建
			{
				CopyFile(strFaceFile,strGifFile,FALSE);
			}
		}
		if ( FileExist(strGifFile) )
		{
			string strOutImg = strGifFile.GetBuffer(strGifFile.GetLength());
			SearchListAddFriend(strOutImg.c_str(), itMapFriend->second, itMapFriend->first->user_sign, (DWORD)itMapFriend->first);
		}
		else
		{
		   string strOutImg = CutRoundImg(strFaceFile, 30, 30, FALSE);
		   SearchListAddFriend(strOutImg.c_str(), itMapFriend->second, itMapFriend->first->user_sign, (DWORD)itMapFriend->first);
		}
	}

	///////////查询完毕后 ，显示查询结果/////////////////////////////////////////////////////////
	////查询群名
	if(!m_mapGroup.empty())
	{
		SearchListAddGroup("群组");
	}
	map<CLUSTER_INFO*,CString>::iterator itMapGroup;
	for(itMapGroup= m_mapGroup.begin();itMapGroup != m_mapGroup.end();itMapGroup++)
	{
		SearchListAddFriend("SysBtn\\icon_group.png", itMapGroup->second, "", (DWORD)itMapGroup->first, FALSE);
	}

	// 没有查找到好友
	if(SearchListGetCount() == 0)
	{	
		SearchListSelPage(1);
		CString strText;
		strText.Format("通过临时会话与\"%s\"聊天", strKey);
		SetTempSessionBtnText(strText);
	}
	else
	{	// 默认显示第一项
		SearchListSetCurSel(1);
	}
}

////查找群和好友中搜索到的结果
BOOL CMainFrame::ShowSearchRes(CString strKey,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster)
{
	map<int,int> mapGroup;
	BOOL bFlag = FALSE;
	int  nFolder = 0;
	BOOL bTail;
	int  n = 0;
	CString strName = _T("");

	if(pCluster!= NULL && pFriend == NULL)     ////群名称查找
	{
		strName.Format("%s",pCluster->szFamilyName);
		//m_listboxSearch.AddCluster(pCluster);////添加群名 
		m_mapGroup[pCluster] = strName;
	}
	////else if(pCluster!= NULL && pFriend != NULL)////群成员查找
	////{
	////	/////////////////////////////////////////////////////////////////
	////	strName = UserInfoToStr(pFriend);            ////群中的组员
	////	strName.AppendFormat("[%s]",pCluster->szFamilyName);////群名
	//// 		
	////	m_mapCluster[pFriend] = strName;
	////}
	else if(pCluster== NULL && pFriend != NULL)	////好友中查找
	{
		/////////////////////////////////////////////////////////////
		////在好友中查找
		strName.Format("%s",pFriend->nickname);

		int nGroupCount = g_GroupInfo.count;
		int nGroup = 0;
		int nIdx = 0;
		if(!bFlag)
		{
			bFlag = TRUE;
			for ( nGroup = 0; nGroup< nGroupCount; nGroup++ )
			{
				nIdx = g_GroupInfo.group[nGroup].group_index;
				//if(nIdx == 1 || nIdx == 2)//////陌生人或者黑名单
				//{
				//	continue;
				//}
				mapGroup[nIdx] = nGroup;
			}
		}
		/////////////插入好友信息///////////////////////////////////
		nFolder = mapGroup[pFriend->group_index];
		CString strGroup = _T("");
		CString strTxt   = _T("");
		strGroup.Format(_T("%s"),g_GroupInfo.group[nFolder].name) ;

		bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
		strTxt.Format(_T("%s"),UserInfoToStr(pFriend)) ;
		if(pFriend->group_index >= MY_TEAM_GROUP_ID)
		{
			m_mapTeamFriend[pFriend] = strTxt;
		}
		else
		{
			m_mapFriend[pFriend] = strTxt;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
////匹配规则判断
BOOL CMainFrame::IsKeyMatch(CString strKey, FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster)
{
	

	CString strNickName = _T(""); ////昵称
	CString strBackName = _T(""); ////备注
	CString strName = _T("");
	if(pCluster!=NULL && pFriend == NULL)
	{
		strName.Format("%s",pCluster->szFamilyName);  ////群名称
		KeySearch(strKey,strName,pFriend,pCluster);
	}
	//else if(pCluster!=NULL && pFriend != NULL)
	//{
	//	strName.Format("%s",UserInfoToStr(pFriend));  ////群内成员名
	//}
	else if(pCluster==NULL && pFriend != NULL)        ////好友组
	{
		strNickName.Format("%s",pFriend->nickname);
		strBackName.Format("%s",UserInfoToStr(pFriend));  ////备注为主，然后呢称
		if(strNickName == strBackName)////两者相等，查询一次
		{
            strName =  strNickName;
			KeySearch(strKey,strName,pFriend,pCluster);
		}
		else      ////两者不等，分开查询
		{
			KeySearch(strKey,strNickName,pFriend,pCluster);
			KeySearch(strKey,strBackName,pFriend,pCluster);
		}
	}
	return FALSE;
}

BOOL CMainFrame::KeySearch(CString strKey,CString strName,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster)
{
		////判空
	if(strKey.IsEmpty())
	{
		return TRUE;
	}
	if(strName.IsEmpty())
	{
		return FALSE;
	}

	strKey  = strKey.MakeLower();
	strName = strName.MakeLower();
	int nStrLen = 0;
	int n = sizeof(strName);
	nStrLen = strName.GetLength();
	
	BOOL bName = CTabWndSearch::IsIncludeChinese(strName); ////待查内容是否包含多个汉字
	BOOL bKey = CTabWndSearch::IsIncludeChinese(strKey);  ////输入关键字是否包含多个汉字

	BOOL bPinYin = FALSE;

	if(!bName&&!bKey)	  // 关键字和待匹配字符都不含汉字， 按照字符串部分匹配
	{

		if(strName.Find(strKey) > -1)
		{
	       ShowSearchRes(strKey,pFriend,pCluster); ////按照串匹配来查找目标信息
		   return TRUE;
		}
		else
		{
			return FALSE;
		}

	}
	else if(bName&&bKey)  // 关键字和待匹配字符都含汉字， 按照字符串部分匹配
	{
		if(strName.Find(strKey) > -1)
		{
			ShowSearchRes(strKey,pFriend,pCluster); ////按照串匹配来查找目标信息
			return TRUE;
		}
		else
		{
		    return FALSE;
	    }
	}
	else if(bName&&!bKey) // 关键字不含,待匹配字符含汉字(同时支持拼音或者部分匹配)
	{	
	
		if(strName.Find(strKey) > -1)  ////优先按照子集部分匹配
		{
			ShowSearchRes(strKey,pFriend,pCluster); ////按照串匹配来查找目标信息
			return TRUE;
		}
		else  ////按照拼音搜索
		{
			bPinYin = TRUE;
		}
	}
	else if(!bName&&bKey)// 关键字含有,待匹配字符不含汉字，搜索失败
	{
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////////////
	////按照拼音模糊搜索
	if(bPinYin)
	{
		////汉字转拼音
	    /*int iLen = strName.GetLength();
		TCHAR *szRes = new TCHAR[iLen];
		lstrcpy(szRes,strName.GetBuffer(iLen));
		strName.ReleaseBuffer();*/
		CString strRes = _T("");
		vector<CString> vecStr;
	    vecStr.resize(50);

		////vecStr = HZ2Pinyin(szRes); /////汉字转拼音
 		vecStr  =  CTabWndSearch::GetLetter(strName); /////汉字转拼音
		for(int i = 0;i<vecStr.size();i++)
		{
			strRes += vecStr[i];
		}

		// 开始匹配。如果之前一位匹配的是全拼（单个字符或数字也算是全拼）
		// 则当前位可以匹配全拼，也可以匹配首字母；
		// 如果之前一位匹配的是首字母，则当前位必须匹配首字母
		int nKeySize = strKey.GetLength();     ////关键字长度
		// 找到首字母匹配的第一个位置，再开始匹配
		CString strFirst = strKey.Left(1);

		int nIndex = 0;
		while(nIndex!=vecStr.size())////两者不等时
		{
			BOOL bRet = FALSE;
			CString strPinYin = strRes;
			CString strTxt = strPinYin.Left(1);
			if(strTxt == strFirst) ////首位匹配，可以是全拼+字符形式
			{	
				if(strPinYin.Left(nKeySize) == strKey) ////第一位匹配且后面的完全匹配
				{
					ShowSearchRes(strKey,pFriend,pCluster); ////按照串匹配来查找目标信息
					return TRUE;
				}
				else////首位匹配，但不是完全匹配（thx首字母)
				{
					int nLen = 0;
					if(nKeySize >vecStr.size()) //// 关键字过长
					{
						return FALSE;
					}
					if(nKeySize == vecStr.size())////两者长度相等时
					{
						nLen = 0;	
					}
					else
					{
                        nLen = 1;
					}
					for(int i = nLen;i<nKeySize;i++)  ///依次比较首字符
					{
						if(vecStr[i].Find(strKey[i])== 0) 
						{
							bRet = TRUE;
						}
						else
						{
							if(strPinYin.Find(strKey) >-1)
							{
								bRet = TRUE;
								break;;
							}
							else
							{
							   bRet = FALSE;
							   break;;
							}
						}
					}
					if(bRet)
					{
					    ShowSearchRes(strKey,pFriend,pCluster); ////按照串匹配来查找目标信息
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
			}
			else
			{
				nIndex++;
				CString strTemp = _T("");
				for(int j = nIndex;j<vecStr.size();j++)
				{
					strTemp += vecStr[j];
				}
				strRes = strTemp;////后移字符串进行遍历
			}
		}
	}

	return FALSE;
}

LRESULT CMainFrame::OnLoginFailed(WPARAM wParam,LPARAM lParam)
{
	m_nLoginStatus      = STATUS_NOT_LOGIN;
	g_ImUserInfo.status = XTREAM_OFFLINE;

	ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
	return 0;
}

LRESULT CMainFrame::OnImOk(WPARAM wParam,LPARAM lParam)
{
	KillTimer(GetHWND(), TIMER_LOGIN);

	if ( wParam==0 )//login success
	{
		DWORD dwTick = GetTickCount() - dwLoginReqTick;
		// 获取好友列表和好友分组
		InitializeIm();
		
		if ( dwTick <1350 )
		{
			SetTimer(GetHWND(), TIMER_DELAY,1350-dwTick,NULL);
		}
		else
		{
			m_nLoginStatus=2;
		}
	}
	//else
	//{
	//	g_ImUserInfo.status = XTREAM_OFFLINE;

	//	m_bLogined=false;
	//	m_nLoginStatus=0;
	//	g_ImUserInfo.bAutoLogin = false;
	//	SaveLoginAccount();
	//	//CMainFrame::CloseDlg();
	//	//CLoginDlg::Show(1, (LPCTSTR)UserInfoToStr(&g_ImUserInfo), (char *)lParam);

	//	SetTaskIconTip((char *)lParam);
	//	ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);

	//	g_pProtocol->Stop();
	//}

	return 0;
}

bool CMainFrame::AddTaskIcon()
{
	memset(&m_nifData,0,sizeof(m_nifData));
	m_nifData.cbSize = sizeof(m_nifData);
	m_nifData.hWnd   = m_hWnd;
	m_nifData.uID    = 0x010C;
	m_nifData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_STATE;
	m_nifData.dwState = NIS_SHAREDICON;
	m_nifData.uCallbackMessage = WM_TRAYICON;
/*
	if ( ::g_bOs2K )
		m_nifData.hIcon = g_imgAppIcon2k.ExtractIcon(3);
	else
*/
	m_nifData.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_NORM));

		// = g_imgAppIconXp.ExtractIcon(0);

	//m_nifData.
	strcpy(m_nifData.szTip,"哞哞");

	return Shell_NotifyIcon(NIM_ADD,&m_nifData);
}

void CMainFrame::SetTaskIconTip(CString strTip)
{
	m_nifData.uFlags = NIF_TIP;
	strcpy(m_nifData.szTip,strTip);

	Shell_NotifyIcon(NIM_MODIFY,&m_nifData);
}

void CMainFrame::ShowTaskIcon(int sex, int status)  
{
	m_nifData.uFlags = NIF_ICON;

	if( m_nifData.hIcon )
	{
		DestroyIcon(m_nifData.hIcon );
		m_nifData.hIcon = NULL;
	}

	int uIcon;

	switch(status)
	{
	case XTREAM_LEAVE:
		{
			uIcon = IDI_LEAVE;
			m_pbtnStatus->SetBkImage("FriendStatus\\status_leave.png");
		}
		break;
	case XTREAM_BUSY:
		{
			uIcon = IDI_BUSY;
			m_pbtnStatus->SetBkImage("FriendStatus\\status_busy.png");
		}
		break;
	case XTREAM_HIDE:
		{
			uIcon = IDI_STEALTH;
			m_pbtnStatus->SetBkImage("FriendStatus\\status_stealth.png");
		}
		break;
	case XTREAM_ONLINE:
	case XTREAM_HANGUP:
		{
			uIcon = IDI_NORM;
			m_pbtnStatus->SetBkImage("FriendStatus\\status_online.png");
		}
		break;
	case XTREAM_OFFLINE:
		//离线暂时不修改头像
		//{
		//	uIcon = IDI_OFFLINE;
		//	m_pbtnStatus->SetBkImage("FriendStatus\\status_offline.png");
		//}
		break;
	}
/*
	if ( ::g_bOs2K )
		m_nifData.hIcon = g_imgAppIcon2k.ExtractIcon(uIcon);
	else
*/
	if(status != XTREAM_OFFLINE)//离线暂时不修改右下角状态
	{
		m_nifData.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIcon));
		if(m_nifData.hIcon != NULL)
		{
			Shell_NotifyIcon(NIM_MODIFY,&m_nifData);
		}
	}
}

void CMainFrame::RemoveTaskIcon()
{
	Shell_NotifyIcon(NIM_DELETE,&m_nifData);
}

LRESULT CMainFrame::OnTaskIconClick(WPARAM w, LPARAM l)
{
	static DWORD dwTick = 0     ;
	DWORD dwNow = GetTickCount();

	if ( 1 == ::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_TRAYICON,w,l) )
	{
		dwTick = dwNow;
		return 1;//floorcluster 已经处理了该消息
	}

	switch(l)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;

	case WM_RBUTTONUP:
		{
			::BringWindowToTop(GetHWND());
			::SetForegroundWindow(GetHWND());

			CPoint point;
			GetCursorPos(&point);

			CMenuEx *popmenu, menu;

			menu.SetTitleImage((DWORD)GetResDLLHandle(),IDB_MENU_TITLE);
			menu.LoadMenu(IDR_RIGHT_CLICK);
			menu.SetDrawLeft(true);

			CImageList	il;

			CResBitmap bm;	
			il.Create(16, 16, ILC_COLOR24|ILC_MASK, 4, 0);
			bm.LoadBitmap(IDB_RIGHT_MENU);    
			il.Add(&bm,RGB(0,255,0));
			bm.DeleteObject();
		
			CMenuEx::s_nTitleGroup[0] = 1;

			menu.SetImageList(&il);

			static UINT nMenuImageIndex[] =
			{
				ID_MENU_ABOUT,
				ID_MENU_CHANGEUSER,
				ID_MENU_CONFIG,
				ID_MENU_REGISTER_P2P,
				ID_MENU_EXIT,
				ID_MENU_HELP,
			};

			menu.SetImageIndex(nMenuImageIndex,sizeof(nMenuImageIndex)/sizeof(UINT));

			popmenu = (CMenuEx *) menu.GetSubMenu(0);
			popmenu->SetDrawLeft(TRUE);
			popmenu->SetDrawTitle(TRUE);

			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint duiPoint(point.x,point.y);
			pMenu->Init(NULL, _T("loginstatus.xml"), duiPoint, &m_PaintManager, NULL, DuiLib::eMenuAlignment_Left | DuiLib::eMenuAlignment_Bottom);
			DuiLib::CMenuUI* rootMenu = pMenu->GetMenuUI();  
			DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
			pNew->SetLineType();
			rootMenu->Add(pNew);  
			int nMenuCount = popmenu->GetMenuItemCount();
			for(int i = 0; i < nMenuCount; ++i)
			{
				UINT nId = popmenu->GetMenuItemID(i);
				CString strMenuText;
				popmenu->GetMenuString(i, strMenuText, MF_BYPOSITION);
				CMenu *pSubMenu = popmenu->GetSubMenu(i);
				if(pSubMenu == NULL)
				{
					if(nId == 0 && strMenuText.IsEmpty())
					{
						DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
						pNew->SetLineType();
						rootMenu->Add(pNew);  
					}
					else
					{
						DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
						CString strName;
						strName.Format("%d", nId);
						pNew->SetName(strName);
						pNew->SetFixedHeight(24);
						pNew->SetText(strMenuText);  
						rootMenu->Add(pNew);  
					}
				}
			}

			pMenu->ResizeMenu();	


			//UINT id = flatmenu.Track(point.x, point.y, GetHWND, true, false, true);
			// 相应选择菜单的消息相应
			//DWORD wP = MAKEWPARAM(id, 0);
			//SendMessage(WM_COMMAND, wP, 0);
		}
		break;
	case WM_LBUTTONUP://鼠标左键也弹出哞哞
		/*
		KillTimer(TIMER_LBUTTONUP);
		if ( dwTick==0 )
		{
			SetTimer(TIMER_LBUTTONUP,500,NULL);
		}
		else
		if ( (dwNow-dwTick)>500 )
		{
			SetTimer(TIMER_LBUTTONUP,500,NULL);
		}

		dwTick = 0;
		break;
		*/
	case WM_LBUTTONDBLCLK:
		KillTimer(GetHWND(), TIMER_LBUTTONUP);

		dwTick = dwNow;

		if (  ::IsWindowVisible(GetHWND())==false )
		{
			int screen_cx = GetSystemMetrics(SM_CXSCREEN);
			int screen_cy = GetSystemMetrics(SM_CYSCREEN);

			CRect rcFrom, rcTo;
			GetWindowRect(GetHWND(),&rcTo);

			rcFrom.left	= screen_cx-120;
			rcFrom.top	= screen_cy-20;
			rcFrom.right  = screen_cx-100;
			rcFrom.bottom = screen_cy;

			DrawAnimatedRects(GetHWND(), IDANI_CAPTION,&rcFrom,&rcTo);//从右下角托盘处弹出窗体,xuexiaofeng

			ShowWindow(true);
		}

		if ( IsIconic(GetHWND()) )
		{
			ShowWindow(true);
		}
		// 显示出窗体
		HideSide(FALSE,/*&wndTopMost*/NULL);
		// 强制显示主窗体
		m_bForceShow=TRUE;

		SetForegroundWindow(GetHWND());
		BringWindowToTop(GetHWND());
		break;

	default:
		break;
	}

	return 1;
}

void CMainFrame::OnLogOut(const char *data,WORD wRet)
{
	LastOpera();
	CloseDlg();
}

LRESULT CMainFrame::OnHeartTimeOut(WPARAM w,LPARAM l)
{
/*
	g_pProtocol->Stop();
	g_ImUserInfo.status = XTREAM_OFFLINE;

	if ( this->m_bLogined )
	{  
		XT_LOGOUT l;
		l.authData = 0;
		l.id = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(l);
		SetUserOffLineFlag();
		SetClusterOfflineFlag();
		g_pMessageHandleWnd->OnHearTimeOut();

		SetTimer(GetHWND(),TIMER_RELOGIN,1000,NULL);

		this->m_bLogined      = false;
		this->m_bHeartTimeOut = true;

		this->SetTaskIconTip("哞哞与服务器失去连接");
		this->ShowUserInfo();
		this->ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
	}
	else
	{
		static char error[255];
		strcpy(error,"连接服务器超时，请检查网络！");
		theApp.m_pMainWnd->PostMessage(WM_IM_OK,1,(LPARAM)error);
	}
*/
	return 0;
}


void CMainFrame::OnLoginReq(const char *data,WORD wRet)
{
	m_nUserStatus  =(DWORD)data;

	if ( m_nLoginStatus!=1 )
	{
		m_nLoginStatus=1;
	}

	CString str;
	str.Format("正在获取用户(%s)信息...",g_ImUserInfo.GetUserName().c_str());
	this->SetTaskIconTip(str);

	dwLoginReqTick = GetTickCount();

	m_bImOk = false;

//	this->m_wndTab.SetCurSel(0);

	SetTimer(GetHWND(), TIMER_LOGIN,350,NULL);
}

void CMainFrame::OnLoginAck(const char *data,WORD wRet)
{
	LPXT_LOGIN_ACK resp=(LPXT_LOGIN_ACK)data;
	static char error[255];
	
	if(g_ImUserInfo.status != XTREAM_OFFLINE)
		return;		// 不是离线状态，丢弃

	if ( wRet==0 )
	{
		KillTimer(GetHWND(), TIMER_RELOGIN);

		// 清除排队队列
#ifdef _CS_PLAT_EX_
		CDuiCSPlatDlg::ClearWaitFriend();
#endif

		// 每登录成功一次，稳定性加1
		int nIdx = m_arrBusiSrv.FindServer(g_pProtocol->GetIMServerIP());
		if(nIdx >= 0)
		{
			m_arrBusiSrv[nIdx].maxConn++;
		}
		g_ImUserInfo.status = m_nPreOnlineStatus;
		g_ImUserInfo.ip	= resp->publicip;

		CString sUserPath;
		CString sUserData;
		sUserPath.Format("%s\\%s",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo.GetUserName().c_str());
		sUserData = sUserPath + "\\user.data";
		if(_access((sUserData),0) == -1)
		{
			::CreateDirectory(sUserPath,NULL);
			CFile f;
			if(f.Open(sUserData,CFile::modeCreate))
				f.Close();
			g_bFirstLogin = true;
		}
		else
		{
			g_bFirstLogin = false;
		}

		g_webUserHeadIconProcess.SafeTerminate();
		g_webUserHeadIconProcess.SetCachePath(g_LocalData.GetWebIconPath());
		g_webFamilyHeadIconProcess.SafeTerminate();
		g_webFamilyHeadIconProcess.SetCachePath(g_LocalData.GetWebIconPath());
		g_magicFaceProcess.SafeTerminate();
		g_magicFaceProcess.SetCachePath(g_LocalData.GetImageFileCachePath());
		g_webFileProcess.SafeTerminate();
/*		
		if ( IsWindowVisible(GetHWND())==false )
		{
			this->ShowWindow(true);
		}

		if ( IsIconic(GetHWND()) )
		{
			this->ShowWindow(true);
		}

		SetForegroundWindow(GetHWND());
		BringWindowToTop(GetHWND());
*/
		ShowUserInfo();	

		// 重新登录成功后，开始心跳检测
		StartHeartBeart();
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_)
		g_pProtocol->GetZmqMsg();
#endif
		// 获取好友信息
		Sleep(50);
		XT_USERINFO_GET_REQ req;
		req.fid = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);
#ifdef _CS_PLAT_EX_
		//客服工作台未处理的用户
		vector<FRIEND_INFO*> vecFriend;
		g_LocalData.GetCurrentBuyer(vecFriend);
		if(vecFriend.size() > 0)
		{
			//CMessageBox::Model(NULL, _T("检测到有未处理的买家,点击确定将打开未处理的会话！"), IM_CAPTION, MB_OK);
			//打开未处理的聊天窗口
			for(int nFriend = 0; nFriend < vecFriend.size(); nFriend++)
			{
				if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 0)
				{
					FRIEND_INFO* pFriend = vecFriend[nFriend];
					BOOL bIsExist = FALSE;
					for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
					{
						CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];

						if(pFriend->GetId() == pDlg->GetFriendInfo()->GetId())
						{
							bIsExist = TRUE;
							//pDlg->SetCsBuyerOffline();
							pDlg->UpdateUrl();
						}
					}
					if(!bIsExist)
					{
						CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
						pDlg->ShowWindow(false);
					}
				}
				else
				{
					FRIEND_INFO* pFriend = vecFriend[nFriend];
					CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
					if(nFriend == 0)
					{
						if(CDuiCSPlatDlg::GetInstance() != NULL)
						{
							CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
						}
					}
					else
					{
						pDlg->ShowWindow(false);
					}
				}
			}
		}
#endif
		//cookie中设置token
		SetCookieToken();

		//重新登录之后获取一次好友列表

		// 开启线程好友列表状态
		//unsigned threadID = 0;
		//_beginthreadex(NULL, 0, ThreadGetAllFriendStatus, NULL, 0, &threadID);

		// 发送获取好友列表的请求
		ImGetFriendList(0,FLS_TOTAL);
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		g_pProtocol->GetOfflineMsgReq();
#endif
		PostMessage(WM_CHECKVERSION, resp->version_flag, (LPARAM)resp->update_config);
	}
	else
	{	
		if ( wRet==SERVER_TIMEOUT )
		{	
			strcpy(error,"登录超时");
			// 登录失败后，3s后继续发登录请求
			SetTimer(GetHWND(), TIMER_RELOGIN, 3000, NULL);

			//SetTaskIconTip(error);
			//ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);
		}
		else
		{
			strcpy(error,g_pProtocol->GetLastError());
			if(strcmp(error, "您的帐号已停用") == 0
				|| strcmp(error, "用户名或密码错误") == 0
				|| strcmp(error, "此账号已被限制登录，重新设置密码，可以解除限制") == 0)
			{
				// 登录失败，重新发送登录请求
				KillTimer(GetHWND(), TIMER_RELOGIN);

				SetTaskIconTip(error);
				ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);
			}
			else
			{
				// 登录失败后，3s后继续发登录请求
				SetTimer(GetHWND(), TIMER_RELOGIN, 3000, NULL);

				//SetTaskIconTip(error);
				//ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);
			}
		}
	}
}

void CMainFrame::OnMenuConfig()
{
	CSAConfig::Show();
}

void CMainFrame::OnMenuRegisterP2p()
{
#ifdef _BUSINESS_VESION
		OpenHtmlPage(SELLER_REGISTER_URL);
#else
		OpenHtmlPage(BUYER_REGISTER_URL);
#endif
}

void CMainFrame::OnMenuAbout()
{
	CAboutBoxDlg::Show();
}

void CMainFrame::OnMenuOnline()
{
	if(g_ImUserInfo.status != XTREAM_OFFLINE)
		m_pbtnStatus->SetBkImage("FriendStatus\\status_online.png");
	else
		m_nPreOnlineStatus = XTREAM_ONLINE;
	ChangeUserStatus(XTREAM_ONLINE);
}

void CMainFrame::OnMenuLeave()
{
	if(g_ImUserInfo.status != XTREAM_OFFLINE)
		m_pbtnStatus->SetBkImage("FriendStatus\\status_leave.png");
	else
		m_nPreOnlineStatus = XTREAM_LEAVE;
	ChangeUserStatus(XTREAM_LEAVE);
}

void CMainFrame::OnMenuHide()
{
	if(g_ImUserInfo.status != XTREAM_OFFLINE)
		m_pbtnStatus->SetBkImage("FriendStatus\\status_stealth.png");
	else
		m_nPreOnlineStatus = XTREAM_HIDE;
	ChangeUserStatus(XTREAM_HIDE);
}

void CMainFrame::OnMenuBusy()
{
	if(g_ImUserInfo.status != XTREAM_OFFLINE)
		m_pbtnStatus->SetBkImage("FriendStatus\\status_busy.png");
	else
		m_nPreOnlineStatus = XTREAM_BUSY;
	ChangeUserStatus(XTREAM_BUSY);
}


void CMainFrame::OnMenuOffline()
{

	m_pbtnStatus->SetBkImage("FriendStatus\\status_offline.png");
	XT_LOGOUT l;
	l.authData = 0;
	l.id = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(l);

	ChangeUserStatus(XTREAM_OFFLINE);
	g_pProtocol->Stop();
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 320;
	lpMMI->ptMinTrackSize.y = 520;

	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::ShowUserInfo()
{/*
	CString str;

	str.Format("  (%s)",StatusToStr(g_ImUserInfo.status));
*/
	m_plbUserName->SetText(g_ImUserInfo.nickname);
	CRect rc;
	GetWindowRect(GetHWND(), &rc);
	int nMaxWidth = rc.Width() - 100;
	int nWidth = m_plbUserName->CalcRealWidth();
	if(nWidth > nMaxWidth)
	{
		nWidth = nMaxWidth;
	}
	m_plbUserName->SetFixedWidth(nWidth);
	m_pEditSign->SetText(g_ImUserInfo.user_sign);

	CString strTip;
	strTip.Format("哞哞 %s",UserInfoToStr(&g_ImUserInfo));
	SetTaskIconTip(strTip);

	ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);

	CString strFile = g_webUserHeadIconProcess.GetFaceFile(&g_ImUserInfo);
	CString strGifFile = "";
	CString strPath =  g_ImUserInfo.face_url;
	if(strPath.Find(".gif")!=-1)
	{
		strGifFile =  strFile + ".gif";////文件改名
		if ( !FileExist(strGifFile) )////不存在则创建
		{
			CopyFile(strFile,strGifFile,FALSE);
		}
	}
	if ( FileExist(strFile) )
	{
		if ( FileExist(strGifFile) )
		{
			g_LocalData.SetHeadImage(g_ImUserInfo.GetUserName().c_str(), strFile);
			m_pHead->SetBkImage("");
			m_pHead->SetNormalGifFile(strGifFile);
		}
		else
	    {
		   g_LocalData.SetHeadImage(g_ImUserInfo.GetUserName().c_str(), strFile);
		   string strHeadIcon = CutRoundImg(strFile, 50, 50);
		   m_pUserImg->SetBkImage(strHeadIcon.c_str());
		   m_pHead->SetNormalGifFile("");
	    }
	}
	//////刷新状态
	//XT_STATUS_GET_REQ reqStatus;
	//reqStatus.fid = g_ImUserInfo.GetId();
	//reqStatus.id  = g_ImUserInfo.GetId();
	//g_pProtocol->SendReq(reqStatus);
}

void CMainFrame::OnUserInfoModified(const char *data,WORD wRet)
{
	LPXT_USERINFO_MOD_ACK resp = (LPXT_USERINFO_MOD_ACK)data;

	if ( wRet==0 /*&& CSelfInfoDlg::pInstance*/)
	{	
	    ShowUserInfo();
		BuildClusterList();
	}
}

void CMainFrame::OnStatusModAck(const char *data,WORD wRet)
{
	LPXT_STATUS_MOD_ACK resp = (LPXT_STATUS_MOD_ACK)data;

	if ( wRet==0 )
	{
		g_ImUserInfo.status = resp->status;
		ShowTaskIcon(g_ImUserInfo.sex,resp->status);
		ShowUserInfo();

		g_ImUserInfo.status = resp->status;

		FRIEND_INFO *pFriend = FindFriend(g_ImUserInfo.GetId());
		if(pFriend != NULL)
		{
			pFriend->status = resp->status;
			//m_pFriendList->SetInvalidateLock(true);
			UpdateFriend(pFriend);
			//m_pFriendList->SetInvalidateLock(false);
		}

		if ( resp->status==XTREAM_OFFLINE )
		{
			m_bLogined=false;
			m_nLoginStatus=0;
			//m_stLoginInfo.SetWindowText("点击下面的按钮登录");
		}
	}
}

void CMainFrame::OnUserInfoGet(const char *data,WORD wRet)
{
	RecentList_UserInfoGet(data, wRet);
	LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;
	if ( wRet==0 )
	{
		FRIEND_INFO *pFriend  = NULL;

		if ( ack->GetId()==g_ImUserInfo.GetId() )
		{
			m_bLogined = true; 

			char password[255];
			strcpy(password,g_ImUserInfo.GetPassword().c_str());
			pFriend = (FRIEND_INFO *)&g_ImUserInfo;
			//string strGuid = g_ImUserInfo.GetGuid();
			//memcpy(pFriend,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));

			UpdateUserFullInfo(pFriend,ack);
			g_ImUserInfo.SetPassword(password);
			//g_ImUserInfo.SetGuid(strGuid.c_str());
			//阿波罗版性别没有不确定的了,一定要是男女,如果没指定,默认是女的
			if( pFriend->sex >= 2 )
			{
				pFriend->sex = 0;
			}

			g_webUserHeadIconProcess.GetUserHeadIcon(&g_ImUserInfo, CWnd::FromHandle(GetHWND()));

			ShowUserInfo();	

			int nGroup = 0;
			int nItem = 0;
			if(m_pFriendList->FindFriend(&g_ImUserInfo, nGroup, nItem))
			{
				FRIEND_INFO *pFriend = ::FindFriend(g_ImUserInfo.GetId());
				ASSERT(pFriend != NULL);
				int nGroupIndex = pFriend->group_index;
				memcpy(pFriend, (FRIEND_INFO*)&g_ImUserInfo, sizeof(FRIEND_INFO));
				pFriend->group_index = nGroupIndex;
				m_pFriendList->SetItem(nGroup, nItem, UserInfoToStr(pFriend), (DWORD)(pFriend));
			}
		}//end if (ack->id==g_ImUserInfo.GetId())
		else
		{
			pFriend = ::FindFriend(ack->GetId());
			if( pFriend)
			{
				CString sCommName = pFriend->comment_name;
				//memcpy(pFriend,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));
				UpdateUserFullInfo(pFriend,ack);
				strncpy(pFriend->comment_name,sCommName,sizeof(pFriend->comment_name)-1);

				FL_ITEM_POS posSel = m_pFriendList->GetCurSel();

				//m_pFriendList->DelItemImage(nGroup,nItem);
				// 注意备注名称不要覆盖了
				sCommName=pFriend->comment_name;
				//memcpy(pFriend,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));
				UpdateUserFullInfo(pFriend,ack);
				strncpy(pFriend->comment_name,sCommName,sizeof(pFriend->comment_name)-1);

				UpdateUserBitmap(pFriend);
				m_pFriendList->UpdateItemImage(pFriend);
				//m_pFriendList->SetInvalidateLock(true);

				pFriend->bRefreshed = true;
				UpdateFriend(pFriend);

				//if ( pFriend->status!=XTREAM_OFFLINE )
				//{
				// 下载该用户的自定义头像图片
				g_webUserHeadIconProcess.GetUserHeadIcon(ack, CWnd::FromHandle(GetHWND()));
				//}
				if(posSel.nGroupID >= 0 && posSel.nItem >= 0)
					m_pFriendList->SetCurSel(posSel.nGroupID,posSel.nItem);
			}
			else
			{
				pFriend = new FRIEND_INFO;
				pFriend->SelfEncrypt();
				//memcpy(pFriend,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));
				UpdateUserFullInfo(pFriend,ack);
				pFriend->group_index = 1;//陌生人
				AddFriend(pFriend, false);
			}	

			m_pRecentList->UpdateUser(pFriend);
			RECENTLIST_OP(UpdateUser(pFriend))
		}

		for ( int i=0; i<g_vecCluster.size(); i++ )
		{
			CLUSTER_INFO *pCluster = g_vecCluster[i];

			FRIEND_INFO *pFriend = pCluster->GetFriendInfo(ack->GetId());
			if ( pFriend )
			{
				int nCluster_Property=pFriend->cluster_property;
				UpdateUserFullInfo(pFriend,ack);
				pFriend->cluster_property=nCluster_Property;
				pFriend->bRefreshed = true;
			}
		}
	}
	else
	{
		if ( m_bImOk==false )
		{
			static char error[255];

			if ( wRet==SERVER_TIMEOUT )
			{
				strcpy(error,"服务器请求个人信息超时！");	
			}
			else
			{
				strcpy(error,g_pProtocol->GetLastError());
			}

			theApp.m_pMainWnd->PostMessage(WM_IM_OK,1,(LPARAM)error);
		}
	}
}

LRESULT CMainFrame::OnNetWorkError(WPARAM w,LPARAM l)
{

	//LOG("网络通讯故障！请检查你的网络！ error code="+IntToStr(w));
	//this->m_pPage1->m_pageFriend.OnNetWorkError();

	return 1;
}

//接收来自web的消息通知
void CMainFrame::OnWebInd(const char * data,WORD wRet)
{
	LPXT_WEB_MSG msg = (LPXT_WEB_MSG)data;

	switch (msg->msg_type)
	{
	case XT_WEB_MSG::USER_MSG_BOX://收件箱
	case XT_WEB_MSG::SYS_MSG:
	case XT_WEB_MSG::FRIEND_MSG:
	case XT_WEB_MSG::MAINPAGE_COMMENT:
	case XT_WEB_MSG::CONTENT_COMMENT:
		{
			PlayTone(EVOICE_SYS_MSG);
			TWebNotificationInfo *pWebNoti = new TWebNotificationInfo();

			strcpy(pWebNoti->szCreateDate,msg->publish_time);
			strcpy(pWebNoti->szContent,msg->content);
			strcpy(pWebNoti->szTitle,msg->title);
			strcpy(pWebNoti->szNickName,msg->sender_nickname);
			strcpy(pWebNoti->szUserName,msg->sender_username);
			strcpy(pWebNoti->szUrl,msg->src_url);
	
			pWebNoti->type      = msg->msg_type;
			pWebNoti->sub_type  = msg->sub_type;
			pWebNoti->log_id    = msg->log_id;
			pWebNoti->user_id   = msg->sender_id;		

			pWebNoti->dwCreateDate    = DateStr2Long(msg->publish_time);
			pWebNoti->notification_id = msg->msg_id;

			C139NoteDlg::m_nFreshMessage++;
			C139NoteDlg::vecNotification.push_back(pWebNoti);

		//	g_pMessageHandleWnd->Add139Notify();	

			CString strTip;
			//strTip.Format("您有%d条新消息",C139NoteDlg::m_nFreshMessage);
			strTip=msg->title;
				
			//写系统消息
			SYSTEM_MSG msgSys;
			CString strMsg =  strTip + ": " + msg->content;
			msgSys.dwTime = time(NULL);
			msgSys.iType  = SYSTEM_MSG::SYSMSG_MOUMOU;
			sprintf(msgSys.szMessage,strMsg);
			g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

			CString strCnt;
			strCnt=msg->content;
			CHintDlg::Show(strTip,strCnt,1,m_hWnd,false);

			

			if (C139NoteDlg::pInstance)
				C139NoteDlg::pInstance->PostMessage(WM_THREAD_NOTIFY,0,0);//InitContent
		}
		break;
	}
}

void CMainFrame::OnNetWorkErr(const char *data,WORD wRet)
{
/*
	if(wRet == NETWORK_DISCONNECT)
	{
		const char error[] = "网络连接断开,请检查网络连接";
		// 网络连接断开
		KillTimer(GetHWND(), TIMER_LOGIN);

		OnNetWorkError();

		m_nLoginStatus=0;
		//m_stLoginInfo.SetWindowText(error);
		SetTaskIconTip(error);
		ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
		g_ImUserInfo.status=XTREAM_OFFLINE;
		// 再响应后台的消息响应时不能直接断开连接，会导致关闭程序时崩溃，应该等响应函数走完后再调用
		//g_pProtocol->Stop();
		SetTimer(GetHWND(), TIMER_DISCONNECT, 1000, NULL);
	}
*/
}


LRESULT CMainFrame::OnCheckVersion(WPARAM w,LPARAM l)
{
	m_strVersionUrl = (LPCSTR)l;
	m_bIsNewVersion = (w == 0)?true:false;
	
	if ( w!=0 )
	{
		if ( w == 2 )// 2强制更新
		{
			/*
			XT_LOGOUT lg;
			lg.id = g_ImUserInfo.GetId();
			lg.authData = 0;
			g_pProtocol->SendReq(lg);
			*/
			g_pProtocol->Stop();


			//CVersionInfoDlg *pDlg = new CVersionInfoDlg(w, (LPCSTR)l, GetDesktopWindow());
			//pDlg->ShowWindow(SW_SHOW);

/*
			CVersionUpdatgeDlg *pDlg = new CVersionUpdatgeDlg(12,(LPCSTR)l);
			pDlg->ShowWindow(SW_SHOW);
		*/	
			CVersionUpdateDlgDui::Show(12, (LPCSTR)l);
		}
		else  if ( w==1 )//自动后台更新 非强制更新
		{
			//if ( m_hMini139UpdateThread==NULL && g_strMini139UpdateFile=="" )
			//{
			//	DWORD dwThreadId=0;
			//	m_hMini139UpdateThread = CreateThread(NULL,0,
			//					(LPTHREAD_START_ROUTINE)Mini139AutoUpdateProc,
			//					  (LPVOID)l,
			//					  0,
			//					&dwThreadId);
			//}


			//已经忽略过了
			WORD wVKIgnore = 0;
			g_LocalData.GetSoftUpdateIgnoreConfig(wVKIgnore);
			//不提醒
			WORD nVK = 0;
			g_LocalData.GetSoftUpdateConfig(nVK);
			if(nVK != 0 ) //有更新时不要安装，但提醒我
			{
				//第一次提示
				if(wVKIgnore == 0)
				{
					CVersionInfoDlg *pDlg = new CVersionInfoDlg(3, (LPCSTR)l, CWnd::FromHandle(GetDesktopWindow()));
					pDlg->ShowWindow(SW_SHOW);
				}
			}
			//自动更新
			else
			{
				g_pProtocol->Stop();
				/*
				CVersionUpdatgeDlg *pDlg = new CVersionUpdatgeDlg(12,(LPCSTR)l);
				pDlg->ShowWindow(SW_SHOW);
				*/
				CVersionUpdateDlgDui::Show(12, (LPCSTR)l);
			}
			CString strParams;
			/*
			SetCurrentDirectory(g_LocalData.GetPath());
			strParams.Format("miniupdate.exe \"%s\" %u /s",(LPCSTR)l, CIMProtocol::s_nCurVer);

			WinExec(strParams,SW_HIDE);
			*/
			IM_LOG(strParams);
		}
		else if( w == 3) // 非强制更新
		{		
			/*
			CVersionInfoDlg *pDlg = new CVersionInfoDlg(w, (LPCSTR)l, GetDesktopWindow());
			pDlg->ShowWindow(SW_SHOW);
			*/
		}
	}

	if( w != 2 )
	{	
		// 获取自己的详细资料
		XT_USERINFO_GET_REQ req;
		req.fid = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);
		m_bLogined = true;

		// 获取在线客服的资料
		//req.fid = HELPER_ID;
		//g_pProtocol->SendReq(req);

		//m_stLoginInfo.SetWindowText("获取个人信息...");
		StartHeartBeart();
		// 加载本地缓存的群列表
		LoadLoacalClusterList();
	}
	return 1;
}

void CMainFrame::OnBnClickedBtnLogin()
{
	if ( m_nLoginStatus == STATUS_LOGINING )//取消登录
	{
		XT_LOGOUT req;
		req.id = g_ImUserInfo.GetId();
		req.authData = 0;
		g_pProtocol->SendReq(req);
		g_pProtocol->Stop();
		KillTimer(GetHWND(), TIMER_LOGIN);
		if (g_hUserMutex)
		{
			ReleaseMutex(g_hUserMutex);
			CloseHandle(g_hUserMutex);
			g_hUserMutex = NULL;
		}
		this->ShowTaskIcon(0,XTREAM_OFFLINE);
		m_bLogined = false;
		m_nLoginStatus=0;
		SetTaskIconTip("哞哞");
		//m_stLoginInfo.SetWindowText("请点击下面的按钮 ");
		g_ImUserInfo.status = XTREAM_OFFLINE;

		this->ShowWindow(SW_MINIMIZE);
	}
	else if ( m_nLoginStatus == STATUS_NOT_LOGIN )
	{
		m_arrBusiSrv.Reset();
		if ( !g_pProtocol->IsRunning() )
			g_pProtocol->Start();
		// 避免出现重复登录框
		g_ImUserInfo.bAutoLogin = false;
		SaveLoginAccount();
		CLoginDlg::Show();
		//dlg.EnableAutoLoginFunction(false);//禁止登录框自动登录

		//if ( dlg.DoModal()==IDOK )
		//{
		//	// 跟新登陆状态图标
		//	UpdateStatusIco();

		//	m_nLoginStatus=1;			
		//	//m_stLoginInfo.SetWindowText("用户信息验证中,请稍候...");

		//	dlg.GetUserInfo(g_ImUserInfo);
		//	g_pProtocol->SendReq(dlg.m_LoginReq);
		//}		
	}
}

//void CMainFrame::OnBnClickedFriendArticle()
//{
//	if ( m_nThreadWebDataRef>0 || m_bFriendListGet==false )
//	{
//		AfxMessageBox("你的所有关注好友的文章还没有完全准备好,请稍候...");
//	}
//	else
//	{
//		if ( CFriendLogEntryDlg::GetArticles().size()>0 )
//		{
//			//if(CFriendLogEntryDlg::IsVisible()==FALSE)	
//			//	CFriendLogEntryDlg::Show(this);
//			//else
//			//{
//				CFriendLogEntryDlg::Show(this);
//			//}
//		}
//		else
//		{
//			AfxMessageBox("对不起，你现在还没有关注文章哦!");
//		}
//	}
//}
void CMainFrame::OnBnClickedButtonStatus()
{
	CRect rc;
	//m_btnStatus.GetWindowRect(rc);

	CFlatPopupMenu menu;
	menu.AddImg(AfxGetResourceHandle(), IDB_STATUS_ONLINE, "PNG");
	menu.AddImg(AfxGetResourceHandle(), IDB_STATUS_BUSY, "PNG");
	menu.AddImg(AfxGetResourceHandle(), IDB_STATUS_LEAVE, "PNG");
	menu.AddImg(AfxGetResourceHandle(), IDB_STATUS_HIDE, "PNG");
	menu.Create(AfxGetInstanceHandle());
	menu.AppendItem(0, "在线", 1, 0);
	menu.AppendItem(0, "忙碌", 2, 1);
	menu.AppendItem(0, "离开", 3, 2);
	menu.AppendItem(0, "隐身", 4, 3);

	// track it
	int	id=menu.Track(rc.left,rc.bottom + 2,GetHWND(),true);
	switch(id)
	{
	case 1:	// 在线
		{
			OnMenuOnline();
		}
		break;
	case 2:	// 忙碌
		{
			OnMenuBusy();
		}
		break;
	case 3:	// 离开
		{
			OnMenuLeave();
		}
		break;
	case 4:	// 隐身
		{
			OnMenuHide();
		}
		break;
	}
}

void CMainFrame::OnUploadPic()
{
	ShowUploadPicDlg((char*)g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname,(char*)g_ImUserInfo.GetPassword().c_str(),g_ImUserInfo.GetId());
}

void CMainFrame::OnStnClickedStaticStatus()
{
	CSAConfig::Show();
}

void CMainFrame::OnBnClickedBtnHome()
{
	if(g_pWebConfig)
	{
		CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(g_ImUserInfo.GetUserName().c_str()));
		RegUserOperation(0x0013);
		MiniOpenUrl(strUrl);
	}

}

void CMainFrame::SaveLoginAccount()
{
	RegAccountStruct r;

	r.dwServerIP = g_pProtocol->GetIMServerIP();

	strcpy(r.cUsername,g_ImUserInfo.GetUserName().c_str());
	strcpy(r.cPass,g_ImUserInfo.GetPassword().c_str());
	r.bHide = g_ImUserInfo.bHideLogin;
	r.bAutoLogin = g_ImUserInfo.bAutoLogin;
	r.bShowHint  = g_ImUserInfo.bShowHint;
	r.bRemeberPsw = g_ImUserInfo.bRemeberPsw;
	r.iLoginFlag = g_ImUserInfo.status;
	g_LocalData.AddUserAccountList(r);
}

//接收系统消息
void CMainFrame::OnRecvSvrMsg(const char *data,WORD wRet)
{
	LPXT_SERVERMSG sysmsg = (LPXT_SERVERMSG)data;
	
	if ( wRet==0 )
	{	
		if ( sysmsg->type==1 )//重登录提示消息
		{
			/*XT_LOGOUT lg;
			lg.id = g_ImUserInfo.GetId();
			lg.authData = 0;
			g_pProtocol->SendReq(lg);*/
			
			this->m_bLogined    = false;
			g_ImUserInfo.status = XTREAM_OFFLINE;

			SetTaskIconTip(sysmsg->message);
			//m_stLoginInfo.SetWindowText(sysmsg->message);

			CMessageBox::Model(GetDesktopWindow(), sysmsg->message, IM_CAPTION, MB_OK|MB_ICONINFORMATION);

			OnLogOut(NULL,0);
			// 再响应后台的消息响应时不能直接断开连接，会导致关闭程序时崩溃，应该等响应函数走完后再调用
			//g_pProtocol->Stop();
			SetTimer(GetHWND(), TIMER_DISCONNECT, 1000, NULL);

			KillTimer(GetHWND(), TIMER_BEAT);
			KillTimer(GetHWND(), TIMER_RELOGIN);	

			// 退出程序
			SaveLoginAccount();
			m_bExit = TRUE;
			CLOSE();
		}
		else
		{
			ShowUserHintDlg(sysmsg->message);
		}		
	}
}

LRESULT CMainFrame::OnHintDlgClick(WPARAM wParam ,LPARAM lParam)
{
	if(lParam == NULL)
		return 0;
	if(wParam == CHintDlg::LINKT_CLUSTER)
	{
		CLUSTER_INFO *pCluster = (CLUSTER_INFO *)(lParam);
		ASSERT(pCluster);
		CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
		if ( pDlg->m_hWnd==NULL )
			//pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());
			pDlg->Create(CClusterSendMsgDlg::IDD,CWnd::FromHandle(GetDesktopWindow()));

		if ( pDlg->IsWindowVisible()==FALSE )
			pDlg->ShowWindow(SW_SHOW);

		if ( pDlg->IsIconic() )
		{
			pDlg->ShowWindow(SW_RESTORE);
		}

		pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
		pDlg->SetForegroundWindow();
		pDlg->BringWindowToTop();
	}
	else if(wParam == CHintDlg::LINKT_FRIEND)
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)(lParam);
		if(pFriend->GetId() == g_ImUserInfo.GetId())
			return 1;

		CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		if ( pDlg->GetHWND()==NULL )
		{
			pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
		}
		if ( pDlg->GetHWND()==NULL )
			//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

		if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			pDlg->ShowWindow(/*SW_SHOW*/true);

		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
			pDlg->ShowWindow(/*SW_RESTORE*/true);

		/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
	}
	return 0;
}

LRESULT CMainFrame::OnThreadNotify(WPARAM w, LPARAM l)
{
	switch ( w )
	{
	case 0://Mini139自动更新完成
		{
			CString strHint;
			strHint.Format("IEC-iM有最新版本可用。\r\n请点击此处更新。");
			CHintDlg::Show(strHint,0,m_hWnd,false);

			if ( m_hMini139UpdateThread )
			{
				CloseHandle(m_hMini139UpdateThread);
				m_hMini139UpdateThread=NULL;
			}
		}
		break;
	case 1://
		{
		}
		break;
	case 2:
		{
			//CloseHandle(m_hLoadCommentList);
			//m_hLoadCommentList = NULL;

			//if (C139NoteDlg::m_nFreshMessage>0)
			//{
			//	::g_pMessageHandleWnd->Add139Notify();	

			//	CString strTip;
			//	strTip.Format("您有%d条新消息",C139NoteDlg::m_nFreshMessage);
			//	m_btn139Note.SetToolTip(strTip);
			//	m_btn139Note.SetWindowText("("+IntToStr(C139NoteDlg::m_nFreshMessage)+")");

			//	SetTimer(TIMER_139NOTE,350,NULL);
			//}

			//if (C139NoteDlg::pInstance)
			//	C139NoteDlg::pInstance->InitContent();

		}
		break;
	}

	return 0;
}

LRESULT CMainFrame::OnTrayIconUpdate(WPARAM w,LPARAM l)
{
	ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);
	return 0;
}

void CMainFrame::ShowMiniPictureBroadCast()
{
	KillTimer(GetHWND(), TIMER_SHOW_PCAST);
	SetTimer(GetHWND(), TIMER_SHOW_PCAST,3000,NULL);
}

void CMainFrame::OnDestroy()
{	
	g_pProtocol->UnRegistCmd(m_hWnd);
	RemoveTaskIcon();
}

void CMainFrame::OnBnClickedBtnGold()
{
	if(g_pWebConfig)
	{
		RegUserOperation(0x0014);
		CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ACCOUNT,StrToWideStr(g_ImUserInfo.GetUserName().c_str()));
		MiniOpenUrl(strUrl);
	}

}

void CMainFrame::StartMiniWebSession()
{
	if ( m_hMiniWebSession==NULL )
	{
		DWORD dwThreadId;
		m_hMiniWebSession = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MiniWebSessionProc,NULL,0,&dwThreadId);
	}
}

void  CMainFrame::NearSide()
{
	BOOL            bChanged = 0;
	RECT            rect;
	GetWindowRect(GetHWND(), &rect);
	m_alignType = ALIGN_NONE;
	if (rect.left < NEAR_SIZE)
	{
		m_alignType = ALIGN_LEFT;
		if ((rect.left != 0) && rect.right != NEAR_SIDE)
		{
			rect.right -= rect.left;
			rect.left = 0;
			bChanged = 1;
		}
	}
	else if (rect.right > m_nScreenW - NEAR_SIZE)
	{
		m_alignType = ALIGN_RIGHT;
		if (rect.right != m_nScreenW && rect.left != m_nScreenW - NEAR_SIDE)
		{
			rect.left += (m_nScreenW - rect.right);
			rect.right = m_nScreenW;
			bChanged = 1;
		}
	}
	//调整上
	else if (rect.top < NEAR_SIZE)
	{
		m_alignType = ALIGN_TOP;
		if (rect.top != 0 && rect.bottom != NEAR_SIDE)
		{
			rect.bottom -= rect.top;
			rect.top = 0;
			bChanged = 1;
		}
	}
	if (bChanged)
	{
		MoveWindow(GetHWND(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	}

}

void CMainFrame::OnSearchFriend()
{
	CAddFriendOrClusterDlg::Show();
}

void CMainFrame::OnSearchFamily()
{
	tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg = CAddFriendOrClusterDlg::Show();
	pDlg->SetSearchType(FS_CLUSTER);
}

void CMainFrame::OnCreateFamily()
{
	CCreateClusterDlg::Show();
}

void CMainFrame::OnMoving(UINT fwSide, LPRECT pRect)
{
	m_bForceShow=FALSE;
	int nS;
	// 未靠边界由窗体被移动的范围pRect测试
	if (m_alignType == ALIGN_NONE)
	{
		// 禁止窗体移到桌面下方
		//if(pRect->bottom + NEAR_SIZE >m_nScreenH)
		//{
		//	m_alignType = ALIGN_NONE;
		//	nS=pRect->bottom-pRect->top;
		//	pRect->top = m_nScreenH-nS;
		//	pRect->bottom = pRect->top + nS;
		//}
		/*else */if (pRect->top < NEAR_SIZE) //在上边有效距离内，自动靠拢。
		{
			m_alignType = ALIGN_TOP;
			pRect->bottom -= pRect->top;
			pRect->top = 0;
		}
		if (pRect->left < NEAR_SIZE) //在左边有效距离内
		{
			m_alignType = ALIGN_LEFT;
			pRect->right -= pRect->left;
			pRect->left = 0;
		}
		else if (pRect->right + NEAR_SIZE > m_nScreenW) //在右边有效距离内，m_nScreenW为屏幕宽度，可由GetSystemMetrics(SM_CYSCREEN)得到。
		{
			m_alignType = ALIGN_RIGHT;
			pRect->left += (m_nScreenW - pRect->right);
			pRect->right = m_nScreenW;
		}
		
		WORD nTop = 0;
		g_LocalData.GetTopShowConfig(nTop);//是否最前显示
		CRect rcMy;
		::GetWindowRect(this->m_hWnd,rcMy);//得到当前的窗口位置
		if(nTop!=0)
		{
			//设为总在最前
			SetWindowPos(GetHWND(), CWnd::wndTopMost.GetSafeHwnd(),rcMy.left,rcMy.top,rcMy.Size().cx,rcMy.Size().cy,SWP_SHOWWINDOW);
		}
		else
		{
			//取消最前显示
			SetWindowPos(GetHWND(), CWnd::wndNoTopMost.GetSafeHwnd(),rcMy.left,rcMy.top,rcMy.Size().cx,rcMy.Size().cy,SWP_SHOWWINDOW);
		}

	}
	else
	{
		// 靠边界由鼠标测试
		POINT           pt;
		GetCursorPos(&pt);
		int nW=pRect->right-pRect->left;
		if (m_alignType == ALIGN_TOP)
		{
			//由于我们移动窗体时，鼠标在标题栏内，当鼠标位置超过有效距离后，我们可以考虑用户要向下拖动鼠标。我们便解除上部停靠。
			if (pt.y > LEAVE_SIZE) 
			{
				m_alignType = ALIGN_NONE;
				pRect->bottom += LEAVE_SIZE;
				pRect->top = LEAVE_SIZE;
			}
			else
			{
				pRect->bottom -= pRect->top;
				pRect->top = 0;
				if (pRect->left < LEAVE_SIZE) //在上部停靠时，我们也考虑左右边角。
				{
					pRect->right -= pRect->left;
					pRect->left = 0;
				}
				else if (pRect->right + LEAVE_SIZE> m_nScreenW)
				{
					pRect->left += (m_nScreenW - pRect->right);
					pRect->right = m_nScreenW;
				}
			}

		}
		if (m_alignType == ALIGN_LEFT)
		{
			// 将窗体移出桌面左边
			if(pRect->left > LEAVE_SIZE)
			{
				m_alignType = ALIGN_NONE;
				pRect->right = LEAVE_SIZE+nW;
				pRect->left = LEAVE_SIZE;
			}
			// 如果用户想移动窗体到左桌面里去则限制移入
			else if(pRect->left < 0)
			{
				m_alignType = ALIGN_LEFT;
				pRect->right = nW;
				pRect->left = 0;
			}
			// 禁止窗体移到桌面下方
			//else if(pRect->bottom + NEAR_SIZE >m_nScreenH)
			//{
			//	m_alignType = ALIGN_LEFT;
			//	nS=pRect->bottom-pRect->top;
			//	pRect->top = m_nScreenH-nS;
			//	pRect->bottom = pRect->top + nS;
			//}
		}
		else if (m_alignType == ALIGN_RIGHT)
		{
			// 将窗体移出桌面右边
			if(pRect->right+LEAVE_SIZE < m_nScreenW)
			{
				m_alignType = ALIGN_NONE;
				pRect->left = m_nScreenW - LEAVE_SIZE - nW;
				pRect->right = m_nScreenW - LEAVE_SIZE;
			}
			else if(pRect->right > m_nScreenW)
			{
				m_alignType = ALIGN_RIGHT;
				pRect->left = m_nScreenW - nW;
				pRect->right = m_nScreenW;
			}
			// 禁止窗体移到桌面下方
			//else if(pRect->bottom + NEAR_SIZE >m_nScreenH)
			//{
			//	m_alignType = ALIGN_RIGHT;
			//	nS=pRect->bottom-pRect->top;
			//	pRect->top = m_nScreenH-nS;
			//	pRect->bottom = pRect->top + nS;
			//}
		}
		if(m_alignType != ALIGN_NONE)
		{
			// 主窗体抛弃子窗体
			//MainDetach();
		}
		
		//SetWindowPos(&wndNoTopMost,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
		WORD nHide = 0;
		g_LocalData.GetHideWinConfig(nHide);//是否自动隐藏
		if(nHide!=0)
		{
			SetWindowPos(GetHWND(), CWnd::wndNoTopMost.GetSafeHwnd(),0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
		}
		   
	}
}





/*
窗体的显示隐藏由该函数完成,参数hide决定显示还是隐藏.
*/
void        CMainFrame::HideSide(BOOL hide,const CWnd*pAfter)
{
	RECT            rc;
	int             moves = 1;  //动画滚动窗体的步数,如果不够平滑,可以增大该值.
	int             xStep, yStep;
	int             xEnd, yEnd;
	int             width;
	int             height;
	register int    i;
	GetWindowRect(GetHWND(), &rc);
	/*if(rc.bottom - NEAR_SIDE <= 0 ||
		rc.right - NEAR_SIDE <= 0 )
		return;*/
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;


	//下边判断窗体该如何移动,由停靠方式决定

	switch (m_alignType)
	{
	case ALIGN_TOP:
		{
			//向上移藏
			xStep = 0;
			xEnd = rc.left;
			if (hide)
			{
				yStep = -rc.bottom / moves;
				yEnd = -height + NEAR_SIDE;
			}
			else
			{
				yStep = -rc.top / moves;
				yEnd = 0;
			}
			break;
		}
	case ALIGN_LEFT:
		{
			//向左移藏
			yStep = 0;
			yEnd = rc.top;
			if (hide)
			{
				xStep = -rc.right / moves;
				xEnd = -width + NEAR_SIDE;
			}
			else
			{
				xStep = -rc.left / moves;
				xEnd = 0;
			}
			break;
		}
	case ALIGN_RIGHT:
		{
			//向右移藏
			yStep = 0;
			yEnd = rc.top;
			if (hide)
			{
				xStep = (m_nScreenW - rc.left) / moves;
				xEnd = m_nScreenW - NEAR_SIDE;
			}
			else
			{
				xStep = (m_nScreenW - rc.right) / moves;
				xEnd = m_nScreenW - width;
			}
			break;
		}
	default:
		// 没有停靠，则为显示
		//if(m_pLastAfter != &)
		//	SetWindowPos( pAfter, rc.left,rc.top, 0, 0, SWP_NOSIZE);
		m_bHide=FALSE;
		m_pLastAfter = pAfter;
		return;
	}
	
	//动画滚动窗体.
	for (i = 0; i < moves; i++)
	{
		rc.left += xStep;
		rc.top += yStep;
		SetWindowPos(GetHWND(), pAfter->GetSafeHwnd(), rc.left, rc.top, 0, 0, 
			SWP_NOSIZE |SWP_NOCOPYBITS);//SWP_NOSENDCHANGING
		//if(i%3)
		//{
		//Invalidate();
		//if(hide == false)
			RedrawWindow(GetHWND(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//}
		Sleep(1);
	}
	//RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	
	SetWindowPos(GetHWND(), pAfter->GetSafeHwnd(), xEnd, yEnd, 0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER);
	m_pLastAfter = pAfter;
	m_bHide=hide;
	if(m_bHide)
	{
//		MainDetach();
	}
}

LRESULT CMainFrame::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
	AddTaskIcon();
	return 0;
}

LRESULT CMainFrame::OnWebPictureNotify(WPARAM w,LPARAM l)
{
	CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)l;
	return 0;
}

void CMainFrame::UpdateUserSign()
{
	CString s = m_pEditSign->GetText().GetData();
	s.Trim();

	char user_sign[255];
	strcpy(user_sign,(LPCSTR)s);

	if ( strcmp(user_sign,g_ImUserInfo.user_sign)!=0 )
	{
		strcpy(g_ImUserInfo.user_sign,user_sign);

		ImNotifyMySign();

		if ( m_hNotifyWebUserSign )
		{
			try
			{
				TerminateThread(m_hNotifyWebUserSign,0);
				CloseHandle(m_hNotifyWebUserSign);
			}
			catch(...)
			{
			}

			m_hNotifyWebUserSign=NULL;
		}
	}

	int nGroup = 0;
	int nItem = 0;
	if(m_pFriendList->FindFriend(&g_ImUserInfo, nGroup, nItem))
	{
		FRIEND_INFO *pFriend = ::FindFriend(g_ImUserInfo.GetId());
		ASSERT(pFriend != NULL);
		int nGroupIndex = pFriend->group_index;
		memcpy(pFriend, (FRIEND_INFO*)&g_ImUserInfo, sizeof(FRIEND_INFO));
		pFriend->group_index = nGroupIndex;
		m_pFriendList->SetItem(nGroup, nItem, UserInfoToStr(pFriend), (DWORD)(pFriend));
	}
}

void CMainFrame::ImNotifyMySign()
{
	g_pProtocol->SendReq(g_ImUserInfo.user_sign,g_ImUserInfo.GetId());
	//发修改个人资料的消息，解决好友无法收到个性签名修改的问题
	FRIEND_INFO user = g_ImUserInfo;
	g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);

	XT_MSG msg;

	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

	msg.msg_type  = 0;
	msg.data_type = XT_MSG::USER_SIGN;
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);
	strcpy(msg.data,g_ImUserInfo.user_sign);
	msg.data_len    = strlen(msg.data)+1;

	FRIEND_LIST::iterator it;
	for ( it = g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
	{
		FRIEND_INFO *pFriend = (*it);
		msg.to_id		= pFriend->GetId();
		msg.dest_ip		= pFriend->ip;
		msg.dest_port	= pFriend->port;
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			continue;
		}
		if ( msg.dest_ip==0 )
			g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);
		else
			g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_P2P);
	}	
}

void CMainFrame::SafeTerminateThreads()
{
	if ( m_hMiniWebSession )
	{
		try
		{
			TerminateThread(m_hMiniWebSession,0);
			CloseHandle(m_hMiniWebSession);
		}
		catch(...)
		{
		}
		m_hMiniWebSession=NULL;
	}

	if ( m_hNotifyWebUserSign )
	{
		try
		{
			TerminateThread(m_hNotifyWebUserSign,0);
			CloseHandle(m_hNotifyWebUserSign);
		}
		catch(...)
		{
		}

		m_hNotifyWebUserSign=NULL;
	} 
}

UINT CMainFrame::MiniWebSessionProc(LPVOID param)
{
	while(1)
	{
		try
		{
			CWebInterfaceWrapper objWI;
			objWI.InitInterface(g_ImUserInfo.GetId(),g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname);
			objWI.MiniOnline();
		}
		catch(...)
		{
		}
		
		Sleep(1000*60*10);//sleep 10 minutes
	}
	return 0;
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch(nID)
	{
	case SC_RESTORE:
		{
			//if(m_pLeftTopSub)
			//	OnActiveSub(m_pLeftTopSub);
			//if(m_pRightTopSub)
			//	OnActiveSub(m_pRightTopSub);

			if(m_bHide)
			{
				// 显示出窗体
				HideSide(FALSE,&CWnd::wndTopMost);
				// 强制显示主窗体
				m_bForceShow = TRUE;
				SetForegroundWindow(GetHWND());
				BringWindowToTop(GetHWND());
				return;
			}
			BringWindowToTop(GetHWND());

		}
		break;
	case SC_MINIMIZE:
		{
			//int screen_cx = GetSystemMetrics(SM_CXSCREEN);
			//int screen_cy = GetSystemMetrics(SM_CYSCREEN);
			//CRect rcFrom, rcTo;
			//GetWindowRect(&rcFrom);
			//rcTo.left	= screen_cx-120;
			//rcTo.top	= screen_cy-20;
			//rcTo.right  = screen_cx-100;
			//rcTo.bottom = screen_cy;
			//DrawAnimatedRects(IDANI_CAPTION,&rcFrom,&rcTo);
			break;
		}
	}
}

HRESULT CMainFrame::OnIMESetContext(WPARAM w,LPARAM l)
{
	if(m_bHide)
	{
		// 显示出窗体
		//HideSide(FALSE,&wndTopMost);
		// 强制显示主窗体
		//m_bForceShow=TRUE;
	}
	return 0;
}

LRESULT CMainFrame::OnCallMe(WPARAM w,LPARAM l)
{
	CString strFromId, strFromNickName, strToId, strToNickName;
	CString strParam = (char *)l;

	::GetSectionValue(strParam,"<fromid>","</fromid>",strFromId);
	::GetSectionValue(strParam,"<fromname>","</fromname>",strFromNickName);
	::GetSectionValue(strParam,"<toid>","</toid>",strToId);
	::GetSectionValue(strParam,"<toname>","</toname>",strToNickName);

	int friendid = atoi((LPCSTR)strToId) ;

	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = NULL;
	FRIEND_INFO* pFriend= NULL;

	if( friendid!=g_ImUserInfo.GetId() )
	{
		pFriend = ::FindFriend(friendid);
		if( pFriend)
		{
			pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		}
		else
		{
			pFriend = new FRIEND_INFO;
			pFriend->SetId(friendid);
			pFriend->SetUserName("");
			pFriend->SetGuid("");
			pFriend->SetPassword("");
			strcpy(pFriend->nickname,(LPCSTR)strToNickName);
			pFriend->group_index = 1;//陌生人
			g_lsFriend.push_back(pFriend);
			pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

			XT_STATUS_GET_REQ reqStatus;
			reqStatus.fid = friendid;
			reqStatus.id  = g_ImUserInfo.GetId();
			g_pProtocol->SendReq(reqStatus);

			//XT_USERINFO_GET_REQ reqUserInfo;
			//reqUserInfo.fid = friendid;
			//g_pProtocol->SendReq(reqUserInfo);	
#ifdef _BUSINESS_VESION
			CMainFrame::GetInstance()->Hangup();
#endif
		}
	}

	if( pDlg)
	{
		if ( pDlg->GetHWND()==NULL )
			//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

		if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			pDlg->ShowWindow(/*SW_SHOW*/true);

		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
			pDlg->ShowWindow(/*SW_RESTORE*/true);

		/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());

	}
	return 1;
}

HRESULT CMainFrame::OnHotKey(WPARAM w,LPARAM l)
{
	if(m_pScreenCtrl != NULL)
	{
		return 0;
	}
	int idHotKey = (int) w;              // identifier of hot key 
	int fuModifiers = (UINT) LOWORD(l);  // key-modifier flags 
	int uVirtKey = (UINT) HIWORD(l);     // virtual-key code 
	if(idHotKey != m_nHotkeyID && idHotKey != m_nSnapShotkeyID)//查看消息和截图都不满足
	{
		return 0;
	}
	if(idHotKey == m_nHotkeyID)//查看消息
	{
		CDuiMsgDlg::Show();
	}
	if(idHotKey == m_nSnapShotkeyID)////截图热键
	{
		////一般对话框
// 		extern HWND  g_hP2PSendMSG;
// 		::PostMessage(g_hP2PSendMSG,WM_SNAPSHOT,1,0);
// 
// 		////群对话框显示
// 		extern HWND  g_hClusterSendMSG;
// 		::PostMessage(g_hClusterSendMSG,WM_SNAPSHOT,1,0);
		extern HWND  g_hP2PSendMSG;//一般对话框
		extern HWND  g_hClusterSendMSG;//群对话框显示
		if(g_hP2PSendMSG && GetActiveWindow() == g_hP2PSendMSG)
		{
			::PostMessage(g_hP2PSendMSG,WM_SNAPSHOT,1,0);
		}
		else if(g_hClusterSendMSG && GetActiveWindow() == g_hClusterSendMSG)
		{
			::PostMessage(g_hClusterSendMSG,WM_SNAPSHOT,1,0);
		}
		else if(CGroupSendDlg::GetInstance() != NULL && CGroupSendDlg::GetInstance()->GetHWND() == GetActiveWindow())
		{
			::PostMessage(CGroupSendDlg::GetInstance()->GetHWND(),WM_SNAPSHOT,1,0);
		}
		else
		{
			if(g_pMessageHandleWnd->GetScreenCtrl() == NULL)
			{
				g_pMessageHandleWnd->NewScreenCtrl();
				if(IDOK == g_pMessageHandleWnd->GetScreenCtrl()->DoModal())
				{
					CString strCacheFileName;
					g_LocalData.SaveImageFileToCache(g_pMessageHandleWnd->GetScreenCtrl()->m_strImageFile,strCacheFileName);
					CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;
					g_pMessageHandleWnd->DeleteScreenCtrl();
				}
				else
				{
					g_pMessageHandleWnd->DeleteScreenCtrl();
				}
			}
		}
	}
	if ( 1 == ::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_TRAYICON,0,WM_LBUTTONDBLCLK) )
	{
		return 1;//floorcluster 已经处理了该消息
	}
	if ( IsWindowVisible(GetHWND())==false )
	{
		int screen_cx = GetSystemMetrics(SM_CXSCREEN);
		int screen_cy = GetSystemMetrics(SM_CYSCREEN);

		CRect rcFrom, rcTo;
		GetWindowRect(GetHWND(), &rcTo);

		rcFrom.left	= screen_cx-120;
		rcFrom.top	= screen_cy-20;
		rcFrom.right  = screen_cx-100;
		rcFrom.bottom = screen_cy;

		DrawAnimatedRects(GetHWND(), IDANI_CAPTION,&rcFrom,&rcTo);//从右下角托盘处弹出窗体

		ShowWindow(SW_SHOW);
	}
	if ( IsIconic(GetHWND()) )
	{
		ShowWindow(SW_RESTORE);
	}
	// 显示出窗体
	HideSide(FALSE,&CWnd::wndTopMost);
	// 强制显示主窗体
	m_bForceShow=TRUE;

	SetForegroundWindow(GetHWND());
	BringWindowToTop(GetHWND());
	
	return 0;
}

HRESULT CMainFrame::OnAddEmotion(WPARAM w,LPARAM l)
{
	if(m_szExternPic[0] == 0)
	{
		return -1;
	}
	CAddEmotionDlg dlg(0,m_szExternPic,CWnd::FromHandle(GetDesktopWindow()));
	dlg.SetAddPic();
	dlg.DoModal();
	return 0;
}

HRESULT CMainFrame::OnCopyUrl(WPARAM w,LPARAM l)
{
	if(OpenClipboard(GetHWND()))
	{
		HGLOBAL clipbuffer;
		char * buffer;
		size_t nLen = strlen(m_szExternPic);
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, nLen+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, m_szExternPic);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
	return 0;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if(pCopyDataStruct)
	{
		if(pCopyDataStruct->cbData < 256)
		{
			if(pCopyDataStruct->lpData)
				memcpy(m_szExternPic,pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		}
	}
	return TRUE;
	//return CXSkinDlg::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnMenuMedia()
{
	if(g_pWebConfig)
	{
		MiniOpenUrl( (LPCSTR)g_pWebConfig->GetWebPageUrl(UPLOAD_MEDIA,StrToWideStr(g_ImUserInfo.GetUserName().c_str()) ) );
	}

}

LRESULT CMainFrame::OnExitMainFrm(WPARAM wParam,LPARAM lParam)
{
	int nRet = wParam;
	if(nRet == 1)//确定按钮
	{
		CLOSE();
	}
	return S_OK;
}

LRESULT CMainFrame::OpenSellerMsgWin(WPARAM wParam,LPARAM lParam)
{
	int nSellerId = (int)wParam;

	char sSellerInfo[512];
	FILE *fp;
	fp = fopen(g_LocalData.GetPath()+"\\xxxzx.txt","rt");
	if(fp != NULL)
	{
		fread(sSellerInfo,512,1,fp);
		fclose(fp);
	}


	Json::Reader jReader;
	Json::Value jvSellerInfo;
	
	jReader.parse(sSellerInfo,jvSellerInfo);
	std::string strCustomerName = jvSellerInfo["customerName"].asString();	
	std::string strSubMerchantName = jvSellerInfo["subMerchantName"].asString();
	int nMerchantID = jvSellerInfo["merchantID"].asInt();
	int nSubMerchantID = jvSellerInfo["subMerchantID"].asInt();
	int nProductID = jvSellerInfo["productID"].asInt();

	
	//在获取好友列表后ok后，打开与客服的对话框
	if(!strCustomerName.empty() && !strSubMerchantName.empty()  && nMerchantID != 0 && nSubMerchantID != 0 )
	{
		FRIEND_INFO* pFriend= NULL;
		int friendid =	nSubMerchantID;
		pFriend = ::FindFriend(friendid);
		if( pFriend)
		{
		}
		else
		{
			pFriend = new FRIEND_INFO;
			pFriend->SetId(friendid);
			pFriend->SetUserName("");
			pFriend->SetGuid("");
			pFriend->SetPassword("");
			strcpy(pFriend->nickname, strSubMerchantName.c_str());
			pFriend->group_index = 1;//陌生人
			g_lsFriend.push_back(pFriend);
	
			XT_STATUS_GET_REQ reqStatus;
			reqStatus.fid = friendid;
			reqStatus.id  = g_ImUserInfo.GetId();
			g_pProtocol->SendReq(reqStatus);
	
			//XT_USERINFO_GET_REQ reqUserInfo;
			//reqUserInfo.fid = friendid;
			//g_pProtocol->SendReq(reqUserInfo);	
#ifdef _BUSINESS_VESION
			CMainFrame::GetInstance()->Hangup();
#endif
		}
	
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		if ( pDlg->GetHWND()==NULL )
			//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

		if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			pDlg->ShowWindow(/*SW_SHOW*/true);

		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
			pDlg->ShowWindow(/*SW_RESTORE*/true);

		/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
	
	}

	return S_OK;
}

LRESULT CMainFrame::OpenWorkMsgWin(WPARAM wParam,LPARAM lParam)
{
	int nSellerId = (int)wParam;

	char sSellerInfo[512];
	FILE *fp;
	fp = fopen(g_LocalData.GetPath()+"\\xxxzx.txt","rt");
	if(fp != NULL)
	{
		fread(sSellerInfo,512,1,fp);
		fclose(fp);
	}


	Json::Reader jReader;
	Json::Value jvSellerInfo;

	jReader.parse(sSellerInfo,jvSellerInfo);
	int nMerchantID = jvSellerInfo["merchantID"].asInt();

	std::vector<SubMerchantIdInfo> vecSubMerchantIdInfo;
	g_LocalData.GetAllSubMerchantIdFromMerchantId(nMerchantID, vecSubMerchantIdInfo);
	
	//char buff[MAX_SEND_BUFFERS];
	//LPXT_GET_USER_STATUS_REQ req = (LPXT_GET_USER_STATUS_REQ)buff;
	//req->cnt = vecSubMerchantIdInfo.size();
	for(int nSubMerchant = 0; nSubMerchant < vecSubMerchantIdInfo.size(); nSubMerchant++)
	{
		if(vecSubMerchantIdInfo[nSubMerchant].bIsMain)
		{
			int nFriendId = vecSubMerchantIdInfo[nSubMerchant].nId;
			FRIEND_INFO* pFriend = ::GetFriendInfo(nFriendId);
			CDuiP2PSendMsgDlg *pDlg = NULL;
			if( pFriend)
			{
				pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
			}
			else
			{
				FRIEND_INFO friendinfo;
				friendinfo.SetId(nFriendId);

				pFriend = new FRIEND_INFO;
				*pFriend = friendinfo;
				pFriend->group_index = 1;//陌生人
				AddFriend(pFriend, false);
				pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
			}	
			::BringWindowToTop(pDlg->GetHWND());
#ifdef _CS_PLAT_EX_
			CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#elif defined _WORK_EX_
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#else
			if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))
				CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#endif
			break;
		}
		//req->id[nSubMerchant] = vecSubMerchantIdInfo[nSubMerchant].nId;
	}
	//g_pProtocol->SendReq(*req);
	return S_OK;
}

LRESULT CMainFrame::OpenCsInsertMsgWin(WPARAM wParam,LPARAM lParam)
{
	char sSellerInfo[512];
	FILE *fp;
	fp = fopen(g_LocalData.GetPath()+"\\xxxzy.txt","rt");
	if(fp != NULL)
	{
		fread(sSellerInfo,512,1,fp);
		fclose(fp);
	}


	Json::Reader jReader;
	Json::Value jvSellerInfo;

	jReader.parse(sSellerInfo,jvSellerInfo);
	//int nMerchantID = jvSellerInfo["merchantID"].asInt();
	int nMonitorID = jvSellerInfo["monitorID"].asInt();
	int nServiceID = jvSellerInfo["serviceID"].asInt();
	int nCustomerID = jvSellerInfo["customerID"].asInt();


	FRIEND_INFO* pFriend = ::GetFriendInfo(nServiceID);
	CDuiP2PSendMsgDlg *pDlg = NULL;
	if( pFriend)
	{
		pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	}
	else
	{
		FRIEND_INFO friendinfo;
		friendinfo.SetId(nServiceID);

		pFriend = new FRIEND_INFO;
		*pFriend = friendinfo;
		pFriend->group_index = 1;//陌生人
		AddFriend(pFriend, false);
		pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	}	
	::BringWindowToTop(pDlg->GetHWND());
	g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;
	//CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	CDuiCSPlatDlg::GetInstance()->MoveP2PSendMsgDlg(pDlg);

	//插入会话
	pDlg->InsertMsg(nServiceID, nCustomerID);
	return S_OK;
}

void CMainFrame::OnBnClickedBtnNearCommCust()
{
	CNearCommCustDlg::Show();
}

#ifdef _BUSINESS_VESION
// 更新最近联系客户应答
void CMainFrame::OnUpdateNearCommCustInfoAck(const char * data, WORD wRet)
{
	LPXT_UPDATE_NEAR_COMM_CUST_INFO_ACK msg = (LPXT_UPDATE_NEAR_COMM_CUST_INFO_ACK)data;
	if (msg->ret != 0)
	{
		m_bTranComplete = TRUE;
		//ASSERT(FALSE);
	}

	// 最后一批次传输完成，调用结束接口
	if (m_bTranComplete)
	{
		KillTimer(GetHWND(), TIMER_CLOSE);
		m_bHasUpdateNearCommCust = true;
		//CLOSE();
	}	
}

// 统计需要更新到后台的最近联系客户信息
void CMainFrame::StatNeedUpdateCustInfo(FRIEND_INFO *pUser)
{
	//return;
	m_vecNeedUpdateCommInfo.clear();

	const FRIEND_LIST *pFl = m_pRecentList->GetRecentFriend();
	FRIEND_LIST::const_iterator it = pFl->begin();
	// 循环遍历最近联系人，如果为客户，则获取最后发送消息的时间，将信息保存，待保存到后台

	for (; it != pFl->end(); ++it)
	{
		FRIEND_INFO *pFriend = NULL;
		CLUSTER_INFO *pCluster = NULL;
		CDuiRecentList::GetRealValue((DWORD)(*it), pFriend, pCluster);
		if(pFriend == NULL)
			continue;
		if(pUser->GetId() == (*it)->GetId())
		{
			int nGroupIndex = (*it)->group_index;
			if (CheckSubGroupIsInMyTeam(nGroupIndex) || !IsBuyer(pUser->GetId()))//不是买家也不写入最近联系客户
			{	// 根据最近联系人的分组ID判断他是否属于客户
				continue;
			}

			// 获取最近的一条消息的时间
			MSG_LIST ml1;
			g_LocalData.GetLastMsgList((*it)->GetId(), CDataManager::USER, ml1, 1);
			POSITION pos = ml1.GetTailPosition();
			if (pos == 0)
			{	//位置出错
				continue;
			}
			// 时间格式化
			uint32 uiSendTime = ml1.GetAt(pos).send_time;
			CTime  tm(uiSendTime);
			CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");

			XT_COMMINFO_UPDATE comminfo;
			strcpy(comminfo.szCommAccount, (*it)->GetUserName().c_str());
			strcpy(comminfo.szCommTime, strTime);
			m_vecNeedUpdateCommInfo.push_back(comminfo);
		}
	}
}

// 更新最近联系客户信息到后台
void CMainFrame::UpdateCustInfoToServer()
{
	if (m_vecNeedUpdateCommInfo.size() == 0)
	{
		m_bHasUpdateNearCommCust = true;
		return;
	}

	// 15s后检查最近联系客户是否传输完毕，如果未完毕则当做已掉线，强制关闭程序
	// 一次只更新10条记录
	m_bTranComplete = FALSE;
	int nLastCount = m_vecNeedUpdateCommInfo.size() % 10;
	int nBatch = m_vecNeedUpdateCommInfo.size() / 10;
	if (nLastCount > 0)
	{
		nBatch++;
	}

	for (int i = 0; i < nBatch; ++i)
	{
		int nCount = 10;
		if (i == nBatch - 1 && nLastCount > 0)
		{
			m_bTranComplete = TRUE;			// 最后一次标记更新完成
			nCount = nLastCount;
		}
		XT_UPDATE_NEAR_COMM_CUST_INFO_REQ *pstruNearCommCust
			= (XT_UPDATE_NEAR_COMM_CUST_INFO_REQ*)malloc(sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_REQ)
			+ nCount*sizeof(XT_COMMINFO_UPDATE));
		memset(pstruNearCommCust, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_REQ) + nCount*sizeof(XT_COMMINFO_UPDATE));
		pstruNearCommCust->uiBusinessID = g_ImUserInfo.GetId();
		pstruNearCommCust->uiCommCount = nCount;
		for (int j = 0; j < nCount; ++j)
		{
			strcpy(pstruNearCommCust->CommInfo[j].szCommAccount, m_vecNeedUpdateCommInfo[i * 10 + j].szCommAccount);
			strcpy(pstruNearCommCust->CommInfo[j].szCommTime, m_vecNeedUpdateCommInfo[i * 10 + j].szCommTime);
		}

		g_pProtocol->SendReq(*pstruNearCommCust);

		free(pstruNearCommCust);
		pstruNearCommCust = NULL;
	}
}

// 子账号挂起功能
void CMainFrame::Hangup()
{
	SetTimer(GetHWND(), TIMER_MOD_HANGUP_STATE, 200, NULL);
}

#endif

// 相应系统设置窗口的自定义消息
LRESULT CMainFrame::OnSelfMsgSystemSettingDlgMsg(WPARAM wP, LPARAM lP)
{
	int nMsgType = (int)wP;
	switch(nMsgType)
	{
	case 0:
		{	// 系统设置窗口点击了确认
			WORD nHide = 0;
			g_LocalData.GetHideWinConfig(nHide);//是否自动隐藏
			if(g_oldHide == 1 && nHide == 0 && (m_alignType == ALIGN_TOP || m_alignType == ALIGN_LEFT || m_alignType == ALIGN_RIGHT))
			{	// 设置为了非自动隐藏，如果此时窗口已隐藏，则显示出来	
				// 显示出窗体
				HideSide(FALSE,NULL);
				// 强制显示主窗体
				m_bForceShow=TRUE;

				SetForegroundWindow(GetHWND());
				BringWindowToTop(GetHWND());
			}

			if (g_oldHide == 0 && nHide == 1)
			{
				m_alignType = ALIGN_NONE;
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				OnMoving(0, &rc);
			}
		}
		break;
	case 1:
		{
		  SetWndHotKey();////设置窗口热键
		}
		break;
	default:
		break;
	}
	return 0;
}

// 指定发送
void CMainFrame::OnSpecifySend()
{
	if(m_pSecifySendDlg != NULL)
	{
		delete m_pSecifySendDlg;
		m_pSecifySendDlg = NULL;
	}
    
	CSpecifySendDlg::Show();
	//m_pSecifySendDlg = new CSpecifySendDlg();
	//m_pSecifySendDlg->Create(CWnd::FromHandle(GetHWND()));
	//m_pSecifySendDlg->CenterWindow();
}


// 指定发送
void CMainFrame::OnSoftUpdate()
{
	if(m_bIsNewVersion)
	{
		CMessageBox::Model(GetHWND(), _T("您已经更新到最新版本哞哞"), IM_CAPTION, MB_OK);
	}
	else
	//升级到最新版本
	{
	/*
		CVersionUpdatgeDlg *pDlg = new CVersionUpdatgeDlg(12,m_strVersionUrl);
		pDlg->ShowWindow(SW_SHOW);
		*/
 		CVersionUpdateDlgDui::Show(12, m_strVersionUrl);
	}

}


// “指定发送”窗口返回的消息
LRESULT CMainFrame::OnSpecSendDlgMsg(WPARAM wp, LPARAM lp)
{
	delete m_pSecifySendDlg;
	m_pSecifySendDlg = NULL;

	return 0;
}


LRESULT CMainFrame::OnModBackName(WPARAM wp, LPARAM lp)
{
	tr1::shared_ptr<CModfCommNameDlg> pDlg = CModfCommNameDlg::GetInstance();
	pDlg->Close(ID_OK);
	return 0;
}

// 当鼠标移动到头像区域时，显示个人资料
//void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
//{
//	CXSkinDlg::OnMouseMove(nFlags, point);
//
//	CRect rc;
//	m_wndFaceIcon.GetWindowRect(rc);
//	ClientToScreen(&point);
//	if(rc.PtInRect(point))
//	{
//		SetTimer(GetHWND(), TIMER_SHOW_USERCARD, 500, NULL);
//	}
//}

// 当弹出用户信息卡片时，头像区域被点击的消息相应
LRESULT CMainFrame::OnUserCardDlgFaceClicked(WPARAM wp, LPARAM lp)
{
	return 0;
}

// 点击头像
void CMainFrame::OnBnClickedFaceIcon()
{
	// 	if(m_pSelfInfoDlg != NULL)
	// 	{
	// 		m_pSelfInfoDlg->ShowWindow(SW_SHOW);
	// 		m_pSelfInfoDlg->BringWindowToTop();
	// 	}
	// 	else
	// 	{
	// 		m_pSelfInfoDlg = new CSelfInfoDlg(CWnd::FromHandle(GetHWND()));
	// 		m_pSelfInfoDlg->Create();
	// 		m_pSelfInfoDlg->CenterWindow(CWnd::FromHandle(GetDesktopWindow()));
	// 	}

	CDuiSelfInfoDlg::Show();

	CUserCardDlg::CloseWindow();
}

void CMainFrame::OnBnClickedBtnMerchantEntrance()
{
	// TODO: 在此添加控件通知处理程序代码

////preivew/online访问url一样，只是preview要设置代理
   #ifdef  _VER_PREVIEW_
			OpenHtmlPage("http://merchant.feiniu.com");	
	#endif

	#ifdef _VER_ONLINE_
			OpenHtmlPage("http://merchant.feiniu.com");
	#endif
	
////beta环境
			OpenHtmlPage("http://merchant.beta.fn.com");

}

void CMainFrame::OnForceOfflineCsInd(const char * data, WORD wRet)
{
	//g_pProtocol->Stop();

	if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
	{
		if(g_pProtocol->IsRunning())
		{
			XT_LOGOUT req;
			req.id = g_ImUserInfo.GetId();
			req.authData = 0;

			g_pProtocol->SendReq(req);
		}
	}
	g_pProtocol->Stop();
	CMessageBox::Model(NULL, "客服主管请您下线", NULL, MB_OK);
	//OnMenuExit();
	CLOSE();
}

#ifdef _BUSINESS_VESION
// 客服转接线程参数
struct STRU_CUSTSER_TRAN_THREAD_PARAM
{
	CMainFrame *pMain;
	XT_CUSTSER_TRAN_REQ req;
};

// 收到客服转接请求
void CMainFrame::OnRecvCustSerTranReqInd(const char * data, WORD wRet)
{
	// 发送应答
	XT_CUSTSER_TRAN_REQ *pAck = (XT_CUSTSER_TRAN_REQ*)data;
	XT_CUSTSER_TRAN_ACK req;
	req.ret = 0;
	req.uSid = g_ImUserInfo.GetId();
	req.uDid = pAck->uSid;
	req.uCustomerId = pAck->uCustomerId;

	g_pProtocol->SendReq(req);

	// 打开聊天窗口
	FRIEND_INFO *pFriend = NULL;
	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg;
	pFriend = ::FindFriend(pAck->uCustomerId);
	if(pFriend == NULL)
	{	//更新好友资料
		Sleep(50);
		XT_USERINFO_GET_REQ req;
		req.fid = pAck->uCustomerId;
		g_pProtocol->SendReq(req);
		
		// 获取好友状态
		Sleep(50);
		XT_STATUS_GET_REQ reqStatus;
		reqStatus.fid = pAck->uCustomerId;
		reqStatus.id = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(reqStatus);
	}	

	// (待接待对象可能不在好友列表中)开启线程，已打开聊天窗口
	unsigned threadID;
	STRU_CUSTSER_TRAN_THREAD_PARAM *pParam = new STRU_CUSTSER_TRAN_THREAD_PARAM;
	pParam->pMain = this;
	memcpy(&pParam->req, pAck, sizeof(XT_CUSTSER_TRAN_REQ));
	_beginthreadex(NULL, 0, ThreadCustSerTran, (void*)pParam, 0, &threadID);
}

// 客服转接功能，打开客户聊天窗口
unsigned __stdcall CMainFrame::ThreadCustSerTran(void * pParam)
{
	STRU_CUSTSER_TRAN_THREAD_PARAM *pThreadParam = (STRU_CUSTSER_TRAN_THREAD_PARAM *)pParam;
	// 打开聊天窗口
	uint32 custId = pThreadParam->req.uCustomerId;

	FRIEND_INFO *pFriend = NULL;
	for(int i = 0; i < 10; ++i)
	{
		Sleep(100);

		pFriend = ::FindFriend(custId);
		if(pFriend != NULL)
		{
			break;
		}
	}

	if(pFriend != NULL)
	{	// 打开聊天窗口
		::PostMessage(pThreadParam->pMain->GetHWND(), WM_OPEN_CUST_MSGDLG, (WPARAM)pThreadParam, 0);
	}
	return 0;
}

// 打开客户聊天窗口
LRESULT CMainFrame::OnOpenCustMsgDlg(WPARAM w,LPARAM l)
{
	STRU_CUSTSER_TRAN_THREAD_PARAM *pThreadParam = (STRU_CUSTSER_TRAN_THREAD_PARAM *)w;
	uint32 sourceId = pThreadParam->req.uSid;
	FRIEND_INFO *pCustFriend = ::FindFriend(pThreadParam->req.uCustomerId);
	if(pCustFriend != NULL)
	{	
		// 打开聊天窗口
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg;
		pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pCustFriend);

		if ( pDlg->GetHWND()==NULL )
			//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg = CDuiP2PSendMsgDlg::NewDlg(/*pFriend*/pCustFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

		if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			pDlg->ShowWindow(/*SW_SHOW*/true);
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pCustFriend->GetId()))
		{
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
				CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
			else
			{
				if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL)
					::FlashWindow(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
			}
			CDuiCSPlatDlg::GetInstance()->SetCustUnMsgVisable(true);//显示客户标签的红点
			CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pCustFriend, false, true, false, false, false);
			CDuiCSPlatDlg::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && CMainFrame::IsBuyer(pCustFriend->GetId()))
		{
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1)
				CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
			else
			{
				if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL)
					::FlashWindow(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
			}
			CDuiCounsel::GetInstance()->SetFriendListElement(pCustFriend, false, true, false, false, false);
			CDuiCounsel::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
#endif
		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
			pDlg->ShowWindow(/*SW_RESTORE*/true);

		/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());

		// 发送转接消息结束消息
		XT_MSG msg;
		memset(&msg,0,sizeof(msg));

		msg.ver         = XTREAM_CLIENT_VERSION;
		msg.from_id     = g_ImUserInfo.GetId();

		msg.fontColor   = g_ImFont.fontColor;
		msg.fontSize    = g_ImFont.fontSize;
		msg.fontStyle   = g_ImFont.flags;
		strcpy(msg.fontName,g_ImFont.fontName);
		strcpy(msg.from_nickname,g_ImUserInfo.nickname);

		msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

		msg.to_id		= pThreadParam->req.uCustomerId;
		msg.dest_ip		= pCustFriend->ip;
		msg.dest_port	= pCustFriend->port;
		msg.msg_type    = 0;

		msg.data_type = XT_MSG::CUST_SER_TRAN_REQ;
		strcpy(msg.from_nickname, g_ImUserInfo.nickname);

		Json::FastWriter jWriter;
		Json::Value jvSellerInfo;
		jvSellerInfo["switchStatus"] = 0;			// 1 表示转接开始 0 表示转接成功 2 表示主管接手会话
		jvSellerInfo["toSubMerchantID"] = g_ImUserInfo.GetId();
		jvSellerInfo["fromSubMerchantID"] = sourceId;
#ifdef _CS_PLAT_EX_
		jvSellerInfo["toSubMerchantName"] = g_ImUserInfo.GetUserName().c_str();
#else
		jvSellerInfo["toSubMerchantName"] = g_ImUserInfo.nickname;
#endif
		string strMsg = jWriter.write(jvSellerInfo);

		strcpy(msg.data, strMsg.c_str());
		msg.data_len = strMsg.length() + 1;

		g_pProtocol->SendP2PMsg(msg);

		////显示转接消息
		pDlg->AddTranMsg(sourceId, pThreadParam->req.szOrderId, pThreadParam->req.szTranReason);	

		BOOL bIsShow = FALSE;
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pCustFriend->GetId()))
		{
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 1)
				pDlg->ShowWindow(false);
			else
				bIsShow = TRUE;
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && CMainFrame::IsBuyer(pCustFriend->GetId()))
		{
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() > 1)
				pDlg->ShowWindow(false);
			else
				bIsShow = TRUE;
		}
#endif

		////显示未读消息
		/*WORD nExitWarn = 0;
		g_LocalData.GetCloseTalkWarnConfig(nExitWarn);///读取配置是否弹出未读消息窗口*/
	   /* if(g_mapMsgUnRead.size()>0)
		{
			PopUnReadMsgDlg();
		}*/

		if(g_mapMsgUnRead.size()>0) ///转接时恢复未读消息
		{
			PopUnReadMsgDlg();
		}

		if(bIsShow)
		{
#ifdef _CS_PLAT_EX_
			CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
			CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pCustFriend, false, true, false, false, false);
#else
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
			CDuiCounsel::GetInstance()->SetFriendListElement(pCustFriend, false, true, false, false, false);
#endif
		}
	}

	delete pThreadParam;
	pThreadParam = NULL;

	return 0;
}

LRESULT CMainFrame::OnOpenOrderMsgDlg(WPARAM w,LPARAM l)
{
	OrderMsg* pOrderMsg = (OrderMsg*)w;
	if( ((pOrderMsg->orderType == 0 || pOrderMsg->orderType == 1 || pOrderMsg->orderType == 2) && g_LocalData.GetOrderSubscibe().orderTradePopup) ||
		(pOrderMsg->orderType == 3 && g_LocalData.GetOrderSubscibe().orderReturnPopup) )
	{
		CDuiOrderNotice::Show(pOrderMsg);
	}
	
	return 0;
}
#endif

// 关闭
void CMainFrame::CLOSE()
{
/////聊天记录实时刷新
//#ifdef _BUSINESS_VESION	// 卖家版退出时更新最近联系人到后台
//	if (g_ImUserInfo.status!=XTREAM_OFFLINE && !m_bHasUpdateNearCommCust)
//	{
//		// 统计需要更新到后台的最近联系客户信息
//		StatNeedUpdateCustInfo();
//		// 更新最近联系客户信息到后台
//		UpdateCustInfoToServer();
//
//		//return;
//	}	
//#endif	

	if ( g_pMessageHandleWnd->HasP2PFile() && g_ImUserInfo.status!=XTREAM_OFFLINE )
	{
		if ( CMessageBox::Model(GetHWND(), "当前有未完成的文件传输，退出哞哞后将全部终止这些文件传输．","文件传输",MB_ICONINFORMATION|MB_OKCANCEL)!=IDOK )
		{
			return ;
		}
	}

	if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
	{
		if(g_pProtocol->IsRunning())
		{
			XT_LOGOUT req;
			req.id = g_ImUserInfo.GetId();
			req.authData = 0;

			g_pProtocol->SendReq(req);
		}
	}

	g_LocalData.SaveAllSystemSetting();

	if(m_nifData.hIcon )
	{
		RemoveTaskIcon();
	}

	LastOpera();
	//强行关闭所有线程
	HANDLE hself = GetCurrentProcess();
	TerminateProcess(hself, 0);

	//Close(IDOK);
}

void CMainFrame::OnMove(int x, int y)
{
	if ( GetHWND() != NULL && x!=0 && y!=0 )
	{
		CRect rcWindow;
		CRect rcSub;
		::GetWindowRect(GetHWND(), &rcWindow);
		g_LocalData.SaveMainFramePosition(rcWindow);
	}
}

// 注册查看消息热键
void CMainFrame::SetWndHotKey()
{
	m_nHotkeyID = GlobalAddAtom("mini139_show_message");
	BOOL bRetMsg = UnregisterHotKey(GetHWND(),m_nHotkeyID);
	WORD wVK;
	WORD wMod;
	WORD wMod2 = 0;
	BOOL b;
	g_LocalData.GetMsgHotKey(wVK,wMod);
	if(wVK == 0 && wMod == 0)
	{
		b = RegisterHotKey(m_hWnd,m_nHotkeyID,MOD_SHIFT|MOD_ALT,'A');
	}
	else
	{
		if(HOTKEYF_ALT & wMod)
			wMod2 |= MOD_ALT;
		if(HOTKEYF_CONTROL & wMod)
			wMod2 |= MOD_CONTROL;
		if(HOTKEYF_SHIFT & wMod)
			wMod2 |= MOD_SHIFT;
		b = RegisterHotKey(GetHWND(),m_nHotkeyID,wMod2,wVK);
	}

	////注册屏幕截图热键
	WORD wMod3 = 0;
	m_nSnapShotkeyID = GlobalAddAtom("mini139_show_snapshot");
	BOOL bRetSnap = UnregisterHotKey(GetHWND(),m_nSnapShotkeyID);
	g_LocalData.GetSnapShotHotKey(wVK,wMod);
	if(wVK == 0 && wMod == 0)
	{
		b = RegisterHotKey(GetHWND(),m_nSnapShotkeyID,MOD_SHIFT|MOD_ALT,'Z');
	}
	else
	{
		if(HOTKEYF_ALT & wMod)
			wMod2 |= MOD_ALT;
		if(HOTKEYF_CONTROL & wMod)
			wMod2 |= MOD_CONTROL;
		if(HOTKEYF_SHIFT & wMod)
			wMod2 |= MOD_SHIFT;
		b = RegisterHotKey(GetHWND(),m_nSnapShotkeyID,wMod2,wVK);
	}
}

// 关闭附加动作
void CMainFrame::LastOpera()
{
	if(g_pMessageHandleWnd != NULL)
	{
		g_pMessageHandleWnd->DeleteAllP2PSendMsgDlg();
	}
	CSystemSettingDlg::CloseDlg();
	CFriendInfoDlg::CloseAll();

	g_LocalData.SaveClusterConfig((char*)g_ImUserInfo.GetUserName().c_str());
	g_LocalData.SaveClusterList((char*)g_ImUserInfo.GetUserName().c_str());

	g_LocalData.SaveClusterExitConfig((char*)g_ImUserInfo.GetUserName().c_str());
	g_LocalData.SaveClusterExitList((char*)g_ImUserInfo.GetUserName().c_str());

	ClearClusterList();

	KillTimer(GetHWND(), TIMER_BEAT);
	KillTimer(GetHWND(), TIMER_RELOGIN);
	//m_mapWebInfo.clear();
	m_mapNewFriend.clear();
	ImSaveUserOperations();

	SaveUserWebData();
	SaveLocalFriends();

	SafeTerminateThreads();

	::g_webFamilyHeadIconProcess.SafeTerminate();
	::g_webUserHeadIconProcess.SafeTerminate();
	::g_webFileProcess.SafeTerminate();

	if(CDialogClusterAuth::GetInstance() != NULL && ::IsWindow(CDialogClusterAuth::GetInstance()->GetHWND()))
	{
		CDialogClusterAuth::GetInstance()->Close(IDOK);
	}
	CDailogClusterCheck::Close();
	CUserAuthReqDlg::Close();
	CUserAuthInfoDlg::Close();

	CContactInviteDlg::Close();
	CUserGuideDlg::Close();
	C139MsgDlg::Close();
	C139NoteDlg::Close();
	CSMSDlg::Close();
	CDuiMsgDlg::CloseDlg();
	CAddFriendOrClusterDlg::CloseDlg();
#ifdef _BUSINESS_VESION
	CNearCommCustDlg::CloseDlg();
  #ifdef _CS_PLAT_EX_////飞牛客服工作台
	CModPassWordDlg::CloseDlg();
  #endif
#endif
	CSpecifySendDlg::CloseDlg();
	CGroupSendDlg::CloseDlg();

	//清除好友关注文章，重置初始化标志
	if( g_pHeadPhoto )
		g_pHeadPhoto->Close();
	if(g_pMyArticles)
		g_pMyArticles->Close();
	if(g_pMyPictures)
		g_pMyPictures->Close();

	m_pipeCallMe.Close();
	m_fifoPics.Clear();
	m_pRecentList->Save();
	g_ImUserInfo.status = XTREAM_OFFLINE;
	g_LocalData.SaveCurrentSelTab(m_pLayoutPage->GetCurSel());
	::ClearUserBitmap();//清除用户自定义头像	
}

/////弹出关闭窗口，限制咨询框操作
void CMainFrame::LockDlg()
{
	if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() >= 1)
	{
		//CDuiP2PSendMsgDlg* pDlg = NULL;
		EnableWindow(CDuiCounsel::GetInstance()->GetHWND(),FALSE);
		//for(int i = 0; i < CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size(); i++)
		//{
		//	pDlg = CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg[i];
		//	EnableWindow(pDlg->GetHWND(),FALSE);
		//}
	}
}

/////关闭或者取消，允许咨询框操作
void CMainFrame::ReleaseDlg()
{
	if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() >= 1)
	{
		//CDuiP2PSendMsgDlg* pDlg = NULL;
		EnableWindow(CDuiCounsel::GetInstance()->GetHWND(),TRUE);
		//for(int i = 0; i < CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size(); i++)
		//{
		//	pDlg = CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg[i];
		//	EnableWindow(pDlg->GetHWND(),TRUE);
		//}
	}
}

//保存用户未读的聊天记录
void CMainFrame::SaveUnReadMsgLog(FRIEND_INFO* pFriend,int nUnReadCnt,int nIndex,BOOL bWrite)
{
	////写配置参数	
	if(bWrite)////仅在哞哞退出时，写.ini文件
	{
		DWORD dwFid = pFriend->GetId();
		DWORD dwCnt = nUnReadCnt;//当前的对话条数
		g_LocalData.SaveUnReadParamsCfg(dwFid,dwCnt,nIndex);
	}
	WORD nExitWarn = 0;
	g_LocalData.GetCloseTalkWarnConfig(nExitWarn);//当前退出是否提示
	if(nExitWarn!=0)///提示未读
	{
	   g_mapMsgUnRead[pFriend] = nUnReadCnt;/////保存或者更新好友ID和未读条数
	}
	//else ///不提示未读
	//{
	//	map<FRIEND_INFO *,UINT>::iterator it = g_mapMsgUnRead.find(pFriend);
	//	if(it != g_mapMsgUnRead.end())
	//	{
	//		g_mapMsgUnRead.erase(it);
	//	}
	//}
}

#ifdef _CS_PLAT_EX_
// 系统消息：排队队列增加成员或减少成员
//void CMainFrame::OnWaitQueuChange(const char *data,WORD wRet)
//{
//	LPXT_WAITQUEU_CHANGE ack = (LPXT_WAITQUEU_CHANGE)data;
//	if(ack->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
//	{	// 增加成员
//		CDuiCSPlatDlg::AddWaitFriend(ack->idBuyer);
//	}
//	else
//	{	// 减少成员
//		CDuiCSPlatDlg::DelWaitFriend(ack->idBuyer);
//	}
//}
void CMainFrame::OnWaitQueuChange(const char *data,WORD wRet)
{
	LPXT_WAITQUEU_CHANGE ack = (LPXT_WAITQUEU_CHANGE)data;
	if(ack->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
	{	// 增加成员
		CDuiCSPlatDlg::AddWaitFriend(ack->idBuyer);
	}
	else if(ack->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
	{	// 减少成员
		CDuiCSPlatDlg::DelWaitFriend(ack->idBuyer);
	}
	else if(ack->bAdd == XT_WAITQUEU_CHANGE::TYPE_QUEUE)
	{
		g_LocalData.SetCsTeamQueue(ack->teamId, ack->QueueLength);
		CDuiCSPlatDlg::UpdateWaitFriend();
	}
}
#endif

// 判断id是否为买家
BOOL CMainFrame::IsBuyer(uint32 nID)
{
	if(nID >= 5000000 && nID < 2000000000)
	{
		return TRUE;
	}

	return FALSE;
}

// 判断id是否为商家
BOOL CMainFrame::IsSeller(uint32 nID)
{
	if(nID >= 0 && nID < 5000000)
	{
		return TRUE;
	}

	return FALSE;
}

// 判断id是否为客服工作人员
BOOL CMainFrame::IsCSer(uint32 nID)
{
	if(nID >= 2000000000 && nID < 2050000000)
	{
		return TRUE;
	}

	return FALSE;
}

// 判断id是否为工作版人员
BOOL CMainFrame::IsWork(uint32 nID)
{
	if(nID >= 2050000000)
	{
		return TRUE;
	}

	return FALSE;
}

// 开启线程检测网络连接
unsigned __stdcall CMainFrame::ThreadCheckNetwork(void * pParam)
{
	while(TRUE)
	{
		g_bNetworkOK = InternetCheckConnection("http://www.feiniu.com",FLAG_ICC_FORCE_CONNECTION ,0);
		Sleep(5000);		// 5s检测一次
	}
	return 0;
}

// 开启线程获取所有好友状态
unsigned __stdcall CMainFrame::ThreadGetAllFriendStatus(void * pParam)
{
	XT_STATUS_GET_REQ reqStatus;
	reqStatus.id  = g_ImUserInfo.GetId();

	FRIEND_LIST::iterator it;
	FRIEND_INFO * pFriend ;
	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
	{
		pFriend = (*it);
		if(pFriend == NULL)
		{
			ASSERT(pFriend);
			continue;
		}

		reqStatus.fid = pFriend->GetId();
		g_pProtocol->SendReq(reqStatus);

		Sleep(200);
	}

	return 0;
}

#ifdef _CS_PLAT_EX_
void CMainFrame::OnRecepteCustAck(const char* data,WORD wRet)
{
	XT_RECEPTE_CUST_ACK *pAck = (XT_RECEPTE_CUST_ACK*)data;
	if(pAck->type == XT_RECEPTE_CUST_ACK::TYPE_AUTO_OFFER)
	{
		CDuiCSPlatDlg::DelWaitFriend(pAck->userid);
	}
}

// 买家退出，主动邀约
void CMainFrame::OnBuyOfflineInd(const char* data,WORD wRet)
{
	//UserOfflineIND *pindpb = (UserOfflineIND*)data;
/*
	if(CDuiCSPlatDlg::GetInstance()->GetWaiterCount() > 0)
	{
		XT_RECEPTE_CUST_REQ req;
		req.idBuyer = 0;
		req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER;	// 主动接待
		req.teamId = 0;
		g_pProtocol->SendReq(req);
	}
*/

}
#endif

//cookie中设置token
void CMainFrame::SetCookieToken()
{
	//cookie中设置token
	std::string key = "YOWEJKRLWKFSJK";
	extern CString g_strToken;
	std::string value = g_strToken;
	value = "\"" + value + "\"";
	std::string domain = COOKIE_URL;
	CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager();  
	CefCookie cookie;  
	CefString(&cookie.name).FromASCII(key.c_str());  
	CefString(&cookie.value).FromASCII(value.c_str());  
	CefString(&cookie.domain).FromASCII(domain.c_str());
	CefString(&cookie.path).FromASCII("/");  
	cookie.httponly = true;

	std::string httpDomain = "http://";  
	httpDomain.append(domain);  
	CefPostTask(TID_IO, NewCefRunnableMethod(manager.get(), &CefCookieManager::SetCookie,  CefString(httpDomain.c_str()), cookie));
}

void CMainFrame::GetOrderSubscibe()
{
	BaseReq baseReq;
	baseReq.set_userid(g_ImUserInfo.GetId());
	g_pProtocol->SendReq(baseReq);
}

//获取订单设置选项
void CMainFrame::OnGetOrderSubscibe(const char *data,WORD wRet)
{
	MsgRemindAck* ack = (MsgRemindAck*)data;
	if ( wRet==0 )
	{
		OrderSubscibe orderSubscibe = g_LocalData.GetOrderSubscibe();
		REMIND_TYPE remindTypeNeworder = ack->neworder();
		REMIND_TYPE remindTypePayorder = ack->payorder();
		REMIND_TYPE remindTypeGoodscomment = ack->goodscomment();
		REMIND_TYPE remindTypeReturnorder = ack->returnorder();

		if(remindTypeNeworder == RT_OFF)
			orderSubscibe.orderNew = false;
		else
			orderSubscibe.orderNew = true;

		if(remindTypePayorder == RT_OFF)
			orderSubscibe.orderPaid = false;
		else
			orderSubscibe.orderPaid = true;

		if(remindTypeGoodscomment == RT_OFF)
			orderSubscibe.orderEvaluate = false;
		else
			orderSubscibe.orderEvaluate = true;

		if(remindTypeReturnorder == RT_OFF)
			orderSubscibe.orderReturn = false;
		else
			orderSubscibe.orderReturn = true;

		if(remindTypeNeworder == RT_OFF && remindTypePayorder  == RT_OFF && remindTypeGoodscomment == RT_OFF)
		{
			orderSubscibe.orderTradePopup = true;
		}
		else
		{
			if(remindTypeNeworder == RT_RECEIVE_AND_POPMSG || remindTypePayorder  == RT_RECEIVE_AND_POPMSG || remindTypeGoodscomment == RT_RECEIVE_AND_POPMSG)
			{
				orderSubscibe.orderTradePopup = true;
			}
			else
			{
				orderSubscibe.orderTradePopup = false;
			}
		}
		if(remindTypeReturnorder == RT_OFF)
		{
			orderSubscibe.orderReturnPopup = true;
		}
		else
		{
			if(remindTypeReturnorder == RT_RECEIVE_AND_POPMSG)
			{
				orderSubscibe.orderReturnPopup = true;
			}
			else
			{
				orderSubscibe.orderReturnPopup = false;
			}
		}
		g_LocalData.SetOrderSubscibe(orderSubscibe);
	}
}

void CMainFrame::GetOrderMsgOffline()
{
	BaseReq baseReq;
	baseReq.set_userid(g_ImUserInfo.GetId());
	g_pProtocol->SendReq(baseReq, CMD_GET_OFFLINE_MSGONTIFY);
}

void CMainFrame::SetLabelOrderMsgUnRead(bool bIsUnRead)
{
	m_pLabelUmMsg->SetVisible(bIsUnRead);
}

bool CMainFrame::GetLabelOrderMsgUnRead()
{
	return m_pLabelUmMsg->IsVisible();
}

void CMainFrame::GetCsTeamInfo()
{
	csTeamInfoReq csTeamReq;
	int nId = g_ImUserInfo.GetId();
	csTeamReq.set_csid(nId);
	g_pProtocol->SendReq(csTeamReq);
}

void CMainFrame::OnGetUserStatusAck(const char* data,WORD wRet)
{
	XT_GET_USER_STATUS_ACK *pAck = (XT_GET_USER_STATUS_ACK*)data;

	if(pAck->cnt <= 0)
		return;

	int nFriendId = -1;
	for(int nUser = 0; nUser < pAck->cnt; nUser++)
	{
		pAck->stat[nUser].id;
		pAck->stat[nUser].b;

		if(pAck->stat[nUser].b)
		{
			nFriendId = pAck->stat[nUser].id;
			break;
		}
	}

	if(nFriendId == -1)
		nFriendId = pAck->stat[0].id;

	FRIEND_INFO* pFriend = ::GetFriendInfo(nFriendId);
	CDuiP2PSendMsgDlg *pDlg = NULL;
	if( pFriend)
	{
		pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	}
	else
	{
		FRIEND_INFO friendinfo;
		friendinfo.SetId(nFriendId);

		pFriend = new FRIEND_INFO;
		*pFriend = friendinfo;
		pFriend->group_index = 1;//陌生人
		AddFriend(pFriend, false);
		pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	}	
	::BringWindowToTop(pDlg->GetHWND());
#ifdef _CS_PLAT_EX_
	CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#elif defined _WORK_EX_
	CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#else
	if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#endif
}

void CMainFrame::SetSendMsgQueue()
{
	vector<SendMsgQueue>& vecSendMsgQueue = g_LocalData.GetSendMsgQueue();
	vector<int> vecSeq;
	//for(int nQueue = 0; nQueue < vecSendMsgQueue.size(); nQueue++)
	//{
	//	SendMsgQueue sendMsgQueueInfo = vecSendMsgQueue[nQueue];
	//	if(GetTickCount() - sendMsgQueueInfo.sendTick > 20000)
	//	{
	//		vecSeq.push_back(sendMsgQueueInfo.seqId);
	//		g_pProtocol->SendP2PMsg(sendMsgQueueInfo.msg);
	//	}
	//}
	uint32 tickNow = GetTickCount();
	if(vecSendMsgQueue.size() > 0 && tickNow - vecSendMsgQueue[0].sendTick > 20000)
	{
		int nSize = vecSendMsgQueue.size();
		for(int nQueue = 0; nQueue < nSize; nQueue++)
		{
			SendMsgQueue& sendMsgQueueInfo = vecSendMsgQueue[nQueue];
			if(tickNow - sendMsgQueueInfo.sendTick > 20000)
			{
				//vecSeq.push_back(sendMsgQueueInfo.seqId);
				g_pProtocol->SendP2PMsg(sendMsgQueueInfo.msg, CIMProtocol::SMT_SERVER, sendMsgQueueInfo.seqId);
				return;
				//Sleep(100);
				//CString str;
				//str.Format("\r\n发消息:%s\r\n", sendMsgQueueInfo.msg.data);
				//TRACE(str);
			}
		}
	}
// 	for(int nSeq = 0; nSeq < vecSeq.size(); nSeq++)
// 	{
// 		g_LocalData.DeleteSendMsgQueue(vecSeq[nSeq]);
// 	}
}