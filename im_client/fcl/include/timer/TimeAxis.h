/******************************************************************
** 文件名:	TimeAxis.h
** 版  权:	(C)
** 创建人:	
** 日  期:	2007-5-6
** 描  述:  

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

/*
 *  分析:
 *  1.时间轴的核心实际是一个高效的排序算法
 *  2.此算法对插入删除操作要求极高
 *  3.主要是插入排序,不是对一堆数字整体排序
 *  4.插入排序实际最主要的要求是如何尽快找到一个值大致所在的位置
 *  
 *  设计方案:
 *  1.把时间轴分成N个刻度,每个刻度内保存此段时间内需要调用的Timer
 *  2.长Timer折算成N个短Timer回调
 *  3.因为在单位时间刻度内只能也只需调用几次,所以同一刻度内的Timer不需排序
 *	4.完整时间轴由N个不同长度段的子时间轴组合而成,这样定时频率在1秒以下
 *    需要频繁调用的定时器可以放在一个每格长度1ms长度等于1秒极度细化的时间轴中
 *    以达到最高效率
 */

#ifndef __TIME_AXIS_IMP_H__
#define __TIME_AXIS_IMP_H__

#include <list>
#include <vector>
#include <hash_map>
#include "thread/ThreadingModel.h"

using namespace stdext;



// 推荐检查频率	:	16(ms) 
// 推荐时间刻度 :   128(ms)
#define CHECK_FREQUENCY	    16	//精确到16ms
#define TIME_GRID			128  

// 时间轴长度
#define TIME_AXIS_LENGTH		360000
#define INVALID_TIMER			0xffffffff

/*
 *  时间轴实现类
 *  CallBackFun		: 回调函数
 *	DebugInfo		: 调试信息
 *  ThreadingModel	: 线程模型选择
 */
template
< 
	class GetTimeFun  ,
	class CallBackFun ,
	class HashFun	  ,
	class DebugInfo ,
	class ThreadingModel = multiThread ,
	uint32 _timeGrid   = TIME_GRID,
	uint32 _axisLength = TIME_AXIS_LENGTH
>
class TimeAxis
{
public:
	/* 
	* 设置定时器
	* dwEventID		:	定时器编号
	* pTimerSink	:	回调函数
	* dwInterval	:	定时时间
	* dwTouchTime	:	触发次数 默认循环触发也就是无限次
	* szTimerDesc   :   调试信息
	*/
	bool SetTimer(  uint32 dwEventID, 
					CallBackFun pTimerSink, 
					uint32 dwInterval, 
					uint32 lTouchTime,
					DebugInfo info );

	/* 
	 * 销毁定时器
	 * dwEventID	:	定时器编号
	 * pTimerSink	:	回调函数
	 */
	void KillTimer( uint32 dwEventID, CallBackFun pTimerSink );

	/* 
	* 因为链表的镶套bug,这里做一个将某个Timer置成无效的功能,外部不要直接删除定时器,只要将该定时器置成无效既可
	* dwEventID		:	定时器编号
	* pTimerSink	:	回调函数
	*/
	void InvalidTimer( uint32 dwEventID, CallBackFun pTimerSink );

	// 定时检查时间轴	
	void OnCheck();

	TimeAxis();

private:
	////////////////////////////////////////////////////////////////////////////////
	struct TIMER_KEY
	{
		uint32		dwEventID;		// 定时器ID
		CallBackFun pTimerSink;		// 定时器回调函数

		friend bool operator==(const TIMER_KEY & key1,const TIMER_KEY & key2)
		{
			return key1.dwEventID == key2.dwEventID && key1.pTimerSink==key2.pTimerSink;
		}
		friend bool operator<(const TIMER_KEY & key1,const TIMER_KEY & key2)
		{
			return !(key1==key2);
		};

		operator size_t() const
		{
			return HashFun()(dwEventID,pTimerSink);
		}
	};

	struct TIMER_DATA				// 定时器数据
	{
		TIMER_KEY	key;			// 定时器键值
		uint32		dwInterval;		// 定时间隔
		long		lTouchTime;		// 触发次数
		DebugInfo	debugInfo;		// 调试信息

		uint32       dwLastTouchTime; // 上次触发时间
	};


	struct TIMER_POS
	{
		uint32		dwGridIndex;	// 定时器所在时间格
		
		typename std::list< TIMER_DATA >::iterator It; // 所在列表中的位置
	};

	enum
	{
		enTimeGridsCount = _axisLength / _timeGrid,
	};

	typedef typename std::list< TIMER_DATA >				TIMER_LIST;
	typedef typename std::vector<TIMER_LIST>				TIME_AXIS;

	typedef stdext::hash_map<TIMER_KEY,TIMER_POS>		TIMER_MAP;

	TIME_AXIS	m_TimeAxis;			// 时间轴
	TIMER_MAP	m_TimerMap;			// 定时器位置索引

	uint32		m_initAxisTime;		// 时间轴初始化时间
	uint32		m_dwLastCheckTime;	// 最后检查的时间

	GetTimeFun  _GetTimeFun;		// 取当前时间函数
	typename ThreadingModel::mutex m_Mutex;	// 多线程锁
	CEventWrapper   m_hEventWrapper;   //当定时队列为空时等待，知道不为空，防止空循环
};

/*
 *	实现细节
 */
#include "TimeAxisImp.h"


#endif//__TIME_AXIS_IMP_H__
