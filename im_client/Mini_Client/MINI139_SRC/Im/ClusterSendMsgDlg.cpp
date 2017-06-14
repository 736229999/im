// im\SendMessageDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "..\imclient.h"
#include ".\clustersendmsgdlg.h"
#include "EmotionDlg.h"
#include "msgdlg.h"
#include "..\FriendInfoDlg.h"
#include "MessageHandleWnd.h"
#include "ClustersettingDlg.h"
#include "saconfig.h"
#include "xtpacket.h"
#include "imagefiledialog.h"
#include "localdata.h"
#include "screencopydlg.h"
#include "magicfacedlg.h"
#include "webprocess.h"
#include "MsgObj.h"
#include "LinkResDlg.h"
#include "..\MainFrame.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "../SystemSettingDlg.h"
//#include "..\..\..\..\im_client\HmgUploadPicture\stdafx.h"
#include "ScreenCtrl.h"
#include "..\DuiMfRecentList.h"
#include "..\CSPlatDlg.h"

// 上传图片到图片空间
extern BOOL upLoadPic(const char* szPic, string &strURL);

HWND  g_hClusterSendMSG = NULL;
const DWORD	CLR_ACTIONS = RGB(0,84,166);	// 聊天动作字符颜色：黑色
const DWORD CLR_OBJECT  = RGB(255,0,0);	    // 聊天对象字符颜色

/*
#define DOWNLOAD_TIMEROUT	240*1000
#define TIMER_DOWNLOADOBJ	2001

#define TIMER_DWN_CPIC		3001
#define TIME_DWN_CPIC		1000
*/
enum  VIEW_CLUSTER
{
     VIEW_CLUSTER_INFO    = 1,
	 VIEW_CLUSTER_MEMBER  = 2,
     VIEW_CLUSTER_PROFILE = 3,
	 VIEW_CLUSTER_SECURITY= 4
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//聊天框宽度
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

//// 解析出群聊天信息中的包含的动作信息
//// 动作定义表达式{\action0[]action1[\object]action2\}
//BOOL ParseMsg(const char* szMsg,
//			  CString& sMsg,
//			  CString& sObj,
//			  CString& sAction0,
//			  CString& sAction1,
//			  CString& sAction2)
//{
//	int nLen=0;
//	const char* p=szMsg;
//	const char* pWord;
//	char* pData;
//
//	if(*p==0)
//		return FALSE;
//	// 动作定义必须以符号"{\"开始,以"\}"结束
//	if(*p=='{'&& *(p+1)=='\\')
//	{
//		// 有动作信息
//		p+=2;
//		if(*p==0)
//		{
//			return FALSE;
//		}
//		// action0的字符串开始
//		pWord=p;
//		while(*p)
//		{
//			// 获得发话人位置
//			if(*p=='[' && *(p+1)==']')
//			{
//				// action0字符串结束
//				nLen=p-pWord;
//				pData=new char[nLen+1];
//				strncpy(pData,pWord,nLen);
//				*(pData+nLen)=0;
//				sAction0=pData;
//				p+=2;
//				// action1字符串开始
//				pWord=p;
//				break;
//			}
//			else
//			{
//				p++;
//			}
//		}
//		// 获得action1
//		while(*p)
//		{
//			if(*p=='[' && *(p+1)=='\\')
//			{
//				nLen=p-pWord;
//				pData=new char[nLen+1];
//				strncpy(pData,pWord,nLen);
//				*(pData+nLen)=0;
//				sAction1=pData;
//				// action1字符串结束
//				p+=2;
//				// obj字符串开始
//				pWord=p;
//				break;
//			}
//			else
//			{
//				p++;
//			}
//		}
//		// 获取object
//		while(*p)
//		{
//			if(*p==']')
//			{
//				// obj字符串结束
//				nLen=p-pWord;
//				pData=new char[nLen+1];
//				strncpy(pData,pWord,nLen);
//				*(pData+nLen)=0;
//				sObj=pData;
//				p+=1;
//				// action2字符串开始
//				pWord=p;
//				break;
//			}
//			else
//			{
//				p++;
//			}
//		}
//		// 获取action2
//		while(*p)
//		{
//			if(*p=='\\' && *(p+1)=='}')
//			{
//				nLen=p-pWord;
//				pData=new char[nLen+1];
//				strncpy(pData,pWord,nLen);
//				*(pData+nLen)=0;
//				sAction2=pData;
//				p+=2;
//				// 消息字符串正式开始
//				sMsg=p;
//				break;
//			}
//			else
//			{
//				p++;
//			}
//		}
//		return TRUE;
//	}
//	else
//	{
//		// 没有包含动作信息
//		return FALSE;
//	}
//}
//
//#ifdef _DEBUG
//void testParseMsg()
//{
//	char * szMsgs[]={
//		"{\\[]aabb\\}",
//			"{\\可爱的[]飞起一脚踢到[\\小明]的头上\\}哈哈",
//			"abc",
//
//	};
//	CString sMsg;
//	CString sObj;
//	CString sAction0;
//	CString sAction1;
//	CString sAction2;
//
//	for(int i=0;i<sizeof(szMsgs)/sizeof(char*);i++)
//	{
//		if(ParseMsg(szMsgs[i],
//			sMsg,
//			sObj,
//			sAction0,
//			sAction1,
//			sAction2)
//			)
//		{
//#ifdef _std_out	
//			cout<<(LPCTSTR)sAction0
//				<<"[]"
//				<<(LPCTSTR)sAction1
//				<<"["
//				<<(LPCTSTR)sObj
//				<<"]"
//				<<(LPCTSTR)sAction2
//				<<endl
//				<<"msg="
//				<<(LPCTSTR)sMsg
//				<<endl;
//
//		}
//		else
//		{
//			cout<<szMsgs[i]<<endl;
//#endif
//		}
//
//		sMsg="";
//		sObj="";
//		sAction0="";
//		sAction1="";
//		sAction2="";
//	}
//
//
//}
//#endif



IMPLEMENT_DYNAMIC(CClusterSendMsgDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CClusterSendMsgDlg)


CClusterSendMsgDlg::CClusterSendMsgDlg(CLUSTER_INFO *pClusterInfo, CWnd *pParent)
: CXSkinDlg(CClusterSendMsgDlg::IDD, pParent),
m_wndTalkToolBar(&m_reditSend, &g_ImFont, this),
m_wndToolBar(this),
m_pClusterInfo(pClusterInfo),
m_wndClusterPanel(m_pClusterInfo, pParent)
{
	//////设置背景颜色//////////////
	m_colorBackground = CHAT_BKG_COLOR;
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	m_nSplitterPos = 130;
	m_wndPopupHistory.SetClusterInfo(m_pClusterInfo);
	//memcpy(&m_clusterInfo, pClusterInfo ,sizeof(CLUSTER_INFO));

	m_hIcon        = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_hAccel	   = LoadAccelerators(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_SENDMSG_HOTKEY));

	m_hSmallIcon = ::ResLoadIcon(IDR_MAINFRAME1);
	m_hBigIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

	m_dwProgressBar = (DWORD)-1;
	m_uPopupHistoryWidth = 0;
	m_bPreShowHistory = false;
	m_bShowHistory = false;

	//m_bAction = FALSE;
	g_LocalData.GetFastWord(m_vecFastWord);
}

CClusterSendMsgDlg::~CClusterSendMsgDlg()
{
	DestroyIcon(m_hSmallIcon);
	DestroyIcon(m_hBigIcon);

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
}

void CClusterSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RE_HISTORY, m_reditHistory);
	DDX_Control(pDX, IDC_RE_SEND, m_reditSend);
	//	DDX_Control(pDX, IDC_STATIC_TITLE, m_captionBar);
	DDX_Control(pDX, IDC_BTN_HISTORY, m_btnHistory);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_STATIC_HIDE1, m_wndHide);
	DDX_Control(pDX, IDC_BTN_DISCUSS, m_btnDiscuss);
	DDX_Control(pDX, IDC_BTN_CLUSTER_ALUBM, m_btnAlbum);
	DDX_Control(pDX, IDC_BTN_CLUSTER_HOME, m_btnHome);
	DDX_Control(pDX, IDC_BTN_CFG, m_btnClusterCfg);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrlFaceIcon);
}


BEGIN_MESSAGE_MAP(CClusterSendMsgDlg, CXSkinDlg)
	ON_COMMAND(IDC_BUTTON_FACE,OnFaceIconClicked)
	//ON_COMMAND(IDC_BUTTON_C,OnBnClickedBtnColor)
	//ON_COMMAND(IDC_BUTTON_I,OnFontItalicClicked)
	ON_COMMAND(IDC_BUTTON_PIC,OnSendPicClicked)
	//ON_COMMAND(IDC_BUTTON_U,OnFontUnderLineClicked)
	//ON_COMMAND(IDC_BUTTON_B,OnFontBoldClicked)
	ON_BN_CLICKED(IDC_BUTTON_MAGIC,OnMagicFace)
	ON_BN_CLICKED(IDC_BUTTON_SCRN,OnScreenCopy)
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnButtonLink)
	ON_COMMAND(IDC_BUTTON_FONT,OnBnClickedFontBar)
	ON_COMMAND(IDC_BUTTON_ACTION,OnBnClickedAction)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY, OnBtnMsgLog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SEND, OnBnClickedBtnSendEx)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_HISTORY, OnBnClickedBtnHistory)
	ON_MESSAGE(WM_EMOTIONSEL_NOTIFY,OnEmotionSelNotify)
	ON_MESSAGE(WM_MAGICFACE_NOTIFY,OnMagicFaceNotify)
	ON_MESSAGE(WM_MAGICFACE_CLICK,OnMagicFaceClick)
	ON_MESSAGE(WM_THREAD_NOTIFY, OnThreadNotify)
	ON_MESSAGE(WM_DOWNLOADFACE,OnStartDownloadFace)
	ON_MESSAGE(WM_DOWNLOADFACE_OK,OnDownloadFaceOK)
	ON_MESSAGE(WM_DOWNLOADPROCESS,OnDownloadProcess)
	ON_MESSAGE(WM_INSERTMSG,OnInsertMsg)
	ON_MESSAGE(WM_USERLINK_CLICK,OnUserLinkClick)
	ON_MESSAGE(WM_USERLINK_RBCLICK,OnUserLinkRBClick)
	//ON_MESSAGE(WM_SET_HEAD_TITLE,OnSetHeadTitle)
	ON_MESSAGE(WM_SNAPSHOT,OnSnapShot)
		
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_XT_MESSAGE()
	ON_STN_CLICKED(IDC_STATIC_TITLE, OnStnClickedStaticTitle)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	SET_HMG_SCROLLBAR(IDC_RE_HISTORY)
	SET_HMG_SCROLLBAR(IDC_RE_SEND)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BTN_DISCUSS, OnBnClickedBtnDiscuss)
	ON_BN_CLICKED(IDC_BTN_CLUSTER_ALUBM, OnBnClickedBtnClusterAlubm)
	ON_BN_CLICKED(IDC_BTN_CLUSTER_HOME, OnBnClickedBtnClusterHome)
	ON_STN_CLICKED(IDC_IMAGE_TITLE, OnStnClickedImageTitle)
	
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_CFG, &CClusterSendMsgDlg::OnBnClickedBtnCfg)
	ON_COMMAND(ID_MENU_VIEW_CLUSTER_INFO, &CClusterSendMsgDlg::OnMenuViewClusterInfo)
	ON_COMMAND(ID_MENU_CLUSTER_PROFILE, &CClusterSendMsgDlg::OnMenuClusterProfile)
	ON_COMMAND(ID_MENU_CLUSTER_EXIT, &CClusterSendMsgDlg::OnMenuClusterExit)
	ON_COMMAND(ID_MENU_REC_NOTICE, &CClusterSendMsgDlg::OnMenuRecNotice)
	ON_COMMAND(ID_MENU_REC_ONLY, &CClusterSendMsgDlg::OnMenuRecOnly)
	ON_COMMAND(ID_MENU_UNREAD_SHOW, &CClusterSendMsgDlg::OnMenuUnreadShow)
	ON_COMMAND(ID_MENU_REJECT_MSG, &CClusterSendMsgDlg::OnMenuRejectMsg)
	ON_BN_CLICKED(IDC_BUTTON_FASTWORD, OnBnClickedFastword)	
	ON_MESSAGE(WM_CLOSETABCTRL_CLOSE,OnCloseTabCtrlClose)
	ON_MESSAGE(WM_KEYDOWN_RETURN,OnKeyDownReturn)
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterSendMsgDlg)
	// 群成员变动通知
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	// 群成员变动通知
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	// 获取用户资料
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGetAck)

	//	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnCMemberStatusInd)
	// 修改用户资料
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK,OnUserInfoModified)
	// 状态改变
	ON_XT_COMMAND(CMD_STATUS_MOD_ACK,OnStatusModAck)
	// 登出
	ON_XT_COMMAND(CMD_LOGOUT,OnLoginOut)
	// 获取临时群成员列表
	ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnClusterMembersAck)
	// 开始上传群图片数据应答
	ON_XT_COMMAND(CMD_START_SEND_CPIC_ACK,OnStartUploadCPicAck)
	// 上传群图片数据
	ON_XT_COMMAND(CMD_SEND_CPIC_ACK,OnUploadCPicAck)
	// 下载群图片数据
	ON_XT_COMMAND(CMD_GET_CPIC_ACK,OnDownloadCPicAck)

	//群主修改群信息后，服务器返回信息
	ON_XT_COMMAND(CMD_SET_CINFO_ACK,OnSetClusterInfoAck)	
	
	//群名片修改后服务器返回信息
    ON_XT_COMMAND(CMD_SET_PROFILE_NAME_ACK, OnSetCMProfileAck)

	//获取群名片信息
	ON_XT_COMMAND(CMD_GET_PROFILE_NAME_ACK, OnGetCMProfileAck)
	
END_XT_MESSAGE_MAP()
// CClusterSendMsgDlg 消息处理程序


BOOL CClusterSendMsgDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();
	m_reditSend.SetLimitWordSize(400);//发送框限制输入400个字
	g_hClusterSendMSG = GetSafeHwnd();
	EnableHelp(true);
	g_pProtocol->RegistCmd(CMD_CMEMBERS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SEND_CPIC_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CPIC_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_START_SEND_CPIC_ACK,m_hWnd);
    g_pProtocol->RegistCmd(CMD_SET_PROFILE_NAME_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_PROFILE_NAME_ACK,m_hWnd);/////向服务器请求获取群名片

	//CLUSTER_INFO * pRealCluster = FindClusterInfo(this->m_clusterInfo.nFamilyID);
	m_pClusterInfo->nMsgNum = 0;
	//显示头像
	//CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&m_clusterInfo);

	//显示群名

	GetDlgItem(IDC_STATIC_USERINFO)->SetFont(&g_font16B); 
	CString strFamilyID;
	strFamilyID.Format("%d", m_pClusterInfo->nFamilyID);
	SetDlgItemText(IDC_STATIC_USERINFO, CString(m_pClusterInfo->szFamilyName) + " [群号：" + 
		strFamilyID + "]");

	//显示群描述
	GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
	SetDlgItemText(IDC_STATIC_SIGN, m_pClusterInfo->szFamilyDesc);

	/*f ( pRealCluster )
	{
		m_clusterInfo = *pRealCluster;*/

		//如果成员还没初始化则向服务器请求
		if( !m_pClusterInfo->bMemberInit )
		{
			XT_GET_CMEMBERS_REQ req;
			req.nFamilyID   = m_pClusterInfo->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			req.req_count = 0;
			req.online_type  = 0;
			req.userstatus   = g_ImUserInfo.status;
			g_pProtocol->SendReq(req);
		}
		
		// 发送时钟消息，获取最新的群命名
		SetTimer(0x03, 300, NULL);
	//}

	SetDlgStyle(ALL);

	m_reditHistory.SetEmotionImageList(&g_imgEmotionList);
	m_reditSend.SetEmotionImageList(&g_imgEmotionList);

	m_reditSend.SetXFont(g_ImFont);

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_SPLITTER1);
	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER1);
	m_wndSplitter1.ShowWindow(SW_HIDE);

	pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(IDC_STATIC_LINE);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	m_wndClusterPanel.Create(CClusterPanel::IDD,this);
	m_wndClusterPanel.MoveWindow(rc);
	m_wndClusterPanel.ShowWindow(SW_SHOW);

	//////消息记录////////////////////////////////
	m_wndPopupHistory.Create(CPopupHistoryDlg::IDD,this);
	m_wndPopupHistory.ShowWindow(SW_HIDE);

	//获取右侧聊天记录窗口宽度
	CRect rcPopupHistory;
	m_wndPopupHistory.GetWindowRect(rcPopupHistory);
	m_uPopupHistoryWidth = rcPopupHistory.Width();
	///////////////////////////////////////////////

	// 字体工具栏
	m_wndTalkToolBar.Create(CP2PTalkToolBar::IDD,this);
	m_wndTalkToolBar.MoveWindow(rc);
	m_wndTalkToolBar.ShowWindow(SW_HIDE);
	// 动作工具栏
	m_wndActionToolBar.Create(CSendMsgActionToolBar::IDD,this);
	m_wndActionToolBar.MoveWindow(rc);
	m_wndActionToolBar.ShowWindow(SW_HIDE);
	
	
	m_wndToolBar.Create(CP2PToolBar::IDD,this);
    m_wndToolBar.MoveWindow(rc);
	m_wndToolBar.ShowWindow(SW_SHOW);
	m_wndToolBar.ShowActionButton(TRUE);

	pWnd->ShowWindow(SW_HIDE);

	// 设置群主页按钮风格
	m_btnHome.SetTextColor(RGB(0,84,166));
	m_btnHome.SetBitmap(IDB_BTN_VISIT,IDB_BTN_VISIT,RGB(0,255,0));
	m_btnHome.SetFlatStyle(CFlatButton::FS_OFFICE,CFlatButton::AS_BOTTOM);
	// 设置群讨论按钮风格
	m_btnDiscuss.SetTextColor(RGB(0,84,166));
	m_btnDiscuss.SetBitmap(IDB_CHANEL_IM ,IDB_CHANEL_IM ,RGB(0,255,0));
	m_btnDiscuss.SetFlatStyle(CFlatButton::FS_OFFICE,CFlatButton::AS_BOTTOM);
	// 设置群相册按钮风格
	m_btnAlbum.SetTextColor(RGB(0,84,166));
	m_btnAlbum.SetBitmap(IDB_CHANEL_ALBUM,IDB_CHANEL_ALBUM,RGB(0,255,0));
	m_btnAlbum.SetFlatStyle(CFlatButton::FS_OFFICE,CFlatButton::AS_BOTTOM);


	m_btnHistory.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
	m_btnClose.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
	UpdateXFont(g_ImFont);
    
	CRect rcDlg;
	GetWindowRect(rcDlg);
	rcDlg.right = rcDlg.left + P2P_DLG_WIDTH;
	rcDlg.bottom = rcDlg.top + P2P_DLG_HIGHT;
	MoveWindow(rcDlg);

	AdjustUI();


	m_wndSplitter1.SetLimitSize(100);

	AttachHMGScrollbar(m_reditHistory.m_hWnd);
	AttachHMGScrollbar(m_reditSend.m_hWnd);

	//m_captionBar.SetWindowText(m_clusterInfo.szFamilyName);

	SetWindowText("群 "+CString(m_pClusterInfo->szFamilyName) );
	m_reditSend.SetStartIndent(80);

	// 显示群提示信息
	//AddClusterTip();
	PopMsg();

	//SetTimer(0x01,1000,NULL);
	// 检测下载群图片队列中是否有任务
	SetTimer(TIMER_DWN_CPIC,TIME_DWN_CPIC,NULL);

	m_reditSend.SetFocus();
	// 动作按钮按下
	if(g_LocalData.GetBasicSystemSetting()->bShowActionToolbar)
		m_wndToolBar.PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_ACTION,1),0);

	m_wndClusterPanel.UpdateClusterList();

	SetIcon(m_hBigIcon,TRUE);
	SetIcon(m_hSmallIcon,FALSE);

	g_pMessageHandleWnd->PostMessage(WM_CLUSTERSENDMSGDLG_NOTIFY,1,this->m_pClusterInfo->nFamilyID);
	//pMainWnd->m_pPage1->m_pageCluster.SetClusterFlashFlag(m_pClusterInfo->nFamilyID,false);	

	//m_btnClusterCfg.SetTextColor(RGB(0,84,166));
	//m_btnClusterCfg.SetBitmap(IDB_BITMAP_SYS_SET_NORMAL,IDB_BTN_SET,RGB(0,255,0));
	m_btnClusterCfg.SetBitmap(IDB_BITMAP_CFG_HOVER,IDB_BITMAP_CFG_NORMAL,IDB_BITMAP_CFG_PRESSED,IDB_BITMAP_CFG_PRESSED);
	
	m_wndToolBar.HideScoreShakeBtn();

	LoadLastTalkMsg();

	SetTimer(0x02,300,NULL);
	SetMinMaxInfo(500,350,0,0);
	return TRUE;  
}

void CClusterSendMsgDlg::OnFaceIconClicked()
{
	CPoint pt;
	GetCursorPos(&pt);

	CEmotionDlg *dlg = new CEmotionDlg(pt.x,pt.y,&g_imgEmotionList,this);
	dlg->m_hEmotionSelWnd = m_hWnd;
	dlg->ShowWindow(SW_SHOW);
}


void CClusterSendMsgDlg::InitResource()
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

void CClusterSendMsgDlg::OnFontBoldClicked()
{
	DWORD dwOld = g_ImFont.flags & XTF_BOLD;

	if ( dwOld )
	{
		g_ImFont.flags &= ~XTF_BOLD;
	}
	else
	{
		g_ImFont.flags |= XTF_BOLD;
	}

	m_reditSend.SetXFont(g_ImFont);
	m_reditSend.SetFocus();

	m_wndTalkToolBar.UpdateFontBtnTips();
}

void CClusterSendMsgDlg::OnFontItalicClicked()
{
	DWORD dwOld = g_ImFont.flags & XTF_ITALIC;

	if ( dwOld )
	{
		g_ImFont.flags &= ~XTF_ITALIC;
	}
	else
	{
		g_ImFont.flags |= XTF_ITALIC;
	}

	m_reditSend.SetXFont(g_ImFont);
	m_reditSend.SetFocus();

	m_wndTalkToolBar.UpdateFontBtnTips();
}

void CClusterSendMsgDlg::OnFontUnderLineClicked()
{
	DWORD dwOld = g_ImFont.flags & XTF_UNDERLINE;

	if ( dwOld )
	{
		g_ImFont.flags &= ~XTF_UNDERLINE;
	}
	else
	{
		g_ImFont.flags |= XTF_UNDERLINE;
	}

	m_reditSend.SetXFont(g_ImFont);
	m_reditSend.SetFocus();

	m_wndTalkToolBar.UpdateFontBtnTips();
}


void CClusterSendMsgDlg::OnBnClickedBtnColor()
{
	CColorDialog dlg(g_ImFont.fontColor,0,this);

	if ( dlg.DoModal()==IDOK )
	{
		g_ImFont.fontColor = dlg.GetColor();
	}

	m_reditSend.SetXFont(g_ImFont);
	m_reditSend.SetFocus();
}

void CClusterSendMsgDlg::OnBnClickedBtnClose()
{
	DestroyWindow();
}

void CClusterSendMsgDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 500;//最小宽度
	lpMMI->ptMinTrackSize.y = 350;//最小高度
}

HBRUSH CClusterSendMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}
	////////////////////////////////////////////////////////////////////
	return hbr;
}

void CClusterSendMsgDlg::UpdateXFont(const XT_FONT &xf)
{
	//	m_btnBold.SetDown( xf.flags & XTF_BOLD );
	//	m_btnItalic.SetDown( xf.flags & XTF_ITALIC );
	//	m_btnUnderLine.SetDown( xf.flags & XTF_UNDERLINE );
}

void CClusterSendMsgDlg::UpdateP2PPanel()
{
	if (m_bShowHistory) {
		m_wndPopupHistory.ShowWindow(SW_SHOW);
		m_wndClusterPanel.ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndPopupHistory.ShowWindow(SW_HIDE);
		m_wndClusterPanel.ShowWindow(SW_SHOW);
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

		MoveWindow(&rc);
	}

	AdjustUI();

	m_bPreShowHistory = m_bShowHistory;

}

void CClusterSendMsgDlg::AdjustUI()
{
	if ( m_wndHide.m_hWnd==NULL )
		return ;

	BOOL bActionBar=FALSE;
	BOOL bFontBar=FALSE;
	int nToolBarDelta=0;
	CRect rc;
	GetClientRect(&rc);
	int cx = rc.Width();
	int cy = rc.Height();

	int nBarCx = 0;
	int nTop   = 82;
	int nLeft  = BORDER_WIDTH;

	if (m_bShowHistory) {
		nBarCx = m_uPopupHistoryWidth + BAR_BORDER_WIDTH;
	}
	else
	{
		nBarCx = AD_WINDOW_WIDTH + BAR_BORDER_WIDTH;
	}

	int nClientCx  = cx-nBarCx-BUTTON_FOLDER_WIDTH;

	m_wndHide.MoveWindow(cx-10,0,10,10);

	if ( m_nSplitterPos>(cy-150) )
	{
		m_nSplitterPos = cy-150;
	}

	// 放置按钮
	//m_btnDiscuss.MoveWindow(2,nTop,98,26);
	//m_btnAlbum.MoveWindow(105,nTop,98,26);
	//m_btnHome.MoveWindow(208,nTop,98,26);

	// 用户面板位置
	CPoint ptUserPanel;
	ptUserPanel.x = cx-nBarCx-BUTTON_FOLDER_WIDTH, ptUserPanel.y = nTop;
	// 用户面板大小
	CSize  szUserPanel;
	szUserPanel.cx = nBarCx, szUserPanel.cy = cy-nTop-1;
	// 聊天历史记录窗位置
	CPoint ptHistory;
	ptHistory.x = nLeft, ptHistory.y=nTop;
	// 聊天历史记录窗大小
	CSize  szHistory;
	szHistory.cx = nClientCx, szHistory.cy=cy-m_nSplitterPos-nTop;
	
	if( m_wndActionToolBar.IsWindowVisible())
	{
		szHistory.cy-=26;
	}
	// 如果有字体工具栏则历史记录窗高度减25
	if ( m_wndTalkToolBar.IsWindowVisible() )
	{
		szHistory.cy-=30;
	}
	// 分隔条开始的y轴位置
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

	m_btnHistory.MoveWindow(nLeft,nTop, 0,0,false);
	szBtn=m_btnHistory.SizeToContent();
	m_btnHistory.Invalidate();

	m_btnClose.MoveWindow(nClientCx-szBtn.cx*2-3,nTop,szBtn.cx,szBtn.cy);
	m_btnClose.SizeToContent();
	//m_btnClose.Invalidate();

	m_btnSend.MoveWindow(nClientCx-szBtn.cx-1,nTop,szBtn.cx,szBtn.cy);
	m_btnSend.SizeToContent();
	//m_btnSend.Invalidate();

	//////消息记录
	m_wndPopupHistory.MoveWindow(ptUserPanel.x,ptUserPanel.y,szUserPanel.cx,szUserPanel.cy);

	m_wndClusterPanel.MoveWindow(ptUserPanel.x,ptUserPanel.y,szUserPanel.cx,szUserPanel.cy);

	m_reditSend.MoveWindow(ptSend.x,ptSend.y,szSend.cx,szSend.cy);
	m_reditHistory.MoveWindow(ptHistory.x,ptHistory.y,szHistory.cx,szHistory.cy);
	//m_wndSplitter1.MoveWindow(ptSplitter.x,ptSplitter.y,szSplitter.cx,szSplitter.cy);
	
	// 布置动作工具栏
	m_wndActionToolBar.MoveWindow(ptToolBar.x,ptToolBar.y,nClientCx,24);
	bActionBar=m_wndActionToolBar.IsWindowVisible();
	bFontBar=m_wndTalkToolBar.IsWindowVisible();
	if(bActionBar)
	{
		m_wndActionToolBar.MoveWindow(ptHistory.x,ptHistory.y+szHistory.cy+2,nClientCx,24);
		nToolBarDelta=26;
	}
	// 布置字体工具栏
	if (bFontBar)
	{
		m_wndTalkToolBar.MoveWindow(ptHistory.x,ptHistory.y+szHistory.cy+nToolBarDelta,nClientCx,30);
	}

	m_wndToolBar.MoveWindow(ptToolBar.x,ptToolBar.y,szToolBar.cx,szToolBar.cy);

	CRgn rgnClient;
	GetRedrawRgn(rgnClient);
	InvalidateRgn(&rgnClient);
}

void CClusterSendMsgDlg::DrawCaptionBar()
{
	CDC *pDC = GetWindowDC();

	if ( pDC==NULL )
		return ;

	//覆盖原来的按钮
	CRect rcWindow;
	GetClientRect(rcWindow);
	rcWindow.top = 1; //边框
	rcWindow.left = rcWindow.left + 410;
	rcWindow.bottom = rcWindow.top + SYS_BTN_SIZE;
	pDC->FillSolidRect(rcWindow, m_colorBackground);

	// 	CRect rcCapt;
	// 	GetClientRect(rcCapt);
	// 	rcCapt.bottom = 82;
	// 	pDC->Rectangle(&rcCapt);
	DrawStyle(pDC);

	ReleaseDC(pDC);
}

void CClusterSendMsgDlg::ProcessMsg(const XT_MSG &msg)
{
	m_dwTickCountActive = GetTickCount();

	switch (msg.data_type)
	{
	case 0://即时聊天消息
		{
			AddTalkMsg(msg,false);
		}
		break;
	//case 6:
	//	// 接收群图片块数据
	//	OnRecvClusterImageFilePacket(msg);
	//	break;
	//case 7:
	//	// 服务器返回接收群图片块成功的应答
	//	OnClusterImageFilePacketAck(msg);
	//	break;
	case 8:
		//OnRecvClusterImageFileReq(msg);
		// 通知本用户开始下载群图片
		SF_RecvStartDownloadCfileNotify(msg);
		break;
	default:
		break;
	}	
}

void CClusterSendMsgDlg::SF_OnRecvFileReq(char *file_name)
{
	// 记录接收的群图片
	g_LocalData.AcceptClusterPic(file_name);

	int count = m_vecWaitImageFiles.size();

	for ( int i=0; i<count; i++ )
	{
		if ( strcmp(m_vecWaitImageFiles[i].szFile,file_name)==0 )
		{
			m_vecWaitImageFiles.erase(m_vecWaitImageFiles.begin()+i);
			count--;
		}
	}
}

void CClusterSendMsgDlg::SF_OnAbortFile(const  char *file_name)
{
	if ( m_hWnd && m_reditHistory.m_hWnd )
		m_reditHistory.OnAbortImageFile(file_name);
}

void CClusterSendMsgDlg::SF_OnRecvFileComplete(const char *file_name)
{
	if ( m_hWnd && m_reditHistory.m_hWnd )
		m_reditHistory.OnRecvImageFile(file_name);
}

void CClusterSendMsgDlg::SF_InitXtMsg(XT_MSG &msg)
{
	memset(&msg,0,sizeof(msg));

	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.face		= 0;
	msg.from_id     = g_ImUserInfo.GetId();

	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

	msg.fontColor   = g_ImFont.fontColor;
	msg.fontSize    = g_ImFont.fontSize;
	strcpy(msg.fontName,g_ImFont.fontName);
	msg.fontStyle   = g_ImFont.flags;
	msg.cluster_id  = m_pClusterInfo->nFamilyID;

	msg.cluster_type = PERM_FAMILY;
	msg.msg_type     = 1;//群聊消息
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

}

void CClusterSendMsgDlg::AddClusterTip()
{
	CString strTip;
	strTip.Format("欢迎您来到群“%s”的聊天室。",this->m_pClusterInfo->szFamilyName);
	
	XT_FONT xf;
	xf.fontColor=RGB(0,166,84);

	m_reditHistory.SetStartIndent(120);

	m_reditHistory.InsertText("\n");
	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.InsertText(strTip);
	m_reditHistory.InsertText("\n");

	m_reditHistory.SetStartIndent(300);

	FRIEND_INFO *pCreator = m_pClusterInfo->GetCreator();

	if(g_pWebConfig)
	{
		strTip.Format("群主页   :%s",(LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(m_pClusterInfo->szFamilyDomain)));
		m_reditHistory.InsertText(strTip);
	}

	
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.SetStartIndent(300);
	m_reditHistory.InsertText("\n");

	try
	{
		if(pCreator == NULL)
			strTip.Format("群创始人 : ");
		else
			strTip.Format("群创始人 : %s",pCreator->nickname);
	}
	catch(...)
	{
		strTip.Format("群创始人 : ");
	}

	m_reditHistory.InsertText(strTip);
	m_reditHistory.InsertText("\n");
	m_reditHistory.SetStartIndent(80);
	m_reditHistory.InsertLine();
	m_reditHistory.InsertText("\n");	
}

void CClusterSendMsgDlg::PushMsg(const _tagMsg & msg)
{
	m_lstMsgRecv.push_back(msg);
	if ( m_lstMsgRecv.size()>100 )
		m_lstMsgRecv.pop_front();
}



void CClusterSendMsgDlg::PopMsg()
{
	if ( m_reditHistory.m_hWnd && m_hWnd )
	{
		list<_tagMsg>::iterator it;

		m_lstMsgRecv.sort();
		
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

void CClusterSendMsgDlg::AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg)
{
	CString sObj;		// 聊天对象
	CString sAction0;	// 聊天动作0
	CString sAction1;	// 聊天动作1
	CString sAction2;	// 聊天动作2
	CString	sMsg;		// 聊天信息
	bool	bHasAction;	// 消息中是否包含动作信息
	CHARRANGE cr;
	XT_FONT xf;
	// 处理动作消息
	bHasAction=ParseMsg(msg.data,sMsg,sObj,sAction0,sAction1,sAction2);


	if ( m_hWnd==NULL || m_reditHistory.m_hWnd==NULL )
	{
		_tagMsg t_msg;
		t_msg.msg  = msg;
		t_msg.type = 0;
		PushMsg(t_msg);	
		return ;
	}

	CTime  tm(msg.send_time);
	CString strTime = tm.Format(" (%H:%M:%S)");
	//CString strDate = tm.Format("%Y.%m.%d");

	// 获得发话人的昵称
	CString strNickName;
	if ( msg.from_id==g_ImUserInfo.GetId() )
	{
		strNickName = g_ImUserInfo.nickname;
	}
	else
	{
		FRIEND_INFO *pFriend = m_pClusterInfo->GetFriendInfo(msg.from_id);
		
		if ( pFriend )
		{
			strNickName = pFriend->nickname;
		}
		else
		{
			strNickName = msg.from_nickname;
		}
	}

	strNickName.TrimRight();

	//strTitle.Format("%s %s ",strNickName,strTime);
	// 发话人名的颜色
	
	// 先用指定的字体回车换行
	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.GetSel(cr);
	m_reditHistory.InsertText("\n");
	m_vecRecords.push_back(cr.cpMin);
	m_reditHistory.SetStartIndent(80);
	// 打印出消息标题（发话人+动作+对象+日期）
	//xf.fontColor=RGB(0,84,166);
	//m_reditHistory.InsertText(strTime,xf);
	if(bHasAction)
	{
		xf.fontColor=CLR_ACTIONS;
		m_reditHistory.InsertText(sAction0,xf);
		xf.fontColor=RGB(0,84,166);
		if(msg.from_id != g_ImUserInfo.GetId())
		{
			m_reditHistory.InsertUserLink(strNickName,msg.from_id);
		}
		else
			m_reditHistory.InsertText(strNickName,xf);
		xf.fontColor=CLR_ACTIONS;
		m_reditHistory.InsertText(sAction1,xf);
		xf.fontColor=CLR_OBJECT;
		if(msg.to_id != g_ImUserInfo.GetId() && msg.to_id != 0)
		{
			m_reditHistory.InsertUserLink(sObj,msg.to_id);
		}
		else
			m_reditHistory.InsertText(sObj,xf);
		xf.fontColor = CLR_ACTIONS;
		m_reditHistory.InsertText(sAction2,xf);
	}	
	else
	{
		xf.fontColor=RGB(0,84,166);
		if(msg.from_id != g_ImUserInfo.GetId())
		{
			m_reditHistory.InsertText(strNickName,xf);
			//m_reditHistory.InsertUserLink(strNickName,msg.from_id);
			//m_reditHistory.InsertText(" 说",xf);
		}
		else
		{
			m_reditHistory.InsertText(strNickName,xf);
		}

	}

	xf.fontColor=RGB(0,84,166);
	m_reditHistory.InsertText(" " + strTime,xf);

	m_reditHistory.InsertText("\n");
	m_reditHistory.SetSel(-1,-1);
	m_reditHistory.GetSel(cr);	

	m_reditHistory.SetParagraphBulleted();

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
	if(bHasAction)
		m_reditHistory.InsertText(sMsg,!isHistoryMsg,&sl);
	else
	{
		if(msg.msg_type == 11)
			m_reditHistory.InsertText(msg.data,false,&sl);
		else
			m_reditHistory.InsertText(msg.data,!isHistoryMsg,&sl);
	}
	// 离线图片不显示加载动画
	if(msg.msg_type != 11)
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

	if ( m_vecRecords.size()>100 )
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

void CClusterSendMsgDlg::OnBnClickedBtnSendEx()
{
	BOOL bPtIndex = m_btnSend.PressedInText();
	if(bPtIndex)
	{
		OnBnClickedBtnSend();
	}
	else
	{
		OnBnClickedBtnHotkey();
	}
}

void CClusterSendMsgDlg::OnBnClickedBtnSend()
{
	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		AddMsgTip("对不起，您当前为离线状态，无法发送信息！");
		return;
	}

	//CLUSTER_INFO *p = FindClusterInfo(m_clusterInfo.nFamilyID);
	if ( m_pClusterInfo && m_pClusterInfo->bRefused )
	{
		m_pClusterInfo->bRefused=false;
	}

	XT_MSG msg;
	SF_InitXtMsg(msg);

	CString text;
	IMAGE_FILE_LIST il;
	m_reditSend.GetText(text,&il);
	m_reditSend.ClearEmotionPicData();

	if ( text.IsEmpty() )
	{
		CMessageBox::Prompt("消息不能为空");
		return ;
	}

	// 判断是否有动作信息
	ACTION_DATA ad;
	if(m_wndActionToolBar.GetActionData(ad))
	{
		text=ad.ToStr()+text;
		msg.to_id = ad.id;
	}
	else
	{
		// 如果对说话对象没有动作和聊天语言,则返回
		if(text.GetLength()==0)
		{
			m_reditSend.SetFocus();
			return;
		}
		ad.sObject=m_wndActionToolBar.GetCurSelObject();
		if(ad.sObject!="")
		{
			ad.sAction1=" 对";
			ad.sAction2="说：";
			text=ad.ToStr()+text;
			msg.to_id = ad.id;
		}
	}
	if ( text.GetLength()>900 )
	{
		//MessageBox();
		//CMessageBox::Prompt("最多输入900个英文字符和450个汉字!");
		AddMsgTip("你发送消息内容超长，请分条发送。");
		return ;
	}

	if ( text.GetLength()==0 )
	{
		m_reditSend.SetFocus();
		return ;
	}

	if ( FindForbiddenWords(text) )
	{
		m_reditSend.ClearMsg();
		m_reditSend.SetFocus();
		AddMsgTip("你发送的消息中有限制用语，消息发送失败！");
		return ;
	}
	// 重置动作为无
	m_wndActionToolBar.ResetAction();

	strcpy(msg.data,text);
	
	msg.data_len=strlen(text)+1;


	// 检测消息中是否有定义信息，并作相应转化
	CString sShowMsg;
	ProcTalkMsg(msg.data,sShowMsg);
	XT_MSG msgShow = msg;
	strncpy(msgShow.data,sShowMsg,sizeof(msgShow.data)-1);
	AddTalkMsg(msgShow,false);

	IMAGE_FILE_LIST::iterator it;

	if ( il.size()>0 )
	{
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
			AddMsgTip("你发送消息内容超长，请分条发送。");
			return;
		}
		else
		{
			strcpy(msg.data, strData);
			msg.data_len = strData.GetLength();
			g_pProtocol->SendClusterMsg(msg);
		}

		il.clear();
	}
	else
	{
		g_pProtocol->SendClusterMsg(msg);
	}
	RegUserOperation(0x00b6);

	bool bSave=g_LocalData.SaveTalkMsg(msgShow);

	CMainFrame::GetInstance()->GetRecentList()->AddCluster(m_pClusterInfo);
	RECENTLIST_OP(AddCluster(m_pClusterInfo))

	m_reditSend.ClearMsg();
	m_reditSend.SetFocus();
}


void CClusterSendMsgDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	if ( nType!=SIZE_MINIMIZED )
	{
		AdjustUI();

		//if ( m_reditHistory.m_hWnd )
		//	m_reditHistory.ScrollToBottom();
	}
}

void CClusterSendMsgDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

BOOL CClusterSendMsgDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( wParam==IDC_SPLITTER1 )
	{
		SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;

		m_nSplitterPos-=pHdr->delta;

		AdjustUI();

		m_reditHistory.ScrollToBottom();
	}

	return CXSkinDlg::OnNotify(wParam, lParam, pResult);
}

void CClusterSendMsgDlg::OnBnClickedBtnHistory()
{
	std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
	pDlg->SetDefaultCluster(m_pClusterInfo->nFamilyID);
}

LRESULT CClusterSendMsgDlg::OnEmotionSelNotify(WPARAM wParam,LPARAM lParam)
{

	CUSTOM_EMOTION * pFace = (CUSTOM_EMOTION * )wParam;
	m_reditSend.InsertFace(pFace,(int)lParam);
	//switch(pFace->type)
	//{
	//case CUSTOM_EMOTION::SYS:
	//	m_reditSend.InsertEmotion(wParam);
	//	break;
	//case CUSTOM_EMOTION::SYS_GIF:
	//	m_reditSend.InsertGifEmotion(wParam);
	//	break;
	//case CUSTOM_EMOTION::CUSTOM:
	//	{
	//		CString strCustomFaceFile = g_LocalData.GetCustomFacePath()+pFace->szFile;
	//		CString strCacheFile = g_LocalData.GetImageFileCachePath()+pFace->szFile;
	//		if ( _access((LPCTSTR)strCacheFile,0)==-1 )
	//			CopyFile(strCustomFaceFile,strCacheFile,FALSE);
	//		m_reditSend.InsertImage(strCacheFile,EMOTION_PIC_DATA());
	//	}
	//    break;
	//case CUSTOM_EMOTION::WEB:
	//	{
	//		CString strCustomFaceFile = g_LocalData.GetCustomFacePath()+pFace->szFile;
	//		CString strCacheFile = g_LocalData.GetImageFileCachePath()+pFace->szFile;
	//		// 先判断缓存文件夹中有没有该表情
	//		if ( _access((LPCTSTR)strCacheFile,0)==-1 )
	//			CopyFile(strCustomFaceFile,strCacheFile,FALSE);

	//		// 如果url为空，则发送本地图片
	//		if(strcmp(pFace->szUrl,"")==0)
	//		{		
	//			// 显示缓存中的图片文件
	//			m_reditSend.InsertImage(strCacheFile,EMOTION_PIC_DATA());
	//		}
	//		else
	//		{
	//			EMOTION_PIC_DATA data;
	//			data.nType = EPT_WEB;
	//			data.sData = pFace->szUrl;
	//			m_reditSend.InsertImage(strCacheFile,data);
	//		}
	//	}
	//    break;
	//default:
	//    break;
	//}
	m_reditSend.SetFocus();

	return 1;
}

void CClusterSendMsgDlg::OnClose()
{
	DestroyWindow();
}

int CClusterSendMsgDlg::DoModal()
{	
	return CXSkinDlg::DoModal();
}

void CClusterSendMsgDlg::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);

	CXSkinDlg::OnDestroy();
	this->m_wndClusterPanel.DestroyWindow();

	list<_tagMsg>::iterator it;

	for ( it=m_lstMsgRecv.begin(); it!=m_lstMsgRecv.end(); it++ )
	{
		switch ( it->type )
		{
		case 0:
			g_LocalData.SaveTalkMsg(it->msg);
			break;
		}
	}

	m_lstMsgRecv.clear();

	::PostMessage(g_pMessageHandleWnd->m_hWnd,WM_CLUSTERSENDMSGDLG_NOTIFY,0,m_pClusterInfo->nFamilyID);
}

void CClusterSendMsgDlg::OnStnClickedStaticTitle()
{
	CClusterSettingDlg::Show(m_pClusterInfo);
}

void CClusterSendMsgDlg::OnCMemberInd(const char *data, WORD wRet)
{
	LPXT_CMEMBER_IND ind = (LPXT_CMEMBER_IND)data;
	FRIEND_INFO member;

	if ( wRet==0 && ind->cluster_id==m_pClusterInfo->nFamilyID && ind->cluster_type==PERM_FAMILY )
	{
		if ( ind->action==0 )//添加成员通知
		{
			if ( m_pClusterInfo->GetFriendInfo(ind->member_id)==NULL )
			{

				member.SetId(ind->member_id);
				member.cluster_property=CMEMBER_NORMAL;
				member.status = XTREAM_OFFLINE;
				FRIEND_INFO* pFriendInfo = /*m_clusterInfo.GetFriendInfo(ind->member_id);*/GetFriendInfo(ind->member_id);
				if(pFriendInfo)
				{
					strncpy(member.nickname,pFriendInfo->nickname,sizeof(member.nickname)-1);
				}
				m_pClusterInfo->members.push_back(member);
				CString str;
				str.Format("【%s】加入群【%s】",/*pFriendInfo->nickname*/ind->nickname,ind->clustername);
				this->AddMsgTip(str);
			}
			else
			{
				FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ind->member_id);
				CString str;
				str.Format("【%s】加入群【%s】",pFriendInfo->nickname,ind->clustername);
				this->AddMsgTip(str);
			}
		}
		else if ( ind->action==1 )//退出,删除
		{
			/*FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ind->member_id);
			if(pFriendInfo)
			{
				strncpy(ind->nickname,pFriendInfo->nickname,sizeof(ind->nickname)-1);
			}
			else
			{
				itoa(ind->uid,ind->nickname,10);
			}*/

			CString str;
			str.Format("【%s】退出了群【%s】",/*pFriendInfo->username*/ind->nickname,ind->clustername);
			this->AddMsgTip(str);

			//m_pClusterInfo->DelMember(ind->member_id);
		}
		else if(ind->action == 2)//设置管理员
		{
			FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ind->member_id);			
			if(pFriendInfo)
			{
				strncpy(ind->nickname,pFriendInfo->nickname,sizeof(ind->nickname)-1);
			}
			else
			{
				itoa(ind->uid,ind->nickname,10);
			}

			FRIEND_INFO* pCreater = m_pClusterInfo->GetCreator();

			CString str;
			str.Format("【%s】被群【%s】的群主【%s】设置成管理员",/*pFriendInfo->username*/ind->nickname,ind->clustername,pCreater->GetUserName().c_str());
			this->AddMsgTip(str);
		}	
		else if(ind->action == 3)//取消管理员
		{
			FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ind->member_id);			
			if(pFriendInfo)
			{
				strncpy(ind->nickname,pFriendInfo->nickname,sizeof(ind->nickname)-1);
			}
			else
			{
				itoa(ind->uid,ind->nickname,10);
			}

			FRIEND_INFO* pCreater = m_pClusterInfo->GetCreator();

			CString str;
			str.Format("【%s】被群【%s】的群主【%s】取消了管理员权限",/*pFriendInfo->username*/ind->nickname,ind->clustername,pCreater->GetUserName().c_str());
			this->AddMsgTip(str);
		}
// 		else if(ind->action == 4)//更改名片
// 		{
// 
// 		}
		else if(ind->action == CMEMBER_INVITE_REFUSE)//被邀请方拒绝入群
		{
			FRIEND_INFO* pFriendInfo = GetFriendInfo(ind->member_id);
			CString str;
			FRIEND_INFO* pCreator = m_pClusterInfo->GetCreator();
			str.Format("【%s】拒绝加入群【%s】",/*pFriendInfo->username*/ind->nickname,ind->clustername);
			this->AddMsgTip(str);
		}
		else if(ind->action == CMEMBER_CREATOR_REFUSE)//群主拒绝被邀请方入群
		{
			FRIEND_INFO* pFriendInfo = GetFriendInfo(ind->member_id);
			FRIEND_INFO* pCreator = m_pClusterInfo->GetCreator();
			CString str;
			str.Format("【%s】拒绝【%s】加入群【%s】",pCreator->GetUserName().c_str(),/*pFriendInfo->username*/ind->nickname,ind->clustername);
			this->AddMsgTip(str);
		}
		m_wndClusterPanel.UpdateClusterList();
	}
}

void CClusterSendMsgDlg::OnSetCMemberAck(const char *data,WORD wRet)
{
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	if ( wRet==0 )
	{
		if ( ack->nFamilyProp==PERM_FAMILY && ack->nFamilyID == m_pClusterInfo->nFamilyID)
		{
			CLUSTER_INFO *pCluster = FindClusterInfo(ack->nFamilyID);

			if ( pCluster!=NULL )//新创建的群
			{
				switch (ack->nAction)
				{
				case CMEMBER_ADD:
					{
						CString strNickName;
						FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ack->nMemberID);
						if(pFriendInfo != NULL)
							strNickName = pFriendInfo->nickname;
						else
							strNickName.Format("%d", ack->nMemberID);
						CString str;
						str.Format("【%s】加入群【%s】", strNickName, pCluster->szFamilyName);
						this->AddMsgTip(str);
					}
					break;

				case CMEMBER_DEL:
					{
						CString strNickName;
						FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ack->nMemberID);
						if(pFriendInfo != NULL)
						{
							strNickName = pFriendInfo->nickname;
						}
						else
						{
							strNickName.Format("%d", ack->nMemberID);
						}

						FRIEND_INFO* pCreater = m_pClusterInfo->GetCreator();
						CString str;
						str.Format("【%s】已被群主【%s】请出群", strNickName,pCreater->GetUserName().c_str());
						this->AddMsgTip(str);
					}
					break;
	
				case CMEMBER_CANCEL_ADMIN:
					{
						CString strNickName;
						FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ack->nMemberID);
						if(pFriendInfo != NULL)
						{
							strNickName = pFriendInfo->nickname;
						}
						else
						{
							strNickName.Format("%d", ack->nMemberID);
						}
						FRIEND_INFO* pCreater = m_pClusterInfo->GetCreator();
						CString str;
						str.Format("【%s】已被群主【%s】取消管理员权限", strNickName,pCreater->GetUserName().c_str());
						this->AddMsgTip(str);
					}
					break;
				case CMEMBER_SET_ADMIN:
					{
						CString strNickName;
						FRIEND_INFO* pFriendInfo = m_pClusterInfo->GetFriendInfo(ack->nMemberID);
						if(pFriendInfo != NULL)
						{
							strNickName = pFriendInfo->nickname;
						}
						else
						{
							strNickName.Format("%d", ack->nMemberID);
						}
						FRIEND_INFO* pCreater = m_pClusterInfo->GetCreator();
						CString str;
						str.Format("【%s】已被群主【%s】设置为管理员", strNickName,pCreater->GetUserName().c_str());
						this->AddMsgTip(str);
					}
					break;

				//case CMEMBER_INVITE_REFUSE:
				//	{
				//		CString str;
				//		FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
				//		str.Format("【%s】拒绝加入群【%s】",pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);
				//		this->AddMsgTip(str);
				//	}
				//	break;
				//case CMEMBER_CREATOR_REFUSE:
				//	{
				//		CString str;
				//		FRIEND_INFO* pCreator = pCluster->GetCreator();
				//		FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
				//		str.Format("【%s】拒绝【%s】加入群【%s】",pCreator->username,pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);
				//		this->AddMsgTip(str);
				//	}
				//	break;
				}
				//		ImGetClusterMember(ack->nFamilyID);
			}
		}
		m_wndClusterPanel.UpdateClusterList();
	}
	else
		if ( wRet==SERVER_TIMEOUT )
		{
			//if ( ack->nFamilyProp==TEMP_FAMILY )
			//	ShowMessage("邀请多人聊天出现意外,服务器处理超时或您的网络有问题.请稍候再试.!",MB_OK|MB_ICONINFORMATION);
			//else
			//if ( ack->nFamilyProp==PERM_FAMILY )
			//{
			//	ShowMessage("设置群成员时出现现意外,服务器处理超时或您的网络有问题.请稍候再试.!",MB_OK|MB_ICONINFORMATION);
			//}
		}
		else
		{
			ShowMessage(g_pProtocol->GetLastError());
		}
}

void CClusterSendMsgDlg::OnUserInfoModified(const char *data,WORD wRet)
{
	LPXT_USERINFO_MOD_ACK resp = (LPXT_USERINFO_MOD_ACK)data;

	if ( wRet==0 )
	{
		FRIEND_INFO *pFriend = m_pClusterInfo->GetFriendInfo(CSAConfig::pInstance->m_UserInfo.GetId());

		if ( pFriend )
		{
			memcpy(pFriend,&CSAConfig::pInstance->m_UserInfo,sizeof(XT_USER_INFO));
			this->m_wndClusterPanel.UpdateClusterList();
		}

	}	
}

void CClusterSendMsgDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawBorder();
	DrawCaptionBar();

	CDC *pDC = GetWindowDC();
	if(pDC==NULL)
		return;

	int nSavedDC = pDC->SaveDC();

	CRect rc;
	GetWindowRect(rc);

	int cx=rc.Width(); 
	int cy=rc.Height();

	pDC->Draw3dRect(1,1,cx-2,cy-2,m_colorBorder,m_colorBorder);

	pDC->RestoreDC(nSavedDC);
	ReleaseDC(pDC);
}

BOOL CClusterSendMsgDlg::OnEraseBkgnd(CDC* pDC)
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

	return FALSE;
}

void CClusterSendMsgDlg::OnUserInfoGetAck(const char *data,WORD wRet)
{
	LPXT_USERINFO_GET_ACK ack=(LPXT_USERINFO_GET_ACK)data;

	if ( wRet==0 )
	{
		FRIEND_INFO *pFriend = m_pClusterInfo->GetFriendInfo(ack->GetId());
		
		if ( pFriend )
		{
			int nCluster_Property=pFriend->cluster_property;
			//memcpy(pFriend,ack,sizeof(XT_USER_INFO));
			UpdateUserFullInfo(pFriend,ack);
			pFriend->cluster_property=nCluster_Property;

			pFriend->bRefreshed = true;

			m_wndClusterPanel.UpdateClusterList();

			//获取一次群名片
			XT_PROFILENAME_GET_REQ reqMem;
			reqMem.nFamilyID   = m_pClusterInfo->nFamilyID;
			g_pProtocol->SendReq(reqMem);
		}
	}
}

BOOL CClusterSendMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( TranslateAccelerator(m_hWnd,m_hAccel,pMsg)==0 )
	{
		if  (  pMsg->message  ==  WM_KEYDOWN  )  
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

		return CXSkinDlg::PreTranslateMessage(pMsg);
	}
	else
	{
		return TRUE;
	}
}
#ifdef _DEBUG
#define TEMP_DEBUG
#endif
void CClusterSendMsgDlg::OnSendPicClicked()
{
	CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,this);
	
	if ( IDOK==dlg.DoModal() )
	{
		if ( dlg.GetSelFileSize() < 512 )
		{
			CString strCacheFileName;
			g_LocalData.SaveImageFileToCache(dlg.GetPathName(),strCacheFileName);
			CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;
					
			m_reditSend.InsertImage(strCacheImageFullPathName/*,EMOTION_PIC_DATA()*/);
			m_reditSend.SetFocus();
		}
		else
		{
			ShowMessage("对不起，在群中您只能发送大小小于 512K 的图片！",MB_ICONINFORMATION|MB_OK);
		}
	}
}
BOOL CClusterSendMsgDlg::DestroyWindow()
{
	CClusterParamSetting::DestroyDlg();
	bool bForce = false;
	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		bForce = true;
	}
	if ( m_vecSendFiles.size()>0 && bForce == false)
	{
		if ( CMessageBox::Model(GetSafeHwnd(),"如果关掉窗口，将会终止未发出的图片，是否关闭窗口？",IM_CAPTION,MB_YESNO)!=IDYES )
		{
			return FALSE;
		}
	}
	if(m_wndToolBar.IsDownloader())
	{
		if(bForce == false &&  CMessageBox::Model(GetSafeHwnd(),"如果关掉窗口，将会终止下载，是否关闭窗口？","",MB_YESNO)==IDNO)
		{
			return FALSE;
		};
		m_wndToolBar.StopDownloadFace();
	}
	////m_reditSend.ClearMsg();
	////m_reditHistory.ClearMsg();

	return __super::DestroyWindow();
}


void CClusterSendMsgDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 0x02:
		{
			if( m_reditHistory.m_hWnd )
			{
				m_reditHistory.ScrollToBottom();
				m_reditSend.SetFocus();
			}

			KillTimer(0x02);
		}
		break;
	case TIMER_DWN_CPIC:
		{
			int i,n;
			RECV_FILE_STRUCT * pRF;
			n = m_vecRecvFiles.size();
			int nTimeNow = GetTickCount();
			for(i = 0;i < n; i++)
			{
				pRF = m_vecRecvFiles[i];
				if((nTimeNow - pRF->recv_tick) > DOWNLOAD_TIMEROUT)
				{
					// 发送文件超时
					TRACE("图片文件(%s)下载失败\n",pRF->file_name);
					SF_OnAbortFile(pRF->file_name);
					SF_RemoveRecvFile(pRF->file_id,pRF->from_id);
					break;
				}
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

				if ((time_now-wi.time_recv)>DOWNLOAD_TIMEROUT)
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
				switch(it->second->state) 
				{
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
				};	// end switch
			}	// end for
		}	// end case
	case 0x03:
		{	
			//发送群名片获取请求
			XT_PROFILENAME_GET_REQ reqMem;
			reqMem.nFamilyID   = m_pClusterInfo->nFamilyID;
			g_pProtocol->SendReq(reqMem);
			KillTimer(0x03);
		}
		break;
	}	// end switch
END0:
	__super::OnTimer(nIDEvent);
}

void CClusterSendMsgDlg::OnLoginOut(const char *data,WORD wRet)
{
	DestroyWindow();
}

void CClusterSendMsgDlg::OnStatusModAck(const char *data,WORD wRet)
{
	LPXT_STATUS_MOD_ACK resp = (LPXT_STATUS_MOD_ACK)data;
	
	if ( wRet==0 )
	{
		if ( resp->status==XTREAM_OFFLINE )
		{
			for ( int i=0; i < m_pClusterInfo->Count(); i++ )
			{
				m_pClusterInfo->members[i].status=XTREAM_OFFLINE;
			}

			m_reditSend.EnableWindow(FALSE);
			m_btnSend.EnableWindow(FALSE);
		}
		else
		{
			m_reditSend.EnableWindow(TRUE);
			m_btnSend.EnableWindow(TRUE);
		}

		m_wndClusterPanel.UpdateClusterList();
	}	
}

bool CClusterSendMsgDlg::GetRedrawRgn(CRgn &rgn)
{
	CRect rc;
	GetClientRect(&rc);

	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);

//	RemoveRgnOfWnd(rgn,&this->m_captionBar);
//	RemoveRgnOfWnd(rgn,&this->m_wndSplitter1);
	RemoveRgnOfWnd(rgn,&m_wndToolBar);
	RemoveRgnOfWnd(rgn,&m_reditHistory);
	RemoveRgnOfWnd(rgn,&m_reditSend);
	RemoveRgnOfWnd(rgn,&m_wndClusterPanel);
	//RemoveRgnOfWnd(rgn,&m_btnSend);
	//RemoveRgnOfWnd(rgn,&m_btnClose);
	//RemoveRgnOfWnd(rgn,&m_btnHistory);
	//RemoveRgnOfWnd(rgn,&m_btnInvite);
	//RemoveRgnOfWnd(rgn,&m_btnCard);
	//RemoveRgnOfWnd(rgn,&m_btnVisit);	
	return true;
}

void CClusterSendMsgDlg::OnBnClickedFontBar()
{
	CRect rc1;
	CRect rcBar;
	int nW;
	int nH;
	int nTop;
	m_reditHistory.GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	BOOL bActionBar = m_wndActionToolBar.IsWindowVisible();
	BOOL bFontBar = m_wndTalkToolBar.IsWindowVisible() ;

	if ( bFontBar )
	{
		// 如果当前字体工具栏已经显示，则隐藏
		rc1.bottom+=30;
		m_wndTalkToolBar.ShowWindow(SW_HIDE);
		m_reditHistory.MoveWindow(rc1);
		// 其他工具栏则相应往下落
		if(bActionBar)
		{
			m_wndActionToolBar.GetWindowRect(&rcBar);
			ScreenToClient(&rcBar);
			rcBar.OffsetRect(0,30);
			m_wndActionToolBar.MoveWindow(rcBar);
		}
		
	}
	else
	{
		// 如果当前字体工具栏隐藏，则显示
		rc1.bottom-=30;
		m_reditHistory.MoveWindow(rc1);	
		// 下一个工具栏的顶部位置
		nTop=rc1.bottom;
		// 其他工具栏也相应上升
		if(bActionBar)
		{
			m_wndActionToolBar.GetWindowRect(&rcBar);
			ScreenToClient(&rcBar);
			rcBar.OffsetRect(0,-26);
			m_wndActionToolBar.MoveWindow(rcBar);
			nTop+=30;
		}
		// 显示字体工具栏
		m_wndTalkToolBar.GetWindowRect(&rcBar);
		nW=rc1.Width();
		nH=rcBar.Height();
		rcBar.left=rc1.left;
		rcBar.top=nTop;
		rcBar.right=rcBar.left+nW;
		rcBar.bottom=rcBar.top+30;
		m_wndTalkToolBar.MoveWindow(rcBar);
		m_wndTalkToolBar.ShowWindow(SW_SHOW);
	}

	m_reditHistory.ScrollToBottom();
	m_reditSend.SetFocus();
}


void CClusterSendMsgDlg::OnScreenCopy()
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

			this->m_reditSend.InsertImage(strCacheImageFullPathName/*,EMOTION_PIC_DATA()*/);

			g_pMessageHandleWnd->DeleteScreenCtrl();
		}
		else
		{
			g_pMessageHandleWnd->DeleteScreenCtrl();
		}
	}
}


void CClusterSendMsgDlg::OnMagicFace()
{
	//ShowMessage("对不起，暂不支持魔法表情！");
	//return ;
	if(g_ImUserInfo.status == FRIEND_INFO::OFFLINE)
	{
		AddMsgTip("对不起，您当前为离线状态，无法发送魔法表情！");
		return;
	}
	CPoint pt;
	GetCursorPos(&pt);
	CMagicFaceDlg::Show(pt.x,pt.y,g_ImUserInfo.GetId(),m_hWnd);
}

LRESULT CClusterSendMsgDlg::OnMagicFaceNotify(WPARAM wParam, LPARAM lParam)
{
	const char * face_name = (LPCTSTR)lParam;
	const char * face_url  = (LPCTSTR)wParam;

	XT_MSG msg;
	SF_InitXtMsg(msg);

	msg.data_type   = XT_MSG::MAGIC_FACE;//魔法表情

	CSendPacket spMsg(msg.data);
	spMsg<<face_name<<face_url;
	msg.data_len = spMsg.GetSize();

	g_pProtocol->SendClusterMsg(msg);

	CString strTip;
	strTip.Format("魔法表情 \"%s\" 已经发出",face_name);
	AddMsgTip(strTip);

	if ( m_mapMagicFace.find(CString(face_name))==m_mapMagicFace.end() )
		m_mapMagicFace.insert(map<CString,CString>::value_type(face_name,face_url));

	ShowMagicFace(face_name);

	return 0;
}

void CClusterSendMsgDlg::AddMsgTip(CString strTip,COLORREF clrText)
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
	m_reditHistory.InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());
	m_reditHistory.InsertText(strTip);

	m_reditHistory.SetSel(cr.cpMax,-1);
	m_reditHistory.SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
	m_reditHistory.SetSel(-1,-1);

	m_reditHistory.ScrollToBottom();
}

void CClusterSendMsgDlg::ShowMagicFace(const char *face_name)
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

bool CClusterSendMsgDlg::RecvMagicFace(uint32 from_id,  CString strFaceName, CString strFaceUrl)
{
	map<CString,CString>::iterator it = m_mapMagicFace.find(strFaceName);

	if ( it==m_mapMagicFace.end() )
		m_mapMagicFace.insert(map<CString,CString>::value_type(strFaceName,strFaceUrl));

	FRIEND_INFO *pFriend=this->m_pClusterInfo->GetFriendInfo(from_id);

	if ( pFriend )
	{        
		CString strTip;
		strTip.Format("%s 发送了魔法表情",pFriend->nickname);

		if ( m_reditHistory.m_hWnd )
		{
			XT_FONT xf;
			xf.fontColor = RGB(122,122,122);

			CHARRANGE cr;

			m_reditHistory.SetSel(-1,-1);
			m_reditHistory.GetSel(cr);

			m_reditHistory.InsertText("\n");
			m_reditHistory.SetStartIndent(230);

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
			t_msg.msg.from_id = from_id;
			sprintf(t_msg.msg.fontName,strFaceName);
			sprintf(t_msg.msg.data,strFaceUrl);
			t_msg.type=2;
			PushMsg(t_msg);
		}
		
		return true;
	}
	else
	{
		return false;
	}

}

void CClusterSendMsgDlg::OnClusterMembersAck(const char *data,WORD wRet)
{
	LPXT_GET_CMEMBERS_ACK ack = (LPXT_GET_CMEMBERS_ACK)data;

	if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_pClusterInfo->nFamilyID && ack->nFamilyProp==PERM_FAMILY && ack->next_pos==0xFFFF )
		{
			CLUSTER_INFO *pCluster =::FindClusterInfo(ack->nFamilyID);

			if ( pCluster )
			{
				m_pClusterInfo = pCluster;
				this->m_wndClusterPanel.UpdateClusterList();
			}
		}
	}
}

// 右边组成员列表中选定了一个好友
LRESULT CClusterSendMsgDlg::OnThreadNotify(WPARAM w, LPARAM l)
{
	FRIEND_INFO *p=(FRIEND_INFO *)l;
	if ( w==1 )//单击群列表
	{
		//try
		//{
		//	// 往动作工具栏中添加一个聊天对象
		////	m_wndActionToolBar.AddObject(p->nickname);
		////	//this->m_reditSend.InsertText(s);
		//	this->m_reditSend.SetFocus();
		//}
		//catch(...)
		//{
		//}
	}
	else if(w==2)//双击群列表
	{
		//try
		//{
		//	CString s=p->nickname;
		//	s+=",";
		//	this->m_reditSend.InsertText(s);
		//	this->m_reditSend.SetFocus();
		//}
		//catch(...)
		//{
		//}

		try
		{
// 			if (m_wndToolBar.m_btnAction.IsDown()==false )
// 			{
// 			//	m_wndToolBar.OnBnClickedButtonAction();
// 			//	OnBnClickedAction();
// 				m_wndToolBar.PostMessage(WM_COMMAND,MAKEWPARAM(IDC_BUTTON_ACTION,1),0);
// 			}

			// 往动作工具栏中添加一个聊天对象
			m_wndActionToolBar.AddObject(p->nickname,p->GetId());
			//this->m_reditSend.InsertText(s);
			this->m_reditSend.SetFocus();
		}
		catch(...)
		{
		}

	}


	return 0;
}

LRESULT CClusterSendMsgDlg::OnMagicFaceClick(WPARAM wParam, LPARAM lParam)
{
	this->ShowMagicFace((char *)lParam);
	return 0;
}
void CClusterSendMsgDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	if ( nState!=WA_INACTIVE )
	{
		//if (this->IsWindowVisible()==FALSE)
		//{
		//	if ( pWndOther && pWndOther->m_hWnd )
		//	{
		//		pWndOther->SetFocus();
		//		pWndOther->BringWindowToTop();
		//	} 
		//}

		this->m_dwTickCountActive;
	}
}

void CClusterSendMsgDlg::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);
	this->m_reditSend.SetFocus();
}

void CClusterSendMsgDlg::OnBnClickedBtnDiscuss()
{
	if(g_pWebConfig)
	{
		CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_DISCUSS,StrToWideStr(m_pClusterInfo->szFamilyDomain));

		RegUserOperation(0x0032);
		MiniOpenUrl(sUrl);
	}

}

void CClusterSendMsgDlg::OnBnClickedBtnClusterAlubm()
{
	if(g_pWebConfig)
	{
		CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_ALBUM,StrToWideStr(m_pClusterInfo->szFamilyDomain));

		RegUserOperation(0x0033);
		MiniOpenUrl(sUrl);
	}

}

void CClusterSendMsgDlg::OnBnClickedBtnClusterHome()
{
	RegUserOperation(0x0031);
	ShowBlogFamilySite( m_pClusterInfo->szFamilyDomain);
}

void CClusterSendMsgDlg::OnStnClickedImageTitle()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CClusterSendMsgDlg::OnBnClickedAction()
{
#if 0
	CRect rc1;
	m_reditHistory.GetWindowRect(&rc1);
	ScreenToClient(&rc1);

	BOOL bActionBar=m_wndActionToolBar.IsWindowVisible();
	BOOL bFontBar=m_wndTalkToolBar.IsWindowVisible() ;
	BASIC_SYSTEM_SETTING state = g_LocalData.GetBasicSystemSetting();
	state.bShowActionToolbar=!bActionBar;

	if ( bActionBar)
	{
		// 如果当前可视，则隐藏
		rc1.bottom+=26;
		m_wndActionToolBar.ShowWindow(SW_HIDE);
		m_reditHistory.MoveWindow(rc1);	
		// 重置动作
		m_wndActionToolBar.ResetObject();
		m_wndActionToolBar.ResetAction();
	}
	else
	{
		// 如果当前不可视，则显示
		CRect rc2=rc1;
		rc1.bottom-=26;
		m_reditHistory.MoveWindow(rc1);	

		rc2.top = rc1.bottom+2;
		rc2.bottom=rc2.top+24;

		m_wndActionToolBar.MoveWindow(rc2);
		m_wndActionToolBar.ShowWindow(SW_SHOW);
		m_reditHistory.MoveWindow(rc1);	
	}

	m_reditHistory.ScrollToBottom();
	m_reditSend.SetFocus();
	g_LocalData.SaveBasicSystemSetting(state);
#endif
}

LRESULT	CClusterSendMsgDlg::OnStartDownloadFace(WPARAM w, LPARAM l)
{
	if ( m_wndToolBar.StartDownloadFace() )
	{
		m_dwProgressBar = m_reditHistory.FindProgressBar();
		if ( m_dwProgressBar==(DWORD)-1 )
			m_dwProgressBar = this->m_reditHistory.InsertProgressBar();

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

LRESULT	CClusterSendMsgDlg::OnDownloadFaceOK(WPARAM w, LPARAM l)
{
	if(w==0)
	{
		if ( m_dwProgressBar!=(DWORD)-1 )
		{
			m_reditHistory.SetProgressBarTip(m_dwProgressBar,"下载完成");
			m_reditHistory.SetProgressBarPos(m_dwProgressBar,100);
		}

		CString strTip;
		strTip.Format("本次成功下载%d个表情",l);
		AddMsgTip(strTip);
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

LRESULT CClusterSendMsgDlg::OnDownloadProcess(WPARAM w, LPARAM l)
{
	int nTask=(int)w;
	int nProcess=(int)l;
	//CString sTip;
	//sTip.Format("当前下载文件(%d|%d)",nProcess,nTask);
	//AddMsgTip(sTip);
	if ( m_dwProgressBar!=(DWORD)-1 )
	{
		int nPos = nProcess*100.0/nTask;
		this->m_reditHistory.SetProgressBarPos(m_dwProgressBar,nPos);
	}
	return 0;
}

LRESULT CClusterSendMsgDlg::OnInsertMsg(WPARAM w, LPARAM l)
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

UINT CClusterSendMsgDlg::DownloadPicProc( LPVOID pParam )
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


void CClusterSendMsgDlg::ProcTalkMsg(const CString & sMsg,CString& sShowMsg)
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
	EMOTION_ID id;
	size_t i;
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

void CClusterSendMsgDlg::StopDownload(const CString& sFileID,bool bTerminate)
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
		if(m_hWnd)
		{
			KillTimer(TIMER_DOWNLOADOBJ);
		}
	}

}

void CClusterSendMsgDlg::Download(const vector<DOWNLOAD_OBJ>& arrTask)
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
			if(m_hWnd)
				SetTimer(TIMER_DOWNLOADOBJ,300,NULL);
		}
		// 保存当前项到字典
		m_dicDownloadObj[sMD5] = pParam;
	}
}
void CClusterSendMsgDlg::OnButtonLink()
{
	CWnd* pParent = GetDesktopWindow();
	CLinkResDlg* pDlg =  CLinkResDlg::NewDlg(this,pParent);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow(GetDesktopWindow());
}


void CClusterSendMsgDlg::OnStartUploadCPicAck(const char * data,WORD wRet)
{
	int nFileId = (int)data;
	if(wRet != 0)
		return;
	SF_StartUploadCfileAck(nFileId);
}

void CClusterSendMsgDlg::OnUploadCPicAck(const char * data,WORD wRet)
{
	LPXT_UPLOAD_CPIC_ACK pAck = (LPXT_UPLOAD_CPIC_ACK)data;
	if(	wRet == 0 &&
		pAck->nCid == m_pClusterInfo->nFamilyID)
	{
		SF_UploadCfileAck(pAck->nFileId);
	}
}

void CClusterSendMsgDlg::OnDownloadCPicAck(const char * data,WORD wRet)
{
	LPXT_DOWNLOAD_CPIC_ACK pAck = (LPXT_DOWNLOAD_CPIC_ACK)data;
	if(pAck->nCId == m_pClusterInfo->nFamilyID)
	{
		SF_DownloadCfileAck(*pAck);
	}
}

void CClusterSendMsgDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);
}


LRESULT CClusterSendMsgDlg::OnUserLinkClick(WPARAM wParam,LPARAM lParam)
{
#if 0

	int id = (int)wParam;
	const char * szName = (const char *)lParam;
	
	// 往动作工具栏中添加一个聊天对象
	m_wndActionToolBar.AddObject(szName,id);
	m_reditSend.SetFocus();
	m_wndClusterPanel.SelUser(id);
#endif
	return 0;
}

LRESULT CClusterSendMsgDlg::OnUserLinkRBClick(WPARAM wParam,LPARAM lParam)
{
#if 0
	int id = (int)wParam;
	const char * szName = (const char *)lParam;
	m_wndClusterPanel.ShowUserMenu(id);
#endif
	return 0;
}



void CClusterSendMsgDlg::OnBnClickedBtnCfg()
{
	//CLUSTER_INFO * pRealCluster = FindClusterInfo(m_pClusterInfo->nFamilyID);
	// 二级菜单
	CFlatPopupMenu menuMsg;
	HMODULE hModule1 = GetModuleHandle("Mini139_res.dll"); 
	menuMsg.AddImg(hModule1, IDB_PNG_CHOOSE, "PNG");
	menuMsg.Create(AfxGetInstanceHandle());
	menuMsg.AppendItem(0,"接收消息并提示",ID_MENU_REC_NOTICE, m_pClusterInfo->radio == CLUSTER_INFO::RECV_SHOW ? 0 : -1);
	menuMsg.AppendItem(0,"接收消息不提示",ID_MENU_REC_ONLY, m_pClusterInfo->radio == CLUSTER_INFO::RECV_NOSHOW ? 0 : -1);
	menuMsg.AppendItem(0,"显示未读数不提示",ID_MENU_UNREAD_SHOW, m_pClusterInfo->radio == CLUSTER_INFO::SHOW_NUM ? 0 : -1);
	menuMsg.AppendItem(0,"不接收该群的消息",ID_MENU_REJECT_MSG, m_pClusterInfo->radio == CLUSTER_INFO::NORECV ? 0 : -1);

	// 一级菜单
	CFlatPopupMenu menu;
	menu.Create(AfxGetInstanceHandle());
	menu.AppendItem(0,"查看/管理群信息",ID_MENU_VIEW_CLUSTER_INFO);
	menu.AppendItem(0,"修改群名片",ID_MENU_CLUSTER_PROFILE);
	menu.AppendPopup(0,"消息记录",menuMsg);
	menu.AppendItem(0,"退出群",ID_MENU_CLUSTER_EXIT);

	// 弹出菜单
	CRect rcBtn;
	m_btnClusterCfg.GetWindowRect(&rcBtn);
	CPoint pt;
	pt.x=rcBtn.left+rcBtn.Width()/2;
	pt.y=rcBtn.bottom+4;
	int id = menu.Track(pt.x, pt.y, GetSafeHwnd(),true);
	
	// 相应选择菜单的消息相应
	DWORD wP = MAKEWPARAM(id, 0);
	SendMessage(WM_COMMAND, wP, 0);
}

void CClusterSendMsgDlg::OnMenuViewClusterInfo()
{
	// TODO: 在此添加命令处理程序代码
	//跳转到群管理窗口
	
	//CLUSTER_INFO * pCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
	//ASSERT(pCluster);
	if(m_pClusterInfo)
	{
		m_pClusterInfo->dwClusterPageShow = VIEW_CLUSTER_INFO;//跳转指定页面
		CClusterParamSetting::Show(m_pClusterInfo);
	}	
}

void CClusterSendMsgDlg::OnMenuClusterProfile()
{
	// TODO: 在此添加命令处理程序代码
	//CLUSTER_INFO * pCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
	//ASSERT(pCluster);
	if(m_pClusterInfo)
	{
		m_pClusterInfo->dwClusterPageShow = VIEW_CLUSTER_PROFILE;//跳转指定页面
		CClusterParamSetting::Show(m_pClusterInfo);
	}	
}

void CClusterSendMsgDlg::OnMenuClusterExit()
{
	// TODO: 在此添加命令处理程序代码
	CString sMsg;
	int nType;// 2，群主，1群管理员，0普通成员
	nType = m_pClusterInfo->GetMemberType(g_ImUserInfo.GetId());
	if(nType != 2)
	{
		sMsg = "确定要退出该群吗?";
	}
	else
	{
		sMsg = "确定要解散该群吗?";
	}
	if ( CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_OKCANCEL) == IDOK )
	{
		if(nType != 2)
		{
			// 退出群
			XT_CLUSTER_APPLY_REQ req;
			req.nAction = 1;
			req.nFamilyID = m_pClusterInfo->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			req.nUserID = g_ImUserInfo.GetId();
			g_pProtocol->SendReq(req);
		}
		else
		{
			// 解散群
			XT_CLUSTER_DISMISS_REQ req;
			req.nFamilyID = m_pClusterInfo->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			g_pProtocol->SendReq(req);
		}
	}
}

void CClusterSendMsgDlg::OnMenuRecNotice()
{
	//CLUSTER_INFO * pRealCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
	m_pClusterInfo->radio = CLUSTER_INFO::RECV_SHOW;
	m_pClusterInfo->bRefused = false;
	m_pClusterInfo->nMsgNum = 0;
	
}

void CClusterSendMsgDlg::OnMenuRecOnly()
{
	//CLUSTER_INFO * pRealCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
	m_pClusterInfo->radio = CLUSTER_INFO::RECV_NOSHOW;
	m_pClusterInfo->bRefused = true;
	m_pClusterInfo->nMsgNum = 0;
}

void CClusterSendMsgDlg::OnMenuUnreadShow()
{
	//CLUSTER_INFO * pRealCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
	m_pClusterInfo->radio = CLUSTER_INFO::SHOW_NUM;
	m_pClusterInfo->bRefused = true;
	m_pClusterInfo->nMsgNum = 0;
}

void CClusterSendMsgDlg::OnMenuRejectMsg()
{
	//CLUSTER_INFO * pRealCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
	m_pClusterInfo->radio = CLUSTER_INFO::NORECV;
	m_pClusterInfo->bRefused = true;
	m_pClusterInfo->nMsgNum = 0;
}

void CClusterSendMsgDlg::OnBtnMsgLog()
{
	m_bShowHistory = !m_bShowHistory;

	if (m_bShowHistory)
	{
		m_wndPopupHistory.LoadAllMsg();
	}

	UpdateP2PPanel();
}

LRESULT CClusterSendMsgDlg::OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam)
{
	m_bShowHistory = !m_bShowHistory;

	if (m_bShowHistory)
	{
		m_wndPopupHistory.LoadAllMsg();
	}

	UpdateP2PPanel();
	return S_OK;
}

//显示历史5条记录
void CClusterSendMsgDlg::LoadLastTalkMsg()
{
	MSG_LIST ml1;
	// 获取用户聊天记录
	g_LocalData.GetLastMsgList(m_pClusterInfo->nFamilyID,CDataManager::CLUSTER,ml1,5);

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

//LRESULT  CClusterSendMsgDlg::OnSetHeadTitle(WPARAM wParam,LPARAM lParam)
//{
//	CLUSTER_INFO * pCluster     =  (CLUSTER_INFO * )lParam;
//	CLUSTER_INFO * pRealCluster = FindClusterInfo(pCluster->nFamilyID);
//	//显示群名
//	if(pRealCluster!=NULL)
//	{
//       GetDlgItem(IDC_STATIC_USERINFO)->SetFont(&g_font16B); 
//	   CString strFamilyID;
//	   strFamilyID.Format("%d", pRealCluster->nFamilyID);
//	    SetDlgItemText(IDC_STATIC_USERINFO, CString(pRealCluster->szFamilyName) + " [群号：" + 
//		strFamilyID + "]");
//
//	 //显示群描述
//	   GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
//	   SetDlgItemText(IDC_STATIC_SIGN, pRealCluster->szFamilyDesc);
//	}
//	
//	return S_OK;
//}

LRESULT  CClusterSendMsgDlg::OnSnapShot(WPARAM wParam,LPARAM lParam)
{
    OnScreenCopy();
	return S_OK;
}

void CClusterSendMsgDlg::OnBnClickedBtnHotkey()
{
	CRect rc;
	m_btnSend.GetWindowRect(&rc);

	CPoint p;
	int	id;
	TCHAR szBuffer[100];

	// create it
	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(AfxGetInstanceHandle());

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

	p.x=rc.right-24;
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


void CClusterSendMsgDlg::OnEnter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_ENTER;
}


void CClusterSendMsgDlg::OnCtrlenter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_CTRLENTER;
}

// 快捷短语消息相应
void CClusterSendMsgDlg::OnCommandFastWordMenu(int id)
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

		//CLUSTER_INFO *p = FindClusterInfo(m_clusterInfo.nFamilyID);
		if ( m_pClusterInfo && m_pClusterInfo->bRefused )
		{
			m_pClusterInfo->bRefused=false;
		}

		XT_MSG msg;
		SF_InitXtMsg(msg);

		CString text(strMsg.c_str());
		text.Trim();

		if ( text.IsEmpty() )
		{
			return ;
		}

		// 判断是否有动作信息
		ACTION_DATA ad;
		if(m_wndActionToolBar.GetActionData(ad))
		{
			text=ad.ToStr()+text;
			msg.to_id = ad.id;
		}
		else
		{
			// 如果对说话对象没有动作和聊天语言,则返回
			if(text.GetLength()==0)
			{
				m_reditSend.SetFocus();
				return;
			}
			ad.sObject=m_wndActionToolBar.GetCurSelObject();
			if(ad.sObject!="")
			{
				ad.sAction1=" 对";
				ad.sAction2="说：";
				text=ad.ToStr()+text;
				msg.to_id = ad.id;
			}
		}
		if ( text.GetLength()>900 )
		{
			//MessageBox();
			CMessageBox::Prompt("最多输入900个英文字符和450个汉字!");
			return ;
		}

		if ( text.GetLength()==0 )
		{
			m_reditSend.SetFocus();
			return ;
		}

		if ( FindForbiddenWords(text) )
		{
			m_reditSend.SetFocus();
			AddMsgTip("你发送的消息中有限制用语，消息发送失败！");
			return ;
		}
		// 重置动作为无
		m_wndActionToolBar.ResetAction();

		strcpy(msg.data,text);

		msg.data_len=strlen(text)+1;


		// 检测消息中是否有定义信息，并作相应转化
		CString sShowMsg;
		ProcTalkMsg(msg.data,sShowMsg);
		XT_MSG msgShow = msg;
		strncpy(msgShow.data,sShowMsg,sizeof(msgShow.data)-1);
		AddTalkMsg(msgShow,false);

		g_pProtocol->SendClusterMsg(msg);

		RegUserOperation(0x00b6);

		bool bSave=g_LocalData.SaveTalkMsg(msgShow);

		CMainFrame::GetInstance()->GetRecentList()->AddCluster(m_pClusterInfo);
		RECENTLIST_OP(AddCluster(m_pClusterInfo))

		m_reditSend.SetFocus();
	}
}

// 快捷短语按钮消息相应
void CClusterSendMsgDlg::OnBnClickedFastword()
{
	CRect rc;
	m_wndToolBar.m_btnFastword.GetWindowRect(&rc);

	CFlatPopupMenu menu;
	menu.Create(AfxGetInstanceHandle());

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
//////修改群名片后反馈信息
void CClusterSendMsgDlg::OnSetCMProfileAck(const char *data, WORD wRet)
{
	/*LPXT_PROFILENAME_SET_ACK ack = (LPXT_PROFILENAME_SET_ACK)data;
	if (wRet == 0 && ack->nFamilyID == m_clusterInfo.nFamilyID)
	{

		for ( int i=0; i<ack.nCount; i++ )
		{
			XT_CMEM_IFNO   &info = ack.MemInfo[i];
			FRIEND_INFO *pFriend = m_clusterInfo.GetFriendInfo(info.nMemID);
		}

			rp>>info.nMemID>>info.ProfileName;

		FRIEND_INFO *pFriend = m_clusterInfo.GetFriendInfo(CSAConfig::pInstance->m_UserInfo.id);

		if ( pFriend )
		{
			memcpy(pFriend,&CSAConfig::pInstance->m_UserInfo,sizeof(XT_USER_INFO));
			this->m_wndClusterPanel.UpdateClusterList();
		}
	}*/
}


/////修改群信息后服务器反馈
void CClusterSendMsgDlg::OnSetClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_SET_CINFO_ACK ack = (LPXT_SET_CINFO_ACK)data;
	if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_pClusterInfo->nFamilyID )
		{
			//CLUSTER_INFO * pRealCluster = FindClusterInfo(m_pClusterInfo->nFamilyID);
			//显示群名
			if(m_pClusterInfo!=NULL)
			{
				GetDlgItem(IDC_STATIC_USERINFO)->SetFont(&g_font16B); 
				CString strFamilyID;
				strFamilyID.Format("%d", m_pClusterInfo->nFamilyID);
				SetDlgItemText(IDC_STATIC_USERINFO, CString(m_pClusterInfo->szFamilyName) + " [群号：" + 
					strFamilyID + "]");

				//显示群描述
				GetDlgItem(IDC_STATIC_SIGN)->SetFont(&g_font12); 
				SetDlgItemText(IDC_STATIC_SIGN, m_pClusterInfo->szFamilyDesc);
	            /*m_wndClusterPanel.UpdateClusterList();*/
				
			}

			// 发送时钟消息，获取最新的群命名
			SetTimer(0x03, 300, NULL);
		}
	}
	else
	{
		if ( wRet==SERVER_TIMEOUT )
		{
			ShowMessage("修改群信息服务器处理超时！");
		}
		else
		{
			ShowMessage(g_pProtocol->GetLastError());
		}
	}
}


//////向服务器请求获取群名片信息
void CClusterSendMsgDlg::OnGetCMProfileAck(const char *data, WORD wRet)
{
	LPXT_PROFILENAME_GET_ACK ack = (LPXT_PROFILENAME_GET_ACK)data;
	if (wRet == 0 && ack->nFamilyID == m_pClusterInfo->nFamilyID)
	{
		// 先将所有成员的群名片清空
		for(int j = 0;j<m_pClusterInfo->Count();j++)
		{
			FRIEND_INFO *pFriend = &(m_pClusterInfo->members[j]);///获取群组成员信息
			memset(pFriend->profile_name, 0, sizeof(pFriend->profile_name));
		}

		for ( int i=0; i<ack->nCount; i++ )
		{
			XT_CMEM_IFNO   &info = ack->MemInfo[i];
			for(int j = 0;j<m_pClusterInfo->Count();j++)
			{
                FRIEND_INFO *pFriend = &(m_pClusterInfo->members[j]);///获取群组成员信息
				if(pFriend->GetId()  ==  info.nMemID)
			    {
			       strcpy(pFriend->profile_name,info.ProfileName);/////直接赋值群名片
				   break;
			    }
			}
		}
		UpdateProfile(m_pClusterInfo);  //更新群名片显示
	}
}

void CClusterSendMsgDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	m_wndClusterPanel.MoveNoticeDlg();
}

LRESULT CClusterSendMsgDlg::OnKeyDownReturn(WPARAM wParam,LPARAM lParam)
{
	return 2;
}

void CClusterSendMsgDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
