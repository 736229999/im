/******************************************************************
** 文件名:	TimeAxisProxy.cpp
** 版  权:	(C)  
** 创建人:	Tony
** 日  期:
** 描  述:
**************************** 修改记录 ******************************
** 修改人: Tony
** 日  期: 2007-5-7
** 描  述: 生成全局的定时器对象,及定时器函数
********************************************************************/

#include "stdafx.h"
#include "timer/TimeAxisProxy.h"

BEGIN_NAMESPACE_FCL

CTimeAxisProxy<> g_timerAix;
//设置定时器
 bool setTimer(  uint32 dwEventID, 
					  ITimerSink * pTimerSink, 
					  uint32 dwInterval, 
					  uint32  lTouchTime/* = TIME_infinity*/,
					  char* szTimerDesc/*=NULL*/ )
 {
	 return g_timerAix.setTimer(dwEventID,pTimerSink,dwInterval,lTouchTime,szTimerDesc);
 }
//销毁定时器
void killTimer( uint32 dwEventID, ITimerSink * pTimerSink )
{
	return g_timerAix.killTimer(dwEventID,pTimerSink);
}
END_NAMESPACE