/************************************************************************ 
 �ļ�������Ӧ�������ʵ��
     ��ҵ��������ļ�Ⱥʵ���������͸��·�ɣ�����ά���������ĸ�ͨѶ��·
 
 �����ˣ� zouyf, 2006-6-15
 
 �޸ļ�¼�� 
************************************************************************/
#pragma once
#include "../../common/Utility.h"
#include "../../include/PublicType.h"
#include "../../common/HashTable.h"
#include "../../include/MiniNetTrans.h"
#include "../../include/SrvFrameProtocol.h"
#include "../../include/MiniAppNetwork.h"
#include "../Mininettrans/ZUdpCommExt.h"
#include <vector>

class CGlobalBusi;

//������Ҫ��ȫ�ֲ�ѯĿ��λ�õ���������ȴ�ȫ��Ӧ������ʱ��·��  �����ղ�ѯĿ�ꡱ
typedef struct tagROUTEDATA
{
	SendDataStruct sendData;
	uint32  origin_sip;     //·�а�ԭʼ��Դip
	uint16  origin_sport;   //·�а�ԭʼ��Դport
	uint8 emDestType;
	uint32      destID;
	uint32      queryDataSeq; //��ѯ��seq�������ظ���ѯ
	uint32 joinTime; //�����ʱ��
	tagROUTEDATA()
	{
		memset(this,0,sizeof(tagROUTEDATA));
		joinTime = timeGetTime();
	}
}ROUTEDATA, * LPROUTEDATA;

//���潫Ͷ�ݵ�Ӧ�ò�Ľ��հ�   �����ղ�ѯԴ��
typedef struct tagRECVPOSTDATA
{
	RecvDataStruct recvData;
	uint32         queryDataSeq;
	uint32 joinTime; //�����ʱ��
}RECVPOSTDATA, * LPRECVPOSTDATA;

//��������������͵İ�          �����Ͳ�ѯĿ�ꡱ
typedef struct tagSRVSENDDATA
{
	SendDataStruct sendData;
	uint32         did;
	uint8          dtype;
	uint32         queryDataSeq;
	uint32 joinTime; //�����ʱ��
}SRVSENDDATA, * LPSRVSENDDATA;

class CZAppNetwork
{
public:
	CZAppNetwork(void);
	~CZAppNetwork(void);
	void SetUserCallback(void * pUserKey,pfnAppNetCallback pfnCallback)
	{
		m_pUserKey = pUserKey;
		m_pfnUserCallback = pfnCallback;
	}
	void SetUserCallback(FunRecvNetData funRecvNetData)
	{
		if(m_pUdpSrvNetTrans != NULL)
		{
			m_pUdpSrvNetTrans->SetUserCallback(funRecvNetData);
		}
//#ifdef ACCESS_SERVER		
//		if(ImSvrp2p != NULL)
//		{	
//			ImSvrp2p->SetUserCallback(funRecvNetData);
//		}
//#endif		
	}

	void SetCallback_SendNetData(FunSendNetData pFunSendNetData)
	{
		m_pFunSendNetData = pFunSendNetData;
	}

	int StartWork(const char * addr,uint16 port = 0);

	void StartTcpWork();

//#ifdef ACCESS_SERVER
//	shared_ptr<ONLINE_USER_INFO> FindOnlineUser(CGlobalBusi *pGlobalBusi, uint32 uid);
//	// �û�����hash�б�Ľӿ�
//	// ��ȡ�����û�hash�б��hash��֧��
//	int GetUserHash_Elems();
//	// ��ȡ�����û�hash�б��hash��֧�����������(�������û�idΪ����)
//	void GetUserHash_ElemIDs(int nElemIdex, std::vector<uint32> &vecID);
//#else
	BOOL FindOnlineUser(CGlobalBusi *pGlobalBusi, uint32 uid);
//#endif
#ifdef ACCESS_SERVER	
	int UserLogin(CGlobalBusi *pGlobalBusi, uint32 uid,uint32 uip,uint16 uport,const char * pSessionKey = NULL,const char * pP2PSessionKey = NULL,uint32 lanip = 0,uint16 lanport = 0,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	void UserLogout(uint32 uid);
#endif
	void FamilyLogin(uint32 fid,uint8 ftype = PERM_FAMILY);
	void FamilyLogout(uint32 fid,uint8 ftype = PERM_FAMILY);

	int SendDataToA(CGlobalBusi *pGlobalBusi, const char * data,int size,int retrys = 0,uint16 interval = 0,uint32 ip = 0,uint16 port = 0,uint32 vlink = 0);
	int SendDataToA(CGlobalBusi *pGlobalBusi, SendDataStruct & sendData);
	void StopWork();
	uint16 GetPort() {return m_listenPort;}
	uint32 GetLocalIP() {return m_listenIP;}

	CUdpNetTrans* GetSrvNetTrans() { return m_pUdpSrvNetTrans; }
/*	
	CUdpNetTrans* GetCntNetTrans() { return m_pUdpCntNetTrans; }
*/

	void AddImServer(uint32 srvid,uint32 srvip);
	void DelImServer(uint32 srvid);

	//����û�����
	int UserType(CGlobalBusi *pGlobalBusi, uint32 id);

	void ImSvrP2PRecvData(int nEvent,const RecvDataStruct * pRecvData);

private:
	friend void  ZAppNetworkMyTimeProc(void* dwUser);
	friend void  ZAppNetworkMyTimeProcNew(union sigval v);
	friend void UdpSrvNetTransCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
/*	
	friend void UdpCntNetTransCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
*/	
	friend void ImSvrP2PCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);

	void HandleSrvRecvData(int nEvent,const RecvDataStruct * pRecvData);
	void HandleRecvRouteData(const RecvDataStruct * pRecvData);
/*	
	void HandleCntRecvData(int nEvent,const RecvDataStruct * pRecvData);
*/	
	void HandleOnlineExpire();
/*	�ļ����书����ʱ����	�ﶡ	
	void HandleTransferData(const RecvDataStruct * pRecvData);
*/
	void DirectTrans2User(const RecvDataStruct * pRecvData,const LPONLINE_USER_INFO lpDUser);

	//m_stLstRouteData
/*	
	void InstRouteData(uint32 srvip,RecvDataStruct * pRecvData);
	void InstRouteData(uint32 srvip,SendDataStruct * pSendData);
*/
/*
	void DelayRouteData(RecvDataStruct * pRecvData);
*/	
	void DirectPostData(RecvDataStruct * pRecvData,const LPONLINE_USER_INFO lpSUser = NULL);
/*	
	void SendDelayRouteData(uint32 ackSeq,uint32 srvip);
*/	
	void DirectPostDelayRouteData(uint32 ackSeq);
/*	
	void QueryData(RecvDataStruct * pRecvData,uint8 flag);
*/	
/*
	void DelDelayRouteData(uint32 ackSeq);
	void ReHandleDelayRouteData(uint32 ackSeq);
*/
	//m_stLstRecvPostData
	void PostBufferRecvData(uint32 ackSeq,const LPONLINE_USER_INFO lpSUser = NULL);
/*
	//m_stLstSrvSendData
	void SendBufferSrvSendData(uint32 ackSeq,uint32 srvip = 0);
*/
	void UpdateInfoNotify(const RecvDataStruct * pRecvData);
	//void LocateInfoQuery(uint8 dtype,uint32 did,uint32 seq,uint8 vlink = 0);

	inline void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint32 seq,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = 0xF2;
		lpHead->from = 0x0200;
		lpHead->len = len;
		lpHead->seq = seq;
		lpHead->sid = m_nManSrvID;
	}
	
	inline void InitServerHead(LPXT_HEAD lpHead,uint16 cmd,uint32 seq,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = 0xF2;
		lpHead->from = XT_HEAD::FROM_SERVER;
		lpHead->len = len;
		lpHead->seq = seq;
		lpHead->sid = m_nManSrvID;
	}

	uint32 AddRequestSeq()
	{
		CMyLock MyLock(&m_seqLock);
		return ++m_nCntREQSeq;
	}

	uint8 GetFamilyType(uint32 fid)
	{
		if(fid & 0x80000000)
			return TEMP_FAMILY;
		else
			return PERM_FAMILY;
	}
private:
//#ifdef ACCESS_SERVER	// ֻ�н���������м�¼�����û���Ϣ	
//	CHashTable<ONLINE_USER_INFO> m_hashUser;     //�����û���λ��ϢHash
//#endif	
	CHashTable<ONLINE_FAMILY_INFO> m_hashFamily; //��½Ⱥ��λ��ϢHash
//#ifdef ACCESS_SERVER
//	SafeList<LPONLINE_USER_INFO> m_lstIndirectUser;     //�Զ�ά���ķ�ֱ���û��б���Ϊ���ѡ������Ա�ȷ�ֱ���û�����Ӧ�ò�ά����
//#endif	
	SafeList<LPONLINE_FAMILY_INFO> m_lstIndirectFamily; //�Զ�ά���ķ�ֱ�ӵ�½Ⱥ�б�

	PMMRESULT m_hMMT;
	timer_t m_timer;

	uint32 m_listenIP;
	uint16 m_listenPort;
	//���ܺϲ�Ϊһ���˿ڣ���Ϊ��������udp���������Ǳ�Ӧ�ò���Ƶģ�Ӧ�ò�ҲҪ��������ȫ��
	//�ӹܵķ�����udp��ֻ��͸��·�ɣ����Լ���������в���֪��seq
	CUdpNetTrans * m_pUdpSrvNetTrans;  //������udp����
	
	//��ȫ�ֽ��� 
	uint32         m_globalUserIP;
	uint16         m_globalUserPort;
	uint32         m_globalFamilyIP;
	uint16         m_globalFamilyPort;

	char           m_globalSessionKey[16];  //ȫ�ַ�����session key
	char           m_mineSessionKey[16];    //�Լ��� session key
	unsigned	int         m_nManSrvID;       //�Լ��ķ�����ID�������ֹ�����ķ��������
	
	volatile uint32 m_nCntREQSeq;       //��ȫ���������seq
	CRITICAL_SECTION m_seqLock;         //seq��
	//CUdpNetTrans * m_pUdpCntNetTrans;  //�ͻ���udp��ֻ��ȫ�ֽ�����ΪӦ�������˽��
	
	//Ӧ�ò�ص�����
	void * m_pUserKey;
	pfnAppNetCallback m_pfnUserCallback;

	//������Ҫ��ȫ�ֲ�ѯĿ��λ�õ���������ȴ�ȫ��Ӧ������ʱ��·��,��ʱ���������Ƿ�Ӧ�����
	//��ѯ���Ƶ������˶��ٶ�Ҫ������ˣ������������紫��㳬ʱ�ط����ƣ�һ�μ��100�����
	SmartList<ROUTEDATA>    m_stLstRouteData;
	SmartList<SRVSENDDATA>  m_stLstSrvSendData;   //��������������͵İ�          �����Ͳ�ѯĿ�ꡱ

	//��������հ������ִ���ʽ
	enum EM_METHOD
	{
		MD_POSTAPP,    //Ͷ�ݵ�Ӧ�ò�
		MD_INSTROUTE,  //��ʱ·��
		MD_DELAYROUTE, //��ʱ·�ɣ��Ȼ��棬����ѯ��λ����·�� �� Ͷ�ݵ�Ӧ�ò㴦��
		MD_DIRECT2USER //ֱ��ת�����û�
	};

	
	//��ȫ�ֲ�ѯ�û���Ⱥ������Ϣ������״̬
	enum EM_QUERYSTATUS
	{
		QS_RECVQDEST = 1,  //���ղ�ѯĿ��
		QS_RECVQSOURCE = 2,//���ղ�ѯԴ
		QS_SENDQDEST = 3,  //���Ͳ�ѯĿ��
		QS_SENDQSOURCE  = 4,//���Ͳ�ѯԴ
		QS_TRANSFERSRC = 5,//�ļ�����Դ��ѯ
		QS_TRANSFERDST = 6//�ļ�����Ŀ�Ĳ�ѯ
	};


	RecvDataStruct m_recvDataDirectPostDelayRoute;

	char plainData[MAX_NETWORK_PACKET];

	struct ServerInfo
	{
		uint32 id;
		uint32 ip;
	};

	CHashTable<ServerInfo>		m_ServerList;		//����IM����������Ϣ
//#ifdef ACCESS_SERVER	// ֻ�н�����������õ�����������Ϊ�˽���webim������������ �ﶡ
//	CUdpNetTrans* ImSvrp2p;//ҵ�������֮��ͨ��
//#endif
	const unsigned int m_nTimerDelay = 1000 * 60 * 1;      		//Delay : 1����
	const unsigned int m_nTimerResolution = 1000 * 60 * 1; 		//Resolution : 1����

	FunSendNetData	m_pFunSendNetData;
};
