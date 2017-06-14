#pragma once


// CUserIPWnd

class CUserIPWnd : public CStatic
{
	DECLARE_DYNAMIC(CUserIPWnd)

public:
	CUserIPWnd();
	virtual ~CUserIPWnd();
	
	UINT		m_ip;	

	UINT		m_localIp;

	WORD		m_port;

	WORD		m_localPort;

protected:

	DECLARE_MESSAGE_MAP()

};


