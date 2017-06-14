// im\SendMessageDlg.cpp : 实现文件
//

#include "stdafx.h"

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
#include "MsgObj.h"
#include "linkresdlg.h"
#include "clientprotocol.h"
#include "SAConfig.h"
#include "../ImTest/ImTest.h"
#include "..\EvalueDlg.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"

#include "../json/json/json.h"
#include "../SystemSettingDlg.h"

#include "../HangupSessionNumSettingDlg.h"

#include "UserCardDlg.h"
#include "ScreenCtrl.h"
#include "DuiCounsel.h"
#include "P2PSendMsgDlgCloseHint.h"
#ifdef _VER_INTERNAL_
#include "..\InternalFriendInfoDlg.h"
#endif
#ifdef _BUSINESS_VESION
#include "..\CustSerTranDlg.h"
#endif
//#define _LOCAL_TEST


#define SIZE_VEDIO 189
#define COLOR_MSGTIP  RGB(144,122,88)
CString ToHexString(BYTE bytes[],int len);
HWND g_hP2PSendMSG = NULL;
#define RIGHT_URL_CLIPBOARD_CHECK "moumou-link: "
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _BUSINESS_VESION
const uint32 P2P_DLG_WIDTH = 700;
//聊天框高度
const uint32 P2P_DLG_HIGHT = 500;
//左侧边框宽度
const uint32 BORDER_WIDTH = 0;
//折叠按钮边栏宽度
const int BUTTON_FOLDER_WIDTH = 0;
//右侧广告栏宽度
const uint32 AD_WINDOW_WIDTH = 285;
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
extern CDuiCounsel *g_pDuiCounsel;
// CP2PSendMsgDlg 对话框
IMPLEMENT_DYNAMIC(CP2PSendMsgDlg, CXSkinDlg)
	IMPLEMENT_HMG_SCROLLBAR(CP2PSendMsgDlg)

CP2PSendMsgDlg::CP2PSendMsgDlg(FRIEND_INFO *pFriend, TP2PMode msgMode, CWnd *pParent, const char *szProductID /*= NULL*/)
	:CXSkinDlg(CP2PSendMsgDlg::IDD, pParent),
	m_wndTalkToolBar(&m_reditSend, &g_ImFont, this),
	m_wndToolBar(this),
	m_wndUserPanel(&m_FriendInfo,this),
	m_wndUserList(this),
	m_wndVedioPanel(this,this),
	m_wndAudioPanel(this,this),
	m_ctrlFaceIcon(IDB_PNG_ROUND_NORMAL,IDB_PNG_ROUND_NORMAL),
	m_strProductID("")
{
	//重写基类的背景色
	m_colorBackground = CHAT_BKG_COLOR;
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

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

	if(szProductID != NULL)
	{
		m_strProductID = szProductID;
	}
}

CP2PSendMsgDlg::~CP2PSendMsgDlg()
{	
	//StopSendThread();////发送
	//StopRecvThread();////接收
	DestroyIcon(m_hSmallIcon);
	DestroyIcon(m_hBigIcon);

	if(m_hAccel)
	{
		DestroyAcceleratorTable(m_hAccel);
	}
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
}
//}

void CP2PSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RE_HISTORY, m_reditHistory);
	DDX_Control(pDX, IDC_RE_SEND, m_reditSend);
	//	DDX_Control(pDX, IDC_STATIC_TITLE, m_captionBar);
	DDX_Control(pDX, IDC_BTN_HISTORY, m_btnHistory);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_BTN_AUDIO, m_btnAudio);
	DDX_Control(pDX, IDC_BTN_VEDIO, m_btnVedio);
	DDX_Control(pDX, IDC_BTN_SENDFILE, m_btnSendFile);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_imageTitle);
	DDX_Control(pDX, IDC_STATIC_HIDE1, m_wndHide);
	DDX_Control(pDX, IDC_BTN_FOLD,m_btnFolder);
	DDX_Control(pDX, IDC_FILETRANS_PROGRESS, m_wndP2PFileTransProgressBar);
	DDX_Control(pDX, IDC_WEB_AD, m_wndAD);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrlFaceIcon);
	DDX_Control(pDX, IDC_BUTTON_HANGUP, m_btnHangup);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAddMember);
	DDX_Control(pDX, IDC_BTN_CUSTSER_TRAN, m_btnCustSerTran);
}


BEGIN_MESSAGE_MAP(CP2PSendMsgDlg, CXSkinDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FACE,OnFaceIconClicked)
	//ON_BN_CLICKED(IDC_BUTTON_C,OnBnClickedBtnColor)
	//ON_BN_CLICKED(IDC_BUTTON_I,OnFontItalicClicked)
	//ON_BN_CLICKED(IDC_BUTTON_U,OnFontUnderLineClicked)
	ON_BN_CLICKED(IDC_BUTTON_SCRN,OnScreenCopyEx)
	ON_BN_CLICKED(IDC_BUTTON_SCRN_HOTKEY,OnScreenCopy)
	//ON_BN_CLICKED(IDC_BUTTON_B,OnFontBoldClicked)
	ON_BN_CLICKED(IDC_BUTTON_PIC,OnSendPicClicked)
	ON_BN_CLICKED(IDC_BUTTON_MAGIC,OnMagicFace)
	ON_BN_CLICKED(IDC_BTN_INVITE,OnInvite)
	ON_BN_CLICKED(IDC_BTN_SENDFILE,OnSendFile)
	ON_BN_CLICKED(IDC_BUTTON_LINK,OnLinkResClicked)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY, OnBnClickedButtonHistory)
	ON_BN_CLICKED(IDC_BUTTON_SHAKE, OnBnClickedButtonShake)
	ON_MESSAGE(WM_QUERY_PEERONLINE,OnQueryPeerOnline)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBnClickedBtnSendEx)
	ON_COMMAND(IDC_BUTTON_FONT,OnBnClickedFontBar)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_HISTORY, OnBnClickedBtnHistory)
	ON_MESSAGE(WM_DOWNLOADFACE,OnStartDownloadFace)
	ON_MESSAGE(WM_DOWNLOADFACE_OK,OnDownloadFaceOK)
	ON_MESSAGE(WM_DOWNLOADPROCESS,OnDownloadProcess)
	ON_MESSAGE(WM_EMOTIONSEL_NOTIFY,OnEmotionSelNotify)
	ON_MESSAGE(WM_MAGICFACE_NOTIFY,OnMagicFaceNotify)
	ON_MESSAGE(WM_MAGICFACE_CLICK,OnMagicFaceClick)
	ON_MESSAGE(WM_USER_DROPFILE,OnUserDragDropFile)
	ON_MESSAGE(WM_SNAPSHOT,OnSnapShot)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_XT_MESSAGE()
	ON_WM_TIMER()
	SET_HMG_SCROLLBAR(IDC_RE_HISTORY)
	SET_HMG_SCROLLBAR(IDC_RE_SEND)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_UNDERLINE, OnUpdateFontStyle)
	ON_UPDATE_COMMAND_UI(ID_BOLD, OnUpdateFontStyle)
	ON_UPDATE_COMMAND_UI(ID_ITALIC, OnUpdateFontStyle)
	ON_WM_PAINT()	
	ON_BN_CLICKED(IDC_BTN_CARD, OnBnClickedBtnCard)
	ON_BN_CLICKED(IDC_BTN_VISIT, OnBnClickedBtnVisit)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_THREAD_NOTIFY, OnThreadNotify)
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BTN_FOLD, OnBnClickedBtnFold)
	ON_MESSAGE(WM_P2PVEDIO_NOTIFY,OnP2PVedioNotify)
	ON_MESSAGE(WM_P2PAUDIO_NOTIFY,OnP2PAudioNotify)
	ON_MESSAGE(WM_P2PNET_NOTIFY,OnP2PNetNotify)
	ON_MESSAGE(WM_INSERTMSG,OnInsertMsg)
	ON_COMMAND(ID_ACC_IP, OnAccIp)
	ON_MESSAGE(WM_RICHEDIT_NOTIFY,OnRichEditLinkClilck)
	ON_BN_CLICKED(IDC_BTN_AUDIO, OnBnClickedBtnAudio)
	ON_BN_CLICKED(IDC_BTN_VEDIO, OnBnClickedBtnVedio)
	ON_MESSAGE(WM_FILERECV_OK,OnRecvFileOK)
	ON_MESSAGE(WM_FILERECV_FAILED,OnRecvFileFailed)
	ON_MESSAGE(WM_FILESEND_OK,OnSendFileOK)
	ON_MESSAGE(WM_FILESEND_FAILED,OnSendFileFailed)
	ON_BN_CLICKED(IDC_BUTTON_SCORE, OnBnClickedButtonScore)
	ON_BN_CLICKED(IDC_BUTTON_FASTWORD, OnBnClickedFastword)
#ifdef _BUSINESS_VESION
	ON_BN_CLICKED(IDC_BUTTON_HANGUP, OnBnClickedBtnHangup)
	ON_BN_CLICKED(IDC_BTN_CUSTSER_TRAN, &CP2PSendMsgDlg::OnBnClickedBtnCustserTran)
#endif
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBnClickedBtnAdd)
	ON_MESSAGE(WM_CLOSETABCTRL_CLOSE,OnCloseTabCtrlClose)
	ON_MESSAGE(WM_CLOSE_ALL,OnCloseAll)
	ON_MESSAGE(WM_CLOSE_ONE,OnCloseOne)
	ON_MESSAGE(WM_KEYDOWN_RETURN,OnKeyDownReturn)
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()
//
//BEGIN_XT_MESSAGE_MAP(CAdvancedFileTransSession)

//END_XT_MESSAGE_MAP()	
//
//BEGIN_XT_RESEND_MAP(CP2PSendMsgDlg)
//ON_XT_RESEND(CMD_DOWNLOAD_REQ,AF_OnDownloadCongestion)
//END_XT_RESEND_MAP()

BEGIN_XT_MESSAGE_MAP(CP2PSendMsgDlg)
	// 临时群成员改动通知
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	// 好友状态改变通知
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnFriendStatusChange)
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

BEGIN_EVENTSINK_MAP(CP2PSendMsgDlg, CXSkinDlg)
	ON_EVENT(CP2PSendMsgDlg, IDC_WEB_AD, 250, CP2PSendMsgDlg::BeforeNavigate2WebAd, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CP2PSendMsgDlg, IDC_WEB_AD, 252, CP2PSendMsgDlg::NavigateComplete2WebAd, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP() 

// CP2PSendMsgDlg 消息处理程序
BOOL CP2PSendMsgDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	m_btnCustSerTran.ShowWindow(SW_HIDE);

	SetDlgStyle(ALL);
	EnableHelp(true);
	g_hP2PSendMSG = GetSafeHwnd();
	//显示头像
	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&m_FriendInfo);
	if ( /*m_FriendInfo.status!=XTREAM_OFFLINE &&*/ FileExist(strFaceFile) )
	{
		m_ctrlFaceIcon.SetPicPath(strFaceFile, m_FriendInfo.status == XTREAM_ONLINE 
			|| m_FriendInfo.status == XTREAM_BUSY
			|| m_FriendInfo.status == XTREAM_LEAVE);
		//m_ctrlFaceIcon.SetFaceFile(strFaceFile,0);
	}
	else
	{
		//m_ctrlFaceIcon.SetUserInfo(&m_FriendInfo);
	}


	//还要显示组名。。。
	
	int nGroupID = m_FriendInfo.group_index;
	int nStep = 0, nGroupIdx = 0;
	std::list<CString> listGroupName;
	while(nGroupID != -1 && nStep < g_GroupInfo.count)
	{
		nGroupIdx = g_GroupInfo.FindItemIndex(nGroupID);
		listGroupName.push_back((CString)g_GroupInfo.group[nGroupIdx].name);
		nGroupID = g_GroupInfo.group[nGroupIdx].group_father;
		nStep++;
	}

	m_strGroupName = "";
	while( !listGroupName.empty() )
	{
		m_strGroupName += listGroupName.back() + "/";
		listGroupName.pop_back();
	}
	
	//显示用户名和签名
	GetDlgItem(IDC_STATIC_USERNAME)->SetFont(&g_font16B); 
	SetDlgItemText(IDC_STATIC_USERNAME, UserInfoToStr(&m_FriendInfo));
	
	//计算用户名的宽度
	CDC *pDC = GetDlgItem( IDC_STATIC_USERNAME )->GetDC();
	CSize size = pDC->GetTextExtent( UserInfoToStr(&m_FriendInfo) );
	GetDlgItem( IDC_STATIC_USERNAME )->ReleaseDC( pDC );

	//计算用户状态的左起位置，并移动
	CRect rectUserName,rectStatusGroup;
	GetDlgItem( IDC_STATIC_USERNAME )->GetWindowRect(&rectUserName);
	GetDlgItem(IDC_STATIC_STATUS_GROUP)->GetWindowRect(&rectStatusGroup);
	rectStatusGroup.left = rectUserName.left + size.cx + 40;
	GetDlgItem(IDC_STATIC_STATUS_GROUP)->MoveWindow(rectStatusGroup);


	GetDlgItem(IDC_STATIC_STATUS_GROUP)->SetFont(&g_font12); 
	SetDlgItemText(IDC_STATIC_STATUS_GROUP, "[" + StatusToStr(m_FriendInfo.status) + "]" + "（" + m_strGroupName +"）") ;
	SetDlgItemText(IDC_STATIC_STATUS_GROUP, "") ;

	GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
	SetDlgItemText(IDC_STATIC_SIGN, m_FriendInfo.user_sign);

	m_btnAddMember.SetPng(IDB_PNG_ADD_GROUP_HOVER, IDB_PNG_ADD_GROUP_NORMAL, IDB_PNG_ADD_GROUP_PRESSED, IDB_PNG_ADD_GROUP_DISABLE);

	//显示广告栏
	// 	char exefile[256];
	// 	GetModuleFileName(NULL, exefile, sizeof(exefile));
	// 	CString html = exefile;
	// 	html = html.Left(html.ReverseFind('\\'));
	// 	html += "\\dialog\\right.html";
	//m_wndAD.Navigate("http://www.baidu.com",&variant1,&variant2,&variant3,&variant4);


/*
	mid-商家id
	flag：为1时表示详情页进来的，为2时表示店铺页进来的
	pid：flag为1时使用，商品id
	sid：flag为2时使用，店铺id
	gid没得用。
	pid：productid
	sid: shopid
	*/


	CString strPara;
#ifdef _BUSINESS_VESION

	//对面是买家
	if( m_FriendInfo.GetGuid().length() > 0 && CMainFrame::IsBuyer(m_FriendInfo.GetId()))
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
		strPara.Format("?merchantID=%d&userGID=%s&isOppositeCustomer=true&userName=%s&userComment=%s&productID=%s",g_ImUserInfo.merchant_id,
			m_FriendInfo.GetGuid().c_str(),strUserName.c_str(),strComment.c_str(), m_strProductID.c_str());
	}
	//对面是卖家或者guid空
	else  /*if(m_FriendInfo.id < 5000000 && m_FriendInfo.id > 0)*/
	{
		strPara.Format("?merchantID=%d&isOppositeCustomer=false",g_ImUserInfo.merchant_id);
	}

	DWORD dT = GetTickCount();
	string strTemp = (LPCTSTR)strPara;
	strPara.Format("%s&%d", strTemp.c_str(), dT);
	m_strAdUrl = RIGHT_URL_SELLER + strPara;

#else
	if(theApp.m_nProductID != 0 )
	{
		strPara.Format("?mid=%d&flag=1&pid=%d",theApp.m_nMerchantID,theApp.m_nProductID);
	}
	else if(!CMainFrame::IsBuyer(m_FriendInfo.GetId()) && m_FriendInfo.GetId() > 0)
	{
		strPara.Format("?smid=%d",m_FriendInfo.GetId());
	}
	
	m_strAdUrl = RIGHT_URL + strPara;
#endif


	m_wndAD.Navigate(m_strAdUrl,NULL,NULL,NULL,NULL);
	//m_wndAD.Refresh();
	m_wndAD.ShowScrollBar(0, FALSE);
	
	m_reditHistory.SetEmotionImageList(&g_imgEmotionList);
	m_reditSend.SetEmotionImageList(&g_imgEmotionList);
	m_reditSend.EnableZYF(m_FriendInfo.isTalkZYF==1);

	m_reditSend.SetXFont(g_ImFont);

	// 	m_btnInvite.SetTextColor(RGB(0,84,166));
	// 	m_btnInvite.SetBitmap(IDB_BTN_INVITE,IDB_BTN_INVITE,RGB(0,255,0));
	// 	m_btnInvite.SetFlatStyle(CFlatButton::FS_OFFICE,CFlatButton::AS_BOTTOM);

	m_btnAudio.SetPng(IDB_PNG_P2P_CHAT_HOVER,IDB_PNG_P2P_CHAT_NORMAL,IDB_PNG_P2P_CHAT_PRESSED,IDB_PNG_P2P_CHAT_NORMAL);
	m_btnAudio.SetToolTip("开始语音通话");
	m_btnVedio.SetPng(IDB_PNG_P2P_VIDEO_HOVER,IDB_PNG_P2P_VIDEO_NORMAL,IDB_PNG_P2P_VIDEO_PRESSED,IDB_PNG_P2P_VIDEO_NORMAL);
	m_btnVedio.SetToolTip("开始视频通话");
	m_btnSendFile.SetPng(IDB_PNG_SENDFILE_HOVER,IDB_PNG_SENDFILE_NORMAL,IDB_PNG_SENDFILE_PRESSED,IDB_PNG_SENDFILE_NORMAL);
	m_btnSendFile.SetToolTip("发送文件");

	// 	m_btnCard.SetTextColor(RGB(0,84,166));
	// 	m_btnCard.SetBitmap(IDB_BTN_CARD,IDB_BTN_CARD,RGB(0,255,0));
	// 	m_btnCard.SetFlatStyle(CFlatButton::FS_OFFICE,CFlatButton::AS_BOTTOM);
	// 
	// 	m_btnVisit.SetTextColor(RGB(0,84,166));
	// 	m_btnVisit.SetBitmap(IDB_BTN_VISIT,IDB_BTN_VISIT,RGB(0,255,0));
	// 	m_btnVisit.SetFlatStyle(CFlatButton::FS_OFFICE,CFlatButton::AS_BOTTOM);

	//	m_btnInvite.SetToolTip("邀请好友加入多人聊天");

	SetBtnFolderBitmap();

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_SPLITTER1);
	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER1);

	m_wndUserPanel.Create(CP2PUserPanel::IDD,this);
	m_wndUserPanel.MoveWindow(rc);

	m_wndUserList.Create(CP2PUserList::IDD,this);
	m_wndUserList.MoveWindow(rc);

	m_wndAudioPanel.Create(CP2PAudioPanel::IDD,this);
	m_wndAudioPanel.MoveWindow(rc);

	m_wndVedioPanel.Create(CP2PVedioPanel::IDD,this);
	m_wndVedioPanel.MoveWindow(rc);

	m_wndTalkToolBar.Create(CP2PTalkToolBar::IDD,this);
	m_wndTalkToolBar.MoveWindow(rc);
	m_wndTalkToolBar.ShowWindow(SW_HIDE);

	m_wndToolBar.Create(CP2PToolBar::IDD,this);
	m_wndToolBar.MoveWindow(rc);
	m_wndToolBar.ShowWindow(SW_SHOW);

	m_wndPopupHistory.Create(CPopupHistoryDlg::IDD, this);
	m_wndPopupHistory.ShowWindow(SW_HIDE);

#ifdef _VER_INTERNAL_
	if(m_pmP2PMode != pmMultiTalk && m_pmP2PMode != pmNone)
	{
		m_pInterNalFriendInfo = new CInternalFriendInfoDlg(&m_FriendInfo, this);
		m_pInterNalFriendInfo->Create(CInternalFriendInfoDlg::IDD, this);
	}
#endif
	//获取右侧聊天记录窗口宽度
	CRect rcPopupHistory;
	m_wndPopupHistory.GetClientRect(rcPopupHistory);
	m_uPopupHistoryWidth = rcPopupHistory.Width();

	m_reditSend.SetStartIndent(80);
	//InitFontToolBar();

	//不显示图片标题栏
	//m_imageTitle.SetBitmap(IDB_TALK_TITLE_BACK);
	m_imageTitle.ShowWindow(SW_HIDE);

	m_btnHistory.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
	//m_btnSend.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
	m_btnClose.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
	//m_btnHotKey.SetBitmap(IDB_BMP_HOTKEY_HOVER,IDB_BMP_HOTKEY_NORMAL,IDB_BMP_HOTKEY_PRESSED,IDB_BMP_HOTKEY_NORMAL);
	UpdateP2PPanel();

	Init(OnRecvVedioData,(DWORD)this,g_ImUserInfo.GetId(),g_pProtocol->m_pUdpComm);
	switch(m_pmP2PMode)
	{
	case pmSingleTalk:
		//显示历史5条记录
		LoadLastTalkMsg();
		break;
	case pmMultiTalk:
		break;
	case pmVedio:
		break;
	case pmAudio:
		break;
	}


	ShowDlgTitle();

	m_wndSplitter1.SetLimitSize(100);

	AttachHMGScrollbar(m_reditHistory.m_hWnd);
	AttachHMGScrollbar(m_reditSend.m_hWnd);

	g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
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

#ifdef _BUSINESS_VESION	
	g_pProtocol->RegistCmd(CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_NEAR_COMM_CUST_REMARK_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CUSTSER_TRAN_ACK, m_hWnd);
#endif
	////g_pProtocol->RegistCmd(CMD_FILE_TRANS_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FILE_TRANS_IND,m_hWnd);

	AF_RegistXtCmd(m_hWnd);
	EnableToolTips();

	m_toolTip.Create(this, TTS_ALWAYSTIP);

	SetTimer(0x01,1000,NULL);
	SetTimer(TIMER_DWN_CPIC,TIME_DWN_CPIC,NULL);
	// 给出提示信息
	AddP2PTip();
	PopMsg();

	m_reditSend.SetFocus();

	// 如果没有获取过好友状态，则获取
	if(m_FriendInfo.ip == 0 )
	{	
		if( m_FriendInfo.status != XTREAM_OFFLINE  )
		{
			// 在允许的次数内重试获取好友最新的ip
			if(m_FriendInfo.p2pRetry > 0)
			{
				m_FriendInfo.p2pRetry--;	// 可以去掉				
				// 获取当前好友的ip和端口
				RefreshFriendStatus();
			}
		}
	}

	m_lblIP.m_ip = m_FriendInfo.ip;
	m_lblIP.m_port = m_FriendInfo.port;
	m_lblIP.m_localIp = m_FriendInfo.localIP;
	m_lblIP.m_localPort = m_FriendInfo.localPort;
	//m_lblIP.SetWindowText(sHide);
	SetIcon(m_hBigIcon,TRUE);
	SetIcon(m_hSmallIcon,FALSE);


	m_reditSend.DragAcceptFiles(TRUE);

	m_bDlgInit = true;

	m_wndP2PFileTransProgressBar.ShowWindow(SW_HIDE);

	if ( this->m_pmP2PMode==pmSingleTalk && m_FriendInfo.bRefreshed==false )
	{
		//更新好友资料
		XT_USERINFO_GET_REQ req;
		req.fid = m_FriendInfo.GetId();
		g_pProtocol->SendReq(req);
	}

	CRect rcDlg;
	GetWindowRect(rcDlg);
	rcDlg.right = rcDlg.left + P2P_DLG_WIDTH;
	rcDlg.bottom = rcDlg.top + P2P_DLG_HIGHT;
	MoveWindow(rcDlg);

	//组内群发添加按钮随对话框变化
	CRect rcAddMember;
	m_btnAddMember.GetWindowRect(&rcAddMember);
	rcAddMember.left = rcDlg.right - rcAddMember.Width();
	rcAddMember.right = rcDlg.right;
	m_btnAddMember.MoveWindow(rcAddMember.left, rcAddMember.top-2, rcAddMember.Width(), rcAddMember.Height());

	AdjustUI();

	NotifyVideoEquipment(&m_FriendInfo);
	SetTimer(TIMER_P2P_STATUS,1000,NULL);

// #ifdef _BUSINESS_VESION	// 卖家版中判断聊天成员是否是我的团队成员，如果不是，则不显示“推送评价”按钮
// 	if (CheckSubGroupIsInMyTeam(m_FriendInfo.group_index))
// 	{
// 		ShowOrHideScoreBtn(FALSE);
// 	}
// 
// 	m_btnHangup.SetPng(IDB_PNG_HANGUP_HOVER, IDB_PNG_HANGUP_NORMAL, IDB_PNG_HANGUP_PRESSED, IDB_PNG_HANGUP_DISABLE);
// 	if(m_pmP2PMode==pmSingleTalk && m_FriendInfo.group_index < MY_TEAM_GROUP_ID)
// 	{	// 只有接待非团队成员时才显示客服转接
// 		m_btnCustSerTran.ShowWindow(SW_SHOW);
// 	}
// 	m_btnCustSerTran.SetPng(IDB_PNG_CUSTSER_HOVER, IDB_PNG_CUSTSER_NORMAL, IDB_PNG_CUSTSER_PRESSED, IDB_PNG_CUSTSER_PRESSED);
// 	m_btnCustSerTran.SetToolTip("客服转接");
// 	SetTimer(TIMER_CLIPBOARD, 10,NULL);
// 
// 	//请求右侧备注
// 	m_strNewComnCustRemarkServer = "";
// 	if(m_FriendInfo.GetId() > 5000000)
// 	{
// 		XT_GET_NEAR_COMM_CUST_REMARK_REQ req;
// 		req.uiBusinessID = g_ImUserInfo.GetId();
// 		strcpy(req.szCommAccount, m_FriendInfo.GetUserName().c_str());
// 		g_pProtocol->SendReq(req);
// 	}
// #endif

	SetDlgItemText(IDC_STATIC_USERNAME, "临时会话");
	m_btnSendFile.ShowWindow(SW_HIDE);
	return TRUE;  
}

void CP2PSendMsgDlg::InitResource()
{
	CXSkinDlg::InitResource();

	m_bmpMinHot.LoadBitmap(IDB_CHAT_MIN_HOT);
	m_bmpMinNormal.LoadBitmap(IDB_CHAT_MIN_NORMAL);
	m_bmpMinDown.LoadBitmap(IDB_CHAT_MIN_DOWN);

	m_bmpMaxNormal.LoadBitmap(IDB_CHAT_MAX_NORMAL);
	m_bmpMaxHot.LoadBitmap(IDB_CHAT_MAX_HOT);
	m_bmpMaxDown.LoadBitmap(IDB_CHAT_MAX_DOWN);

	m_bmpRestoreHot.LoadBitmap(IDB_CHAT_MAX_HOT);
	m_bmpRestoreNormal.LoadBitmap(IDB_CHAT_MAX_NORMAL);
	m_bmpRestoreDown.LoadBitmap(IDB_CHAT_MAX_DOWN);

	m_bmpCloseNormal.LoadBitmap(IDB_CHAT_CLOSE_NORMAL);
	m_bmpCloseHot.LoadBitmap(IDB_CHAT_CLOSE_HOT);
	m_bmpCloseDown.LoadBitmap(IDB_CHAT_CLOSE_DOWN);
}


void CP2PSendMsgDlg::UpdateSignature()
{	

	GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
	SetDlgItemText(IDC_STATIC_SIGN, m_FriendInfo.user_sign);
	Invalidate();
}


void CP2PSendMsgDlg::OnFaceIconClicked()
{
	CPoint pt;
	GetCursorPos(&pt);

	CEmotionDlg *dlg = new CEmotionDlg(pt.x,pt.y,&g_imgEmotionList,this);
	dlg->m_hEmotionSelWnd = m_hWnd;
	dlg->ShowWindow(SW_SHOW);	
}

void CP2PSendMsgDlg::OnSendPicClicked()
{
	bool bCanSendPic=false ;

	if ( m_pmP2PMode!=pmMultiTalk )
	{
		if ( m_FriendInfo.status==XTREAM_OFFLINE )
		{
			//MessageBox("对不起对方不在线！不能发送图片！",IM_CAPTION,MB_OK|MB_ICONWARNING);	
			ShowMessage("对方不在线或隐身！不能发送图片！");
		}
		else
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
		CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,this);

		if ( IDOK==dlg.DoModal() )
		{
			if ( dlg.GetSelFileSize()<1024 )
			{
				CString strCacheFileName;
				g_LocalData.SaveImageFileToCache(dlg.GetPathName(),strCacheFileName);
				CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

				//m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());
				m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);
				m_reditSend.SetFocus();
			}
			else
			{
				ShowMessage("对不起，您只能发送小于1M的图片！",MB_ICONINFORMATION|MB_OK);
			}
		}
	}
}

//void CP2PSendMsgDlg::OnFontBoldClicked()
//{
//	DWORD dwOld = g_ImFont.flags & XTF_BOLD;
//
//	if ( dwOld )
//	{
//		g_ImFont.flags &= ~XTF_BOLD;
//	}
//	else
//	{
//		g_ImFont.flags |= XTF_BOLD;
//	}
//
//	m_reditSend.SetXFont(g_ImFont);
//	m_reditSend.SetFocus();
//
//	m_wndTalkToolBar.UpdateFontBtnTips();
//}

//void CP2PSendMsgDlg::OnFontItalicClicked()
//{
//	DWORD dwOld = g_ImFont.flags & XTF_ITALIC;
//
//	if ( dwOld )
//	{
//		g_ImFont.flags &= ~XTF_ITALIC;
//	}
//	else
//	{
//		g_ImFont.flags |= XTF_ITALIC;
//	}
//
//	m_reditSend.SetXFont(g_ImFont);
//	m_reditSend.SetFocus();
//
//	m_wndTalkToolBar.UpdateFontBtnTips();
//}


//void CP2PSendMsgDlg::OnFontUnderLineClicked()
//{
//	DWORD dwOld = g_ImFont.flags & XTF_UNDERLINE;
//
//	if ( dwOld )
//	{
//		g_ImFont.flags &= ~XTF_UNDERLINE;
//	}
//	else
//	{
//		g_ImFont.flags |= XTF_UNDERLINE;
//	}
//
//	m_reditSend.SetXFont(g_ImFont);
//	m_reditSend.SetFocus();
//
//	m_wndTalkToolBar.UpdateFontBtnTips();
//}

//void CP2PSendMsgDlg::OnBnClickedBtnColor()
//{
//	CColorDialog dlg(g_ImFont.fontColor,0,this);
//
//	if ( dlg.DoModal()==IDOK )
//	{
//		g_ImFont.fontColor = dlg.GetColor();
//	}
//
//	m_reditSend.SetXFont(g_ImFont);
//	m_reditSend.SetFocus(); 	
//}

void CP2PSendMsgDlg::OnBnClickedBtnClose()
{

	DestroyWindow();
}

void CP2PSendMsgDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	//int screen_cy = GetSystemMetrics(SM_CYSCREEN);

	//CWnd *pShellTrayWnd = FindWindow("Shell_TrayWnd",NULL);

	//if (pShellTrayWnd)
	//{
	//	CRect rcShellTrayWnd;
	//	pShellTrayWnd->GetWindowRect(&rcShellTrayWnd);

	//	if (rcShellTrayWnd.left<=0 && rcShellTrayWnd.bottom>=screen_cy )
	//	{
	//		//shell tray wnd on screen bottom
	//		screen_cy=rcShellTrayWnd.top;
	//	}
	//}


	//lpMMI->ptMaxTrackSize.x = screen_cx;
	//lpMMI->ptMaxTrackSize.y = screen_cy;

	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

HBRUSH CP2PSendMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);


	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

void CP2PSendMsgDlg::ShowFileTransInfoBar(bool bShow)
{
	if (bShow)
	{
		if ( m_wndP2PFileTransProgressBar.IsWindowVisible()==FALSE )
		{
			CRect rc;
			m_reditHistory.GetWindowRect(&rc);
			ScreenToClient(&rc);


			SCROLLINFO si1;
			memset(&si1,0,sizeof(si1));
			si1.cbSize = sizeof(si1);

			m_reditHistory.GetScrollInfo(SB_VERT,&si1);


			m_wndP2PFileTransProgressBar.MoveWindow(rc.left,rc.top,rc.Width(),65);
			m_reditHistory.MoveWindow(rc.left,rc.top+65,rc.Width(),rc.Height()-65);

			m_wndP2PFileTransProgressBar.ShowWindow(SW_SHOW);


			SCROLLINFO si2;
			memset(&si2,0,sizeof(si2));
			si2.cbSize = sizeof(si2);

			m_reditHistory.SetSel(-1,-1);
			m_reditHistory.GetScrollInfo(SB_VERT,&si2);

			si2.nPos = si2.nMax-(si1.nMax-si1.nPos);
			if ( si2.nPos<0 )
				si2.nPos=0;

			m_reditHistory.SetScrollInfo(SB_VERT,&si2);
		}
	}
	else
	{
		if ( m_wndP2PFileTransProgressBar.IsWindowVisible() )
		{
			CRect rc;
			m_reditHistory.GetWindowRect(&rc);
			ScreenToClient(&rc);


			SCROLLINFO si1;
			memset(&si1,0,sizeof(si1));
			si1.cbSize = sizeof(si1);

			m_reditHistory.GetScrollInfo(SB_VERT,&si1);

			m_wndP2PFileTransProgressBar.ShowWindow(SW_HIDE);
			rc.top-=65;

			m_reditHistory.MoveWindow(&rc);   

			SCROLLINFO si2;
			memset(&si2,0,sizeof(si2));
			si2.cbSize = sizeof(si2);

			m_reditHistory.GetScrollInfo(SB_VERT,&si2);

			si2.nPos = si2.nMax-(si1.nMax-si1.nPos);
			if ( si2.nPos<0 )
				si2.nPos=0;

			m_reditHistory.SetScrollInfo(SB_VERT,&si2);
		}
	}
}

void CP2PSendMsgDlg::AdjustUI()
{
	if ( m_wndHide.m_hWnd==NULL )
		return ;

	CRect rc; 
	GetClientRect(&rc);

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

	CPoint ptUserPanel;
	ptUserPanel.x = cx-nBarCx-BUTTON_FOLDER_WIDTH, ptUserPanel.y = nTop;

	CSize  szUserPanel;
	szUserPanel.cx = nBarCx, szUserPanel.cy = cy-nTop-1;

	//m_imageTitle.MoveWindow(0,nTop,cx,31);

	//nTop+=34;

	//不显示折叠按钮
	//m_btnFolder.MoveWindow(cx-BUTTON_FOLDER_WIDTH-2,nTop-1, BUTTON_FOLDER_WIDTH,21);
	m_btnFolder.ShowWindow(SW_HIDE);

	CPoint ptHistory;
	ptHistory.x = nLeft, ptHistory.y=nTop;

	CSize  szHistory;
	szHistory.cx = nClientCx, szHistory.cy=cy-m_nSplitterPos-nTop;

	if ( m_wndTalkToolBar.IsWindowVisible() )
	{
		szHistory.cy-=30;
	}

	nTop = cy-m_nSplitterPos;

	CPoint ptSplitter;
	ptSplitter.x = nLeft, ptSplitter.y = nTop+1;

	CSize  szSplitter;
	szSplitter.cx = nClientCx, szSplitter.cy = 3;

	CPoint ptToolBar;
	ptToolBar.x = nLeft, ptToolBar.y = nTop;

	CSize  szToolBar;
	szToolBar.cx = nClientCx, szToolBar.cy = 28;

	nTop +=28;

	CPoint ptSend;
	ptSend.x = nLeft, ptSend.y = nTop;

	CSize  szSend;
	szSend.cx = nClientCx, szSend.cy = cy - 35-nTop;

	nTop+=(3+szSend.cy);

	CSize szBtn;

	bool bSizeNS=true, bSizeEW=true;

	if ( rc.Height()<m_rcPre.Height() )
		bSizeNS=false;

	if ( rc.Width()<m_rcPre.Width() )
		bSizeEW=false;

	if ( bSizeNS )
	{    
		if ( bSizeEW )
		{	
			LayoutUserWindow(ptUserPanel, szUserPanel, ptHistory, szHistory);
			LayoutFileWindow(ptSend, szSend, ptHistory, szHistory, ptSplitter, szSplitter, ptToolBar, szToolBar);
			LayoutButton(nLeft, nTop, szBtn, nClientCx);
		}
		else
		{
			LayoutButton(nLeft, nTop, szBtn, nClientCx);
			LayoutFileWindow(ptSend, szSend, ptHistory, szHistory, ptSplitter, szSplitter, ptToolBar, szToolBar);
			LayoutUserWindow(ptUserPanel, szUserPanel, ptHistory, szHistory);
		}	
	}
	else
	{
		if ( bSizeEW )
		{	
			LayoutUserWindow(ptUserPanel, szUserPanel, ptHistory, szHistory);
			LayoutFileWindow(ptSend, szSend, ptHistory, szHistory, ptSplitter, szSplitter, ptToolBar, szToolBar);
		}
		else
		{
			LayoutFileWindow(ptSend, szSend, ptHistory, szHistory, ptSplitter, szSplitter, ptToolBar, szToolBar);
			LayoutUserWindow(ptUserPanel, szUserPanel, ptHistory, szHistory);
		}

		LayoutButton(nLeft, nTop, szBtn, nClientCx);
	}

	if ( m_wndTalkToolBar.IsWindowVisible() )
	{
		m_wndTalkToolBar.MoveWindow(ptHistory.x,ptHistory.y+szHistory.cy,nClientCx,30);
	}

	CRgn rgnClient;
	GetRedrawRgn(rgnClient);
	InvalidateRgn(&rgnClient);

	m_rcPre = rc;

#ifdef _BUSINESS_VESION
	CRect rcDlg;
	GetWindowRect(rcDlg);
	CRect rcBtn;
	rcBtn.left = rcDlg.right - TITLE_BAR_CY*4 + 1;
	rcBtn.right = rcBtn.left + TITLE_BAR_CY;
	rcBtn.top = rcDlg.top + 1;
	rcBtn.bottom = rcBtn.top + TITLE_BAR_CY;
	ScreenToClient(rcBtn);
	m_btnHangup.MoveWindow(rcBtn);
	m_btnHangup.ShowWindow(SW_SHOW);
#endif
}

void CP2PSendMsgDlg::LayoutButton(int nLeft, int nTop, CSize& szBtn, int nClientCx)
{
	m_btnHistory.MoveWindow(nLeft,nTop, 0,0,false);
	szBtn=m_btnHistory.SizeToContent();
	m_btnHistory.Invalidate();

	m_btnClose.MoveWindow(nClientCx-szBtn.cx*2-3,nTop,szBtn.cx,szBtn.cy);
	m_btnClose.SizeToContent();
	//m_btnClose.Invalidate();

	m_btnSend.MoveWindow(nClientCx-szBtn.cx-1,nTop,szBtn.cx,szBtn.cy);
	m_btnSend.SizeToContent();

	/*szBtn=m_btnHotKey.SizeToContent();
	m_btnHotKey.MoveWindow(nClientCx-24,nTop,16,szBtn.cy);
	m_btnHotKey.SizeToContent();*/
	//m_btnSend.Invalidate();
}

void CP2PSendMsgDlg::LayoutFileWindow(CPoint& ptSend, CSize& szSend, CPoint& ptHistory, CSize& szHistory,
	CPoint& ptSplitter, CSize& szSplitter, CPoint& ptToolBar, CSize& szToolBar)
{
	m_reditSend.MoveWindow(ptSend.x,ptSend.y,szSend.cx,szSend.cy);

	if ( m_wndP2PFileTransProgressBar.IsWindowVisible()==TRUE )
	{
		int nProgressBarHeight = 65;
		m_reditHistory.MoveWindow(ptHistory.x,ptHistory.y+nProgressBarHeight,szHistory.cx,szHistory.cy-nProgressBarHeight);
		m_wndP2PFileTransProgressBar.MoveWindow(ptHistory.x,ptHistory.y,szHistory.cx,nProgressBarHeight);
	}
	else
	{	
		m_reditHistory.MoveWindow(ptHistory.x,ptHistory.y,szHistory.cx,szHistory.cy);
	}

	m_wndSplitter1.MoveWindow(ptSplitter.x,ptSplitter.y,szSplitter.cx,szSplitter.cy);
	m_wndToolBar.MoveWindow(ptToolBar.x,ptToolBar.y,szToolBar.cx,szToolBar.cy);
}

void CP2PSendMsgDlg::LayoutUserWindow(CPoint& ptUserPanel, CSize& szUserPanel, CPoint& ptHistory, CSize& szHistory)
{
	if (m_bShowHistory)
	{
		m_wndPopupHistory.MoveWindow(ptUserPanel.x,ptHistory.y/*-1*/,szUserPanel.cx,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y) );
	}
	else 
	{
		switch(m_pmP2PMode)
		{
		case pmSingleTalk:
			//m_wndUserPanel.MoveWindow(ptUserPanel.x,ptHistory.y-1,szUserPanel.cx,szUserPanel.cy+1-(ptHistory.y-ptUserPanel.y) );
			m_wndAD.MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y) );
#ifdef _VER_INTERNAL_
			if(m_pInterNalFriendInfo != NULL)
				m_pInterNalFriendInfo->MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y) );
#endif
			break;
		case pmMultiTalk:
			m_wndUserList.MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy+1-(ptHistory.y-ptUserPanel.y) );
			break;
		case pmVedio:
			//m_wndVedioPanel.MoveWindow(ptUserPanel.x,ptHistory.y-1,szUserPanel.cx,szUserPanel.cy+1-(ptHistory.y-ptUserPanel.y) );
			m_wndAD.MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y) );
			m_wndVedioPanel.MoveWindow(ptHistory.x+szUserPanel.cx,ptHistory.y+254,350,450);
#ifdef _VER_INTERNAL_
			if(m_pInterNalFriendInfo != NULL)
				m_pInterNalFriendInfo->MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y));
#endif
			break;
		case pmAudio:
			{
				m_wndAD.MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y) );
				//int nHeight = szUserPanel.cy+1-(ptHistory.y-ptUserPanel.y);
				//m_wndUserPanel.MoveWindow(ptUserPanel.x,ptHistory.y-1,szUserPanel.cx, 255 );
				//m_wndAD.MoveWindow(ptUserPanel.x,ptHistory.y-1,szUserPanel.cx, 255 );
				//m_wndAudioPanel.MoveWindow(ptUserPanel.x,ptHistory.y+254,400,351);
#ifdef _VER_INTERNAL_
				if(m_pInterNalFriendInfo != NULL)
					m_pInterNalFriendInfo->MoveWindow(ptUserPanel.x+1,ptHistory.y/*-1*/,szUserPanel.cx-1,szUserPanel.cy/*+1*/-(ptHistory.y-ptUserPanel.y));
#endif
			}
			break;
		}
	}

}

void CP2PSendMsgDlg::AddMsgTip(CString strTip, COLORREF clrText)
{
	if ( m_hWnd==NULL || m_reditHistory.m_hWnd==NULL )
	{
		_tagMsg t_msg;
		sprintf(t_msg.msg.data,strTip);
		t_msg.type = 1;
		PushMsg(t_msg);
		return ;
	}

	XT_FONT xf;
	xf.fontColor = clrText;

	CHARRANGE cr;

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.GetSel(cr);

	m_reditHistory.InsertText("\n");
	m_reditHistory.SetStartIndent(230);

	CResBitmap bmpLine;
	bmpLine.LoadBitmap(IDB_HINT_FLAG2);

	if ( bmpLine.GetSafeHandle() )
		m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

	m_reditHistory.InsertText(" "+strTip);

	m_reditHistory.SetSel(cr.cpMax,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.SetSel(-1,-1);

	m_reditHistory.ScrollToBottom();
}

void CP2PSendMsgDlg::AddVedioSettingTip()
{
	if (g_mapVideoDriverIndex.size()==1 )
	{
		return ;
	}

	XT_FONT xf;
	xf.fontColor = COLOR_MSGTIP;

	CHARRANGE cr;

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.GetSel(cr);

	m_reditHistory.SetStartIndent(120);

	CResBitmap bmpLine;
	bmpLine.LoadBitmap(IDB_HINT_VEDIO);
	m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());
	m_reditHistory.InsertText(" 点击这里 ");
	m_reditHistory.InsertLink("设置摄像头");

	m_reditHistory.SetSel(cr.cpMax,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.InsertText("\n");

}

void CP2PSendMsgDlg::OnP2pMsgTimeOut()
{
	//m_FriendInfo.ip   = 0;
	//m_FriendInfo.port = 0;	


}

void CP2PSendMsgDlg::AddP2PTip()
{
	return;
	if ( m_pmP2PMode!=pmMultiTalk )
	{
		XT_FONT xf;
		xf.fontColor=RGB(0,166,84);

		m_reditHistory.SetStartIndent(120);
		m_reditHistory.InsertText("\n");

		m_reditHistory.SetStartIndent(230);

		CResBitmap bmpLine;
		bmpLine.LoadBitmap(IDB_HINT_FLAG2);
		m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

		m_reditHistory.SetSel(-1,-1);
		m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

		m_reditHistory.InsertText(" 有奖活动");
		m_reditHistory.InsertText("\n");

		m_reditHistory.SetStartIndent(80);
		m_reditHistory.InsertLine();
	}
}

void CP2PSendMsgDlg::AddCMemberTip(XT_USER_INFO *user, int action)
{
	CString strTip;

	if ( action==1 )
		strTip.Format("%s 进来了",UserInfoToStr(user) );
	else
		strTip.Format("%s 退出了",UserInfoToStr(user));

	AddMsgTip(strTip);
}

void CP2PSendMsgDlg::SaveClusterMsg(const XT_MSG &msg)
{
	if ( m_bClusterInit && m_pmP2PMode==pmMultiTalk )
	{
		XT_MSG msg_temp = msg;
		msg_temp.msg_type = 0;//p2p聊天消息
		size_t i,c;
		c = m_clusterInfo.Count();
		for ( i = 0; i< c; i++ )
		{
			//if(	msg_temp.from_id == m_clusterInfo.members[i].id ||
			//	msg_temp.from_id == g_ImUserInfo.GetId())
			//{
			g_LocalData.SaveTalkMsg(msg_temp,m_clusterInfo.members[i].GetId());
			//}
		}
	}
}

void CP2PSendMsgDlg::ProcessMsg(const XT_MSG &msg)
{
#ifdef _IM_UNIT_TEST
	IMTEST->RecvP2pMsg(m_FriendInfo, msg);
	return;
#endif
	switch (msg.data_type)
	{
	case XT_MSG::IM_MSG:		// 即时聊天消息
	case XT_MSG::EVALUA_ACK:	// 服务评价表应答
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
	case XT_MSG::SF_FILE_REQ:
		// 收到发送方发过来的图片文件请求
		SF_RecvPfileReq(msg);
		break;
	case XT_MSG::SF_FILE_ACK:
		// 收到接收方应答，并开始发送数据(仅一次)
		SF_RecvPfileAck(msg);
		break;
	case XT_MSG::SF_PACKET_REQ:
		// 接收发送方发送的数据
		SF_RecvPfilePacket(msg);
		break;
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

// 				CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(&m_FriendInfo);
// 
// 				if ( pDlg->m_hWnd==NULL )
// 					pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
// 
// 				if ( pDlg->IsWindowVisible()==FALSE )
// 					pDlg->ShowWindow(SW_SHOW);
// 
// 				if (pDlg->IsIconic())
// 					pDlg->ShowWindow(SW_RESTORE);
// 
// 				pDlg->BringWindowToTop();
				//开始抖
				SetTimer(TIMER_WIN_SHAKE,20,NULL);
				m_nWinShakeCnt = 0;
			}
		}
		break;

		//case 6://recv cluster image file packet
		//	OnRecvClusterImageFilePacket(msg);
		//	break;
		//case 7:
		//	OnClusterImageFilePacketAck(msg);
		//	break;
	case XT_MSG::SF_CFILE_REQ:
		// 通知本用户开始下载群图片
		SF_RecvStartDownloadCfileNotify(msg);
		//OnRecvClusterImageFileReq(msg);
		break;
	case XT_MSG::MM_CANCEL:
		MM_OnRecvCancel(msg);
		break;
	case XT_MSG::MM_REPLY:
		MM_OnReply(msg);
		break;
	case XT_MSG::MM_STOP:
		MM_OnStop(msg);
		break;
	case XT_MSG::PUSH_EVALUA:
		{
			AddTalkMsg(msg, false);

// 			CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(&m_FriendInfo);
// 
// 			if (pDlg->m_hWnd == NULL)
// 				pDlg->Create(CP2PSendMsgDlg::IDD, GetDesktopWindow());
// 
// 			if (pDlg->IsWindowVisible() == FALSE)
// 				pDlg->ShowWindow(SW_SHOW);
// 
// 			if (pDlg->IsIconic())
// 				pDlg->ShowWindow(SW_RESTORE);
// 
// 			pDlg->BringWindowToTop();

			// 弹出评价窗口
// 			CEvalueDlg dlg(pDlg);
// 			if (IDOK == dlg.DoModal())
// 			{
// 				CString strEvalueGrade = dlg.GetEvalueGrade();
// 				CString strSuggest = dlg.GetSuggest();
// 				int nGrade= (int)dlg.GetEvalueGradeEn();
// 
// 				XT_MSG sendmsg;
// 				SF_InitXtMsg(sendmsg);
// 				sendmsg.ver = nGrade;
// 				sendmsg.data_type = XT_MSG::EVALUA_ACK;
// 				CSendPacket sp(sendmsg.data);
// 
// 				CString text = "客户对您的评价为：";
// 				text += strEvalueGrade;
// 				if (!strSuggest.IsEmpty())
// 				{
// 					text += "\n客户对您的建议为：" + strSuggest;
// 				}
// 				strcpy(sendmsg.data, text);
// 				sendmsg.data_len = strlen(text) + 1;
// 
// 				g_pProtocol->SendP2PMsg(sendmsg);
// 
// 				text = "谢谢您的评价";
// 				strcpy(sendmsg.data, text);
// 				sendmsg.data_len = strlen(text) + 1;
// 				AddTalkMsg(sendmsg, false);
// 			}
		}
		break;
	default:
		break;
	}	
}

void CP2PSendMsgDlg::SF_OnAbortFile(const char *file_name)
{
	if ( m_hWnd && m_reditHistory.m_hWnd )
		m_reditHistory.OnAbortImageFile(file_name);
}

void CP2PSendMsgDlg::SF_OnRecvFileComplete(const char *file_name)
{
	if ( m_hWnd && m_reditHistory.m_hWnd )
		m_reditHistory.OnRecvImageFile(file_name);
}

void CP2PSendMsgDlg::MM_InitXtMsg(XT_MSG &msg)
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

void CP2PSendMsgDlg::MM_CancelRequest(int mmType, uint32 mmID)
{
	XT_MSG msg;
	MM_InitXtMsg(msg);

	msg.data_type = XT_MSG::MM_CANCEL;

	CSendPacket sp(msg.data);

	strcpy(msg.from_nickname,m_FriendInfo.nickname);

	sp<<mmID<<(uint8)mmType;

	msg.data_len = sp.GetSize();

	g_pProtocol->SendP2PMsg(msg);
}

void CP2PSendMsgDlg::MM_Reply(int mmType,uint32 mmID,bool bAccept)
{
	if ( bAccept )
	{
		m_mmWaitInfo.mmID    = mmID;
		m_mmWaitInfo.mmType  = mmType;
		m_mmWaitInfo.nAccept = 1;
	}
	else
	{
		XT_MSG msg;
		MM_InitXtMsg(msg);

		msg.data_type = XT_MSG::MM_REPLY;

		CSendPacket sp(msg.data);

		uint8 nAccept;
		if  (bAccept)
			nAccept=1;
		else
			nAccept=0;

		sp<<(uint32)mmID<<(uint8)mmType<<(uint8)nAccept;

		msg.data_len = sp.GetSize();

		g_pProtocol->SendP2PMsg(msg);
	}

	if ( mmType==0 )
	{
		if ( bAccept )
			m_reditHistory.SetMMBarTip(mmID,ODT_RECV_VEDIO,"您同意了对方的视频聊天请求",RGB(255,0,0));
		else
			m_reditHistory.SetMMBarTip(mmID,ODT_RECV_VEDIO,"您拒绝了对方的视频聊天请求",RGB(255,0,0));
	}
	else
	{
		if ( bAccept )
			m_reditHistory.SetMMBarTip(mmID,ODT_RECV_AUDIO,"您同意了对方的语音聊天请求",RGB(255,0,0));
		else
			m_reditHistory.SetMMBarTip(mmID,ODT_RECV_AUDIO,"您拒绝了对方的语音聊天请求",RGB(255,0,0));
	}

	for ( int i=0; i<m_vecRecvMMInfo.size(); i++ )
	{
		if ( m_vecRecvMMInfo[i].nMMID!=mmID )
		{
			if ( m_vecRecvMMInfo[i].nMMType==0 )
				m_reditHistory.SetMMBarTip(m_vecRecvMMInfo[i].nMMID,ODT_RECV_VEDIO,"请求已过期",RGB(222,222,0));
			else
				m_reditHistory.SetMMBarTip(m_vecRecvMMInfo[i].nMMID,ODT_RECV_AUDIO,"请求已过期",RGB(222,222,0));
		}
	}

	m_vecRecvMMInfo.clear();

	for ( int i=0; i<m_vecSendMMInfo.size(); i++ )
	{
		if ( m_vecSendMMInfo[i].nMMType==0 )
			m_reditHistory.SetMMBarTip(m_vecSendMMInfo[i].nMMID,ODT_SEND_VEDIO,"请求已过期",RGB(222,222,0));
		else
			m_reditHistory.SetMMBarTip(m_vecSendMMInfo[i].nMMID,ODT_SEND_AUDIO,"请求已过期",RGB(222,222,0));
	}

	m_vecSendMMInfo.clear();
}

void CP2PSendMsgDlg::MM_Stop()
{
	XT_MSG msg;
	MM_InitXtMsg(msg);

	msg.data_type = XT_MSG::MM_STOP;

	CSendPacket sp(msg.data);

	strcpy(msg.from_nickname,m_FriendInfo.nickname);

	sp<<(uint8)0;

	msg.data_len = sp.GetSize();

	g_pProtocol->SendP2PMsg(msg);

	if ( m_pmP2PMode==pmVedio)
		AddMsgTip("您终止了视频聊天！",COLOR_MSGTIP);
	else
		AddMsgTip("您终止了语音聊天！",COLOR_MSGTIP);

	m_reditHistory.ScrollToBottom();

}

void CP2PSendMsgDlg::MM_SendRequest(int mmType)
{
	XT_MSG msg;
	MM_InitXtMsg(msg);

	msg.data_type = XT_MSG::MM_REQ;

	CSendPacket sp(msg.data);

	strcpy(msg.from_nickname,m_FriendInfo.nickname);

	DWORD dwMMID = this->m_nSendFileSequence++;

	sp<<(uint32)dwMMID<<(uint8)mmType<<(uint32)g_pProtocol->GetTranServerIP()<<(uint16)g_pProtocol->GetTranServerPort();

	msg.data_len = sp.GetSize();

	g_pProtocol->SendP2PMsg(msg);

	XT_FONT		xf;

	CString strTitle,strDate, strTime;
	CTime  tm=CTime::GetCurrentTime();

	strTime=tm.Format("%H:%M:%S");
	strDate=tm.Format("%Y.%m.%d");
	if(mmType)

	strTitle.Format("%s %s %s \n\n 发送%s通话邀请\n",::UserInfoToStr(&g_ImUserInfo),strDate,strTime,mmType==0 ? "视频" : "语音");	

	xf.fontColor=RGB(74,123,211);

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.InsertText("\n");
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	m_reditHistory.SetStartIndent(80);
	m_reditHistory.InsertText(strTitle);

//不在聊天框中提示
/*
	if ( mmType==0 )
		m_reditHistory.InsertMMInfoBar(dwMMID,true,"vedio",m_FriendInfo.nickname);
	else
		m_reditHistory.InsertMMInfoBar(dwMMID,true,"audio",m_FriendInfo.nickname);

	static bool bVedioSetting=false;

	if ( bVedioSetting==false && mmType==0 )
	{
		AddVedioSettingTip();
		bVedioSetting = true;
	}
*/

	tagMMInfo mi;
	mi.nMMID   = dwMMID;
	mi.nMMType = mmType;
	this->m_vecSendMMInfo.push_back(mi);

	m_reditHistory.ScrollToBottom();
}

void CP2PSendMsgDlg::MM_OnRecvCancel(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint8  mmType;
	uint32 mmID;
	rp>>mmID>>mmType;

	if ( mmType==0 )//视频
	{
		m_wndVedioPanel.StopVedio();
		m_reditHistory.SetMMBarTip(mmID,ODT_RECV_VEDIO,"取消了视频聊天",RGB(255,0,0));
		if(m_wndVedioPanel && m_wndVedioPanel.IsWindowEnabled())
		{
			m_wndVedioPanel.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_wndAudioPanel.StopAudio();
		m_reditHistory.SetMMBarTip(mmID,ODT_RECV_AUDIO,"取消了语音聊天",RGB(255,0,0));
		if(m_wndAudioPanel && m_wndAudioPanel.IsWindowVisible())
		{
			m_wndAudioPanel.ShowWindow(SW_HIDE);
		}
	}
}

void CP2PSendMsgDlg::MM_OnStop(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint8  mmType;

	rp>>mmType;

	if ( m_pmP2PMode==pmVedio )
	{
		m_wndVedioPanel.StopVedio();

		m_pmP2PMode = pmSingleTalk;
		//UpdateP2PPanel();

		AddMsgTip("对方终止了视频聊天!",COLOR_MSGTIP);
		if(m_wndVedioPanel && m_wndVedioPanel.IsWindowEnabled())
		{
			m_wndVedioPanel.ShowWindow(SW_HIDE);
		}
	}
	else
		if ( m_pmP2PMode==pmAudio )
		{
			m_wndAudioPanel.StopAudio();

			m_pmP2PMode = pmSingleTalk;
			//UpdateP2PPanel();

			AddMsgTip("对方终止了语音聊天!",COLOR_MSGTIP);
			if(m_wndAudioPanel && m_wndAudioPanel.IsWindowVisible())
			{
				m_wndAudioPanel.ShowWindow(SW_HIDE);
			}
		}

}

void CP2PSendMsgDlg::RefreshFriendStatus()
{
	XT_STATUS_GET_REQ req;
	req.fid = m_FriendInfo.GetId();
	req.id = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CP2PSendMsgDlg::MM_OnReply(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint8  mmType;
	uint32 mmID;
	uint8  nAccept;

	rp>>mmID>>mmType>>nAccept;

	if ( mmType==0 )
	{
		if ( nAccept )
		{
			m_reditHistory.SetMMBarTip(mmID,ODT_SEND_VEDIO,"对方同意了您的视频聊天请求",RGB(255,0,0));
			//在这里启动视频聊天连接
			m_pmP2PMode = pmVedio;
			//UpdateP2PPanel();

			//if(startVideo)
			//startVideo(g_ImUserInfo.GetId(),m_FriendInfo.id);

			//StartVideo2(g_ImUserInfo.GetId(),m_FriendInfo.id);

			m_bMMStartTalk = true;

			m_wndVedioPanel.SetStatusText("正在与对方进行网络连接");
			m_wndVedioPanel.VideoStartTalk();


		}
		else
		{
			m_reditHistory.SetMMBarTip(mmID,ODT_SEND_VEDIO,"对方拒绝了您的视频聊天请求",RGB(255,0,0));
		}
	}
	else
	{
		if ( nAccept )
		{
			m_reditHistory.SetMMBarTip(mmID,ODT_SEND_AUDIO,"对方同意了您的语音聊天请求",RGB(255,0,0));
			//在这里启动语音聊天
			m_pmP2PMode = pmAudio;
			//UpdateP2PPanel();

			m_bMMStartTalk = true;

			m_wndAudioPanel.SetStatusText("正在与对方进行网络连接");
			m_wndAudioPanel.AudioStartTalk();
		}
		else
		{
			m_reditHistory.SetMMBarTip(mmID,ODT_SEND_AUDIO,"对方拒绝了您的语音聊天请求",RGB(255,0,0));
		}
	}

	if ( nAccept )
	{
		for ( int i=0; i<m_vecSendMMInfo.size(); i++ )
		{
			if ( m_vecSendMMInfo[i].nMMID!=mmID )
			{
				if ( m_vecSendMMInfo[i].nMMType==0 )
					m_reditHistory.SetMMBarTip(m_vecSendMMInfo[i].nMMID,ODT_SEND_VEDIO,"请求已过期",RGB(222,222,0));
				else
					m_reditHistory.SetMMBarTip(m_vecSendMMInfo[i].nMMID,ODT_SEND_AUDIO,"请求已过期",RGB(222,222,0));
			}
		}

		for ( int i=0; i<m_vecRecvMMInfo.size(); i++ )
		{
			if ( m_vecRecvMMInfo[i].nMMType==0 )
				m_reditHistory.SetMMBarTip(m_vecRecvMMInfo[i].nMMID,ODT_RECV_VEDIO,"请求已过期",RGB(222,222,0));
			else
				m_reditHistory.SetMMBarTip(m_vecRecvMMInfo[i].nMMID,ODT_RECV_AUDIO,"请求已过期",RGB(222,222,0));
		}

		m_vecSendMMInfo.clear();
		m_vecRecvMMInfo.clear();
	}
}


void CP2PSendMsgDlg::MM_OnRecvRequest(const XT_MSG &msg)
{
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));

	uint8  mmType;
	uint32 mmID;

	rp>>mmID>>mmType>>m_nPeerTransServerIp>> m_nPeerTransServerPort;

	m_nPeerTransServerPort = m_nPeerTransServerPort;

	XT_FONT		xf;

	CString strTitle,strDate, strTime;
	CTime  tm=CTime::GetCurrentTime();

	strTime=tm.Format("%H:%M:%S");
	strDate=tm.Format("%Y.%m.%d");

	//好友发给我的信息
	
	strTitle.Format("%s %s %s \n\n 发送%s通话邀请\n",::UserInfoToStr(&m_FriendInfo),strDate,strTime,mmType==0 ? "视频" : "语音");	

	xf.fontColor=RGB(96,92,168);;

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.InsertText("\n");
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	m_reditHistory.SetStartIndent(80);
	m_reditHistory.InsertText(strTitle);


//聊天框中不弹提示
/* 
	if ( mmType==0 )
		m_reditHistory.InsertMMInfoBar(mmID,false,"vedio",m_FriendInfo.nickname);
	else
		m_reditHistory.InsertMMInfoBar(mmID,false,"audio",m_FriendInfo.nickname);

	if ( mmType==0 )
	{
		static bool b=false;
		if ( b==false )
		{
			AddVedioSettingTip();
			b=true;
		}
	}
*/
	tagMMInfo mi;
	mi.nMMID   = mmID;
	mi.nMMType = mmType;

	m_vecRecvMMInfo.push_back(mi);

	RefreshFriendStatus();

	m_reditHistory.ScrollToBottom();

	CRect rc;
	GetWindowRect(rc);
	//打开音频窗口
	if(mmType == 1)
	{
		m_wndAudioPanel.ShowWindow(SW_SHOW);
		m_wndAudioPanel.MoveWindow(rc.right,rc.top,CP2PAudioPanel::m_WinW,CP2PAudioPanel::m_WinH);
		m_wndAudioPanel.SetWindowText("与" + UserInfoToStr(&m_FriendInfo) + "语音通话");
		m_wndAudioPanel.AudioRecvRequest();
	}
	//打开视频窗口
	else if(mmType == 0)
	{
		m_wndVedioPanel.ShowWindow(SW_SHOW);
		m_wndVedioPanel.MoveWindow(rc.right,rc.top,CP2PVedioPanel::m_WinW,CP2PVedioPanel::m_WinH);
		m_wndVedioPanel.SetWindowText("与" + UserInfoToStr(&m_FriendInfo) + "视频通话");
		m_wndVedioPanel.VideoRecvRequest();
	}


	
}

void CP2PSendMsgDlg::SF_InitXtMsg(XT_MSG &msg)
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

void CP2PSendMsgDlg::PushMsg(const _tagMsg & msg)
{
	m_lstMsgRecv.push_back(msg);
	if ( m_lstMsgRecv.size()>50 )
		m_lstMsgRecv.pop_front();
}

void CP2PSendMsgDlg::PopMsg()
{
	if ( m_reditHistory.m_hWnd && m_hWnd )
	{
		list<_tagMsg>::iterator it;

		//	m_lstMsgRecv.sort();

		for ( it=m_lstMsgRecv.begin(); it!=m_lstMsgRecv.end(); it++ )
		{
			//AddTalkMsg( *it, false );
			switch ( it->type )
			{
			case 0:
				AddTalkMsg(it->msg,false);
				g_LocalData.SaveTalkMsg(it->msg);
				break;
			case 1:
				AddMsgTip(it->msg.data);
				break;
			case 2:
				RecvMagicFace(it->msg.from_id,it->msg.fontName,it->msg.data);
				break;	
			}
		}

		m_lstMsgRecv.clear();
	}
}

void CP2PSendMsgDlg::AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg)
{
	if ( m_hWnd==NULL || m_reditHistory.m_hWnd==NULL )
	{
		_tagMsg t_msg;
		t_msg.msg  = msg;
		t_msg.type = 0;
		PushMsg(t_msg);

		return ;
	}

	m_reditHistory.SetSel(-1,-1);
	CHARRANGE cr;

	XT_FONT xf;

	CString strTitle,strDate, strTime;
	CTime  tm(msg.send_time);

	strTime=tm.Format("%H:%M:%S");
	strDate=tm.Format("%Y.%m.%d");

	//好友发给我的信息
	if ( msg.from_id!=g_ImUserInfo.GetId() )
	{
		strTitle.Format("%s ",msg.from_nickname);
	}
	else
	{
		strTitle.Format("%s ",g_ImUserInfo.nickname);	
	}

	if ( isHistoryMsg )
	{
		xf.fontColor=RGB(122,122,122);
	}
	else
	{
		if ( g_ImUserInfo.GetId()==msg.from_id )
			xf.fontColor=RGB(0,128,64);
		else
			xf.fontColor=RGB(22,68,223);
	}

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.GetSel(cr);
	m_reditHistory.InsertText("\n");

	m_vecRecords.push_back(cr.cpMin);

	m_reditHistory.SetStartIndent(80);
	m_reditHistory.InsertText(strTitle);
	m_reditHistory.SetSel(cr.cpMin,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.GetSel(cr);
	strTitle.Format("(%s %s):", strDate,strTime);
	m_reditHistory.InsertText(strTitle);
	m_reditHistory.InsertText("\n");
	m_reditHistory.SetSel(cr.cpMin,-1);
	XT_FONT xfG;
	xfG.fontColor = 0x808080;
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfG));

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.GetSel(cr);

	XT_FONT   xfMsg;
	xfMsg.flags     = msg.fontStyle;

	if ( isHistoryMsg )
		xfMsg.fontColor=RGB(122,122,122);
	else
		xfMsg.fontColor=msg.fontColor;

	sprintf(xfMsg.fontName,msg.fontName);
	xfMsg.fontSize  = msg.fontSize;

	CStringList sl;

	m_reditHistory.SetStartIndent(300);
	m_reditHistory.InsertText(msg.data,!isHistoryMsg,&sl);

	// 离线消息图片不显示等待下载动画
	if(msg.msg_type != 10 && msg.msg_type != 11)
	{
		for ( int i=0; i<sl.GetCount(); i++ )
		{
			tagWaitImageFileStruct wi;
			wi.time_recv = GetTickCount();
			sprintf(wi.szFile,sl.GetAt(sl.FindIndex(i)));
			m_vecWaitImageFiles.push_back(wi);
		}
	}


	m_reditHistory.SetSel(cr.cpMax,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xfMsg));
	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

	if ( m_vecRecords.size()>50 )
	{
		int cp = m_vecRecords[0];

		for ( int i=1; i<m_vecRecords.size(); i++ )
			m_vecRecords[i]-=cp;

		m_reditHistory.SetSel(0,cp);
		m_reditHistory.ReplaceSel("");
		m_reditHistory.SetSel(-1,-1);

		m_vecRecords.erase(m_vecRecords.begin());
	}

	m_reditHistory.ScrollToBottom();
}

void CP2PSendMsgDlg::OnBnClickedBtnSend()
{
	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		AddMsgTip("对不起，您当前为离线状态，无法发送信息！");
		return;
	}

	XT_MSG msg;
	SF_InitXtMsg(msg);

	msg.data_type   = 0;//即时聊天消息

	CString text;
	IMAGE_FILE_LIST il;
	IMAGE_FILE_LIST::iterator it;

	m_reditSend.GetText(text,&il);
	m_reditSend.ClearEmotionPicData();

	if ( text.IsEmpty() )
	{
		CMessageBox::Prompt("消息不能为空");
		return ;
	}

	if ( text.GetLength()>1300 )
	{
		//MessageBox("最多输入900个英文字符或450个汉字!");
		//CMessageBox::Prompt("无法输入太多汉字!");
		AddMsgTip("你发送消息内容超长，请分条发送。");
		return ;
	}

	if ( FindForbiddenWords(text) )
	{
		m_reditSend.ClearMsg();
		m_reditSend.SetFocus();
		AddMsgTip("你发送的消息中有限制用语，消息发送失败！");
		return ;
	}

	if ( text.GetLength()==0 )
	{
		m_reditSend.SetFocus();
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
		g_pProtocol->SendP2PMsg(msg,type);

		/*CRecentListCtrl *pRecentList=(CRecentListCtrl *)g_pRecentListCtrl;
		pRecentList->AddUser(&m_FriendInfo);*/

		SF_SetTransMode(type);

		if ( il.size()>0 )
		{
			//if ( m_FriendInfo.status!=XTREAM_OFFLINE )
			{
				int nPackets = 5/il.size() + 1;

				for ( it=il.begin(); it!=il.end(); it++ )
				{
					//SF_SendPfileReq( *it, nPackets );	
				}
			}
			//else
			//{
				//AddMsgTip("对方不在线或隐身!可能无法收到您的发送的图片!");
			//}
		}

		strcpy(msgShow.from_nickname,g_ImUserInfo.nickname);
		g_LocalData.SaveTalkMsg(msgShow,m_FriendInfo.GetId());

		if(g_pDuiCounsel != NULL)
		{
			FRIEND_INFO * pInfo = ::GetFriendInfo(m_FriendInfo.GetId());
			g_pDuiCounsel->SetFriendListElement(pInfo, true, false, false, false, false);
		}
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
	RegUserOperation(0x00c4);

	m_reditSend.ClearMsg();
	m_reditSend.SetFocus();
}

void CP2PSendMsgDlg::OnBnClickedBtnSendEx()
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


void CP2PSendMsgDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	if ( m_bDlgInit==false )
	{
		if ( m_wndHide.m_hWnd )
		{
			m_bDlgInit = true;
			AdjustUI();
			return ;
		}
	}

	if ( m_bDlgInit && nType!=SIZE_MINIMIZED )
	{
		AdjustUI();

		if ( m_bUpdateP2PPanel )
		{ 			
			m_bUpdateP2PPanel = false;
			UpdateP2PPanel();
		}
	}
	if( m_wndAudioPanel && m_wndAudioPanel.IsWindowVisible())
	{
		CRect rc;
		GetWindowRect(&rc);
		CRect rcAudioPanel;
		m_wndAudioPanel.GetWindowRect(&rcAudioPanel);
		m_wndAudioPanel.MoveWindow(rc.right+1, rc.top, rcAudioPanel.Width(), rcAudioPanel.Height());
	}
	if( m_wndVedioPanel && m_wndVedioPanel.IsWindowVisible())
	{
		CRect rc;
		GetWindowRect(&rc);
		CRect rcVedioPanel;
		m_wndVedioPanel.GetWindowRect(&rcVedioPanel);
		m_wndVedioPanel.MoveWindow(rc.right+1, rc.top, rcVedioPanel.Width(), rcVedioPanel.Height());
	}

// 	if(g_pDuiCounsel != NULL && m_FriendInfo.GetId() >= 5000000)
// 	{
// 		if(g_pDuiCounsel->m_pDlgSelectP2PSendMsgDlg == this || g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1)
// 		{
// 			CRect rc;
// 			GetWindowRect(&rc);
// 			::SetWindowPos(*g_pDuiCounsel, *this, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
// 			::MoveWindow(*g_pDuiCounsel, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
// 			g_pDuiCounsel->MoveAllWindow(this);
// 		}
// 	}
}

void CP2PSendMsgDlg::OnHelpClick()
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

BOOL CP2PSendMsgDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( wParam==IDC_SPLITTER1 )
	{
		SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;

		m_nSplitterPos-=pHdr->delta;

		AdjustUI();

		m_reditHistory.ScrollToBottom();
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
				m_reditHistory.SetFileBarTip(id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));
			}
			else
			{

				CP2PRecvFile *pFile = AF_FindRecvFile(id);

				// 现在暂时不实现断点续传功能
				/*if ( pFile )
					pFile->SaveFilePosition(m_FriendInfo.GetId());*/

				AF_AbortRecvFile(id);
				m_reditHistory.SetFileBarTip(id,ODT_RECV_FILE,"您取消了接收该文件",RGB(255,0,0));
			}

			m_wndP2PFileTransProgressBar.DeleteItem(nItem);
			if ( m_wndP2PFileTransProgressBar.GetItemCount()==0 )
				ShowFileTransInfoBar(false);

			return FALSE;
		}
	}

	return CXSkinDlg::OnNotify(wParam, lParam, pResult);
}

void CP2PSendMsgDlg::OnBnClickedBtnHistory()
{
	//CMsgDlg * pDlg = CMsgDlg::GetInstance();
	//pDlg->BringWindowToTop();
	//pDlg->SetDefaultFriend(m_FriendInfo.GetId());
}

LRESULT CP2PSendMsgDlg::OnMagicFaceClick(WPARAM wParam, LPARAM lParam)
{
	ShowMagicFace((char *)lParam);
	return 0;
}

LRESULT CP2PSendMsgDlg::OnQueryPeerOnline(WPARAM wParam, LPARAM lParam)
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

LRESULT CP2PSendMsgDlg::OnMagicFaceNotify(WPARAM wParam, LPARAM lParam)
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

	//if (this->m_pmP2PMode!=pmMultiTalk )
	//{
	//	((CRecentListCtrl *)g_pRecentListCtrl)->AddUser(&m_FriendInfo);
	//}

	if ( m_mapMagicFace.find(CString(face_name))==m_mapMagicFace.end() )
		m_mapMagicFace.insert(map<CString,CString>::value_type(face_name,face_url));

	ShowMagicFace(face_name);

	return 0;
}

LRESULT CP2PSendMsgDlg::OnEmotionSelNotify(WPARAM wParam,LPARAM lParam)
{
	CUSTOM_EMOTION * pFace = (CUSTOM_EMOTION * )wParam;
	m_reditSend.InsertFace(pFace,(int)lParam);

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

	m_reditSend.SetFocus();
	return 1;
}

void CP2PSendMsgDlg::OnClose()
{
//	BOOL bIsHint = FALSE;
// 	if(g_pDuiCounsel != NULL && g_pDuiCounsel->m_vecP2PSendMsgDlg.size() >= 2)
// 	{
// 		for(int nDlg = 0; nDlg < g_pDuiCounsel->m_vecP2PSendMsgDlg.size(); nDlg++)
// 		{
// 			if(g_pDuiCounsel->m_vecP2PSendMsgDlg[nDlg] == this)
// 			{
// 				bIsHint = TRUE;
// 				break;
// 			}
// 		}
// 	}
// 	if(/*g_pMessageHandleWnd->GetP2PSendMsgDlgSize() > 1*/bIsHint)
// 	{
// 		std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> pWnd = CP2PSendMsgDlgCloseHint::NewDlg(GetSafeHwnd());
// 		pWnd->ShowModal();
// 	}
// 	else
//		SendMessage(WM_CLOSE_ONE);

// #ifdef _BUSINESS_VESION
// 	//更新右侧备注
// 	if(m_strNewComnCustRemarkClient != m_strNewComnCustRemarkServer)
// 	{
// 		XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ req;
// 		req.uiBusinessID = g_ImUserInfo.GetId();
// 		strcpy(req.szCommAccount, m_FriendInfo.GetUserName().c_str());
// 
// 		// 时间格式化
// 		uint32 uiSendTime = time(NULL);
// 		CTime  tm(uiSendTime);
// 		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
// 		strcpy(req.szCommTime, strTime);
// 
// 		strncpy(req.szRemarks, m_strNewComnCustRemarkClient, 200);
// 		g_pProtocol->SendReq(req);
// 	}
// #endif
 
	DestroyWindow();
}

int CP2PSendMsgDlg::DoModal()
{
	return CXSkinDlg::DoModal();
}

void CP2PSendMsgDlg::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);

	m_wndUserList.DestroyWindow();
	m_wndUserPanel.DestroyWindow();
	m_wndAD.DestroyWindow();
	m_wndVedioPanel.DestroyWindow();
	m_wndAudioPanel.DestroyWindow();
#ifdef _VER_INTERNAL_
	if(m_pInterNalFriendInfo != NULL)
		m_pInterNalFriendInfo->DestroyWindow();
#endif

	if ( m_pmP2PMode==pmMultiTalk )
	{
		CXSkinDlg::OnDestroy();

		//关闭组内群发不需要退出临时群
// 		XT_CLUSTER_APPLY_REQ req;
// 		req.nAction = 1;
// 		req.nFamilyID  = m_clusterInfo.nFamilyID;
// 		req.nFamilyProp = TEMP_FAMILY;
// 		req.nUserID = g_ImUserInfo.GetId();
// 		g_pProtocol->SendReq(req);	
// 
 		::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2PSENDMSGDLG_NOTIFY,(WPARAM)m_pmP2PMode,(LPARAM)m_clusterInfo.nFamilyID);
	}
	else
	{
		FRIEND_INFO *pRealFriend = FindFriend(m_FriendInfo.GetId());

		if ( pRealFriend )
		{
			pRealFriend->bClosePanel = m_bClosePanel;
			pRealFriend->isTalkZYF   = m_reditSend.IsEnableZYF() ? 1:0;
		}

		CXSkinDlg::OnDestroy();

		::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2PSENDMSGDLG_NOTIFY,(WPARAM)m_pmP2PMode,(LPARAM)&m_FriendInfo);
	}

}

void CP2PSendMsgDlg::DrawCaptionBar()
{
	CDC *pDC = GetWindowDC();

	if ( pDC==NULL )
		return ;

	//覆盖原来的按钮
	CRect rcWindow;
	GetClientRect(rcWindow);
	rcWindow.top = 1; //边框
	rcWindow.left = rcWindow.right - SYS_BTN_SIZE*3;		
	rcWindow.bottom = rcWindow.top + SYS_BTN_SIZE;
	pDC->FillSolidRect(rcWindow, m_colorBackground);

	// 	CRect rcCapt;
	// 	GetClientRect(rcCapt);
	// 	rcCapt.bottom = 82;
	// 	pDC->Rectangle(&rcCapt);
	DrawStyle(pDC);

	ReleaseDC(pDC);
}

void CP2PSendMsgDlg::ShowDlgTitle()
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

	SetWindowText(/*str*/"临时会话");
	SetDlgItemText(IDC_STATIC_USERNAME, "临时会话");
	//in_addr addr;
	//addr.S_un.S_addr = ntohl(m_FriendInfo.ip);
}
//if ( m_FriendInfo.ip )
//{
void CP2PSendMsgDlg::OnRecvVedioData(BYTE *pBmpData, DWORD dwUser)
{
	CP2PSendMsgDlg *pDlg = (CP2PSendMsgDlg *)dwUser;
	pDlg->m_wndVedioPanel.DrawVedioFrame(pBmpData);
}

//显示历史5条记录
void CP2PSendMsgDlg::LoadLastTalkMsg()
{
	MSG_LIST ml1;
	// 获取用户聊天记录
	g_LocalData.GetLastMsgList(m_FriendInfo.GetId(),CDataManager::USER,ml1,5);

	POSITION pos=ml1.GetTailPosition();

	MSG_LIST ml2;

	int count=0;

	while ( count++<5 )
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

	for ( int i=0; i<ml2.GetCount(); i++)
	{
		pos = ml2.FindIndex(i);
		AddTalkMsg(ml2.GetAt(pos),true);
	}

	if ( ml2.GetCount()>0 )
	{
		m_reditHistory.InsertText("\n");
		m_reditHistory.SetStartIndent(80);
		m_reditHistory.InsertLine();
		m_reditHistory.InsertText("\n");
	}
}
//好友状态更改通知
void CP2PSendMsgDlg::OnFriendStatusChange(const char *data,WORD wRet)
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
				if ( FileExist(strFaceFile) )
				{
					m_ctrlFaceIcon.SetPicPath(strFaceFile, m_FriendInfo.status == XTREAM_ONLINE 
						|| m_FriendInfo.status == XTREAM_BUSY
						|| m_FriendInfo.status == XTREAM_LEAVE);
				}

				//更新状态和分组
				SetDlgItemText(IDC_STATIC_STATUS_GROUP, "[" + StatusToStr(m_FriendInfo.status) + "]" + "（" + m_strGroupName +"）") ;
				SetDlgItemText(IDC_STATIC_STATUS_GROUP, "") ;

				ShowDlgTitle();
				m_wndUserPanel.UpdateFriendInfo();
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

void CP2PSendMsgDlg::ChangeHeadStatus(FRIEND_INFO *pFriend)
{
	//更新头像
	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&m_FriendInfo);
	if ( FileExist(strFaceFile) )
	{
		m_ctrlFaceIcon.SetPicPath(strFaceFile, pFriend->status == XTREAM_ONLINE 
			|| pFriend->status == XTREAM_BUSY
			|| pFriend->status == XTREAM_LEAVE);
	}

	SetDlgItemText(IDC_STATIC_STATUS_GROUP, "[" + StatusToStr(pFriend->status) + "]" + "（" + m_strGroupName +"）") ;
	SetDlgItemText(IDC_STATIC_STATUS_GROUP, "") ;
}


void CP2PSendMsgDlg::DrawControlLine()
{
	if ( m_reditHistory.m_hWnd==NULL )
	{
		return ;
	}

	CDC *pDC = GetDC();

	if ( pDC==NULL )
	{
		return ;
	}

	int nSavedDC = pDC->SaveDC();

	CRect rcHistory, rcSend;

	//m_captionBar.GetWindowRect(&rcCaption);
	//ScreenToClient(&rcCaption);
	m_reditHistory.GetWindowRect(&rcHistory);
	ScreenToClient(&rcHistory);

	m_reditSend.GetWindowRect(&rcSend);
	ScreenToClient(&rcSend);

	//rcCaption.InflateRect(1,1);
	rcHistory.InflateRect(1,1);
	rcSend.InflateRect(1,1);
	rcSend.top-=25;

	//pDC->Draw3dRect(&rcHistory,0xCF4705,0xCF4705);
	COLORREF clrLine = 0xCF4705;

	CPen pen(PS_SOLID, 1, clrLine);

	pDC->SelectObject(&pen);

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	//int y_point = rcCaption.top+rcCaption.Height()/2;

	//	pDC->MoveTo(rcCaption.left,y_point);
	//	pDC->LineTo(rcHistory.left,rcHistory.bottom-1);
	//	pDC->LineTo(rcHistory.right-1,rcHistory.bottom-1);
	//	pDC->LineTo(rcHistory.right-1,y_point);

	pDC->Draw3dRect(&rcSend,clrLine,clrLine);
	pDC->Draw3dRect(&rcHistory,clrLine,clrLine);

	// CRgn rgn;
	//rgn.CreateRectRgn(rcCaption.left,rcCaption.top,
	//	rcCaption.right+1, y_point+1 );

	//pDC->SelectObject(&rgn);
	//CPoint pt(16,16);

	//pDC->RoundRect(&rcCaption,pt);

	ReleaseDC(pDC);
}

/*
void CP2PSendMsgDlg::CreateFontNameBox()
{
int nIndex=0;
while (m_wndFontTool.GetItemID(nIndex)!=ID_FONTNAME)
{
nIndex++;
}

CRect rcFont;

m_wndFontTool.SetButtonInfo(nIndex,ID_FONTNAME,TBBS_SEPARATOR,110);
m_wndFontTool.GetItemRect(nIndex,&rcFont);
rcFont.bottom+=200;
rcFont.top+=2;
rcFont.InflateRect(-2,0);

m_comboFontName.Create(WS_CHILD|WS_VSCROLL|WS_VISIBLE|CBS_DISABLENOSCROLL|CBS_SORT|CBS_DROPDOWNLIST
|CBS_HASSTRINGS,rcFont,&m_wndFontTool,ID_FONTNAME);

m_comboFontName.SetFont(&m_font);
}

void CP2PSendMsgDlg::CreateFontSizeBox()
{
int nIndex=0;
while (m_wndFontTool.GetItemID(nIndex)!=ID_FONTSIZE)
{
nIndex++;
}

CRect rcFont;

m_wndFontTool.SetButtonInfo(nIndex,ID_FONTSIZE,TBBS_SEPARATOR,70);
m_wndFontTool.GetItemRect(nIndex,&rcFont);
rcFont.bottom+=200;
rcFont.top+=2;
rcFont.InflateRect(-2,0);

m_comboFontSize.Create(WS_CHILD|WS_VSCROLL|WS_VISIBLE|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST
|CBS_HASSTRINGS,rcFont,&m_wndFontTool,ID_FONTSIZE);

m_comboFontSize.SetFont(&m_font);
}
*/


BOOL CP2PSendMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	if  ( pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
			{
				HOTKEY_CONFIG *cfg=g_LocalData.GetHotKeyConfig();

				if ( cfg->sendMsgKey==SMK_ENTER  )
				{
					if ( !KEYDOWN(VK_CONTROL) )
						OnBnClickedBtnSend();
					else
					{
						if (m_reditSend.IsFocused() )
							m_reditSend.InsertText("\n");
					}
				}
				else if(cfg->sendMsgKey==SMK_CTRLENTER )
				{
					if  ( KEYDOWN(VK_CONTROL) )
						OnBnClickedBtnSend();
					else
					{
						if (m_reditSend.IsFocused() )
							m_reditSend.InsertText("\n");
					}
				}
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		default:
			;
		}  
	}  

	if ( TranslateAccelerator(m_hWnd,m_hAccel,pMsg)==0 )
	{
		return CXSkinDlg::PreTranslateMessage(pMsg);
	}
	else
	{
		return TRUE;
	}


}
/*
int CP2PSendMsgDlg::InitFontToolBar()
{
if (!m_wndFontTool.CreateEx(this, TBSTYLE_NOPREFIX|TBSTYLE_FLAT|TBSTYLE_CUSTOMERASE, WS_CHILD | WS_VISIBLE | CBRS_TOP 
| CBRS_TOOLTIPS | CBRS_FLYBY   ) ||
!m_wndFontTool.LoadToolBar(IDR_TOOLBAR_FONT))
{
TRACE0("Failed to create toolbar\n");
return -1;      // fail to create
}

SIZE sz;
sz.cx = 19; 
sz.cy = 19;

m_wndFontTool.InitToolBar(sz,IDB_TOOL_FONT_NORMAL,IDB_TOOL_FONT_HOT);

CreateFontNameBox();
CreateFontSizeBox();

return 0;
}
*/

void CP2PSendMsgDlg::OnSendFile()
{
	if ( m_pmP2PMode==pmMultiTalk )
	{
		ShowMessage("对不起！多人聊天不能发送文件！请先和他/她单聊，然后发送文件。谢谢！");
	}
	else
	{
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,NULL,this);
		if ( dlg.DoModal()==IDOK )
		{	
			CString strFileName = dlg.GetPathName();
			P2PSendFile(strFileName);
		}
	}
}

void CP2PSendMsgDlg::OnInvite()
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
	else
		if ( m_pmP2PMode==pmVedio )
		{
			AddMsgTip("对不起，不支持多人视频聊天",COLOR_MSGTIP);
			return ;
		}
		else
			if ( m_pmP2PMode==pmAudio )
			{
				AddMsgTip("对不起，不支持多人语音聊天",COLOR_MSGTIP);
				return ;
			}
			else
			{
				if ( m_bGroupDestoryed )
				{
					AddMsgTip("您不能邀请, 该多人聊天已经撤销。",COLOR_MSGTIP);
					return ;
				}
			}

			CClusterMemberSelectDlg dlg(&m_clusterInfo,this);
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


void CP2PSendMsgDlg::UpdateP2PPanel()
{
	if ( IsIconic() )
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
	case pmAudio:
		m_wndAudioPanel.ShowWindow(SW_HIDE);
		break;
	case pmVedio:
		break;
	}

	switch(m_pmP2PMode)
	{
	case pmSingleTalk:
		::ShowWindow(m_wndAD.GetSafeHwnd(), SW_SHOW);
#ifdef _VER_INTERNAL_
		if(m_pInterNalFriendInfo != NULL)
			::ShowWindow(m_pInterNalFriendInfo->GetSafeHwnd(), SW_SHOW);
#endif
		break;
	case pmMultiTalk:
		m_wndUserList.ShowWindow(SW_SHOW);
		::ShowWindow(m_wndAD.GetSafeHwnd(), SW_HIDE);
#ifdef _VER_INTERNAL_
		if(m_pInterNalFriendInfo != NULL)
			::ShowWindow(m_pInterNalFriendInfo->GetSafeHwnd(), SW_HIDE);
#endif
		break;
	case pmAudio:
		m_wndAudioPanel.ShowWindow(SW_SHOW);
		::ShowWindow(m_wndAD.GetSafeHwnd(), SW_SHOW);
#ifdef _VER_INTERNAL_
		if(m_pInterNalFriendInfo != NULL)
			::ShowWindow(m_pInterNalFriendInfo->GetSafeHwnd(), SW_SHOW);
#endif
		break;
	case pmVedio:
		m_wndVedioPanel.ShowWindow(SW_SHOW);
		::ShowWindow(m_wndAD.GetSafeHwnd(), SW_SHOW);
#ifdef _VER_INTERNAL_
		if(m_pInterNalFriendInfo != NULL)
			::ShowWindow(m_pInterNalFriendInfo->GetSafeHwnd(), SW_SHOW);
#endif
		break;
	}


	if (m_bShowHistory) {
		m_wndPopupHistory.ShowWindow(SW_SHOW);
		m_wndUserList.ShowWindow(SW_HIDE);
		::ShowWindow(m_wndAD.GetSafeHwnd(), SW_HIDE);
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
		GetWindowRect(&rc);
		rc.right+=nSize;

		if ( nSize>0 )
		{
			m_nPreDlgHeight = rc.Height();
		}
		else
		{
			rc.bottom = rc.top + m_nPreDlgHeight;
		}

		MoveWindow(&rc);
	}



	//FIXME: 临时隐藏
	//m_wndVedioPanel.ShowWindow(SW_HIDE);
	m_wndUserPanel.ShowWindow(SW_HIDE);
	m_wndSplitter1.ShowWindow(SW_HIDE);

	AdjustUI();

	if ( m_pmP2PMode==pmVedio)
		SetMinMaxInfo(500,500,0,0);
	else
		SetMinMaxInfo(500,300,0,0);

	m_pmPreP2PMode = m_pmP2PMode;
	m_bPreShowHistory = m_bShowHistory;
#ifdef _VER_INTERNAL_
	::ShowWindow(m_wndAD.GetSafeHwnd(), SW_HIDE);
#endif 

}

void CP2PSendMsgDlg::UpdateCluserInfo(uint32 cid)
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

void CP2PSendMsgDlg::OnCMemberInd(const char *data, WORD wRet)
{
	LPXT_CMEMBER_IND ind = (LPXT_CMEMBER_IND)data;
	if ( wRet==0 )
	{
		UpdateCluserInfo(ind->cluster_id);
	}
// 	if(m_pmP2PMode!=pmMultiTalk)
// 		return;
// 	size_t c;
// 	int nItem = -1;	
// 	if ( wRet==0 && m_bClusterInit )
// 	{
// 		if ( ind->cluster_id==m_clusterInfo.nFamilyID && ind->cluster_type==TEMP_FAMILY && ind->member_id!=0 )
// 		{
// 			if ( ind->action==0 )//添加
// 			{
// 				nItem = m_clusterInfo.FindMember(ind->member_id);
// 				if(nItem != -1)
// 				{
// 					m_uiAddFriendId = 0;
// 					return;
// 				}
// 				m_uiAddFriendId = ind->member_id;
// 				FRIEND_INFO * pFriend = ::GetFriendInfo(ind->member_id);
// 				if(pFriend != NULL)
// 				{
// 					pFriend->status = FRIEND_INFO::ONLINE;
// 					m_clusterInfo.members.push_back(*pFriend);
// 					m_wndUserList.UpdateClusterList();
// 					AddCMemberTip(pFriend,1);
// 					ShowDlgTitle();
// 					m_uiAddFriendId = 0;
// 				}
// 				else
// 				{
// 					FRIEND_INFO f;
// 					f.id = ind->member_id;
// 					itoa(ind->member_id,f.nickname,10);
// 					strcpy(f.username,f.nickname);
// 					f.status = FRIEND_INFO::ONLINE;
// 					m_clusterInfo.members.push_back(f);
// 					m_wndUserList.UpdateClusterList();
// 					AddCMemberTip(&f,1);
// 					ShowDlgTitle();
// 					XT_USERINFO_GET_REQ req;
// 					req.fid = m_uiAddFriendId;
// 					g_pProtocol->SendReq(req);
// 				}
// 			}
// 			else if ( ind->action==1 )//退出,删除
// 			{
// 				uint32 mid = ind->member_id;
// 				int nItem = -1;	
// 				c = m_clusterInfo.Count();
// 
// 				nItem = m_clusterInfo.FindMember(ind->member_id); 
// 
// 				if ( nItem != -1 )
// 				{
// 					AddCMemberTip((XT_USER_INFO *)&m_clusterInfo.members[nItem],2);
// 					// 删除
// 					m_clusterInfo.members.erase(m_clusterInfo.members.begin() + nItem);
// 					if ( m_clusterInfo.Count() == 2 )
// 					{
// 						uint32 fid = m_clusterInfo.members[0].id==g_ImUserInfo.GetId() ? 
// 							m_clusterInfo.members[1].id : m_clusterInfo.members[0].id;
// 
// 						m_pmP2PMode = pmSingleTalk;
// 
// 						SetBtnFolderBitmap();
// 
// 						FRIEND_INFO *pFriend = FindFriend(fid);
// 
// 						if ( pFriend==NULL )
// 						{
// 							pFriend=m_clusterInfo.GetFriendInfo(fid);
// 							pFriend->ip=0;
// 							pFriend->port=0;
// 						}
// 
// 						if ( pFriend )
// 						{
// 							m_FriendInfo = * pFriend;
// 							m_wndUserList.ShowWindow(SW_HIDE);
// 							m_wndUserPanel.ShowWindow(SW_SHOW);
// 							m_wndUserPanel.SetFriendInfo(&m_FriendInfo);
// 
// 
// 							AddMsgTip("多人临时对话已经撤销了！");
// 
// 							m_wndUserPanel.UpdateFriendInfo();
// 							m_wndUserPanel.AdjustUI();
// 
// 
// 							if ( g_pMessageHandleWnd->FindP2PSendMsgDlg(pFriend->id)==false )
// 							{
// 								::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2P_TO_GROUP,1,m_clusterInfo.nFamilyID);
// 
// 								m_clusterInfo.nFamilyID=0;
// 								m_clusterInfo.members.clear();
// 							}
// 							else
// 							{
// 								CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
// 
// 								if ( pDlg->IsIconic() )
// 									pDlg->ShowWindow(SW_RESTORE);
// 
// 								pDlg->BringWindowToTop();
// 								pDlg->AddMsgTip("多人临时对话已经撤销了！");
// 
// 								MessageBox("多人临时对话已经撤销了！");
// 								DestroyWindow();
// 								return ;
// 							}
// 						}
// 						else
// 						{
// 							MessageBox("多人临时对话已经撤销了！");
// 							DestroyWindow();
// 							return ;
// 						}
// 					}	
// 					else if ( m_clusterInfo.Count() < 2 )
// 					{
// 						MessageBox("多人临时对话已经撤销了！");
// 						DestroyWindow();
// 						return ;
// 					}
// 					else
// 					{
// 						m_wndUserList.UpdateClusterList();
// 					}
// 				}
// 			}
// 		}
// 		//更新标题
// 		ShowDlgTitle();
// 	}
}

void CP2PSendMsgDlg::OnGetClusterMember(const char *data,WORD wRet)
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
				if ( IsWindowVisible()==false )
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
					FlashWindow(TRUE);
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


void CP2PSendMsgDlg::OnClusterDismissInd(const char *data,WORD wRet)
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

void CP2PSendMsgDlg::OnFriendInfoGet(const char *data, WORD wRet)
{
	LPXT_USERINFO_GET_ACK ack=(LPXT_USERINFO_GET_ACK)data;

	FRIEND_INFO f;

	if ( wRet==0 )
	{
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
				m_wndUserPanel.UpdateFriendInfo();
			}
		}

		ShowDlgTitle();
	}
}

void CP2PSendMsgDlg::UpdateFriendInfo(FRIEND_INFO *pFriend)
{
	memcpy(&m_FriendInfo,pFriend,sizeof(XT_USER_INFO));
	m_wndUserPanel.UpdateFriendInfo();
}

void CP2PSendMsgDlg::OnLogOut(const char *data,WORD wRet)
{
	DestroyWindow();
}

BOOL CP2PSendMsgDlg::DestroyWindow()
{
	BOOL bForce = false;

	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		bForce = true;
	}

	if ( IsWindowVisible() )
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

		if ( m_pmP2PMode==pmAudio )
		{
			m_wndAudioPanel.StopAudio();
			MM_Stop();
		}

		if ( m_pmP2PMode==pmVedio )
		{
			m_wndVedioPanel.StopVedio();
			MM_Stop();
		}


		m_reditSend.ClearMsg();
		m_reditHistory.ClearMsg();
	}

	if(m_wndToolBar.IsDownloader())
	{
		if(bForce == FALSE && CMessageBox::Model(GetSafeHwnd(),"如果关掉窗口，将会终止下载，是否关闭窗口？","",MB_YESNO/*|MB_DEFBUTTON2*/)==IDNO)
		{
			return FALSE;
		};
		m_wndToolBar.StopDownloadFace();
	}

	return __super::DestroyWindow();
}

void CP2PSendMsgDlg::SF_OnRecvFileReq(char *file_name)
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

void CP2PSendMsgDlg::SF_OnRecvFilePacket(char * file_name, int file_id )
{
	SF_OnRecvFileReq(file_name);
}

void CP2PSendMsgDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
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

					if ( m_bMMStartTalk )
					{						
						if ( g_ImUserInfo.ip== m_FriendInfo.ip )
						{
							//StartTalk(m_FriendInfo.id,m_FriendInfo.localIP,htons(m_FriendInfo.localPort),g_pProtocol->GetTranServerIP(),g_pProtocol->GetTranServerPort());
							if(m_pmP2PMode == pmAudio)
							{
								m_wndAudioPanel.StartAudio(m_FriendInfo.localIP);
								m_wndAudioPanel.SetStatusText("连接成功");
							}
							else
							{
								m_wndVedioPanel.StartVedio(0,m_FriendInfo.localIP);
								m_wndVedioPanel.SetStatusText("连接成功");
							}
						}
						else
						{
							//StartTalk(m_FriendInfo.id,m_FriendInfo.ip,htons(m_FriendInfo.port),g_pProtocol->GetTranServerIP(),g_pProtocol->GetTranServerPort());

							if(m_pmP2PMode == pmAudio)
							{
								m_wndAudioPanel.StartAudio(m_FriendInfo.ip);
								m_wndAudioPanel.SetStatusText("连接成功");
							}
							else
							{
								m_wndVedioPanel.StartVedio(0,m_FriendInfo.ip);
								m_wndVedioPanel.SetStatusText("连接成功");
							}
						}

						m_bMMStartTalk=false;
					}

					if ( m_bMMWaitTalk )
					{			
						if ( g_ImUserInfo.ip== m_FriendInfo.ip )
						{
							//WaitTalk(m_FriendInfo.id,m_FriendInfo.localIP,htons(m_FriendInfo.localPort),m_nPeerTransServerIp,m_nPeerTransServerPort);
							if(m_pmP2PMode == pmAudio)
							{
								m_wndAudioPanel.StartAudio(m_FriendInfo.localIP);
								m_wndAudioPanel.SetStatusText("连接成功");
							}
							else
							{
								m_wndVedioPanel.StartVedio(0, m_FriendInfo.localIP);
								m_wndVedioPanel.SetStatusText("连接成功");
							}
						}
						else
						{
							//WaitTalk(m_FriendInfo.id,m_FriendInfo.ip,htons(m_FriendInfo.port),m_nPeerTransServerIp,m_nPeerTransServerPort);
							if(m_pmP2PMode == pmAudio)
							{
								m_wndAudioPanel.StartAudio(m_FriendInfo.ip);
								m_wndAudioPanel.SetStatusText("连接成功");
							}
							else
							{
								m_wndVedioPanel.StartVedio(0, m_FriendInfo.ip);
								m_wndVedioPanel.SetStatusText("连接成功");

							}

						}

						XT_MSG msg;
						MM_InitXtMsg(msg);

						msg.data_type = XT_MSG::MM_REPLY;

						CSendPacket sp(msg.data);

						sp<<(uint32)m_mmWaitInfo.mmID<<(uint8)m_mmWaitInfo.mmType<<(uint8)m_mmWaitInfo.nAccept;

						msg.data_len = sp.GetSize();

						g_pProtocol->SendP2PMsg(msg);

						m_bMMWaitTalk = false;
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

				if ((time_now-wi.time_recv) > 20000)
				{
					m_reditHistory.OnAbortImageFile(wi.szFile);				
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
			GetWindowRect(rcDlg);
			int nXpos = rcDlg.Width(); //宽度
			int nYpos = rcDlg.Height();//宽度
			switch(m_nWinShakeCnt%4)
			{
			case 0:
				rcDlg.left =  rcDlg.left + 3;
				break;
			case 1:
				rcDlg.top =  rcDlg.top + 3;
				break;
			case 2:
				rcDlg.left =  rcDlg.left - 3;
				break;
			case 3:
				rcDlg.top =  rcDlg.top - 3;
				break;
			default:
				break;
			}

			rcDlg.right = rcDlg.left + nXpos;//宽度
			rcDlg.bottom = rcDlg.top + nYpos;//高度P2P_DLG_HIGHT;
			MoveWindow(rcDlg);
			m_nWinShakeCnt++;

			if(m_nWinShakeCnt > 20)
				KillTimer(TIMER_WIN_SHAKE);
		}		
		break;
	case TIMER_SHOW_USERCARD:
		{
			KillTimer(nIDEvent);

			CPoint   point;
			GetCursorPos(&point);

			/*CRect rc;
			m_ctrlFaceIcon.GetWindowRect(rc);

			if(rc.PtInRect(point))
			{
				static std::tr1::shared_ptr<CUserCardDlg> pInstance = CUserCardDlg::GetInstance();
				if(pInstance == NULL)
				{
					int nLeft = rc.left-318;

					if ( nLeft<0 )
					{	
						CRect rcDlg;
						GetWindowRect(rcDlg);
						nLeft = rcDlg.right + 8;
					}

					CUserCardDlg::Show(&m_FriendInfo,nLeft,rc.top,rc);
				}
			}*/
		
		}
		break;
	case TIMER_CLIPBOARD:
		{
			char * buffer;
			if(OpenClipboard())
			{
				CString strData = (char *)GetClipboardData(CF_TEXT);
				int nClipBoardIndex = strData.Find(RIGHT_URL_CLIPBOARD_CHECK);
				if(strData != "" && nClipBoardIndex != -1)//判断如果有"moumou-link:"才获取剪切板
				{
					if(GetSafeHwnd() == GetActiveWindow()->GetSafeHwnd())
					{
						CString strUrlCheck(RIGHT_URL_CLIPBOARD_CHECK);
						int nUrlCheckLength = strUrlCheck.GetLength();
						CString strClipBoardUrl = strData.Right(strData.GetLength() - nClipBoardIndex - nUrlCheckLength);
						m_reditSend.InsertLink(strClipBoardUrl);
						EmptyClipboard();
					}
				}	
				CloseClipboard();
			}
		}
		break;
	}

END0:
	__super::OnTimer(nIDEvent);
}

void CP2PSendMsgDlg::OnStatusGetAck(const char *data,WORD wRet)
{
	if ( wRet == 0 && m_pmP2PMode!=pmMultiTalk )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;

		if ( ack && ack->id == m_FriendInfo.GetId() )
		{
			if ( ack->status == XTREAM_HIDE )
			{
				ack->status = XTREAM_OFFLINE;
			}
			m_FriendInfo.ip     = ack->ip;
			m_FriendInfo.port   = ack->port;
			m_FriendInfo.status = ack->status;
			m_FriendInfo.localIP = ack->lanip;
			m_FriendInfo.localPort = ack->lanport;
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
			m_wndUserPanel.UpdateFriendInfo();

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


void CP2PSendMsgDlg::OnStatusModAck(const char *data,WORD wRet)
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
					m_wndUserPanel.ShowWindow(SW_SHOW);
					m_wndUserPanel.SetFriendInfo(&m_FriendInfo);
					m_wndUserPanel.UpdateFriendInfo();

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

			m_wndUserPanel.UpdateFriendInfo();

			m_reditSend.EnableWindow(FALSE);
			m_btnSend.EnableWindow(FALSE);
		}
		else
		{
			m_reditSend.EnableWindow(TRUE);
			m_btnSend.EnableWindow(TRUE);

			RefreshFriendStatus();

			m_wndUserPanel.UpdateFriendInfo();
		}			
	}	
}

BOOL CP2PSendMsgDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient, rcHide, rcBottom;
	GetClientRect(rcClient);
	if( m_wndHide.GetSafeHwnd())
	{
		m_wndHide.GetWindowRect(rcHide);
	}
	ScreenToClient(&rcHide);

	m_reditSend.GetWindowRect(&rcBottom);
	ScreenToClient(&rcBottom);
	rcBottom.top = rcBottom.bottom;
	rcBottom.bottom = rcClient.bottom;
	rcBottom.left = rcClient.left;

	if (rcClient.right!=rcHide.right)
		return FALSE;

	CRgn rgnPaint;
	GetRedrawRgn(rgnPaint);
	pDC->SelectObject(&rgnPaint);

	pDC->FillSolidRect(rcClient,m_colorBackground);
	pDC->FillSolidRect(rcBottom, CHAT_BOTTOM_COLOR);

	// 绘制聊天区与广告栏之间的分割线
	CRect rcAD;
	m_wndAD.GetWindowRect(&rcAD);
	ScreenToClient(&rcAD);
	CPen pen(PS_SOLID, 1, RGB(222, 222, 222));
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rcAD.left-1, rcAD.top);
	pDC->LineTo(rcAD.left-1, rcAD.bottom);
	pDC->SelectObject(pOldPen);

	return FALSE;
}

void CP2PSendMsgDlg::OnUpdateFontStyle(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(1);
}

void CP2PSendMsgDlg::OnPaint()
{
	CPaintDC dc(this);

	DrawBorder();
	DrawCaptionBar();
	m_btnHangup.Invalidate(TRUE);
	CDC *pDC = GetWindowDC();
	if(pDC==NULL)
		return;

	int nSavedDC = pDC->SaveDC();

	CRect rc;
	GetWindowRect(rc);

	int cx=rc.Width(); 
	int cy=rc.Height();

	pDC->Draw3dRect(1,1,cx-2,cy-2,m_colorBorder,m_colorBorder);  //边框

	pDC->RestoreDC(nSavedDC);
	ReleaseDC(pDC);

	//DrawBorder();

	//	CRect rcHistory, rcTool, rcSend;
	//
	// 	if ( m_wndToolBar.m_hWnd && m_reditHistory.m_hWnd && m_reditSend.m_hWnd )
	// 	{
	//         m_reditHistory.GetWindowRect(&rcHistory);
	// 		m_wndToolBar.GetWindowRect(&rcTool);
	// 		m_reditSend.GetWindowRect(&rcSend);
	// 
	// 		rcHistory.right++;
	// 		rcSend.right++;
	// 
	// 		if ( m_wndP2PFileTransProgressBar.IsWindowVisible() )
	// 		{
	// 			CRect rcTemp;
	// 			m_wndP2PFileTransProgressBar.GetWindowRect(&rcTemp);
	// 			rcHistory.top = rcTemp.top;
	// 		}
	// 
	// 		ScreenToClient(&rcHistory); rcHistory.InflateRect(1,1);
	// 		ScreenToClient(&rcTool);
	// 		ScreenToClient(&rcSend); rcSend.InflateRect(1,1);
	// 
	// 		COLORREF clrBorder = BORDER_COLOR;
	// 
	// 		dc.Draw3dRect(rcHistory,clrBorder,clrBorder);
	// 
	// 		rcSend.top = rcTool.bottom+1;
	// 		dc.Draw3dRect(rcSend,clrBorder,clrBorder);
	// 		
	// 		CPen pen(PS_SOLID,1,clrBorder);
	// 
	// 		CPen *pOldpen = dc.SelectObject(&pen);
	// 
	// 		dc.MoveTo(rcHistory.left,rcHistory.bottom);
	// 		dc.LineTo(rcSend.left,rcSend.top);
	// 
	// 		dc.MoveTo(rcHistory.right-1,rcHistory.bottom);
	// 		dc.LineTo(rcSend.right-1,rcSend.top);
	// 
	// 		dc.SelectObject(pOldpen);
	// 	}
}

bool CP2PSendMsgDlg::GetRedrawRgn(CRgn &rgn)
{
	CRect rc;
	GetClientRect(&rc);

	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);

	RemoveRgnOfWnd(rgn,&m_wndToolBar);
	//RemoveRgnOfWnd(rgn,&m_wndSplitter1);
	RemoveRgnOfWnd(rgn,&m_reditHistory);
	RemoveRgnOfWnd(rgn,&m_reditSend);

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

void CP2PSendMsgDlg::OnBnClickedFontBar()
{
	CRect rc1;
	m_reditHistory.GetWindowRect(&rc1);
	ScreenToClient(&rc1);

	if ( m_wndTalkToolBar.IsWindowVisible() )
	{
		rc1.bottom+=30;

		m_wndTalkToolBar.ShowWindow(SW_HIDE);
		m_reditHistory.MoveWindow(rc1);		
	}
	else
	{
		CRect rc2=rc1;

		rc1.bottom-=30;
		m_reditHistory.MoveWindow(rc1);	

		rc2.top = rc1.bottom;
		rc2.bottom=rc2.top+30;

		m_wndTalkToolBar.MoveWindow(rc2);
		m_wndTalkToolBar.ShowWindow(SW_SHOW);
		m_reditHistory.MoveWindow(rc1);	
	}

	m_reditSend.SetFocus();
}



void CP2PSendMsgDlg::OnScreenCopyOption()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	m_wndToolBar.m_btnScreen.GetWindowRect(&rc);

	CPoint p;
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

	id=menu.Track(p.x,p.y,GetSafeHwnd(),true);


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

void CP2PSendMsgDlg::OnScreenCopyEx()
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

void CP2PSendMsgDlg::OnScreenCopy()
{	

	HOTKEY_CONFIG *cfg;
	cfg = g_LocalData.GetHotKeyConfig();

	if(cfg->bScreenCopyHideWindow)
	{
		this->ShowWindow(SW_HIDE);
	}

	//CScreenCopyDlg dlg(GetDesktopWindow());
	CScreenCtrl dlg(GetDesktopWindow());
	dlg.m_strImageFileCachePath = g_LocalData.GetImageFileCachePath();
	if ( IDOK==dlg.DoModal() )
	{
		CString strCacheFileName;
		g_LocalData.SaveImageFileToCache(dlg.m_strImageFile,strCacheFileName);
		CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

		DeleteFile(dlg.m_strImageFile);

		//m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());
		m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);
	}
	
	this->ShowWindow(SW_SHOW);
}

void CP2PSendMsgDlg::OnMagicFace()
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

	if ( bCanSendPic )
	{
		CPoint pt;
		GetCursorPos(&pt);

		CMagicFaceDlg::Show(pt.x,pt.y,g_ImUserInfo.GetId(),m_hWnd);
	}
}

void CP2PSendMsgDlg::ShowMagicFace(const char *face_name)
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

void CP2PSendMsgDlg::RecvSendFileReq(const char *data, WORD wRet)
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
			m_reditHistory.SetFileBarTip(req.fid, ODT_RECV_FILE,"您拒绝接收该文件",RGB(255,0,0));
			m_reditHistory.InsertLink("更新设置");
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

			m_reditHistory.SetSel(-1,-1);
			m_reditHistory.InsertText("\n");
			m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

			m_reditHistory.SetStartIndent(80);
			m_reditHistory.InsertText(strTitle);



			m_reditHistory.InsertFileInfoBar(req.fid,false,req.name,req.size.sizel);

			if ( m_reditHistory.m_hWnd )
				m_reditHistory.ScrollToBottom();

			tagP2PFile f;
			f.nID     = req.fid;
			f.nSize   = req.size.sizel;
			f.strFile = req.name;

			m_vecRecvFile.push_back(f);

			/*CRecentListCtrl *pRecentList=(CRecentListCtrl *)g_pRecentListCtrl;
			pRecentList->AddUser(&m_FriendInfo);*/
		}
	}
}

bool CP2PSendMsgDlg::RecvMagicFace(uint32 friend_id, CString strFaceName, CString strFaceUrl)
{
	map<CString,CString>::iterator it = m_mapMagicFace.find(strFaceName);

	if ( it==m_mapMagicFace.end() )
		m_mapMagicFace.insert(map<CString,CString>::value_type(strFaceName,strFaceUrl));

	CString strTip;

	FRIEND_INFO *pFriend = NULL;

	if ( m_pmP2PMode!=pmMultiTalk )
	{
		pFriend=&m_FriendInfo;
	}
	else
	{
		pFriend = ::GetFriendInfo(friend_id);
	}

	if ( pFriend && pFriend->GetId()==friend_id )
	{
		CString strTip;
		strTip.Format("%s 发送了魔法表情",pFriend->nickname);

		if ( m_hWnd && m_reditHistory.m_hWnd )
		{
			XT_FONT xf;
			xf.fontColor = RGB(122,122,122);

			CHARRANGE cr;

			m_reditHistory.SetSel(-1,-1);
			m_reditHistory.GetSel(cr);

			m_reditHistory.InsertText("\n");
			m_reditHistory.SetStartIndent(120);

			CResBitmap bmpLine;
			bmpLine.LoadBitmap(IDB_HINT_FLAG2);
			m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

			m_reditHistory.InsertText(strTip);

			strTip.Format("“%s”",strFaceName);
			m_reditHistory.InsertLink(strTip);
			m_reditHistory.InsertText("  ");

			m_reditHistory.SetSel(cr.cpMax,-1);
			m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			m_reditHistory.SetSel(-1,-1);

			m_reditHistory.ScrollToBottom();
		}
		else
		{
			_tagMsg t_msg;
			t_msg.msg.from_id = friend_id;
			sprintf(t_msg.msg.fontName,strFaceName);
			sprintf(t_msg.msg.data,strFaceUrl);
			t_msg.type=2;
			PushMsg(t_msg); 
		}
	}
	else
	{
		return false;
	}

	/*CRecentListCtrl *pRecentList=(CRecentListCtrl *)g_pRecentListCtrl;
	pRecentList->AddUser(&m_FriendInfo);*/

	return true;
}
void CP2PSendMsgDlg::OnBnClickedBtnCard()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

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

void CP2PSendMsgDlg::OnBnClickedBtnVisit()
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

void CP2PSendMsgDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	if ( nState!=WA_INACTIVE )
	{
		//if (IsWindowVisible() && m_bEnableActive==false )
		//{
		//	if ( pWndOther && pWndOther->m_hWnd )
		//	{
		//		pWndOther->SetFocus();
		//		pWndOther->BringWindowToTop();
		//	}
		//}

		if ( m_pmP2PMode==pmSingleTalk || m_pmP2PMode==pmAudio )
		{
			m_wndUserPanel.Invalidate();
			m_wndAD.Invalidate();

			//CPoint ptOrigin;
			//GetCursorPos(&ptOrigin);

			//CRect rcClick;
			//m_wndUserPanel.m_wndAvatarFrame.GetWindowRect(&rcClick);

			//SetCursorPos(rcClick.left+2,rcClick.top+2);
			//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			//SetCursorPos(ptOrigin.x,ptOrigin.y);
		}

// 		if(g_pDuiCounsel != NULL && m_FriendInfo.GetId() >= 5000000)
// 		{			
// 			if(g_pDuiCounsel->m_pDlgSelectP2PSendMsgDlg == this || g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1)
// 			{
// 				CRect rc;
// 				GetWindowRect(&rc);
// 				::SetWindowPos(*g_pDuiCounsel, *this, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_NOACTIVATE);
// 				::MoveWindow(*g_pDuiCounsel, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
// 			}
// 		}
		m_reditHistory.ScrollToBottom();
		m_reditSend.SetFocus();
		m_dwTickCountActive = GetTickCount();
	}
}



LRESULT CP2PSendMsgDlg::OnThreadNotify(WPARAM w, LPARAM l)
{
	if ( w==0 )
	{
		FRIEND_INFO *p=(FRIEND_INFO *)l;

		try
		{
			CString s;
			s.Format("%s,",p->nickname);
			m_reditSend.InsertText(s);
			m_reditSend.SetFocus();
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

void CP2PSendMsgDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);
	if ( bShow )
	{
		m_reditSend.SetFocus();
	}
}

void CP2PSendMsgDlg::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	m_reditSend.SetFocus();
}

// 建立临时组成员群聊
void CP2PSendMsgDlg::CreateTempCluster(const CLUSTER_INFO& ci,int nGroupIndex)
{
	//m_pmP2PMode = pmMultiTalk;
	////////////////////////////////////////////////////////////////
	//显示用户名和签名
	GetDlgItem(IDC_STATIC_USERNAME)->SetFont(&g_font16B); 
	SetDlgItemText(IDC_STATIC_USERNAME,ci.szFamilyName);

	//显示群描述
	GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
	SetDlgItemText(IDC_STATIC_SIGN, _T(""));
	//显示添加按钮
	m_btnAddMember.ShowWindow(SW_SHOW);
	////////////////////////////////////////////////////////////////
	XT_CREATE_CLUSTER_REQ req;
	req.nFamilyProp  = TEMP_FAMILY;
	g_pProtocol->SendReq(req,&m_wSeq);

	m_clusterInfo=ci;
	m_nTempGroupTalkIndex=nGroupIndex;

	m_wndUserList.ShowWindow(SW_SHOW);
	m_wndUserPanel.ShowWindow(SW_HIDE);
	// 临时群建立前不能发送消息
	m_btnSend.EnableWindow(FALSE);
	// 将自己添加进列表
	m_clusterInfo.members.push_back(g_ImUserInfo);
	m_wndUserList.SetClusterInfo(&m_clusterInfo);
	m_wndUserList.UpdateClusterList();
	//UpdateP2PPanel();
}


void CP2PSendMsgDlg::SetClusterMember(uint32 clusterId)
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
void CP2PSendMsgDlg::OnCreateTempClusterAck(const char *data,WORD wRet)
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
			//::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2P_TO_GROUP,0,(LPARAM)&m_FriendInfo);
			m_pmP2PMode = pmMultiTalk;
			// 显示标题
			ShowDlgTitle();
			SetBtnFolderBitmap();
			if ( IsWindowVisible()==FALSE )
				ShowWindow(SW_SHOW);

			if (IsIconic() )
			{
				ShowWindow(SW_RESTORE);
			}
			BringWindowToTop();
		}
		else
		{
			g_pMessageHandleWnd->m_mapTempClusterMsgDlg.insert(
				map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::value_type(m_clusterInfo.nFamilyID,this));
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
void CP2PSendMsgDlg::OnTempCMembersSetAck(const char *data,WORD wRet)
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

LRESULT	CP2PSendMsgDlg::OnStartDownloadFace(WPARAM w, LPARAM l)
{
	if ( m_wndToolBar.StartDownloadFace() )
	{
		m_dwProgressBar = m_reditHistory.FindProgressBar();
		if ( m_dwProgressBar==(DWORD)-1 )
			m_dwProgressBar = m_reditHistory.InsertProgressBar();
		m_reditHistory.SetProgressBarPos(m_dwProgressBar,0);
		m_reditHistory.SetProgressBarTip(m_dwProgressBar,"正在下载表情...");
		m_reditHistory.ScrollToBottom();
	}
	else
	{
		AddMsgTip("正在另一个窗口中下载表情...",RGB(0,122,0));
	}

	return 0;
}

LRESULT	CP2PSendMsgDlg::OnDownloadFaceOK(WPARAM w, LPARAM l)
{
	if(w==0)
	{
		if ( m_dwProgressBar!=(DWORD)-1 )
		{
			m_reditHistory.SetProgressBarTip(m_dwProgressBar,"下载完成");
			m_reditHistory.SetProgressBarPos(m_dwProgressBar,100);

			CString strTip;
			strTip.Format("本次成功下载%d个表情",l);
			AddMsgTip(strTip);
		}
	}
	else if(w==1)
	{
		//	AddMsgTip("表情文件下载失败:");
		//	AddMsgTip((LPCTSTR)l);
		if ( m_dwProgressBar!=(DWORD)-1 )
		{
			m_reditHistory.SetProgressBarTip(m_dwProgressBar,"下载失败!");
		}
	}

	return 0;
}

LRESULT CP2PSendMsgDlg::OnDownloadProcess(WPARAM w, LPARAM l)
{
	int nTask=(int)w;
	int nProcess=(int)l;
	//CString sTip;
	//sTip.Format("当前下载文件(%d|%d)",nProcess,nTask);
	//AddMsgTip(sTip);
	if ( m_dwProgressBar!=(DWORD)-1 )
	{
		int nPos = nProcess*100.0/nTask;
		m_reditHistory.SetProgressBarPos(m_dwProgressBar,nPos);
	}
	return 0;
}

void CP2PSendMsgDlg::OnBnClickedBtnFold()
{
	m_bClosePanel = !m_bClosePanel;

	SetBtnFolderBitmap();

	AdjustUI();

	if ( m_reditHistory.m_hWnd )
		m_reditHistory.ScrollToBottom();
}

void CP2PSendMsgDlg::SetBtnFolderBitmap()
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
	if(!download.DownLoadWebFile(pObj->sUrl,pObj->sSaved))
	{
		pObj->state = 2;
		return 1;
	}
	pObj->state = 1;
	return 0;
};


void CP2PSendMsgDlg::ProcTalkMsg(const CString & sMsg,CString& sShowMsg)
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
						SetTimer(TIMER_DOWNLOADOBJ,300,NULL);
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

void CP2PSendMsgDlg::P2PSendFile(CString strFile)
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

		m_reditHistory.SetSel(-1,-1);
		m_reditHistory.InsertText("\n");
		m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

		m_reditHistory.SetStartIndent(80);
		m_reditHistory.InsertText(strTitle);

		long nFileSize = ::GetFileSize(strFile);

		tagP2PFile f;
		f.nID		= file_id;
		f.nSize		= nFileSize;
		f.strFile	= strFile;

		m_vecSendFile.push_back(f);

		m_reditHistory.InsertFileInfoBar(file_id,true,strFileName,nFileSize);

		if ( m_reditHistory.m_hWnd )
			m_reditHistory.ScrollToBottom();

		//CRecentListCtrl *pRecentList=(CRecentListCtrl *)g_pRecentListCtrl;
		//pRecentList->AddUser(&m_FriendInfo);

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


void CP2PSendMsgDlg::P2PSendFileReplay(long nFileId,bool bRecv)
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
void CP2PSendMsgDlg::AF_OnRecvStatusNotify(uint32 id,TASK_STATUS status)
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

void CP2PSendMsgDlg::OnConnectTalkOk(bool bOk)
{
	if ( bOk )
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,0,1);
	else
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,0,0);
}

void CP2PSendMsgDlg::OnAcceptTalkOk(bool bOk)
{
	if ( bOk )
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,1,1);
	else
		::PostMessage(m_hWnd,WM_P2PNET_NOTIFY,1,0);
}

bool CP2PSendMsgDlg::AF_CanTrans(uint32 id)
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

			m_reditHistory.SetFileBarTip(id,ODT_SEND_FILE,"服务器中转失败",RGB(255,0,0));
			return false;
		}
	}

	return false;
}

void CP2PSendMsgDlg::AF_OnSendStatusNotify(uint32 id,TASK_STATUS status)
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

void CP2PSendMsgDlg::AF_OnSendFileReturn(uint32 fileId,bool bAccept)
{

	uint16 file_id = fileId;

	int n = m_vecSendFile.size();

	for ( int i=0; i<n; i++ )
	{
		if ( m_vecSendFile[i].nID==file_id )
		{
			if ( bAccept )//对方同意接受
			{
				m_reditHistory.SetFileBarTip(file_id,ODT_SEND_FILE,"对方同意了接收该文件！");

				ShowFileTransInfoBar(true);

				m_wndP2PFileTransProgressBar.AddItem(m_vecSendFile[i].strFile,m_vecSendFile[i].nSize,m_vecSendFile[i].nID,TRUE);
			}
			else
			{
				m_reditHistory.SetFileBarTip(file_id,ODT_SEND_FILE,"对方拒绝接收！",RGB(255,0,0) );
			}

			m_vecSendFile.erase(m_vecSendFile.begin()+i);
			break;
		}
	}
}

void CP2PSendMsgDlg::StopDownload(const CString& sFileID,bool bTerminate)
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
		KillTimer(TIMER_DOWNLOADOBJ);
	}

}


void CP2PSendMsgDlg::Download(const vector<DOWNLOAD_OBJ>& arrTask)
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
			SetTimer(TIMER_DOWNLOADOBJ,300,NULL);
		}
		// 保存当前项到字典
		m_dicDownloadObj[sMD5] = pParam;
	}
}

LRESULT CP2PSendMsgDlg::OnP2PAudioNotify(WPARAM w,LPARAM l)
{
	switch(l)
	{
	case 1://stop audio talk
		if ( m_pmP2PMode==pmAudio )
		{
			m_wndAudioPanel.StopAudio();
			MM_Stop();

			m_pmP2PMode=pmSingleTalk;
			//UpdateP2PPanel();
			if(m_wndAudioPanel && m_wndAudioPanel.IsWindowVisible())
			{
				m_wndAudioPanel.ShowWindow(SW_HIDE);
			}
		}
		break;
	}
	return 0;
}

LRESULT CP2PSendMsgDlg::OnP2PNetNotify(WPARAM wParam,LPARAM lParam)
{
	switch (wParam)
	{
	case 0:// vedio connect  
	case 1:// vedio accept/wait  
		{
			if ( lParam==1 )
			{
				if ( m_pmP2PMode==pmVedio )
				{
					m_wndVedioPanel.SetStatusText("连接成功");
					//m_wndVedioPanel.StartVedio(g_LocalData.GetDefaultCameraIndex());
				}
				else
					if ( m_pmP2PMode==pmAudio )
					{
						m_wndAudioPanel.SetStatusText("连接成功");
						//m_wndAudioPanel.StartAudio();
					}
			}
			else
				if (lParam==0)
				{
					if ( m_pmP2PMode==pmVedio )
						m_wndVedioPanel.SetStatusText("视频连接失败，请重新再试");
					else
						if ( m_pmP2PMode==pmAudio )
							m_wndAudioPanel.SetStatusText("语音连接失败，请重新再试");
				}
		}
		break;
	}

	return 0;
}

LRESULT CP2PSendMsgDlg::OnP2PVedioNotify(WPARAM w,LPARAM l)
{
	switch(l)
	{
	case 1://stop vedio talk
		if ( m_pmP2PMode==pmVedio )
		{
			MM_Stop();
			m_wndVedioPanel.StopVedio();

			m_pmP2PMode=pmSingleTalk;
			//UpdateP2PPanel();
			if(m_wndVedioPanel && m_wndVedioPanel.IsWindowVisible())
			{
				m_wndVedioPanel.ShowWindow(SW_HIDE);
			}
		}
		break;
	}
	return 0;
}

LRESULT CP2PSendMsgDlg::OnUserDragDropFile(WPARAM w,LPARAM l)
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

LRESULT CP2PSendMsgDlg::OnInsertMsg(WPARAM w, LPARAM l)
{
	LPCTSTR szMsg=(LPCTSTR)l;
	if(szMsg!=NULL)
	{
		//m_reditSend.ClearMsg();
		m_reditSend.InsertText(szMsg);
		m_reditSend.SetFocus();
	}
	return 0;
}

void CP2PSendMsgDlg::OnLinkResClicked()
{
	CWnd* pParent = GetDesktopWindow();
	CLinkResDlg* pDlg =  CLinkResDlg::NewDlg(this,pParent);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(GetDesktopWindow());

}


void CP2PSendMsgDlg::OnStartUploadCPicAck(const char * data,WORD wRet)
{
	int nFileId = (int)data;
	if(wRet != 0)
		return;

	SF_StartUploadCfileAck(nFileId);
}

void CP2PSendMsgDlg::OnUploadCPicAck(const char * data,WORD wRet)
{
	LPXT_UPLOAD_CPIC_ACK pAck = (LPXT_UPLOAD_CPIC_ACK)data;
	if(	wRet == 0 && 
		pAck->nCid == m_clusterInfo.nFamilyID)
	{
		SF_UploadCfileAck(pAck->nFileId);
	}
}

void CP2PSendMsgDlg::OnDownloadCPicAck(const char * data,WORD wRet)
{
	LPXT_DOWNLOAD_CPIC_ACK pAck = (LPXT_DOWNLOAD_CPIC_ACK)data;
	if(pAck->nCId == m_clusterInfo.nFamilyID)
	{
		SF_DownloadCfileAck(*pAck);
	}
}

void CP2PSendMsgDlg::OnGetFriendBasicInfoAck(const char *data,WORD wRet)
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
void CP2PSendMsgDlg::OnGetNearCommCustRemarkAck(const char *data,WORD wRet)
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
					m_wndAD.Navigate(m_strAdUrl, NULL, NULL, NULL, NULL);

				}
			}
		}
	}
}

void CP2PSendMsgDlg::OnUpdateNearCommCustRemarkAck(const char *data,WORD wRet)
{
	LPXT_UPDATE_NEAR_COMM_CUST_REMARK_ACK ack = (LPXT_UPDATE_NEAR_COMM_CUST_REMARK_ACK)data;
	//assert(ack->ret == 0);
}
#endif
	
typedef  BOOL  (_cdecl *GetIPInfoPrc)(unsigned int nIp,const char ** szZone,const char ** szName);

void CP2PSendMsgDlg::OnAccIp()
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

LRESULT CP2PSendMsgDlg::OnRichEditLinkClilck(WPARAM wParam, LPARAM lParam)
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

			CFileDialog dlg(FALSE,"",(LPCTSTR)strDefaultFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"",this,0);

			if ( dlg.DoModal()==IDOK )
			{
				CString strNewFile = dlg.GetPathName();
				m_reditHistory.SetFileBarTip(file_id,ODT_RECV_FILE,"另存为 "+strNewFile);				

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
		m_reditHistory.SetFileBarTip(file_id,ODT_RECV_FILE,"您同意了接收该文件");
		////////////////////////////////////////////////////////////////////////
		P2PSendFileReplay(file_id,true);
		break;
	case RN_FILE_REFUSE:////拒绝
		m_reditHistory.SetFileBarTip(file_id,ODT_RECV_FILE,"您拒绝接收该文件",RGB(255,0,0));
		P2PSendFileReplay(file_id,false);
		break;
	case RN_FILE_CANCEL:////取消
		{
			//P2PID id;
			//id.fid = file_id;
			//id.uid = m_FriendInfo.id;
			AF_AbortSendFile(file_id);

			m_reditHistory.SetFileBarTip(file_id,ODT_SEND_FILE,"您取消了发送该文件",RGB(255,0,0));

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
	case RN_VEDIO_CANCEL:
		{
			if ( m_pmP2PMode!=pmMultiTalk )
				MM_CancelRequest(0,file_id);

			m_reditHistory.SetMMBarTip(file_id,ODT_SEND_VEDIO,"您取消了视频聊天请求",RGB(255,0,0));
		}
		break;
	case RN_AUDIO_CANCEL:
		{
			if ( m_pmP2PMode!=pmMultiTalk )
				MM_CancelRequest(1,file_id);

			m_reditHistory.SetMMBarTip(file_id,ODT_SEND_AUDIO,"您取消了语音聊天请求",RGB(255,0,0));
		}
		break;
	case RN_VEDIO_SETTING:
		{
			CSAConfig::Show(9);
		}
		break;
	case RN_VEDIO_ACCEPT:
		{
			if ( m_pmP2PMode!=pmMultiTalk && m_pmP2PMode==pmSingleTalk )
			{
				if ( m_bClosePanel )
				{
					OnBnClickedBtnFold();
				}

				m_pmP2PMode = pmVedio;
				//UpdateP2PPanel();

				MM_Reply(0,file_id,true);

				m_bMMWaitTalk = true;

				m_wndVedioPanel.SetStatusText("正在进行网络连接");

			}
			else
			{
				if ( m_pmP2PMode==pmVedio )
				{
					this->m_reditHistory.SetMMBarTip(file_id,ODT_RECV_VEDIO,"当前正在视频聊天，请关闭后再试",RGB(255,0,0));
				}
				if ( m_pmP2PMode==pmAudio )
				{
					this->m_reditHistory.SetMMBarTip(file_id,ODT_RECV_VEDIO,"当前正在语音聊天，请关闭后再试",RGB(255,0,0));
				}
				if ( m_pmP2PMode==pmMultiTalk )
				{
					this->m_reditHistory.SetMMBarTip(file_id,ODT_RECV_VEDIO,"不能在多人聊天状态下视频聊天",RGB(255,0,0));
				}
			}
		}
		break;
	case RN_AUDIO_ACCEPT:
		{
			if ( m_pmP2PMode==pmSingleTalk )
			{
				m_reditHistory.SetMMBarTip(file_id,ODT_RECV_AUDIO,"您同意了对方的语音聊天请求",RGB(255,0,0));

				if ( m_bClosePanel )
				{
					OnBnClickedBtnFold();
				}

				MM_Reply(1,file_id,true);	
				m_pmP2PMode = pmAudio;
				//UpdateP2PPanel();

				m_bMMWaitTalk = true;   //zhangmin
				m_wndAudioPanel.SetStatusText("正在与对方进行网络连接");



			}
			else
			{
				if ( m_pmP2PMode==pmVedio )
				{
					this->m_reditHistory.SetMMBarTip(file_id,ODT_RECV_AUDIO,"当前正在视频聊天，请关闭后再试",RGB(255,0,0));
				}
				if ( m_pmP2PMode==pmAudio )
				{
					this->m_reditHistory.SetMMBarTip(file_id,ODT_RECV_AUDIO,"当前正在语音聊天，请关闭后再试",RGB(255,0,0));
				}
				if ( m_pmP2PMode==pmMultiTalk )
				{
					this->m_reditHistory.SetMMBarTip(file_id,ODT_RECV_AUDIO,"不能在多人聊天状态下语音聊天",RGB(255,0,0));
				}
			}
		}
		break;
	case RN_VEDIO_REFUSE:
		{
			if ( m_pmP2PMode!=pmMultiTalk )
				MM_Reply(0,file_id,false);	
			//m_reditHistory.SetMMBarTip(file_id,ODT_RECV_VEDIO,"您拒绝了对方的视频聊天请求",RGB(255,0,0));
			if(m_wndVedioPanel && m_wndVedioPanel.IsWindowVisible())
			{
				m_wndVedioPanel.ShowWindow(SW_HIDE);
			}
		}
		break;
	case RN_AUDIO_REFUSE:
		{
			if ( m_pmP2PMode!=pmMultiTalk )
				MM_Reply(1,file_id,false);	
			//m_reditHistory.SetMMBarTip(file_id,ODT_RECV_AUDIO,"您拒绝了对方的语音聊天请求",RGB(255,0,0));
			if(m_wndAudioPanel && m_wndAudioPanel.IsWindowVisible())
			{
				m_wndAudioPanel.ShowWindow(SW_HIDE);
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

void CP2PSendMsgDlg::AF_OnRecvSpeedNotify(uint32 id,int nBPS,float fPercent)
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


void CP2PSendMsgDlg::AF_OnSendSpeedNotify(uint32 id,int nBPS,float fPercent)
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

LRESULT CP2PSendMsgDlg::OnRecvFileFailed(WPARAM wParam, LPARAM lParam)
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

void CP2PSendMsgDlg::AF_OnDownloadFailed(uint32  id,FILE_TRAN_ERR err)
{
	SendMessage(WM_FILERECV_FAILED,id,(LPARAM)err);	
}

LRESULT CP2PSendMsgDlg::OnRecvFileOK(WPARAM wParam,LPARAM lParam)
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

			XT_MSG msg;
			msg.msg_type = 0;
			msg.from_id  = g_ImUserInfo.GetId();
			msg.fontSize = 10;
			msg.fontColor = RGB(0,0,255);
			msg.send_time = time(0);

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

void CP2PSendMsgDlg::AF_OnDownloadOK(uint32 id)
{
	SendMessage(WM_FILERECV_OK,id);
}

LRESULT CP2PSendMsgDlg::OnSendFileOK(WPARAM wParam,LPARAM lParam)
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
			msg.send_time = time(0);

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

void CP2PSendMsgDlg::AF_OnSendOK(uint32 id)
{
	SendMessage(WM_FILESEND_OK,id,0);
}


LRESULT CP2PSendMsgDlg::OnSendFileFailed(WPARAM wParam,LPARAM lParam)
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
					m_reditHistory.SetFileBarTip(id ,ODT_SEND_FILE,str,RGB(255,0,0));
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

void CP2PSendMsgDlg::AF_OnSendFailed(uint32 id,FILE_TRAN_ERR err)
{
	SendMessage(WM_FILESEND_FAILED,id,(LPARAM)err);
}

void CP2PSendMsgDlg::AF_OnRecvAbortTranReq(const P2P_ABORT_TRAN_REQ & req)
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
					m_reditHistory.SetFileBarTip(fileId ,ODT_RECV_FILE,strTip,RGB(255,0,0));
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


void CP2PSendMsgDlg::OnP2PTestAck(const char * data, WORD wRet)
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

void CP2PSendMsgDlg::OnBnClickedBtnAudio()
{	
#ifdef _LOCAL_TEST
	InitAudio(m_hWnd);
	StartAudioCap();
	return;
#endif
	
	CRect rc;
	GetWindowRect(rc);

	if ( this->m_pmP2PMode == pmSingleTalk )
	{
		int mmType = 1;//音频

		RefreshFriendStatus();
		m_wndAudioPanel.ShowWindow(SW_SHOW);
		m_wndAudioPanel.MoveWindow(rc.right,rc.top,CP2PAudioPanel::m_WinW,CP2PAudioPanel::m_WinH);
		m_wndAudioPanel.SetWindowText("与" + UserInfoToStr(&m_FriendInfo) + "语音通话");

		m_wndAudioPanel.AudioCall();
		MM_SendRequest(mmType);
	}
	else
	{
		CString strTip;

		switch(m_pmP2PMode)
		{
		case pmMultiTalk:
			strTip = "对不起，不支持多人语音聊天";
			break;
		case pmVedio:
			strTip = "您正在和对方视频聊天,请退出该动作再试";
			break;
		case pmAudio:
			strTip = "正在进行语音聊天";
			break;
		}

		this->AddMsgTip(strTip,COLOR_MSGTIP);
	}
}

void CP2PSendMsgDlg::OnBnClickedBtnVedio()
{
	
	CRect rc;
	GetWindowRect(rc);

	if ( this->m_pmP2PMode==pmSingleTalk )
	{
		int mmType = 0;//视频
		RefreshFriendStatus();
		CRect rc;
		GetWindowRect(rc);

		m_wndVedioPanel.ShowWindow(SW_SHOW);
		m_wndVedioPanel.MoveWindow(rc.right,rc.top,CP2PVedioPanel::m_WinW,CP2PVedioPanel::m_WinH);
		m_wndVedioPanel.VideoCall();
		m_wndVedioPanel.SetWindowText("与" + UserInfoToStr(&m_FriendInfo) + "视频通话");
		MM_SendRequest(mmType);
	}
	else
	{
		CString strTip;

		switch(m_pmP2PMode)
		{
		case pmMultiTalk:
			strTip = "对不起，不支持多人视频聊天";
			break;
		case pmAudio:
			strTip = "您正在和对方语音聊天,请退出该动作再试";
			break;
		case pmVedio:
			strTip = "正在进行视频聊天";
			break;
		}

		this->AddMsgTip(strTip,COLOR_MSGTIP);
	}
}


void CP2PSendMsgDlg::OnBnClickedButtonHistory()
{
	m_bShowHistory = !m_bShowHistory;

	if (m_bShowHistory)
	{
		m_wndPopupHistory.LoadAllMsg();
	}

	UpdateP2PPanel();
}

LRESULT CP2PSendMsgDlg::OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam)
{
	m_bShowHistory = !m_bShowHistory;

	if (m_bShowHistory)
	{
		m_wndPopupHistory.LoadAllMsg();
	}

	UpdateP2PPanel();
	return S_OK;
}

LRESULT CP2PSendMsgDlg::OnCloseAll(WPARAM wParam,LPARAM lParam)
{
	g_pMessageHandleWnd->CloseAllP2PSendMsgDlg();
	return S_OK;
}

LRESULT CP2PSendMsgDlg::OnCloseOne(WPARAM wParam,LPARAM lParam)
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
	}
#endif
	if(g_pDuiCounsel != NULL)
		g_pDuiCounsel->m_bIsUpdate = TRUE;
	DestroyWindow();

	return S_OK;
}

void CP2PSendMsgDlg::OnBnClickedButtonShake()
{
	WORD nVK = 0;
	g_LocalData.GetShakeWinConfig(nVK);
	if(nVK == 1)////本地支持振屏
	{
		int nShakeTime = time(0);

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
		/*	SetTimer(TIMER_WIN_SHAKE,20,NULL);
			m_nWinShakeCnt = 0;
			m_nLastShakeTime = nShakeTime;*/
		}	

	}
	else////本地不支持振屏
	{

		XT_MSG msg;
		MM_InitXtMsg(msg);

		msg.data_type = XT_MSG::WIN_SHAKE;
		CSendPacket sp(msg.data);
		strcpy(msg.from_nickname,m_FriendInfo.nickname);

		CString text = "不支持振屏~";

		strcpy(msg.data,text);
		msg.data_len = strlen(text)+1;
		AddTalkMsg(msg,false);
		//g_pProtocol->SendP2PMsg(msg);

		///////本地抖动	
		//SetTimer(TIMER_WIN_SHAKE,20,NULL);
		//m_nWinShakeCnt = 0;
	}
}

void CP2PSendMsgDlg::OnBnClickedBtnHotkey()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	m_btnSend.GetWindowRect(&rc);

	CPoint p;
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

	id=menu.Track(p.x,p.y,GetSafeHwnd(),true);

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


LRESULT  CP2PSendMsgDlg::OnSnapShot(WPARAM wParam,LPARAM lParam)
{
	OnScreenCopy();
	return S_OK;
}


void CP2PSendMsgDlg::OnEnter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_ENTER;
}


void CP2PSendMsgDlg::OnCtrlenter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_CTRLENTER;
}

// 显示或隐藏评分按钮
void CP2PSendMsgDlg::ShowOrHideScoreBtn(BOOL bShow)
{
	if (IsWindow(GetSafeHwnd()))
	{
		m_wndToolBar.ShowOrHideScoreBtn(bShow);
	}
}

// “推送评价”按钮
void CP2PSendMsgDlg::OnBnClickedButtonScore()
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
void CP2PSendMsgDlg::OnCommandFastWordMenu(int id)
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

		if ( text.GetLength()>1300 )
		{
			//MessageBox("最多输入900个英文字符或450个汉字!");
			//CMessageBox::Prompt("最多输入1300个英文字符或650个汉字!");
			AddMsgTip("你发送消息内容超长，请分条发送。");
			return ;
		}

		if ( FindForbiddenWords(text) )
		{
			m_reditSend.SetFocus();
			AddMsgTip("你发送的消息中有限制用语，消息发送失败！");
			return ;
		}

		if ( text.GetLength()==0 )
		{
			m_reditSend.SetFocus();
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

			//CRecentListCtrl *pRecentList=(CRecentListCtrl *)g_pRecentListCtrl;
			//pRecentList->AddUser(&m_FriendInfo);

			SF_SetTransMode(type);

			strcpy(msgShow.from_nickname,g_ImUserInfo.nickname);
			g_LocalData.SaveTalkMsg(msgShow,m_FriendInfo.GetId());
		}
		else
		{
			g_pProtocol->SendClusterMsg(msg);
			SaveClusterMsg(msgShow);
		}

		RegUserOperation(0x00c4);

		m_reditSend.SetFocus();
	}	
}

// 快捷短语按钮消息相应
void CP2PSendMsgDlg::OnBnClickedFastword()
{
	CRect rc;
	m_wndToolBar.m_btnFastword.GetWindowRect(&rc);

	CFlatPopupMenu menu;
	menu.Create(hInstance);

	g_LocalData.GetFastWord(m_vecFastWord);
	for(int i = 0; i < m_vecFastWord.size(); ++i)
	{
		menu.AppendItem(0,m_vecFastWord[i].c_str(),i+1);
	}
	menu.AppendItem(CFlatPopupMenu::itemSeparator,NULL,-1);
	menu.AppendItem(0,"添加/修改快捷回复", m_vecFastWord.size()+1);

	CPoint p;
	int	id;

	// track it
	p.x=rc.left;
	p.y=rc.bottom+1;
	id=menu.Track(p.x,p.y,GetSafeHwnd(),true);

	// 消息相应
	OnCommandFastWordMenu(id-1);
}

#ifdef _BUSINESS_VESION
// 账号挂起
void CP2PSendMsgDlg::OnBnClickedBtnHangup()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// 弹出菜单
	CRect rcBtn;
	m_btnHangup.GetWindowRect(rcBtn);

	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(hInstance);
	menu.AppendItem(0, "不自动挂起", 1, pMainFrame->GetHangupMenuIndex() == 1 ? 0 : -1);
	menu.AppendItem(0, "20人挂起", 2, pMainFrame->GetHangupMenuIndex() == 2 ? 0 : -1);
	menu.AppendItem(0, "50人挂起", 3, pMainFrame->GetHangupMenuIndex() == 3 ? 0 : -1);
	menu.AppendItem(0, "100人挂起", 4, pMainFrame->GetHangupMenuIndex() == 4 ? 0 : -1);
	menu.AppendItem(CFlatPopupMenu::itemSeparator,NULL,-1);
	menu.AppendItem(0,"自定义挂起", 5, pMainFrame->GetHangupMenuIndex() == 5 ? 0 : -1);

	CPoint p;
	int	id;

	// track it
	p.x=rcBtn.left;
	p.y=rcBtn.bottom+1;
	id=menu.Track(p.x,p.y,GetSafeHwnd(),true);

	switch(id)
	{
	case 1:
		{
			pMainFrame->SetHangupMenuIndex(1);
			pMainFrame->SetHangupStatusNeededSessions(-1);
			pMainFrame->Hangup();
		}
		break;
	case 2:
		{
			pMainFrame->SetHangupMenuIndex(2);
			pMainFrame->SetHangupStatusNeededSessions(20);
			pMainFrame->Hangup();
		}
		break;
	case 3:
		{
			pMainFrame->SetHangupMenuIndex(3);
			pMainFrame->SetHangupStatusNeededSessions(50);
			pMainFrame->Hangup();
		}
		break;
	case 4:
		{
			pMainFrame->SetHangupMenuIndex(4);
			pMainFrame->SetHangupStatusNeededSessions(100);
			pMainFrame->Hangup();
		}
		break;
	case 5:
		{	// 弹出自定义窗口
			pMainFrame->SetHangupMenuIndex(5);
			int nNum = pMainFrame->GetHangupStatusNeededSessions();
			CHangupSessionNumSettingDlg dlg(nNum, this);
			if(IDOK == dlg.DoModal())
			{
				nNum = dlg.GetNum();
				pMainFrame->SetHangupStatusNeededSessions(nNum);
				pMainFrame->Hangup();
			}
		}
		break;
	}
}
#endif

// 处理鼠标移动消息，当鼠标移动到头像区域时，弹出好友资料窗口
void CP2PSendMsgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CXSkinDlg::OnMouseMove(nFlags, point);

	CRect rc;
	m_ctrlFaceIcon.GetWindowRect(rc);
	ClientToScreen(&point);
	if(rc.PtInRect(point))
	{
		SetTimer(TIMER_SHOW_USERCARD, 500, NULL);
	}
}

void CP2PSendMsgDlg::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CClusterMemberSelectDlg dlg(&m_clusterInfo,this);
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
void CP2PSendMsgDlg::NotifyServer_OpenedMsgDlg()
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

// 打开好友聊天窗口的其他链接
void CP2PSendMsgDlg::BeforeNavigate2WebAd(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	CString strUrl = (char *)(_bstr_t(URL)); 
	if(strUrl.Find(RIGHT_URL_SELLER) == -1 && strUrl.Find(RIGHT_URL) == -1 && strUrl.Find("http://im.feiniu.com") == -1)
	{
		*Cancel =TRUE;
		ShellExecute(NULL, "open", strUrl, NULL, NULL, SW_SHOWNORMAL);
	}
#ifdef _BUSINESS_VESION
	//与右侧页面交互
	else if(strUrl.Find("http://im.feiniu.com") == 0)
	{
		*Cancel =TRUE;
		int nBeg = strUrl.Find("comment=");
		if(nBeg > 0)
		{
			nBeg += strlen("comment=");
			int nEnd = strUrl.Find("&",nBeg);
			if(nEnd == -1)
				nEnd = strUrl.GetLength();
			if(nEnd > nBeg)
			{
				m_strNewComnCustRemarkClient = strUrl.Mid(nBeg , nEnd - nBeg);
			}

		}
	}
#endif
}

void CP2PSendMsgDlg::NavigateComplete2WebAd(LPDISPATCH pDisp, VARIANT* URL)
{
	CString strUrl = (char *)(_bstr_t(URL)); 
	if(strUrl.Find(RIGHT_URL_SELLER) != -1 || strUrl.Find(RIGHT_URL) != -1)
	{
		m_wndAD.Refresh();
	}
}

void CP2PSendMsgDlg::OnMove(int x, int y)
{
	__super::OnMove(x, y);
	if( m_wndAudioPanel && m_wndAudioPanel.IsWindowVisible())
	{
		CRect rc;
		GetWindowRect(&rc);
		CRect rcAudioPanel;
		m_wndAudioPanel.GetWindowRect(&rcAudioPanel);
		m_wndAudioPanel.MoveWindow(rc.right+1, rc.top, rcAudioPanel.Width(), rcAudioPanel.Height());
	}
	if( m_wndVedioPanel && m_wndVedioPanel.IsWindowVisible())
	{
		CRect rc;
		GetWindowRect(&rc);
		CRect rcVedioPanel;
		m_wndVedioPanel.GetWindowRect(&rcVedioPanel);
		m_wndVedioPanel.MoveWindow(rc.right+1, rc.top, rcVedioPanel.Width(), rcVedioPanel.Height());
	}
// 	if(g_pDuiCounsel != NULL && m_FriendInfo.GetId() >= 5000000)
// 	{
// 		if(g_pDuiCounsel->m_pDlgSelectP2PSendMsgDlg == this || g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1)
// 		{
// 			CRect rc;
// 			GetWindowRect(&rc);
// 			CRect rcCounsel;
// 			::GetWindowRect(*g_pDuiCounsel, &rcCounsel);
// 			g_pDuiCounsel->ShowWindow(true);
// 			::SetWindowPos(*g_pDuiCounsel, *this, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
// 			::MoveWindow(*g_pDuiCounsel, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
// 		}
// 	}
}
//
//////A向B发文件，由服务端转发到B中响应
//void  CP2PSendMsgDlg::OnRecvFileAck(const char *data,WORD wRet)
//{
//
//	LPSEND_FILE_PACK_ACK pAck = NULL;
//	pAck = (LPSEND_FILE_PACK_ACK)malloc(sizeof(SEND_FILE_PACK_ACK));
//	memset(pAck,0,sizeof(SEND_FILE_PACK_ACK));
//	pAck = (LPSEND_FILE_PACK_ACK)data;
//	if ( wRet == 0)
//	{
//
//		if ( pAck)
//		{
//			if(pAck->flag == 0x02)////传输的文件内容
//			{
//				extern DWORD g_dwRecvID;
//			   if(0 == ::PostThreadMessage(g_dwRecvID, WM_P2P_RECV, CMD_FILE_ACK, (LPARAM)pAck))////；发送消息
//			   {
//				 Sleep(30);
//			   }
//			}
//		}
//	}
//}

////服务端返回给B端的确认
void  CP2PSendMsgDlg::OnRecvServerAck(const char *data,WORD wRet)
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

void CP2PSendMsgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	if(g_pDuiCounsel != NULL && m_FriendInfo.GetId() >= 5000000)
// 	{
// 		if(g_pDuiCounsel->m_pDlgSelectP2PSendMsgDlg == this || g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1)
// 		{
// 			CRect rc;
// 			GetWindowRect(&rc);
// 			::SetWindowPos(*g_pDuiCounsel, *this, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
// 			::MoveWindow(*g_pDuiCounsel, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
// 		}
// 	}
	/*CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if ( !m_rcClose.PtInRect(pt) && !m_rcMin.PtInRect(pt) && !m_rcMax.PtInRect(pt) && !m_rcHelp.PtInRect(pt))
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));*/
	__super::OnLButtonDown(nFlags, point);
}


void CP2PSendMsgDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	if(g_pDuiCounsel != NULL && m_FriendInfo.GetId() >= 5000000)
// 	{
// 		if(g_pDuiCounsel->m_pDlgSelectP2PSendMsgDlg == this || g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1)
// 		{
// 			CRect rc;
// 			GetWindowRect(&rc);
// 			::SetWindowPos(*g_pDuiCounsel, *this, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(),SWP_SHOWWINDOW);
// 			::MoveWindow(*g_pDuiCounsel, rc.left - /*rcCounsel.Width()*/210, rc.top, /*rcCounsel.Width()*/210, rc.Height(), TRUE);
// 		}
// 	}
	__super::OnNcLButtonDown(nHitTest, point);
}

#ifdef _BUSINESS_VESION
// 客服转接
void CP2PSendMsgDlg::OnBnClickedBtnCustserTran()
{
	CCustSerTranDlg duiFrame;
	duiFrame.Create(GetSafeHwnd(), _T("选择转移对象"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow();
	if(0 == duiFrame.ShowModal())
	{	// 点确认
		uint32 destid = duiFrame.m_nSelFriendID;
		FRIEND_INFO *pDestFriend = ::FindFriend(destid);
		if(destid >= 0 && pDestFriend != NULL)
		{
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

			msg.send_time   = time(0);

			msg.to_id		= m_FriendInfo.GetId();
			msg.dest_ip		= m_FriendInfo.ip;
			msg.dest_port	= m_FriendInfo.port;
			msg.msg_type    = 0;

			msg.data_type = XT_MSG::CUST_SER_TRAN_REQ;
			strcpy(msg.from_nickname, g_ImUserInfo.nickname);

			Json::FastWriter jWriter;
			Json::Value jvSellerInfo;
			jvSellerInfo["switchStatus"] = 1;
			jvSellerInfo["toSubMerchantID"] = destid;
			jvSellerInfo["fromSubMerchantID"] = g_ImUserInfo.GetId();
			jvSellerInfo["toSubMerchantName"] = pDestFriend->nickname;
			string strMsg = jWriter.write(jvSellerInfo);

			strcpy(msg.data, strMsg.c_str());
			msg.data_len = strMsg.length() + 1;

			g_pProtocol->SendP2PMsg(msg);

			// 发送转接请求
			XT_CUSTSER_TRAN_REQ req;
			req.uSid = g_ImUserInfo.GetId();
			req.uDid = destid;
			strncpy(req.szOrderId, duiFrame.m_strOrderID.c_str(), 63);
			strncpy(req.szTranReason, duiFrame.m_strReason.c_str(), 511);
			req.uCustomerId = m_nFriendId;

			g_pProtocol->SendReq(req);
		}
	}
}

// 收到客服转接确认包
void CP2PSendMsgDlg::OnRecvCustSerTranAckInd(const char * data, WORD wRet)
{
	XT_CUSTSER_TRAN_ACK *pAck = (XT_CUSTSER_TRAN_ACK *)data;
	if(pAck->uCustomerId == m_FriendInfo.GetId())
	{
		// 显示转接成功消息
		XT_FONT xf;
		xf.fontColor=RGB(254,154,105);

		m_reditHistory.SetSel(-1,-1);
		m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
		m_reditHistory.SetStartIndent(80);
		m_reditHistory.InsertText("\n----转接成功\n");
	}
}

void CP2PSendMsgDlg::AddTranMsg(uint32 fromId, const char *pOrderId, const char *pReason)
{
	FRIEND_INFO *pFriend = ::FindFriend(fromId);
	if(pFriend == NULL)
	{
		return;
	}

	// 历史消息记录中打印转接消息
	XT_FONT xf;
	xf.fontColor=RGB(254,154,105);

	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.SetStartIndent(80);
	CString strFrom;
	strFrom.Format("\n----转自\"%s\"\n", pFriend->nickname);
	m_reditHistory.InsertText(strFrom);

	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	CString strOrder;
	strOrder.Format("订单编号：%s\n", pOrderId);
	m_reditHistory.InsertText(strOrder);

	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	CString strReason;
	strReason.Format("转接原因：%s\n", pReason);
	m_reditHistory.InsertText(strReason);

	m_reditHistory.SetSel(-1,-1);
}
#endif

LRESULT CP2PSendMsgDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(CXSkinDlg::OnNcHitTest(point) == HTMENU)
		return HTCAPTION;
	return CXSkinDlg::OnNcHitTest(point);
}

LRESULT CP2PSendMsgDlg::OnKeyDownReturn(WPARAM wParam,LPARAM lParam)
{
	return 2;
}

void CP2PSendMsgDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(  nChar  )  
	{  
	case 0x0D:
		{
			HOTKEY_CONFIG *cfg=g_LocalData.GetHotKeyConfig();

			if ( cfg->sendMsgKey==SMK_ENTER  )
			{
				if ( !KEYDOWN(VK_CONTROL) )
					OnBnClickedBtnSend();
				else
				{
					if (m_reditSend.IsFocused() )
						m_reditSend.InsertText("\n");
				}
			}
			else if(cfg->sendMsgKey==SMK_CTRLENTER )
			{
				if  ( KEYDOWN(VK_CONTROL) )
					OnBnClickedBtnSend();
				else
				{
					if (m_reditSend.IsFocused() )
						m_reditSend.InsertText("\n");
				}
			}
		}
		return;
		break;
	case VK_ESCAPE:
		PostMessage(WM_CLOSE,0,0);
		return;
	default:
		;
	}  
	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}
