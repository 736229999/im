// MiniNetTrans.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "../../include/MiniNetTrans.h"

#include "./ZUdpCommExt.h"
//#include "./ZTcpComm.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

CUdpNetTrans::CUdpNetTrans(int nConsumeThreadCount /* = 2*/)
{
	m_pUdpComm = new CZUdpCommExt(nConsumeThreadCount);
}

CUdpNetTrans::~CUdpNetTrans(void)
{
	StopListen();
	delete (CZUdpCommExt *)m_pUdpComm;
	m_pUdpComm = NULL;
}

void CUdpNetTrans::SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback)
{
	((CZUdpCommExt *)m_pUdpComm)->SetUserCallback(pUserKey,pfnCallback);
}

void CUdpNetTrans::SetUserCallback(FunRecvNetData funRecvNetData)
{
	((CZUdpCommExt *)m_pUdpComm)->SetUserCallback(funRecvNetData);
}

int CUdpNetTrans::StartListen(const char * addr,int port /*= 0*/)
{
	return ((CZUdpCommExt *)m_pUdpComm)->StartListen(addr,port);
}
void CUdpNetTrans::StartTcpWork()
{
	((CZUdpCommExt *)m_pUdpComm)->StartTcpWork();
}
int CUdpNetTrans::SendDataTo(const SendDataStruct &sendData)
{
	return ((CZUdpCommExt *)m_pUdpComm)->SendDataTo(sendData);
}
void CUdpNetTrans::StopListen()
{
	((CZUdpCommExt *)m_pUdpComm)->StopListen();
}

int CUdpNetTrans::GetSendQueueSize()
{
	return ((CZUdpCommExt *)m_pUdpComm)->GetSendQueueSize();
}

int CUdpNetTrans::GetRecvQueuesize()
{
	return ((CZUdpCommExt *)m_pUdpComm)->GetRecvQueueSize();
}

int CUdpNetTrans::GetReSendListSize()
{
	return ((CZUdpCommExt *)m_pUdpComm)->GetReSendListSize();
}

void CUdpNetTrans::GetSendPacketNum(int& nSend,int& nFailed,int& nRecvNum)
{
	return ((CZUdpCommExt *)m_pUdpComm)->GetSendPacketNum(nSend,nFailed,nRecvNum);
}

uint16 CUdpNetTrans::GetPort() 
{
	return ((CZUdpCommExt *)m_pUdpComm)->GetPort();
}
uint32 CUdpNetTrans::GetLocalIP()
{
	return ((CZUdpCommExt *)m_pUdpComm)->GetLocalIP();
}

void CUdpNetTrans::SetServerID(int id)
{
	((CZUdpCommExt *)m_pUdpComm)->SetServerID(id);
}


bool CUdpNetTrans::FilterRecvData(RecvDataStruct* pRecvData)
{
	return ((CZUdpCommExt *)m_pUdpComm)->FilterRecvData(pRecvData);
}
void CUdpNetTrans::AddServerReply(SendDataStruct & sendData)
{
	((CZUdpCommExt *)m_pUdpComm)->AddServerReply(sendData);
}

void CUdpNetTrans::ResetPacketNum()
{
	((CZUdpCommExt *)m_pUdpComm)->ResetSendPacketNum();
}

/*
CTcpNetTrans::CTcpNetTrans(int nConsumeThreadCount )
{
	m_pTcpComm = new CZTcpComm(nConsumeThreadCount);
}

CTcpNetTrans::~CTcpNetTrans(void)
{
	StopListen();
	delete m_pTcpComm;
	m_pTcpComm = NULL;
}

void CTcpNetTrans::SetUserCallback(pfnIOCPCallback pfnCallback)
{
	((CZTcpComm *)m_pTcpComm)->SetUserCallback(pfnCallback);
}
int CTcpNetTrans::StartListen(const char * addr,int port )
{
	return ((CZTcpComm *)m_pTcpComm)->StartListen(addr,port);
}
int CTcpNetTrans::SendData(void *pContext,char *sendData,int len)
{
	return ((CZTcpComm *)m_pTcpComm)->SendData(pContext,sendData,len);
}
void CTcpNetTrans::StopListen()
{
	((CZTcpComm *)m_pTcpComm)->StopListen();
}

void CTcpNetTrans::RejectConn(void *pContext)
{
	((CZTcpComm *)m_pTcpComm)->RejectConn(pContext);
}
*/




