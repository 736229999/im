// ----------------------------------------------------------------
// �ļ�:MessageData.h
// �汾: 
// ����: xy
// ����: 2006-12-12
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
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
// ������Ϣ�ṹ����
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
// ϵͳ��Ϣ�ṹ����
//------------------------------------------------------

const FIELD_INFO SYS_MSG_VER1[] = {
	{"iType",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"dwTime",			MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"szMessage",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	512	} },
};

//------------------------------------------------------
// Ⱥϵͳ��Ϣ�ṹ����
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
// ������Ϣ�ṹ���壨��ʱ��ʵ�֣�
//------------------------------------------------------

const FIELD_INFO FRIEND_INFO_VER1[] = {
	{"id",				MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },	// 0
};

//------------------------------------------------------
// ���ݹ���
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
	// ��������б�
	//------------------------------------------------------
	//bool			SaveFriendList(const FRIEND_LIST & fl);

	//------------------------------------------------------
	// ����������Ϣ
	//------------------------------------------------------
	bool			SaveMsg(const XT_MSG & msg,uint32 fid);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	bool			SaveSysMsg(uint32 uid,const SYSTEM_MSG & msg);
	
	//------------------------------------------------------
	// ����Ⱥϵͳ��Ϣ
	//------------------------------------------------------
	bool			SaveClusterSysMsg(uint32 uid,const CLUSTER_SYSTEM_MSG & msg);

	//------------------------------------------------------
	// ���õ�ǰӦ�ó�����Ŀ¼
	//------------------------------------------------------
	void			SetAppDir(const CString & sDir)
	{ m_sAppDir = sDir; };
	
	//------------------------------------------------------
	// ���õ�ǰ�û���
	//------------------------------------------------------
	void			SetCurUser(const CString & sUser)
	{ m_sCurUser = sUser; };

	//------------------------------------------------------
	// ��ȡ���������¼
	//------------------------------------------------------
	bool			GetAllMsg(MSG_LIST & msgs,uint32 id,MSG_TYPE type);

	//------------------------------------------------------
	// ��ȡ����ϵͳ��Ϣ
	//------------------------------------------------------
	bool			GetAllSysMsg(SYSTEM_MSG_LIST & sml,uint32 uid);

	//------------------------------------------------------
	// ��ȡȺϵͳ��Ϣ
	//------------------------------------------------------
	bool            GetClusterSysMsg(CLUSTER_SYSTEM_MSG_LIST & sml,uint32 uid);

	//------------------------------------------------------
	// ��ȡ�����n�������¼
	//------------------------------------------------------
	bool			GetLastMsg(MSG_LIST & msgs,uint32 id,MSG_TYPE type,uint32 nCount);

	//------------------------------------------------------
	// ɾ��Ⱥ�����¼
	//------------------------------------------------------
	bool			DeleteClusterTalkFile(uint32 cid);

	//------------------------------------------------------
	// ɾ�����������¼
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

	CMiniRecord *	m_pMsg;			// �����¼
	CMiniRecord *	m_pSysMsg;		// ϵͳ��Ϣ��¼
	CMiniRecord *	m_pClusterSysMsg;	// Ⱥϵͳ��Ϣ��¼
	CString			m_sAppDir;		// Ӧ�ó���Ĺ���Ŀ¼
	CString			m_sCurUser;		// ��ǰ�û���
	MapMsgDb		m_dbUser;		// �û������¼���ݿ�
	MapMsgDb		m_dbCluster;	// Ⱥ�����¼���ݿ�
};


_MINIDATA_END