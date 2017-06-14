#include "stdafx.h"
#include "timeCount.h"



CTimeCount	CTimeCount::s_timeCount;

LONGLONG *	CTimeCount::m_pHandles;	// 定时器计时

int			CTimeCount::m_nHandles;	// 定时器个数

LONGLONG	CTimeCount::m_nFreq;	// 定时器频率

const double CTimeCount::TIME_UNIT = 1000000.0;