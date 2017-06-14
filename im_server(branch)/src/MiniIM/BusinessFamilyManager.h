#pragma once

#include "BusinessExIMSrv.h"

class CDataBaseWeb;
class CDataBaseMini;
class CDataBaseFamily;
class CGlobalBusi;


class CBusinessFamilyGlobalFunc
{
public:
	CBusinessFamilyGlobalFunc();
	~CBusinessFamilyGlobalFunc();
	int GetOfflineFamilyMsg(CPermFamily *pFamily, CDataBaseMini *pDataBaseMini);
	int SaveBufFamilyMsg(CPermFamily *pFamily, CDataBaseMini *pDataBaseMini);
	int GetFamilyInfo(uint32 nFamilyID,CDataBaseFamily *pDataBaseFamily, CDataBaseMini *pDataBaseMini,CMiniAppNetwork *pAppNetwork,CGlobalBusi *pGlobalFamilyBusi);
	int ReleaseFamilyInfo(uint32 nFamilyID,CMiniAppNetwork *pAppNetwork,CGlobalBusi *pGlobalFamilyBusi,CDataBaseMini* pDataBaseMini);
	int ReleaseTempFamilyInfo(uint32 nFamilyID,CMiniAppNetwork *pAppNetwork,CGlobalBusi *pGlobalFamilyBusi);
	void TransitSetMemberMsg(CGlobalBusi *pGlobalbusi, uint32 recvID, uint8 action, uint32 actionID, uint32 familyID, uint8 familyProp, const char* familyName,uint32 familyMsgID = 0, uint32 nInviteID = 0);
};

class CBusinessGetFamilyList : public CBusinessExIMSrv
{
public:
	CBusinessGetFamilyList(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseMini * m_pDataBaseMini;

};

class CBusinessGetFamilyInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetFamilyInfo(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseMini * m_pDataBaseMini;
};

class CBusinessGetFamilyMem : public CBusinessExIMSrv
{
public:
	CBusinessGetFamilyMem(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseMini * m_pDataBaseMini;
};

class CBusinessSetFamilyInfo : public CBusinessExIMSrv
{
public:
	CBusinessSetFamilyInfo(CRouterBusiness* pRouter);
	void SyncFamilyInfo(CRecvPacket& packet, uint32 uid);

	virtual int DoWork();
private:
	CDataBaseFamily * m_pDataBaseFamily;
};

class CBusinessCreateFamily : public CBusinessExIMSrv
{
public:
	CBusinessCreateFamily(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseWeb * m_pDataBaseWeb;
};

class CBusinessSetFamilyMember : public CBusinessExIMSrv
{
public:
	CBusinessSetFamilyMember(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	//static SafeMap<uint32, uint32> m_CurrentMap; 
	//void TransitSetMemMsg(uint32 recvID, uint8 action, uint32 actionID, uint32 familyID, char* familyName, uint8 familyProp);
	CDataBaseFamily * m_pDataBaseFamily;
};

class CBusinessFamilyApply : public CBusinessExIMSrv
{
public:
	CBusinessFamilyApply(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	//void TransitApplyMsg(uint32 recvID, uint8 action, uint32 actionID, uint32 familyID, uint8 familyProp, char* familyName);

	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseMini * m_pDataBaseMini;
};

class CBusinessFamilyVerify : public CBusinessExIMSrv
{
public:
	CBusinessFamilyVerify(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	void TransitFamilyVerifyMsg(uint32 recvID, uint32 familyID,char* msg,int msgLen);
	//void TransitFamilyMemJoinMsg(uint32 recvID, uint32 familyID, uint32 requestID,char* familyName);
	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseMini * m_pDataBaseMini;
};

class CBusinessFamilyFind : public CBusinessExIMSrv
{
public:
	CBusinessFamilyFind(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	CDataBaseFamily * m_pDataBaseFamily;
};

class CBusinessDismissFamily : public CBusinessExIMSrv
{
public:
	CBusinessDismissFamily(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	void TransitDisbandMsg(uint32 recvID,uint32 senderID, uint32 familyID, uint8 familyProp);
	CDataBaseFamily * m_pDataBaseFamily;
	CDataBaseMini * m_pDataBaseMini;
};

class CBusinessFamilySendMsg : public CBusinessExIMSrv
{
public:
	CBusinessFamilySendMsg(CRouterBusiness* pRouter);

	virtual int DoWork();
private:
	void TransitFamilyMsg(uint32 recvID,uint32 familyID,uint32 msgID,char * msg,int msgLen,uint8 familyProp = PERM_FAMILY, int retrytime = SEND_RETRY_TIME);
	void AddFileBuf(int nUserNum,uint32 nFamilyId);
	CDataBaseMini * m_pDataBaseMini;
};

class CBusinessFamilyNotify : public CBusinessExIMSrv
{
public:
	CBusinessFamilyNotify(CRouterBusiness* pRouter);
	int SendReply();
	virtual int DoWork();

	void SyncFamilyStat(CRecvPacket& packet,uint32 id);
};

class CBusinessFamilyNotifyAck : public CBusinessExIMSrv
{
public:
	CBusinessFamilyNotifyAck(CRouterBusiness* pRouter);

	void SetRowPacket(BYTE* pRowPacket,int nPacketLength);
	int SendReply() { return RESULT_SUCCESS;} 
	virtual int DoWork() { return RESULT_SUCCESS;}
};


class CBusinessFamiliyOfflineMsg : public CBusinessExIMSrv
{
public:
	CBusinessFamiliyOfflineMsg(CRouterBusiness* pRouter);
	~CBusinessFamiliyOfflineMsg();
	int SendReply();
	virtual int DoWork();
	ST_OFFLINE_MSG* m_pFamilyMsgBuf;
};

class CBusinessFamiliyOfflineMsgAck : public CBusinessExIMSrv
{
public:
	CBusinessFamiliyOfflineMsgAck(CRouterBusiness* pRouter);

	void SetRowPacket(BYTE* pRowPacket,int nPacketLength);
	int SendReply() { return RESULT_SUCCESS;} 
	virtual int DoWork() { return RESULT_SUCCESS;}
};

class CBusinessFamiliyGetProfile : public CBusinessExIMSrv
{
public:
	CBusinessFamiliyGetProfile(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	CDataBaseFamily * m_pDataBaseFamily;
};


class CBusinessFamiliyGetProfileByID: public CBusinessExIMSrv
{
public:
	CBusinessFamiliyGetProfileByID(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	CDataBaseFamily * m_pDataBaseFamily;
};


class CBusinessFamiliySetProfile : public CBusinessExIMSrv
{
public:
	CBusinessFamiliySetProfile(CRouterBusiness* pRouter);
	
	void Notify(XT_PROFILENAME_SET_ACK ack );

	virtual int DoWork();

private:
	CDataBaseFamily * m_pDataBaseFamily;
};

class CBusinessFamiliyInviteReq : public CBusinessExIMSrv
{
public:
	CBusinessFamiliyInviteReq(CRouterBusiness* pRouter);
	virtual int DoWork();

	void NotifyOthers();
};

class CBusinessFamiliyAgreeReq : public CBusinessExIMSrv
{
public:
	CBusinessFamiliyAgreeReq(CRouterBusiness* pRouter);
	virtual int DoWork();

	void Notify();
};

class CBusinessGetOtherClusterInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetOtherClusterInfo(CRouterBusiness* pRouter);
	virtual int DoWork();
	virtual int SendReply();
};




