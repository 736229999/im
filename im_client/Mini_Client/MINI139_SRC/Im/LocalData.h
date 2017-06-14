#pragma once


/************************************************************************ 
* 
* �ļ�����localdata.h 
* 
* �ļ�������xtream �������ݹ������ڶ�дim�����ڱ��ص�����
*           ���¼�ʺţ������б������¼��,
* 
* �����ˣ� ѦС��,2005��8��18��
* 
* �汾�ţ�1.0 
* 
* �޸ļ�¼�� 
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

// ������Ϣ�ķ�ʽ

enum EM_SEND_MSG_KEY
{
	SMK_ENTER,		// Enter
	SMK_CTRLENTER,	// Ctrl + Enter
};

// ��������
enum EM_VOICE_TYPE
{
	EVOICE_RECEIVE_MSG,			// �յ���ʱ��Ϣ
	EVOICE_FRIEND_ONLINE,		// ��ϵ������
	EVOICE_SYS_MSG,				// ϵͳ��Ϣ
	EVOICE_SHOCK_SCREEN,		// ������	
	EVOICE_VIDEO_CALL,			// ��Ƶ����
	EVOICE_VOICE_CALL,			// ��������
};

// �ȼ�������Ϣ

typedef struct _HOTKEY_CONFIG
{
	EM_SEND_MSG_KEY sendMsgKey;	// ��Ϣ���ͷ�ʽ
	BOOL			bScreenCopyHideWindow;  //�����Ƿ����ش���
	// ������������

}HOTKEY_CONFIG,*LPHOTKEY_CONFIG;

// ����ϵͳ������Ϣ

typedef struct _BASIC_SYSTEM_SETTING
{
	BOOL		bPlayMagicFace;		// �Ƿ񲥷�ħ������
	BOOL		bShowActionToolbar;	// �Ƿ�Ĭ����ʾ�������鹤����
	BOOL		bShowFriendPicWnd;	// �Ƿ��Զ���ʾ��ע����ͼƬ����
	BOOL		bShowFriendArtWnd;	// �Ƿ��Զ���ʾ��ע�������´���
	BOOL		bShowMiniPictureBroadCast;//�Ƿ��Զ���ʾMiniͼƬ����
	BOOL		bAutoSyncMsgRecord;	// ����ʱ�Ƿ��Զ�ͬ����Ϣ��¼
	BOOL		bAutoClearMsgRecord;// �ر�ʱ�Ƿ��Զ�ɾ����Ϣ��¼
	// ������������
}BASIC_SYSTEM_SETTING,*LPBASIC_SYSTEM_SETTING;


struct tagSavedArticleItem
{
	char    szTitle[MAX_ARTICLE_TITLE_LEN+1];
	DWORD   dwSavedID;
};

//����
struct tagSMSItem
{
	DWORD	dwFriendId;
	char    szUserName[MAX_USERNAME_LEN+1];
	char    szNickName[MAX_NICKNAME_LEN+1];
	char    szTime[32];
	int		nType;//0 send, 1 recv
	char	message[255];
};

// ���˿ռ� - ��������
typedef  struct ZONE_SETTING
{
	char    szBackupPath[255];//����Ŀ¼

} ZONE_SETTING, *LPZONE_SETTING;

//���˿ռ䡡�����ļ�����
typedef  struct FILE_SETTING
{
	char    szRecvFilePath[255];//Ĭ�Ͻ����ļ�·��
	BOOL    bSaveFilePosition ;//�Ƿ�ϵ�����

} FILE_SETTING, *LPFILE_SETTING;

//typedef CList<RegAccountStruct,RegAccountStruct> USER_ACCOUNT_LIST;
typedef std::list<RegAccountStruct>	USER_ACCOUNT_LIST;
typedef CList<XT_MSG,XT_MSG> MSG_LIST;
//typedef CList<SYSTEM_MSG,SYSTEM_MSG> SYSTEM_MSG_LIST;//ϵͳ��Ϣ
typedef CList<tagSavedArticleItem,tagSavedArticleItem> SAVED_ARTICLE_LIST;//δ����������б�
typedef std::vector<tagSrvStatus> SERVER_LIST;
typedef std::vector<tagSMSItem> SMS_LIST;

struct WEB_FACE_SETTING{
	CString		sHost;
	CString		sUrl;
};

struct APP_SETTING{
	
	WEB_FACE_SETTING	webFaceSetting;
	
};

// �Զ��ظ�
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

//������ݶ���
struct FastWordInfo
{
	CString strNum;//��ݱ���
	CString strWord;//��ݶ���
	XT_FONT font;//����
	BOOL bIsLoad;//�Ƿ�Ϊ���صĿ�ݶ���
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

//������ݶ�����
struct FastWordGroupInfo
{
	CString strGroupName;//����
	vector<FastWordInfo> vecFastWordInfo;//��ݶ��Ｏ��
	BOOL bIsLoad;//�Ƿ�Ϊ���صĿ�ݶ���
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

//�̼Ұ���ж�����Ϣ���ѹ���
#ifdef _BUSINESS_VESION
//��������
struct OrderSubscibe
{
	bool orderNew;//�¶��� trueΪ���ģ�falseΪ������
	bool orderPaid;//����Ѹ���
	bool orderEvaluate;//���������
	bool orderTradePopup;//������Ϣ�Ƿ񵯿�����
	bool orderReturn;//�����˻���Ϣ
	bool orderReturnPopup;//�˻���Ϣ�Ƿ񵯿�����
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
	string itemPicUrl;//��ƷͼƬ��ַ
	string itemName;//��Ʒ����
	string itemStandard;//��Ʒ���
	int itemCount;//��Ʒ����
	double itemPrice;//��Ʒ�۸�
	OrderItem()
	{
		itemPicUrl = "";
		itemName = "";
		itemStandard = "";
		itemCount = 0;
		itemPrice = 0.0;
	}
};

//������Ϣ
struct OrderMsg
{
	string orderData;
	string orderUuid;//Ψһ��ʶһ������
	bool bIsRead;
	int32 dwOrderTime;
	string buyerName;//���ID
	int32 orderType;//�������� 0.�¶�����1.����Ѹ��2.��������ۣ�3.�˻���Ϣ
	string orderNum;//������
	string orderPackNum;//������
	string orderTime;//����ʱ��
	double orderPrice;//�������
	double orderFreight;//�˷�
	string orderContent;//�������ݻ��˻�ԭ��
	std::vector<OrderItem*> vecOrderItem;//�����б�
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
	int nTranState;//1.ת������2.ת�Ӿܾ�
	int time;
	string strSystemMsg;
	TranMsg()
	{
		memset(this,0,sizeof(TranMsg));
	}
	
};
//�̼�ID��Ӧ���������ʺ�ID
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

	static HOTKEY_CONFIG		s_hotKeyConfig;			// �ȼ���������
	static BASIC_SYSTEM_SETTING	s_basicSystemSetting;	// ����ϵͳ��������
	static APP_SETTING			s_appSetting;			// Ӧ�ó�������
	static BOOL					s_bAppSetting;

public:
	//------------------------------------------------------
	// ��ȡ��ȡ��Ϣ���ȼ�
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
	// ������ȡ��Ϣ���ȼ�
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

	/////��Ϣδ���˳�,�´���������
	void                SaveCloseTalkWarnConfig(WORD & nVk);
	void                GetCloseTalkWarnConfig(WORD & nVk);

	/////��Ϣδ���˳�,�´ε�¼����
	void                SaveExitMsgWarnConfig(WORD & nVk);
    void                GetExitMsgWarnConfig(WORD & nVk);

    ////�´ε�¼ʱ����⵽δ����Ϣ�Ƿ���ʾ
	void                SaveLoginUnReadMsgCfg(WORD & nVk);
    void                GetLoginUnReadMsgCfg(WORD & nVk);

	////����δ����Ϣ�б����
	void                SaveUnReadParamsCfg(DWORD & nVk,DWORD & nVk2,int nKeyIndex);
    void                GetUnReadParamsCfg(DWORD & nVk,DWORD & nVk2,int nKeyIndex);
	
	////����δ����������
	void                SaveUnReadWndCnt(WORD & nVk);
	void                GetUnReadWndCnt(WORD & nVk);
	//------------------------------------------------------
	// ��ȡ��ǰӦ�ó���汾
	//------------------------------------------------------
	int					GetCurVersion();
	CString             SetDumpFilePath();///����dump�ļ��洢·��
	//------------------------------------------------------
	// ��ȡӦ�ó������ã�ע���ڴ�ǰʹ��::CoInitialize(NULL)
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
	// ���ط������б�
	//------------------------------------------------------
	BOOL LoadServerList(SERVER_LIST & arrSrvList);
	//------------------------------------------------------
	// ����������б�
	//-----------------------------------------------------
	BOOL SaveServerList(const SERVER_LIST & arrSrvList);
	
	//------------------------------------------------------
	// ��ȡ�ȼ�������Ϣ
	//------------------------------------------------------
	HOTKEY_CONFIG* GetHotKeyConfig();
	//------------------------------------------------------
	// ��û���ϵͳ������Ϣ
	//------------------------------------------------------
	BASIC_SYSTEM_SETTING* GetBasicSystemSetting();

	void FlushMsgData();

	void GetImServerList(CStringList &sl);

	void AddUserAccountList(RegAccountStruct regAccount);
	void GetUserAccountList(CString & strUserName,USER_ACCOUNT_LIST &ul);

	void AddSystemMsg(DWORD dwUserId, const SYSTEM_MSG& msg);
	void AddClusterSystemMsg(DWORD dwUserId, const CLUSTER_SYSTEM_MSG &msg);//���Ⱥϵͳ��Ϣ
	void GetSystemMsgList(DWORD dwUserId,SYSTEM_MSG_LIST &sml);
	void GetClusterSystemMsgList(DWORD dwUserId, CLUSTER_SYSTEM_MSG_LIST & sml );//��ȡȺϵͳ��Ϣ
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

/////������Ⱥ��Ϣ��¼
	void LoadClusterExitConfig(char *username);
	void SaveClusterExitConfig(char *username);

	void SaveClusterExitList(char *username);
	void LoadClusterExitList(char *username);

	void SaveAddressBookGroupInfo(const ContactGroupInfo &info);
	bool LoadAddressBookGroupInfo(ContactGroupInfo &info);

	void SaveAddressBook(ADDRESS_BOOK_LIST &addbookList);
	bool LoadAddressBook(ADDRESS_BOOK_LIST &addbookList);

	//��Ƶ����
	int GetDefaultCameraIndex();
	void SetDefaultCameraIndex(int nIndex);

	//���˿ռ�����
	void GetZoneSetting(ZONE_SETTING &setting);
	void SetZoneSetting(const ZONE_SETTING &setting);

	void GetHistoryFaceList(CStringList &slFace);
	void SaveHistoryFaceUrl(CString strUrl);
    
	void GetHistoryFamilyFaceList(CStringList &slFace, long nFamilyID);
	void SaveHistoryFamilyFaceUrl(CString strUrl, long nFamilyID);

	//��ʾ������ʾ
	bool IsShowHelpDemo();
	void SetShowHelpDemo(bool bShow);

	void GetRefuseUsers(vector<uint32> &vecRefuseUser);
	void SaveRefuseUsers(const vector<uint32> &vecRefuseUser);

	//����δ����Ϣ����
	int  LoadWebNotifications(vector<TWebNotificationInfo *> &webNotifications);
	void SaveWebNotifications(const vector<TWebNotificationInfo *> &webNotifications);

	void DeleteSMSFile(DWORD dwFid);
	void GetMobileSMSList(DWORD dwFid, SMS_LIST &sms_list);
	void AddMobileSMS(DWORD dwFid,tagSMSItem item);

	// �Զ��ظ�
	int GetAutoReplyItemCount();
	BOOL GetAutoReplyItem(int nIndex, AUTO_REPLY &item);
	BOOL SetAutoReplyItem(int nIndex, const AUTO_REPLY &item);
	void AddAutoReplyItem(const AUTO_REPLY &item);
	BOOL DeleteAutoReplyItem(int nIndex);
	int GetAllAutoReplyItem(vector<AUTO_REPLY> &allItem);
	void SetAllAutoReplyItem(const vector<AUTO_REPLY> &allItem);

	// ����ʱ�Զ��Ӵ�����
	void GetAutoAccept_Online(AUTO_REPLY &item);
	void SetAutoAccept_Online(const AUTO_REPLY &item);

	// ����ʱ�Զ��Ӵ�����
	void GetAutoAccept_Offline(AUTO_REPLY &item);
	void SetAutoAccept_Offline(const AUTO_REPLY &item);

	// ��ݶ���(��)
	int GetFastWord(vector<string> &vecFastWord);
	void SetFastWord(const vector<string> &vecFastWord);

	//��ݶ���(��)
	
	BOOL CommonCopyFile(CString sourceFileName, CString DestFileName);//�����ļ�
#ifdef _CS_PLAT_EX_	
	void LoadFastWordInfo();//���ӿڻ�ȡ��ݶ���
#endif
	vector<FastWordGroupInfo> GetFastWordInfo();//��ȡ��ݶ���
	void SetFastWordInfo(vector<FastWordGroupInfo>& vecFastWordInfo);//���ÿ�ݶ���

	BOOL ReadFastWordInfo();//�������ļ��ж�ȡ��ݶ���
	void SaveFastWordInfo();//�����ݶ����������ļ�

	BOOL IsGroupLoad(CString strGroupName);
	BOOL IsFastWordLoad(CString strGroupName, int nFastWordIndex);//�ж��Ƿ�Ϊ��Ӫ��̨���صĿ�ݶ���
	CString GetFastWordText(CString strGroupName, int nFastWordIndex);//���ݷ���Ϳ�ݶ����λ�û�ȡ����
	int GetGroupIndex(CString strFastWordGroupName);//��ȡ�����ڵ�λ�ã������ж��Ƿ���ʾ�������Ʋ˵������Ϊ3�������ƺ����ơ����Ϊ2�������ơ����Ϊ1��������
	int GetNodeIndex(CString strGroupName, int nFastWordIndex);//��ȡ��ݶ������ڵ�λ�ã������ж��Ƿ���ʾ�������Ʋ˵������Ϊ3�������ƺ����ơ����Ϊ2�������ơ����Ϊ1��������
	int GetGroupCount();

	void FastWordGroupUp(CString strFastWordGroupName);//������
	void FastWordGroupDown(CString strFastWordGroupName);//������
	void FastWordGroupDelete(CString strFastWordGroupName);//ɾ����
	void FastWordGroupRename(CString strGroupName, CString strChangedGroupName);//��������

	void FastWordUp(CString strGroupName, int nFastWordIndex);//��ݶ�������
	void FastWordDown(CString strGroupName, int nFastWordIndex);//�����������
	void FastWordDelete(CString strGroupName, int nFastWordIndex);//ɾ����ݶ���

	XT_FONT GetFastWordFont(CString strFastWord);//��ȡ��ݶ����Ӧ������

	// ���յ���ͼƬ
	void AcceptFriendPic(const char *pFriendPic);
	void AcceptClusterPic(const char *pClusterPic);

	// ��ȡ���յ��ĺ���ͼƬ�б�
	int GetAcceptedFriendPicList(vector<string> &vecPicList);

	// ��ȡ���յ���ȺͼƬ�б�
	int GetAcceptedClusterPicList(vector<string> &vecPicList);

	// ��պ���ͼƬ�б�
	void ClearAcceptedFriendPicList();

	// ���ȺͼƬ�б�
	void ClearAcceptedClusterPicList();

	// ��ʾ����Ϣ
	string GetVoicePath(EM_VOICE_TYPE type);
	void SetVoicePath(EM_VOICE_TYPE type, const char *pVoicePath);
	BOOL VoiceIsEnable(EM_VOICE_TYPE type);
	void EnableVoice(EM_VOICE_TYPE type, BOOL bEnable);

	// ���������Ϣ��¼
	void DeleteAllMsg();

	void ReadMediaDevSetting();
	void SaveMediaDevSetting();
	void SetMediaDevSetting(MEDIA_DEV_SETTING *setting);
	MEDIA_DEV_SETTING* GetMediaDevSetting();
	int GetVoiceInVolumeSize();
	int GetVoiceOutVolumeSize();
	//------------------------------------------------------
	// �������ļ��л�ȡ�Ƿ���ʾ����ҳ
	//------------------------------------------------------
	BOOL ReadGuidePageSetting(CString strGuidePageDate);
	//------------------------------------------------------
	// �������ϵͳ������Ϣ
	//------------------------------------------------------
	BOOL SaveGuidePageSetting(CString strGuidePageDate);

	// ��ȡ����ͼƬ
	string GetBkImage(const char *szUserName);

	// ���ñ���ͼƬ
	void SetBkImage(const char *szUserName, const char *szBkImg);

	// ��ȡͷ��·��
	string GetHeadImage(const char *szUserName);

	// ����ͷ��
	void SetHeadImage(const char *szUserName, const char *szHeadImg);

	void SetLoginTime(uint32 time);

	uint32 GetTimeNow();

	// ��ȡ����������й����в�����Բ��ͼƬ��·��
	string GetRoundPath(const char *szImgName);

	enum LOG_LEVEL     // ��־����
	{
		LL_ERROR = 1,              
		LL_WARN = 2,               
		LL_INFORMATION = 3         
	};

	// д��־����
	void WriteLog(LOG_LEVEL level, const char *pLogText, ...);
	void WriteLog(string logText, LOG_LEVEL level = LL_ERROR);

	//Base64����
	std::string Base64EncodeServer(const char* szInput, int inByte);
	std::string Base64EncodeServer( const char* szInput);

protected:

	//------------------------------------------------------
	// �������ļ��л�ȡ�ȼ�������Ϣ
	//------------------------------------------------------
	BOOL	ReadHotKeyConfig();
	//------------------------------------------------------
	// �������ļ��л�ȡ����ϵͳ������Ϣ
	//------------------------------------------------------
	BOOL	ReadBasicSystemSetting();

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	BOOL	ReadAppSetting();

	// ��ͼƬ�б��ж�ȡ��¼
	void ReadAcceptedPicList(const char *pPicListFile, vector<string> &vecFicList);

	// ��ȡ���յ��ĺ���ͼƬ�б�
	void ReadAcceptedFriendPicList();

	// ��ȡ���յ���ȺͼƬ�б�
	void ReadAcceptedClusterPicList();

	// ��ͼƬ�б�д���ļ�
	void WriteAcceptedPicList(const char *pPicListFile, const vector<string> &vecFicList);

	// �����յ��ĺ���ͼƬ�б�д���ļ�
	void WriteAcceptedFriendPicList();

	// �����յ���Ⱥ���յ���ȺͼƬ�б�
	void WriteAcceptedClusterPicList();

	// ��ȡ�Զ��ظ���Ϣ�������Զ��Ӵ���
	void ReadAutoReply();

	// �����Զ��ظ���Ϣ�������Զ��Ӵ���
	void SaveAutoReply();

	// ���Զ��ظ����Զ��Ӵ���Ϣ��д��ini�ļ�
	void SetAutoReply(const AUTO_REPLY &item, const char *pSectionName, inifile::IniFile &inifile);

	// ��Ini�ļ��ж�ȡ�Զ��ظ����Զ��Ӵ���Ϣ��
	void GetAutoReply(inifile::IniFile &inifile, const char *pSectionName, AUTO_REPLY &item);

	// ��ȡ��ݶ���
	void ReadFastWord();

	// �����ݶ���
	void SaveFastWord();

	// ��ȡ��ʾ����·��
	void ReadVoiceInfo();

	// ������ʾ����·��
	void SaveVoiceInfo();

	//------------------------------------------------------
	// �������ϵͳ������Ϣ
	//------------------------------------------------------
	BOOL SaveBasicSystemSetting();

	//------------------------------------------------------
	// �����ȼ�������Ϣ
	//------------------------------------------------------
	BOOL SaveHotKeyConfig();

	// �������ļ���ȡ����ͼƬ
	void ReadBkInfo();

	// ���汳��ͼƬ���ļ�
	void SaveBkInfo();

	// ɾ��Բ��ͼƬ
	void RemoveRoundPath();

	// ��ȡ�ϵ������ϵ���ļ�
	void ReadOldRecentDB(vector<FRIEND_INFO_OLD> &vecOldFL);

	// ת��Ϊ�µ��ڴ�
	void RecentDBCovent(vector<FRIEND_INFO_OLD> &vecOldFL, vector<FRIEND_INFO> &vecFL);

	// �����µ������ϵ��db�ļ�
	void SaveNewRecentDbFile(vector<FRIEND_INFO> &vecFL);

	// ��ȡ�����ϵ���µ�sqlite db�ļ�
	void ReadNewRecentDbFile(FRIEND_LIST &fl);

	// �����µĵ�¼�˻��ļ�
	void SaveNewUserAccountListDbFile(USER_ACCOUNT_LIST &ul, const char *szLastUsername);

	// ��ȡ��¼�˻���sqlite db�ļ�
	void ReadNewUserAccountListDbFile(USER_ACCOUNT_LIST &ul, CString &strLastUserName);

	// �����µĺ����б�db�ļ�
	void SaveNewFriendListDbFile(FRIEND_LIST &fl);

	// ��ȡ�µĺ����б��ļ�
	void ReadNewFriendListDbFile(FRIEND_LIST &fl);

	// ����Ⱥ��Ϣ
	void SaveClusterInfo(vector<CLUSTER_INFO *> vecCluster);

	// ��ȡ�µ�Ⱥ�б��ļ�
	void ReadNewClusterList();
public:
	// ����ͷ�����̨��ǰ��������
	void SaveCurrentBuyerDbFile(FRIEND_INFO* pFriend);

	// ɾ���ͷ�����̨��ǰ��������
	void DeleteCurrentBuyerDbFile(int nId);

	//��ȡ������������
	void GetCurrentBuyer(vector<FRIEND_INFO*>& vecFriend);

	void SaveSystemSetting();
	void SaveAllSystemSetting();
#ifdef _BUSINESS_VESION
	//������Ϣ����ѡ��
	void SetOrderSubscibe(OrderSubscibe& orderSubscibeInfo);//���ö�����������
	OrderSubscibe GetOrderSubscibe();//��ȡ������������
	
	//��������
	void AddNewOrder(string orderData, string orderUuid);//����¶���
	void SetNewOrderData(OrderMsg* pOrderMsg, string orderData);//�����¶������ڴ�
	void AddPayOrder(string orderData, string orderUuid);//����Ѹ���
	void SetPayOrderData(OrderMsg* pOrderMsg, string orderData);//�����Ѹ������ڴ�
	void AddGoodsCommentOrder(string orderData, string orderUuid);//���������
	void SetGoodsCommentOrderData(OrderMsg* pOrderMsg, string orderData);//�������������ڴ�
	void AddReturnOrder(string orderData, string orderUuid);//����˻���
	void SetReturnOrderData(OrderMsg* pOrderMsg, string orderData);//�����˻������ڴ�
	void SaveOrderMsg(int nType, const string &orderData, const string &orderUuid);//���涩�����ݵ��������ݿ�
	void ReadOrderMsg();//�ӱ������ݿ��ȡ���ж������ڴ�
	void SetOrderMsgRead(const string &orderUuid);//����һ������Ϊ�Ѷ�״̬
	std::vector<OrderMsg*> GetOrderMsgTrade();//��ȡ�ڴ��н��׶���(�¶������Ѹ��������)
	std::vector<OrderMsg*> GetOrderMsgReturn();//��ȡ�ڴ����˻�������
	int GetOrderTradeUnReadCount();//��ȡδ�����׶���������
	int GetOrderReturnUnReadCount();//��ȡδ���˻�������������
	void RemoveAllOrder();//����ڴ��ж�������

	//�̼�����
	int GetMerchantIdFormSubMerchantId(int nId);//�������ʺ�ID��ȡ�̼�ID
	void GetAllSubMerchantIdFromMerchantId(int nId, std::vector<SubMerchantIdInfo>& vecSubMerchantIdInfo);//�����̼�ID��ȡ�������ʺ�ID�����ʺ�����
	//�Ŷ�����
	int GetCsQueueCount();
	void SetCsTeamQueue(int nTeam, int nCount);
	//����ͼƬ����
	string GetUrlFile(string strUrl);
	void SetUrlFile(string strUrl, string strFile);
	//��ӷ�����Ϣ����
	void AddSendMsgQueue(SendMsgQueue sendMsgQueueInfo, int nSeqOld, int nSeqIdNew);
	BOOL SetSendMsgSeqNewSeq(int nSeqId, int nSeqIdNew, uint32 tick);
	void DeleteSendMsgQueue(int nSeq);
	vector<SendMsgQueue>& GetSendMsgQueue();
	//�豸ID
	uint32 GetDeviceId();
	void SetDeviceId(uint32 nDeviceId);

	//ת������
	void SaveTranMsg(TranMsg* pTranMsg);
	void LoadTranMsg();
	std::vector<TranMsg*> GetTranMsg();
#endif

private:
	CString			m_strPath;			// �û����ݵ�·��
	CString		 	m_strProgramPath;	//����װ·��(moumou.exe)·��
	CString			m_sUser;			// ��ǰ�û�
	CWinAddrBook	m_objWindAddrBook;	
	CDataManager	*m_pMsgData;			// ������Ϣ������
	vector<AUTO_REPLY> m_vecAutoReplyItem;	// �Զ��ظ�	

	AUTO_REPLY m_AutoAcceptOnline;		// ����ʱ�Զ��Ӵ�
	AUTO_REPLY m_AutoAcceptOffline;		// ����ʱ�Զ��Ӵ�

	vector<string> m_vecFastWord;	// ��ݶ���(��)
	vector<FastWordGroupInfo> m_vecFastWordInfo; //��ݶ���(��)

	//�Ŷ�����
	std::map<int, int> m_mapCsTeamToQueue;

	vector<string> m_vecClusterPic;		// ���յ�ȺͼƬ
	vector<string> m_vecFriendPic;		// ���յĺ���ͼƬ
	map<EM_VOICE_TYPE, string> m_mapVoicePath;	// ��ʾ��Ϣ�����ļ�·��	
	map<EM_VOICE_TYPE, BOOL> m_mapVoiceEnable;	// ��ʾ���Ƿ�����	
	MEDIA_DEV_SETTING* m_MediaDevSetting;	// ������Ƶ�豸����
	string m_strBkImage;

	vector<STRU_BK_INFO> m_vecBkInfo;

	uint32 m_timeLogin;//��ȡ�ķ�����ʱ��
	uint32 m_tickLogin;//��ȡ�Լ��ʺŵ�¼��ʱ��

	OrderSubscibe m_orderSubscibeInfo;//��������ѡ��
	vector<OrderMsg*> m_vecOrderMsgTradeInfo;//����������Ϣ
	vector<OrderMsg*> m_vecOrderMsgReturnInfo;//�����˻���Ϣ

	vector<SendMsgQueue> m_vecSendMsgQueue;//������Ϣ����
	CSyncCritical m_lockSendMsg;

	map<string, string> m_mapUrlFile;//����ͼƬ��ַ��Ӧ�ı����ļ�

	vector<TranMsg*> m_vecTranMsgInfo;//�ͷ�ת����Ϣ
	uint32 m_nDeviceId;//�豸ID
	CLog *m_pLog;
};

extern CLocalData		g_LocalData;

// ��ȡgif�ļ���
std::string GetGifName(int i);
// ��ȡgif�ļ�����
int GetGifIndex(const char *szGifName);



