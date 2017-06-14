#pragma once
#include "ErrorCode.h"
#include "DataBaseLib.h"
#include "RouterBusiness.h"
#include "ConfigInfo.h"

//������ת�����Կͻ��˵���Ϣ��ֱ�ӷ���ϵͳ��Ϣ��Ӧ��,
//��ҵ����������������,���ٷ�������
class CBusinessWebMsg
{
public:
	CBusinessWebMsg();
	~CBusinessWebMsg(void);
	void SendSystemLog(ST_SYSTEM_LOG *pSystemLog);
	void SystemLog(uint32 recID,uint32 projID,uint32 operid,uint32 userid,uint8 dtype);
	void SendWokuMsg(ST_WOKU_MSG *pMsg);
};



class CSystemLogCheck
{
public:
	CSystemLogCheck();
	virtual ~CSystemLogCheck(void);
	int         StartWork();
	void		StopWork();
private:
	friend void  MySystemLogProc(void * dwUser);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
private:
	CConfigInfo		m_Config;

	CDataBaseWeb m_pDataBaseWeb;
	uint32 m_nRecID;
	CBusinessWebMsg m_WebMsg;
	ST_SYSTEM_LOG*	m_pSystemLogBuf;
	PMMRESULT m_hMMT;
};

class COperationCheck
{
public:
	COperationCheck();
	virtual ~COperationCheck();
	int		StartWork();
	void	StopWork();
private:
	friend void  MyOperationProc(void * dwUser);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
private:
	CConfigInfo m_Config;
	CDataBaseWeb m_pDataBaseWeb;
	uint32 m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nBroadcastID,m_nUserCommentID,m_nLogCommentID;
	CBusinessWebMsg m_WebMsg;
	ST_WOKU_MSG*	m_pOperationBuf;
	PMMRESULT m_hMMT;
};
