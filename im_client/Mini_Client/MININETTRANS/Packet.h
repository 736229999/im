#ifndef __NET_PACKET__
#define __NET_PACKET__

/******************************************************************
** 文件名:	Packet.h
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-5
** 描  述:  这个类主要用用来对收发的数据进行打包，解包处理

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
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
	void operator =(CPacket& pack); //禁止赋值函数使用
	P2P_HEAD*  m_head;
	int32 m_packetLength;
	iovec m_packOverBuf;
};



#endif/*__NET_PACKET__*/