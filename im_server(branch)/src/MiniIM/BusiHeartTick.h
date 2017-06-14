/************************************************************************ 
 文件描述：处理心跳业务，客户端心跳检测
 
 创建人： Zouyf, 2006-6-29 
 
 修改记录： 
            
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
	const unsigned int m_nTimerDelay = 1000 * 60 * 1;      		//Delay : 1分钟
	const unsigned int m_nTimerResolution = 1000 * 60 * 1; 		//Resolution : 1分钟
	const unsigned int m_nHeartTickTimeOut = 1000 * 60 * 3;		//心跳超时时间，3分钟	
	const unsigned int m_nHeartTickWebTimeOut = 1000 * 60 * 10;		//心跳超时时间，10分钟

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

	const unsigned int m_nTimerDelay = 1000 * 60 * 1;      		//Delay : 1分钟
	const unsigned int m_nTimerResolution = 1000 * 60 * 1; 		//Resolution : 1分钟
};
*/
