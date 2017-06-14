#pragma once
#include "impublic.h"
#include "BasicSocket.h"

class CUdpComm
{
public:
	CUdpComm(void);
	~CUdpComm(void);

public:
	static UINT RecvProc(LPVOID param);

public:
	bool StartListen();
	bool StopListen();

	int  SendUdpData(const SendDataStruct &sendData);
	void SetRecvList(FIFO2<RecvDataStruct> *pList);
	inline WORD  GetPort() { return m_wPort; }
	inline DWORD GetLocalIP()  {
		return m_dwLocalIP;
	}

private:
	CBasicSocket m_udpSocket;

	FIFO2<RecvDataStruct> * m_pRecvList;

	WORD	m_wPort;
	DWORD   m_dwLocalIP;

	CWinThread *m_pThreadRecv;

	bool m_bStopListen;
};
