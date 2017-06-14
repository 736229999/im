#include "ThreadMsgNotify.h"
#include "AppManager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include <getopt.h>
#include <unistd.h>
#include "Log.h"

#include "StdPublic.h"
#include "PublicType.h"
#include "PacketBuffer.h"
#include "Function.h"

#include "../DataBaseLib/MysqlLib.h"
#include "UserManager.h"

using namespace std;

// 消息记录内存
std::map<std::string, StruMsgNotifyData> CThreadMsgNotify::g_mapMsgNotify;
CSyncCritical CThreadMsgNotify::m_lockMsgNotify;

CThreadMsgNotify::CThreadMsgNotify()
{
    m_pGlobalBusi = NULL;
    m_pMemCache = NULL;
}

CThreadMsgNotify::~CThreadMsgNotify()
{
    if(m_pGlobalBusi != NULL)
    {
        delete m_pGlobalBusi;
        m_pGlobalBusi = NULL;
    }
    if(m_pMemCache != NULL)
    {
        delete m_pMemCache;
        m_pMemCache = NULL;
    }
    m_DatabaseMini.DisConnect();
}

int CThreadMsgNotify::StartWork()
{
    char *memcached_addr = CAppManager::GetInstance().GetConfig().GetMemcachedAddr();
    int memcached_port = CAppManager::GetInstance().GetConfig().GetMemcachedPort();

    m_pMemCache = new ImMemCache();
    if(m_pMemCache->init_ImMemcached(memcached_addr,memcached_port) < 0)
    {
        CLog::Log("CThreadMsgNotify",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
        return -1;
    }

    //初始化与全局服务器的连接
#ifdef IM_USE_CACHE
    m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),m_pMemCache);
#else
    m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
#endif

    //初始化数据库
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    CAppManager::GetInstance().GetConfig().GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);

    CLog::Log("CThreadMsgNotify::StartWork",CLog::TYPE_DEBUG,"DB miniuid:%s minipwd:%s minisid:%s", miniuid, minipwd, minisid);
    m_DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
    if( !m_DatabaseMini.ConnectDB() )
    {
        CLog::Log("CThreadMsgNotify::StartWork",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return -1;
    }

    // 创建线程
    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        Proc,                       // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

    return 0;
}

void* CThreadMsgNotify::Proc( LPVOID lpParam )
{
    CThreadMsgNotify* pThis = (CThreadMsgNotify*)lpParam;
    assert(pThis != NULL);

	if(CAppManager::GetInstance().GetFifoMsgNotify() == NULL)
	{
		CLog::Log("CThreadMsgNotify::Proc", CLog::TYPE_ERROR, "消息推送buf管道为空");
		return NULL;
	}

    while(true)
    {
        char buffer[MSGNOTIFY_BUF_LENGTH] = {0};
        int nLen;
        CAppManager::GetInstance().GetFifoMsgNotify()->Pop((BYTE*)buffer, nLen);
        CLog::Log("CThreadMsgNotify", CLog::TYPE_DEBUG, "MsgBuffer %u",CAppManager::GetInstance().GetFifoMsgNotify()->GetSize());

        MsgPushInd ind;
        if(ind.ParseFromArray(buffer, nLen))
        {
            CLog::Log("CThreadMsgNotify", CLog::TYPE_IMPORTANT, "消息推送包解包成功");
            switch (ind.mptype())
            {
                case MP_NEWORDER:
                {
                    pThis->RecvNewOrder(ind.mpdata().c_str(), ind.mpdata().length());
                }
                break;
                case MP_PAYORDER:
                {
                    pThis->RecvPayOrder(ind.mpdata().c_str(), ind.mpdata().length());
                }
                break;
                case MP_GOODSCOMMENT:
                {
                    pThis->RecvGoodsComment(ind.mpdata().c_str(), ind.mpdata().length());
                }
                break;
                case MP_RETURNORDER:
                {
                    pThis->RecvReturnOrder(ind.mpdata().c_str(), ind.mpdata().length());
                }
                break;
                default:
                    break;
            }
        }
        else
        {
            CLog::Log("CThreadMsgNotify", CLog::TYPE_ERROR, "消息推送包解包失败");
        }
    }

    return NULL;
}

// 收到新订单
void CThreadMsgNotify::RecvNewOrder(const char *pData, int nLength)
{
    NewOrder neworder;
    if(neworder.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvNewOrder", CLog::TYPE_IMPORTANT, "新订单包解包成功 length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvNewOrder", CLog::TYPE_ERROR, "新订单包解包失败");
        return;
    }

    // 得到商家所有客服
    vector<uint32> arrRecentID;
    GetNotifySellerID(neworder.memguid().c_str(), atoi(neworder.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_IMPORTANT, "没有[%s]子账号在此服务器", neworder.merchantid().c_str());
        return;
    }

    // 生成uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_IMPORTANT, "新消息 UUID:%s, datalength:%d", szUUID, nLength);

    // 将新消息数据写入数据库
    string strBuffer;
    strBuffer.assign(pData, nLength);

	// 不存数据库，改存内存
    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_NEWORDER))
    {
        // 插入失败
        CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_ERROR, "新条目写入内存失败");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_DEBUG, "新条目写入内存成功");
    }

    SendToOnlineSeller(arrRecentID, szUUID);
}

// 收到已付款订单
void CThreadMsgNotify::RecvPayOrder(const char *pData, int nLength)
{
    PayOrder payorder;
    if(payorder.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvPayOrder", CLog::TYPE_IMPORTANT, "已付款订单包解包成功 length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvPayOrder", CLog::TYPE_ERROR, "已付款订单包解包失败");
        return;
    }

    // 得到商家所有客服
    vector<uint32> arrRecentID;
    GetNotifySellerID(payorder.memguid().c_str(), atoi(payorder.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_IMPORTANT, "没有[%s]子账号在此服务器", payorder.merchantid().c_str());
        return;
    }

    // 生成uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_IMPORTANT, "新消息 UUID:%s, datalength:%d", szUUID, nLength);

    // 将新消息数据写入数据库
    string strBuffer;
    strBuffer.assign(pData, nLength);

    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_PAYORDER))
    {
        // 插入失败
        CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_ERROR, "新条目写入内存失败");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_DEBUG, "新条目写入内存成功");
    }

    SendToOnlineSeller(arrRecentID, szUUID);
}

// 收到商品评价
void CThreadMsgNotify::RecvGoodsComment(const char *pData, int nLength)
{
    GoodsComment goodscomment;
    if(goodscomment.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvGoodsComment", CLog::TYPE_IMPORTANT, "商品评价包解包成功 length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvGoodsComment", CLog::TYPE_ERROR, "商品评价包解包失败");
        return;
    }

    // 得到商家所有客服
    vector<uint32> arrRecentID;
    GetNotifySellerID(goodscomment.memguid().c_str(), atoi(goodscomment.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_IMPORTANT, "没有[%s]子账号在此服务器", goodscomment.merchantid().c_str());
        return;
    }

    // 生成uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_IMPORTANT, "新消息 UUID:%s, datalength:%d", szUUID, nLength);

    // 将新消息数据写入数据库
    string strBuffer;
    strBuffer.assign(pData, nLength);

    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_GOODSCOMMENT))
    {
        // 插入失败
        CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_ERROR, "新条目写入内存失败");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_DEBUG, "新条目写入内存成功");
    }

    SendToOnlineSeller(arrRecentID, szUUID);
}

// 收到退货订单
void CThreadMsgNotify::RecvReturnOrder(const char *pData, int nLength)
{
    ReturnOrder returnorder;
    if(returnorder.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvReturnOrder", CLog::TYPE_IMPORTANT, "退货订单包解包成功 length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvReturnOrder", CLog::TYPE_ERROR, "退货订单包解包失败");
        return;
    }

    // 得到商家所有客服
    vector<uint32> arrRecentID;
    GetNotifySellerID(returnorder.memguid().c_str(), atoi(returnorder.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_IMPORTANT, "没有[%s]子账号在此服务器", returnorder.merchantid().c_str());
        return;
    }

    // 生成uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_IMPORTANT, "新消息 UUID:%s, datalength:%d", szUUID, nLength);

    // 将新消息数据写入数据库
    string strBuffer;
    strBuffer.assign(pData, nLength);

    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_RETURNORDER))
    {
        // 插入失败
        CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_ERROR, "新条目写入内存失败");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_DEBUG, "新条目写入内存成功");
    }
    SendToOnlineSeller(arrRecentID, szUUID);
}

void CThreadMsgNotify::SendToSeller(CGlobalBusi *pGlobalBusi, CDataBaseMini *pDB, uint32 did, const Buf_ClientSend &ack)
{
    // 在数据库中查找数据
    string buffer;
    EuMsgPush buftype;
    if(!SearchMsgNotify(ack.bufunitidx().c_str(), buffer, buftype))
    {
        // 插入失败
        CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_ERROR, "查询消息推送条目失败");
        return;
    }

    // 消息订阅配置过滤
    MsgRemindAck remind;
    pDB->GetMsgRemind(did, remind);
    bool issend = true;
    switch(buftype)
    {
        case MP_NEWORDER:
            if(remind.neworder() == RT_OFF) issend = false;
            break;
        case MP_PAYORDER:
            if(remind.payorder() == RT_OFF) issend = false;
            break;
        case MP_RETURNORDER:
            if(remind.returnorder() == RT_OFF)  issend = false;
            break;
        case MP_GOODSCOMMENT:
            if(remind.goodscomment() == RT_OFF) issend = false;
            break;
        default:
            issend = false;
            break;
    }
    if(!issend)
    {
        CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_ERROR, "消息类型%d配置为不推送给客户端", buftype);
        DeleteMsgNotify(ack.bufunitidx().c_str(),did);
        return;
    }

    MsgPushInd ind;
    ind.set_mptype(EuMsgPush(buftype));
    ind.set_mpdata(buffer.c_str(), buffer.length());
    int msg_size = ind.ByteSize();
    char *szData = new char[msg_size];
    ind.SerializeToArray(szData, msg_size);
    CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_DEBUG, "MsgPushInd buf length:%d", msg_size);

    // 判断包的长度，如果大于800，就分包发
    int nPacketLength = 800;
    int nPacketCount = msg_size/nPacketLength + (msg_size%nPacketLength > 0 ? 1 : 0);

    Buf_ServerSend send;
    int idxpacket = ack.idxpacketreq();
    if(idxpacket == nPacketCount - 1 && msg_size%nPacketLength > 0)
    {
        nPacketLength = msg_size%nPacketLength;
    }
    CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_DEBUG, "MsgPushInd buf packetcount:%d, idxpacketack:%d, packetlength:%d", nPacketCount, idxpacket, nPacketLength);
    send.set_packetcount(nPacketCount);
    send.set_idxpacket(idxpacket);
    send.set_bufunitidx(ack.bufunitidx().c_str());
    send.set_buflocation(LOCATION_DB);
    send.set_packetdata(szData + idxpacket*800, nPacketLength);
    delete[] szData;
    msg_size = send.ByteSize();
    szData = new char[msg_size];
    send.SerializeToArray(szData, msg_size);
    CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_DEBUG, "Buf_ServerSend buf length:%d", msg_size);

    // 构造转发消息结构体
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + msg_size,
        did);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_SEND_BUF_IND;

    BYTE* indbuf = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) ) ;
    memcpy(indbuf, szData, msg_size);

    delete[] szData;

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + msg_size;
    CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_ERROR,"向客服%d发送系统消息 Buf_ServerSend",did);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
        sendData.data,sendData.data_len,SEND_RETRY_TIME);

    if(idxpacket == nPacketCount - 1)
    {
        // 发送完成后，删除数据库中的记录
        CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_DEBUG, "消息已推送，删除数据:uuid:%s, did:%u", ack.bufunitidx().c_str(), did);
        DeleteMsgNotify(ack.bufunitidx().c_str(), did);
    }
}

// 获取需要通告的商家ID[同时只获取在线的]
void CThreadMsgNotify::GetNotifySellerID(const char *pGuid, int merchantID, vector<uint32> &vecSellerID)
{
	CUserManager userManager(m_pGlobalBusi);
    vecSellerID.clear();

    CLog::Log("CThreadMsgNotify::GetNotifySellerID", CLog::TYPE_IMPORTANT, "merchant id:%d", merchantID);
    // 所有对应的子账号
    AccountArray ids;
    MYSQL_LIB->getAllSubAccount(merchantID, ids);
    // 获取主管账号
    uint32_t mainaccount;
    MYSQL_LIB->getMainAccount(merchantID, mainaccount);
    // 获取最近联系商家
    uint32 starttime = time(0) - 7*24*60*60;
    vector<uint32> arrRecentID;
    m_DatabaseMini.GetRecentContact(pGuid,starttime,arrRecentID);

	shared_ptr<CUser> pUserMain = userManager.Find(mainaccount);
	if(pUserMain != NULL && (int)(pUserMain->m_nServerIp) == CAppManager::GetInstance().GetConfig().GetDeviceIp())
    {
    	vecSellerID.push_back(mainaccount);
    }
    for(int i = 0; i < (int)ids.size(); ++i)
    {
        uint32 subID = ids[i];
        auto itRecent = std::find(arrRecentID.begin(), arrRecentID.end(), subID);
        auto itIsExist = std::find(vecSellerID.begin(), vecSellerID.end(), subID);

		shared_ptr<CUser> pUser = userManager.Find(subID);

        if(itRecent != arrRecentID.end() && itIsExist == vecSellerID.end()
        	&& pUser != NULL && (int)(pUser->m_nServerIp) == CAppManager::GetInstance().GetConfig().GetDeviceIp())
        {
            vecSellerID.push_back(subID);
        }
    }
}

// 推送数据存入数据库后，将消息推送给在线商家
void CThreadMsgNotify::SendToOnlineSeller(const vector<uint32> &vecSellerID, const char *pUUID)
{
	CUserManager userManager(m_pGlobalBusi);
    /*uint8 cnt = */vecSellerID.size();
    for(int i = 0; i < (int)vecSellerID.size(); ++i)
    {
    	uint32 userid = vecSellerID[i];
    	shared_ptr<CUser> pUser = userManager.Find(userid);
    	if(pUser == NULL)
    	{
    		CLog::Log("SendToOnlineSeller",CLog::TYPE_ERROR, "商家[%u]不在本机", userid);
    		DeleteMsgNotify(pUUID, userid);
    	}
    	else
    	{
    		// 在线
            Buf_ClientSend ack;
            ack.set_idxpacketreq(0);
            ack.set_bufunitidx(pUUID);
            ack.set_buflocation(LOCATION_MEM);
    		SendToSeller(m_pGlobalBusi, &m_DatabaseMini, userid, ack);
    	}
    }
	/*	

    //查询用户状态信息
    std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(vecSellerID.size());
    if (RESULT_FAILED == m_pGlobalBusi->QueryUserStatus(&vecSellerID[0], &userStatus[0], cnt))
    {
        CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "QueryUserStatus failed!");
        return;
    }
    else
    {
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "QueryUserStatus success!");
        for(int i = 0; i < (int)userStatus.size(); ++i)
        {
            GSRV_GET_USER_STATUS_ACK::tagUserStatus &stats = userStatus[i];

            //CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "推送给商家:%u", stats.id);

            if(stats.status != XTREAM_OFFLINE)
            {
                // 在线
                Buf_ClientSend ack;
                ack.set_idxpacketreq(0);
                ack.set_bufunitidx(pUUID);
                ack.set_buflocation(LOCATION_DB);
                
                CUser *pUser = CAppManager::GetInstance().GetUserManager()->Find(stats.id);
                if(pUser)
                {
                    // 客户端连的本机，直接推送消息
                    // 将新订单推送给商家
                    SendToSeller(&m_DatabaseMini, stats.id, ack);
                }
                else
                {
              
                    // 客户端连的其他服务器，先将消息转到其他服务器，再推送
                    uint32 toid = stats.id;
                    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
                    uint8 count = 1;
                    m_pGlobalBusi->QueryUserControlInfo(&toid, &UserInfo, count);
                    if(UserInfo.uid != toid)
                    {
                        CLog::Log("SendToOnlineSeller",CLog::TYPE_ERROR,"获取客服%d信息失败",toid);
                        continue;
                    }
                    else
                    {
						int msg_size = ack.ByteSize();
						char *szData = new char[msg_size];
						ack.SerializeToArray(szData, msg_size);
						CLog::Log("SendToOnlineSeller",CLog::TYPE_DEBUG, "消息推送数据[%s]发送到服务器[%u]", ack.bufunitidx().c_str(), UserInfo.srvip);
							
                        // 构造转发消息结构体
                        SendDataStruct sendData;
                        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
                        CAppManager::GetInstance().InitHead(
                            lpSendHead,
                            CMD_SERVERMSG_IND,
                            sizeof(XT_SERVER_TRANSMIT) + msg_size,
                            toid,XT_HEAD::DEST_TYPE_SERVER);

                        LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
                        lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
                        lpSrvTransmit->msgtype              = CMD_SERVER_SEND_BUF_IND;

                        CSendPacket sendPacket( (char*)sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
                        sendPacket.WriteData(szData, msg_size);
                        delete[] szData;

                        sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sendPacket.GetSize();
                        CAppManager::GetInstance().GetAppNetWork().SendDataTo(
                            sendData.data,sendData.data_len,0,0,UserInfo.srvip, PORT_IM_SVRP2P);
                    }
                    
                }
            }
        }
    }
    */
}

BOOL CThreadMsgNotify::InsertMsgNotify(const char *pUuid, const std::vector<uint32> &vecUserID, const std::string &buffer, EuMsgPush nBufferType)
{
	m_lockMsgNotify.Lock();
	CLog::Log("CThreadMsgNotify::InsertMsgNotify",CLog::TYPE_DEBUG, "写入UUID[%s]", pUuid);
	if(g_mapMsgNotify.find(pUuid) != g_mapMsgNotify.end())
	{
		CLog::Log("CThreadMsgNotify::InsertMsgNotify",CLog::TYPE_ERROR, "uuid重复%s", pUuid);
		m_lockMsgNotify.Unlock();
		return FALSE;
	}

	StruMsgNotifyData msgNotify;
	msgNotify.enType = nBufferType;
	msgNotify.strData = buffer;
	msgNotify.vecID = vecUserID;
	g_mapMsgNotify[pUuid] = msgNotify;
	m_lockMsgNotify.Unlock();
	return TRUE;
}

void CThreadMsgNotify::DeleteMsgNotify(const char *pUuid, uint32 userid)
{
	m_lockMsgNotify.Lock();
	CLog::Log("CThreadMsgNotify::DeleteMsgNotify",CLog::TYPE_DEBUG, "删除UUID[%s] USERID[%u]", pUuid, userid);
	map<string, StruMsgNotifyData>::iterator it = g_mapMsgNotify.find(pUuid);
	if(it == g_mapMsgNotify.end())
	{
		CLog::Log("CThreadMsgNotify::DeleteMsgNotify",CLog::TYPE_ERROR, "UUID[%s]不存在", pUuid);
		m_lockMsgNotify.Unlock();
		return;
	}

	auto itExist = std::find(it->second.vecID.begin(), it->second.vecID.end(), userid);
	if(itExist != it->second.vecID.end())
	{
		it->second.vecID.erase(itExist);
	}

	if(it->second.vecID.size() == 0)
	{
		CLog::Log("CThreadMsgNotify::DeleteMsgNotify",CLog::TYPE_DEBUG, "删除UUID[%s]", pUuid);
		g_mapMsgNotify.erase(it);
	}
	m_lockMsgNotify.Unlock();
}

BOOL CThreadMsgNotify::SearchMsgNotify(const char *pUuid, std::string &buffer, EuMsgPush &nBufferType)
{
	m_lockMsgNotify.Lock();
	CLog::Log("CThreadMsgNotify::SearchMsgNotify",CLog::TYPE_DEBUG, "查询UUID[%s]", pUuid);
	map<string, StruMsgNotifyData>::iterator it = g_mapMsgNotify.find(pUuid);
	if(it == g_mapMsgNotify.end())
	{
		CLog::Log("CThreadMsgNotify::SearchMsgNotify",CLog::TYPE_DEBUG, "UUID不存在[%s]", pUuid);
		m_lockMsgNotify.Unlock();
		return FALSE;
	}

	buffer = it->second.strData;
	nBufferType = it->second.enType;

	m_lockMsgNotify.Unlock();
	return TRUE;
}

// 删除一个商家的所有消息推送数据，在商家退出时调用
void CThreadMsgNotify::DeleteSellerData(uint32 userid)
{
	CLog::Log("CThreadMsgNotify::DeleteSellerData",CLog::TYPE_DEBUG, "清空商家[%u]所有的消息推送数据", userid);
	// 包含此商家的uuid
	vector<string> vecUUID;

	m_lockMsgNotify.Lock();
	map<string, StruMsgNotifyData>::iterator it = g_mapMsgNotify.begin();
	for(; it != g_mapMsgNotify.end(); ++it)
	{
		auto itExist = std::find(it->second.vecID.begin(), it->second.vecID.end(), userid);
		if(itExist != it->second.vecID.end())
		{
			vecUUID.push_back(it->first);
		}
	}
	m_lockMsgNotify.Unlock();

	for(int i = 0; i < (int)vecUUID.size(); ++i)
	{
		DeleteMsgNotify(vecUUID[i].c_str(), userid);
	}
}