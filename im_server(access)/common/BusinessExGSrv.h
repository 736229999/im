
#ifndef BUSINESS_EXGSRV_H
#define  BUSINESS_EXGSRV_H



/************************************************************************ 
�ļ�������ȫ�ַ�������ҵ�����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-20 
************************************************************************/
#include "Business.h"


class CUdpNetTrans;

//ȫ�ַ������͵�ҵ��
class CBusinessExGSrv : public CBusiness
{
public:
	CBusinessExGSrv(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck = 0);


	//���ø�ҵ��Ҫ�����ҵ�������
	//pRowPacket--��ҵ�����������
	//nPacketLength-���������ݰ�����
	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);


	//����Ӧ�������
	//����ֵ:0�ɹ�������ֵʧ��
	virtual int SendReply();

	static void SetNetTrans(CUdpNetTrans* pNetTrans) { m_pNetTrans = pNetTrans; }

protected:
	CRecvPacket				m_RecvPacket;		//ҵ�����Э������		
	CSendPacket				m_SendPacket;		//ҵ�����Э��Ӧ��

	static	CUdpNetTrans*	m_pNetTrans;		//���緢�Ͳ����ָ��

	char					m_globalSessionKey[16];//ȫ�ַ�����session key

};

#endif

