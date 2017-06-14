#include "stdafx.h"
#include "BaseUdpTrans.h"

CBaseUdpTrans::CBaseUdpTrans(void)
{
	m_szIPAddr[0] = '\0' ;
	m_nPort = 0;
}

CBaseUdpTrans::~CBaseUdpTrans(void)
{
}

void CBaseUdpTrans::OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
//	printf("CBaseUdpTrans::OnReceive\n");
	CBaseUdpTrans * pObj = (CBaseUdpTrans *)pUserKey;
	switch(nEvent) 
	{
	case NE_RECVDATA:
		{
			pObj->HandleRecvData(pRecvData);
		}
		break;
	case NE_SENDFAILED:
		break;
	case NE_LISTENSTOP:
		break;
	default:
		;
	}
}

void CBaseUdpTrans::HandleRecvData(const RecvDataStruct * pRecvData)
{
	return ;
}

int CBaseUdpTrans::StartWork(const char * addr,uint16 port)
{
	printf("CBaseUdpTrans::StartWork \n");
	m_nPort = port;
	strcpy_s(m_szIPAddr,addr);
	m_UdpNetTrans.SetUserCallback(this,OnReceive);
	int nRet = m_UdpNetTrans.StartListen(addr,port);
	if(nRet < 0)
		return -1;
	return 0;
}

void CBaseUdpTrans::StopWork()
{
	m_UdpNetTrans.StopListen();
}
