
/******************************************************************
** �ļ���:	TransProcess.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-7-3 16:45
** ��  ��:  ��Ҫʵ����ת����������ʽ������ת����

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

#ifndef  __TRANS_SERVER_PROCESS
#define  __TRANS_SERVER_PROCESS

#include "winsock2.h"
#include "p2pDef.h"
#include "Channel.h"


#define TRAN_INFO_TIMEOUT	40000		// ��ת�������ϵ��û���Ϣn�����ʱ


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

	RouterInfo							m_tranUsers;		// �ṩ��ת���񱣴���û���Ϣ

};

#endif/*__TRANS_SERVER_PROCESS*/