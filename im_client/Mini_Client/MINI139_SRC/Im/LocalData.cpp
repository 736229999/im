// im\LocalData.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "LocalData.h"
#include "xtpacket.h"
#include <io.h>

#include "SpaceTypeDef.h"
#include "..\..\..\Excel\CSVFile.h"
#include "MessageHandleWnd.h"
#import "msxml3.dll" 

#include "CppSQLite3.h"
#include "Log.h"
#include "LogManager.h"

#include "curl.h"
#include "../json/json/json.h"
#include "..\..\HmgCrtLib\hmgcrtlib.h"
#include "DuiOrderNotice.h"
#include "MsgDlg.h"

#define DB_KEY "№☆★↓〓―♂々～‖タネニщвм"
#define DB_KEY_L 32

using namespace inifile;

map<DWORD,DWORD>  g_mapServiceLog;////保存好友ID和状态参数
map<DWORD,CString>  g_mapServiceURL;////GUID为空时，保存对应的买家ID
CString g_strToken = "";
map<int, CString> g_mapFriendIdToOrder;//买家ID对应的订单号，
// 替换Sql中的特殊字符
CString SqlEncode(CString theString)
{
	theString.Replace("\"", "&quot;");
	theString.Replace("\'", "&#39;");
	theString.Replace("\n", "<br/>");
	return theString;
}

// Sql中的特殊字符还原
CString SqlDecode(CString theString)
{
	theString.Replace("&quot;", "\"");
	theString.Replace("&#39;", "\'");
	theString.Replace("<br/>", "\n");
	return theString;
}

bool ItemSortByTime(OrderMsg *pOrderMsg1, OrderMsg *pOrderMsg2 )
{
	int32 dwTime1 = pOrderMsg1->dwOrderTime;
	int32 dwTime2 = pOrderMsg2->dwOrderTime;
	return dwTime1 > dwTime2;
}
// CLocalData

const int DATA_VERSION				= ((int)0x00000002);
const int ACCEPT_DATA_VERSION		= ((int)0x00000002);
const int MSG_DATA_VERSION			= ((int)0x00000001);
const int ACCEPT_MSG_DATA_VERSION	= ((int)0x00000001);

// （新）好友信息的版本
const int VER_FRIEND_INFO = 1;
// （新）群信息的版本
const int VER_CLUSTER_INFO = 1;
// （新）最近登录账户信息的版本
const int VER_NEAR_ACCOUNT_INFO = 1;
//客服工作台当前聊天买家
const int VER_CURRENT_BUYER_INFO = 1;
//获取订单消息版本
const int VER_ORDER_MSG_INFO = 1;


// 默认热键
const HOTKEY_CONFIG DEFAULT_HOTKEY_CONFIG={
	SMK_ENTER,
	false
};

BYTE CLocalData::key[16]={0x01,0x7f,0x0c,0x5e,0x02,0x35,0x8d,0x42,0x9e,0x10,0x31,0x23,0x45,0x63,0x25,0x13};	

HOTKEY_CONFIG CLocalData::s_hotKeyConfig = DEFAULT_HOTKEY_CONFIG;
BASIC_SYSTEM_SETTING CLocalData::s_basicSystemSetting;
BOOL CLocalData::s_bAppSetting = FALSE;
APP_SETTING CLocalData::s_appSetting;
map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////保存未读消息的好友ID和未读条数
#define CSCTAN_VER	1		// csctan.dat文件的版本号
#define BKINFO_VER	1		// 背景文件版本号
CLocalData::CLocalData()
{
#ifdef _CS_PLAT_EX_
	m_AutoAcceptOnline.strText = "";
#else
	m_AutoAcceptOnline.strText = "欢迎光临";
#endif
	m_AutoAcceptOnline.font = g_ImFont;
	m_AutoAcceptOnline.font.fontSize = 12;

#ifdef _CS_PLAT_EX_
	m_AutoAcceptOffline.strText = "";
#else
	m_AutoAcceptOffline.strText = "欢迎光临";
#endif
	m_AutoAcceptOffline.font = g_ImFont;
	m_AutoAcceptOffline.font.fontSize = 12;

	m_vecFastWord.push_back("哦");
	m_vecFastWord.push_back("好了，好了，我知道了");
	m_vecFastWord.push_back("不会是真的吧？");

	m_MediaDevSetting = NULL;

	m_strBkImage = "BKImage\\1.png";

	m_timeLogin = time(0);
	m_tickLogin = GetTickCount();

	m_pMsgData = new CDataManager();
	CString strModulePath =  g_LocalData.GetPath();
	g_LocalData.SetPath(strModulePath);

	//快捷短语默认存在一个未分组
	FastWordGroupInfo fastWordGroupInfo;
	fastWordGroupInfo.strGroupName = "未分组";
	m_vecFastWordInfo.push_back(fastWordGroupInfo);

	m_nDeviceId = rand();
	m_pLog = NULL;
}

CLocalData::~CLocalData()
{
	// 保存原接收图片列表
	if(!m_strPath.IsEmpty() && !m_sUser.IsEmpty())
	{
		WriteAcceptedFriendPicList();
		WriteAcceptedClusterPicList();

		SaveAutoReply();

		// 保存快捷短语
		SaveFastWord();

		// 保存提示音信息
		SaveVoiceInfo();

		// 保存基本配置信息
		SaveBasicSystemSetting();

		// 保存热键配置
		SaveHotKeyConfig();

		// 保存字体
		SaveXtFont(g_ImFont);

		SaveMediaDevSetting();

		SaveBkInfo();

		if(m_MediaDevSetting != NULL)
		{
			delete m_MediaDevSetting;
			m_MediaDevSetting = NULL;
		}

		RemoveRoundPath();

		if(m_pMsgData != NULL)
		{
			delete m_pMsgData;
			m_pMsgData = NULL;
		}

		// 清空消息记录
		if(s_basicSystemSetting.bAutoClearMsgRecord)
		{
			DeleteAllMsg();
		}

		RemoveAllOrder();

		// 关闭log文件
		CString strUserLog;
		strUserLog.Format("%s\\%s\\log.txt",m_strPath, m_sUser);
		LogManager::RemoveLog((LPCTSTR)strUserLog);
	}

	if(m_pMsgData != NULL)
	{
		delete m_pMsgData;
		m_pMsgData = NULL;
	}
	
	for(int nTranMsg = 0; nTranMsg < m_vecTranMsgInfo.size(); nTranMsg++)
	{
		TranMsg* pTranMsg = m_vecTranMsgInfo[nTranMsg];
		if (pTranMsg != NULL)
		{
			delete pTranMsg;
			pTranMsg = NULL;
		}
	}
}

void CLocalData::SaveSystemSetting()
{
	if(!m_strPath.IsEmpty() && !m_sUser.IsEmpty())
	{
		//保存自动回复
		SaveAutoReply();
		// 保存快捷短语
		SaveFastWord();
		// 保存提示音信息
		SaveVoiceInfo();
		// 保存基本配置信息
		SaveBasicSystemSetting();
		// 保存热键配置
		SaveHotKeyConfig();

		SaveMediaDevSetting();
	}
}

void CLocalData::SaveAllSystemSetting()
{
	if(!m_strPath.IsEmpty() && !m_sUser.IsEmpty())
	{
		WriteAcceptedFriendPicList();
		WriteAcceptedClusterPicList();

		SaveAutoReply();

		// 保存快捷短语
		SaveFastWord();

		// 保存提示音信息
		SaveVoiceInfo();

		// 保存基本配置信息
		SaveBasicSystemSetting();

		// 保存热键配置
		SaveHotKeyConfig();

		// 保存字体
		SaveXtFont(g_ImFont);

		SaveMediaDevSetting();

		SaveBkInfo();

		// 关闭log文件
		CString strUserLog;
		strUserLog.Format("%s\\%s\\log.txt",m_strPath, m_sUser);
		LogManager::RemoveLog((LPCTSTR)strUserLog);
	}
}

void CLocalData::FlushMsgData()
{
	m_pMsgData->FlushAllMsg();
}

void CLocalData::SetPath(CString strPath)
{
	strPath.Trim();
/*
	if ( strPath.Right(1)!="\\" )
		strPath+="\\";
*/

	if ( _access(strPath,0)==-1 )
	{
		CreateDirectory(strPath,NULL);
	}

	m_strPath = strPath;
	m_pMsgData->SetAppDir(m_strPath);

	ReadBkInfo();
}


void CLocalData::SetProgramPath(CString strPath)
{
	strPath.Trim();

	if ( strPath.Right(1)!="\\" )
		strPath+="\\";

	m_strProgramPath = strPath;
}


void CLocalData::SetUser(const CString & sUser, const CString &sUserFileName)
{
	// 保存原接收图片列表
	if(!m_strPath.IsEmpty() && !m_sUser.IsEmpty())
	{
		WriteAcceptedFriendPicList();
		WriteAcceptedClusterPicList();

		// 保存自动回复信息
		SaveAutoReply();

		// 保存快捷短语
		SaveFastWord();

		// 保存提示音信息
		SaveVoiceInfo();

		// 保存基本配置信息
		SaveBasicSystemSetting();

		// 保存热键配置
		SaveHotKeyConfig();

		// 保存字体
		SaveXtFont(g_ImFont);
		SaveMediaDevSetting();

		RemoveRoundPath();

		if(m_pMsgData != NULL)
		{
			delete m_pMsgData;
			m_pMsgData = NULL;
		}

		// 清空消息记录
		if(s_basicSystemSetting.bAutoClearMsgRecord)
		{
			DeleteAllMsg();
		}

		// 关闭log文件
		CString strUserLog;
		strUserLog.Format("%s\\%s\\log.txt",m_strPath, m_sUser);
		LogManager::RemoveLog((LPCTSTR)strUserLog);
	}

	m_sUser = sUserFileName;
	if(m_pMsgData == NULL)
	{
		m_pMsgData = new CDataManager();
		CString strModulePath =  g_LocalData.GetPath();
		g_LocalData.SetPath(strModulePath);
	}

	m_pMsgData->SetCurUser(sUserFileName);

	// 读取新用户接收图片列表
	if(!m_strPath.IsEmpty() && !m_sUser.IsEmpty())
	{
		// 打开log文件
		CString strUserFolder;
		strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);
		if ( _access(strUserFolder,0)==-1 )
		{
			CreateDirectory(strUserFolder,NULL);
		}
		CString strUserLog;
		strUserLog.Format("%s\\%s\\log.txt",m_strPath, m_sUser);
		LogManager::RemoveLog((LPCTSTR)strUserLog);
		m_pLog = LogManager::OpenLog((LPCTSTR)strUserLog, CLog::LL_INFORMATION);

		ReadAcceptedFriendPicList();
		ReadAcceptedClusterPicList();

		// 读取新的自动回复信息
		ReadAutoReply();

		// 读取快捷短语
		ReadFastWord();

		// 读取提示音信息
		ReadVoiceInfo();

		// 读取基本配置信息
		ReadBasicSystemSetting();

		// 读取热键配置
		ReadHotKeyConfig();

		// 读取字体
		GetXtFont(g_ImFont);
		ReadMediaDevSetting();		
	}
}

//获取 IM 服务器列表
void CLocalData::GetImServerList(CStringList &sl)
{
	CString strImIni = m_strPath;
	strImIni+="imserver.ini";

	int nSiteCount = GetPrivateProfileInt("IMServer","count",10, strImIni);

	CString strName;
	CString strDefault;
	char    szValue[255];

	sl.RemoveAll();

	for ( int i=0; i<nSiteCount; i++ )
	{
		strName.Format("site%d",i+1);
		strDefault.Format("site%d.hmg.cn",i+1);
		GetPrivateProfileString("IMServer",strName,strDefault,szValue,255,strImIni);

		sl.AddTail(szValue);
	}
}

//获取登录帐户列表
// 返回值
//   dwLstXid 上次最后登录的xid
//   ul  登录帐户列表
void CLocalData::GetUserAccountList(CString & strUserName, USER_ACCOUNT_LIST &ul)
{
	CString strNew = m_strPath+"\\csctaNew.db";
	if(FileExist(strNew))
	{	// 存在，读取	
		ReadNewUserAccountListDbFile(ul, strUserName);
	}
	else
	{	
		BOOL bIsOldFile = FALSE;
		CString strFile = m_strPath+"\\csctaN.dat";
		CStdioFile file;
		ul.clear();
		if ( file.Open(strFile,CFile::modeRead|CFile::typeBinary) )
		{
			try
			{
				uint8 uVer = CSCTAN_VER;			// 版本号
				file.Read(&uVer, sizeof(uint8));
				if(uVer == 1)
				{
					char cUsername[MAX_USERNAME_LEN+1] = {0};
					file.Read(cUsername,sizeof(cUsername));
					strUserName=cUsername;

					RegAccountStruct reg;
					char crypt[1024];
					long crypt_len;

					TEAEncrypt((char *)&reg,sizeof(reg),(char *)key,
						crypt,&crypt_len);

					int nRead = 0;

					while ( (nRead=file.Read(crypt,crypt_len))>0 )
					{
						RegAccountStruct r;
						long plain_len;

						TEADecrypt(crypt,crypt_len,(char *)key,
							(char *)&r,&plain_len);

						if ( plain_len!=sizeof(r) )
						{
							bIsOldFile = TRUE;
							break;
						}
						else
						{
							ul.push_back(r);
							TRACE("\n%s, %s",r.cUsername,r.cPass );
						}
					}
				}
			}
			catch(...)
			{
				IM_LOG("GetUserAccountList exception!");
				ul.clear();
			}

			file.Close();
		}
		if(bIsOldFile)
		{
			struct RegAccountStruct_Last//for save to local file use
			{
				char    cUsername[MAX_USERNAME_LEN_LAST+1];
				char	cPass[MAX_PASSWORD_LEN_LAST+1];
				BOOL	bHide;		//隐身登录
				int		iLoginFlag; //登录状态
				BOOL	bAutoLogin;	//自动登录
				BOOL	bRemeberPsw; //记住密码
				BOOL    bShowHint;
				DWORD	dwServerIP;	//登录server
				RegAccountStruct_Last()
				{
					memset(this, 0, sizeof(RegAccountStruct_Last));
				}
			};

			if ( file.Open(strFile,CFile::modeRead|CFile::typeBinary) )
			{
				try
				{
					uint8 uVer = CSCTAN_VER;			// 版本号
					file.Read(&uVer, sizeof(uint8));
					if(uVer == 1)
					{
						char cUsername[MAX_USERNAME_LEN_LAST+1] = {0};
						file.Read(cUsername,sizeof(cUsername));
						strUserName=cUsername;

						RegAccountStruct_Last reg;
						char crypt[1024];
						long crypt_len;

						TEAEncrypt((char *)&reg,sizeof(reg),(char *)key,
							crypt,&crypt_len);

						int nRead = 0;

						while ( (nRead=file.Read(crypt,crypt_len))>0 )
						{
							RegAccountStruct_Last r;
							long plain_len;

							TEADecrypt(crypt,crypt_len,(char *)key,
								(char *)&r,&plain_len);

							if ( plain_len!=sizeof(r) )
							{
								bIsOldFile = TRUE;
								break;
							}
							else
							{
								RegAccountStruct r_new;
								strcpy(r_new.cUsername, r.cUsername);
								strcpy(r_new.cPass, r.cPass);
								r_new.bHide = r.bHide;
								r_new.iLoginFlag = r.iLoginFlag;
								r_new.bAutoLogin = r.bAutoLogin;
								r_new.bRemeberPsw = r.bRemeberPsw;
								r_new.bShowHint = r.bShowHint;
								r_new.dwServerIP = r.dwServerIP;

								ul.push_back(r_new);
								TRACE("\n%s, %s",r_new.cUsername,r_new.cPass );
							}
						}
					}
				}
				catch(...)
				{
					IM_LOG("GetUserAccountList exception!");
					ul.clear();
				}

				file.Close();
			}
		}

		// 写入新的文件
		SaveNewUserAccountListDbFile(ul, strUserName);
		rename(strFile, strFile+".bak");
	}	
}

void CLocalData::AddUserAccountList(RegAccountStruct regAccount)
{	
	USER_ACCOUNT_LIST ul;
	USER_ACCOUNT_LIST::iterator it,it2,itE;

	CString strUserName;
	GetUserAccountList(strUserName,ul);

	// 删除旧的配置文件
	CString strFileOld = m_strPath+"\\cscta.dat";
	DeleteFile(strFileOld);
	
	// 过滤重复的纪录
	int i = 0;
	ul.push_front(regAccount);
	itE = ul.end();
	CString sName;
	for(it = ul.begin();it != itE;it++)
	{
		sName = it->cUsername;
		it2 = it;
		it2++;
		for(;it2 != itE;)
		{
			if(sName == it2->cUsername)
			{
				TRACE("!!!出现重复的帐号%s!!!\n",(LPCTSTR)sName);
				it2 = ul.erase(it2);
			}
			else
			{
				it2++;
			}
		}
		
	}
	
	// 保存新记录
	SaveNewUserAccountListDbFile(ul, regAccount.cUsername);
}

BOOL CLocalData::ClearSystemMessage(uint32 userid)
{
	CString str;
	str.Format("%s\\%s\\sd%u.msg",m_strPath,m_sUser,userid);
	return DeleteFile(str);
}

BOOL CLocalData::DeleteClusterTalkFile(uint32 cid)
{
	//CString str;
	//str.Format("%s\\%s\\cd%u.db",m_strPath,g_ImUserInfo.GetUserName().c_str(),cid);
	//return DeleteFile(str);
	return m_pMsgData->DeleteClusterTalkFile(cid);
}

BOOL CLocalData::DeleteP2PTalkFile(uint32 fid)
{
	//CString str;
	//str.Format("%s\\%s\\fd%u.db",m_strPath,g_ImUserInfo.GetUserName().c_str(),fid);
	return m_pMsgData->DeleteUserTalkFile(fid);
	//return DeleteFile(str);
}

void CLocalData::LoadClusterConfig(char *username)
{
	CString str;
	str.Format("%s\\%s\\ctp.dat",m_strPath,username);

	struct tagCfg
	{
		uint32   cid;
		bool     bShow;
		bool     bRefuse;
	} record;

	CStdioFile file;
	int nVersion = 0;
	if ( file.Open(str,CFile::modeRead|CFile::typeBinary) )
	{
		DWORD dwRead=0;
		file.Read(&nVersion,sizeof(nVersion));
		if(nVersion < ACCEPT_DATA_VERSION)
		{
			assert(0 && "群配置信息版本不兼容");
			TRACE("群配置信息版本不兼容");
			file.Close();
			return;
		}
		try
		{
			while(1)
			{
				dwRead = file.Read(&record,sizeof(record) );

				if (dwRead==sizeof(record))
				{
					CLUSTER_INFO *p = FindClusterInfo(record.cid);
					if ( p )
					{
						p->bAutoShowMagic=record.bShow;
						p->bRefused      =record.bRefuse;
					}
					else
					{
						//ASSERT(0);
					}

				}
				else
				{
					break;
				}

			}
		}
		catch(...)
		{
			ASSERT(0);
		}

		file.Close();
	}
}

void CLocalData::SaveClusterList(char *username)
{
	CString strFile;
	strFile.Format("%s\\%s\\clm.dat",m_strPath,username);
	DeleteFile(strFile);

	SaveClusterInfo(g_vecCluster);
}

void CLocalData::LoadClusterList(char *username)
{
	CString strFile;
	strFile.Format("%s\\%s\\clm.dat",m_strPath,username);
	DeleteFile(strFile);
	ClearClusterList();
	ReadNewClusterList();
}

void CLocalData::SaveClusterConfig(char *username)
{
	CString str;
	str.Format("%s\\%s\\ctp.dat",m_strPath,username);
	DeleteFile(str);

	struct tagCfg
	{
		uint32  cid;
		bool    bShow;
		bool    bRefuse;
	} record;

	CStdioFile file;

	if ( file.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		file.Write(&DATA_VERSION,sizeof(DATA_VERSION));
		for ( int i=0; i<g_vecCluster.size(); i++ )
		{
			record.cid     = g_vecCluster[i]->nFamilyID;
			record.bShow   = g_vecCluster[i]->bAutoShowMagic;
			record.bRefuse = g_vecCluster[i]->bRefused;

			file.Write(&record,sizeof(record));
		}
		file.Close();
	}
}

//////已退群消息记录
void CLocalData::SaveClusterExitList(char *username)
{
	//CString strFile;
	//strFile.Format("%s\\%s\\clmexit.dat",m_strPath,username);
	//DeleteFile(strFile);

	//char szHead[255] = {0} ;
	//long nClusterCount     ;

	//strcpy(szHead,username);
	//nClusterCount  = g_vecClusterExit.size();

	//CStdioFile f;
	//size_t i,c;

	//if ( f.Open(strFile,CFile::typeBinary|CFile::modeCreate|CFile::modeWrite) )
	//{
	//	// 写入版本
	//	f.Write(&DATA_VERSION,sizeof(DATA_VERSION));
	//	f.Write(szHead,sizeof(szHead));
	//	f.Write(&nClusterCount,sizeof(nClusterCount));

	//	for ( long n=0; n<nClusterCount; n++ )
	//	{
	//		CLUSTER_INFO *pCluster = g_vecClusterExit[n];
	//		c = pCluster->Count();
	//		f.Write(&c,sizeof(size_t));
	//		f.Write(&pCluster->bRefused,sizeof(pCluster->bRefused));
	//		f.Write(&pCluster->bBasicInfo,sizeof(pCluster->bBasicInfo));
	//		f.Write(&pCluster->radio, sizeof(pCluster->radio));
	//		f.Write(&pCluster->nMsgNum, sizeof(pCluster->nMsgNum));
	//		f.Write(pCluster,sizeof(XT_CLUSTER_INFO));

	//		for ( i = 0; i < c; i++ )
	//		{
	//			f.Write(&pCluster->members[i],sizeof(FRIEND_INFO));
	//		}
	//	}
	//	f.Close();
	//}

}

void CLocalData::LoadClusterExitList(char *username)
{
//	CString strFile;
//	strFile.Format("%s\\%s\\clmexit.dat",m_strPath,username);
//
//	char szHead[255] = {0} ;
//	long nClusterCount     ;
//	int nVersion = 0;
//	size_t members = 0;
//	size_t i = 0;
//	int nLen = 0;
//	FRIEND_INFO finf;
//
//	CStdioFile file;
//
//	ClearClusterExitList();
//
//
//	if ( file.Open(strFile,CFile::modeRead|CFile::typeBinary) )
//	{
//		long nFileLength = file.GetLength();
//
//		if ( nFileLength>=(255+sizeof(nClusterCount)+sizeof(int)))
//		{
//			// 读出版本
//			file.Read(&nVersion,sizeof(nVersion));
//			if(nVersion < ACCEPT_DATA_VERSION)
//			{
//				TRACE("加载群列表版本不兼容\n");
//				file.Close();
//				assert(0 && "加载群列表版本不兼容");
//				return;
//			}
//			file.Read(szHead,sizeof(szHead));
//
//			if ( stricmp(szHead,username)!=0 )
//			{
//				file.Close();
//				ASSERT(0);
//				return ;
//			}
//
//			file.Read(&nClusterCount,sizeof(nClusterCount));
//
//			try
//			{
//				for (int n=0; n<nClusterCount; n++ )
//				{
//					CLUSTER_INFO *pCluster = new CLUSTER_INFO();
//
//					file.Read(&members,sizeof(size_t));
//					file.Read(&pCluster->bRefused,sizeof(pCluster->bRefused));
//					file.Read(&pCluster->bBasicInfo,sizeof(pCluster->bBasicInfo));
//					if(nVersion >= DATA_VERSION)
//					{
//						file.Read(&pCluster->radio, sizeof(pCluster->radio));
//						file.Read(&pCluster->nMsgNum, sizeof(pCluster->nMsgNum));
//					}
//					file.Read(pCluster,sizeof(XT_CLUSTER_INFO));
//					if(members > MAX_CLUSTER_MEMBERS * 2)
//						break;
//					for ( i = 0; i< members; i++ )
//					{
//						nLen = file.Read(&finf,sizeof(FRIEND_INFO));
//						if(nLen <= 0)
//							break;
//						finf.status = XTREAM_OFFLINE;
//						pCluster->members.push_back(finf);				
//					}
//
//					g_vecClusterExit.push_back(pCluster);
//				}
//			}
//			catch(...)
//			{
//				//	ClearClusterList();
//				goto EXIT;
//			}
//		}
//EXIT:
//		file.Close();
//	}
}



void CLocalData::LoadClusterExitConfig(char *username)
{
	//CString str;
	//str.Format("%s\\%s\\ctpexit.dat",m_strPath,username);

	//struct tagCfg
	//{
	//	uint32   cid;
	//	bool     bShow;
	//	bool     bRefuse;
	//} record;

	//CStdioFile file;
	//int nVersion = 0;
	//if ( file.Open(str,CFile::modeRead|CFile::typeBinary) )
	//{
	//	DWORD dwRead=0;
	//	file.Read(&nVersion,sizeof(nVersion));
	//	if(nVersion < ACCEPT_DATA_VERSION)
	//	{
	//		assert(0 && "群配置信息版本不兼容");
	//		TRACE("群配置信息版本不兼容");
	//		file.Close();
	//		return;
	//	}
	//	try
	//	{
	//		while(1)
	//		{
	//			dwRead = file.Read(&record,sizeof(record) );

	//			if (dwRead==sizeof(record))
	//			{
	//				CLUSTER_INFO *p = FindClusterInfo(record.cid);
	//				if ( p )
	//				{
	//					p->bAutoShowMagic=record.bShow;
	//					p->bRefused      =record.bRefuse;
	//				}
	//				else
	//				{
	//					//ASSERT(0);
	//				}

	//			}
	//			else
	//			{
	//				break;
	//			}

	//		}
	//	}
	//	catch(...)
	//	{
	//		ASSERT(0);
	//	}

	//	file.Close();
	//}
}

void CLocalData::SaveClusterExitConfig(char *username)
{
	/*CString str;
	str.Format("%s\\%s\\ctpexit.dat",m_strPath,username);
	DeleteFile(str);

	struct tagCfg
	{
		uint32  cid;
		bool    bShow;
		bool    bRefuse;
	} record;

	CStdioFile file;

	if ( file.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		file.Write(&DATA_VERSION,sizeof(DATA_VERSION));
		for ( int i=0; i<g_vecClusterExit.size(); i++ )
		{
			record.cid     = g_vecClusterExit[i]->nFamilyID;
			record.bShow   = g_vecClusterExit[i]->bAutoShowMagic;
			record.bRefuse = g_vecClusterExit[i]->bRefused;

			file.Write(&record,sizeof(record));
		}
		file.Close();
	}*/
}

void CLocalData::GetSystemMsgList(DWORD dwUserId, SYSTEM_MSG_LIST & sml )
{
	m_pMsgData->GetAllSysMsg(sml,dwUserId);
}

void CLocalData::GetClusterSystemMsgList(DWORD dwUserId, CLUSTER_SYSTEM_MSG_LIST & sml )
{
	m_pMsgData->GetClusterSysMsg(sml,dwUserId);
}

void CLocalData::DeleteSMSFile(DWORD dwFid)
{	
	CString strFile;
	strFile.Format("%s\\%s\\sms%u.dat",m_strPath,m_sUser,dwFid);
	DeleteFile(strFile);
}

void CLocalData::GetMobileSMSList(DWORD dwFid, SMS_LIST &sms_list )
{
	CString strFile;
	strFile.Format("%s\\%s\\sms%u.dat",m_strPath,m_sUser,dwFid);

	CStdioFile objFile;

	if ( objFile.Open(strFile,CFile::modeRead|CFile::typeBinary) )
	{
		tagSMSItem item;

		int nFlag;
		objFile.Read(&nFlag,sizeof(int));

		if ( nFlag == sizeof(item) )
		{
			while ( objFile.Read(&item,sizeof(item))==sizeof(item) )
			{
				sms_list.push_back(item);
			}
		}

		objFile.Close();
	}
}

void CLocalData::AddMobileSMS(DWORD dwFid,tagSMSItem item)
{
	CString strFile;
	strFile.Format("%s\\%s\\sms%u.dat",m_strPath,m_sUser,dwFid);
	
	if ( FileExist(strFile) )
	{
		CStdioFile objFile;
		if ( objFile.Open(strFile,CFile::modeWrite|CFile::typeBinary) )
		{
			objFile.SeekToEnd();
			objFile.Write(&item,sizeof(item));
			objFile.Close();
		}
	}
	else
	{
		CStdioFile objFile;
		if ( objFile.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
		{
			int nFlag = sizeof(item);
			objFile.Write(&nFlag,sizeof(int));
			objFile.Write(&item,sizeof(item));
			objFile.Close();
		}
	}
}


void CLocalData::GetMsgList(DWORD dwFid,
							CDataManager::MSG_TYPE nMsgType, 
							MSG_LIST &ml)
{
	m_pMsgData->GetAllMsg(ml,dwFid,nMsgType);
}

void CLocalData::GetLastMsgList(DWORD dwFid,
								CDataManager::MSG_TYPE nMsgType, 
								MSG_LIST &ml,
								int nCount )
{
	m_pMsgData->GetLastMsg(ml,dwFid,nMsgType,nCount);
}

void CLocalData::AddSystemMsg(DWORD dwUserId, const SYSTEM_MSG &msg)
{
	m_pMsgData->SaveSysMsg(dwUserId,msg);
}

void CLocalData::AddClusterSystemMsg(DWORD dwUserId, const CLUSTER_SYSTEM_MSG &msg)
{
	m_pMsgData->SaveClusterSysMsg(dwUserId,msg);
}

bool CLocalData::SaveTalkMsg(const XT_MSG &msg, uint32 fid)
{
	return m_pMsgData->SaveMsg(msg,fid);
}

void CLocalData::SaveFriendList(FRIEND_LIST &fl)
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s",m_strPath,m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	SaveNewFriendListDbFile(fl);
}

//获取最近联系人列表
void CLocalData::GetRecentFriends(FRIEND_LIST &fl)
{
	// 先读取老版本的文件
	CString strFile;
	strFile.Format("%s\\%s\\recent.db",m_strPath,m_sUser);
	if(_access(strFile, 0) != -1)
	{	// 存在
		vector<FRIEND_INFO_OLD> vecOldFL;
		ReadOldRecentDB(vecOldFL);

		vector<FRIEND_INFO> vecNewFL;
		RecentDBCovent(vecOldFL, vecNewFL);

		// 重命名
		rename(strFile, strFile+".bak");

		// 写入到新文件中
		SaveNewRecentDbFile(vecNewFL);
	}
	
	// 读取新的db文件
	ReadNewRecentDbFile(fl);
}


//保存最近联系人列表
void CLocalData::SaveRecentFriends(FRIEND_LIST &fl)
{
	// 写入到新文件中
	vector<FRIEND_INFO> vecNewFL;

	FRIEND_LIST::iterator it;
	for ( it=fl.begin(); it!=fl.end(); it++ )
	{
		FRIEND_INFO *pFriend = (*it);
		vecNewFL.push_back(*pFriend);
	}
	SaveNewRecentDbFile(vecNewFL);
}


//获取本地好友列表
void CLocalData::GetFriendList(FRIEND_LIST &fl)
{
	CString strFile;
	strFile.Format("%s\\%s\\fl.db",m_strPath,m_sUser);
	if(_access(strFile,0)!=-1)
	{	// 删除文件
		CStdioFile file;
		int nVersion = 0;

		if ( file.Open(strFile,CFile::modeRead|CFile::typeBinary) )
		{
			// 读出版本
			file.Read(&nVersion,sizeof(nVersion));
			if(nVersion != ACCEPT_DATA_VERSION)
			{
				file.Close();
				return ;
			}

			vector<FRIEND_INFO_OLD> vecFriendO;
			while ( 1 )
			{
				FRIEND_INFO_OLD *pFriend=new FRIEND_INFO_OLD;

				if ( file.Read(pFriend,sizeof(FRIEND_INFO_OLD))==sizeof(FRIEND_INFO_OLD) )
				{				
					pFriend->ip = 0;
					pFriend->port = 0;
					pFriend->localIP = 0;
					pFriend->localPort = 0;
					pFriend->status = XTREAM_OFFLINE;
					pFriend->p2pRetry = 3;
					pFriend->connFlag = FRIEND_INFO::NONE;
					pFriend->pUserWebArticleInfo = NULL;
					pFriend->pUserWebPictureInfo = NULL;
					vecFriendO.push_back(*pFriend);		
					delete pFriend;
				}
				else
				{
					delete pFriend;
					break;
				}
			}

			file.Close();

			vector<FRIEND_INFO> vecFriendN;
			RecentDBCovent(vecFriendO, vecFriendN);
			FRIEND_LIST fl;
			for(int i = 0; i < (int)vecFriendN.size(); ++i)
			{
				fl.push_back(&vecFriendN[i]);
			}
			SaveNewFriendListDbFile(fl);
		}

		rename(strFile, strFile+".bak");
	}

	ClearList(fl);

	ReadNewFriendListDbFile(fl);
}

void CLocalData::GetGroupInfo(GROUP_INFO &info)
{
	CString strFile;
	strFile.Format("%s\\%s\\gp.db",m_strPath,m_sUser);

	CStdioFile file;
	int nVersion = 0;

	if ( file.Open(strFile,CFile::modeRead|CFile::typeBinary) )
	{
		char buff[8096];
		long crypt_len,plain_len;

		try
		{
			// 读出版本
			file.Read(&nVersion,sizeof(nVersion));
			if(nVersion < ACCEPT_DATA_VERSION)
			{
				file.Close();
				return ;
			}

			TEAEncrypt((char *)&info,sizeof(info),
				(char *)key,
				buff,&crypt_len);
			if ( file.Read(buff,crypt_len)==crypt_len )
			{
				if (TEADecrypt(buff,crypt_len,
					(char *)key,
					buff,&plain_len))
				{
					memcpy(&info,buff,plain_len);
				}			
			}
		}
		catch(...)
		{
		}

		file.Close();
	}

	// 卖家版的“我的团队”分组现在是按照添加普通分组方式添加的，不是默认的，所以先注释掉，
	// 如果以后改为默认分组了再取消注释
//#ifdef _BUSINESS_VESION
//	if (g_GroupInfo.count < 4)
//	{
//		g_GroupInfo.count=4;
//
//		g_GroupInfo.group[0].group_index = 0;
//		strcpy(g_GroupInfo.group[0].name, "我的团队");
//
//		g_GroupInfo.group[1].group_index = 1;
//		strcpy(g_GroupInfo.group[1].name, "我的好友");
//
//		g_GroupInfo.group[2].group_index = 2;
//		strcpy(g_GroupInfo.group[2].name, "陌生人");
//
//		g_GroupInfo.group[3].group_index = 3;
//		strcpy(g_GroupInfo.group[3].name, "黑名单");
//	}
//#else
	if ( g_GroupInfo.count<3 )
	{
#ifdef _CS_PLAT_EX_  //飞牛客服工作台(去掉好友组)
		g_GroupInfo.count = 2;

		g_GroupInfo.group[0].group_index = 1;
		strcpy(g_GroupInfo.group[0].name, "陌生人");

		g_GroupInfo.group[1].group_index = 2;
		strcpy(g_GroupInfo.group[1].name, "黑名单");
#else
		g_GroupInfo.count = 3;

		g_GroupInfo.group[0].group_index = 0;
		strcpy(g_GroupInfo.group[0].name, "我的好友");

		g_GroupInfo.group[1].group_index = 1;
		strcpy(g_GroupInfo.group[1].name, "陌生人");

		g_GroupInfo.group[2].group_index = 2;
		strcpy(g_GroupInfo.group[2].name, "黑名单");
#endif
		
	}
//#endif	

	info.Sort();
}

void CLocalData::SaveGroupInfo(const GROUP_INFO &info)
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s",m_strPath,m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	CString strFile;
	strFile.Format("%s\\%s\\gp.db",m_strPath,m_sUser);
	//DeleteFile(strFile);

	CStdioFile file;

	GROUP_INFO temp=info;

	if ( file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		char buff[8096];
		long buff_len;
		// 写入版本
		file.Write(&DATA_VERSION,sizeof(DATA_VERSION));

		temp.Sort();

		TEAEncrypt((char *)&temp,sizeof(temp),
			(char *)key,
			buff,&buff_len);

		file.Write(buff,buff_len);

		file.Close();
	}

}

void CLocalData::SaveImageFileToCustomFace(const CString& strImageFilePathName,CString &strCustomFaceFileName )
{
	CString strCacheFolder;
	strCacheFolder.Format("%s\\%s\\CustomFace",m_strPath,m_sUser);

	if ( _access(strCacheFolder,0)==-1 )
	{
		CreateDirectory(strCacheFolder,NULL);
	}


	CString str1,str2,strExt;
	ExtractFilePath(strImageFilePathName,str1,str2,strExt);

	MD5_CTX md5T;
	unsigned char digest[16];

	char szImageFilePathName[1024];
	strcpy(szImageFilePathName,strImageFilePathName);

	md5T.MD5Update ((unsigned char*)szImageFilePathName, strImageFilePathName.GetLength());
	md5T.MD5Final (digest);

	DWORD dwUid=g_ImUserInfo.GetId();

    strCustomFaceFileName.Format("{%x-%x-%x-%x}%s",
			(((long *)digest))[0]^dwUid,
			(((long *)digest))[1]^dwUid,
			(((long *)digest))[2]^dwUid,
			(((long *)digest))[3]^dwUid,strExt);

	CopyFile(strImageFilePathName,strCacheFolder+"\\"+strCustomFaceFileName,FALSE);
}


void CLocalData::SaveImageFileToCache(const CString& strImageFilePathName,CString &strCacheFileName )
{
	CString strCacheFolder;
	strCacheFolder.Format("%s\\%s\\ImageCache",m_strPath,m_sUser);

	if ( _access(strCacheFolder,0)==-1 )
	{
		CreateDirectory(strCacheFolder,NULL);
	}

	CString str1,str2,strExt;
	ExtractFilePath(strImageFilePathName,str1,str2,strExt);

	MD5_CTX md5T;
	unsigned char digest[16];

	char szImageFilePathName[1024];
	strcpy(szImageFilePathName,strImageFilePathName);

	md5T.MD5Update ((unsigned char*)szImageFilePathName, strImageFilePathName.GetLength());
	md5T.MD5Final (digest);

	DWORD dwUid = g_ImUserInfo.GetId();

    strCacheFileName.Format("{%x-%x-%x-%x}%s",
			(((long *)digest))[0]^dwUid,
			(((long *)digest))[1]^dwUid,
			(((long *)digest))[2]^dwUid,
			(((long *)digest))[3]^dwUid,strExt);

	CopyFile(strImageFilePathName,strCacheFolder+"\\"+strCacheFileName,FALSE);
}

CString CLocalData::GetWebIconPath()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath,m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}


	CString strCustomFace;
	strCustomFace.Format("%s\\%s\\webicon\\",m_strPath,m_sUser);

	if ( _access(strCustomFace,0)==-1 )
	{
		CreateDirectory(strCustomFace,NULL);
	}
	return strCustomFace;
}

CString CLocalData::GetWebFilePath()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath,m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}


	CString strCustomFace;
	strCustomFace.Format("%s\\%s\\webfile\\",m_strPath,m_sUser);

	if ( _access(strCustomFace,0)==-1 )
	{
		CreateDirectory(strCustomFace,NULL);
	}
	return strCustomFace;
}


static map<int, string> g_mapGif_i2name;
static map<int, string>* GetGifMap()
{
	if(g_mapGif_i2name.empty())
	{
		g_mapGif_i2name[0] = "b01.gif";		g_mapGif_i2name[1] = "b02.gif";		g_mapGif_i2name[2] = "b03.gif";		g_mapGif_i2name[3] = "b04.gif";		g_mapGif_i2name[4] = "b05.gif";
		g_mapGif_i2name[5] = "b06.gif";		g_mapGif_i2name[6] = "b07.gif";		g_mapGif_i2name[7] = "b08.gif";		g_mapGif_i2name[8] = "b09.gif";		g_mapGif_i2name[9] = "b10.gif";
		g_mapGif_i2name[10] = "b11.gif";	g_mapGif_i2name[11] = "b12.gif";	g_mapGif_i2name[12] = "b13.gif";	g_mapGif_i2name[13] = "b14.gif";	g_mapGif_i2name[14] = "b15.gif";
		g_mapGif_i2name[15] = "b16.gif";	g_mapGif_i2name[16] = "b17.gif";	g_mapGif_i2name[17] = "b18.gif";	g_mapGif_i2name[18] = "b19.gif";	g_mapGif_i2name[19] = "b20.gif";
		g_mapGif_i2name[20] = "b21.gif";	g_mapGif_i2name[21] = "b22.gif";	g_mapGif_i2name[22] = "b23.gif";	g_mapGif_i2name[23] = "b24.gif";	g_mapGif_i2name[24] = "b25.gif";
		g_mapGif_i2name[25] = "b26.gif";	g_mapGif_i2name[26] = "b27.gif";	g_mapGif_i2name[27] = "b28.gif";	g_mapGif_i2name[28] = "b29.gif";	g_mapGif_i2name[29] = "b30.gif";
		g_mapGif_i2name[30] = "b31.gif";	g_mapGif_i2name[31] = "b32.gif";	g_mapGif_i2name[32] = "b33.gif";	g_mapGif_i2name[33] = "b34.gif";	g_mapGif_i2name[34] = "b35.gif";
		g_mapGif_i2name[35] = "b36.gif";	g_mapGif_i2name[36] = "b37.gif";	g_mapGif_i2name[37] = "b38.gif";	g_mapGif_i2name[38] = "b39.gif";	g_mapGif_i2name[39] = "b40.gif";
		g_mapGif_i2name[40] = "b54.gif";	g_mapGif_i2name[41] = "b55.gif";	g_mapGif_i2name[42] = "b41.gif";	g_mapGif_i2name[43] = "b42.gif";	g_mapGif_i2name[44] = "b43.gif";
		g_mapGif_i2name[45] = "b44.gif";	g_mapGif_i2name[46] = "b45.gif";	g_mapGif_i2name[47] = "b46.gif";	g_mapGif_i2name[48] = "b47.gif";	g_mapGif_i2name[49] = "b48.gif";
		g_mapGif_i2name[50] = "b49.gif";	g_mapGif_i2name[51] = "b50.gif";	g_mapGif_i2name[52] = "b51.gif";	g_mapGif_i2name[53] = "b52.gif";	g_mapGif_i2name[54] = "b53.gif";
	}

	return &g_mapGif_i2name;
}

// 获取gif文件名
string GetGifName(int i)
{
	map<int, string> *pMapGif_i2name = GetGifMap();
	if(pMapGif_i2name->find(i) == pMapGif_i2name->end())
	{
		return "";
	}
	return pMapGif_i2name->at(i);
}

// 获取gif文件索引
int GetGifIndex(const char *szGifName)
{
	map<int, string> *pMapGif_i2name = GetGifMap();

	map<int, string>::iterator it = pMapGif_i2name->begin();
	for(; it != pMapGif_i2name->end(); ++it)
	{
		if(it->second == szGifName)
		{
			return it->first;
		}
	}

	return -1;
}

CString CLocalData::GetGifFacePath(int i)
{
	CString str;
	str.Format("%sEmotion\\%s",m_strProgramPath,GetGifName(i-1).c_str());
	return str;
}

CString CLocalData::GetCustomFacePath()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath,m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}


	CString strCustomFace;
	strCustomFace.Format("%s\\%s\\CustomFace\\",m_strPath,m_sUser);

	if ( _access(strCustomFace,0)==-1 )
	{
		CreateDirectory(strCustomFace,NULL);
	}

	return strCustomFace;
}

CString CLocalData::GetImageFileCachePath()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath,m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	CString strCacheFolder;
	strCacheFolder.Format("%s\\%s\\ImageCache\\",m_strPath,m_sUser);

	if ( _access(strCacheFolder,0)==-1 )
	{
		CreateDirectory(strCacheFolder,NULL);
	}

	return strCacheFolder;
}

void CLocalData::SaveCurrentSelTab(int nTab)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	::WritePrivateProfileString("Tab","index",IntToStr(nTab),strConfigFile);
}


int CLocalData::GetLastSelTab()
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	int nTab;
//amin 1->0
	nTab =::GetPrivateProfileInt("Tab","index",0,strConfigFile);

	if ( nTab<0 || nTab>2 )
	{
		nTab=1;
	}

	return nTab;
}

void CLocalData::SaveCurrentSelFloor(int nFloor)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);
    
	::WritePrivateProfileString("Floor","index",IntToStr(nFloor),strConfigFile);
}


int CLocalData::GetLastSelFloor()
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	int nFloor;

	nFloor =::GetPrivateProfileInt("Floor","index",1,strConfigFile);

	if ( nFloor<0 || nFloor>3 )
	{
		nFloor=0;
	}

	return nFloor;
}

void CLocalData::SaveLastUploadPicFolder(CString strFolder)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);    
	::WritePrivateProfileString("UpPic","folder",strFolder,strConfigFile);
}

void CLocalData::GetLastUploadPicFolder(CString & strFolder)
{
	char szFolder[1024];

	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);
	::GetPrivateProfileString("UpPic","folder","",szFolder,sizeof(szFolder),strConfigFile);
	strFolder = szFolder;
}

void CLocalData::SaveUserRecvFileSetting(const FILE_SETTING &setting)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);    
	::WritePrivateProfileString("RecvFile","folder",setting.szRecvFilePath,strConfigFile);
	::WritePrivateProfileString("RecvFile","saveposition",IntToStr(setting.bSaveFilePosition),strConfigFile);

}

void CLocalData::GetUserRecvFileSetting(FILE_SETTING &setting)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	memset(&setting,0,sizeof(setting));

	::GetPrivateProfileString("RecvFile","folder","",setting.szRecvFilePath,sizeof(setting.szRecvFilePath),strConfigFile);
	setting.bSaveFilePosition = ::GetPrivateProfileInt("RecvFile","saveposition",0,strConfigFile);

	if ( CString(setting.szRecvFilePath)=="" )
	{
		CString strUserFolder;
		strUserFolder.Format("%s\\Recieve Files\\",m_strPath);

		if ( _access(strUserFolder,0)==-1 )
		{
			CreateDirectory(strUserFolder,NULL);
		}

		strncpy(setting.szRecvFilePath,(LPCTSTR)strUserFolder,sizeof(setting.szRecvFilePath)-1);
	}
}


void CLocalData::SaveXtFont(const XT_FONT &xf)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath, m_sUser);
    
	::WritePrivateProfileString("Font","size",IntToStr(xf.fontSize),strConfigFile);
	::WritePrivateProfileString("Font","color",UIntToStr(xf.fontColor),strConfigFile);
	::WritePrivateProfileString("Font","flags",IntToStr(xf.flags),strConfigFile);
	::WritePrivateProfileString("Font","name",xf.fontName,strConfigFile);
}

void CLocalData::GetXtFont(XT_FONT &xf)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	xf.fontSize    =::GetPrivateProfileInt("Font","size",12,strConfigFile);
	xf.fontColor   =::GetPrivateProfileInt("Font","color",0,strConfigFile);
	xf.flags       =::GetPrivateProfileInt("Font","flags",0,strConfigFile);

	vector<string> *pVecSysFont = GetAllSysFontName();
	if(find(pVecSysFont->begin(), pVecSysFont->end(), "微软雅黑") != pVecSysFont->end())
	{
		GetPrivateProfileString("Font","name","微软雅黑",xf.fontName,sizeof(xf.fontName),strConfigFile);
	}
	else
	{
		GetPrivateProfileString("Font","name","宋体",xf.fontName,sizeof(xf.fontName),strConfigFile);
	}
}

void CLocalData::SaveMainFramePosition(const CRect& rect)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	if (rect.left<0 && rect.right<0 && rect.top<0 && rect.bottom<0 )
		return ;
    
	::WritePrivateProfileString("MainWindow","left",IntToStr(rect.left),strConfigFile);
	::WritePrivateProfileString("MainWindow","top",IntToStr(rect.top),strConfigFile);
	::WritePrivateProfileString("MainWindow","right",IntToStr(rect.right),strConfigFile);
	::WritePrivateProfileString("MainWindow","bottom",IntToStr(rect.bottom),strConfigFile);
}

void CLocalData::SaveSubWndWidth(const CString & sWnd,int nWidth)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	if (nWidth < 0 || nWidth > 4000)
		return ;
	::WritePrivateProfileString(sWnd,"width",IntToStr(nWidth),strConfigFile);

}

int CLocalData::GetSubWndWidth(const CString &sWnd)
{
	int nWidth = 0;
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	nWidth =::GetPrivateProfileInt(sWnd,"width",0,strConfigFile);

	return nWidth;
}

void CLocalData::SaveSubWndMode(const CString & sWnd,int nMode)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);
	::WritePrivateProfileString(sWnd,"mode",IntToStr(nMode),strConfigFile);
}

int CLocalData::GetSubWndMode(const CString & sWnd)
{
	int nWidth = 0;
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);
	nWidth =::GetPrivateProfileInt(sWnd,"mode",0,strConfigFile);
	return nWidth;
}

bool CLocalData::IsShowAllFriend() const
{
	bool bAll = true;
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);
	bAll =::GetPrivateProfileInt("FriendList","showAll",1,strConfigFile) != 0;

	return bAll;
}

void CLocalData::SetShowAllFriend(bool bAll)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);
	::WritePrivateProfileString("FriendList","showAll",IntToStr(bAll),strConfigFile);
}

void CLocalData::GetMainFramePostion(CRect &rect)
{
	CString strConfigFile;
	strConfigFile.Format("%s\\%s\\pro.ini",m_strPath,m_sUser);

	rect.left =::GetPrivateProfileInt("MainWindow","left",0,strConfigFile);
	rect.top  =::GetPrivateProfileInt("MainWindow","top",20,strConfigFile);
	rect.right=::GetPrivateProfileInt("MainWindow","right",282,strConfigFile);
	rect.bottom =::GetPrivateProfileInt("MainWindow","bottom",520,strConfigFile);

	CRect rcBar;
	rcBar.SetRect(rect.left,rect.top,rect.right,rect.bottom);

	rcBar.bottom=rcBar.top+20;

	CRect rcScreen;
	rcScreen.SetRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	CRect rcTest;
	rcTest.IntersectRect(&rcScreen,&rcBar);

	if (rcTest.IsRectEmpty() || rcTest.IsRectNull() )
	{
		rect.left = 0;
		rect.top = 20;
		rect.right = 282;
		rect.bottom = 500+20;
	}

	//靠右
	rect.left += rcScreen.right - rect.right - 20;
	rect.right = rcScreen.right - 20;
}

void CLocalData::SaveMsgHotKey(WORD nVk,WORD nMod)
{

	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"MsgKey", 
		"vk",
		s, 
		sCfg);
	s.Format("%d",nMod);
	::WritePrivateProfileString(
		"MsgKey", 
		"mod",
		s, 
		sCfg);
}

void CLocalData::SaveSnapShotHotKey(WORD nVk,WORD nMod)
{

	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"SnapShotKey", 
		"vk",
		s, 
		sCfg);
	s.Format("%d",nMod);
	::WritePrivateProfileString(
		"SnapShotKey", 
		"mod",
		s, 
		sCfg);
}

void CLocalData::GetSnapShotHotKey(WORD & nVk,WORD & nMod)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	nVk = ::GetPrivateProfileInt(
		"SnapShotKey", 
		"vk",
		0, 
		sCfg); 
	nMod = ::GetPrivateProfileInt(
		"SnapShotKey", 
		"mod",
		0, 
		sCfg); 	
}

void CLocalData::GetMsgHotKey(WORD & nVk,WORD & nMod)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	nVk = ::GetPrivateProfileInt(
		"MsgKey", 
		"vk",
		0, 
		sCfg); 
	nMod = ::GetPrivateProfileInt(
		"MsgKey", 
		"mod",
		0, 
		sCfg); 	
}

BOOL CLocalData::SaveHotKeyConfig()
{
	BOOL bRt=FALSE;
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",s_hotKeyConfig.sendMsgKey);
	bRt=::WritePrivateProfileString(
		"HotKey", 
		"SendMessageKey",
		s, 
		sCfg);
	
	s.Format("%d",s_hotKeyConfig.bScreenCopyHideWindow);
	bRt=::WritePrivateProfileString(
		"ScreenCopy", 
		"bHide",
		s, 
		sCfg);
	return bRt;	
}

void CLocalData::SaveShakeWinConfig(WORD nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"ShakeWindow", 
		"shakeN",
		s, 
		sCfg);
}





void CLocalData::SaveWakeupConfig(WORD  nVk,WORD  nStatus)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"Wakeup", 
		"time",
		s, 
		sCfg);

	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	s.Format("%d",nStatus);
	::WritePrivateProfileString(
		"Wakeup", 
		"status",
		s, 
		sCfg);
}

void CLocalData::GetWakeupConfig(WORD & nVk,WORD & nStatus)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"Wakeup", 
		"time",
		0, 
		sCfg);

	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	nStatus = ::GetPrivateProfileInt(
		"Wakeup", 
		"status",
		0, 
		sCfg);
}

void CLocalData::GetShakeWinConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"ShakeWindow", 
		"shakeN",
		1, 
		sCfg);
}


void CLocalData::GetFileTransConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"FileTrans", 
		"trans",
		0, 
		sCfg);
}


void CLocalData::SaveFileTransConfig(WORD nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"FileTrans", 
		"trans",
		s, 
		sCfg);
}


void CLocalData::GetSoftUpdateConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"SoftUpdate", 
		"mention",
		0, 
		sCfg);
}


void CLocalData::SaveSoftUpdateConfig(WORD nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"SoftUpdate", 
		"mention",
		s, 
		sCfg);
}


void CLocalData::GetSoftUpdateIgnoreConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"SoftUpdate", 
		"ignore",
		0, 
		sCfg);
}


void CLocalData::SaveSoftUpdateIgnoreConfig(WORD nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"SoftUpdate", 
		"ignore",
		s, 
		sCfg);
}






void CLocalData::SaveExitWarnConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"ExitWarnN", 
		"warn",
		s, 
		sCfg);
}

void CLocalData::GetExitWarnConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"ExitWarnN", 
		"warn",
		1, 
		sCfg);
}


void CLocalData::SaveVoiceConfig(CString strVoice,int nVoiceID)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVoiceID);//WAV序号
	::WritePrivateProfileString(
		"Voice", 
		s,
		strVoice, 
		sCfg);
}

void CLocalData::GetVoiceConfig(CString &strVoice,int nVoiceID)
{
	char Set[255];
	int nResult = 0;
	memset(Set, 0, 255);	
	CString s;
	s.Format("%d",nVoiceID);//WAV序号
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	nResult = GetPrivateProfileString("Voice",s,"",Set,255,sCfg);
	if (nResult > 0)
	{
		strVoice = Set;
	}
}

void CLocalData::SaveTopShowConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"Topshow", 
		"top",
		s, 
		sCfg);
}

void CLocalData::GetTopShowConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"Topshow", 
		"top",
		0, 
		sCfg);
}

void CLocalData::SaveHideWinConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"Hidewindow", 
		"hide",
		s, 
		sCfg);
}

void CLocalData::GetHideWinConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"Hidewindow", 
		"hide",
		0, 
		sCfg);
}

HOTKEY_CONFIG* CLocalData::GetHotKeyConfig()
{
	return &s_hotKeyConfig;
}

BOOL CLocalData::ReadHotKeyConfig()
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	s_hotKeyConfig.sendMsgKey=(EM_SEND_MSG_KEY)::GetPrivateProfileInt(
		"HotKey", 
		"SendMessageKey",
		SMK_ENTER, 
		sCfg);


	s_hotKeyConfig.bScreenCopyHideWindow=(bool)::GetPrivateProfileInt(
		"HotKey", 
		"SendMessageKey",
		0, 
		sCfg); 
	return TRUE;
}

BOOL CLocalData::SaveGuidePageSetting(CString strGuidePageDate)
{
	BOOL bRt=FALSE;
	CString sCfg;
	sCfg.Format("%s\\GuidePageConfig.ini",m_strPath);
	CString s;

	// 关闭时是否删除消息记录
	s.Format("%d", 0);
	bRt=::WritePrivateProfileString(
		"GuidePage", 
		strGuidePageDate,
		s, 
		sCfg);

	return bRt;	
}

BOOL CLocalData::SaveBasicSystemSetting()
{
	BOOL bRt=FALSE;
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	// 是否自动播放魔法表情
	s.Format("%d",s_basicSystemSetting.bPlayMagicFace);
	bRt=::WritePrivateProfileString(
		"BasicSystemSetting", 
		"PlayMagicFace",
		s, 
		sCfg);
	// 是否自动显示关注好友图片窗口
	s.Format("%d",s_basicSystemSetting.bShowFriendPicWnd);
	bRt=::WritePrivateProfileString(
		"BasicSystemSetting", 
		"ShowFriendPicWnd",
		s, 
		sCfg);

	// 是否自动显示关注好友文章窗口
	s.Format("%d",s_basicSystemSetting.bShowFriendArtWnd);
	bRt=::WritePrivateProfileString(
		"BasicSystemSetting", 
		"ShowFriendArtWnd",
		s, 
		sCfg);


	//
	s.Format("%d",s_basicSystemSetting.bShowMiniPictureBroadCast);
	bRt=::WritePrivateProfileString(
		"BasicSystemSetting", 
		"MiniPictureBroadCast",
		s, 
		sCfg);

	// 动作表情工具栏显示状态
	s.Format("%d",s_basicSystemSetting.bShowActionToolbar);
	bRt = ::WritePrivateProfileString(
		"Toolbar",
		"ActionToolbar",
		s,
		sCfg
	);
	
	// 启动时是否自动同步消息记录
	s.Format("%d", s_basicSystemSetting.bAutoSyncMsgRecord);
	bRt=::WritePrivateProfileString(
		"BasicSystemSetting", 
		"AutoSyncMsgRecord",
		s, 
		sCfg);

	// 关闭时是否删除消息记录
	s.Format("%d", s_basicSystemSetting.bAutoClearMsgRecord);
	bRt=::WritePrivateProfileString(
		"BasicSystemSetting", 
		"AutoClearMsgRecord",
		s, 
		sCfg);

	return bRt;	
}

BASIC_SYSTEM_SETTING* CLocalData::GetBasicSystemSetting()
{
	return &s_basicSystemSetting;
}

BOOL CLocalData::ReadGuidePageSetting(CString strGuidePageDate)
{
	CString sCfg;
	sCfg.Format("%s\\GuidePageConfig.ini",m_strPath);
	// 是否自动播放魔法表情
	BOOL bIsGuidePage = GetPrivateProfileInt(
		"GuidePage", 
		strGuidePageDate,
		TRUE, 
		sCfg
		); 
	return bIsGuidePage;
}

BOOL CLocalData::ReadBasicSystemSetting()
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	// 是否自动播放魔法表情
	s_basicSystemSetting.bPlayMagicFace=(EM_SEND_MSG_KEY)::GetPrivateProfileInt(
		"BasicSystemSetting", 
		"PlayMagicFace",
		TRUE, 
		sCfg
	); 
	// 是否自动显示关注好友图片窗口
	s_basicSystemSetting.bShowFriendPicWnd =(EM_SEND_MSG_KEY)::GetPrivateProfileInt(
		"BasicSystemSetting", 
		"ShowFriendPicWnd",
		TRUE, 
		sCfg
	); 
	// 是否自动显示关注好友文章窗口
	s_basicSystemSetting.bShowFriendArtWnd =(EM_SEND_MSG_KEY)::GetPrivateProfileInt(
		"BasicSystemSetting", 
		"ShowFriendArtWnd",
		TRUE, 
		sCfg
	);
	// 动作表情工具栏显示状态
	s_basicSystemSetting.bShowMiniPictureBroadCast = ::GetPrivateProfileInt(
		"BasicSystemSetting",
		"MiniPictureBroadCast",
		TRUE,
		sCfg
	);
	// 动作表情工具栏显示状态
	s_basicSystemSetting.bShowActionToolbar = ::GetPrivateProfileInt(
		"Toolbar",
		"ActionToolbar",
		TRUE,
		sCfg
	);

	// 启动时是否自动同步消息记录
	s_basicSystemSetting.bAutoSyncMsgRecord = ::GetPrivateProfileInt(
		"BasicSystemSetting",
		"AutoSyncMsgRecord",
		TRUE,
		sCfg
		);

	// 关闭时是否删除消息记录
	s_basicSystemSetting.bAutoClearMsgRecord = ::GetPrivateProfileInt(
		"BasicSystemSetting",
		"AutoClearMsgRecord",
		FALSE,
		sCfg
		);

	return TRUE;
}

//保存通讯录分组信息
void CLocalData::SaveAddressBookGroupInfo(const ContactGroupInfo &info)
{
	CString str;
	str.Format("%s\\%s\\addgroupinfo.dat",m_strPath,m_sUser);
	//DeleteFile(str);

	CStdioFile file;

	if ( file.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		file.Write(&info,sizeof(info));
		file.Close();
	}
}

//加载通讯录分组信息
bool CLocalData::LoadAddressBookGroupInfo(ContactGroupInfo &info)
{
	CString str;
	str.Format("%s\\%s\\addgroupinfo.dat",m_strPath,m_sUser);

	CStdioFile file;

	bool bRet = false;

	if ( file.Open(str,CFile::modeRead|CFile::typeBinary) )
	{
		int nFileSize = file.GetLength();

		if ( nFileSize==sizeof(info) )
		{
			char *pData = (char *)&info;

			int nRead = 0;

			while ( nRead<nFileSize )
			{
				int n=file.Read(pData,255);
				nRead+=n;
				pData+=n;
			}

			bRet = true;
		}

		file.Close();
	}

	return bRet ;
}

//保存通讯录联系人名单列表
void CLocalData::SaveAddressBook(ADDRESS_BOOK_LIST &addbookList)
{
	CString str;
	str.Format("%s\\%s\\adressbook.xml",m_strPath,m_sUser);
	//DeleteFile(str);

	CStdioFile file;

	if ( file.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		file.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\" ?>");
		file.WriteString("\n");
		file.WriteString("<addressbook>");
		
		ADDRESS_BOOK_LIST::iterator it;

		int nItem = 0;

		for ( it=addbookList.begin(); it!=addbookList.end(); it++ )
		{
			ContactPersonInfo *pInfo = (*it);
			file.WriteString("<item"+IntToStr(nItem)+">");

			file.WriteString("<address>"+pInfo->strAddress+"</address>");
			file.WriteString("<birthday>"+pInfo->strBirthday+"</birthday>");
			file.WriteString("<blog>"+pInfo->strBlog+"</blog>");
			file.WriteString("<career>"+pInfo->strCareer+"</career>");
			file.WriteString("<company>"+pInfo->strCompany+"</company>");
			file.WriteString("<email>"+pInfo->strEmail+"</email>");
			file.WriteString("<msn>"+pInfo->strMSNNum+"</msn>");
			file.WriteString("<qq>"+pInfo->strQQNum+"</qq>");
			file.WriteString("<name>"+pInfo->strName+"</name>");
			file.WriteString("<note>"+pInfo->strNote+"</note>");
			file.WriteString("<phone>"+pInfo->strPhone+"</phone>");
			file.WriteString("<school>"+pInfo->strSchool+"</school>");
			file.WriteString("<sex>"+IntToStr(pInfo->nSex)+"</sex>");
			file.WriteString("<group>"+IntToStr(pInfo->nGroupIndex)+"</group>");
			file.WriteString("<type>"+IntToStr(pInfo->nType)+"</type>");
			file.WriteString("<invited>"+IntToStr(pInfo->nInvited)+"</invited>");

			file.WriteString("</item"+IntToStr(nItem)+">");	

			nItem++;
		}

		file.WriteString("</addressbook>");
	}
}

void SplitContactPersonInfo(const CString &strInfo, ContactPersonInfo *pOutInfo)
{
	GetSectionValue(strInfo,"<address>","</address>",pOutInfo->strAddress);
	GetSectionValue(strInfo,"<birthday>","</birthday>",pOutInfo->strBirthday);
	GetSectionValue(strInfo,"<career>","</career>",pOutInfo->strCareer);
	GetSectionValue(strInfo,"<company>","</company>",pOutInfo->strCompany);
	GetSectionValue(strInfo,"<email>","</email>",pOutInfo->strEmail);
	GetSectionValue(strInfo,"<msn>","</msn>",pOutInfo->strMSNNum);
	GetSectionValue(strInfo,"<qq>","</qq>",pOutInfo->strQQNum);
	GetSectionValue(strInfo,"<blog>","</blog>",pOutInfo->strBlog);
	GetSectionValue(strInfo,"<name>","</name>",pOutInfo->strName);
	GetSectionValue(strInfo,"<note>","</note>",pOutInfo->strNote);
	GetSectionValue(strInfo,"<phone>","</phone>",pOutInfo->strPhone);
	GetSectionValue(strInfo,"<school>","</school>",pOutInfo->strSchool);

	CString strType;
	GetSectionValue(strInfo,"<type>","</type>",strType);
	pOutInfo->nType = atoi((LPCSTR)strType);

	CString strSex;
	GetSectionValue(strInfo,"<sex>","</sex>",strSex);
	pOutInfo->nSex = atoi((LPCSTR)strSex);

	CString strGroup;
	GetSectionValue(strInfo,"<group>","</group>",strGroup);
	pOutInfo->nGroupIndex = atoi((LPCSTR)strGroup);

	CString strInvited;
	GetSectionValue(strInfo,"<invited>","</invited>",strInvited);
	pOutInfo->nInvited = atoi((LPCSTR)strInvited);

}

//加载通讯录联系人名单列表
bool CLocalData::LoadAddressBook(ADDRESS_BOOK_LIST &addbookList)
{
	CString str;
	str.Format("%s\\%s\\adressbook.xml",m_strPath,m_sUser);

	ClearAddressBookList(addbookList);

	CString strData="";

	bool bRet = false;

	CStdioFile file;

	if ( file.Open(str,CFile::modeRead|CFile::typeText) )
	{
		CString strTemp;
		while(file.ReadString(strTemp) )
			strData+=strTemp;

		file.Close();
	}

	if ( strData!="" )
	{
		int nItem     = 0;
		int nSplitPos = 0;

		while ( 1 )
		{
			CString str1 = "<item"+IntToStr(nItem)+">";
			CString str2 = "</item"+IntToStr(nItem)+">";

			int iPos1 = strData.Find(str1,nSplitPos);
			int iPos2 = strData.Find(str2,nSplitPos+5);

			if ( iPos2>iPos1 && iPos1>=nSplitPos )
			{
				int iLength = str1.GetLength();

				CString strContactInfo = strData.Mid(iPos1+iLength,iPos2-iPos1-iLength);
				
				ContactPersonInfo *pInfo = new ContactPersonInfo();

				pInfo->nType = 0;//
				
				SplitContactPersonInfo(strContactInfo,pInfo);

				addbookList.push_back(pInfo);

				nSplitPos = iPos2+str2.GetLength();

				nItem++;
			}
			else
			{
				break;
			}
		}

		bRet = true;
	}


	ContactList cl;
	m_objWindAddrBook.LoadContacts(cl);

	int nCount = cl.size();

	for ( int i=0; i<nCount; i++ )
	{
		ContactPersonInfo *pInfo = new ContactPersonInfo();
		pInfo->strName	  = cl[i]->DisplayName;
		pInfo->strEmail   = cl[i]->EmailAddr;
		pInfo->strBlog    = cl[i]->HomePage;
		pInfo->strCompany = cl[i]->Company;
		pInfo->strNote    = cl[i]->Comment;
		pInfo->strAddress = cl[i]->Country + " " + cl[i]->City + " " + cl[i]->Street;
		pInfo->strPhone   = cl[i]->Mobile;
	
		cl[i]->EmailAddr.MakeLower();
		cl[i]->EmailAddr.Trim();

		bool bExist = false;

		ADDRESS_BOOK_LIST::iterator it;
		for ( it=addbookList.begin(); it!=addbookList.end(); it++ )
		{
			CString strEmail = (*it)->strEmail;
			CString strName  = (*it)->strName;
			strEmail.MakeLower();
			strEmail.Trim();


			if ( strEmail==cl[i]->EmailAddr && strName==cl[i]->DisplayName )
			{
				bExist = true;
				break;
			}
		}

		if ( bExist==false )
		{
			addbookList.push_back(pInfo);
			pInfo->nType=1;//out look address
			pInfo->nInvited=0;
		}
	}

	return bRet;
}

int CLocalData::GetDefaultCameraIndex()
{
	CString strIni;
	strIni.Format("%s\\%s\\zone_a.ini",m_strPath,m_sUser);

	char szValue[255];

	GetPrivateProfileString("Camera","index","0",szValue,sizeof(szValue),strIni);
	
	return atoi(szValue);	
}

void CLocalData::SetDefaultCameraIndex(int nIndex)
{
	CString strIni;
	strIni.Format("%s\\%s\\zone_a.ini",m_strPath,m_sUser);

	WritePrivateProfileString("Camera","index",IntToStr(nIndex),strIni);
}

void CLocalData::GetZoneSetting(ZONE_SETTING &setting)
{
	CString strIni;
	strIni.Format("%s\\%s\\zone_a.ini",m_strPath,m_sUser);

	char szValue[255];

	GetPrivateProfileString("Article","backup_path",m_strPath,szValue,sizeof(szValue),strIni);

	strncpy(setting.szBackupPath,szValue,sizeof(setting.szBackupPath)-1);

	int l = strlen(setting.szBackupPath);

	while (l>0)
	{
		l--;
		if ( setting.szBackupPath[l]!=' ' )
		{
			if(setting.szBackupPath[l]!='\\')
			{
				setting.szBackupPath[l+1]='\\';
				setting.szBackupPath[l+2]=0;
			}
			else
			{
				break;
			}
		}		
	}
}

void CLocalData::SetZoneSetting(const ZONE_SETTING &setting)
{
	CString strIni;
	strIni.Format("%s\\%s\\zone_a.ini",m_strPath,m_sUser);

	WritePrivateProfileString("Article","backup_path",setting.szBackupPath,strIni);
}

void CLocalData::GetHistoryFamilyFaceList(CStringList &slFace, long nFamilyID)
{
	CString strFile;
	strFile.Format("%s\\%s\\history_face%d.dat",m_strPath,m_sUser,nFamilyID);

	CStdioFile f;
	if ( f.Open(strFile,CFile::modeRead|CFile::typeText) )
	{
		CString s;
		while ( f.ReadString(s) )
		{
			slFace.AddTail(s);
		}
		f.Close();
	}
}

void CLocalData::SaveHistoryFamilyFaceUrl(CString strUrl, long nFamilyID)
{
	CString strDefault;
	strDefault.Format("%s\\%s\\",m_strPath,m_sUser);

	if ( _access((LPCSTR)strDefault,0)==-1 )
		CreateDirectory(strDefault,NULL);

	CString strFile ;//= strDefault+"history_face.dat";
	strFile.Format("%shistory_face%d.dat",strDefault,nFamilyID);

	CStdioFile f;

	if ( _access((LPCSTR)strFile,0) )
	{
		if (f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			f.WriteString(strUrl);
			f.WriteString("\n");
		}
		f.Close();
	}
	else
	{
		if ( f.Open(strFile,CFile::modeReadWrite|CFile::typeText) )
		{
			CString s;
			strUrl.Trim();
			strUrl.MakeLower();

			bool bFind=false;

			while ( f.ReadString(s) )
			{
				s.Trim();
				s.MakeLower();
				if ( s==strUrl )
				{
					bFind = true;
					break;
				}
			}

			if ( bFind==false )
			{
				f.SeekToEnd();
				f.WriteString(strUrl);
				f.WriteString("\n");
			}

			f.Close();
		}
	}
}

void CLocalData::GetHistoryFaceList(CStringList &slFace)
{
	CString strFile;
	strFile.Format("%s\\%s\\history_face.dat",m_strPath,m_sUser);

	CStdioFile f;
	if ( f.Open(strFile,CFile::modeRead|CFile::typeText) )
	{
		CString s;
		while ( f.ReadString(s) )
		{
			slFace.AddTail(s);
		}
		f.Close();
	}

}

void CLocalData::SaveHistoryFaceUrl(CString strUrl)
{
	CString strDefault;
	strDefault.Format("%s\\%s\\",m_strPath,m_sUser);

	if ( _access((LPCSTR)strDefault,0)==-1 )
		CreateDirectory(strDefault,NULL);

	CString strFile = strDefault+"history_face.dat";

	CStdioFile f;

	if ( _access((LPCSTR)strFile,0) )
	{
		if (f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			f.WriteString(strUrl);
			f.WriteString("\n");
		}
		f.Close();
	}
	else
	{
		if ( f.Open(strFile,CFile::modeReadWrite|CFile::typeText) )
		{
			CString s;
			strUrl.Trim();
			strUrl.MakeLower();

			bool bFind=false;

			while ( f.ReadString(s) )
			{
				s.Trim();
				s.MakeLower();
				if ( s==strUrl )
				{
					bFind = true;
					break;
				}
			}

			if ( bFind==false )
			{
				f.SeekToEnd();
				f.WriteString(strUrl);
				f.WriteString("\n");
			}

			f.Close();
		}
	}
	

}


bool CLocalData::IsShowHelpDemo()
{
	CString strIni = m_strPath;
	strIni+="\\pro.ini";

	int nRun = ::GetPrivateProfileInt( "ShowDemoHelp" ,"Run",1,strIni);

	return nRun!=0 ;
}

void CLocalData::SetShowHelpDemo(bool bShow)
{
	CString strIni = m_strPath;
	strIni+="\\pro.ini";

	CString strRun;
	strRun = bShow? "1":"0";
	WritePrivateProfileString("ShowDemoHelp","Run",strRun,strIni);
}

int CLocalData::GetCurVersion()
{
	int nVer = XT_HEAD::FROM_CLIENT;
	CString strIni = m_strPath;
	strIni+="app.dat";
	const char keyVersion[16] = "1798asdflkjq	0";
	char plian[100] = "" ; 
	char szCrypt[100];
	long nLen = 0;
	long nCrypt = 0;
	CFile f;

	if(f.Open(strIni,CFile::modeRead) == FALSE)
		return nVer;
	nCrypt = f.Read(szCrypt,100);
	if(nCrypt > 100)
		return XT_HEAD::FROM_CLIENT;
	if(TEADecrypt(szCrypt,nCrypt,(char *)keyVersion,plian,&nLen))
	{
		nVer = atoi(plian);
	}
	
	f.Close();
	return nVer;
}

const APP_SETTING & CLocalData::GetAppSetting()
{
	if(s_bAppSetting == false)
	{
		ReadAppSetting();	
	}
	return s_appSetting;
}

bool CLocalData::IsAppSettingExist() const
{
	CString str = GetPath() + "\\app.config";
	return (_access((LPCSTR)str,0) != -1 );
}


void GetDownloadSetting(MSXML2::IXMLDOMNodePtr nodePtr,WEB_FACE_SETTING &webFace)
{
	MSXML2::IXMLDOMNodeListPtr nodeListPtr;
	MSXML2::IXMLDOMNodePtr itPtr,attrPtr;
	MSXML2::IXMLDOMNamedNodeMapPtr attrsPtr;
	nodeListPtr = nodePtr->childNodes;
	int iLen = nodeListPtr->length;
	int i;
	CString sNode;
	// 遍列当前下载配置列表
	for(i = 0;i < iLen; i++)
	{
		itPtr=nodeListPtr->Getitem(i);
		if(itPtr->nodeType==MSXML2::NODE_TEXT ||
			itPtr->nodeType==MSXML2::NODE_COMMENT)
			continue;
		else if(itPtr->nodeType==MSXML2::NODE_ELEMENT)
		{
			sNode = (const char*)itPtr->nodeName;
			if(sNode == "webface")
			{
				attrsPtr = itPtr->attributes;
					
				attrPtr = attrsPtr->getNamedItem("host");
				if(attrPtr)
					webFace.sHost = (const char *)attrPtr->text;
				attrPtr = attrsPtr->getNamedItem("url");
				if(attrPtr)
					webFace.sUrl = (const char *)attrPtr->text;
			}
			else if(sNode == "")
			{
				// 其他配置项
				// .....
			}	
		}
	}
}

BOOL CLocalData::ReadAppSetting()
{
	CString sConfig = GetPath() + "\\app.config";
	_variant_t varXml(sConfig);
	_variant_t varOut((bool)TRUE);
	MSXML2::IXMLDOMDocumentPtr docPtr;
	HRESULT hr;
	hr = docPtr.CreateInstance("msxml2.domdocument");
	if(hr < 0)
	{
		// "msxml解析器加载失败，检测是否已经安装";
		return false;
	}
	docPtr->PutresolveExternals(FALSE);
	docPtr->PutvalidateOnParse(FALSE);

	varOut = docPtr->load(varXml);
	if ((bool)varOut == false)
	{
		return false;
	}
	MSXML2::IXMLDOMNodePtr	nodePtr;
	nodePtr = docPtr->GetlastChild();
	string sNode = (char*)nodePtr->nodeName;
	if(stricmp(sNode.c_str(),"app") != 0)
	{
		return false;
	}	

	MSXML2::IXMLDOMNodeListPtr nodeListPtr;
	MSXML2::IXMLDOMNodePtr itPtr;
	nodeListPtr = nodePtr->childNodes;
	int iLen = nodeListPtr->length;
	int i;
	// 遍列当前服务器列表
	for(i = 0;i < iLen; i++)
	{
		itPtr=nodeListPtr->Getitem(i);
		if(itPtr->nodeType==MSXML2::NODE_TEXT ||
			itPtr->nodeType==MSXML2::NODE_COMMENT)
			continue;
		else if(itPtr->nodeType==MSXML2::NODE_ELEMENT)
		{
			sNode = (char*)itPtr->nodeName;
			if(stricmp( sNode.c_str(),"download") == 0)
			{
				// 获取web定义表情
				GetDownloadSetting(itPtr,s_appSetting.webFaceSetting);
			}
			else if(stricmp( sNode.c_str(),"") == 0)
			{
				// 其他配置项
				// .....
			}	
		}
	}
	return true;
}

//返回未读消息个数
int CLocalData::LoadWebNotifications(vector<TWebNotificationInfo *> &webNotifications)
{
	CString strFile;
	strFile.Format("%s\\%s\\webnotif.dat",m_strPath,m_sUser);

	int n=webNotifications.size();

	for ( int i=0; i<n; i++ )
	{
		delete webNotifications[i];
	}

	webNotifications.clear();

	n = 0;

	CStdioFile f;

	if ( f.Open(strFile,CFile::modeRead|CFile::typeBinary) )
	{
		uint32 nUserId;
		f.Read(&nUserId,sizeof(nUserId));
		if ( nUserId==g_ImUserInfo.GetId() )
		{
			while ( 1 )
			{
				TWebNotificationInfo * info = new TWebNotificationInfo();

				if ( f.Read(info,sizeof(TWebNotificationInfo))==sizeof(TWebNotificationInfo) )
				{
					if ( info->is_read==0 )
						n++;

					info->dwCreateDate = DateStr2Long(info->szCreateDate);

					webNotifications.push_back(info);	
				}			
				else
				{
					delete info;
					break;
				}
			}
		}
		f.Close();
	}

	return n;
}

void CLocalData::SaveWebNotifications(const vector<TWebNotificationInfo *> &webNotifications)
{
	CString strFile;
	strFile.Format("%s\\%s\\webnotif.dat",m_strPath,m_sUser);

	DeleteFile( strFile );

	CStdioFile f;

	if ( f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		uint32 id = g_ImUserInfo.GetId();
		f.Write(&id,sizeof(id));
		for ( int i=0; i<webNotifications.size(); i++ )
		{
			f.Write(webNotifications[i],sizeof(TWebNotificationInfo));
		}

		f.Close();
	}
}

void CLocalData::GetRefuseUsers(vector<uint32> &vecRefuseUser)
{
	CString strFile;
	strFile.Format("%s\\%s\\rfu.dat",m_strPath,m_sUser);

	CStdioFile f;
	vecRefuseUser.clear();

	if ( f.Open(strFile,CFile::modeRead|CFile::typeBinary) )
	{
		int iFlag ;
		f.Read(&iFlag,sizeof(int));

		if ( iFlag==0xcdab1 )
		{
			uint32 id;
			while ( f.Read(&id,sizeof(uint32)) )
			{
				vecRefuseUser.push_back(id);
			}
		}

		f.Close();
	}

}

void CLocalData::SaveRefuseUsers(const vector<uint32> &vecRefuseUser)
{
	CString strFile;
	strFile.Format("%s\\%s\\rfu.dat",m_strPath,m_sUser);

	//DeleteFile( strFile );

	CStdioFile f;

	if ( f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		int iFlag = 0xcdab1;
		f.Write(&iFlag,sizeof(int));

	
		int n=vecRefuseUser.size();
		for ( int i=0; i<n; i++ )
		{
			uint32 id = vecRefuseUser[i];
			f.Write(&id,sizeof(id));
		}

		f.Close();
	}
}

CLocalData		g_LocalData;
//CDataManager	g_LocalMsgData;

// 自动回复
int CLocalData::GetAutoReplyItemCount()
{
	return m_vecAutoReplyItem.size();
}

BOOL CLocalData::GetAutoReplyItem(int nIndex, AUTO_REPLY &item)
{
	if(nIndex < 0 || nIndex >= m_vecAutoReplyItem.size())
	{
		return FALSE;
	}

	item = m_vecAutoReplyItem[nIndex];
	return TRUE;
}

BOOL CLocalData::SetAutoReplyItem(int nIndex, const AUTO_REPLY &item)
{
	if(nIndex < 0 || nIndex >= m_vecAutoReplyItem.size())
	{
		return FALSE;
	}

	m_vecAutoReplyItem[nIndex] = item;
	return TRUE;
}

void CLocalData::AddAutoReplyItem(const AUTO_REPLY &item)
{
	m_vecAutoReplyItem.push_back(item);
}

BOOL CLocalData::DeleteAutoReplyItem(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_vecAutoReplyItem.size())
	{
		return FALSE;
	}

	m_vecAutoReplyItem.erase(m_vecAutoReplyItem.begin() + nIndex);

	return TRUE;
}

int CLocalData::GetAllAutoReplyItem(vector<AUTO_REPLY> &allItem)
{
	allItem = m_vecAutoReplyItem;
	return allItem.size();
}

void CLocalData::SetAllAutoReplyItem(const vector<AUTO_REPLY> &allItem)
{
	m_vecAutoReplyItem = allItem;
}

// 在线时自动接待短语
void CLocalData::GetAutoAccept_Online(AUTO_REPLY &item)
{
	item = m_AutoAcceptOnline;
}

void CLocalData::SetAutoAccept_Online(const AUTO_REPLY &item)
{
	m_AutoAcceptOnline = item;
}

// 离线时自动接待短语
void CLocalData::GetAutoAccept_Offline(AUTO_REPLY &item)
{
	item = m_AutoAcceptOffline;
}

void CLocalData::SetAutoAccept_Offline(const AUTO_REPLY &item)
{
	m_AutoAcceptOffline = item;
}

// 快捷短语
int CLocalData::GetFastWord(vector<string> &vecFastWord)
{
	vecFastWord = m_vecFastWord;
	return vecFastWord.size();
}

void CLocalData::SetFastWord(const vector<string> &vecFastWord)
{
	m_vecFastWord = vecFastWord;
}

// 接收到的图片
void CLocalData::AcceptFriendPic(const char *pFriendPic)
{
	if(find(m_vecFriendPic.begin(), m_vecFriendPic.end(), pFriendPic) == m_vecFriendPic.end())
	{
		m_vecFriendPic.push_back(pFriendPic);
	}
}

void CLocalData::AcceptClusterPic(const char *pClusterPic)
{
	if(find(m_vecClusterPic.begin(), m_vecClusterPic.end(), pClusterPic) == m_vecClusterPic.end())
	{
		m_vecClusterPic.push_back(pClusterPic);
	}
}

// 获取接收到的好友图片列表
int CLocalData::GetAcceptedFriendPicList(vector<string> &vecPicList)
{
	vecPicList = m_vecFriendPic;
	return vecPicList.size();
}

// 获取接收到的群图片列表
int CLocalData::GetAcceptedClusterPicList(vector<string> &vecPicList)
{
	vecPicList = m_vecClusterPic;
	return vecPicList.size();
}

// 从图片列表中读取记录
void CLocalData::ReadAcceptedPicList(const char *pPicListFile, vector<string> &vecFicList)
{
	// 判断文件是否存在，不存在就创建
	CStdioFile f;
	if ( _access(pPicListFile,0) )
	{
		f.Open(pPicListFile,CFile::modeCreate|CFile::typeText);
		f.Close();
		return;
	}
	
	vecFicList.clear();
	
	if ( f.Open(pPicListFile, CFile::modeRead|CFile::typeText) )
	{
		CString s;

		while ( f.ReadString(s) )
		{
			s.Trim();
			vecFicList.push_back((LPCTSTR)s);
		}

		f.Close();
	}
}

// 读取接收到的好友图片列表
void CLocalData::ReadAcceptedFriendPicList()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 图片文件
	CString strPicListPath = strUserFolder + "FriendPic.list";
	ReadAcceptedPicList(strPicListPath, m_vecFriendPic);
}

// 读取接收到的群图片列表
void CLocalData::ReadAcceptedClusterPicList()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 图片文件
	CString strPicListPath = strUserFolder + "ClusterPic.list";
	ReadAcceptedPicList(strPicListPath, m_vecClusterPic);
}

// 清空好友图片列表
void CLocalData::ClearAcceptedFriendPicList()
{
	// 删除列表文件
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 图片文件
	CString strPicListPath = strUserFolder + "FriendPic.list";
	DeleteFile(strPicListPath);

	// 清除内存
	m_vecFriendPic.clear();
}

// 清空群图片列表
void CLocalData::ClearAcceptedClusterPicList()
{
	// 删除列表文件
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 图片文件
	CString strPicListPath = strUserFolder + "ClusterPic.list";
	DeleteFile(strPicListPath);

	// 清除内存
	m_vecClusterPic.clear();
}

// 将图片列表写入文件
void CLocalData::WriteAcceptedPicList(const char *pPicListFile, const vector<string> &vecFicList)
{
	// 写文件
	CStdioFile f;
	if ( f.Open(pPicListFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		for(int i = 0; i < vecFicList.size(); ++i)
		{
			string strPic = vecFicList[i];
			strPic += "\n";
			f.WriteString(strPic.c_str());
		}
		
		f.Close();
	}
}

// 将接收到的好友图片列表写入文件
void CLocalData::WriteAcceptedFriendPicList()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 图片文件
	CString strPicListPath = strUserFolder + "FriendPic.list";
	WriteAcceptedPicList(strPicListPath, m_vecFriendPic);
}

// 读接收到的群接收到的群图片列表
void CLocalData::WriteAcceptedClusterPicList()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 图片文件
	CString strPicListPath = strUserFolder + "ClusterPic.list";
	WriteAcceptedPicList(strPicListPath, m_vecClusterPic);
}

// 将自动回复（自动接待信息）写入ini文件
void CLocalData::SetAutoReply(const AUTO_REPLY &item, const char *pSectionName, IniFile &inifile)
{
	inifile.setStringValue(pSectionName, "TEXT", (LPCTSTR)item.strText);
	inifile.setStringValue(pSectionName, "FONTNAME", (LPCTSTR)item.font.fontName);
	inifile.setIntValue(pSectionName, "FONTSIZE", item.font.fontSize);
	inifile.setIntValue(pSectionName, "FONTFLAGS", (int)item.font.flags);
	inifile.setIntValue(pSectionName, "FONTCOLOR", (int)item.font.fontColor);
}

// 从Ini文件中读取自动回复（自动接待信息）
void CLocalData::GetAutoReply(IniFile &inifile, const char *pSectionName, AUTO_REPLY &item)
{
	int rlt = 0;
	item.strText = inifile.getStringValue(pSectionName, "TEXT", rlt).c_str();
	if(item.strText.IsEmpty())
	{
		item.strText = "欢迎光临";
	}
	CString strTemp;
	strTemp = inifile.getStringValue(pSectionName, "FONTNAME", rlt).c_str();
	if(strTemp.IsEmpty())
	{
		strTemp = "微软雅黑";
	}
	strcpy(item.font.fontName, strTemp);
	int nTemp = 0;
	nTemp = inifile.getIntValue(pSectionName, "FONTSIZE", rlt);
	item.font.fontSize = (nTemp <= 0 ? 12 : nTemp);
	nTemp = 0;
	nTemp = inifile.getIntValue(pSectionName, "FONTFLAGS", rlt);
	item.font.flags = nTemp;
	nTemp = 0;
	nTemp = inifile.getIntValue(pSectionName, "FONTCOLOR", rlt);
	item.font.fontColor = nTemp;
}

// 读取自动回复信息（包含自动接待）
void CLocalData::ReadAutoReply()
{
	m_vecAutoReplyItem.clear();
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 配置文件路径
	CString strIniPath = strUserFolder + "AutoReply.ini";
	if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}	

	// 读取
	IniFile iniFile;
	iniFile.load((LPCTSTR)strIniPath);

	int rlt = 0;	
	int nCount = iniFile.getIntValue("自动回复条目", "数量", rlt);
	for(int i = 0; i < nCount; ++i)
	{
		CString strSection;
		strSection.Format("条目%d", i);

		AUTO_REPLY item;
		GetAutoReply(iniFile, strSection, item);
		m_vecAutoReplyItem.push_back(item);
	}

	CString strSection = "在线自动接待";
	GetAutoReply(iniFile, strSection, m_AutoAcceptOnline);

	strSection = "离线自动接待";
	GetAutoReply(iniFile, strSection, m_AutoAcceptOffline);
}

// 保存自动回复信息（包含自动接待）
void CLocalData::SaveAutoReply()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 配置文件路径
	CString strIniPath = strUserFolder + "AutoReply.ini";
	//if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}
	
	// 写入
	IniFile iniFile;
	int nCount = m_vecAutoReplyItem.size();
	iniFile.setIntValue("自动回复条目", "数量", nCount);
	for(int i = 0; i < nCount; ++i)
	{
		CString strSection;
		strSection.Format("条目%d", i);
		SetAutoReply(m_vecAutoReplyItem[i], strSection, iniFile);
	}

	CString strSection = "在线自动接待";
	SetAutoReply(m_AutoAcceptOnline, strSection, iniFile);

	strSection = "离线自动接待";
	SetAutoReply(m_AutoAcceptOffline, strSection, iniFile);
	iniFile.saveas((LPCTSTR)strIniPath);
}

// 读取快捷短语
void CLocalData::ReadFastWord()
{
	m_vecFastWord.clear();
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 配置文件路径
	CString strIniPath = strUserFolder + "FastWord.ini";
	if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}	

	// 读取
	IniFile iniFile;
	iniFile.load((LPCTSTR)strIniPath);

	int rlt = 0;
	int nCount = 0;
	nCount = iniFile.getIntValue("快捷短语", "数量", rlt);
	for(int i = 0; i < nCount; ++i)
	{
		CString strName;
		strName.Format("短语%d", i);

		CString strValue;
		strValue = iniFile.getStringValue("快捷短语", (LPCTSTR)strName, rlt).c_str();
		m_vecFastWord.push_back((LPCTSTR)strValue);
	}
}

// 保存快捷短语
void CLocalData::SaveFastWord()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 配置文件路径
	CString strIniPath = strUserFolder + "FastWord.ini";
	//if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}	

	// 读取
	IniFile iniFile;

	int nCount = m_vecFastWord.size();
	iniFile.setIntValue("快捷短语", "数量", nCount);
	for(int i = 0; i < nCount; ++i)
	{
		CString strName;
		strName.Format("短语%d", i);
		iniFile.setStringValue("快捷短语", (LPCTSTR)strName, m_vecFastWord[i].c_str());
	}
	iniFile.saveas((LPCTSTR)strIniPath);
}

// 读取提示声音路径
void CLocalData::ReadVoiceInfo()
{
	m_mapVoicePath.clear();
	m_mapVoiceEnable.clear();
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 声音文件默认路径
	string strDefPath = string((LPCTSTR)GetModulePath()) + "Voice\\";
	m_mapVoicePath[EVOICE_RECEIVE_MSG] = strDefPath + "即时消息声-清脆声.wav";
	m_mapVoicePath[EVOICE_FRIEND_ONLINE] = strDefPath + "联系人上线-敲门声.wav";
	m_mapVoicePath[EVOICE_SYS_MSG] = strDefPath + "系统消息-牛铃4节奏.wav";
	m_mapVoicePath[EVOICE_SHOCK_SCREEN] = strDefPath + "闪屏抖动声-闪过音效.wav";
	m_mapVoicePath[EVOICE_VIDEO_CALL] = strDefPath + "视频呼叫声-长哞声.wav";
	m_mapVoicePath[EVOICE_VOICE_CALL] = strDefPath + "语音呼叫声-短哞声.wav";

	CString strIniPath = strUserFolder + "Voice.ini";
	if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();

		// 第一次创建文件，写入默认提示音路径
		CString strPath = GetModulePath() + "Voice\\";
		IniFile iniFile;
		iniFile.setStringValue("收到即时消息", "Path", strPath + "即时消息声-清脆声.wav");
		iniFile.setIntValue("收到即时消息", "Enable", 1);
		iniFile.setStringValue("联系人上线", "Path", strPath + "联系人上线-敲门声.wav");
		iniFile.setIntValue("联系人上线", "Enable", 1);
		iniFile.setStringValue("系统消息", "Path", strPath + "系统消息-牛铃4节奏.wav");
		iniFile.setIntValue("系统消息", "Enable", 1);
		iniFile.setStringValue("闪屏振动", "Path", strPath + "闪屏抖动声-闪过音效.wav");
		iniFile.setIntValue("闪屏振动", "Enable", 1);
		iniFile.setStringValue("视频呼叫", "Path", strPath + "视频呼叫声-长哞声.wav");
		iniFile.setIntValue("视频呼叫", "Enable", 1);
		iniFile.setStringValue("语音呼叫", "Path", strPath + "语音呼叫声-短哞声.wav");
		iniFile.setIntValue("语音呼叫", "Enable", 1);
		iniFile.saveas((LPCTSTR)strIniPath);
	}	

	// 读取
	IniFile iniFile;
	iniFile.load((LPCTSTR)strIniPath);

	int rlt = 0;
	CString strPath = "";
	strPath = iniFile.getStringValue("收到即时消息", "Path", rlt).c_str();
	int nEnable = 1;
	nEnable = iniFile.getIntValue("收到即时消息", "Enable", rlt);
	if ( _access(strPath,0)!=-1 )
		m_mapVoicePath[EVOICE_RECEIVE_MSG] = (LPCTSTR)strPath;
	m_mapVoiceEnable[EVOICE_RECEIVE_MSG] = nEnable;

	strPath = iniFile.getStringValue("联系人上线", "Path", rlt).c_str();
	nEnable = iniFile.getIntValue("联系人上线", "Enable", rlt);
	if ( _access(strPath,0)!=-1 )
		m_mapVoicePath[EVOICE_FRIEND_ONLINE] = (LPCTSTR)strPath;
	m_mapVoiceEnable[EVOICE_FRIEND_ONLINE] = nEnable;

	strPath = iniFile.getStringValue("系统消息", "Path", rlt).c_str();
	nEnable = iniFile.getIntValue("系统消息", "Enable", rlt);
	if ( _access(strPath,0)!=-1 )
		m_mapVoicePath[EVOICE_SYS_MSG] = (LPCTSTR)strPath;
	m_mapVoiceEnable[EVOICE_SYS_MSG] = nEnable;

	strPath = iniFile.getStringValue("闪屏振动", "Path", rlt).c_str();
	nEnable = iniFile.getIntValue("闪屏振动", "Enable", rlt);
	if ( _access(strPath,0)!=-1 )
		m_mapVoicePath[EVOICE_SHOCK_SCREEN] = (LPCTSTR)strPath;
	m_mapVoiceEnable[EVOICE_SHOCK_SCREEN] = nEnable;

	strPath = iniFile.getStringValue("视频呼叫", "Path", rlt).c_str();
	nEnable = iniFile.getIntValue("视频呼叫", "Enable", rlt);
	if ( _access(strPath,0)!=-1 )
		m_mapVoicePath[EVOICE_VIDEO_CALL] = (LPCTSTR)strPath;
	m_mapVoiceEnable[EVOICE_VIDEO_CALL] = nEnable;

	strPath = iniFile.getStringValue("语音呼叫", "Path", rlt).c_str();
	nEnable = iniFile.getIntValue("语音呼叫", "Enable", rlt);
	if ( _access(strPath,0)!=-1 )
		m_mapVoicePath[EVOICE_VOICE_CALL] = (LPCTSTR)strPath;
	m_mapVoiceEnable[EVOICE_VOICE_CALL] = nEnable;
}

// 保存提示声音路径
void CLocalData::SaveVoiceInfo()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);

	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	// 配置文件路径
	CString strIniPath = strUserFolder + "Voice.ini";
	//if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}	

	// 写入
	IniFile iniFile;
	iniFile.setStringValue("收到即时消息", "Path", m_mapVoicePath[EVOICE_RECEIVE_MSG].c_str());
	iniFile.setIntValue("收到即时消息", "Enable", (int)m_mapVoiceEnable[EVOICE_RECEIVE_MSG]);
	iniFile.setStringValue("联系人上线", "Path", m_mapVoicePath[EVOICE_FRIEND_ONLINE].c_str());
	iniFile.setIntValue("联系人上线", "Enable", (int)m_mapVoiceEnable[EVOICE_FRIEND_ONLINE]);
	iniFile.setStringValue("系统消息", "Path", m_mapVoicePath[EVOICE_SYS_MSG].c_str());
	iniFile.setIntValue("系统消息", "Enable", (int)m_mapVoiceEnable[EVOICE_SYS_MSG]);
	iniFile.setStringValue("闪屏振动", "Path", m_mapVoicePath[EVOICE_SHOCK_SCREEN].c_str());
	iniFile.setIntValue("闪屏振动", "Enable", (int)m_mapVoiceEnable[EVOICE_SHOCK_SCREEN]);
	iniFile.setStringValue("视频呼叫", "Path", m_mapVoicePath[EVOICE_VIDEO_CALL].c_str());
	iniFile.setIntValue("视频呼叫", "Enable", (int)m_mapVoiceEnable[EVOICE_VIDEO_CALL]);
	iniFile.setStringValue("语音呼叫", "Path", m_mapVoicePath[EVOICE_VOICE_CALL].c_str());
	iniFile.setIntValue("语音呼叫", "Enable", (int)m_mapVoiceEnable[EVOICE_VOICE_CALL]);
	iniFile.saveas((LPCTSTR)strIniPath);
}

string CLocalData::GetVoicePath(EM_VOICE_TYPE type)
{
	return m_mapVoicePath[type];
}

void CLocalData::SetVoicePath(EM_VOICE_TYPE type, const char *pVoicePath)
{
	m_mapVoicePath[type] = pVoicePath;
}

BOOL CLocalData::VoiceIsEnable(EM_VOICE_TYPE type)
{
	return m_mapVoiceEnable[type];
}

void CLocalData::EnableVoice(EM_VOICE_TYPE type, BOOL bEnable)
{
	m_mapVoiceEnable[type] = bEnable;
}

// 清空所有消息记录
void CLocalData::DeleteAllMsg()
{
	CString strUserPath;
	strUserPath.Format("%s\\%s\\",m_strPath,m_sUser);
	strUserPath += "*.msg";
	// 查找消息记录文件
	vector<string> vecMsgFile;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strUserPath);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDirectory() && !finder.IsDots())
		{
			CString strName = finder.GetFileName();
			vecMsgFile.push_back((LPCTSTR)finder.GetFilePath());
		}
	}

	// 删除文件
	for(int i = 0; i != vecMsgFile.size(); ++i)
	{
		DeleteFile(vecMsgFile[i].c_str());
	}

	// 删除最近联系人
	CString strFile;
	strFile.Format("%s\\%s\\recentNew.db",m_strPath,m_sUser);
	DeleteFile(strFile);
}
void CLocalData::ReadMediaDevSetting()
{
	if(m_MediaDevSetting == NULL)
	{
		m_MediaDevSetting = new MEDIA_DEV_SETTING();
	}

	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);
	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}
	CString strIniPath = strUserFolder + "MediaDevSetting.ini";
	if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}	
	IniFile iniFile;
	iniFile.load((LPCTSTR)strIniPath);
	int rlt = 0;
	m_MediaDevSetting->nIdxVoiceInSel = iniFile.getIntValue("语音视频设备", "声音输入选择", rlt);
	m_MediaDevSetting->nIdxVoiceOutSel = iniFile.getIntValue("语音视频设备", "声音输出选择", rlt);
	m_MediaDevSetting->nIdxVedioSel = iniFile.getIntValue("语音视频设备", "摄像头选择", rlt);
	m_MediaDevSetting->nVoiceInSize = iniFile.getIntValue("语音视频设备", "声音输入音量", rlt);
	m_MediaDevSetting->nVoiceOutSize = iniFile.getIntValue("语音视频设备", "声音输出音量", rlt);
}
void CLocalData::SaveMediaDevSetting()
{
	CString strUserFolder;
	strUserFolder.Format("%s\\%s\\",m_strPath, m_sUser);
	if ( _access(strUserFolder,0)==-1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}
	CString strIniPath = strUserFolder + "MediaDevSetting.ini";
	//if ( _access(strIniPath,0)==-1 )
	{	// 创建文件
		CStdioFile f;
		f.Open(strIniPath, CFile::typeText|CFile::modeCreate);
		f.Close();
	}
	IniFile iniFile;
	iniFile.setIntValue("语音视频设备", "声音输入选择", m_MediaDevSetting->nIdxVoiceInSel);
	iniFile.setIntValue("语音视频设备", "声音输出选择", m_MediaDevSetting->nIdxVoiceOutSel);
	iniFile.setIntValue("语音视频设备", "摄像头选择", m_MediaDevSetting->nIdxVedioSel);
	iniFile.setIntValue("语音视频设备", "声音输入音量", m_MediaDevSetting->nVoiceInSize);
	iniFile.setIntValue("语音视频设备", "声音输出音量", m_MediaDevSetting->nVoiceOutSize);
	iniFile.saveas((LPCTSTR)strIniPath);
}
void CLocalData::SetMediaDevSetting(MEDIA_DEV_SETTING *setting)
{
	m_MediaDevSetting = setting;
}
MEDIA_DEV_SETTING* CLocalData::GetMediaDevSetting()
{
	return m_MediaDevSetting;
}
int CLocalData::GetVoiceInVolumeSize()
{
	return m_MediaDevSetting->nVoiceInSize;
}
int CLocalData::GetVoiceOutVolumeSize()
{
	return m_MediaDevSetting->nVoiceOutSize;
}

void CLocalData::SetLoginTime(uint32 time)
{
	if(time != 0)
		m_timeLogin = time;
	m_tickLogin = GetTickCount();
}

uint32 CLocalData::GetTimeNow()
{
	uint32 tickNow = GetTickCount();
	uint32 time = (tickNow - m_tickLogin) / 1000 + m_timeLogin;
	return time;
}

// 获取保存程序运行过程中产生的圆形图片的路径
string CLocalData::GetRoundPath(const char *szImgName)
{
	CString strImgFolder;
	strImgFolder.Format("%s\\%s\\TempImg\\",m_strPath, m_sUser);
	if ( _access(strImgFolder,0)==-1 )
	{
		CreateDirectory(strImgFolder,NULL);
	}

	return (LPCTSTR)(strImgFolder + szImgName);
}

// 删除圆形图片
void CLocalData::RemoveRoundPath()
{
	CString strImgFolder;
	strImgFolder.Format("%s\\%s\\TempImg\\",m_strPath, m_sUser);

	DeleteDirectory(strImgFolder);
}

// 读取背景配置文件
void CLocalData::ReadBkInfo()
{
	CString strImgFile = m_strPath + "\\bkinfo.dat";

	CStdioFile file;
	if ( file.Open(strImgFile,CFile::modeRead|CFile::typeBinary) )
	{
		// 读取版本号
		uint8 uVer = 0;
		file.Read(&uVer, sizeof(uint8));
		if(uVer == 1)
		{
			// 读取数组个数
			DWORD dwCount = 0;
			file.Read(&dwCount, sizeof(DWORD));
			for(int i = 0; i < dwCount; ++i)
			{
				STRU_BK_INFO bk;
				file.Read(bk.szUserName, sizeof(bk.szUserName));
				file.Read(bk.szBkImg, sizeof(bk.szBkImg));
				file.Read(bk.szHeadImg, sizeof(bk.szHeadImg));

				vector<STRU_BK_INFO>::iterator it = std::find(m_vecBkInfo.begin(), m_vecBkInfo.end(), bk.szUserName);
				if(it == m_vecBkInfo.end())
				{
					m_vecBkInfo.push_back(bk);
				}
				else
				{
					strcpy(it->szBkImg, bk.szBkImg);
					strcpy(it->szHeadImg, bk.szHeadImg);
				}
			}
		}
		file.Close();
	}
}

// 保存背景图片到文件
void CLocalData::SaveBkInfo()
{
	CString strImgFile = m_strPath + "\\bkinfo.dat";

	CStdioFile file;
	if(file.Open(strImgFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		// 写入版本号
		uint8 uVer = BKINFO_VER;
		file.Write(&uVer, sizeof(uint8));

		// 写入数量
		DWORD dwCount = m_vecBkInfo.size();
		file.Write(&dwCount, sizeof(DWORD));

		// 写入背景信息
		for(int i = 0; i < dwCount; ++i)
		{
			STRU_BK_INFO &bk = m_vecBkInfo[i];
			file.Write(bk.szUserName, sizeof(bk.szUserName));
			file.Write(bk.szBkImg, sizeof(bk.szBkImg));
			file.Write(bk.szHeadImg, sizeof(bk.szHeadImg));
		}
	}
}

// 获取背景图片
string CLocalData::GetBkImage(const char *szUserName)
{
	vector<STRU_BK_INFO>::iterator it
		= find(m_vecBkInfo.begin(), m_vecBkInfo.end(), szUserName);

	if(it == m_vecBkInfo.end())
	{
		return "BKImage\\1.png";
	}
	else
	{
		return it->szBkImg;
	}
}

// 获取头像路径
string CLocalData::GetHeadImage(const char *szUserName)
{
	vector<STRU_BK_INFO>::iterator it
		= find(m_vecBkInfo.begin(), m_vecBkInfo.end(), szUserName);

	if(it == m_vecBkInfo.end())
	{
		return "SysBtn\\icon_100x100.png";
	}
	else
	{
		string strHeadImg = it->szHeadImg;
		// 判断文件是否存在
		if(FileExist(strHeadImg.c_str()))
			return it->szHeadImg;
		else
			return "SysBtn\\icon_100x100.png";
	}
}

// 设置背景图片
void CLocalData::SetBkImage(const char *szUserName, const char *szBkImg)
{
	vector<STRU_BK_INFO>::iterator it
		= find(m_vecBkInfo.begin(), m_vecBkInfo.end(), szUserName);

	if(it == m_vecBkInfo.end())
	{
		STRU_BK_INFO bk;
		strcpy(bk.szUserName, szUserName);
		strcpy(bk.szBkImg, szBkImg);
		m_vecBkInfo.push_back(bk);
	}
	else
	{
		strcpy(it->szUserName, szUserName);
		strcpy(it->szBkImg, szBkImg);
	}
}

// 设置头像
void CLocalData::SetHeadImage(const char *szUserName, const char *szHeadImg)
{
	vector<STRU_BK_INFO>::iterator it
		= find(m_vecBkInfo.begin(), m_vecBkInfo.end(), szUserName);

	if(it == m_vecBkInfo.end())
	{
		STRU_BK_INFO bk;
		strcpy(bk.szUserName, szUserName);
		strcpy(bk.szHeadImg, szHeadImg);
		m_vecBkInfo.push_back(bk);
	}
	else
	{
		strcpy(it->szUserName, szUserName);
		strcpy(it->szHeadImg, szHeadImg);
	}
}

/////test.dmp路径修改到"我的文档(C:\\Users)
CString CLocalData::SetDumpFilePath()
{
	CString sCfg;
	sCfg.Format("%s\\%s\\",m_strPath,m_sUser);
	return sCfg;
}

/////消息未读退出，下次聊天提醒
void CLocalData::SaveCloseTalkWarnConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"CloseTalkMsg", 
		"warn",
		s, 
		sCfg);
}

void CLocalData::GetCloseTalkWarnConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"CloseTalkMsg", 
		"warn",
		0, 
		sCfg);
}

/////消息未读退出,下次登录提醒
void CLocalData::SaveExitMsgWarnConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"ExitWarnMsg", 
		"warn",
		s, 
		sCfg);
}

void CLocalData::GetExitMsgWarnConfig(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"ExitWarnMsg", 
		"warn",
		0, 
		sCfg);
}

/////退出或者切换用户，下次登录时是否弹出未读消息
void CLocalData::SaveLoginUnReadMsgCfg(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"LoginWarnMsg", 
		"warn",
		s, 
		sCfg);
}

void CLocalData::GetLoginUnReadMsgCfg(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"LoginWarnMsg", 
		"warn",
		0, 
		sCfg);
}

//复制文件的函数
BOOL CLocalData::CommonCopyFile(CString SourceFileName, CString DestFileName)
{
	CFile sourceFile ;
	CFile destFile ;
	CFileException ex;
	if (!sourceFile.Open((LPCTSTR)SourceFileName,CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString ErrorMsg = "打开文件：" ;
		ErrorMsg += SourceFileName ;
		ErrorMsg += "失败。n错误信息为：n" ;
		ErrorMsg += szError ;
		//AfxMessageBox(ErrorMsg);
		return FALSE ;
	}
	else
	{
		if (!destFile.Open((LPCTSTR)DestFileName, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex))
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			CString ErrorMsg = "创建文件：" ;
			ErrorMsg += DestFileName ;
			ErrorMsg += "失败。n错误信息为：n" ;
			ErrorMsg += szError ;
			//AfxMessageBox(ErrorMsg);
			sourceFile.Close();
			return FALSE ;
		}
		BYTE buffer[4096];
		DWORD dwRead;
		do
		{
			dwRead = sourceFile.Read(buffer, 4096);
			destFile.Write(buffer, dwRead);
		}
		while (dwRead > 0); 
		destFile.Close();
		sourceFile.Close();
	}
	return TRUE ;
}

//调运营后台接口获取快捷短语
static long curl_writer(void *data, int size, int nmemb, string *content)
{
	long sizes = size * nmemb;
	*content += string((char*)data, sizes);
	return sizes;
}

#ifdef _CS_PLAT_EX_
void CLocalData::LoadFastWordInfo()
{
	CString strURL = QUICK_PHRASE_URL;
	

	CString strParam;
	strParam.Format("customerServiceId=%d", g_ImUserInfo.GetId());
	strURL += strParam;

	CURL *curl;
	CURLcode res;
	string strRet;
	string strError;

	curl = curl_easy_init();
	if(curl) 
	{
		string strHttp = strURL;
		curl_easy_setopt(curl, CURLOPT_URL,strHttp.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRet); 

#ifdef _VER_PREVIEW_		
		curl_easy_setopt(curl, CURLOPT_PROXY, PROXY_URL); 
#endif
		//设置cookie
		extern CString g_strToken;
		CString cookie = g_strToken;
		cookie = "\"" + cookie + "\"";
		cookie = "YOWEJKRLWKFSJK=" + cookie;
		curl_easy_setopt(curl, CURLOPT_COOKIE , cookie); 
		
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
			string strRetutf8 = UTF8_Convert(strRet.c_str(), FALSE);
			//解析json字符创
			Json::Reader reader;
			Json::Value recv;
			if (reader.parse(strRetutf8, recv))
			{
				//获取result字段
				string result = recv["success"].asString();
				if(result == "true")
				{
					int nCount = recv["data"]["quickPhraseList"].size();
					for(int i = 0; i < nCount; ++i)
					{
						Json::Value strList = recv["data"]["quickPhraseList"][i];
						int phraseid = strList["phraseId"].asInt(); //288
						string phraseSummary = strList["phraseSummary"].asString(); //(可用)快捷短语 "xx"
						string phraseGroupName = strList["phraseGroupName"].asString();//(可用)快捷短语分组 "未分组",
						string quickCode = strList["quickCode"].asString(); //(可用)快捷编码 "x",
						string lastOperator = strList["lastOperator"].asString(); //"test",
						int fontSize = strList["fontSize"].asInt(); //(可用) 12,
						int fontColor = strList["fontColor"].asInt(); //(可用) 0,
						int fontStyle = strList["fontStyle"].asInt(); //(可用) 0,
						string fontName = strList["fontName"].asString(); //(可用) "'Microsoft Yahei'",
						string updateTime = strList["updateTime"].asString(); //"2015-11-27 08:48:06"
						CString strGroupName = phraseGroupName.c_str();
						vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
						if(it == m_vecFastWordInfo.end())
						{
							FastWordGroupInfo fastWordGroupInfo;
							fastWordGroupInfo.bIsLoad = TRUE;
							fastWordGroupInfo.strGroupName = phraseGroupName.c_str();
							m_vecFastWordInfo.push_back(fastWordGroupInfo);
							it = m_vecFastWordInfo.end();
							it--;
						}
						FastWordGroupInfo& fastWordGroupInfo = *it;

						//快捷短语改为不去重
						CString strFastWord = phraseSummary.c_str();
						FastWordInfo fastWordInfo;
						fastWordInfo.bIsLoad = TRUE;
						fastWordInfo.strWord = strFastWord;
						fastWordInfo.strNum = quickCode.c_str();
						CString strFontName = fontName.c_str();
						strFontName.Remove('\'');
						if(strFontName == "")
							strFontName = "微软雅黑";
						strcpy(fastWordInfo.font.fontName, /*fontName.c_str()*/strFontName);
						fastWordInfo.font.fontColor = fontColor;
						if(fontSize == 0)
							fontSize = 12;
						fastWordInfo.font.fontSize = fontSize;
						fastWordInfo.font.flags = fontStyle;

						fastWordGroupInfo.vecFastWordInfo.push_back(fastWordInfo);
					}
				}
			}
		}
	}
}
#endif

vector<FastWordGroupInfo> CLocalData::GetFastWordInfo()
{
	return m_vecFastWordInfo;
}

void CLocalData::SetFastWordInfo(vector<FastWordGroupInfo>& vecFastWordInfo)
{
	m_vecFastWordInfo.clear();
	m_vecFastWordInfo = vecFastWordInfo;
	if(g_pMessageHandleWnd != NULL)
		g_pMessageHandleWnd->UpdateAllFastWord();
}

BOOL CLocalData::ReadFastWordInfo()
{
	CString strFastWordCsv;
	strFastWordCsv.Format("%s\\%s\\FastWord.csv",m_strPath,m_sUser);

	if(!FileExist(strFastWordCsv))
	{
		//CString strFastWordCsvCustom = g_LocalData.GetProgramPath() + "\\ExcelTemplate\\FastWordCustom.csv";
		//CommonCopyFile(strFastWordCsvCustom, strFastWordCsv);
		return FALSE;
	}
	//else
	//	m_vecFastWordInfo.clear();
	CCSVFile file(strFastWordCsv);
	CStringArray arrayHead;
	file.ReadData(arrayHead);
	if(arrayHead.GetSize() < 3)
	{
		return FALSE;
	}
	if(arrayHead.GetAt(0) != "快捷编码" || arrayHead.GetAt(1) != "快捷短语" || arrayHead.GetAt(2) != "快捷短语分组")
	{
		return FALSE;
	}
	CStringArray arrayBody;
	while (file.ReadData(arrayBody))
	{
		CString strFastWordInfoNum = "";
		CString strFastWord = "";
		CString strGroupName = "";
		CString strFontFlag = "";
		CString strFontColor = "";
		CString strFontName = "";
		CString strFontSize = "";

		for(int nBody = 0; nBody < arrayBody.GetSize(); nBody++)
		{
			if(nBody == 0)
				strFastWordInfoNum = arrayBody.GetAt(nBody);
			else if(nBody == 1)
				strFastWord = arrayBody.GetAt(nBody);
			else if(nBody == 2)
				strGroupName = arrayBody.GetAt(nBody);
			else if(nBody == 3)
				strFontFlag = arrayBody.GetAt(nBody);
			else if(nBody == 4)
				strFontColor = arrayBody.GetAt(nBody);
			else if(nBody == 5)
				strFontName = arrayBody.GetAt(nBody);
			else if(nBody == 6)
				strFontSize = arrayBody.GetAt(nBody);
		}
		if(strGroupName == "")
			strGroupName = "未分组";
		strFastWord.Replace("&#13;", "\r");
		strFastWord.Replace("&#44;", ",");
		strGroupName.Replace("&#13;", "\r");
		strGroupName.Replace("&#44;", ",");

		vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
		if(it == m_vecFastWordInfo.end())
		{
			FastWordGroupInfo fastWordGroupInfo;
			fastWordGroupInfo.strGroupName = strGroupName;
			m_vecFastWordInfo.push_back(fastWordGroupInfo);
			it = m_vecFastWordInfo.end();
			it--;
		}
		FastWordGroupInfo& fastWordGroupInfo = *it;

		//快捷短语改为不去重
		FastWordInfo fastWordInfo;
		fastWordInfo.strWord = strFastWord;
		fastWordInfo.strNum = strFastWordInfoNum;
		strcpy(fastWordInfo.font.fontName, strFontName);
		fastWordInfo.font.fontColor = atoi(strFontColor);
		fastWordInfo.font.fontSize = atoi(strFontSize);
		fastWordInfo.font.flags = atoi(strFontFlag);

		fastWordGroupInfo.vecFastWordInfo.push_back(fastWordInfo);
	}
	file.Close();
	return TRUE;
}

void CLocalData::SaveFastWordInfo()
{
	CString strFastWordCsv;
	strFastWordCsv.Format("%s\\%s\\FastWord.csv",m_strPath,m_sUser);
	if(FileExist(strFastWordCsv))
	{
		CStdioFile   file( strFastWordCsv,CFile::modeWrite);
		file.SetLength(0);
		file.Close();
	}
	CCSVFile file(strFastWordCsv, CCSVFile::modeWrite);;
	CStringArray arrayHead;
	arrayHead.Add("快捷编码");
	arrayHead.Add("快捷短语");
	arrayHead.Add("快捷短语分组");
	arrayHead.Add("");
	arrayHead.Add("");
	arrayHead.Add("");
	arrayHead.Add("");
	file.WriteData(arrayHead);
	for(int nGroupInfo = 0; nGroupInfo < m_vecFastWordInfo.size(); nGroupInfo++)
	{
		FastWordGroupInfo& fastWordGroupInfo = m_vecFastWordInfo[nGroupInfo];
		for(int nFastWordInfo = 0; nFastWordInfo < fastWordGroupInfo.vecFastWordInfo.size(); nFastWordInfo++)
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWordInfo];
			if(fastWordInfo.bIsLoad)//如果为运营后台下载的快阶段则不存入本地文件
				continue;

			CString strFastWord = fastWordInfo.strWord;
			CString strGroupName = fastWordGroupInfo.strGroupName;

			strFastWord.Replace("\r\n", "&#13;");
			strFastWord.Replace("\n", "&#13;");
			strFastWord.Replace("\r", "&#13;");
			strFastWord.Replace(",", "&#44;");
			strGroupName.Replace("\r\n", "&#13;");
			strGroupName.Replace("\n", "&#13;");
			strGroupName.Replace("\r", "&#13;");
			strGroupName.Replace(",", "&#44;");

			CStringArray arrayBody;
			CString strFastWordNum;
			strFastWordNum = fastWordInfo.strNum;

			//CString strGroupName = fastWordGroupInfo.strGroupName;
			//if(fastWordGroupInfo.strGroupName == "")
			//	strGroupName = "未分组";
			arrayBody.Add(strFastWordNum);
			arrayBody.Add(strFastWord);
			arrayBody.Add(strGroupName);
			CString strFontFlag = "";
			strFontFlag.Format("%d", fastWordInfo.font.flags);
			CString strFontColor = "";
			strFontColor.Format("%d", fastWordInfo.font.fontColor);
			CString strFontName = fastWordInfo.font.fontName;
			CString strFontSize = "";
			strFontSize.Format("%d", fastWordInfo.font.fontSize);

			arrayBody.Add(strFontFlag);
			arrayBody.Add(strFontColor);
			arrayBody.Add(strFontName);
			arrayBody.Add(strFontSize);
			file.WriteData(arrayBody);
		}
		if(fastWordGroupInfo.vecFastWordInfo.size() == 0)
		{
			CString strGroupName = fastWordGroupInfo.strGroupName;
			strGroupName.Replace("\r\n", "&#13;");
			strGroupName.Replace("\n", "&#13;");
			strGroupName.Replace("\r", "&#13;");
			strGroupName.Replace(",", "&#44;");

			CStringArray arrayBody;
			arrayBody.Add("");
			arrayBody.Add("");
			arrayBody.Add(strGroupName);
			arrayBody.Add("");
			arrayBody.Add("");
			arrayBody.Add("");
			arrayBody.Add("");
			file.WriteData(arrayBody);
		}
	}
	file.Close();
}

int CLocalData::GetGroupIndex(CString strFastWordGroupName)
{
	int nFirst = 0;//除了未分组第一个分组不能上移
	if(m_vecFastWordInfo.size() > 0)
	{
		if(m_vecFastWordInfo[0].strGroupName == "未分组")
			nFirst = 1;
	}
	for(int nGroup = 0; nGroup < m_vecFastWordInfo.size(); nGroup++)
	{
		
		if(m_vecFastWordInfo[nGroup].strGroupName == strFastWordGroupName)
		{
			if(nGroup == m_vecFastWordInfo.size() - 1 && nGroup == nFirst)
				return 3;
			else if(nGroup == nFirst)
				return 1;
			else if(nGroup == m_vecFastWordInfo.size() - 1)
				return 2;
		}
	}
	return 0;
}

int CLocalData::GetNodeIndex(CString strGroupName, int nFastWordIndex)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			if(nFastWordIndex == fastWordGroupInfo.vecFastWordInfo.size()-1 && nFastWordIndex == 0)
				return 3;
			else if(nFastWordIndex == 0)
				return 1;
			else if(nFastWordIndex == fastWordGroupInfo.vecFastWordInfo.size()-1)
			{
				if(nFastWordIndex != 0)//如果上一个快捷短语为运营后台下载的那么不能上移
				{
					vector<FastWordInfo>::iterator itr = fastWordGroupInfo.vecFastWordInfo.begin() + nFastWordIndex - 1;
					if(itr->bIsLoad)
						return 3;
				}
				return 2;
			}
			else if(nFastWordIndex != 0)//如果上一个快捷短语为运营后台下载的那么不能上移
			{
				vector<FastWordInfo>::iterator itr = fastWordGroupInfo.vecFastWordInfo.begin() + nFastWordIndex - 1;
				if(itr->bIsLoad)
					return 1;
			}
		}
	}
	return 0;
}

BOOL CLocalData::IsGroupLoad(CString strGroupName)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		if(it->bIsLoad)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

BOOL CLocalData::IsFastWordLoad(CString strGroupName, int nFastWordIndex)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWordIndex];
			if(fastWordInfo.bIsLoad)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}

CString CLocalData::GetFastWordText(CString strGroupName, int nFastWordIndex)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWordIndex];
			return fastWordInfo.strWord;
		}
	}
	return "";
}

int CLocalData::GetGroupCount()
{
	return m_vecFastWordInfo.size();
}

void CLocalData::FastWordGroupUp(CString strFastWordGroupName)
{
	vector<FastWordGroupInfo>::iterator itr = m_vecFastWordInfo.begin();
	for (; itr != m_vecFastWordInfo.end(); itr++)
	{
		if(itr->strGroupName == strFastWordGroupName)
		{
			vector<FastWordGroupInfo>::iterator itrNew = itr--;
			FastWordGroupInfo fastWordGroupInfo = *itr;
			m_vecFastWordInfo.erase(itr);
			m_vecFastWordInfo.insert(itrNew, fastWordGroupInfo);
			break;
		}
	}
}

void CLocalData::FastWordGroupDown(CString strFastWordGroupName)
{
	vector<FastWordGroupInfo>::iterator itr = m_vecFastWordInfo.begin();
	for (; itr != m_vecFastWordInfo.end(); itr++)
	{
		if(itr->strGroupName == strFastWordGroupName)
		{
			vector<FastWordGroupInfo>::iterator itrNew = itr++;
			FastWordGroupInfo fastWordGroupInfo = *itr;
			m_vecFastWordInfo.erase(itr);
			m_vecFastWordInfo.insert(itrNew, fastWordGroupInfo);
			break;
		}
	}
}

void CLocalData::FastWordGroupDelete(CString strFastWordGroupName)
{
	vector<FastWordGroupInfo>::iterator itr = m_vecFastWordInfo.begin();
	for (; itr != m_vecFastWordInfo.end(); itr++)
	{
		if(itr->strGroupName == strFastWordGroupName)
		{
			FastWordGroupInfo fastWordGroupInfo = *itr;
			m_vecFastWordInfo.erase(itr);
			break;
		}
	}
}

void CLocalData::FastWordUp(CString strGroupName, int nFastWordIndex)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			vector<FastWordInfo>::iterator itrNode = fastWordGroupInfo.vecFastWordInfo.begin() + nFastWordIndex;
			vector<FastWordInfo>::iterator itrNew = itrNode--;
			FastWordInfo fastWordInfo = *itrNode;
			fastWordGroupInfo.vecFastWordInfo.erase(itrNode);
			fastWordGroupInfo.vecFastWordInfo.insert(itrNew, fastWordInfo);
		}
	}
}

void CLocalData::FastWordDown(CString strGroupName, int nFastWordIndex)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			vector<FastWordInfo>::iterator itrNode = fastWordGroupInfo.vecFastWordInfo.begin() + nFastWordIndex;
			vector<FastWordInfo>::iterator itrNew = itrNode++;
			FastWordInfo fastWordInfo = *itrNode;
			fastWordGroupInfo.vecFastWordInfo.erase(itrNode);
			fastWordGroupInfo.vecFastWordInfo.insert(itrNew, fastWordInfo);
		}
	}
}

void CLocalData::FastWordDelete(CString strGroupName, int nFastWordIndex)
{
	vector<FastWordGroupInfo>::iterator it = std::find(m_vecFastWordInfo.begin(), m_vecFastWordInfo.end(), strGroupName);
	if(it != m_vecFastWordInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			vector<FastWordInfo>::iterator itrNode = fastWordGroupInfo.vecFastWordInfo.begin() + nFastWordIndex;
			fastWordGroupInfo.vecFastWordInfo.erase(itrNode);
		}
	}
}

void CLocalData::FastWordGroupRename(CString strGroupName, CString strChangedGroupName)
{
	vector<FastWordGroupInfo>::iterator itr = m_vecFastWordInfo.begin();
	for (; itr != m_vecFastWordInfo.end(); itr++)
	{
		if(itr->strGroupName == strGroupName)
		{
			itr->strGroupName = strChangedGroupName;
		}
	}
}

XT_FONT CLocalData::GetFastWordFont(CString strFastWord)
{
	vector<FastWordGroupInfo>::iterator itr = m_vecFastWordInfo.begin();
	for (; itr != m_vecFastWordInfo.end(); itr++)
	{
		vector<FastWordInfo>::iterator itrNode = itr->vecFastWordInfo.begin();
		for(; itrNode != itr->vecFastWordInfo.end(); itrNode++)
		{
			if(itrNode->strWord == strFastWord)
			{
				return itrNode->font;
			}
		}
	}
	return g_ImFont;
}


////保存未读消息的好友id及消息个数
void CLocalData::SaveUnReadParamsCfg(DWORD & nVk,DWORD & nVk2,int nKeyIndex)
{
	CString strFid = _T("");
	CString strCnt = _T("");
	strFid.Format(_T("FID%d"),nKeyIndex); ////好友ID序号
	strCnt.Format(_T("CNT%d"),nKeyIndex); ////未读数序号

	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%u",nVk);
	::WritePrivateProfileString(
		"UnReadMsgList", 
		strFid,
		s, 
		sCfg);
	
		s.Format("%u",nVk2);
	::WritePrivateProfileString(
		"UnReadMsgList", 
		strCnt,
		s, 
		sCfg);
}

void CLocalData::GetUnReadParamsCfg(DWORD & nVk,DWORD & nVk2,int nKeyIndex)
{
	CString strFid = _T("");
	CString strCnt = _T("");
	strFid.Format(_T("FID%d"),nKeyIndex+1); ////好友ID
	strCnt.Format(_T("CNT%d"),nKeyIndex+1); ////未读数
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"UnReadMsgList", 
		strFid,
		0, 
		sCfg);

	nVk2 = ::GetPrivateProfileInt(
		"UnReadMsgList", 
		strCnt,
		0, 
		sCfg);
}



////保存未读消息窗口总数
void CLocalData::SaveUnReadWndCnt(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s;
	s.Format("%d",nVk);
	::WritePrivateProfileString(
		"UnReadWindow", 
		"total",
		s, 
		sCfg);
}


/////获取未读消息窗口数
void CLocalData::GetUnReadWndCnt(WORD & nVk)
{
	CString sCfg;
	sCfg.Format("%s\\%s\\Config.ini",m_strPath,m_sUser);
	CString s = _T("");
	nVk = ::GetPrivateProfileInt(
		"UnReadWindow", 
		"total",
		0, 
		sCfg);
}

// 读取老的最近联系人文件
void CLocalData::ReadOldRecentDB(vector<FRIEND_INFO_OLD> &vecOldFL)
{
	CString strFile;
	strFile.Format("%s\\%s\\recent.db",m_strPath,m_sUser);
	int nVersion = 0;
	CStdioFile file;

	if ( file.Open(strFile,CFile::modeRead|CFile::typeBinary) )
	{
		try
		{
			// 读出版本
			file.Read(&nVersion,sizeof(nVersion));

			if(nVersion == 2)
			{			
				FRIEND_INFO_OLD *pFriend=new FRIEND_INFO_OLD;
				char * buff = new char[8192];

				while ( file.Read(buff,sizeof(FRIEND_INFO_OLD))==sizeof(FRIEND_INFO_OLD) )
				{
					memcpy(pFriend,buff,sizeof(FRIEND_INFO_OLD));
					vecOldFL.push_back(*pFriend);
				}

				delete pFriend;
				delete [] buff;
			}
		}
		catch(...)
		{
		}


		file.Close();
	}
}

// 转换为新的内存
void CLocalData::RecentDBCovent(vector<FRIEND_INFO_OLD> &vecOldFL, vector<FRIEND_INFO> &vecFL)
{
	for(int i = 0; i < (int)vecOldFL.size(); ++i)
	{
		FRIEND_INFO_OLD &oldF = vecOldFL[i];
		FRIEND_INFO newF;
		memcpy((char*)&newF, (char*)&oldF, 4);	
		memcpy((char*)&newF+4, (char*)&oldF+4, 41);
		memcpy((char*)&newF+125, (char*)&oldF+45, sizeof(FRIEND_INFO_OLD)-45);
		newF.SelfEncrypt();
		vecFL.push_back(newF);
	}
}

// 保存新的最近联系人db文件
void CLocalData::SaveNewRecentDbFile(vector<FRIEND_INFO> &vecFL)
{
	CString strFile;
	strFile.Format("%s\\%s\\recentNew.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");
		
		
		if(bDbFileExist && db.tableExists("IM_FRIEND"))
		{	// 文件存在，清空表
			db.execDML("delete from IM_FRIEND");
		}
		else
		{	// 文件不存在，创建表
			db.execDML("create table IM_FRIEND(	\
				Ver number(3) not null,\
				UserId number(20) primary key,	\
				UserName	varchar2(150) not null,\
				NickName	varchar2(150),		\
				Email		varchar2(80),		\
				Birthday	varchar2(10),		\
				CommentName	varchar2(150),		\
				FaceUrl		varchar2(120),		\
				FlashUrl	varchar2(120),		\
				BindMobile	number(3),			\
				Sex			number(3),			\
				Age			number(3),			\
				Province	number(3),			\
				Country		number(3),			\
				City		number(3),			\
				Career		number(3),			\
				Rule		number(3),			\
				FamilyFlag	number(3),			\
				Linkpop		number(3),			\
				Address		varchar2(255),		\
				UserSign	varchar2(255),		\
				Description	varchar2(255),		\
				ProfileName	varchar2(100),		\
				Guid		varchar2(100),		\
				GroupIndex	number(10),			\
				ClientVer	number(20),			\
				Ip			number(20),			\
				LocalIp		number(20),			\
				Port		number(10),			\
				LocalPort	number(10),			\
				ConnFlag	number(3),			\
				Key			varchar2(20),		\
				Level		number(3),			\
				Equipment	number(3),			\
				ClusterProperty number(3),			\
				ClusterMember number(3),			\
				Status number(3),			\
				WebLocation varchar2(120),		\
				IsRefreshed	number(3),			\
				IsClosePanel number(3),			\
				P2pRetry number(3),			\
				IsBasicInfo number(3),			\
				IsTalkZYF number(3),			\
				HasWebInfo number(3),			\
				IsInviting number(3),			\
				Reserved number(20)				\
				);");
		}

		// 插入数据
		for(int i = 0; i < (int)vecFL.size(); ++i)
		{
			FRIEND_INFO &fi = vecFL[i];

			CString strInsertSql; 
			strInsertSql.Format("insert into IM_FRIEND values(%d,%d,'%s','%s','%s','%s','%s','%s','%s',\
								%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s',%d,%d,\
								%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d);", 
								VER_FRIEND_INFO,
								fi.GetId(), 
								SqlEncode(fi.GetUserName().c_str()),
								SqlEncode(fi.nickname),
								SqlEncode(fi.email),
								SqlEncode(fi.birthday),
								SqlEncode(fi.comment_name),
								SqlEncode(fi.face_url),
								SqlEncode(fi.flash_url),
								fi.bind_mobile,
								fi.sex,
								fi.age,
								fi.province,
								fi.country,
								fi.city,
								fi.career,
								fi.rule,
								fi.familyflag,
								fi.linkpop,
								SqlEncode(fi.address),
								SqlEncode(fi.user_sign),
								SqlEncode(fi.description),
								SqlEncode(fi.profile_name),
								SqlEncode(fi.GetGuid().c_str()),
								fi.group_index,
								fi.ver,
								fi.ip,
								fi.localIP,
								fi.port,
								fi.localPort,
								fi.connFlag,
								SqlEncode(fi.key),
								fi.level,
								fi.equipment,
								fi.cluster_property,
								fi.cluster_member,
								fi.status,
								SqlEncode(fi.web_location),
								fi.bRefreshed,
								fi.bClosePanel,
								fi.p2pRetry,
								fi.bBasicInfo,
								fi.isTalkZYF,
								fi.webInfo,
								fi.bInviting,
								fi.reserved);

			db.execDML(strInsertSql);
		}

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write recentNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 读取最近联系人新的sqlite db文件
void CLocalData::ReadNewRecentDbFile(FRIEND_LIST &fl)
{
	CString strFile;
	strFile.Format("%s\\%s\\recentNew.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;
	if(!bDbFileExist)
		return;		// 文件不存在，返回

	// 读取文件数据
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		if(!db.tableExists("IM_FRIEND"))
			return;		// 表不存在，直接返回

		// 读取表数据
		CppSQLite3Table t = db.getTable("select * from IM_FRIEND;");

		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);
			FRIEND_INFO *pFriend = new FRIEND_INFO();
			pFriend->SelfEncrypt();

			int nVer = atoi(t.fieldValue("Ver"));
			pFriend->SetId(atoi(t.fieldValue("UserId")));
			pFriend->SetUserName((LPCTSTR)SqlDecode(t.fieldValue("UserName")));
			strcpy(pFriend->nickname, (LPCTSTR)SqlDecode(t.fieldValue("NickName")));
			strcpy(pFriend->email, (LPCTSTR)SqlDecode(t.fieldValue("Email")));
			strcpy(pFriend->birthday, (LPCTSTR)SqlDecode(t.fieldValue("Birthday")));
			strcpy(pFriend->comment_name, (LPCTSTR)SqlDecode(t.fieldValue("CommentName")));
			strcpy(pFriend->face_url, (LPCTSTR)SqlDecode(t.fieldValue("FaceUrl")));
			strcpy(pFriend->flash_url, (LPCTSTR)SqlDecode(t.fieldValue("FlashUrl")));
			pFriend->bind_mobile = atoi(t.fieldValue("BindMobile"));
			pFriend->sex = atoi(t.fieldValue("Sex"));
			pFriend->age = atoi(t.fieldValue("Age"));
			pFriend->province = atoi(t.fieldValue("Province"));
			pFriend->country = atoi(t.fieldValue("Country"));
			pFriend->city = atoi(t.fieldValue("City"));
			pFriend->career = atoi(t.fieldValue("Career"));
			pFriend->rule = atoi(t.fieldValue("Rule"));
			pFriend->familyflag = atoi(t.fieldValue("FamilyFlag"));
			pFriend->linkpop = atoi(t.fieldValue("Linkpop"));
			strcpy(pFriend->address, (LPCTSTR)SqlDecode(t.fieldValue("Address")));
			strcpy(pFriend->user_sign, (LPCTSTR)SqlDecode(t.fieldValue("UserSign")));
			strcpy(pFriend->description, (LPCTSTR)SqlDecode(t.fieldValue("Description")));
			strcpy(pFriend->profile_name, (LPCTSTR)SqlDecode(t.fieldValue("ProfileName")));
			pFriend->SetGuid((LPCTSTR)SqlDecode(t.fieldValue("Guid")));
			pFriend->group_index = atoi(t.fieldValue("GroupIndex"));
			pFriend->ver = atoi(t.fieldValue("ClientVer"));
			pFriend->ip = atoi(t.fieldValue("Ip"));
			pFriend->localIP = atoi(t.fieldValue("LocalIp"));
			pFriend->port = atoi(t.fieldValue("Port"));
			pFriend->localPort = atoi(t.fieldValue("LocalPort"));
			pFriend->connFlag = atoi(t.fieldValue("ConnFlag"));
			strcpy(pFriend->key, (LPCTSTR)SqlDecode(t.fieldValue("Key")));
			pFriend->level = atoi(t.fieldValue("Level"));
			pFriend->equipment = atoi(t.fieldValue("Equipment"));
			pFriend->cluster_property = atoi(t.fieldValue("ClusterProperty"));
			pFriend->cluster_member = atoi(t.fieldValue("ClusterMember"));
			pFriend->status = atoi(t.fieldValue("Status"));
			strcpy(pFriend->web_location, (LPCTSTR)SqlDecode(t.fieldValue("WebLocation")));
			pFriend->bRefreshed = atoi(t.fieldValue("IsRefreshed"));
			pFriend->bClosePanel = atoi(t.fieldValue("IsClosePanel"));
			pFriend->p2pRetry = atoi(t.fieldValue("P2pRetry"));
			pFriend->bBasicInfo = atoi(t.fieldValue("IsBasicInfo"));
			pFriend->isTalkZYF = atoi(t.fieldValue("IsTalkZYF"));
			pFriend->webInfo = atoi(t.fieldValue("HasWebInfo"));
			pFriend->bInviting = atoi(t.fieldValue("IsInviting"));
			pFriend->reserved = atoi(t.fieldValue("Reserved"));

			pFriend->status = XTREAM_OFFLINE;
			fl.push_back(pFriend);
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("read recentNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 保存新的登录账户文件
void CLocalData::SaveNewUserAccountListDbFile(USER_ACCOUNT_LIST &ul, const char *szLastUsername)
{
	CString strFile = m_strPath+"\\csctaNew.db";

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(bDbFileExist && db.tableExists("IM_UserAccountList"))
		{	// 文件存在，清空表
			db.execDML("delete from IM_UserAccountList");
		}
		else
		{	// 文件不存在，创建表
			db.execDML("create table IM_UserAccountList(\
					   Ver number(3) not null,\
					   UserName blob,\
					   Password	blob,\
					   IsHide number(3),\
					   LoginFlag number(3),\
					   IsAutoLogin number(3),\
					   IsRemeberPsw	number(3),\
					   IsShowHint number(3),\
					   ServerIp number(20),\
					   IsLast number(3)\
					   );");
		}

		// 插入数据
		USER_ACCOUNT_LIST::iterator it = ul.begin();
		for(; it != ul.end(); ++it)
		{
			BOOL bLast = strcmp(it->cUsername, szLastUsername) == 0;

			CppSQLite3Binary blobUserName;
			char crypt[1024] = {0};
			long crypt_len = 0;
			TEAEncrypt(it->cUsername, sizeof it->cUsername, (char *)key, crypt, &crypt_len);
			blobUserName.setBinary((const unsigned char*)crypt, crypt_len);

			CppSQLite3Binary blobPassword;
			memset(crypt, 0, 1024);
			TEAEncrypt(it->cPass, sizeof it->cPass, (char *)key, crypt, &crypt_len);
			blobPassword.setBinary((const unsigned char*)crypt, crypt_len);

			CppSQLite3Buffer strInsertSql; 
			strInsertSql.format("insert into IM_UserAccountList values(%d,%Q,%Q,%d,%d,%d,%d,%d,%d,%d);", 
				VER_NEAR_ACCOUNT_INFO,
				blobUserName.getEncoded(),
				blobPassword.getEncoded(),
				it->bHide,
				it->iLoginFlag,
				it->bAutoLogin,
				it->bRemeberPsw,
				it->bShowHint,
				it->dwServerIP,
				bLast);

			db.execDML(strInsertSql);
		}

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write csctaNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 读取登录账户的sqlite db文件
void CLocalData::ReadNewUserAccountListDbFile(USER_ACCOUNT_LIST &ul, CString &strLastUserName)
{
	CString strFile = m_strPath+"\\csctaNew.db";

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;
	if(!bDbFileExist)
		return;		// 文件不存在，返回

	// 读取文件数据
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		if(!db.tableExists("IM_UserAccountList"))
			return;		// 表不存在，直接返回

		// 读取表数据
		CppSQLite3Table t = db.getTable("select * from IM_UserAccountList;");

		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);

			RegAccountStruct ac;
			long len = 0;

			// 解密用户名
			CppSQLite3Binary blobUserName;
			blobUserName.setEncoded((unsigned char*)t.fieldValue("UserName"));
			TEADecrypt((char*)blobUserName.getBinary(), blobUserName.getBinaryLength(), (char *)key, ac.cUsername, &len);

			// 解密密码
			CppSQLite3Binary blobPwd;
			blobPwd.setEncoded((unsigned char*)t.fieldValue("Password"));
			TEADecrypt((char*)blobPwd.getBinary(), blobPwd.getBinaryLength(), (char *)key, ac.cPass, &len);

			int nVer = atoi(t.fieldValue("Ver"));

			ac.bAutoLogin = atoi(t.fieldValue("IsAutoLogin"));
			ac.bHide = atoi(t.fieldValue("IsHide"));
			ac.iLoginFlag = atoi(t.fieldValue("LoginFlag"));
			ac.bRemeberPsw = atoi(t.fieldValue("IsRemeberPsw"));
			ac.bShowHint = atoi(t.fieldValue("IsShowHint"));
			ac.dwServerIP = atoi(t.fieldValue("ServerIp"));

			if(atoi(t.fieldValue("IsLast")) != 0)
			{
				strLastUserName = ac.cUsername;
			}

			ul.push_back(ac);
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("read csctaNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 保存新的好友列表db文件
void CLocalData::SaveNewFriendListDbFile(FRIEND_LIST &fl)
{
	CString strFile;
	strFile.Format("%s\\%s\\flNew.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(bDbFileExist && db.tableExists("IM_FRIEND"))
		{	// 文件存在，清空表
			db.execDML("delete from IM_FRIEND");
		}
		else
		{	// 文件不存在，创建表
			db.execDML("create table IM_FRIEND(	\
					   Ver number(3) not null,\
					   UserId	number(20) primary key,	\
					   UserName	varchar2(150) not null,\
					   NickName	varchar2(150),		\
					   Email		varchar2(80),		\
					   Birthday	varchar2(10),		\
					   CommentName	varchar2(150),		\
					   FaceUrl		varchar2(120),		\
					   FlashUrl	varchar2(120),		\
					   BindMobile	number(3),			\
					   Sex			number(3),			\
					   Age			number(3),			\
					   Province	number(3),			\
					   Country		number(3),			\
					   City		number(3),			\
					   Career		number(3),			\
					   Rule		number(3),			\
					   FamilyFlag	number(3),			\
					   Linkpop		number(3),			\
					   Address		varchar2(255),		\
					   UserSign	varchar2(255),		\
					   Description	varchar2(255),		\
					   ProfileName	varchar2(100),		\
					   Guid		varchar2(100),		\
					   GroupIndex	number(10),			\
					   ClientVer	number(20),			\
					   Ip			number(20),			\
					   LocalIp		number(20),			\
					   Port		number(10),			\
					   LocalPort	number(10),			\
					   ConnFlag	number(3),			\
					   Key			varchar2(20),		\
					   Level		number(3),			\
					   Equipment	number(3),			\
					   ClusterProperty number(3),			\
					   ClusterMember number(3),			\
					   Status number(3),			\
					   WebLocation varchar2(120),		\
					   IsRefreshed	number(3),			\
					   IsClosePanel number(3),			\
					   P2pRetry number(3),			\
					   IsBasicInfo number(3),			\
					   IsTalkZYF number(3),			\
					   HasWebInfo number(3),			\
					   IsInviting number(3),			\
					   Reserved number(20)			\
					   );");
		}

		// 插入数据
		FRIEND_LIST::iterator it = fl.begin();
		for(; it != fl.end(); ++it)
		{
			FRIEND_INFO &fi = *(*it);

			CString strInsertSql; 
			strInsertSql.Format("insert into IM_FRIEND values(%d,%d,'%s','%s','%s','%s','%s','%s','%s',\
								%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s',%d,%d,\
								%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d);", 
								VER_FRIEND_INFO,
								fi.GetId(), 
								SqlEncode(fi.GetUserName().c_str()),
								SqlEncode(fi.nickname),
								SqlEncode(fi.email),
								SqlEncode(fi.birthday),
								SqlEncode(fi.comment_name),
								SqlEncode(fi.face_url),
								SqlEncode(fi.flash_url),
								fi.bind_mobile,
								fi.sex,
								fi.age,
								fi.province,
								fi.country,
								fi.city,
								fi.career,
								fi.rule,
								fi.familyflag,
								fi.linkpop,
								SqlEncode(fi.address),
								SqlEncode(fi.user_sign),
								SqlEncode(fi.description),
								SqlEncode(fi.profile_name),
								SqlEncode(fi.GetGuid().c_str()),
								fi.group_index,
								fi.ver,
								fi.ip,
								fi.localIP,
								fi.port,
								fi.localPort,
								fi.connFlag,
								SqlEncode(fi.key),
								fi.level,
								fi.equipment,
								fi.cluster_property,
								fi.cluster_member,
								fi.status,
								SqlEncode(fi.web_location),
								fi.bRefreshed,
								fi.bClosePanel,
								fi.p2pRetry,
								fi.bBasicInfo,
								fi.isTalkZYF,
								fi.webInfo,
								fi.bInviting,
								fi.reserved);

			db.execDML(strInsertSql);
		}

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write flNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 读取新的好友列表文件
void CLocalData::ReadNewFriendListDbFile(FRIEND_LIST &fl)
{
	CString strFile;
	strFile.Format("%s\\%s\\flNew.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;
	if(!bDbFileExist)
		return;		// 文件不存在，返回

	// 读取文件数据
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		if(!db.tableExists("IM_FRIEND"))
			return;		// 表不存在，直接返回

		// 读取表数据
		CppSQLite3Table t = db.getTable("select * from IM_FRIEND;");

		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);
			FRIEND_INFO *pFriend = new FRIEND_INFO();
			pFriend->SelfEncrypt();

			int nVer = atoi(t.fieldValue("Ver"));
			pFriend->SetId(atoi(t.fieldValue("UserId")));
			pFriend->SetUserName((LPCTSTR)SqlDecode(t.fieldValue("UserName")));
			strcpy(pFriend->nickname, (LPCTSTR)SqlDecode(t.fieldValue("NickName")));
			strcpy(pFriend->email, (LPCTSTR)SqlDecode(t.fieldValue("Email")));
			strcpy(pFriend->birthday, (LPCTSTR)SqlDecode(t.fieldValue("Birthday")));
			strcpy(pFriend->comment_name, (LPCTSTR)SqlDecode(t.fieldValue("CommentName")));
			strcpy(pFriend->face_url, (LPCTSTR)SqlDecode(t.fieldValue("FaceUrl")));
			strcpy(pFriend->flash_url, (LPCTSTR)SqlDecode(t.fieldValue("FlashUrl")));
			pFriend->bind_mobile = atoi(t.fieldValue("BindMobile"));
			pFriend->sex = atoi(t.fieldValue("Sex"));
			pFriend->age = atoi(t.fieldValue("Age"));
			pFriend->province = atoi(t.fieldValue("Province"));
			pFriend->country = atoi(t.fieldValue("Country"));
			pFriend->city = atoi(t.fieldValue("City"));
			pFriend->career = atoi(t.fieldValue("Career"));
			pFriend->rule = atoi(t.fieldValue("Rule"));
			pFriend->familyflag = atoi(t.fieldValue("FamilyFlag"));
			pFriend->linkpop = atoi(t.fieldValue("Linkpop"));
			strcpy(pFriend->address, (LPCTSTR)SqlDecode(t.fieldValue("Address")));
			strcpy(pFriend->user_sign, (LPCTSTR)SqlDecode(t.fieldValue("UserSign")));
			strcpy(pFriend->description, (LPCTSTR)SqlDecode(t.fieldValue("Description")));
			strcpy(pFriend->profile_name, (LPCTSTR)SqlDecode(t.fieldValue("ProfileName")));
			pFriend->SetGuid((LPCTSTR)SqlDecode(t.fieldValue("Guid")));
			pFriend->group_index = atoi(t.fieldValue("GroupIndex"));
			pFriend->ver = atoi(t.fieldValue("ClientVer"));
			pFriend->ip = atoi(t.fieldValue("Ip"));
			pFriend->localIP = atoi(t.fieldValue("LocalIp"));
			pFriend->port = atoi(t.fieldValue("Port"));
			pFriend->localPort = atoi(t.fieldValue("LocalPort"));
			pFriend->connFlag = atoi(t.fieldValue("ConnFlag"));
			strcpy(pFriend->key, (LPCTSTR)SqlDecode(t.fieldValue("Key")));
			pFriend->level = atoi(t.fieldValue("Level"));
			pFriend->equipment = atoi(t.fieldValue("Equipment"));
			pFriend->cluster_property = atoi(t.fieldValue("ClusterProperty"));
			pFriend->cluster_member = atoi(t.fieldValue("ClusterMember"));
			pFriend->status = atoi(t.fieldValue("Status"));
			strcpy(pFriend->web_location, (LPCTSTR)SqlDecode(t.fieldValue("WebLocation")));
			pFriend->bRefreshed = atoi(t.fieldValue("IsRefreshed"));
			pFriend->bClosePanel = atoi(t.fieldValue("IsClosePanel"));
			pFriend->p2pRetry = atoi(t.fieldValue("P2pRetry"));
			pFriend->bBasicInfo = atoi(t.fieldValue("IsBasicInfo"));
			pFriend->isTalkZYF = atoi(t.fieldValue("IsTalkZYF"));
			pFriend->webInfo = atoi(t.fieldValue("HasWebInfo"));
			pFriend->bInviting = atoi(t.fieldValue("IsInviting"));
			pFriend->reserved = atoi(t.fieldValue("Reserved"));

			pFriend->ip = 0;
			pFriend->port = 0;
			pFriend->localIP = 0;
			pFriend->localPort = 0;
			pFriend->status = XTREAM_OFFLINE;
			pFriend->p2pRetry = 3;
			pFriend->connFlag = FRIEND_INFO::NONE;
			pFriend->pUserWebArticleInfo = NULL;
			pFriend->pUserWebPictureInfo = NULL;

			fl.push_back(pFriend);
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("read flNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 保存群信息
void CLocalData::SaveClusterInfo(vector<CLUSTER_INFO *> vecCluster)
{
	// 判断文件和表是否存在
	CString strFile;
	strFile.Format("%s\\%s\\clNew.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(bDbFileExist && db.tableExists("IM_FRIEND"))
		{	// 文件存在，清空表
			db.execDML("delete from IM_FRIEND");
		}
		else
		{	// 文件不存在，创建表
			db.execDML("create table IM_FRIEND(	\
					   Ver number(3) not null,\
					   FamilyId	number(20),\
					   UserId number(20),\
					   UserName	varchar2(150) not null,\
					   NickName	varchar2(150),\
					   Email varchar2(80),\
					   Birthday	varchar2(10),\
					   CommentName varchar2(150),\
					   FaceUrl varchar2(120),\
					   FlashUrl	varchar2(120),\
					   BindMobile number(3),\
					   Sex number(3),\
					   Age number(3),\
					   Province	number(3),\
					   Country number(3),\
					   City number(3),\
					   Career number(3),\
					   Rule number(3),\
					   FamilyFlag number(3),\
					   Linkpop number(3),\
					   Address varchar2(255),\
					   UserSign	varchar2(255),\
					   Description varchar2(255),\
					   ProfileName varchar2(100),\
					   Guid varchar2(100),\
					   GroupIndex number(10),\
					   ClientVer number(20),\
					   Ip number(20),\
					   LocalIp number(20),\
					   Port number(10),\
					   LocalPort number(10),\
					   ConnFlag number(3),\
					   Key varchar2(20),\
					   Level number(3),\
					   Equipment number(3),\
					   ClusterProperty number(3),\
					   ClusterMember number(3),\
					   Status number(3),\
					   WebLocation varchar2(120),\
					   IsRefreshed number(3),\
					   IsClosePanel number(3),\
					   P2pRetry number(3),\
					   IsBasicInfo number(3),\
					   IsTalkZYF number(3),\
					   HasWebInfo number(3),\
					   IsInviting number(3),\
					   Reserved number(20),\
					   constraint pk primary key (FamilyId,UserId));");
		}

		if(bDbFileExist &&  db.tableExists("IM_CLUSTER"))
		{	// 文件存在，清空表
			db.execDML("delete from IM_CLUSTER");
		}
		else
		{	// 文件不存在，创建表
			db.execDML("create table IM_CLUSTER(\
					   Ver number(3) not null,\
					   FamilyId	number(20) primary key,\
					   FamilyProp number(3),\
					   FamilyType number(3),\
					   FamilyClass number(10),\
					   CreatorID number(20),\
					   FamilyFaceID	number(3),\
					   FamilyFaceUrl varchar2(120),\
					   JoinFlag	number(3),\
					   ViewFlag	number(3),\
					   FamilyName varchar2(150),\
					   FamilyDesc varchar2(800),\
					   FamilyDomain	varchar2(50),\
					   NumberLimit number(10),\
					   FamilySign varchar2(300),\
					   FamilyNotice varchar2(500),\
					   IsAutoShowMagic number(3),\
					   IsRefused number(3),\
					   RefreshFlag number(3),\
					   IsMemberInit number(3),\
					   IsBasicInfoV number(3),\
					   Radio number(3),\
					   TickGet number(20),\
					   ClusterPageShow number(20));");
		}

		for(int i = 0; i < (int)vecCluster.size(); ++i)
		{
			CLUSTER_INFO *pCluster = vecCluster[i];
			CString strInsertSql; 
			strInsertSql.Format("insert into IM_CLUSTER values(%d,%d,%d,%d,%d,%d,%d,\
'%s',\
%d,%d,\
'%s','%s','%s',\
%d,\
'%s','%s',\
%d,%d,%d,%d,%d,%d,%d,%d);", 
								VER_CLUSTER_INFO,
								pCluster->nFamilyID,
								pCluster->nFamilyProp,
								pCluster->nFamilyType,
								pCluster->nFamilyClass,
								pCluster->nCreatorID,
								pCluster->nFamilyFaceID,
								SqlEncode(pCluster->szFamilyFaceUrl),
								pCluster->nJoinFlag,
								pCluster->nViewFlag,
								SqlEncode(pCluster->szFamilyName),
								SqlEncode(pCluster->szFamilyDesc),
								SqlEncode(pCluster->szFamilyDomain),
								pCluster->nNumberLimit,
								SqlEncode(pCluster->szFamilySign),
								SqlEncode(pCluster->szFamilyNotice),
								pCluster->bAutoShowMagic,
								pCluster->bRefused,
								pCluster->bRefreshFlag,
								pCluster->bMemberInit,
								pCluster->bBasicInfo,
								pCluster->radio,
								pCluster->dwTickGet,
								pCluster->dwClusterPageShow);

			db.execDML(strInsertSql);

			// 插入好友
			for(int j = 0; j < (int)pCluster->members.size(); ++j)
			{
				FRIEND_INFO &fi = pCluster->members[j];
				strInsertSql.Format("insert into IM_FRIEND values(%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s',\
									%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s',%d,%d,\
									%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d);", 
									VER_FRIEND_INFO,
									pCluster->nFamilyID,
									fi.GetId(), 
									SqlEncode(fi.GetUserName().c_str()),
									SqlEncode(fi.nickname),
									SqlEncode(fi.email),
									SqlEncode(fi.birthday),
									SqlEncode(fi.comment_name),
									SqlEncode(fi.face_url),
									SqlEncode(fi.flash_url),
									fi.bind_mobile,
									fi.sex,
									fi.age,
									fi.province,
									fi.country,
									fi.city,
									fi.career,
									fi.rule,
									fi.familyflag,
									fi.linkpop,
									SqlEncode(fi.address),
									SqlEncode(fi.user_sign),
									SqlEncode(fi.description),
									SqlEncode(fi.profile_name),
									SqlEncode(fi.GetGuid().c_str()),
									fi.group_index,
									fi.ver,
									fi.ip,
									fi.localIP,
									fi.port,
									fi.localPort,
									fi.connFlag,
									SqlEncode(fi.key),
									fi.level,
									fi.equipment,
									fi.cluster_property,
									fi.cluster_member,
									fi.status,
									SqlEncode(fi.web_location),
									fi.bRefreshed,
									fi.bClosePanel,
									fi.p2pRetry,
									fi.bBasicInfo,
									fi.isTalkZYF,
									fi.webInfo,
									fi.bInviting,
									fi.reserved);

				db.execDML(strInsertSql);
			}			
		}

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write clNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 读取新的群列表文件
void CLocalData::ReadNewClusterList()
{
	CString strFile;
	strFile.Format("%s\\%s\\clNew.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;
	if(!bDbFileExist)
		return;		// 文件不存在，返回

	// 读取文件数据
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		if(!db.tableExists("IM_FRIEND")
			|| !db.tableExists("IM_CLUSTER"))
			return;		// 表不存在，直接返回

		// 读取表数据
		CppSQLite3Table t = db.getTable("select * from IM_CLUSTER;");
		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);
			CLUSTER_INFO *pCluster = new CLUSTER_INFO();

			int nVer = atoi(t.fieldValue("Ver"));
			pCluster->nFamilyID = atoi(t.fieldValue("FamilyId"));
			pCluster->nFamilyProp = atoi(t.fieldValue("FamilyProp"));
			pCluster->nFamilyType = atoi(t.fieldValue("FamilyType"));
			pCluster->nFamilyClass = atoi(t.fieldValue("FamilyClass"));
			pCluster->nCreatorID = atoi(t.fieldValue("FamilyClass"));
			pCluster->nFamilyFaceID = atoi(t.fieldValue("FamilyFaceID"));
			strcpy(pCluster->szFamilyFaceUrl, (LPCTSTR)SqlDecode(t.fieldValue("FamilyFaceUrl")));
			pCluster->nJoinFlag = atoi(t.fieldValue("JoinFlag"));
			pCluster->nViewFlag = atoi(t.fieldValue("ViewFlag"));
			strcpy(pCluster->szFamilyName, (LPCTSTR)SqlDecode(t.fieldValue("FamilyName")));
			strcpy(pCluster->szFamilyDesc, (LPCTSTR)SqlDecode(t.fieldValue("FamilyDesc")));
			strcpy(pCluster->szFamilyDomain, (LPCTSTR)SqlDecode(t.fieldValue("FamilyDomain")));
			pCluster->nNumberLimit = atoi(t.fieldValue("NumberLimit"));
			strcpy(pCluster->szFamilySign, (LPCTSTR)SqlDecode(t.fieldValue("FamilySign")));
			strcpy(pCluster->szFamilyNotice, (LPCTSTR)SqlDecode(t.fieldValue("FamilyNotice")));
			pCluster->bAutoShowMagic = atoi(t.fieldValue("IsAutoShowMagic"));
			pCluster->bRefused = atoi(t.fieldValue("IsRefused"));
			pCluster->bRefreshFlag = atoi(t.fieldValue("RefreshFlag"));
			pCluster->bMemberInit = atoi(t.fieldValue("IsMemberInit"));
			pCluster->bBasicInfo = atoi(t.fieldValue("IsBasicInfoV"));
			pCluster->radio = atoi(t.fieldValue("Radio"));
			pCluster->dwTickGet = atoi(t.fieldValue("TickGet"));
			pCluster->dwClusterPageShow = atoi(t.fieldValue("ClusterPageShow"));

			g_vecCluster.push_back(pCluster);
		}

		t = db.getTable("select * from IM_FRIEND;");
		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);

			FRIEND_INFO fi;
			FRIEND_INFO *pFriend = &fi;
			pFriend->SelfEncrypt();

			int nVer = atoi(t.fieldValue("Ver"));
			pFriend->SetId(atoi(t.fieldValue("UserId")));
			pFriend->SetUserName((LPCTSTR)SqlDecode(t.fieldValue("UserName")));
			strcpy(pFriend->nickname, (LPCTSTR)SqlDecode(t.fieldValue("NickName")));
			strcpy(pFriend->email, (LPCTSTR)SqlDecode(t.fieldValue("Email")));
			strcpy(pFriend->birthday, (LPCTSTR)SqlDecode(t.fieldValue("Birthday")));
			strcpy(pFriend->comment_name, (LPCTSTR)SqlDecode(t.fieldValue("CommentName")));
			strcpy(pFriend->face_url, (LPCTSTR)SqlDecode(t.fieldValue("FaceUrl")));
			strcpy(pFriend->flash_url, (LPCTSTR)SqlDecode(t.fieldValue("FlashUrl")));
			pFriend->bind_mobile = atoi(t.fieldValue("BindMobile"));
			pFriend->sex = atoi(t.fieldValue("Sex"));
			pFriend->age = atoi(t.fieldValue("Age"));
			pFriend->province = atoi(t.fieldValue("Province"));
			pFriend->country = atoi(t.fieldValue("Country"));
			pFriend->city = atoi(t.fieldValue("City"));
			pFriend->career = atoi(t.fieldValue("Career"));
			pFriend->rule = atoi(t.fieldValue("Rule"));
			pFriend->familyflag = atoi(t.fieldValue("FamilyFlag"));
			pFriend->linkpop = atoi(t.fieldValue("Linkpop"));
			strcpy(pFriend->address, (LPCTSTR)SqlDecode(t.fieldValue("Address")));
			strcpy(pFriend->user_sign, (LPCTSTR)SqlDecode(t.fieldValue("UserSign")));
			strcpy(pFriend->description, (LPCTSTR)SqlDecode(t.fieldValue("Description")));
			strcpy(pFriend->profile_name, (LPCTSTR)SqlDecode(t.fieldValue("ProfileName")));
			pFriend->SetGuid((LPCTSTR)SqlDecode(t.fieldValue("Guid")));
			pFriend->group_index = atoi(t.fieldValue("GroupIndex"));
			pFriend->ver = atoi(t.fieldValue("ClientVer"));
			pFriend->ip = atoi(t.fieldValue("Ip"));
			pFriend->localIP = atoi(t.fieldValue("LocalIp"));
			pFriend->port = atoi(t.fieldValue("Port"));
			pFriend->localPort = atoi(t.fieldValue("LocalPort"));
			pFriend->connFlag = atoi(t.fieldValue("ConnFlag"));
			strcpy(pFriend->key, (LPCTSTR)SqlDecode(t.fieldValue("Key")));
			pFriend->level = atoi(t.fieldValue("Level"));
			pFriend->equipment = atoi(t.fieldValue("Equipment"));
			pFriend->cluster_property = atoi(t.fieldValue("ClusterProperty"));
			pFriend->cluster_member = atoi(t.fieldValue("ClusterMember"));
			pFriend->status = atoi(t.fieldValue("Status"));
			strcpy(pFriend->web_location, (LPCTSTR)SqlDecode(t.fieldValue("WebLocation")));
			pFriend->bRefreshed = atoi(t.fieldValue("IsRefreshed"));
			pFriend->bClosePanel = atoi(t.fieldValue("IsClosePanel"));
			pFriend->p2pRetry = atoi(t.fieldValue("P2pRetry"));
			pFriend->bBasicInfo = atoi(t.fieldValue("IsBasicInfo"));
			pFriend->isTalkZYF = atoi(t.fieldValue("IsTalkZYF"));
			pFriend->webInfo = atoi(t.fieldValue("HasWebInfo"));
			pFriend->bInviting = atoi(t.fieldValue("IsInviting"));
			pFriend->reserved = atoi(t.fieldValue("Reserved"));

			pFriend->status = XTREAM_OFFLINE;

			uint32 nFamilyID = atoi(t.fieldValue("FamilyId"));
			
			CLUSTER_INFO *pCluster = NULL;
			for(int i = 0; i < (int)g_vecCluster.size(); ++i)
			{
				if(nFamilyID == g_vecCluster[i]->nFamilyID)
				{
					pCluster = g_vecCluster[i];
					break;
				}
			}
			if(pCluster != NULL)
			{
				pCluster->members.push_back(fi);
			}
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("read clNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 写日志操作
void CLocalData::WriteLog(LOG_LEVEL level, const char *pLogText, ...)
{
	if(m_pLog == NULL)
		return;

	char* p = (char*)pLogText;

	while(*p)
	{
		if(*p == '%')
		{
			*p = '&';
		}
		p++;
	}

	va_list args;
	char logText[LOG_LINE_MAX] = {0};
	va_start(args, pLogText);
	vsnprintf_s(logText, LOG_LINE_MAX - 1, pLogText, args);
	CLog::LOG_LEVEL ll = (CLog::LOG_LEVEL)((int)level);
	m_pLog->WriteLog(logText, ll);
}

void CLocalData::WriteLog(string logText, LOG_LEVEL level /*= LL_ERROR*/)
{
	if(m_pLog == NULL)
		return;
	CLog::LOG_LEVEL ll = (CLog::LOG_LEVEL)((int)level);
	m_pLog->WriteLog(logText, ll);
}

//imserver的base64编码
std::string CLocalData::Base64EncodeServer(const char* szInput, int inByte)
{
	static char Base64Str[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int		L,I,J;
	byte	B1,B2,B3,B4;
	std::string strTemp,strResult;

	L = inByte;
	J = 0;

	for ( I=0; I<L; I++)
	{
		strTemp=strTemp+szInput[I];

		if ( J<2 )
		{
			J++;
		}
		else
		{
			J=0;
			B1=((byte)strTemp[0])>>2;
			B2=((((byte)strTemp[0])&3)<<4)+(((byte)strTemp[1])>>4);
			B3=((byte)strTemp[2]>>6) + (((byte)strTemp[1]&0x0F)<<2);
			B4=(byte)strTemp[2]&0x3F;

			strResult+=Base64Str[B1];
			strResult+=Base64Str[B2];
			strResult+=Base64Str[B3];
			strResult+=Base64Str[B4];

			strTemp="";
		}
	}

	if ( J==1 )
	{
		B1=(byte)strTemp[0]>>2;
		B2=((byte)strTemp[0]&3)<<4;

		strResult+=Base64Str[B1];
		strResult+=Base64Str[B2];
		strResult+="==";
	}

	if ( J==2 )
	{
		B1=(byte)strTemp[0]>>2;
		B2=(((byte)strTemp[0]&3)<<4)+((byte)strTemp[1]>>4);
		B3=((byte)strTemp[1]&0x0F)<<2;

		strResult+=Base64Str[B1];
		strResult+=Base64Str[B2];
		strResult+=Base64Str[B3];
		strResult+="=";
	}

	return strResult;
}

//imserver的base64编码
std::string CLocalData::Base64EncodeServer( const char* szInput)
{
	//string strInput(szInput);
	return Base64EncodeServer(szInput, 16);
}

// 客服工作台保存客户信息到db文件
void CLocalData::SaveCurrentBuyerDbFile(FRIEND_INFO* pFriend)
{
	CString strFile;
	strFile.Format("%s\\%s\\currentBuyer.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(!db.tableExists("IM_CURRENT_BUYER"))
		{	// 文件不存在，创建表
			db.execDML("create table IM_CURRENT_BUYER(	\
					   Ver number(3) not null,\
					   UserId number(20) primary key,	\
					   UserName	varchar2(150),      \
					   NickName	varchar2(150),		\
					   OrederSeq varchar2(150)      \
					   );");
		}
		

		CppSQLite3Table t = db.getTable("select * from IM_CURRENT_BUYER;");

		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);
			int nVer = atoi(t.fieldValue("Ver"));
			int n1 = pFriend->GetId();
			int n2 = atoi(t.fieldValue("UserId"));
			if(pFriend->GetId() == atoi(t.fieldValue("UserId")) )
			{
				// 插入数据
				CString strInsertSql; 
				strInsertSql.Format("update IM_CURRENT_BUYER set UserName = '%s', NickName = '%s', OrederSeq = '%s' where UserId = %d;", 
					SqlEncode(pFriend->GetUserName().c_str()),
					SqlEncode(pFriend->nickname),
					g_mapFriendIdToOrder[pFriend->GetId()],
					pFriend->GetId()
				);

				db.execDML(strInsertSql);

				db.execDML("commit;");

				return;
			}
		}

		// 插入数据
		int nId = pFriend->GetId();
		CString strInsertSql; 
		strInsertSql.Format("insert into IM_CURRENT_BUYER values(%d,%d,'%s','%s','%s');", 
							VER_CURRENT_BUYER_INFO,
							nId, 
							SqlEncode(pFriend->GetUserName().c_str()),
							SqlEncode(pFriend->nickname),
							g_mapFriendIdToOrder[pFriend->GetId()]
							);

		db.execDML(strInsertSql);

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write recentNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

// 客服工作台保存客户信息到db文件
void CLocalData::DeleteCurrentBuyerDbFile(int nId)
{
	CString strFile;
	strFile.Format("%s\\%s\\currentBuyer.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(db.tableExists("IM_CURRENT_BUYER"))
		{	
			// 插入数据
			CString strInsertSql; 
			strInsertSql.Format("delete from IM_CURRENT_BUYER where UserId = %d;", 
				nId
			);

			db.execDML(strInsertSql);
		}



		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write recentNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

void CLocalData::GetCurrentBuyer(vector<FRIEND_INFO*>& vecFriend)
{
	CString strFile;
	strFile.Format("%s\\%s\\currentBuyer.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(db.tableExists("IM_CURRENT_BUYER"))
		{	

			CppSQLite3Table t = db.getTable("select * from IM_CURRENT_BUYER;");

			for (int row = 0; row < t.numRows(); row++)
			{
				t.setRow(row);
				FRIEND_INFO *pFriend = new FRIEND_INFO();
				pFriend->SelfEncrypt();

				int nVer = atoi(t.fieldValue("Ver"));
				pFriend->SetId(atoi(t.fieldValue("UserId")));
				pFriend->SetUserName((LPCTSTR)SqlDecode(t.fieldValue("UserName")));
				strcpy(pFriend->nickname, (LPCTSTR)SqlDecode(t.fieldValue("NickName")));
				g_mapFriendIdToOrder[pFriend->GetId()] = t.fieldValue("OrederSeq");
				vecFriend.push_back(pFriend);
			}
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write recentNew.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

#ifdef _BUSINESS_VESION
void CLocalData::SetOrderSubscibe(OrderSubscibe& orderSubscibeInfo)
{
	m_orderSubscibeInfo = orderSubscibeInfo;
}

OrderSubscibe CLocalData::GetOrderSubscibe()
{
	return m_orderSubscibeInfo;
}

std::vector<OrderMsg*> CLocalData::GetOrderMsgTrade()
{
	return m_vecOrderMsgTradeInfo;
}

std::vector<OrderMsg*> CLocalData::GetOrderMsgReturn()
{
	return m_vecOrderMsgReturnInfo;
}

void CLocalData::SaveOrderMsg(int nType, const string &orderData, const string &orderUuid)
{
	CString strFile;
	strFile.Format("%s\\%s\\orderMsg.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(!db.tableExists("IM_ORDER_MSG"))
		{	// 文件不存在，创建表
			db.execDML("create table IM_ORDER_MSG(	\
					   Ver number(3) not null,\
					   OrderType number(3),    \
					   OrderIsRead number(3),   \
					   orderUuid varchar2(100),  \
					   OrderData blob	\
					   );");
		}

		// 插入数据
		CppSQLite3Binary  blob;  
		blob.setBinary((const unsigned char*)orderData.c_str(), orderData.length());  
		CppSQLite3Buffer bufSQL;  
	
		CppSQLite3Buffer strInsertSql; 
		strInsertSql.format("insert into IM_ORDER_MSG values(%d,%d,%d,'%s',%Q);", 
			VER_ORDER_MSG_INFO,
			nType, 
			0,
			orderUuid.c_str(),
			blob.getEncoded()
		);

		db.execDML(strInsertSql);

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write orderMsg.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

void CLocalData::ReadOrderMsg()
{
	CString strFile;
	strFile.Format("%s\\%s\\orderMsg.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(db.tableExists("IM_ORDER_MSG"))
		{	

			CppSQLite3Table t = db.getTable("select * from IM_ORDER_MSG;");

			for (int row = 0; row < t.numRows(); row++)
			{
				t.setRow(row);
				CppSQLite3Binary  blob;
				int nVer = atoi(t.fieldValue("Ver"));
				int nType = atoi(t.fieldValue("OrderType"));
				int nIsRead = atoi(t.fieldValue("OrderIsRead"));
				string orderUuid = (LPCTSTR)SqlDecode(t.fieldValue("orderUuid"));
				blob.setEncoded((unsigned char*)t.fieldValue("OrderData")); 
				OrderMsg* pOrderMsg = new OrderMsg;

				//CString strOrderData = (LPCTSTR)SqlDecode(t.fieldValue("OrderData"));
				const unsigned char* pbin = blob.getBinary();
				const char* p = (const char*)blob.getBinary();
				pOrderMsg->orderUuid = orderUuid;
				pOrderMsg->orderData.assign((const char*)blob.getBinary(), blob.getBinaryLength());
				if(nType == 0)
					SetNewOrderData(pOrderMsg, pOrderMsg->orderData);
				else if(nType == 1)
					SetPayOrderData(pOrderMsg, pOrderMsg->orderData);
				else if(nType == 2)
					SetGoodsCommentOrderData(pOrderMsg, pOrderMsg->orderData);
				else if(nType == 3)
					SetReturnOrderData(pOrderMsg, pOrderMsg->orderData);

				pOrderMsg->orderType = nType;
				if(nIsRead != 0)
					pOrderMsg->bIsRead = true;
				if(!pOrderMsg->bIsRead && !CMainFrame::GetInstance()->GetLabelOrderMsgUnRead())
				{
					CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(true);
				}
			}
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write orderMsg.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

void CLocalData::SetOrderMsgRead(const string &orderUuid)
{
	CString strFile;
	strFile.Format("%s\\%s\\orderMsg.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(!db.tableExists("IM_ORDER_MSG"))
		{	// 文件不存在，创建表
			return;
		}
		// 插入数据
		CString strInsertSql; 
		strInsertSql.Format("update IM_ORDER_MSG set OrderIsRead = %d where orderUuid = '%s';", 
			1,
			orderUuid.c_str()
			);


		db.execDML(strInsertSql);

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write orderMsg.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

void CLocalData::AddNewOrder(string orderData, string orderUuid)
{
	OrderMsg* pOrderMsg = new OrderMsg;
	pOrderMsg->orderData = orderData;
	pOrderMsg->orderUuid = orderUuid;
	pOrderMsg->orderType = 0;
	SetNewOrderData(pOrderMsg, orderData);
	
	CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(true);
	if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL && IsWindowVisible(CDuiMsgDlg::GetInstance()->GetHWND()))
	{
		CDuiMsgDlg::GetInstance()->AddOrderMsg();
	}
	::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_OPEN_ORDER_MSG_DLG, (WPARAM)pOrderMsg, 0);
}

void CLocalData::SetNewOrderData(OrderMsg* pOrderMsg, string orderData)
{
	CString str = orderData.c_str();
	int nLength = orderData.length();
	NewOrder newOrder;
	if(newOrder.ParseFromArray(orderData.c_str(), orderData.length()))
	{
		string ogno = newOrder.ogno();
		string ogseq = newOrder.ogseq();	// 订单流水号
		string memguid = newOrder.memguid(); // 会员mem_guid
		double totalPay = newOrder.totalpay(); // 支付金额;
		string buyerName = newOrder.buyername();//用户名
		string memberName = newOrder.membername(); // 收件人
		string membercellphone = newOrder.membercellphone(); // 收件人手机号
		string insertdate = newOrder.insertdate(); // 下单时间
		if(insertdate.empty())
		{	// 给个默认时间，不让程序弹出错误提示窗
			insertdate = "2016-1-1 0:0:0";
		}
		string payType = newOrder.paytype(); // 支付类型
		string packNo = newOrder.paytype();							// 包裹号
		uint32 packState = newOrder.packstate();						// 包裹状态
		string packStateStr = newOrder.packstatestr();						// 包裹状态描述
		uint32 type = newOrder.type();								// 包裹类型 1-自营 2-商城
		string packQueryNo = newOrder.packqueryno();						// 物流接口查询包裹号	
		string merchantId = newOrder.merchantid();						// 商家Id
		double freight = newOrder.freight();							// 包裹运费金额 

		pOrderMsg->bIsRead = false;
		pOrderMsg->buyerName = buyerName;
		pOrderMsg->orderNum = ogno;
		pOrderMsg->orderPackNum = packQueryNo;
		pOrderMsg->orderTime = insertdate;
		//pOrderMsg->orderPrice = totalPay - freight;
		pOrderMsg->orderFreight = freight;

		int nYear, nMonth, nDate, nHour, nMin, nSec;
		sscanf(pOrderMsg->orderTime.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		CTime t(nYear, nMonth, nDate, nHour, nMin, nSec);
		pOrderMsg->dwOrderTime = t.GetTime();
		
		for(int nGoods = 0; nGoods < newOrder.goodslist().size(); nGoods++)
		{
			OrderGoodsInfo orderGoodsInfo = newOrder.goodslist().Get(nGoods);
			string itno = orderGoodsInfo.itno();	// 自营商品itno
			string productName = orderGoodsInfo.productname(); // 商品名称
			string sourceUrl = orderGoodsInfo.sourceurl();	// 卖场地址
			string picUrl = orderGoodsInfo.picurl();	// 图片地址
			uint32 qty = orderGoodsInfo.qty();	// 商品数量
			uint32 isMall = orderGoodsInfo.ismall();	// 是否商城商品，1-商城，0-自营
			string skuId = orderGoodsInfo.skuid();	// 商城商品sku_id
			double price = orderGoodsInfo.price(); // 商品原价
			double subTotalPrice = orderGoodsInfo.subtotalprice(); // 商品小计
			double coupons = orderGoodsInfo.coupons();	// 优惠券分摊金额
			double voucher = orderGoodsInfo.voucher(); // 抵用券分摊金额
			double shoppingCash = orderGoodsInfo.shoppingcash(); // 购物金分摊金额
			double shoppingCard = orderGoodsInfo.shoppingcard(); // 购物卡分摊金额
			double pointsPrice = orderGoodsInfo.pointsprice(); // 积分分摊金额
			double promote = orderGoodsInfo.promote(); // 行销活动分摊金额
			double shippingTax = orderGoodsInfo.shippingtax(); // 跨境商品行邮税
			string freightNumber = orderGoodsInfo.freightnumber(); // 自营运费序号
			string kind = orderGoodsInfo.kind();	// 自营商品类型
			string color = orderGoodsInfo.color();	 // 颜色
			string size = orderGoodsInfo.size();	 // 大小

			OrderItem* pOrderItemInfo = new OrderItem;
			pOrderItemInfo->itemCount = qty;
			pOrderItemInfo->itemName = productName;
			pOrderItemInfo->itemPicUrl = picUrl;
			pOrderItemInfo->itemPrice = subTotalPrice - coupons - voucher - /*shoppingCash - shoppingCard - pointsPrice -*/ promote;
			pOrderMsg->orderPrice += pOrderItemInfo->itemPrice;
			pOrderItemInfo->itemStandard = color + " " + size;
			if(color == "" && size == "")
				pOrderItemInfo->itemStandard = "--";
			pOrderMsg->vecOrderItem.push_back(pOrderItemInfo);
		}

		m_vecOrderMsgTradeInfo.push_back(pOrderMsg);

		std::sort(m_vecOrderMsgTradeInfo.begin(),m_vecOrderMsgTradeInfo.end(),ItemSortByTime);
	}
}

void CLocalData::AddPayOrder(string orderData, string orderUuid)
{
	OrderMsg* pOrderMsg = new OrderMsg;
	pOrderMsg->orderData = orderData;
	pOrderMsg->orderUuid = orderUuid;
	pOrderMsg->orderType = 1;
	SetPayOrderData(pOrderMsg, orderData);
	
	CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(true);
	if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL && IsWindowVisible(CDuiMsgDlg::GetInstance()->GetHWND()))
	{
		CDuiMsgDlg::GetInstance()->AddOrderMsg();
	}
	::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_OPEN_ORDER_MSG_DLG, (WPARAM)pOrderMsg, 0);
}

void CLocalData::SetPayOrderData(OrderMsg* pOrderMsg, string orderData)
{
	CString str = orderData.c_str();
	int nLength = orderData.length();
	PayOrder payOrder;
	if(payOrder.ParseFromArray(orderData.c_str(), orderData.length()))
	{
		string ogno = payOrder.ogno();
		string ogseq = payOrder.ogseq();	// 订单流水号
		string memguid = payOrder.memguid(); // 会员mem_guid
		double totalPay = payOrder.totalpay(); // 支付金额;
		string buyerName = payOrder.buyername();//用户名
		string memberName = payOrder.membername(); // 收件人
		string membercellphone = payOrder.membercellphone(); // 收件人手机号
		string insertdate = payOrder.insertdate(); // 下单时间
		string paydate = payOrder.paydate();
		if(paydate.empty())
		{	// 给个默认时间，不让程序弹出错误提示窗
			paydate = "2016-1-1 0:0:0";
		}
		string payType = payOrder.paytype(); // 支付类型
		string packNo = payOrder.paytype();							// 包裹号
		uint32 packState = payOrder.packstate();						// 包裹状态
		string packStateStr = payOrder.packstatestr();						// 包裹状态描述
		uint32 type = payOrder.type();								// 包裹类型 1-自营 2-商城
		string packQueryNo = payOrder.packqueryno();						// 物流接口查询包裹号	
		string merchantId = payOrder.merchantid();						// 商家Id
		double freight = payOrder.freight();							// 包裹运费金额 

		pOrderMsg->bIsRead = false;
		pOrderMsg->buyerName = buyerName;
		pOrderMsg->orderNum = ogno;
		pOrderMsg->orderPackNum = packQueryNo;
		pOrderMsg->orderTime = paydate;
		//pOrderMsg->orderPrice = totalPay - freight;
		pOrderMsg->orderFreight = freight;

		int nYear, nMonth, nDate, nHour, nMin, nSec;
		sscanf(pOrderMsg->orderTime.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		CTime t(nYear, nMonth, nDate, nHour, nMin, nSec);
		pOrderMsg->dwOrderTime = t.GetTime();

		for(int nGoods = 0; nGoods < payOrder.goodslist().size(); nGoods++)
		{
			OrderGoodsInfo orderGoodsInfo = payOrder.goodslist().Get(nGoods);
			string itno = orderGoodsInfo.itno();	// 自营商品itno
			string productName = orderGoodsInfo.productname(); // 商品名称
			string sourceUrl = orderGoodsInfo.sourceurl();	// 卖场地址
			string picUrl = orderGoodsInfo.picurl();	// 图片地址
			uint32 qty = orderGoodsInfo.qty();	// 商品数量
			uint32 isMall = orderGoodsInfo.ismall();	// 是否商城商品，1-商城，0-自营
			string skuId = orderGoodsInfo.skuid();	// 商城商品sku_id
			double price = orderGoodsInfo.price(); // 商品原价
			double subTotalPrice = orderGoodsInfo.subtotalprice(); // 商品小计
			double coupons = orderGoodsInfo.coupons();	// 优惠券分摊金额
			double voucher = orderGoodsInfo.voucher(); // 抵用券分摊金额
			double shoppingCash = orderGoodsInfo.shoppingcash(); // 购物金分摊金额
			double shoppingCard = orderGoodsInfo.shoppingcard(); // 购物卡分摊金额
			double pointsPrice = orderGoodsInfo.pointsprice(); // 积分分摊金额
			double promote = orderGoodsInfo.promote(); // 行销活动分摊金额
			double shippingTax = orderGoodsInfo.shippingtax(); // 跨境商品行邮税
			string freightNumber = orderGoodsInfo.freightnumber(); // 自营运费序号
			string kind = orderGoodsInfo.kind();	// 自营商品类型
			string color = orderGoodsInfo.color();	 // 颜色
			string size = orderGoodsInfo.size();	 // 大小

			OrderItem* pOrderItemInfo = new OrderItem;
			pOrderItemInfo->itemCount = qty;
			pOrderItemInfo->itemName = productName;
			pOrderItemInfo->itemPicUrl = picUrl;
			pOrderItemInfo->itemPrice = subTotalPrice - coupons - voucher - /*shoppingCash - shoppingCard - pointsPrice -*/ promote;
			pOrderMsg->orderPrice += pOrderItemInfo->itemPrice;
			pOrderItemInfo->itemStandard = color + " " + size;
			if(color == "" && size == "")
				pOrderItemInfo->itemStandard = "--";
			pOrderMsg->vecOrderItem.push_back(pOrderItemInfo);
		}

		m_vecOrderMsgTradeInfo.push_back(pOrderMsg);

		std::sort(m_vecOrderMsgTradeInfo.begin(),m_vecOrderMsgTradeInfo.end(),ItemSortByTime);
	}
}
void CLocalData::AddGoodsCommentOrder(string orderData, string orderUuid)
{
	OrderMsg* pOrderMsg = new OrderMsg;
	pOrderMsg->orderData = orderData;
	pOrderMsg->orderUuid = orderUuid;
	pOrderMsg->orderType = 2;
	SetGoodsCommentOrderData(pOrderMsg, orderData);
	
	CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(true);
	if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL && IsWindowVisible(CDuiMsgDlg::GetInstance()->GetHWND()))
	{
		CDuiMsgDlg::GetInstance()->AddOrderMsg();
	}
	::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_OPEN_ORDER_MSG_DLG, (WPARAM)pOrderMsg, 0);
}

void CLocalData::SetGoodsCommentOrderData(OrderMsg* pOrderMsg, string orderData)
{
	CString str = orderData.c_str();
	int nLength = orderData.length();
	GoodsComment goodsComment;
	if(goodsComment.ParseFromArray(orderData.c_str(), orderData.length()))
	{
		string ogno = goodsComment.ogno();
		string ogseq = goodsComment.ogseq();	// 订单流水号
		string memguid = goodsComment.memguid(); // 会员mem_guid
		string memberName = goodsComment.membername(); // 收件人
		string membercellphone = goodsComment.membercellphone(); // 收件人手机号
		string insertdate = goodsComment.insertdate(); // 下单时间
		if(insertdate.empty())
		{	// 给个默认时间，不让程序弹出错误提示窗
			insertdate = "2016-1-1 0:0:0";
		}
		string commentdate = goodsComment.commentdate();
		if(commentdate.empty())
		{	// 给个默认时间，不让程序弹出错误提示窗
			commentdate = "2016-1-1 0:0:0";
		}
		string payType = goodsComment.paytype(); // 支付类型
		string merchantId = goodsComment.merchantid();						// 商家Id
		string buyername = goodsComment.bugername();						   // 买家姓名
		double evaluatelevel = goodsComment.evaluatelevel();					   // 评价等级
		string evaluatecontext = goodsComment.evaluatecontext();				   // 评价内容
		//const OrderGoodsInfo &goods = goodsComment.goods();					   // 评价的商品

		pOrderMsg->bIsRead = false;
		pOrderMsg->buyerName = buyername;
		pOrderMsg->orderNum = ogno;
		pOrderMsg->orderTime = commentdate;
		pOrderMsg->orderContent = evaluatecontext;

		int nYear, nMonth, nDate, nHour, nMin, nSec;
		sscanf(pOrderMsg->orderTime.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		CTime t(nYear, nMonth, nDate, nHour, nMin, nSec);
		pOrderMsg->dwOrderTime = t.GetTime();

		//for(int nGoods = 0; nGoods < payOrder.goodslist().size(); nGoods++)
		//{
			const OrderGoodsInfo &orderGoodsInfo = goodsComment.goods();
			string itno = orderGoodsInfo.itno();	// 自营商品itno
			string productName = orderGoodsInfo.productname(); // 商品名称
			string sourceUrl = orderGoodsInfo.sourceurl();	// 卖场地址
			string picUrl = orderGoodsInfo.picurl();	// 图片地址
			uint32 qty = orderGoodsInfo.qty();	// 商品数量
			uint32 isMall = orderGoodsInfo.ismall();	// 是否商城商品，1-商城，0-自营
			string skuId = orderGoodsInfo.skuid();	// 商城商品sku_id
			double price = orderGoodsInfo.price(); // 商品原价
			double subTotalPrice = orderGoodsInfo.subtotalprice(); // 商品小计
			double coupons = orderGoodsInfo.coupons();	// 优惠券分摊金额
			double voucher = orderGoodsInfo.voucher(); // 抵用券分摊金额
			double shoppingCash = orderGoodsInfo.shoppingcash(); // 购物金分摊金额
			double shoppingCard = orderGoodsInfo.shoppingcard(); // 购物卡分摊金额
			double pointsPrice = orderGoodsInfo.pointsprice(); // 积分分摊金额
			double promote = orderGoodsInfo.promote(); // 行销活动分摊金额
			double shippingTax = orderGoodsInfo.shippingtax(); // 跨境商品行邮税
			string freightNumber = orderGoodsInfo.freightnumber(); // 自营运费序号
			string kind = orderGoodsInfo.kind();	// 自营商品类型
			string color = orderGoodsInfo.color();	 // 颜色
			string size = orderGoodsInfo.size();	 // 大小

			OrderItem* pOrderItemInfo = new OrderItem;
			pOrderItemInfo->itemCount = qty;
			pOrderItemInfo->itemName = productName;
			pOrderItemInfo->itemPicUrl = picUrl;
			pOrderItemInfo->itemPrice = subTotalPrice - coupons - voucher - /*shoppingCash - shoppingCard - pointsPrice -*/ promote;
			pOrderItemInfo->itemStandard = color + " " + size;
			if(color == "" && size == "")
				pOrderItemInfo->itemStandard = "--";
			pOrderMsg->vecOrderItem.push_back(pOrderItemInfo);
		//}

		m_vecOrderMsgTradeInfo.push_back(pOrderMsg);

		std::sort(m_vecOrderMsgTradeInfo.begin(),m_vecOrderMsgTradeInfo.end(),ItemSortByTime);
	}
}

void CLocalData::AddReturnOrder(string orderData, string orderUuid)
{
	OrderMsg* pOrderMsg = new OrderMsg;
	pOrderMsg->orderData = orderData;
	pOrderMsg->orderUuid = orderUuid;
	pOrderMsg->orderType = 3;
	SetReturnOrderData(pOrderMsg, orderData);

	CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(true);
	if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL && IsWindowVisible(CDuiMsgDlg::GetInstance()->GetHWND()))
	{
		CDuiMsgDlg::GetInstance()->AddOrderMsg();
	}
	::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_OPEN_ORDER_MSG_DLG, (WPARAM)pOrderMsg, 0);
}

void CLocalData::SetReturnOrderData(OrderMsg* pOrderMsg, string orderData)
{
	CString str = orderData.c_str();
	int nLength = orderData.length();
	ReturnOrder returnOrder;
	if(returnOrder.ParseFromArray(orderData.c_str(), orderData.length()))
	{
		string rsSeq = returnOrder.rsseq();     // 退货批次流水号
		string ogno = returnOrder.ogno();       // 订单编号
		string ogseq = returnOrder.ogsseq();	// 订单流水号
		string applyDate = returnOrder.applydate();                          // 申请时间
		if(applyDate.empty())
		{	// 给个默认时间，不让程序弹出错误提示窗
			applyDate = "2016-1-1 0:0:0";
		}
		double applyPrice = returnOrder.applyprice();                         // 申请金额
		string merchantId = returnOrder.merchantid();                         // 商家ID
		string memGuid = returnOrder.memguid();                            // 会员编号
		string buyername = returnOrder.buyername();                          // 买家名字

		pOrderMsg->bIsRead = false;
		pOrderMsg->buyerName = buyername;
		pOrderMsg->orderNum = ogno;
		pOrderMsg->orderPackNum = rsSeq;
		pOrderMsg->orderTime = applyDate;
		//pOrderMsg->orderContent = evaluatecontext;

		int nYear, nMonth, nDate, nHour, nMin, nSec;
		sscanf(pOrderMsg->orderTime.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		CTime t(nYear, nMonth, nDate, nHour, nMin, nSec);
		pOrderMsg->dwOrderTime = t.GetTime();

		for(int nGoods = 0; nGoods < returnOrder.goodslist().size(); nGoods++)
		{
			const ReturnGoodsInfo &returnGoodsInfo = returnOrder.goodslist().Get(nGoods);
			string skuId = returnGoodsInfo.skuid();                              // 商城商品SKUID
			double productPrice = returnGoodsInfo.productprice();                       // 商品价格
			int32 productQty = returnGoodsInfo.productqty();                          // 商品数量
			int32 returnQty = returnGoodsInfo.returnqty();                           // 退货数量
			string productMainUrl = returnGoodsInfo.productmainurl();                     // 商品主图
			string sourceUrl = returnGoodsInfo.sourceurl();                          // 商品URL
			string color = returnGoodsInfo.color();                              // 颜色
			string size = returnGoodsInfo.size();                               // 大小
			string reason = returnGoodsInfo.reason();                             // 原因
			double aprnPromote = returnGoodsInfo.aprnpromote();                       // 分摊到此商品的优惠金额
			double aprnVoucher = returnGoodsInfo.aprnvoucher();                       // 分摊到此商品的优惠券金额
			double shippingTax = returnGoodsInfo.shippingtax();                       // 分摊到此商品的行邮税金额
			double origPrice = returnGoodsInfo.origprice();                         // 商品原价
			double aprnPoint = returnGoodsInfo.aprnpoint();                         // 分摊到此商品的积分
			double aprnPointPrice = returnGoodsInfo.aprnpointprice();                    // 分摊到此商品的积分抵扣金额
			double aprnCash = returnGoodsInfo.aprncash();                          // 实退现金
			double refundablePrice = returnGoodsInfo.refundableprice();                   // 应退金额
			string productName = returnGoodsInfo.productname();                   // 商品名称
			
			OrderItem* pOrderItemInfo = new OrderItem;
			pOrderItemInfo->itemCount = returnQty;
			pOrderItemInfo->itemName = productName;
			pOrderItemInfo->itemPicUrl = productMainUrl;
			//退换货的金额需要乘以商品数量
			pOrderItemInfo->itemPrice = (origPrice - aprnPromote - aprnVoucher) * pOrderItemInfo->itemCount;
			pOrderItemInfo->itemStandard = color + " " + size;
			if(color == "" && size == "")
				pOrderItemInfo->itemStandard = "--";
			pOrderMsg->vecOrderItem.push_back(pOrderItemInfo);

			if(nGoods == 0)
			{
				pOrderMsg->orderContent = reason;
				pOrderMsg->orderPackNum = returnGoodsInfo.rssseq();
			}
		}

		m_vecOrderMsgReturnInfo.push_back(pOrderMsg);

		std::sort(m_vecOrderMsgReturnInfo.begin(),m_vecOrderMsgReturnInfo.end(),ItemSortByTime);
	}
}

int CLocalData::GetOrderTradeUnReadCount()
{
	int nUnRead = 0;
	for(int nOrder = 0; nOrder < m_vecOrderMsgTradeInfo.size(); nOrder++)
	{
		if(!m_vecOrderMsgTradeInfo[nOrder]->bIsRead)
			nUnRead++;
	}
	return nUnRead;
}

int CLocalData::GetOrderReturnUnReadCount()
{
	int nUnRead = 0;
	for(int nOrder = 0; nOrder < m_vecOrderMsgReturnInfo.size(); nOrder++)
	{
		if(!m_vecOrderMsgReturnInfo[nOrder]->bIsRead)
			nUnRead++;
	}
	return nUnRead;
}

void CLocalData::RemoveAllOrder()
{
	for(int nOrder = 0; nOrder < m_vecOrderMsgTradeInfo.size(); nOrder++)
	{
		OrderMsg* pOrderMsg = m_vecOrderMsgTradeInfo[nOrder];
		for(int nItem = 0; nItem < pOrderMsg->vecOrderItem.size(); nItem++)
		{
			OrderItem* pOrderItem = pOrderMsg->vecOrderItem[nItem];
			delete pOrderItem;
			pOrderItem = NULL;
		}
		delete pOrderMsg;
		pOrderMsg = NULL;
	}
	for(int nOrder = 0; nOrder < m_vecOrderMsgReturnInfo.size(); nOrder++)
	{
		OrderMsg* pOrderMsg = m_vecOrderMsgReturnInfo[nOrder];
		for(int nItem = 0; nItem < pOrderMsg->vecOrderItem.size(); nItem++)
		{
			OrderItem* pOrderItem = pOrderMsg->vecOrderItem[nItem];
			delete pOrderItem;
			pOrderItem = NULL;
		}
		delete pOrderMsg;
		pOrderMsg = NULL;
	}
}
#endif

//排队数据
int CLocalData::GetCsQueueCount()
{
	int nCountAll = 0;
	std::map<int, int>::iterator itr = m_mapCsTeamToQueue.begin();
	for(;itr != m_mapCsTeamToQueue.end(); itr++)
	{
		nCountAll += itr->second;
	}
	return nCountAll;
}

void CLocalData::SetCsTeamQueue(int nTeam, int nCount)
{
	m_mapCsTeamToQueue[nTeam] = nCount;
}

//根据子帐号ID获取商家ID
int CLocalData::GetMerchantIdFormSubMerchantId(int nId)
{
	string strURL = "";

	CURL *curl;
	CURLcode res;
	string strRet;
	string strError;

	curl = curl_easy_init();
	if(curl) 
	{
		struct curl_httppost *formpost = 0;
		struct curl_httppost *lastptr  = 0;

#ifdef _VER_PREVIEW_		
		curl_easy_setopt(curl, CURLOPT_PROXY, PROXY_URL); 
#endif
		curl_easy_setopt(curl,CURLOPT_URL,MERCHANT_API); //url地址

		CString strParam;
		strParam.Format("method=feiniu.merchant.getMerchantIdByMemberId&params=%d&version=1.0", nId);
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS, strParam); //post参数  
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, curl_writer); //对返回的数据进行操作的函数地址  
		curl_easy_setopt(curl,CURLOPT_WRITEDATA, &strRet); //这是write_data的第四个参数值  
		curl_easy_setopt(curl,CURLOPT_POST,1); //设置问非0表示本次操作为post  
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
			string strRetutf8 = UTF8_Convert(strRet.c_str(), FALSE);
			//解析json字符创
			Json::Reader reader;
			Json::Value recv;

			if (reader.parse(strRetutf8, recv))
			{
				string strCode = recv["code"].asString();
				if(strCode == "200")
				{
					CString strId;
					strId.Format("%d", nId);
					int nMerchantId = recv["body"][strId].asInt();
					return nMerchantId;
				}
			}
		}
	}
	return 0;
}

//根据商家ID获取所有子帐号ID和子帐号类型
void CLocalData::GetAllSubMerchantIdFromMerchantId(int nId, std::vector<SubMerchantIdInfo>& vecSubMerchantIdInfo)
{
	string strURL = "";

	CURL *curl;
	CURLcode res;
	string strRet;
	string strError;

	curl = curl_easy_init();
	if(curl) 
	{
		struct curl_httppost *formpost = 0;
		struct curl_httppost *lastptr  = 0;

#ifdef _VER_PREVIEW_		
		curl_easy_setopt(curl, CURLOPT_PROXY, PROXY_URL); 
#endif
		curl_easy_setopt(curl,CURLOPT_URL,MERCHANT_API); //url地址  

		CString strParam;
		strParam.Format("method=feiniu.member.getSubAccountList&params=%d&version=1.0", nId);

		curl_easy_setopt(curl,CURLOPT_POSTFIELDS, strParam); //post参数  
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, curl_writer); //对返回的数据进行操作的函数地址  
		curl_easy_setopt(curl,CURLOPT_WRITEDATA, &strRet); //这是write_data的第四个参数值  
		curl_easy_setopt(curl,CURLOPT_POST,1); //设置问非0表示本次操作为post  
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
			string strRetutf8 = UTF8_Convert(strRet.c_str(), FALSE);
			//解析json字符创
			Json::Reader reader;
			Json::Value recv;

			if (reader.parse(strRetutf8, recv))
			{
				//获取result字段
				string strCode = recv["code"].asString();
				if(strCode == "200")
				{
					int nCount = recv["body"].size();

					for(int i = 0; i < nCount; ++i)
					{
						Json::Value strBody = recv["body"][i];
						int nMemberType = strBody["memberType"].asInt(); //商家子帐号类型 1为主账号 2为子帐号
						int nMemberId = strBody["memberId"].asInt(); //商家子帐号ID
						string strMemberName = strBody["username"].asString();

						SubMerchantIdInfo subMerchantIdInfo;
						subMerchantIdInfo.bIsMain = nMemberType == 1 ? TRUE : FALSE;
						subMerchantIdInfo.nId = nMemberId;
						subMerchantIdInfo.strName = strMemberName;

						vecSubMerchantIdInfo.push_back(subMerchantIdInfo);
					}
					Json::Reader readerBody;
					Json::Value recvBody;
				}
			}
		}
	}
}

//订单图片数据,获取图片地址对应的文件名
string CLocalData::GetUrlFile(string strUrl)
{
	return m_mapUrlFile[strUrl];
}

//设置图片地址对应的文件名
void CLocalData::SetUrlFile(string strUrl, string strFile)
{
	m_mapUrlFile[strUrl] = strFile;
}

//添加发送消息队列
void CLocalData::AddSendMsgQueue(SendMsgQueue sendMsgQueueInfo, int nSeqOld, int nSeqIdNew)
{
	m_lockSendMsg.Lock();
	if(!g_LocalData.SetSendMsgSeqNewSeq(nSeqOld, nSeqIdNew, sendMsgQueueInfo.sendTick))
		m_vecSendMsgQueue.push_back(sendMsgQueueInfo);
	m_lockSendMsg.Unlock();
}

void CLocalData::DeleteSendMsgQueue(int nSeq)
{
	m_lockSendMsg.Lock();
	vector<SendMsgQueue>::iterator itr = std::find(m_vecSendMsgQueue.begin(), m_vecSendMsgQueue.end(), nSeq);
	if(itr != m_vecSendMsgQueue.end())
	{
		m_vecSendMsgQueue.erase(itr);
	}
	m_lockSendMsg.Unlock();
}

vector<SendMsgQueue>& CLocalData::GetSendMsgQueue()
{
	return m_vecSendMsgQueue;
}

BOOL CLocalData::SetSendMsgSeqNewSeq(int nSeqId, int nSeqIdNew, uint32 tick)
{
	m_lockSendMsg.Lock();
	vector<SendMsgQueue>::iterator itr = std::find(m_vecSendMsgQueue.begin(), m_vecSendMsgQueue.end(), nSeqId);
	BOOL bIsExist = FALSE;
	if(itr != m_vecSendMsgQueue.end())
	{
		itr->seqId = nSeqIdNew;
		itr->sendTick = tick;
		bIsExist = TRUE;
	}
	m_lockSendMsg.Unlock();
	return bIsExist;
}

uint32 CLocalData::GetDeviceId()
{
	return m_nDeviceId;
}

void CLocalData::SetDeviceId(uint32 nDeviceId)
{
	m_nDeviceId = nDeviceId;
}

void CLocalData::SaveTranMsg(TranMsg* pTranMsg)
{
	m_vecTranMsgInfo.push_back(pTranMsg);
	CString strMsgContent;
	if(pTranMsg->nTranState == 1)
	{
		strMsgContent.Format("【转接请求】(%s)将买家 %s 转接给你服务", pTranMsg->strFromName.c_str(), pTranMsg->strCusterName.c_str());
	}
	else if (pTranMsg->nTranState == 2)
	{
		strMsgContent.Format("【转接拒绝】(%s)拒绝了买家 %s 的转接任务", pTranMsg->strFromName.c_str(), pTranMsg->strCusterName.c_str());
	}
	pTranMsg->strSystemMsg = strMsgContent;
	CString strFile;
	strFile.Format("%s\\%s\\TranMsg.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		// 使用事务，提高效率
		db.execDML("begin;");

		if(!db.tableExists("IM_SYSTEM_MSG"))
		{	// 文件不存在，创建表
			db.execDML("create table IM_SYSTEM_MSG(	\
					   Ver number(3) not null,\
					   MsgContent varchar2(100),    \
					   MsgTime number(20)   \
					   );");
		}

		CppSQLite3Buffer strInsertSql; 
		strInsertSql.format("insert into IM_SYSTEM_MSG values(%d,'%s',%d);", 
			VER_ORDER_MSG_INFO,
			strMsgContent, 
			pTranMsg->time
			);

		db.execDML(strInsertSql);

		db.execDML("commit;");
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write TranMsg.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

void CLocalData::LoadTranMsg()
{
	CString strFile;
	strFile.Format("%s\\%s\\TranMsg.db",m_strPath,m_sUser);

	// 判断文件是否存在
	bool bDbFileExist = _access(strFile,0)!=-1;

	// 文件不存在，创建表结构
	try
	{
		CppSQLite3DB db;
		db.open(strFile, DB_KEY, DB_KEY_L);

		if(db.tableExists("IM_SYSTEM_MSG"))
		{	
			CppSQLite3Table t = db.getTable("select * from IM_SYSTEM_MSG;");

			for (int row = 0; row < t.numRows(); row++)
			{
				t.setRow(row);
				CppSQLite3Binary  blob;
				int nVer = atoi(t.fieldValue("Ver"));
				string strMsgContent = (LPCTSTR)SqlDecode(t.fieldValue("MsgContent"));
				int nTime = atoi(t.fieldValue("MsgTime"));
				TranMsg* pTranMsg = new TranMsg;
				pTranMsg->strSystemMsg = strMsgContent;
				pTranMsg->time = nTime;
				//CString strOrderData = (LPCTSTR)SqlDecode(t.fieldValue("OrderData"));
				m_vecTranMsgInfo.push_back(pTranMsg);
			}
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString strError;
		strError.Format("write TranMsg.db error, sqlite code:%d errorMsg:%s", e.errorCode(), e.errorMessage());
		//m_pLog->WriteLog((LPCTSTR)strError);
		WriteLog(CLocalData::LL_ERROR, strError);
	}
}

std::vector<TranMsg*> CLocalData::GetTranMsg()
{
	return m_vecTranMsgInfo;
}