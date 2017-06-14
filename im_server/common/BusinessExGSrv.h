
#ifndef BUSINESS_EXGSRV_H
#define  BUSINESS_EXGSRV_H



/************************************************************************ 
文件描述：全局服务器上业务基类的声明文件 

创建人：hechang
时间：	2006-6-20 
************************************************************************/
#include "Business.h"


class CUdpNetTrans;

//全局服务类型的业务
class CBusinessExGSrv : public CBusiness
{
public:
	CBusinessExGSrv(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck = 0);


	//设置该业务要处理的业务请求包
	//pRowPacket--该业务的请求数据
	//nPacketLength-－请求数据包长度
	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);


	//发送应答包处理
	//返回值:0成功，其它值失败
	virtual int SendReply();

	static void SetNetTrans(CUdpNetTrans* pNetTrans) { m_pNetTrans = pNetTrans; }

protected:
	CRecvPacket				m_RecvPacket;		//业务处理的协议请求		
	CSendPacket				m_SendPacket;		//业务处理的协议应答

	static	CUdpNetTrans*	m_pNetTrans;		//网络发送层对象指针

	char					m_globalSessionKey[16];//全局服务器session key

};

#endif

