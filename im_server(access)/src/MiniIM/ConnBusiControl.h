/************************************************************************ 
 �ļ�������̬����ҵ������������ע�ᣬ����ʱ���������״̬�б�
 
 �����ˣ� Zouyf, 2006-6-29 
 
 �޸ļ�¼�� 
            
************************************************************************/
#pragma once
#include "../../include/SrvFrameProtocol.h"
#include "../../common/Utility.h"
#include "BaseUdpTrans.h"

#include <map>
using namespace std;

typedef struct tagSRVSTATUSINFO
{
	tagSrvStatus SrvStatus;
	uint32 liveTime;
	tagSRVSTATUSINFO()
	{
		memset(this,0,sizeof(tagSRVSTATUSINFO));
	}
}SRVSTATUSINFO, * LPSRVSTATUSINFO;

typedef map<uint32,tagSrvStatus *> MAPSRVSTATUS;
typedef pair<uint32,tagSrvStatus *> PAIRSRVSTATUS;

class CConnBusi;
//ʵ��Э��ļӽ��������ݵĽ��շ���
class CConnBusiControl: public CBaseUdpTrans
{
public:
	CConnBusiControl(uint32 connSrvIP,uint16 connSrvPort);
	~CConnBusiControl(void);
	virtual int StartWork(const char * addr,uint16 port = 0);
	virtual void StopWork();

	void SetConnBusi(CConnBusi * pConnBusi)
	{
		m_pConnBusi = pConnBusi;
	}

	inline void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = SERVERID_CONNECT)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = XT_HEAD::FLAG_CNTREQUEST;
		lpHead->from = XT_HEAD::FROM_CLIENT;
		lpHead->len = len;
		lpHead->seq = IncRequestSeq();
		lpHead->sid = m_nManSrvID; 
	}	
	void SendRequestData(SendDataStruct & sendData);

	SmartList<SRVSTATUSINFO> & GetSrvStatusInfoLst(){return m_stLstSrvStatusInfo;}

	SRVSTATUSINFO*  GetBestSrvInfo() { return m_pBestSrvInfo ;	}


private:
	
	friend void  BusiControlMyTimeProc(void* dwUser);
	friend void  BusiControlMyTimeProcNew(union sigval v);
	void HandleTimer();

	void HandleRecvData(const RecvDataStruct * pRecvData);

	//��ʱҵ��
	void TimerBusiRequest();
	void TimerBusiSrvStatusLstReq();

	inline uint32 IncRequestSeq()
	{
		return ++m_nCntREQSeq;
	}


	//ҵ��Ӧ����
	void OnSrvRegisterAck(const LPXT_HEAD lpHead,const char * plainBody);
	void OnStatusListReqAck(const LPXT_HEAD lpHead,const char * plainBody);
private:
	PMMRESULT m_hMMT;
	timer_t m_timer;

	uint8          m_srvType;        //����������
	uint32         m_nManSrvID;       //�Լ��ķ�����ID�������ֹ�����ķ��������
	
	//��������������
	volatile uint32 m_nCntREQSeq;       //������������seq
	uint32         m_connSrvIP;
	uint16         m_connSrvPort;
	char           m_connSessionKey[16];    //���������session key
	char           m_mineSessionKey[16];    //�Լ��� session key

	//ҵ��ʵ����
	CConnBusi     * m_pConnBusi;
	uint32         m_timerStatusReq; //״̬�б��Զ������ʱ��

	//ҵ������
	SmartList<SRVSTATUSINFO> m_stLstSrvStatusInfo; //������״̬�б�
	uint16 m_nextPos;
	SRVSTATUSINFO*			 m_pBestSrvInfo;		//��ǰ����������õķ�����

	const unsigned int m_nTimerDelay = 1000 * 30 * 1;;      		//Delay : 1����
	const unsigned int m_nTimerResolution = 1000 * 30 * 1; 		//Resolution : 1����
};
