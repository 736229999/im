/******************************************************************
** 文件名:	ITimeAxis.h
** 版  权:	
** 创建人:	
** 日  期:	2007-5-7
** 描  述:  

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#ifndef __I_TIME_AXIS_H___
#define __I_TIME_AXIS_H___


enum
{
	TIME_infinity = 0x7fffffff
};

struct ITimerSink
{
	virtual void onTimer( uint32 lEventID ) = 0;
};

struct ITimeAxis 
{
	/* 
	* 设置定时器
	* dwEventID	:	定时器编号
	* pTimerSink	:	回调函数
	* dwInterval	:	定时时间
	* dwTouchTime	:	触发次数 默认循环触发也就是无限次
	* szTimerDesc   :   调试信息
	*/
	virtual bool setTimer(  uint32 dwEventID, 
							ITimerSink * pTimerSink, 
							uint32 dwInterval, 
							uint32 lTouchTime=TIME_infinity,
							char* szTimerDesc=0 ) = 0;

	/* 
	* 销毁定时器
	* dwEventID	:	定时器编号
	* pTimerSink	:	回调函数
	*/
	virtual void killTimer( uint32 dwEventID, ITimerSink * pTimerSink ) = 0;

	// 定时检查时间轴	
	virtual void onCheck() = 0;
};


#endif//__I_TIME_AXIS_H___
