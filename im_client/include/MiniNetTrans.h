/************************************************************************ 
 �ļ�������udp����ͨѶ��ӿ�
    ʵ�ֿ��ٵ��������ݴ��䣬�����ص�I/O����ɶ˿�ģ�ͣ������̳߳ء��շ�����ʵ�ָ�Ч���������ݴ���
 
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
************************************************************************/

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� MININETTRANS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// MININETTRANS_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once
#ifdef MININETTRANS_EXPORTS
#define MININETTRANS_API __declspec(dllexport)
#else
#define MININETTRANS_API __declspec(dllimport)
#endif

// �����Ǵ� MiniNetTrans.dll ������
#include "./publictype.h"
//#include "fcl.h"
#include "winsock2.h"
//#include "config.h"

#define NE_SENDOK		0
#define NE_RESEND		1
#define NE_RECVDATA		100	// �յ�һ�����ݰ�
#define NE_SENDFAILED	200	// ����ʧ��
#define NE_LISTENSTOP	300	// ����ֹͣ
#define NE_ACKTIMEOUT	400	// Ӧ��ʱ
#define NE_RECVCONGEST	800	// ����ӵ��	

typedef int	XSOCKET;

typedef int NETSOCKET;

#define ERR_SOCKET		-1

//����ʧ��ʱ pRecvData ָ��SendDataStruct * �ṹ��Ϊ����ʧ�ܵ�ԭʼ����
typedef DWORD (*pfnUserNetCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);


class MININETTRANS_API CUdpNetTrans
{
public:
	/*
	��������: 
	��Ҫ����˵����nConsumeThreadCount �� �������̸߳���
	�� �� ֵ��
	*/ 
	//CUdpNetTrans(int nConsumeThreadCount = 1);

	virtual ~CUdpNetTrans(void){};

	/*
	��������: �����û��ص������ͻص�����
	��Ҫ����˵����pUserKey���û��ص�����
				  pfnCallback���û��ص�����
	�� �� ֵ��
	*/ 
	virtual void SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback) = 0;

	/*
	��������: ��ʼUDP����������
	��Ҫ����˵����addr��IP ��ַ
				  prot��udp�����˿�
	�� �� ֵ��0 �ɹ���< 0 ʧ��
	*/ 
	virtual int StartListen(const char * addr,int port = 0) = 0;
	
	/*
	��������: ��������
	��Ҫ����˵����sendData ���������ݽṹ�����
	�� �� ֵ��0 �ɹ���< 0 ʧ��
	*/ 
	virtual int SendDataTo(const SendDataStruct &sendData,int nPrior = 0) = 0;

	/*
	��������: ֹͣ��������
	��Ҫ����˵����
	�� �� ֵ��
	*/ 
	virtual void StopListen() = 0;

	//����ô�������ڷ����������������÷�����id
	virtual void SetServerID(uint32 id) = 0;

	virtual uint16 GetPort() = 0;

	virtual uint32 GetLocalIP() = 0;

	virtual long GetSendQueueSize() = 0;

	virtual long GetRecvQueueSize() = 0;

	virtual int GetReSendListSize() = 0;

	virtual void GetSendPacketNum(int& nSend,int& nFailed,int& nRecvNum) = 0;
	//virtual void ResetPacketNum() = 0;


	//----------------------���º���������ʽ����ʹ��-----------------------

	//------------------------------------------------------
	// �����׽���
	//------------------------------------------------------
	virtual XSOCKET		Socket(uint32 nWndSize,uint32 nSendbufSize) = 0; 
	
	//------------------------------------------------------
	// ��ʼ��������,ʧ�ܷ���-1
	//------------------------------------------------------
	virtual XSOCKET		Accept(uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort) = 0;

	//------------------------------------------------------
	// �������ݣ�������
	//------------------------------------------------------
	virtual int			Send(XSOCKET s,const char * pData,int nLen) = 0;

	//------------------------------------------------------
	// �������ݣ�������
	//------------------------------------------------------
	virtual int			Recv(XSOCKET s,char * pBuf,int nLen) = 0;

	//------------------------------------------------------
	// ��ȡָ�����ӳɹ����͵��ֽ���
	//------------------------------------------------------
	virtual int			GetSuccSend(XSOCKET s) = 0;

	//------------------------------------------------------
	// ��ʼ����, �ɹ�����0
	//------------------------------------------------------
	virtual int			Connect(
		XSOCKET s,
		uint32 myId,
		uint32 friendId,
		uint32 ip,
		uint16 port,
		uint32 tranIp,
		uint16 tranPort
		) = 0;

	//------------------------------------------------------
	// �ر��׽���
	//------------------------------------------------------
	virtual void		CloseSocket(XSOCKET s) = 0;

	//------------------------------------------------------
	// �ر����ӣ����ȴ����ݷ������
	//------------------------------------------------------
	virtual void		CloseWait(XSOCKET s) = 0;

	/**�Ƿ�����Ϊ��ת������ʹ��
	*   @param val:   true-��ת��������false���ͻ���ʹ��
	*/
    virtual void        setTransServer(bool val) = 0;

	//�����ظ����󣬷�������Ӧ��
	//����ֵtrue--���˳ɹ�������Ҫ��������
	//		false-���ܹ��ˣ���Ҫ��������
	virtual bool FilterRecvData(RecvDataStruct* pRecvData) = 0;

	virtual void AddServerReply(SendDataStruct & sendData) = 0;

	virtual const char *	GetVersion() const = 0;
};

extern "C" MININETTRANS_API CUdpNetTrans* factoryCreateNet(int type = 0, int nConsumeThreadCount = 1, int nQuereCount = 1000, int nBufferSize = 1600);