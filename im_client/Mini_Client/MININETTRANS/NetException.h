/******************************************************************
** �ļ���:	NetException.cpp
** ��  Ȩ:	
** ������:	Tony
** ��  ��:	2007-6-4
** ��  ��:  �������Ҫ��UDT�����˼�룬UDT�������ӵ��߼�ͨ����

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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
	   @return  :  ������Ϣ˵�� 
	   @note:  ���ش�����Ϣ˵��
	*/
	virtual const char* getErrorMessage();

     /**
	    @note:   ���ش�����
	 */
	virtual const int getErrorCode() const;

private:
	int m_iMajor;        // major exception categories

	// 0: correct condition
	// 1: ���������쳣
	// 2: ���������ж�
	// 3: �ڴ��쳣
	// 4: �ļ��쳣
	// 5: ��֧�ֵķ�������
	// 6+: δ֪���󣬣�û�ж��壩

	int m_iMinor;        // ������

	int m_iErrno;        // ϵͳ���صĴ�����

	char m_pcMsg[1024];  // ������Ϣ������
};


#endif/*__NET_EXCEPTION__*/