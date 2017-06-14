#pragma once

#include <map>
using namespace std;

#include "XUDSRecvWnd.h"
#include "XUDSSendWnd.h"

#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")



typedef int XSOCKET;



class XUDSRecvCapCalc
{
public:

	XUDSRecvCapCalc()
	{
		m_nBeg = 0;
		m_nEnd = 0;
		m_AI = 0;
		m_I = 0;
		m_nLastTick = -1;
		memset(m_interHistroy,0,sizeof(m_interHistroy));
	}

public:

	UINT				GetCurInter();

	//------------------------------------------------------
	// tickCount��λΪ΢��,���غ��ϴν��հ�֮���ʱ����
	//------------------------------------------------------
	UINT				AddRecvTick(ULONGLONG tickCount)
	{
		UINT nCount = m_nEnd - m_nBeg;
		UINT inter;
		UINT ai = 0;
		UINT cur = m_nEnd % 16;
		inter = tickCount - m_nLastTick;
		if(nCount == 16)
		{
			// ���㵱ǰ��ʱ����(�����¼������ȥ��ɵ�һ�����)
			m_I += inter;
			m_I -= m_interHistroy[m_nBeg % 16];
			// ���㵱ǰƽ��ֵ
			ai = m_I / 16;
			m_nBeg++;
			m_nEnd++;
			m_nLastTick = tickCount;
		}
		else if(nCount == 0)
		{
			if(m_nLastTick != -1)
			{
				m_nEnd++;
				m_interHistroy[0] = inter;
				m_I += inter;
			}
			m_nLastTick = tickCount;
			return inter;
		}
		else
		{
			m_nEnd++;
			m_I += inter;
			m_interHistroy[cur] = inter;
			m_nLastTick = tickCount;
			return inter;
		}
		// �޳����̫�������
		m_interHistroy[cur] = inter;
		int i;
		UINT ci = 0;
		int nC = 0;
		for(i = 0; i < 16;i++)
		{
			if(m_interHistroy[i] > ai * 8 || m_interHistroy[i] < ai / 8)
				continue;
			ci += m_interHistroy[i];
			nC++;
		}
		if(nC >= 8)
		{
			m_AI = ci / nC;
		}
		else
		{
			m_AI = 0;
		}
		return inter;
	};

	UINT				GetAI()
	{
		return m_AI;
	};

protected:

	ULONGLONG			m_interHistroy[16];

	ULONGLONG			m_nLastTick;

	UINT				m_nBeg;

	UINT				m_nEnd;

	UINT				m_I;				// ��ʱ����

	UINT				m_AI;				// ƽ�����ʱ��
};


class XUDS;


class XUDSocket
{
public:
	
	XUDSocket(XSOCKET xs , int nWndSize ,int nSendBufSize);

	~XUDSocket(void);

	enum STATE{
		INVALID = 0,
		CONNECTED,
		CLOSED,
	};

	friend class XUDS;
	friend class XUDSSendWnd;

public:

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	XSOCKET			GetSocketId()
	{
		return m_xsocket;
	}

	//------------------------------------------------------
	// �ر�����
	//------------------------------------------------------
	void			Close();

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	int				Send(const char * pData,int nLen);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	bool			SendWait();

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	int				Recv(char * pBuf,int nLen);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void			Init(SOCKET s,SOCKADDR_IN addrPeer,uint32 nDestId);

	//------------------------------------------------------
	// ��ȡ�ɹ����͵��ֽ���
	//------------------------------------------------------
	int				GetSuccSend();

	//------------------------------------------------------
	// ��ȡ���ͻ����С
	//------------------------------------------------------
	int				GetSendBufSize()
	{
		return m_sendWnd.GetSendBufSize();
	}

	int				GetSendWndSize()
	{
		return m_sendWnd.GetSendWndSize();
	}

protected:

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void			ProcessData(const XUDS_PACKET & packet);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void			ProcessDataAck(const XUDS_PACKET & packet);

	//------------------------------------------------------
	// ���ߣ�΢�룩
	//------------------------------------------------------
	void			USleep(UINT us)
	{
		UINT ms = us>>10;
		if(ms > 0)
		{
			Sleep(ms);
			us = us - (ms << 10);
		}
		CTimeCount::USleep(us);
	};

	//------------------------------------------------------
	// ��ʱ�ش�
	//------------------------------------------------------
	void			OnTimerResend();
	
	//------------------------------------------------------
	// ӵ������
	//------------------------------------------------------
	void			OnTimerControl();

	//------------------------------------------------------
	// ��ʱ��Ӧ
	//------------------------------------------------------
	void			OnTimer();
	
	//------------------------------------------------------
	// �ط����ݰ�
	//------------------------------------------------------
	void			SendPacket(XUDS_DATA * pData);

	//------------------------------------------------------
	// ����ش�����
	//------------------------------------------------------
	void			AddResend(int nSeq)
	{
		m_scResendList.Lock();
		if(m_lstResend.size() > 100)
		{
			m_lstResend.pop_back();
		}
		m_lstResend.push_back(nSeq);
		
		m_scResendList.Unlock();
	};

	//------------------------------------------------------
	// ��ӿ����ش�����
	//------------------------------------------------------
	void			AddQuickResend(int nSeq)
	{
		m_scResendList.Lock();
		if(m_lstResend.size() > 100)
		{
			m_lstResend.pop_back();
		}
		m_lstResend.push_front(nSeq);
		m_scResendList.Unlock();
	};

	void			SlowDown();


	
#ifdef _MyDebug


	int				DebugGetRecvTimes()
	{ return m_recvWnd.m_nRecvCount; };


	int				DebugGetRepeatRecvTimes()
	{ return m_recvWnd.m_nRepeatCount; } ;


	void			DebugPrintAckInfo(int nCountPerPage)
	{
		cout<<"ack info"<<endl;
		for(int i =0;i < m_sendWnd.m_nAckSeq;i++)
		{
			cout<<i<<":"<<m_sendWnd.m_pAckInfo[i]<<endl;
			if(i % nCountPerPage == (nCountPerPage - 1))
			{
				cout<<"press to continue.."<<endl;
				getchar();
			}
		}
	}

	void			DebugWriteInfo(const char * szFile,int nRecords = -1)
	{
		ofstream ofs(szFile);
		/*ofs<<"resend times = "<<DebugGetResendTimes()<<endl;
		ofs<<"send times = "<< DebugGetSendTimes()<<endl;
		ofs<<"wait times = "<< DebugGetWaitTimes()<<endl;
		ofs<<"RTT = "<< DebugGetRTT()<<endl;
		ofs<<"totoal send count="<<DebugGetResendTimes() + DebugGetSendCount()<<endl;
		ofs<<"quick resend ="<<DebugGetQuickResendTimes()<<endl;*/
		m_sendWnd.WriteInfo(ofs);
		/*int i;
		ofs<<"====ai info===="<<endl;
		for(i = 0;i < m_nAISeq;i++)
		{
			ofs<<i<<"\t"<<m_pAIInfo[i]<<endl;
		}*/
	}

#endif

protected:

	void			SendAck(uint32 seq,uint16 nLen,XUDS_DATA_ACK::RESULT rst);
	static DWORD WINAPI	ThreadSendData(LPVOID lpParam);

	void				SendData();

	ULONGLONG			m_nLastAckTick;

	ULONGLONG			m_nLastSendTick;

protected:

	XSOCKET			m_xsocket;		// ���ӻỰ��

	XUDSSendWnd		m_sendWnd;		// ���ݷ��ʹ���

	XUDSRecvWnd		m_recvWnd;		// ���ݽ��մ���

	STATE			m_state;		// �׽��ֵ�״̬

	UINT			m_SPT;			// ���Ͱ�ʱ������΢�룩

	UINT			m_RPT;			// ���հ�ʱ������΢�룩

	UINT			m_PI;			// 

	double			m_fChannelCap;	// �ŵ�����

	BOOL			m_bQuickStart;	// ������
	
	SOCKADDR_IN		m_addrPeer;		// ���ӷ���ַ

	SOCKET			m_socket;		// �׽���

	uint32			m_nDestId;		// Ŀ���û�id

	//uint32			m_seq;			// �������

	ULONGLONG		m_nLastTick;	// ��ʱ�����ϴ��յ����ݰ�ʱ�����ʱ��

	ULONGLONG		m_nResendTick;	// ��ʱ�����ϴ��ش���tick

	ULONGLONG		m_nControlTick;	// ��ʱ�����ϴ�ӵ�����Ƶ�tick

	XUDSRecvCapCalc	m_RecvCapCalc;	// ����������������

	Common::CXEvent	m_eventConnect;	// �����¼�

	Common::CXEvent	m_eventTranOk;	// ��ת���ӽ����¼� 

	bool			m_bTranServer;	// �Ƿ�ʹ����ת������

	//uint32			m_nRecvPos;

	UINT			m_nTickLastSlow;
	UINT			m_nTickLastAck;
	HANDLE			m_hSend;
	BOOL			m_bSend;
	list<int>		m_lstResend;
	CSyncCritical	m_scResendList;

	bool			HasResend()
	{
		CMyLock lock(m_scResendList.Get());
		return m_lstResend.size() > 0;
	}
#ifdef _MyDebug
	int				m_nWait;
	int				m_nSendCount;
	/*UINT *			m_pAIInfo;
	int				m_nAI;
	int				m_nAISeq;*/
#endif

};


class XSocketMap:public map<XSOCKET,XUDSocket*>
{
public:
	void Lock()
	{
		m_sc.Lock();
	};

	void Unlock()
	{
		m_sc.Unlock();
	};

	CRITICAL_SECTION *	GetCS()
	{
		return m_sc.Get();
	}

	
protected:

	CSyncCritical	m_sc;

};
