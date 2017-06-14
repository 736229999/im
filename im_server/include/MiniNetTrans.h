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
class CGlobalBusi;
#ifdef MININETTRANS_EXPORTS
	#define MININETTRANS_API 
#else
	#define MININETTRANS_API 
#endif

// �����Ǵ� MiniNetTrans.dll ������
#include "PublicType.h"
//#include "../../Common/CPClientContext.h"

#define NE_SENDOK		0
#define NE_RESEND		1
#define NE_RECVDATA   100   //�յ�һ�����ݰ�
#define NE_SENDFAILED 200 //����ʧ��
#define NE_LISTENSTOP 300 //����ֹͣ
#define NE_ACKTIMEOUT 400 //Ӧ��ʱ

//����ʧ��ʱ pRecvData ָ��SendDataStruct * �ṹ��Ϊ����ʧ�ܵ�ԭʼ����
typedef void (*pfnUserNetCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
typedef void (*pfnIOCPCallback)(void *pUserKey,int nEvent,char * pRecvData,int nlen);

// �ص����������յ��������ݰ�ʱ��Ӧ�ò�ػ񣬷���TRUE����ʾ����������Ͷ�ݵ��ŶӶ��У�����FALSE��Ͷ�ݡ����ڽ��������
typedef BOOL (*FunRecvNetData)(void *pNet, const RecvDataStruct &recv);

// �ص�����������Ҫ���͸��ͻ��˵����ݰ��������ݰ�ת�����������
typedef void (*FunSendNetData)(CGlobalBusi *pGlobalBusi, const SendDataStruct &send);


class MININETTRANS_API CUdpNetTrans
{
public:
	/*
	��������: 
	��Ҫ����˵����nConsumeThreadCount �� �������̸߳���
	�� �� ֵ��
	*/ 
	CUdpNetTrans(int nConsumeThreadCount = 1);
	~CUdpNetTrans(void);

	/*
	��������: �����û��ص������ͻص�����
	��Ҫ����˵����pUserKey���û��ص�����
				  pfnCallback���û��ص�����
	�� �� ֵ��
	*/ 
	void SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback);

	void SetUserCallback(FunRecvNetData funRecvNetData);

	/*
	��������: ��ʼUDP����������
	��Ҫ����˵����addr��IP ��ַ
				  prot��udp�����˿�
	�� �� ֵ��0 �ɹ���< 0 ʧ��
	*/ 
	int StartListen(const char * addr,int port = 0);
	
	/*
	��������: ��������
	��Ҫ����˵����sendData ���������ݽṹ�����
	�� �� ֵ��0 �ɹ���< 0 ʧ��
	*/ 
	int SendDataTo(const SendDataStruct &sendData);

	/*
	��������: ֹͣ��������
	��Ҫ����˵����
	�� �� ֵ��
	*/ 
	void StopListen();

	//����ô�������ڷ����������������÷�����id
	void SetServerID(int id);

	uint16 GetPort();
	uint32 GetLocalIP();

	int GetSendQueueSize();
	int GetRecvQueuesize();
	int GetReSendListSize() ;

	void GetSendPacketNum(int& nSend,int& nFailed,int& nRecvNum);
	void ResetPacketNum();


	//�����ظ����󣬷�������Ӧ��
	//����ֵtrue--���˳ɹ�������Ҫ��������
	//		false-���ܹ��ˣ���Ҫ��������
	bool FilterRecvData(RecvDataStruct* pRecvData);
	void AddServerReply(SendDataStruct & sendData);

	void * GetUdpComm() {return m_pUdpComm;}

	void StartTcpWork();

private:
	void * m_pUdpComm;
};

class MININETTRANS_API CTcpNetTrans
{
public:
	/*
	��������: 
	��Ҫ����˵����nConsumeThreadCount �� �������̸߳���
	�� �� ֵ��
	*/ 
	CTcpNetTrans(int nConsumeThreadCount = 1);
	~CTcpNetTrans(void);

	/*
	��������: �����û��ص������ͻص�����
	��Ҫ����˵����pUserKey���û��ص�����
	pfnCallback���û��ص�����
	�� �� ֵ��
	*/ 
	void SetUserCallback(pfnIOCPCallback pfnCallback);

	/*
	��������: ��ʼUDP����������
	��Ҫ����˵����addr��IP ��ַ
	prot��udp�����˿�
	�� �� ֵ��0 �ɹ���< 0 ʧ��
	*/ 
	int StartListen(const char * addr,int port = 0);

	/*
	��������: ��������
	��Ҫ����˵����sendData ���������ݽṹ�����
	�� �� ֵ��0 �ɹ���< 0 ʧ��
	*/ 
	int SendData(void *pContext,char *sendData,int len);

	/*
	��������: ֹͣ��������
	��Ҫ����˵����
	�� �� ֵ��
	*/ 
	void StopListen();

	void RejectConn(void *pContext);
private:
	void * m_pTcpComm;
};
