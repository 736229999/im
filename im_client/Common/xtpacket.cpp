#include "stdafx.h"
#include "xtpacket.h"

#pragma comment(lib , "ws2_32.lib")

CSendPacket& CSendPacket::operator << (const char *str)
{
	if (str)
	{
		uint16 len = (uint16)strlen(str) + 1;
		if (HasData(len + sizeof(len)))
		{
			operator << (len);
			strcpy(m_pCursor, str); // copy include null terminator
			m_pCursor += len;
		}
	}

	return (*this ) ;
}

CSendPacket& CSendPacket::operator << (uint8 data)
{
	if (HasData(sizeof(data)))
	{
		*(uint8 *)m_pCursor = data;
		m_pCursor += sizeof(data);
	}

	return (*this);
}

CSendPacket& CSendPacket::operator << (uint16 data)
{
	if (HasData(sizeof(data)))
	{
		*(uint16 *)m_pCursor = data;
		m_pCursor += sizeof(data);
	}

	return (*this);
}


CSendPacket& CSendPacket::operator << (uint32 data)
{
	if (HasData(sizeof(data)))
	{
		*(uint32 *) m_pCursor = data;
		m_pCursor += sizeof(data);
	}

	return (*this ) ;
}

CSendPacket& CSendPacket::operator << (uint64 data)
{
	if (HasData(sizeof(data)))
	{
		*(uint64 *) m_pCursor = data ;
		m_pCursor += sizeof(data);
	}

	return (*this ) ;
}

void CSendPacket::WriteData(const char* buf,  uint32 n)
{
	if (HasData(n + sizeof(n)))
	{
		operator << (n);
		memcpy(m_pCursor, buf, n);
		m_pCursor += n;
	}
}

void CSendPacket::WritePacket(const char* buf, uint32 n)
{
	if(HasData(n))
	{
		memcpy(m_pCursor, buf, n);
		m_pCursor += n;
	}
}
 

void CSendPacket::SetXtHead(const XT_HEAD &h)
{
	(*this)<<h.flag<<h.from<<h.cmd<<h.seq<<h.sid<<h.dtype<<h.did<<h.len;
}

CRecvPacket& CRecvPacket::operator >> (char * str)
{
	uint16 len;
	operator >> (len);
	if (len > 0 && HasData(len) && !m_pCursor[len - 1])
	{
		memcpy(str,m_pCursor,len);
		m_pCursor += len;
	}

	return (*this);
}

CRecvPacket& CRecvPacket::operator >> (const char * str)
{ 
	char * p=(char *)str;

	return (*this)>>p;
}

CRecvPacket& CRecvPacket::operator >> (uint8 &data)
{
	if (HasData(sizeof(data)))
	{
		data = *(uint8 *) m_pCursor;
		m_pCursor += sizeof(data);
	}
	else
	{
		data = 0;
	}

	return (*this);
}

CRecvPacket& CRecvPacket::operator >> (uint16 &data)
{
	if (HasData(sizeof(data)))
	{
		data = *(uint16 *) m_pCursor;
		m_pCursor += sizeof(data);
	}	
	else
	{
		data = 0;
	}

	return (*this);
}

CRecvPacket& CRecvPacket::operator >> (uint32 &data)
{
	if (HasData(sizeof(data)))
	{
		data = *(uint32 *) m_pCursor;
		m_pCursor += sizeof(data);
	}
	else
	{
		data = 0;
	}

	return (*this);
}

CRecvPacket& CRecvPacket::operator >> (uint64 &data)
{
	if (HasData(sizeof(data))) 
	{
		data = *(uint64 *) m_pCursor;
		m_pCursor += sizeof(data);
	}
	else
	{
		data = 0;
	}

	return (*this);
}

int CRecvPacket::ReadData(char *buff, uint32 buff_len) 
{
	uint32 n; 
	operator >> (n) ; //数据长度

	int ret = buff_len>n ? n : buff_len;

	if (n > 0 && HasData(n)) 
	{
		memcpy(buff,m_pCursor,ret);
		m_pCursor += ret;		
	}

	return ret;
}


int CRecvPacket::ReadPacket(char *buff, uint32 buff_len) 
{
	uint32 n; 
	int ret = n = buff_len;

	if (n > 0 && HasData(n)) 
	{
		memcpy(buff,m_pCursor,ret);
		m_pCursor += ret;		
	}
	return ret;
}

void CRecvPacket::GetXtHead(XT_HEAD &h)
{
	(*this)>>h.flag>>h.from>>h.cmd>>h.seq>>h.sid>>h.dtype>>h.did>>h.len;
}

