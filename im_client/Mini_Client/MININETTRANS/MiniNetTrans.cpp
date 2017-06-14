// MiniNetTrans.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "MiniNetTrans.h"

#include "ZUdpCommExt.h"
//#include "fcl.h"


//class globalTrace
//{
//public:
//	globalTrace()
//	{
//		fclCreateTrace();
//		m_richTrace.create(NULL);
//		fclRegisterTraceSink(&m_richTrace,tlAll);
//	}
//	~globalTrace()
//	{
//		fclUnregisterTraceSink(&m_richTrace);
//
//		fclCloseTrace();
//	}
//private:
//	CTraceInteralRichEdit m_richTrace;//打开独立的窗口实时输出信息
//};


#ifdef _DEBUG
//globalTrace globalT;
#endif

CUdpNetTrans* factoryCreateNet(int type /*= 0*/, int nConsumeThreadCount /*= 1*/, int nQuereCount /*= 1000*/, int nBufferSize /*= 1600*/)
{
	if(type == 0)
	{
		return new CZUdpCommExt(nConsumeThreadCount, nQuereCount, nBufferSize);
	}
	else
	{
		return NULL;
	}
}


//BOOL APIENTRY DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 )
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//    return TRUE;
//}
//
//CUdpNetTrans::CUdpNetTrans(int nConsumeThreadCount /* = 2*/)
//{
//	m_pUdpComm = new CZUdpCommExt(nConsumeThreadCount);
//}
//
//CUdpNetTrans::~CUdpNetTrans(void)
//{
//	StopListen();
//	delete m_pUdpComm;
//	m_pUdpComm = NULL;
//}
//
//void CUdpNetTrans::SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback)
//{
//	((CZUdpCommExt *)m_pUdpComm)->SetUserCallback(pUserKey,pfnCallback);
//}
//int CUdpNetTrans::StartListen(const char * addr,int port /*= 0*/)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->StartListen(addr,port);
//}
//int CUdpNetTrans::SendDataTo(const SendDataStruct &sendData,int nPrior)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->SendDataTo(sendData,nPrior);
//}
//void CUdpNetTrans::StopListen()
//{
//	((CZUdpCommExt *)m_pUdpComm)->StopListen();
//}
//
////long CUdpNetTrans::GetSendQueueSize()
////{
////	return ((CZUdpCommExt *)m_pUdpComm)->GetSendQueueSize();
////}
////
////long CUdpNetTrans::GetRecvQueuesize()
////{
////	return ((CZUdpCommExt *)m_pUdpComm)->GetRecvQueueSize();
////}
//
//int CUdpNetTrans::GetReSendListSize()
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->GetReSendListSize();
//}
//
//void CUdpNetTrans::GetSendPacketNum(int& nSend,int& nFailed,int& nRecvNum)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->GetSendPacketNum(nSend,nFailed,nRecvNum);
//}
//
//uint16 CUdpNetTrans::GetPort() 
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->GetPort();
//}
//uint32 CUdpNetTrans::GetLocalIP()
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->GetLocalIP();
//}
//
//void CUdpNetTrans::SetServerID(uint32 id)
//{
//	((CZUdpCommExt *)m_pUdpComm)->SetServerID(id);
//}
//
//
//bool CUdpNetTrans::FilterRecvData(RecvDataStruct* pRecvData)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->FilterRecvData(pRecvData);
//}
//void CUdpNetTrans::AddServerReply(SendDataStruct & sendData)
//{
//	((CZUdpCommExt *)m_pUdpComm)->AddServerReply(sendData);
//}
//
//void CUdpNetTrans::ResetPacketNum()
//{
//	((CZUdpCommExt *)m_pUdpComm)->ResetSendPacketNum();
//}
//
//long CUdpNetTrans::GetSendQueueSize()
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->GetSendQueueSize();
//}
//
//long CUdpNetTrans::GetRecvQueueSize()
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->GetRecvQueueSize();
//}
//
//SESSION_ID CUdpNetTrans::Accept(uint32 src_id,uint32 my_id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->Accept(src_id,my_id);
//}
//
//void CUdpNetTrans::CloseAccept(uint32 src_id,uint32 my_id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->CloseAccept(src_id,my_id);
//}
//
//SESSION_ID CUdpNetTrans::Connect(uint32 dest_ip,uint32 dest_port,uint32 dest_id,uint32 my_id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->Connect(dest_ip,dest_port,dest_id,my_id);
//}
//
//int	CUdpNetTrans::Send(SESSION_ID id,const char * data,int nLen)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->Send(id,data,nLen);
//}
//
//int CUdpNetTrans::Recv(SESSION_ID id,char * buf,int nLen)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->Recv(id,buf,nLen);
//}
//
//
//
//void CUdpNetTrans::SendEnd(SESSION_ID id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->SendEnd(id);
//}
//
//void CUdpNetTrans::CloseRecv(SESSION_ID id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->CloseRecv(id);
//}
//
//void CUdpNetTrans::CloseSend(SESSION_ID id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->CloseSend(id);
//}
//
//
//void CUdpNetTrans::StopTran(SESSION_ID id)
//{
//	return ((CZUdpCommExt *)m_pUdpComm)->StopTran(id);
//}
//
//void CUdpNetTrans::PrintDebug(SESSION_ID id)
//{
//#ifdef _DEBUG
//	((CZUdpCommExt *)m_pUdpComm)->PrintDebug(id);
//#endif
//}
