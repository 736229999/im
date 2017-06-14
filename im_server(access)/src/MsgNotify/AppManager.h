#pragma once
#include "SyncEvent.h"
#include <vector>


class CFIFOPacketBuffer;
class CKafKaConsumer;
class CConfigInfo;
class CThreadResponseKafka;
class CAppManager
{
public:
	~CAppManager(void);

	//��ʼ��
	//����ֵ:true--�ɹ���false--ʧ��
	bool Init();
	CConfigInfo* GetConfig()	{	return m_pConfig;	}

	//�õ�����ʵ��
	static CAppManager& GetInstance();

	void Exit();
	void Stop();

	static bool g_bRun;

	CFIFOPacketBuffer* GetFifoKafkaMsg()	{	return m_pKafKaMsg;	}

private:
	CAppManager(void);

private:
	CConfigInfo*			m_pConfig;				// ������Ϣ
	CFIFOPacketBuffer*		m_pKafKaMsg;			// �յ�������kafka���ݶ������˹ܵ���
	std::vector<CKafKaConsumer*> m_vecpKafkaConsumer;
	CSyncEvent				m_hBlock;				// ����������

	CThreadResponseKafka*	m_pThreadResponseKafka;	// ��Ӧkafka��Ϣ���߳�
};

