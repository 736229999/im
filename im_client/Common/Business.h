#pragma once

/************************************************************************ 
�ļ�������ҵ�����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-10 
************************************************************************/
#include "publicType.h"
#include "xtpacket.h"
#include "SrvFrameProtocol.h"
#include "protocol.h"

class CRouterBusiness;


//ҵ���࣬������ҵ���ɸ������������Ǹ������Э���ҵ����

class CBusiness
{
public:
	//���ܺ���
	//pRouter -- ҵ��·����
	//nBusinessID -- ҵ�������ID
	//nBusinessIDAck -- ҵ���Ӧ��ID
	CBusiness(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck = 0);
	virtual ~CBusiness(void);

	//���ø�ҵ��Ҫ�����ҵ�������
	//pRowPacket--��ҵ�����������
	//nPacketLength-���������ݰ�����
	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);


	//��ʼ����
	//����ֵ������
	virtual int DoWork() = 0;

	//����Ӧ�������,��ͬ��ҵ������������⴦������������޸�Ĭ��ʵ��
	//����ֵ:0�ɹ�������ֵʧ��
	virtual int SendReply();

	//����Ӧ����
		//ȡ��Ӧ��
	void CancelReply() { m_bNeedSendReply = false;	}
		//��ѯ�Ƿ���ҪӦ��
	bool IsNeedReply() { return m_bNeedSendReply; 	}


	//����һЩ��Ϣ�Ա����´�����һ��ҵ��
	virtual void Reset();

	static void SetServerID(uint32 id) { m_ServerID = id; 	}

protected:
	CRouterBusiness*		m_pRouter;				//�����ҵ���ҵ��·����

	BYTE*		m_RowPacketBuf;							//ԭʼЭ�������
	int			m_nRowPacketLength;						//ԭʼЭ������ݳ���

	SendDataStruct*	m_pSendData;							//ҵ�����귵�ص����ݰ�

	uint16			m_nBusinessID;							//ҵ�������ID
	uint16			m_nBusinessIDAck;						//ҵ���Ӧ��ID
 
	bool			m_bNeedSendReply;						//��ҵ����ҪӦ���

	static	uint32			m_ServerID;						//��������ID

};

