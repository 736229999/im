/************************************************************************ 
 文件描述：udp网络通讯层接口
    实现快速的网络数据传输，采用重叠I/O、完成端口模型，利用线程池、收发队列实现高效的网络数据传输
 
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

// 下列 ifdef 块是创建使从 DLL 导出更简单的
//宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MININETTRANS_EXPORTS
// 符号编译的。在使用此 DLL 的
//任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将 
// MININETTRANS_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
class CGlobalBusi;
#ifdef MININETTRANS_EXPORTS
	#define MININETTRANS_API 
#else
	#define MININETTRANS_API 
#endif

// 此类是从 MiniNetTrans.dll 导出的
#include "PublicType.h"
//#include "../../Common/CPClientContext.h"

#define NE_SENDOK		0
#define NE_RESEND		1
#define NE_RECVDATA   100   //收到一个数据包
#define NE_SENDFAILED 200 //发送失败
#define NE_LISTENSTOP 300 //监听停止
#define NE_ACKTIMEOUT 400 //应答超时

//发送失败时 pRecvData 指向SendDataStruct * 结构，为发送失败的原始数据
typedef void (*pfnUserNetCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
typedef void (*pfnIOCPCallback)(void *pUserKey,int nEvent,char * pRecvData,int nlen);

// 回调函数。当收到网络数据包时供应用层截获，返回TRUE，表示继续将数据投递到排队队列，返回FALSE则不投递。用于接入服务器
typedef BOOL (*FunRecvNetData)(void *pNet, const RecvDataStruct &recv);

// 回调函数。拦截要发送给客户端的数据包，将数据包转给接入服务器
typedef void (*FunSendNetData)(CGlobalBusi *pGlobalBusi, const SendDataStruct &send);


class MININETTRANS_API CUdpNetTrans
{
public:
	/*
	功能描述: 
	必要参数说明：nConsumeThreadCount ： 消费者线程个数
	返 回 值：
	*/ 
	CUdpNetTrans(int nConsumeThreadCount = 1);
	~CUdpNetTrans(void);

	/*
	功能描述: 设置用户回调参数和回调函数
	必要参数说明：pUserKey：用户回调参数
				  pfnCallback：用户回调函数
	返 回 值：
	*/ 
	void SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback);

	void SetUserCallback(FunRecvNetData funRecvNetData);

	/*
	功能描述: 开始UDP监听服务器
	必要参数说明：addr：IP 地址
				  prot：udp监听端口
	返 回 值：0 成功，< 0 失败
	*/ 
	int StartListen(const char * addr,int port = 0);
	
	/*
	功能描述: 发送数据
	必要参数说明：sendData ：发送数据结构体对象
	返 回 值：0 成功，< 0 失败
	*/ 
	int SendDataTo(const SendDataStruct &sendData);

	/*
	功能描述: 停止监听服务
	必要参数说明：
	返 回 值：
	*/ 
	void StopListen();

	//如果该传输层用于服务器，则用于设置服务器id
	void SetServerID(int id);

	uint16 GetPort();
	uint32 GetLocalIP();

	int GetSendQueueSize();
	int GetRecvQueuesize();
	int GetReSendListSize() ;

	void GetSendPacketNum(int& nSend,int& nFailed,int& nRecvNum);
	void ResetPacketNum();


	//过滤重复请求，返回已有应答
	//返回值true--过滤成功，不需要继续处理
	//		false-不能过滤，需要继续处理
	bool FilterRecvData(RecvDataStruct* pRecvData);
	void AddServerReply(SendDataStruct & sendData);

	void * GetUdpComm() {return m_pUdpComm;}

	void StartTcpWork();

private:
	void * m_pUdpComm;
};

class MININETTRANS_API CTcpNetTrans
{
public:
	/*
	功能描述: 
	必要参数说明：nConsumeThreadCount ： 消费者线程个数
	返 回 值：
	*/ 
	CTcpNetTrans(int nConsumeThreadCount = 1);
	~CTcpNetTrans(void);

	/*
	功能描述: 设置用户回调参数和回调函数
	必要参数说明：pUserKey：用户回调参数
	pfnCallback：用户回调函数
	返 回 值：
	*/ 
	void SetUserCallback(pfnIOCPCallback pfnCallback);

	/*
	功能描述: 开始UDP监听服务器
	必要参数说明：addr：IP 地址
	prot：udp监听端口
	返 回 值：0 成功，< 0 失败
	*/ 
	int StartListen(const char * addr,int port = 0);

	/*
	功能描述: 发送数据
	必要参数说明：sendData ：发送数据结构体对象
	返 回 值：0 成功，< 0 失败
	*/ 
	int SendData(void *pContext,char *sendData,int len);

	/*
	功能描述: 停止监听服务
	必要参数说明：
	返 回 值：
	*/ 
	void StopListen();

	void RejectConn(void *pContext);
private:
	void * m_pTcpComm;
};
