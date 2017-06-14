/************************************************************************ 
 文件描述：tcp网络通讯层接口
    实现快速的网络数据传输，完成端口模型，利用线程池、收发队列实现高效的网络数据传输
 
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/


#pragma once
#include "../../include/PublicType.h"
#include "../../common/ZObjectPool.h"
#include "../../common/ZSignalQueue.h"
#include "../../include/MiniNetTrans.h"
#include "ClientContext.h"
#include "Utility.h"

//#include <winsock2.h>

#define WAIT_TIMEOUT_INTERVAL 100

//Op codes for IOCP
#define OP_READ     0
#define OP_WRITE    1

class CZTcpComm
{
public:
	CZTcpComm(int nConsumeThreadCount = 1);
	~CZTcpComm(void);
public:
	void SetUserCallback(pfnIOCPCallback pfnCallback)
	{
		m_pfnUserCallback = pfnCallback;
	}

	int StartListen(const char* addr,int nPort);
	int StopListen();
	int SendData(void *pContext,char *sendData,int len);
	void RejectConn(void *pContext);

protected:
	void ProcessData(void);



private:
	bool InitializeIOCP();
	bool Initialize();
	void CleanUp();
	void DeInitialize();

	void AcceptConnection(SOCKET ListenSocket);
	bool AssociateWithIOCP(CClientContext   *pClientContext);
	static DWORD WINAPI AcceptThread(LPVOID lParam);
	static DWORD WINAPI WorkerThread(LPVOID lpParam);
	void AddToClientList(CClientContext   *pClientContext);
	void RemoveFromClientListAndFreeMemory(CClientContext   *pClientContext);
	void CleanClientList();
	int GetNoOfProcessors();


private:
	SOCKET		m_SendSocket;
	SOCKET		m_ListenSocket;
	WSAEVENT	m_hAcceptEvent;
	HANDLE		m_hAcceptThread;
	HANDLE		m_hShutdownEvent;
	int			m_nThreads;
	HANDLE		*m_phWorkerThreads;
	CRITICAL_SECTION m_csClientList;
	HANDLE m_hIOCompletionPort;
	std::vector<CClientContext *> m_ClientContext;
	pfnIOCPCallback	m_pfnUserCallback;
};






