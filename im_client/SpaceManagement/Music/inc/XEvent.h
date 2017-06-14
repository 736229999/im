#pragma once

#include <Windows.h>

//------------------------------------------------------
// WindowsÊÂ¼þÀà
//------------------------------------------------------

class XEvent
{
protected:

	XEvent(const XEvent & o);

	void operator = (const XEvent & o);

public:

	operator HANDLE () const
	{
		return m_hEvent;
	}

	explicit XEvent(BOOL bInitState = FALSE)
	{
		m_hEvent = ::CreateEvent(NULL,FALSE,bInitState,0);
	};

	void	ReCreate()
	{
		if(m_hEvent)
		{
			::CloseHandle(m_hEvent);
		}
		m_hEvent = ::CreateEvent(NULL,FALSE,FALSE,0);
	}

	~XEvent()
	{
		if(m_hEvent)
		{
			::CloseHandle(m_hEvent);
		}
	}

	void	Set()
	{
		if(m_hEvent)
		{
			::SetEvent(m_hEvent);
		}
	}

public:

	HANDLE	m_hEvent;

};
