#pragma once

/************************************************************************ 
文件描述：全局服务器上业务基类的声明文件 

创建人：hechang
时间：	2006-6-20 
************************************************************************/
#include "Business.h"
#include "AppManager.h"
#include "ErrorCode.h"

class CMiniAppNetwork;

//全局服务类型的业务
class CBusinessExIMSrv : public CBusiness
{
public:
	CBusinessExIMSrv(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck = 0);


	//设置该业务要处理的业务请求包
	//pRowPacket--该业务的请求数据
	//nPacketLength-－请求数据包长度
	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);


	//发送应答包处理
	//返回值:0成功，其它值失败
	virtual int SendReply();

	static void SetNetTrans(CMiniAppNetwork* pAppNetwork) { m_pAppNetwork = pAppNetwork; }

	void  SetErrorReturn(const char* pError);

protected:
	CRecvPacket				m_RecvPacket;		//业务处理的协议请求		
	CSendPacket				m_SendPacket;		//业务处理的协议应答

	XT_HEAD* m_pRecvHead;
	XT_HEAD* m_pSendHead;

	static	CMiniAppNetwork*	m_pAppNetwork;		//网络发送层对象指针

};
