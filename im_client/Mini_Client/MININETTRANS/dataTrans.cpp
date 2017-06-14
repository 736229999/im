/******************************************************************
** �ļ���:	dataTrans.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-7-2 15:38
** ��  ��:  ��NetTrans����ʽ�����һ����װ,�����������ת��ʽ�������ṩͳһ�ӿ�

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

#include "stdafx.h"
#include "dataTrans.h"


CDataTrans::CDataTrans(const uint32 id,CUdpNetTrans* udpNetTrans)
{
	m_selfId = id;
	m_pNetTrans = udpNetTrans;
}

NETSOCKET CDataTrans::socket(uint32 wndSize /* = 50 */)
{
	return m_pNetTrans->socket(wndSize);
}
int CDataTrans::connect(NETSOCKET u,uint32 fid,uint32 friendIp,uint16 friendPort,uint32 nTranIP,uint16 nPort)
{
	int len = sizeof(sockaddr_in);
	int res = SOCKET_ERROR;
	m_peerAddr.sin_family = AF_INET;
	m_peerAddr.sin_addr.S_un.S_addr = friendIp;
	m_peerAddr.sin_port = friendPort;
	//�ȳ���ֱ��p2p��������
	if(friendIp != NULL && friendPort != NULL &&\
		m_pNetTrans->p2pTest(fid,friendIp,friendPort,10000))
	{
		//Trace("p2p Test ok!\n");
		res = m_pNetTrans->connect(u,&m_peerAddr,&len);
		m_connType = CONN_P2P;
	}
	else if(nTranIP != NULL && nPort != NULL)
	{//��������ת��������
		m_peerAddr.sin_addr.S_un.S_addr = nTranIP;
		m_peerAddr.sin_port = nPort;
		if(m_pNetTrans->queryFriend(&m_peerAddr,m_selfId,fid))
			res = m_pNetTrans->connect(u,&m_peerAddr,&len,m_selfId,fid);
		else
		{
			//Error("Login transServer fail!\n");
		}
		m_connType = CONN_SRV_TRANS;
	}
	return res;
}
int CDataTrans::send(const NETSOCKET u, char* buf,const uint32 len)
{
	return m_pNetTrans->send(u,buf,len);
}
int CDataTrans::recv(const NETSOCKET u, char* buf,const int len)
{
	return m_pNetTrans->recv(u,buf,len);
}
NETSOCKET CDataTrans::accept(uint32 fid, uint32 friendIp,uint16 friendPort,uint32 nTranIP,uint16 nPort)
{
	int len = sizeof(sockaddr_in);
	NETSOCKET s = SOCKET_ERROR;
    m_peerAddr.sin_family = AF_INET;
	m_peerAddr.sin_addr.S_un.S_addr = friendIp;
	m_peerAddr.sin_port = friendPort;
	//���ȳ���ֱ��p2p��������
	if(friendIp != NULL && friendPort != NULL &&\
		m_pNetTrans->p2pTest(fid,friendIp,friendPort,10000))
	{
		s = m_pNetTrans->accept(NULL,&m_peerAddr,&len);
		 m_connType = CONN_P2P;
	}
	else if(nTranIP != NULL && nPort != NULL)
	{//��������ת��������
		m_peerAddr.sin_addr.S_un.S_addr = nTranIP;
		m_peerAddr.sin_port = nPort;
		if(m_pNetTrans->queryFriend(&m_peerAddr,m_selfId,fid))
			s = m_pNetTrans->accept(NULL,&m_peerAddr,&len);
	}
	return s;
}
void CDataTrans::close( NETSOCKET u)
{
	return m_pNetTrans->close(u);
}