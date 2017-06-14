#pragma once
#include "../include/MiniNetTrans.h"

class CBaseUdpTrans
{
public:
	CBaseUdpTrans(void);
	virtual ~CBaseUdpTrans(void);
	virtual int StartWork(const char * addr,uint16 port = 0);
	int SendDataTo(const SendDataStruct &sendData)
	{
		return m_UdpNetTrans.SendDataTo(sendData);	
	}
	virtual void StopWork();

	char* GetIPAddr() { return m_szIPAddr; 	}
	uint16 GetPort()  { return m_nPort;		}

	CUdpNetTrans* GetNetTrans() { return &m_UdpNetTrans; 	}

private:
	virtual void HandleRecvData(const RecvDataStruct * pRecvData) = 0;
private:
	static void OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
	CUdpNetTrans  m_UdpNetTrans;		//ÍøÂç´«ËÍ²ã

	char	m_szIPAddr[20];
	uint16	m_nPort;
};
