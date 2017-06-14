/******************************************************************
** �ļ���:	ITimeAxis.h
** ��  Ȩ:	
** ������:	
** ��  ��:	2007-5-7
** ��  ��:  

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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
	* ���ö�ʱ��
	* dwEventID	:	��ʱ�����
	* pTimerSink	:	�ص�����
	* dwInterval	:	��ʱʱ��
	* dwTouchTime	:	�������� Ĭ��ѭ������Ҳ�������޴�
	* szTimerDesc   :   ������Ϣ
	*/
	virtual bool setTimer(  uint32 dwEventID, 
							ITimerSink * pTimerSink, 
							uint32 dwInterval, 
							uint32 lTouchTime=TIME_infinity,
							char* szTimerDesc=0 ) = 0;

	/* 
	* ���ٶ�ʱ��
	* dwEventID	:	��ʱ�����
	* pTimerSink	:	�ص�����
	*/
	virtual void killTimer( uint32 dwEventID, ITimerSink * pTimerSink ) = 0;

	// ��ʱ���ʱ����	
	virtual void onCheck() = 0;
};


#endif//__I_TIME_AXIS_H___
