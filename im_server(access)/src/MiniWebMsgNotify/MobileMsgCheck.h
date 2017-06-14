#pragma once
/*
#include "ErrorCode.h"
#include "DataBaseLib.h"
#include "RouterBusiness.h"
#include "ConfigInfo.h"

class CMobileMsgCheck
{
public:
	CMobileMsgCheck(void);
	virtual ~CMobileMsgCheck(void);
	int         StartWork();
	void		StopWork();
private:
	friend void  MobileMsgProc(void* dwMobileMsg);
	
	friend void  MobileMsgProcNew(union sigval v);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
private:
	CDataBaseWeb m_pDataBaseWeb;
	uint32 m_nRecID;
	void SendMobileMsg(ST_MOBILE_MSG *pMsg);
	ST_MOBILE_MSG*	m_pMobileMsgBuf;
	PMMRESULT m_hMMT;
	timer_t m_timer;
	CConfigInfo m_Config;
};*/
