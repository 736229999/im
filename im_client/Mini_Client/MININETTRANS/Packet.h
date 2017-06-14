#ifndef __NET_PACKET__
#define __NET_PACKET__

/******************************************************************
** �ļ���:	Packet.h
** ��  Ȩ:	
** ������:	Tony
** ��  ��:	2007-6-5
** ��  ��:  �������Ҫ���������շ������ݽ��д�����������

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/
#include "fcl.h"
#include "netMisc.h"


struct iovec
{
	int iov_len;
	char* iov_base;
};

class CPacket
{
	friend  class CChannel;
public:
	CPacket();
	CPacket(int32 packType, char* buf,int size);
	int getLength() const;

	void setLength(const int len);
private:
	void operator =(CPacket& pack); //��ֹ��ֵ����ʹ��
	P2P_HEAD*  m_head;
	int32 m_packetLength;
	iovec m_packOverBuf;
};



#endif/*__NET_PACKET__*/