/******************************************************************
** 文件名:	TimeAxisProxy.h
** 版  权:	
** 创建人:	
** 日  期:	2007-5-7
** 描  述:  

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
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
template< const int AXIS_LENGTH/*时间轴长度*/=720000 >
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
  bool  m_bFinish; //控制定时器检查线程的中止标志
public:
	/* 
	* 设置定时器
	* dwEventID		:	定时器编号
	* pTimerSink	:	回调函数
	* dwInterval	:	定时时间
	* dwTouchTime	:	触发次数 默认循环触发也就是无限次
	* szTimerDesc   :   调试信息
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
	* 销毁定时器
	* dwEventID	:	定时器编号
	* pTimerSink	:	回调函数
	*/
	virtual void killTimer( uint32 dwEventID, ITimerSink * pTimerSink )
	{
		CALL_BACK_FUN callbackFun;
		callbackFun.pTimerSink = pTimerSink;
		m_longAxis.InvalidTimer( dwEventID , callbackFun );
		m_shortAxis.InvalidTimer( dwEventID , callbackFun );
	}

	// 定时检查时间轴	
	virtual void onCheck()
	{
		m_longAxis.OnCheck();
		m_shortAxis.OnCheck();
	}
private:
	
	//定时器回调函数
	static DWORD WINAPI timerThreadProc(LPVOID lpParameter)

	    {
			CTimeAxisProxy* pTim = (CTimeAxisProxy*)lpParameter;
			while(!pTim->m_bFinish)
			{
				pTim->onCheck();
			}
			return 0;
		}
		//初始化定时器类      
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
///导出常用API
///////////////////////////////////////////////////////////////////////////////////
BEGIN_API
/*
 *   @param  dwEventID:  定时器ID
 *   @param  pTimerSink: 定时器回调函数指针
 *   @param  dwInterval: 定时间隔，毫秒为单位
 *   @param  lTouchTime: 触发次数，默认为无限次
 *   @param  szTimerDesc: 可加入的调试信息
*/
FCL_API bool setTimer(  uint32 dwEventID, 
					  ITimerSink * pTimerSink, 
					  uint32 dwInterval, 
					  uint32  lTouchTime = TIME_infinity,
					  char* szTimerDesc=NULL );
/*
 *  @param dwEventID: 定时器ID
 *  @param pTimerSink: 定时器回调函数指针
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