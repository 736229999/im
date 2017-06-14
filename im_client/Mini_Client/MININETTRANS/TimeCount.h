// ----------------------------------------------------------------
// �ļ�:TimeCount.h
// �汾: 
// ����: xy
// ����: 2007-7-25
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include <assert.h>
#include <Windows.h>
//
//#include <iostream>
//using namespace std;

typedef int HTIMECOUNT;

class CTimeCount
{
protected:

	CTimeCount()
	{
		LARGE_INTEGER liFreq;
		m_pHandles = new LONGLONG[1];
		memset(m_pHandles,0,sizeof(LONGLONG)*1);
		m_nHandles = 1;
		QueryPerformanceFrequency (&liFreq);
		m_nFreq = liFreq.QuadPart;
	};
	
	// ʱ�䵥λ��΢�룩
	const static  double TIME_UNIT;

public:

	static ULONGLONG GetCPUTickCount()
	{
		__asm _emit 0x0F 
		__asm _emit 0x31 
	}
	
	// Ϊ��߾��ȾͿ����ӳ�����ʱ��,Ĭ�ϲ���100����
	static ULONGLONG GetCPUFreq(UINT nTestTime = 100)
	{
		HTIMECOUNT h1;
		LONGLONG l1,l2,lOverHead1,lTime,lTickCount;
		l1 = CTimeCount::GetCPUTickCount();
		h1 = BeginTimer();
		lTime = EndTimer(h1);
		l2 = CTimeCount::GetCPUTickCount();
		lOverHead1 = l2 - l1;

		l1 = CTimeCount::GetCPUTickCount();
		h1 = BeginTimer();
		Sleep(nTestTime);	
		lTime = EndTimer(h1);
		l2 = CTimeCount::GetCPUTickCount();
		lTickCount = l2 - l1 - lOverHead1;
		return ULONGLONG((TIME_UNIT / (double)lTime)*lTickCount); 
	}

	~CTimeCount()
	{
		if(m_pHandles)
		{
			delete [] m_pHandles;
			m_pHandles = NULL;
		}
	};

	static void USleep(const unsigned __int64 & time)
	{
		ULONGLONG cEnd,c;
		QueryPerformanceCounter((LARGE_INTEGER*)&cEnd);
		c = cEnd;
		cEnd += ULONGLONG(time * ( m_nFreq / 1000000.0));
		while (c < cEnd)
		{
#ifdef IA32
			__asm__ volatile ("nop; nop; nop; nop; nop;");
#elif IA64
			__asm__ volatile ("nop 0; nop 0; nop 0; nop 0; nop 0;");
#endif
			QueryPerformanceCounter((LARGE_INTEGER*)&c);
		}
	}

	static HTIMECOUNT	BeginTimer()
	{
		int i;
		for(i = 0; i < m_nHandles; i++)
		{
			if(m_pHandles[i] == 0)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&m_pHandles[i]);
				return i;
			}
		}
		LONGLONG * pNewSpace = new LONGLONG[m_nHandles * 2];
		memset(pNewSpace,0,m_nHandles * 2);
		memcpy(pNewSpace,m_pHandles,sizeof(LONGLONG) * m_nHandles);
		m_nHandles *= 2;
		delete [] m_pHandles;
		m_pHandles = pNewSpace;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_pHandles[i]);
		return i;
	};

	//------------------------------------------------------
	// ΢�뼶�ļ���
	//------------------------------------------------------
	static ULONGLONG	GetTickCount()
	{
		ULONGLONG	tick;
		QueryPerformanceCounter((LARGE_INTEGER*)&tick);
		return ULONGLONG(tick * (TIME_UNIT / m_nFreq));
	};

	// ���ؼ�ʱʱ�䣬��λ��1΢��
	static LONGLONG		EndTimer(HTIMECOUNT hTimer)
	{
		LONGLONG nTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nTime);
		nTime -= m_pHandles[hTimer];
		nTime = LONGLONG((double)nTime * (TIME_UNIT / m_nFreq) );
		m_pHandles[hTimer] = 0;
		return nTime;
	};

private:

	static CTimeCount		s_timeCount;	

	static LONGLONG *		m_pHandles;		// ��ʱ����ʱ

	static int				m_nHandles;		// ��ʱ������

	static LONGLONG			m_nFreq;		// ��ʱ��Ƶ��

};


#define beginTimer()		CTimeCount::BeginTimer()

#define endTimer(hTimer)	CTimeCount::EndTimer(hTimer)

#define getTickCount()		CTimeCount::GetTickCount()

