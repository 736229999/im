#include "StdAfx.h"
#include "DuiP2PSendMsgDlg.h"
#include "..\imclient.h"
#include ".\p2psendmsgdlg.h"
#include "emotiondlg.h"
#include "msgdlg.h"
#include "..\FriendInfoDlg.h"
#include "MessageHandleWnd.h"
#include "P2PInviteDlg.h"
#include "imagefiledialog.h"
#include "xtpacket.h"
#include "localdata.h"
#include "afxpriv.h"
#include "customlistctrl.h"
#include "screencopydlg.h"
#include "magicfacedlg.h"
#include "webprocess.h"
#include "..\MainFrame.h"
#include "..\urlop.h"
#include "MsgObj.h"
#include "linkresdlg.h"
#include "clientprotocol.h"
#include "SAConfig.h"
#include "../ImTest/ImTest.h"
#include "..\EvalueDlg.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "winuser.h"

#include "../json/json/json.h"
#include "../SystemSettingDlg.h"

#include "../HangupSessionNumSettingDlg.h"

#include "UserCardDlg.h"
#include "ScreenCtrl.h"
#include "DuiCounsel.h"
#include "../CExitMsgWarning.h"

#ifdef _VER_INTERNAL_
#include "..\InternalFriendInfoDlg.h"
#endif
#ifdef _BUSINESS_VESION
#include "..\CustSerTranDlg.h"
#endif
#include "..\DuiMfRecentList.h"
using namespace DuiLib;
#include "P2PSendMsgDlgCloseHint.h"
#include "FastWordGroupRename.h"
#include "FastWordImport.h"
#include "FastWordExport.h"
#include "FastWordNew.h"
#include "FastWordSelectMenu.h"
#include "..\CSPlatDlg.h"
//#include "..\urlop.h"
#include "..\TopBar.h"
#include "../CheckServiceLogWarn.h"
#include "..\..\..\include\ClientProtocol.h"
#include "curl.h"
#include "RichEditUtil.h"
#include "DuiPicture.h"

#define SIZE_VEDIO 189
#define COLOR_MSGTIP  RGB(144,122,88)
CString ToHexString(BYTE bytes[],int len);
HWND g_hP2PSendMSG = NULL;
#define RIGHT_URL_CLIPBOARD_CHECK "moumou-link: "

// 打开接手会话聊天窗口的消息响应
#define WM_OPEN_TRAN_CHAT_MSGDLG          WM_USER+0x129
#define WM_SERVICE_LOG_WORN               WM_USER+0x130

#ifdef _BUSINESS_VESION
const uint32 P2P_DLG_WIDTH = 700;
//聊天框高度
const uint32 P2P_DLG_HIGHT = 500;
//左侧边框宽度
const uint32 BORDER_WIDTH = 0;
//折叠按钮边栏宽度
const int BUTTON_FOLDER_WIDTH = 0;
//右侧广告栏宽度
#ifdef _CS_PLAT_EX_
const uint32 AD_WINDOW_WIDTH = 360;
#elif defined _WORK_EX_
const uint32 AD_WINDOW_WIDTH = 288;
#else
const uint32 AD_WINDOW_WIDTH = 288;
#endif
//右侧工具窗口边框宽度
const uint32 BAR_BORDER_WIDTH = 0;
#else
const uint32 P2P_DLG_WIDTH = 600;
//聊天框高度
const uint32 P2P_DLG_HIGHT = 470;
//左侧边框宽度
const uint32 BORDER_WIDTH = 0;
//折叠按钮边栏宽度
const int BUTTON_FOLDER_WIDTH = 0;
//右侧广告栏宽度
const uint32 AD_WINDOW_WIDTH = 190;
//右侧工具窗口边框宽度
const uint32 BAR_BORDER_WIDTH = 0;
#endif

static long curl_writer(void *data, int size, int nmemb, string *content)
{
	long sizes = size * nmemb;
	*content += string((char*)data, sizes);
	return sizes;
}

// 上传图片到图片空间
BOOL upLoadPic(const char* szPic, string &strURL)
{
	strURL = "";

	CURL *curl;
	CURLcode res;
	string strRet;
	string strError;

	//图片文件保存的全路径
	string strPicPathFile = szPic;

	//bwwan 获取图片文件后缀名
	string extName("jpg");
	string::size_type pos = strPicPathFile.find(".");
	if(pos != std::string::npos)
	{
		extName = strPicPathFile.substr(pos + 1);
	}

	curl = curl_easy_init();
	if(curl) 
	{
		struct curl_httppost *formpost = 0;
		struct curl_httppost *lastptr  = 0;
		curl_formadd(&formpost,  
			&lastptr,  
			CURLFORM_COPYNAME, "img",  
			CURLFORM_FILE, strPicPathFile.c_str(),  
			CURLFORM_END);  
		curl_formadd(&formpost,
			&lastptr, 
			CURLFORM_COPYNAME, "imgName",  
			CURLFORM_COPYCONTENTS, "MouMouPic",
			CURLFORM_END);  
		curl_formadd(&formpost,
			&lastptr, 
			CURLFORM_COPYNAME, "extName",  
			CURLFORM_COPYCONTENTS,extName.c_str(), 
			CURLFORM_END);  
		/*
		curl_formadd(&formpost,
		&lastptr, 
		CURLFORM_COPYNAME, "uploadType",  
		CURLFORM_COPYCONTENTS, "3",
		CURLFORM_END);  
		*/
#ifdef _VER_PREVIEW_		
		curl_easy_setopt(curl, CURLOPT_PROXY, PROXY_URL); 
#endif
		string strHttp = LOAD_PIC_URL;
		curl_easy_setopt(curl, CURLOPT_URL,strHttp.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRet); 
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);   // 30s超时
		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
		{
			strError = curl_easy_strerror(res);
		}

		/* always cleanup */ 
		curl_easy_cleanup(curl);

		if(res == CURLE_OK)
		{
			//解析json字符创
			Json::Reader reader;
			Json::Value recv;
			if (reader.parse(strRet, recv))
			{
				//获取result字段
				bool result = recv["success"].asBool();
				if(result)
				{
					strURL = recv["data"]["imageURL"].asString();

					// 去掉前缀
					if(strURL.find(PICTURE_URL) != string::npos)
					{
						strURL = strURL.substr(strlen(PICTURE_URL));
					}
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
					else if(strURL.find(PICTURE_URL2) != string::npos)
					{
						strURL = strURL.substr(strlen(PICTURE_URL2));
					}
					else if(strURL.find(PICTURE_URL3) != string::npos)
					{
						strURL = strURL.substr(strlen(PICTURE_URL3));
					}
#endif
				}
			}
		}
	}

	//上传容错处理
	if(strURL == "")
	{	// 记录日志
		//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, "上传图片失败：src:%s,url:%s", szPic, strURL.c_str());
		CString str;
		str.Format("上传图片失败：src:%s,url:%s", szPic, strURL.c_str());
		g_LocalData.WriteLog(CLocalData::LL_INFORMATION, str);
	}
	else
	{
		//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, "上传图片成功：src:%s,url:%s", szPic, strURL.c_str());
		CString str;
		str.Format("上传图片成功：src:%s,url:%s", szPic, strURL.c_str());
		g_LocalData.WriteLog(CLocalData::LL_INFORMATION, str);
	}
	return !strURL.empty();
}

extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////未读消息的好友ID和未读条数
IMPLEMENT_HMG_SCROLLBAR(CDuiP2PSendMsgDlg)

BEGIN_XT_MESSAGE_MAP(CDuiP2PSendMsgDlg)
	// 临时群成员改动通知
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	// 好友状态改变通知
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnFriendStatusChange)
	// 买家退出通知
	ON_XT_COMMAND(CMD_BUYER_OFFLINE_IND,OnFriendOffLine)
	// 获取临时群成员列表
	ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnGetClusterMember)
	// 临时群解散
	ON_XT_COMMAND(CMD_CDISMISS_IND,OnClusterDismissInd)
	// 获取好友状态
	ON_XT_COMMAND(CMD_STATUS_GET_ACK,OnStatusGetAck)
	// 修改状态
	ON_XT_COMMAND(CMD_STATUS_MOD_ACK,OnStatusModAck)
	// 登出
	ON_XT_COMMAND(CMD_LOGOUT,OnLogOut)
	// 获取好友资料
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnFriendInfoGet)
	// 建立临时群
	ON_XT_COMMAND(CMD_CREATE_CLUSTER_ACK,OnCreateTempClusterAck)
	// 临时群成员设置
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnTempCMembersSetAck)
	// 开始上传群图片数据应答
	ON_XT_COMMAND(CMD_START_SEND_CPIC_ACK,OnStartUploadCPicAck)
	// 上传群图片数据
	ON_XT_COMMAND(CMD_SEND_CPIC_ACK,OnUploadCPicAck)
	// 下载群图片数据
	ON_XT_COMMAND(CMD_GET_CPIC_ACK,OnDownloadCPicAck)
	// 获取好友基本资料
	ON_XT_COMMAND(CMD_GET_FRIEND_BASICINFO_ACK,OnGetFriendBasicInfoAck)
	// 获取转接之前的30条聊天记录
	ON_XT_COMMAND(CMD_WEB_MORE_MSG_ACK,OnWebMoreMsgAck)
#ifdef _CS_PLAT_EX_
	// 主管获取客服当前聊天买家
	ON_XT_COMMAND(CMD_MONITOR_SET_BUYER_IND,OnMonitorSetBuyerInd)
	// 主管获取客服接待的买家
	ON_XT_COMMAND(CMD_MONITOR_START_ACK, OnMonitorStartAck)
	// 主管获取客服的动作，聊天
	ON_XT_COMMAND(CMD_MONITOR_START_IND, OnMonitorStartInd)
	// 主管取消监控客服应答
	ON_XT_COMMAND(CMD_MONITOR_END_ACK, OnMonitorEndAck)
	// 主管收到取消监控客服消息
	ON_XT_COMMAND(CMD_MONITOR_END_IND, OnMonitorEndInd)
	//主管插入会话应答
	ON_XT_COMMAND(CMD_MONITOR_INSERT_CHAT_ACK, OnMonitorInsertChatAck)
	// 客服和买家都要获取主管插入的会话
	ON_XT_COMMAND(CMD_MONITOR_INSERT_CHAT_IND, OnMonitorInsertChatInd)
	// 主管接手会话应答
	ON_XT_COMMAND(CMD_MONITOR_TRAN_CHAT_ACK, OnMonitorTranChatAck)
	//客服和买家收到主管接收会话
	ON_XT_COMMAND(CMD_MONITOR_TRAN_CHAT_IND, OnMonitorTranChatInd)
	////根据用户ID获取客服CSID
	//ON_XT_COMMAND(CMD_GET_USERID_MAP_CSID_ACK, OnGetCSIDFromUserID)

	
#endif
#ifdef _BUSINESS_VESION
	//卖家版获取最近联系客户右侧备注
	ON_XT_COMMAND(CMD_GET_NEAR_COMM_CUST_REMARK_ACK, OnGetNearCommCustRemarkAck)
	//卖家版修改最近联系客户右侧备注
	ON_XT_COMMAND(CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK, OnUpdateNearCommCustRemarkAck)
	ON_XT_COMMAND(CMD_CUSTSER_TRAN_ACK_IND, OnRecvCustSerTranAckInd)
 #endif
	// p2p测试结果
	ON_XT_COMMAND(CMD_P2P_TEST_ACK,OnP2PTestAck)

	////ON_XT_COMMAND(CMD_SEND_FILE_REQ,AF_RecvSendFileReq)
	ON_XT_COMMAND(CMD_SEND_FILE_ACK,AF_RecvSendFileAck)
	ON_XT_COMMAND(CMD_DOWNLOAD_FILE_REQ,AF_RecvDownloadFileReq)
	ON_XT_COMMAND(CMD_DOWNLOAD_FILE_ACK,AF_RecvDownloadFileAck)
	//ON_XT_COMMAND(CMD_DOWNLOAD_REQ,AF_RecvDownloadReq)
	//ON_XT_COMMAND(CMD_DOWNLOAD_ACK,AF_RecvDownloadAck)	
	ON_XT_COMMAND(CMD_FIN_TRAN_REQ,AF_RecvFinTranReq)	
	ON_XT_COMMAND(CMD_FIN_TRAN_ACK,AF_RecvFinTranAck)	
	ON_XT_COMMAND(CMD_ABORT_TRAN_REQ,AF_RecvAbortTranReq)
	ON_XT_COMMAND(CMD_ABORT_TRAN_ACK,AF_RecvAbortTranAck)
	// 通知B登录中转服务器请求
	//ON_XT_COMMAND(CMD_P2P_NOTIFY_LOGIN_REQ,AF_RecvNotifyLoginReq)
	// A登录中转服务应答
	//ON_XT_COMMAND(CMD_TRANS_LOGIN_ACK,AF_RecvALoginTransAck)
	// B登录中转服务器应答
	//ON_XT_COMMAND(CMD_TRANS_RECVLOGIN_ACK,AF_RecvBLoginTransAck)
	// 收到p2p测试包
	//ON_XT_COMMAND(CMD_P2P_TEST_REQ,AF_RecvP2PTestReq)
	// 处理透明中转失败应答包
	//ON_XT_COMMAND(CMD_ROUTERUSERACKPACKET_ACK,AF_RecvTransDataAck)

	//B端接收服务器转发分组文件信息////////////////////////////////////
	ON_XT_COMMAND(CMD_FILE_TRANS_REQ,AF_OnRecvFileAck)			////发送消息
	ON_XT_COMMAND(CMD_FILE_TRANS_IND,AF_OnSenderRecvAck)		////向服务端发
	ON_XT_COMMAND(CMD_FILE_TRANS_IND_ACK,OnRecvServerAck)		////服务端返回
	ON_XT_COMMAND(CMD_FILE_TRANS_NOTIFY_ACK, AF_OnRecvTranFileNotifyAck)
END_XT_MESSAGE_MAP()

// BEGIN_EVENTSINK_MAP(CDuiP2PSendMsgDlg, CDialog)
// 	ON_EVENT(CDuiP2PSendMsgDlg, IDC_WEB_AD, 252, CDuiP2PSendMsgDlg::NavigateComplete2WebAd, VTS_DISPATCH VTS_PVARIANT)
// END_EVENTSINK_MAP() 

string Base64Decode(const char* Data,int DataByte,int& OutByte)
{
	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	//返回值
	string strDecode;
	int nValue;
	int i= 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data!='\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode+=(nValue & 0x00FF0000) >> 16;
			OutByte++;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode+=(nValue & 0x0000FF00) >> 8;
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode+=nValue & 0x000000FF;
					OutByte++;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}
	return strDecode;
}

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);

CDuiP2PSendMsgDlg::CDuiP2PSendMsgDlg(StruExtraInfo struExtInfo)
	: m_struExtInfo(struExtInfo)
{
	m_wndAD = NULL;
	
	m_pwndTalkToolBar = NULL;
#ifdef _BUSINESS_VESION
	m_pCustSerTranDlg = NULL;
#endif
#ifdef _CS_PLAT_EX_
	m_bIsBuyerNone = FALSE;
#endif
}

CDuiP2PSendMsgDlg::~CDuiP2PSendMsgDlg()
{
#ifdef _BUSINESS_VESION
	if(m_pCustSerTranDlg != NULL)
	{
		delete m_pCustSerTranDlg;
		m_pCustSerTranDlg = NULL;
	}
#endif

	map<CString,DOWNLOAD_OBJ*>::iterator it,itEnd;
	if(m_dicDownloadObj.size()>0)
	{
		itEnd = m_dicDownloadObj.end();
		for(it = m_dicDownloadObj.begin();it != itEnd;it++)
		{
			try
			{
				TerminateThread(it->second->handle,0);
				CloseHandle(it->second->handle);
				delete it->second;
			}
			catch(...)
			{
			}
		}
	}	

#ifdef _VER_INTERNAL_
	if(m_pInterNalFriendInfo != NULL)
	{
		delete m_pInterNalFriendInfo;
		m_pInterNalFriendInfo = NULL;
	}
#endif

	if(m_pwndTalkToolBar != NULL)
	{
		delete m_pwndTalkToolBar;
		m_pwndTalkToolBar = NULL;
	}
}

DuiLib::UILIB_RESOURCETYPE CDuiP2PSendMsgDlg::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiP2PSendMsgDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

CDuiP2PSendMsgDlg* CDuiP2PSendMsgDlg::NewDlg(FRIEND_INFO* pFriend, TP2PMode msgMode, CWnd *pParent, StruExtraInfo struExtInfo,int nUnReadNum)
{
	CDuiP2PSendMsgDlg* pInstance = NULL;
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = new CDuiP2PSendMsgDlg(struExtInfo);
		CString str;
		str.Format("与 %s(%s) 聊天中",UserInfoToStr(pFriend),StatusToStr(pFriend->status));
		//创建正在咨询框
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() == NULL  || !IsWindow(CDuiCSPlatDlg::GetInstance()->GetHWND()))
		{
			CDuiCSPlatDlg::Show();
		}
		//聊天窗口改为正在咨询窗口的子窗口，保证左右两边不分家
		pInstance->Create(CDuiCSPlatDlg::GetInstance()->GetHWND(), /*"聊天窗口"*/str, UI_WNDSTYLE_FRAME_DISMAX | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE | WS_EX_APPWINDOW);
#elif defined _WORK_EX_
		if(CDuiCounsel::GetInstance() == NULL  || !IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
		{
			CDuiCounsel::Show();
		}
		pInstance->Create(CDuiCounsel::GetInstance()->GetHWND(), /*"聊天窗口"*/str, UI_WNDSTYLE_FRAME_DISMAX | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE | WS_EX_APPWINDOW);
#else
		if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))
		{
			if(CDuiCounsel::GetInstance() == NULL  || !IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
			{
				CDuiCounsel::Show();
			}
			pInstance->Create(CDuiCounsel::GetInstance()->GetHWND(), /*"聊天窗口"*/str, UI_WNDSTYLE_FRAME_DISMAX | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE | WS_EX_APPWINDOW);
		}
		else
		{
			pInstance->Create(GetDesktopWindow(), /*"聊天窗口"*/str, UI_WNDSTYLE_FRAME_DISMAX | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE | WS_EX_APPWINDOW);
			pInstance->ShowWindow();
			pInstance->CenterWindow();
		}
#endif

#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() >= 1)
		{
			pInstance->InitPos();
		}
#elif defined _WORK_EX_
		if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() >= 1)
		{
			pInstance->InitPos();
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() >= 1)
		{
			if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))//卖家聊天窗口不需要和买家窗口对齐
				pInstance->InitPos();
		}
#endif
		else
		{
#ifdef _CS_PLAT_EX_
			CRect rcDlg;
			::GetWindowRect(pInstance->GetHWND(), &rcDlg);
			CRect rcCounsel;
			::GetWindowRect(CDuiCSPlatDlg::GetInstance()->GetHWND(), &rcCounsel);

			int cxMax = GetSystemMetrics(SM_CXSCREEN);
			int cyMax = GetSystemMetrics(SM_CYSCREEN);

			int cx = ( cxMax - rcCounsel.Width() - rcDlg.Width() ) / 2;
			int cy = ( cyMax - rcCounsel.Height()) / 2;
			
			::MoveWindow(CDuiCSPlatDlg::GetInstance()->GetHWND(), cx, cy, rcCounsel.Width(),rcDlg.Height(), TRUE);
#elif defined _WORK_EX_
			CRect rcDlg;
			::GetWindowRect(pInstance->GetHWND(), &rcDlg);
			CRect rcCounsel;
			::GetWindowRect(CDuiCounsel::GetInstance()->GetHWND(), &rcCounsel);

			int cxMax = GetSystemMetrics(SM_CXSCREEN);
			int cyMax = GetSystemMetrics(SM_CYSCREEN);

			int cx = ( cxMax - rcCounsel.Width() - rcDlg.Width() ) / 2;
			int cy = ( cyMax - rcCounsel.Height()) / 2;

			::MoveWindow(CDuiCounsel::GetInstance()->GetHWND(), cx, cy, rcCounsel.Width(), rcDlg.Height(), TRUE);
#else
			if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))
			{
				CRect rcDlg;
				::GetWindowRect(pInstance->GetHWND(), &rcDlg);
				CRect rcCounsel;
				::GetWindowRect(CDuiCounsel::GetInstance()->GetHWND(), &rcCounsel);

				int cxMax = GetSystemMetrics(SM_CXSCREEN);
				int cyMax = GetSystemMetrics(SM_CYSCREEN);

				int cx = ( cxMax - rcCounsel.Width() - rcDlg.Width() ) / 2;
				int cy = ( cyMax - rcDlg.Height()) / 2;

				::MoveWindow(CDuiCounsel::GetInstance()->GetHWND(), cx, cy, rcCounsel.Width(), /*rcCounsel*/rcDlg.Height(), TRUE);
			}
#endif
		}
		pInstance->SetFriend(pFriend, msgMode, pParent,nUnReadNum);
		g_pMessageHandleWnd->AddP2PSendMsgDlg(pInstance);

		//工作版也获取好友信息，聊天窗口可以显示正确的用户名
		if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(g_ImUserInfo.GetId())/*&& pFriend->GetGuid() == ""*/)
		{	
			Sleep(100);
			// 获取好友详情已获得guid
			XT_USERINFO_GET_REQ req;
			req.fid = pFriend->GetId();
			g_pProtocol->SendReq(req);

			CString strLog;
			strLog.Format("获取好友信息ID:%d", pFriend->GetId());
			g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);

			SetTimer(pInstance->GetHWND(),TIMER_GETFRIEND_INFO,3000,NULL);

//#ifndef _CS_PLAT_EX_
//			Sleep(200);
//			//请求右侧备注
//			if(CMainFrame::IsBuyer(pFriend->GetId()))
//			{
//				XT_GET_NEAR_COMM_CUST_REMARK_REQ req;
//				req.uiBusinessID = g_ImUserInfo.GetId();
//				strncpy(req.szCommAccount, pFriend->GetUserName().c_str(), 31);
//				g_pProtocol->SendReq(req);
//			}
//#endif

#ifdef _CS_PLAT_EX_
			g_LocalData.SaveCurrentBuyerDbFile(pFriend);
#endif
		}
		//打开聊天窗口之后刷新用户状态
		pInstance->RefreshFriendStatus();

#ifdef _CS_PLAT_EX_
		if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
		{
			CRect rcWindow;
			GetWindowRect(pInstance->GetHWND(), &rcWindow);
			pInstance->ResizeClient(rcWindow.Width()+75, rcWindow.Height());
		}
#endif
	}
	return pInstance;
}

void CDuiP2PSendMsgDlg::SetExtInfo(StruExtraInfo struExtInfo)
{
	m_struExtInfo = struExtInfo;
}

void CDuiP2PSendMsgDlg::InitPos()
{
#ifdef _CS_PLAT_EX_
	CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg;
#else
	CDuiP2PSendMsgDlg* pDlg = CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg;
#endif
	if(pDlg != NULL)
	{
		CRect rcCurrent;
		::GetWindowRect(pDlg->GetHWND(), &rcCurrent);
		if(rcCurrent.Width() > 0 && rcCurrent.Height() > 0)
			::MoveWindow(GetHWND(), rcCurrent.left, rcCurrent.top, rcCurrent.Width(), rcCurrent.Height(), TRUE);
	}
	return;
	CRect rc;
	//::GetWindowRect(g_pDuiCounsel->GetHWND(), &rc);

#ifdef _CS_PLAT_EX_
	int nDlgSize = /*CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg.size()*/g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size();
	::GetWindowRect(/*CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg[nDlgSize-1]->GetHWND()*/g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlgSize-1]->GetHWND(), &rc);
#else
	int nDlgSize = CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size();
	::GetWindowRect(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg[nDlgSize-1]->GetHWND(), &rc);
#endif

	ASSERT(::IsWindow(m_hWnd));
	ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
	HWND hWnd=*this;
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	if (hWndCenter!=NULL)
		hWnd=hWndCenter;

	// 处理多显示器模式下屏幕居中
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
	rcArea = oMonitor.rcWork;

	if( hWndCenter == NULL )
		rcCenter = rcArea;
	else
		::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if( xLeft < rcArea.left ) xLeft = rcArea.left;
	else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
	if( yTop < rcArea.top ) yTop = rcArea.top;
	else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
	//::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CDuiP2PSendMsgDlg::SetFriend(FRIEND_INFO *pFriend, TP2PMode msgMode, CWnd *pParent,int nUnReadNum)
{
	if ( pFriend )
	{
		m_FriendInfo   = * pFriend   ;
		m_bClosePanel  = pFriend->bClosePanel;	
		m_wndPopupHistory.SetFriedInfo(pFriend);
	}
	else
	{
		m_bClosePanel  = false;
	}

//#ifdef _CS_PLAT_EX_
//	m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_SELLER).c_str());
//#elif defined _WORK_EX_
//	if(!CMainFrame::IsWork(m_FriendInfo.GetId()))
//		m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_SELLER).c_str());
//	else
//		m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_WORK).c_str());
//#else
//	if(!CMainFrame::IsWork(m_FriendInfo.GetId()))
//		m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_SELLER).c_str());
//	else
//		m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_WORK).c_str());
//#endif
	
	m_nFriendId = m_FriendInfo.GetId();

	uint32 friendIp;
	uint16 friendPort;
	if(m_FriendInfo.ip == g_ImUserInfo.ip)
	{
		friendIp = m_FriendInfo.localIP;
		friendPort = m_FriendInfo.localPort;
	}
	else
	{
		friendIp = m_FriendInfo.ip;
		friendPort = m_FriendInfo.port;
	}

	AF_SetInfo(g_ImUserInfo.GetId(),m_FriendInfo.GetId(),friendIp,htons(friendPort),g_pProtocol->GetTranServerIP(),htons(g_pProtocol->GetTranServerPort()),g_pProtocol->m_pUdpComm);

	m_pmP2PMode    =  msgMode ;
	m_pmPreP2PMode =  pmNone  ;

	m_bClusterInit    = false;
	m_uiAddFriendId   = 0;
	m_bGroupDestoryed = false;
	//m_hIcon		   = g_imgHead16.ExtractIcon(MAKE_HEAD_INDEX(pFriend->face,pFriend->status));
	m_hIcon        = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_hAccel	   = LoadAccelerators(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_SENDMSG_HOTKEY));

	m_nSplitterPos        = 130;
	m_dwTickCountActive   = GetTickCount();
	m_nTempGroupTalkIndex     = -1;
	m_clusterInfo.nFamilyProp = TEMP_FAMILY;
	m_dwProgressBar = (DWORD)-1;
	m_uPopupHistoryWidth = 0;

	m_hSmallIcon = ::ResLoadIcon(IDR_MAINFRAME1);
	m_hBigIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

	m_bDlgInit	 = false;

	m_nSendFileSequence = GetTickCount();

	memset(&m_rcPre,0,sizeof(m_rcPre));

	m_dwIpTick	   = 0;
	m_bMMStartTalk = false;
	m_bMMWaitTalk  = false;
	m_bUpdateP2PPanel = false;
	m_bPreShowHistory = false;
	m_bShowHistory = false;

	// 获取快捷短语
	g_LocalData.GetFastWord(m_vecFastWord);

	m_nLastShakeTime = 0;

#ifdef _VER_INTERNAL_
	m_pInterNalFriendInfo = NULL;
#endif

	m_nFriendId = m_FriendInfo.GetId();

	CDuiString strFriendName = UserInfoToStr(&m_FriendInfo);
	CLabelUI* pLabelFriendName =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelfriendname")));
	pLabelFriendName->SetText(strFriendName);

	CLabelUI* pLabelFriendSign =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelfriendsign")));
	pLabelFriendSign->SetText(pFriend->user_sign);

	//显示头像
	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&m_FriendInfo);
	CString strGifFile =  "";
	CString strPath =  m_FriendInfo.face_url;
	if(strPath.Find(".gif")!=-1)
	{
    	strGifFile =  strFaceFile +".gif";////GIF文件
		if ( !FileExist(strGifFile) )////不存在则创建
		{
			CopyFile(strFaceFile,strGifFile,FALSE);
		}
	}
	BOOL bIsGray = FALSE;
	if(m_FriendInfo.status == XTREAM_HIDE || m_FriendInfo.status == XTREAM_OFFLINE)
	{
		bIsGray = TRUE;
	}
	
	string strImg = CutRoundImg(strFaceFile, 50, 50, bIsGray);
	
	strFaceFile = strImg.c_str();
	if ( /*m_FriendInfo.status!=XTREAM_OFFLINE &&*/ FileExist(strFaceFile) )
	{
		if ( FileExist(strGifFile) )
		{
			if(bIsGray)///离线
			{
				LPCSTR szHead = (LPSTR)(LPCSTR)strFaceFile;
				CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
				pVLhead->SetBkImage(szHead);
				m_pHead->SetBkImage("");
				m_pHead->SetNormalGifFile("");
			}
			else//在线
			{
				CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
				pVLhead->SetBkImage("");
				m_pHead->SetBkImage("");
				m_pHead->SetNormalGifFile(strGifFile);
			}
		}
		else 
		{
			LPCSTR szHead = (LPSTR)(LPCSTR)strFaceFile;
			CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
			pVLhead->SetBkImage(szHead);
			m_pHead->SetBkImage("");
			m_pHead->SetNormalGifFile("");
		}
	}
	else
	{
		//m_ctrlFaceIcon.SetUserInfo(&m_FriendInfo);
	}

		//m_wndAD->SetHomePage(m_strAdUrl);
		//m_wndAD->SetAutoNavigation(true);
	//}

	//m_wndAD.Navigate(m_strAdUrl,NULL,NULL,NULL,NULL);
 	//m_wndAD->SetHomePage(m_strAdUrl);
 	//m_wndAD->SetAutoNavigation(true);
	//m_wndAD->Navigate2(m_strAdUrl);

#ifdef _BUSINESS_VESION	// 卖家版中判断聊天成员是否是我的团队成员，如果不是，则不显示“推送评价”按钮
// 	if (CheckSubGroupIsInMyTeam(m_FriendInfo.group_index))
// 	{
// 		ShowOrHideScoreBtn(FALSE);
// 	}

//	m_btnHangup.SetPng(IDB_PNG_HANGUP_HOVER, IDB_PNG_HANGUP_NORMAL, IDB_PNG_HANGUP_PRESSED, IDB_PNG_HANGUP_DISABLE);
// 	if(m_pmP2PMode==pmSingleTalk && m_FriendInfo.group_index < MY_TEAM_GROUP_ID)
// 	{	// 只有接待非团队成员时才显示客服转接
// 		m_btnCustSerTran.ShowWindow(SW_SHOW);
// 	}
// 	m_btnCustSerTran.SetPng(IDB_PNG_CUSTSER_HOVER, IDB_PNG_CUSTSER_NORMAL, IDB_PNG_CUSTSER_PRESSED, IDB_PNG_CUSTSER_PRESSED);
// 	m_btnCustSerTran.SetToolTip("客服转接");

#endif
	//DWORD mask =::SendMessage(m_reditHistory.m_hWnd,EM_GETEVENTMASK, 0, 0);  
	//mask = mask | ENM_LINK  | ENM_MOUSEEVENTS | ENM_SCROLLEVENTS |ENM_KEYEVENTS;  
	//::SendMessage(m_reditHistory.m_hWnd,EM_SETEVENTMASK, 0, mask);  
	//::SendMessage(m_reditHistory.m_hWnd,EM_AUTOURLDETECT, true, 0);  

	//m_reditHistory.SetEmotionImageList(&g_imgEmotionList);
	//m_reditHistory.SetShowLinkInformation(TRUE);
	//m_reditSend.SetEmotionImageList(&g_imgEmotionList);
	//m_reditSend.EnableZYF(m_FriendInfo.isTalkZYF==1);

	//m_reditSend.SetXFont(g_ImFont);
	ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
	RichEdit_SetDefFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);

	switch(m_pmP2PMode)
	{
	case pmSingleTalk:
		//显示历史5条记录
		LoadLastTalkMsg(nUnReadNum);
		break;
	case pmMultiTalk:
		break;
	}
	
	//AttachHMGScrollbar(m_reditHistory.GetSafeHwnd());
	//AttachHMGScrollbar(m_reditSend.GetSafeHwnd());

	SetTimer(GetHWND(),0x01,1000,NULL);
	SetTimer(GetHWND(), FRM_TIMER_ID, MIN_FRM_DELAY, NULL);  
	//SetTimer(GetHWND(),TIMER_DWN_CPIC,TIME_DWN_CPIC,NULL);

	//AddP2PTip();
	//PopMsg();

	//for(int n = 0; n < 15; n++)
	//{
	//	m_reditHistory.InsertText("12345");
	//	m_reditHistory.InsertText("\n");
	//	m_reditHistory.SetSel(-1, -1);
	//}
	//m_reditHistory.ScrollToBottom();
	//m_reditHistory.Perform(WM_VSCROLL,SB_BOTTOM,0);
	

	//m_reditSend.SetStartIndent(80);
	RichEdit_SetStartIndent(pTextServicesSend, 80);
	//m_reditSend.SetFocus();

	//m_reditSend.DragAcceptFiles(TRUE);

	m_pLabelHangupOn->SetVisible(false);
	if(!CMainFrame::IsBuyer(pFriend->GetId()))
	{
		//如果对方是卖家则屏蔽部分功能
		//推送服务评价表
		CButtonUI* pBtnScore = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscore")));
		pBtnScore->SetVisible(false);
		//客服转接
		CButtonUI* pBtnTran = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btntran")));
		pBtnTran->SetVisible(false);
		m_pLabelHangupOff->SetVisible(false);
	}
	else
	{
		//如果对方是买家则屏蔽部分功能
		//文件传输
		CButtonUI* pBtnFile = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnfile")));
		pBtnFile->SetVisible(false);
#ifdef _CS_PLAT_EX_
		m_pLabelHangupOff->SetVisible(false);
#else
		//CHorizontalLayoutUI* pHLHeadTool = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HLHeadTool")));
		//pHLHeadTool->SetFixedWidth(130);
#endif
	}
	
	//监控按钮是否可见
#ifdef _CS_PLAT_EX_
	m_pBtnMonitorOn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnMonitorOn")));
	m_pBtnMonitorOff = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnMonitorOff")));
	m_pBtnInsertMsg = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnInsertMsg")));
	m_pBtnTranMsg = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTranMsg")));
	m_nIdMonitorBuyer = 0;
	m_bIsInsertMsg = FALSE;
	m_bIsMonitor = FALSE;
	//监控按钮暂时屏蔽
	//if(g_ImUserInfo.jifen == 1 && 
	//	!CMainFrame::IsBuyer(pFriend->GetId()) && IsMonitorOn(pFriend->group_index) && 
	//	((pFriend->group_index != g_ImUserInfo.group_index && pFriend->jifen == 1) || pFriend->jifen != 1) )
	//	m_pBtnMonitorOn->SetVisible(true);
#endif

#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
	if(!CMainFrame::IsBuyer(pFriend->GetId()) && !CMainFrame::IsWork(pFriend->GetId()))
	{	// 对方非买家，设置阴影
		m_PaintManager.GetShadow()->SetImage("menu\\shadow.png");
		CRect rc(30,30,30,30);
		m_PaintManager.GetShadow()->SetShadowCorner(rc);
	}
#endif

	UpdateFastWordInfo();//刷新快捷短语

	if(!CMainFrame::IsBuyer(pFriend->GetId()))
	{
		UpdateUrl();
	}
}

// 更新url
void CDuiP2PSendMsgDlg::UpdateUrl()
{
	CString strPara;
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
	if( m_FriendInfo.GetGuid().length() > 0 && CMainFrame::IsBuyer(m_FriendInfo.GetId()))//对面是买家
	{
		string strComment = "";
		if(strcmp(m_FriendInfo.comment_name, "") != 0)
		{
			strComment = m_FriendInfo.comment_name;
		}
		string strUserName = "";
		if(strcmp(m_FriendInfo.GetUserName().c_str(), "") != 0)
		{
			strUserName = m_FriendInfo.GetUserName().c_str();
		}
		else
		{
			strUserName = m_FriendInfo.nickname;
		}
		g_LocalData.WriteLog(CLocalData::LL_INFORMATION, m_FriendInfo.GetGuid().c_str());
		strPara.Format("?merchantID=%d&customerToken=%s&isOppositeCustomer=true&userName=%s&userComment=%s&productID=%s&orderSeq=%s&serviceId=%d",g_ImUserInfo.merchant_id,
			m_FriendInfo.GetGuid().c_str(),strUserName.c_str(),strComment.c_str(), m_struExtInfo.strProductID.c_str(), m_struExtInfo.strOrderSeq.c_str(), g_ImUserInfo.GetId());
	}
	else if(CMainFrame::IsWork(m_FriendInfo.GetId()))
	{
		strPara.Format("?staffId=%d",m_FriendInfo.GetId());
	}
	else//对面是卖家
	{
		strPara.Format("?merchantID=%d&serviceId=%d&isOppositeCustomer=false",g_ImUserInfo.merchant_id, g_ImUserInfo.GetId());
	}
	if(!CMainFrame::IsWork(m_FriendInfo.GetId()))
	{
		DWORD dT = GetTickCount();
		string strTemp = (LPCTSTR)strPara;
		strPara.Format("%s&%d", strTemp.c_str(), dT);
		m_strAdUrl = RIGHT_URL_SELLER + strPara;
	}
	else
		m_strAdUrl = RIGHT_URL_WORK + strPara;

	if( !CMainFrame::IsBuyer(m_FriendInfo.GetId()) /*|| (CMainFrame::IsBuyer(m_FriendInfo.GetId()) && m_FriendInfo.GetGuid() != "")*/ )
	{
		//m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
		SetTimer(GetHWND(), TIMER_UPDATEURL,500,NULL);
	}
#endif

#if defined (BUSINESS_VESION) && !defined(_CS_PLAT_EX_) //买家版
	if(theApp.m_nProductID != 0 )
	{
		strPara.Format("?mid=%d&flag=1&pid=%d",theApp.m_nMerchantID,theApp.m_nProductID);
	}
	else if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()))
	{
		strPara.Format("?smid=%d",m_FriendInfo.GetId());
	}

	m_strAdUrl = RIGHT_URL + strPara;

	if( (!CMainFrame::IsBuyer(m_FriendInfo.GetId())) || (CMainFrame::IsBuyer(m_FriendInfo.GetId()) && m_FriendInfo.GetGuid() != "") )
	{
		//m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
		SetTimer(GetHWND(), TIMER_UPDATEURL,500,NULL);
	}
#endif

#ifdef _CS_PLAT_EX_  //飞牛客服工作台
	if( m_FriendInfo.GetGuid().length() > 0 && CMainFrame::IsBuyer(m_FriendInfo.GetId()))//对面是买家
	{
		extern map<DWORD,CString>  g_mapServiceURL;////GUID为空时，保存对应的买家ID
		CString strGUID = m_FriendInfo.GetGuid().c_str();
		if(m_FriendInfo.GetId()!=0 && strGUID =="")
		{
			g_mapServiceURL[m_FriendInfo.GetId()] = strGUID;//GUID为空时，保存起来
		}	
		else if(m_FriendInfo.GetId()!=0 && strGUID !="")//再次刷新如果guid不为空则去掉guid为空的数据
		{
			map<DWORD,CString>::iterator itr = g_mapServiceURL.begin();
			for (; itr != g_mapServiceURL.end(); itr++)
			{
				if(itr->first == m_FriendInfo.GetId())
				{
					g_mapServiceURL.erase(itr);
					break;
				}
			}
		}
		extern map<int, CString> g_mapFriendIdToOrder;
		CString strParams = "";
		///////////////////////////////////////local.fn.com:8080
		////直接访问的方式
		int nAskingType = 0;
		if(g_mapFriendIdToOrder[m_FriendInfo.GetId()] == "")
		{
			nAskingType = 2;
			strParams.Format("customerToken=%s&goodsAskingType=%d", \
				m_FriendInfo.GetGuid().c_str(),nAskingType);
		}
		else
		{
			nAskingType = 1;
			strParams.Format("customerToken=%s&goodsAskingType=%d&orderToken=%s", \
				m_FriendInfo.GetGuid().c_str(),nAskingType,g_mapFriendIdToOrder[m_FriendInfo.GetId()]);
		}
		m_strAdUrl = RIGHT_URL_SELLER + strParams;
	}
	else//对面是客服人员（我的团队)
	{
		CString strParams = "";
		strParams.Format("customerServiceId=%d&serviceId=%d",g_ImUserInfo.GetId(),m_FriendInfo.GetId());
		m_strAdUrl = EMPLOYEE_RIGHT_URL + strParams;
	}

	if( (!CMainFrame::IsBuyer(m_FriendInfo.GetId())) || (CMainFrame::IsBuyer(m_FriendInfo.GetId()) && m_FriendInfo.GetGuid() != "") )
	{
		//m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
		SetTimer(GetHWND(), TIMER_UPDATEURL,500,NULL);
	}
#endif

#ifdef _WORK_EX_
	if(CMainFrame::IsSeller(m_FriendInfo.GetId()))//对面是商家
	{
		int nMerchantId = g_LocalData.GetMerchantIdFormSubMerchantId(m_FriendInfo.GetId());
		strPara.Format("?merchantId=%d",nMerchantId);
		m_strAdUrl = RIGHT_URL_SELLER + strPara;
	}
	else if(CMainFrame::IsWork(m_FriendInfo.GetId()))
	{
		strPara.Format("?staffId=%d",m_FriendInfo.GetId());
		m_strAdUrl = RIGHT_URL_WORK + strPara;
	}
	//m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
	SetTimer(GetHWND(), TIMER_UPDATEURL,500,NULL);
#endif
}

LRESULT CDuiP2PSendMsgDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//// 禁止聊天窗口位于顶端最大化，或者左右边缘时停靠
	//if(WM_EXITSIZEMOVE == uMsg)////拖动停止时设置为正常大小显示
	//{
	//    ShowWindow(SW_NORMAL);
	//}
	if(WM_MOUSEMOVE == uMsg)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		CButtonUI* pBtnScreenTool = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
		CRect rcTool = pBtnScreenTool->GetPos();
		CButtonUI* pBtnScreenToolConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
		CRect rcConfig = pBtnScreenToolConfig->GetPos();
		if(rcTool.PtInRect(pt))
		{
			CButtonUI* pBtnScreenToolConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
			pBtnScreenToolConfig->SetNormalImage("P2PSendMsgDlgTool\\screenshots_choose_hover.png");
		}
		else if(rcConfig.PtInRect(pt))
		{
			CButtonUI* pBtnScreenTool = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
			pBtnScreenTool->SetNormalImage("P2PSendMsgDlgTool\\screenshots_hover.png");
		}
		else
		{
			CButtonUI* pBtnScreenToolConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
			pBtnScreenToolConfig->SetNormalImage("P2PSendMsgDlgTool\\screenshots_choose_normal.png");

			CButtonUI* pBtnScreenTool = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
			pBtnScreenTool->SetNormalImage("P2PSendMsgDlgTool\\screenshots_normal.png");
		}
		//快捷短语组弹出的菜单，编辑、上移、下移、删除
		if(m_pMenuFastWord == NULL || !IsWindowVisible(m_pMenuFastWord->GetHWND()))
		{
			BOOL bIsExist = FALSE;
			CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treefastword")));
			for(int nFastWordGroup = 0; nFastWordGroup < pTreeView->GetCount(); nFastWordGroup++)
			{
				CTreeNodeUI* pFastWordGroup = (CTreeNodeUI*)pTreeView->GetItemAt(nFastWordGroup);
				DuiLib::CButtonUI* pBtnConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pFastWordGroup, _T("fastwordconfig")));
				if(pFastWordGroup->IsItemHot())
				{
					if(pBtnConfig != NULL && pFastWordGroup->GetFolderButton()->GetText() != "未分组")
						pBtnConfig->SetVisible(true);
					bIsExist = TRUE;
					m_pSelectGroupNode = pFastWordGroup;
				}
				else
				{
					if(pBtnConfig != NULL)
						pBtnConfig->SetVisible(false);
				}
			}
			if(!bIsExist)
			{
				for(int nFastWordGroup = 0; nFastWordGroup < pTreeView->GetCount(); nFastWordGroup++)
				{
					CTreeNodeUI* pFastWordGroup = (CTreeNodeUI*)pTreeView->GetItemAt(nFastWordGroup);
					DuiLib::CButtonUI* pBtnConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pFastWordGroup, _T("fastwordconfig")));
					if(pBtnConfig != NULL)
						pBtnConfig->SetVisible(false);
				}
				m_pSelectGroupNode = NULL;
			}
		}
		//快捷短语弹出的菜单，编辑、上移、下移、删除
		if(m_pMenuFastWordNode == NULL || !IsWindowVisible(m_pMenuFastWordNode->GetHWND()))
		{
			BOOL bIsExist = FALSE;
			CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treefastword")));
			for(int nFastWordGroup = 0; nFastWordGroup < pTreeView->GetCount(); nFastWordGroup++)
			{
				CTreeNodeUI* pFastWordGroup = (CTreeNodeUI*)pTreeView->GetItemAt(nFastWordGroup);
				DuiLib::CButtonUI* pBtnNodeConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pFastWordGroup, _T("fastwordnodeconfig")));
				if(pFastWordGroup->IsItemHot())
				{
					if(pBtnNodeConfig != NULL && pFastWordGroup->GetFolderButton()->GetText() != "未分组")
						pBtnNodeConfig->SetVisible(true);
					bIsExist = TRUE;
					m_pSelectFastWordNode = pFastWordGroup;
				}
				else
				{
					if(pBtnNodeConfig != NULL)
						pBtnNodeConfig->SetVisible(false);
				}
			}
			if(!bIsExist)
			{
				for(int nFastWordGroup = 0; nFastWordGroup < pTreeView->GetCount(); nFastWordGroup++)
				{
					CTreeNodeUI* pFastWordGroup = (CTreeNodeUI*)pTreeView->GetItemAt(nFastWordGroup);
					DuiLib::CButtonUI* pBtnNodeConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pFastWordGroup, _T("fastwordnodeconfig")));

					if(pBtnNodeConfig != NULL)
						pBtnNodeConfig->SetVisible(false);
				}
				m_pSelectFastWordNode = NULL;
			}
		}
		ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
		m_strLinkInfo = Richedit_GetLinkInfo(m_reditHistory, pTextServicesHistory, pt.x, pt.y);

		if(m_strLinkInfo != "" && m_strLinkInfoPre != m_strLinkInfo)
		{
			HCURSOR hCursor = NULL;
			hCursor = LoadCursor(NULL,IDC_HAND);
			if(hCursor)
			{
				SetCursor(hCursor);
			}
		}
		m_strLinkInfoPre = m_strLinkInfo;
	}
	if(WM_SETCURSOR)
	{
		if(m_strLinkInfo != "")
			return TRUE;
	}
	/*if( WM_MOUSELEAVE == uMsg)
	{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CButtonUI* pBtnScreenTool = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
	CRect rc = pBtnScreenTool->GetPos();
	if(!rc.PtInRect(pt))
	{
	CButtonUI* pBtnScreenToolConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
	pBtnScreenToolConfig->SetNormalImage("P2PSendMsgDlgTool\\screenshots_choose_normal.png");
	}
	CButtonUI* pBtnScreenToolConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
	rc = pBtnScreenToolConfig->GetPos();
	if(!rc.PtInRect(pt))
	{
	CButtonUI* pBtnScreenTool = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
	pBtnScreenTool->SetNormalImage("P2PSendMsgDlgTool\\screenshots_normal.png");
	}
	}*/

	// 处理后台交互消息
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	if(WM_EMOTIONSEL_NOTIFY == uMsg)
	{
		OnEmotionSelNotify(wParam, lParam);
	}

	if(WM_SIZE == uMsg)
	{
		UpdateBk();
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL /*&& CMainFrame::IsBuyer(m_FriendInfo.GetId())*/)
		{
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg == this)
			{
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				
				//CRect rcCSPlat;
				//GetWindowRect(CDuiCSPlatDlg::GetInstance()->GetHWND(), &rcCSPlat);

				//::SetWindowPos(CDuiCSPlatDlg::GetInstance()->GetHWND(), GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
				::MoveWindow(CDuiCSPlatDlg::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
				CDuiCSPlatDlg::GetInstance()->MoveAllWindow(this);
			}
		}
#elif defined _WORK_EX_
		if(CDuiCounsel::GetInstance() != NULL )
		{
			if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg == this)
			{
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				//::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
				::MoveWindow(CDuiCounsel::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
				CDuiCounsel::GetInstance()->MoveAllWindow(this);
			}
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(m_FriendInfo.GetId()) || CMainFrame::IsWork(m_FriendInfo.GetId())) )
		{
			if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg == this)
			{
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				//::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
				::MoveWindow(CDuiCounsel::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
				CDuiCounsel::GetInstance()->MoveAllWindow(this);
			}
		}
#endif
		//窗口大小变化时关闭快捷短语菜单
		if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL)
		{
			//CFastWordSelectMenu::GetInstance()->ShowWindow(false);
			SendMessage(WM_FASTWORD_MENU_HIDE);
			//m_reditSend.SetShowFastWordMenu(FALSE);//快捷短语菜单
		}
	}

//	if(WM_ACTIVATE == uMsg)
//	{
//		return TRUE;
//		if(wParam != WA_INACTIVE)
//		{
//#ifdef _CS_PLAT_EX_
//			if(CDuiCSPlatDlg::GetInstance() != NULL && IsWindowVisible(GetHWND()))
//			{			
//				if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg == this || g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)
//				{
//					CRect rc;
//					GetWindowRect(GetHWND(), &rc);
//
//					CRect rcCSPlat;
//					GetWindowRect(CDuiCSPlatDlg::GetInstance()->GetHWND(), &rcCSPlat);
//
//					::SetWindowPos(CDuiCSPlatDlg::GetInstance()->GetHWND(), GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
//				}
//			}
//#elif defined _WORK_EX_
//			if(CDuiCounsel::GetInstance() != NULL && IsWindowVisible(GetHWND()))
//			{			
//				if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg == this || CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1)
//				{
//					CRect rc;
//					GetWindowRect(GetHWND(), &rc);
//					::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
//				}
//			}
//#else
//			if(CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(m_FriendInfo.GetId()) || CMainFrame::IsWork(m_FriendInfo.GetId())) && IsWindowVisible(GetHWND()))
//			{			
//				if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg == this || CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1)
//				{
//					CRect rc;
//					GetWindowRect(GetHWND(), &rc);
//					::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
//				}
//			}
//#endif
//		}
//		else
//		{
//			//窗口焦点失去时关闭快捷短语菜单
//			if(m_bIsFirstFastWordMenu)
//			{
//				m_bIsFirstFastWordMenu = FALSE;
//			}
//			else
//			{
//				if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL && ::IsWindowVisible(CFastWordSelectMenu::GetInstance()->GetHWND()))
//				{
//					//CFastWordSelectMenu::GetInstance()->ShowWindow(false);
//					SendMessage(WM_FASTWORD_MENU_HIDE);
//					//m_reditSend.SetShowFastWordMenu(FALSE);//快捷短语菜单
//				}
//			}
//		}
//	}

	if(WM_MOVE == uMsg)
	{
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->GetHWND() != NULL && IsWindowVisible(GetHWND()))
		{
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg == this)
			{
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				::MoveWindow(CDuiCSPlatDlg::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
				CDuiCSPlatDlg::GetInstance()->MoveAllWindow(this);
			}
		}
#elif defined _WORK_EX_
		if(CDuiCounsel::GetInstance() != NULL && IsWindowVisible(GetHWND()))
		{
			if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg == this /*|| CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() == 1*/)
			{
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				::MoveWindow(CDuiCounsel::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
				CDuiCounsel::GetInstance()->MoveAllWindow(this);
			}
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(m_FriendInfo.GetId()) || CMainFrame::IsWork(m_FriendInfo.GetId())) && IsWindowVisible(GetHWND()))
		{
			if(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg == this /*|| CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() == 1*/)
			{
				CRect rc;
				GetWindowRect(GetHWND(), &rc);
				::MoveWindow(CDuiCounsel::GetInstance()->GetHWND(), rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
				CDuiCounsel::GetInstance()->MoveAllWindow(this);

				//CRect rc;
				//GetWindowRect(GetHWND(), &rc);
				//CRect rcCounsel;
				//::GetWindowRect(CDuiCounsel::GetInstance()->GetHWND(), &rcCounsel);
				//CDuiCounsel::GetInstance()->ShowWindow(true);
				//::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), *this, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
			}
		}
#endif
		//窗口移动时关闭快捷短语菜单
		if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL)
		{
			//CFastWordSelectMenu::GetInstance()->ShowWindow(false);
			SendMessage(WM_FASTWORD_MENU_HIDE);
			//m_reditSend.SetShowFastWordMenu(FALSE);//快捷短语菜单
		}
	}

	if(WM_RICHEDIT_NOTIFY == uMsg)
	{
		OnRichEditLinkClilck(wParam, lParam);
	}

	if(WM_FILERECV_OK == uMsg)
	{
		OnRecvFileOK(wParam, lParam);
	}
	if(WM_FILERECV_FAILED == uMsg)
	{
		OnRecvFileFailed(wParam, lParam);
	}
	if(WM_FILESEND_OK == uMsg)
	{
		OnSendFileOK(wParam, lParam);
	}
	if(WM_FILESEND_FAILED == uMsg)
	{
		OnSendFileFailed(wParam, lParam);
	}
	if(WM_CLOSETABCTRL_CLOSE == uMsg)
	{
		OnCloseTabCtrlClose(wParam, lParam);
	}
	if(WM_NOTIFY == uMsg)
	{
		LPNMHDR pnmh = (LPNMHDR) lParam; 
		if(pnmh->code == EN_LINK)
		{
			ENLINK* pLink = (ENLINK*)pnmh;
			if(pLink->msg == WM_LBUTTONDOWN)
			{
				m_reditHistory->SetSel(pLink->chrg);
				CString strUrl = m_reditHistory->GetSelText();

				if ( strUrl.Find("://")>-1 || strUrl.Find("www")>-1 || strUrl.Find(".")>-1 )
				{
					if ( strUrl.Find("feiniu.com")>-1 )
					{
						MiniOpenUrl(strUrl);
					}
					else
					{
						OpenHtmlPage(strUrl);
					}
				}

				OLE_DATA_TYPE odtType;
				IMiniFileInfoBar *pFileBar = NULL; 
				int nFileBarPos;
				ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
				pFileBar = RichEdit_FindNearestFileInfoBar(pTextServicesHistory, pLink->chrg.cpMin,odtType,nFileBarPos);

				if ( odtType==ODT_RECV_FILE )
				{
					if(strUrl == "接收")
					{
						PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_ACCEPT,pFileBar->OwnerData);
					}
					else if(strUrl == "拒绝")
					{
						PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_REFUSE,pFileBar->OwnerData);
					}
					else if(strUrl == "另存为")
					{
						PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_SAVEAS,pFileBar->OwnerData);
					}
				}
				else if ( odtType==ODT_SEND_FILE )
				{
					if ( strUrl=="取消" )
					{
						PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_CANCEL,pFileBar->OwnerData);
					}
				}
				if( strUrl == "打开文件" ||  strUrl == "打开文件夹")
				{
					m_reditHistory->SetSel(0, pLink->chrg.cpMin);
					CString str = m_reditHistory->GetSelText();
					CString strBegin = "文件 \"";
					int nBegin = strBegin.GetLength();
					while ( str.Find("文件 \"") != -1)
					{
						str = str.Right(str.GetLength() - nBegin + 1);
					}

					nBegin = str.Find("\"");
					str = str.Right(str.GetLength() - nBegin - 1);
					int nEnd = str.Find("\" 接收完成。");
					str = str.Left(nEnd);

					CString str1,str2,strExt;
					ExtractFilePath(str,str1,str2,strExt);
					if(!FileExist(str))
					{
						CMessageBox::Model(NULL, "文件不存在！", NULL, MB_OK);
					}
					else
					{
						if(strUrl == "打开文件")
							ShellExecute(NULL,"open",str, NULL,NULL,SW_SHOWNORMAL);
						else if(strUrl == "打开文件夹")
							ShellExecute(NULL,"open",str1, NULL,NULL,SW_SHOWNORMAL);
					}
				}
			}
		}

		if(pnmh->code == NM_COOLSB_CUSTOMDRAW && (pnmh->idFrom == 10001 || pnmh->idFrom == 10002))
		{
			LRESULT rlt = 0;
			OnHMGSBCustomDraw(pnmh->idFrom, pnmh, &rlt);
		}
			
		//::SendMessage(m_reditHistory.m_hWnd, WM_NOTIFY, wParam, lParam);
		if ( wParam==IDC_SPLITTER1 )
		{
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;

			m_nSplitterPos-=pHdr->delta;

			AdjustUI();

			//m_reditHistory.ScrollToBottom();
			m_reditHistory->EndDown();
		}

		if ( wParam==IDC_FILETRANS_PROGRESS )
		{
			int nItem = m_wndP2PFileTransProgressBar.GetCurSel();

			DWORD dwUser;
			BOOL  bSendFile;

			if ( m_wndP2PFileTransProgressBar.GetItemInfo(nItem,dwUser,bSendFile) )
			{
				uint32 id;
				id = dwUser;
				//id.uid = m_FriendInfo.id;

				if ( bSendFile )
				{
					AF_AbortSendFile(id);
					//m_reditHistory.SetFileBarTip(id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));
					ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
					RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));
				}
				else
				{

					CP2PRecvFile *pFile = AF_FindRecvFile(id);

					// 现在暂时不实现断点续传功能
					/*if ( pFile )
					pFile->SaveFilePosition(m_FriendInfo.GetId());*/

					AF_AbortRecvFile(id);
					//m_reditHistory.SetFileBarTip(id,ODT_RECV_FILE,"您取消了接收该文件",RGB(255,0,0));
					ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
					RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, id,ODT_RECV_FILE,"您取消了接收该文件",RGB(255,0,0));
				}

				m_wndP2PFileTransProgressBar.DeleteItem(nItem);
				if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
					ShowFileTransInfoBar(false);

				return FALSE;
			}
		}
	}
	if(WM_KEYDOWN_RETURN == uMsg)
	{
		return 2;
	}
	if(WM_KEYDOWN == uMsg)
	{
		switch(  wParam  )  
		{  
		case 0x0D:
			{
				if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL && ::IsWindowVisible(CFastWordSelectMenu::GetInstance()->GetHWND()))
				{
					CFastWordSelectMenu::GetInstance()->HandleMessage(WM_KEYDOWN, wParam, lParam);
					return TRUE;
				}

				HOTKEY_CONFIG *cfg=g_LocalData.GetHotKeyConfig();

				if ( cfg->sendMsgKey==SMK_ENTER  )
				{
					if ( !KEYDOWN(VK_CONTROL) )
					{
#ifdef _CS_PLAT_EX_
						if(!m_bIsMonitor || (m_bIsMonitor && m_bIsInsertMsg))
#endif
						OnBnClickedBtnSend();
					}
					else
					{
						//if (m_reditSend.IsFocused() )
						//	m_reditSend.InsertText("\n");
						if (m_reditSend->IsFocused() )
							m_reditSend->InsertText(-1, "\n");
					}
				}
				else if(cfg->sendMsgKey==SMK_CTRLENTER )
				{
					if  ( KEYDOWN(VK_CONTROL) )
					{
#ifdef _CS_PLAT_EX_
						if(!m_bIsMonitor || (m_bIsMonitor && m_bIsInsertMsg))
#endif
						OnBnClickedBtnSend();
					}
					else
					{
						//if (m_reditSend.IsFocused() )
						//	m_reditSend.InsertText("\n");
						if (m_reditSend->IsFocused() )
							m_reditSend->InsertText(-1, "\n");
					}
				}
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
			break;
		case VK_OEM_2:
			{

				CString strText = m_reditSend->GetText();
				int nFastWord = strText.ReverseFind('/');
				if(nFastWord == -1)
					strText = "";
				else
				{
					strText = strText.Right(strText.GetLength() - nFastWord - 1);
				}
				DuiLib::CPoint point = m_reditSend->PosFromChar(VK_OEM_2);
				point.y = point.y - m_reditSend->GetPos().top;
				char* pChar = (LPSTR)(LPCSTR)strText;
				SendMessage(WM_FASTWORD_MENU, MAKELONG(point.x, point.y), (DWORD)pChar);
			}
			break;
		case VK_BACK:
			{		
				//if(strText == "")
				//{
					if(SendMessage(WM_FASTWORD_MENU_HIDE, wParam, lParam) == 2)
						return TRUE;
				//}
			}
			break;
			//上下左右键都发送至父窗口，用于其他操作，暂时没有任何操作
		case VK_UP:
		case VK_DOWN:
			{
				SendMessage(WM_KEYDOWN_UPDOWN, wParam, lParam);
			}
			break;
		case VK_LEFT:
		case VK_RIGHT:
			{
				SendMessage(WM_FASTWORD_MENU_HIDE, wParam, lParam);
			}
			break;
		case 0x58://CTRL+X
			{
				DuiLib::CRichEditUI* pEdit = NULL;
				if(m_PaintManager.GetFocus() == m_reditSend)
					pEdit = m_reditSend;
				else if(m_PaintManager.GetFocus() == m_reditHistory)
					pEdit = m_reditHistory;
				else
					return FALSE;

				if(pEdit != NULL)
				{
					CString text= Richedit_GetSelText(pEdit);
					if(OpenClipboard(GetHWND()))
					{
						HGLOBAL clipbuffer;
						char * buffer;
						char * pstr = (char *)(LPCSTR)text;

						EmptyClipboard();
						clipbuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(pstr)+1)*sizeof(WCHAR));
						buffer = (char *)GlobalLock(clipbuffer);

						strcpy(buffer,pstr);

						GlobalUnlock(clipbuffer);
						SetClipboardData(CF_TEXT,clipbuffer);
						CloseClipboard();
					}
				}
				if(pEdit == m_reditSend)
					m_reditSend->ReplaceSel("", false);
				return TRUE;
			}
			break;
		case 0x43://CTRL+C
			if  ( KEYDOWN(VK_CONTROL) )
			{
				DuiLib::CRichEditUI* pEdit = NULL;
				if(m_PaintManager.GetFocus() == m_reditSend)
					pEdit = m_reditSend;
				else if(m_PaintManager.GetFocus() == m_reditHistory)
					pEdit = m_reditHistory;
				else
					return FALSE;

				if(pEdit != NULL)
				{
					CString text= Richedit_GetSelText(pEdit);
					if(OpenClipboard(GetHWND()))
					{
						HGLOBAL clipbuffer;
						char * buffer;
						char * pstr = (char *)(LPCSTR)text;

						EmptyClipboard();
						clipbuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(pstr)+1)*sizeof(WCHAR));
						buffer = (char *)GlobalLock(clipbuffer);

						strcpy(buffer,pstr);

						GlobalUnlock(clipbuffer);
						SetClipboardData(CF_TEXT,clipbuffer);
						CloseClipboard();
					}
				}
				return TRUE;
			}
			break;
		case 0x56://CTRL+V
			if  ( KEYDOWN(VK_CONTROL) )
			{
				//DuiLib::CRichEditUI* pEdit = NULL;
				//if(m_PaintManager.GetFocus() == m_reditSend)
				//	pEdit = m_reditSend;
				//else if(m_PaintManager.GetFocus() == m_reditHistory)
				//	pEdit = m_reditHistory;

				if(m_PaintManager.GetFocus() != m_reditSend)
					return FALSE;
				if(OpenClipboard(GetHWND()))
				{
					CBitmap* pBitmap = (CBitmap*)GetClipboardData(CF_BITMAP);
					if(pBitmap != NULL)
					{
						CImage image;
						image.Attach((HBITMAP)pBitmap);
						CString str;
						str.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));
						image.Save(str);
						CString strCacheFileName;
						g_LocalData.SaveImageFileToCache(str,strCacheFileName);
						strCacheFileName = g_LocalData.GetImageFileCachePath() + strCacheFileName;
						image.Save(strCacheFileName);
						image.Detach();

						RichEdit_InsertImage(m_reditSend, (LPCTSTR)strCacheFileName, -1, -1);
						//删除临时图片
						DeleteFile(str);
					}
					char * buffer;
					buffer = (char *)GetClipboardData(CF_TEXT);
					if(buffer != NULL && strcmp(buffer, "") != 0)
					{
						ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
						RichEdit_InsertText(pTextServicesSend, buffer, m_reditSend, FALSE, NULL, GetHWND(), TRUE);
					}
					CloseClipboard();
				}
				return TRUE;
			}
			break;
		default:
			;
		}  
	}
	//if(WM_SIZE == uMsg)
	//{
	//	UpdateBk();
	//}
	if(WM_CLOSE_ONE == uMsg)
	{
		SendMsgDlgNotify();
	}
	if(WM_CLOSE_ALL == uMsg)
	{
		g_pMessageHandleWnd->CloseAllP2PSendMsgDlg();
	}
	if(WM_SNAPSHOT == uMsg)
	{
		OnScreenCopy();
	}
	if(WM_SYSCOMMAND == uMsg)
	{
		if(wParam == SC_CLOSE)
		{
			CloseAllDlg();
		    return TRUE;
		}
	}
	//快捷短语组重命名
	if(WM_FASTWORD_GROUP_RENAME == uMsg)
	{
		char* pChangedGroupName = (char*)wParam;
		CString strChangedGroupName = pChangedGroupName;
		char* pGroupName = (char*)lParam;
		CString strGroupName = pGroupName;

		g_LocalData.FastWordGroupRename(strGroupName, strChangedGroupName);
		UpdateFastWordInfo();
		g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
		g_LocalData.SaveFastWordInfo();
	}
	//弹出快捷短语菜单
	if(WM_FASTWORD_MENU == uMsg)
	{
		CRect rcWindow;
		::GetWindowRect(GetHWND(), &rcWindow);

		CHorizontalLayoutUI* pHLricheditsendmsg = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hlricheditsendmsg")));
		CRect rc = pHLricheditsendmsg->GetPos();

		int x = LOWORD(wParam) + 15;
		int y = HIWORD(wParam) + 10;

		DuiLib::CPoint point(x, y);
		ClientToScreen(m_hWnd, &point);

		rc.left = rc.left + x;
		rc.top = rc.top + y;
		rc.left = rc.left + rcWindow.left;
		rc.top = rc.top	+ rcWindow.top;
		rc.right = rc.left + 135;
		rc.bottom = rc.top + 26;

		m_bIsFirstFastWordMenu = TRUE;
		CFastWordSelectMenu::NewDlg(GetHWND());
		
		CRect rcMenu;
		::GetWindowRect(CFastWordSelectMenu::GetInstance()->GetHWND(), rcMenu);
		::MoveWindow(CFastWordSelectMenu::GetInstance()->GetHWND(), rc.left, rc.top, rcMenu.Width(), rcMenu.Height(), TRUE);
		char* pChar = (char*)lParam;
		CFastWordSelectMenu::GetInstance()->SetFastWordSelectMenu(g_LocalData.GetFastWordInfo(), pChar);
		CFastWordSelectMenu::GetInstance()->ShowWindow(true);
		SetTimer(GetHWND(), TIMER_FASTWORDSELECTMENU,20,NULL);
	}
	//隐藏快捷短语菜单
	if(WM_FASTWORD_MENU_HIDE == uMsg)
	{
		if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL)
		{
			//CFastWordSelectMenu::GetInstance()->ShowWindow(false);
			CFastWordSelectMenu::GetInstance()->Close();
			//m_reditSend.SetShowFastWordMenu(FALSE);快捷短语菜单
			m_bIsFirstFastWordMenu = FALSE;
			SetTimer(GetHWND(), TIMER_FASTWORDSELECTMENU,20,NULL);
			return 2;
		}
	}
	//鼠标滚轮消息
	if(WM_MOUSEWHEEL == uMsg)
	{	
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL && ::IsWindow(CFastWordSelectMenu::GetInstance()->GetHWND()))
		{
			::SendMessage(CFastWordSelectMenu::GetInstance()->GetHWND(), uMsg, wParam, lParam);
			//CFastWordSelectMenu::GetInstance()->HandleMessage(uMsg, wParam, lParam);
			return 2;
		}
	}
	//上下键,快捷短语菜单暂时未处理上下键
	else if(WM_KEYDOWN_UPDOWN == uMsg && (wParam == VK_UP || wParam == VK_DOWN))
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL && ::IsWindowVisible(CFastWordSelectMenu::GetInstance()->GetHWND()))
		{
			m_bIsFirstFastWordMenu = TRUE;
			CFastWordSelectMenu::GetInstance()->HandleMessage(WM_KEYDOWN, wParam, lParam);
			SetTimer(GetHWND(), TIMER_FASTWORDSELECTMENU,20,NULL);
			return 2;
		}
	}
	//快捷短语菜单选择一项
	if(WM_FASTWORD_MENU_SELECT == uMsg)
	{
		CString str = (char*)wParam;
		//m_reditSend.SetText(str);
		m_reditSend->SetText(str);
		g_ImFont = g_LocalData.GetFastWordFont(str);
		//m_reditSend.SetXFont(g_ImFont);
		ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
		RichEdit_SetFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
	}
	//快捷编码完全吻合并且只有一个的时候直接发送的输入框
	if(WM_FASTWORD_MENU_SELECT_FIRST == uMsg)
	{
		//CFastWordSelectMenu::GetInstance()->ShowWindow(false);
		SendMessage(WM_FASTWORD_MENU_HIDE);
		//m_reditSend.SetShowFastWordMenu(FALSE);快捷短语菜单
		CString str = (char*)wParam;
		//m_reditSend.SetText(str);
		m_reditSend->SetText(str);
		g_ImFont = g_LocalData.GetFastWordFont(str);
		//m_reditSend.SetXFont(g_ImFont);
		ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
		RichEdit_SetFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
	}
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		// 是否最大化
// 		BOOL IsMax = IsZoomed(GetHWND());
// 		SendMessage(WM_SYSCOMMAND, IsMax ? SC_RESTORE : SC_MAXIMIZE, 0); 
// 		return 0;
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		if(CMainFrame::IsBuyer(m_FriendInfo.GetId()) || CMainFrame::IsWork(m_FriendInfo.GetId()))
		{
			CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

			if(pbtnMax->IsVisible())
			{
				pbtnMax->SetVisible(FALSE);
				pbtnRestore->SetVisible(TRUE);
				ShowDlgMax(TRUE);
			}
			else if(pbtnRestore->IsVisible())
			{
				pbtnMax->SetVisible(TRUE);
				pbtnRestore->SetVisible(FALSE);
				ShowDlgMax(FALSE);
			}
			return 0;
		}
		else
		{
			// 是否最大化
			BOOL IsMax = IsZoomed(GetHWND());
			SendMessage(WM_SYSCOMMAND, IsMax ? SC_RESTORE : SC_MAXIMIZE, 0); 
			return 0;
		}
#else
		CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
		CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

		if(pbtnMax->IsVisible())
		{
			pbtnMax->SetVisible(FALSE);
			pbtnRestore->SetVisible(TRUE);
			ShowDlgMax(TRUE);
		}
		else if(pbtnRestore->IsVisible())
		{
			pbtnMax->SetVisible(TRUE);
			pbtnRestore->SetVisible(FALSE);
			ShowDlgMax(FALSE);
		}
		return 0;
#endif

	}
	if(WM_IS_NAVEGATE == uMsg)
	{
		char* pUrl = (char*)wParam;
		return CallbackNagive((DWORD)this, pUrl);
	}
	else if(WM_RBUTTON == uMsg)
	{
		RbuttonDownWeb();
	}
#ifdef _CS_PLAT_EX_
	else if(WM_OPEN_TRAN_CHAT_MSGDLG == uMsg)	
	{
		OnOpenTranChatMsgDlg(wParam, lParam);
	}
	else if(WM_SERVICE_LOG_WORN == uMsg)
	{
		DWORD dwStatus = wParam;
		ServiceLogWorn(dwStatus);
	}
#endif
	if (WM_RBUTTONDOWN == uMsg)
	{
		//LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
		OnRButtonDown(uMsg, wParam, lParam);
		return 0;
	}
	// 	else if(WM_KILLFOCUS == uMsg)
	// 	{
	// 		if(m_PaintManager.GetFocus() == m_reditSend && (m_pMenuMsgRButton == NULL || !IsWindowVisible(m_pMenuMsgRButton->GetHWND())) )
	// 		{
	// 			TEventUI eve;
	// 			eve.Type = UIEVENT_KILLFOCUS;
	// 			m_reditSend->DoEvent(eve);
	// 		}
	// 	}
	else if(WM_LBUTTONDOWN == uMsg)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		if(m_PaintManager.GetPaintWindow() != NULL)
		{
			if(m_PaintManager.FindControl(pt) == m_reditSend )
			{
				SetEditFocus();
			}
			if(m_strLinkInfo != "")
			{
				OpenHtmlPage(m_strLinkInfo);
			}
		}
	}
	else if(WM_LBUTTONDBLCLK == uMsg)
	{
		CString strImage = Richedit_GetSelImage(m_reditHistory);
		if(strImage != "")
		{
			string strFilePath = strImage;
			CDuiPicture::Show(strFilePath);
		}
	}
	else if(WM_WEBFILE_NOTIFY == uMsg)
	{
		IMAGE_FILE_STRUCT *pStruct = (IMAGE_FILE_STRUCT*)wParam;
		char * szFile = (char *)lParam;
		ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
		Richedit_ReplaceRecvImg(pTextServicesHistory, m_reditHistory, pStruct, szFile);
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CDuiP2PSendMsgDlg::InitWindow()
{
	g_LocalData.WriteLog(CLocalData::LL_INFORMATION, "CDuiP2PSendMsgDlg bgn");
	WindowImplBase::InitWindow();

	//创建窗口时先打开一个地址，防止一段时间不打开地址会造成右侧页白屏的问题
	m_wndAD = static_cast<CCefWebUI*>(m_PaintManager.FindControl(_T("ActiveXDemo1")));
	m_wndAD->SetParentWnd(this);
	m_wndAD->SetUrl(URLEncode("about:blank").c_str());
	//m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_SELLER).c_str());
//#ifdef BUSINESS_VESION
//	m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL).c_str());
//#else
//	m_wndAD->SetUrl(URLEncode((LPCTSTR)RIGHT_URL_SELLER).c_str());
//#endif

	//CVerticalLayoutUI* pVLWebright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLwebright")));
	//pVLWebright->SetFixedWidth(AD_WINDOW_WIDTH);

	m_reditSend = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richeditsendmsg")));
	m_reditHistory = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedithistory")));

	m_reditHistory->SetLinkMask();
	m_reditHistory->SetReadOnly();

	m_pwndTalkToolBar->SetRichEdit(m_reditSend);

	SetIcon(IDR_MAINFRAME);
	g_hP2PSendMSG = GetHWND();

	m_screenCopyTick = 0;
	m_pSelectGroupNode = NULL;
	m_pSelectFastWordNode = NULL;
	m_pMenuFastWord = NULL;
	m_pMenuFastWordNode = NULL;
	m_bIsFirstFastWordMenu = FALSE;
	m_strLastReditSendText = "";
	m_fontLast = g_ImFont;
	m_bIsFontLast = FALSE;
	m_bCanTran = TRUE;
	m_bIsGetFriendInfo = FALSE;
	m_nInsertBuyerId = 0;
#ifdef _CS_PLAT_EX_
	m_bIsTran = FALSE;
#endif
	//m_nCSID = 0;
	SetTimer(GetHWND(), TIMER_REDITSEND_TEXT_CHANGE, 100, NULL);

	//AttachHMGScrollbar(m_reditHistory.m_hWnd);
	//AttachHMGScrollbar(m_reditSend.m_hWnd);

	m_pLabelHangupOn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hangupon")));
	m_pLabelHangupOff = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hangupoff")));

	m_pBtnSend = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnsend")));
	m_pBtnSendConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnsendconfig")));
	m_pTabRight = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabRight")));

	CRect rect( 0,0,0,0 );//自己去改值看变化
	m_wndP2PFileTransProgressBar.Create( "",WS_VISIBLE|WS_GROUP|SS_NOTIFY|SS_BITMAP,rect,CWnd::FromHandle(GetHWND()));
	m_wndP2PFileTransProgressBar.ShowWindow( SW_HIDE );

	CButtonUI* pBtnHead = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnhead")));
	pBtnHead->SetBkImage("P2PSendMsgDlgTool\\88x88_border.png");
	CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
	m_pHead = static_cast<CButtonGifUI*>(m_PaintManager.FindControl(_T("btnhead")));
	UpdateBk();
	m_nSendFileSequence = GetTickCount();

	m_pBtnSend = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnsend")));
	m_pBtnSend->SetShortcut('s');

	g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_BUYER_OFFLINE_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CMEMBERS_IND, m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CDISMISS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CREATE_CLUSTER_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SEND_CPIC_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CPIC_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_START_SEND_CPIC_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_FRIEND_BASICINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2P_TEST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FILE_TRANS_IND_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_WEB_MORE_MSG_ACK,m_hWnd);

#ifdef _BUSINESS_VESION	
	g_pProtocol->RegistCmd(CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_NEAR_COMM_CUST_REMARK_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CUSTSER_TRAN_ACK, m_hWnd);
#endif
	////g_pProtocol->RegistCmd(CMD_FILE_TRANS_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FILE_TRANS_IND,m_hWnd);

	g_pProtocol->RegistCmd(CMD_SEND_FILE_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_DOWNLOAD_FILE_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_DOWNLOAD_FILE_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FIN_TRAN_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FIN_TRAN_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_ABORT_TRAN_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_ABORT_TRAN_ACK,m_hWnd);

#ifdef _CS_PLAT_EX_
	g_pProtocol->RegistCmd(CMD_MONITOR_SET_BUYER_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_START_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_START_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_END_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_END_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_INSERT_CHAT_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_INSERT_CHAT_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_TRAN_CHAT_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MONITOR_TRAN_CHAT_IND,m_hWnd);
	////g_pProtocol->RegistCmd(CMD_GET_USERID_MAP_CSID_ACK ,m_hWnd);//改为从WEB端获得用户CSID
#endif

	SetTimer(*this, TIMER_P2P_STATUS, 1000, NULL);

#ifndef _BUSINESS_VESION
	//非商家版屏蔽下面的按钮
	//推送服务评价表
	CButtonUI* pBtnScore = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscore")));
	pBtnScore->SetVisible(false);
	//快捷短语
	CButtonUI* pBtnFastword = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnfastword")));
	pBtnFastword->SetVisible(false);
	//客服转接
	CButtonUI* pBtnTran = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btntran")));
	pBtnTran->SetVisible(false);
	//挂起设置
	CButtonUI* pBtnConfigbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("configbtn")));
	pBtnConfigbtn->SetVisible(false);
			
	//CHorizontalLayoutUI* pHLHeadTool = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HLHeadTool")));
	//pHLHeadTool->SetMaxWidth(78);
#endif

#ifdef _CS_PLAT_EX_
	//挂起设置
	CButtonUI* pBtnConfigbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("configbtn")));
	pBtnConfigbtn->SetVisible(false);
	//CHorizontalLayoutUI* pHLHeadTool = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HLHeadTool")));
	//pHLHeadTool->SetFixedWidth(78);
#endif

#ifdef _WORK_EX_
	//挂起设置
	CButtonUI* pBtnConfigbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("configbtn")));
	pBtnConfigbtn->SetVisible(false);
	//CHorizontalLayoutUI* pHLHeadTool = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HLHeadTool")));
	//pHLHeadTool->SetFixedWidth(78);
#endif
	m_pTabRight->SetFixedWidth(AD_WINDOW_WIDTH);
	CVerticalLayoutUI* pVlWebright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLwebright")));
	pVlWebright->SetFixedWidth(AD_WINDOW_WIDTH);
}

void CDuiP2PSendMsgDlg::Notify(DuiLib::TNotifyUI& msg) 
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender->GetName() == _T("btnsend")) 
		{
			OnBnClickedBtnSend();
		}
		else if(msg.pSender->GetName() == _T("btnsendconfig")) 
		{
			OnBnClickedBtnHotkey();
		}
		else if(msg.pSender->GetName() == _T("btnfile")) 
		{
			CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,NULL,NULL);
			if ( dlg.DoModal()==IDOK )
			{	
				CString strFileName = dlg.GetPathName();
				P2PSendFile(strFileName);
			}
		}
#ifdef _BUSINESS_VESION
		else if(msg.pSender->GetName() == _T("btntran")) 
		{
			OnBnClickedBtnCustserTran();
		}
#endif
		else if(msg.pSender->GetName() == _T("btnfont")) 
		{
			OnBnClickedFontBar();
		}
		else if(msg.pSender->GetName() == _T("btnemotion")) 
		{
			OnFaceIconClicked();
		}	
		else if(msg.pSender->GetName() == _T("btnsendPic"))
		{
			OnSendPicClicked();
		}
		else if(msg.pSender->GetName() == _T("btnscreentool"))
		{
			OnScreenCopy();
		}
		else if(msg.pSender->GetName() == _T("btnscreentoolconfig"))
		{
			OnScreenCopyOption();
		}
		else if(msg.pSender->GetName() == _T("btnshake"))
		{
			OnBnClickedButtonShake();
		}
		else if(msg.pSender->GetName() == _T("btnscore"))
		{
			OnBnClickedButtonScore();
		}
		else if(msg.pSender->GetName() == _T("btnfastword") || msg.pSender->GetName() == _T("btnfastwordclose"))
		{
			OnBnClickedFastword(msg);
		}
		else if(msg.pSender->GetName() == _T("btnhistory"))
		{
			OnBnClickedButtonHistory();
		}
		else if(msg.pSender->GetName() == _T("closebtn"))
		{
#ifdef _CS_PLAT_EX_  //飞牛客服工作台
			if(CloseAllDlg())
				return;
#elif defined _WORK_EX_
			if(CloseAllDlg())
				return;
#else
			if(CMainFrame::IsBuyer(GetFriendInfo()->GetId()) || CMainFrame::IsWork(GetFriendInfo()->GetId()))
			{
				if(CloseAllDlg())
					return;
			}
#endif
		}
		else if(msg.pSender->GetName() == _T("maxbtn"))
		{
#ifdef _CS_PLAT_EX_
			CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

			pbtnMax->SetVisible(FALSE);
			pbtnRestore->SetVisible(TRUE);

			ShowDlgMax(TRUE);
			return;
#else
			if(CMainFrame::IsBuyer(m_FriendInfo.GetId()))
			{
				CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
				CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

				pbtnMax->SetVisible(FALSE);
				pbtnRestore->SetVisible(TRUE);

				ShowDlgMax(TRUE);
				return;
			}
#endif
		}
		else if(msg.pSender->GetName() == _T("restorebtn"))
		{
#ifdef _CS_PLAT_EX_
			CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

			pbtnMax->SetVisible(TRUE);
			pbtnRestore->SetVisible(FALSE);

			ShowDlgMax(FALSE);
			return;
#else
			if(CMainFrame::IsBuyer(m_FriendInfo.GetId()))
			{
				CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
				CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

				pbtnMax->SetVisible(TRUE);
				pbtnRestore->SetVisible(FALSE);

				ShowDlgMax(FALSE);
				return;
			}
#endif
		}
#ifdef _BUSINESS_VESION
		else if(msg.pSender->GetName() == _T("configbtn"))
		{
			OnBnClickedBtnHangup();
		}
#endif
		//快捷短语组点击设置菜单
		else if(msg.pSender->GetName() == _T("fastwordconfig") )
		{
			m_pMenuFastWord = new DuiLib::CMenuWnd();
			DuiLib::CPoint point = msg.ptMouse;
			ClientToScreen(m_hWnd, &point);
			m_pMenuFastWord->Init(NULL, _T("FastWordMenu.xml"), point, &m_PaintManager, NULL);

			CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treefastword")));
			CTreeNodeUI* pNode = (DuiLib::CTreeNodeUI*)msg.pSender->GetParent()->GetParent()->GetParent()->GetParent();
			CString str = pNode->GetFolderButton()->GetText();
			//如果为下载的快捷短语组则无法做任何修改
			BOOL bIsGroupLoad = g_LocalData.IsGroupLoad(str);
			if(bIsGroupLoad)
			{
				m_pMenuFastWord->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
				m_pMenuFastWord->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
				m_pMenuFastWord->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
				m_pMenuFastWord->GetMenuUI()->GetItemAt(3)->SetEnabled(false);
			}
			int nGroupIndex = g_LocalData.GetGroupIndex(str);
			if(nGroupIndex == 1)
			{
				m_pMenuFastWord->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
				m_pMenuFastWord->ResizeMenu();
			}
			else if(nGroupIndex == 2)
			{
				m_pMenuFastWord->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
				m_pMenuFastWord->ResizeMenu();
			}
			else if(nGroupIndex == 3)
			{
				m_pMenuFastWord->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
				m_pMenuFastWord->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
				m_pMenuFastWord->ResizeMenu();
			}
		}
		//快捷短语点击设置菜单
		else if(msg.pSender->GetName() == _T("fastwordnodeconfig") )
		{
			m_pMenuFastWordNode = new DuiLib::CMenuWnd();
			DuiLib::CPoint point = msg.ptMouse;
			ClientToScreen(m_hWnd, &point);
			m_pMenuFastWordNode->Init(NULL, _T("FastWordMenuNode.xml"), point, &m_PaintManager, NULL);

			CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treefastword")));
			CTreeNodeUI* pNode = (DuiLib::CTreeNodeUI*)msg.pSender->GetParent()->GetParent()->GetParent();

			DuiLib::CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pNode, _T("fastword")));

			CString strGroupName = m_pSelectFastWordNode->GetParentNode()->GetFolderButton()->GetText();
			int nFastWordIndex = m_pSelectFastWordNode->GetNodeIndex();

			//如果为下载的快捷短语则无法做任何修改
			BOOL bIsFastWordLoad = g_LocalData.IsFastWordLoad(strGroupName, nFastWordIndex);
			if(bIsFastWordLoad)
			{
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(3)->SetEnabled(false);
			}

			int nNodeIndex = g_LocalData.GetNodeIndex(strGroupName, nFastWordIndex);
			if(nNodeIndex == 1)
			{
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
				m_pMenuFastWordNode->ResizeMenu();
			}
			else if(nNodeIndex == 2)
			{
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
				m_pMenuFastWordNode->ResizeMenu();
			}
			else if(nNodeIndex == 3)
			{
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
				m_pMenuFastWordNode->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
				m_pMenuFastWordNode->ResizeMenu();
			}
		}
		//导入快捷短语
		else if(msg.pSender->GetName() == _T("btnFastWordImport") )
		{
			CFastWordImport::NewDlg(GetHWND());
		}
		//导出快捷短语
		if(msg.pSender->GetName() == _T("btnFastWordExport") )
		{
			CFastWordExport::NewDlg(GetHWND());
		}
		//新建快捷短语
		if(msg.pSender->GetName() == _T("btnFastWordNew") )
		{
			CFastWordNew::NewDlg(GetHWND());
		}
		//挂起按钮
		if(msg.pSender->GetName() == _T("hangupon") || msg.pSender->GetName() == _T("hangupoff"))
		{
			if(CMainFrame::GetInstance()->GetHangupSelect())
			{
				HangupCancel();
				CMainFrame::GetInstance()->SetHangupSelect(false);
				CMainFrame::GetInstance()->Hangup();
			}
			else if(!CMainFrame::GetInstance()->GetHangupSelect())
			{
				if(CMessageBox::Model(NULL, "设置为\"我要挂起\"后，用户通过点击您网站上的\"在线客服\"发送的消息，将不会分流到您这里。您确定要挂起吗？", IM_CAPTION, MB_OKCANCEL | MB_ICONWARNING) == IDOK)
				{
					CMainFrame::GetInstance()->SetHangupSelect(true);
					CMainFrame::GetInstance()->Hangup();
				}
			}
		}
#ifdef _CS_PLAT_EX_
		//监控
		if(msg.pSender->GetName() == _T("btnMonitorOn") )
		{
			//开启监控
			XT_MONITOR_START_REQ req;
			req.idMaster = g_ImUserInfo.GetId();
			req.idSeller = m_FriendInfo.GetId();
			g_pProtocol->SendReq(req);
		}
		//取消监控
		if(msg.pSender->GetName() == _T("btnMonitorOff") )
		{
			//关闭监控
			XT_MONITOR_END_REQ req;
			req.idMaster = g_ImUserInfo.GetId();
			req.idSeller = m_FriendInfo.GetId();
			g_pProtocol->SendReq(req);
		}
		//插入会话
		if(msg.pSender->GetName() == _T("btnInsertMsg") )
		{
			//打开插入会话
			if(!m_bIsInsertMsg)
			{
				XT_MONITOR_INSERT_CHAT_REQ req;
				req.idMaster = g_ImUserInfo.GetId();
				req.idSeller = m_FriendInfo.GetId();
				req.idBuyer = m_nIdMonitorBuyer;
				g_pProtocol->SendReq(req);
			}
		}
		//接手会话
		if(msg.pSender->GetName() == _T("btnTranMsg") )
		{
			//接手会话
			XT_MONITOR_TRAN_CHAT_REQ req;
			req.idMaster = g_ImUserInfo.GetId();
			req.idSeller = m_FriendInfo.GetId();
			req.idBuyer = m_nIdMonitorBuyer;
			g_pProtocol->SendReq(req);
		}
#endif
	}
	//快捷短语选中一项
	if(msg.sType == _T("itemclick"))
	{
		CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treefastword")));
		if(msg.pSender->GetParent()->GetParent() == pTreeView)
		{
			CTreeNodeUI* pNode = ((CTreeNodeUI*)msg.pSender);
			DuiLib::CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pNode, _T("fastword")));

			if(m_pSelectFastWordNode != NULL && m_pSelectFastWordNode->GetParentNode() != NULL)
			{
				CString strGroupName = m_pSelectFastWordNode->GetParentNode()->GetFolderButton()->GetText();
				int nFastWordIndex = m_pSelectFastWordNode->GetNodeIndex();

				if(pLabelWord != NULL)
				{
					CString strText = g_LocalData.GetFastWordText(strGroupName, nFastWordIndex);
					//m_reditSend.SetText(strText);
					m_reditSend->SetText(strText);
					//点击快捷短语记录之前的字体，用于发送之后还原
					if(m_bIsFontLast == FALSE)//获取改之前的字体
					{
						//m_fontLast = m_reditSend.GetXFont();
						m_fontLast = g_ImFont;//m_reditSend->GetXFont();
					}
					m_bIsFontLast = TRUE;
					g_ImFont = g_LocalData.GetFastWordFont(strText);
					//m_reditSend.SetXFont(g_ImFont);
					ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
					RichEdit_SetFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
				}
			}

		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

LRESULT CDuiP2PSendMsgDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == WM_TIMER)
	{
		OnTimer(uMsg, wParam, lParam, bHandled); 
	}
	else if (uMsg == WM_MENUCLICK)//处理菜单点击消息
	{
		CDuiString *strMenuName = (CDuiString*)wParam;
		BOOL bChecked = (BOOL)lParam;		 
		if ( *strMenuName == _T("MenuRename") )//快捷短语组重命名
		{
			CString strGroupName = m_pSelectGroupNode->GetFolderButton()->GetText();
			CFastWordGroupRename::NewDlg(GetHWND(), strGroupName);
		}
		else if ( *strMenuName == _T("MenuUp") )//快捷短语组上移
		{
			if(m_pSelectGroupNode != NULL)
			{
				CString strGoupName = m_pSelectGroupNode->GetFolderButton()->GetText();
				g_LocalData.FastWordGroupUp(strGoupName);
				UpdateFastWordInfo();
				g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
				g_LocalData.SaveFastWordInfo();
			}
		}
		else if ( *strMenuName == _T("MenuDown") )//快捷短语组下移
		{
			if(m_pSelectGroupNode != NULL)
			{
				CString strGoupName = m_pSelectGroupNode->GetFolderButton()->GetText();
				g_LocalData.FastWordGroupDown(strGoupName);
				UpdateFastWordInfo();
				g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
				g_LocalData.SaveFastWordInfo();
			}
		}
		else if ( *strMenuName == _T("MenuDelete") )//快捷短语组删除
		{
			if(m_pSelectGroupNode != NULL)
			{
				CString strGoupName = m_pSelectGroupNode->GetFolderButton()->GetText();
				g_LocalData.FastWordGroupDelete(strGoupName);
				UpdateFastWordInfo();
				g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
				g_LocalData.SaveFastWordInfo();
			}
		}

		else if ( *strMenuName == _T("MenuRenameNode") )//快捷短语编辑
		{
			CString strGroupName = m_pSelectFastWordNode->GetParentNode()->GetFolderButton()->GetText();
			int nFastWordIndex = m_pSelectFastWordNode->GetNodeIndex();
			CFastWordNew::NewDlg(GetHWND(), 2, strGroupName, nFastWordIndex);
		}
		else if ( *strMenuName == _T("MenuUpNode") )//快捷短语上移
		{
			if(m_pSelectGroupNode != NULL)
			{
				CString strGroupName = m_pSelectFastWordNode->GetParentNode()->GetFolderButton()->GetText();
				int nFastWordIndex = m_pSelectFastWordNode->GetNodeIndex();
				g_LocalData.FastWordUp(strGroupName, nFastWordIndex);
				UpdateFastWordInfo();
				g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
				g_LocalData.SaveFastWordInfo();
			}
		}
		else if ( *strMenuName == _T("MenuDownNode") )//快捷短语下移
		{
			if(m_pSelectGroupNode != NULL)
			{
				CString strGroupName = m_pSelectFastWordNode->GetParentNode()->GetFolderButton()->GetText();
				int nFastWordIndex = m_pSelectFastWordNode->GetNodeIndex();
				g_LocalData.FastWordDown(strGroupName, nFastWordIndex);
				UpdateFastWordInfo();
				g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
				g_LocalData.SaveFastWordInfo();
			}
		}
		else if ( *strMenuName == _T("MenuDeleteNode") )//快捷短语删除
		{
			if(m_pSelectGroupNode != NULL)
			{
				CString strGroupName = m_pSelectFastWordNode->GetParentNode()->GetFolderButton()->GetText();
				int nFastWordIndex = m_pSelectFastWordNode->GetNodeIndex();
				g_LocalData.FastWordDelete(strGroupName, nFastWordIndex);
				UpdateFastWordInfo();
				g_LocalData.SetFastWordInfo(g_LocalData.GetFastWordInfo());
				g_LocalData.SaveFastWordInfo();
			}
		}
		else if ( *strMenuName == _T("MenuReditSendCut") )
		{
			m_reditSend->Cut();
		}
		else if ( *strMenuName == _T("MenuReditSendCopy") )
		{
			//m_reditSend->Copy();
			CString text= Richedit_GetSelText(m_reditSend);
			if(OpenClipboard(GetHWND()))
			{
				HGLOBAL clipbuffer;
				char * buffer;
				char * pstr = (char *)(LPCSTR)text;

				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(pstr)+1)*sizeof(WCHAR));
				buffer = (char *)GlobalLock(clipbuffer);

				strcpy(buffer,pstr);

				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
			}

		}
		else if ( *strMenuName == _T("MenuReditSendPaste") )
		{
			//m_reditSend->PasteSpecial(CF_TEXT);
			if(OpenClipboard(GetHWND()))
			{
				CBitmap* pBitmap = (CBitmap*)GetClipboardData(CF_BITMAP);
				if(pBitmap != NULL)
				{
					CImage image;
					image.Attach((HBITMAP)pBitmap);
					CString str;
					str.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));
					image.Save(str);
					CString strCacheFileName;
					g_LocalData.SaveImageFileToCache(str,strCacheFileName);
					strCacheFileName = g_LocalData.GetImageFileCachePath() + strCacheFileName;
					image.Save(strCacheFileName);
					image.Detach();
	
					RichEdit_InsertImage(m_reditSend, (LPCTSTR)strCacheFileName, -1, -1);
					//删除临时图片
					DeleteFile(str);
				}
				char * buffer;
				buffer = (char *)GetClipboardData(CF_TEXT);
				if(buffer != NULL && strcmp(buffer, "") != 0)
				{
					ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
					RichEdit_InsertText(pTextServicesSend, buffer, m_reditSend, FALSE, NULL, GetHWND());
				}
				CloseClipboard();
			}

		}
		else if ( *strMenuName == _T("MenuReditSendSelectAll") )
		{
			m_reditSend->SetSel(0, -1);
		}
		else if ( *strMenuName == _T("MenuReditSendClear") )
		{
			//m_reditSend->SetText("");
			m_reditSend->SetSel(0, -1);
			m_reditSend->ReplaceSel("", false);
			ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
			RichEdit_SetDefFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
		}
		else if ( *strMenuName == _T("MenuReditRecvCopy") )
		{
			//m_reditHistory->Copy();
			CString text= Richedit_GetSelText(m_reditHistory);
			if(OpenClipboard(GetHWND()))
			{
				HGLOBAL clipbuffer;
				char * buffer;
				char * pstr = (char *)(LPCSTR)text;

				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(pstr)+1)*sizeof(WCHAR));
				buffer = (char *)GlobalLock(clipbuffer);

				strcpy(buffer,pstr);

				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
			}
		}
		else if ( *strMenuName == _T("MenuReditRecvSelectAll") )
		{
			m_reditHistory->SetSel(0, -1);
		}
		else if ( *strMenuName == _T("MenuReditRecvClear") )
		{
			//m_reditHistory->SetText("");
			m_reditHistory->SetSel(0, -1);
			m_reditHistory->ReplaceSel("", false);
		}
		else if ( *strMenuName == _T("MenuWebRefresh") )
		{
			//刷新右侧页
			m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());

			CString strLogUrl;
			strLogUrl.Format("刷新地址:%s", m_strAdUrl);
			g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLogUrl);
		}
		m_pMenuMsgRButton = NULL;
		m_pMenuWebRButton = NULL;
		//delete strMenuName;
	}
	return DuiLib::WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

BOOL CDuiP2PSendMsgDlg::CallbackNagive(DWORD dwThis, char *p)
{
	CDuiP2PSendMsgDlg *pDlg = (CDuiP2PSendMsgDlg *)dwThis;
	CString strUrl = URLDecode(p).c_str();
	/*
	if(strUrl.Find(RIGHT_URL_SELLER) == -1 && strUrl.Find(RIGHT_URL) == -1 && strUrl.Find("http://im.feiniu.com") == -1)
	{
		//*Cancel =TRUE;
		ShellExecute(NULL, "open", strUrl, NULL, NULL, SW_SHOWNORMAL);
		return TRUE;
	}
	*/

#ifdef _CS_PLAT_EX_//飞牛客服工作台
	int nServiceLogFlag = 0;
	if(strUrl.Find("http://im.feiniu.com/?savedlog=") == 0)//是否保存服务小记
	{
		extern CLocalData  g_LocalData;
		CString strSavedlog = strUrl.Right(strUrl.GetLength()- CString("http://im.feiniu.com/?savedlog=").GetLength());
		nServiceLogFlag = _ttoi(strSavedlog);//（1--已保存;2--保存不完整）

		if(nServiceLogFlag == 1)
		{
			m_bCanTran = FALSE;
		}
		/*DWORD dwFid = pDlg->m_FriendInfo.GetId();///当前聊天框好友ID*/
		DWORD dwStatus = nServiceLogFlag;//当前的服务小记的状态

		PostMessage(WM_SERVICE_LOG_WORN, dwStatus, NULL);
		return TRUE;
	}
#endif

#ifdef _BUSINESS_VESION
	//与右侧页面交互
	/*else */if(strUrl.Find("http://im.feiniu.com/?link=") == 0)
	{
		CString strGoodsUrlE = strUrl.Right(strUrl.GetLength() - CString("http://im.feiniu.com/?link=").GetLength());
		int OutByte = 0;
		strGoodsUrlE.Replace(" ", "+");
		string strGoodsUrlD = Base64Decode(strGoodsUrlE, strGoodsUrlE.GetLength(),OutByte);
		CString strText = strGoodsUrlD.c_str();
		
		if(strText.Find("http://") == 0)
			//pDlg->m_reditSend.InsertLink(strGoodsUrlD.c_str());
			pDlg->m_reditSend->InsertText(-1, strGoodsUrlD.c_str());
		else
		{
			strText = UTF8_Convert(strText, FALSE);
			//pDlg->m_reditSend.InsertText(strText);
			pDlg->m_reditSend->InsertText(-1, strText);
		}
		return TRUE;
	}
	else if(strUrl.Find("http://im.feiniu.com") == 0)
	{
		//*Cancel =TRUE;
		int nBeg = strUrl.Find("comment=");
		if(nBeg > 0)
		{
			nBeg += strlen("comment=");
			int nEnd = strUrl.Find("&",nBeg);
			if(nEnd == -1)
				nEnd = strUrl.GetLength();
			if(nEnd > nBeg)
			{
				pDlg->m_strNewComnCustRemarkClient = strUrl.Mid(nBeg , nEnd - nBeg);
			}

		}
		return TRUE;
	}

#endif

	return FALSE;
}

void CDuiP2PSendMsgDlg::InitResource()
{
}


void CDuiP2PSendMsgDlg::UpdateSignature()
{	
}

void CDuiP2PSendMsgDlg::OnFaceIconClicked()
{
	DuiLib::CPoint pt;
	GetCursorPos(&pt);

	CEmotionDlg *dlg = new CEmotionDlg(pt.x,pt.y,&g_imgEmotionList,CWnd::FromHandle(GetHWND()));
	dlg->m_hEmotionSelWnd = m_hWnd;
	dlg->ShowWindow(SW_SHOW);	
}

void CDuiP2PSendMsgDlg::OnSendPicClicked()
{
	bool bCanSendPic=false ;

	if ( m_pmP2PMode!=pmMultiTalk )
	{
		//if ( m_FriendInfo.status==XTREAM_OFFLINE )
		//{
		//	//MessageBox("对不起对方不在线！不能发送图片！",IM_CAPTION,MB_OK|MB_ICONWARNING);	
		//	ShowMessage("对方不在线或隐身！不能发送图片！");
		//}
		//else
		{
			bCanSendPic = true;	
		}		
	}
	else
	{
		bCanSendPic = true;
	}

	if ( bCanSendPic )
	{
		CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,CWnd::FromHandle(GetHWND()));

		if ( IDOK==dlg.DoModal() )
		{
			if ( dlg.GetSelFileSize()<1024 )
			{
				CString strCacheFileName;
				g_LocalData.SaveImageFileToCache(dlg.GetPathName(),strCacheFileName);
				CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

				//m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());
				//m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);
				//m_reditSend.SetFocus();
				RichEdit_InsertImage(m_reditSend, (LPCTSTR)strCacheImageFullPathName, -1, -1);
				SetEditFocus();
			}
			else
			{
				ShowMessage("对不起，您只能发送小于1M的图片！",MB_ICONINFORMATION|MB_OK);
			}
		}
	}
}

void CDuiP2PSendMsgDlg::ShowFileTransInfoBar(bool bShow)
{
	if (bShow)
	{
		if ( m_wndP2PFileTransProgressBar.IsWindowVisible()==FALSE )
		{

			CWndUI* pWndReditHistory = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("richedithistory")));
			CRect rc = pWndReditHistory->GetPos();

			SCROLLINFO si1;
			memset(&si1,0,sizeof(si1));
			si1.cbSize = sizeof(si1);

			//m_reditHistory.GetScrollInfo(SB_VERT,&si1);


			m_wndP2PFileTransProgressBar.MoveWindow(rc.left,rc.top,rc.Width(),65);
			//m_reditHistory.MoveWindow(rc.left,rc.top+65,rc.Width(),rc.Height()-65);

			m_wndP2PFileTransProgressBar.ShowWindow(SW_SHOW);


			SCROLLINFO si2;
			memset(&si2,0,sizeof(si2));
			si2.cbSize = sizeof(si2);

			//m_reditHistory.SetSel(-1,-1);
			//m_reditHistory.GetScrollInfo(SB_VERT,&si2);

			si2.nPos = si2.nMax-(si1.nMax-si1.nPos);
			if ( si2.nPos<0 )
				si2.nPos=0;

			//m_reditHistory.SetScrollInfo(SB_VERT,&si2);
		}
	}
	else
	{
		if ( m_wndP2PFileTransProgressBar.IsWindowVisible() )
		{
			CWndUI* pWndReditHistory = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("richedithistory")));
			CRect rc = pWndReditHistory->GetPos();

			SCROLLINFO si1;
			memset(&si1,0,sizeof(si1));
			si1.cbSize = sizeof(si1);

			//m_reditHistory.GetScrollInfo(SB_VERT,&si1);

			m_wndP2PFileTransProgressBar.ShowWindow(SW_HIDE);
			//rc.top-=65;

			//m_reditHistory.MoveWindow(&rc);   

			SCROLLINFO si2;
			memset(&si2,0,sizeof(si2));
			si2.cbSize = sizeof(si2);

			//m_reditHistory.GetScrollInfo(SB_VERT,&si2);

			si2.nPos = si2.nMax-(si1.nMax-si1.nPos);
			if ( si2.nPos<0 )
				si2.nPos=0;

			//m_reditHistory.SetScrollInfo(SB_VERT,&si2);
		}
	}
}

void CDuiP2PSendMsgDlg::AdjustUI()
{
	if ( m_wndHide.m_hWnd==NULL )
		return ;

	CRect rc; 
	//GetClientRect(&rc);
	//::GetClientRect(m_reditHistory.GetSafeHwnd(), &rc);

	int cx = rc.Width();
	int cy = rc.Height();

	int nTop   = 90;
	int nLeft  = BORDER_WIDTH;

	int nClientCx ;
	int nBarCx    ;	

	if ( m_bClosePanel )
	{
		nBarCx = 0;
	}
	else
	{
		if (m_bShowHistory) {
			nBarCx = m_uPopupHistoryWidth + BAR_BORDER_WIDTH;
		}
		else
		{
			nBarCx = AD_WINDOW_WIDTH + BAR_BORDER_WIDTH;
		}
	}

	nClientCx = cx-nBarCx-BUTTON_FOLDER_WIDTH;

	m_wndHide.MoveWindow(cx-10,0,10,10);

	if ( m_nSplitterPos>(cy-150) )
	{
		m_nSplitterPos = cy-150;
	}

	DuiLib::CPoint ptUserPanel;
	ptUserPanel.x = cx-nBarCx-BUTTON_FOLDER_WIDTH, ptUserPanel.y = nTop;

	DuiLib::CSize  szUserPanel;
	szUserPanel.cx = nBarCx, szUserPanel.cy = cy-nTop-1;

	//不显示折叠按钮
	//m_btnFolder.MoveWindow(cx-BUTTON_FOLDER_WIDTH-2,nTop-1, BUTTON_FOLDER_WIDTH,21);
	m_btnFolder.ShowWindow(SW_HIDE);

	DuiLib::CPoint ptHistory;
	ptHistory.x = nLeft, ptHistory.y=nTop;

	DuiLib::CSize  szHistory;
	szHistory.cx = nClientCx, szHistory.cy=cy-m_nSplitterPos-nTop;

	nTop = cy-m_nSplitterPos;

	DuiLib::CPoint ptSplitter;
	ptSplitter.x = nLeft, ptSplitter.y = nTop+1;

	DuiLib::CSize  szSplitter;
	szSplitter.cx = nClientCx, szSplitter.cy = 3;

	DuiLib::CPoint ptToolBar;
	ptToolBar.x = nLeft, ptToolBar.y = nTop;

	DuiLib::CSize  szToolBar;
	szToolBar.cx = nClientCx, szToolBar.cy = 28;

	nTop +=28;

	DuiLib::CPoint ptSend;
	ptSend.x = nLeft, ptSend.y = nTop;

	DuiLib::CSize  szSend;
	szSend.cx = nClientCx, szSend.cy = cy - 35-nTop;

	nTop+=(3+szSend.cy);

	DuiLib::CSize szBtn;

	bool bSizeNS=true, bSizeEW=true;

	if ( rc.Height()<m_rcPre.Height() )
		bSizeNS=false;

	if ( rc.Width()<m_rcPre.Width() )
		bSizeEW=false;

	m_rcPre = rc;
}

void CDuiP2PSendMsgDlg::LayoutButton(int nLeft, int nTop, DuiLib::CSize& szBtn, int nClientCx)
{

}

void CDuiP2PSendMsgDlg::LayoutFileWindow(DuiLib::CPoint& ptSend, DuiLib::CSize& szSend, DuiLib::CPoint& ptHistory, DuiLib::CSize& szHistory,
	DuiLib::CPoint& ptSplitter, DuiLib::CSize& szSplitter, DuiLib::CPoint& ptToolBar, DuiLib::CSize& szToolBar)
{

}

void CDuiP2PSendMsgDlg::LayoutUserWindow(DuiLib::CPoint& ptUserPanel, DuiLib::CSize& szUserPanel, DuiLib::CPoint& ptHistory, DuiLib::CSize& szHistory)
{

}

void CDuiP2PSendMsgDlg::AddMsgTip(CString strTip, COLORREF clrText)
{
	//if ( m_hWnd==NULL || m_reditHistory.m_hWnd==NULL )
	//{
	//	_tagMsg t_msg;
	//	sprintf(t_msg.msg.data,strTip);
	//	t_msg.type = 1;
	//	PushMsg(t_msg);
	//	return ;
	//}

	XT_FONT xf;
	xf.fontColor = clrText;

	CHARRANGE cr;

	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(cr);

	m_reditHistory->InsertText(-1, "\n");
	//m_reditHistory->SetStartIndent(230);
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	RichEdit_SetStartIndent(pTextServicesHistory, 230);

	CResBitmap bmpLine;
	bmpLine.LoadBitmap(IDB_HINT_FLAG2);

	if ( bmpLine.GetSafeHandle() )
		Richedit_InsertBitmap(m_reditHistory, ((HBITMAP)bmpLine.GetSafeHandle()));

	m_reditHistory->InsertText(-1, " "+strTip);

	m_reditHistory->SetSel(cr.cpMax,-1);
	m_reditHistory->SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory->SetSel(-1,-1);

	m_reditHistory->EndDown();
}

//文件传输完成后添加打开文件和打开文件夹
void CDuiP2PSendMsgDlg::AddFileOpenTip()
{
	//CHARRANGE cr;

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(cr);

	//m_reditHistory.InsertText("\n");
	//m_reditHistory.SetStartIndent(230);

	//XT_FONT xf;
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.InsertLink("打开文件");
	//m_reditHistory.InsertText(" / ");
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.InsertLink("打开文件夹");

	CHARRANGE cr;

	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(cr);

	m_reditHistory->InsertText(-1, "\n");
	//m_reditHistory->SetStartIndent(230);
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	RichEdit_SetStartIndent(pTextServicesHistory, 230);


	XT_FONT xf;
	long lStartChar = 0, lEndChar = 0;
	m_reditHistory->SetSel(-1,-1);
	//RichEdit_SetStartIndent(m_pTextServicesHistory, 80);
	RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
	m_reditHistory->InsertText(-1, "打开文件");
	lEndChar = lStartChar + _tcslen("打开文件");
	RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, true);

	m_reditHistory->SetSel(-1,-1);
	RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
	m_reditHistory->InsertText(-1, " / ");
	lEndChar = lStartChar + _tcslen(" / ");
	RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

	m_reditHistory->SetSel(-1,-1);
	RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
	m_reditHistory->InsertText(-1, "打开文件夹");
	lEndChar = lStartChar + _tcslen("打开文件夹");
	RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, true);
}

void CDuiP2PSendMsgDlg::AddP2PTip()
{
	return;
	//if ( m_pmP2PMode!=pmMultiTalk )
	//{
	//	XT_FONT xf;
	//	xf.fontColor=RGB(0,166,84);

	//	m_reditHistory.SetStartIndent(120);
	//	m_reditHistory.InsertText("\n");

	//	m_reditHistory.SetStartIndent(230);

	//	CResBitmap bmpLine;
	//	bmpLine.LoadBitmap(IDB_HINT_FLAG2);
	//	m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

	//	m_reditHistory.SetSel(-1,-1);
	//	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	//	m_reditHistory.InsertText(" 有奖活动");
	//	m_reditHistory.InsertText("\n");

	//	m_reditHistory.SetStartIndent(80);
	//	m_reditHistory.InsertLine();
	//}
}

void CDuiP2PSendMsgDlg::AddCMemberTip(XT_USER_INFO *user, int action)
{
	CString strTip;

	if ( action==1 )
		strTip.Format("%s 进来了",UserInfoToStr(user) );
	else
		strTip.Format("%s 退出了",UserInfoToStr(user));

	AddMsgTip(strTip);
}

void CDuiP2PSendMsgDlg::SaveClusterMsg(const XT_MSG &msg)
{
	if ( m_bClusterInit && m_pmP2PMode==pmMultiTalk )
	{
		XT_MSG msg_temp = msg;
		msg_temp.msg_type = 0;//p2p聊天消息
		size_t i,c;
		c = m_clusterInfo.Count();
		for ( i = 0; i< c; i++ )
		{
			g_LocalData.SaveTalkMsg(msg_temp,m_clusterInfo.members[i].GetId());
		}
	}
}

void CDuiP2PSendMsgDlg::ProcessMsg(const XT_MSG &msg)
{
#ifdef _IM_UNIT_TEST
	IMTEST->RecvP2pMsg(m_FriendInfo, msg);
	return;
#endif
	switch (msg.data_type) 
	{
	case XT_MSG::IM_MSG:		// 即时聊天消息
	//case XT_MSG::EVALUA_ACK:	// 服务评价表应答
	case XT_MSG::AUTO_REPLY:	// 自动接待
		{
			if (msg.msg_type==0 || msg.msg_type == 10 )
			{
				AddTalkMsg(msg,false);
			}
			else//群聊消息
			{	
				if ( m_bClusterInit==false )
				{
					m_fifoMsg.Push(msg);
				}
				else
				{
					AddTalkMsg(msg,false);
				}
			}
		}
		break;
	case XT_MSG::EVALUA_ACK:
		{
			if (msg.msg_type==0 || msg.msg_type == 10 )
			{
				AddTalkMsg(msg,false);

				XT_MSG msg;
				SF_InitXtMsg(msg);
				msg.data_type   = XT_MSG::IM_MSG;//即时聊天消息

				CString text = "您已评价，感谢您的宝贵意见哦~~";
				strcpy(msg.data,text);
				msg.data_len=strlen(text)+1;

				int  type = CIMProtocol::SMT_SERVER;
				g_pProtocol->SendP2PMsg(msg,type);
				text = "[自动回复]您已评价，感谢您的宝贵意见哦~~";//自动回复前面添加提示
				strcpy(msg.data,text);
				AddTalkMsg(msg,false);
			}
			else//群聊消息
			{	
				if ( m_bClusterInit==false )
				{
					m_fifoMsg.Push(msg);
				}
				else
				{
					AddTalkMsg(msg,false);
				}
			}
		}
		break;
	//case XT_MSG::SF_FILE_REQ:
	//	// 收到发送方发过来的图片文件请求
	//	SF_RecvPfileReq(msg);
	//	break;
	case XT_MSG::SF_FILE_ACK:
		// 收到接收方应答，并开始发送数据(仅一次)
		SF_RecvPfileAck(msg);
		break;
	//case XT_MSG::SF_PACKET_REQ:
	//	// 接收发送方发送的数据
	//	SF_RecvPfilePacket(msg);
	//	break;
	case XT_MSG::SF_PACKET_ACK:
		// 发送方发送图片数据
		SF_RecvPfilePacketAck(msg);
		break;
	case XT_MSG::SF_ABORT:
		if(m_pmP2PMode!=pmMultiTalk)
		{
			SF_RecvAbortPfileTransmit(msg);
		}
		break;
	case XT_MSG::WIN_SHAKE:
		{
			WORD nVK = 0;
			g_LocalData.GetShakeWinConfig(nVK);
			if(nVK == 1)////支持振屏
			{
				XT_MSG msgShow = msg;
				CString text = UserInfoToStr(&m_FriendInfo) + "发送了一个窗口抖动~";
				strcpy(msgShow.data,text);
				msgShow.data_len = strlen(text)+1;
				AddTalkMsg(msgShow,false);
// 				//开始抖
 				SetTimer(*this, TIMER_WIN_SHAKE,20,NULL);
				m_nWinShakeCnt = 0;
			}
		}
		break;
	case XT_MSG::SF_CFILE_REQ:
		// 通知本用户开始下载群图片
		SF_RecvStartDownloadCfileNotify(msg);
		break;
	case XT_MSG::PUSH_EVALUA:
		{
			AddTalkMsg(msg, false);
		}
		break;
	default:
		break;
	}	
}

void CDuiP2PSendMsgDlg::SF_OnAbortFile(const char *file_name)
{
	//if ( m_hWnd && m_reditHistory.m_hWnd )
	//	m_reditHistory.OnAbortImageFile(file_name);
}

void CDuiP2PSendMsgDlg::SF_OnRecvFileComplete(const char *file_name)
{
	//if ( m_hWnd && m_reditHistory.m_hWnd )
	//	m_reditHistory.OnRecvImageFile(file_name);
}

void CDuiP2PSendMsgDlg::RefreshFriendStatus()
{
	XT_STATUS_GET_REQ req;
	req.fid = m_FriendInfo.GetId();
	req.id = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CDuiP2PSendMsgDlg::SF_InitXtMsg(XT_MSG &msg)
{
	memset(&msg,0,sizeof(msg));

	msg.ver         = XTREAM_CLIENT_VERSION;
	//	msg.face		= g_ImUserInfo.face;
	msg.from_id     = g_ImUserInfo.GetId();

	msg.fontColor   = g_ImFont.fontColor;
	msg.fontSize    = g_ImFont.fontSize;
	msg.fontStyle   = g_ImFont.flags;
	strcpy(msg.fontName,g_ImFont.fontName);
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

	if ( m_pmP2PMode!=pmMultiTalk )
	{
		msg.to_id		= m_FriendInfo.GetId();
		msg.dest_ip		= m_FriendInfo.ip;
		msg.dest_port	= m_FriendInfo.port;
		msg.msg_type    = 0;		
	}
	else
	{
		msg.cluster_id   = m_clusterInfo.nFamilyID;
		msg.cluster_type = 2;
		msg.msg_type     = 1;
	}
}

void CDuiP2PSendMsgDlg::PushMsg(const _tagMsg & msg)
{
	m_lstMsgRecv.push_back(msg);
	if ( m_lstMsgRecv.size()>50 )
		m_lstMsgRecv.pop_front();
}

void CDuiP2PSendMsgDlg::PopMsg()
{
	//if ( m_reditHistory.m_hWnd && m_hWnd )
	//{
	//	list<_tagMsg>::iterator it;

	//	//	m_lstMsgRecv.sort();

	//	for ( it=m_lstMsgRecv.begin(); it!=m_lstMsgRecv.end(); it++ )
	//	{
	//		//AddTalkMsg( *it, false );
	//		switch ( it->type )
	//		{
	//		case 0:
	//			AddTalkMsg(it->msg,false);
	//			g_LocalData.SaveTalkMsg(it->msg);
	//			break;
	//		case 1:
	//			AddMsgTip(it->msg.data);
	//			break;
	//		case 2:
	//			RecvMagicFace(it->msg.from_id,it->msg.fontName,it->msg.data);
	//			break;	
	//		}
	//	}

	//	m_lstMsgRecv.clear();
	//}
}

void CDuiP2PSendMsgDlg::AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg,int nUnReadNum)
{
	if ( m_hWnd==NULL || /*m_reditHistory.m_hWnd==NULL*/m_reditHistory==NULL )
	{
		_tagMsg t_msg;
		t_msg.msg  = msg;
		t_msg.type = 0;
		PushMsg(t_msg);

		return ;
	}
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	CString strData = msg.data;
	if((strData.Find("已将您转接至客服")  != -1) || (strData.Find("----将由客服人员") != -1))
	{
		XT_FONT xfTran;
		xfTran.fontColor=RGB(254,154,105);

		//m_reditHistory.SetSel(-1,-1);
		//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfTran));
		//m_reditHistory.SetStartIndent(80);
		m_reditHistory->SetSel(-1,-1);
		RichEdit_SetFont(pTextServicesHistory, xfTran.fontName, xfTran.fontSize, xfTran.fontColor, xfTran.flags & XTF_BOLD, xfTran.flags & XTF_ITALIC, xfTran.flags & XTF_UNDERLINE, false);
		RichEdit_SetStartIndent(pTextServicesHistory, 80);

		//m_reditHistory.InsertText(strData);
		//m_reditHistory.SetSel(-1,-1);
		//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(g_ImFont));
		m_reditHistory->InsertText(-1, strData);
		m_reditHistory->SetSel(-1,-1);
		RichEdit_SetFont(pTextServicesHistory, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
		return;
	}

	//m_reditHistory.SetSel(-1,-1);
	m_reditHistory->SetSel(-1,-1);
	CHARRANGE cr;

	XT_FONT xf;

	CString strTitle,strDate, strTime;
	CTime  tm(msg.send_time);

	strTime=tm.Format("%H:%M:%S");
	strDate=tm.Format("%Y.%m.%d");

	//好友发给我的信息
	if ( msg.from_id!=g_ImUserInfo.GetId() )
	{
		FRIEND_INFO *pFriend = ::FindFriend(msg.from_id);
		if(pFriend == NULL)
		{
			strTitle.Format("%s ", msg.from_nickname);
		}
		else
		{
			strTitle.Format("%s ", UserInfoToStrEx(pFriend));
		}
	}
	else
	{
		strTitle.Format("%s ", UserInfoToStrEx(&g_ImUserInfo));	
	}

	if ( isHistoryMsg )
	{
		xf.fontColor=RGB(122,122,122); ///一般的历史消息颜色
	}
	else
	{
		if(nUnReadNum>0)///未读消息深色辨识
		{
			xf.fontColor=RGB(22,68,223);
		}
		else
		{
			if ( g_ImUserInfo.GetId()==msg.from_id )
			{
				xf.fontColor=RGB(0,128,64); ///自己发送的信息
			}
			else
			{
				xf.fontColor=RGB(22,68,223);///外部接收的信息
			}
		}
	}

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.GetSel(cr);
	//m_reditHistory.InsertText("\n");
	m_reditHistory->SetSel(-1,-1);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	m_reditHistory->GetSel(cr);
	m_reditHistory->InsertText(-1, "\n");
	m_vecRecords.push_back(cr.cpMin);
	//m_reditHistory.SetStartIndent(80);
	//m_reditHistory.InsertText(strTitle);
	//m_reditHistory.SetSel(cr.cpMin,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(cr);
	//strTitle.Format("(%s %s):", strDate,strTime);
	//m_reditHistory.InsertText(strTitle);
	//m_reditHistory.InsertText("\n");
	//m_reditHistory.SetSel(cr.cpMin,-1);
	//XT_FONT xfG;
	//xfG.fontColor = 0x808080;
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfG));

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(cr);

	RichEdit_SetStartIndent(pTextServicesHistory, 80);
	m_reditHistory->InsertText(-1, strTitle);
	m_reditHistory->SetSel(cr.cpMin,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(cr);
	strTitle.Format("(%s %s):", strDate,strTime);
	m_reditHistory->InsertText(-1, strTitle);
	m_reditHistory->InsertText(-1, "\n");
	m_reditHistory->SetSel(cr.cpMin,-1);
	XT_FONT xfG;
	xfG.fontColor = 0x808080;
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfG));
	RichEdit_SetFont(pTextServicesHistory, xfG.fontName, xfG.fontSize, xfG.fontColor, xfG.flags & XTF_BOLD, xfG.flags & XTF_ITALIC, xfG.flags & XTF_UNDERLINE, false);

	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(cr);


	XT_FONT   xfMsg;
	xfMsg.flags     = msg.fontStyle;

	if ( isHistoryMsg )
	{
		if(nUnReadNum>0)///未读消息深色辨识
		{
			xfMsg.fontColor=msg.fontColor;
		}
		else
		{
		    xfMsg.fontColor=RGB(122,122,122);
		}
	}
	else
		xfMsg.fontColor=msg.fontColor;

	sprintf(xfMsg.fontName,msg.fontName);
	xfMsg.fontSize  = msg.fontSize;

	CStringList sl;

	//m_reditHistory.SetStartIndent(300);
	RichEdit_SetStartIndent(pTextServicesHistory, 300);
	
	BOOL bIsFileTran = FALSE;
	if(strData.Find("文件 \"") != -1 && strData.Find("\" 接收完成。") != -1)
		bIsFileTran = TRUE;
	if(bIsFileTran)
		xfMsg.fontColor=RGB(0,0,255);

	CString strUrl = msg.data;
	int nCount = 0;
	//m_reditHistory.InsertText(msg.data,!isHistoryMsg,&sl);
	while( strUrl.Find(STORE_URL) != -1 || strUrl.Find(ITEM_URL) != -1 )
	{
		CString strText;
		if(strUrl.Find(STORE_URL) == -1)
			strText = strUrl.Left(strUrl.Find(ITEM_URL));
		else if(strUrl.Find(ITEM_URL) == -1)
			strText = strUrl.Left(strUrl.Find(STORE_URL));
		else if(strUrl.Find(STORE_URL) < strUrl.Find(ITEM_URL))
			strText = strUrl.Left(strUrl.Find(STORE_URL));
		else
			strText = strUrl.Left(strUrl.Find(ITEM_URL));

		if((strText == "" && nCount != 0) || (strText != ""))
		{
			long lStartChar = 0, lEndChar = 0;
			m_reditHistory->SetSel(-1,-1);
			RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);

			m_reditHistory->SetSel(-1,-1);
			//RichEdit_SetFont(pTextServicesHistory, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
			RichEdit_InsertText(pTextServicesHistory, strText, m_reditHistory, !isHistoryMsg, &sl, GetHWND());
			
			m_reditHistory->SetSel(lStartChar,-1);
			if(!isHistoryMsg)
				RichEdit_SetFont(pTextServicesHistory, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
			else
				RichEdit_SetFont(pTextServicesHistory, g_ImFont.fontName, g_ImFont.fontSize, xfMsg.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
			m_reditHistory->SetSel(-1,-1);

			m_reditHistory->InsertText(-1, "\n");
		}

		strUrl = strUrl.Right(strUrl.GetLength() - strText.GetLength());

		long lStartChar = 0, lEndChar = 0;
		m_reditHistory->SetSel(-1,-1);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);

		int nLeft = 0;
		if(strUrl.Find(ITEM_URL) != -1 && (strUrl.Find(ITEM_URL) < strUrl.Find(STORE_URL) || strUrl.Find(STORE_URL) == -1))
		{
			CString strUrlSize = ITEM_URL;
			CString strItemRight = strUrl.Right(strUrl.GetLength() - strUrlSize.GetLength() - strUrl.Find(ITEM_URL));
			CString strItem = "";
			while(strItemRight.GetAt(0) >= 0x30 && strItemRight.GetAt(0) <= 0x39)
			{
				strItem += strItemRight.GetAt(0);
				strItemRight.Delete(0);
			}
			nLeft = strUrl.Find(ITEM_URL) + strUrlSize.GetLength() + strItem.GetLength();
		}
		else if(strUrl.Find(STORE_URL) != -1 && (strUrl.Find(STORE_URL) < strUrl.Find(ITEM_URL) || strUrl.Find(ITEM_URL) == -1))
			nLeft = strUrl.Find(".html") + 5;

		CString strUrlNew;
		strUrlNew = strUrl.Left(nLeft);
		m_reditHistory->InsertText(-1, strUrlNew);

		lEndChar = lStartChar + _tcslen(strUrlNew);
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);

		XT_FONT xf;
		RichEdit_SetFont(pTextServicesHistory, /*xf*/g_ImFont.fontName, /*xf*/g_ImFont.fontSize, /*xf*/g_ImFont.fontColor, /*xf*/g_ImFont.flags & XTF_BOLD, /*xf*/g_ImFont.flags & XTF_ITALIC, /*xf*/g_ImFont.flags & XTF_UNDERLINE, true);
		m_reditHistory->SetSel(-1,-1);

		RichEdit_SetFont(pTextServicesHistory, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
		m_reditHistory->GetSel(cr);
		m_reditHistory->InsertText(-1, "\n");

		RichEdit_InsertText(pTextServicesHistory, strUrlNew, m_reditHistory, !isHistoryMsg, &sl, GetHWND());
		m_reditHistory->InsertText(-1, "\n");

		strUrl = strUrl.Right(strUrl.GetLength() - strUrlNew.GetLength());
		nCount++;
	}
	RichEdit_InsertText(pTextServicesHistory, /*msg.data*/strUrl, m_reditHistory, !isHistoryMsg, &sl, GetHWND());

	m_reditHistory->SetSel(cr.cpMax,-1);
	RichEdit_SetFont(pTextServicesHistory, xfMsg.fontName, xfMsg.fontSize, xfMsg.fontColor, xfMsg.flags & XTF_BOLD, xfMsg.flags & XTF_ITALIC, xfMsg.flags & XTF_UNDERLINE, false);

	if(bIsFileTran)
		AddFileOpenTip();

	// 离线消息图片不显示等待下载动画
	if(msg.msg_type != 10 && msg.msg_type != 11)
	{
		for ( int i=0; i<sl.GetCount(); i++ )
		{
			tagWaitImageFileStruct wi;
			wi.time_recv = GetTickCount();
			sprintf(wi.szFile,sl.GetAt(sl.FindIndex(i)));
			string strFile = wi.szFile;
			if(strFile.find("http") != string::npos)
			{
				wi.type = 1;
			}
			m_vecWaitImageFiles.push_back(wi);
		}
	}


	//m_reditHistory.SetSel(cr.cpMax,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	m_reditHistory->SetSel(-1,-1);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);


	if ( m_vecRecords.size()>50 )
	{
		int cp = m_vecRecords[0];

		for ( int i=1; i<m_vecRecords.size(); i++ )
			m_vecRecords[i]-=cp;

		//m_reditHistory.SetSel(0,cp);
		//m_reditHistory.ReplaceSel("");
		//m_reditHistory.SetSel(-1,-1);
		m_reditHistory->SetSel(0,cp);
		//m_reditHistory->ReplaceSel("");
		m_reditHistory->SetSel(-1,-1);

		m_vecRecords.erase(m_vecRecords.begin());
	}

	//m_reditHistory.ScrollToBottom();
	m_reditHistory->EndDown();
}

void CDuiP2PSendMsgDlg::OnBnClickedBtnSend()
{
	CRect rcWnd;
	//GetWindowRect(m_reditHistory.m_hWnd, &rcWnd);
	POINT pt;
	pt.x = rcWnd.left-1;
	pt.y = rcWnd.top-1;

	//断线暂时不提醒此消息
	//if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	//{
	//	CTopBarDlg::Show(GetHWND(), "对不起，您当前为离线状态，无法发送信息！", pt, rcWnd.Width()+2);
	//	return;
	//}

	XT_MSG msg;
	SF_InitXtMsg(msg);
#ifdef _CS_PLAT_EX_
	if(m_bIsInsertMsg)
		msg.data_type   = XT_MSG::IM_MSG_INSERT;//插入会话消息
	else
#endif
		msg.data_type   = XT_MSG::IM_MSG;//即时聊天消息

	CString text;
	IMAGE_FILE_LIST il;
	IMAGE_FILE_LIST::iterator it;

	//m_reditSend.GetText(text,&il);
	//m_reditSend.ClearEmotionPicData();
	string strText;
	ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
	RichEdit_GetText(pTextServicesSend, strText, &il);

	text = strText.c_str();

	//CString strLog;
	//strLog.Format("发送消息:%s", text);
	//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);
	
	if ( text.IsEmpty() )
	{
		CTopBarDlg::Show(GetHWND(), "消息不能为空", pt, rcWnd.Width()+2);
		return ;
	}

	if ( text.GetLength()>800 )
	{
		//MessageBox("最多输入900个英文字符或450个汉字!");
		//CMessageBox::Prompt("无法输入太多汉字!");
		CTopBarDlg::Show(GetHWND(), "你发送消息内容超长，请分条发送。", pt, rcWnd.Width()+2);
		return ;
	}

	if ( FindForbiddenWords(text) )
	{
		//m_reditSend.ClearMsg();
		//m_reditSend.SetFocus();
		//m_reditSend->SetText("");
		m_reditSend->SetSel(0, -1);
		m_reditSend->ReplaceSel("", false);
		SetEditFocus();
		CTopBarDlg::Show(GetHWND(), "你发送的消息中有限制用语，消息发送失败！", pt, rcWnd.Width()+2);
		return ;
	}

	if(m_FriendInfo.status == XTREAM_OFFLINE && CMainFrame::IsBuyer(m_FriendInfo.GetId()))
	{	// 对方为买家，并且离线，弹出提示
		//CTopBarDlg::Show(GetHWND(), "买家不在线，可能接收不到您发出的消息。", pt, rcWnd.Width()+2);
	}

	if ( text.GetLength()==0 )
	{
		//m_reditSend.SetFocus();
		SetEditFocus();
		return ;
	}
	

	strcpy(msg.data,text);
	msg.data_len=strlen(text)+1;



	// 检测消息中是否有定义信息，并作相应转化
	//CString sShowMsg;
	//ProcTalkMsg(msg.data,sShowMsg);
	XT_MSG msgShow = msg;
	//modify by amin
	//strncpy(msgShow.data,sShowMsg,sizeof(msgShow.data)-1);
	AddTalkMsg(msgShow,false);

	//添加"群发"字样
	if(m_pmP2PMode == pmMultiTalk)
	{
		text = "【群发】 "  + text;
		strcpy(msg.data,text);
		msg.data_len=strlen(text)+1;
	}

	if ( m_pmP2PMode != pmMultiTalk )
	{
		int  type = CIMProtocol::SMT_SERVER;
		FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
		if(pInfo == NULL)
		{
			type = CIMProtocol::SMT_SERVER;
		}
		// 如果没有获取过好友状态，则获取
		else if( pInfo->ip == 0)
		{	
			//if( pInfo->status != XTREAM_OFFLINE  )
			//{
			// 在允许的次数内重试获取好友最新的ip
			if(pInfo->p2pRetry > 0)
			{
				pInfo->connFlag = FRIEND_INFO::NONE;
				pInfo->p2pRetry--;
				// 获取当前好友的ip和端口
				RefreshFriendStatus();
			}
			else
			{
				// 无法重试了，则使用服务器中转
				pInfo->connFlag = FRIEND_INFO::SERVER;
			}
			//}
			// 通过服务器中转
			type = CIMProtocol::SMT_SERVER;
		}
		else 
		{
			// 好友状态已经获取，可以开始p2p测试
			if( pInfo->connFlag == FRIEND_INFO::NONE ||
				pInfo->connFlag == FRIEND_INFO::P2P_FAILED
				)
			{
				XT_P2PTEST_REQ req;
				// 如果公网ip相同，则可能在同一个内网，尝试内网连接
				if(pInfo->ip == g_ImUserInfo.ip)
				{
					pInfo->connFlag = FRIEND_INFO::TRY_LAN;
					req.dest_ip = pInfo->localIP;
					req.dest_port = pInfo->localPort;
					req.ipType = XT_P2PTEST_REQ::LAN;
				}
				else
				{
					pInfo->connFlag = FRIEND_INFO::TRY_P2P;
					req.dest_ip = pInfo->ip;
					req.dest_port = pInfo->port;
					req.ipType = XT_P2PTEST_REQ::P2P;	
				}
				req.id = g_ImUserInfo.GetId();
				req.toid = pInfo->GetId();
				// 发送p2p测试包
				g_pProtocol->SendP2pTestPacket(req);
			}

			// 确定数据发送方式
			//if(pInfo->connFlag == FRIEND_INFO::LAN)
			//{
			//	// 尝试内网连接
			//	type = CIMProtocol::SMT_LAN;
			//}
			//else if(pInfo->connFlag == FRIEND_INFO::P2P)
			//{
			//	//直接发送到P2P客户端
			//	type = CIMProtocol::SMT_P2P; 	
			//}
			//else
			//{
			//	type = CIMProtocol::SMT_SERVER;
			//}
			//数据发送方式全改为经过服务器
			type = CIMProtocol::SMT_SERVER;
		}

#ifdef _MSG_JSON_
		if(msg.data_type == XT_MSG::IM_MSG)
		{
			Json::FastWriter jFastWrite;
			Json::Value write;
			write["pid"] = 10086;
			write["txt"] = (LPTSTR)(LPCTSTR)text;
			std::string strSendData = jFastWrite.write(write);

			strcpy(msg.data,strSendData.c_str());
			msg.data_len = strSendData.size();
		}
#endif
		
		SF_SetTransMode(type);
//#ifdef _CS_PLAT_EX_
//		if(m_bIsInsertMsg)
//		{
//			XT_MONITOR_INSERT_CHAT_REQ req;
//			req.idMaster = g_ImUserInfo.GetId();
//			strcpy(req.nicknameMaster, g_ImUserInfo.nickname);
//			req.idSeller = m_FriendInfo.GetId();
//			req.idBuyer = m_nIdMonitorBuyer;
//			req.action = 1;
//			req.msg = msg;
//			g_pProtocol->SendReq(req);
//			return;
//		}
//#endif
		if ( il.size()>0 )
		{	// 包含图片时，待所有图片传输到图片空间后再发出去
			/*int nPackets = 5/il.size() + 1;

			std::tr1::shared_ptr<XT_MSG> pMsg = tr1::shared_ptr<XT_MSG>(new XT_MSG(msg));

			for ( it=il.begin(); it!=il.end(); it++ )
			{
			SF_SendPfileReq( *it, nPackets, pMsg, type );
			}*/
			CString strData = msg.data;
			for ( it=il.begin(); it!=il.end(); it++ )
			{
				CString strPicFile = g_LocalData.GetImageFileCachePath() + it->file_name;
				string strUrl;
				if(upLoadPic(strPicFile, strUrl))
				{	// 上传成功，替换url
					strData.Replace(it->file_name, strUrl.c_str());	
				}
				else
				{	// 上传失败，替换为错误图片
					strData.Replace(it->file_name, "noimage");					
				}
			}
			if(strData.GetLength() > 800)
			{
				CTopBarDlg::Show(GetHWND(), "你发送消息内容超长，请分条发送。", pt, rcWnd.Width()+2);
				return;
			}
			else
			{
				strcpy(msg.data, strData);
				msg.data_len = strData.GetLength();
				if(msg.data_type == XT_MSG::IM_MSG_INSERT)
					g_pProtocol->SendP2PMsg(msg,type, 0, m_nInsertBuyerId);
				else
					g_pProtocol->SendP2PMsg(msg,type);
			}
		}
		else
		{
			if(msg.data_type == XT_MSG::IM_MSG_INSERT)
				g_pProtocol->SendP2PMsg(msg,type, 0, m_nInsertBuyerId);
			else
				g_pProtocol->SendP2PMsg(msg,type);
		}

		strcpy(msgShow.from_nickname,g_ImUserInfo.nickname);
		g_LocalData.SaveTalkMsg(msgShow,m_FriendInfo.GetId());
		CMainFrame::GetInstance()->GetRecentList()->AddUser(&m_FriendInfo);
		RECENTLIST_OP(AddUser(&m_FriendInfo))
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL)
		{
			FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
			CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pInfo, true, false, false, false, false);
		}
#else
		if(CDuiCounsel::GetInstance() != NULL)
		{
			FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
			CDuiCounsel::GetInstance()->SetFriendListElement(pInfo, true, false, false, false, false);
		}
#endif
		///聊天记录实时刷新
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版退出时更新最近联系人到后台
		if (g_ImUserInfo.status!=XTREAM_OFFLINE/* && !m_bHasUpdateNearCommCust*/)
		{
			// 统计需要更新到后台的最近联系客户信息
			CMainFrame::GetInstance()->StatNeedUpdateCustInfo(&m_FriendInfo);
			// 更新最近联系客户信息到后台
			CMainFrame::GetInstance()->UpdateCustInfoToServer();

			//return;
		}	
#endif	

	}
	else
	{
		g_pProtocol->SendClusterMsg(msg);

		// 发送消息的同时发送聊天信息中包含的图片
		if ( il.size()>0 )
		{
			//多人聊天
			int nPackets = 5/il.size() + 1;

			for ( it=il.begin(); it!=il.end(); it++ )
			{
				SF_StartUploadCfileReq( *it,m_clusterInfo.nFamilyID, nPackets );
			}
		}

		SaveClusterMsg(msgShow);
	}

	il.clear();
	//RegUserOperation(0x00c4);

	//m_reditSend.ClearMsg();
	//m_reditSend.SetFocus();
	//m_reditSend->SetText("");
	m_reditSend->SetSel(0, -1);
	m_reditSend->ReplaceSel("", false);
	SetEditFocus();

	////ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
	//RichEdit_SetFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
	//RichEdit_SetDefFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);

	//快捷短语发送之后字体还原
	if(m_bIsFontLast)
	{
		g_ImFont = m_fontLast;
		m_bIsFontLast = FALSE;
		//m_reditSend.SetXFont(g_ImFont);
		//ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
		RichEdit_SetDefFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
	}
}

void CDuiP2PSendMsgDlg::OnBnClickedBtnSendEx()
{
	BOOL bPtInText = m_btnSend.PressedInText();

	if(bPtInText)
	{
		OnBnClickedBtnSend();
	}
	else
	{
		OnBnClickedBtnHotkey();
	}
}

void CDuiP2PSendMsgDlg::OnHelpClick()
{
	if ( this->m_pmP2PMode!=pmMultiTalk )
	{
		OpenHtmlPage("www.feiniu.com");
	}
	else
	{
		OpenHtmlPage("www.feiniu.com");
	}
}

// BOOL CDuiP2PSendMsgDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
// {
// 	// TODO: 在此添加专用代码和/或调用基类
// 	if ( wParam==IDC_SPLITTER1 )
// 	{
// 		SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
// 
// 		m_nSplitterPos-=pHdr->delta;
// 
// 		AdjustUI();
// 
// 		m_reditHistory.ScrollToBottom();
// 	}
// 
// 	if ( wParam==IDC_FILETRANS_PROGRESS )
// 	{
// 		int nItem = m_wndP2PFileTransProgressBar.GetCurSel();
// 
// 		DWORD dwUser;
// 		BOOL  bSendFile;
// 
// 		if ( m_wndP2PFileTransProgressBar.GetItemInfo(nItem,dwUser,bSendFile) )
// 		{
// 			uint32 id;
// 			id = dwUser;
// 			//id.uid = m_FriendInfo.id;
// 
// 			if ( bSendFile )
// 			{
// 				AF_AbortSendFile(id);
// 				m_reditHistory.SetFileBarTip(id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));
// 			}
// 			else
// 			{
// 
// 				CP2PRecvFile *pFile = AF_FindRecvFile(id);
// 
// 				// 现在暂时不实现断点续传功能
// 				/*if ( pFile )
// 					pFile->SaveFilePosition(m_FriendInfo.GetId());*/
// 
// 				AF_AbortRecvFile(id);
// 				m_reditHistory.SetFileBarTip(id,ODT_RECV_FILE,"您取消了接收该文件",RGB(255,0,0));
// 			}
// 
// 			m_wndP2PFileTransProgressBar.DeleteItem(nItem);
// 			if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
// 				ShowFileTransInfoBar(false);
// 
// 			return FALSE;
// 		}
// 	}
// 
// 	return CXSkinDlg::OnNotify(wParam, lParam, pResult);
// }

void CDuiP2PSendMsgDlg::OnBnClickedBtnHistory()
{
	std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
	pDlg->SetDefaultFriend(m_FriendInfo.GetId());
}

LRESULT CDuiP2PSendMsgDlg::OnMagicFaceClick(WPARAM wParam, LPARAM lParam)
{
	ShowMagicFace((char *)lParam);
	return 0;
}

LRESULT CDuiP2PSendMsgDlg::OnQueryPeerOnline(WPARAM wParam, LPARAM lParam)
{
	bool * pResult = (bool *)lParam;

	if ( m_pmP2PMode!=pmMultiTalk )
	{
		if ( m_FriendInfo.status==XTREAM_OFFLINE )
		{
			*pResult = false;
		}
		else
		{
			*pResult = true;
		}
	}
	else
	{
		*pResult=true;
	}

	return 0;
}

LRESULT CDuiP2PSendMsgDlg::OnMagicFaceNotify(WPARAM wParam, LPARAM lParam)
{
	const char * face_name = (LPCSTR)lParam;
	const char * face_url  = (LPCSTR)wParam;

	XT_MSG msg;
	SF_InitXtMsg(msg);

	msg.data_type   = 9;//魔法表情

	CSendPacket spMsg(msg.data);
	spMsg<<face_name<<face_url;
	msg.data_len = spMsg.GetSize();
	int type;
	if ( m_pmP2PMode!=pmMultiTalk )
	{

		type = CIMProtocol::SMT_SERVER;
		FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
		//ASSERT(pInfo);
		if(pInfo == NULL)
		{
			type = CIMProtocol::SMT_SERVER;;
		}
		// 如果没有获取过好友状态，则获取
		else if( pInfo->ip == 0)
		{	
			// 在允许的次数内重试获取好友最新的ip
			if(pInfo->p2pRetry > 0)
			{
				pInfo->connFlag = FRIEND_INFO::NONE;
				pInfo->p2pRetry--;
				// 获取当前好友的ip和端口
				RefreshFriendStatus();
			}
			else
			{
				// 无法重试了，则使用服务器中转
				pInfo->connFlag = FRIEND_INFO::SERVER;
			}
			type = CIMProtocol::SMT_SERVER;
		}
		else 
		{
			// 好友状态已经获取，可以开始p2p测试
			if( pInfo->connFlag == FRIEND_INFO::NONE ||
				pInfo->connFlag == FRIEND_INFO::P2P_FAILED
				)
			{
				XT_P2PTEST_REQ req;
				// 如果公网ip相同，则可能在同一个内网，尝试内网连接
				if(pInfo->ip == g_ImUserInfo.ip)
				{
					pInfo->connFlag = FRIEND_INFO::TRY_LAN;
					req.dest_ip = pInfo->localIP;
					req.dest_port = pInfo->localPort;
					req.ipType = XT_P2PTEST_REQ::LAN;
				}
				else
				{
					pInfo->connFlag = FRIEND_INFO::TRY_P2P;
					req.dest_ip = pInfo->ip;
					req.dest_port = pInfo->port;
					req.ipType = XT_P2PTEST_REQ::P2P;	
				}
				req.id = g_ImUserInfo.GetId();
				req.toid = pInfo->GetId();
				// 发送p2p测试包
				g_pProtocol->SendP2pTestPacket(req);
			}

			// 确定数据发送方式
//			if(pInfo->connFlag == FRIEND_INFO::LAN)
//			{
//				// 尝试内网连接
//				type = CIMProtocol::SMT_LAN;
//			}
//			else if(pInfo->connFlag == FRIEND_INFO::P2P)
//			{
//				//直接发送到P2P客户端
//				type = CIMProtocol::SMT_P2P; 
//#ifdef _DEBUG
//				int i = 1000;
//				//while(i-- > 0)
//				//{
//				//	g_pProtocol->SendP2PMsg(msg,type);
//				//}
//#endif
//			}
//			else
//			{
//				type = CIMProtocol::SMT_SERVER;
//			}
			//数据发送方式全改为经过服务器
			type = CIMProtocol::SMT_SERVER;
		}		

		g_pProtocol->SendP2PMsg(msg,type);		
		SF_SetTransMode(type);
	}
	else
	{
		g_pProtocol->SendClusterMsg(msg);
	}

	CString strTip;
	strTip.Format("魔法表情 \"%s\" 已经发出",face_name);
	AddMsgTip(strTip);

	if (this->m_pmP2PMode!=pmMultiTalk )
	{
		CMainFrame::GetInstance()->GetRecentList()->AddUser(&m_FriendInfo);
		RECENTLIST_OP(AddUser(&m_FriendInfo))
	}

	if ( m_mapMagicFace.find(CString(face_name))==m_mapMagicFace.end() )
		m_mapMagicFace.insert(map<CString,CString>::value_type(face_name,face_url));

	ShowMagicFace(face_name);

	return 0;
}

LRESULT CDuiP2PSendMsgDlg::OnEmotionSelNotify(WPARAM wParam,LPARAM lParam)
{
	CUSTOM_EMOTION * pFace = (CUSTOM_EMOTION * )wParam;
	//m_reditSend.InsertFace(pFace,(int)lParam);
	ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
	Richedit_InsertEmotion(pTextServicesSend, m_reditSend, (int)(lParam + 1));

	//switch( pFace->type)
	//{
	//// 系统内置
	//case CUSTOM_EMOTION::SYS:
	//	m_reditSend.InsertEmotion((int)lParam);
	//	break;
	//// 系统gif
	//case CUSTOM_EMOTION::SYS_GIF:
	//	m_reditSend.InsertGifEmotion((int)lParam);
	//	break;
	//// 自定义表情
	//case CUSTOM_EMOTION::CUSTOM:
	//	{
	//		CString strCustomFaceFile = g_LocalData.GetCustomFacePath()+pFace->szFile;
	//		CString strCacheFile = g_LocalData.GetImageFileCachePath()+pFace->szFile;
	//		// 先判断缓存文件夹中有没有该表情
	//		if ( _access((LPCSTR)strCacheFile,0)==-1 )
	//			CopyFile(strCustomFaceFile,strCacheFile,FALSE);
	//		// 显示缓存中的图片文件
	//		m_reditSend.InsertImage(strCacheFile,EMOTION_PIC_DATA());
	//		
	//		break;
	//	}
	//case CUSTOM_EMOTION::WEB:
	//	{
	//		CString strCustomFaceFile = g_LocalData.GetCustomFacePath()+pFace->szFile;
	//		CString strCacheFile = g_LocalData.GetImageFileCachePath()+pFace->szFile;
	//		// 先判断缓存文件夹中有没有该表情
	//		if ( _access((LPCSTR)strCacheFile,0)==-1 )
	//			CopyFile(strCustomFaceFile,strCacheFile,FALSE);
	//		// 如果url为空，则发送本地图片
	//		if(strcmp(pFace->szUrl,"")==0)
	//		{
	//			// 显示缓存中的图片文件
	//			m_reditSend.InsertImage(strCacheFile,EMOTION_PIC_DATA());
	//			
	//		}
	//		else
	//		{
	//			EMOTION_PIC_DATA data;
	//			data.nType = EPT_WEB;
	//			data.sData = pFace->szUrl;
	//			m_reditSend.InsertImage(strCacheFile,data);
	//			
	//		}
	//    break;
	//	}
	//default:
	//    break;
	//}

	//m_reditSend.SetFocus();
	SetEditFocus();
	return 1;
}

void CDuiP2PSendMsgDlg::OnFinalMessage(HWND hWnd)
{
#ifdef _CS_PLAT_EX_
	if(CMainFrame::IsBuyer(m_FriendInfo.GetId()) && !m_bIsTran)
	{
		// 向服务端发送已接待好友请求
		XT_RECEPTE_CUST_REQ req;
		//int nBuyerID = CDuiCSPlatDlg::GetFirstWaitBuyer();
		//if(nBuyerID >= 0)
		{
			req.idBuyer = (uint32)m_FriendInfo.GetId()/*nBuyerID*/;
			req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_QUIT;	// 主动接待
			req.teamId = (uint32)100;
			g_pProtocol->SendReq(req);

			//CDuiCSPlatDlg::DelWaitFriend(nBuyerID);
		}
	}
#endif

	g_pProtocol->UnRegistCmd(hWnd);
	m_wndUserList.DestroyWindow();
	//m_pwndUserPanel->DestroyWindow();
#ifdef _VER_INTERNAL_
	if(m_pInterNalFriendInfo != NULL)
		m_pInterNalFriendInfo->DestroyWindow();
#endif
	if(m_hAccel)
	{
		DestroyAcceleratorTable(m_hAccel);
	}
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);

	::PostMessage(g_pMessageHandleWnd->m_hWnd, WM_P2PSENDMSGDLG_NOTIFY, (WPARAM)m_pmP2PMode, (LPARAM)&m_FriendInfo);
}

void CDuiP2PSendMsgDlg::ShowDlgTitle()
{
	CString str;

	if ( m_pmP2PMode!=pmMultiTalk || m_clusterInfo.Count()==0 )
	{
		str.Format("与 %s(%s) 聊天中",UserInfoToStr(&m_FriendInfo),StatusToStr(m_FriendInfo.status));
	}
	else
	{
		str.Format("%s 与 %d 人对话中",
			UserInfoToStr(&g_ImUserInfo),
			m_clusterInfo.Count()-1);
	}
}

//显示历史5条记录
void CDuiP2PSendMsgDlg::LoadLastTalkMsg(int nUnReadNum)
{
	MSG_LIST ml1;
	// 获取用户聊天记录
	if(nUnReadNum>0)
	{
	   g_LocalData.GetLastMsgList(m_FriendInfo.GetId(),CDataManager::USER,ml1,nUnReadNum+5);///获取聊天记录
	}
	else
	{
	   g_LocalData.GetLastMsgList(m_FriendInfo.GetId(),CDataManager::USER,ml1,5);//获取历史记录
	}

	POSITION pos=ml1.GetTailPosition();

	MSG_LIST ml2;

	int count=0;
	int nTotal= 0;
	nTotal = (nUnReadNum > 0)?nUnReadNum+5:5;
	while (count++<nTotal)
	{		
		if ( pos )
		{
			XT_MSG msg = ml1.GetAt(pos);
			ml2.AddHead(msg);
			ml1.GetPrev(pos);
		}
		else
		{
			break;
		}
	}
	//历史消息条数
	int nHistory = ml2.GetCount() - nUnReadNum;
	for ( int i=0; i<ml2.GetCount(); i++)
	{
		pos = ml2.FindIndex(i);
		if(i < nHistory)
		{
			AddTalkMsg(ml2.GetAt(pos),true);
			if(i == nHistory - 1)//历史消息后面加上横线
			{
				//m_reditHistory.InsertText("\n");
				//m_reditHistory.SetStartIndent(80);
				//m_reditHistory.InsertLine();
				//m_reditHistory.InsertText("\n");

				m_reditHistory->InsertText(-1, "\n");
				ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
				RichEdit_SetStartIndent(pTextServicesHistory, 80);
				RichEdit_InsertLine(pTextServicesHistory);
				m_reditHistory->InsertText(-1, "\n");
			}
		}
		else
		{
		    AddTalkMsg(ml2.GetAt(pos),false,nUnReadNum);
		}
	}
	SetTimer(GetHWND(), TIMER_EDIT_ENDDOWN, 100, NULL);
}
//好友状态更改通知
void CDuiP2PSendMsgDlg::OnFriendStatusChange(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;
		//如果是隐身的话，则状态图标和离开一样
		if ( ind->status==XTREAM_HIDE )
		{
			ind->status=XTREAM_OFFLINE;
		}

		if ( m_pmP2PMode!=pmMultiTalk)
		{
			if ( m_FriendInfo.GetId()==ind->fid )
			{
				m_FriendInfo.ver  = ind->version;
				m_FriendInfo.status = ind->status;

				//更新头像
				CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&m_FriendInfo);
				CString strGifFile = "";
				CString strPath =  m_FriendInfo.face_url;
				if(strPath.Find(".gif")!=-1)
				{
			        strGifFile =  strFaceFile +".gif";////GIF文件
					if ( !FileExist(strGifFile) )////不存在则创建
					{
						CopyFile(strFaceFile,strGifFile,FALSE);
					}
				}
				if ( FileExist(strFaceFile) )
				{
// 					m_ctrlFaceIcon.SetPicPath(strFaceFile, m_FriendInfo.status == XTREAM_ONLINE 
// 						|| m_FriendInfo.status == XTREAM_BUSY
// 						|| m_FriendInfo.status == XTREAM_LEAVE);

					BOOL bIsGray = FALSE;
					if(m_FriendInfo.status == XTREAM_HIDE || m_FriendInfo.status == XTREAM_OFFLINE)
					{
						bIsGray = TRUE;
					}
					string strImg = CutRoundImg(strFaceFile, 70, 70, bIsGray);
					strFaceFile = strImg.c_str();
					if ( /*m_FriendInfo.status!=XTREAM_OFFLINE &&*/ FileExist(strFaceFile) )
					{
						if ( FileExist(strGifFile) )
						{
							if(bIsGray)///离线
							{
								LPCSTR szHead = (LPSTR)(LPCSTR)strFaceFile;
								CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
								pVLhead->SetBkImage(szHead);
								m_pHead->SetBkImage("");
								m_pHead->SetNormalGifFile("");
							}
							else//在线
							{
								CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
								pVLhead->SetBkImage("");
								m_pHead->SetBkImage("");
								m_pHead->SetNormalGifFile(strGifFile);
							}
						}
						else
						{
							LPCSTR szHead = (LPSTR)(LPCSTR)strFaceFile;//GetPicPath(strFaceFile, TRUE);
							/*CButtonUI* pBtnHead = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnhead")));
							pBtnHead->SetBkImage("P2PSendMsgDlg\\icon_70x70.png");*/
							CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
							pVLhead->SetBkImage(szHead);
							m_pHead->SetBkImage("");
							m_pHead->SetNormalGifFile("");
						}
					}
				}

				//更新状态和分组
				//SetDlgItemText(IDC_STATIC_STATUS_GROUP, "[" + StatusToStr(m_FriendInfo.status) + "]" + "（" + m_strGroupName +"）") ;

				//ShowDlgTitle();
				CString str;
				str.Format("与 %s(%s) 聊天中",UserInfoToStr(&m_FriendInfo),StatusToStr(m_FriendInfo.status));
				SetWindowText(GetHWND(), str);

				FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId()) != NULL ? ::GetFriendInfo(m_FriendInfo.GetId()) : &m_FriendInfo;
#ifdef _CS_PLAT_EX_
				if(CDuiCSPlatDlg::GetInstance() != NULL)
					CDuiCSPlatDlg::GetInstance()->UpdateFriendListElement(pInfo);
#else
				if(CDuiCounsel::GetInstance() != NULL)
					CDuiCounsel::GetInstance()->UpdateFriendListElement(pInfo);
#endif

				//m_pwndUserPanel->UpdateFriendInfo();
			}
		}
		else
		{
			FRIEND_INFO *pFriend = m_clusterInfo.GetFriendInfo(ind->fid);

			if ( pFriend )
				pFriend->status=ind->status;

			if ( m_pmP2PMode==pmMultiTalk )
				m_wndUserList.UpdateClusterList();
		}
	}
}


void CDuiP2PSendMsgDlg::OnFriendOffLine(const char *data,WORD wRet)
{
	LPXT_STATUS_IND ind = (LPXT_STATUS_IND)data;
	if(ind->fid == m_FriendInfo.GetId())
	{
		FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId()) != NULL ? ::GetFriendInfo(m_FriendInfo.GetId()) : &m_FriendInfo;
#ifdef _CS_PLAT_EX_
		m_bCanTran = FALSE;
		XT_FONT xf;
		xf.fontColor=RGB(254,154,105);

		//m_reditHistory.SetSel(-1,-1);
		//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
		//m_reditHistory.SetStartIndent(80);
		//m_reditHistory.InsertText("\n----买家已退出!\n");

		//m_reditHistory.ScrollToBottom();

		m_reditHistory->SetSel(-1,-1);
		ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
		m_reditHistory->InsertText(-1, "\n");
		RichEdit_SetStartIndent(pTextServicesHistory, 80);
		m_reditHistory->InsertText(-1, "----买家已退出!\n");
		m_reditHistory->EndDown();

		if(CDuiCSPlatDlg::GetInstance() != NULL)
			CDuiCSPlatDlg::GetInstance()->UpdateFriendListElement(pInfo);
#else
		if(CDuiCounsel::GetInstance() != NULL)
			CDuiCounsel::GetInstance()->UpdateFriendListElement(pInfo);
#endif
	}
}

void CDuiP2PSendMsgDlg::ChangeHeadStatus(FRIEND_INFO *pFriend)
{
	//SetDlgItemText(IDC_STATIC_STATUS_GROUP, "[" + StatusToStr(pFriend->status) + "]" + "（" + m_strGroupName +"）") ;
}


void CDuiP2PSendMsgDlg::DrawControlLine()
{
// 	if ( m_reditHistory.m_hWnd==NULL )
// 	{
// 		return ;
// 	}
// 
// 	CDC *pDC = GetDC();
// 
// 	if ( pDC==NULL )
// 	{
// 		return ;
// 	}
// 
// 	int nSavedDC = pDC->SaveDC();
// 
// 	CRect rcHistory, rcSend;
// 
// 	//m_captionBar.GetWindowRect(&rcCaption);
// 	//ScreenToClient(&rcCaption);
// 	m_reditHistory.GetWindowRect(&rcHistory);
// 	ScreenToClient(&rcHistory);
// 
// 	m_reditSend.GetWindowRect(&rcSend);
// 	ScreenToClient(&rcSend);
// 
// 	//rcCaption.InflateRect(1,1);
// 	rcHistory.InflateRect(1,1);
// 	rcSend.InflateRect(1,1);
// 	rcSend.top-=25;
// 
// 	//pDC->Draw3dRect(&rcHistory,0xCF4705,0xCF4705);
// 	COLORREF clrLine = 0xCF4705;
// 
// 	CPen pen(PS_SOLID, 1, clrLine);
// 
// 	pDC->SelectObject(&pen);
// 
// 	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
// 	pDC->SelectObject(pBrush);
// 
// 	//int y_point = rcCaption.top+rcCaption.Height()/2;
// 
// 	//	pDC->MoveTo(rcCaption.left,y_point);
// 	//	pDC->LineTo(rcHistory.left,rcHistory.bottom-1);
// 	//	pDC->LineTo(rcHistory.right-1,rcHistory.bottom-1);
// 	//	pDC->LineTo(rcHistory.right-1,y_point);
// 
// 	pDC->Draw3dRect(&rcSend,clrLine,clrLine);
// 	pDC->Draw3dRect(&rcHistory,clrLine,clrLine);
// 
// 	// CRgn rgn;
// 	//rgn.CreateRectRgn(rcCaption.left,rcCaption.top,
// 	//	rcCaption.right+1, y_point+1 );
// 
// 	//pDC->SelectObject(&rgn);
// 	//CPoint pt(16,16);
// 
// 	//pDC->RoundRect(&rcCaption,pt);
// 
// 	ReleaseDC(pDC);
}

void CDuiP2PSendMsgDlg::OnSendFile()
{
	if ( m_pmP2PMode==pmMultiTalk )
	{
		ShowMessage("对不起！多人聊天不能发送文件！请先和他/她单聊，然后发送文件。谢谢！");
	}
	else
	{
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,NULL,CWnd::FromHandle(GetHWND()));
		if ( dlg.DoModal()==IDOK )
		{	
			CString strFileName = dlg.GetPathName();
			P2PSendFile(strFileName);
		}
	}
}

void CDuiP2PSendMsgDlg::OnInvite()
{
	if ( m_pmP2PMode==pmSingleTalk )
	{
		m_clusterInfo.nFamilyID=(uint32)-1;

		if (m_FriendInfo.GetId()!=0 && m_FriendInfo.status==XTREAM_OFFLINE )
		{
			AddMsgTip("对方不在线或隐身，不能发起多人聊天！",COLOR_MSGTIP);
			return ;
		}
		else if(m_FriendInfo.group_index == 1 || m_FriendInfo.group_index == 2)
		{
			AddMsgTip("当前聊天的好友为陌生人或黑名单好友，不能邀请他人聊天！",COLOR_MSGTIP);
			return;
		}

		if ( m_poolRecv.size()>0 || m_poolSend.size()>0 || m_vecSendFile.size()>0 || m_vecRecvFile.size()>0 )
		{
			AddMsgTip("当前聊天框中有未传的文件，请关闭所有的文件之后再邀请他人聊天！",COLOR_MSGTIP);
			return ;
		}
	}

	CClusterMemberSelectDlg dlg(&m_clusterInfo,CWnd::FromHandle(GetHWND()));
	dlg.SetP2pInviteFriend(&m_FriendInfo);

	if ( IDOK==dlg.DoModal() )
	{
		dlg.GetClusterInfo(m_clusterInfo);

		if ( m_pmP2PMode!=pmMultiTalk )
		{
			::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2P_TO_GROUP,0,(LPARAM)&m_FriendInfo);
		}

		m_pmP2PMode = pmMultiTalk;
		UpdateP2PPanel();

		m_wndUserList.SetClusterInfo(&m_clusterInfo);
		m_wndUserList.UpdateClusterList();

		SetBtnFolderBitmap();

		ShowDlgTitle();

		m_bClusterInit = true;
	}
}


void CDuiP2PSendMsgDlg::UpdateP2PPanel()
{
	if ( IsMinimized(*this) )
	{
		m_bUpdateP2PPanel=true;
		return;
	}
	

	switch(m_pmPreP2PMode)
	{
	case pmSingleTalk:
		break;
	case pmMultiTalk:
		m_wndUserList.ShowWindow(SW_HIDE);
		break;
	}

	switch(m_pmP2PMode)
	{
	case pmSingleTalk:
		//::ShowWindow(m_wndAD.GetSafeHwnd(), SW_SHOW);
#ifdef _VER_INTERNAL_
		if(m_pInterNalFriendInfo != NULL)
			::ShowWindow(m_pInterNalFriendInfo->GetSafeHwnd(), SW_SHOW);
#endif
		break;
	case pmMultiTalk:
		m_wndUserList.ShowWindow(SW_SHOW);
		//::ShowWindow(m_wndAD.GetSafeHwnd(), SW_HIDE);
#ifdef _VER_INTERNAL_
		if(m_pInterNalFriendInfo != NULL)
			::ShowWindow(m_pInterNalFriendInfo->GetSafeHwnd(), SW_HIDE);
#endif
		break;
	}


	if (m_bShowHistory) {
		m_wndPopupHistory.ShowWindow(SW_SHOW);
		m_wndUserList.ShowWindow(SW_HIDE);
		//::ShowWindow(m_wndAD.GetSafeHwnd(), SW_HIDE);
#ifdef _VER_INTERNAL_
		//消息记录打开时隐藏好友信息
		m_pInterNalFriendInfo->ShowWindow(SW_HIDE);
#endif
	}
	else
	{
		m_wndPopupHistory.ShowWindow(SW_HIDE);
	}

	if (m_bShowHistory != m_bPreShowHistory)
	{
		int nSize = 0;
		if (m_bShowHistory)
		{
			nSize = m_uPopupHistoryWidth - AD_WINDOW_WIDTH;
		} 
		else
		{
			nSize = AD_WINDOW_WIDTH - m_uPopupHistoryWidth;
		}
		CRect rc;
		GetWindowRect(*this, &rc);
		rc.right+=nSize;

		if ( nSize>0 )
		{
			m_nPreDlgHeight = rc.Height();
		}
		else
		{
			rc.bottom = rc.top + m_nPreDlgHeight;
		}

		MoveWindow(*this,rc.left, rc.top, rc.Width(), rc.Height(), true);
	}



	//FIXME: 临时隐藏
	m_wndSplitter1.ShowWindow(SW_HIDE);

	AdjustUI();

	m_pmPreP2PMode = m_pmP2PMode;
	m_bPreShowHistory = m_bShowHistory;
#ifdef _VER_INTERNAL_
	m_wndAD->SetVisible(false);
#endif 

}

void CDuiP2PSendMsgDlg::UpdateCluserInfo(uint32 cid)
{
	ASSERT(m_hWnd);

	m_clusterInfo.members.clear();
	m_clusterInfo.nFamilyProp = TEMP_FAMILY;
	m_clusterInfo.nFamilyID   = cid;

	XT_GET_CMEMBERS_REQ req;

	req.nFamilyID   = cid;
	req.nFamilyProp = TEMP_FAMILY;
	req.req_count     = 0;
	req.start_pos = 0;
	req.online_type = 0;//所有

	g_pProtocol->SendReq(req);
}

void CDuiP2PSendMsgDlg::OnCMemberInd(const char *data, WORD wRet)
{
	LPXT_CMEMBER_IND ind = (LPXT_CMEMBER_IND)data;
	if ( wRet==0 )
	{
		UpdateCluserInfo(ind->cluster_id);
	}
}

void CDuiP2PSendMsgDlg::OnGetClusterMember(const char *data,WORD wRet)
{
	LPXT_GET_CMEMBERS_ACK ack = (LPXT_GET_CMEMBERS_ACK)data;
	char szTemp[20];
	FRIEND_INFO * pFriend = NULL;
	FRIEND_INFO * pInfo = NULL;
	uint32 id;
	bool bExistMyself = false;

	vector<uint32>  arrGetBasicInfo;
	if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_clusterInfo.nFamilyID && ack->nFamilyProp==TEMP_FAMILY )
		{
			for ( int i=0; i<ack->return_count; i++ )
			{
				id = ack->members[i].id;
				if(id == g_ImUserInfo.GetId())
					bExistMyself = true;
				// 本地查询是否已经有该好友信息
				pInfo = ::GetFriendInfo(id);
				pFriend = m_clusterInfo.GetFriendInfo(id);
				if ( pFriend == NULL )
				{
					m_clusterInfo.members.push_back(FRIEND_INFO());
					pFriend = &m_clusterInfo.members[m_clusterInfo.Count()-1];
				}
				// 如果存在该好友信息则直接获取
				if(pInfo != NULL)
				{
					if(!pInfo->bBasicInfo)
					{
						// 需要获取基本资料
						arrGetBasicInfo.push_back(pInfo->GetId());
					}
					*pFriend = *pInfo;
				}
				else
				{
					pFriend->SetId(ack->members[i].id);
					strcpy(pFriend->nickname,itoa(pFriend->GetId(),szTemp,10));

					//// 如果没有该用户的资料则将其作为陌生人
					//FRIEND_INFO * pUser = new FRIEND_INFO(*pFriend);
					//pUser->group_index = 1;
					//g_lsFriend.push_back(pUser);
					FRIEND_INFO * pUser = new FRIEND_INFO(*pFriend);
					pUser->group_index = 1;
					g_lsFriendTemp.push_back(pUser);

					// 需要获取基本资料
					arrGetBasicInfo.push_back(pFriend->GetId());
				}
				pFriend->cluster_property = ack->members[i].prop;
				// 临时群中的成员都是在线状态
				//pFriend->status = FRIEND_INFO::ONLINE;临时群成员状态不变
			}

			m_wndUserList.SetClusterInfo(&m_clusterInfo);
			m_wndUserList.UpdateClusterList();

			if ( ack->next_pos!=0xFFFF && (ack->next_pos != 0 ))
			{
				XT_GET_CMEMBERS_REQ req;
				req.nFamilyID   = m_clusterInfo.nFamilyID;
				req.nFamilyProp = TEMP_FAMILY;
				req.req_count   = 0;
				req.start_pos = ack->next_pos;
				req.online_type = 0;//所有
				req.userstatus  = g_ImUserInfo.status;
				g_pProtocol->SendReq(req);
			}
			else
			{
				m_bClusterInit = true;
				// 如果临时群中没有自己则关闭窗体
				if(bExistMyself == false)
				{
					DestroyWindow();
					return;
				}
				XT_MSG msg;
				while ( m_fifoMsg.Pop(msg) )
				{
					AddTalkMsg(msg,false);
				}
				ShowDlgTitle();
				if ( IsWindowVisible(*this)==false )
				{
					for ( int i=0; i<m_clusterInfo.Count(); i++ )
					{
						if ( m_clusterInfo.members[i].GetId() != g_ImUserInfo.GetId() )
						{						
							m_FriendInfo = m_clusterInfo.members[i];
							break;
						}						
					}
					ShowWindow(SW_SHOW);
					FlashWindow(*this, TRUE);
				}
			}
			// 获取好友基本资料
			//CFloorPageCluster::ImGetFriendBasicInfo(arrGetBasicInfo);
		}
	}
	else
	{
		if( ack->nFamilyID==m_clusterInfo.nFamilyID && 
			ack->nFamilyProp==TEMP_FAMILY &&
			m_pmP2PMode==pmMultiTalk)
		{
			//	DestroyWindow();
		}
	}
}


void CDuiP2PSendMsgDlg::OnClusterDismissInd(const char *data,WORD wRet)
{
	LPXT_CLUSTER_DISMISS_IND ind = (LPXT_CLUSTER_DISMISS_IND)data;

	if ( wRet==0 && m_pmP2PMode==pmMultiTalk)
	{
		if ( ind->cluster_type==TEMP_FAMILY && ind->cluster_id==m_clusterInfo.nFamilyID )
		{
			DestroyWindow();
			ShowMessage("多人聊天已经撤销!");
		}
	}
}

UINT CDuiP2PSendMsgDlg::UpdateUrlThread( LPVOID pParam )
{
	CDuiP2PSendMsgDlg * pDlg = (CDuiP2PSendMsgDlg*)pParam;
	pDlg->UpdateUrl();
	return 0;
};

void CDuiP2PSendMsgDlg::OnFriendInfoGet(const char *data, WORD wRet)
{
	LPXT_USERINFO_GET_ACK ack=(LPXT_USERINFO_GET_ACK)data;

	FRIEND_INFO f;

	if ( wRet==0 )
	{
		m_bIsGetFriendInfo = TRUE;
		CString strLog = "获取好友信息应答";
		g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);
#ifdef _CS_PLAT_EX_
		if(m_nIdMonitorBuyer == ack->GetId() && m_bIsMonitor == TRUE)
		{
			UpdateUserFullInfo(&f,ack);
			// 刷新界面
			CLabelUI* pLabelFriendName =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelfriendname")));
			pLabelFriendName->SetText(/*f.nickname*/UserInfoToStr(&f));
			CLabelUI* pLabelFriendSign =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelfriendsign")));
			pLabelFriendSign->SetText(f.user_sign);
		}
#endif
		if ( m_pmP2PMode==pmMultiTalk && ack->GetId()==m_uiAddFriendId )
		{
			//memcpy(&f,ack,sizeof(XT_USER_INFO));
			UpdateUserFullInfo(&f,ack);
			FRIEND_INFO * pFriend = m_clusterInfo.GetFriendInfo(ack->GetId());
			FRIEND_INFO * pUser = ::GetFriendInfo(ack->GetId());
			if(pUser == NULL)
			{
				// 作为临时数据添加
				f.group_index = 1;
				FRIEND_INFO * pUser = new FRIEND_INFO(f);
				//g_lsFriend.push_back(pUser);
				g_lsFriendTemp.push_back(pUser);
			}
			if ( pFriend==NULL )
			{
				m_clusterInfo.members.push_back(f);
				m_wndUserList.UpdateClusterList();
				AddCMemberTip(&f,1);
				ShowDlgTitle();
			}
			else
			{
				*pFriend = f;
				m_wndUserList.UpdateClusterList();
				AddCMemberTip(&f,1);
			}

			m_uiAddFriendId = 0;
		}

		if ( m_pmP2PMode!=pmMultiTalk && ack->GetId()==m_FriendInfo.GetId() )
		{
			FRIEND_INFO *pRealFriend = FindFriend(m_FriendInfo.GetId());

			if ( pRealFriend )
			{
				memcpy(&m_FriendInfo,pRealFriend,sizeof(XT_USER_INFO));
				//m_pwndUserPanel->UpdateFriendInfo();
				if(CMainFrame::IsBuyer(m_FriendInfo.GetId()) /*&& m_FriendInfo.GetGuid() == ""*/)
				{
					string strGuid = ack->GetGuid();
					m_FriendInfo.SetGuid(ack->GetGuid().c_str());
					pRealFriend->SetGuid(ack->GetGuid().c_str());
					CString strLogGuid;
					strLogGuid.Format("从服务器获取到guid:%s", ack->GetGuid().c_str());
					g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLogGuid);
					UpdateUrl();
				}
				
				// 刷新界面
				FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
				CLabelUI* pLabelFriendName =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelfriendname")));
				pLabelFriendName->SetText(/*pInfo->nickname*/UserInfoToStr(pInfo));
				CLabelUI* pLabelFriendSign =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelfriendsign")));
				pLabelFriendSign->SetText(pInfo->user_sign);
#ifdef _CS_PLAT_EX_
				if(CDuiCSPlatDlg::GetInstance() != NULL)
					CDuiCSPlatDlg::GetInstance()->UpdateFriendListElement(pInfo);
#else
				if(CDuiCounsel::GetInstance() != NULL)
					CDuiCounsel::GetInstance()->UpdateFriendListElement(pInfo);
#endif
			}
#ifndef _CS_PLAT_EX_
			//请求右侧备注
			m_strNewComnCustRemarkServer = "";
			if(CMainFrame::IsBuyer(m_FriendInfo.GetId()))
			{
				XT_GET_NEAR_COMM_CUST_REMARK_REQ req;
				req.uiBusinessID = g_ImUserInfo.GetId();
				strncpy(req.szCommAccount, m_FriendInfo.GetUserName().c_str(), 31);
				Sleep(100);
				g_pProtocol->SendReq(req);
			}
#endif
		}

		ShowDlgTitle();
	}
	else
	{
		if(m_FriendInfo.GetId() == ack->GetId())
		{
#ifdef _CS_PLAT_EX_
			g_LocalData.DeleteCurrentBuyerDbFile(m_FriendInfo.GetId());
#endif
			Close();
		}
	}
}

void CDuiP2PSendMsgDlg::UpdateFriendInfo(FRIEND_INFO *pFriend)
{
	memcpy(&m_FriendInfo,pFriend,sizeof(XT_USER_INFO));
	//m_pwndUserPanel->UpdateFriendInfo();
}

void CDuiP2PSendMsgDlg::OnLogOut(const char *data,WORD wRet)
{
	//DestroyWindow();
}

BOOL CDuiP2PSendMsgDlg::DestroyWindow()
{
	BOOL bForce = false;

	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		bForce = true;
	}

	if ( IsWindowVisible(*this) )
	{
		BOOL bHasEmotionFile = (m_vecSendFiles.size()>0);
		BOOL bHasP2PFile     = (m_poolRecv.size()>0 || m_poolSend.size()>0 || m_vecSendFile.size()>0 || m_vecRecvFile.size()>0 ); 


		if ( bForce == FALSE)
		{
			CString strTip="";

			if ( bHasEmotionFile && bHasP2PFile)
			{
				strTip="如果关掉窗口，将会终止未发出的图片和终止所有的文件传输。是否关闭窗口？";
			}
			else
			{
				if (bHasEmotionFile)
				{
					strTip="如果关掉窗口，将会终止未发出的图片。是否关闭窗口？";
				}

				if (bHasP2PFile)
				{
					strTip="如果关掉窗口，将会终止所有的文件传输。是否关闭窗口？";
				}
			}
/*
			if ( strTip!="" )
			{
				if ( MessageBox(strTip,IM_CAPTION,MB_ICONINFORMATION|MB_YESNO)!=IDYES )
				{
					return FALSE;
				}
			}
*/
		}

		if ( bHasEmotionFile )
		{
			SF_AbortPfileTransmit();
		}

		if ( bHasP2PFile )
		{
			for ( int i=0; i<m_vecRecvFile.size(); i++ )
			{
				P2PSendFileReplay(m_vecRecvFile[i].nID,false);
			}
			for ( int i=0; i<m_vecSendFile.size(); i++ )
			{
				//P2PID id;
				//id.fid = m_vecSendFile[i].nID;
				//id.uid = m_FriendInfo.id;
				AF_AbortSendFile(m_vecSendFile[i].nID);
			}

			AF_AbortAllSendFiles();
			AF_AbortAllRecvFiles(m_FriendInfo.GetId());
		}

		//m_reditSend.ClearMsg();
		//m_reditSend->SetText("");
		m_reditSend->SetSel(0, -1);
		m_reditSend->ReplaceSel("", false);
		//m_reditHistory.ClearMsg();
		//m_reditHistory->SetText("");
		m_reditHistory->SetSel(0, -1);
		m_reditHistory->ReplaceSel("", false);
	}

	if(m_wndToolBar.IsDownloader())
	{
		if(bForce == FALSE && CMessageBox::Model(GetHWND(),"如果关掉窗口，将会终止下载，是否关闭窗口？","",MB_YESNO/*|MB_DEFBUTTON2*/)==IDNO)
		{
			return FALSE;
		};
		m_wndToolBar.StopDownloadFace();
	}
	return FALSE;
//	return __super::DestroyWindow();
}

void CDuiP2PSendMsgDlg::SF_OnRecvFileReq(char *file_name)
{
	// 记录接收的群图片
	g_LocalData.AcceptFriendPic(file_name);

	int count = m_vecWaitImageFiles.size();

	for ( int i=0; i<count; i++ )
	{
		if ( strcmp(m_vecWaitImageFiles[i].szFile,file_name)==0 )
		{
			m_vecWaitImageFiles[i].time_recv = GetTickCount();
		}
	}
}

void CDuiP2PSendMsgDlg::SF_OnRecvFilePacket(char * file_name, int file_id )
{
	SF_OnRecvFileReq(file_name);
}

LRESULT CDuiP2PSendMsgDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case TIMER_P2P_STATUS:
		{
			DWORD dwTick = GetTickCount();

			int nSendFile = m_vecWaitSendFile.size();
			int nRecvFile = m_vecWaitRecvFile.size();

			if ( nSendFile>0 || nRecvFile || m_bMMStartTalk || m_bMMWaitTalk )
			{
				if ( this->m_FriendInfo.ip==0 || (dwTick - m_dwIpTick)>30000 )//30秒
				{
					this->RefreshFriendStatus();
				}
				else
				{
					//if ( g_ImUserInfo.ip != m_FriendInfo.ip )
					//{
					//	XT_P2PTEST_REQ req;

					//	req.id        = g_ImUserInfo.GetId();
					//	req.dest_ip   = m_FriendInfo.ip;
					//	req.dest_port = m_FriendInfo.port;
					//	req.toid      = m_FriendInfo.id;
					//	req.ipType    = XT_P2PTEST_REQ::P2P;

					//	g_pProtocol->SendP2pTestPacket(req);
					//}

					if ( nSendFile>0 )
					{
						for ( int i=0; i<nSendFile; i++ )
						{
							tagP2PFile p2pFile = m_vecWaitSendFile[i];

							CAdvancedFileTransSession::SENDFILE_RESULT  sr = AF_SendFile(p2pFile.strFile,p2pFile.nID);

							if (sr!=SR_OK)
							{
								CString strError;

								switch(sr)
								{
								case SR_SIZE_ZERO:
									strError.Format("文件 \"%s(0KB)\" 内没有任何数据。发送失败！",p2pFile.strFile);
									break;
								case SR_DIRECTORY:
									strError.Format("\"%s\"是一个文件夹。不支持发送目录及其包含的文件！",p2pFile.strFile);
									break;
								case SR_FILE_REPEAT:
									strError.Format("发送重复的文件 \"%s\" 。",p2pFile.strFile);
									break;
								default:
									strError.Format("文件 \"%s\" 正在被使用，请确保该文件没有被别的程序使用再发送！",p2pFile.strFile);
									break;
								}

								AddMsgTip(strError,RGB(255,0,0));
							}
						}

						m_vecWaitSendFile.clear();
					}

					if ( nRecvFile>0 )
					{
						for ( int i=0; i<nRecvFile; i++ )
						{
							tagWaitRecvFile waitFile = m_vecWaitRecvFile[i];
							AF_AcceptFile(waitFile.nFileId,waitFile.nSize,waitFile.strLocalFile,waitFile.nStartPosition,m_nPeerTransServerIp,m_nPeerTransServerPort,true);
						}

						m_vecWaitRecvFile.clear();
					}
				}
			}
		}
		break;
	case TIMER_DWN_CPIC:
		{
			int i,n;
			RECV_FILE_STRUCT * pRF;
			n = m_vecRecvFiles.size();
			for(i = 0;i < n; i++)
			{
				pRF = m_vecRecvFiles[i];
				if(pRF->nWaitTimes > 0)
				{
					pRF->nWaitTimes--;
				}
				else if(pRF->nWaitTimes == 0)
				{
					// 如果等待完毕,则重新开始下载
					SF_DownloadCfile(pRF);
					// 设置为负数,停止发送下载命令
					pRF->nWaitTimes = -1;
				}
			}
		}
		break;
	case 0x01:
		{
			int count=m_vecWaitImageFiles.size();

			DWORD time_now = GetTickCount();

			for ( int i=0; i<count; i++ )
			{
				tagWaitImageFileStruct &wi=m_vecWaitImageFiles[i];

				if ((time_now-wi.time_recv) > 60000)
				{
					//m_reditHistory.OnAbortImageFile(wi.szFile);				
					m_vecWaitImageFiles.erase(m_vecWaitImageFiles.begin()+i);
					i--;
					count--;
				}	
			}
		}
		break;
	case TIMER_DOWNLOADOBJ:
		{
			map<CString,DOWNLOAD_OBJ*>::iterator it,itEnd;
			it = m_dicDownloadObj.begin();
			itEnd = m_dicDownloadObj.end();
			CString sFile;
			for(;it!=itEnd;it++)
			{
				sFile = it->first;
				switch(it->second->state) {
				case DS_DOWNLOADING:	// 正在下载
					if(it->second->timer > DOWNLOAD_TIMEROUT)
					{
						// 下载超时
						StopDownload(sFile);
						SF_OnAbortFile(sFile);
					}
					else
					{
						it->second->timer+=300;
					}
					// 应立刻跳出循环
					goto END0;
				case DS_SUCCESS:		// 下载成功
					SF_OnRecvFileComplete(sFile);
					StopDownload(sFile,false);
					// 应立刻跳出循环
					goto END0;
				case DS_FAILED:			// 下载失败
					SF_OnAbortFile(sFile);
					StopDownload(sFile,false);
					// 应立刻跳出循环
					goto END0;
				};
			}
		}
		break;
	case TIMER_WIN_SHAKE:
		{
			//抖动窗口
			CRect rcDlg;
			GetWindowRect(*this, &rcDlg);
			int nXpos = rcDlg.Width(); //宽度
			int nYpos = rcDlg.Height();//宽度
			switch(m_nWinShakeCnt%4)
			{
			case 0:
				rcDlg.left =  rcDlg.left + 7;
				break;
			case 1:
				rcDlg.top =  rcDlg.top + 7;
				break;
			case 2:
				rcDlg.left =  rcDlg.left - 7;
				break;
			case 3:
				rcDlg.top =  rcDlg.top - 7;
				break;
			default:
				break;
			}

			rcDlg.right = rcDlg.left + nXpos;//宽度
			rcDlg.bottom = rcDlg.top + nYpos;//高度P2P_DLG_HIGHT;
			MoveWindow(*this, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(), true);
			m_nWinShakeCnt++;

			if(m_nWinShakeCnt >= 20)
				KillTimer(*this, TIMER_WIN_SHAKE);
		}		
		break;
	case TIMER_SHOW_USERCARD:
		{
			KillTimer(*this, wParam);

			DuiLib::CPoint   point;
			GetCursorPos(&point);

			CRect rc;
			//m_ctrlFaceIcon.GetWindowRect(rc);

			if(rc.PtInRect(point))
			{
				static std::tr1::shared_ptr<CUserCardDlg> pInstance = CUserCardDlg::GetInstance();
				if(pInstance == NULL)
				{
					int nLeft = rc.left-318;

					if ( nLeft<0 )
					{	
						CRect rcDlg;
						GetWindowRect(*this, &rcDlg);
						nLeft = rcDlg.right + 8;
					}

					CUserCardDlg::Show(&m_FriendInfo,nLeft,rc.top);
				}
			}
		
		}
		break;
	case TIMER_FASTWORDSELECTMENU:
		{
			//输出框失去焦点时获取焦点
			//m_reditSend.SetFocus();
			//::SendMessage( CFastWordSelectMenu::GetInstance()->GetHWND(), WM_KILLFOCUS, NULL, NULL);
			SetEditFocus();
			KillTimer(GetHWND(), TIMER_FASTWORDSELECTMENU);
		}
		break;
	case TIMER_REDITSEND_TEXT_CHANGE:
		{
		    //判断输入框文字变化就发送快捷短语菜单显示的消息
			//if(m_reditSend.m_hWnd == NULL)
			//	break;
			CString strReditSend;
			//m_reditSend.GetText(strReditSend);
			strReditSend = m_reditSend->GetText();
			CString strText = strReditSend;
			if(strReditSend != m_strLastReditSendText)
			{
				//m_reditSend.SendFastWordSelectMenu();快捷短语菜单
				if(CFastWordSelectMenu::GetInstance() != NULL && CFastWordSelectMenu::GetInstance()->GetHWND() != NULL && IsWindowVisible(CFastWordSelectMenu::GetInstance()->GetHWND()))
				{
					int nFastWord = strReditSend.ReverseFind('/');
					if(nFastWord == -1)
						strText = "";
					else
					{
						strText = strText.Right(strText.GetLength() - nFastWord - 1);
					}

					DuiLib::CPoint point = m_reditSend->PosFromChar(/*VK_OEM_2*/atoi(strText.Right(1)));
					point.y = point.y - m_reditSend->GetPos().top;
					char* pChar = (LPSTR)(LPCSTR)strText;
					if(strText != "")
						SendMessage(WM_FASTWORD_MENU, MAKELONG(point.x, point.y), (DWORD)pChar);
				}


				m_strLastReditSendText = strReditSend;
			}
			//清空输入框时字体还原
			if(strReditSend == "" && m_bIsFontLast)
			{
				g_ImFont = m_fontLast;
				m_bIsFontLast = FALSE;
				//m_reditSend.SetXFont(g_ImFont);
				ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
				RichEdit_SetDefFont(pTextServicesSend, g_ImFont.fontName, g_ImFont.fontSize, g_ImFont.fontColor, g_ImFont.flags & XTF_BOLD, g_ImFont.flags & XTF_ITALIC, g_ImFont.flags & XTF_UNDERLINE, false);
			}
		}
		break;
	case TIMER_SCREEN_COPY://截屏隐藏窗口时隔0.1秒再弹出截屏，防止XP系统有时隐藏不了窗口的问题
		{
			if(g_pMessageHandleWnd->GetScreenCtrl() == NULL)
			{
				g_pMessageHandleWnd->NewScreenCtrl();
				if(IDOK == g_pMessageHandleWnd->GetScreenCtrl()->DoModal())
				{
					CString strCacheFileName;
					g_LocalData.SaveImageFileToCache(g_pMessageHandleWnd->GetScreenCtrl()->m_strImageFile,strCacheFileName);
					CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

					DeleteFile(g_pMessageHandleWnd->GetScreenCtrl()->m_strImageFile);

					//m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);
					RichEdit_InsertImage(m_reditSend, (LPCTSTR)strCacheImageFullPathName, -1, -1);

					g_pMessageHandleWnd->DeleteScreenCtrl();
				}
				else
				{
					g_pMessageHandleWnd->DeleteScreenCtrl();
				}
			}

			this->ShowWindow(true);
			KillTimer(GetHWND(), TIMER_SCREEN_COPY);
		}
		break;
	case TIMER_GETFRIEND_INFO:
		{
			KillTimer(GetHWND(), TIMER_GETFRIEND_INFO);
			if(!m_bIsGetFriendInfo)
			{
				CString strLog = "3秒未获取到用户信息，再获取一次";
				g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);

				Sleep(100);
				// 获取好友详情已获得guid
				XT_USERINFO_GET_REQ req;
				req.fid = m_FriendInfo.GetId();
				g_pProtocol->SendReq(req);
				SetTimer(GetHWND(),TIMER_GETFRIEND_INFO,3000,NULL);
			}
		}
		break;
	case TIMER_UPDATEURL:
		{
			m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
			CString strLogUrl;
			strLogUrl.Format("刷新地址:%s", m_strAdUrl);
			g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLogUrl);
			KillTimer(GetHWND(), TIMER_UPDATEURL);
		}
		break;
	case TIMER_EDIT_ENDDOWN:
		{
			if(m_reditHistory->GetPos() != 0)
			{
				m_reditHistory->EndDown();
				KillTimer(GetHWND(), TIMER_EDIT_ENDDOWN);
			}
		}
		break;
	case FRM_TIMER_ID:
		{
			ITextServices* pTextServicesSend = m_reditSend->GetTextServices();
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			Richedit_UpdateGif(m_reditSend, pTextServicesSend);
			Richedit_UpdateGif(m_reditHistory, pTextServicesHistory);
		}
		break;
	}

END0:
	return 0;
// 	__super::OnTimer(nIDEvent);
}

void CDuiP2PSendMsgDlg::OnStatusGetAck(const char *data,WORD wRet)
{
	if ( wRet == 0 && m_pmP2PMode!=pmMultiTalk )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;

		if ( ack && ack->id == m_FriendInfo.GetId() )
		{
			if ( ack->status == XTREAM_HIDE || ack->status == XTREAM_OFFLINE)
			{
				ack->status = XTREAM_OFFLINE;
			}
			if(m_FriendInfo.status == ack->status)
				return;
			m_FriendInfo.ip     = ack->ip;
			m_FriendInfo.port   = ack->port;
			m_FriendInfo.status = ack->status;
			m_FriendInfo.localIP = ack->lanip;
			m_FriendInfo.localPort = ack->lanport;

			UpdateFriendState();

			if(m_FriendInfo.ip == 0)
				m_FriendInfo.p2pRetry = 0;
			memcpy(m_FriendInfo.key,ack->p2pkey,sizeof(ack->p2pkey));
			FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
			if(pInfo)
			{
				pInfo->ip     = ack->ip;
				pInfo->port   = ack->port;
				pInfo->localIP = ack->lanip;
				pInfo->localPort = ack->lanport;
				pInfo->status = ack->status;
				if(pInfo->ip == 0)
					pInfo->p2pRetry = 0;
				memcpy(pInfo->key,ack->p2pkey,sizeof(ack->p2pkey));
			}

			m_dwIpTick = GetTickCount();

			PEER_ENDPOINT pt;
			pt.ip = m_FriendInfo.ip;
			pt.port = m_FriendInfo.port;
			pt.localIp = m_FriendInfo.localIP;
			pt.localPort = m_FriendInfo.localPort;
			//AF_OnGetStatusOK(pt);
			ShowDlgTitle();
			//m_pwndUserPanel->UpdateFriendInfo();

			m_lblIP.m_ip = m_FriendInfo.ip;
			m_lblIP.m_port = m_FriendInfo.port;
			m_lblIP.m_localIp = m_FriendInfo.localIP;
			m_lblIP.m_localPort = m_FriendInfo.localPort;


			uint32 friendIp;
			uint16 friendPort;
			if(m_FriendInfo.ip == g_ImUserInfo.ip)
			{
				friendIp = m_FriendInfo.localIP;
				friendPort = m_FriendInfo.localPort;
			}
			else
			{
				friendIp = m_FriendInfo.ip;
				friendPort = m_FriendInfo.port;
			}

			AF_SetInfo(g_ImUserInfo.GetId(),m_FriendInfo.GetId(),friendIp,htons(friendPort),g_pProtocol->GetTranServerIP(),htons(g_pProtocol->GetTranServerPort()),g_pProtocol->m_pUdpComm);			
		}
	}
	else if(wRet == SERVER_TIMEOUT)
	{
		SendDataStruct * pSend = (SendDataStruct * )data;
		LPXT_HEAD pH = (LPXT_HEAD)pSend->data;
		if(m_FriendInfo.GetId() == pH->did)
			AF_OnGetStatusFailed();
	}


}


void CDuiP2PSendMsgDlg::OnStatusModAck(const char *data,WORD wRet)
{
	LPXT_STATUS_MOD_ACK resp = (LPXT_STATUS_MOD_ACK)data;

	if ( wRet==0 )
	{
		if ( resp->status==XTREAM_OFFLINE )
		{
			if ( m_pmP2PMode==pmMultiTalk )
			{
				uint32 fid = m_clusterInfo.members[0].GetId()==g_ImUserInfo.GetId() ? 
					m_clusterInfo.members[1].GetId() : m_clusterInfo.members[0].GetId();

				m_pmP2PMode = pmSingleTalk;

				FRIEND_INFO *pFriend = ::GetFriendInfo(fid);
				if(pFriend)
				{
					// 如果好友离线，则通讯方式改由服务器转发
					pFriend->connFlag = FRIEND_INFO::NONE;
				}

				if ( pFriend==NULL )
				{
					pFriend=m_clusterInfo.GetFriendInfo(fid);
					pFriend->ip=0;
					pFriend->port=0;
				}

				if ( pFriend )
				{
					m_FriendInfo = * pFriend;
					m_wndUserList.ShowWindow(SW_HIDE);
					//m_pwndUserPanel->ShowWindow(SW_SHOW);
					//m_pwndUserPanel->SetFriendInfo(&m_FriendInfo);
					//m_pwndUserPanel->UpdateFriendInfo();

					::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2P_TO_GROUP,1,m_clusterInfo.nFamilyID);

					ShowDlgTitle();
				}
				else
				{
					//	DestroyWindow();
					return ;
				}							
			}

			m_FriendInfo.status=XTREAM_OFFLINE;
			g_ImUserInfo.status=XTREAM_OFFLINE;	

			//m_pwndUserPanel->UpdateFriendInfo();

			//m_reditSend.EnableWindow(FALSE);
			m_reditSend->SetEnabled(false);
			m_btnSend.EnableWindow(FALSE);
		}
		else
		{
			//m_reditSend.EnableWindow(TRUE);
			m_reditSend->SetEnabled(true);
			//m_btnSend.EnableWindow(TRUE);

			RefreshFriendStatus();

			//m_pwndUserPanel->UpdateFriendInfo();
		}			
	}	
}

void CDuiP2PSendMsgDlg::OnUpdateFontStyle(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(1);
}

bool CDuiP2PSendMsgDlg::GetRedrawRgn(CRgn &rgn)
{
// 	CRect rc;
// 	GetClientRect(*this, &rc);
// 
// 	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);
// 
// 	RemoveRgnOfWnd(rgn,&m_wndToolBar);
// 	//RemoveRgnOfWnd(rgn,&m_wndSplitter1);
// 	RemoveRgnOfWnd(rgn,&m_reditHistory);
// 	RemoveRgnOfWnd(rgn,&m_reditSend);

	/*
	if (m_btnHistory)
	{
	RemoveRgnOfWnd(rgn, &m_dlgHistory);
	}

	if ( m_pmP2PMode!=pmMultiTalk )
	{
	RemoveRgnOfWnd(rgn,&m_wndAD);
	} 
	else
	{
	RemoveRgnOfWnd(rgn,&m_wndUserList);
	} */


	//RemoveRgnOfWnd(rgn,&m_wndTitle);  //hechang，20110803
	//RemoveRgnOfWnd(rgn,&m_btnSend);
	//RemoveRgnOfWnd(rgn,&m_btnClose);
	//RemoveRgnOfWnd(rgn,&m_btnHistory);
	//RemoveRgnOfWnd(rgn,&m_btnInvite);
	//RemoveRgnOfWnd(rgn,&m_btnCard);
	//RemoveRgnOfWnd(rgn,&m_btnVisit);	
	return true;
}

void CDuiP2PSendMsgDlg::OnBnClickedFontBar()
{
// 	CRect rc1;
// 	//m_reditHistory.GetWindowRect(*this, &rc1);
// 	//ScreenToClient(&rc1);
// 	::GetClientRect(m_reditHistory.GetSafeHwnd(), &rc1);
// 
// 	if ( m_wndTalkToolBar.IsWindowVisible() )
// 	{
// 		rc1.bottom+=30;
// 
// 		m_wndTalkToolBar.ShowWindow(SW_HIDE);
// 		m_reditHistory.MoveWindow(rc1);		
// 	}
// 	else
// 	{
// 		CRect rc2=rc1;
// 
// 		rc1.bottom-=30;
// 		m_reditHistory.MoveWindow(rc1);	
// 
// 		rc2.top = rc1.bottom;
// 		rc2.bottom=rc2.top+30;
// 
// 		m_wndTalkToolBar.MoveWindow(rc2);
// 		m_wndTalkToolBar.ShowWindow(SW_SHOW);
// 		m_reditHistory.MoveWindow(rc1);	
// 	}
// 
// 	m_reditSend.SetFocus();
	CHorizontalLayoutUI* pHlTalkBar = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hltalkbar")));
	if(pHlTalkBar->IsVisible())
	{
		pHlTalkBar->SetVisible(false);
	}
	else
	{
		pHlTalkBar->SetVisible(true);
	}
	
}



void CDuiP2PSendMsgDlg::OnScreenCopyOption()
{
	// TODO: 在此添加控件通知处理程序代码
	//CRect rc;
	//m_wndToolBar.m_btnScreen.GetWindowRect(&rc);

	CButtonUI* pBtnScreenCopyConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
	RECT rc = pBtnScreenCopyConfig->GetPos();
	RECT rcParent;
	::GetWindowRect(*this, &rcParent);
	rc.left += rcParent.left;
	rc.top += rcParent.top;
	rc.right += rcParent.left;
	rc.bottom += rcParent.top;

	DuiLib::CPoint p;
	int	id;

	// create it
	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(hInstance);

	// add some items
	HOTKEY_CONFIG *cfg;
	cfg = g_LocalData.GetHotKeyConfig();

	if(cfg->bScreenCopyHideWindow == SMK_ENTER)
	{
		menu.AppendItem(0,"截屏时显示当前窗口",1,0);
		menu.AppendItem(0,"截屏时隐藏当前窗口",2);
	}
	else
	{
		menu.AppendItem(0,"截屏时显示当前窗口",1);
		menu.AppendItem(0,"截屏时隐藏当前窗口",2,0);
	}

	// track it
	p.x = rc.right - 24;
	p.y = rc.bottom + 2;

	id=menu.Track(p.x,p.y,*this,true);


	cfg = g_LocalData.GetHotKeyConfig();
	switch(id)
	{
		case 1:
			cfg->bScreenCopyHideWindow = false;
			break;
		case 2:
			cfg->bScreenCopyHideWindow = true;
			break;
		default:
			break;
	}

	
}

void CDuiP2PSendMsgDlg::OnScreenCopyEx()
{
	BOOL bPtInText = m_wndToolBar.m_btnScreen.PressedInText();

	if(bPtInText)
	{
		OnScreenCopy();
	}
	else
	{
		OnScreenCopyOption();
	}
}

void CDuiP2PSendMsgDlg::OnScreenCopy()
{	
	if(GetTickCount() - m_screenCopyTick < 2000)
	{
		return;
	}
	m_screenCopyTick = GetTickCount();

	HOTKEY_CONFIG *cfg;
	cfg = g_LocalData.GetHotKeyConfig();

	if(cfg->bScreenCopyHideWindow)
	{
		this->ShowWindow(false);
		SetTimer(GetHWND(), TIMER_SCREEN_COPY, 100, NULL);
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

				DeleteFile(g_pMessageHandleWnd->GetScreenCtrl()->m_strImageFile);

				//m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());
				//m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);
				RichEdit_InsertImage(m_reditSend, (LPCTSTR)strCacheImageFullPathName, -1, -1);

				g_pMessageHandleWnd->DeleteScreenCtrl();
			}
			else
			{
				g_pMessageHandleWnd->DeleteScreenCtrl();
			}
		}
	}
	//if(cfg->bScreenCopyHideWindow)
	//{
	//	this->ShowWindow(true);
	//}
}

void CDuiP2PSendMsgDlg::OnMagicFace()
{
	return;
	bool bCanSendPic=false ;
	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		AddMsgTip("对不起，您当前为离线状态，无法发送魔法表情！");
		return;
	}
	if ( m_pmP2PMode!=pmMultiTalk )
	{
		//if ( &m_FriendInfo )
		//{
		//	if ( m_FriendInfo.status==XTREAM_OFFLINE )
		//	{
		//		//MessageBox("对不起对方不在线！不能发送魔法表情！",IM_CAPTION,MB_OK|MB_ICONWARNING);	
		//		ShowMessage("对方不在线或隐身！不能发送魔法表情！");
		//	}
		//	else
		//	{
		//		bCanSendPic = true;	
		//	}							
		//}
		//else
		//{
		bCanSendPic = true;	
		//}
	}
	else
	{
		bCanSendPic = true;
	}

// 	if ( bCanSendPic )
// 	{
// 		CPoint pt;
// 		GetCursorPos(&pt);
// 
// 		CMagicFaceDlg::Show(pt.x,pt.y,g_ImUserInfo.GetId(),m_hWnd);
// 	}
}

void CDuiP2PSendMsgDlg::ShowMagicFace(const char *face_name)
{
	CString strFaceName=face_name;

	map<CString,CString>::iterator it = m_mapMagicFace.find(strFaceName);

	if ( it==m_mapMagicFace.end() )
	{
		CString strTip;
		strTip.Format("不能播放魔法表情\"%s\"",face_name);
		AddMsgTip(strTip);
	}
	else
	{
		g_magicFaceProcess.ShowMagicFace(it->second);
	}
}

void CDuiP2PSendMsgDlg::RecvSendFileReq(const char *data, WORD wRet)
{
	P2P_SENDFILE_REQ  & req = *(LPP2P_SENDFILE_REQ)data;

	m_nPeerTransServerIp   = req.tranIp;
	m_nPeerTransServerPort = req.tranPort;// 网络字节

	RefreshFriendStatus();

	bool bDuplicate = false;

	int n = m_vecRecvFile.size();

	for ( int i=0; i<n; i++ )
	{
		if ( m_vecRecvFile[i].nID==req.fid )
		{
			bDuplicate = true;
			break;
		}
	}


	CString strFileName = req.name ;
	CString strFileExt = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1);
	CString strSize,strTips;
	int nFileSize = ceil(req.size.sizel * 1.0 / 1024);
	strSize.Format("(%d KB)",nFileSize);
	bool bRefuse = false;
	
	WORD nVKTrans;
	g_LocalData.GetFileTransConfig(nVKTrans);
	switch(nVKTrans)
	{
		case 1:
		{
			bRefuse = true;
			strTips= "对方尝试向您发送\"" +  strFileName + strSize+ "\"，由于您的安全级别设置为高，无法接收任何文件。";
			break;
		}
		case 3:
		{
			break;
		}
		case 2:
		default:
		{
			if(strFileExt == "exe" || strFileExt == "dll" || strFileExt == "bat")
			{
				bRefuse = true;
				strTips = "对方尝试向您发送\"" +  strFileName + strSize + "\"，由于您的安全级别设置为中，哞哞会屏蔽存在风险的文件。";
			}
			break;
		}
			break;
	}


	
	if ( bDuplicate==false )
	{
		//P2PID id;
		//id.fid = req.fid;
		//id.uid = m_FriendInfo.id;
		
		
		if(bRefuse)
		{
			AddMsgTip(strTips);
			//直接拒绝接受
			//m_reditHistory.SetFileBarTip(req.fid, ODT_RECV_FILE,"您拒绝接收该文件",RGB(255,0,0));
			//m_reditHistory.InsertLink("更新设置");
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory,req.fid, ODT_RECV_FILE,"您拒绝接收该文件",RGB(255,0,0));
			AF_AcceptFile(req.fid,0,"",0,0,0,FALSE);
			//P2PSendFileReplay( req.fid,false);
		}
		else if ( bDuplicate==false && AF_FindRecvFile(req.fid) == NULL )
		{
			XT_FONT		xf;

			CString strTitle,strDate, strTime;
			CTime  tm=CTime::GetCurrentTime();

			strTime=tm.Format("%H:%M:%S");
			strDate=tm.Format("%Y.%m.%d");

			//好友发给我的信息
			strTitle.Format("%s %s %s 发送",m_FriendInfo.nickname,strDate,strTime);	

			xf.fontColor=RGB(96,92,168);

			//m_reditHistory.SetSel(-1,-1);
			//m_reditHistory.InsertText("\n");
			//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

			//m_reditHistory.SetStartIndent(80);
			//m_reditHistory.InsertText(strTitle);



			//m_reditHistory.InsertFileInfoBar(req.fid,false,req.name,req.size.sizel);

			//if ( m_reditHistory.m_hWnd )
			//	m_reditHistory.ScrollToBottom();

			m_reditHistory->SetSel(-1,-1);
			m_reditHistory->InsertText(-1, "\n");
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
			RichEdit_SetStartIndent(pTextServicesHistory, 80);
			m_reditHistory->InsertText(-1, strTitle);
			m_reditHistory->InsertText(-1, "\n");
			RichEdit_InsertFileInfoBar(pTextServicesHistory,req.fid,false,req.name,req.size.sizel);
			InsertFileInfoBar(req.fid,false,req.name,req.size.sizel);
			m_reditHistory->EndDown();

			tagP2PFile f;
			f.nID     = req.fid;
			f.nSize   = req.size.sizel;
			f.strFile = req.name;

			m_vecRecvFile.push_back(f);

			CMainFrame::GetInstance()->GetRecentList()->AddUser(&m_FriendInfo);
			RECENTLIST_OP(AddUser(&m_FriendInfo))
		}
	}
}

bool CDuiP2PSendMsgDlg::RecvMagicFace(uint32 friend_id, CString strFaceName, CString strFaceUrl)
{
	//map<CString,CString>::iterator it = m_mapMagicFace.find(strFaceName);

	//if ( it==m_mapMagicFace.end() )
	//	m_mapMagicFace.insert(map<CString,CString>::value_type(strFaceName,strFaceUrl));

	//CString strTip;

	//FRIEND_INFO *pFriend = NULL;

	//if ( m_pmP2PMode!=pmMultiTalk )
	//{
	//	pFriend=&m_FriendInfo;
	//}
	//else
	//{
	//	pFriend = ::GetFriendInfo(friend_id);
	//}

	//if ( pFriend && pFriend->GetId()==friend_id )
	//{
	//	CString strTip;
	//	strTip.Format("%s 发送了魔法表情",pFriend->nickname);

	//	if ( m_hWnd && m_reditHistory.m_hWnd )
	//	{
	//		XT_FONT xf;
	//		xf.fontColor = RGB(122,122,122);

	//		CHARRANGE cr;

	//		m_reditHistory.SetSel(-1,-1);
	//		m_reditHistory.GetSel(cr);

	//		m_reditHistory.InsertText("\n");
	//		m_reditHistory.SetStartIndent(120);

	//		CResBitmap bmpLine;
	//		bmpLine.LoadBitmap(IDB_HINT_FLAG2);
	//		m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

	//		m_reditHistory.InsertText(strTip);

	//		strTip.Format("“%s”",strFaceName);
	//		m_reditHistory.InsertLink(strTip);
	//		m_reditHistory.InsertText("  ");

	//		m_reditHistory.SetSel(cr.cpMax,-1);
	//		m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//		m_reditHistory.SetSel(-1,-1);

	//		m_reditHistory.ScrollToBottom();
	//	}
	//	else
	//	{
	//		_tagMsg t_msg;
	//		t_msg.msg.from_id = friend_id;
	//		sprintf(t_msg.msg.fontName,strFaceName);
	//		sprintf(t_msg.msg.data,strFaceUrl);
	//		t_msg.type=2;
	//		PushMsg(t_msg); 
	//	}
	//}
	//else
	//{
	//	return false;
	//}

	//CMainFrame::GetInstance()->GetRecentList()->AddUser(&m_FriendInfo);
	//RECENTLIST_OP(AddUser(&m_FriendInfo))

	return true;
}
void CDuiP2PSendMsgDlg::OnBnClickedBtnCard()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pmP2PMode!=pmMultiTalk)
	{
		/*::SendMessage(pMainFrame->m_pPage1->m_pageAddbook.m_hWnd,
			WM_SAVECARD_NOTIFY,0,(LPARAM)&m_FriendInfo );*/
	}
	else
	{
		int nSel = m_wndUserList.m_wndList.GetCurSel();
		if( nSel == -1 )
		{
			ShowMessage("请先选中一位好友");
			return;
		}

		FRIEND_INFO* pFriendInfo = 	(FRIEND_INFO*)m_wndUserList.m_wndList.GetItemData(nSel);
		if( pFriendInfo)
		{
			/*::SendMessage(pMainFrame->m_pPage1->m_pageAddbook.m_hWnd,
				WM_SAVECARD_NOTIFY,0,(LPARAM)pFriendInfo );*/
		}
	}
}

void CDuiP2PSendMsgDlg::OnBnClickedBtnVisit()
{
	CString strHtml;	

	if( m_pmP2PMode!=pmMultiTalk )
	{
		if(g_pWebConfig == NULL) return;
		strHtml = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(m_FriendInfo.GetUserName().c_str()));
	}
	else
	{
		int nSel = m_wndUserList.m_wndList.GetCurSel();
		if( nSel == -1 )
		{
			ShowMessage("请先选中一位好友");
			return;
		}

		FRIEND_INFO* pFriendInfo = 	(FRIEND_INFO*)m_wndUserList.m_wndList.GetItemData(nSel);
		if( pFriendInfo && ( g_pWebConfig != NULL) )
		{
			strHtml = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(pFriendInfo->GetUserName().c_str()));
		}
	}

	RegUserOperation(0x0022);
	MiniOpenUrl(strHtml);
}



LRESULT CDuiP2PSendMsgDlg::OnThreadNotify(WPARAM w, LPARAM l)
{
	if ( w==0 )
	{
		FRIEND_INFO *p=(FRIEND_INFO *)l;

		try
		{
			CString s;
			s.Format("%s,",p->nickname);
			//m_reditSend.InsertText(s);
			//m_reditSend.SetFocus();
			m_reditSend->InsertText(-1, s);
			SetEditFocus();
		}
		catch(...)
		{
		}
	}
	else
	{

	}

	return 0;
}

// 建立临时组成员群聊
void CDuiP2PSendMsgDlg::CreateTempCluster(const CLUSTER_INFO& ci,int nGroupIndex)
{
	//m_pmP2PMode = pmMultiTalk;
	////////////////////////////////////////////////////////////////
	//显示用户名和签名
// 	GetDlgItem(IDC_STATIC_USERNAME)->SetFont(&g_font16B); 
// 	SetDlgItemText(IDC_STATIC_USERNAME,ci.szFamilyName);

	//显示群描述
// 	GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
// 	SetDlgItemText(IDC_STATIC_SIGN, _T(""));
	//显示添加按钮
	//m_btnAddMember.ShowWindow(SW_SHOW);
	////////////////////////////////////////////////////////////////
	XT_CREATE_CLUSTER_REQ req;
	req.nFamilyProp  = TEMP_FAMILY;
	g_pProtocol->SendReq(req,&m_wSeq);

	m_clusterInfo=ci;
	m_nTempGroupTalkIndex=nGroupIndex;

	m_wndUserList.ShowWindow(SW_SHOW);
	//m_pwndUserPanel->ShowWindow(SW_HIDE);
	// 临时群建立前不能发送消息
	//m_btnSend.EnableWindow(FALSE);
	// 将自己添加进列表
	m_clusterInfo.members.push_back(g_ImUserInfo);
	m_wndUserList.SetClusterInfo(&m_clusterInfo);
	m_wndUserList.UpdateClusterList();
	//UpdateP2PPanel();
}


void CDuiP2PSendMsgDlg::SetClusterMember(uint32 clusterId)
{
	char buff[MAX_SEND_BUFFERS];
	XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);

	req.nFamilyID = clusterId;
	req.nAction   = CMEMBER_ADD;
	req.nFamilyProp = TEMP_FAMILY;
	req.nCount  = 0;
	size_t i,c;
	c = m_clusterInfo.Count();
	for ( i = 0; i < c; i++ )
	{
		if ( m_clusterInfo.members[i].GetId()!=g_ImUserInfo.GetId() )
			req.nMemID[req.nCount++]=m_clusterInfo.members[i].GetId();
	}
	g_pProtocol->SendReq(req);	
}

// 临时群建立的应答
void CDuiP2PSendMsgDlg::OnCreateTempClusterAck(const char *data,WORD wRet)
{
	LPXT_CREATE_CLUSTER_ACK ack = (LPXT_CREATE_CLUSTER_ACK)data;

	if(g_pProtocol->GetLastAckSequenceId()!=m_wSeq)
		return;

	if ( wRet==0 )
	{
		// 是否为临时群建立
		m_clusterInfo.nFamilyID = ack->nFamilyID;
		m_clusterInfo.nFamilyProp = TEMP_FAMILY;
		SetClusterMember(ack->nFamilyID);
		if ( m_pmP2PMode!=pmMultiTalk )
		{
			::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2P_TO_GROUP,0,(LPARAM)&m_FriendInfo);
			m_pmP2PMode = pmMultiTalk;
			// 显示标题
			ShowDlgTitle();
			SetBtnFolderBitmap();
			if ( IsWindowVisible(*this)==FALSE )
				ShowWindow(SW_SHOW);

			if (/*IsIconic()*/IsMinimized(*this) )
			{
				ShowWindow(SW_RESTORE);
			}
			BringWindowToTop(*this);
		}
	}
	else if ( wRet==SERVER_TIMEOUT )
	{
		CMessageBox::Prompt("建立临时群聊超时");
	}
	else
	{
		CMessageBox::Prompt(g_pProtocol->GetLastError());
	}
	if( wRet!=0)
	{
		CMessageBox::Prompt("创建临时群聊失败！");
	}
}

// 临时群成员建立的应答
void CDuiP2PSendMsgDlg::OnTempCMembersSetAck(const char *data,WORD wRet)
{
	if(wRet==0)
	{
		m_bClusterInit = true;
		// 临时群创建成果，则可以发送消息
		m_btnSend.EnableWindow(TRUE);
		m_pmP2PMode = pmMultiTalk;
		UpdateP2PPanel();
	}

}

LRESULT	CDuiP2PSendMsgDlg::OnStartDownloadFace(WPARAM w, LPARAM l)
{
	//if ( m_wndToolBar.StartDownloadFace() )
	//{
	//	m_dwProgressBar = m_reditHistory.FindProgressBar();
	//	if ( m_dwProgressBar==(DWORD)-1 )
	//		m_dwProgressBar = m_reditHistory.InsertProgressBar();
	//	m_reditHistory.SetProgressBarPos(m_dwProgressBar,0);
	//	m_reditHistory.SetProgressBarTip(m_dwProgressBar,"正在下载表情...");
	//	m_reditHistory.ScrollToBottom();
	//}
	//else
	//{
	//	AddMsgTip("正在另一个窗口中下载表情...",RGB(0,122,0));
	//}

	return 0;
}

LRESULT	CDuiP2PSendMsgDlg::OnDownloadFaceOK(WPARAM w, LPARAM l)
{
	//if(w==0)
	//{
	//	if ( m_dwProgressBar!=(DWORD)-1 )
	//	{
	//		m_reditHistory.SetProgressBarTip(m_dwProgressBar,"下载完成");
	//		m_reditHistory.SetProgressBarPos(m_dwProgressBar,100);

	//		CString strTip;
	//		strTip.Format("本次成功下载%d个表情",l);
	//		AddMsgTip(strTip);
	//	}
	//}
	//else if(w==1)
	//{
	//	//	AddMsgTip("表情文件下载失败:");
	//	//	AddMsgTip((LPCTSTR)l);
	//	if ( m_dwProgressBar!=(DWORD)-1 )
	//	{
	//		m_reditHistory.SetProgressBarTip(m_dwProgressBar,"下载失败!");
	//	}
	//}

	return 0;
}

LRESULT CDuiP2PSendMsgDlg::OnDownloadProcess(WPARAM w, LPARAM l)
{
	//int nTask=(int)w;
	//int nProcess=(int)l;
	////CString sTip;
	////sTip.Format("当前下载文件(%d|%d)",nProcess,nTask);
	////AddMsgTip(sTip);
	//if ( m_dwProgressBar!=(DWORD)-1 )
	//{
	//	int nPos = nProcess*100.0/nTask;
	//	m_reditHistory.SetProgressBarPos(m_dwProgressBar,nPos);
	//}
	return 0;
}

void CDuiP2PSendMsgDlg::OnBnClickedBtnFold()
{
	//m_bClosePanel = !m_bClosePanel;

	//SetBtnFolderBitmap();

	//AdjustUI();

	//if ( m_reditHistory.m_hWnd )
	//	m_reditHistory.ScrollToBottom();
}

void CDuiP2PSendMsgDlg::SetBtnFolderBitmap()
{
	if ( m_bClosePanel==false )
	{
		m_btnFolder.SetBitmap(IDB_SHOW_P2PFOLDER_HOVER,IDB_SHOW_P2PFOLDER_NORMAL,IDB_SHOW_P2PFOLDER_NORMAL,IDB_SHOW_P2PFOLDER_NORMAL);

		if ( m_pmP2PMode!=pmMultiTalk )
		{
			m_btnFolder.SetToolTip("隐藏对方形象");
		}
		else
		{
			m_btnFolder.SetToolTip("隐藏多人对话成员列表");
		}
	}
	else
	{
		m_btnFolder.SetBitmap(IDB_CLOSE_P2PFOLDER_HOVER,IDB_CLOSE_P2PFOLDER_NORMAL,IDB_CLOSE_P2PFOLDER_NORMAL,IDB_CLOSE_P2PFOLDER_NORMAL);

		if ( m_pmP2PMode!=pmMultiTalk )
		{
			m_btnFolder.SetToolTip("显示对方形象");
		}
		else
		{
			m_btnFolder.SetToolTip("显示多人对话成员列表");
		}
	}
}

UINT DownloadPicProc( LPVOID pParam )
{
	DOWNLOAD_OBJ * pObj = (DOWNLOAD_OBJ*)pParam;
	if(pObj==NULL)
		return -1;
	// 开始下载
	pObj->state = 0;
	CHttpDownload download;
	//图片改造
	CString strUrl = pObj->sUrl;
	if (strUrl.Find("http://") == -1)
	{
		strUrl = /*PICTURE_URL*/GetPicRandomUrl() + strUrl;
	}
	if(!download.DownLoadWebFile(/*pObj->sUrl*/strUrl,pObj->sSaved))
	{
		pObj->state = 2;
		return 1;
	}
	pObj->state = 1;
	return 0;
};


void CDuiP2PSendMsgDlg::ProcTalkMsg(const CString & sMsg,CString& sShowMsg)
{
	CMsgObj msg;
	CString sMD5;
	CString sFile;
	vector<string> arrImg;
	vector<string> arrShow;
	msg.Load(sMsg);
	msg.GetWebImage(arrImg);
	// 先判断这些图片链接是否存在于自定义表情中
	size_t nC = arrImg.size();
	arrShow.resize(nC);
	size_t i;
	EMOTION_ID id;
	for(i = 0;i < nC;i++)
	{
		id = FindFaceUseUrl(arrImg[i].c_str());
		// 本地表情中没有该图片，需要下载
		if(id.nGroup == -1)
		{
			// 先获得该文件的md5名
			GetMD5WebFileName(arrImg[i].c_str(),sMD5);		
			// 判断该文件是否已经下载
			if(FileExist(g_LocalData.GetImageCanchePath()+"\\"+sMD5))
			{
				// 该文件已经下载,则直接使用它的文件名显示
				arrShow[i] = sMD5;
			}
			else
			{
				arrShow[i] = sMD5;
				// 下载该文件
				// 下载前检测是否正在下载
				if(m_dicDownloadObj.find(sMD5) == m_dicDownloadObj.end())
				{
					DOWNLOAD_OBJ * pParam = new DOWNLOAD_OBJ;
					pParam->handle = NULL;
					pParam->sSaved = g_LocalData.GetImageCanchePath()+"\\"+sMD5;
					pParam->sUrl = arrImg[i].c_str();
					pParam->timer = 0;
					pParam->state = DS_DOWNLOADING;

					CWinThread * pThread;
					pThread = AfxBeginThread(DownloadPicProc,pParam);
					if(pThread)
						pParam->handle = (HANDLE)*pThread;
					// 设置下载任务的检测定时器
					if(m_dicDownloadObj.size()==0)
					{
						SetTimer(*this, TIMER_DOWNLOADOBJ,300,NULL);
					}
					// 保存当前项到字典
					m_dicDownloadObj[sMD5] = pParam;
				}
			}
		}
		// 本地有该文件缓存，不需要下载
		else
		{
			sFile = g_LocalData.GetImageCanchePath() +"\\"+ g_customEmotionDB[id.nGroup][id.nItem].szFile;
			if(!FileExist(sFile))
			{
				CopyFile(g_LocalData.GetCustomFacePath()+"\\" + g_customEmotionDB[id.nGroup][id.nItem].szFile,sFile,false);
			}
			arrShow[i] = g_customEmotionDB[id.nGroup][id.nItem].szFile;
		}
	}
	msg.ReplaceWebImage(arrShow);
	sShowMsg = msg.ToMsg().c_str();

}

void CDuiP2PSendMsgDlg::P2PSendFile(CString strFile)
{
	CString str1,str2,str3;
	::ExtractFilePath(strFile,str1,str2,str3);

	CString strFileName = str2+str3;


	str3.Trim();
	str3.MakeLower();

	//判断文件的后缀是否是 .exe, .dll
/*
	if ( str3==".exe" || str3==".dll" )
	{
		AddMsgTip("MouMou 不允许发送可疑的二进制文件("+strFile+")。杜绝传播网络病毒人人有责！",RGB(255,0,0));
	}
	else
	*/
	{
		uint16 file_id  = m_nSendFileSequence++;

		RefreshFriendStatus();

		XT_FONT		xf;

		CString strTitle,strDate, strTime;
		CTime  tm=CTime::GetCurrentTime();

		strTime=tm.Format("%H:%M:%S");
		strDate=tm.Format("%Y.%m.%d");

		//好友发给我的信息
		strTitle.Format("%s %s \\ %s 发送",g_ImUserInfo.nickname,strDate,strTime);	

		xf.fontColor=RGB(74,123,211);

		//m_reditHistory.SetSel(-1,-1);
		//m_reditHistory.InsertText("\n");
		//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

		//m_reditHistory.SetStartIndent(80);
		//m_reditHistory.InsertText(strTitle);
		m_reditHistory->SetSel(-1,-1);
		m_reditHistory->InsertText(-1, "\n");
		ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
		RichEdit_SetStartIndent(pTextServicesHistory, 80);
		m_reditHistory->InsertText(-1, strTitle);

		long nFileSize = ::GetFileSize(strFile);

		tagP2PFile f;
		f.nID		= file_id;
		f.nSize		= nFileSize;
		f.strFile	= strFile;

		m_vecSendFile.push_back(f);

		m_reditHistory->InsertText(-1, "\n");
		//m_reditHistory.InsertFileInfoBar(file_id,true,strFileName,nFileSize);
		RichEdit_InsertFileInfoBar(pTextServicesHistory,file_id,true,strFileName,nFileSize);
		InsertFileInfoBar(file_id,true,strFileName,nFileSize);

		//if ( m_reditHistory.m_hWnd )
		//	m_reditHistory.ScrollToBottom();
		m_reditHistory->EndDown();

		CMainFrame::GetInstance()->GetRecentList()->AddUser(&m_FriendInfo);
		RECENTLIST_OP(AddUser(&m_FriendInfo))

		if ( m_FriendInfo.status==XTREAM_OFFLINE )
		{
			AddMsgTip("对方不在线或隐身，可能无法立即回应你发送的文件．");
		}

		tagP2PFile p2pFile;

		p2pFile.nID     = file_id;
		p2pFile.strFile = strFile;
		p2pFile.nSize   = nFileSize;

		m_vecWaitSendFile.push_back(p2pFile);
	}	
}


void CDuiP2PSendMsgDlg::P2PSendFileReplay(long nFileId,bool bRecv)
{
	CString strLocalFile;

	TRACE("\nP2PSendFileReplay id=%d",nFileId);

	int n = m_vecRecvFile.size();

	for ( int i=0; i<n; i++ )
	{
		if ( m_vecRecvFile[i].nID==nFileId )
		{
			if ( bRecv )
			{				
				ShowFileTransInfoBar(true);
				m_wndP2PFileTransProgressBar.AddItem(m_vecRecvFile[i].strFile,
					m_vecRecvFile[i].nSize,
					m_vecRecvFile[i].nID,
					FALSE);

				map<int,CString>::iterator it = m_mapRecvFilePath.find((int)nFileId);

				if ( it!=m_mapRecvFilePath.end() )
					strLocalFile=it->second;
				else
				{
					FILE_SETTING setting;
					g_LocalData.GetUserRecvFileSetting(setting);
					strLocalFile=CString(setting.szRecvFilePath)+m_vecRecvFile[i].strFile;
				}

				uint32  senderUID		= 0;
				LONG	nStartPosition	= 0;
				//get当前断点位置
				CString    strPositionFile = strLocalFile+".cfg";

				CFile f;

				if ( f.Open(strPositionFile,CFile::modeRead|CFile::typeBinary) )
				{
					f.Read(&senderUID,sizeof(senderUID));
					f.Read(&nStartPosition,sizeof(LONG));
					f.Close();

					if ( senderUID!=m_FriendInfo.GetId() )
					{
						nStartPosition=0;
					}
					else
					{
						CString strTempFile = strLocalFile+".~tmp";

						if ( !FileExist(strTempFile) )
						{
							nStartPosition = 0;
						}
					}
				}

				tagWaitRecvFile waitFile;

				waitFile.nFileId        = nFileId;
				waitFile.nSize          = m_vecRecvFile[i].nSize;
				waitFile.nStartPosition = nStartPosition;
				waitFile.strLocalFile   = strLocalFile;

				m_vecWaitRecvFile.push_back(waitFile);
				//AF_AcceptFile(nFileId,m_vecRecvFile[i].nSize,strLocalFile,nStartPosition,m_nPeerTransServerIp,m_nPeerTransServerPort,true);
			}
			else
			{
				AF_AcceptFile(nFileId,0,strLocalFile,0,0,0,FALSE);
			}

			m_vecRecvFile.erase(m_vecRecvFile.begin()+i);
			break;
		}
	}	
}
/*
TS_P2P_START_CONNECT,		// 开始准备p2p连接
TS_P2P_START_DOWNLOAD,		// p2p连接成功，接收者B开始下载
TS_P2P_WAIT_DOWNLOAD,		// p2p连接成功，发送者A等待接收者B下载
TS_TRANS_WAIT,				// 接收者B等待发送者A通知登录中转服务器
TS_TRANS_START_DOWNLOAD,	// B开始通过中转服务器下载文件数据
TS_TRANS_START_CONNECT,		// A开始连接中转服务器*/
void CDuiP2PSendMsgDlg::AF_OnRecvStatusNotify(uint32 id,TASK_STATUS status)
{

	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,FALSE);
	if ( nItem!=-1 )
	{
		CString strTip;

		switch(status)
		{
		case TS_P2P_START_CONNECT:
			strTip="开始准备p2p连接";
			break;
		case TS_P2P_START_DOWNLOAD:
			strTip="p2p连接成功，正在准备接收";
			break;
		case TS_P2P_WAIT_DOWNLOAD:
			strTip="p2p连接成功，正在准备发送";
			break;
		}
		m_wndP2PFileTransProgressBar.SetItemProgressTip(nItem,strTip);
	}
}

void CDuiP2PSendMsgDlg::OnConnectTalkOk(bool bOk)
{
	if ( bOk )
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,0,1);
	else
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,0,0);
}

void CDuiP2PSendMsgDlg::OnAcceptTalkOk(bool bOk)
{
	if ( bOk )
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,1,1);
	else
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,1,0);
}

bool CDuiP2PSendMsgDlg::AF_CanTrans(uint32 id)
{
	CP2PSendFile *pFile = AF_FindSendFile(id);

	if ( pFile )
	{
		if ( pFile->GetSize()<(5*1024*1024) )
		{
			return true;
		}
		else
		{
			int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,TRUE);

			if ( nItem!=-1 )
				m_wndP2PFileTransProgressBar.DeleteItem(nItem);

			if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
			{
				ShowFileTransInfoBar(false);
			}

			CString strTip;
			strTip.Format("文件 \"%s\" 大小超过5M,服务器拒绝提供中转!",pFile->GetFileName());
			AddMsgTip(strTip,RGB(255,0,0));

			//m_reditHistory.SetFileBarTip(id,ODT_SEND_FILE,"服务器中转失败",RGB(255,0,0));
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, id,ODT_SEND_FILE,"服务器中转失败",RGB(255,0,0));
			return false;
		}
	}

	return false;
}

void CDuiP2PSendMsgDlg::AF_OnSendStatusNotify(uint32 id,TASK_STATUS status)
{
	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,TRUE);
	if ( nItem!=-1 )
	{
		CString strTip;

		switch(status)
		{
		case TS_P2P_START_CONNECT:
			strTip="开始准备p2p连接";
			break;
		case TS_P2P_START_DOWNLOAD:
			strTip="p2p连接成功，正在准备接收";
			break;
		case TS_P2P_WAIT_DOWNLOAD:
			strTip="p2p连接成功，正在准备发送";
			break;
			/*case TS_TRANS_WAIT:
			strTip="正在建立服务器中转连接";
			break;
			case TS_TRANS_START_DOWNLOAD:
			strTip="正在通过服务器中转接收文件";
			break;
			case TS_TRANS_START_CONNECT:
			strTip="正在建立服务器中转连接";
			break;*/
		}

		m_wndP2PFileTransProgressBar.SetItemProgressTip(nItem,strTip);
	}
}

void CDuiP2PSendMsgDlg::AF_OnSendFileReturn(uint32 fileId,bool bAccept)
{

	uint16 file_id = fileId;

	int n = m_vecSendFile.size();

	for ( int i=0; i<n; i++ )
	{
		if ( m_vecSendFile[i].nID==file_id )
		{
			if ( bAccept )//对方同意接受
			{
				//m_reditHistory.SetFileBarTip(file_id,ODT_SEND_FILE,"对方同意了接收该文件！");
				ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
				RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, file_id,ODT_SEND_FILE,"对方同意了接收该文件！");

				ShowFileTransInfoBar(true);

				m_wndP2PFileTransProgressBar.AddItem(m_vecSendFile[i].strFile,m_vecSendFile[i].nSize,m_vecSendFile[i].nID,TRUE);
			}
			else
			{
				//m_reditHistory.SetFileBarTip(file_id,ODT_SEND_FILE,"对方拒绝接收！",RGB(255,0,0) );
				ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
				RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, file_id,ODT_SEND_FILE,"对方拒绝接收！",RGB(255,0,0) );
			}

			m_vecSendFile.erase(m_vecSendFile.begin()+i);
			break;
		}
	}
}

void CDuiP2PSendMsgDlg::StopDownload(const CString& sFileID,bool bTerminate)
{
	map<CString,DOWNLOAD_OBJ*>::iterator it;
	it = m_dicDownloadObj.find(sFileID);
	if(it == m_dicDownloadObj.end())
	{
		return;
	}
	HANDLE hH = it->second->handle;

	if(bTerminate)
	{	
		try
		{
			TerminateThread(hH,0);
			CloseHandle(hH);
		}catch(...)
		{
		}
	}

	delete it->second;
	m_dicDownloadObj.erase(it);
	if(m_dicDownloadObj.size()==0)
	{
		KillTimer(*this, TIMER_DOWNLOADOBJ);
	}

}


void CDuiP2PSendMsgDlg::Download(const vector<DOWNLOAD_OBJ>& arrTask)
{
	CWinThread * pThread;
	size_t nC = arrTask.size();
	if(nC == 0)
		return ;
	CString sMD5;
	for(size_t i = 0;i < nC; i++)
	{
		// 先获得该文件的md5名
		GetMD5WebFileName(arrTask[i].sUrl,sMD5);		
		// 下载前检测任务是否正在下载
		if(m_dicDownloadObj.find(sMD5) != m_dicDownloadObj.end())
		{
			continue;
		}
		DOWNLOAD_OBJ * pParam = new DOWNLOAD_OBJ(arrTask[i]);
		pThread = AfxBeginThread(DownloadPicProc,pParam);
		if(pThread)
			pParam->handle = (HANDLE)*pThread;
		// 设置下载任务的检测定时器
		if(m_dicDownloadObj.size()==0)
		{
			SetTimer(*this, TIMER_DOWNLOADOBJ,300,NULL);
		}
		// 保存当前项到字典
		m_dicDownloadObj[sMD5] = pParam;
	}
}

LRESULT CDuiP2PSendMsgDlg::OnUserDragDropFile(WPARAM w,LPARAM l)
{

	if ( m_pmP2PMode==pmMultiTalk )
	{
		AddMsgTip("对不起！多人聊天不能发送文件！请先和他/她单聊，然后发送文件。谢谢！");
	}
	else
	{ 
		CString strFile = (char *)w;	
		P2PSendFile(strFile);	
	}

	return 0;
}

LRESULT CDuiP2PSendMsgDlg::OnInsertMsg(WPARAM w, LPARAM l)
{
	LPCTSTR szMsg=(LPCTSTR)l;
	if(szMsg!=NULL)
	{
		//m_reditSend.InsertText(szMsg);
		//m_reditSend.SetFocus();
		m_reditSend->InsertText(-1, szMsg);
		SetEditFocus();
	}
	return 0;
}

void CDuiP2PSendMsgDlg::OnLinkResClicked()
{
	CWnd* pParent = CWnd::FromHandle(GetDesktopWindow());
	CLinkResDlg* pDlg =  CLinkResDlg::NewDlg(CWnd::FromHandle(*this),pParent);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(CWnd::FromHandle(GetDesktopWindow()));

}


void CDuiP2PSendMsgDlg::OnStartUploadCPicAck(const char * data,WORD wRet)
{
	int nFileId = (int)data;
	if(wRet != 0)
		return;

	SF_StartUploadCfileAck(nFileId);
}

void CDuiP2PSendMsgDlg::OnUploadCPicAck(const char * data,WORD wRet)
{
	LPXT_UPLOAD_CPIC_ACK pAck = (LPXT_UPLOAD_CPIC_ACK)data;
	if(	wRet == 0 && 
		pAck->nCid == m_clusterInfo.nFamilyID)
	{
		SF_UploadCfileAck(pAck->nFileId);
	}
}

void CDuiP2PSendMsgDlg::OnDownloadCPicAck(const char * data,WORD wRet)
{
	LPXT_DOWNLOAD_CPIC_ACK pAck = (LPXT_DOWNLOAD_CPIC_ACK)data;
	if(pAck->nCId == m_clusterInfo.nFamilyID)
	{
		SF_DownloadCfileAck(*pAck);
	}
}

void CDuiP2PSendMsgDlg::OnGetFriendBasicInfoAck(const char *data,WORD wRet)
{
	LPXT_GET_FRIEND_BASICINFO_NOTIFY pAck = (LPXT_GET_FRIEND_BASICINFO_NOTIFY)data;
	size_t j;
	int nFriend;
	CLUSTER_INFO * pCluster = NULL;
	FRIEND_INFO *pFriend    = NULL;
	bool bNeedUpdate        = false;

	if(m_pmP2PMode!=pmMultiTalk)
	{
		return;
	}
	if(wRet != 0)
	{
		return;
	}

	bNeedUpdate = false;
	// 查找更新群成员资料
	for ( j = 0; j < pAck->count; j++ )
	{
		nFriend = m_clusterInfo.FindMember(pAck->FriendInfo[j].nID);
		if(nFriend < 0)
			continue;
		bNeedUpdate = true;

		pFriend = &m_clusterInfo.members[nFriend];
		pFriend->bBasicInfo = true;
		//		pFriend->face = pAck->FriendInfo[j].nHeadId;
		pFriend->sex = pAck->FriendInfo[j].nSexy;
		pFriend->SetUserName(pAck->FriendInfo[j].szUserName);
		strcpy(pFriend->nickname,pAck->FriendInfo[j].szNickName);
		strcpy(pFriend->comment_name,pAck->FriendInfo[j].szCommentName);
		strcpy(pFriend->user_sign,pAck->FriendInfo[j].szUserSign);
		pFriend->SetGuid(pAck->FriendInfo[j].szGUID);//新增自营GUID标识
	}
	if(bNeedUpdate)
	{
		m_wndUserList.UpdateClusterList();	
	}
}
#ifdef _BUSINESS_VESION
void CDuiP2PSendMsgDlg::OnGetNearCommCustRemarkAck(const char *data,WORD wRet)
{
	LPXT_GET_NEAR_COMM_CUST_REMARK_ACK pAck = (LPXT_GET_NEAR_COMM_CUST_REMARK_ACK)data;

	if(pAck->ret == 0)
	{
		if(pAck->uiBusinessID == g_ImUserInfo.GetId() && strcmp(m_FriendInfo.GetUserName().c_str(), pAck->szCommAccount) == 0)
		{
			m_strNewComnCustRemarkServer = pAck->szRemarks;
			m_strNewComnCustRemarkClient = pAck->szRemarks;
			
			int nBeg = m_strAdUrl.Find("userComment=");
			if(nBeg > 0)
			{
				nBeg += strlen("userComment=");
				int nEnd = m_strAdUrl.Find("&", nBeg);
				if(nEnd == -1 )
					nEnd = m_strAdUrl.GetLength();
				if(nEnd >= nBeg)
				{
					CString csRemark = m_strAdUrl.Mid(nBeg , nEnd - nBeg);

					CString csUserComment = "userComment=" + csRemark;
					CString csNew = "userComment=";
					csNew = csNew + pAck->szRemarks;
					m_strAdUrl.Replace(csUserComment, csNew);

					//m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
					SetTimer(GetHWND(), TIMER_UPDATEURL,500,NULL);
				}
			}
		}
	}
}

void CDuiP2PSendMsgDlg::OnUpdateNearCommCustRemarkAck(const char *data,WORD wRet)
{
	LPXT_UPDATE_NEAR_COMM_CUST_REMARK_ACK ack = (LPXT_UPDATE_NEAR_COMM_CUST_REMARK_ACK)data;
	//assert(ack->ret == 0);
}
#endif
	
typedef  BOOL  (_cdecl *GetIPInfoPrc)(unsigned int nIp,const char ** szZone,const char ** szName);

void CDuiP2PSendMsgDlg::OnAccIp()
{
	if(m_lblIP.IsWindowVisible() == FALSE)
	{
		CString sHide;
		in_addr ip;
		BOOL bRt = FALSE;
		/*ip.s_addr = g_pProtocol->GetIMServerIP();
		sHide = inet_ntoa(ip);
		sHide.Format("服务器:%s\t",);*/
		if(m_lblIP.m_ip != 0)
		{
			CString sZone;
			CString sName;
			CString sIP;
			uint32 nIP;
			const char * szZone = "";
			const char * szName = "" ;

			HMODULE hDll = ::LoadLibrary("IPQuery.dll");
			if(hDll)
			{
				GetIPInfoPrc p = (GetIPInfoPrc)::GetProcAddress(hDll,"GetIPInfo");
				if(p)
				{
					nIP = ntohl(m_lblIP.m_ip);
					bRt = p(nIP,&szZone,&szName);
					sZone = szZone;
					sName = szName;
				}
				::FreeLibrary(hDll);
				ip.s_addr = m_lblIP.m_ip;
				sIP = inet_ntoa(ip);
				sHide.Format("好友:%s \n%s %s",(LPCTSTR)sIP ,sZone,sName);
			}
			else
			{
				CString sSvr;
				in_addr ip;
				ip.s_addr = g_pProtocol->GetIMServerIP();
				sSvr.Format("服务器:%s",inet_ntoa(ip));
				ip.s_addr = m_lblIP.m_ip;
				sHide.Format("%s \t\t好友:%s:%d",(LPCTSTR)sSvr, inet_ntoa(ip),m_lblIP.m_port);
			}	
		}
		else
		{
			in_addr ip;
			ip.s_addr = g_pProtocol->GetIMServerIP();
			sHide.Format("服务器:%s",inet_ntoa(ip));
			//sHide = "未知";
		}
		m_lblIP.SetWindowText(sHide);
		m_lblIP.ShowWindow(SW_SHOW);
	}
	else
	{
		m_lblIP.ShowWindow(SW_HIDE);
	}

	//CString s;
	//s.Format("resend = %d,qresend=%d",g_nResend,g_nQResend);
	//g_nResend = 0;
	//g_nQResend = 0;
	//m_lblIP.SetWindowText(s);


}

LRESULT CDuiP2PSendMsgDlg::OnRichEditLinkClilck(WPARAM wParam, LPARAM lParam)
{
	uint32 file_id = lParam;

	switch ( wParam )
	{
	case RN_FILE_SAVEAS:////另存为
		{
			CString strDefaultFileName;

			for ( int i=0; i<m_vecRecvFile.size(); i++ )
			{
				if ( m_vecRecvFile[i].nID==file_id )
				{
					strDefaultFileName = m_vecRecvFile[i].strFile;
					break;
				}
			}

			CFileDialog dlg(FALSE,"",(LPCTSTR)strDefaultFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"",CWnd::FromHandle(*this),0);

			if ( dlg.DoModal()==IDOK )
			{
				CString strNewFile = dlg.GetPathName();
				//m_reditHistory.SetFileBarTip(file_id,ODT_RECV_FILE,"另存为 "+strNewFile);			
				ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
				RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, file_id,ODT_RECV_FILE,"另存为 "+strNewFile);	

				if ( FileExist(strNewFile) )
					DeleteFile(strNewFile);

				if ( FileExist(strNewFile+".~tmp" ) )
				{
					CString strPath,strName,strExt;
					ExtractFilePath(strNewFile,strPath,strName,strExt);

					for ( int i=0; i<1000; i++ )
					{
						CString strDestFile = strPath+strName+"("+IntToStr(i+1)+")"+strExt;
						if ( FileExist(strDestFile)==FALSE )
						{
							strNewFile = strDestFile;
							break;
						}
					}
				}

				AF_SetRecvFilePath(file_id,strNewFile);
				P2PSendFileReplay(file_id,true);
			}
		}
		break;
	case RN_FILE_ACCEPT: ////接受
		{
			//m_reditHistory.SetFileBarTip(file_id,ODT_RECV_FILE,"您同意了接收该文件");
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, file_id,ODT_RECV_FILE,"您同意了接收该文件");
			////////////////////////////////////////////////////////////////////////
			P2PSendFileReplay(file_id,true);
		}
		break;
	case RN_FILE_REFUSE:////拒绝
		{
			//m_reditHistory.SetFileBarTip(file_id,ODT_RECV_FILE,"您拒绝接收该文件",RGB(255,0,0));
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory, file_id,ODT_RECV_FILE,"您拒绝接收该文件",RGB(255,0,0));
			P2PSendFileReplay(file_id,false);
		}
		break;
	case RN_FILE_CANCEL:////取消
		{
			//P2PID id;
			//id.fid = file_id;
			//id.uid = m_FriendInfo.id;
			AF_AbortSendFile(file_id);

			//m_reditHistory.SetFileBarTip(file_id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory,file_id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));

			for ( int i=0; i<m_vecSendFile.size();i++ )
			{
				if ( m_vecSendFile[i].nID==file_id )
				{
					m_vecSendFile.erase(m_vecSendFile.begin()+i);
					break;
				}
			}
		}
		break;
	case RN_FILETRANS_SETTING:
		{
			std::tr1::shared_ptr<CSystemSettingDlg> pDlg = CSystemSettingDlg::Show();
			pDlg->ShowTab(1, 3);

		}
		break;
	}

	return 0;
}

void CDuiP2PSendMsgDlg::AF_OnRecvSpeedNotify(uint32 id,int nBPS,float fPercent)
{

	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,FALSE);

	if ( nItem!=-1 )
	{
		CString str;
		str.Format("接收速度:%.2fKb/s,已接收%.2f%%",nBPS/1024.0,fPercent);
		m_wndP2PFileTransProgressBar.SetItemProgressPos(nItem,fPercent);
		m_wndP2PFileTransProgressBar.SetItemProgressTip(nItem,str);
	}
}


void CDuiP2PSendMsgDlg::AF_OnSendSpeedNotify(uint32 id,int nBPS,float fPercent)
{

	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,TRUE);
	if ( nItem!=-1 )
	{
		CString str;
		str.Format("发送速度:%.2fKb/s,已发送%.2f%%",nBPS/1024.0,fPercent);
		m_wndP2PFileTransProgressBar.SetItemProgressPos(nItem,fPercent);
		m_wndP2PFileTransProgressBar.SetItemProgressTip(nItem,str);
	}
}

LRESULT CDuiP2PSendMsgDlg::OnRecvFileFailed(WPARAM wParam, LPARAM lParam)
{
	uint32  id = wParam;
	FILE_TRAN_ERR err = (FILE_TRAN_ERR)lParam;

	CP2PRecvFile *pFile = AF_FindRecvFile(id);

	if ( pFile )
	{
		CString sFilePath = pFile->GetFilePath();
		CString sFileName = pFile->GetFileName();
		int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,FALSE);

		if ( nItem!=-1 )
		{
			m_wndP2PFileTransProgressBar.DeleteItem(nItem);

			if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
			{
				ShowFileTransInfoBar(false);
			}

			if ( err==FTE_DOWNLOADFILE_NOTEXIST )
			{
				CString strTip;
				strTip.Format("%s 取消了发送文件\"%s\"",m_FriendInfo.nickname,sFileName );
				AddMsgTip(strTip,RGB(255,0,0));	
			}
			else
			{
				CString str;
				str.Format("文件 \"%s\" 接收失败！",sFilePath);
				AddMsgTip(str,RGB(255,0,0));
			}

			pFile->SaveFilePosition(m_FriendInfo.GetId());
		}
	}
	else
	{	
		for ( int i=0; i<m_vecRecvFile.size(); i++ )
		{
			if ( m_vecRecvFile[i].nID == id  )
			{				
				int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id ,FALSE);

				if ( nItem!=-1 )
				{
					m_wndP2PFileTransProgressBar.DeleteItem(nItem);

					if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
					{
						ShowFileTransInfoBar(false);
					}
				}

				CString str;
				str.Format("文件 \"%s\" 接收失败！",m_vecRecvFile[i].strFile);
				AddMsgTip(str);

				m_vecRecvFile.erase(m_vecRecvFile.begin()+i);

				break;
			}
		}
	}

	return 0;
}

void CDuiP2PSendMsgDlg::AF_OnDownloadFailed(uint32  id,FILE_TRAN_ERR err)
{
	SendMessage(WM_FILERECV_FAILED,id,(LPARAM)err);	
}

LRESULT CDuiP2PSendMsgDlg::OnRecvFileOK(WPARAM wParam,LPARAM lParam)
{
	uint32 id = wParam;
	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,FALSE);

	if ( nItem!=-1 )
	{
		m_wndP2PFileTransProgressBar.DeleteItem(nItem);

		if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
		{
			ShowFileTransInfoBar(false);
		}

		CP2PRecvFile *pFile = AF_FindRecvFile(id);

		if ( pFile )
		{
			CString str;
			str.Format("文件 \"%s\" 接收完成。",pFile->GetFilePath());
			AddMsgTip(str,RGB(0,0,255));
			AddFileOpenTip();//添加打开文件和打开文件夹

			XT_MSG msg;
			msg.msg_type = 0;
			msg.from_id  = g_ImUserInfo.GetId();
			msg.fontSize = 10;
			msg.fontColor = RGB(0,0,255);
			msg.send_time = /*time(0)*/g_LocalData.GetTimeNow();

			strcpy(msg.fontName,"宋体");
			strcpy(msg.data,(LPCTSTR)str);
			strcpy(msg.from_nickname,g_ImUserInfo.nickname);

			CString    strCfgFile = CString(pFile->GetFilePath())+".cfg";
			DeleteFile(strCfgFile);

			g_LocalData.SaveTalkMsg(msg,m_FriendInfo.GetId());
		}

		for ( int i=0; i<m_vecRecvFile.size(); i++ )
		{
			if ( m_vecRecvFile[i].nID==id )
			{
				m_vecRecvFile.erase(m_vecRecvFile.begin()+i);
				break;
			}
		}
	}	

	return 0;
}

void CDuiP2PSendMsgDlg::AF_OnDownloadOK(uint32 id)
{
	SendMessage(WM_FILERECV_OK,id);
}

LRESULT CDuiP2PSendMsgDlg::OnSendFileOK(WPARAM wParam,LPARAM lParam)
{
	uint32 id = wParam;

	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,TRUE);

	if ( nItem!=-1 )
	{
		m_wndP2PFileTransProgressBar.DeleteItem(nItem);

		if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
		{
			ShowFileTransInfoBar(false);
		}

		CP2PSendFile *pFile = AF_FindSendFile(id);
		if ( pFile )
		{
			CString str;
			str.Format("文件 \"%s\" 发送完成。",pFile->GetFileName());
			AddMsgTip(str);

			XT_MSG msg;
			msg.msg_type = 0;
			msg.from_id  = m_FriendInfo.GetId();
			msg.fontSize = 10;
			msg.fontColor = RGB(0,0,255);
			msg.send_time = /*time(0)*/g_LocalData.GetTimeNow();

			strcpy(msg.fontName,"宋体");
			strcpy(msg.data,(LPCTSTR)str);
			strcpy(msg.from_nickname,m_FriendInfo.nickname);


			g_LocalData.SaveTalkMsg(msg,m_FriendInfo.GetId());
		}

		for ( int i=0; i<m_vecSendFile.size(); i++ )
		{
			if ( m_vecSendFile[i].nID==id )
			{
				m_vecSendFile.erase(m_vecSendFile.begin()+i);
				break;
			}
		}
	}

	return 0;
}

void CDuiP2PSendMsgDlg::AF_OnSendOK(uint32 id)
{
	SendMessage(WM_FILESEND_OK,id,0);
}


LRESULT CDuiP2PSendMsgDlg::OnSendFileFailed(WPARAM wParam,LPARAM lParam)
{
	uint32 id = wParam;
	FILE_TRAN_ERR err=(FILE_TRAN_ERR)lParam;

	CP2PSendFile *pFile = AF_FindSendFile(id);

	if ( pFile )
	{	
		int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(id,TRUE);

		if ( nItem!=-1 )
		{
			m_wndP2PFileTransProgressBar.DeleteItem(nItem);

			if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
			{
				ShowFileTransInfoBar(false);
			}

			CString str;
			str.Format("文件 \"%s\" 发送失败，请重新再试！",pFile->GetFileName());
			AddMsgTip(str,RGB(255,0,0));
		}
		else
		{
			int n=m_vecSendFile.size();

			for ( int i=0; i<n; i++ )
			{
				if ( m_vecSendFile[i].nID== id  )
				{
					CString str;
					str.Format("文件 \"%s\" 发送失败，请重新再试！",m_vecSendFile[i].strFile);
					//m_reditHistory.SetFileBarTip(id ,ODT_SEND_FILE,str,RGB(255,0,0));
					ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
					RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory,id ,ODT_SEND_FILE,str,RGB(255,0,0));
					m_vecSendFile.erase(m_vecSendFile.begin()+i);
					break;
				}
			}
		}	
	}
	//else
	//{
	//	ShowMessage("<发送文件失败>");
	//}
	return 0;
}

void CDuiP2PSendMsgDlg::AF_OnSendFailed(uint32 id,FILE_TRAN_ERR err)
{
	SendMessage(WM_FILESEND_FAILED,id,(LPARAM)err);
}

void CDuiP2PSendMsgDlg::AF_OnRecvAbortTranReq(const P2P_ABORT_TRAN_REQ & req)
{
	if (req.friendId != m_FriendInfo.GetId() )
		return;

	BOOL bSendFile = (req.type==_P2P_ABORT_TRAN_REQ::STOP_SEND? FALSE : TRUE);
	int nItem = m_wndP2PFileTransProgressBar.FindItemByItemData(req.fid,bSendFile);	

	//P2PID id;
	uint32 fileId = req.fid;
	/*id.fid = req.fid;
	id.uid = m_FriendInfo.id;*/

	if ( bSendFile )
	{
		CP2PSendFile *pFile = AF_FindSendFile(fileId);


		if ( pFile )
		{
			CString strTip;
			strTip.Format("%s 取消接收文件\"%s\"",m_FriendInfo.nickname,pFile->GetFileName());
			AddMsgTip(strTip,RGB(255,0,0));	
		}
	}
	else
	{
		CP2PRecvFile *pFile = AF_FindRecvFile(fileId);

		if ( pFile )
		{
			CString strTip;
			strTip.Format("%s 取消发送文件\"%s\"",m_FriendInfo.nickname,pFile->GetFileName() );
			AddMsgTip(strTip,RGB(255,0,0));	

			// 现在暂时不实现断点续传功能
			//pFile->SaveFilePosition(m_FriendInfo.GetId());
		}
		else
		{
			int n=m_vecRecvFile.size();
			for ( int i=0; i<n; i++ )
			{
				if ( m_vecRecvFile[i].nID== fileId )
				{
					CString strTip;
					strTip.Format("%s 取消了发送该文件",m_FriendInfo.nickname);
					//m_reditHistory.SetFileBarTip(fileId ,ODT_RECV_FILE,strTip,RGB(255,0,0));
					ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
					RichEdit_SetFileBarTip(m_reditHistory, pTextServicesHistory,fileId ,ODT_RECV_FILE,strTip,RGB(255,0,0));
					m_vecRecvFile.erase(m_vecRecvFile.begin()+i);
					break;
				}
			}
		}
	}

	if ( nItem!=-1 )
	{
		m_wndP2PFileTransProgressBar.DeleteItem(nItem);

		if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
			ShowFileTransInfoBar(false);
	}

}


void CDuiP2PSendMsgDlg::OnP2PTestAck(const char * data, WORD wRet)
{	
	LPXT_P2PTEST_ACK pAck = (LPXT_P2PTEST_ACK)data;
	FRIEND_INFO * pFriend;
	uint32 id;
	if( wRet == 0 )
	{
		pFriend = FindFriend(pAck->id);
		// 与该好友的连接成功
		if(pFriend)
		{
			if(pAck->testType == XT_P2PTEST_ACK::LAN)
			{
				pFriend->connFlag = FRIEND_INFO::LAN;
				//AF_OnP2PTestOK(pAck);
			}
			else
			{
				pFriend->connFlag = FRIEND_INFO::P2P;
				pFriend->p2pRetry = 5;
				//AF_OnP2PTestOK(pAck);
			}
		}
	}
	else if( wRet == SERVER_TIMEOUT )
	{
		// 与该好友的连接测试失败
		SendDataStruct * pSend = (SendDataStruct *)data;
		LPXT_HEAD pH = (LPXT_HEAD)pSend->data;
		id = pH->did;
		uint8 ipType = *((uint8*)(pSend->data + sizeof(XT_HEAD)));
		pFriend = FindFriend(id);
		if(pFriend)
		{
			// 内网测试失败，继续公网p2p测试
			if(ipType == XT_P2PTEST_IND::LAN)
			{
				pFriend->connFlag = FRIEND_INFO::TRY_P2P;
				// 公网p2p测试
				XT_P2PTEST_REQ req;
				req.dest_ip = pFriend->ip;
				req.dest_port = pFriend->port;
				req.ipType = XT_P2PTEST_REQ::P2P;
				req.id = g_ImUserInfo.GetId();
				req.toid = pFriend->GetId();
				g_pProtocol->SendP2pTestPacket(req);
			}
			// 公网p2p测试失败，则由服务器中转通讯
			else if(ipType == XT_P2PTEST_IND::P2P)
			{
				pFriend->ip = 0;
				pFriend->port = 0;
				pFriend->connFlag = FRIEND_INFO::SERVER;
				//AF_OnP2PTestFailed();
			}
		}
	}
}

void CDuiP2PSendMsgDlg::OnBnClickedButtonHistory()
{
	m_bShowHistory = !m_bShowHistory;

	CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

	int nSel = m_pTabRight->GetCurSel();
	if(nSel == 1)
	{	// 表示取消显示历史消息
		m_pTabRight->SelectItem(0);
		m_pTabRight->SetFixedWidth(AD_WINDOW_WIDTH);
#ifdef _CS_PLAT_EX_
		if(!pbtnRestore->IsVisible())
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
		}
#else
		if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()))
		{
			if(!IsMaximized(*this))
			{
				CRect rc;
				::GetWindowRect(*this, &rc);
				MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
			}
		}
		else
		{
			if(!pbtnRestore->IsVisible())
			{
				CRect rc;
				::GetWindowRect(*this, &rc);
				MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
			}
		}
#endif
	}
	else
	{
		m_wndPopupHistory.LoadAllMsg();	
		m_pTabRight->SelectItem(1);
		m_pTabRight->SetFixedWidth(388);
		if(nSel == 0)
		{
#ifdef _CS_PLAT_EX_
			if(!pbtnRestore->IsVisible())
			{
				CRect rc;
				::GetWindowRect(*this, &rc);
				MoveWindow(*this, rc.left, rc.top, rc.Width() + (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
			}
#else
			if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()))
			{
				if(!IsMaximized(*this))
				{
					CRect rc;
					::GetWindowRect(*this, &rc);
					MoveWindow(*this, rc.left, rc.top, rc.Width() + (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
				}
			}
			else
			{
				if(!pbtnRestore->IsVisible())
				{
					CRect rc;
					::GetWindowRect(*this, &rc);
					MoveWindow(*this, rc.left, rc.top, rc.Width() + (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
				}
			}
#endif
		}
	}
	/*
	CVerticalLayoutUI* pVlWebright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLwebright")));
	CVerticalLayoutUI* pVlHistoryright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhistoryright")));
	CVerticalLayoutUI* pVlFastWord = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLFastWord")));
	if(pVlWebright->IsVisible())
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() + 103, rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(true);
		CRect rc = pVlHistoryright->GetPos();
		if(rc == CRect(0,0,0,0))
		{
			GetWindowRect(GetHWND(), &rc);
			m_wndPopupHistory.UpdateEditRect(rc);
		}

		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(true);
		pVlFastWord->SetVisible(false);
	}
	else if(pVlFastWord->IsVisible())
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(true);
		CRect rc = pVlHistoryright->GetPos();
		if(rc == CRect(0,0,0,0))
		{
			GetWindowRect(GetHWND(), &rc);
			m_wndPopupHistory.UpdateEditRect(rc);
		}

		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(true);
		pVlFastWord->SetVisible(false);
	}
	else
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - 103, rc.Height(), TRUE);
		}

		pVlWebright->SetVisible(true);
		pVlHistoryright->SetVisible(false);
		pVlFastWord->SetVisible(false);
	}
	*/
	//UpdateP2PPanel();
}

LRESULT CDuiP2PSendMsgDlg::OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam)
{
	CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

	m_pTabRight->SelectItem(0);
	m_pTabRight->SetFixedWidth(AD_WINDOW_WIDTH);
#ifdef _CS_PLAT_EX_
	if(!pbtnRestore->IsVisible())
	{
		CRect rc;
		::GetWindowRect(*this, &rc);
		MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
	}
#else
	if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()))
	{
		if(!IsMaximized(*this))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
		}
	}
	else
	{
		if(!pbtnRestore->IsVisible())
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
		}
	}
#endif
	
	m_bShowHistory = !m_bShowHistory;
	/*
	if (m_bShowHistory)
	{
		m_wndPopupHistory.LoadAllMsg();
	}

	//UpdateP2PPanel();
	CVerticalLayoutUI* pVlWebright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLwebright")));
	CVerticalLayoutUI* pVlHistoryright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhistoryright")));
	if(pVlWebright->IsVisible())
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() + 103, rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(true);
	}
	else
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - 103, rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(true);
		pVlHistoryright->SetVisible(false);
	}
	*/
	return S_OK;
}

void CDuiP2PSendMsgDlg::MM_InitXtMsg(XT_MSG &msg)
{
	memset(&msg,0,sizeof(msg));

	msg.ver         = XTREAM_CLIENT_VERSION;
	//	msg.face		= g_ImUserInfo.face;
	msg.from_id     = g_ImUserInfo.GetId();

	msg.fontColor   = g_ImFont.fontColor;
	msg.fontSize    = g_ImFont.fontSize;
	msg.fontStyle   = g_ImFont.flags;
	strcpy(msg.fontName,g_ImFont.fontName);
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

	msg.send_time   = time(0);

	msg.to_id		= m_FriendInfo.GetId();
	msg.dest_ip		= m_FriendInfo.ip;
	msg.dest_port	= m_FriendInfo.port;
	msg.msg_type    = 0;

}

void CDuiP2PSendMsgDlg::OnBnClickedButtonShake()
{
	WORD nVK = 0;
	// 是否允许振屏不是控制发送端
	//g_LocalData.GetShakeWinConfig(nVK);
	//if(nVK == 0)////本地支持振屏
	//{
		int nShakeTime = /*time(0)*/g_LocalData.GetTimeNow();

		//抖动过于频繁判断
		if(nShakeTime - m_nLastShakeTime < 10)
		{
			AddMsgTip("您发送窗口抖动过于频繁，请稍候再发");
		}
		else
		{
			XT_MSG msg;
			MM_InitXtMsg(msg);

			msg.data_type = XT_MSG::WIN_SHAKE;
			CSendPacket sp(msg.data);
			strcpy(msg.from_nickname,m_FriendInfo.nickname);

			CString text = "您发送了一个窗口抖动~";

			strcpy(msg.data,text);
			msg.data_len = strlen(text)+1;

			AddTalkMsg(msg,false);
			g_pProtocol->SendP2PMsg(msg);
			/////本地抖动屏蔽	
			SetTimer(*this, TIMER_WIN_SHAKE,20,NULL);
			m_nWinShakeCnt = 0;
			m_nLastShakeTime = nShakeTime;
		}
}

void CDuiP2PSendMsgDlg::OnBnClickedBtnHotkey()
{
	// TODO: 在此添加控件通知处理程序代码
	//CRect rc;
	//m_btnSend.GetWindowRect(&rc);

	m_pBtnSendConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnsendconfig")));
	RECT rc = m_pBtnSendConfig->GetPos();
	RECT rcParent;
	::GetWindowRect(*this, &rcParent);
	rc.left += rcParent.left;
	rc.top += rcParent.top;
	rc.right += rcParent.left;
	rc.bottom += rcParent.top;

	DuiLib::CPoint p;
	int	id;

	// create it
	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(hInstance);

	// add some items
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();

	if(cfg->sendMsgKey == SMK_ENTER)
	{
		menu.AppendItem(0,"按Enter键发送消息",1,0);
		menu.AppendItem(0,"按Ctrl+Enter键发送消息",2);
	}
	else
	{
		menu.AppendItem(0,"按Enter键发送消息",1);
		menu.AppendItem(0,"按Ctrl+Enter键发送消息",2,0);
	}

	// track it

	p.x=rc.right - 24;
	p.y=rc.bottom+2;

	id=menu.Track(p.x,p.y,*this,true);

	// 选择结果
	switch (id)
	{
	case 1:
		{
			OnEnter();
		}
		break;
	case 2:
		{
			OnCtrlenter();
		}
		break;
	}
}


LRESULT  CDuiP2PSendMsgDlg::OnSnapShot(WPARAM wParam,LPARAM lParam)
{
	OnScreenCopy();
	return S_OK;
}


void CDuiP2PSendMsgDlg::OnEnter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_ENTER;
}


void CDuiP2PSendMsgDlg::OnCtrlenter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_CTRLENTER;
}

// 显示或隐藏评分按钮
void CDuiP2PSendMsgDlg::ShowOrHideScoreBtn(BOOL bShow)
{
	if (IsWindow(*this))
	{
		m_wndToolBar.ShowOrHideScoreBtn(bShow);
	}
}

// “推送评价”按钮
void CDuiP2PSendMsgDlg::OnBnClickedButtonScore()
{
#ifdef _BUSINESS_VESION
	XT_MSG msg;
	MM_InitXtMsg(msg);

	msg.data_type = XT_MSG::PUSH_EVALUA;
	CSendPacket sp(msg.data);
	strcpy(msg.from_nickname, m_FriendInfo.nickname);

	CString text = _T("给您发了一份评价表，请您对我们的服务做出评价");
	strcpy(msg.data, text);
	msg.data_len = strlen(text) + 1;

	g_pProtocol->SendP2PMsg(msg);

	text = _T("您成功发送服务评价表");
	strcpy(msg.data, text);
	msg.data_len = strlen(text) + 1;
	AddTalkMsg(msg, false);
#endif
}

// 快捷短语消息相应
void CDuiP2PSendMsgDlg::OnCommandFastWordMenu(int id)
{
	int nIndex = id;
	if(nIndex == m_vecFastWord.size())
	{
		std::tr1::shared_ptr<CSystemSettingDlg> pDlg = CSystemSettingDlg::Show();
		pDlg->ShowTab(0, 6);
		return;
	}

	if(nIndex >= 0 && nIndex < m_vecFastWord.size())
	{
		string strMsg = m_vecFastWord[nIndex];

		XT_MSG msg;
		SF_InitXtMsg(msg);

		msg.data_type   = 0;	//即时聊天消息

		CString text(strMsg.c_str());
		text.Trim();

		if ( text.GetLength()>800 )
		{
			//MessageBox("最多输入900个英文字符或450个汉字!");
			//CMessageBox::Prompt("最多输入800个英文字符或650个汉字!");
			AddMsgTip("你发送消息内容超长，请分条发送。");
			return ;
		}

		if ( FindForbiddenWords(text) )
		{
			//m_reditSend.SetFocus();
			SetEditFocus();
			AddMsgTip("你发送的消息中有限制用语，消息发送失败！");
			return ;
		}

		if ( text.GetLength()==0 )
		{
			//m_reditSend.SetFocus();
			SetEditFocus();
			return ;
		}

		strcpy(msg.data,text);
		msg.data_len=strlen(text)+1;

		// 检测消息中是否有定义信息，并作相应转化
		//CString sShowMsg;
		//ProcTalkMsg(msg.data,sShowMsg);
		XT_MSG msgShow = msg;
		//modify by amin
		//strncpy(msgShow.data,sShowMsg,sizeof(msgShow.data)-1);
		AddTalkMsg(msgShow,false);

		if ( m_pmP2PMode!=pmMultiTalk )
		{
			int  type = CIMProtocol::SMT_SERVER;
			FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
			if(pInfo == NULL)
			{
				type = CIMProtocol::SMT_SERVER;
			}
			// 如果没有获取过好友状态，则获取
			else if( pInfo->ip == 0)
			{	
				//if( pInfo->status != XTREAM_OFFLINE  )
				//{
				// 在允许的次数内重试获取好友最新的ip
				if(pInfo->p2pRetry > 0)
				{
					pInfo->connFlag = FRIEND_INFO::NONE;
					pInfo->p2pRetry--;
					// 获取当前好友的ip和端口
					RefreshFriendStatus();
				}
				else
				{
					// 无法重试了，则使用服务器中转
					pInfo->connFlag = FRIEND_INFO::SERVER;
				}
				//}
				// 通过服务器中转
				type = CIMProtocol::SMT_SERVER;
			}
			else 
			{
				// 好友状态已经获取，可以开始p2p测试
				if( pInfo->connFlag == FRIEND_INFO::NONE ||
					pInfo->connFlag == FRIEND_INFO::P2P_FAILED
					)
				{
					XT_P2PTEST_REQ req;
					// 如果公网ip相同，则可能在同一个内网，尝试内网连接
					if(pInfo->ip == g_ImUserInfo.ip)
					{
						pInfo->connFlag = FRIEND_INFO::TRY_LAN;
						req.dest_ip = pInfo->localIP;
						req.dest_port = pInfo->localPort;
						req.ipType = XT_P2PTEST_REQ::LAN;
					}
					else
					{
						pInfo->connFlag = FRIEND_INFO::TRY_P2P;
						req.dest_ip = pInfo->ip;
						req.dest_port = pInfo->port;
						req.ipType = XT_P2PTEST_REQ::P2P;	
					}
					req.id = g_ImUserInfo.GetId();
					req.toid = pInfo->GetId();
					// 发送p2p测试包
					g_pProtocol->SendP2pTestPacket(req);
				}

				// 确定数据发送方式
				//if(pInfo->connFlag == FRIEND_INFO::LAN)
				//{
				//	// 尝试内网连接
				//	type = CIMProtocol::SMT_LAN;
				//}
				//else if(pInfo->connFlag == FRIEND_INFO::P2P)
				//{
				//	//直接发送到P2P客户端
				//	type = CIMProtocol::SMT_P2P; 	
				//}
				//else
				//{
				//	type = CIMProtocol::SMT_SERVER;
				//}
				//数据发送方式全改为经过服务器
				type = CIMProtocol::SMT_SERVER;
			}

			g_pProtocol->SendP2PMsg(msg,type);

			SF_SetTransMode(type);

			strcpy(msgShow.from_nickname,g_ImUserInfo.nickname);
			g_LocalData.SaveTalkMsg(msgShow,m_FriendInfo.GetId());
			CMainFrame::GetInstance()->GetRecentList()->AddUser(&m_FriendInfo);
			RECENTLIST_OP(AddUser(&m_FriendInfo))
		}
		else
		{
			g_pProtocol->SendClusterMsg(msg);
			SaveClusterMsg(msgShow);
		}

		RegUserOperation(0x00c4);

		//m_reditSend.SetFocus();
		SetEditFocus();
	}	
}

// 快捷短语按钮消息相应
void CDuiP2PSendMsgDlg::OnBnClickedFastword(DuiLib::TNotifyUI& msg)
{
	CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

	int nSel = m_pTabRight->GetCurSel();
	if(nSel == 2)
	{	// 表示取消显示历史消息
		m_pTabRight->SelectItem(0);
		m_pTabRight->SetFixedWidth(AD_WINDOW_WIDTH);

#ifdef _CS_PLAT_EX_
		if(!pbtnRestore->IsVisible())
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
		}
#else
		if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()))
		{
			if(!IsMaximized(*this))
			{
				CRect rc;
				::GetWindowRect(*this, &rc);
				MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
			}
		}
		else
		{
			if(!pbtnRestore->IsVisible())
			{
				CRect rc;
				::GetWindowRect(*this, &rc);
				MoveWindow(*this, rc.left, rc.top, rc.Width() - (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
			}
		}
#endif
	}
	else
	{
		m_pTabRight->SelectItem(2);
		m_pTabRight->SetFixedWidth(388);
		if(nSel == 0)
		{
#ifdef _CS_PLAT_EX_
			if(!pbtnRestore->IsVisible())
			{
				CRect rc;
				::GetWindowRect(*this, &rc);
				MoveWindow(*this, rc.left, rc.top, rc.Width() + (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
			}
#else
			if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()))
			{
				if(!IsMaximized(*this))
				{
					CRect rc;
					::GetWindowRect(*this, &rc);
					MoveWindow(*this, rc.left, rc.top, rc.Width() + (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
				}
			}
			else
			{
				if(!pbtnRestore->IsVisible())
				{
					CRect rc;
					::GetWindowRect(*this, &rc);
					MoveWindow(*this, rc.left, rc.top, rc.Width() + (388 - AD_WINDOW_WIDTH), rc.Height(), TRUE);
				}
			}
#endif
		}
	}	
	/*
	CVerticalLayoutUI* pVlWebright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLwebright")));
	CVerticalLayoutUI* pVlHistoryright = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhistoryright")));
	CVerticalLayoutUI* pVlFastWord = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLFastWord")));

	if(pVlWebright->IsVisible())
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() + 103, rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(false);
		pVlFastWord->SetVisible(true);
	}
	else if(pVlHistoryright->IsVisible())
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(false);
		pVlHistoryright->SetVisible(false);
		pVlFastWord->SetVisible(true);
	}
	else
	{
		if(!(IsMaximized(*this) || IsMinimized(*this)))
		{
			CRect rc;
			::GetWindowRect(*this, &rc);
			MoveWindow(*this, rc.left, rc.top, rc.Width() - 103, rc.Height(), TRUE);
		}
		pVlWebright->SetVisible(true);
		pVlHistoryright->SetVisible(false);
		pVlFastWord->SetVisible(false);
	}
	*/
}

#ifdef _BUSINESS_VESION
// 账号挂起
void CDuiP2PSendMsgDlg::OnBnClickedBtnHangup()
{
	// 弹出菜单
	CRect rcBtn;
	// 	m_btnHangup.GetWindowRect(rcBtn);

	CButtonUI* pBtnConfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("configbtn")));
	rcBtn = pBtnConfig->GetPos();
	RECT rcParent;
	::GetWindowRect(*this, &rcParent);
	rcBtn.left += rcParent.left;
	rcBtn.top += rcParent.top;
	rcBtn.right += rcParent.left;
	rcBtn.bottom += rcParent.top;

	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(hInstance);
	menu.AppendItem(0, "挂起", 6, CMainFrame::GetInstance()->GetHangupSelect() ? 0 : -1);
	menu.AppendItem(CFlatPopupMenu::itemSeparator,NULL,-1);
	menu.AppendItem(0, "不自动挂起", 1, CMainFrame::GetInstance()->GetHangupMenuIndex() == 1 ? 0 : -1);
	menu.AppendItem(0, "20人挂起", 2, CMainFrame::GetInstance()->GetHangupMenuIndex() == 2 ? 0 : -1);
	menu.AppendItem(0, "50人挂起", 3, CMainFrame::GetInstance()->GetHangupMenuIndex() == 3 ? 0 : -1);
	menu.AppendItem(0, "100人挂起", 4, CMainFrame::GetInstance()->GetHangupMenuIndex() == 4 ? 0 : -1);
	menu.AppendItem(CFlatPopupMenu::itemSeparator,NULL,-1);
	static CString strCustomHangupText = "自定义挂起";
	menu.AppendItem(0,strCustomHangupText, 5, CMainFrame::GetInstance()->GetHangupMenuIndex() == 5 ? 0 : -1);

	DuiLib::CPoint p;
	int	id;

	// track it
	p.x=rcBtn.left;
	p.y=rcBtn.bottom+1;
	id=menu.Track(p.x,p.y,*this,true);

	if(id != 5 && id != 6 && id != 0)
	{
		strCustomHangupText = "自定义挂起";
	}
	switch(id)
	{
	case 1:
		{
			CMainFrame::GetInstance()->SetHangupMenuIndex(1);
			CMainFrame::GetInstance()->SetHangupStatusNeededSessions(-1);
			CMainFrame::GetInstance()->Hangup();
		}
		break;
	case 2:
		{
			CMainFrame::GetInstance()->SetHangupMenuIndex(2);
			CMainFrame::GetInstance()->SetHangupStatusNeededSessions(20);
			CMainFrame::GetInstance()->Hangup();
		}
		break;
	case 3:
		{
			CMainFrame::GetInstance()->SetHangupMenuIndex(3);
			CMainFrame::GetInstance()->SetHangupStatusNeededSessions(50);
			CMainFrame::GetInstance()->Hangup();
		}
		break;
	case 4:
		{
			CMainFrame::GetInstance()->SetHangupMenuIndex(4);
			CMainFrame::GetInstance()->SetHangupStatusNeededSessions(100);
			CMainFrame::GetInstance()->Hangup();
		}
		break;
	case 5:
		{	// 弹出自定义窗口
			CMainFrame::GetInstance()->SetHangupMenuIndex(5);
			int nNum = CMainFrame::GetInstance()->GetHangupStatusNeededSessions();
			CHangupSessionNumSettingDlg dlg(nNum, CWnd::FromHandle(*this));
			if(IDOK == dlg.DoModal())
			{
				nNum = dlg.GetNum();
				CMainFrame::GetInstance()->SetHangupStatusNeededSessions(nNum);
				strCustomHangupText.Format("自定义(%d人)", nNum);
				CMainFrame::GetInstance()->Hangup();
			}
		}
		break;
	case 6://手动挂起
		{
			if(CMainFrame::GetInstance()->GetHangupSelect())
			{
				HangupCancel();
				CMainFrame::GetInstance()->SetHangupSelect(false);
				CMainFrame::GetInstance()->Hangup();
			}
			else if(!CMainFrame::GetInstance()->GetHangupSelect())
			{
				if(CMessageBox::Model(NULL, "设置为\"我要挂起\"后，用户通过点击您网站上的\"在线客服\"发送的消息，将不会分流到您这里。您确定要挂起吗？", IM_CAPTION, MB_OKCANCEL | MB_ICONWARNING) == IDOK)
				{
					CMainFrame::GetInstance()->SetHangupSelect(true);
					CMainFrame::GetInstance()->Hangup();
				}
			}
		}
		break;
	}
}
#endif

// 处理鼠标移动消息，当鼠标移动到头像区域时，弹出好友资料窗口
// void CDuiP2PSendMsgDlg::OnMouseMove(UINT nFlags, CPoint point)
// {
// 	CXSkinDlg::OnMouseMove(nFlags, point);
// 
// 	CRect rc;
// 	m_ctrlFaceIcon.GetWindowRect(rc);
// 	ClientToScreen(&point);
// 	if(rc.PtInRect(point))
// 	{
// 		SetTimer(TIMER_SHOW_USERCARD, 500, NULL);
// 	}
// }

void CDuiP2PSendMsgDlg::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CClusterMemberSelectDlg dlg(&m_clusterInfo,CWnd::FromHandle(GetHWND()));
	 	if ( IDOK==dlg.DoModal() )
	 	{
	 		//更新组内群发成员
			UpdateCluserInfo(m_clusterInfo.nFamilyID);
	 	}
	 	else
	 	{
	 		return ;
	 	}
}

// 通知服务端打开了一个聊天窗口
void CDuiP2PSendMsgDlg::NotifyServer_OpenedMsgDlg()
{
#ifndef _BUSINESS_VESION
	XT_MSG msg;
	MM_InitXtMsg(msg);

	msg.data_type = XT_MSG::OPEN_P2PSENDMSG_DLG;
	if(strcmp(g_ImUserInfo.nickname, "") != 0)
		strcpy(msg.from_nickname, g_ImUserInfo.nickname);
	else
		strcpy(msg.from_nickname, g_ImUserInfo.GetUserName().c_str());

	strcpy(msg.data, g_ImUserInfo.GetGuid().c_str());
	msg.data_len = strlen(msg.data) + 1;

	g_pProtocol->SendP2PMsg(msg);
#endif
}

void CDuiP2PSendMsgDlg::NavigateComplete2WebAd(LPDISPATCH pDisp, VARIANT* URL)
{
	CString strUrl = (char *)(_bstr_t(URL)); 
	if(strUrl.Find(RIGHT_URL_SELLER) != -1 || strUrl.Find(RIGHT_URL) != -1)
	{
		//m_wndAD->Refresh();
	}
}

////服务端返回给B端的确认
void  CDuiP2PSendMsgDlg::OnRecvServerAck(const char *data,WORD wRet)
{
	if ( wRet == 0)
	{
		LPSEND_FILE_PACK_IND_ACK pAck = (LPSEND_FILE_PACK_IND_ACK)data;
		if ( pAck)
		{
			;
		}
	}
}

#ifdef _BUSINESS_VESION
// 客服转接
void CDuiP2PSendMsgDlg::OnBnClickedBtnCustserTran()
{
	m_pCustSerTranDlg = new CCustSerTranDlg;
	m_pCustSerTranDlg->Create(GetHWND(), _T("选择转移对象"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	m_pCustSerTranDlg->CenterWindow();
	if(0 == m_pCustSerTranDlg->ShowModal())
	{	// 点确认

		uint32 destid = m_pCustSerTranDlg->m_nSelFriendID;
		FRIEND_INFO *pDestFriend = ::FindFriend(destid);
#ifdef _CS_PLAT_EX_
		//客服工作台的客服转接
		XT_MSG msg;
		memset(&msg,0,sizeof(msg));
		msg.ver         = XTREAM_CLIENT_VERSION;
		msg.from_id     = g_ImUserInfo.GetId();
		msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
		msg.to_id		= destid;
		msg.data_type   = XT_MSG::CUST_SER_TRAN_REQ;//插入会话消息

		extern map<int, CString> g_mapFriendIdToOrder;
		CString text;// = g_mapFriendIdToOrder[m_FriendInfo.GetId()];
		text.Format("tranReq:%s:%d", UserInfoToStr(&m_FriendInfo), m_FriendInfo.GetId());
		//text.Format("%d:%s", m_FriendInfo.GetId(), text);
		strcpy(msg.data, text);
		msg.data_len=strlen(text)+1;
		//SF_SetTransMode(CIMProtocol::SMT_SERVER);
		g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);

		XT_FONT xf;
		xf.fontColor=RGB(254,154,105);

		
		m_reditHistory->SetSel(-1,-1);
		m_reditHistory->InsertText(-1, "\n");
		ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
		
		RichEdit_SetStartIndent(pTextServicesHistory, 80);

		CString strTranWait;
		strTranWait.Format("----等待客服\"%s\"转接确认。转接成功后，该窗口将自动关闭。", pDestFriend->nickname);
		m_reditHistory->InsertText(-1, strTranWait);
		m_reditHistory->EndDown();
		SetTranPre(true);
#else
		if(destid >= 0 && pDestFriend != NULL)
		{
			{
				// 客服转接后订单编号发到转接客服
				XT_MSG msg;
				memset(&msg,0,sizeof(msg));
				msg.ver         = XTREAM_CLIENT_VERSION;
				msg.from_id     = g_ImUserInfo.GetId();
				msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
				msg.to_id		= destid;
				msg.data_type   = XT_MSG::IM_MSG_ORDER_SEQ;//插入会话消息

				extern map<int, CString> g_mapFriendIdToOrder;
				CString text = g_mapFriendIdToOrder[m_FriendInfo.GetId()];
				text.Format("%d:%s", m_FriendInfo.GetId(), text);
				strcpy(msg.data, text);
				msg.data_len=strlen(text)+1;
				SF_SetTransMode(CIMProtocol::SMT_SERVER);
				g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);
			}
			// 发送正在转接消息
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

			msg.to_id		= m_FriendInfo.GetId();
			msg.dest_ip		= m_FriendInfo.ip;
			msg.dest_port	= m_FriendInfo.port;
			msg.msg_type    = 0;

			msg.data_type = XT_MSG::CUST_SER_TRAN_REQ;
			strcpy(msg.from_nickname, g_ImUserInfo.nickname);

			Json::FastWriter jWriter;
			Json::Value jvSellerInfo;
			jvSellerInfo["switchStatus"] = 1;		// 1 表示转接开始 0 表示转接成功 2 表示主管接手会话
			jvSellerInfo["toSubMerchantID"] = destid;
			jvSellerInfo["fromSubMerchantID"] = g_ImUserInfo.GetId();
#ifdef _CS_PLAT_EX_
			jvSellerInfo["toSubMerchantName"] = pDestFriend->GetUserName().c_str();
#else
			jvSellerInfo["toSubMerchantName"] = pDestFriend->nickname;
#endif
			string strMsg = jWriter.write(jvSellerInfo);

			strcpy(msg.data, strMsg.c_str());
			msg.data_len = strMsg.length() + 1;

			g_pProtocol->SendP2PMsg(msg);

			// 发送转接请求
			XT_CUSTSER_TRAN_REQ req;
			req.uSid = g_ImUserInfo.GetId();
			req.uDid = destid;
			strncpy(req.szOrderId, m_pCustSerTranDlg->m_strOrderID.c_str(), 63);
			strncpy(req.szTranReason, m_pCustSerTranDlg->m_strReason.c_str(), 511);
			req.uCustomerId = m_nFriendId;

			g_pProtocol->SendReq(req);

			delete m_pCustSerTranDlg;
			m_pCustSerTranDlg = NULL;
		}
#endif
	}
}

// 收到客服转接确认包
void CDuiP2PSendMsgDlg::OnRecvCustSerTranAckInd(const char * data, WORD wRet)
{
	XT_CUSTSER_TRAN_ACK *pAck = (XT_CUSTSER_TRAN_ACK *)data;
	if(pAck->uCustomerId == m_FriendInfo.GetId())
	{
		// 显示转接成功消息
		XT_FONT xf;
		xf.fontColor=RGB(254,154,105);

		//m_reditHistory.SetSel(-1,-1);
		//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
		//m_reditHistory.SetStartIndent(80);
		m_reditHistory->SetSel(-1,-1);
		ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
		RichEdit_SetStartIndent(pTextServicesHistory, 80);
		

#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		CString strTranSuccess = "";
		FRIEND_INFO* pFriend = ::GetFriendInfo(pAck->uSid);
		if(pFriend != NULL)
			strTranSuccess.Format("\n----[%s]已将您转接至客服：%s\n", UserInfoToStr(&g_ImUserInfo), UserInfoToStr(pFriend));
		else
			strTranSuccess.Format("\n----[%s]已将您转接至客服：%d\n", UserInfoToStr(&g_ImUserInfo), pAck->uDid);
		//m_reditHistory.InsertText(strTranSuccess);
		m_reditHistory->InsertText(-1, strTranSuccess);
		XT_MSG msgTranSuccess;
		msgTranSuccess.send_time   = g_LocalData.GetTimeNow();
		strcpy(msgTranSuccess.data, strTranSuccess);
		g_LocalData.SaveTalkMsg(msgTranSuccess,m_FriendInfo.GetId());
		SendMsgDlgNotify();
#else
		//m_reditHistory.InsertText("\n----转接成功\n");
		m_reditHistory->InsertText(-1, "\n----转接成功\n");
#endif

#ifdef _CS_PLAT_EX_
		//客服转接之后服务小记设置为已保存状态
		extern map<DWORD,DWORD>  g_mapServiceLog;
		g_mapServiceLog[m_FriendInfo.GetId()] = 1;
		//m_bIsTran = TRUE;

		// 客服转接成功后，主动邀约一个买家
		if(CDuiCSPlatDlg::GetInstance()->GetWaiterCount() > 0)
		{
/*
			XT_RECEPTE_CUST_REQ req;
			req.idBuyer = 0;
			req.type = (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER;	// 主动接待
			req.teamId = 0;
			g_pProtocol->SendReq(req);
*/
		}
		SendMsgDlgNotify();
#endif
		//m_reditHistory.ScrollToBottom();
		m_reditHistory->EndDown();

		//UpdateFriendState();
	}
}

void CDuiP2PSendMsgDlg::AddTranMsg(uint32 fromId, const char *pOrderId, const char *pReason)
{
	FRIEND_INFO *pFriend = ::FindFriend(fromId);
	if(pFriend == NULL)
	{
		return;
	}

#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
	////获取之前30条消息
	XT_WEB_MORE_MSG_REQ req;
	req.sid = fromId;
	req.did = m_FriendInfo.GetId();
	req.msgid = 0;
	req.msgtime = g_LocalData.GetTimeNow();
	req.isMerchant = 0;
	req.msgnum = 30;
	memset(req.uuid, 0, sizeof(req.uuid));
	g_pProtocol->SendReq(req);
#endif

	// 历史消息记录中打印转接消息
	XT_FONT xf;
	xf.fontColor=RGB(254,154,105);
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.SetStartIndent(80);
	m_reditHistory->SetSel(-1,-1);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	RichEdit_SetStartIndent(pTextServicesHistory, 80);

#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版

	//m_reditHistory.GetSel(m_crTran);
	//CString strFrom;
	//strFrom.Format("\n----[%s]已将您转接至客服：%s\n", UserInfoToStr(pFriend), UserInfoToStr(&g_ImUserInfo));
	//m_reditHistory.InsertText(strFrom);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	//XT_MSG msgTranSuccess;
	//msgTranSuccess.send_time   = g_LocalData.GetTimeNow();
	//strcpy(msgTranSuccess.data, strFrom);
	//g_LocalData.SaveTalkMsg(msgTranSuccess,m_FriendInfo.GetId());

	//strFrom.Format("----将由客服人员[%s]为您服务", UserInfoToStr(&g_ImUserInfo));
	//m_reditHistory.InsertText(strFrom);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	//msgTranSuccess.send_time   = g_LocalData.GetTimeNow();
	//strcpy(msgTranSuccess.data, strFrom);
	//g_LocalData.SaveTalkMsg(msgTranSuccess,m_FriendInfo.GetId());

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.ScrollToBottom();

	m_reditHistory->GetSel(m_crTran);
	CString strFrom;
	strFrom.Format("\n----[%s]已将您转接至客服：%s\n", UserInfoToStr(pFriend), UserInfoToStr(&g_ImUserInfo));
	m_reditHistory->InsertText(-1, strFrom);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

	XT_MSG msgTranSuccess;
	msgTranSuccess.send_time   = g_LocalData.GetTimeNow();
	strcpy(msgTranSuccess.data, strFrom);
	g_LocalData.SaveTalkMsg(msgTranSuccess,m_FriendInfo.GetId());

	strFrom.Format("----将由客服人员[%s]为您服务", UserInfoToStr(&g_ImUserInfo));
	m_reditHistory->InsertText(-1, strFrom);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

	msgTranSuccess.send_time   = g_LocalData.GetTimeNow();
	strcpy(msgTranSuccess.data, strFrom);
	g_LocalData.SaveTalkMsg(msgTranSuccess,m_FriendInfo.GetId());

	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->EndDown();

#else
	//CString strFrom;
	//strFrom.Format("\n----转自\"%s\"\n", pFriend->nickname);
	//m_reditHistory.InsertText(strFrom);

	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//CString strOrder;
	//strOrder.Format("订单编号：%s\n", pOrderId);
	//m_reditHistory.InsertText(strOrder);

	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//CString strReason;
	//strReason.Format("转接原因：%s\n", pReason);
	//m_reditHistory.InsertText(strReason);

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.ScrollToBottom();

	//CString strFrom;
	//strFrom.Format("\n----转自\"%s\"\n", pFriend->nickname);
	//m_reditHistory->InsertText(-1, strFrom);

	//RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	//CString strOrder;
	//strOrder.Format("订单编号：%s\n", pOrderId);
	//m_reditHistory->InsertText(-1, strOrder);

	////m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	//CString strReason;
	//strReason.Format("转接原因：%s\n", pReason);
	//m_reditHistory->InsertText(-1, strReason);

	//m_reditHistory->SetSel(-1,-1);
	//m_reditHistory->EndDown();

#endif
}
#endif

void CDuiP2PSendMsgDlg::UpdateBk()
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
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,105' dest='0,0,%d,105'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,105' dest='0,0,%d,105' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}

#ifdef _BUSINESS_VESION
	if(m_pCustSerTranDlg != NULL && ::IsWindow(m_pCustSerTranDlg->GetHWND()))
	{
		m_pCustSerTranDlg->UpdateBk();
	}
#endif
}

void CDuiP2PSendMsgDlg::SendMsgDlgNotify()
{
#ifdef _BUSINESS_VESION
	//更新右侧备注
	if(m_strNewComnCustRemarkClient != m_strNewComnCustRemarkServer)
	{
		XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ req;
		req.uiBusinessID = g_ImUserInfo.GetId();
		strcpy(req.szCommAccount, m_FriendInfo.GetUserName().c_str());

		// 时间格式化
		uint32 uiSendTime = time(NULL);
		CTime  tm(uiSendTime);
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		strcpy(req.szCommTime, strTime);

		strncpy(req.szRemarks, m_strNewComnCustRemarkClient, 200);
		g_pProtocol->SendReq(req);

		Sleep(200);
	}
#endif

#ifdef _CS_PLAT_EX_
	g_LocalData.DeleteCurrentBuyerDbFile(m_FriendInfo.GetId());
#endif
	FRIEND_INFO *pRealFriend = FindFriend(m_FriendInfo.GetId());
	if ( pRealFriend != NULL )
	{
		pRealFriend->bClosePanel = m_bClosePanel;
		//pRealFriend->isTalkZYF   = m_reditSend.IsEnableZYF() ? 1:0;

		////0:本次未读消息不提示 ,1:本次未读消息退出提示   
		WORD nExitWarn = 0;
		g_LocalData.GetCloseTalkWarnConfig(nExitWarn);////获得是否提示的信息
		if(nExitWarn == 0)////本次未读不提示
		{
			map<FRIEND_INFO *,UINT>::iterator it = g_mapMsgUnRead.find(pRealFriend);
			if(it != g_mapMsgUnRead.end())
			{
				//delete it->first;
				g_mapMsgUnRead.erase(it);////清除好友
			}
		}
	}
#ifdef _CS_PLAT_EX_
	extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
	map<DWORD,DWORD>::iterator it = g_mapServiceLog.find(/*pRealFriend->GetId()*/m_FriendInfo.GetId());
	if(it != g_mapServiceLog.end())
	{
		g_mapServiceLog.erase(it);//清除对应的服务小记的信息，在下次打开再关闭时，需要重新判断
	}

	////去除GUID为空的买家ID,下次打开重新判断
	extern map<DWORD,CString> g_mapServiceURL;
	map<DWORD,CString>::iterator iter = g_mapServiceURL.find(/*pRealFriend->GetId()*/m_FriendInfo.GetId());
	if(iter != g_mapServiceURL.end())
	{
		g_mapServiceURL.erase(iter);////清除好友
	}
	
	//关闭一个聊天窗口之后显示上一个窗口
	int nSize = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size();
	if(nSize <= 1)
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = NULL;
	else
	{
		for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
		{
			CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nSize - 1 - nDlg];

			if(pDlg != NULL && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
			{
				g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;
				if(CDuiCSPlatDlg::GetInstance() != NULL)
				{
					if(CMainFrame::IsBuyer(pDlg->GetFriendInfo()->GetId()))
						CDuiCSPlatDlg::GetInstance()->SetLayoutPageCust();
					else
						CDuiCSPlatDlg::GetInstance()->SetLayoutPageTeam();
				}
				g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->ShowWindow(true);
				break;
			}
		}
	}
	if(m_bIsMonitor)
	{
		//关闭监控
		XT_MONITOR_END_REQ req;
		req.idMaster = g_ImUserInfo.GetId();
		req.idSeller = m_FriendInfo.GetId();
		g_pProtocol->SendReq(req);
	}
#endif
	Close();
}

//发送框设置焦点
void CDuiP2PSendMsgDlg::SetEditFocus()
{
	//m_reditSend.SetFocus();
	TEventUI eve;
	eve.Type = UIEVENT_SETFOCUS;
	m_reditSend->DoEvent(eve);
	m_PaintManager.SetFocus(NULL);
	m_PaintManager.SetFocus(m_reditSend);
}

void CDuiP2PSendMsgDlg::UpdateFastWordInfo()
{
	m_mapExpandNode.clear();
	CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("treefastword")));
	if(pTreeView == NULL)
		return;
	for(int nNode = 0; nNode < pTreeView->GetCount(); nNode++)
	{
		CTreeNodeUI* pTreeNode = (CTreeNodeUI*)pTreeView->GetItemAt(nNode);
		if(pTreeNode->GetFolderButton()->GetCheck())
		{
			CString strGroup = pTreeNode->GetFolderButton()->GetText();
			m_mapExpandNode[strGroup] = TRUE;
		}
	}
	while(pTreeView->GetCount() > 0)
	{
		pTreeView->RemoveAll();
	}
	vector<FastWordGroupInfo> vecWordGroupInfo = g_LocalData.GetFastWordInfo();
	for(int nGroup = 0; nGroup < vecWordGroupInfo.size(); nGroup++)
	{

		FastWordGroupInfo fastWordGroupInfo = vecWordGroupInfo[nGroup];
		fastWordGroupInfo.strGroupName;
		//CTreeNodeUI* pTreeNode = new CTreeNodeUI;
		//pTreeNode->SetAttribute("folderattr", "width=\"9999\" normalimage=\"file='SysBtn\\gourp_off_arrow.png' dest='8,7,20,19'\" hotimage=\"file='SysBtn\\gourp_off_arrow_hover.png' dest='8,7,20,19'\" selectedimage=\"file='SysBtn\\gourp_on_arrow.png' dest='8,7,20,19'\" selectedhotimage=\"file='SysBtn\\gourp_on_arrow_hover.png' dest='8,7,20,19'\" align=\"left,vcenter\" textpadding=\"28,0,0,0\"");
		DuiLib::CDialogBuilder builder;
		CTreeNodeUI* pTreeNode = static_cast<DuiLib::CTreeNodeUI*>(builder.Create(_T("FastWordElement.xml"), (UINT)0, NULL));

		DuiLib::CButtonUI* pBtnConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pTreeNode, _T("fastwordconfig")));
		pBtnConfig->SetVisible(false);
		//vecMousemoveBtn.push_back(pBtnConfig);
		//if(fastWordGroupInfo.strGroupName != "未分组")
		//	pTreeNode->SetMousemoveBtnVisible(vecMousemoveBtn);
		pTreeView->Add(pTreeNode);
		pTreeNode->GetFolderButton()->SetText(fastWordGroupInfo.strGroupName);
		for(int nFastWordInfo = 0; nFastWordInfo < fastWordGroupInfo.vecFastWordInfo.size(); nFastWordInfo++)
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWordInfo];
			DuiLib::CDialogBuilder builder2;
			CTreeNodeUI* pTreeNodeChild = static_cast<DuiLib::CTreeNodeUI*>(builder2.Create(_T("FastWordNodeElement.xml"), (UINT)0, NULL));//new CTreeNode
			DuiLib::CButtonUI* pBtnConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pTreeNodeChild, _T("fastwordnodeconfig")));
			pBtnConfig->SetVisible(false);
			DuiLib::CLabelUI* pLabelNum = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pTreeNodeChild, _T("fastwordnum")));
			pLabelNum->SetText(fastWordInfo.strNum);
			DuiLib::CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pTreeNodeChild, _T("fastword")));
			//快捷短语中如果有图片则显示[图片]
			CString strText = fastWordInfo.strWord;
			while(strText.Find("<image:",0) != -1)
			{
				int nStart = strText.Find("<image:",0);
				int nEnd = strText.Find(">",0);
				strText.Replace(strText.Mid(nStart, nEnd - nStart + 1), "[图片]");
			}
			pLabelWord->SetText(strText);
			pTreeNodeChild->SetDrawBkColor(0xFFFFFFFF);
			pLabelNum->SetToolTip(strText);
			pLabelWord->SetToolTip(strText);
			pTreeNode->Add(pTreeNodeChild);
		}
		if(m_mapExpandNode[fastWordGroupInfo.strGroupName])
		{
			pTreeView->SetItemExpand(true, pTreeNode);
			pTreeNode->GetFolderButton()->SetCheck(true);
		}
	}
	pTreeView->SetAttribute("visiblefolderbtn", "true");
}

//设置手动挂起按钮状态
void CDuiP2PSendMsgDlg::SetHangupState(bool bIsVisable)
{
#ifndef _CS_PLAT_EX_
	if(CMainFrame::IsBuyer(m_FriendInfo.GetId()))
	{
		if(!CMainFrame::GetInstance()->GetHangupSelect() || bIsVisable)
		{
			m_pLabelHangupOn->SetVisible(bIsVisable);
			m_pLabelHangupOff->SetVisible(!bIsVisable);
		}
	}
#endif
}

//取消手动挂起
void CDuiP2PSendMsgDlg::HangupCancel()
{
	if(m_pLabelHangupOn->IsVisible())
	{
		m_pLabelHangupOn->SetVisible(false);
		m_pLabelHangupOff->SetVisible(true);

		int nHangupMenuIndex = CMainFrame::GetInstance()->GetHangupMenuIndex();
		switch(nHangupMenuIndex)
		{
		case 1:
			{
				CMainFrame::GetInstance()->SetHangupStatusNeededSessions(-1);
			}
			break;
		case 2:
			{
				CMainFrame::GetInstance()->SetHangupStatusNeededSessions(20);
			}
			break;
		case 3:
			{
				CMainFrame::GetInstance()->SetHangupStatusNeededSessions(50);
			}
			break;
		case 4:
			{
				CMainFrame::GetInstance()->SetHangupStatusNeededSessions(100);
			}
			break;
		case 5:
			{	// 弹出自定义窗口
				CMainFrame::GetInstance()->SetHangupStatusNeededSessions(CMainFrame::GetInstance()->GetHangupStatusNeededSessions());
			}
			break;
		case 6://手动挂起
			{
				CMainFrame::GetInstance()->SetHangupSelect(false);
			}
			break;
		}
	}
}

//设置用户为在线状态
void CDuiP2PSendMsgDlg::UpdateFriendState()
{
	static XT_STATUS_IND ind;
	ind.fid = m_FriendInfo.GetId();
	ind.status = m_FriendInfo.status;
	g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_FRIEND_STATUS_IND),(LPARAM)&ind);
}

//关闭窗口，包括关闭窗口的提示框
BOOL CDuiP2PSendMsgDlg::CloseAllDlg()
{
	BOOL bIsHint = FALSE;
#ifdef _CS_PLAT_EX_  //飞牛客服工作台(先判断是否保存服务小记，再判断是否有未读消息)
	if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() >= 2)
	{
		for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
		{
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg]->m_hWnd != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] == this)
			{
				bIsHint = TRUE;
				break;
			}
		}
	}
	if(/*g_pMessageHandleWnd->GetP2PSendMsgDlgSize() > 1*/bIsHint)//有多个咨询框
	{
		CMainFrame::GetInstance()->LockDlg();////锁住对话框
		extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
		DWORD dwServiceLogCnt = g_mapServiceLog.size();
		int nBuyerWndCnt = CDuiCSPlatDlg::GetInstance()->CalcBuyerTalkingWnd();//和买家聊天的窗口数
		if(nBuyerWndCnt == 0)//对面没有买家，直接判断未读消息
		{
			int nUnRead = 0;
			BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsPopMsgWarn(nUnRead);    /////是否有未读消息
			if(bUnRead) ////有未读消息的多个咨询框，弹出关闭所有还是关闭当前
			{		
				std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_MUL_TALK,GetHWND());
				pWnd->ShowModal();
				return TRUE ;	
			}
			else////无未读消息的多个咨询窗口，弹出关闭所有还是关闭当前
			{
				std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> pWnd = CP2PSendMsgDlgCloseHint::NewDlg(GetHWND());
				pWnd->ShowModal();
				return TRUE ;
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
					return TRUE ;
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
					return TRUE ;
				}
				else////全都保存了完整的服务小记，再检查是否有未读消息
				{
					int nTag = SAVED_SERVICELOG;
					if(GetIsTran())
						nTag = IS_BUYER_TRAN;
					std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(nTag,GetHWND());
					pWnd->ShowModal();
					if(pWnd->m_bClickedBtnOK)///服务小记全都保存完整，点确认关闭，再检测未读消息
					{
						////////////////
						int nUnRead = 0;
						BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsPopMsgWarn(nUnRead);    /////是否有未读消息
						if(bUnRead) ////有未读消息的多个咨询框，弹出关闭所有还是关闭当前
						{		
							std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_MUL_TALK,GetHWND());
							pWnd->ShowModal();
							return TRUE ;	
						}
						else////无未读消息的多个咨询窗口，弹出关闭所有还是关闭当前
						{
							std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> pWnd = CP2PSendMsgDlgCloseHint::NewDlg(GetHWND());
							pWnd->ShowModal();
							return TRUE ;
						}
					}
					else
					{
						return TRUE ;
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
			if( m_FriendInfo.GetGuid().length() > 0 && CMainFrame::IsBuyer(m_FriendInfo.GetId()))  
			{
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNSAVED_SERVICELOG,GetHWND());
				pWnd->ShowModal();
				return TRUE ;
			}
			else//对面不是买家，直接检测是否有未读消息
			{
				int nUnRead = 0;
				BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsPopMsgWarn(nUnRead);  
				if(bUnRead) ////有未读消息
				{
					if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg == this)//判断选中的是否为当前窗口
					{
						CMainFrame::GetInstance()->LockDlg();////锁住对话框
						std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
						pWnd->ShowModal();
						if(pWnd->m_bUnReadMsgShow)///确认关闭
						{
							SendMsgDlgNotify();
						}
						CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
						return TRUE ;
					}					
				}
				else//无未读消息
				{
					SendMsgDlgNotify();
					CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
					return TRUE;
				}
			}
		}
		else if(dwServiceLogCnt == 1)//对面是买家，且保存了服务小记或者不完整的服务小记
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
				return TRUE ;
			}
			else//保存了完整的服务小记，再判断是否有未读消息 
			{
				//////////////////////////////////////////////////
				int nTag = SAVED_SERVICELOG;
				if(GetIsTran())
					nTag = IS_BUYER_TRAN;
				std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(nTag,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bClickedBtnOK)///服务小记全都保存完整，点确认关闭，再检测未读消息
				{
					//////////////////////////
					int nUnRead = 0;
					BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsPopMsgWarn(nUnRead);  
					if(bUnRead) ////有未读消息
					{
						if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg == this)//判断选中的是否为当前窗口
						{
							CMainFrame::GetInstance()->LockDlg();////锁住对话框
							std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
							pWnd->ShowModal();
							if(pWnd->m_bUnReadMsgShow)///确认关闭
							{
								SendMsgDlgNotify();
							}
							CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
							return TRUE ;
						}					
					}
					else//无未读消息
					{
						SendMsgDlgNotify();
						CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
						return TRUE;
					}
				}
				else
				{
					return TRUE ;
				}
			}
		}
	}
	else if(CDuiCSPlatDlg::GetInstance() == NULL)////没有咨询框聊天
	{
		SendMsgDlgNotify();
		CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
		return TRUE;
	}
#else
	if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() >= 2)
	{
		for(int nDlg = 0; nDlg < CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size(); nDlg++)
		{
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg[nDlg] == this)
			{
				bIsHint = TRUE;
				break;
			}
		}
	}
	if(/*g_pMessageHandleWnd->GetP2PSendMsgDlgSize() > 1*/bIsHint)
	{
		CMainFrame::GetInstance()->LockDlg();////锁住对话框
		int nUnRead = 0;
		BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nUnRead);    /////是否有未读消息
		if(bUnRead) ////有未读消息的多个咨询框
		{		
			std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_MUL_TALK,GetHWND());
			pWnd->ShowModal();
			return TRUE ;	
		}
		else////无未读消息的多个咨询窗口，弹出关闭所有还是关闭当前
		{
			std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> pWnd = CP2PSendMsgDlgCloseHint::NewDlg(GetHWND());
			pWnd->ShowModal();
			return TRUE ;
		}
	}
	if(CDuiCounsel::GetInstance() != NULL &&CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() == 1)////只有一个咨询框
	{
		int nUnRead = 0;
		BOOL bUnRead = CDuiCounsel::GetInstance()->IsPopMsgWarn(nUnRead);  
		if(bUnRead) ////有未读消息
		{
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg[0] == this)///判断选中的是否为当前窗口
			{
				CMainFrame::GetInstance()->LockDlg();////锁住对话框
				std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bUnReadMsgShow)///确认关闭
				{
					SendMsgDlgNotify();
				}
				CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
				return TRUE ;
			}					
		}
		else
		{
			SendMsgDlgNotify();
			CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
			return TRUE;
		}
	}
	else if(CDuiCounsel::GetInstance() == NULL)////没有咨询框聊天
	{
		SendMsgDlgNotify();
		CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
		return TRUE;
	}
#endif
}

#ifdef _CS_PLAT_EX_
//关闭窗口提示
void CDuiP2PSendMsgDlg::ServiceLogWorn(DWORD dwStatus)
{
	extern map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
	map<DWORD,DWORD>::iterator it;
	FRIEND_INFO *pRealFriend = FindFriend(m_FriendInfo.GetId());//好友信息
	if(pRealFriend == NULL)
	{
		return;
	}
	it = g_mapServiceLog.find(pRealFriend->GetId());
	if(it != g_mapServiceLog.end())///之前保存了
	{
		it->second = dwStatus;//更新状态
	}
	else//没找到则添加
	{
		g_mapServiceLog[pRealFriend->GetId()] = dwStatus;
	}
	if(dwStatus == SAVED_SERVICELOG)//当前的服务小记已保存，再判断当前窗口是否有未读消息
	{
		int nTag = SAVED_SERVICELOG;
		if(GetIsTran())
			nTag = IS_BUYER_TRAN;
		std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(nTag,GetHWND());
		pWnd->ShowModal();
		if(pWnd->m_bClickedBtnOK)///确认关闭，关闭当前窗口
		{
			if(CDuiCSPlatDlg::GetInstance() != NULL)////有咨询框
			{
				int nUnRead = 0;
				int nFriendID = pRealFriend->GetId();
				BOOL bUnRead = CDuiCSPlatDlg::GetInstance()->IsCurrentPopMsgWarn(nUnRead,nFriendID); //查看当前会话有无未读消息 
				if(bUnRead) ////有未读消息
				{
					/*if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg[0] == this)///判断选中的是否为当前窗口*/
					//{
					CMainFrame::GetInstance()->LockDlg();////锁住对话框
					std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
					pWnd->ShowModal();
					if(pWnd->m_bUnReadMsgShow)///确认关闭
					{
						SendMsgDlgNotify();
					}
					CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
					return;
					//}					
				}
				else
				{
					SendMsgDlgNotify();
					CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
					return;
				}
			}
		}
	}
	else if(dwStatus == UNCOMPLETED_SERVICELOG)//未保存完整
	{ 
		BOOL bUncompleted = FALSE;
		std::tr1::shared_ptr<CheckServiceLogWarn> pWnd = CheckServiceLogWarn::Show(UNCOMPLETED_SERVICELOG,GetHWND());
		pWnd->ShowModal();//保存不完整,当前页面显示
	}
}

//获取监控的买家ID
int CDuiP2PSendMsgDlg::GetMonitorBuyerId()
{
	return m_nIdMonitorBuyer;
}

// 主管获取客服当前聊天买家
void CDuiP2PSendMsgDlg::OnMonitorSetBuyerInd(const char *data,WORD wRet)
{
	LPXT_MONITOR_SET_BUYER_IND ind = (LPXT_MONITOR_SET_BUYER_IND)data;
	if(ind->idSeller != m_FriendInfo.GetId())
		return;
	if(ind->idBuyer == 0 && m_bIsMonitor)
	{
		m_nIdMonitorBuyer = ind->idBuyer;
		CMessageBox::Model(GetHWND(), "客服已经不存在和买家的会话！", "提示", MB_OK);

		//关闭监控
		XT_MONITOR_END_REQ req;
		req.idMaster = g_ImUserInfo.GetId();
		req.idSeller = m_FriendInfo.GetId();
		g_pProtocol->SendReq(req);
		return;
	}

	//如果正在插入会话则无法改变当前聊天的客户
	if(m_bIsInsertMsg)
	{
		return;
	}

	m_nIdMonitorBuyer = ind->idBuyer;

	// 更新买家资料
	XT_USERINFO_GET_REQ req;
	req.fid = m_nIdMonitorBuyer;
	g_pProtocol->SendReq(req); 

	// 显示聊天消息
	//m_reditHistory.ClearMsg();
	//m_reditHistory->SetText("");
	m_reditHistory->SetSel(0, -1);
	m_reditHistory->ReplaceSel("", false);
	map<uint32, vector<XT_MSG>>::iterator it = m_mapHistoryMsg.find(ind->idBuyer);
	if(it != m_mapHistoryMsg.end())
	{
		for(size_t i = 0; i < it->second.size(); ++i)
		{
			AddTalkMsg(it->second[i], false);
		}
	}
}

// 主管获取客服接待的买家
void CDuiP2PSendMsgDlg::OnMonitorStartAck(const char *data,WORD wRet)
{
	LPXT_MONITOR_START_ACK ack = (LPXT_MONITOR_START_ACK)data;
	if(ack->idSeller != m_FriendInfo.GetId())
		return;
	if(ack->ret == 0)
	{
		if(ack->idBuyer == 0)
		{
			if(!m_bIsBuyerNone)
			{
				m_bIsBuyerNone = TRUE;
				if(IDOK == CMessageBox::Model(GetHWND(), "客服不存在和买家的会话！", "提示", MB_OK) )
				{
					m_bIsBuyerNone = FALSE;
				}
			}
			
			//关闭监控
			XT_MONITOR_END_REQ req;
			req.idMaster = g_ImUserInfo.GetId();
			req.idSeller = m_FriendInfo.GetId();
			g_pProtocol->SendReq(req);
			return;
		}
		//开启监控按钮隐藏，其他按钮显示
		//m_pBtnMonitorOn->SetVisible(false);
		//m_pBtnMonitorOff->SetVisible(true);
		//m_pBtnInsertMsg->SetVisible(true);
		//m_pBtnTranMsg->SetVisible(true);

		ack->idSeller;
		m_nIdMonitorBuyer = ack->idBuyer;
		m_bIsMonitor = TRUE;

		//监控之后发送按钮变为灰色
		m_pBtnSend->SetEnabled(false);
		m_pBtnSendConfig->SetEnabled(false);
		SetToolEnabled(false);
		//监控之后清空历史消息
		//m_reditHistory.ClearMsg();
		//m_reditHistory->SetText("");
		m_reditHistory->SetSel(0, -1);
		m_reditHistory->ReplaceSel("", false);
		// 更新买家资料
		XT_USERINFO_GET_REQ req;
		req.fid = m_nIdMonitorBuyer;
		g_pProtocol->SendReq(req);   
	}
	else if(ack->ret == 2)
	{
		//监控的客服不存在和买家的会话
		CMessageBox::Model(GetHWND(), "该客服已被其他主管监控！", "提示", MB_OK);
	}
	else
	{
		//监控的客服不存在和买家的会话
		if(!m_bIsBuyerNone)
		{
			m_bIsBuyerNone = TRUE;
			if(IDOK == CMessageBox::Model(GetHWND(), "客服不存在和买家的会话！", "提示", MB_OK) )
			{
				m_bIsBuyerNone = FALSE;
			}
		}
	}
}

//主管获取客服的动作，聊天
void CDuiP2PSendMsgDlg::OnMonitorStartInd(const char *data,WORD wRet)
{
	LPXT_MONITOR_START_IND ind = (LPXT_MONITOR_START_IND)data;

	XT_MSG msgAcceptAuto;
	memset(&msgAcceptAuto,0,sizeof(msgAcceptAuto));
	msgAcceptAuto.ver         = XTREAM_CLIENT_VERSION;
	msgAcceptAuto.from_id     = ind->msg.from_id;
	msgAcceptAuto.fontColor   = ind->msg.fontColor;
	msgAcceptAuto.fontSize    = ind->msg.fontSize;
	msgAcceptAuto.fontStyle   = ind->msg.fontStyle;
	strcpy(msgAcceptAuto.fontName,ind->msg.fontName);
	strcpy(msgAcceptAuto.from_nickname,ind->msg.from_nickname);
	msgAcceptAuto.send_time   = ind->msg.send_time;
	msgAcceptAuto.to_id		= ind->msg.to_id;
	msgAcceptAuto.msg_type    = 0;		
	
	CString text(ind->msg.data);

	if(ind->msg.data_type == XT_MSG::PUSH_EVALUA)
	{
		text = _T("您成功发送服务评价表");
	}

	strcpy(msgAcceptAuto.data, text);
	msgAcceptAuto.data_len=strlen(text)+1;

	msgAcceptAuto.data_type   = XT_MSG::IM_MSG;

	m_mapHistoryMsg[ind->idBuyer].push_back(msgAcceptAuto);
	if(m_nIdMonitorBuyer == ind->idBuyer)
	{
		AddTalkMsg(msgAcceptAuto, false);
	}

// 	if(ind->action == 0)//客服发送消息
// 	{
// 
// 	}
// 	else if(ind->action == 1)//客服收到消息
// 	{
// 
// 	}
}

// 主管取消监控客服应答
void CDuiP2PSendMsgDlg::OnMonitorEndAck(const char *data,WORD wRet)
{
	LPXT_MONITOR_END_ACK ack = (LPXT_MONITOR_END_ACK)data;
	
	if(ack->ret == 0)
	{
		//取消监控
		CloseMonitor(); 
		//取消监控后还原和客服的历史消息
		//m_reditHistory.ClearMsg();
		//m_reditHistory->SetText("");
		m_reditHistory->SetSel(0, -1);
		m_reditHistory->ReplaceSel("", false);
		LoadLastTalkMsg();
		//取消监控之后发送按钮颜色还原
		m_pBtnSend->SetEnabled(true);
		m_pBtnSendConfig->SetEnabled(true);
		SetToolEnabled(true);
		m_pBtnInsertMsg->SetEnabled(true);
		m_bIsInsertMsg = FALSE;
		// 更新客服资料
		XT_USERINFO_GET_REQ req;
		req.fid = m_FriendInfo.GetId();
		g_pProtocol->SendReq(req); 

	}
	else
	{
		//取消监控失败
	}
}

// 主管收到取消监控客服消息
void CDuiP2PSendMsgDlg::OnMonitorEndInd(const char *data,WORD wRet)
{

}

//主管插入会话应答
void CDuiP2PSendMsgDlg::OnMonitorInsertChatAck(const char *data,WORD wRet)
{
	LPXT_MONITOR_INSERT_CHAT_ACK ack = (LPXT_MONITOR_INSERT_CHAT_ACK)data;
	if(ack->ret == 0)
	{
		m_bIsInsertMsg = TRUE;
		//点击插入会话之后发送按钮颜色还原
		m_pBtnSend->SetEnabled(true);
		m_pBtnSendConfig->SetEnabled(true);
		SetToolEnabled(true);
		m_pBtnInsertMsg->SetEnabled(false);//插入会话之后不可用
	}
	else
	{
		//插入会话失败
	}
}

//客服和买家都要获取主管插入的会话
void CDuiP2PSendMsgDlg::OnMonitorInsertChatInd(const char *data,WORD wRet)
{
	LPXT_MONITOR_INSERT_CHAT_IND ind = (LPXT_MONITOR_INSERT_CHAT_IND)data;

	if(ind->action == 0)
	{
		//提示插入会话
		FRIEND_INFO *pMaster = ::FindFriend(ind->idMaster);
		if(pMaster != NULL)
		{
			// 历史消息记录中打印转接消息
			XT_FONT xf;
			xf.fontColor=RGB(254,154,105);

			//m_reditHistory.SetSel(-1,-1);
			//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			//m_reditHistory.SetStartIndent(80);
			//CString strFrom;
			//strFrom.Format("\n\n客服主管\"%s\"进入会话\n", pMaster->nickname);
			//m_reditHistory.InsertText(strFrom);

			//m_reditHistory.SetSel(-1,-1);
			//m_reditHistory.ScrollToBottom();

			m_reditHistory->SetSel(-1,-1);
			ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
			RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
			RichEdit_SetStartIndent(pTextServicesHistory, 80);
			CString strFrom;
			strFrom.Format("\n\n客服主管\"%s\"进入会话\n", pMaster->nickname);
			m_reditHistory->InsertText(-1, strFrom);

			m_reditHistory->SetSel(-1,-1);
			m_reditHistory->EndDown();
		}
	}
	else
	{
		XT_MSG msgAcceptAuto;
		memset(&msgAcceptAuto,0,sizeof(msgAcceptAuto));
		msgAcceptAuto.ver         = XTREAM_CLIENT_VERSION;
		msgAcceptAuto.from_id     = ind->msg.from_id;
		msgAcceptAuto.fontColor   = ind->msg.fontColor;
		msgAcceptAuto.fontSize    = ind->msg.fontSize;
		msgAcceptAuto.fontStyle   = ind->msg.fontStyle;
		strcpy(msgAcceptAuto.fontName,ind->msg.fontName);
		strcpy(msgAcceptAuto.from_nickname,ind->msg.from_nickname);
		msgAcceptAuto.send_time   = ind->msg.send_time;
		msgAcceptAuto.to_id		= ind->msg.to_id;
		msgAcceptAuto.msg_type    = 0;		
		msgAcceptAuto.data_type   = ind->msg.msg_type;

		CString text(ind->msg.data);
		strcpy(msgAcceptAuto.data, text);
		msgAcceptAuto.data_len=strlen(text)+1;
		AddTalkMsg(msgAcceptAuto, false);
	}
}

//主管接手会话应答
void CDuiP2PSendMsgDlg::OnMonitorTranChatAck(const char *data,WORD wRet)
{
	LPXT_MONITOR_TRAN_CHAT_ACK ack = (LPXT_MONITOR_TRAN_CHAT_ACK)data;
	if(ack->ret == 0)
	{
		//取消监控
		CloseMonitor();
		//接手会话之后发送按钮颜色还原
		m_pBtnSend->SetEnabled(true);
		m_pBtnSendConfig->SetEnabled(true);
		SetToolEnabled(true);
		m_pBtnInsertMsg->SetEnabled(true);//取消监控后插入会话可用
		m_bIsInsertMsg = FALSE;
		// 更新客服资料
		XT_USERINFO_GET_REQ req;
		req.fid = m_FriendInfo.GetId();
		g_pProtocol->SendReq(req); 
		//接手会话
		unsigned threadID;
		_beginthreadex(NULL, 0, ThreadTranChat, (void*)this, 0, &threadID);
	}
	else
	{
		//接手会话失败
	}
}

// 客服转接功能，打开客户聊天窗口
unsigned __stdcall CDuiP2PSendMsgDlg::ThreadTranChat(void * pParam)
{
	CDuiP2PSendMsgDlg *pDlg = (CDuiP2PSendMsgDlg *)pParam;
	// 打开聊天窗口
	uint32 custId = pDlg->GetMonitorBuyerId();

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
		::PostMessage(pDlg->GetHWND(), WM_OPEN_TRAN_CHAT_MSGDLG, (WPARAM)pDlg, 0);
	}
	return 0;
}

//客服和买家收到主管接收会话
void CDuiP2PSendMsgDlg::OnMonitorTranChatInd(const char *data,WORD wRet)
{
	LPXT_MONITOR_TRAN_CHAT_IND ind = (LPXT_MONITOR_TRAN_CHAT_IND)data;
	//提示XX主管接手会话
	FRIEND_INFO* pFriendMaster = ::GetFriendInfo(ind->idMaster);

	XT_FONT xf;
	xf.fontColor=RGB(254,154,105);

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.SetStartIndent(80);

	m_reditHistory->SetSel(-1,-1);
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	RichEdit_SetStartIndent(pTextServicesHistory, 80);

	//接手会话之后服务小记设置为已保存状态
	extern map<DWORD,DWORD>  g_mapServiceLog;
	g_mapServiceLog[m_FriendInfo.GetId()] = 1;

	//m_bIsTran = TRUE;
	CString strFrom;
	if(pFriendMaster != NULL)
	{
		strFrom.Format("\n----当前会话已被主管\"%s\"接手\n", pFriendMaster->nickname);
	}
	else
	{
		strFrom.Format("\n----当前会话已被接手\n", pFriendMaster->nickname);
	}
	//m_reditHistory.InsertText(strFrom);
	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.ScrollToBottom();

	m_reditHistory->InsertText(-1, strFrom);
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->EndDown();

	//客服收到主管接手会话之后给主管发送订单
	XT_MSG msg;
	memset(&msg,0,sizeof(msg));
	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();
	msg.send_time   = g_LocalData.GetTimeNow();
	msg.to_id		= ind->idMaster;
	msg.data_type   = XT_MSG::IM_MSG_ORDER_SEQ;//插入会话消息

	extern map<int, CString> g_mapFriendIdToOrder;
	CString text = g_mapFriendIdToOrder[m_FriendInfo.GetId()];
	text.Format("%d:%s", m_FriendInfo.GetId(), text);
	strcpy(msg.data, text);
	msg.data_len=strlen(text)+1;
	SF_SetTransMode(CIMProtocol::SMT_SERVER);
	g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);
/*
	// 主动邀约一个买家
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

// 打开接手会话聊天窗口
LRESULT CDuiP2PSendMsgDlg::OnOpenTranChatMsgDlg(WPARAM w,LPARAM l)
{
	CDuiP2PSendMsgDlg *pDlg = (CDuiP2PSendMsgDlg *)w;
	FRIEND_INFO *pCustFriend = ::FindFriend(pDlg->GetMonitorBuyerId());
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
		if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pCustFriend->GetId()))
		{
			//if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
			CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
			CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pCustFriend, false, false, false, false, false);
			CDuiCSPlatDlg::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
			pDlg->ShowWindow(/*SW_RESTORE*/true);

		::BringWindowToTop(pDlg->GetHWND());
		CString strFrom;

		strFrom.Format("\n----已接手\"%s\"的会话\n", pCustFriend->nickname);
		pDlg->SetHistoryText(strFrom);

		//pDlg->UpdateFriendState();
		pDlg->RefreshFriendStatus();
	}

	return 0;
}

void CDuiP2PSendMsgDlg::SetHistoryText(CString strText)
{
	XT_FONT xf;
	xf.fontColor=RGB(254,154,105);

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.SetStartIndent(80);

	//m_reditHistory.InsertText(strText);

	//m_reditHistory.SetSel(-1,-1);

	m_reditHistory->SetSel(-1,-1);
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	RichEdit_SetStartIndent(pTextServicesHistory, 80);
	m_reditHistory->InsertText(-1, strText);
	m_reditHistory->SetSel(-1,-1);
}

void CDuiP2PSendMsgDlg::CloseMonitor()
{
	if(m_bIsMonitor)
	{
		//m_pBtnMonitorOn->SetVisible(true);
		//m_pBtnMonitorOff->SetVisible(false);
		//m_pBtnInsertMsg->SetVisible(false);
		//m_pBtnTranMsg->SetVisible(false);

		m_bIsMonitor = FALSE;
	}
}
//
////获取客户CSID
//void CDuiP2PSendMsgDlg::OnGetCSIDFromUserID(const char *data,WORD wRet)
//{ 
//	LPXT_USER_MAP_CSID_ACK ack = (LPXT_USER_MAP_CSID_ACK)data;
//	if(ack->ret == 0 &&!m_bRet)//控制服务器多次返回
//	{
//		m_bRet = TRUE;
//		m_nCSID = ack->csid;;
//		extern CString g_strToken;
//		CString strTokenHex = g_strToken;	
//		////local.fn.com:8080
//		CString strParams = "";
//		strParams.Format("customerServiceId=%d&serviceId=%d&auth_token=%s",g_ImUserInfo.GetId(),m_nCSID,g_strToken);	
//		m_strAdUrl = EMPLOYEE_RIGHT_URL + strParams;
//		if(m_wndAD->GetPos().top == 0)
//		{
//		    m_wndAD->SetUrl(URLEncode((LPCTSTR)m_strAdUrl).c_str());
//		}
//	}
//}
BOOL CDuiP2PSendMsgDlg::GetIsTran()
{
	return m_bIsTran;
}

void CDuiP2PSendMsgDlg::UpdateIsTran()
{
	m_bIsTran = FALSE;
}

BOOL CDuiP2PSendMsgDlg::IsMonitorOn(int nGroupIndex)
{
	if(g_ImUserInfo.group_index == nGroupIndex)
		return TRUE;
	for (int nGroup = 0; nGroup < g_GroupInfo.count; nGroup++)
	{
		if(nGroupIndex == g_GroupInfo.group[nGroup].group_index)
		{
			if(g_GroupInfo.group[nGroup].group_father == -1)
				return FALSE;
			return IsMonitorOn(g_GroupInfo.group[nGroup].group_father);
		}
	}
	return FALSE;
}

void CDuiP2PSendMsgDlg::SetToolEnabled(bool bIsEnable)
{
	CButtonUI* pBtnfont = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnfont")));
	CButtonUI* pBtnemotion = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnemotion")));
	CButtonUI* pBtnsendPic = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnsendPic")));
	CButtonUI* pBtnscreentool = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
	CButtonUI* pBtnscreentoolconfig = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
	CButtonUI* pBtnshake = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnshake")));
	CButtonUI* pBtnscore = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnscore")));
	CButtonUI* pBtnfastword = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnfastword")));
	CButtonUI* pBtnhistory = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnhistory")));

	pBtnfont->SetEnabled(bIsEnable);
	pBtnemotion->SetEnabled(bIsEnable);
	pBtnsendPic->SetEnabled(bIsEnable);
	pBtnscreentool->SetEnabled(bIsEnable);
	pBtnscreentoolconfig->SetEnabled(bIsEnable);
	pBtnshake->SetEnabled(bIsEnable);
	pBtnscore->SetEnabled(bIsEnable);
	pBtnfastword->SetEnabled(bIsEnable);
	pBtnhistory->SetEnabled(bIsEnable);
}
#endif

//最大化或者最小化
void CDuiP2PSendMsgDlg::ShowDlgMax(BOOL bIsShowMax)
{
	if(bIsShowMax)
	{
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
#ifdef _CS_PLAT_EX_
		CDuiCSPlatDlg::GetInstance()->SetPreRc(this);

		CRect rcLeft;
		::GetWindowRect(CDuiCSPlatDlg::GetInstance()->GetHWND(), &rcLeft);

		CRect rc;
		rc.left = rcLeft.Width();
		rc.top = 0;
		rc.right = cx;
		rc.bottom = cy;
		::MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
		CDuiCSPlatDlg::GetInstance()->MoveAllWindow(this);
		CDuiCSPlatDlg::GetInstance()->SetAllWindowMax(TRUE);
#else
		CDuiCounsel::GetInstance()->SetPreRc(this);

		CRect rcLeft;
		::GetWindowRect(CDuiCounsel::GetInstance()->GetHWND(), &rcLeft);

		CRect rc;
		rc.left = rcLeft.Width();
		rc.top = 0;
		rc.right = cx;
		rc.bottom = cy;
		::MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
		CDuiCounsel::GetInstance()->MoveAllWindow(this);
		CDuiCounsel::GetInstance()->SetAllWindowMax(TRUE);
#endif
	}
	else
	{
#ifdef _CS_PLAT_EX_
		CRect rcPre = CDuiCSPlatDlg::GetInstance()->GetPreRc();
		::MoveWindow(GetHWND(), rcPre.left, rcPre.top, rcPre.Width(), rcPre.Height(), TRUE);
		CDuiCSPlatDlg::GetInstance()->MoveAllWindow(this);
		CDuiCSPlatDlg::GetInstance()->SetAllWindowMax(FALSE);
#else
		CRect rcPre = CDuiCounsel::GetInstance()->GetPreRc();
		::MoveWindow(GetHWND(), rcPre.left, rcPre.top, rcPre.Width(), rcPre.Height(), TRUE);
		CDuiCounsel::GetInstance()->MoveAllWindow(this);
		CDuiCounsel::GetInstance()->SetAllWindowMax(FALSE);
#endif
	}

}

//设置窗口为最大化或者最小化状态
void CDuiP2PSendMsgDlg::SetWindowMax(BOOL bIsMax)
{
	CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
	CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

	if(bIsMax)
	{
		pbtnMax->SetVisible(FALSE);
		pbtnRestore->SetVisible(TRUE);
	}
	else
	{
		pbtnMax->SetVisible(TRUE);
		pbtnRestore->SetVisible(FALSE);
	}
}

void CDuiP2PSendMsgDlg::SetCsBuyerOffline()
{
	static XT_STATUS_IND ind;
	ind.fid = m_FriendInfo.GetId();
	ind.status = XTREAM_OFFLINE;
	g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_FRIEND_STATUS_IND),(LPARAM)&ind);
	g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_BUYER_OFFLINE_IND),(LPARAM)&ind);
}

//获取30条历史消息
void CDuiP2PSendMsgDlg::OnWebMoreMsgAck(const char *data,WORD wRet)
{
	LPXT_WEB_MORE_MSG_ACK pAck = (LPXT_WEB_MORE_MSG_ACK)data;
	if(wRet != 0)
	{
		return;
	}

	if(pAck->did == m_FriendInfo.GetId())
	{
		FRIEND_INFO* pFromFriend = ::GetFriendInfo(pAck->sid);
		FRIEND_INFO* pToFriend = ::GetFriendInfo(m_FriendInfo.GetId());
		CString strFromFriendName;
		CString strToFriendName;
		if(pFromFriend != NULL)
		{
			strFromFriendName = UserInfoToStr(pFromFriend);
		}
		else
		{
			strFromFriendName.Format("%d", pAck->sid);
		}

		strToFriendName = UserInfoToStr(&m_FriendInfo);

		for(int nMsg = 0; nMsg < pAck->actualnum; nMsg++)
		{
			XT_MSG msg;
			msg.from_id = pAck->msgInfo[nMsg].id;
			msg.send_time = pAck->msgInfo[nMsg].send_time;
			msg.fontStyle = pAck->msgInfo[nMsg].fontStyle;
			msg.fontSize = pAck->msgInfo[nMsg].fontSize;
			strcpy(msg.fontName,pAck->msgInfo[nMsg].fontName);
			msg.fontColor = pAck->msgInfo[nMsg].fontColor;
			strcpy(msg.data, pAck->msgInfo[nMsg].data);
			AddTranFromMsg(msg, strFromFriendName, strToFriendName);
		}
	}
}

//显示历史消息，插入到转接提示之前
void CDuiP2PSendMsgDlg::AddTranFromMsg(const XT_MSG &msg, CString strFromName, CString strToName)
{
	if ( m_hWnd==NULL || /*m_reditHistory.m_hWnd==NULL*/m_reditHistory==NULL )
	{
		return ;
	}

	CHARRANGE cr;
	CHARRANGE crEndPre;
	CHARRANGE crEndNow;

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(crEndPre);
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(crEndPre);

	XT_FONT xf;

	CString strTitle,strDate, strTime;
	CTime  tm(msg.send_time);

	strTime=tm.Format("%H:%M:%S");
	strDate=tm.Format("%Y.%m.%d");

	//好友发给我的信息
	if ( msg.from_id == m_FriendInfo.GetId() )
	{
		strTitle = strToName;
	}
	else
	{
		strTitle = strFromName;	
	}

	if ( g_ImUserInfo.GetId()==msg.from_id )
	{
		xf.fontColor=RGB(0,128,64); ///自己发送的信息
	}
	else
	{
		xf.fontColor=RGB(22,68,223);///外部接收的信息
	}

	m_reditHistory->SetSel(m_crTran);

	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.GetSel(cr);
	//m_reditHistory.InsertText("\n");
	m_reditHistory->SetSel(-1,-1);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	m_reditHistory->GetSel(cr);
	m_reditHistory->InsertText(-1, "\n");

	//m_reditHistory.SetStartIndent(80);
	//m_reditHistory.InsertText(strTitle);
	RichEdit_SetStartIndent(pTextServicesHistory, 80);
	m_reditHistory->InsertText(-1, strTitle);
	
	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(crEndNow);
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(crEndNow);

	//m_reditHistory.SetSel(cr.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory->SetSel(cr.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

	//m_reditHistory.SetSel(cr.cpMin + crEndNow.cpMin - crEndPre.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	//m_reditHistory.GetSel(cr);
	m_reditHistory->SetSel(cr.cpMin + crEndNow.cpMin - crEndPre.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	m_reditHistory->GetSel(cr);

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(crEndPre);
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(crEndPre);

	//m_reditHistory.SetSel(cr);
	//strTitle.Format("(%s %s):", strDate,strTime);
	//m_reditHistory.InsertText(strTitle);
	//m_reditHistory.InsertText("\n");
	m_reditHistory->SetSel(cr);
	strTitle.Format("(%s %s):", strDate,strTime);
	m_reditHistory->InsertText(-1, strTitle);
	m_reditHistory->InsertText(-1, "\n");

	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(crEndNow);
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(crEndNow);

	//m_reditHistory.SetSel(cr.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	m_reditHistory->SetSel(cr.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	XT_FONT xfG;
	xfG.fontColor = 0x808080;
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfG));
	//m_reditHistory.SetSel(cr.cpMin + crEndNow.cpMin - crEndPre.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	//m_reditHistory.GetSel(cr);
	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(crEndPre);
	//m_reditHistory.SetSel(cr);
	RichEdit_SetFont(pTextServicesHistory, xfG.fontName, xfG.fontSize, xfG.fontColor, xfG.flags & XTF_BOLD, xfG.flags & XTF_ITALIC, xfG.flags & XTF_UNDERLINE, false);
	m_reditHistory->SetSel(cr.cpMin + crEndNow.cpMin - crEndPre.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	m_reditHistory->GetSel(cr);
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(crEndPre);
	m_reditHistory->SetSel(cr);
	XT_FONT   xfMsg;
	xfMsg.flags     = msg.fontStyle;

	xfMsg.fontColor=msg.fontColor;

	sprintf(xfMsg.fontName,msg.fontName);
	xfMsg.fontSize  = msg.fontSize;

	CStringList sl;

	//m_reditHistory.SetStartIndent(300);
	RichEdit_SetStartIndent(pTextServicesHistory, 300);

	CString strData = msg.data;
	BOOL bIsFileTran = FALSE;
	if(strData.Find("文件 \"") != -1 && strData.Find("\" 接收完成。") != -1)
		bIsFileTran = TRUE;
	if(bIsFileTran)
		xfMsg.fontColor=RGB(0,0,255);

	//m_reditHistory.InsertText(msg.data,false,&sl);
	//m_reditHistory.SetSel(-1,-1);
	//m_reditHistory.GetSel(crEndNow);

	//m_reditHistory.SetSel(cr.cpMax,cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	//m_reditHistory.SetSel(cr.cpMin + crEndNow.cpMin - crEndPre.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	//m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	//m_reditHistory.ScrollToBottom();

	RichEdit_InsertText(pTextServicesHistory, msg.data, m_reditHistory, false, &sl, GetHWND());
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->GetSel(crEndNow);
	m_reditHistory->SetSel(cr.cpMax,cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	RichEdit_SetFont(pTextServicesHistory, xfMsg.fontName, xfMsg.fontSize, xfMsg.fontColor, xfMsg.flags & XTF_BOLD, xfMsg.flags & XTF_ITALIC, xfMsg.flags & XTF_UNDERLINE, false);
	m_reditHistory->SetSel(cr.cpMin + crEndNow.cpMin - crEndPre.cpMin, cr.cpMin + crEndNow.cpMin - crEndPre.cpMin);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	m_reditHistory->EndDown();
}

void CDuiP2PSendMsgDlg::InsertFileInfoBar(DWORD dwFileID,bool bSendFile,CString strFileName, long nFileSize)
{
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	m_reditHistory->InsertText(-1, " \n");
	XT_FONT xf;
	long lStartChar = 0, lEndChar = 0;
	if ( bSendFile )
	{
		m_reditHistory->SetSel(-1,-1);
		RichEdit_SetStartIndent(pTextServicesHistory, 80);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
		m_reditHistory->InsertText(-1, "取消");
		lEndChar = lStartChar + _tcslen("取消");
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, true);
	}
	else
	{
		m_reditHistory->SetSel(-1,-1);
		RichEdit_SetStartIndent(pTextServicesHistory, 80);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
		m_reditHistory->InsertText(-1, "接收");
		lEndChar = lStartChar + _tcslen("接收");
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, true);

		m_reditHistory->SetSel(-1,-1);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
		m_reditHistory->InsertText(-1, " / ");
		lEndChar = lStartChar + _tcslen(" / ");
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

		m_reditHistory->SetSel(-1,-1);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
		m_reditHistory->InsertText(-1, "拒绝");
		lEndChar = lStartChar + _tcslen("拒绝");
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, true);

		m_reditHistory->SetSel(-1,-1);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
		m_reditHistory->InsertText(-1, " / ");
		lEndChar = lStartChar + _tcslen(" / ");
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

		m_reditHistory->SetSel(-1,-1);
		RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
		m_reditHistory->InsertText(-1, "另存为");
		lEndChar = lStartChar + _tcslen("另存为");
		RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
		RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, true);
	}
	m_reditHistory->SetSel(-1,-1);
	RichEdit_GetSel(pTextServicesHistory, lStartChar, lEndChar);
	m_reditHistory->InsertText(-1, "\n");
	lEndChar = lStartChar + _tcslen("\n");
	RichEdit_SetSel(pTextServicesHistory, lStartChar, lEndChar);
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	m_reditHistory->EndDown();
}

LRESULT CDuiP2PSendMsgDlg::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI * pControl = m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{	
		if (pControl == m_reditSend)
		{
			m_pMenuMsgRButton = new DuiLib::CMenuWnd();
			ClientToScreen(m_hWnd, &pt);
			m_pMenuMsgRButton->Init(NULL, _T("ReditSendRbtnMenu.xml"), pt, &m_PaintManager, NULL);
			if(m_reditSend->GetSelectionType() == SEL_EMPTY)
			{
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
			}
			if(!m_reditSend->CanPaste())
			{
				//m_pMenuMsgRButton->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
			}
			if(m_reditSend->GetText() == "")
			{
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(3)->SetEnabled(false);
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(4)->SetEnabled(false);
			}
		}
		else if (pControl == m_reditHistory)
		{
			m_pMenuMsgRButton = new DuiLib::CMenuWnd();
			ClientToScreen(m_hWnd, &pt);
			m_pMenuMsgRButton->Init(NULL, _T("ReditRecvRbtnMenu.xml"), pt, &m_PaintManager, NULL);
			m_pMenuMsgRButton->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
			m_pMenuMsgRButton->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
			if(m_reditHistory->GetSelectionType() == SEL_EMPTY)
			{
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
			}
			if(m_reditHistory->GetText() == "")
			{
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(3)->SetEnabled(false);
				m_pMenuMsgRButton->GetMenuUI()->GetItemAt(4)->SetEnabled(false);
			}
		}
	}
	return 0;
}

void CDuiP2PSendMsgDlg::RbuttonDownWeb()
{

	DuiLib::CPoint pt;
	GetCursorPos(&pt);
	//ClientToScreen(m_hWnd, &pt);

	m_pMenuWebRButton = new DuiLib::CMenuWnd();
	m_pMenuWebRButton->Init(NULL, _T("WebRbtnMenu.xml"), pt, &m_PaintManager, NULL);
	//m_pMenuMsgRButton->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
	//m_pMenuMsgRButton->GetMenuUI()->GetItemAt(2)->SetEnabled(false);
	//if(m_reditHistory->GetSelectionType() == SEL_EMPTY)
	//{
	//	m_pMenuMsgRButton->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
	//}
	//if(m_reditHistory->GetText() == "")
	//{
	//	m_pMenuMsgRButton->GetMenuUI()->GetItemAt(3)->SetEnabled(false);
	//	m_pMenuMsgRButton->GetMenuUI()->GetItemAt(4)->SetEnabled(false);
	//}
}

BOOL CDuiP2PSendMsgDlg::CanTran()
{
	return m_bCanTran;
}

void CDuiP2PSendMsgDlg::TranCuster(int nTranId)
{
	int destid = nTranId;
	FRIEND_INFO *pDestFriend = ::FindFriend(destid);
	if(destid >= 0 && pDestFriend != NULL)
	{
		{
			// 客服转接后订单编号发到转接客服
			XT_MSG msg;
			memset(&msg,0,sizeof(msg));
			msg.ver         = XTREAM_CLIENT_VERSION;
			msg.from_id     = g_ImUserInfo.GetId();
			msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
			msg.to_id		= destid;
			msg.data_type   = XT_MSG::IM_MSG_ORDER_SEQ;//插入会话消息

			extern map<int, CString> g_mapFriendIdToOrder;
			CString text = g_mapFriendIdToOrder[m_FriendInfo.GetId()];
			text.Format("%d:%s", m_FriendInfo.GetId(), text);
			strcpy(msg.data, text);
			msg.data_len=strlen(text)+1;
			SF_SetTransMode(CIMProtocol::SMT_SERVER);
			g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);
		}
		// 发送正在转接消息
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

		msg.to_id		= m_FriendInfo.GetId();
		msg.dest_ip		= m_FriendInfo.ip;
		msg.dest_port	= m_FriendInfo.port;
		msg.msg_type    = 0;

		msg.data_type = XT_MSG::CUST_SER_TRAN_REQ;
		strcpy(msg.from_nickname, g_ImUserInfo.nickname);

		Json::FastWriter jWriter;
		Json::Value jvSellerInfo;
		jvSellerInfo["switchStatus"] = 1;		// 1 表示转接开始 0 表示转接成功 2 表示主管接手会话
		jvSellerInfo["toSubMerchantID"] = destid;
		jvSellerInfo["fromSubMerchantID"] = g_ImUserInfo.GetId();
#ifdef _CS_PLAT_EX_
		jvSellerInfo["toSubMerchantName"] = pDestFriend->GetUserName().c_str();
#else
		jvSellerInfo["toSubMerchantName"] = pDestFriend->nickname;
#endif
		string strMsg = jWriter.write(jvSellerInfo);

		strcpy(msg.data, strMsg.c_str());
		msg.data_len = strMsg.length() + 1;

		g_pProtocol->SendP2PMsg(msg);

		// 发送转接请求
		XT_CUSTSER_TRAN_REQ req;
		req.uSid = g_ImUserInfo.GetId();
		req.uDid = destid;
		strncpy(req.szOrderId, m_pCustSerTranDlg->m_strOrderID.c_str(), 63);
		strncpy(req.szTranReason, m_pCustSerTranDlg->m_strReason.c_str(), 511);
		req.uCustomerId = m_nFriendId;

		g_pProtocol->SendReq(req);

		delete m_pCustSerTranDlg;
		m_pCustSerTranDlg = NULL;

		//SendMsgDlgNotify();
	}
}

void CDuiP2PSendMsgDlg::AddTranRefuse(string strFromName)
{
	XT_FONT xf;
	xf.fontColor=RGB(254,154,105);

	
	m_reditHistory->SetSel(-1,-1);
	m_reditHistory->InsertText(-1, "\n");
	ITextServices* pTextServicesHistory = m_reditHistory->GetTextServices();
	RichEdit_SetFont(pTextServicesHistory, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);
	RichEdit_SetStartIndent(pTextServicesHistory, 80);

	CString strTranWait;
	strTranWait.Format("----\"%s\"拒绝了转接任务。", strFromName.c_str());
	m_reditHistory->InsertText(-1, strTranWait);
	m_reditHistory->EndDown();
}

void CDuiP2PSendMsgDlg::SetTranPre(bool bIsTranPre)
{
	FRIEND_INFO* pReadFriend = ::GetFriendInfo(m_FriendInfo.GetId());
	CDuiCSPlatDlg::GetInstance()->SetFriendListElementTranPre(pReadFriend, bIsTranPre);
}

void CDuiP2PSendMsgDlg::InsertMsg(int nServiceId, int nCusterId)
{
	if(g_ImUserInfo.jifen != 1 || m_FriendInfo.jifen == 1 )
	{
		CMessageBox::Model(NULL, _T("无法插入会话！请检查登录帐号是否为主管账号，插入会话帐号是否不为主管帐号！"), NULL, MB_OK);
		return;
	}
	if(m_nInsertBuyerId != 0)
	{
		if(m_nInsertBuyerId == nCusterId)
		{
			CMessageBox::Model(NULL, _T("正在运行，请先停止"), NULL, MB_OK);
			return;
		}
		else
		{
			CMessageBox::Model(NULL, _T("点击确认插入新的会话！"), NULL, MB_OK);
		}
	}
	m_nInsertBuyerId = nCusterId;
	//开启监控
	XT_MONITOR_START_REQ req;
	req.idMaster = g_ImUserInfo.GetId();
	req.idSeller = nServiceId;
	g_pProtocol->SendReq(req);

	Sleep(500);

	XT_MONITOR_INSERT_CHAT_REQ req2;
	req2.idMaster = g_ImUserInfo.GetId();
	req2.idSeller = nServiceId;
	req2.idBuyer = nCusterId;
	g_pProtocol->SendReq(req2);
}