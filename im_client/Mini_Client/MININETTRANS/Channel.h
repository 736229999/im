#ifndef __NET_CHANNEL__
#define __NET_CHANNEL__
/******************************************************************
** 文件名:	Channel.h
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-4
** 描  述:  这个类主要用UDT的设计思想,面向连接的逻辑通道类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
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
	  @param addr:  要使用的socket地址
	  @note:在指定的地址进行绑定侦听
	*/
	int open(const sockaddr_in* addr = NULL);

	/**
	   @note:   关闭socket句柄
	*/
	void close();

    /**
	   @note:  获得socket关联的发送缓冲区大小
	*/
	int getSndBufSize();

    /**
	   @return:  获得socket关联的接收缓冲区大小
	*/
	int getRcvBufSize();

     /**
	   @note:  设置socket关联的发送缓冲区大小
	 */
	void setSndBufSize(const int& size);

	/**
	   @note:  设置socket关联的接收缓冲区大小 
	*/
	void setRcvBufSize(const int& size);

	/**
	   @param addr:   输出参数，存放本通道正在使用的socket的地址信息
	*/

	void getSockAddr(sockaddr_in* addr) ;

	/**
	  @param addr:   输出参数，获得本通道正在使用的socket的对方地址信息
	*/

	void getPeerAddr(sockaddr_in* addr) ;

	/**
	   @param addr: 数据包发送的目的地址
	   @param packet: 待发送的数据包
	*/
	int sendto(const sockaddr_in* addr,  char* buf, int32 len) ;

	/**
	   @param addr:    接收到的数据包的源地址
	   @param packet:  用来接收数据包的对象
	*/
	int recvfrom(sockaddr_in* addr, char* buf, int32 len) ;

	/**
	@note: 获取套接字 
	*/
	SOCKET getSocket();

private:


	int m_iIPversion;                    // IP version

	SOCKET m_iSocket;

	int m_iSndBufSize;                   // UDP 发送缓冲区大小

	int m_iRcvBufSize;                   // UDP 接收缓冲区大小

	//typedef multiThread   threadMode;

	//threadMode::mutex   m_sndMutex;   //发送数据同步锁

	//threadMode::mutex   m_rcvMutex;   //接收数据同步锁
};


#endif/*__NET_CHANNEL__*/