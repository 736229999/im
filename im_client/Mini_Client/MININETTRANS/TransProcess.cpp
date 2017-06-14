
/******************************************************************
** �ļ���:	TransProcess.cpp
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-7-3 16:45
** ��  ��:  ��Ҫʵ����ת����������ʽ������ת����

**************************** �޸ļ�¼ ******************************
** �޸���: xy
** ��  ��: 
** ��  ��: 
*******************************************************************/
#include "stdafx.h"
#include "TransProcess.h"


CDataProc::CDataProc()
{
}

bool CDataProc::ProcessTran(CChannel * pChannel,uint32 srcIp,uint16 srcPort,char * buf,int nLen)
{
	if(!pChannel) return false;
	
	XUDS_HEADER *	pUds = (XUDS_HEADER*)buf;
	
	if(pUds->cmd == CMD_QUERY_REQ)
	{
		if(nLen < sizeof(XUDS_QUERY_REQ))
			return false;
		XUDS_QUERY_ACK ack;

		// �����û�����Ϣ

		XUDS_QUERY_REQ * pQuery = (XUDS_QUERY_REQ*)buf;
		TRAN_USER_INFO &info = m_tranUsers[pQuery->myId];
		info.addr.sin_family = AF_INET;
		info.addr.sin_addr.S_un.S_addr = srcIp;
		info.addr.sin_port = srcPort;
		info.tick = GetTickCount();

		printf("user %d logon conn %d\n",pQuery->myId,pQuery->queryId);
	
		// ����ָ���û���Ϣ

		
		RouterInfo::iterator it;
		it = m_tranUsers.find(pQuery->queryId);
		if(it == m_tranUsers.end())
		{
			ack.rt = XUDS_QUERY_ACK::ID_NOT_EXIST;
		}
		else
		{
			if((GetTickCount() - it->second.tick) > TRAN_INFO_TIMEOUT)
			{
				ack.rt = XUDS_QUERY_ACK::INFO_TIMEOUT;
			}
			else
			{
				ack.rt = XUDS_QUERY_ACK::OK;
			}
			ack.ip = it->second.addr.sin_addr.S_un.S_addr;
			ack.port = it->second.addr.sin_port;
			ack.friendId = pQuery->queryId;
			ack.did = pQuery->myId;
			ack.seq = pQuery->seq;
			ack.socketId = pQuery->socketId;
		}

		// ����ack

		sockaddr_in		addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = srcIp;
		addr.sin_port = srcPort;
		pChannel->sendto(&addr,(char*)&ack,sizeof(ack));
	}
	else
	{
		// ������ֱ��ת��
		RouterInfo::iterator it;
		it = m_tranUsers.find(pUds->did);
		//sockaddr_in addr = it->second.addr;

		if(it != m_tranUsers.end())
		{
			//printf("->%d,len=%d,port=%d\n",pUds->did,nLen,ntohs(addr.sin_port));
			pChannel->sendto(&it->second.addr,buf,nLen);
		}
	}
	return true;
}

//
//bool CDataProc::processPacket(CChannel* pChannel, uint32 srcIp, uint16 srcPort,char* buf,int& len)
//{
//	if(!pChannel) return false;
//
//    P2P_HEAD* pHead = (P2P_HEAD*)buf;
//	if(pHead->cmd == CMD_QUERY_REQ)  //ֻ������������֣������Ķ�ת��
//	{
//		ibuffer iBuf(pHead->data,pHead->len);
//		obuffer2048 oBuf;
//		QueryInfo qInfo;
//		uint32 selfId,friendId;
//		iBuf >> selfId >> friendId;
//		Trace("user %d login!\n",selfId);
//		//ע���Լ���fid�͵�ַ
//		FriendInfo& selfInfo = m_dataRouter[selfId];  //���û�У����Զ�����һ��,���ظ�ע��
//        //ע��selfId
//		selfInfo.addr.sin_family = AF_INET;
//		selfInfo.addr.sin_addr.S_un.S_addr = srcIp;
//		selfInfo.addr.sin_port = srcPort;
//		time(&selfInfo.loginTime);  //��ǰ��ʱ��
//		//���Һ��ѵĵ�ַ���ҵ��򷵻ظ���ѯ��
//		if(m_dataRouter.find(friendId) != m_dataRouter.end())
//		{
//			FriendInfo& fInfo = m_dataRouter[friendId];
//			qInfo.selfId = selfId;
//			qInfo.selfIp = srcIp;
//			qInfo.selfPort = srcPort;
//			qInfo.friendId = friendId;
//			qInfo.friendIp = selfInfo.addr.sin_addr.S_un.S_addr;
//			qInfo.friendPort = selfInfo.addr.sin_port;
//			P2P_HEAD head;
//			ZeroMemory(&head,sizeof(head));
//			head.cmd = CMD_QUERY_ACK;
//			head.flag = 0xf4;
//			head.len = sizeof(head) + sizeof(FriendInfo);
//			oBuf << head <<(uint32)selfId << (uint32)friendId << fInfo;
//			pChannel->sendto(&selfInfo.addr,(char*)oBuf.buffer(),(int32)oBuf.size());
//		}
//	}
//	else
//	{//ת������Ӧ�ĵ�ַ
//		if(m_dataRouter.find(pHead->fid) != m_dataRouter.end())
//		{
//			FriendInfo& friendInfo = m_dataRouter[pHead->fid];
//			pChannel->sendto(&friendInfo.addr,buf,len);
//		}
//		else
//		{
//			Error("Not find friend address!,friendId:%d\n",pHead->fid);
//			return false;
//		}
//
//	}
//
//	return true;
//}
