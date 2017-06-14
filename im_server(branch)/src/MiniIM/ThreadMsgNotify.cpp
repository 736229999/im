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

// ��Ϣ��¼�ڴ�
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

    //��ʼ����ȫ�ַ�����������
#ifdef IM_USE_CACHE
    m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),m_pMemCache);
#else
    m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
#endif

    //��ʼ�����ݿ�
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    CAppManager::GetInstance().GetConfig().GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);

    CLog::Log("CThreadMsgNotify::StartWork",CLog::TYPE_DEBUG,"DB miniuid:%s minipwd:%s minisid:%s", miniuid, minipwd, minisid);
    m_DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
    if( !m_DatabaseMini.ConnectDB() )
    {
        CLog::Log("CThreadMsgNotify::StartWork",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return -1;
    }

    // �����߳�
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
		CLog::Log("CThreadMsgNotify::Proc", CLog::TYPE_ERROR, "��Ϣ����buf�ܵ�Ϊ��");
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
            CLog::Log("CThreadMsgNotify", CLog::TYPE_IMPORTANT, "��Ϣ���Ͱ�����ɹ�");
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
            CLog::Log("CThreadMsgNotify", CLog::TYPE_ERROR, "��Ϣ���Ͱ����ʧ��");
        }
    }

    return NULL;
}

// �յ��¶���
void CThreadMsgNotify::RecvNewOrder(const char *pData, int nLength)
{
    NewOrder neworder;
    if(neworder.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvNewOrder", CLog::TYPE_IMPORTANT, "�¶���������ɹ� length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvNewOrder", CLog::TYPE_ERROR, "�¶��������ʧ��");
        return;
    }

    // �õ��̼����пͷ�
    vector<uint32> arrRecentID;
    GetNotifySellerID(neworder.memguid().c_str(), atoi(neworder.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_IMPORTANT, "û��[%s]���˺��ڴ˷�����", neworder.merchantid().c_str());
        return;
    }

    // ����uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_IMPORTANT, "����Ϣ UUID:%s, datalength:%d", szUUID, nLength);

    // ������Ϣ����д�����ݿ�
    string strBuffer;
    strBuffer.assign(pData, nLength);

	// �������ݿ⣬�Ĵ��ڴ�
    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_NEWORDER))
    {
        // ����ʧ��
        CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_ERROR, "����Ŀд���ڴ�ʧ��");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvNewOrder",CLog::TYPE_DEBUG, "����Ŀд���ڴ�ɹ�");
    }

    SendToOnlineSeller(arrRecentID, szUUID);
}

// �յ��Ѹ����
void CThreadMsgNotify::RecvPayOrder(const char *pData, int nLength)
{
    PayOrder payorder;
    if(payorder.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvPayOrder", CLog::TYPE_IMPORTANT, "�Ѹ����������ɹ� length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvPayOrder", CLog::TYPE_ERROR, "�Ѹ���������ʧ��");
        return;
    }

    // �õ��̼����пͷ�
    vector<uint32> arrRecentID;
    GetNotifySellerID(payorder.memguid().c_str(), atoi(payorder.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_IMPORTANT, "û��[%s]���˺��ڴ˷�����", payorder.merchantid().c_str());
        return;
    }

    // ����uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_IMPORTANT, "����Ϣ UUID:%s, datalength:%d", szUUID, nLength);

    // ������Ϣ����д�����ݿ�
    string strBuffer;
    strBuffer.assign(pData, nLength);

    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_PAYORDER))
    {
        // ����ʧ��
        CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_ERROR, "����Ŀд���ڴ�ʧ��");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvPayOrder",CLog::TYPE_DEBUG, "����Ŀд���ڴ�ɹ�");
    }

    SendToOnlineSeller(arrRecentID, szUUID);
}

// �յ���Ʒ����
void CThreadMsgNotify::RecvGoodsComment(const char *pData, int nLength)
{
    GoodsComment goodscomment;
    if(goodscomment.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvGoodsComment", CLog::TYPE_IMPORTANT, "��Ʒ���۰�����ɹ� length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvGoodsComment", CLog::TYPE_ERROR, "��Ʒ���۰����ʧ��");
        return;
    }

    // �õ��̼����пͷ�
    vector<uint32> arrRecentID;
    GetNotifySellerID(goodscomment.memguid().c_str(), atoi(goodscomment.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_IMPORTANT, "û��[%s]���˺��ڴ˷�����", goodscomment.merchantid().c_str());
        return;
    }

    // ����uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_IMPORTANT, "����Ϣ UUID:%s, datalength:%d", szUUID, nLength);

    // ������Ϣ����д�����ݿ�
    string strBuffer;
    strBuffer.assign(pData, nLength);

    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_GOODSCOMMENT))
    {
        // ����ʧ��
        CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_ERROR, "����Ŀд���ڴ�ʧ��");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvGoodsComment",CLog::TYPE_DEBUG, "����Ŀд���ڴ�ɹ�");
    }

    SendToOnlineSeller(arrRecentID, szUUID);
}

// �յ��˻�����
void CThreadMsgNotify::RecvReturnOrder(const char *pData, int nLength)
{
    ReturnOrder returnorder;
    if(returnorder.ParseFromArray(pData, nLength))
    {
        CLog::Log("CThreadMsgNotify::RecvReturnOrder", CLog::TYPE_IMPORTANT, "�˻�����������ɹ� length:%d", nLength);
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvReturnOrder", CLog::TYPE_ERROR, "�˻����������ʧ��");
        return;
    }

    // �õ��̼����пͷ�
    vector<uint32> arrRecentID;
    GetNotifySellerID(returnorder.memguid().c_str(), atoi(returnorder.merchantid().c_str()), arrRecentID);
    if(arrRecentID.size() == 0)
    {
    	CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_IMPORTANT, "û��[%s]���˺��ڴ˷�����", returnorder.merchantid().c_str());
        return;
    }

    // ����uuid
    char szUUID[60] = {0};
    random_uuid(szUUID);
    CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_IMPORTANT, "����Ϣ UUID:%s, datalength:%d", szUUID, nLength);

    // ������Ϣ����д�����ݿ�
    string strBuffer;
    strBuffer.assign(pData, nLength);

    if(!InsertMsgNotify(szUUID, arrRecentID, strBuffer, MP_RETURNORDER))
    {
        // ����ʧ��
        CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_ERROR, "����Ŀд���ڴ�ʧ��");
        return;
    }
    else
    {
        CLog::Log("CThreadMsgNotify::RecvReturnOrder",CLog::TYPE_DEBUG, "����Ŀд���ڴ�ɹ�");
    }
    SendToOnlineSeller(arrRecentID, szUUID);
}

void CThreadMsgNotify::SendToSeller(CGlobalBusi *pGlobalBusi, CDataBaseMini *pDB, uint32 did, const Buf_ClientSend &ack)
{
    // �����ݿ��в�������
    string buffer;
    EuMsgPush buftype;
    if(!SearchMsgNotify(ack.bufunitidx().c_str(), buffer, buftype))
    {
        // ����ʧ��
        CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_ERROR, "��ѯ��Ϣ������Ŀʧ��");
        return;
    }

    // ��Ϣ�������ù���
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
        CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_ERROR, "��Ϣ����%d����Ϊ�����͸��ͻ���", buftype);
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

    // �жϰ��ĳ��ȣ��������800���ͷְ���
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

    // ����ת����Ϣ�ṹ��
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
    CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_ERROR,"��ͷ�%d����ϵͳ��Ϣ Buf_ServerSend",did);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
        sendData.data,sendData.data_len,SEND_RETRY_TIME);

    if(idxpacket == nPacketCount - 1)
    {
        // ������ɺ�ɾ�����ݿ��еļ�¼
        CLog::Log("CThreadMsgNotify::SendToSeller",CLog::TYPE_DEBUG, "��Ϣ�����ͣ�ɾ������:uuid:%s, did:%u", ack.bufunitidx().c_str(), did);
        DeleteMsgNotify(ack.bufunitidx().c_str(), did);
    }
}

// ��ȡ��Ҫͨ����̼�ID[ͬʱֻ��ȡ���ߵ�]
void CThreadMsgNotify::GetNotifySellerID(const char *pGuid, int merchantID, vector<uint32> &vecSellerID)
{
	CUserManager userManager(m_pGlobalBusi);
    vecSellerID.clear();

    CLog::Log("CThreadMsgNotify::GetNotifySellerID", CLog::TYPE_IMPORTANT, "merchant id:%d", merchantID);
    // ���ж�Ӧ�����˺�
    AccountArray ids;
    MYSQL_LIB->getAllSubAccount(merchantID, ids);
    // ��ȡ�����˺�
    uint32_t mainaccount;
    MYSQL_LIB->getMainAccount(merchantID, mainaccount);
    // ��ȡ�����ϵ�̼�
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

// �������ݴ������ݿ�󣬽���Ϣ���͸������̼�
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
    		CLog::Log("SendToOnlineSeller",CLog::TYPE_ERROR, "�̼�[%u]���ڱ���", userid);
    		DeleteMsgNotify(pUUID, userid);
    	}
    	else
    	{
    		// ����
            Buf_ClientSend ack;
            ack.set_idxpacketreq(0);
            ack.set_bufunitidx(pUUID);
            ack.set_buflocation(LOCATION_MEM);
    		SendToSeller(m_pGlobalBusi, &m_DatabaseMini, userid, ack);
    	}
    }
	/*	

    //��ѯ�û�״̬��Ϣ
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

            //CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "���͸��̼�:%u", stats.id);

            if(stats.status != XTREAM_OFFLINE)
            {
                // ����
                Buf_ClientSend ack;
                ack.set_idxpacketreq(0);
                ack.set_bufunitidx(pUUID);
                ack.set_buflocation(LOCATION_DB);
                
                CUser *pUser = CAppManager::GetInstance().GetUserManager()->Find(stats.id);
                if(pUser)
                {
                    // �ͻ������ı�����ֱ��������Ϣ
                    // ���¶������͸��̼�
                    SendToSeller(&m_DatabaseMini, stats.id, ack);
                }
                else
                {
              
                    // �ͻ��������������������Ƚ���Ϣת��������������������
                    uint32 toid = stats.id;
                    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
                    uint8 count = 1;
                    m_pGlobalBusi->QueryUserControlInfo(&toid, &UserInfo, count);
                    if(UserInfo.uid != toid)
                    {
                        CLog::Log("SendToOnlineSeller",CLog::TYPE_ERROR,"��ȡ�ͷ�%d��Ϣʧ��",toid);
                        continue;
                    }
                    else
                    {
						int msg_size = ack.ByteSize();
						char *szData = new char[msg_size];
						ack.SerializeToArray(szData, msg_size);
						CLog::Log("SendToOnlineSeller",CLog::TYPE_DEBUG, "��Ϣ��������[%s]���͵�������[%u]", ack.bufunitidx().c_str(), UserInfo.srvip);
							
                        // ����ת����Ϣ�ṹ��
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
	CLog::Log("CThreadMsgNotify::InsertMsgNotify",CLog::TYPE_DEBUG, "д��UUID[%s]", pUuid);
	if(g_mapMsgNotify.find(pUuid) != g_mapMsgNotify.end())
	{
		CLog::Log("CThreadMsgNotify::InsertMsgNotify",CLog::TYPE_ERROR, "uuid�ظ�%s", pUuid);
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
	CLog::Log("CThreadMsgNotify::DeleteMsgNotify",CLog::TYPE_DEBUG, "ɾ��UUID[%s] USERID[%u]", pUuid, userid);
	map<string, StruMsgNotifyData>::iterator it = g_mapMsgNotify.find(pUuid);
	if(it == g_mapMsgNotify.end())
	{
		CLog::Log("CThreadMsgNotify::DeleteMsgNotify",CLog::TYPE_ERROR, "UUID[%s]������", pUuid);
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
		CLog::Log("CThreadMsgNotify::DeleteMsgNotify",CLog::TYPE_DEBUG, "ɾ��UUID[%s]", pUuid);
		g_mapMsgNotify.erase(it);
	}
	m_lockMsgNotify.Unlock();
}

BOOL CThreadMsgNotify::SearchMsgNotify(const char *pUuid, std::string &buffer, EuMsgPush &nBufferType)
{
	m_lockMsgNotify.Lock();
	CLog::Log("CThreadMsgNotify::SearchMsgNotify",CLog::TYPE_DEBUG, "��ѯUUID[%s]", pUuid);
	map<string, StruMsgNotifyData>::iterator it = g_mapMsgNotify.find(pUuid);
	if(it == g_mapMsgNotify.end())
	{
		CLog::Log("CThreadMsgNotify::SearchMsgNotify",CLog::TYPE_DEBUG, "UUID������[%s]", pUuid);
		m_lockMsgNotify.Unlock();
		return FALSE;
	}

	buffer = it->second.strData;
	nBufferType = it->second.enType;

	m_lockMsgNotify.Unlock();
	return TRUE;
}

// ɾ��һ���̼ҵ�������Ϣ�������ݣ����̼��˳�ʱ����
void CThreadMsgNotify::DeleteSellerData(uint32 userid)
{
	CLog::Log("CThreadMsgNotify::DeleteSellerData",CLog::TYPE_DEBUG, "����̼�[%u]���е���Ϣ��������", userid);
	// �������̼ҵ�uuid
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