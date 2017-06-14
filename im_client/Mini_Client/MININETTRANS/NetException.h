/******************************************************************
** 文件名:	NetException.cpp
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-4
** 描  述:  这个类主要用UDT的设计思想，UDT面向连接的逻辑通道类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/


#ifndef __NET_EXCEPTION__
#define __NET_EXCEPTION__
#pragma  once

#include "config.h"

class  CNetException
{
public:
	CNetException(int major = 0, int minor = 0, int err = -1);
	CNetException(const CNetException& e);
	virtual ~CNetException();

    /**
	   @return  :  错误信息说明 
	   @note:  返回错误信息说明
	*/
	virtual const char* getErrorMessage();

     /**
	    @note:   返回错误码
	 */
	virtual const int getErrorCode() const;

private:
	int m_iMajor;        // major exception categories

	// 0: correct condition
	// 1: 网络启动异常
	// 2: 网络连接中断
	// 3: 内存异常
	// 4: 文件异常
	// 5: 不支持的方法调用
	// 6+: 未知错误，（没有定义）

	int m_iMinor;        // 错误码

	int m_iErrno;        // 系统返回的错误码

	char m_pcMsg[1024];  // 错误信息缓冲区
};


#endif/*__NET_EXCEPTION__*/