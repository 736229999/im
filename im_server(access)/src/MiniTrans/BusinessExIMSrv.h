#pragma once

/************************************************************************ 
�ļ�������ȫ�ַ�������ҵ�����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-20 
************************************************************************/
#include "Business.h"
#include "AppManager.h"
#include "ErrorCode.h"

class CMiniAppNetwork;

//ȫ�ַ������͵�ҵ��
class CBusinessExIMSrv : public CBusiness
{
public:
	CBusinessExIMSrv(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck = 0);


	//���ø�ҵ��Ҫ�����ҵ�������
	//pRowPacket--��ҵ�����������
	//nPacketLength-���������ݰ�����
	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);


	//����Ӧ�������
	//����ֵ:0�ɹ�������ֵʧ��
	virtual int SendReply();

	static void SetNetTrans(CMiniAppNetwork* pAppNetwork) { m_pAppNetwork = pAppNetwork; }

	void  SetErrorReturn(const char* pError);

protected:
	CRecvPacket				m_RecvPacket;		//ҵ�����Э������		
	CSendPacket				m_SendPacket;		//ҵ�����Э��Ӧ��

	XT_HEAD* m_pRecvHead;
	XT_HEAD* m_pSendHead;

	static	CMiniAppNetwork*	m_pAppNetwork;		//���緢�Ͳ����ָ��

};
