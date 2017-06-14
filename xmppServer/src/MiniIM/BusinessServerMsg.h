#pragma once
#include "BusinessExIMSrv.h"


//������ת�����Կͻ��˵���Ϣ��ֱ�ӷ���ϵͳ��Ϣ��Ӧ��,
//��ҵ����������������,���ٷ�������
class CBusinessServerMsg :
	public CBusinessExIMSrv
{
public:
	CBusinessServerMsg(CRouterBusiness* pRouter);
	~CBusinessServerMsg(void);

	virtual int DoWork();
	virtual int SendReply();

private:
	void VerifyFriend();
	void DelFriend();
	void FriendStatusChanged();
	void P2PMessage();

	//������ת��Ϣ
	void FamilyMsg();
	void FamilyDismissMsg();
	void FamilyVerifyMsg();
	void FamilyMemJoinMsg();
//	void FamilyApplyMsg();
	void FamilySetMemMsg();


	XT_SERVER_TRANSMIT*  m_pServerMsgReq ;
};


//�ͻ���Ӧ�������֪ͨ
class CBusinessServerMsgAck :
	public CBusinessExIMSrv
{
public:
	CBusinessServerMsgAck(CRouterBusiness* pRouter);
	~CBusinessServerMsgAck(void);

	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);

	virtual int DoWork();
	virtual int SendReply();

private:

};

