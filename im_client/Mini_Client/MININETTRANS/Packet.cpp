/******************************************************************
** 文件名:	Packet.cpp
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-5
** 描  述:  这个类主要用用来对收发的数据进行打包，解包处理

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/
#include "stdafx.h"
#include "Packet.h"

CPacket::CPacket()
{
	m_packOverBuf.iov_base = (char*)&m_head;
	m_packOverBuf.iov_len = m_packetLength;
}
CPacket::CPacket(int32 packType, char* buf,int size)
{
	m_head = (P2P_HEAD*)buf;
	m_packetLength = size;
	m_packOverBuf.iov_base = (char*)&m_head;
	m_packOverBuf.iov_len = m_packetLength;
}
CPacket::getLength()const
{
	return m_packetLength;
}

void CPacket::setLength(const int len)
{
	m_packetLength = len;
}