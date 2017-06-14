/************************************************************************ 
 �ļ���������������ҵ�񣬿ͻ����������
 
 �����ˣ� Zouyf, 2006-6-29 
 
 �޸ļ�¼�� 
            
************************************************************************/
#pragma once
#include "BusinessExIMSrv.h"
#include "DataBaseLib.h"

class CBusiHeartTick : public CBusinessExIMSrv
{
public:
	CBusiHeartTick(CRouterBusiness* pRouter);
	virtual ~CBusiHeartTick(void);
	virtual int DoWork();
};

class CHeartTickCheck
{
public:
	CHeartTickCheck();
	virtual ~CHeartTickCheck(void);
	int         StartWork();
	void		StopWork();
private:
	friend void  MyHeartTimeProc(void* dwUser);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();

	void SendLogoutReq(CUser* pUser);
private:
	PMMRESULT m_hMMT;
};

class CTimeLenSave
{
public:
	CTimeLenSave();
	virtual ~CTimeLenSave(void);
	int         StartWork();
	void		StopWork();
private:
	friend void  TimeLenProc(void* dwUser);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
private:
	CDataBaseMini m_pDataBaseMini;
	PMMRESULT m_hMMT;
};

