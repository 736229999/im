/******************************************************************
** �ļ���:	TimeAxis.h
** ��  Ȩ:	(C)
** ������:	
** ��  ��:	2007-5-6
** ��  ��:  

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

/*
 *  ����:
 *  1.ʱ����ĺ���ʵ����һ����Ч�������㷨
 *  2.���㷨�Բ���ɾ������Ҫ�󼫸�
 *  3.��Ҫ�ǲ�������,���Ƕ�һ��������������
 *  4.��������ʵ������Ҫ��Ҫ������ξ����ҵ�һ��ֵ�������ڵ�λ��
 *  
 *  ��Ʒ���:
 *  1.��ʱ����ֳ�N���̶�,ÿ���̶��ڱ���˶�ʱ������Ҫ���õ�Timer
 *  2.��Timer�����N����Timer�ص�
 *  3.��Ϊ�ڵ�λʱ��̶���ֻ��Ҳֻ����ü���,����ͬһ�̶��ڵ�Timer��������
 *	4.����ʱ������N����ͬ���ȶε���ʱ������϶���,������ʱƵ����1������
 *    ��ҪƵ�����õĶ�ʱ�����Է���һ��ÿ�񳤶�1ms���ȵ���1�뼫��ϸ����ʱ������
 *    �Դﵽ���Ч��
 */

#ifndef __TIME_AXIS_IMP_H__
#define __TIME_AXIS_IMP_H__

#include <list>
#include <vector>
#include <hash_map>
#include "thread/ThreadingModel.h"

using namespace stdext;



// �Ƽ����Ƶ��	:	16(ms) 
// �Ƽ�ʱ��̶� :   128(ms)
#define CHECK_FREQUENCY	    16	//��ȷ��16ms
#define TIME_GRID			128  

// ʱ���᳤��
#define TIME_AXIS_LENGTH		360000
#define INVALID_TIMER			0xffffffff

/*
 *  ʱ����ʵ����
 *  CallBackFun		: �ص�����
 *	DebugInfo		: ������Ϣ
 *  ThreadingModel	: �߳�ģ��ѡ��
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
	* ���ö�ʱ��
	* dwEventID		:	��ʱ�����
	* pTimerSink	:	�ص�����
	* dwInterval	:	��ʱʱ��
	* dwTouchTime	:	�������� Ĭ��ѭ������Ҳ�������޴�
	* szTimerDesc   :   ������Ϣ
	*/
	bool SetTimer(  uint32 dwEventID, 
					CallBackFun pTimerSink, 
					uint32 dwInterval, 
					uint32 lTouchTime,
					DebugInfo info );

	/* 
	 * ���ٶ�ʱ��
	 * dwEventID	:	��ʱ�����
	 * pTimerSink	:	�ص�����
	 */
	void KillTimer( uint32 dwEventID, CallBackFun pTimerSink );

	/* 
	* ��Ϊ���������bug,������һ����ĳ��Timer�ó���Ч�Ĺ���,�ⲿ��Ҫֱ��ɾ����ʱ��,ֻҪ���ö�ʱ���ó���Ч�ȿ�
	* dwEventID		:	��ʱ�����
	* pTimerSink	:	�ص�����
	*/
	void InvalidTimer( uint32 dwEventID, CallBackFun pTimerSink );

	// ��ʱ���ʱ����	
	void OnCheck();

	TimeAxis();

private:
	////////////////////////////////////////////////////////////////////////////////
	struct TIMER_KEY
	{
		uint32		dwEventID;		// ��ʱ��ID
		CallBackFun pTimerSink;		// ��ʱ���ص�����

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

	struct TIMER_DATA				// ��ʱ������
	{
		TIMER_KEY	key;			// ��ʱ����ֵ
		uint32		dwInterval;		// ��ʱ���
		long		lTouchTime;		// ��������
		DebugInfo	debugInfo;		// ������Ϣ

		uint32       dwLastTouchTime; // �ϴδ���ʱ��
	};


	struct TIMER_POS
	{
		uint32		dwGridIndex;	// ��ʱ������ʱ���
		
		typename std::list< TIMER_DATA >::iterator It; // �����б��е�λ��
	};

	enum
	{
		enTimeGridsCount = _axisLength / _timeGrid,
	};

	typedef typename std::list< TIMER_DATA >				TIMER_LIST;
	typedef typename std::vector<TIMER_LIST>				TIME_AXIS;

	typedef stdext::hash_map<TIMER_KEY,TIMER_POS>		TIMER_MAP;

	TIME_AXIS	m_TimeAxis;			// ʱ����
	TIMER_MAP	m_TimerMap;			// ��ʱ��λ������

	uint32		m_initAxisTime;		// ʱ�����ʼ��ʱ��
	uint32		m_dwLastCheckTime;	// ������ʱ��

	GetTimeFun  _GetTimeFun;		// ȡ��ǰʱ�亯��
	typename ThreadingModel::mutex m_Mutex;	// ���߳���
	CEventWrapper   m_hEventWrapper;   //����ʱ����Ϊ��ʱ�ȴ���֪����Ϊ�գ���ֹ��ѭ��
};

/*
 *	ʵ��ϸ��
 */
#include "TimeAxisImp.h"


#endif//__TIME_AXIS_IMP_H__
