/************************************************************************ 
 文件描述：态负载业务，向接入服务器注册，并定时请求服务器状态列表
 
 创建人： Zouyf, 2006-6-29 
 
 修改记录： 
            
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
//实现协议的加解密与数据的接收发送
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

	//定时业务
	void TimerBusiRequest();
	void TimerBusiSrvStatusLstReq();

	inline uint32 IncRequestSeq()
	{
		return ++m_nCntREQSeq;
	}


	//业务应答处理
	void OnSrvRegisterAck(const LPXT_HEAD lpHead,const char * plainBody);
	void OnStatusListReqAck(const LPXT_HEAD lpHead,const char * plainBody);
private:
	PMMRESULT m_hMMT;
	timer_t m_timer;

	uint8          m_srvType;        //服务器类型
	uint32         m_nManSrvID;       //自己的服务器ID，本机手工分配的服务器编号
	
	//与接入服务器交互
	volatile uint32 m_nCntREQSeq;       //向接入请求包的seq
	uint32         m_connSrvIP;
	uint16         m_connSrvPort;
	char           m_connSessionKey[16];    //接入服务器session key
	char           m_mineSessionKey[16];    //自己的 session key

	//业务实现类
	CConnBusi     * m_pConnBusi;
	uint32         m_timerStatusReq; //状态列表自动请求计时器

	//业务数据
	SmartList<SRVSTATUSINFO> m_stLstSrvStatusInfo; //服务器状态列表
	uint16 m_nextPos;
	SRVSTATUSINFO*			 m_pBestSrvInfo;		//当前服务性能最好的服务器

	const unsigned int m_nTimerDelay = 1000 * 30 * 1;;      		//Delay : 1分钟
	const unsigned int m_nTimerResolution = 1000 * 30 * 1; 		//Resolution : 1分钟
};
