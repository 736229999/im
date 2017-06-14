/************************************************************************ 
 �ļ���������������ҵ�񣬿ͻ����������
 
 �����ˣ� Zouyf, 2006-6-29 
 
 �޸ļ�¼�� 
            
************************************************************************/
#pragma once
#include "BusinessExIMSrv.h"
#include "DataBaseLib.h"

#ifdef ACCESS_SERVER
class CBusiHeartTick : public CBusinessExIMSrv
{
public:
	CBusiHeartTick(CRouterBusiness* pRouter);
	virtual ~CBusiHeartTick(void);
	virtual int DoWork();
};

class ImMemCache;
class CGlobalBusi;
class CUserManager;
class CHeartTickCheck
{
public:
	CHeartTickCheck();
	virtual ~CHeartTickCheck(void);
	int         StartWork();
	void		StopWork();
private:
	friend void  MyHeartTimeProc(void* dwUser);
	friend void  MyHeartTimeProcNew(union sigval v);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();

#ifdef IM_USE_MULTIDEVICE
	void SendLogoutReq(uint32 userid,uint32 deviceId);
#else
	void SendLogoutReq(uint32 userid);
#endif
private:
	PMMRESULT m_hMMT;
	timer_t m_timer;
	const unsigned int m_nTimerDelay = 1000 * 60 * 1;      		//Delay : 1����
	const unsigned int m_nTimerResolution = 1000 * 60 * 1; 		//Resolution : 1����
	const unsigned int m_nHeartTickTimeOut = 1000 * 60 * 3;		//������ʱʱ�䣬3����	
	const unsigned int m_nHeartTickWebTimeOut = 1000 * 60 * 10;		//������ʱʱ�䣬10����

	ImMemCache *m_pRedis;
	CGlobalBusi *m_pGlobalBusi;
	CUserManager *m_pUserManager;
};
#endif
/*
class CTimeLenSave
{
public:
	CTimeLenSave();
	virtual ~CTimeLenSave(void);
	int         StartWork();
	void		StopWork();
private:
	friend void  TimeLenProc(void* dwUser);
	friend void  TimeLenProcNew(union sigval v);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
private:
	CDataBaseMini m_pDataBaseMini;
	PMMRESULT m_hMMT;
	timer_t m_timer;

	const unsigned int m_nTimerDelay = 1000 * 60 * 1;      		//Delay : 1����
	const unsigned int m_nTimerResolution = 1000 * 60 * 1; 		//Resolution : 1����
};
*/
