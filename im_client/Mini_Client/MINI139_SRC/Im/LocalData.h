#pragma once


/************************************************************************ 
* 
* 文件名：localdata.h 
* 
* 文件描述：xtream 本地数据管理，用于读写im保存在本地的数据
*           如登录帐号，好友列表，聊天记录等,
* 
* 创建人： 薛小峰,2005年8月18日
* 
* 版本号：1.0 
* 
* 修改记录： 
* 
************************************************************************/ 
#include "impublic.h"
#include "SrvFrameProtocol.h"
#include "winaddrbook.h"

#include "../MiniData/MessageData.h"
#include "../IniFile.h"

#include "..\improtocol.pb.h"
#include "P2PFile.h"
using namespace mini_data;

// 发送信息的方式

enum EM_SEND_MSG_KEY
{
	SMK_ENTER,		// Enter
	SMK_CTRLENTER,	// Ctrl + Enter
};

// 声音类型
enum EM_VOICE_TYPE
{
	EVOICE_RECEIVE_MSG,			// 收到及时消息
	EVOICE_FRIEND_ONLINE,		// 联系人上线
	EVOICE_SYS_MSG,				// 系统消息
	EVOICE_SHOCK_SCREEN,		// 闪屏振动	
	EVOICE_VIDEO_CALL,			// 视频呼叫
	EVOICE_VOICE_CALL,			// 语音呼叫
};

// 热键配置信息

typedef struct _HOTKEY_CONFIG
{
	EM_SEND_MSG_KEY sendMsgKey;	// 信息发送方式
	BOOL			bScreenCopyHideWindow;  //截屏是否隐藏窗口
	// 。。。待增加

}HOTKEY_CONFIG,*LPHOTKEY_CONFIG;

// 基本系统设置信息

typedef struct _BASIC_SYSTEM_SETTING
{
	BOOL		bPlayMagicFace;		// 是否播放魔法表情
	BOOL		bShowActionToolbar;	// 是否默认显示动作表情工具栏
	BOOL		bShowFriendPicWnd;	// 是否自动显示关注好友图片窗口
	BOOL		bShowFriendArtWnd;	// 是否自动显示关注好友文章窗口
	BOOL		bShowMiniPictureBroadCast;//是否自动显示Mini图片联播
	BOOL		bAutoSyncMsgRecord;	// 启动时是否自动同步消息记录
	BOOL		bAutoClearMsgRecord;// 关闭时是否自动删除消息记录
	// 。。。待增加
}BASIC_SYSTEM_SETTING,*LPBASIC_SYSTEM_SETTING;


struct tagSavedArticleItem
{
	char    szTitle[MAX_ARTICLE_TITLE_LEN+1];
	DWORD   dwSavedID;
};

//短信
struct tagSMSItem
{
	DWORD	dwFriendId;
	char    szUserName[MAX_USERNAME_LEN+1];
	char    szNickName[MAX_NICKNAME_LEN+1];
	char    szTime[32];
	int		nType;//0 send, 1 recv
	char	message[255];
};

// 个人空间 - 文章设置
typedef  struct ZONE_SETTING
{
	char    szBackupPath[255];//备份目录

} ZONE_SETTING, *LPZONE_SETTING;

//个人空间　－　文件传输
typedef  struct FILE_SETTING
{
	char    szRecvFilePath[255];//默认接收文件路径
	BOOL    bSaveFilePosition ;//是否断点续传

} FILE_SETTING, *LPFILE_SETTING;

//typedef CList<RegAccountStruct,RegAccountStruct> USER_ACCOUNT_LIST;
typedef std::list<RegAccountStruct>	USER_ACCOUNT_LIST;
typedef CList<XT_MSG,XT_MSG> MSG_LIST;
//typedef CList<SYSTEM_MSG,SYSTEM_MSG> SYSTEM_MSG_LIST;//系统消息
typedef CList<tagSavedArticleItem,tagSavedArticleItem> SAVED_ARTICLE_LIST;//未发表的文章列表
typedef std::vector<tagSrvStatus> SERVER_LIST;
typedef std::vector<tagSMSItem> SMS_LIST;

struct WEB_FACE_SETTING{
	CString		sHost;
	CString		sUrl;
};

struct APP_SETTING{
	
	WEB_FACE_SETTING	webFaceSetting;
	
};

// 自动回复
struct AUTO_REPLY{
	CString strText;
	XT_FONT font;
};

struct STRU_BK_INFO
{
	char szUserName[64];
	char szBkImg[128];
	char szHeadImg[128];

	STRU_BK_INFO()
	{
		memset(szUserName, 0, 64);
		strcpy(szBkImg, "BKImage\\1.png");
		strcpy(szHeadImg, "SysBtn\\icon_100x100.png");
	}

	BOOL operator==(const char *pUserName)
	{
		return strcmp(szUserName, pUserName) == 0;
	}
};

struct MEDIA_DEV_SETTING
{
	int nIdxVoiceInSel;			
	int nIdxVoiceOutSel;	
	int nIdxVedioSel;
	int nVoiceInSize;
	int nVoiceOutSize;
	MEDIA_DEV_SETTING()
	{
		nIdxVoiceInSel = 0;
		nIdxVoiceOutSel = 0;
		nIdxVedioSel = 0;
		nVoiceInSize = 100;
		nVoiceOutSize = 100;
	}
};

//单个快捷短语
struct FastWordInfo
{
	CString strNum;//快捷编码
	CString strWord;//快捷短语
	XT_FONT font;//字体
	BOOL bIsLoad;//是否为下载的快捷短语
	FastWordInfo()
	{
		bIsLoad = FALSE;
		strNum = "";
		strWord = "";
	}
	BOOL operator==(const char *pWord)
	{
		return (strWord == pWord || strNum == pWord);
	}
};

//单个快捷短语组
struct FastWordGroupInfo
{
	CString strGroupName;//组名
	vector<FastWordInfo> vecFastWordInfo;//快捷短语集合
	BOOL bIsLoad;//是否为下载的快捷短语
	FastWordGroupInfo()
	{
		strGroupName = "";
		bIsLoad = FALSE;
	}

	BOOL operator==(const char *pGroupName)
	{
		return (strGroupName == pGroupName);
	}
};

struct SubMerchantIdInfo
{
	BOOL bIsMain;
	int nId;
	std::string strName;
	SubMerchantIdInfo()
	{
		bIsMain = FALSE;
		nId = 0;
		strName = "";
	}
};

//商家版才有订单消息提醒功能
#ifdef _BUSINESS_VESION
//订单订阅
struct OrderSubscibe
{
	bool orderNew;//新订单 true为订阅；false为不订阅
	bool orderPaid;//买家已付款
	bool orderEvaluate;//买家已评价
	bool orderTradePopup;//交易消息是否弹框提醒
	bool orderReturn;//申请退货消息
	bool orderReturnPopup;//退货消息是否弹框提醒
	OrderSubscibe()
	{
		orderNew = true;
		orderPaid = true;
		orderEvaluate = true;
		orderTradePopup = true;
		orderReturn = true;
		orderReturnPopup = true;
	}
};

struct OrderItem
{
	string itemPicUrl;//商品图片地址
	string itemName;//商品名称
	string itemStandard;//商品规格
	int itemCount;//商品数量
	double itemPrice;//商品价格
	OrderItem()
	{
		itemPicUrl = "";
		itemName = "";
		itemStandard = "";
		itemCount = 0;
		itemPrice = 0.0;
	}
};

//交易消息
struct OrderMsg
{
	string orderData;
	string orderUuid;//唯一标识一个订单
	bool bIsRead;
	int32 dwOrderTime;
	string buyerName;//买家ID
	int32 orderType;//订单类型 0.新订单；1.买家已付款；2.买家已评价；3.退货消息
	string orderNum;//订单号
	string orderPackNum;//包裹号
	string orderTime;//订单时间
	double orderPrice;//订单金额
	double orderFreight;//运费
	string orderContent;//评价内容或退货原因
	std::vector<OrderItem*> vecOrderItem;//订单列表
	OrderMsg()
	{
		orderData = "";
		bIsRead = false;
		buyerName = "";
		orderType = -1;
		orderNum = "";
		orderTime = "";
		orderPrice = 0.0;
		orderFreight = 0.0;
		orderContent = "";
	}
};
#endif

struct SendMsgQueue
{
	uint32 seqId;
	XT_MSG msg;
	uint32 sendTick;

	SendMsgQueue()
	{
		memset(this,0,sizeof(XT_MSG));
	}

	BOOL operator==(const int nSeqId)
	{
		return seqId == nSeqId;
	}
};

struct TranMsg
{
	int nFromId;
	string strFromName;
	int nCusterId;
	string strCusterName;
	int nTranState;//1.转接请求；2.转接拒绝
	int time;
	string strSystemMsg;
	TranMsg()
	{
		memset(this,0,sizeof(TranMsg));
	}
	
};
//商家ID对应的所有子帐号ID
class CLog;
class CLocalData : public CObject 
{
public:

	CLocalData();

	virtual ~CLocalData();

	void	SetPath(CString strPath);


	void 	SetProgramPath(CString strPath);


	void	SetUser(const CString & sUser, const CString &sUserFileName);

	static	BYTE key[16];

private:

	static HOTKEY_CONFIG		s_hotKeyConfig;			// 热键配置数据
	static BASIC_SYSTEM_SETTING	s_basicSystemSetting;	// 基本系统设置数据
	static APP_SETTING			s_appSetting;			// 应用程序设置
	static BOOL					s_bAppSetting;

public:
	//------------------------------------------------------
	// 获取提取消息的热键
	//------------------------------------------------------
	void				GetMsgHotKey(WORD & nVk,WORD & nMod);
	void                GetSnapShotHotKey(WORD & nVk,WORD & nMod);
    void                GetWakeupConfig(WORD & nVk,WORD & nStatus);
	void                GetShakeWinConfig(WORD & nVk);
	void                GetTopShowConfig(WORD & nVk);
	void                GetHideWinConfig(WORD & nVk);
	void                GetExitWarnConfig(WORD & nVk);
	void                GetVoiceConfig(CString &strVoice,int nVoiceID);
	void				GetFileTransConfig(WORD & nVk);
	void 				GetSoftUpdateConfig(WORD & nVk);
	void 				GetSoftUpdateIgnoreConfig(WORD & nVk);

	
	




	//------------------------------------------------------
	// 保存提取消息的热键
	//------------------------------------------------------
	void				SaveMsgHotKey(WORD nVk,WORD nMod);
	void				SaveSnapShotHotKey(WORD nVk,WORD nMod);
	void                SaveShakeWinConfig(WORD nVk);
	void                SaveWakeupConfig(WORD  nVk,WORD  nStatus);
	void                SaveTopShowConfig(WORD & nVk);
	void                SaveHideWinConfig(WORD & nVk);
	void                SaveExitWarnConfig(WORD & nVk);
	void                SaveVoiceConfig(CString strVoice,int nVoiceID);
	void 				SaveFileTransConfig(WORD nVk);
	void 				SaveSoftUpdateConfig(WORD nVk);
	void 				SaveSoftUpdateIgnoreConfig(WORD nVk);

	/////消息未读退出,下次聊天提醒
	void                SaveCloseTalkWarnConfig(WORD & nVk);
	void                GetCloseTalkWarnConfig(WORD & nVk);

	/////消息未读退出,下次登录提醒
	void                SaveExitMsgWarnConfig(WORD & nVk);
    void                GetExitMsgWarnConfig(WORD & nVk);

    ////下次登录时，检测到未读消息是否显示
	void                SaveLoginUnReadMsgCfg(WORD & nVk);
    void                GetLoginUnReadMsgCfg(WORD & nVk);

	////保存未读消息列表参数
	void                SaveUnReadParamsCfg(DWORD & nVk,DWORD & nVk2,int nKeyIndex);
    void                GetUnReadParamsCfg(DWORD & nVk,DWORD & nVk2,int nKeyIndex);
	
	////保存未读窗口总数
	void                SaveUnReadWndCnt(WORD & nVk);
	void                GetUnReadWndCnt(WORD & nVk);
	//------------------------------------------------------
	// 获取当前应用程序版本
	//------------------------------------------------------
	int					GetCurVersion();
	CString             SetDumpFilePath();///设置dump文件存储路径
	//------------------------------------------------------
	// 获取应用程序设置，注意在此前使用::CoInitialize(NULL)
	//------------------------------------------------------
	const APP_SETTING &	GetAppSetting();
	//------------------------------------------------------
	// 
	//------------------------------------------------------
	bool				IsAppSettingExist() const;
	//------------------------------------------------------
	// 
	//------------------------------------------------------
	const CString& GetPath() const
	{ return m_strPath; };
	//------------------------------------------------------
	// 
	//------------------------------------------------------
	CString GetImageCanchePath() const
	{
		CString sRt;
		sRt.Format("%sdata\\%s\\ImageCache",m_strPath,g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
		return sRt;
	}

	const CString& GetProgramPath()const 
	{
		return m_strProgramPath;
	}


	//------------------------------------------------------
	// 加载服务器列表
	//------------------------------------------------------
	BOOL LoadServerList(SERVER_LIST & arrSrvList);
	//------------------------------------------------------
	// 保存服务器列表
	//-----------------------------------------------------
	BOOL SaveServerList(const SERVER_LIST & arrSrvList);
	
	//------------------------------------------------------
	// 获取热键定义信息
	//------------------------------------------------------
	HOTKEY_CONFIG* GetHotKeyConfig();
	//------------------------------------------------------
	// 获得基本系统设置信息
	//------------------------------------------------------
	BASIC_SYSTEM_SETTING* GetBasicSystemSetting();

	void FlushMsgData();

	void GetImServerList(CStringList &sl);

	void AddUserAccountList(RegAccountStruct regAccount);
	void GetUserAccountList(CString & strUserName,USER_ACCOUNT_LIST &ul);

	void AddSystemMsg(DWORD dwUserId, const SYSTEM_MSG& msg);
	void AddClusterSystemMsg(DWORD dwUserId, const CLUSTER_SYSTEM_MSG &msg);//添加群系统消息
	void GetSystemMsgList(DWORD dwUserId,SYSTEM_MSG_LIST &sml);
	void GetClusterSystemMsgList(DWORD dwUserId, CLUSTER_SYSTEM_MSG_LIST & sml );//获取群系统消息
	BOOL ClearSystemMessage(uint32 userid);

	bool SaveTalkMsg(const XT_MSG & msg, uint32 fid=0);
	void GetMsgList(DWORD dwFid, CDataManager::MSG_TYPE nMsgType, MSG_LIST &ml);
	void GetLastMsgList(DWORD dwFid, CDataManager::MSG_TYPE nMsgType, MSG_LIST &ml,int nCount);

	BOOL DeleteP2PTalkFile(uint32 fid);
	BOOL DeleteClusterTalkFile(uint32 cid);

	void SaveFriendList(FRIEND_LIST &fl);
	void GetFriendList(FRIEND_LIST &fl);

	void SaveRecentFriends(FRIEND_LIST &fl);
	void GetRecentFriends(FRIEND_LIST &fl);
	
	void SaveGroupInfo(const GROUP_INFO &info);
	void GetGroupInfo(GROUP_INFO &info);
	
	void SaveImageFileToCustomFace(const CString& strImageFilePathName, CString &strCustomFaceFileName);
	void SaveImageFileToCache(const CString& strImageFilePathName, CString &strCacheFileName);
	
	CString GetImageFileCachePath();
	CString GetGifFacePath(int i);
	CString GetCustomFacePath();
	CString GetWebIconPath();
	CString GetWebFilePath();

	void SaveXtFont(const XT_FONT &xf);
	void GetXtFont(XT_FONT &xf);

	void SaveCurrentSelFloor(int nFloor);
	int  GetLastSelFloor();

	void SaveCurrentSelTab(int nTab);
	int  GetLastSelTab();

	void SaveLastUploadPicFolder(CString strFolder);
	void GetLastUploadPicFolder(CString & strFolder);

	void SaveUserRecvFileSetting(const FILE_SETTING &setting );
	void GetUserRecvFileSetting(FILE_SETTING &setting);

	void SaveMainFramePosition(const CRect& rect);
	void SaveSubWndWidth(const CString & sWnd,int nWidth);
	int	 GetSubWndWidth(const CString & sWnd);
	void SaveSubWndMode(const CString & sWnd,int nMode);
	int  GetSubWndMode(const CString & sWnd);
	void GetMainFramePostion(CRect &rect);	
	bool IsShowAllFriend() const;
	void SetShowAllFriend(bool bAll);

	void LoadClusterConfig(char *username);
	void SaveClusterConfig(char *username);

	void SaveClusterList(char *username);
	void LoadClusterList(char *username);

/////保存退群消息记录
	void LoadClusterExitConfig(char *username);
	void SaveClusterExitConfig(char *username);

	void SaveClusterExitList(char *username);
	void LoadClusterExitList(char *username);

	void SaveAddressBookGroupInfo(const ContactGroupInfo &info);
	bool LoadAddressBookGroupInfo(ContactGroupInfo &info);

	void SaveAddressBook(ADDRESS_BOOK_LIST &addbookList);
	bool LoadAddressBook(ADDRESS_BOOK_LIST &addbookList);

	//视频设置
	int GetDefaultCameraIndex();
	void SetDefaultCameraIndex(int nIndex);

	//个人空间设置
	void GetZoneSetting(ZONE_SETTING &setting);
	void SetZoneSetting(const ZONE_SETTING &setting);

	void GetHistoryFaceList(CStringList &slFace);
	void SaveHistoryFaceUrl(CString strUrl);
    
	void GetHistoryFamilyFaceList(CStringList &slFace, long nFamilyID);
	void SaveHistoryFamilyFaceUrl(CString strUrl, long nFamilyID);

	//显示帮助演示
	bool IsShowHelpDemo();
	void SetShowHelpDemo(bool bShow);

	void GetRefuseUsers(vector<uint32> &vecRefuseUser);
	void SaveRefuseUsers(const vector<uint32> &vecRefuseUser);

	//返回未读消息个数
	int  LoadWebNotifications(vector<TWebNotificationInfo *> &webNotifications);
	void SaveWebNotifications(const vector<TWebNotificationInfo *> &webNotifications);

	void DeleteSMSFile(DWORD dwFid);
	void GetMobileSMSList(DWORD dwFid, SMS_LIST &sms_list);
	void AddMobileSMS(DWORD dwFid,tagSMSItem item);

	// 自动回复
	int GetAutoReplyItemCount();
	BOOL GetAutoReplyItem(int nIndex, AUTO_REPLY &item);
	BOOL SetAutoReplyItem(int nIndex, const AUTO_REPLY &item);
	void AddAutoReplyItem(const AUTO_REPLY &item);
	BOOL DeleteAutoReplyItem(int nIndex);
	int GetAllAutoReplyItem(vector<AUTO_REPLY> &allItem);
	void SetAllAutoReplyItem(const vector<AUTO_REPLY> &allItem);

	// 在线时自动接待短语
	void GetAutoAccept_Online(AUTO_REPLY &item);
	void SetAutoAccept_Online(const AUTO_REPLY &item);

	// 离线时自动接待短语
	void GetAutoAccept_Offline(AUTO_REPLY &item);
	void SetAutoAccept_Offline(const AUTO_REPLY &item);

	// 快捷短语(旧)
	int GetFastWord(vector<string> &vecFastWord);
	void SetFastWord(const vector<string> &vecFastWord);

	//快捷短语(新)
	
	BOOL CommonCopyFile(CString sourceFileName, CString DestFileName);//复制文件
#ifdef _CS_PLAT_EX_	
	void LoadFastWordInfo();//调接口获取快捷短语
#endif
	vector<FastWordGroupInfo> GetFastWordInfo();//获取快捷短语
	void SetFastWordInfo(vector<FastWordGroupInfo>& vecFastWordInfo);//设置快捷短语

	BOOL ReadFastWordInfo();//从配置文件中读取快捷短语
	void SaveFastWordInfo();//保存快捷短语至配置文件

	BOOL IsGroupLoad(CString strGroupName);
	BOOL IsFastWordLoad(CString strGroupName, int nFastWordIndex);//判断是否为运营后台下载的快捷短语
	CString GetFastWordText(CString strGroupName, int nFastWordIndex);//根据分组和快捷短语的位置获取内容
	int GetGroupIndex(CString strFastWordGroupName);//获取组所在的位置，用于判断是否显示上移下移菜单，结果为3则不能上移和下移、结果为2则不能下移、结果为1则不能上移
	int GetNodeIndex(CString strGroupName, int nFastWordIndex);//获取快捷短语所在的位置，用于判断是否显示上移下移菜单，结果为3则不能上移和下移、结果为2则不能下移、结果为1则不能上移
	int GetGroupCount();

	void FastWordGroupUp(CString strFastWordGroupName);//组上移
	void FastWordGroupDown(CString strFastWordGroupName);//组下移
	void FastWordGroupDelete(CString strFastWordGroupName);//删除组
	void FastWordGroupRename(CString strGroupName, CString strChangedGroupName);//组重命名

	void FastWordUp(CString strGroupName, int nFastWordIndex);//快捷短语上移
	void FastWordDown(CString strGroupName, int nFastWordIndex);//短语短语下移
	void FastWordDelete(CString strGroupName, int nFastWordIndex);//删除快捷短语

	XT_FONT GetFastWordFont(CString strFastWord);//获取快捷短语对应的字体

	// 接收到的图片
	void AcceptFriendPic(const char *pFriendPic);
	void AcceptClusterPic(const char *pClusterPic);

	// 获取接收到的好友图片列表
	int GetAcceptedFriendPicList(vector<string> &vecPicList);

	// 获取接收到的群图片列表
	int GetAcceptedClusterPicList(vector<string> &vecPicList);

	// 清空好友图片列表
	void ClearAcceptedFriendPicList();

	// 清空群图片列表
	void ClearAcceptedClusterPicList();

	// 提示音信息
	string GetVoicePath(EM_VOICE_TYPE type);
	void SetVoicePath(EM_VOICE_TYPE type, const char *pVoicePath);
	BOOL VoiceIsEnable(EM_VOICE_TYPE type);
	void EnableVoice(EM_VOICE_TYPE type, BOOL bEnable);

	// 清空所有消息记录
	void DeleteAllMsg();

	void ReadMediaDevSetting();
	void SaveMediaDevSetting();
	void SetMediaDevSetting(MEDIA_DEV_SETTING *setting);
	MEDIA_DEV_SETTING* GetMediaDevSetting();
	int GetVoiceInVolumeSize();
	int GetVoiceOutVolumeSize();
	//------------------------------------------------------
	// 从配置文件中获取是否显示引导页
	//------------------------------------------------------
	BOOL ReadGuidePageSetting(CString strGuidePageDate);
	//------------------------------------------------------
	// 保存基本系统设置信息
	//------------------------------------------------------
	BOOL SaveGuidePageSetting(CString strGuidePageDate);

	// 获取背景图片
	string GetBkImage(const char *szUserName);

	// 设置背景图片
	void SetBkImage(const char *szUserName, const char *szBkImg);

	// 获取头像路径
	string GetHeadImage(const char *szUserName);

	// 设置头像
	void SetHeadImage(const char *szUserName, const char *szHeadImg);

	void SetLoginTime(uint32 time);

	uint32 GetTimeNow();

	// 获取保存程序运行过程中产生的圆形图片的路径
	string GetRoundPath(const char *szImgName);

	enum LOG_LEVEL     // 日志级别
	{
		LL_ERROR = 1,              
		LL_WARN = 2,               
		LL_INFORMATION = 3         
	};

	// 写日志操作
	void WriteLog(LOG_LEVEL level, const char *pLogText, ...);
	void WriteLog(string logText, LOG_LEVEL level = LL_ERROR);

	//Base64加密
	std::string Base64EncodeServer(const char* szInput, int inByte);
	std::string Base64EncodeServer( const char* szInput);

protected:

	//------------------------------------------------------
	// 从配置文件中获取热键定义信息
	//------------------------------------------------------
	BOOL	ReadHotKeyConfig();
	//------------------------------------------------------
	// 从配置文件中获取基本系统设置信息
	//------------------------------------------------------
	BOOL	ReadBasicSystemSetting();

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	BOOL	ReadAppSetting();

	// 从图片列表中读取记录
	void ReadAcceptedPicList(const char *pPicListFile, vector<string> &vecFicList);

	// 读取接收到的好友图片列表
	void ReadAcceptedFriendPicList();

	// 读取接收到的群图片列表
	void ReadAcceptedClusterPicList();

	// 将图片列表写入文件
	void WriteAcceptedPicList(const char *pPicListFile, const vector<string> &vecFicList);

	// 将接收到的好友图片列表写入文件
	void WriteAcceptedFriendPicList();

	// 读接收到的群接收到的群图片列表
	void WriteAcceptedClusterPicList();

	// 读取自动回复信息（包含自动接待）
	void ReadAutoReply();

	// 保存自动回复信息（包含自动接待）
	void SaveAutoReply();

	// 将自动回复（自动接待信息）写入ini文件
	void SetAutoReply(const AUTO_REPLY &item, const char *pSectionName, inifile::IniFile &inifile);

	// 从Ini文件中读取自动回复（自动接待信息）
	void GetAutoReply(inifile::IniFile &inifile, const char *pSectionName, AUTO_REPLY &item);

	// 读取快捷短语
	void ReadFastWord();

	// 保存快捷短语
	void SaveFastWord();

	// 读取提示声音路径
	void ReadVoiceInfo();

	// 保存提示声音路径
	void SaveVoiceInfo();

	//------------------------------------------------------
	// 保存基本系统设置信息
	//------------------------------------------------------
	BOOL SaveBasicSystemSetting();

	//------------------------------------------------------
	// 保存热键定义信息
	//------------------------------------------------------
	BOOL SaveHotKeyConfig();

	// 从配置文件读取背景图片
	void ReadBkInfo();

	// 保存背景图片到文件
	void SaveBkInfo();

	// 删除圆形图片
	void RemoveRoundPath();

	// 读取老的最近联系人文件
	void ReadOldRecentDB(vector<FRIEND_INFO_OLD> &vecOldFL);

	// 转换为新的内存
	void RecentDBCovent(vector<FRIEND_INFO_OLD> &vecOldFL, vector<FRIEND_INFO> &vecFL);

	// 保存新的最近联系人db文件
	void SaveNewRecentDbFile(vector<FRIEND_INFO> &vecFL);

	// 读取最近联系人新的sqlite db文件
	void ReadNewRecentDbFile(FRIEND_LIST &fl);

	// 保存新的登录账户文件
	void SaveNewUserAccountListDbFile(USER_ACCOUNT_LIST &ul, const char *szLastUsername);

	// 读取登录账户的sqlite db文件
	void ReadNewUserAccountListDbFile(USER_ACCOUNT_LIST &ul, CString &strLastUserName);

	// 保存新的好友列表db文件
	void SaveNewFriendListDbFile(FRIEND_LIST &fl);

	// 读取新的好友列表文件
	void ReadNewFriendListDbFile(FRIEND_LIST &fl);

	// 保存群信息
	void SaveClusterInfo(vector<CLUSTER_INFO *> vecCluster);

	// 读取新的群列表文件
	void ReadNewClusterList();
public:
	// 保存客服工作台当前聊天的买家
	void SaveCurrentBuyerDbFile(FRIEND_INFO* pFriend);

	// 删除客服工作台当前聊天的买家
	void DeleteCurrentBuyerDbFile(int nId);

	//获取正在聊天的买家
	void GetCurrentBuyer(vector<FRIEND_INFO*>& vecFriend);

	void SaveSystemSetting();
	void SaveAllSystemSetting();
#ifdef _BUSINESS_VESION
	//订单消息订阅选项
	void SetOrderSubscibe(OrderSubscibe& orderSubscibeInfo);//设置订单订阅内容
	OrderSubscibe GetOrderSubscibe();//获取订单订阅内容
	
	//订单数据
	void AddNewOrder(string orderData, string orderUuid);//添加新订单
	void SetNewOrderData(OrderMsg* pOrderMsg, string orderData);//保存新订单至内存
	void AddPayOrder(string orderData, string orderUuid);//添加已付款
	void SetPayOrderData(OrderMsg* pOrderMsg, string orderData);//保存已付款至内存
	void AddGoodsCommentOrder(string orderData, string orderUuid);//添加已评价
	void SetGoodsCommentOrderData(OrderMsg* pOrderMsg, string orderData);//保存已评价至内存
	void AddReturnOrder(string orderData, string orderUuid);//添加退换货
	void SetReturnOrderData(OrderMsg* pOrderMsg, string orderData);//保存退换货至内存
	void SaveOrderMsg(int nType, const string &orderData, const string &orderUuid);//保存订单数据到本地数据库
	void ReadOrderMsg();//从本地数据库读取所有订单至内存
	void SetOrderMsgRead(const string &orderUuid);//设置一个订单为已读状态
	std::vector<OrderMsg*> GetOrderMsgTrade();//获取内存中交易订单(新订单、已付款、已评价)
	std::vector<OrderMsg*> GetOrderMsgReturn();//获取内存中退换货订单
	int GetOrderTradeUnReadCount();//获取未读交易订单的数量
	int GetOrderReturnUnReadCount();//获取未读退换货订单的数量
	void RemoveAllOrder();//清空内存中订单数据

	//商家数据
	int GetMerchantIdFormSubMerchantId(int nId);//根据子帐号ID获取商家ID
	void GetAllSubMerchantIdFromMerchantId(int nId, std::vector<SubMerchantIdInfo>& vecSubMerchantIdInfo);//根据商家ID获取所有子帐号ID和子帐号类型
	//排队数据
	int GetCsQueueCount();
	void SetCsTeamQueue(int nTeam, int nCount);
	//订单图片数据
	string GetUrlFile(string strUrl);
	void SetUrlFile(string strUrl, string strFile);
	//添加发送消息队列
	void AddSendMsgQueue(SendMsgQueue sendMsgQueueInfo, int nSeqOld, int nSeqIdNew);
	BOOL SetSendMsgSeqNewSeq(int nSeqId, int nSeqIdNew, uint32 tick);
	void DeleteSendMsgQueue(int nSeq);
	vector<SendMsgQueue>& GetSendMsgQueue();
	//设备ID
	uint32 GetDeviceId();
	void SetDeviceId(uint32 nDeviceId);

	//转接数据
	void SaveTranMsg(TranMsg* pTranMsg);
	void LoadTranMsg();
	std::vector<TranMsg*> GetTranMsg();
#endif

private:
	CString			m_strPath;			// 用户数据的路径
	CString		 	m_strProgramPath;	//程序安装路径(moumou.exe)路径
	CString			m_sUser;			// 当前用户
	CWinAddrBook	m_objWindAddrBook;	
	CDataManager	*m_pMsgData;			// 聊天消息管理器
	vector<AUTO_REPLY> m_vecAutoReplyItem;	// 自动回复	

	AUTO_REPLY m_AutoAcceptOnline;		// 在线时自动接待
	AUTO_REPLY m_AutoAcceptOffline;		// 在线时自动接待

	vector<string> m_vecFastWord;	// 快捷短语(旧)
	vector<FastWordGroupInfo> m_vecFastWordInfo; //快捷短语(新)

	//排队人数
	std::map<int, int> m_mapCsTeamToQueue;

	vector<string> m_vecClusterPic;		// 接收的群图片
	vector<string> m_vecFriendPic;		// 接收的好友图片
	map<EM_VOICE_TYPE, string> m_mapVoicePath;	// 提示消息声音文件路径	
	map<EM_VOICE_TYPE, BOOL> m_mapVoiceEnable;	// 提示音是否启动	
	MEDIA_DEV_SETTING* m_MediaDevSetting;	// 语音视频设备设置
	string m_strBkImage;

	vector<STRU_BK_INFO> m_vecBkInfo;

	uint32 m_timeLogin;//获取的服务器时间
	uint32 m_tickLogin;//获取自己帐号登录的时间

	OrderSubscibe m_orderSubscibeInfo;//订单订阅选项
	vector<OrderMsg*> m_vecOrderMsgTradeInfo;//订单交易消息
	vector<OrderMsg*> m_vecOrderMsgReturnInfo;//订单退货消息

	vector<SendMsgQueue> m_vecSendMsgQueue;//发送消息队列
	CSyncCritical m_lockSendMsg;

	map<string, string> m_mapUrlFile;//订单图片地址对应的本地文件

	vector<TranMsg*> m_vecTranMsgInfo;//客服转接消息
	uint32 m_nDeviceId;//设备ID
	CLog *m_pLog;
};

extern CLocalData		g_LocalData;

// 获取gif文件名
std::string GetGifName(int i);
// 获取gif文件索引
int GetGifIndex(const char *szGifName);



