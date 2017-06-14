
/******************************************************************
** 文件名:	TransProcess.h
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-7-3 16:45
** 描  述:  主要实现中转服务器的流式数据中转服务

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#ifndef  __TRANS_SERVER_PROCESS
#define  __TRANS_SERVER_PROCESS

#include "winsock2.h"
#include "p2pDef.h"
#include "Channel.h"


#define TRAN_INFO_TIMEOUT	40000		// 中转服务器上的用户信息n毫秒过时


class CDataProc
{
	struct TRAN_USER_INFO
	{
		TRAN_USER_INFO()
		{
			memset(this,0,sizeof(TRAN_USER_INFO));
		}
		sockaddr_in	addr;
		uint32		tick;
	};

public:

    CDataProc();

	bool ProcessTran(CChannel * pChannel,uint32 srcIp,uint16 srcPort,char * buf,int  nLen);

private:

	typedef map<uint32,TRAN_USER_INFO>	RouterInfo;

	RouterInfo							m_tranUsers;		// 提供中转服务保存的用户信息

};

#endif/*__TRANS_SERVER_PROCESS*/