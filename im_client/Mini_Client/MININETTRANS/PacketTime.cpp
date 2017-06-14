
/******************************************************************
** 文件名:	PacketTime.cpp
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-6-11 14:27
** 描  述:  udp数据包的时间管理类，用来计算rtt，带宽，数据流量等

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#include "stdafx.h"
#include "PacketTime.h"
#include "math.h"

CPacketTime::CPacketTime(uint32 wndSize /* = 16 */):
m_iAWSize(wndSize),
m_piPktWindow(NULL),
m_iRWSize(wndSize),
m_piRTTWindow(NULL),
m_piPCTWindow(NULL),
m_piPDTWindow(NULL),
m_iPWSize(wndSize),
m_piProbeWindow(NULL)
{
	m_piPktWindow = new int[m_iAWSize];
	m_piRTTWindow = new int[m_iRWSize];
	m_piPCTWindow = new int[m_iRWSize];
	m_piPDTWindow = new int[m_iRWSize];
	m_piProbeWindow = new int[m_iPWSize];

	m_iPktWindowPtr = 0;
	m_iRTTWindowPtr = 0;
	m_iProbeWindowPtr = 0;

	m_LastArrTime = m_timer.getTick();

	m_iLastSentTime = 0;
	m_iMinPktSndInt = 1000000;

	for (int i = 0; i < m_iAWSize; ++ i)
		m_piPktWindow[i] = 1;

	for (int j = 0; j < m_iRWSize; ++ j)
		m_piRTTWindow[j] = m_piPCTWindow[j] = m_piPDTWindow[j] = 0;

	for (int k = 0; k < m_iPWSize; ++ k)
		m_piProbeWindow[k] = 1000;
}


CPacketTime::~CPacketTime()
{
	delete [] m_piPktWindow;
	delete [] m_piRTTWindow;
	delete [] m_piPCTWindow;
	delete [] m_piPDTWindow;
	delete [] m_piProbeWindow;
}

int CPacketTime::getMinPktSndInt() const
{
	return m_iMinPktSndInt;
}

int CPacketTime::getPktRcvSpeed() const
{
	// sorting
	int temp,speed;
	for (int i = 0, n = (m_iAWSize >> 1) + 1; i < n; ++ i)
		for (int j = i, m = m_iAWSize; j < m; ++ j)
			if (m_piPktWindow[i] > m_piPktWindow[j])
			{
				temp = m_piPktWindow[i];
				m_piPktWindow[i] = m_piPktWindow[j];
				m_piPktWindow[j] = temp;
			}

			// read the median value
			int median = (m_piPktWindow[(m_iAWSize >> 1) - 1] + m_piPktWindow[m_iAWSize >> 1]) >> 1;
			int count = 0;
			int sum = 0;
			int upper = median << 3;
			int lower = median >> 3;

			// median filtering
			for (int k = 0, l = m_iAWSize; k < l; ++ k)
				if ((m_piPktWindow[k] < upper) && (m_piPktWindow[k] > lower))
				{
					++ count;
					sum += m_piPktWindow[k];
				}

				// claculate speed, or return 0 if not enough valid value
				if (count > (m_iAWSize >> 1))
					speed = (int)ceil(1000000.0 / (sum / count));
				else
					speed = 0;
	return speed > 10?speed:10;  //不小于10个包
}


int CPacketTime::getBandwidth() const
{
	// sorting
	int temp;
	for (int i = 0, n = (m_iPWSize >> 1) + 1; i < n; ++ i)
		for (int j = i, m = m_iPWSize; j < m; ++ j)
			if (m_piProbeWindow[i] > m_piProbeWindow[j])
			{
				temp = m_piProbeWindow[i];
				m_piProbeWindow[i] = m_piProbeWindow[j];
				m_piProbeWindow[j] = temp;
			}

			// read the median value
			int median = (m_piProbeWindow[(m_iPWSize >> 1) - 1] + m_piProbeWindow[m_iPWSize >> 1]) >> 1;
			int count = 1;
			int sum = median;
			int upper = median << 3;
			int lower = median >> 3;

			// median filtering
			for (int k = 0, l = m_iPWSize; k < l; ++ k)
				if ((m_piProbeWindow[k] < upper) && (m_piProbeWindow[k] > lower))
				{
					++ count;
					sum += m_piProbeWindow[k];
				}

				return (int)ceil(1000000.0 / (double(sum) / double(count)));
}

void CPacketTime::onPktSent(const int& currtime)
{
	int interval = currtime - m_iLastSentTime;

	if ((interval < m_iMinPktSndInt) && (interval > 0))
		m_iMinPktSndInt = interval;

	m_iLastSentTime = currtime;
}

void CPacketTime::onPktArrival(uint32& msgNo)
{
	m_CurrArrTime = m_timer.getTick();

	// record the packet interval between the current and the last one
	m_piPktWindow[m_iPktWindowPtr] = int(m_CurrArrTime - m_LastArrTime);

	// the window is logically circular
	m_iPktWindowPtr = (m_iPktWindowPtr + 1) % m_iAWSize;

	// remember last packet arrival time
	m_LastArrTime = m_CurrArrTime;

	// check if it is probing packet pair
	if (0 == (msgNo & 0xF))
		probe1Arrival();
	else if (1 == (msgNo & 0xF))
		probe2Arrival();
}

void CPacketTime::ack2Arrival(const int& rtt)
{
	// record RTT, comparison (1 or 0), and absolute difference
	m_piRTTWindow[m_iRTTWindowPtr] = rtt;
	m_piPCTWindow[m_iRTTWindowPtr] = (rtt > m_piRTTWindow[(m_iRTTWindowPtr - 1 + m_iRWSize) % m_iRWSize]) ? 1 : 0;
	m_piPDTWindow[m_iRTTWindowPtr] = abs(rtt - m_piRTTWindow[(m_iRTTWindowPtr - 1 + m_iRWSize) % m_iRWSize]);

	// the window is logically circular
	m_iRTTWindowPtr = (m_iRTTWindowPtr + 1) % m_iRWSize;
}

void CPacketTime::probe1Arrival()
{
	m_ProbeTime = m_timer.getTick();
}

void CPacketTime::probe2Arrival()
{
	m_CurrArrTime = m_timer.getTick();

	// record the probing packets interval
	m_piProbeWindow[m_iProbeWindowPtr] = int(m_CurrArrTime - m_ProbeTime);
	// the window is logically circular
	m_iProbeWindowPtr = (m_iProbeWindowPtr + 1) % m_iPWSize;
}
