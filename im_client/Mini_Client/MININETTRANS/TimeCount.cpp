#include "stdafx.h"
#include "timeCount.h"



CTimeCount	CTimeCount::s_timeCount;

LONGLONG *	CTimeCount::m_pHandles;	// ��ʱ����ʱ

int			CTimeCount::m_nHandles;	// ��ʱ������

LONGLONG	CTimeCount::m_nFreq;	// ��ʱ��Ƶ��

const double CTimeCount::TIME_UNIT = 1000000.0;