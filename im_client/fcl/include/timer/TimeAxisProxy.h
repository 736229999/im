/******************************************************************
** �ļ���:	TimeAxisProxy.h
** ��  Ȩ:	
** ������:	
** ��  ��:	2007-5-7
** ��  ��:  

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

#ifndef __TIME_AXIS_PROXY_H__
#define __TIME_AXIS_PROXY_H__

//#include <string>

//#include "fcl.h"
//#include "framework.h"
#include "timer/MyTimer.h"
#include "Config.h"
#include "timer/ITimeAxis.h"
#include "timer/TimeAxis.h"
#include "thread/ThreadingModel.h"
#include <assert.h>
using namespace stdext;
#pragma warning(push,3)

#pragma warning(disable: 4311)

BEGIN_NAMESPACE_FCL
class GET_TIME_FUN 
{
public:
	uint32 operator()()
	{
		return m_timer.getTick();
	}
private:
   NewTimer<SystemTick,uint32,RES2> m_timer;
};

struct CALL_BACK_FUN 
{
	void operator()(uint32 lEventID)
	{
		pTimerSink->onTimer(lEventID);
	}

	friend bool operator==(const CALL_BACK_FUN & callBack,const CALL_BACK_FUN & callBack2)
	{
		return callBack2.pTimerSink==callBack.pTimerSink;
	}

	ITimerSink * pTimerSink;
};

struct TIMER_HASH
{
	uint32 operator()(uint32 lEventID,const CALL_BACK_FUN & fun) const 
	{
		return (uint32)(lEventID+(uint32)fun.pTimerSink);
	}
};

///////////////////////////////////////////////////////////////////////////////////
template< const int AXIS_LENGTH/*ʱ���᳤��*/=720000 >
class CTimeAxisProxy :ITimeAxis
{
public:
	CTimeAxisProxy()
	{
		m_bFinish = false; 
		initlize();
	}
	~CTimeAxisProxy()
	{
		m_bFinish = true;
		Sleep(1000);
	//	TerminateThread(m_hTimerThread,0);
	}
  bool  m_bFinish; //���ƶ�ʱ������̵߳���ֹ��־
public:
	/* 
	* ���ö�ʱ��
	* dwEventID		:	��ʱ�����
	* pTimerSink	:	�ص�����
	* dwInterval	:	��ʱʱ��
	* dwTouchTime	:	�������� Ĭ��ѭ������Ҳ�������޴�
	* szTimerDesc   :   ������Ϣ
	*/
	virtual bool setTimer(  uint32 dwEventID, 
							ITimerSink * pTimerSink, 
							uint32 dwInterval, 
							uint32  lTouchTime = TIME_infinity,
							char* szTimerDesc=NULL )
	{
		if ( dwInterval < 1000 )
		{
			CALL_BACK_FUN callbackFun;
			callbackFun.pTimerSink = pTimerSink;
			if (szTimerDesc==0)
				szTimerDesc = ("");
			return m_shortAxis.SetTimer( dwEventID , callbackFun, dwInterval , lTouchTime, szTimerDesc );
		}
		else
		{
			CALL_BACK_FUN callbackFun;
			callbackFun.pTimerSink = pTimerSink;
			if (szTimerDesc==0)
				szTimerDesc = ("");
			return m_longAxis.SetTimer( dwEventID , callbackFun, dwInterval , lTouchTime, szTimerDesc );
		}
	}

	/* 
	* ���ٶ�ʱ��
	* dwEventID	:	��ʱ�����
	* pTimerSink	:	�ص�����
	*/
	virtual void killTimer( uint32 dwEventID, ITimerSink * pTimerSink )
	{
		CALL_BACK_FUN callbackFun;
		callbackFun.pTimerSink = pTimerSink;
		m_longAxis.InvalidTimer( dwEventID , callbackFun );
		m_shortAxis.InvalidTimer( dwEventID , callbackFun );
	}

	// ��ʱ���ʱ����	
	virtual void onCheck()
	{
		m_longAxis.OnCheck();
		m_shortAxis.OnCheck();
	}
private:
	
	//��ʱ���ص�����
	static DWORD WINAPI timerThreadProc(LPVOID lpParameter)

	    {
			CTimeAxisProxy* pTim = (CTimeAxisProxy*)lpParameter;
			while(!pTim->m_bFinish)
			{
				pTim->onCheck();
			}
			return 0;
		}
		//��ʼ����ʱ����      
		void initlize()
		{
			m_hTimerThread = CreateThread(NULL,NULL,timerThreadProc,this,NULL,NULL);
			assert(m_hTimerThread);
		}
protected:
	typedef TimeAxis<	GET_TIME_FUN  ,
							CALL_BACK_FUN ,
							TIMER_HASH,
							char* ,
							multiThread,
							1,
							1000>	SHORT_AXIS;

	typedef TimeAxis<	GET_TIME_FUN  ,
							CALL_BACK_FUN ,
							TIMER_HASH,
							char* ,
							multiThread,
							128,
							AXIS_LENGTH>	LONG_AXIS;

	SHORT_AXIS	m_shortAxis;
	LONG_AXIS	m_longAxis;
	HANDLE m_hTimerThread;

};
///��������API
///////////////////////////////////////////////////////////////////////////////////
BEGIN_API
/*
 *   @param  dwEventID:  ��ʱ��ID
 *   @param  pTimerSink: ��ʱ���ص�����ָ��
 *   @param  dwInterval: ��ʱ���������Ϊ��λ
 *   @param  lTouchTime: ����������Ĭ��Ϊ���޴�
 *   @param  szTimerDesc: �ɼ���ĵ�����Ϣ
*/
FCL_API bool setTimer(  uint32 dwEventID, 
					  ITimerSink * pTimerSink, 
					  uint32 dwInterval, 
					  uint32  lTouchTime = TIME_infinity,
					  char* szTimerDesc=NULL );
/*
 *  @param dwEventID: ��ʱ��ID
 *  @param pTimerSink: ��ʱ���ص�����ָ��
*/
FCL_API void killTimer( uint32 dwEventID, ITimerSink * pTimerSink );

END_API
//class CTimeCheckTask : public CTask
//{
//public:
//	CTimeCheckTask( ITimeAxis* pAxis) : m_pAxis(pAxis)
//	{
//	}
//
//	virtual uint32 Run()
//	{
//		m_pAxis->onCheck();
//		return enTaskReturn_Repeat;
//	}
//
//	ITimeAxis * m_pAxis;
//};

///////////////////////////////////////////////////////////////////////////////////
#pragma warning(pop)


END_NAMESPACE
#endif/*__TIME_AXIS_PROXY_H__*/