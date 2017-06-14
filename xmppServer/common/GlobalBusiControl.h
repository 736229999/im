/************************************************************************ 
 文件描述：全局客户端业务控制类
 
 创建人： Zouyf, 2006-7-7 
 
 修改记录： 
            
************************************************************************/
#pragma once
#include "BaseUdpTrans.h"
#include "SrvFrameProtocol.h"
#include "PublicType.h"
#include <map>
using namespace std;

class CGlobalBusi;
class GlobalBusiControl : public CBaseUdpTrans
{
public:
	GlobalBusiControl(uint32 globalSrvIP,uint16 globalSrvPort);
	virtual ~GlobalBusiControl(void);
	
	/*
	static GlobalBusiControl & GetInstance()
	{
		static GlobalBusiControl obj;
		return obj;
	}*/

	uint32 AllocVLink() {return ++m_vLink;}
	void   RegisterLink(uint32 vlink,CGlobalBusi * pGlobalBusi);

	void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = 0xF2;
		lpHead->from = 2;
		lpHead->len = len;
		lpHead->seq = ++m_seq;
		lpHead->sid = m_nManSrvID;
	}

	void SendRequestData(SendDataStruct & sendData);
private:
	virtual void HandleRecvData(const RecvDataStruct * pRecvData);

private:
	uint32 m_vLink;
	map<uint32,CGlobalBusi *> m_mapGlobalBusi;
	volatile uint32 m_seq;
	uint32 m_nManSrvID; // myself id

	//全局服务器信息
	char m_globalSessionKey[16];
	uint32 m_globalSrvIP;
	uint16 m_globalSrvPort;

	char  m_mineSessionKey[16];    //自己的 session key
};

