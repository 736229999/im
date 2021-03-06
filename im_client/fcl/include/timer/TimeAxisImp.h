/******************************************************************
** 文件名:	TimeAxisImp.h
** 版  权:	(C)
** 创建人:	
** 日  期:	2007-5-6
** 描  述:  

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/


#ifndef __TIME_AXIS_TEMPLATE_IMP_H__
#define __TIME_AXIS_TEMPLATE_IMP_H__

#include <assert.h>
#include "profile/profile.h"

#	pragma warning(push)
#	pragma warning(disable:4996)

////////////////////////* ʱ����ϸ�*/////////////////////////
//using namespace fcl;

#define TIMEAXISCLASS	TimeAxis<GetTimeFun,CallBackFun,HashFun,DebugInfo,ThreadingModel,_timeGrid,_axisLength>
#define TIMEAXIS_TEMPLATE_DECLARE		\
		template						\
		<								\
			class GetTimeFun ,			\
			class CallBackFun,			\
			class HashFun	 ,			\
			class DebugInfo ,			\
			class ThreadingModel ,		\
			uint32 _timeGrid,			\
			uint32 _axisLength			\
		>							



/*
 *	定时器构造函数

 */
TIMEAXIS_TEMPLATE_DECLARE
TIMEAXISCLASS::TimeAxis() : m_TimeAxis(enTimeGridsCount)
{
	m_initAxisTime = _GetTimeFun();
	m_dwLastCheckTime = m_initAxisTime;
}

/* 
 * ��ʱ�

 * dwEventID	:  定时器事件ID
 * pTimerSink   ： 回调仿函数

 * dwInterval	:  定时时间ʱ�

 * dwTouchTime	:  触发次数
 * szTimerDesc  ： 调试信息
 */

TIMEAXIS_TEMPLATE_DECLARE
bool TIMEAXISCLASS::SetTimer(  uint32 dwEventID, 
								CallBackFun pTimerSink, 
								uint32 dwInterval, 
								uint32 lTouchTime,
								DebugInfo info )
{
	typename ThreadingModel::scope_lock lock(m_Mutex);
	
	(void)lock;

	TIMER_KEY key;
	key.dwEventID = dwEventID;
	key.pTimerSink= pTimerSink;
	if ( m_TimerMap.find(key)!=m_TimerMap.end() )
	{
		//TRACE("ʱ����);
		return false;
	}

	// ����
	TIMER_DATA timer;
	timer.key = key;
	timer.lTouchTime  = lTouchTime;
	timer.dwInterval  = dwInterval;
	timer.debugInfo   = info;
	timer.dwLastTouchTime = _GetTimeFun();
	//timer.lLongTimerFlag = dwInterval / _axisLength;

	uint32 index = (timer.dwLastTouchTime+timer.dwInterval/*�Ҫ%_axisLength*/-m_initAxisTime) / _timeGrid;
	index %= enTimeGridsCount;

	m_TimeAxis[index].push_back(timer);

	// ���

	TIMER_POS pos;
	pos.dwGridIndex = index;
	pos.It = --m_TimeAxis[index].end();
	typename TIMER_MAP::value_type value(key,pos);
	
	m_TimerMap.insert(value);
    //激活事件

	m_hEventWrapper.Active();
	return true;
}

 /* 
 *  删除指定的定时器
 * dwEventID	:	定时器ID
 * pTimerSink	:  定时器仿函数
 */

TIMEAXIS_TEMPLATE_DECLARE
void TIMEAXISCLASS::KillTimer( uint32 dwEventID, CallBackFun pTimerSink )
{
	// 
	TIMER_KEY key;
	key.dwEventID = dwEventID;
	key.pTimerSink= pTimerSink;
	typename TIMER_MAP::iterator It = m_TimerMap.find(key);
	
	if ( It ==m_TimerMap.end() )
	{
		return;
	}

	// ɾڵ�

	TIMER_POS & pos = (*It).second;
	assert(pos.dwGridIndex < enTimeGridsCount );
	m_TimeAxis[pos.dwGridIndex].erase(pos.It);

	// ���

	m_TimerMap.erase(It);
	return;
}

/* 
* 
* dwEventID		:
* pTimerSink	:
*/
TIMEAXIS_TEMPLATE_DECLARE
void TIMEAXISCLASS::InvalidTimer( uint32 dwEventID, CallBackFun pTimerSink )
{
	// 
	typename ThreadingModel::scope_lock lock(m_Mutex);
	
	(void)lock;

	// 
	TIMER_KEY key;
	key.dwEventID = dwEventID;
	key.pTimerSink= pTimerSink;
	
	typename TIMER_MAP::iterator It = m_TimerMap.find(key);
	
	if ( It ==m_TimerMap.end() )
	{
		return;
	}

	// ɾڵ�

	TIMER_POS & pos = (*It).second;
	TIMER_DATA & timer = *pos.It;
	timer.key.dwEventID = INVALID_TIMER;

	// ���

	m_TimerMap.erase(It);

	return;
}

/*
 *
 */ 	

TIMEAXIS_TEMPLATE_DECLARE
void TIMEAXISCLASS::OnCheck()
{
	PROFILE("TIMEAXISCLASS::OnCheck");
	bool bEmpty = true;// 时间轴是否为空，没有定时数据
	uint32 lCurrentTime = _GetTimeFun();
	uint32 lCurrentGrid = (lCurrentTime - m_initAxisTime) / _timeGrid;
	uint32 lLastGrid    = (m_dwLastCheckTime-m_initAxisTime) / _timeGrid;
    

	for ( uint32 grid = lLastGrid; grid <= lCurrentGrid ; ++grid )
	{
		uint32 index = grid % enTimeGridsCount;

		typename ThreadingModel::scope_lock lock(m_Mutex);
		
		(void)lock;

		typename TIMER_LIST::iterator It = m_TimeAxis[index].begin();
		typename TIMER_LIST::iterator itTemp;

		
		for ( ; It != m_TimeAxis[index].end(); /*++It*/ )
		{
			bEmpty = false;//队列里有数据

			TIMER_DATA & timer = *It;
			itTemp = It;
			++It;

			// ��Чʱ�

			if ( timer.key.dwEventID == INVALID_TIMER )
			{
				m_TimeAxis[index].erase( itTemp );
				continue;
			}

			// �

			uint32 timeNow = _GetTimeFun();

			// �������Ƶ��ǲ��

			if ( timeNow/*lCurrentTime*/ - timer.dwLastTouchTime >= timer.dwInterval )
			{
				// ʱ�

				lock.unlock();
				try
				{
				timer.key.pTimerSink(timer.key.dwEventID);
				}
				catch(...)
				{
					KillTimer(timer.key.dwEventID,timer.key.pTimerSink);
					//此处如果出现异常，一般是因为定时器防函数pTimerSink已经析构，但用户忘记Kill掉定时器的原因

					assert(true);
				}
				lock.lock();
				timer.dwLastTouchTime = timeNow/*lCurrentTime*/;
				--timer.lTouchTime;

				// ���Լɾ�
				if ( timer.key.dwEventID == INVALID_TIMER )
				{
					m_TimeAxis[index].erase( itTemp );
					continue;
				}

				// �ٶʱ�
				if ( timer.lTouchTime <= 0 )
				{
					KillTimer(timer.key.dwEventID,timer.key.pTimerSink);
					continue;
				}else // ת�һε��

				{
					uint32 i = (timer.dwLastTouchTime+timer.dwInterval%_axisLength-m_initAxisTime) / _timeGrid;
					i %= enTimeGridsCount;

					m_TimeAxis[i].push_back(timer);

					typename TIMER_MAP::iterator pos = m_TimerMap.find(timer.key);
					assert( pos != m_TimerMap.end() );
					TIMER_POS & p = (*pos).second;
					p.dwGridIndex = index;
					p.It = --m_TimeAxis[i].end();

					m_TimeAxis[index].erase( itTemp );
				}
			}
		}
	}
	//如果时间轴队列为空，则等待，知道有数据进入队列

	if(bEmpty)
		WaitForSingleObject(m_hEventWrapper.m_handle,1);
	m_dwLastCheckTime = lCurrentTime;
}


#	pragma warning(pop)

#endif//__TIME_AXIS_TEMPLATE_IMP_H__
