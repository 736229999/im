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
#ifdef MININETTRANS_EXPORTS
#define MININETTRANS_API __declspec(dllexport)
#else
#define MININETTRANS_API __declspec(dllimport)
#endif

// 此类是从 MiniNetTrans.dll 导出的
#include "./publictype.h"
//#include "fcl.h"
#include "winsock2.h"
//#include "config.h"

#define NE_SENDOK		0
#define NE_RESEND		1
#define NE_RECVDATA		100	// 收到一个数据包
#define NE_SENDFAILED	200	// 发送失败
#define NE_LISTENSTOP	300	// 监听停止
#define NE_ACKTIMEOUT	400	// 应答超时
#define NE_RECVCONGEST	800	// 接收拥塞	

typedef int	XSOCKET;

typedef int NETSOCKET;

#define ERR_SOCKET		-1

//发送失败时 pRecvData 指向SendDataStruct * 结构，为发送失败的原始数据
typedef DWORD (*pfnUserNetCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);


class MININETTRANS_API CUdpNetTrans
{
public:
	/*
	功能描述: 
	必要参数说明：nConsumeThreadCount ： 消费者线程个数
	返 回 值：
	*/ 
	//CUdpNetTrans(int nConsumeThreadCount = 1);

	virtual ~CUdpNetTrans(void){};

	/*
	功能描述: 设置用户回调参数和回调函数
	必要参数说明：pUserKey：用户回调参数
				  pfnCallback：用户回调函数
	返 回 值：
	*/ 
	virtual void SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback) = 0;

	/*
	功能描述: 开始UDP监听服务器
	必要参数说明：addr：IP 地址
				  prot：udp监听端口
	返 回 值：0 成功，< 0 失败
	*/ 
	virtual int StartListen(const char * addr,int port = 0) = 0;
	
	/*
	功能描述: 发送数据
	必要参数说明：sendData ：发送数据结构体对象
	返 回 值：0 成功，< 0 失败
	*/ 
	virtual int SendDataTo(const SendDataStruct &sendData,int nPrior = 0) = 0;

	/*
	功能描述: 停止监听服务
	必要参数说明：
	返 回 值：
	*/ 
	virtual void StopListen() = 0;

	//如果该传输层用于服务器，则用于设置服务器id
	virtual void SetServerID(uint32 id) = 0;

	virtual uint16 GetPort() = 0;

	virtual uint32 GetLocalIP() = 0;

	virtual long GetSendQueueSize() = 0;

	virtual long GetRecvQueueSize() = 0;

	virtual int GetReSendListSize() = 0;

	virtual void GetSendPacketNum(int& nSend,int& nFailed,int& nRecvNum) = 0;
	//virtual void ResetPacketNum() = 0;


	//----------------------以下函数用于流式传输使用-----------------------

	//------------------------------------------------------
	// 创建套接字
	//------------------------------------------------------
	virtual XSOCKET		Socket(uint32 nWndSize,uint32 nSendbufSize) = 0; 
	
	//------------------------------------------------------
	// 开始接受连接,失败返回-1
	//------------------------------------------------------
	virtual XSOCKET		Accept(uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort) = 0;

	//------------------------------------------------------
	// 发送数据（阻塞）
	//------------------------------------------------------
	virtual int			Send(XSOCKET s,const char * pData,int nLen) = 0;

	//------------------------------------------------------
	// 接收数据（阻塞）
	//------------------------------------------------------
	virtual int			Recv(XSOCKET s,char * pBuf,int nLen) = 0;

	//------------------------------------------------------
	// 获取指定链接成功发送的字节数
	//------------------------------------------------------
	virtual int			GetSuccSend(XSOCKET s) = 0;

	//------------------------------------------------------
	// 开始连接, 成功返回0
	//------------------------------------------------------
	virtual int			Connect(
		XSOCKET s,
		uint32 myId,
		uint32 friendId,
		uint32 ip,
		uint16 port,
		uint32 tranIp,
		uint16 tranPort
		) = 0;

	//------------------------------------------------------
	// 关闭套接字
	//------------------------------------------------------
	virtual void		CloseSocket(XSOCKET s) = 0;

	//------------------------------------------------------
	// 关闭连接，并等待数据发送完成
	//------------------------------------------------------
	virtual void		CloseWait(XSOCKET s) = 0;

	/**是否是作为中转服务器使用
	*   @param val:   true-中转服务器，false－客户端使用
	*/
    virtual void        setTransServer(bool val) = 0;

	//过滤重复请求，返回已有应答
	//返回值true--过滤成功，不需要继续处理
	//		false-不能过滤，需要继续处理
	virtual bool FilterRecvData(RecvDataStruct* pRecvData) = 0;

	virtual void AddServerReply(SendDataStruct & sendData) = 0;

	virtual const char *	GetVersion() const = 0;
};

extern "C" MININETTRANS_API CUdpNetTrans* factoryCreateNet(int type = 0, int nConsumeThreadCount = 1, int nQuereCount = 1000, int nBufferSize = 1600);