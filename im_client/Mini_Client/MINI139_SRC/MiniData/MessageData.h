// ----------------------------------------------------------------
// 文件:MessageData.h
// 版本: 
// 作者: xy
// 日期: 2006-12-12
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once


#include <MiniDataFile.h>
#include "../Im/impublic.h"

using namespace mini_data;

typedef CList<XT_MSG,XT_MSG> MSG_LIST;

#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

_MINIDATA_BEG

//------------------------------------------------------
// 聊天消息结构定义
//------------------------------------------------------

const FIELD_INFO MSG_VER1[] = {
	{"msg_type",		MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 0
	{"cluster_id",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 1
	{"cluster_type",	MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 2
	{"recv_flag",		MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 3
	{"from_id",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 4
	{"from_nickname",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	41	} },	// 5
	{"to_id",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 6
	{"ver",				MV_INT16,	{MINI_RESTRICT::EMPTY,		0	} },	// 7
	{"dest_ip",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 8
	{"dest_port",		MV_INT16,	{MINI_RESTRICT::EMPTY,		0	} },	// 9
	{"send_time",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 10
	{"face",			MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 11
	{"charset",			MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 12
	{"fontName",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	50	} },	// 13
	{"fontSize",		MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 14
	{"fontStyle",		MV_INT16,	{MINI_RESTRICT::EMPTY,		0	} },	// 15
	{"fontColor",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 16
	{"data_type",		MV_INT8,	{MINI_RESTRICT::EMPTY,		0	} },	// 17
	{"data",			MV_DATA,	{MINI_RESTRICT::DATA_LEN,	1500	} },	// 18
	{"data_len",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 19
};																			

//------------------------------------------------------
// 系统消息结构定义
//------------------------------------------------------

const FIELD_INFO SYS_MSG_VER1[] = {
	{"iType",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"dwTime",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"szMessage",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	512	} },
};

//------------------------------------------------------
// 群系统消息结构定义
//------------------------------------------------------

const FIELD_INFO CLUSTER_SYS_MSG_VER1[] = {
	{"iType",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"dwTime",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"szMessage",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	512	} },
	{"nInviteId",		MV_INT32,	{MINI_RESTRICT::EMPTY,	    0	} },
	{"szNickName",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	512	} },
	{"nClusterId",		MV_INT32,	{MINI_RESTRICT::EMPTY,	    0	} },
	{"szClusterName",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	512	} },
};

//------------------------------------------------------
// 好友信息结构定义（暂时无实现）
//------------------------------------------------------

const FIELD_INFO FRIEND_INFO_VER1[] = {
	{"id",				MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 0
};

//------------------------------------------------------
// 数据管理
//------------------------------------------------------

class CDataManager
{
public:

	CDataManager(void);

	~CDataManager(void);

	enum MSG_TYPE{
		USER,
		CLUSTER,
	};

public:

	void		FlushAllMsg();

	//------------------------------------------------------
	// 保存好友列表
	//------------------------------------------------------
	//bool			SaveFriendList(const FRIEND_LIST & fl);

	//------------------------------------------------------
	// 保存聊天消息
	//------------------------------------------------------
	bool			SaveMsg(const XT_MSG & msg,uint32 fid);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	bool			SaveSysMsg(uint32 uid,const SYSTEM_MSG & msg);
	
	//------------------------------------------------------
	// 保存群系统消息
	//------------------------------------------------------
	bool			SaveClusterSysMsg(uint32 uid,const CLUSTER_SYSTEM_MSG & msg);

	//------------------------------------------------------
	// 设置当前应用程序工作目录
	//------------------------------------------------------
	void			SetAppDir(const CString & sDir)
	{ m_sAppDir = sDir; };
	
	//------------------------------------------------------
	// 设置当前用户名
	//------------------------------------------------------
	void			SetCurUser(const CString & sUser)
	{ m_sCurUser = sUser; };

	//------------------------------------------------------
	// 获取所有聊天记录
	//------------------------------------------------------
	bool			GetAllMsg(MSG_LIST & msgs,uint32 id,MSG_TYPE type);

	//------------------------------------------------------
	// 获取所有系统消息
	//------------------------------------------------------
	bool			GetAllSysMsg(SYSTEM_MSG_LIST & sml,uint32 uid);

	//------------------------------------------------------
	// 获取群系统消息
	//------------------------------------------------------
	bool            GetClusterSysMsg(CLUSTER_SYSTEM_MSG_LIST & sml,uint32 uid);

	//------------------------------------------------------
	// 获取最近的n条聊天记录
	//------------------------------------------------------
	bool			GetLastMsg(MSG_LIST & msgs,uint32 id,MSG_TYPE type,uint32 nCount);

	//------------------------------------------------------
	// 删除群聊天记录
	//------------------------------------------------------
	bool			DeleteClusterTalkFile(uint32 cid);

	//------------------------------------------------------
	// 删除好友聊天记录
	//------------------------------------------------------
	bool			DeleteUserTalkFile(uint32 uid);

protected:

	CMiniDataFile *	FindDataFile(uint32 id,MSG_TYPE type)
	{
		MapMsgDbIt it;
		if(type == USER)
		{
			it = m_dbUser.find(id);
			if(it != m_dbUser.end())
				return it->second;
			else
				return NULL;
		}
		else
		{
			it = m_dbCluster.find(id);
			if(it != m_dbCluster.end())
				return it->second;
			else
				return NULL;
		}
	};

	void	CloseDataFile(uint32 id,MSG_TYPE type)
	{
		MapMsgDbIt it;
		if(type == USER)
		{
			it = m_dbUser.find(id);
			if(it != m_dbUser.end())
			{
				it->second->Close();
				delete it->second;
				m_dbUser.erase(it);
			}
		}
		else
		{
			it = m_dbCluster.find(id);
			if(it != m_dbCluster.end())
			{
				it->second->Close();
				delete it->second;
				m_dbCluster.erase(it);
			}
		}
	}

	CMiniDataFile *	CreateDataFile(uint32 id,int type);

	bool			GetMsg(const CMiniRecord * pRecord, XT_MSG & msg);

protected:

	typedef std::map<uint32,CMiniDataFile*>	MapMsgDb;
	typedef MapMsgDb::iterator				MapMsgDbIt;
	static CMiniRecordInfo					MSG_INFO;
	static CMiniRecordInfo					SYS_MSG_INFO;
	static CMiniRecordInfo					CLUSTER_SYS_MSG_INFO;
	static const char						KEY[16];

	CMiniRecord *	m_pMsg;			// 聊天记录
	CMiniRecord *	m_pSysMsg;		// 系统消息记录
	CMiniRecord *	m_pClusterSysMsg;	// 群系统消息记录
	CString			m_sAppDir;		// 应用程序的工作目录
	CString			m_sCurUser;		// 当前用户名
	MapMsgDb		m_dbUser;		// 用户聊天记录数据库
	MapMsgDb		m_dbCluster;	// 群聊天记录数据库
};


_MINIDATA_END