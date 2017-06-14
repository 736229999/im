
#ifndef ATL_TIME_H
#define ATL_TIME_H

#include <ctime>

class CTime
{
public:
	CTime(time_t nSecond)
	{
		m_nSecond = nSecond;

		struct tm ts;
		localtime_r(&m_nSecond, &ts);
		m_nYear = ts.tm_year+1900;
		m_nMonth = ts.tm_mon+1;
		m_nDay = ts.tm_mday;
		m_nHour = ts.tm_hour;
		m_nMin  = ts.tm_min;
		m_nSec  = ts.tm_sec;
	}

	int GetDay()
	{
		return m_nDay;
	}

	int GetYear()
	{
		return m_nYear;
	}

	int GetMonth()
	{
		return m_nMonth;
	}

	int GetHour()
	{
		return m_nHour;
	}

	int GetMinute()
	{
		return m_nMin;
	}

	int GetSecond()
	{
		return m_nSec;
	}
private:
	time_t m_nSecond;
	int   m_nYear;
	int	  m_nMonth;
	int	  m_nDay;
	int	  m_nHour;
	int	  m_nMin;
	int	  m_nSec;

};

#endif


