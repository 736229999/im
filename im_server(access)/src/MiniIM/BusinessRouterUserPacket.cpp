#include "stdafx.h"
#include "BusinessRouterUserPacket.h"
#include "AppManager.h"
#include "RouterBusiness.h"
#include "protocol.h"
#include "BusiCacheInterface.h"
#include "improtocol.pb.h"
#include "imapi.h"
#include "UserManager.h"


CBusinessRouterPacket::CBusinessRouterPacket(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_ROUTERPACKET_REQ,CMD_ROUTERPACKET_REQ)
{
}

CBusinessRouterPacket::~CBusinessRouterPacket()
{
}

int CBusinessRouterPacket::DoWork()
{

    //转发该消息
    SendDataStruct sendData;
    RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
    memcpy(sendData.data, pRecData->data, pRecData->data_len);
    sendData.data_len = pRecData->data_len;
    sendData.vlink = pRecData->vlink;


    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
    this->CancelReply();
    return RESULT_SUCCESS;
}


CBusinessRouterAckPacket::CBusinessRouterAckPacket(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_ROUTERPACKET_ACK,CMD_ROUTERPACKET_ACK)
{
}

CBusinessRouterAckPacket::~CBusinessRouterAckPacket()
{
}

int CBusinessRouterAckPacket::DoWork()
{

    //转发该消息
    SendDataStruct sendData;
    RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
    memcpy(sendData.data, pRecData->data, pRecData->data_len);
    sendData.data_len = pRecData->data_len;
    sendData.vlink = pRecData->vlink;


    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
    this->CancelReply();
    return RESULT_SUCCESS;
}


CBusinessRouterUserPacket::CBusinessRouterUserPacket(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_ROUTERUSERPACKET_REQ,CMD_ROUTERUSERPACKET_REQ)
{
}

CBusinessRouterUserPacket::~CBusinessRouterUserPacket()
{
}

int CBusinessRouterUserPacket::DoWork()
{

    //转发该消息
    SendDataStruct sendData;
    RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
    memcpy(sendData.data, pRecData->data, pRecData->data_len);
    sendData.data_len = pRecData->data_len;
    sendData.vlink = pRecData->vlink;


    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
    this->CancelReply();
    return RESULT_SUCCESS;
}

CBusinessRouterUserAckPacket::CBusinessRouterUserAckPacket(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_ROUTERUSERPACKET_ACK,CMD_ROUTERUSERPACKET_ACK)
{
}

CBusinessRouterUserAckPacket::~CBusinessRouterUserAckPacket()
{
}

int CBusinessRouterUserAckPacket::DoWork()
{

    //转发该消息
    SendDataStruct sendData;
    RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
    memcpy(sendData.data, pRecData->data, pRecData->data_len);
    sendData.data_len = pRecData->data_len;
    sendData.vlink = pRecData->vlink;


    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
    this->CancelReply();
    return RESULT_SUCCESS;
}


CBusinessReqTransFile::CBusinessReqTransFile(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_TRANSFILE_REQ,CMD_TRANSFILE_REQ)
{
}

CBusinessReqTransFile::~CBusinessReqTransFile()
{
}

int CBusinessReqTransFile::DoWork()
{
    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->sid);
    if (pUser != NULL)
    {
        XT_ROUTERPACKET_REQ* pRouterReq  = (XT_ROUTERPACKET_REQ*)m_RecvPacket.GetData();

        //转发该消息
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
        int len = 0;

        CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
        sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
        sendPacket  <<pUser->m_nIP
                    <<pUser->m_nPort;
        len += sendPacket.GetSize();

        CAppManager::GetInstance().InitHead(lpSendHead,CMD_TRANSFILE_REQ,len,pRouterReq->uid);
        sendData.data_len = len + (int)sizeof(XT_HEAD);

        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0);
    }
    this->CancelReply();
    return RESULT_SUCCESS;
}

CBusinessAckTransFile::CBusinessAckTransFile(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_TRANSFILE_ACK,CMD_TRANSFILE_ACK)
{
}

CBusinessAckTransFile::~CBusinessAckTransFile()
{
}

int CBusinessAckTransFile::DoWork()
{
    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->sid);
    if (pUser != NULL)
    {
        XT_ROUTERPACKET_ACK* pRouterAck  = (XT_ROUTERPACKET_ACK*)m_RecvPacket.GetData();

        //转发该消息
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
        int len = 0;

        CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
        sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
        sendPacket  <<pUser->m_nIP
                    <<pUser->m_nPort;
        len += sendPacket.GetSize();

        CAppManager::GetInstance().InitHead(lpSendHead,CMD_TRANSFILE_ACK,len,pRouterAck->uid);
        sendData.data_len = len + (int)sizeof(XT_HEAD);

        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0);
    }
    this->CancelReply();
    return RESULT_SUCCESS;
}

CBusinessTransfer::CBusinessTransfer(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_FILE_TRANS_REQ,CMD_FILE_TRANS_ACK)
{
}

CBusinessTransfer::~CBusinessTransfer()
{
}

int CBusinessTransfer::DoWork()
{
    SendDataStruct sendData;
    SEND_FILE_PACK_REQ* FileReq = (SEND_FILE_PACK_REQ*)m_RecvPacket.GetData();

    m_SendPacket<<FileReq->send_id
                <<FileReq->fid
                <<FileReq->flag
                <<FileReq->file_id
                <<FileReq->pack_id
                <<FileReq->size;

    CLog::Log("CBusinessTransfer::DoWork",CLog::TYPE_IMPORTANT,"fid:%d, did:%d, file:%d, pack:%d, size:%d",
              FileReq->send_id,FileReq->fid,FileReq->file_id,FileReq->pack_id,FileReq->size);

    CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
    sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    int len = sendPacket.GetSize();
    XT_HEAD* pHead = (XT_HEAD*)sendData.data;

    CAppManager::GetInstance().InitHead(pHead,CMD_FILE_TRANS_REQ,len,FileReq->fid);
    sendData.data_len = len + (int)sizeof(XT_HEAD);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,0);

    return RESULT_SUCCESS;
}

CBusinessTransferInd::CBusinessTransferInd(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_FILE_TRANS_IND,CMD_FILE_TRANS_IND_ACK)
{
}

CBusinessTransferInd::~CBusinessTransferInd()
{
}

int CBusinessTransferInd::DoWork()
{
    SendDataStruct sendData;
    SEND_FILE_PACK_IND* FileInd = (SEND_FILE_PACK_IND*)m_RecvPacket.GetData();

    m_SendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
    sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    int len = sendPacket.GetSize();
    XT_HEAD* pHead = (XT_HEAD*)sendData.data;

    CAppManager::GetInstance().InitHead(pHead,CMD_FILE_TRANS_IND,len,FileInd->fid);
    sendData.data_len = len + (int)sizeof(XT_HEAD);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,0);

    return RESULT_SUCCESS;
}

CBusinessReqCusterTran::CBusinessReqCusterTran(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_CUSTSER_TRAN_REQ,0)
{
}
CBusinessReqCusterTran::~CBusinessReqCusterTran()
{
}

int CBusinessReqCusterTran::DoWork()
{
    XT_CUSTSER_TRAN_REQ* CusterTranReq = (XT_CUSTSER_TRAN_REQ*)m_RecvPacket.GetData();

	// 构建转发消息
    // 转发给dest
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
                        lpSendHead,
                        CMD_SERVERMSG_IND,
                        sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_CUSTSER_TRAN_REQ),
                        CusterTranReq->uDid);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_CUSTSER_TRAN_REQ;

    XT_CUSTSER_TRAN_REQ* CusterTranReqDest = (XT_CUSTSER_TRAN_REQ*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    memcpy(CusterTranReqDest, CusterTranReq, sizeof(XT_CUSTSER_TRAN_REQ));

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_CUSTSER_TRAN_REQ);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                        sendData.data,sendData.data_len,SEND_RETRY_TIME);

    CLog::Log("CBusinessReqCusterTran",CLog::TYPE_DEBUG,"send to dest[%u]！", CusterTranReq->uDid);
        
    CancelReply();

    return RESULT_SUCCESS;

}
CBusinessAckCusterTran::CBusinessAckCusterTran(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_CUSTSER_TRAN_ACK,0)
{
}
CBusinessAckCusterTran::~CBusinessAckCusterTran()
{
}

int CBusinessAckCusterTran::DoWork()
{
    SendDataStruct sendData;
    XT_CUSTSER_TRAN_ACK* CusterTranAck = (XT_CUSTSER_TRAN_ACK*)m_RecvPacket.GetData();

	// 构建转发消息
    // 转发给dest
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
                        lpSendHead,
                        CMD_SERVERMSG_IND,
                        sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_CUSTSER_TRAN_REQ),
                        CusterTranAck->uDid);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_CUSTSER_TRAN_ACK;

    XT_CUSTSER_TRAN_ACK* CusterTranAckDest = (XT_CUSTSER_TRAN_ACK*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    memcpy(CusterTranAckDest, CusterTranAck, sizeof(XT_CUSTSER_TRAN_ACK));

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_CUSTSER_TRAN_ACK);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                        sendData.data,sendData.data_len,SEND_RETRY_TIME);

    CLog::Log("CBusinessAckCusterTran",CLog::TYPE_DEBUG,"send to dest[%u]！", CusterTranAck->uDid);

    CancelReply();
#ifdef IM_USE_IMAPI
	//   8. 客服转接，ServiceId和customerId,group_id必填，toServiceID和toServiceGroup 是要转接到的客服的ID和组ID

		imapi csTransReq;
		string info;
		csTransReq.imapiReq(imapi::CSTRANSFER,info,CusterTranAck->uDid,CusterTranAck->uCustomerId,0,0,0,CusterTranAck->uSid,0);
	
#else

    // 这里需要刷新买家卖家的内存
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    ClientService * clientService_S = NULL;
    clientService_S = csManager.Find(CusterTranAck->uSid);

    ClientService * clientService_D = NULL;
    clientService_D = csManager.Find(CusterTranAck->uDid);

    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(CusterTranAck->uCustomerId);

    if(clientService_D != NULL)
    {
        CLog::Log("CBusinessAckCusterTran", CLog::TYPE_DEBUG, "客服%s清空买家%u", clientService_D->m_CSname, CusterTranAck->uCustomerId);
        clientService_D->RemoveCustomer_x(CusterTranAck->uCustomerId);

        /*修改用户信息到memcached中*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService_D);
    }
    else
    {
        CsCustOper(m_pRouter, UO_DEL_CUST, CusterTranAck->uDid, CusterTranAck->uCustomerId);
    }

    int userteamid = -1;

    if(pUser != NULL)
    {
        pUser->m_nCSuserid = CusterTranAck->uSid;
        userteamid = pUser->getCsTeamId();

        CLog::Log("LoginSuccess", CLog::TYPE_IMPORTANT, "向全局更新一个WEB用户：%d 状态 %d", pUser->m_nID,pUser->m_nStatus);
        m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0);
    }
    else
    {
        // 如果买家不在此服务器，通告其他服务器来修改
        //查询全局服务器并清除web所在服务器其接待上下文
        tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
        uint8 count = 1;
        m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&(CusterTranAck->uCustomerId),&UserInfo,count);

        if(UserInfo.uid != CusterTranAck->uCustomerId)
        {
            CLog::Log("CBusinessAckCusterTran",CLog::TYPE_IMPORTANT,
                      "查询买家全局信息失败! %u", CusterTranAck->uCustomerId);
        }
        else
        {
            userteamid = UserInfo.teamID;
            SetWebContext(m_pRouter, CusterTranAck->uCustomerId, CusterTranAck->uSid, userteamid);
        }
    }

    if(clientService_S != NULL)
    {
        CLog::Log("CBusinessAckCusterTran", CLog::TYPE_DEBUG, "客服%s添加买家%u", clientService_S->m_CSname, CusterTranAck->uCustomerId);
        clientService_S->AddCustomer_x(userteamid, CusterTranAck->uCustomerId);

        /*修改用户信息到memcached中*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService_S);
    }
    else
    {
    	CsCustOper(m_pRouter, UO_ADD_CUST, CusterTranAck->uSid, CusterTranAck->uCustomerId);
    }

    //推送用户所在组的所有排队用户信息
    if(userteamid != -1)
    {
        PushQueueSubAccountToKafka(m_pRouter,userteamid);
    }
#endif

    return RESULT_SUCCESS;
}

