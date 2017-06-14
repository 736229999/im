#pragma once

/************************************************************************ 
文件描述：业务基类的声明文件 

创建人：hechang
时间：	2006-6-10 
************************************************************************/
#include "publicType.h"
#include "xtpacket.h"
#include "SrvFrameProtocol.h"
#include "protocol.h"

class CRouterBusiness;


//业务类，各种子业务将由该类派生，他们负责各个协议的业务处理

class CBusiness
{
public:
	//构架函数
	//pRouter -- 业务路由器
	//nBusinessID -- 业务的请求ID
	//nBusinessIDAck -- 业务的应答ID
	CBusiness(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck = 0);
	virtual ~CBusiness(void);

	//设置该业务要处理的业务请求包
	//pRowPacket--该业务的请求数据
	//nPacketLength-－请求数据包长度
	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);


	//开始处理
	//返回值：保留
	virtual int DoWork() = 0;

	//发送应答包处理,不同的业务类如果有特殊处理可以在这里修改默认实现
	//返回值:0成功，其它值失败
	virtual int SendReply();

	//管理应答处理
		//取消应答
	void CancelReply() { m_bNeedSendReply = false;	}
		//查询是否需要应答
	bool IsNeedReply() { return m_bNeedSendReply; 	}


	//重置一些信息以便重新处理下一个业务
	virtual void Reset();

	static void SetServerID(uint32 id) { m_ServerID = id; 	}

protected:
	CRouterBusiness*		m_pRouter;				//管理该业务的业务路由器

	BYTE*		m_RowPacketBuf;							//原始协议包数据
	int			m_nRowPacketLength;						//原始协议包数据长度

	SendDataStruct*	m_pSendData;							//业务处理完返回的数据包

	uint16			m_nBusinessID;							//业务的请求ID
	uint16			m_nBusinessIDAck;						//业务的应答ID
 
	bool			m_bNeedSendReply;						//该业务需要应答包

	static	uint32			m_ServerID;						//服务器的ID

};

