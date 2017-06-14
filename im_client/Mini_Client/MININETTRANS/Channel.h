#ifndef __NET_CHANNEL__
#define __NET_CHANNEL__
/******************************************************************
** �ļ���:	Channel.h
** ��  Ȩ:	
** ������:	Tony
** ��  ��:	2007-6-4
** ��  ��:  �������Ҫ��UDT�����˼��,�������ӵ��߼�ͨ����

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/


//#include "fcl.h"

#include "winsock2.h"

typedef int		int32;

class CChannel
{
public:

	CChannel();

	CChannel(int version);

	~CChannel();

    /**
	  @param addr:  Ҫʹ�õ�socket��ַ
	  @note:��ָ���ĵ�ַ���а�����
	*/
	int open(const sockaddr_in* addr = NULL);

	/**
	   @note:   �ر�socket���
	*/
	void close();

    /**
	   @note:  ���socket�����ķ��ͻ�������С
	*/
	int getSndBufSize();

    /**
	   @return:  ���socket�����Ľ��ջ�������С
	*/
	int getRcvBufSize();

     /**
	   @note:  ����socket�����ķ��ͻ�������С
	 */
	void setSndBufSize(const int& size);

	/**
	   @note:  ����socket�����Ľ��ջ�������С 
	*/
	void setRcvBufSize(const int& size);

	/**
	   @param addr:   �����������ű�ͨ������ʹ�õ�socket�ĵ�ַ��Ϣ
	*/

	void getSockAddr(sockaddr_in* addr) ;

	/**
	  @param addr:   �����������ñ�ͨ������ʹ�õ�socket�ĶԷ���ַ��Ϣ
	*/

	void getPeerAddr(sockaddr_in* addr) ;

	/**
	   @param addr: ���ݰ����͵�Ŀ�ĵ�ַ
	   @param packet: �����͵����ݰ�
	*/
	int sendto(const sockaddr_in* addr,  char* buf, int32 len) ;

	/**
	   @param addr:    ���յ������ݰ���Դ��ַ
	   @param packet:  �����������ݰ��Ķ���
	*/
	int recvfrom(sockaddr_in* addr, char* buf, int32 len) ;

	/**
	@note: ��ȡ�׽��� 
	*/
	SOCKET getSocket();

private:


	int m_iIPversion;                    // IP version

	SOCKET m_iSocket;

	int m_iSndBufSize;                   // UDP ���ͻ�������С

	int m_iRcvBufSize;                   // UDP ���ջ�������С

	//typedef multiThread   threadMode;

	//threadMode::mutex   m_sndMutex;   //��������ͬ����

	//threadMode::mutex   m_rcvMutex;   //��������ͬ����
};


#endif/*__NET_CHANNEL__*/