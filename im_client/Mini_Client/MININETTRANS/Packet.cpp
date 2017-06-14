/******************************************************************
** �ļ���:	Packet.cpp
** ��  Ȩ:	
** ������:	Tony
** ��  ��:	2007-6-5
** ��  ��:  �������Ҫ���������շ������ݽ��д�����������

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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