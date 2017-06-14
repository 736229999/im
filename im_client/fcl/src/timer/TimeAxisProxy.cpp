/******************************************************************
** �ļ���:	TimeAxisProxy.cpp
** ��  Ȩ:	(C)  
** ������:	Tony
** ��  ��:
** ��  ��:
**************************** �޸ļ�¼ ******************************
** �޸���: Tony
** ��  ��: 2007-5-7
** ��  ��: ����ȫ�ֵĶ�ʱ������,����ʱ������
********************************************************************/

#include "stdafx.h"
#include "timer/TimeAxisProxy.h"

BEGIN_NAMESPACE_FCL

CTimeAxisProxy<> g_timerAix;
//���ö�ʱ��
 bool setTimer(  uint32 dwEventID, 
					  ITimerSink * pTimerSink, 
					  uint32 dwInterval, 
					  uint32  lTouchTime/* = TIME_infinity*/,
					  char* szTimerDesc/*=NULL*/ )
 {
	 return g_timerAix.setTimer(dwEventID,pTimerSink,dwInterval,lTouchTime,szTimerDesc);
 }
//���ٶ�ʱ��
void killTimer( uint32 dwEventID, ITimerSink * pTimerSink )
{
	return g_timerAix.killTimer(dwEventID,pTimerSink);
}
END_NAMESPACE