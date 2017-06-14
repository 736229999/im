#ifndef _XT_PACKET_H
#define _XT_PACKET_H

#include "impublic.h"
//#include "winsock2.h"

/************************************************************************ 
* 
* �ļ�����xtpacket.h
* 
* �ļ�����: �������ݰ�������,�������ݰ�������
*           ��װ�˶����ݰ����,����Ĳ���
* 
* �����ˣ� ѦС��,2005��8��25��
* 
* �汾�ţ�2.0 
* 
* �޸ļ�¼�� 2.0 ������Э���޸Ĵ��࣬����uint16��uint32���Ͳ�ʵ�������ֽڵ������ֽڵ�ת�� by zouyf 2006-06-13
* 
************************************************************************/ 

class CIMPacket
{
public:		
	virtual int GetSize( ) = 0 ;
	const char* GetData( )   { return m_pData; }
	const char* GetCursor()  { return m_pCursor; }
	const char* SetCursor(int nPos){m_pCursor = m_pData + nPos; return m_pCursor; }
	void Reset()  { m_pCursor = m_pData; }

protected:
	char*	m_pCursor  ;
	char*	m_pData    ;
};

class CRecvPacket : public CIMPacket
{
public:
	CRecvPacket(char* data, int len)
	{
		SetData(data,len);
	}

	CRecvPacket() { ;}			//add by hechang,���ڹ������������ð�����


	void SetData(char* data,int len)
	{
		m_pData		= data;
		m_pCursor	= m_pData;
		m_nDataLen	= len;
	}

public://����������
	CRecvPacket& operator >> (char * str)	;
	CRecvPacket& operator >> (const char *str)	;
	CRecvPacket& operator >> (uint8 &data)		;
	CRecvPacket& operator >> (uint16 &data)		;
	CRecvPacket& operator >> (uint32 &data)		;
	CRecvPacket& operator >> (uint64 &data)		;
	CRecvPacket& operator >> (float &data)		;

public:
	virtual int  GetSize()  { return m_nDataLen; }	
	void  GetXtHead(XT_HEAD &h);
	int   ReadData(char *buff, uint32 buff_len)  ;
	int   ReadPacket(char *buff, uint32 buff_len) ;	
	bool  HasData(int nDatalen) { return m_pData + m_nDataLen - m_pCursor >= nDatalen;}
	int		m_nDataLen;
};

class CSendPacket : public CIMPacket
{
public:
	CSendPacket(char *data ,int nSize = MAX_SEND_BUFFERS)
	{
		SetData(data, nSize);
	}

	CSendPacket() { m_nMaxPacketSize = 0 ; }		//add by hechang,���ڹ������������ð�����

	void SetData(char* data,int nSize = MAX_SEND_BUFFERS )
	{
		m_pData=data;
		m_pCursor = m_pData ;
		m_nMaxPacketSize = nSize;
	}

public://����������
	CSendPacket& operator << (const char *str)	;
	CSendPacket& operator << (uint8 data)		;
	CSendPacket& operator << (uint16 data)		;
	CSendPacket& operator << (uint32 data)		;
	CSendPacket& operator << (uint64 data)		;
	CSendPacket& operator << (float data)		;


public:
	virtual int GetSize() { return (int)(m_pCursor - m_pData); }	
	void SetXtHead(const XT_HEAD &h);
	void WriteData(const char* buf, uint32 buf_len) ;
	void WritePacket(const char* buf, uint32 buf_len);

	bool HasData(int len) { return len<= m_nMaxPacketSize - (m_pCursor-m_pData);}

private:
	int		m_nMaxPacketSize;
};

#endif
