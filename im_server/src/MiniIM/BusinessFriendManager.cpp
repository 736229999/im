#include<algorithm>
#include "stdafx.h"
#include "BusinessFriendManager.h"
#include "AppManager.h"
#include "RouterBusiness.h"
#include "protocol.h"
#include "ClientProtocol.h"
#include "md5.h"
#include "Function.h"
#include "BusinessServerMsg.h"
#include "NotifyProtocol.h"
#include "../DataBaseLib/MysqlLib.h"
#include <time.h>
#include <iconv.h>
#include "hessian/hessianLogi.h"
#include "BusiCacheInterface.h"
#ifndef IM_USE_IMAPI
#include "ClientService.h"
#endif
#include "improtocol.pb.h"
#include "ThreadMsgNotify.h"
#include "imapi.h"
#include "UserManager.h"
#include "BusiLogin.h"


int CalcQueueLength(int QueueOrigin,int row = 4,int col = 4);

#define MAX_QUERY_BASIC_INFO_COUNT 200




//����Ӵ�����  ���ַ�ʽ һ�ֿ�ʼ�Ӵ���һ�ֽ����Ӵ�

//����ģʽ�Ǵ�����������Ӵ�ģʽ
CBusinessCSQueue::CBusinessCSQueue(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_RECEPTE_CUST_REQ,CMD_RECEPTE_CUST_ACK)
{
    pData = new SendDataStruct;
    if( pData == NULL)
        CLog::Log("CBusinessCSQueue",CLog::TYPE_ERROR,"New failed!");
}

CBusinessCSQueue::~CBusinessCSQueue()
{
    if(pData)
    {
        delete pData;
        pData = NULL;
    }

}


void CBusinessCSQueue::WebLogOut(uint32 fromid,uint32 toid,uint16 type)   //��WEB�����������˳�����
{
    // ����ת����Ϣ�ṹ��
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + sizeof(uint8) + sizeof(uint32) + sizeof(uint32) + sizeof(uint32) + m_RecvPacket.GetSize(),
        toid,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACK);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_LOGOUT_CS_WEB;

    BYTE* pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) ) ;
    * ((uint8*)pVerifyFriendReq ) = (uint8)0;
    * ((uint32*)(pVerifyFriendReq + 1)) = toid;
	* ((uint32*)(pVerifyFriendReq + 5)) = fromid;
    * ((uint16*)(pVerifyFriendReq + 9)) = type;

    pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint8) + sizeof(uint32) + sizeof(uint32) + sizeof(uint16));
    memcpy(pVerifyFriendReq,m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint8) + sizeof(uint32) + sizeof(uint32) + sizeof(uint16) + m_RecvPacket.GetSize();
    CLog::Log("CBusinessCSQueue::WebLogOut",CLog::TYPE_ERROR,"%u��WEB�������û�%u�˳�����",fromid,toid);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}

void CBusinessCSQueue::SetWebContext(uint32 toid,uint32 csid,uint32 teamid)
{
    //��ѯȫ�ַ����������web���ڷ�������Ӵ�������
    CConfigInfo m_ConfigInfo;
    list < uint32 > ServerList;
    m_ConfigInfo.GetServerList(&ServerList);
    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&toid,&UserInfo,count);

    if(UserInfo.uid != toid)
    {
        return;
    }
    else
    {
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

        char* pMsgData = sendData.data + sizeof(XT_HEAD);

        CSendPacket SendPacket;
        SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

        SendPacket  << toid
                    << (uint8)0xCC
                    <<  (uint32)csid
                    <<  (uint32)teamid;

        sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_SET_WEBUSERINFO_REQ,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0,0,UserInfo.srvip, PORT_IM_SVRP2P);
        CLog::Log("CBusinessCSQueue::SetWebContext",CLog::TYPE_IMPORTANT,
                  "֪ͨ������ %u ����web�û� %d�ĽӴ���Ϣ�ڿͷ�%u ��%u !",
                  UserInfo.srvip,toid,csid,teamid);
    }
}


void CBusinessCSQueue::ClearWebContext(uint32 toid,uint32 &teamid)
{
    //��ѯȫ�ַ����������web���ڷ�������Ӵ�������
    CConfigInfo m_ConfigInfo;
    list < uint32 > ServerList;
    m_ConfigInfo.GetServerList(&ServerList);
    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&toid,&UserInfo,count);

    if(UserInfo.uid != toid)
    {
        teamid = 0;
        CLog::Log("CBusinessCSQueue::ClearWebContext",CLog::TYPE_IMPORTANT,"��ȡȫ����Ϣ�쳣 toid %u UserInfo.id %u !",toid,UserInfo.uid);
        return;
    }
    else
    {
        teamid = UserInfo.teamID;
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

        char* pMsgData = sendData.data + sizeof(XT_HEAD);

        CSendPacket SendPacket;
        SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

        SendPacket  << toid
                    << (uint8)0xBB;

        sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_SET_WEBUSERINFO_REQ,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0,0,UserInfo.srvip, PORT_IM_SVRP2P);
        CLog::Log("CBusinessCSQueue::ClearWebContext",CLog::TYPE_IMPORTANT,"֪ͨ������ %u ȥ��web�û� %d��ȫ����Ϣ !",UserInfo.srvip,toid);
    }
}

#if 0
void CBusinessCSQueue::updateCsQueue(ClientService *clientService)
{
    CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"���¿ͷ���Ϣ %u has %u !",clientService->m_nID);
    uint8 count = 1;
    CSinfo_t csinfo;

    sprintf(csinfo.m_Csname,"%s",clientService->m_CSname);
    csinfo.m_CurConsult = clientService->m_CurConsult;
    csinfo.m_CurReception = clientService->m_CurReception;
    csinfo.m_MaxRecConsult = clientService->m_MaxRecConsult;
    csinfo.m_nID = clientService->m_nID;
    csinfo.m_nChattingBuyerID = clientService->GetChattingBuyerID();
    csinfo.m_nDirectorID = clientService->GetDirectorID();
    csinfo.m_State = clientService->m_State;
    csinfo.m_Customerid.clear();
    map<uint32,CSortArray<customer_t> *>::iterator mappedIter;
    for(mappedIter = clientService->m_Customerlist.begin(); mappedIter != clientService->m_Customerlist.end();
        mappedIter++)
    {
        for(int i = 0; i < (mappedIter->second)->GetSize(); ++i)
        {
            customer_t *pFriendInfo = (mappedIter->second)->FindByPos(i);
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT,"��ǰ�� %u  ���û� %u",
                      pFriendInfo->teamid,
                      pFriendInfo->userid,
                      pFriendInfo->ReceptionState);
            csinfo.m_Customerid.push_back(pFriendInfo->userid);
        }
    }

    m_pRouter->GetGlobalBusi()->AddModiCSInfo(&csinfo,count,tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_MODIFY);

}

#endif





void CBusinessCSQueue::SendWebQueueInfoIND(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = CAppManager::GetInstance().GetSendSeq();
    pHead->cmd  = CMD_QUEUE_LENGTH_IND;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),pData->data,pData->data_len,SEND_RETRY_TIME);

}

int CBusinessCSQueue::DoWork()
{
    XT_RECEPTE_CUST_REQ *req = (XT_RECEPTE_CUST_REQ *)m_RecvPacket.GetData();


    CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, request id:%u teamId %u, type %d IN" ,
              m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag, req->idBuyer,req->teamId,req->type);
    //char *p = (char *)req;

#ifndef IM_USE_IMAPI
    XT_WAITQUEU_CHANGE QueueInd;
    QueueInd.idBuyer = req->idBuyer;
    QueueInd.teamId = req->teamId;
    ClientService *clientService = CAppManager::GetInstance().GetCSManager().Find(m_pRecvHead->sid);
    if(!clientService)
    {
        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "���ҿͷ�������ʧ�� id: %u",m_pRecvHead->sid );
        return RESULT_FAILED;
    }
#endif
    if(req->type == XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER)   //�Զ��Ӵ�
    {
#ifdef IM_USE_IMAPI
		//  7. ������Լ��ServiceId��group_id���
		
		imapi csAskforConReq;
		string info;
		csAskforConReq.imapiReq(imapi::CSASKFORCONSULT,info,m_pRecvHead->sid);
	
	
#else
	
        //��ȡ�ͷ�������


        vector<uint32> groups;
        CDataBaseMini *pDatabase = m_pRouter->GetDataBaseMini();
        int ret = pDatabase->CS_Get_Groups(m_pRecvHead->sid,groups);
        if(ret != DB_SUCCESS)
        {
            CLog::Log("CBusinessCSQueue",CLog::TYPE_ERROR,"Query CS %u's groups failed!",m_pRecvHead->sid);
            return RESULT_FAILED;
        }
        else
        {
            groups.push_back(0);    //������0����Ŷ�
            for(int i = 0 ; i < (int)groups.size(); i++)
            {

                CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,
                          "Query CS %u's in group %u!",
                          m_pRecvHead->sid,groups[i]);
            }
        }

        //��ȡ�ͷ��������Ŷ����

        tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo qinfo[64];

        ret = m_pRouter->GetGlobalBusi()->QueryCSQueueInfo((const uint32 *)&groups[0],(uint8)groups.size(),(tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo *)&qinfo);
        {
            if(ret != MEMCACHED_GET_INFO_SUCCESS)
            {
                if(ret == MEMCACHED_NO_INFO)
                {
                    CLog::Log("QueryCSQueueInfo",CLog::TYPE_IMPORTANT,"Query team %u EMPTY!",qinfo[0].teamID);
                }
                else
                {
                    CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"Query team %u fail!",qinfo[0].teamID);
                    return RESULT_SUCCESS;
                }
            }
        }

        vector<CSQList_t> customersList;
        for(int i = 0 ; i < (int)groups.size(); i++)
        {

            customersList.insert(customersList.end(),qinfo[i].m_Customerlist.begin(),qinfo[i].m_Customerlist.end());
        }


        for(int i = 0 ; i < (int)customersList.size(); i++)
        {

            CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"before Query  has %u in team %u level %u requesttime %u!",
                      customersList[i].userid,customersList[i].teamID,customersList[i].Level,customersList[i].requestTime);
        }
        //����
        if(customersList.size() > 0)
        {
            sort(customersList.begin(),customersList.end(),SortQueueMethod);
			CheckAndCleanQueue(m_pRouter,customersList);
        }
        else
        {
            CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"No Queue");
        }
       
		
        //ѭ����ȡ����ȡ���Ŷ�index��


        //��ȫ��memcachedɾ���ŶӶ���

        if(customersList.size() == 0)
        {
            //���ݳ���д�����ݰ�
            m_SendPacket << (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER
                         << (uint32)0
                         << (uint32)0;

            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "�ͷ� %uδ�����Ŷӣ��޷������Ӵ�",m_pRecvHead->sid);

            return RESULT_SUCCESS;
        }

        tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
        cSinfo.customerID = customersList[0].userid;
        cSinfo.teamID = customersList[0].teamID;
        uint8 count = 1;
        ret = -1;


        //���û����뵽�Ӵ�������
        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "���û����뵽�Ӵ������� id: %u",customersList[0].userid);
        clientService->AddCustomer_x(customersList[0].teamID,customersList[0].userid);

        //CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(customersList[0].userid);
        if(pUser != NULL)
        {
            pUser->setCsuserId(m_pRecvHead->sid);
            pUser->setCsTeamId(customersList[0].teamID);
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "���û��ӽӴ�������ɾ�� id: %u",m_pRecvHead->sid);

            // �û���Ϣ��д��redis
//            m_pRouter->GetUserManager()->AddUserToRedis(pUser);
        }
        else//֪ͨ���ڵ��û�����WEB������
        {
            SetWebContext(customersList[0].userid,m_pRecvHead->sid,customersList[0].teamID);
        }
        /*�޸��û���Ϣ��memcached��*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "�ӱ����ŶӶ���ɾ���û�,֪ͨ�����ͷ� �����ؿͷ�Ϊid: %u",m_pRecvHead->sid);

        QueueInd.bAdd = XT_WAITQUEU_CHANGE::TYPE_DELETE;
        QueueInd.teamId = customersList[0].teamID;
        QueueInd.idBuyer = customersList[0].userid;
        clientService->NotifyTeamQueue(m_pRouter->GetGlobalBusi(), &QueueInd,1);

        CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"��ȫ��memcachedɾ���Ŷ��û� %u!",cSinfo.customerID);
        ret = m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("CBusinessCSQueue",CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
        }
        PushQueueToKafka(m_pRouter,customersList[0].teamID,customersList[0].userid,m_pRecvHead->sid);

        //���ݳ���д�����ݰ�
        m_SendPacket << (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER
                     << (uint32)customersList[0].userid
                     << (uint32)customersList[0].teamID;

        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "����ͻ� %u �� �ͷ�%u",customersList[0].userid,m_pRecvHead->sid);

        return RESULT_SUCCESS;

#endif    
	}
    
    else if((req->type == XT_RECEPTE_CUST_REQ::TYPE_QUIT)) //�˳��Ӵ� ���ﲻ���˳��û���ֻ֪ͨweb������ձ��غͿͷ��Ĺ���
    {
        uint32 teamID = -1; //��¼�û����ڵ���ID������֪ͨweb
        //֪ͨweb�˽��û�����
        CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "֪ͨweb�˽��û����� id: %u",m_pRecvHead->sid );
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(req->idBuyer);
        if(pUser != NULL)
        {
            // pUser->SendLogoutReq(pUser);
            //���ǳ�web�û���ֻ�޸��û��Ϳͷ��Ĺ���
            CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "���û��ӽӴ�������ɾ�� id: %u",m_pRecvHead->sid );
            teamID = pUser->getCsTeamId();
            uint32 curCSUserID = pUser->m_nCSuserid;
			#ifndef IM_USE_IMAPI
            clientService->RemoveCustomer_x(req->idBuyer);
			#endif
            pUser->ClearWebClient();	
            if(curCSUserID == m_pRecvHead->sid)
            {
            	 //�ظ�web�˳��ͷ��Ӵ�֪ͨ,���ô����ֱ�ӷ���
            	WebLogOut(m_pRecvHead->sid,req->idBuyer);
				#ifndef IM_USE_IMAPI
            	/*�޸��û���Ϣ��memcached��*/
            	CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
				#endif
            }
            else
            {
            	CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "���[%u]�Ŀͷ�[%u]��Ϊ[%u]",req->idBuyer, curCSUserID, m_pRecvHead->sid );
            }
        }
        else
        {
            CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "֪ͨweb���û����ڱ��� %u",m_pRecvHead->sid );
            //��ȫ�ַ�������ȡ
            ClearWebContext(req->idBuyer,teamID);
            WebLogOut(m_pRecvHead->sid,req->idBuyer);
        }
        //���û��ӽӴ�������ɾ��
#ifdef IM_USE_IMAPI
        //�����û�������������Ŷ��û���Ϣ
        //PushQueueSubAccountToKafka(m_pRouter,teamID);
 
    	// 0���ͷ��رմ��ڣ�ֹͣ�Ӵ���SERVICE_ID��group_id���
		uint32 uid = m_pRecvHead->sid;

		imapi closeReq;
		string info;
		closeReq.imapiReq(imapi::CLIENTCLOSEWINDOW,info,uid,req->idBuyer,0);
#else
		PushQueueSubAccountToKafka(m_pRouter,teamID);
#endif		 

    }
    return RESULT_SUCCESS;
}

CBusinessModHangUp::CBusinessModHangUp(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_MOD_HANGUP_STATE_REQ,CMD_MOD_HANGUP_STATE_ACK)
{

}

int CBusinessModHangUp::DoWork()
{
    XT_MOD_HANGUP_STATE_REQ *req = (XT_MOD_HANGUP_STATE_REQ*)m_RecvPacket.GetData();

    CLog::Log("CBusinessModHangUp", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, uUserId:%u, uHangupState:%u IN" ,
              m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag, req->uUserId, req->uHangupState);

    uint8_t cnt = 1;
    std::vector<tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow> userFlowModi(cnt);
    std::vector<tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow> userFlowModiAck(cnt);

    userFlowModi[0].id = req->uUserId;
    userFlowModi[0].flow = req->uHangupState;

    //��ѯ�û������ģ���ͬ��һ��

    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(req->uUserId);
    if( pUser != NULL)
    {
        pUser->m_nflow = req->uHangupState;
//        m_pRouter->GetUserManager()->AddUserToRedis(pUser);
    }
	else
	{
	    m_SendPacket<<userFlowModiAck[0].id
            <<(uint8)0xff
            <<(uint8)0xff;
		return RESULT_SUCCESS;
	}
    //�ӿ�֧��id���飬Ŀǰֻ��һ��id������
    m_pRouter->GetGlobalBusi()->ModiUserFlow(&userFlowModi[0], cnt, &userFlowModiAck[0]);

    m_SendPacket<<userFlowModiAck[0].id
                <<userFlowModiAck[0].status
                <<userFlowModiAck[0].flow;

    CLog::Log("CBusinessModHangUp", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, id:%u, status:%u, flow:%u OUT" ,
              m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag,
              userFlowModiAck[0].id,
              userFlowModiAck[0].status, userFlowModiAck[0].flow);
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        uint32 merchant_id = 0;
        /*CUser::m_UserListLock.Lock();
        CUser *pUser = NULL;
        __gnu_cxx::hash_map<uint32,CUser*>::iterator iter = CUser::m_userhash.find(req->uUserId);
        if(iter != CUser::m_userhash.end())
        {
            pUser = iter->second;
            merchant_id = pUser->m_merchantid;
        }
        CUser::m_UserListLock.Unlock();*/
        //�ϴ�����Ӵ�����
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(merchant_id,req->uUserId);
        time_t tm;
        time(&tm);
        try
        {
            SubMerchantAction info;
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = merchant_id;
            }
            info.subMerchantID = req->uUserId;
            info.actionTime = (int)tm;
            if(req->uHangupState==1 )
            {
                info.actionType = ACTION_HANGUP;
            }
            else if (req->uHangupState==0)
            {
                info.actionType = ACTION_RECEPTION;
            }
            else
            {
                info.actionType = ACTION_ERROR;
                CLog::Log("CBusinessModHangUp_hessian",CLog::TYPE_ERROR,"id:%d, HangupState:%d is wrong!",req->uUserId,req->uHangupState);
            }


            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured:%s",
                      merchant_id, req->uUserId, e.what());
        }

    }


    return RESULT_SUCCESS;

}



CBusinessGetOnlineStatus::CBusinessGetOnlineStatus(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_USER_STATUS_REQ,CMD_GET_USER_STATUS_ACK)
{

}

int CBusinessGetOnlineStatus::DoWork()
{

    //��ȡ���ݰ�ָ��
    XT_GET_USER_STATUS_REQ *req = (XT_GET_USER_STATUS_REQ*)m_RecvPacket.GetData();

    //��ʼ���û�״̬����
//  std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(req->cnt);
    GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[MAX_SEARCH_FRINED_RETURN_NUM];
    uint8_t cnt = req->cnt;

    //��ȡ�û�״̬��Ϣ

#ifdef IM_USE_CACHE
    if (MEMCACHED_NO_INFO < CMemcacheQueryUserStatusi(m_pRouter,req->id,cnt,userStatus))
    {
        CLog::Log("CBusinessGetOnlineStatus",CLog::TYPE_ERROR,"QueryUserStatus error! id:%d ",req->id[0]);
        return RESULT_FAILED;
    }
#else

    if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(req->id, &userStatus[0], cnt))
    {
        CLog::Log("CBusinessGetOnlineStatus",CLog::TYPE_ERROR,"QueryUserStatus error! id:%d ",req->id[0]);
        return RESULT_FAILED;
    }
#endif


    //���ݳ���д�����ݰ�
    m_SendPacket << (uint32_t)cnt;

    //����״̬���飬д�����ݰ�
    for (uint8_t i = 0; i < cnt; ++i)
    {
        m_SendPacket << userStatus[i].id;
        m_SendPacket << uint32_t(userStatus[i].status == XTREAM_OFFLINE ? 0 : 1);
    }

    return RESULT_SUCCESS;
}

CBusinessCheckSubOnline::CBusinessCheckSubOnline(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_SUB_ONLINE_REQ,CMD_GET_SUB_ONLINE_ACK)
{

}
//���û����˺ŵ��ݲ�����memcached�쳣���
int CBusinessCheckSubOnline::DoWork()
{
    XT_GET_SUB_ONLINE_REQ *req = (XT_GET_SUB_ONLINE_REQ*)m_RecvPacket.GetData();

    m_SendPacket << req->cnt;

    for (uint32 i = 0; i < req->cnt; ++i)
    {
        m_SendPacket << req->id[i];
        AccountArray ids;       //���ж�Ӧ�����˺�

        //��ȡ���е����˺�
        MYSQL_LIB->getAllSubAccount(req->id[i], ids);
        //ids.push_back(req->id[i]);

        uint8_t cnt = ids.size();

        //��ʼ���û�״̬��Ϣ
        std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(ids.size());
        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(&ids[0], &userStatus[0], cnt))
        {

            //д�����ݰ�
            m_SendPacket << (uint32_t)0;
            CLog::Log("CBusinessHasSubOnline",CLog::TYPE_ERROR, "can't find %d's sub account", req->id[i]);
        }
        else
        {
            //�ж����˺����Ƿ������ߵ�
            auto ifind = std::find_if(userStatus.begin(), userStatus.begin() + cnt,
                                      [] (const GSRV_GET_USER_STATUS_ACK::tagUserStatus& a) ->bool
            {
                return a.status != XTREAM_OFFLINE;
            });

            //�ж��Ƿ�����
            if (ifind == (userStatus.begin() + cnt))
            {
                m_SendPacket << (uint32_t)0;
            }
            else
            {
                m_SendPacket << (uint32_t)1;
            }
#ifdef DEBUG
            CLog::Log("CBusinessHasSubOnline",CLog::TYPE_DEBUG, "check %d's sub account if is online!", req->id[i]);
#endif
        }
    }
#ifdef DEBUG
    CLog::Log("CBusinessHasSubOnline",CLog::TYPE_DEBUG,"check sub account size %d", req->cnt);
#endif
    return RESULT_SUCCESS;
}

#define AVERAGE_FLOW 2
#define CAPACITY_FLOW 3
CBusinessGetSubAccount::LastAccountMap CBusinessGetSubAccount::account_load_map_;
std::mutex CBusinessGetSubAccount::account_load_mutex_;
CBusinessGetSubAccount::CBusinessGetSubAccount(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_SUB_ACCOUNT_REQ,CMD_GET_SUB_ACCOUNT_ACK)
{

}


int CBusinessGetSubAccount::NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange)    //֪ͨ����������Ա�Ŷ�״�� 0 �����Լ� 1 �������Լ�
{
    CDataBaseMini* pMinibase = m_pRouter->GetDataBaseMini();
    vector<uint32> TeamUser;

    if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(QueueChange->teamId,TeamUser))
    {
        SetErrorReturn("��ȡ��ϸ����ʧ��");
        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", QueueChange->teamId);
        return RESULT_FAILED;
    }
    for(unsigned int i = 0 ; i < TeamUser.size(); ++i )
    {

        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"�ڲ�����:TEAM %d ��ѯ����: TEAM %d ,���͸� %u",
                  QueueChange->teamId,QueueChange->teamId,TeamUser[i]);
        //֪ͨ�������ڵĿͷ�
        {
            SendDataStruct sendData;
            LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
            lpSendHead->cmd      = CMD_SERVERMSG_IND;
            lpSendHead->did      = TeamUser[i];
            lpSendHead->dtype    = XT_HEAD::DEST_TYPE_USER;
            lpSendHead->flag     = XT_HEAD::FLAG_SRVACTION;
            lpSendHead->from     = XT_HEAD::FROM_SERVER;
            CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
            lpSendHead->sid      = CAppManager::GetInstance().GetConfig().GetServerNo();

            LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
            lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
            lpSrvTransmit->msgtype              = CMD_WAITQUEU_CHANGE;


            XT_WAITQUEU_CHANGE* pTeamMatesNotify = (XT_WAITQUEU_CHANGE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
            pTeamMatesNotify->bAdd    = QueueChange->bAdd;
            pTeamMatesNotify->idBuyer = QueueChange->idBuyer;
            pTeamMatesNotify->teamId = QueueChange->teamId;
            strcpy(pTeamMatesNotify->nicknameBuyer,QueueChange->nicknameBuyer);
            pTeamMatesNotify->QueueLength = 0;  //ÿ���ͷ��Ķ��г����ǲ�ͬ�ģ����͵�ʱ������������
            //pFriendStatusNotify->status = m_nStatus;

            sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);
            CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"׼������");

            CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);

        }
    }

    //��memcachedֱ��д��ȫ�ַ������У����ز��������Ϣ
#if 0

    //����ú�����ֱ������������,��ֱ���޸�������б�״̬

    CsQueue * csQueue =  CAppManager::GetInstance().GetCSQueueManager().Find(QueueChange->teamId); //���ҷ����ŶӶ���
    if(!csQueue)
    {
        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"��ѯ�Ŷ�����Ϣʧ�� :%d��", QueueChange->teamId);
    }
    if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
    {
        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"�����û����ŶӶ����� :TEAM %d UID:%u",
                  QueueChange->teamId,QueueChange->idBuyer);
        csQueue->AddToCSQueueList(QueueChange->idBuyer);
    }
    else if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
    {
        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"���Ŷ��û��Ӷ������Ƴ� (����ѯ) :TEAM %d UID:%u",
                  QueueChange->teamId,QueueChange->idBuyer);
        csQueue->DelFromCSQueueList(QueueChange->idBuyer);
    }
#endif


    return RESULT_SUCCESS;

}


int CBusinessGetSubAccount::AddToQueueAndNotify(CUser* User,uint32 buyerId,XT_GET_SUB_ACCOUNT_REQ *req)
{
    //�����ŶӶ���
//  csQueue->AddToCSQueueList(buyerId);
    //��ȫ�ּ����ŶӶ���
    tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
    uint8 count = 1;
    int ret;
    cSinfo.customerID = buyerId;
    cSinfo.teamID = req->group_id;
    time_t tm;
    time(&tm);
    cSinfo.customerLevel = User->m_nLevel;
    cSinfo.customerReqTime = (uint32)tm;        // ��ǰ�Ŷ��û��ȼ�
    User->setCsTeamId(req->group_id);           // ����teamid,�����memcache��ɾ��ʱɾ���� sd add

    CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_ADD user %u!",cSinfo.customerID);
    ret =  m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_ADD);
    if(ret != MEMCACHED_GET_INFO_SUCCESS)
    {
        CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_ERROR,"ȫ�ּ����ŶӶ����û� %u failed!");
    }
    //֪ͨ���пͻ���
    //������Ϊ�������ڴ��л�ȡ�ͷ���Ϣ
    //������Ҫ�ĳɴ�memcached�л�ȡ�������˺���Ϣ����ȥ��
    //ClientService *clientService = CAppManager::GetInstance().GetCSManager().Find(req->merchant_id);
    // if(clientService)     //�ҵ��ͷ���ֱ������
    // {
    XT_WAITQUEU_CHANGE notify;  //���ҿͷ���ֱ��ȫ������
    notify.bAdd = XT_WAITQUEU_CHANGE::TYPE_ADD;
    notify.idBuyer = buyerId;
    notify.teamId = req->group_id;
    //clientService->NotifyTeamQueue(&notify);
    NotifyTeamQueueInfo(&notify);
    // }
    // else//û�ҵ����ؿͷ�
    //  {
    //      CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "δ��ȡ���˺������ģ��������µ�¼�û�");
    //  }

    return 0;
}

int CBusinessGetSubAccount::DoWork()
{
    //��ȡ�������ݰ�ָ��
    XT_GET_SUB_ACCOUNT_REQ *req = (XT_GET_SUB_ACCOUNT_REQ*)m_RecvPacket.GetData();
    uint32 id = 0;
    uint32 QueueLength = 0;     //�����Ŷӣ��Ŷӵ�λ��
    uint32 RevisedQueueLength = 0;
    shared_ptr<CUser> pUser = NULL;
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    uint32 buyerId = m_pRecvHead->sid;
    pUser = pUserManager->Find(buyerId);


    //��ȡ�������ݰ�ָ��
    //XT_GET_SUB_ACCOUNT_ACK *ack = (XT_GET_SUB_ACCOUNT_ACK*)m_SendPacket.GetData();
#if 1
    if(req->type == XT_GET_SUB_ACCOUNT_REQ::CS) //�ǿͷ���ҵ���߼�
    {
        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, uUserId:%u, request id:%u IN" ,
                  m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag,m_pRecvHead->sid);
        //  uint32 buyerId = m_pRecvHead->sid;
        vector<uint32> ids;
        uint8 count = 1;
        int ret = -1;

#if 0
        if(ClientService::m_cshash.size() <= 0)
        {
            CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "��ǰû�пͷ�����");
            id = 0;
            QueueLength = 0xffffffff;
            m_SendPacket << (uint32)id
                         << (uint32)QueueLength;
            return RESULT_SUCCESS;
        }
#endif
        if(pUser == NULL)
        {
            CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "û�����������");
            id = 0;
            QueueLength = 0xffffffff;
            m_SendPacket << (uint32)id
                         << (uint32)QueueLength;
            return RESULT_SUCCESS;

        }
        if((id = pUser->getCsuserId()) > 0 && ((uint32)pUser->getCsTeamId() == req->group_id))
        {
            QueueLength = 0;
            CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "�Ѿ����������û� %u ���ͷ� %u",buyerId,id);
        }
        else
        {
            pUser->setCsTeamId(0);
            pUser->setCsuserId(0);
//            pUserManager->AddUserToRedis(pUser);

            /*
                ģ������


                buyerId = 5000012;
                uint32 team = 100;
                uint32 userlist[]={2000000008,2000000006};
                req->group_id = team;
                req->merchant_id = 2000000008;

                ids.push_back(2000000008);
                ids.push_back(2000000006);


                end     ģ������

            */
            CDataBaseMini *pDatabase = m_pRouter->GetDataBaseMini();

            ret = pDatabase->CS_Get_Group_Account(req->group_id,ids);
            if(ret != DB_SUCCESS)
            {
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Query team %u subaccount failed!",req->group_id);
            }
            else
            {
                for(int i = 0 ; i < (int)ids.size(); i++)
                {

                    CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"Query team %u has member %u !",req->group_id,ids[i]);
                }
            }


            //����  ��ʱ�жϿ��Է���
#ifdef IM_USE_CACHE
            //��ȫ��memcached��ȡ�ŶӶ���

            tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo qinfo;
            ret = m_pRouter->GetGlobalBusi()->QueryCSQueueInfo(&req->group_id,count,&qinfo);
            {
                if(ret != MEMCACHED_GET_INFO_SUCCESS)
                {
                    if(ret == MEMCACHED_NO_INFO)
                    {
                        CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"Query team %u EMPTY!",qinfo.teamID);
                    }
                    else
                    {
                        CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Query team %u fail!",qinfo.teamID);
                        m_SendPacket << (uint32)id
                                     << (uint32)10;
                        return RESULT_SUCCESS;
                    }
                }
            }
            for(int i = 0 ; i < (int)qinfo.m_Customerlist.size(); i++)
            {

                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"before Query team %u has %u level %u requesttime %u!",
                          qinfo.teamID,qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);
            }
            //����
            if(qinfo.m_Customerlist.size() > 0)
            {
                sort(qinfo.m_Customerlist.begin(),qinfo.m_Customerlist.end(),SortQueueMethod);
				CheckAndCleanQueue(m_pRouter,qinfo);
            }
            else
            {
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"No Queue");
            }
            for(int i = 0 ; i < (int)qinfo.m_Customerlist.size(); i++)
            {

                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"after Query team %u has %u level %u requesttime %u!",
                          qinfo.teamID,qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);
            }

#endif
#if 0
            //���汾��һ�£���Ҫ�Ǳ�����
            CSQueueManager & csQueueManager = CAppManager::GetInstance().GetCSQueueManager();
            csQueue = csQueueManager.Find(req->group_id);
            if(csQueue == NULL)
            {
                CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "����һ���µ��ŶӶ��� %u" ,req->group_id);
                csQueue = csQueueManager.AddHash(req->group_id);
                csQueue->setCSID(req->group_id);
            }
#endif


            if(qinfo.m_Customerlist.size() > 0) //���Ŷӣ��ŶӶ�����������
            {
                vector<CSQList_t>::iterator result = find_if(qinfo.m_Customerlist.begin( ), qinfo.m_Customerlist.end(),[buyerId] (const CSQList_t& a) ->bool
                {
                    return (a.userid == (uint32)buyerId);
                }
                                                            ); //����3
                if(( result != qinfo.m_Customerlist.end() ))    //�ڶ������ҵ�
                {
                    //����λ����Ϣ
                    //д�����ݰ�
                    if((qinfo.m_Customerlist[0].userid) == buyerId)     //���ϿɽӴ������Կ��ԽӴ���
                    {
                        int ret = GetCSSubAccount_x(m_pRouter,req->group_id,buyerId,ids,id);
                        if(id != 0)
                        {
                            QueueLength = 0;        //�����ɹ��������Ŷ�
                            pUser = pUserManager->Find(buyerId);
                            pUser->setCsuserId(id);
                            pUser->setCsTeamId(req->group_id);
                            tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID pUTeamID;
                            pUTeamID.id = buyerId;
                            pUTeamID.teamID = req->group_id;
                            uint8 count = 1;
                            CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
                            if(RESULT_FAILED == pGlobalBusi->ModiUserTeamID(&pUTeamID,count))
                            {
                                CLog::Log("CBusinessGetSubAccount", CLog::TYPE_ERROR,"ȫ���޸����û�: %u ʧ��!", buyerId);
                            }

                            //��ȫ��memcachedɾ���ŶӶ���

                            tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
                            cSinfo.customerID = buyerId;
                            cSinfo.teamID = req->group_id;

                            CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"1��ȫ��memcachedɾ���Ŷ��û� %u!",cSinfo.customerID);
                            ret = m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
                            if(ret != MEMCACHED_GET_INFO_SUCCESS)
                            {
                                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
                            }

                        }
                        else if(ret == NO_CS_ONLINE)
                        {
                            QueueLength = -1;
                        }
                        else if(ret == NO_USABLE_CS)
                        {
                            QueueLength = 1;
                        }
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "�����е�һ���������ǣ��Ŷ��û� %d ���г���  %d  OUT", id,QueueLength);
                    }
                    else
                    {
                        QueueLength = result - qinfo.m_Customerlist.begin() + 1;
                        RevisedQueueLength = CalcQueueLength(QueueLength);
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT,
                                  "��ҵȴ���1���Ŷ��û� %u ���г���  %d ����Ϊ%d OUT", buyerId,QueueLength,RevisedQueueLength);
                        m_SendPacket << (uint32)id
                                     << (uint32)RevisedQueueLength;
                        return RESULT_SUCCESS;
                    }
                    m_SendPacket << (uint32)id
                                 << (uint32)QueueLength;

                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT,
                              "��ҵȴ���3���Ŷ��û� %u ���г���  %d  OUT", buyerId,QueueLength);
                    return RESULT_SUCCESS;
                }
                else        //�������Ŷ���Ա������û�д��û�
                {
                    QueueLength = qinfo.m_Customerlist.size() + 1;
                    RevisedQueueLength = CalcQueueLength(QueueLength);
                    AddToQueueAndNotify(pUser.get(),buyerId,req);
                    m_SendPacket << (uint32)id
                                 << (uint32)RevisedQueueLength;

                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT,
                              "��ҵȴ���2���Ŷ��û� %u ���г���  %d ����Ϊ%d OUT", buyerId,QueueLength,RevisedQueueLength);
                    return RESULT_SUCCESS;
                }

            }
            else    //û�Ŷ� ����ô��ʼ����
            {
                int ret = GetCSSubAccount_x(m_pRouter,req->group_id,buyerId,ids,id);
                if(id != 0)
                {
                    pUser = pUserManager->Find(buyerId);
                    if(pUser != NULL)
                    {
                        pUser->setCsuserId(id);
                        pUser->setCsTeamId(req->group_id);
                        QueueLength = 0;        //�����ɹ��������Ŷ�
                        //m_SendPacket << (uint32)id
                        //      << (uint32)0;
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "��ҵȴ���3���Ŷ��û� %u ���г���  %d  OUT", buyerId,QueueLength);
                        //return RESULT_SUCCESS;
                        tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID pUTeamID;
                        pUTeamID.id = buyerId;
                        pUTeamID.teamID = req->group_id;
						pUTeamID.csid = id;
                        uint8 count = 1;
                        CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
                        if(RESULT_FAILED == pGlobalBusi->ModiUserTeamID(&pUTeamID,count))
                        {
                            CLog::Log("CBusinessGetSubAccount", CLog::TYPE_ERROR,"ȫ���޸����û�: %u ʧ��!", buyerId);
                        }
                    }
                    else
                    {
                        id = 0;
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "��ȡ�������%uʧ�� (��� logout) OUT" ,buyerId);
                    }
                }
                else if(ret == NO_CS_ONLINE)
                {
                    QueueLength = -1;
                    m_SendPacket << (uint32)id
                                 << (uint32)QueueLength;
                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "��ҵȴ���4���Ŷ��û� %u ���г���  %d  OUT" ,buyerId,QueueLength);
                    return RESULT_SUCCESS;
                }
                else    //û�пɽӴ����û�---��һ�ο�ʼ�Ŷ�
                {
                    AddToQueueAndNotify(pUser.get(),buyerId,req);
                    QueueLength = 1;
                    m_SendPacket << (uint32)id
                                 << (uint32)QueueLength;
                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "��ҵȴ���5���Ŷ��û� %u ���г���  %d  OUT" ,buyerId,QueueLength);
                    return RESULT_SUCCESS;
                }

            }


        }

    }
    else
#endif
    {
        if(req->group_id == 0 && req->merchant_id == 0)
        {
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"request Error,sid :%d",m_pRecvHead->sid);
            m_SendPacket << (uint32)id
                         << (uint32)0xffffffff;
            return RESULT_SUCCESS;
        }
        AccountArray ids;

        CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();

        //�ж��������ͣ�ѡ��ʹ�õ����ݿ�
        if(req->type == 0)
        {
            MYSQL_LIB->getAllSubAccount(req->merchant_id, ids);
        }
        else
        {
            if(DB_SUCCESS != pDatabase->GetSubAccount(req->group_id,ids))
            {
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"merchant_id:%d ,get SubAccount in SubGroup id:%d failed",req->merchant_id,req->group_id);
            }
        }


        //���������ϵ�˺�
        uint32 ConnAccount = 0;
        std::vector<tagGSRV_GET_USER_FLOW_ACK::tagUserFlow> userFlow(ids.size());
        uint8_t cnt = ids.size();
        if(DB_SUCCESS != pDatabase->SearchConnAccount(m_pRecvHead->sid,ids,ConnAccount))
        {
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Get Flow Mode Error,sid :%d",m_pRecvHead->sid);
        }
        else
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"id:%d ,ConnAccount:%d ",m_pRecvHead->sid,ConnAccount);

        //�����ϵ�˺��Ƿ�ɷ���
        int i = 0;
        int online_num = 0;
        if(m_pRouter->GetGlobalBusi()->QueryUserFlow(&ids[0], &userFlow[0], cnt))
        {
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"id:%d ,Query User Flow Error,ConnAccount:%d ",m_pRecvHead->sid,ConnAccount);
            cnt = 0;
        }
        else
        {
            for(i = 0; i < cnt; i ++)
            {
                if(userFlow[i].status != XTREAM_OFFLINE && userFlow[i].status != XTREAM_LEAVE
                   && userFlow[i].flow == XT_MOD_HANGUP_STATE_REQ::RECEPTION)
                {
                    online_num ++;

                    if(userFlow[i].id == ConnAccount)
                    {
                        id = userFlow[i].id;
                        break;
                    }
                }
            }
        }

        int flow_type = req->type;
        if(i == cnt)//�����ϵ�˺Ų��ɷ���
        {
            if(online_num == 0 && req->type != 0)//�����������˺ſɷ�,���߷���
            {
                ids.clear();
                MYSQL_LIB->getAllSubAccount(req->merchant_id, ids);

                flow_type = 0;//���߷�
            }
        }

        if(id == 0)
        {
            //��ȡ�̼ҷ�����ʽ
            int flow_mode = -1;
            if(DB_SUCCESS != pDatabase->GetFlowMode(req->merchant_id,flow_mode))
            {
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"Get Flow Mode Error! merchant_id:%d",req->merchant_id);
                flow_mode = AVERAGE_FLOW;
            }

            //��ȡ��ѷ������˺�
            if(flow_mode == AVERAGE_FLOW)
            {
                //����

                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"AVERAGE_FLOW mode");
                if(m_pRouter->GetGlobalBusi()->GetBestAccount(req->merchant_id, req->group_id, ids, flow_type, id))
                {
                    CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"merchant_id:%d ,can't find %d's sub account",req->merchant_id,ConnAccount);
                }
            }
            else if(flow_mode == CAPACITY_FLOW)
            {
                //�Ӵ�������
                cnt = ids.size();
                userFlow.resize(ids.size());
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"CAPACITY_FLOW mode");
                if(m_pRouter->GetGlobalBusi()->QueryUserFlow(&ids[0], &userFlow[0], cnt))
                {
                    CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"merchant_id:%d ,Query User Flow Error,ConnAccount:%d ",req->merchant_id,ConnAccount);
                }

                size_t m = 0;
                std::vector<uint32> FlowMember;
                for(m = 0; m < cnt; m ++)
                {

                    CLog::Log("222CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"id:%d ,status :%d userFlow:%d",userFlow[m].id,userFlow[m].status,userFlow[m].flow);
                    if(userFlow[m].status != XTREAM_OFFLINE && userFlow[m].status != XTREAM_LEAVE
                       && userFlow[m].flow == XT_MOD_HANGUP_STATE_REQ::RECEPTION)
                    {

                        CLog::Log("222CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"add :id:%d ,status :%d ",userFlow[m].id,userFlow[m].status);
                        FlowMember.push_back(userFlow[m].id);
                    }
                }

                //��ȡδ�ظ���
                uint32 NoReplyNum = 0;
                uint32 t_NoReplyNum = 0;
                uint32 count = 0;

                std::map<uint32,uint32> ReplyNum_Map;
                std::vector<uint32> ReplyNum_ID;

                time_t tm;
                time(&tm);
                uint32 cur_time = (uint32)tm;

                if(!FlowMember.empty())
                {
                    for(m = 0; m < FlowMember.size(); m ++)
                    {
                        if(DB_SUCCESS != pDatabase->GetNoReplyNum(FlowMember[m],cur_time,NoReplyNum))
                        {
                            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"GetNoReplyNum Error,id:%d ",FlowMember[m]);
                        }
                        else
                        {
                            if(count == 0)
                            {
                                t_NoReplyNum = NoReplyNum;
                                count ++;
                            }

                            if(NoReplyNum < t_NoReplyNum)
                                t_NoReplyNum = NoReplyNum;

                            ReplyNum_Map.insert(make_pair(FlowMember[m],NoReplyNum));
                        }
                    }

                    for(std::map<uint32,uint32>::iterator iter = ReplyNum_Map.begin(); iter != ReplyNum_Map.end(); ++ iter)
                    {
                        if(t_NoReplyNum == iter->second)
                            ReplyNum_ID.push_back(iter->first);
                    }

                    //�������
                    int rand_size = 0;

                    srand((unsigned)time(NULL));

                    rand_size = ReplyNum_ID.size();
                    if(rand_size != 0)
                        id = ReplyNum_ID[rand()%rand_size];
                    else
                        id = 0;

                    CLog::Log("CBusinessGetSubAccount",CLog::TYPE_DEBUG," ReplyNum_ID:%d ",id);
                }
            }
        }
    }

    if(id == 0 && (req->type != XT_GET_SUB_ACCOUNT_REQ::CS)) //�ǿͷ�����ʧ�ܣ�ȡ�������˺�
    {
        CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
        if( DB_SUCCESS != pDatabase->Get_Agent_Merchant(req->merchant_id,id))
        {
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"��ȡ�����̼�ʧ�� :%d",id);
            MYSQL_LIB->getMainAccount(req->merchant_id, id);
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"��ȡ���̼�ID :%d",id);
        }

        CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"��ȡ�����̼�:%u",id);
    }

    /*�������ʺŻ�����Ϣ*/
    char    strEmail[MAX_EMAIL_LEN+1] = "";
    char    strUsername[MAX_USERNAME_LEN+1] = "";   // �û���
    char    strNickname[MAX_NICKNAME_LEN+1] = "";   // �س�
    char    strMobile[MAX_PHONE_LEN+1] = "";            // �ֻ�
    char    usersign[MAX_USERSIGN_LEN+1] = "";      //����ǩ��
    char    face_url[MAX_HEADURL_LEN+1] = "";       //ͷ����Ϣ

    if(id != 0)  //����̨ʧ���Ƿ���-1������ʧ�ܷ��ص���0
    {
        //  CUser* pUser = NULL;
        //      CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();
        pUser = pUserManager->Find(id);
        if( NULL == pUser || (!pUser->m_bIsFullInfoValid) )
        {
            XT_SERVER_USERINFO_GET_ACK UserInfo;
            //˵�����û����ڱ���������,�����ݿ��в���
            CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
            if( DB_SUCCESS != pDatabase->Get_User_Info(id,UserInfo))
            {
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Get_User_Info Error:%d",id);
            }
            else
            {
                strcpy(strEmail, UserInfo.email);
                strcpy(strUsername, UserInfo.username);
                strcpy(strMobile, UserInfo.mobile);
                strcpy(strNickname, UserInfo.nickname);
                strcpy(usersign, UserInfo.usersign);
                strcpy(face_url, UserInfo.face_url);

                CLog::Log("CBusinessGetSubAccount1",
                          CLog::TYPE_DEBUG,
                          "get %d' or %d's sub account,id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s,face_url %s",
                          req->merchant_id,req->group_id, id , strEmail,strUsername, strMobile,strNickname,usersign,face_url);
            }

        }
        else
        {
            strcpy(strEmail, pUser->m_szEmail);
            strcpy(strUsername, pUser->m_szUserName);
            strcpy(strMobile, pUser->m_szPhone);
            strcpy(strNickname, pUser->m_szNickName);
            strcpy(usersign, pUser->m_szUserSign);
            strcpy(face_url,pUser->m_szHeadURL);
            CLog::Log("CBusinessGetSubAccount2",
                      CLog::TYPE_DEBUG,
                      "get %d' or %d's sub account,id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s,face_url %s",
                      req->merchant_id,req->group_id, id , strEmail,strUsername, strMobile,strNickname,usersign,face_url);
        }

    }
    else
    {
        CLog::Log("CBusinessGetSubAccount3",
                  CLog::TYPE_DEBUG,
                  "get %d' or %d's sub account,id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s",
                  req->merchant_id, req->group_id,id , strEmail,strUsername, strMobile,strNickname,usersign);
    }

    //д�����ݰ�
    m_SendPacket << (uint32)id
                 << (uint32)QueueLength
                 << strEmail
                 << strUsername
                 << strMobile
                 << strNickname
                 << usersign
                 << face_url;

    CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT,
              "sid:%u, did:%x, flag:%u, id:%u, email:%s, mobile:%s, nickname:%s,usersign %s face_url %s OUT" ,
              m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag,
              id, strEmail, strUsername, strMobile, strNickname,usersign,face_url);
    return RESULT_SUCCESS;
}

CBusinessGetFriendBasicInfo::CBusinessGetFriendBasicInfo(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_FRIEND_BASICINFO_REQ,CMD_GET_FRIEND_BASICINFO_ACK)
{
    m_pNotifyData = new SendDataStruct;
    m_NotifyPacket.SetData( m_pNotifyData->data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),
                            MAX_SEND_BUFFERS - sizeof(XT_HEAD) - sizeof(XT_SERVER_TRANSMIT));

    m_pFriendInfo = new XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo[MAX_QUERY_BASIC_INFO_COUNT];
}

CBusinessGetFriendBasicInfo::~CBusinessGetFriendBasicInfo()
{
    SAFE_DELETE(m_pNotifyData);

    if( m_pFriendInfo )
    {
        delete[] m_pFriendInfo;
        m_pFriendInfo = NULL;
    }
}

int CBusinessGetFriendBasicInfo::DoWork()
{
    XT_GET_FRIEND_BASICINFO_REQ* pReq  = (XT_GET_FRIEND_BASICINFO_REQ*)m_RecvPacket.GetData();
    XT_GET_FRIEND_BASICINFO_ACK* pHead = (XT_GET_FRIEND_BASICINFO_ACK*)m_SendPacket.GetData();
    pHead->ret = RESULT_SUCCESS;
    m_SendPacket.SetCursor(sizeof(XT_GET_FRIEND_BASICINFO_ACK));

    //����һ���򵥵�Ӧ����û�
    SendReply();

    //֪ͨ��ܲ㲻��Ҫ�ٷ�Ӧ����.�ڲ��Ѿ���Ӧ��
    CancelReply();

    //����ͨ��֪ͨ�·����ѻ������ϸ��û�
    //CUser* pUser = NULL;
    //CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();

    BYTE FriendInfoBuf[sizeof(XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo)+6*sizeof(uint32)];
    CSendPacket FriendPacket((char*)FriendInfoBuf);

    InitNotifyHead();

    CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
    CDataBaseMini* pDBMini= m_pRouter->GetDataBaseMini();
    uint16 NextReturnPos = 0 ;
    uint16 nRetCount = 0;
    uint32 QueryId = pReq->type == pReq->TYPE_FRIEND ? m_pRecvHead->sid : 0;

    if (pReq->count > 300  )
        return RESULT_FAILED;

    //for(int a=0;a<100;a++)
    //{
    //CLog::Log("CBusinessGetFriendBasicInfo",CLog::TYPE_IMPORTANT,"pReq->FriendID :%u",pReq->FriendID[a]);

    //}

    while(NextReturnPos < pReq->count)
    {

        if( nRetCount == 0 )
        {
            nRetCount = min(MAX_QUERY_BASIC_INFO_COUNT,pReq->count - NextReturnPos);
            memset(m_pFriendInfo, 0, sizeof(XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo)*nRetCount);

            if( DB_SUCCESS != pDatabase->Get_Basic_UserInfo(QueryId ,pReq->FriendID + NextReturnPos,m_pFriendInfo,nRetCount) )
            {
                NextReturnPos += MAX_QUERY_BASIC_INFO_COUNT;
                continue;
            }
            else
            {
                NextReturnPos += MAX_QUERY_BASIC_INFO_COUNT;
            }
        }

        int nfindex = 0;
        while( nRetCount > 0 )
        {
            XT_SERVER_CS_GET_ACK lpAck;
            if(m_pFriendInfo[nfindex].nID>= cs_Boundary && pDBMini->Get_CS_Info(m_pFriendInfo[nfindex].nID,lpAck) != DB_ERROR)
            {
                // �õ�Ȩ�� // ����2000000000��ʾ�ͷ�����̨�汾�еĿͷ���Ա����Ҫ��ѯ����Ȩ�޸��ͻ���
                m_pFriendInfo[nfindex].LoveFlag = 1-lpAck.m_permission;     // ����Ȩ��ͨ��LoveFlag�ֶδ����ͻ���
            }
            //CLog::Log("CBusinessGetFriendBasicInfo",CLog::TYPE_IMPORTANT,"nRetCount:%d",nRetCount);
            FriendPacket<< m_pFriendInfo[nfindex].nID
                        << m_pFriendInfo[nfindex].szHeadUrl
                        << m_pFriendInfo[nfindex].nSexy
                        << m_pFriendInfo[nfindex].szNickName
                        << m_pFriendInfo[nfindex].szUserName
                        << m_pFriendInfo[nfindex].szCommentName
                        << m_pFriendInfo[nfindex].LoveFlag
                        << m_pFriendInfo[nfindex].szUserSign
                        << m_pFriendInfo[nfindex].mobileStatus;
            if(strcmp(m_pFriendInfo[nfindex].szGuid, "") == 0)
            {
                FriendPacket<< m_pFriendInfo[nfindex].szGuid;
            }
            else
            {
                string strKey = CAppManager::GetInstance().GetConfig().GetAesKey();
                char szAesGuid[128] = {0};
                int nAesGuidLen = 128;
                sprintf_s(m_pFriendInfo[nfindex].szGuid + 36, "%d", m_pRecvHead->sid);
                int rlt = AesEncode(strKey.c_str(), strKey.length(), m_pFriendInfo[nfindex].szGuid, strlen(m_pFriendInfo[nfindex].szGuid), szAesGuid, nAesGuidLen);
                if(rlt == -1)
                {
                    CLog::Log("CBusinessGetFriendBasicInfo", CLog::TYPE_ERROR, "GUID AES E Failed");
                    FriendPacket<< m_pFriendInfo[nfindex].szGuid;
                }
                else
                {
                    string strBase64Guid = Base64Encode(szAesGuid, nAesGuidLen);
                    CLog::Log("CBusinessGetFriendBasicInfo", CLog::TYPE_IMPORTANT, "GUID AES:%s", strBase64Guid.c_str());
                    FriendPacket<< strBase64Guid.c_str();
                }

            }
            CLog::Log("CBusinessGetFriendBasicInfo", CLog::TYPE_IMPORTANT, "queryid:%d, id:%d, headurl:%s, sexy:%d, nickname:%s, username:%s, commentname:%s, loveflag:%d, usersign:%s, mobilestatus:%d, guid:%s",
                      QueryId,m_pFriendInfo[nfindex].nID, m_pFriendInfo[nfindex].szHeadUrl, m_pFriendInfo[nfindex].nSexy,
                      m_pFriendInfo[nfindex].szNickName, m_pFriendInfo[nfindex].szUserName, m_pFriendInfo[nfindex].szCommentName,
                      m_pFriendInfo[nfindex].LoveFlag, m_pFriendInfo[nfindex].szUserSign, m_pFriendInfo[nfindex].mobileStatus,
                      m_pFriendInfo[nfindex].szGuid);

            ++nfindex;
            --nRetCount;

            if( !AddToNotifyPacket(&FriendPacket) )
            {
                //��װ����,���ͳ�ȥ
                SendNofity();

                //Ϊ��һ����׼��
                InitNotifyHead();
                AddToNotifyPacket(&FriendPacket);

            }

            FriendPacket.Reset();
        }


    }

    //�����һ��
    SendNofity();

    return RESULT_SUCCESS;
}

void CBusinessGetFriendBasicInfo::InitNotifyHead()
{
    LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pNotifyData->data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        0,
        m_pRecvHead->sid);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(m_pNotifyData->data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_GET_FRIEND_BASICINFO_NOTIFY;

    m_NotifyPacket.Reset();

    //���������λ��
    m_pCount  = (uint16*)m_NotifyPacket.GetData();
    m_NotifyPacket << (uint16)0;        //���صĺ������ϸ���,��ʱ���ֵ
}

//
bool CBusinessGetFriendBasicInfo::AddToNotifyPacket(CSendPacket* pTempPacket)
{
    if( m_NotifyPacket.HasData(pTempPacket->GetSize()) )
    {
        m_NotifyPacket.WritePacket(pTempPacket->GetData(),pTempPacket->GetSize());
        ++(*m_pCount);
        return true;
    }

    return false;
}

void CBusinessGetFriendBasicInfo::SendNofity()
{
	CLog::Log("CBusinessGetFriendBasicInfo::SendNofity", CLog::TYPE_IMPORTANT, "������ %d", *m_pCount);
    if( 0 !=  *m_pCount)
    {
        //LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pNotifyData->data;
        m_pNotifyData->data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_NotifyPacket.GetSize());

        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            m_pNotifyData->data,m_pNotifyData->data_len,SEND_RETRY_TIME);
    }
}

CBusinessGetUserFamilyFlag::CBusinessGetUserFamilyFlag(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_USERFAMILYFLAG_GET_REQ,CMD_USERFAMILYFLAG_GET_ACK)
{

}
int CBusinessGetUserFamilyFlag::DoWork()
{
    XT_USERFAMILYFLAG_GET_REQ* lpReq = (XT_USERFAMILYFLAG_GET_REQ *)m_RecvPacket.GetData();
    XT_USERFAMILYFLAG_GET_ACK* lpAck = (XT_USERFAMILYFLAG_GET_ACK *)m_SendPacket.GetData();

    //CUser* pUser = CAppManager::GetInstance().GetUserManager()->Find();

    m_pRouter->GetDataBaseWeb()->Get_User_FamilyFlag(lpReq->userID, lpAck->JoinFlag, lpReq->count);
    lpAck->ret = RESULT_SUCCESS;
    lpAck->count = lpReq->count;
    m_SendPacket.SetCursor(sizeof(lpAck->JoinFlag)*(lpAck->count - 1) + sizeof(XT_USERFAMILYFLAG_GET_ACK));

    return RESULT_SUCCESS;
}
//---------------------------------------------------------------------

CBusinessGetFriendFullInfo::CBusinessGetFriendFullInfo(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_USERINFO_GET_REQ,CMD_USERINFO_GET_ACK)
{

}


int CBusinessGetFriendFullInfo::DoWork()
{
	XT_USERINFO_GET_REQ UserInfoReq;
	m_RecvPacket>>UserInfoReq.fid;
    XT_USERINFO_GET_REQ* pUserInfoReq = &UserInfoReq;
    XT_SERVER_USERINFO_GET_ACK* pUserInfoAck = (XT_SERVER_USERINFO_GET_ACK*)m_SendPacket.GetData();

    uint32  nClientID;
    shared_ptr<CUser> pUser = NULL;
    CUserManager *pUserManager = m_pRouter->GetUserManager();

    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        m_SendPacket << (uint8)0;       //�������
        nClientID = UserInfoReq.fid;
        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT, "GetFriendFullInfo��%d, IN", nClientID);
    }
    else
    {
        pUserInfoAck->ret = 0;
        nClientID = pUserInfoReq->fid;
    }

    pUser = pUserManager->Find(nClientID);
    if (NULL == pUser && nClientID == m_pRecvHead->sid)
        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "����δ��¼�û���ѯ�Լ�����ϸ��Ϣ��%d", nClientID);

    if( NULL == pUser ||(!pUser->m_bIsFullInfoValid))
    {
        XT_SERVER_USERINFO_GET_ACK UserInfo;
        //˵�����û����ڱ���������,�����ݿ��в���
        CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
#ifndef _USE_MYSQL
        if( DB_SUCCESS != pDatabase->Get_User_Info(nClientID,UserInfo))
#else
        if(DB_SUCCESS != MYSQL_LIB->getUserInfo(nClientID,UserInfo))
#endif
        {
            SetErrorReturn("��ȡ��ϸ����ʧ��");
            return RESULT_FAILED;
        }

		// �������ң�����http�ӿڻ�ȡһ�������Ϣ
    	char szMobileNo[MAX_PHONE_LEN + 1]="";
    	char szEmail[MAX_EMAIL_LEN+1]="";
    	char szNickName[61]="";
    	char szHeadImgUrl[MAX_HEADURL_LEN+1] ="";
    	if(nClientID >= Id_Boundary && nClientID < cs_Boundary)
    	{
    		if(GetBuyerInfo(UserInfo.guid, szMobileNo, szEmail, szNickName, szHeadImgUrl))
           	{
               	CLog::Log("CBusinessGetFriendFullInfo",CLog::TYPE_IMPORTANT,
                   	      "email:%s, mobileno:%s, szNickname:%s, szHeadImgFile:%s",
                       	  szEmail, szMobileNo, szNickName, szHeadImgUrl);
				if(strcmp(szNickName, "") != 0)
				{
					strncpy(UserInfo.nickname, szNickName, MAX_NICKNAME_LEN);
				}
           	}
    	}
        
        m_pRouter->GetDataBaseMini()->Get_User_OnlineTime(nClientID,UserInfo.online_time);
//printf("\n-------------------------UserInfo.description=\n%s-----------------------------------------------------------\n",UserInfo.description);
        if(NULL == pUser )
        {
            if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
            {
                m_SendPacket<< nClientID
                            << UserInfo.username
                            << UserInfo.nickname
                            << UserInfo.email
                            << UserInfo.birthday
                            << UserInfo.sex
                            << UserInfo.province
                            << UserInfo.country
                            << UserInfo.city
                            << UserInfo.face_url
                            << UserInfo.career
                            << UserInfo.rule
                            << UserInfo.familyflag
                            << UserInfo.gold_money
                            << UserInfo.online_time
                            << UserInfo.linkpop
                            << UserInfo.address
                            << UserInfo.mobile
                            << UserInfo.description
                            << UserInfo.usersign
                            << UserInfo.avatoruri
                            << UserInfo.mobilestatus
                            << UserInfo.integral;

                if(strcmp(UserInfo.guid, "") == 0)
                {
                    m_SendPacket<< UserInfo.guid;
                }
                else
                {
                    string strKey = CAppManager::GetInstance().GetConfig().GetAesKey();
                    char szAesGuid[128] = {0};
                    int nAesGuidLen = 128;
                    sprintf_s(UserInfo.guid + 36, "%d", m_pRecvHead->sid);
                    int rlt = AesEncode(strKey.c_str(), strKey.length(), UserInfo.guid, strlen(UserInfo.guid), szAesGuid, nAesGuidLen);
                    if(rlt == -1)
                    {
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "GUID AES E Failed");
                        m_SendPacket<< UserInfo.guid;
                    }
                    else
                    {
                        string strBase64Guid = Base64Encode(szAesGuid, nAesGuidLen);
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT, "GUID AES:%s", strBase64Guid.c_str());
                        m_SendPacket<< strBase64Guid.c_str();
                    }
                }
            }
            else
            {
                pUserInfoAck->id = nClientID;
                strcpy(pUserInfoAck->username, UserInfo.username);
                strcpy(pUserInfoAck->nickname, UserInfo.nickname);
                strcpy(pUserInfoAck->email, UserInfo.email);
                strcpy(pUserInfoAck->birthday, UserInfo.birthday);
                pUserInfoAck->sex = UserInfo.sex;
                pUserInfoAck->province = UserInfo.province;
                pUserInfoAck->country = UserInfo.country;
                pUserInfoAck->city = UserInfo.city;
                strcpy(pUserInfoAck->face_url, UserInfo.face_url);
                pUserInfoAck->career = UserInfo.career;
                pUserInfoAck->rule = UserInfo.rule;
                pUserInfoAck->familyflag = UserInfo.familyflag;
                pUserInfoAck->gold_money = UserInfo.gold_money;
                pUserInfoAck->online_time = UserInfo.online_time;
                pUserInfoAck->linkpop = UserInfo.linkpop;
                strcpy(pUserInfoAck->address, UserInfo.address);
                strcpy(pUserInfoAck->mobile, UserInfo.mobile);
                strcpy(pUserInfoAck->description, UserInfo.description);
                strcpy(pUserInfoAck->usersign, UserInfo.usersign);
                strcpy(pUserInfoAck->avatoruri, UserInfo.avatoruri);
                pUserInfoAck->mobilestatus = UserInfo.mobilestatus;
                pUserInfoAck->integral = UserInfo.integral;
                m_SendPacket.SetCursor(sizeof(XT_SERVER_USERINFO_GET_ACK));
            }

            CLog::Log("CBusinessGetFriendFullInfo::DoWork",CLog::TYPE_IMPORTANT,
                      "pUser = NULL id:%d, username:%s, nickname:%s, email:%s, birthday:%s,sex:%d, province:%d,country:%d,\
city:%d, face_url:%s, career:%d, rule:%d, familyflag:%d, gold_money:%d, online_time:%d,linkpop:%d, address:%s,\
mobile:%s, description:%s, usersign:%s, avatoruri:%s, mobilestatus:%d, integral:%d, guid:%s groupname %s, OUT",
                      nClientID, UserInfo.username, UserInfo.nickname, UserInfo.email, UserInfo.birthday, UserInfo.sex, UserInfo.province,
                      UserInfo.country, UserInfo.city, UserInfo.face_url, UserInfo.career, UserInfo.rule, UserInfo.familyflag, UserInfo.gold_money,
                      UserInfo.online_time, UserInfo.linkpop, UserInfo.address, UserInfo.mobile, UserInfo.description, UserInfo.usersign, UserInfo.avatoruri,
                      UserInfo.mobilestatus, UserInfo.integral, UserInfo.guid,UserInfo.workGroup);
        }
        else
        {
            strcpy(pUser->m_szUserName,UserInfo.username);
            strcpy(pUser->m_szNickName,UserInfo.nickname);
            strcpy(pUser->m_szEmail,UserInfo.email);
            strcpy(pUser->m_szBirthday,UserInfo.birthday);
            pUser->m_nSexy      = UserInfo.sex;
            pUser->m_nProvince  = UserInfo.province;
            pUser->m_nCountry   = UserInfo.country;
            pUser->m_nCity      = UserInfo.city;
            strcpy(pUser->m_szHeadURL,UserInfo.face_url);
            pUser->m_nCareer    = UserInfo.career;
            pUser->m_AllowRule  = UserInfo.rule;
            pUser->m_FamilyFlag = UserInfo.familyflag;
            pUser->m_nMoney     = UserInfo.gold_money;
            pUser->m_nOnlineTime= UserInfo.online_time;
            pUser->m_linkpop    = UserInfo.linkpop;
            strcpy(pUser->m_szAddress,UserInfo.address);
            strcpy(pUser->m_szPhone,UserInfo.mobile);
            strcpy(pUser->m_szDescription,UserInfo.description);
            strcpy(pUser->m_szUserSign, UserInfo.usersign);
            strcpy(pUser->m_szAvatoruri, UserInfo.avatoruri);
            pUser->m_nMobileStatus = UserInfo.mobilestatus;
            pUser->m_nIntegral = UserInfo.integral;
            strcpy(pUser->m_szGUID,UserInfo.guid);
			pUser->m_nCSRecptionLimit = UserInfo.csreceptionlimit;
            pUser->m_bIsFullInfoValid = true;
			strcpy(pUser->m_nWorkGroup,UserInfo.workGroup);
//			pUserManager->AddUserToRedis(pUser);
            //��ȫ�ַ��������¸��û���������
            uint8 flag = m_pRecvHead->flag;
#if 0
            uint8 count = 1;


            tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo GlobalUserInfo;
            GlobalUserInfo.id     = pUser->m_nID;
            strcpy_s(GlobalUserInfo.username,pUser->m_szUserName);
            strcpy_s(GlobalUserInfo.nickname,pUser->m_szNickName);
            GlobalUserInfo.clientip     = pUser->m_nIP;
            GlobalUserInfo.clientport   = pUser->m_nPort;
            memcpy(GlobalUserInfo.p2psessionkey,pUser->m_P2PSessionKey,MAX_SESSIONKEY_LEN);
            memcpy(GlobalUserInfo.sessionkey,pUser->m_SessionKey,MAX_SESSIONKEY_LEN);
            GlobalUserInfo.rule   = pUser->m_AllowRule;
            GlobalUserInfo.serverip=inet_addr(CAppManager::GetInstance().GetConfig().GetinListenAddr());
            GlobalUserInfo.sexy   = pUser->m_nSexy;
            GlobalUserInfo.status = pUser->m_nStatus;
            GlobalUserInfo.lanip    = pUser->m_nLanIP;
            GlobalUserInfo.logintime = pUser->m_nLoginTime;
            //GlobalUserInfo.lanport    = pUser->m_nLanPort;

            if (pUser->m_nStatus == XTREAM_OFFLINE)
                CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d!", pUser->m_nID);

            CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
            if(RESULT_FAILED == pGlobalBusi->AddModiUserInfo(&GlobalUserInfo,count,NULL,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag))
            {
                CLog::Log("CBusinessGetFriendFullInfo->AddModiUserInfo", CLog::TYPE_ERROR,"ȫ��ע���û�: %d ʧ��!", pUser->m_nID);
                pGlobalBusi->AddModiUserInfo(&GlobalUserInfo,count,NULL,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag);
            }
#endif

			/*  modify by zhangmin
            int LogoutCount=0;
	
            while(LogoutCount < 10)
            {
                if( ! pUserManager->UserLogoutLockHandle(pUser->m_nID, CUserManager::LOGOUT_LOCK_GET))
                {
                    break;
                }
                LogoutCount++;
                CLog::Log("CBusinessGetFriendFullInfo",CLog::TYPE_IMPORTANT,"�û� %d����ɾ��������,�ȴ��� %d!",pUser->m_nID,LogoutCount);
                sleep(1);
            }
            */
            CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
			if(pUser->m_nID < Id_Boundary || pUser->m_nID >= cs_Boundary)
            {	// ��ȡ�����Ϣʱ��ˢredis
            	if(RESULT_FAILED == pGlobalBusi->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag))
            	{
                	CLog::Log("CBusinessGetFriendFullInfo->AddModiUserInfo", CLog::TYPE_ERROR,"ȫ��ע���û�: %d ʧ��!", pUser->m_nID);
                	pGlobalBusi->AddModiUserInfo(pUser.get(),0,flag);
            	}
			}
#ifdef IM_USE_IMAPI
				//3���ͷ�����,service_id���
				if(pUser->m_nUsertype == XT_LOGIN_REQ::CS)
				{
					imapi csOnlineReq;
					string info;
					csOnlineReq.imapiReq(imapi::CSONLINEREQ,info,nClientID);
				}
#else			

//ͨ����ѯ���ݿ������ӱ����Լ�ȫ�ַ������Ŀͷ���Ϣ
            if(pUser->m_nUsertype == XT_LOGIN_REQ::CS)
            {
                ClientService* clientServer = NULL;

                CSManager& csManager = CAppManager::GetInstance().GetCSManager();
                clientServer = csManager.Find(nClientID);
                if(clientServer == NULL)
                {
                    CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "����û������ͷ�����Ϣ %u!", nClientID);
                }
                else
                {

                    XT_SERVER_CS_GET_ACK CSInfo;

                    CDataBaseMini* pMinibase = m_pRouter->GetDataBaseMini();
                    if( DB_SUCCESS != pMinibase->Get_CS_Info(nClientID,CSInfo))
                    {
                        SetErrorReturn("��ȡ�ͷ���Ϣʧ��");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "Get_CS_Info %u failed!", nClientID);
                        return RESULT_FAILED;
                    }

                    CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT,
                              "Get_CS_Info %u �û���%s �û�״̬%d ���Ӵ��� %d Ȩ�� %d!",
                              nClientID,CSInfo.m_CSname,CSInfo.m_State,CSInfo.m_MaxRecConsult,CSInfo.m_permission);
                    if(CSInfo.m_CSname[0] == 0)
                    {
                        SetErrorReturn("�˺Ų�����");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "clientServer %u m_State %d",nClientID,clientServer->m_State);
                        return RESULT_FAILED;
                    }
                    strcpy(clientServer->m_CSname,pUser->m_szUserName);
                    clientServer->m_State = (uint32)CSInfo.m_State;

                    if(clientServer->m_State == 0)
                    {
                        SetErrorReturn("�˺���ͣ��");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "clientServer %u m_State %d",nClientID,clientServer->m_State);
                        return RESULT_FAILED;
                    }

                    clientServer->m_MaxRecConsult = CSInfo.m_MaxRecConsult; //��ѯ���ݿ⣬��ȡ�ͷ�����needfix duyong
                    clientServer->m_Permission = CSInfo.m_permission;
                    clientServer->m_CurReception = 0;
                    clientServer->m_CurConsult = 0;
                    clientServer->m_Customerlist.clear();
                    clientServer->m_teamMates.clear(); //��ѯ���ݿ⣬��ȡ�ͷ�����needfix duyong


                    if( DB_SUCCESS != pMinibase->CS_Get_Group_Friend(nClientID,clientServer->m_teamMates))
                    {
                        SetErrorReturn("��ȡ��ϸ����ʧ��");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "CS_Get_Group_Friend %u failed!", nClientID);
                        return RESULT_FAILED;
                    }


                    //��ѯ���ݿ⣬��ȡ�ͷ�����needfix duyong


                    CSinfo_t csInfo;
                    csInfo.m_nID = clientServer->m_nID;
                    strcpy(csInfo.m_Csname,pUser->m_szUserName);
                    csInfo.m_State = pUser->m_nStatus;
                    csInfo.m_MaxRecConsult = clientServer->m_MaxRecConsult;
                    csInfo.m_CurReception = clientServer->m_CurReception;
                    csInfo.m_CurConsult = clientServer->m_CurConsult;
                    csInfo.m_nChattingBuyerID = clientServer->GetChattingBuyerID();
                    csInfo.m_nDirectorID = clientServer->GetDirectorID();
                    csInfo.m_Customerid.clear();
#ifdef IM_USE_CACHE
                    uint8 count = 1;
                    CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
                    if(RESULT_FAILED == pGlobalBusi->AddModiCSInfo(&csInfo,count,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD))
                    {
                        CLog::Log("CBusinessGetFriendFullInfo->AddModiCSInfo", CLog::TYPE_ERROR,"ȫ��ע���û�: %d ʧ��!", pUser->m_nID);
                        pGlobalBusi->AddModiCSInfo(&csInfo,count,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD);
                    }
#endif
                    CLog::Log("CBusinessGetFriendFullInfo->AddModiCSInfo",
                              CLog::TYPE_IMPORTANT,"ȫ��ע��ͷ� : %u �û���Ϊ %s �û�״̬ %d �û����Ӵ��� %d  �û���ǰ�Ӵ��� %d  �û���ǰ��ѯ�� %d !",
                              csInfo.m_nID,csInfo.m_Csname,csInfo.m_State,csInfo.m_MaxRecConsult,
                              csInfo.m_CurReception,csInfo.m_CurConsult);

                    for(int i = 0 ; i < (int)clientServer->m_teamMates.size(); i++)
                    {
                        CLog::Log("CBusinessGetFriendFullInfo->AddModiCSInfo", CLog::TYPE_IMPORTANT,"ȫ��ע��ͷ�������Ϣ: %u ��ǰ�� %u ���� %u!",
                                  csInfo.m_nID,clientServer->m_teamMates[i].teamid,clientServer->m_teamMates[i].userid);

                    }

                    //�¿ͷ���¼����һ����û���Ŷӳ��Է���
                    {
                        //��ȡ�ͷ������飬ÿ���鶼����
                        vector<uint32> groups;
                        CDataBaseMini *pDatabase = m_pRouter->GetDataBaseMini();
                        int ret = pDatabase->CS_Get_Groups(pUser->m_nID,groups);
                        if(ret != DB_SUCCESS)
                        {
                            CLog::Log("CBusinessGetFriendFullInfo",CLog::TYPE_ERROR,"Query CS %u's groups failed!",pUser->m_nID);
                            return RESULT_FAILED;
                        }
                        else
                        {
                            groups.push_back(0);    //������0����Ŷ�
                            for(int i = 0 ; i < (int)groups.size(); i++)
                            {
                                CLog::Log("CBusinessGetFriendFullInfo",CLog::TYPE_IMPORTANT,
                                          "�¿ͷ����ߣ���������������Query CS %u's in group %u!",
                                          pUser->m_nID,groups[i]);

                                PushQueueSubAccountToKafka(m_pRouter,groups[i]);
                            }
                        }


                    }

                }
            }
#endif
            ReturnUserInfo(pUser.get(), pUserInfoAck);
        }

    }
    else
    {

        //�ڷ������ϣ�ֱ�ӷ����ڴ�ֵ
        ReturnUserInfo(pUser.get(), pUserInfoAck);
    }


    return RESULT_SUCCESS;
}

void CBusinessGetFriendFullInfo::ReturnUserInfo(CUser* pUser, XT_SERVER_USERINFO_GET_ACK* pUserInfoAck)
{
    CDataBaseMini* pDBMini= m_pRouter->GetDataBaseMini();
    XT_SERVER_CS_GET_ACK lpAck;
    if(pUser->m_nID>= cs_Boundary && pUser->m_nID < work_Boundary && pDBMini->Get_CS_Info(pUser->m_nID,lpAck) == DB_SUCCESS)
    {
        // �õ�Ȩ�� // ����2000000000��ʾ�ͷ�����̨�汾�еĿͷ���Ա����Ҫ��ѯ����Ȩ�޸��ͻ���
        pUser->m_nIntegral = 1-lpAck.m_permission;      // ����Ȩ��ͨ��LoveFlag�ֶδ����ͻ���
		// ����Ȩ��ͬ����redis
		if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0))
        {
            CLog::Log("CBusinessGetFriendFullInfo::ReturnUserInfo", CLog::TYPE_ERROR,"�ͷ���Ϣд��redisʧ��", pUser->m_nID);
        }
        else
        {
        	CLog::Log("CBusinessGetFriendFullInfo::ReturnUserInfo", CLog::TYPE_ERROR,"�ͷ���Ϣд��redis�ɹ�", pUser->m_nID);
        }
    }

    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        m_SendPacket<< pUser->m_nID
                    << pUser->m_szUserName
                    << pUser->m_szNickName
                    << pUser->m_szEmail
                    << pUser->m_szBirthday
                    << pUser->m_nSexy
                    << pUser->m_nProvince
                    << pUser->m_nCountry
                    << pUser->m_nCity
                    << pUser->m_szHeadURL
                    << pUser->m_nCareer
                    << pUser->m_AllowRule
                    << pUser->m_FamilyFlag
                    << pUser->m_nMoney
                    << pUser->m_nOnlineTime
                    << pUser->m_linkpop
                    << pUser->m_szAddress
                    << pUser->m_szPhone
                    << pUser->m_szDescription
                    << pUser->m_szUserSign
                    << pUser->m_szAvatoruri
                    << pUser->m_nMobileStatus
                    << pUser->m_nIntegral;

        if(strcmp(pUser->m_szGUID, "") == 0)
        {
            m_SendPacket<< pUser->m_szGUID;
        }
        else
        {
            string strKey = CAppManager::GetInstance().GetConfig().GetAesKey();
            char szAesGuid[128] = {0};
            int nAesGuidLen = 128;

            sprintf_s(pUser->m_szGUID + 36, "%d", m_pRecvHead->sid);
            int rlt = AesEncode(strKey.c_str(), strKey.length(), pUser->m_szGUID, strlen(pUser->m_szGUID), szAesGuid, nAesGuidLen);
            if(rlt == -1)
            {
                CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "GUID AES E Failed");
                m_SendPacket<< pUser->m_szGUID;
            }
            else
            {
                string strBase64Guid = Base64Encode(szAesGuid, nAesGuidLen);
                CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT, "GUID AES:%s", strBase64Guid.c_str());
                m_SendPacket<< strBase64Guid.c_str();
            }
        }
    }
    else
    {
        pUserInfoAck->id = pUser->m_nID;
        strcpy(pUserInfoAck->username, pUser->m_szUserName);
        strcpy(pUserInfoAck->nickname, pUser->m_szNickName);
        strcpy(pUserInfoAck->email, pUser->m_szEmail);
        strcpy(pUserInfoAck->birthday, pUser->m_szBirthday);
        pUserInfoAck->sex = pUser->m_nSexy;
        pUserInfoAck->province = pUser->m_nProvince;
        pUserInfoAck->country = pUser->m_nCountry;
        pUserInfoAck->city = pUser->m_nCity;
        strcpy(pUserInfoAck->face_url, pUser->m_szHeadURL);
        pUserInfoAck->career = pUser->m_nCareer;
        pUserInfoAck->rule = pUser->m_AllowRule;
        pUserInfoAck->familyflag = pUser->m_FamilyFlag;
        pUserInfoAck->gold_money = pUser->m_nMoney;
        pUserInfoAck->online_time = pUser->m_nOnlineTime;
        pUserInfoAck->linkpop = pUser->m_linkpop;
        strcpy(pUserInfoAck->address, pUser->m_szAddress);
        strcpy(pUserInfoAck->mobile, pUser->m_szPhone);
        strcpy(pUserInfoAck->description, pUser->m_szDescription);
        strcpy(pUserInfoAck->usersign, pUser->m_szUserSign);
        strcpy(pUserInfoAck->avatoruri, pUser->m_szAvatoruri);
        pUserInfoAck->mobilestatus = pUser->m_nMobileStatus;
        pUserInfoAck->integral = pUser->m_nIntegral;
        m_SendPacket.SetCursor(sizeof(XT_SERVER_USERINFO_GET_ACK));
    }

    CLog::Log("CBusinessGetFriendFullInfo::ReturnUserInfo",CLog::TYPE_IMPORTANT,
              "id:%d, username:%s, nickname:%s, email:%s, birthday:%s,sexy:%d, province:%d, country:%d, city:%d,\
headurl:%s, career:%d, allowrule:%d, familyflag:%d, money:%d,onlinetime:%d, linkpop:%d, address:%s, phone:%s,\
descrption:%s, usersign:%s, avatoruri:%s, mobilestatus:%d, intergral:%d, guid:%s, OUT",
              pUser->m_nID, pUser->m_szUserName, pUser->m_szNickName, pUser->m_szEmail, pUser->m_szBirthday,pUser->m_nSexy,
              pUser->m_nProvince,pUser->m_nCountry, pUser->m_nCity, pUser->m_szHeadURL, pUser->m_nCareer, pUser->m_AllowRule,
              pUser->m_FamilyFlag,pUser->m_nMoney, pUser->m_nOnlineTime, pUser->m_linkpop, pUser->m_szAddress, pUser->m_szPhone,
              pUser->m_szDescription, pUser->m_szUserSign,pUser->m_szAvatoruri, pUser->m_nMobileStatus, pUser->m_nIntegral, pUser->m_szGUID);
}

//---------------------------------------------------------------------

CBusinessModifyUserInfo::CBusinessModifyUserInfo(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_USERINFO_MOD_REQ,CMD_USERINFO_MOD_ACK)
{

}


int CBusinessModifyUserInfo::DoWork()
{

    XT_USERINFO_MOD_REQ* pUserInfoModReq = (XT_USERINFO_MOD_REQ*)m_RecvPacket.GetData();
    XT_USERINFO_MOD_ACK* pUserInfoModAck = (XT_USERINFO_MOD_ACK*)m_SendPacket.GetData();
    XT_USERINFO_MOD_REQ UserInfo;
    UserInfo.id = m_pRecvHead->sid;
    UserInfo.familyflag = 0;
    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        m_RecvPacket >> UserInfo.email
                     >> UserInfo.password
                     >> UserInfo.nickname
                     >> UserInfo.birthday
                     >> UserInfo.sex
                     >> UserInfo.province
                     >> UserInfo.country
                     >> UserInfo.city
                     >> UserInfo.career
                     >> UserInfo.rule
                     >> UserInfo.familyflag
                     >> UserInfo.linkpop
                     >> UserInfo.address
                     >> UserInfo.mobile
                     >> UserInfo.description
                     >> UserInfo.face_url;
    }
    else
    {
        strcpy(UserInfo.email, pUserInfoModReq->email);
        strcpy(UserInfo.password, pUserInfoModReq->password);
        strcpy(UserInfo.nickname, pUserInfoModReq->nickname);
        strcpy(UserInfo.birthday, pUserInfoModReq->birthday);
        UserInfo.sex = pUserInfoModReq->sex;
        UserInfo.province = pUserInfoModReq->province;
        UserInfo.country = pUserInfoModReq->country;
        UserInfo.city = pUserInfoModReq->city;
        UserInfo.career = pUserInfoModReq->career;
        UserInfo.rule = pUserInfoModReq->rule;
        UserInfo.familyflag = pUserInfoModReq->familyflag;
        UserInfo.linkpop = pUserInfoModReq->linkpop;

        strcpy(UserInfo.address, pUserInfoModReq->address);
        strcpy(UserInfo.mobile, pUserInfoModReq->mobile);
        strcpy(UserInfo.description, pUserInfoModReq->description);
        strcpy(UserInfo.face_url, pUserInfoModReq->face_url);
    }
//  ��Ŀ����
    if(strcmp(UserInfo.password,"") != 0)
        MD5Password(UserInfo.password);


    CLog::Log("CBusinessModifyUserInfo", CLog::TYPE_DEBUG, "id:%d, headurl:%s, sexy:%d, nickname:%s, username:%s, commentname:%s, usersign:%s, mobilestatus:%d nicklen:%d,IN",
              UserInfo.id, UserInfo.face_url, UserInfo.sex,
              UserInfo.nickname, UserInfo.username, UserInfo.comment_name,
              UserInfo.user_sign, UserInfo.mobilestatus,strlen(   UserInfo.nickname));

    int ret = m_pRouter->GetDataBaseWeb()->Modify_User_Info(&UserInfo);

    if( DB_SUCCESS !=  ret)
    {
        char strError[MAX_ERROR_BUF]= {0};
        CError::DB_Error(ret,strError,true);
        if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
            SetErrorReturn(strError);
        else
        {
            pUserInfoModAck->ret = RESULT_FAILED;
            int len = strlen(strError);
            if(len > 39)
                len = 39;
            strncpy(pUserInfoModAck->error, strError, len);
            pUserInfoModAck->error[len] = 0;
            m_SendPacket.SetCursor(sizeof(XT_USERINFO_MOD_ACK));
        }

    }
    else
    {
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(UserInfo.id, TRUE);
        if( pUser != NULL )
        {
			STRU_PART_USERINFO struPartUserinfo;
        
            strcpy(struPartUserinfo.email,UserInfo.email);
            if (strcmp(UserInfo.nickname, "") !=0)
                strcpy(struPartUserinfo.nickname,UserInfo.nickname);
            else
            	strcpy(struPartUserinfo.nickname,pUser->m_szNickName);
            strcpy(struPartUserinfo.birthday,UserInfo.birthday);
            struPartUserinfo.sex      = UserInfo.sex;
            struPartUserinfo.province  = UserInfo.province;
            struPartUserinfo.country   = UserInfo.country;
            struPartUserinfo.city      = UserInfo.city;
            struPartUserinfo.career    = UserInfo.career;
            struPartUserinfo.rule  = UserInfo.rule;
            struPartUserinfo.linkpop    = UserInfo.linkpop;
            strcpy(struPartUserinfo.address,UserInfo.address);
            strcpy(struPartUserinfo.mobile,UserInfo.mobile);
            strcpy(struPartUserinfo.description,UserInfo.description);
            strcpy(struPartUserinfo.face_url, UserInfo.face_url);

            //��ȫ�ָ���һ��
            if (pUser->m_nStatus == XTREAM_OFFLINE)
                CLog::Log("CBusinessModifyUserInfo", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d", pUser->m_nID);

            m_pRouter->GetGlobalBusi()->ModiPartUserInfo(UserInfo.id,struPartUserinfo);
        }
        if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
            m_SendPacket << (uint8)RESULT_SUCCESS;
        else
        {
            pUserInfoModAck->ret = RESULT_SUCCESS;
            m_SendPacket.SetCursor(sizeof(XT_USERINFO_MOD_ACK));
        }

        //֪ͨ����
        pUser->NotifyFriendInfo(m_pRouter->GetGlobalBusi());
    }


    CLog::Log("CBusinessModifyUserInfo", CLog::TYPE_IMPORTANT, "id:%d, headurl:%s, sexy:%d, nickname:%s, username:%s, commentname:%s, usersign:%s, mobilestatus:%d nicklen:%d,OUT",
              UserInfo.id, UserInfo.face_url, UserInfo.sex,
              UserInfo.nickname, UserInfo.username, UserInfo.comment_name,
              UserInfo.user_sign, UserInfo.mobilestatus,strlen(   UserInfo.nickname));


    return RESULT_SUCCESS;
}


void CBusinessModifyUserInfo::MD5Password(char* pPassword)
{
    unsigned char digest[16];

    char password[255] = {0};
    //sprintf_s(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",pPassword);
    sprintf_s(password,"%s",pPassword);

    MD5_CTX md5T;
    md5T.MD5Init();
    md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
    md5T.MD5Final (digest);

    string sHex=ToHexString(digest,16);

    strcpy(pPassword,sHex.c_str());

}

//---------------------------------------------------------------------
CBusinessCustListMod::CBusinessCustListMod(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_UPDATE_NEAR_COMM_CUST_INFO_REQ,CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK)
{
}

int CBusinessCustListMod::DoWork()
{
    //XT_UPDATE_NEAR_COMM_CUST_INFO_REQ* pCustInfoMod = (XT_UPDATE_NEAR_COMM_CUST_INFO_REQ*)m_RecvPacket.GetData();
    uint32 BusinessID = 0, iCommCount = 0, i = 0;
    char Account[MAX_CUST_ACCOUNT_LEN] = {0};
    char Time[MAX_CUST_TIME_LEN] = {0};
    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();

    m_RecvPacket>>BusinessID
                >>iCommCount;

    if(iCommCount <= 0)
    {
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<"customer number error";
    }
    else
    {
        for(i = 0; i < iCommCount; i++)
        {
            m_RecvPacket>>Account
                        >>Time;
            if(RESULT_FAILED == pDatabase->Mod_Cust_Info(BusinessID,Account,Time))
            {
                break;
            }
        }
        if(i < iCommCount)
        {
            m_SendPacket<<(uint8)RESULT_FAILED
                        <<"database operate error";
        }
        else
            m_SendPacket<<(uint8)RESULT_SUCCESS;

    }

    return RESULT_SUCCESS;
}

//---------------------------------------------------------------------
CBusinessModifyCustRemark::CBusinessModifyCustRemark(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_UPDATE_NEAR_COMM_CUST_REMARK_REQ,CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK)
{
}

int CBusinessModifyCustRemark::DoWork()
{
    XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ req;
    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();

    m_RecvPacket>>req.uiBusinessID
                >>req.szCommAccount
                >>req.szCommTime
                >>req.szRemarks;
    CLog::Log("CBusinessModifyCustRemark", CLog::TYPE_IMPORTANT, "id:%d, CommAccount:%s, CommTime:%s, Remarks:%s, IN",
              req.uiBusinessID, req.szCommAccount,req.szCommTime,req.szRemarks);
    int len = strlen(req.szRemarks);
    if(len >200)
    {
        CLog::Log("CBusinessModifyCustRemark", CLog::TYPE_ERROR, "Remark length is too long! id:%d, clientAccount:%s, lenth:%d",
                  req.uiBusinessID,req.szCommAccount,len);
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }
    if(RESULT_FAILED == pDatabase->Mod_Cust_Remark(req.uiBusinessID,req.szCommAccount,req.szCommTime,req.szRemarks))
    {
        m_SendPacket<<(uint8)RESULT_FAILED;
        CLog::Log("CBusinessModifyCustRemark", CLog::TYPE_ERROR, "Remark Save database error! id:%d, CommAccount:%s",
                  req.uiBusinessID, req.szCommAccount);
    }
    else
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS;
        CLog::Log("CBusinessModifyCustRemark", CLog::TYPE_IMPORTANT, "Remark Save database success! id:%d, CommAccount:%s, CommTime:%s, Remarks:%s, OUT",
                  req.uiBusinessID,req.szCommAccount, req.szCommTime, req.szRemarks);
    }
    return RESULT_SUCCESS;
}

CBusinessGetCustRemark::CBusinessGetCustRemark(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_NEAR_COMM_CUST_REMARK_REQ,CMD_GET_NEAR_COMM_CUST_REMARK_ACK)
{

}
int CBusinessGetCustRemark::DoWork()
{
    XT_GET_NEAR_COMM_CUST_REMARK_REQ req;
    m_RecvPacket >> req.uiBusinessID
                 >> req.szCommAccount;
    CLog::Log("CBusinessGetCustRemark", CLog::TYPE_IMPORTANT, "id:%d, CommAccount:%s, IN",
              req.uiBusinessID, req.szCommAccount);
	
	if(strlen(req.szCommAccount) > 32)
	{
        m_SendPacket<<(uint8)RESULT_FAILED;
        CLog::Log("CBusinessGetCustRemark", CLog::TYPE_ERROR, "Remark szCommAccount too long ,len %d",strlen(req.szCommAccount));
		return RESULT_SUCCESS;
	}
		
    XT_GET_NEAR_COMM_CUST_REMARK_ACK ack;
    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();

    if( RESULT_FAILED == pDatabase->Get_Cust_Remark(req.uiBusinessID,req.szCommAccount,ack.szCommTime,ack.szRemarks))
    {
        m_SendPacket<<(uint8)RESULT_FAILED;
        CLog::Log("CBusinessGetCustRemark", CLog::TYPE_ERROR, "Remark Get database error! id:%d, CommAccount:%s",
                  req.uiBusinessID,req.szCommAccount);
    }
    else
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<req.uiBusinessID
                    <<req.szCommAccount
                    <<ack.szCommTime
                    <<ack.szRemarks;
        CLog::Log("CBusinessGetCustRemark", CLog::TYPE_IMPORTANT, "Remark Get database success! id:%d, CommAccount:%s, CommTime:%s, Remarks:%s",
                  req.uiBusinessID, req.szCommAccount, ack.szCommTime, ack.szRemarks);
    }
    return RESULT_SUCCESS;
}





#define MAX_CUST_INFO_NUM 1000
CBusinessCustListReq::CBusinessCustListReq(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_NEAR_COMM_CUST_REQ,CMD_GET_NEAR_COMM_CUST_ACK)
{
    pCustInfo = new XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo[MAX_CUST_INFO_NUM];
    pData = new SendDataStruct;
    if(pCustInfo == NULL || pData == NULL)
        CLog::Log("CBusinessCustListReq::CBusinessCustListReq",CLog::TYPE_ERROR,"New failed!");
}

CBusinessCustListReq::~CBusinessCustListReq()
{
    if(pCustInfo)
    {
        delete[] pCustInfo;
        pCustInfo = NULL;
    }
    if(pData)
    {
        delete pData;
        pData = NULL;
    }
}

int CBusinessCustListReq::DoWork()
{
    uint32 BusinessID = 0; uint32 MaxCustCount = 0; uint32 nRetCount = 0;
    uint32 initCount = 0;
    uint32 i = 0;
    uint16 Start_pos = 0; uint16 CustCount = 0;
    uint8 Direct = 0;
    char BTime[MAX_CUST_TIME_LEN] = {0};
    char ETime[MAX_CUST_TIME_LEN] = {0};

    XT_GET_NEAR_COMM_CUST_ACK* pCustInfoAck = (XT_GET_NEAR_COMM_CUST_ACK*)m_SendPacket.GetData();
    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();

    CancelReply();

    m_RecvPacket>>BusinessID
                >>BTime
                >>ETime
                >>MaxCustCount
                >>Start_pos
                >>CustCount
                >>Direct;

    CLog::Log("CBusinessCustListReq::DoWork",CLog::TYPE_IMPORTANT,"id:%d, BTime:%s, ETime:%s, MaxCustCount:%d, Start_pos:%d, CustCount:%d, Direct:%d ,IN",
              BusinessID, BTime, ETime, MaxCustCount, Start_pos, CustCount, Direct);

    if(Start_pos != 999)
    {
        nRetCount = CustCount;
        if(RESULT_FAILED == pDatabase->Get_Cust_Info(BusinessID,BTime,ETime,MaxCustCount,Start_pos,CustCount,Direct,pCustInfo,nRetCount))
        {
            m_SendPacket<<(uint8)RESULT_FAILED
                        <<"database operate error";

            CLog::Log("CBusinessCustListReq::DoWork",CLog::TYPE_ERROR,"Get_Cust_Info failed! id:%d ",BusinessID);
        }

        while(nRetCount > i)
        {
            m_SendPacket<<(uint32)RESULT_SUCCESS
                        <<BusinessID
                        <<(uint8)0x01
                        <<initCount;
            for(; i < nRetCount; i ++)
            {
                if(!m_SendPacket.HasData(sizeof(XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo)))
                {
                    break;
                }
                m_SendPacket<<pCustInfo[i].szCommAccount
                            <<pCustInfo[i].szCommTime
                            <<pCustInfo[i].szTransInfo
                            <<pCustInfo[i].szRemarks;
                pCustInfoAck->actualCount ++;

                CLog::Log("CBusinessCustListReq::DoWork",CLog::TYPE_DEBUG,"id:%d, CommAccount:%s, CommTime:%s, TransInfo:%s, Remarks:%s, OUT",
                          BusinessID, pCustInfo[i].szCommAccount, pCustInfo[i].szCommTime, pCustInfo[i].szTransInfo, pCustInfo[i].szRemarks);
            }
            SendCustListAck(m_SendPacket);
            m_SendPacket.Reset();
        }
    }
    else
    {
        if(RESULT_FAILED == pDatabase->Get_Cust_Num(BusinessID,BTime,ETime,nRetCount))
        {
            m_SendPacket<<(uint8)RESULT_FAILED
                        <<"database operate error";

            CLog::Log("CBusinessCustListReq::DoWork",CLog::TYPE_ERROR,"Get_Cust_Num failed! id:%d",BusinessID);
        }
        else
        {
            if(nRetCount > 1000)
                nRetCount = 1000;
            m_SendPacket<<(uint32)RESULT_SUCCESS
                        <<BusinessID
                        <<(uint8)0x00
                        <<nRetCount;
            CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"id:%d, nRetCount:%d ,send",BusinessID, nRetCount);
            SendCustListAck(m_SendPacket);
        }
    }

    return RESULT_SUCCESS;
}

void CBusinessCustListReq::SendCustListAck(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_SRVACK;
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = m_pRecvHead->seq++;
    pHead->cmd  = CMD_GET_NEAR_COMM_CUST_ACK;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),pData->data,pData->data_len);

}

//---------------------------------------------------------------------
const char MyGroupName[] = "�ҵ��Ŷ�";
#define MYGROUPPID 255
#define MYGROUPID 250


bool SortGroupListMethod(const GROUPEX &x,const GROUPEX &y)
{
    if(x.partentid != y.partentid)
        return x.partentid < y.partentid;
    else
        return x.groupid < y.groupid;
}


// ��ȡ�����б�
bool CBusinessGetGroupInfo::GetFriendGroupList(int id,uint8 &count, vector<GROUPEX> &friendgrouplist)
{
	count = 0;
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strfriendgrouplisturl(CAppManager::GetInstance().GetConfig().GetFriendGroupListUrl());
	CLog::Log("CBusinessGetGroupInfo",CLog::TYPE_ERROR,"post %s %s",strfriendgrouplisturl.c_str(),CAppManager::GetInstance().GetConfig().GetFriendGroupListUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << id;
	
	string strFriendGrouplist;
	int httpStaus = httpClient.Post(strfriendgrouplisturl, ssParams.str(), strFriendGrouplist);
	GROUPEX friendgroup;

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strFriendGrouplist, jvRecvResponse)){
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true"){
				if(jvRecvResponse.isMember("data")) {
					if(jvRecvResponse["data"].isMember("organNameInfos")){
						Json::Value jvFriendList = jvRecvResponse["data"]["organNameInfos"];
						for(int i = 0; i < (int)jvFriendList.size() && i < 10000; ++i)
						{
							Json::Value jvFriendItem = jvFriendList[i];
							if(!jvFriendItem.isMember("id"))
							{
								continue;
							}
							
							int nId = jvFriendItem["id"].asInt();

							friendgroup.groupid = nId;
							
							if(jvFriendItem.isMember("parentId"))
							{
								friendgroup.partentid = jvFriendItem["parentId"].asInt();
							}
					
							if(jvFriendItem.isMember("organName"))
							{	
								string GroupNameFull = jvFriendItem["organName"].asString();								
								
								int pos = GroupNameFull.rfind('\\');
								string GroupName = GroupNameFull.substr(pos + 1);
								int len = GroupName.length();
								len = len > MAX_GROUPNAME_LEN ? MAX_GROUPNAME_LEN: len;
								
								utf2gbk((char *)GroupName.c_str(),len,friendgroup.szGroupName,len);
								//strncpy(friendgroup.szGroupName, jvFriendItem["organName"].asCString(),len > MAX_GROUPNAME_LEN ? MAX_GROUPNAME_LEN: len);
							}
							
							CLog::Log("CBusinessGetGroupInfo",CLog::TYPE_IMPORTANT,
								"i: %d,groupid:%d,parentId:%d organName: %s",
								i,friendgroup.groupid,friendgroup.partentid,friendgroup.szGroupName);
							if(friendgroup.partentid == 255)
							{
								friendgrouplist.insert(friendgrouplist.begin(),friendgroup);
							}
							else
							{
								friendgrouplist.push_back(friendgroup);
							}
							//memset(&friendgroup,0,sizeof(friendgroup));
							count++;
						}
						if(friendgrouplist.size() > 1)
						{
							sort(friendgrouplist.begin() + 1,friendgrouplist.end(),SortGroupListMethod);
						}
						return true;
					}
				}
			}
	    }
	}

	return false;
}

CBusinessGetGroupInfo::CBusinessGetGroupInfo(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GROUPINFO_GET_REQ,CMD_GROUPINFO_GET_ACK)
{

}


int CBusinessGetGroupInfo::DoWork()
{
	XT_GROUPINFO_GET_REQ req;
	m_RecvPacket>>req.id;
	m_RecvPacket>>req.group_count;
	m_RecvPacket>>req.nType;
	
    XT_GROUPINFO_GET_REQ* pGroupGetReq = &req;
    XT_GROUPINFO_GET_ACK* pGroupGetAck = (XT_GROUPINFO_GET_ACK*)m_SendPacket.GetData();

    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();
    if( 0 == pGroupGetReq->group_count)
    {
        pGroupGetReq->group_count = MAX_GROUP_NUM;
    }
	vector<GROUPEX> friendgrouplist;

    if( RESULT_FAILED == pDatabase->Get_User_GroupInfo(pGroupGetReq->id,m_GroupInfo,pGroupGetReq->group_count,m_pRecvHead->flag))
    {
        if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        {
            m_SendPacket<< (uint8)RESULT_FAILED
                        << "��ȡ���ѷ���ʧ��";
        }
        else
        {
            int len;
            char* error = NULL;
            pGroupGetAck->ret = RESULT_FAILED;
            len = strlen("��ȡ���ѷ���ʧ��");
            error = (char*)pGroupGetAck + sizeof(uint8);
            strncpy(error,"��ȡ���ѷ���ʧ��",strlen("��ȡ���ѷ���ʧ��"));
            *(error + len) = 0;
            m_SendPacket.SetCursor(sizeof(uint8)+len+1);
        }
    }
    else
    {
        if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        {
			uint8 FriendGroupCount = 0;
            m_SendPacket << (uint8)RESULT_SUCCESS;

            if (pGroupGetReq->nType == 2)
            {
                //uint8 backendGroupCount = CSGroup.vecG.size();
                
				if(GetFriendGroupList(pGroupGetReq->id,FriendGroupCount,friendgrouplist))
				{
					m_SendPacket<< (uint8)(FriendGroupCount + pGroupGetReq->group_count);					
					CLog::Log("CBusinessGetGroupInfo",CLog::TYPE_ERROR,"FriendGroupCount: %d %d",FriendGroupCount,pGroupGetReq->group_count);
				}
				else
				{
					m_SendPacket<<pGroupGetReq->group_count;
				}
                
            }
            else
            {
                m_SendPacket<<pGroupGetReq->group_count;
            }

            for(int i = 0 ; i < pGroupGetReq->group_count; ++i)
            {
                m_SendPacket << m_GroupInfo[i].nGroupPid
                             << m_GroupInfo[i].nGroupID
                             << m_GroupInfo[i].szGroupName;
            }

            if(m_pRecvHead->flag == XT_HEAD::FLAG_SELLCNTREQ)
            {

                CLog::Log("CBusinessGetGroupInfo",CLog::TYPE_ERROR,"nType: %d",pGroupGetReq->nType);
                ///For �ͷ�����̨
                if (pGroupGetReq->nType == 2)
                {
                    //CSGroup.vecG;
                    for(int j = 0; j < FriendGroupCount; ++j)
                    {
                        m_SendPacket << friendgrouplist[j].partentid
                                     << friendgrouplist[j].groupid
                                     << friendgrouplist[j].szGroupName;
                        CLog::Log("CBusinessGetGroupInfo",CLog::TYPE_IMPORTANT,"i: %d,partentid:%d,groupid:%d,szGroupName:%s",
                                  j,friendgrouplist[j].partentid,friendgrouplist[j].groupid,friendgrouplist[j].szGroupName);
                    }
                }
                else
                {

                    m_SendPacket<<(uint8)MYGROUPPID
                                <<(uint8)MYGROUPID
                                <<MyGroupName;
                }
            }

        }
        else
        {
            int i;
            pGroupGetAck->ret = RESULT_SUCCESS;
            pGroupGetAck->group_count = pGroupGetReq->group_count;
            char* pInfo = (char*)pGroupGetAck + sizeof(uint8) + sizeof(uint8);

            for(i= 0 ; i < pGroupGetReq->group_count; ++i)
            {
                memcpy(pInfo + sizeof(XT_GROUPINFO_GET_ACK::tagGroupInfo)*i, (char*)&m_GroupInfo[i], sizeof(XT_GROUPINFO_GET_ACK::tagGroupInfo));
            }
            m_SendPacket.SetCursor(sizeof(uint8)+sizeof(uint8)+sizeof(XT_GROUPINFO_GET_ACK::tagGroupInfo)*i);
        }
    }

    return RESULT_SUCCESS;
}

//-----------------------------------------------------------------

CBusinessGetFriendList::CBusinessGetFriendList(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_FRIENDLIST_REQ,CMD_FRIENDLIST_ACK)
{
}

// ��ȡ�����б�
bool CBusinessGetFriendList::GetFriendList(int id, int merchantId,uint32 &count, XT_FRIENDLIST_ACK::tagFriendInfo *friendlist)
{
	count = 0;
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strfriendlisturl(CAppManager::GetInstance().GetConfig().GetFriendListUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << id;
	ssParams<<"&merchantId=" << merchantId;
	
	string strFriendlist;
	int httpStaus = httpClient.Post(strfriendlisturl, ssParams.str(), strFriendlist);
	CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"post %s %s",strfriendlisturl.c_str(),CAppManager::GetInstance().GetConfig().GetFriendListUrl());

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strFriendlist, jvRecvResponse)){
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true"){
				if(jvRecvResponse.isMember("data")) {
					if(jvRecvResponse["data"].isMember("friendItemList")){
						Json::Value jvFriendList = jvRecvResponse["data"]["friendItemList"];
						for(int i = 0; i < (int)jvFriendList.size() && i < 10000; ++i)
						{
							Json::Value jvFriendItem = jvFriendList[i];
							if(!jvFriendItem.isMember("friendId"))
							{
								continue;
							}
							
							int nId = jvFriendItem["friendId"].asInt();

							friendlist[i].friendId = nId;
							#if 0
							if(jvFriendItem.isMember("status"))
							{
								friendlist[i].status = jvFriendItem["status"].asInt();
							}
							else
							{
								friendlist[i].status = 19;
							}
							#endif
							if(jvFriendItem.isMember("groupId"))
							{								
								friendlist[i].groupId = jvFriendItem["groupId"].asInt();
							}
							
							CLog::Log("CBusinessGetFriendList",CLog::TYPE_IMPORTANT,"i: %d,friendId:%d,groupId:%d",
													  i,friendlist[i].friendId,friendlist[i].groupId);
							
							count++;
						}
						
						return true;
					}
				}
			}
	    }
	}

	return false;
}


int CBusinessGetFriendList::DoWork()
{
    MyGroupAccount GroupAccount;
    XT_FRIENDLIST_REQ req;
    m_RecvPacket >> req.merchant_id
    			 >> req.id
    			 >> req.start_pos
    			 >> req.nType; 
    //XT_FRIENDLIST_REQ* pGetFriendListReq = &req;

	XT_FRIENDLIST_REQ* pGetFriendListReq = (XT_FRIENDLIST_REQ*)m_RecvPacket.GetData();

    XT_FRIENDLIST_ACK* pGetFriendListAck = (XT_FRIENDLIST_ACK*)m_SendPacket.GetData();
#if 0
    const int nFriendNumInOnPacket = 10000;

    XT_FRIENDLIST_ACK::tagFriendInfo friendInfo[nFriendNumInOnPacket] ;

    //�����ݿ��в�ѯ
    CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
    int nReturnNum = nFriendNumInOnPacket;
    if(DB_SUCCESS !=  pDatabase->Get_User_FriendList( pGetFriendListReq->id,
            pGetFriendListReq->start_pos,
            friendInfo,
            nReturnNum,
            m_pRecvHead->flag) )
    {
        SetErrorReturn("���ݿⷱæ,��ȡ�����б�ʧ��");
        return RESULT_FAILED;
    }

#if 1   //ȡ���������ѯ�������˺� duyong
    //��ѯ�������˺�
    if(m_pRecvHead->flag == XT_HEAD::FLAG_SELLCNTREQ && pGetFriendListReq->merchant_id != 0)
    {
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_IMPORTANT,"pGetFriendListReq->nType:%d",
                  pGetFriendListReq->nType);
        if(pGetFriendListReq->nType == 2)
        {
#if 0
            CGetBackendGroup *pbackendGroup = CAppManager::GetInstance().GetBackendGroupIns();
            uint8 backendMemCount = pbackendGroup->m_vecCSPlatPerson.size();
            int i =0;
            for( i = 0; i < backendMemCount; i++)
            {

                friendInfo[nReturnNum+i].friendId = pbackendGroup->m_vecCSPlatPerson[i].id;
                friendInfo[nReturnNum+i].groupId = pbackendGroup->m_vecCSPlatPerson[i].csGroupId;

                CLog::Log("CBusinessGetFriendList",CLog::TYPE_IMPORTANT,"i: %d,friendId:%d,groupId:%d",
                          i,friendInfo[nReturnNum+i].friendId,friendInfo[nReturnNum+i].groupId);

            }
            nReturnNum +=i;

				
#endif
			uint32 count = 0;
			if(GetFriendList(pGetFriendListReq->id,0,count,&friendInfo[nReturnNum]))
			{
				nReturnNum +=count;
			}
			else
			{
				CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"Request user %u fail",
										  pGetFriendListReq->id);	
				SetErrorReturn("���ݿ�ӿڷ�æ����ȡ�����б�ʧ��");
                return RESULT_FAILED;
			}

        }
        else
        {
            if(DB_SUCCESS != MYSQL_LIB->getAllSubAccount(pGetFriendListReq->merchant_id, pGetFriendListReq->id, GroupAccount))
            {
                SetErrorReturn("���ݿⷱæ,��ȡ�����б�ʧ��");
                return RESULT_FAILED;
            }
            else
            {
                int num = 0;

                MyGroupAccount::iterator iter;

                int friend_index = nReturnNum;
                for(iter = GroupAccount.begin(); iter != GroupAccount.end(); ++ iter)
                    //��ȡ�����û����˺ű���
                {
                    //��subaccount�û�������������Ϣ
                    //û��subaccount�û����������û���Ϣ��β��
                    for(num = 0; num < nReturnNum; num++)
                    {
                        if(friendInfo[num].friendId == iter->id)
                        {
                            friendInfo[num].groupId = MYGROUP_ID;
                            break;
                        }
                    }

                    if(num == nReturnNum)//û�ں������ҵ������˻�
                    {
                        friendInfo[friend_index].friendId = iter->id;
                        friendInfo[friend_index].groupId = MYGROUP_ID;
                        friend_index++;
                    }
                }
                nReturnNum = friend_index;
            }
        }
    }

#endif
#if 0
    for (int i = 0 ; i < nReturnNum ; i++)
    {
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_DEBUG,"TOTAL USER %d userid %d  groupid %d",nReturnNum,friendInfo[i].friendId,friendInfo[i].groupId);
    }
#endif

    //����״̬��ȫ�ַ�������ѯ
    uint32 idList[nFriendNumInOnPacket];
    GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[nFriendNumInOnPacket];
    uint8   idcount=0;
    int j =0;
    for( j = 0 ; j < nReturnNum; ++j)
    {
        idList[j] = friendInfo[j].friendId;
        userStatus[j].id = idList[j];
        userStatus[j].status = XTREAM_OFFLINE;
    }
    idcount = min(GET_FRIEND_NUMBER,nReturnNum);
#ifdef IM_USE_CACHE
    if (MEMCACHED_NO_INFO < CMemcacheQueryUserStatusi(m_pRouter,idList,idcount,userStatus))
    {
        SetErrorReturn("��ѯ�û�״̬ʧ��");
        return RESULT_FAILED;
    }
#else
    if( RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(
            idList ,
            userStatus ,
            idcount ) )
    {
        SetErrorReturn("��ѯ�û�״̬ʧ��");
        return RESULT_FAILED;
    }
#endif
    //CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"ALL %d",idcount);

    //�����û�״̬
    for(int j =0; j < idcount; ++j)
    {
        if( friendInfo[j].friendId == userStatus[j].id)
        {
            friendInfo[j].status = userStatus[j].status;

            //  CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"ALL %d ,id %u , state %d",idcount,userStatus[j].id,userStatus[j].status);
        }
        else
        {
            SetErrorReturn("��ѯ�û�״̬ʧ��");
            return RESULT_FAILED;
        }
    }

    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(pGetFriendListReq->id, TRUE);
    if( NULL == pUser)
    {
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"����һ��δ��½�û���ѯ�����б�,id:%d",pGetFriendListReq->id);
        return RESULT_FAILED;
    }
#if 0
    else
    {
        FriendInfo* pFriendInfo;
        //pUser->m_FriendList.Clear();
        //�����û������б�
        for(int i = 0 ; i < nReturnNum ; ++i )
        {
            pFriendInfo = pUser->m_FriendList.Add(friendInfo[i].friendId);

            if( pFriendInfo )
            {
                pFriendInfo->id     = friendInfo[i].friendId;
                pFriendInfo->status = friendInfo[i].status;
                pFriendInfo->GroupID = friendInfo[i].groupId;
            }
        }

    }
#endif

#endif
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(pGetFriendListReq->id, TRUE);
    if( NULL == pUser)
    {
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"����һ��δ��½�û���ѯ�����б�,id:%d",pGetFriendListReq->id);
        return RESULT_FAILED;
    }

    //�����������
    {
        //FriendInfo* pFriendInfo;
		uint8 nReturnNum = pUser->m_FriendList.GetSize();
        m_SendPacket<<(uint8)RESULT_SUCCESS;    //�������
        m_SendPacket<<(uint16)0 ;               //�´η������������ʼλ��,���滹Ҫ���ݾ���ֵ�޸�
        m_SendPacket<<(uint8)nReturnNum;

        char FriendInfoTempBuf[sizeof(FriendInfo)];
        CSendPacket SendPacketTemp(FriendInfoTempBuf);

        //bool bIsLastFriend = false;
        int i;
        for( i = 0 ; i <  nReturnNum ; ++i )
        {
            SendPacketTemp<<pUser->m_FriendList.FindByPos(i)->id
                          <<pUser->m_FriendList.FindByPos(i)->GroupID
                          <<pUser->m_FriendList.FindByPos(i)->status;

            CLog::Log("CBusinessGetFriendList",CLog::TYPE_IMPORTANT,"FINALLY ,id %u , state %d",pUser->m_FriendList.FindByPos(i)->id,pUser->m_FriendList.FindByPos(i)->status);
            //�����һ�����ѵ���Ϣ�Ƿ���װ�ص���
            //if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
            if((i >= GET_FRIEND_NUMBER) || (!m_SendPacket.HasData(SendPacketTemp.GetSize())))
            {
                //�����ߴ��˻��ߵ���ͻ��˽��պ����������ֵ���˳�
                pGetFriendListAck->count = i  ;
                break;
            }
            else
            {
            /*
                pFriendInfo = pUser->m_FriendList.Add(friendInfo[i].friendId);

                if( pFriendInfo )
                {
                    pFriendInfo->id     = friendInfo[i].friendId;
                    pFriendInfo->status = friendInfo[i].status;
                    pFriendInfo->GroupID = friendInfo[i].groupId;
                }
			*/
                m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
                SendPacketTemp.Reset();
            }
        }

        int nNextPos =  pGetFriendListReq->start_pos + i;
        if( i >= nReturnNum)
        {
            //���һ��������
            nNextPos = 0xffff;
            pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());
        }
        pGetFriendListAck->next_pos = nNextPos;

    }

    return RESULT_SUCCESS;
}

//---------------------------------------------------------------------

CBusinessGroupInfoModify::CBusinessGroupInfoModify(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GROUPINFO_MOD_REQ,CMD_GROUPINFO_MOD_ACK)
{

}

// ��Ӻ��ѷ���
bool CBusinessGroupInfoModify::AddGroup(int uId, int groupPid, int groupId, const char *groupName, string &error)
{
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strAddGroupUrl(CAppManager::GetInstance().GetConfig().GetAddGroupUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << uId;
	ssParams<<"&id=" << groupId;
	ssParams<<"&parentId=" << groupPid;
	char szUtf8GroupName[1024] = {0};
	gbk2utf8(szUtf8GroupName, groupName);
	ssParams<<"&organName=" << szUtf8GroupName;
	
	string strResult;
	int httpStaus = httpClient.Post(strAddGroupUrl, ssParams.str(), strResult);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strResult, jvRecvResponse))
	    {
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
	    	{
	    		return true;
	    	}
	    	/*
	    	else
	    	{
	    		if(jvRecvResponse.isMember("message"))
	    		{
	    			error = jvRecvResponse["message"].asString();
	    		}
	    	}
	    	*/
	    }
	}

	error = "��ӷ���ʧ��";
	return false;
}

// �޸ĺ��ѷ���
bool CBusinessGroupInfoModify::ModGroup(int uId, int groupId, const char *groupName, string &error)
{
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strModGroupUrl(CAppManager::GetInstance().GetConfig().GetModGroupUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << uId;
	ssParams<<"&groupId=" << groupId;
	char szUtf8GroupName[1024] = {0};
	gbk2utf8(szUtf8GroupName, groupName);
	ssParams<<"&groupName=" << szUtf8GroupName;
	
	string strResult;
	int httpStaus = httpClient.Post(strModGroupUrl, ssParams.str(), strResult);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strResult, jvRecvResponse))
	    {
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
	    	{
	    		return true;
	    	}
	    	/*
	    	else
	    	{
	    		if(jvRecvResponse.isMember("message"))
	    		{
	    			error = jvRecvResponse["message"].asString();
	    		}
	    	}
	    	*/
	    }
	}

	error = "�޸ķ���ʧ��";
	return false;
}

// ɾ�����ѷ���
bool CBusinessGroupInfoModify::DelGroup(int uId, int groupId, string &error)
{
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strDelGroupUrl(CAppManager::GetInstance().GetConfig().GetDelGroupUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << uId;
	ssParams<<"&groupId=" << groupId;
	
	string strResult;
	int httpStaus = httpClient.Post(strDelGroupUrl, ssParams.str(), strResult);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strResult, jvRecvResponse))
	    {
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
	    	{
	    		return true;
	    	}
	    	/*
	    	else
	    	{
	    		if(jvRecvResponse.isMember("message"))
	    		{
	    			error = jvRecvResponse["message"].asString();
	    		}
	    	}
	    	*/
	    }
	}

	error = "ɾ������ʧ��";
	return false;
}


int CBusinessGroupInfoModify::DoWork()
{
    //XT_GROUPINFO_MOD_REQ* pGroupInfoModReq = (XT_GROUPINFO_MOD_REQ*)m_RecvPacket.GetData();
    //XT_GROUPINFO_MOD_ACK* pGroupInfoModAck = (XT_GROUPINFO_MOD_ACK*)m_SendPacket.GetData();

    //CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();

    XT_GROUPINFO_MOD_REQ req;
    m_RecvPacket >> req.uid
                 >> req.action
                 >> req.group_pid
                 >> req.group_id
                 >> req.name;

    bool bRlt = false;   
    string error;
    switch(req.action)
    {
        case GROUP_ADD:
        	bRlt = AddGroup((int)req.uid,(int)req.group_pid,(int)req.group_id,req.name,error);
            break;
        case GROUP_MODIFY:
            bRlt = ModGroup((int)req.uid,(int)req.group_id,req.name,error);
            break;
        case GROUP_DEL:
        	bRlt = DelGroup((int)req.uid,(int)req.group_id,error);
            break;
        default:
            CLog::Log("CBusinessGroupInfoModify",CLog::TYPE_ERROR,"�Ƿ�action����,uid:%d, group_id:%d",req.uid,req.group_id);
            SetErrorReturn("�Ƿ����������");
            return RESULT_FAILED;
    }

    if(!bRlt)
    {
        SetErrorReturn(error.c_str());
    }

    m_SendPacket << (uint8)RESULT_SUCCESS
                 << req.action
                 << req.group_pid
                 << req.group_id
                 << req.name;

    return RESULT_SUCCESS;
}


//---------------------------------------------------------------------



CBusinessSearchFriend::CBusinessSearchFriend(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_XTREAM_SEARCH_REQ,CMD_XTREAM_SEARCH_ACK)
{
    m_nReturnCount = 0 ;
}


int CBusinessSearchFriend::DoWork()
{
    m_pSearchFriendReq = (XT_XTREAM_SEARCH_REQ*)m_RecvPacket.GetData();
    m_pSearchFriendAck = (XT_XTREAM_SEARCH_ACK*)m_SendPacket.GetData();

    int nResult = 0;

    if( m_pSearchFriendReq->search_type == XTREAM_SEARCH_ONLINE ) //���������û�
    {
        nResult =  FindOnlineUser();
    }
    else if( m_pSearchFriendReq->search_type == XTREAM_SEARCH_PRECISION ) //��ȷ����
    {
        nResult = FindPrecision() ;
    }
    else
    {
        CLog::Log("CBusinessSearchFriend",CLog::TYPE_ERROR,"�Ƿ���ѯ����");
        nResult = RESULT_FAILED;
    }



    return nResult;

}


int CBusinessSearchFriend::FindOnlineUser()
{
    GSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo FriendInfo[MAX_SEARCH_FRINED_RETURN_NUM];
    int nResult;
    m_SendPacket << (uint8)RESULT_SUCCESS   //�������
                 << (uint8)0  //�����û���Ϣ����,����Ҫ���ݾ���ֵ�޸����ֵ
                 << (uint16)0    //nextpos
                 << (uint32)0;   //totalnum;

    //��ȫ�ַ�������ѯ
    CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();

    m_nReturnCount = (m_pSearchFriendReq->count < sizeof(FriendInfo)/sizeof(FriendInfo[0])) ? m_pSearchFriendReq->count:sizeof(FriendInfo)/sizeof(FriendInfo[0]) ;

    //m_nReturnCount = sizeof(FriendInfo)/sizeof(FriendInfo[0]);

    uint32 nTotalOnlineNum = 0;
    nResult = pGlobalBusi->SearchOnlineUser(FriendInfo,m_nReturnCount,nTotalOnlineNum,m_pSearchFriendReq->startpos);
    CLog::Log("FindOnlineUser",CLog::TYPE_DEBUG,"nResult %d ",nResult);

    if( RESULT_SUCCESS == nResult )
    {
        //���ؽ��
        for( int i = 0 ; i < m_nReturnCount ; ++i )
        {
            m_SendPacket << FriendInfo[i].clientid
                         << FriendInfo[i].sexy
                         << FriendInfo[i].status
                         << FriendInfo[i].rule
                         << FriendInfo[i].username
                         << FriendInfo[i].nickname;
        }

        //�����ϸ���
        m_pSearchFriendAck->count = m_nReturnCount;
        m_pSearchFriendAck->nextpos = 0;//������Ч�ģ������
        m_pSearchFriendAck->nTotalNum = nTotalOnlineNum;
    }
    /*	// ���������������redis�ҵ������
#ifdef IM_USE_CACHE
    else if(MEMCACHED_SERVER_DOWN == nResult) //memacached �ҵ�
    {

        CLog::Log("FindOnlineUser",CLog::TYPE_DEBUG,"memacached �ҵ�");
        int i = 0 ;
        CUser *pUser = NULL;
        int pos = 0;
        CUser::m_UserListLock.Lock();;
        for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it = CUser::m_userhash.begin(); it != CUser::m_userhash.end(); ++it)
        {
            pUser = it->second;
            if(!pUser)
                continue;
            if( pos < m_pSearchFriendReq->startpos )
            {
                if( pUser->m_nStatus != XTREAM_HIDE && pUser->m_nStatus != XTREAM_OFFLINE)
                {
                    pos++;
                }
                continue;
            }
            if( pUser->m_nStatus != XTREAM_HIDE && pUser->m_nStatus != XTREAM_OFFLINE)
            {

                CLog::Log("FindOnlineUser",CLog::TYPE_DEBUG,"USER id %u USER status %d username %s",
                          pUser->m_nID,pUser->m_nStatus,pUser->m_szUserName);
                m_SendPacket << pUser->m_nID
                             << pUser->m_nSexy
                             << pUser->m_nStatus
                             << pUser->m_AllowRule
                             << pUser->m_szUserName
                             << pUser->m_szNickName;
                i++;
            }
            if( i >= m_nReturnCount)  //��෵��m_nReturnCount���û���Ϣ
                break;
        }
        //�����ϸ���
        m_pSearchFriendAck->count = i;
        m_pSearchFriendAck->nextpos = 0;//������Ч�ģ������
        m_pSearchFriendAck->nTotalNum = CUser::m_userhash.size();
    }
#endif
	*/
    else
    {
        SetErrorReturn("��ѯʧ��");
    }

    return nResult;
}



int CBusinessSearchFriend::FindPrecision()
{
    CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
    XT_XTREAM_SEARCH_ACK::tagFriendInfo  FriendInfo[MAX_SEARCH_FRINED_RETURN_NUM];
    int nResult = RESULT_FAILED;

    uint16 startpos =0;
    uint8  type;
    uint8 nRequireNum = 0 ;


    //��������ѯ��������һ�β�ѯֻ��ʹ��������һ������
    struct SearchCondition
    {
        union
        {
            char        user_name[MAX_USERNAME_LEN];
            uint32      userid;
            char        nick_name[MAX_NICKNAME_LEN];
            char        user_email[MAX_EMAIL_LEN];
        };
    } searchCondition;

    m_nReturnCount = 0;
    m_RecvPacket >>startpos >> type >> nRequireNum;
    if( nRequireNum ==0)
    {
        nRequireNum = sizeof(FriendInfo)/sizeof(FriendInfo[0]);
    }

    searchCondition.userid = 0;
    m_nReturnCount = nRequireNum;
    m_RecvPacket >> searchCondition.userid;
    if( searchCondition.userid != 0)
    {
        nResult = pDatabase->Search_User_WithUserID(searchCondition.userid,FriendInfo);
        if (RESULT_SUCCESS == nResult)
            m_nReturnCount = 1;
    }
    else
    {
        searchCondition.user_name[0] = '\0';
        m_RecvPacket >> searchCondition.user_name ;
        if( searchCondition.user_name[0] != '\0' )
        {
            nResult = pDatabase->Search_User_WithUserName(searchCondition.user_name,
                      FriendInfo);
            if( RESULT_SUCCESS ==  nResult )
            {
                m_nReturnCount = 1;
            }
        }
        else
        {
            searchCondition.nick_name[0] = '\0';
            m_nReturnCount = nRequireNum;
            m_RecvPacket >> searchCondition.nick_name;
            if( searchCondition.nick_name[0] != '\0' )
            {
                nResult = pDatabase->Search_User_WithNickName(searchCondition.nick_name,
                          startpos,
                          FriendInfo,
                          m_nReturnCount);
            }
            else
            {
                searchCondition.user_email[0] = '\0';
                m_RecvPacket >> searchCondition.user_email;
                if( searchCondition.user_email[0] != '\0' )
                {
                    nResult = pDatabase->Search_User_WithEmail(searchCondition.user_email,
                              startpos,
                              FriendInfo,
                              m_nReturnCount);
                }
                else
                {
                    nResult = RESULT_FAILED;
                }
            }
        }

    }

    if( DB_SUCCESS == nResult )
    {
        m_SendPacket << (uint8)RESULT_SUCCESS;
        m_SendPacket << (uint8)m_nReturnCount;
        if( m_nReturnCount < nRequireNum)
        {
            m_SendPacket << (uint16)0xffff;
        }
        else
        {
            m_SendPacket << (uint16)(startpos+m_nReturnCount);
        }
        m_SendPacket << (uint32)0;

#ifdef IM_USE_CACHE
        if (m_nReturnCount > 0)
        {
            //��ѯ����û�������״̬,��ȫ�ַ�������ѯ
            uint32 idList[MAX_SEARCH_FRINED_RETURN_NUM];
            GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[MAX_SEARCH_FRINED_RETURN_NUM];
            memset(userStatus, 0, sizeof(GSRV_GET_USER_STATUS_ACK::tagUserStatus) * MAX_SEARCH_FRINED_RETURN_NUM);

            int j =0;
            for( j = 0 ; j < m_nReturnCount; ++j)
            {
                idList[j] = FriendInfo[j].id;
                FriendInfo[j].status = XTREAM_OFFLINE;
            }
            CMemcacheQueryUserStatusi(m_pRouter,idList,m_nReturnCount,FriendInfo);
            //���ؽ��
            for( int i = 0 ; i < m_nReturnCount ; ++i )
            {
                m_SendPacket << FriendInfo[i].id
                             << FriendInfo[i].sexy
                             << FriendInfo[i].status
                             << FriendInfo[i].rule
                             << FriendInfo[i].UserName
                             << FriendInfo[i].NickName;
            }

        }
#else
        if (m_nReturnCount > 0)
        {
            //��ѯ����û�������״̬,��ȫ�ַ�������ѯ
            uint32 idList[MAX_SEARCH_FRINED_RETURN_NUM];
            GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[MAX_SEARCH_FRINED_RETURN_NUM];
            memset(userStatus, 0, sizeof(GSRV_GET_USER_STATUS_ACK::tagUserStatus) * MAX_SEARCH_FRINED_RETURN_NUM);

            uint8   idcount=0;
            int j =0;
            for( j = 0 ; j < m_nReturnCount; ++j)
            {
                idList[j] = FriendInfo[j].id;
                FriendInfo[j].status = XTREAM_OFFLINE;
            }
            idcount = m_nReturnCount;
            if( RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(
                    idList ,
                    userStatus ,
                    idcount ) )
            {
                //SetErrorReturn("��ѯ�û�״̬ʧ��");
                //return RESULT_FAILED;

                //����ʧ��,������Щ���Ϊ����
                for(j=0; j<m_nReturnCount; ++j)
                {
                    FriendInfo[j].status = XTREAM_OFFLINE;
                }
            }
            else
            {
                //�����û�״̬
                for( j =0; j < idcount; ++j)
                {
                    if( FriendInfo[j].id == userStatus[j].id)
                    {
                        FriendInfo[j].status = userStatus[j].status;
                    }
                }
            }
            //���ؽ��
            for( int i = 0 ; i < m_nReturnCount ; ++i )
            {
                m_SendPacket << FriendInfo[i].id
                             << FriendInfo[i].sexy
                             << FriendInfo[i].status
                             << FriendInfo[i].rule
                             << FriendInfo[i].UserName
                             << FriendInfo[i].NickName;
            }
        }

#endif
    }
    else
    {
        SetErrorReturn("û���ҵ���ؼ�¼");
    }

    return nResult;
}

//---------------------------------------------------------------------


CBusinessFriendModify::CBusinessFriendModify(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_FRIENDGROUP_MOD_REQ,CMD_FRIENDGROUP_MOD_ACK)
{
    m_pFriendGroupReq = NULL;
    m_pFriendGroupAck = NULL;
    m_pUser           = NULL;
}


int CBusinessFriendModify::DoWork()
{
    m_pFriendGroupReq = (XT_FRIENDGROUP_MOD_REQ*)m_RecvPacket.GetData();
    m_pFriendGroupAck = (XT_FRIENDGROUP_MOD_ACK*)m_SendPacket.GetData();


    m_pUser = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->uid, TRUE);

    if( NULL == m_pUser)
    {
        //�գ����û��Ѿ��������ˣ����ؼ�������
        CLog::Log("CBusinessFriendModify",CLog::TYPE_DEBUG,"�û�%d������",m_pFriendGroupReq->uid);
        return RESULT_SUCCESS;
    }

    uint8 nResult;
    switch(m_pFriendGroupReq->action)
    {
        case XT_FRIENDGROUP_MOD_REQ::ACTION_ADD:
            nResult = AddFriendToGroup();
            break;
        case XT_FRIENDGROUP_MOD_REQ::ACTION_MODIFY:
        case XT_FRIENDGROUP_MOD_REQ::ACTION_BLACK:
            nResult = ModifyFriendGroup();
            break;
        case XT_FRIENDGROUP_MOD_REQ::ACTION_DEL:
            nResult = DoDelFriendGroup(m_pFriendGroupReq->uid, m_pFriendGroupReq->fid);
            break;
        default:
            CLog::Log("CBusinessFriendModify",CLog::TYPE_ERROR,"����action����:%d, uid:%d",m_pFriendGroupReq->action,m_pFriendGroupReq->uid);
    }

    if( RESULT_SUCCESS ==nResult )
    {
        m_pFriendGroupAck->ret    = nResult;
        m_pFriendGroupAck->action = m_pFriendGroupReq->action;
        m_pFriendGroupAck->fid    = m_pFriendGroupReq->fid;
        m_pFriendGroupAck->group_id=m_pFriendGroupReq->group_id;
        m_SendPacket.SetCursor(sizeof(XT_FRIENDGROUP_MOD_ACK));
    }



    return RESULT_SUCCESS ;
}

uint8  CBusinessFriendModify::AddFriendToGroup()
{
    uint8 AllowRule;    //������֤��ʽ

    FriendInfo* pFriend = m_pUser->m_FriendList.Find(m_pFriendGroupReq->fid);

    //����öԷ��û���û��������б�����Ҫ����ӶԷ�Ϊ����,���޸�������
    if( NULL == pFriend )
    {
        if(m_pFriendGroupReq->group_id == GROUPID_BLACKLIST)
        {
            //�������ӵ�������������Ҫ��֤
            AllowRule = FRIEND_JOIN_ANYONE;
        }
        else
        {
            //���Է��û��Ƿ��ڱ���������
            shared_ptr<CUser> pFriendUser = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->fid);
            if( pFriendUser != NULL)   //�ҵ���
            {
                AllowRule = pFriendUser->m_AllowRule;
            }
            else
            {
                //û�ҵ�,�����ݿ��в���
                if( RESULT_FAILED ==m_pRouter->GetDataBaseWeb()->Get_User_JoinFlag(m_pFriendGroupReq->fid,AllowRule) )
                {
                    SetErrorReturn("�õ�������֤��ʽʧ��");
                    return RESULT_FAILED;
                }
            }
        }
        m_pFriendGroupAck->flag = AllowRule;
        switch(AllowRule)
        {
            case FRIEND_JOIN_ANYONE:
                return DoAddFriend();
                break;
            case FRIEND_JOIN_VERIFY:
            case FRIEND_JOIN_REJECT:
                //����Ҫ��һ������,��������֤��Ϣ���û�
                break;
        }

    }
    else
    {
        return ModifyFriendGroup();
    }

    return RESULT_SUCCESS;
}

uint8  CBusinessFriendModify::ModifyFriendGroup()
{
    FriendInfo* pFriend = m_pUser->m_FriendList.Find(m_pFriendGroupReq->fid);
    uint8 groupID = 0;
    uint8 AllowRule = FRIEND_JOIN_ANYONE;
    if( NULL == pFriend )
    {
        //û�иú��ѣ�������
//      SetErrorReturn("�޸ĺ��ѷ���ʧ��");
//      return RESULT_FAILED;
        m_pRouter->GetDataBaseWeb()->Get_User_FriendGroup(m_pUser->m_nID, m_pFriendGroupReq->fid, groupID);
    }
    else
    {
        groupID = pFriend->GroupID;
    }

    if( groupID != m_pFriendGroupReq->group_id)
    {
        if (groupID == GROUPID_BLACKLIST)
        {
            shared_ptr<CUser> pFriendUser = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->fid);
            if( pFriendUser != NULL)   //�ҵ���
            {
                AllowRule = pFriendUser->m_AllowRule;
            }
            else
            {
                //û�ҵ�,�����ݿ��в���
                if( RESULT_FAILED ==m_pRouter->GetDataBaseWeb()->Get_User_JoinFlag(m_pFriendGroupReq->fid,AllowRule) )
                {
                    SetErrorReturn("�õ�������֤��ʽʧ��");
                    return RESULT_FAILED;
                }
            }
            if (AllowRule == FRIEND_JOIN_ANYONE)
                DoAddFriend();
        }
        else
        {
			// �޸ĺ��ѷ���
			if(RESULT_FAILED == DoModFriendGroup(m_pFriendGroupReq->uid,m_pFriendGroupReq->fid,m_pFriendGroupReq->group_id) )
			{
				return RESULT_FAILED;
			}
			else
			{
				if (pFriend)
                    pFriend->GroupID = m_pFriendGroupReq->group_id;
			}
        }
    }

    m_pFriendGroupAck->flag = AllowRule;
    if (m_pFriendGroupReq->action == XT_FRIENDGROUP_MOD_REQ::ACTION_BLACK)
    {
    	DoDelFriendGroup(m_pFriendGroupReq->fid, m_pFriendGroupReq->uid);
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->fid, TRUE);
        if( pUser != NULL) //�ҵ���
        {
            pUser->m_FriendList.Del(m_pFriendGroupReq->uid);
        }

        //������֪ͨɾ���Լ�
        SendFriendDelMe(m_pFriendGroupReq->fid,m_pFriendGroupReq->uid);
    }

    return RESULT_SUCCESS;
}

void  CBusinessFriendModify::SendFriendDelMe(uint32 friendID,uint32 myID)
{
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_FRIENDADD_NOTIFY),
        friendID);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL;


    tagXT_FRIENDDEL_NOTIFY* pFriendDelNotify = (XT_FRIENDADD_NOTIFY*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    pFriendDelNotify->ThatOneID = myID;

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_FRIENDADD_NOTIFY);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}


uint8   CBusinessFriendModify::DoDelFriendGroup(int userid, int friendid)
{
	// ����imapi�ӿ�ɾ������
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strDelFriendUrl(CAppManager::GetInstance().GetConfig().GetDelFriendUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << userid;
	ssParams<<"&friendId=" << friendid;
	
	string strRlt;
	int httpStaus = httpClient.Post(strDelFriendUrl, ssParams.str(), strRlt);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strRlt, jvRecvResponse))
	    {
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
	    	{	
	    		return RESULT_SUCCESS;
	    	}
	    }
	}

	SetErrorReturn("ɾ������ʧ��");
	return RESULT_FAILED;
}

uint8  CBusinessFriendModify::DoAddFriend()
{	
	// �õ�����״̬
	uint8 status = XTREAM_OFFLINE;
	shared_ptr<CUser> pUserFriend = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->fid);
	if(pUserFriend != NULL)
	{
		status = pUserFriend->m_nStatus;
	}

	// ����imapi�ӿ���Ӻ���
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strAddFriendUrl(CAppManager::GetInstance().GetConfig().GetAddFriendUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << m_pFriendGroupReq->uid;
	ssParams<<"&friendId=" << m_pFriendGroupReq->fid;
	ssParams<<"&status=" << (int)status;
	ssParams<<"&groupId=" << (int)m_pFriendGroupReq->group_id;
	
	string strRlt;
	int httpStaus = httpClient.Post(strAddFriendUrl, ssParams.str(), strRlt);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strRlt, jvRecvResponse))
	    {
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
	    	{	
	    		return RESULT_SUCCESS;
	    	}
	    }
	}

	SetErrorReturn("��Ӻ���ʧ��");
	return RESULT_FAILED;
/*
    //���ݿ����
    int nResult = m_pRouter->GetDataBaseWeb()->Add_Friend(
                      m_pFriendGroupReq->uid,
                      m_pFriendGroupReq->fid,
                      m_pFriendGroupReq->group_id) ;
    if( nResult ==  DB_SUCCESS)
    {
        //�Ӹú��ѵ��ڴ��б�
        FriendInfo* pFriendInfo = m_pUser->m_FriendList.Add(m_pFriendGroupReq->fid);
        if( pFriendInfo )
        {
            pFriendInfo->id = m_pFriendGroupReq->fid;
            pFriendInfo->GroupID = m_pFriendGroupReq->group_id;
            if (m_pFriendGroupReq->group_id != GROUPID_BLACKLIST)
                SendFriendNotify(m_pFriendGroupReq->fid,m_pFriendGroupReq->uid);
        }
        return RESULT_SUCCESS;
    }
    else
    {
        char szBuf[MAX_ERROR_BUF];
        CError::DB_Error(nResult, szBuf, true);
        SetErrorReturn(szBuf);
    }

    return RESULT_FAILED;
*/
}

// ���ýӿ��޸ĺ��ѷ���
uint8  CBusinessFriendModify::DoModFriendGroup(int uid, int friendid, int groupid)
{	
	// ����imapi�ӿ���Ӻ���
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strModFriendUrl(CAppManager::GetInstance().GetConfig().GetModFriendGroupUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << uid;
	ssParams<<"&friendId=" << friendid;
	ssParams<<"&groupId=" << groupid;
	
	string strRlt;
	int httpStaus = httpClient.Post(strModFriendUrl, ssParams.str(), strRlt);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strRlt, jvRecvResponse))
	    {
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
	    	{	
	    		return RESULT_SUCCESS;
	    	}
	    }
	}

	SetErrorReturn("�޸ĺ��ѷ���ʧ��");
	return RESULT_FAILED;
}


//֪ͨ�û���ĳ���˼�Ϊ�˺���
//BeAddedID --���ӷ�
//ToAddID   --������Ӻ��ѷ�
void  CBusinessFriendModify::SendFriendNotify(uint32 BeAddedID,uint32 ToAddID)
{
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_FRIENDADD_NOTIFY),
        BeAddedID);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = XT_SERVER_TRANSMIT::TYPE_FRIEND_ADD;


    XT_FRIENDADD_NOTIFY* pFriendAddNotify = (XT_FRIENDADD_NOTIFY*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    pFriendAddNotify->ThatOneID = ToAddID;

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_FRIENDADD_NOTIFY);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);

}



//---------------------------------------------------------------------

CBusinessVerifyFriend::CBusinessVerifyFriend(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_USERAUTH_REQ,CMD_USERAUTH_ACK)
{

}


int CBusinessVerifyFriend::DoWork()
{
    XT_USERAUTH_REQ* pVerifyFriendReq = (XT_USERAUTH_REQ*)m_RecvPacket.GetData();
    XT_USERAUTH_ACK* pVerifyFriendAck = (XT_USERAUTH_ACK*)m_SendPacket.GetData();

    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->sid, TRUE);
    if( pUser != NULL)
    {
        if (pVerifyFriendReq->action == XT_USERAUTH_REQ::ACTION_REQUEST && pUser->m_FriendList.GetSize() >= MAX_FRIENDS_NUM)
        {
            m_SendPacket<<(uint8)RESULT_FAILED
                        <<"���ĺ���������,��������Ӻ����ˣ�";
        }
        else
        {
            pVerifyFriendAck->ret    = RESULT_SUCCESS;
            pVerifyFriendAck->action = pVerifyFriendReq->action;
            pVerifyFriendAck->to_id  = pVerifyFriendReq->to_id;
            m_SendPacket.SetCursor(sizeof(XT_USERAUTH_ACK));
            SendVerifyFriendNotify(pVerifyFriendReq->to_id,pVerifyFriendReq->groupId);
        }
    }

    return RESULT_SUCCESS;
}


void CBusinessVerifyFriend::SendVerifyFriendNotify(uint32 to_id, uint8 groupid)
{
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        (uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),
        to_id);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = XT_SERVER_TRANSMIT::TYPE_VERIFY_FRIEND;


    BYTE* pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    memcpy(pVerifyFriendReq,m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize());


    //����Է��ڵ�ǰIM������,ֱ�Ӹ�������
    XT_USERAUTH_ACK* pVerifyFriendAck = (XT_USERAUTH_ACK*)m_SendPacket.GetData();
    if( pVerifyFriendAck->action == XT_USERAUTH_REQ::ACTION_AGREE )
    {
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(to_id);
        if( pUser != NULL )
        {
            CBusinessServerMsg ServerMsg(m_pRouter);
            RecvDataStruct      recvData;
            memcpy(recvData.data,sendData.data,sendData.data_len);
            recvData.data_len = sendData.data_len;
            ServerMsg.SetRowPacket((BYTE*)&recvData,recvData.data_len);
            ServerMsg.DoWork();
        }
    }




    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME );
}

//---------------------------------------------------------------------

CBusinessModifyStatus::CBusinessModifyStatus(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_STATUS_MOD_REQ,CMD_STATUS_MOD_ACK)
{

}


int CBusinessModifyStatus::DoWork()
{
    XT_STATUS_MOD_REQ* pVerifyFriendReq = (XT_STATUS_MOD_REQ*)m_RecvPacket.GetData();
    XT_STATUS_MOD_ACK* pVerifyFriendAck = (XT_STATUS_MOD_ACK*)m_SendPacket.GetData();

    uint32& UserId = pVerifyFriendReq->id;
    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(UserId, TRUE);


    CLog::Log("CBusinessModifyStatus",CLog::TYPE_DEBUG,"�޸��û�%u״̬ �� %d IN",UserId,pVerifyFriendReq->status);
    if( pUser != NULL )
    {
        uint8 oldState = pUser->m_nStatus;
        pUser->m_nStatus = pVerifyFriendReq->status;

        //֪ͨȫ�ַ�����״̬����
        GSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser UserStatus;
        UserStatus.id = UserId;
        UserStatus.status = pVerifyFriendReq->status;
        uint8 count = 1;
        if (pUser->m_nStatus == XTREAM_OFFLINE)
            CLog::Log("CBusinessModifyStatus", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d", pUser->m_nID);

        if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->ModiUserStatus(&UserStatus,count))
        {
            SetErrorReturn("���緱æ,����״̬ʧ��");
        }
        else
        {
            //֪ͨ���û������ߺ���״̬����
            pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());

            //֪ͨ�������״̬
            pUser->NotifyFamilyStatus(&(CAppManager::GetInstance().GetAppNetWork()),m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

            pVerifyFriendAck->ret = RESULT_SUCCESS;
            pVerifyFriendAck->status = pVerifyFriendReq->status;

            m_SendPacket.SetCursor(sizeof(XT_STATUS_MOD_ACK));

            if(pUser->m_nUsertype == XT_LOGIN_REQ::CS)  //�ͷ�
            {
                struct tm newtime;
                time_t aclock;
                time( &aclock );
                localtime_s(&newtime, &aclock );
                using namespace LogInterface;
                using namespace std;
                if(pUser->m_nStatus == XTREAM_LEAVE)        //�������״̬
                {
                    hessianLogi hess(pUser->m_merchantid,pUser->m_nID);
                    try
                    {
                        SubMerchantAction info;
                        info.merchantID = -1;//pUser->m_merchantid;
                        info.subMerchantID = pUser->m_nID;
                        info.actionTime = (int)aclock;
                        info.actionType = ACTION_LEAVE;

                        ResultInfo res = hess.GetHessiani()->uploadAction(info);

                        CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                                  info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
                    }
                    catch (std::exception& e)
                    {
                        CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                                  pUser->m_merchantid, pUser->m_nID, e.what());
                    }
                }
                if(oldState == XTREAM_LEAVE)        //�ӹ���״̬�˳�
                {

                    //�ϴ���¼����

                    hessianLogi hess(pUser->m_merchantid,pUser->m_nID);
                    try
                    {
                        SubMerchantAction info;
                        info.merchantID = -1;//pUser->m_merchantid;
                        info.subMerchantID = pUser->m_nID;
                        info.actionTime = (int)aclock;
                        info.actionType = ACTION_NOT_LEAVE;

                        ResultInfo res = hess.GetHessiani()->uploadAction(info);

                        CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                                  info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
                    }
                    catch (std::exception& e)
                    {
                        CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                                  pUser->m_merchantid, pUser->m_nID, e.what());
                    }
                }

            }

        }


    }
    CLog::Log("CBusinessFriendModify",CLog::TYPE_DEBUG,"�޸��û�%u״̬ �� %d OUT",UserId,pVerifyFriendReq->status);

    return RESULT_SUCCESS;
}
//---------------------------------------------------------------------
#define MAX_LIST_NUM 20
CBusinessWebBusiList::CBusinessWebBusiList(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_WEB_BUSI_LIST_REQ,CMD_WEB_BUSI_LIST_ACK)
{
    pListInfo = new XT_WEB_BUSI_LIST_ACK::BusiList[MAX_LIST_NUM];
    pData = new SendDataStruct;
    if(pListInfo == NULL || pData == NULL)
        CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"New failed!");
}

CBusinessWebBusiList::~CBusinessWebBusiList()
{
    if(pListInfo)
    {
        delete[] pListInfo;
        pListInfo = NULL;
    }
    if(pData)
    {
        delete pData;
        pData = NULL;
    }
}

int CBusinessWebBusiList::DoWork()
{
    XT_WEB_BUSI_LIST_REQ WebBusiList;
    uint32 nRetCount;
    uint32 len = 0;
    uint32 i = 0;

    uint32 id_list[MAX_LIST_NUM];
    tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_LIST_NUM];

    XT_WEB_BUSI_LIST_ACK* pWebBusiListAck = (XT_WEB_BUSI_LIST_ACK*)m_SendPacket.GetData();

    CancelReply();

    m_RecvPacket>>WebBusiList.id
                >>WebBusiList.msgnum;
    CLog::Log("CBusinessWebBusiList",CLog::TYPE_DEBUG,"QueryUseriD %d  msgnum %d ",WebBusiList.id,WebBusiList.msgnum);

    nRetCount = WebBusiList.msgnum;
    if(nRetCount > MAX_LIST_NUM)
        nRetCount = MAX_LIST_NUM;
    if(m_pRouter->GetDataBaseMini()->WebBusiListReq(&WebBusiList,pListInfo,nRetCount) == DB_ERROR)
    {
        CLog::Log("WebBusiList", CLog::TYPE_ERROR, "WebBusiList Req Failed! id:%d ",WebBusiList.id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebBusiList.id
                    <<WebBusiList.msgnum;
        return RESULT_SUCCESS;
    }
    seq = m_pRecvHead->seq;
    if(nRetCount > 0)
    {
        uint8 count = nRetCount;
        for(uint8 j = 0; j < nRetCount; j ++)
        {
            id_list[j] = pListInfo[j].id;
        }

#ifdef IM_USE_CACHE
        if (MEMCACHED_NO_INFO < CMemcacheQueryUserStatusi(m_pRouter,id_list,count,UStatus))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"QueryUserStatus error! id:%d ",WebBusiList.id);
        }
#else

        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(id_list,UStatus,count))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"QueryUserStatus error! id:%d ",WebBusiList.id);
        }

#endif
    }

    while(nRetCount > i)
    {
        pWebBusiListAck->actualnum = 0;

        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebBusiList.id
                    <<WebBusiList.msgnum
                    <<pWebBusiListAck->actualnum;

        for(; i < nRetCount; i ++)
        {
            len = sizeof(pListInfo[i].id)+strlen(pListInfo[i].nickname)+sizeof(pListInfo[i].time)
                  +sizeof(UStatus[i].status)+strlen(pListInfo[i].headImgUrl)+strlen(pListInfo[i].usersign);
            if(!m_SendPacket.HasData(len))
            {
                break;
            }

            m_SendPacket<<pListInfo[i].id
                        <<pListInfo[i].nickname
                        <<pListInfo[i].time
                        <<UStatus[i].status
                        <<pListInfo[i].headImgUrl
                        <<pListInfo[i].usersign;
            pWebBusiListAck->actualnum ++;

            CLog::Log("CBusinessWebBusiList",CLog::TYPE_DEBUG,
                      "pListInfo id:%d, nickname:%s, time:%u, status:%d,headImgUrl:%s usersign %s,count:%u",
                      pListInfo[i].id, pListInfo[i].nickname, pListInfo[i].time,
                      UStatus[i].status, pListInfo[i].headImgUrl,pListInfo[i].usersign,pWebBusiListAck->actualnum);
        }
        SendWebBusiListAck(m_SendPacket);
        m_SendPacket.Reset();
    }
    if(nRetCount == 0)
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebBusiList.id
                    <<WebBusiList.msgnum
                    <<(uint32)0;
        SendWebBusiListAck(m_SendPacket);
    }
    return RESULT_SUCCESS;
}

void CBusinessWebBusiList::SendWebBusiListAck(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = CAppManager::GetInstance().GetSendSeq();
    pHead->cmd  = CMD_WEB_BUSI_LIST_ACK;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),pData->data,pData->data_len,SEND_RETRY_TIME);

}

//---------------------------------------------------------------------
#define MAX_MSG_NUM 25
CBusinessWebMoreMsg::CBusinessWebMoreMsg(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_WEB_MORE_MSG_REQ,CMD_WEB_MORE_MSG_ACK)
{
    pMsgInfo = new XT_WEB_MORE_MSG_ACK::MsgInfo[MAX_MSG_NUM];
    pData = new SendDataStruct;
    if(pMsgInfo == NULL || pData == NULL)
        CLog::Log("CBusinessWebMoreMsg",CLog::TYPE_ERROR,"New failed");
}

CBusinessWebMoreMsg::~CBusinessWebMoreMsg()
{
    if(pMsgInfo)
    {
        delete[] pMsgInfo;
        pMsgInfo = NULL;
    }
    if(pData)
    {
        delete pData;
        pData = NULL;
    }
}

int CBusinessWebMoreMsg::DoWork()
{
    XT_WEB_MORE_MSG_REQ WebMoreMsg;
    vector<XT_WEB_MORE_MSG_ACK::MsgInfo> user_msgs;
    uint32 nRetCount;

    uint32 i = 0;
    uint32 len = 0;
    uint32 tmp_len = 0;
    std::vector<uint32_t> ids;

    //XT_WEB_MORE_MSG_ACK* pWebMsgAck = (XT_WEB_MORE_MSG_ACK*)m_SendPacket.GetData();
    uint8 *pOver = (uint8*)(m_SendPacket.GetData()+21);
    uint32 *pActualnum = (uint32*)(m_SendPacket.GetData()+22);

    CancelReply();

    m_RecvPacket>>WebMoreMsg.sid
                >>WebMoreMsg.did
                >>WebMoreMsg.msgtime
                >>WebMoreMsg.msgnum
                >>WebMoreMsg.msgid
                >>WebMoreMsg.uuid
                >>WebMoreMsg.isMerchant;

    nRetCount = WebMoreMsg.msgnum;
    if(nRetCount > 25)
        nRetCount = 25;


    ids.clear();

    CLog::Log("WebMoreMsg", CLog::TYPE_DEBUG, "WebMoreMsg.isMerchant:%d, nRetCount:%d",WebMoreMsg.isMerchant,nRetCount);
    if(WebMoreMsg.isMerchant == 1)
    {
        MYSQL_LIB->getAllSubAccount(WebMoreMsg.did, ids);
    }
    else
    {
        ids.push_back(WebMoreMsg.did);
    }
    //get all sub users id
    CLog::Log("WebMoreMsg", CLog::TYPE_DEBUG, "WebMoreMsg.did:%d, size:%d ",WebMoreMsg.did,ids.size());



    if(m_pRouter->GetDataBaseMini()->WebMsgMoreReq(&WebMoreMsg,pMsgInfo,ids,nRetCount) == DB_ERROR)
    {
        CLog::Log("WebMoreMsg", CLog::TYPE_ERROR, "WebMoreMsg Req Failed! sid:%d ",WebMoreMsg.sid);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebMoreMsg.sid
                    <<WebMoreMsg.did
                    <<WebMoreMsg.msgtime
                    <<WebMoreMsg.msgnum
                    <<WebMoreMsg.msgid
                    <<WebMoreMsg.uuid;
        SendWebMoreMsgAck(m_SendPacket);//��ȡʧ��Ҳ֪ͨ
        return RESULT_SUCCESS;
    }

    tmp_len = sizeof(uint32)+sizeof(uint32)+sizeof(uint32)+sizeof(uint8)+sizeof(uint32)+sizeof(uint16);
    seq = m_pRecvHead->seq;

    while(nRetCount > i)
    {
        uint32 actualnum= 0;

        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebMoreMsg.sid
                    <<WebMoreMsg.did
                    <<WebMoreMsg.msgtime
                    <<WebMoreMsg.msgnum
                    <<nRetCount
                    <<(uint8)1
                    <<(uint32)0/*pWebMsgAck->actualnum*/
                    <<WebMoreMsg.uuid;
        for(; i < nRetCount; i ++)
        {
            len = tmp_len + strlen(pMsgInfo[i].fontName)+strlen(pMsgInfo[i].nickName)+strlen(pMsgInfo[i].data) + 3*sizeof(uint16) + 3;
            if(!m_SendPacket.HasData(len))
            {
                break;
            }

            CLog::Log("WebMoreMsg", CLog::TYPE_DEBUG, "userid:%d, nickname:%s, data:%s ",
                      pMsgInfo[i].id,pMsgInfo[i].nickName,pMsgInfo[i].data);

            m_SendPacket<<pMsgInfo[i].id
                        <<pMsgInfo[i].send_time
                        <<pMsgInfo[i].msgid
                        <<pMsgInfo[i].fontSize
                        <<pMsgInfo[i].fontColor
                        <<pMsgInfo[i].fontStyle
                        <<pMsgInfo[i].fontName
                        <<pMsgInfo[i].nickName
                        <<pMsgInfo[i].data;
            actualnum++;
        }
        if(i < nRetCount)
            *pOver = (uint8)0;
        else
            *pOver = (uint8)1;

        *pActualnum = actualnum;   

        SendWebMoreMsgAck(m_SendPacket);
        m_SendPacket.Reset();
    }
    if(nRetCount == 0)
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebMoreMsg.sid
                    <<WebMoreMsg.did
                    <<WebMoreMsg.msgtime
                    <<WebMoreMsg.msgnum
                    <<nRetCount
                    <<(uint8)1
                    <<(uint32)0
                    <<WebMoreMsg.uuid;
        SendWebMoreMsgAck(m_SendPacket);
    }
    return RESULT_SUCCESS;
}

void CBusinessWebMoreMsg::SendWebMoreMsgAck(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        pHead->flag = XT_HEAD::FLAG_CNTREQUEST;
    }
    else
    {
        pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
    }
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = CAppManager::GetInstance().GetSendSeq();
    pHead->cmd  = CMD_WEB_MORE_MSG_ACK;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),pData->data,pData->data_len,SEND_RETRY_TIME);

}


//---------------------------------------------------------------------

CBusinessSendMessage::CBusinessSendMessage(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_P2PMSG_SEND,CMD_P2PMSG_ACK)
{
#ifdef PRESS_TEST
    LoadMerchantId();
#endif
}

#if 0
void CBusinessSendMessage::updateCsQueue(ClientService *clientService)
{
    CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"���¿ͷ���Ϣ %u has %u !",clientService->m_nID);
    uint8 count = 1;
    CSinfo_t csinfo;

    sprintf(csinfo.m_Csname,"%s",clientService->m_CSname);
    csinfo.m_CurConsult = clientService->m_CurConsult;
    csinfo.m_CurReception = clientService->m_CurReception;
    csinfo.m_MaxRecConsult = clientService->m_MaxRecConsult;
    csinfo.m_nID = clientService->m_nID;
    csinfo.m_nChattingBuyerID = clientService->GetChattingBuyerID();
    csinfo.m_nDirectorID = clientService->GetDirectorID();

    csinfo.m_Customerid.clear();
    map<uint32,CSortArray<customer_t> *>::iterator mappedIter;
    for(mappedIter = clientService->m_Customerlist.begin(); mappedIter != clientService->m_Customerlist.end();
        mappedIter++)
    {
        for(int i = 0; i < (mappedIter->second)->GetSize(); ++i)
        {
            customer_t *pFriendInfo = (mappedIter->second)->FindByPos(i);
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT,"��ǰ�� %u  ���û� %u",
                      pFriendInfo->teamid,
                      pFriendInfo->userid,
                      pFriendInfo->ReceptionState);
            csinfo.m_Customerid.push_back(pFriendInfo->userid);
        }
    }

    m_pRouter->GetGlobalBusi()->AddModiCSInfo(&csinfo,count,tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_MODIFY);

}
#endif

void CBusinessSendMessage::SaveWebMsg()
{
    XT_TALK msg;

    //�������յ�����Ϣ��ʱ��
    time(&(msg.recv_time));


    m_RecvPacket>>msg.from_id
                >>msg.to_id
                >>msg.data_type
                >>msg.ver
                >>msg.send_time
                >>msg.recv_flag;

    CLog::Log("WebMsgSave", CLog::TYPE_DEBUG, "msg type %d",msg.data_type);

    if(msg.data_type == XT_MSG::SF_FILE_REQ || msg.data_type == XT_MSG::SF_PACKET_REQ
       || msg.data_type == XT_MSG::SF_FILE_ACK /*|| msg.data_type == XT_MSG::SF_PACKET_ACK*/)
        m_RecvPacket.ReadData(msg.data,sizeof(msg.data));

    //����Ϊweb�豸
    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),m_pRecvHead->did) == TYPE_WEB)
        msg.to_terminal = XT_TALK::WEB_TERMINAL;

    //if(m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ || m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)
    //������Ϣ�����۵�
    if(msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::XT_RECOMENT || msg.data_type == XT_MSG::AUTO_REPLY)
    {
        m_RecvPacket>>msg.fontSize
                    >>msg.fontColor
                    >>msg.fontStyle
                    >>msg.fontName;

        m_RecvPacket.ReadData(msg.data,sizeof(msg.data));
        m_RecvPacket>>msg.from_nickname;

        //ֻ��web?
        if( (msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::XT_RECOMENT)
            /*&& (m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ || m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)*/
          )
        {
            CLog::Log("WebMsgSave", CLog::TYPE_DEBUG, "%s %s", msg.data, msg.from_nickname);
#if 0
//ѹ�����ʱ��
#ifdef PRESS_TEST
            char strTime[56];
            DWORD nBeginTime = timeGetTime();
            sprintf(strTime,"#11#%u",nBeginTime);
            strcat(msg.data,strTime);
            CLog::Log("WebMsgSave", CLog::TYPE_DEBUG, "[%u %u %u %u %u]  [%u %u %u %u %s] [%s %s ] [%u]",
                      msg.from_id, msg.to_id, msg.data_type, msg.ver, msg.send_time,
                      msg.recv_flag,msg.fontSize,msg.fontColor, msg.fontStyle, msg.fontName,
                      msg.data,msg.from_nickname,(uint32)pthread_self());
#endif
#endif
/*
#ifndef IMG_SAVE
            //���ﲻ�棬����һ���߳�����
            uint32 logintime =0;
            CUser::m_UserListLock.Lock();
            CUser *pUser = NULL;//CUser::m_pUserListHead;
            //int m_nID = (xtTalkMsg.from_id>xtTalkMsg.to_id)?xtTalkMsg.from_id:xtTalkMsg.to_id;
            //�����ʱ��
            int m_nID;
            if(xtTalkMsg.from_id > Id_Boundary  && xtTalkMsg.from_id < cs_Boundary)
            {
                m_nID = xtTalkMsg.from_id;
            }
            else
            {
                m_nID = xtTalkMsg.to_id;
            }
            __gnu_cxx::hash_map<uint32,CUser*>::iterator iter = CUser::m_userhash.find(m_nID);
            if(iter != CUser::m_userhash.end())
            {
                pUser = iter->second;
                logintime = pUser->m_nLoginTime;
            }
            CUser::m_UserListLock.UnLock();
            if(m_pRouter->GetDataBaseMini()->WebMsgSave(&msg,logintime) != DB_SUCCESS)
            {
                CLog::Log("WebMsgSave", CLog::TYPE_ERROR, "WebMsg Save Failed");
            }
#endif
*/
        }
    }

    if(msg.data_type == XT_MSG::XT_SERVICE)
    {
        uint32 seller_id = msg.from_id;
        uint32 buyer_id = msg.to_id;
        uint16 recomm = 0;

        if(m_pRouter->GetDataBaseMini()->WebServiceSave(buyer_id,seller_id,recomm,msg.send_time) == DB_ERROR)
        {
            CLog::Log("WebRecomentSave", CLog::TYPE_ERROR, "WebServiceSave Save Failed! buyer_id:%d, seller_id:%d", buyer_id, seller_id);
        }
    }

    else if(msg.data_type == XT_MSG::XT_RECOMENT)
    {
        uint32 logintime = 0;
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(msg.from_id);
        if(pUser == NULL)
        {
            tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo UserInfo;
            uint8 count = 1;
            m_pRouter->GetGlobalBusi()->QueryUserInfo(&msg.from_id,count,&UserInfo);
            logintime = UserInfo.logintime;

            CLog::Log("WebRecomentSave", CLog::TYPE_IMPORTANT, "WebRecommSave find buyer %u logintime %u On Memcached", msg.from_id,logintime);
        }
        else
        {
            logintime = pUser->m_nLoginTime;
        }
        if(m_pRouter->GetDataBaseMini()->WebRecommSave(msg.from_id,msg.to_id,msg.ver,msg.send_time,logintime,msg.data) == DB_ERROR)
        {
            CLog::Log("WebRecomentSave", CLog::TYPE_ERROR, "WebRecoment Save Failed! from_id:%d, to_id:%d ,ver:%d,logintime %u send_time:%d,msg %s",
                      msg.from_id,msg.to_id,msg.ver,logintime,msg.send_time,msg.data);
        }
        CLog::Log("WebRecomentSave", CLog::TYPE_IMPORTANT, "WebRecoment Saved! from_id:%d, to_id:%d ,ver:%d, logintime %u send_time:%d,msg %s",
                  msg.from_id,msg.to_id,msg.ver,logintime,msg.send_time,msg.data);

        //XT_TALK* talk = (XT_TALK*)m_RecvPacket.GetData();
        //talk->data_type = XT_MSG::IM_MSG;
    }


    //���ֺ�ͼƬ��Ϣ�������̱߳���
#ifdef IMG_SAVE
    if(msg.data_type == XT_MSG::IM_MSG ||  msg.data_type == XT_MSG::SF_FILE_REQ
       || msg.data_type == XT_MSG::SF_PACKET_REQ || msg.data_type == XT_MSG::SF_FILE_ACK || msg.data_type == XT_MSG::AUTO_REPLY/*|| msg.data_type == XT_MSG::SF_PACKET_ACK*/)
    {
		uint32 m_nID;
		if(msg.from_id > Id_Boundary && msg.from_id < cs_Boundary)
		{
			m_nID = msg.from_id;
		}
		else
		{
			m_nID = msg.to_id;
		}
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_nID);
        if(pUser == NULL)
        {
            tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo UserInfo;
            uint8 count = 1;
            m_pRouter->GetGlobalBusi()->QueryUserInfo(&m_nID,count,&UserInfo);
            msg.logintime = UserInfo.logintime;
        }
        else
        {
            msg.logintime = pUser->m_nLoginTime;
        }
		

		//�������redis ��ʱʱ��
		if(m_nID == msg.from_id)
        {
	        if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->setExpireUserinfo(m_nID))
	        {
	            CLog::Log("SaveWebMsg", CLog::TYPE_ERROR,"�����û� %u ��ʱʱ��ʧ��!",m_nID);
	        }
			else
			{
				CLog::Log("SaveWebMsg", CLog::TYPE_IMPORTANT,"�����û� %u ��ʱʱ��!", m_nID);
			}
		}
        CAppManager::GetInstance().GetChatMsgBuffer0()->Push((BYTE*)&msg,sizeof(XT_TALK));
    }
#endif
}






int CBusinessSendMessage::DoWork()
{
	XT_SERVER_P2PMSG_SEND* pP2PMsgSend  = (XT_SERVER_P2PMSG_SEND*)m_RecvPacket.GetData();
    XT_SERVER_P2PMSG_ACK* pP2PMsgAck    = (XT_SERVER_P2PMSG_ACK*)m_SendPacket.GetData();

    pP2PMsgAck->Sender  = pP2PMsgSend->Sender;
    pP2PMsgAck->Receiver= pP2PMsgSend->Recver;
    pP2PMsgAck->type    = pP2PMsgSend->type;
#ifdef IM_USE_MULTIDEVICE

    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	uint32 deviceId = lpRecvHead->deviceId;
#endif

    m_SendPacket.SetCursor(sizeof(XT_SERVER_P2PMSG_ACK));
#ifdef PRESS_TEST
    DWORD nBeginTime = timeGetTime();
#endif

#ifdef PRE_INTERNAL
#else
    SaveWebMsg();
#endif

#ifdef PRESS_TEST
    CLog::Log("SendMsg1",CLog::TYPE_STATISTIC,"TimeStatistic: %u %u %u %u",
              pP2PMsgAck->Sender, pP2PMsgAck->Receiver,pP2PMsgAck->type,timeGetTime() - nBeginTime);

    nBeginTime = timeGetTime();
#endif

    //ת������Ϣ
    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),m_pRecvHead->did) == TYPE_WEB &&
       (pP2PMsgSend->type >= XT_MSG::SF_FILE_REQ && pP2PMsgSend->type <=XT_MSG::SF_PACKET_ACK)
      )
    {
        //ͼƬ��Ϣ��¼�Ĳ������web�ˣ���ͼƬ������

    }
    else
    {
    	#ifdef IM_USE_MULTIDEVICE		
        RouterMsgNotify(pP2PMsgSend->Recver,deviceId);
		#else
        RouterMsgNotify(pP2PMsgSend->Recver);
		#endif
    }
#ifdef PRESS_TEST
    CLog::Log("SendMsg2",CLog::TYPE_STATISTIC,"TimeStatistic: %u %u %u %u",
              pP2PMsgAck->Sender, pP2PMsgAck->Receiver,pP2PMsgAck->type,timeGetTime() - nBeginTime);
#endif

#ifdef WEBTIMEOUT

    //web�˷�����Ϣ�����������ʱ�䵽��ʱ����
    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),m_pRecvHead->sid) == TYPE_WEB)
    {
 #ifdef IM_USE_MULTIDEVICE  
 		m_pRouter->GetGlobalBusi()->ModiDeviceUserHeartBeat(m_pRecvHead->sid,m_pRecvHead->deviceId,timeGetTime());
 #else 
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->sid);
        if( pUser != NULL )
        {
            pUser->m_nNewlyLiveTime = timeGetTime();
            m_pRouter->GetGlobalBusi()->ModiNewLiveTime(m_pRecvHead->sid,pUser->m_nNewlyLiveTime);
            CLog::Log("SendMsg1",CLog::TYPE_STATISTIC,"����web�û�%u�����ʱ��%u",
                      pUser->m_nID,pUser->m_nNewlyLiveTime);

            // ����������ڴ�
			LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(m_pRecvHead->sid);
			if( pOnlineUser != NULL )
			{
				pOnlineUser->newlylivetime = pUser->m_nNewlyLiveTime;
			}          
        }
#endif
    }
#endif
    return RESULT_SUCCESS;
}

void CBusinessSendMessage::RouterMsgNotify(uint32 to_id,uint32 deviceId)
{
    // �ж���Ϣ���ͣ����Ϊ����Ự��Ϣ����Ҫ����Ϣת���ͷ��Ϳͻ�
    XT_SERVER_P2PMSG_SEND* pP2PMsgSend  = (XT_SERVER_P2PMSG_SEND*)m_RecvPacket.GetData();
    if(pP2PMsgSend->type == XT_MSG::IM_MSG_INSERT)
    {
        // �ж�����������Ƿ���ڿͷ����������ڱ���أ�������ڱ���أ�����Ϣת��������
        bool bExistMaster = CUser::g_cs_exist(pP2PMsgSend->Sender, m_pRouter->GetGlobalBusi());        // ����
        bool bExistCS = CUser::g_cs_exist(pP2PMsgSend->Recver, m_pRouter->GetGlobalBusi());            // �ͷ�
        if(bExistMaster && bExistCS)
        {
            // ����
            // �жϿͷ��Ƿ񱻼��
            bool monitored = false;
            uint32 cs_master_id = (uint32)-1;
            bool success = CUser::g_cs_ismonitored(pP2PMsgSend->Recver, monitored, cs_master_id, m_pRouter->GetGlobalBusi());
            if(success && monitored && cs_master_id == pP2PMsgSend->Sender)
            {
                uint32 nBuyerID = (uint32)-1;       // ���id			// ����Ự������ĸ��ֽ�Ϊ���id
				BYTE *pBuyerID = (BYTE*)m_RecvPacket.GetData() + m_RecvPacket.GetSize() - sizeof(uint32);
                nBuyerID = *((uint32*)pBuyerID);
                {
                    CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"check success��master:%u, cs:%u, buyer:%u",
                              pP2PMsgSend->Sender,  pP2PMsgSend->Recver, nBuyerID);

                    // ����ת����Ϣ
                    // ת�����ͷ�
                    SendDataStruct sendData;
                    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

                    CAppManager::GetInstance().InitHead(
                        lpSendHead,
                        CMD_SERVERMSG_IND,
                        sizeof(XT_SERVER_TRANSMIT) + sizeof(uint32) + sizeof(uint8) + m_RecvPacket.GetSize() - sizeof(uint32),
                        pP2PMsgSend->Recver);

                    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
                    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
                    lpSrvTransmit->msgtype              = CMD_MONITOR_INSERT_CHAT_IND;

                    BYTE *pData = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
                    *((uint32*)pData) = pP2PMsgSend->Sender;
                    pData  = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT)) + sizeof(uint32);
                    *((uint8*)pData) = 1;
                    pData = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT)) + sizeof(uint32) + sizeof(uint8);
                    memcpy(pData,m_RecvPacket.GetData(),m_RecvPacket.GetSize()-sizeof(uint32));
                    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint32) + sizeof(uint8) + m_RecvPacket.GetSize()-sizeof(uint32);

                    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                        sendData.data,sendData.data_len,SEND_RETRY_TIME);

                    CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"send to cs��");


                    // ת�������
                    // ��ҵ�¼��ʽ
                    //uint8 buyer_login_flag = 0;
                    // ���ز������
                    CUserManager *pUserManager = m_pRouter->GetUserManager();
                    shared_ptr<CUser> pUser = pUserManager->Find(nBuyerID);
                    if(pUser == NULL)
                    {	
                    	CLog::Log("CBusinessSendMessageInsert", CLog::TYPE_DEBUG, "buyer offline");
                        return;
                    }

                    SendDataStruct sendData2buyer;

                    lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData2buyer.data + sizeof(XT_HEAD));
                    lpSrvTransmit->origin_id = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
                    lpSrvTransmit->msgtype = CMD_P2PMSG_SEND;

                    pData = (BYTE*)(sendData2buyer.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
                    memcpy(pData, m_RecvPacket.GetData(),m_RecvPacket.GetSize()-sizeof(uint32));
                    // �޸���Ϣ����Ϊ0
                    pP2PMsgSend  = (XT_SERVER_P2PMSG_SEND*)pData;
                    pP2PMsgSend->type = XT_MSG::IM_MSG;
                    pP2PMsgSend->Recver = nBuyerID;

                    lpSendHead = (LPXT_HEAD)sendData2buyer.data;
                    CAppManager::GetInstance().InitHead(lpSendHead,
                                                        CMD_SERVERMSG_IND,
                                                        (uint16)(sizeof(XT_SERVER_TRANSMIT) +  m_RecvPacket.GetSize() - sizeof(uint32)),
                                                        nBuyerID,
                                                        XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACTION);

                    sendData2buyer.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize() - sizeof(uint32);
                    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData2buyer.data,sendData2buyer.data_len,SEND_RETRY_TIME);

                    CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"send to buyer��");

                    return;
                }
            }
        }

        CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"check failed��master:%u, cs:%u", pP2PMsgSend->Sender,  pP2PMsgSend->Recver);
        return;
    }

    //�ж������ҷ�����ҵ���Ϣ��������Ҵ���δ�Ӵ�״̬�������ҵ���Ϣ��λ
#ifndef IM_USE_IMAPI
    if(pP2PMsgSend->Sender > cs_Boundary && m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),pP2PMsgSend->Recver) == TYPE_WEB && pP2PMsgSend->type == 0)//���жϽ��շ�����Ҷ����ǿͷ�
    {
        //uint32 team_id;
        ClientService * clientService = NULL;
        CSManager & csManager = CAppManager::GetInstance().GetCSManager();
        clientService = csManager.Find(pP2PMsgSend->Sender);
        if(clientService)
        {
            //ver�ֶ���ΪteamID�ֶ�
            clientService->ModifyCustomerState_x(pP2PMsgSend->Recver);
            CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        }
        else
        {
			CsCustOper(m_pRouter,UO_ADD_RECPTION,pP2PMsgSend->Sender,pP2PMsgSend->Recver);			
            CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,"�ÿͷ������ڱ���������:%d�����͵��ͷ����ڷ���������",pP2PMsgSend->Sender);
        }
    }
#endif
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),to_id,XT_HEAD::DEST_TYPE_USER);
    else
        CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),to_id,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACTION);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_P2PMSG_SEND;


    BYTE* pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    memcpy(pVerifyFriendReq,m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    CLog::Log("CBusinessServerMsg",CLog::TYPE_IMPORTANT,"�û� %u  ������Ϣ�� %u ",pP2PMsgSend->Sender,to_id);

    uint32_t merchantID = 0;

    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),to_id) == TYPE_WEB)//��ֵ�̼�ID��Ϣ
    {

        MYSQL_LIB->getMainMerchant(pP2PMsgSend->Sender,merchantID);
        CLog::Log("CBusinessServerMsg",CLog::TYPE_IMPORTANT,"��ȡ���� %u ���̼�ID %u",pP2PMsgSend->Sender,merchantID);
    }

    memcpy(pVerifyFriendReq + m_RecvPacket.GetSize(),&merchantID,sizeof(uint32_t));


    sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize() + sizeof(uint32_t));
#ifdef IM_USE_MULTIDEVICE
	vector<multiDevice_t> deviceinfos;
	if(m_pRouter->GetGlobalBusi()->QueryUserDeviceInfo(to_id,0,deviceinfos) == RESULT_FAILED)
	{		
		CLog::Log("SendMsg3",CLog::TYPE_DEBUG,"QueryUserDeviceInfo: %u failed",to_id);
		return;
	}
	//���շ����������û�������
	for(auto deviceid:deviceinfos)
	{	
		lpSendHead->deviceId = deviceid.deviceId;
		lpSendHead->seq++;
		CLog::Log("SendMsg5",CLog::TYPE_DEBUG,"sendmsg to %u, whose deviceinfos size %d ,user %u send to device:  %u",to_id,deviceinfos.size(),deviceid.userId,deviceid.deviceId);
		CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
	}
#else
    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
#endif
    // �ж�����������Ƿ���ڿͷ����������ڱ���أ�������ڱ���أ�����Ϣת��������
    if(pP2PMsgSend->type == XT_MSG::IM_MSG
       || pP2PMsgSend->type == XT_MSG::XT_SERVICE
       || pP2PMsgSend->type == XT_MSG::AUTO_REPLY
       || pP2PMsgSend->type == XT_MSG::XT_RECOMENT)
    {
        uint32 nFromID = (uint32)(*(uint32*)pVerifyFriendReq);
        bool src_is_cs = CUser::g_cs_exist(nFromID, m_pRouter->GetGlobalBusi());
        bool dest_is_cs = CUser::g_cs_exist(to_id, m_pRouter->GetGlobalBusi());

        if( (src_is_cs && !dest_is_cs) || (!src_is_cs && dest_is_cs ) )
        {
            // ����˫�������̼�
            CLog::Log("CBusinessSendMessageIND",CLog::TYPE_DEBUG,"exist CS");
            uint32 cs_id = src_is_cs ? nFromID : to_id;
            uint32 cust_id = src_is_cs ? to_id : nFromID;

            // �ж��Ƿ����ڱ����
            bool monitored = false;
            uint32 cs_master_id = (uint32)-1;
            bool success = CUser::g_cs_ismonitored(cs_id, monitored, cs_master_id, m_pRouter->GetGlobalBusi());
            if(success && monitored)
            {
                CLog::Log("CBusinessSendMessageIND",CLog::TYPE_DEBUG,"is Monitoring");

                // �鿴�����Ƿ��ڱ�̨�������ϣ����ڣ���Ϣת��������������
                /*tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
                uint8 count = 1;
                m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&cs_master_id,&UserInfo,count);
                char szServerIP[64] = {0};
                CAppManager::GetInstance().GetConfig().GetDeviceIp(szServerIP);
                if(strcmp(szServerIP, ))*/

                //uint32 chatting_buyer_id = (uint32)-1;
                //success = g_cs_get_chatting_buyer(cs_id, chatting_buyer_id, m_pRouter->GetGlobalBusi());
                //if(success && chatting_buyer_id == cust_id)
                {
                    //0.�ͷ�������Ϣ��1.�ͷ��յ���Ϣ��
                    uint8  action = cs_id != nFromID;

                    // ����ת����Ϣ�ṹ��
                    SendDataStruct sendData;
                    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
                    CAppManager::GetInstance().InitHead(
                        lpSendHead,
                        CMD_SERVERMSG_IND,
                        sizeof(XT_SERVER_TRANSMIT) + sizeof(uint8) + sizeof(uint32) + sizeof(uint32) + m_RecvPacket.GetSize(),
                        cs_master_id);

                    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
                    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
                    lpSrvTransmit->msgtype              = CMD_MONITOR_START_IND;

                    BYTE* pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) ) ;
                    * ((uint8*)pVerifyFriendReq ) = action;
                    * ((uint32*)(pVerifyFriendReq + 1)) = cs_id;
                    * ((uint32*)(pVerifyFriendReq + 5)) = cust_id;

                    pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint8) + sizeof(uint32) + sizeof(uint32));
                    memcpy(pVerifyFriendReq,m_RecvPacket.GetData(),m_RecvPacket.GetSize());

                    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint8) + sizeof(uint32) + sizeof(uint32) + m_RecvPacket.GetSize();

                    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                        sendData.data,sendData.data_len,SEND_RETRY_TIME);


                }
            }
        }
    }

    //�������Զ��ظ�
#if PRESS_TEST

    uint32 from_id = (uint32)(*(uint32*)pVerifyFriendReq);

    bool match = false;
    for(auto m_id:m_MerchInfo)
    {
        if(to_id == m_id || from_id == m_id)
        {
            CLog::Log("SendMsg2",CLog::TYPE_DEBUG,"MATCHED TOID %u,fromID:%u,matchid:%u",to_id,from_id,m_id);
            match = true;
        }
    }
    if(match == false)
    {
        //CLog::Log("SendMsg2",CLog::TYPE_DEBUG,"not match");
        return;
    }

    //�������Һ����
    * ((uint32*)pVerifyFriendReq ) = to_id;
    * ((uint32*)(pVerifyFriendReq + 4)) = from_id;

    CLog::Log("SendMsg2",CLog::TYPE_DEBUG,"ServerAutoReply: %u %u",*(uint32*)pVerifyFriendReq, *((uint32*)(pVerifyFriendReq + 4)));
    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),from_id,XT_HEAD::DEST_TYPE_USER);
    else
        CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),from_id,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACTION);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);

#endif

#ifdef IM_USE_MULTIDEVICE
   uint32 from_id = (uint32)(*(uint32*)pVerifyFriendReq);

   //�������Һ����  ���豸�����������Ͷ�

  //* ((uint32*)pVerifyFriendReq ) = to_id;
  //* ((uint32*)(pVerifyFriendReq + 4)) = from_id;
    
   
  * ((uint32*)pVerifyFriendReq ) = from_id;
  * ((uint32*)(pVerifyFriendReq + 4)) = to_id;   
  * ((uint8*)(pVerifyFriendReq + 8)) = (uint8)XT_MSG::DEVICE_REPLY;
 

   if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
	   CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),from_id,XT_HEAD::DEST_TYPE_USER);
   else
	   CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),from_id,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACTION);

	
	deviceinfos.clear();
	if(m_pRouter->GetGlobalBusi()->QueryUserDeviceInfo(from_id,0,deviceinfos) == RESULT_FAILED)
	{		
		CLog::Log("SendMsg2",CLog::TYPE_DEBUG,"QueryUserDeviceInfo: %u failed",from_id);
		return;
	}
	
	for(auto deviceid:deviceinfos)
	{
		if(deviceid.deviceId == deviceId)
		{
			continue;
		}	
		
		CLog::Log("SendMsg4",CLog::TYPE_DEBUG,
			"Server Devices Send Back: fromid %u toid %u device size %d, self device %u dest device %u",
			*(uint32*)pVerifyFriendReq, *((uint32*)(pVerifyFriendReq + 4)),deviceinfos.size(),deviceId,deviceid.deviceId);
		lpSendHead->deviceId = deviceid.deviceId;
		lpSendHead->seq++;
		CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
		
	}

#endif

}
#ifdef PRESS_TEST
void CBusinessSendMessage::LoadMerchantId()
{
    Json::Reader jReader(Json::Features::strictMode());
    Json::Value jValue;
    char *szMerIdConfig = m_ConfigInfo.GetMerchantInfo();
    m_MerchInfo.clear();
    if(szMerIdConfig == NULL)
    {
        CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"GetMerchantInfo fail\r\n");
        return;
    }

    if(szMerIdConfig[0] == '{')
    {
        if(jReader.parse(szMerIdConfig, jValue))
        {
            int total = jValue["total"].asInt();
            for(int count = 1; count <= total ; count++)
            {
                char var1[32] = {0};
                sprintf(var1,"mid%d",count);
                uint32 Merchant = jValue[var1].asInt();
                //if(Merchant == 0)
                //  break;
                m_MerchInfo.push_back(Merchant);
                CLog::Log("LogConfig",CLog::TYPE_DEBUG,"Merchantid config: mid%d --- %u",count,Merchant);
            }
        }
    }

}
#endif



//---------------------------------------------------------------------


//---------------------------------------------------------------------

CBusinessGetLinkStatus::CBusinessGetLinkStatus(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_STATUS_GET_REQ,CMD_STATUS_GET_ACK)
{

}


int CBusinessGetLinkStatus::DoWork()
{
    XT_STATUS_GET_REQ* pStatusGetReq = (XT_STATUS_GET_REQ*)m_RecvPacket.GetData();
    XT_STATUS_GET_ACK* pStatusGetAck = (XT_STATUS_GET_ACK*)m_SendPacket.GetData();

    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(pStatusGetReq->fid);
    if( pUser != NULL )
    {

        pStatusGetAck->ret  = RESULT_SUCCESS;
        pStatusGetAck->id   = pUser->m_nID;
        pStatusGetAck->ip   = pUser->m_nIP;
        pStatusGetAck->port = pUser->m_nPort;
        pStatusGetAck->lanip= pUser->m_nLanIP;
        pStatusGetAck->lanport=pUser->m_nLanPort;
        pStatusGetAck->status=pUser->m_nStatus;
        pStatusGetAck->ver  = pUser->m_ClientVersion;
        memcpy(pStatusGetAck->p2pkey,pUser->m_P2PSessionKey,MAX_SESSIONKEY_LEN);

		if(m_pRecvHead->sid >= cs_Boundary && m_pRecvHead->sid < work_Boundary
		&& pStatusGetReq->fid >= Id_Boundary && pStatusGetReq->fid < cs_Boundary)
		{	// ��Ӫ�ͷ��������״̬
			CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_DEBUG,"��Ӫ�ͷ��������״̬ userid:%u friend:%u",m_pRecvHead->sid,pStatusGetAck->id);
			vector<uint32> vecCustID;
			if(m_pRouter->GetGlobalBusi()->GetCusterList(m_pRecvHead->sid,vecCustID))
			{	// ��ȡ��ѯ�б�ɹ�
				CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_IMPORTANT,"��ȡ��ѯ�б�ɹ� userid:%u",m_pRecvHead->sid);
				auto it = std::find(vecCustID.begin(), vecCustID.end(), pStatusGetAck->id);
				if(it == vecCustID.end())
				{	// δ����ѯ�б��У���״̬��Ϊ����
					CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_IMPORTANT,"���δ����ѯ�б��� userid:%u friend:%u",m_pRecvHead->sid, pStatusGetAck->id);
					pStatusGetAck->status = XTREAM_OFFLINE;
				}
				else
				{	// ����ѯ�б���
					CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_IMPORTANT,"�������ѯ�б��� userid:%u friend:%u",m_pRecvHead->sid, pStatusGetAck->id);
					pStatusGetAck->status = XTREAM_ONLINE;
				}
			}
			else
			{
				CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_ERROR,"��ȡ�Ŷ��б�ʧ�� userid:%u",m_pRecvHead->sid);
			}
		}
        
        CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_DEBUG,"userid:%u friend:%u status:%d",m_pRecvHead->sid, pStatusGetAck->id,pStatusGetAck->status);
    }
    else    //�ú��Ѳ�����
    {

        pStatusGetAck->ret  = RESULT_SUCCESS;
        pStatusGetAck->id   = pStatusGetReq->fid;
        pStatusGetAck->ip   = 0;        //��0��ʾ������
        pStatusGetAck->port = 0;
        pStatusGetAck->lanip= 0;
        pStatusGetAck->lanport=0;
        pStatusGetAck->status = XTREAM_OFFLINE;
        CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_DEBUG,"not found friend: %d status: %d",pStatusGetAck->id,pStatusGetAck->status);
    }



    m_SendPacket.SetCursor(sizeof(XT_STATUS_GET_ACK));

    return RESULT_SUCCESS;
}

//-----------------------------------------------------------------

CBusinessModFriendComment::CBusinessModFriendComment(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_MOD_FCNAME_REQ,CMD_MOD_FCNAME_ACK)
{

}


int CBusinessModFriendComment::DoWork()
{
    XT_MOD_FRIEND_COMMNAME_REQ FriendCommReq;

    m_RecvPacket >> FriendCommReq.id
                 >> FriendCommReq.fid
                 >> FriendCommReq.commName;

    CLog::Log("CBusinessModFriendComment::DoWork",CLog::TYPE_DEBUG,"id:%d, fid:%d, commName:%s, recieve",
              FriendCommReq.id, FriendCommReq.fid, FriendCommReq.commName);

    int nResult = m_pRouter->GetDataBaseWeb()->Modify_Friend_Comment(FriendCommReq.id,FriendCommReq.fid,FriendCommReq.commName);

    if( nResult != DB_SUCCESS )
    {
        char szError[MAX_ERROR_BUF];
        CError::DB_Error(nResult, szError, true);

        m_SendPacket << (uint8) RESULT_FAILED
                     << szError;
    }
    else
    {
        m_SendPacket << (uint8)RESULT_SUCCESS
                     << FriendCommReq.fid
                     << FriendCommReq.commName ;
        CLog::Log("CBusinessModFriendComment::DoWork",CLog::TYPE_DEBUG,"id:%d, fid:%d, commName:%s, send",
                  FriendCommReq.id, FriendCommReq.fid, FriendCommReq.commName);
    }


    return RESULT_SUCCESS;
}

//-----------------------------------------------------------------


//-----------------------------------------------------------------
//�û���Ϊͳ�Ʒ���
CBusinessUserActionAudit::CBusinessUserActionAudit(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_USER_ACTION_AUDIT_REQ,CMD_USER_ACTION_AUDIT_ACK)
{

}


int CBusinessUserActionAudit::DoWork()
{
    XT_USER_ACTION_AUDIT_REQ*   pReq = (XT_USER_ACTION_AUDIT_REQ*)m_RecvPacket.GetData();
    XT_USER_ACTION_AUDIT_ACK*   pAck = (XT_USER_ACTION_AUDIT_ACK*)m_SendPacket.GetData();

    pAck->result = RESULT_SUCCESS;

    if (pReq->count * sizeof(XT_USER_ACTION_AUDIT_REQ::Action) > (m_RecvPacket.GetSize() - sizeof(pReq->count)))
    {
        pAck->result = RESULT_FAILED;
    }
    else
    {
        m_pRouter->GetDataBaseMini()->Save_User_Action(m_pRecvHead->sid,pReq->action,pReq->count);
    }


    m_SendPacket.SetCursor(sizeof(XT_USER_ACTION_AUDIT_ACK));

    return RESULT_SUCCESS;
}

//�޸ĸ���ǩ��
CBusinessModifyUnderWrite::CBusinessModifyUnderWrite(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_MODIFYUNDERWRITE_REQ,CMD_MODIFYUNDERWRITE_ACK)
{

}


int CBusinessModifyUnderWrite::DoWork()
{
    char szUnderWrite[MAX_USERSIGN_LEN + 1];
    memset(szUnderWrite, 0, sizeof(szUnderWrite));
    m_RecvPacket>>szUnderWrite;

    CUserManager *pUserManager = m_pRouter->GetUserManager();

    shared_ptr<CUser> pUser = pUserManager->Find(m_pRecvHead->sid);
    if (pUser != NULL)
    {
        strncpy(pUser->m_szUserSign, szUnderWrite,MAX_USERSIGN_LEN);
        m_pRouter->GetGlobalBusi()->ModiUserSign(m_pRecvHead->sid,szUnderWrite);
    }

    m_pRouter->GetDataBaseWeb()->Modify_User_UnderWrite(m_pRecvHead->sid, szUnderWrite);

    m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

CBusinessModifyFriendFlag::CBusinessModifyFriendFlag(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_MODIFYFRIENDFLAG_REQ,CMD_MODIFYFRIENDFLAG_ACK)
{

}


int CBusinessModifyFriendFlag::DoWork()
{
    LPXT_MODIFY_FRIENDFLAG_REQ lpReq = (XT_MODIFY_FRIENDFLAG_REQ*)m_RecvPacket.GetData();

    int dbret = m_pRouter->GetDataBaseWeb()->Modify_Friend_Flag(m_pRecvHead->sid, lpReq->friendid, lpReq->flag);
    if (dbret != DB_SUCCESS)
    {
        char szError[MAX_ERROR_BUF];
        CError::DB_Error(dbret, szError, true);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<m_pRecvHead->sid
                    <<lpReq->friendid
                    <<lpReq->flag
                    <<szError;
    }
    else
        m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

CBusinessFriendInvite::CBusinessFriendInvite(CRouterBusiness *pRouter):
    CBusinessExIMSrv(pRouter,CMD_FRIENDINVITE_REQ,CMD_FRIENDINVITE_ACK)
{
    m_Req =  new XT_FRIENDINVITE_REQ;
}

CBusinessFriendInvite::~CBusinessFriendInvite()
{
    SAFE_DELETE(m_Req);
}
int CBusinessFriendInvite::DoWork()
{
    m_RecvPacket >> m_Req->emailAddress
                 >> m_Req->emailTitle
                 >> m_Req->emailContent;

    m_pRouter->GetDataBaseWeb()->SendFriendInvite(m_pRecvHead->sid, m_Req->emailAddress,m_Req->emailTitle,m_Req->emailContent);

    m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

CBusinessPayMoney::CBusinessPayMoney(CRouterBusiness *pRouter):
    CBusinessExIMSrv(pRouter,CMD_PAYMONEY_REQ,CMD_PAYMONEY_ACK)
{
}

CBusinessPayMoney::~CBusinessPayMoney()
{
}
int CBusinessPayMoney::DoWork()
{
    uint32 money = 0,consumetype = 0, logid = 0;
    m_RecvPacket >> consumetype>>logid>>money;
    if(money <= 0 )
        return RESULT_FAILED;

    float fMoney = float(money / 100.0);
    int ret = m_pRouter->GetDataBaseWeb()->PayMoney(m_pRecvHead->sid,consumetype,logid,fMoney);
    if (ret == 0)
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS;
    }
    else if(ret == 2)
    {
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<"�����˻����㣡";

    }
    else
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<"����ʧ�ܣ����Ժ����ԣ�";


    return RESULT_SUCCESS;
}
CBusinessSaveHeadFile::CBusinessSaveHeadFile(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter, CDM_HEAD_FILE_UPLOAD_REQ, CDM_HEAD_FILE_UPLOAD_ACK)
{

}
int CBusinessSaveHeadFile::DoWork()
{
	return RESULT_SUCCESS;
}


CBusinessGetOtherUser::CBusinessGetOtherUser(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_GET_USER_OTHER_QUERY,CMDGSRV_GET_USER_OTHER_ACK)
{


}

int CBusinessGetOtherUser::DoWork()
{


    GSRV_GET_ONLINE_FULLINFO_QUERY* pQueryHead = (GSRV_GET_ONLINE_FULLINFO_QUERY*)m_RecvPacket.GetData();
    GSRV_GET_ONLINE_FULLINFO_ACK*    pAckHead   = (GSRV_GET_ONLINE_FULLINFO_ACK*)m_SendPacket.GetData();


    m_SendPacket << (uint8)0;       //�ɹ���־
    m_SendPacket << (uint8)0;       //���ظ���,�������ʵ��ֵ�������
    CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"CMDGSRV_GET_USER_OTHER_QUERY count %d",pQueryHead->count);


    shared_ptr<CUser> pUser = NULL;
    uint32 nClientID;
    int i;
    for(i = 0 ; i < pQueryHead->count; ++i)
    {
        nClientID = pQueryHead->userid[i];
        if( 0 > nClientID ||
            nClientID >= MAX_139_ACCOUNT_NUM)
        {
            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"id_over:%u", nClientID);
            CancelReply();
            return false;
        }
        pUser = m_pRouter->GetUserManager()->Find(nClientID);
        if( NULL == pUser)
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"dont have INFO:%u", nClientID);
            //˵�����û�������
            m_SendPacket << nClientID
                         << (uint8)XTREAM_OFFLINE    //����
                         //�������������ֵ,���������
                         << (uint16)0                //version
                         << "username"
                         << "nickname"
                         << (uint32)0                //ip
                         << (uint16)0                //port
                         << (uint8)0                 //sexy
                         << (uint32)0;               //serverip

            m_SendPacket.WritePacket((const char*)pAckHead->user[0].sessionkey,sizeof(pAckHead->user[0].sessionkey));
            m_SendPacket.WritePacket((const char*)pAckHead->user[0].p2psessionkey,sizeof(pAckHead->user[0].p2psessionkey));

        }
        else
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"HAVE USER INFO:%u", nClientID);
            m_SendPacket << nClientID
                         << pUser->m_nStatus
                         << (uint16)0    //pUser->m_version
                         << pUser->m_szUserName
                         << pUser->m_szNickName
                         << pUser->m_nIP
                         << pUser->m_nPort
                         << pUser->m_nSexy
                         << pUser->m_nLanIP;

            m_SendPacket.WritePacket((const char*)pUser->m_SessionKey,sizeof(pAckHead->user[0].sessionkey));
            m_SendPacket.WritePacket((const char*)pUser->m_P2PSessionKey,sizeof(pAckHead->user[0].p2psessionkey));
        }

        //�����һ���û�����Ϣ�Ƿ���װ�ص���
        if( !m_SendPacket.HasData(sizeof(pAckHead->user[0])) )
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"�����ߴ���,�˳�");
            //�����ߴ���,�˳�
            break;
        }

    }

    pAckHead->count = i ;

    CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"CMDGSRV_GET_USER_OTHER_QUERY return count is %d",i);
    return true ;
}

int CBusinessGetOtherUser::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //Ӧ��
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherUser:SendReply",CLog::TYPE_IMPORTANT,"��������:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
                              pSendData->data_len,
                              pSendData->retry_times,
                              pSendData->time_interval,
                              pRecvData->src_ip,
                              PORT_MEMCACHED_ONLINE,
                              m_pSendData->vlink
                             );
    return RESULT_SUCCESS;
}


CBusinessGetOtherUserStatus::CBusinessGetOtherUserStatus(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_USERSTATUS_OTHER_QUERY,CMDGSRV_USERSTATUS_OTHER_ACK)
{


}


int CBusinessGetOtherUserStatus::DoWork()
{


    GSRV_GET_ONLINE_FULLINFO_QUERY* pQueryHead = (GSRV_GET_ONLINE_FULLINFO_QUERY*)m_RecvPacket.GetData();
    GSRV_GET_ONLINE_FULLINFO_ACK*    pAckHead   = (GSRV_GET_ONLINE_FULLINFO_ACK*)m_SendPacket.GetData();


    m_SendPacket << (uint8)0;       //�ɹ���־
    m_SendPacket << (uint8)0;       //���ظ���,�������ʵ��ֵ�������
    CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"CMDGSRV_GET_USER_OTHER_QUERY count %d",pQueryHead->count);


    shared_ptr<CUser> pUser = NULL;
    uint32 nClientID;
    int i;
    for(i = 0 ; i < pQueryHead->count; ++i)
    {
        nClientID = pQueryHead->userid[i];
        if( 0 > nClientID ||
            nClientID >= MAX_139_ACCOUNT_NUM)
        {
            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"id_over:%u", nClientID);
            CancelReply();
            return false;
        }
        pUser = m_pRouter->GetUserManager()->Find(nClientID);
        if( NULL == pUser)
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"dont have INFO:%u", nClientID);
            //˵�����û�������
            m_SendPacket << nClientID
                         << (uint8)XTREAM_OFFLINE;//����
        }
        else
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"HAVE USER INFO:%u", nClientID);
            m_SendPacket << nClientID
                         << pUser->m_nStatus ;
        }

        //�����һ���û�����Ϣ�Ƿ���װ�ص���
        if( !m_SendPacket.HasData(sizeof(pAckHead->user[0])) )
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"�����ߴ���,�˳�");
            //�����ߴ���,�˳�
            break;
        }
    }

    pAckHead->count = i ;

    CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"CMDGSRV_GET_USER_OTHER_QUERY return count is %d",i);
    return true ;
}

int CBusinessGetOtherUserStatus::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //Ӧ��
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherUserStatus:SendReply",CLog::TYPE_IMPORTANT,"��������:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
                              pSendData->data_len,
                              pSendData->retry_times,
                              pSendData->time_interval,
                              pRecvData->src_ip,
                              PORT_MEMCACHED_ONLINE,
                              m_pSendData->vlink
                             );
    return RESULT_SUCCESS;
}




CBusinessGetOtherTUser::CBusinessGetOtherTUser(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_TUSERINFO_OTHER_QUERY,CMDGSRV_TUSERINFO_OTHER_ACK)
{


}

int CBusinessGetOtherTUser::DoWork()
{
    CLog::Log("CBusinessGetOtherTUser",CLog::TYPE_IMPORTANT,"CMDGSRV_TUSERINFO_OTHER_QUERY");
    uint32 id;
    shared_ptr<CUser> pUser = NULL;

    m_RecvPacket>>id;

    if(id < 0 || id >= MAX_139_ACCOUNT_NUM)
    {
        CLog::Log("COnlineManager:GetTUserInfo",CLog::TYPE_ERROR,"id_over:%u", id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<id;
        return false;
    }

    pUser = m_pRouter->GetUserManager()->Find(id);
    if(pUser == NULL)
    {
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<id;
        CancelReply();
    }
    else
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<id
                    <<pUser->m_nIP
                    <<pUser->m_nPort;

        m_SendPacket.WritePacket((const char*)pUser->m_SessionKey,sizeof(pUser->m_SessionKey));
    }
    return true;

}



int CBusinessGetOtherTUser::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //Ӧ��
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherTUser:SendReply",CLog::TYPE_IMPORTANT,"��������:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
                              pSendData->data_len,
                              pSendData->retry_times,
                              pSendData->time_interval,
                              pRecvData->src_ip,
                              pRecvData->port,
                              m_pSendData->vlink
                             );
    return RESULT_SUCCESS;
}


CBusinessGetOtherCtrlUser::CBusinessGetOtherCtrlUser(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_USERCONTROLINFO_OTHER_QUERY,CMDGSRV_USERCONTROLINFO_OTHER_ACK)
{


}

int CBusinessGetOtherCtrlUser::DoWork()
{
    CLog::Log("CBusinessGetOtherCtrlUser",CLog::TYPE_IMPORTANT,"CMDGSRV_USERCONTROLINFO_OTHER_QUERY ");
    uint8 nReturnUserNum;
    shared_ptr<CUser> pUser = NULL;
    m_RecvPacket>>nReturnUserNum;
    GSRV_USERCONTROLINFO_QUERY_ACK*  pAckHead   = (GSRV_USERCONTROLINFO_QUERY_ACK*)m_SendPacket.GetData();

    uint32  nClientID;

    m_SendPacket << (uint8)0;           //�ɹ���־
    m_SendPacket << (uint8)0;           //���ظ���,�������ʵ��ֵ�������

    int nOneUserInfoSize = sizeof(GSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo);

    int i;
    for( i = 0 ; i < nReturnUserNum; ++i )
    {
        m_RecvPacket>>nClientID;

        if( 0 > nClientID ||
            nClientID >= MAX_139_ACCOUNT_NUM)
        {
            CLog::Log("COnlineManager:GetUserLinkInfo",CLog::TYPE_ERROR,"id_over:%u", nClientID);
            return false;
        }
        pUser = m_pRouter->GetUserManager()->Find(nClientID);
        if( NULL == pUser)
        {
            //˵�����û�������
            m_SendPacket << nClientID
                         << (uint32)0;   //����

            //���漸��ֵ����û�����,�����������
            m_SendPacket.WritePacket((const char*)"7788414",sizeof(pUser->m_SessionKey));
            m_SendPacket.WritePacket((const char*)"7788414",sizeof(pUser->m_SessionKey));
            //�յ��㲥������û��������ݣ�ֱ��ȡ������
            CancelReply();
        }
        else
        {
            m_SendPacket << nClientID
                         << pUser->m_nServerIp;

            m_SendPacket.WritePacket((const char*)pUser->m_SessionKey,sizeof(pUser->m_SessionKey));
            m_SendPacket.WritePacket((const char*)pUser->m_P2PSessionKey,sizeof(pUser->m_P2PSessionKey));
            m_SendPacket << pUser->flag;

        }

        //�����һ���û�����Ϣ�Ƿ���װ�ص���
        if( !m_SendPacket.HasData(nOneUserInfoSize) )
        {
            //�����ߴ���,�˳�
            break;
        }

    }

    pAckHead->count = i;

    return true;
}
//10002�������Դ�˿ڼ���
int CBusinessGetOtherCtrlUser::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //Ӧ��
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherCtrlUser:SendReply",CLog::TYPE_IMPORTANT,"��������:ip %u port %d vlink %d",
              pRecvData->src_ip,pRecvData->port, pRecvData->vlink);

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
                              pSendData->data_len,
                              pSendData->retry_times,
                              pSendData->time_interval,
                              pRecvData->src_ip,
                              pRecvData->port,
                              m_pSendData->vlink
                             );
    return RESULT_SUCCESS;
}








CBusinessModifyCSPWD::CBusinessModifyCSPWD(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_MOD_PSW_REQ,CMD_MOD_PSW_ACK)
{
    //�޸��˺ſͷ��˺�����
}
int CBusinessModifyCSPWD::DoWork()
{
    //char username[100];
    //char pwd[100];
    XT_MOD_PSW_REQ modifyPwd;
    //���ܵ���Ҫ�޸ĵ������¼��
    m_RecvPacket>>modifyPwd.uid
                >>modifyPwd.szPsw
                >>modifyPwd.random_key;
    //���޸ĵ��˺��������д��IM_USER��
    if(m_pRouter->GetDataBaseMini()->Modify_CS_Pwd(modifyPwd.uid,modifyPwd.szPsw) == DB_ERROR)
    {
        CLog::Log("ModifyCSReq", CLog::TYPE_ERROR, "Modify CS PWD failed! name:%s ",modifyPwd.uid);
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }


    m_SendPacket<<(uint8)RESULT_SUCCESS
                <<modifyPwd.uid
                <<modifyPwd.szPsw;

    return RESULT_SUCCESS;
}


/*********************************************���ÿͷ��Ӵ������***************************************************/
CCSetChatWithBuyer::CCSetChatWithBuyer(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_MONITOR_SET_BUYER_REQ,CMD_MONITOR_SET_BUYER_ACK)
{

}

int CCSetChatWithBuyer::DoWork()
{
    XT_MONITOR_SET_BUYER_REQ req;
    m_RecvPacket>>req.idSeller;
    m_RecvPacket>>req.idBuyer;
    CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "idSeller:%d  idBuyer:%d",req.idSeller, req.idBuyer);

    // ����id���ҿͷ�
    bool is_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!is_cs)
    {
        // δ���ҵ��ͷ�
        CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // �õ���ǰ������������
    uint32 chatting_buyer_id = (uint32)-1;
    bool success = CUser::g_cs_get_chatting_buyer(req.idSeller,chatting_buyer_id,m_pRouter->GetGlobalBusi());
    if(success && chatting_buyer_id == req.idBuyer)
    {
        CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "�ظ�����");
        m_SendPacket<<(uint8)RESULT_SUCCESS;
        return RESULT_SUCCESS;
    }

    // ���ÿͷ�������������
    CUser::g_cs_set_chatting_buyer(req.idSeller,req.idBuyer,m_pRouter->GetGlobalBusi());

    // �жϿͷ��Ƿ����ڱ����
    CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "check monitor");
    bool monitored = false;
    uint32 cs_master_id = (uint32)-1;
    success = CUser::g_cs_ismonitored(req.idSeller,monitored,cs_master_id,m_pRouter->GetGlobalBusi());
    if(success && monitored)
    {
        // �����,����ϵͳ��ת��Ϣ
        CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "monitoring");
        SendChatBuyerChanged(cs_master_id, req.idSeller, req.idBuyer);
    }

    // ���ͻظ�
    m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

// ϵͳ��ת��Ϣ��֪ͨ��������صĿͷ��ı���������������
void  CCSetChatWithBuyer::SendChatBuyerChanged(uint32 nDirectorID, uint32 nCsId, uint32 nBuyerID)
{
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_MONITOR_SET_BUYER_IND),
        nDirectorID);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_MONITOR_SET_BUYER_IND;

    XT_MONITOR_SET_BUYER_IND* pSetBuyer = (XT_MONITOR_SET_BUYER_IND*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    pSetBuyer->idSeller = nCsId;
    pSetBuyer->idBuyer = nBuyerID;

    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_MONITOR_SET_BUYER_IND);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}


/***************************************************���ܼ�ع���****************************************************/
// ��ʼ���
CDirectorMonitorStart::CDirectorMonitorStart(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_MONITOR_START_REQ,CMD_MONITOR_START_ACK)
{

}

int CDirectorMonitorStart::DoWork()
{
    XT_MONITOR_START_REQ req;
    m_RecvPacket>>req.idMaster;
    m_RecvPacket>>req.idSeller;

    CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "idMaster:%d  idSeller:%d",req.idMaster, req.idSeller);

    // ����id���ҿͷ�
    bool exist_master = CUser::g_cs_exist(req.idMaster,m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller,m_pRouter->GetGlobalBusi());
    if(!exist_master || !exist_cs)
    {
        // δ���ҵ��ͷ�
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // �ж�Ȩ���Ƿ�����
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUserMaster = pUserManager->Find(req.idMaster);
	if(pUserMaster == NULL)
	{	// redis�л�ȡ����ȫ����Ϣʧ��
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "not in the server %d",req.idMaster);
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
	}
    if(pUserMaster->m_nIntegral == 0)
    {
        // ��Ϊ����
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "not master");
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // ��ȡ�ͷ����ڽӴ������
    uint32 nChattingBuyerID = (uint32)-1;
    CUser::g_cs_get_chatting_buyer(req.idSeller,nChattingBuyerID,m_pRouter->GetGlobalBusi());
    if(nChattingBuyerID == (uint32)-1 || nChattingBuyerID == 0)
    {
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "no chatting buyer");
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // ��ȡ�ͷ����ڱ�˭���
    bool monitored = false;
    uint32 cs_master_id;
    CUser::g_cs_ismonitored(req.idSeller, monitored, cs_master_id, m_pRouter->GetGlobalBusi());
    if(monitored && cs_master_id != req.idMaster)
    {
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "�ͷ��Ѿ����������ܼ��");
        m_SendPacket<<(uint8)2;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // ��ʼ���
    CUser::g_cs_set_director(req.idSeller,req.idMaster,m_pRouter->GetGlobalBusi());

    // ����ظ��ṹ��
    m_SendPacket<<(uint8)RESULT_SUCCESS;
    m_SendPacket<<req.idSeller;
    m_SendPacket<<nChattingBuyerID;

    return RESULT_SUCCESS;
}


/***************************************************������ع���****************************************************/
// �������
CDirectorMonitorStop::CDirectorMonitorStop(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_MONITOR_END_REQ,CMD_MONITOR_END_ACK)
{

}

int CDirectorMonitorStop::DoWork()
{
    XT_MONITOR_END_REQ req;
    m_RecvPacket>>req.idMaster;
    m_RecvPacket>>req.idSeller;

    CLog::Log("CDirectorMonitorStop", CLog::TYPE_DEBUG, "idMaster:%d  idSeller:%d",req.idMaster, req.idSeller);

    // ����id���ҿͷ�
    bool exist_master = CUser::g_cs_exist(req.idMaster, m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!exist_master)
    {
        // δ���ҵ��ͷ�
        CLog::Log("CDirectorMonitorStop", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // ȡ�����
    CLog::Log("CDirectorMonitorStop", CLog::TYPE_DEBUG, "stop success");
    if(exist_cs)
    {
        CUser::g_cs_set_director(req.idSeller,(uint32)-1,m_pRouter->GetGlobalBusi());
    }

    // ����ظ��ṹ��
    m_SendPacket<<(uint8)RESULT_SUCCESS;
    return RESULT_SUCCESS;
}


/***************************************************���ܲ���Ự****************************************************/
CMonitorInsertChat::CMonitorInsertChat(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_MONITOR_INSERT_CHAT_REQ,CMD_MONITOR_INSERT_CHAT_ACK)
{

}

int CMonitorInsertChat::DoWork()
{
    XT_MONITOR_INSERT_CHAT_REQ req;
    m_RecvPacket>>req.idMaster;
    m_RecvPacket>>req.idSeller;
    m_RecvPacket>>req.idBuyer;
    CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "idMaster:%u  idSeller:%u  idBuyer:%u",req.idMaster, req.idSeller, req.idBuyer);

    // ����id���ҿͷ�
    bool exist_master = CUser::g_cs_exist(req.idMaster, m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!exist_master || !exist_cs)
    {
        CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // �жϿͷ����ڱ����
    bool monitored = false;
    uint32 cs_master_id = (uint32)-1;
    bool success = CUser::g_cs_ismonitored(req.idSeller,monitored,cs_master_id,m_pRouter->GetGlobalBusi());
    if(!success || !monitored)
    {
        CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "not be Monitored");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // �жϼ�ؿͷ��������Ƿ���ȷ
    if(cs_master_id != req.idMaster)
    {
        CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "not same monitor");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    m_SendPacket<<(uint8)RESULT_SUCCESS;

    CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "Length : %d", sizeof(XT_HEAD)  + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_MONITOR_INSERT_CHAT_IND));

    // ��Ϣ��ת����֮�ͷ�����Ҳ���Ự��ʼ
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + sizeof(uint32) + sizeof(uint8),
        req.idSeller);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype              = CMD_MONITOR_INSERT_CHAT_IND;

    BYTE *pData = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
    *((uint32*)pData) = req.idMaster;
    pData = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT)) + sizeof(uint32);
    *((uint8*)pData) = 0;
    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint32) + sizeof(uint8);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);

    // ��֮���
    CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "bgn insert chat to buyer");
    SendDataStruct sendData2buyer;

    lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData2buyer.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype = CMD_P2PMSG_SEND;

    CSendPacket sendPacket(sendData2buyer.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));

    XT_MSG msg;
    msg.data_type = XT_MSG::IM_MSG_INSERT;

    sendPacket<<req.idSeller
              <<req.idBuyer
              <<msg.data_type
              <<msg.ver
              <<msg.send_time
              <<msg.recv_flag
              <<msg.fontSize
              <<msg.fontColor
              <<msg.fontStyle
              <<msg.fontName;

    Json::FastWriter jWriter;
    Json::Value jvSellerInfo;
    jvSellerInfo["switchStatus"] = 2;
    jvSellerInfo["toSubMerchantID"] = req.idMaster;
    jvSellerInfo["fromSubMerchantID"] = req.idSeller;
    char csname[256] = {0};
    CUser::g_cs_get_name(cs_master_id,csname,m_pRouter->GetGlobalBusi());
    jvSellerInfo["toSubMerchantName"] = csname;
    string strMsg = jWriter.write(jvSellerInfo);
    strcpy(msg.data, strMsg.c_str());
    msg.data_len = strMsg.length() + 1;
    CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "###Json String:%s", msg.data);

    sendPacket.WriteData(msg.data,sizeof(msg.data));
    sendPacket<<msg.from_nickname;

    sendData2buyer.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_SERVER_P2PMSG_SEND) + sendPacket.GetSize());

    LPXT_HEAD lpSendHead2buyer = (LPXT_HEAD)sendData2buyer.data;

    CAppManager::GetInstance().InitHead(lpSendHead2buyer,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) +  sendPacket.GetSize()),req.idBuyer,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_SRVACTION);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData2buyer.data,sendData2buyer.data_len,SEND_RETRY_TIME);

    return RESULT_SUCCESS;
}


/***************************************************���ܽ��ֻỰ****************************************************/
CMonitorTranChat::CMonitorTranChat(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_MONITOR_TRAN_CHAT_REQ,CMD_MONITOR_TRAN_CHAT_ACK)
{

}

int CMonitorTranChat::DoWork()
{
    XT_MONITOR_TRAN_CHAT_REQ req;
    m_RecvPacket>>req.idMaster;
    m_RecvPacket>>req.idSeller;
    m_RecvPacket>>req.idBuyer;
    CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "idMaster:%u  idSeller:%u  idBuyer:%u",req.idMaster, req.idSeller, req.idBuyer);

    // ����id���ҿͷ�
    bool exist_master = CUser::g_cs_exist(req.idMaster, m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!exist_master || !exist_cs)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // �жϿͷ����ڱ����
    bool monitored = false;
    uint32 cs_master_id = (uint32)-1;
    bool success = CUser::g_cs_ismonitored(req.idSeller,monitored,cs_master_id,m_pRouter->GetGlobalBusi());
    if(!success || !monitored)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "not be Monitored");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // �жϼ�ؿͷ��������Ƿ���ȷ
    if(cs_master_id != req.idMaster)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "not same monitor");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }


    // ���id
    uint32 buyerID =  (uint32)-1;
    CUser::g_cs_get_chatting_buyer(req.idSeller,buyerID,m_pRouter->GetGlobalBusi());
    // ���ز������
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    int userteamid = -1;
    shared_ptr<CUser> pUser = pUserManager->Find(buyerID);
    if(pUser == NULL)
    {
        // �����в����ڣ���ȫ���в���
        uint32 lsid[1] = {buyerID};
        tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userinfo[1];
        uint8 count = 1;

        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserInfo(lsid,count,userinfo))
        {
            // ȫ���в������ʧ�ܣ���ʾ���δ��½
            CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "buyer offline");
            m_SendPacket<<(uint8)RESULT_FAILED;
            return RESULT_FAILED;
        }
        else
        {
            userteamid = userinfo[0].teamID;
        }
    }
    else
    {
        userteamid = pUser->getCsTeamId();
    }

    m_SendPacket<<(uint8)RESULT_SUCCESS;
#ifndef IM_USE_IMAPI
    // �ӿͷ���ɾ�����
    ClientService * clientService = NULL;
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    clientService = csManager.Find(req.idSeller);
    if(clientService != NULL)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "�ͷ�%s������%u", clientService->m_CSname, req.idBuyer);
        clientService->RemoveCustomer_x(req.idBuyer);

        /*�޸��û���Ϣ��memcached��*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
    }
    else
    {
        CsCustOper(m_pRouter, UO_DEL_CUST, req.idSeller, req.idBuyer);
    }
#endif
    // �޸����������
    if(pUser != NULL)
    {
        pUser->m_nCSuserid = req.idMaster;

        CLog::Log("LoginSuccess", CLog::TYPE_IMPORTANT, "��ȫ�ָ���һ��WEB�û���%d ״̬ %d", pUser->m_nID,pUser->m_nStatus);
        m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0);
    }
    else
    {
        // �����Ҳ��ڴ˷�������ͨ���������������޸�
        //��ѯȫ�ַ����������web���ڷ�������Ӵ�������
        tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
        uint8 count = 1;
        uint32 custid = req.idBuyer;
        m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&custid,&UserInfo,count);

        if(UserInfo.uid != custid)
        {
            CLog::Log("CMonitorTranChat",CLog::TYPE_IMPORTANT,
                      "��ѯ���ȫ����Ϣʧ��! %u", custid);
        }
        else
        {
            userteamid = UserInfo.teamID;
            SetWebContext(m_pRouter, custid, req.idMaster, userteamid);
        }
    }
#ifndef IM_USE_IMAPI
    // �޸�����������
    clientService = csManager.Find(req.idMaster);
    if(clientService != NULL)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "�ͷ�%s������%u", clientService->m_CSname, req.idBuyer);
        clientService->AddCustomer_x(userteamid, req.idBuyer);

        /*�޸��û���Ϣ��memcached��*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
    }
    else
    {
        CsCustOper(m_pRouter, UO_ADD_CUST, req.idMaster, req.idBuyer);
    }
#endif
    // ��Ϣ��ת����֮�ͷ���������ܽ����˻Ự
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        sizeof(XT_SERVER_TRANSMIT) + sizeof(uint32),
        req.idSeller);

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype          = CMD_MONITOR_TRAN_CHAT_IND;

    BYTE *pData = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
    *((uint32*)pData) = req.idMaster;
    sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(uint32);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);

    // ��֪���
    CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "bgn insert chat to buyer");
    SendDataStruct sendData2buyer;

    lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData2buyer.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype = CMD_P2PMSG_SEND;

    CSendPacket sendPacket(sendData2buyer.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));

    XT_MSG msg;
    msg.data_type = XT_MSG::CUST_SER_TRAN_REQ;

    sendPacket<<req.idSeller
              <<req.idBuyer
              <<msg.data_type
              <<msg.ver
              <<msg.send_time
              <<msg.recv_flag
              <<msg.fontSize
              <<msg.fontColor
              <<msg.fontStyle
              <<msg.fontName;

    Json::FastWriter jWriter;
    Json::Value jvSellerInfo;
    jvSellerInfo["switchStatus"] = 2;           // 1 ��ʾת�ӿ�ʼ 0 ��ʾת�ӳɹ� 2 ��ʾ���ܽ��ֻỰ
    jvSellerInfo["toSubMerchantID"] = req.idMaster;
    jvSellerInfo["fromSubMerchantID"] = req.idSeller;
    char csname[256] = {0};
    /*
        CGetBackendGroup *pbackendGroup = CAppManager::GetInstance().GetBackendGroupIns();
        auto ifind = std::find_if(pbackendGroup->m_vecCSPlatPerson.begin(), pbackendGroup->m_vecCSPlatPerson.end(),
                        [cs_master_id] (const XT_CS_INFO& a) ->bool
                        {
                            return a.id == cs_master_id;
                        });
        if(ifind != pbackendGroup->m_vecCSPlatPerson.end())
        {
            strcpy(csname, ifind->username);
        }
        else
        {

        }
    */
    CUser::g_cs_get_name(cs_master_id,csname,m_pRouter->GetGlobalBusi());
    jvSellerInfo["toSubMerchantName"] = csname;
    string strMsg = jWriter.write(jvSellerInfo);
    strcpy(msg.data, strMsg.c_str());
    msg.data_len = strMsg.length() + 1;
    CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "###Json String:%s", msg.data);

    sendPacket.WriteData(msg.data,sizeof(msg.data));
    sendPacket<<msg.from_nickname;

    sendData2buyer.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_SERVER_P2PMSG_SEND) + sendPacket.GetSize());

    LPXT_HEAD lpSendHead2buyer = (LPXT_HEAD)sendData2buyer.data;

    CAppManager::GetInstance().InitHead(lpSendHead2buyer,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) +  sendPacket.GetSize()),req.idBuyer,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACTION);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData2buyer.data,sendData2buyer.data_len,SEND_RETRY_TIME);

    //�����û�������������Ŷ��û���Ϣ
    if(userteamid != -1)
    {
	#ifdef IM_USE_IMAPI
		// 9. ���ֻỰ��ServiceId��customerId,group_id���toServiceID��toServiceGroup ��Ҫת�ӵ��Ŀͷ���ID����ID
		imapi csControlReq;
		string info;
		csControlReq.imapiReq(imapi::CSCONTROL,info,req.idSeller,req.idBuyer,userteamid, 0,0,req.idMaster,userteamid);
	#else
		PushQueueSubAccountToKafka(m_pRouter,userteamid);
	#endif
	
    }

    return RESULT_SUCCESS;
}

bool SortMsgMethod(const XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList &x,const XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList &y)
{
    return x.lastMsgTime > y.lastMsgTime;
}

CBusinessWebLastMsg::CBusinessWebLastMsg(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_LAST_MSG_REQ,CMD_GET_LAST_MSG_ACK)
{
    pMsgInfo = new XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList[MAX_MSG_NUM];
    pData = new SendDataStruct;
    if(pMsgInfo == NULL || pData == NULL)
        CLog::Log("CBusinessWebMoreMsg",CLog::TYPE_ERROR,"New failed");
}

CBusinessWebLastMsg::~CBusinessWebLastMsg()
{
    if(pMsgInfo)
    {
        delete[] pMsgInfo;
        pMsgInfo = NULL;
    }
    if(pData)
    {
        delete pData;
        pData = NULL;
    }
}
void CBusinessWebLastMsg::SendWebMoreLastMsgAck(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = CAppManager::GetInstance().GetSendSeq();
    pHead->cmd  = CMD_GET_LAST_MSG_ACK;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),pData->data,pData->data_len,SEND_RETRY_TIME);

}

int CBusinessWebLastMsg::DoWork()
{
    XT_WEB_BUSI_LAST_MSG_REQ WebLastMsg;
    XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList user_msgs[25];
    uint32 nRetCount;


    uint32 len = 0;
    uint32 tmp_len = 0;
    std::vector<uint32_t> ids;

    XT_WEB_BUSI_LAST_MSG_ACK* pWebMsgAck = (XT_WEB_BUSI_LAST_MSG_ACK*)m_SendPacket.GetData();

    CancelReply();

    m_RecvPacket>>WebLastMsg.id
                >>WebLastMsg.msgnum;

    nRetCount = WebLastMsg.msgnum;
    if(nRetCount > 25)
        nRetCount = 25;

    CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "����id %u ������� %d ",WebLastMsg.id,WebLastMsg.msgnum);
    ids.clear();

    //ȡ�����ϵ�˺��Լ���Ϣ
    if(m_pRouter->GetDataBaseMini()->WebLastMsgReq(&WebLastMsg,user_msgs,nRetCount) == DB_ERROR)
    {
        CLog::Log("WebLastMsgReq", CLog::TYPE_ERROR, "WebLastMsgReq Req Failed! sid:%d ",WebLastMsg.id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebLastMsg.id
                    <<WebLastMsg.msgnum;
        SendWebMoreLastMsgAck(m_SendPacket);//��ȡʧ��Ҳ֪ͨ
        return RESULT_SUCCESS;
    }

    vector<uint32_t> userlist;
    vector<uint32_t> subaccount;
    CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "nRetCount %d ",nRetCount);
    if(nRetCount == 0)
    {
        CLog::Log("WebLastMsgReq", CLog::TYPE_ERROR, "WebLastMsgReq RealCount = 0! sid:%d ",WebLastMsg.id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebLastMsg.id
                    <<WebLastMsg.msgnum;

        SendWebMoreLastMsgAck(m_SendPacket);//��ȡʧ��Ҳ֪ͨ
        return RESULT_SUCCESS;
    }

//  vector<MerchantandSubAccount> merchantInfo;

    for(int i = 0 ; i < (int)nRetCount; i++)
    {
        //struct MerchantandSubAccount tmp_account;

        MYSQL_LIB->getMainMerchant((uint32_t)user_msgs[i].subAccountID, (uint32_t &)user_msgs[i].merchantID);

        CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "user_msgs[i].subAccountID %u ���˺� %u  ��Ϣ %s",
                  user_msgs[i].subAccountID,user_msgs[i].merchantID,user_msgs[i].lastMsg);
    }

    //ȡÿ����ϵ�˺ŵ����˺�
#if 0
    vector<MerchantandSubAccount> merchantInfo;
    MYSQL_LIB->getMainMerchants(subaccount, merchantInfo);
#endif
    //merge
    if(nRetCount > 1)
        sort(user_msgs,user_msgs+nRetCount,SortMsgMethod);

    for(int i = 0 ; i < (int)nRetCount; i++)
    {
        for(int j = i+1 ; j < (int)nRetCount; j++)
        {
            if((user_msgs[i].merchantID == user_msgs[j].merchantID )&&
               (user_msgs[j].merchantID != 0xffffffff))
            {
                user_msgs[j].merchantID = 0xffffffff;
            }
            if(user_msgs[j].merchantID == 0)
            {
                user_msgs[j].merchantID = 0xffffffff;
            }
        }
    }

    uint32 correctCount = 0;        //�����û�����
    for(int i = 0 ; i < (int)nRetCount; i++)
    {
        if((user_msgs[i].merchantID != 0xffffffff) && (user_msgs[i].merchantID != 0))
        {

            pMsgInfo[correctCount].merchantID = user_msgs[i].merchantID;
            pMsgInfo[correctCount].subAccountID = user_msgs[i].subAccountID;
            pMsgInfo[correctCount].lastMsgTime = user_msgs[i].lastMsgTime;
            strcpy(pMsgInfo[correctCount].nickname,user_msgs[i].nickname);
            strcpy(pMsgInfo[correctCount].lastMsg,user_msgs[i].lastMsg);

            CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "��ȷ������ %u ����ʱ��%u ���˺� %u ��Ϣ  %s",
                      pMsgInfo[correctCount].merchantID,
                      pMsgInfo[correctCount].lastMsgTime,
                      pMsgInfo[correctCount].subAccountID,
                      pMsgInfo[correctCount].lastMsg);

            correctCount++;
        }
        CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "���е����� %u ����ʱ��%u ���˺� %u ��Ϣ	%s",
                  user_msgs[i].merchantID,
                  user_msgs[i].lastMsgTime,
                  user_msgs[i].subAccountID,
                  user_msgs[i].lastMsg);

    }
    nRetCount = correctCount;

	if(correctCount == 0)
	{
	    CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "WebLastMsgReq Finally return = 0! sid:%d ",WebLastMsg.id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebLastMsg.id
                    <<WebLastMsg.msgnum;
        SendWebMoreLastMsgAck(m_SendPacket);//��ȡʧ��Ҳ֪ͨ
        return RESULT_SUCCESS;
	}
    //get all sub users id
    CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "WebMoreMsg.did:%d, size:%d ",WebLastMsg.id,nRetCount);

    tmp_len = sizeof(uint32)+sizeof(uint32)+sizeof(uint32);
    seq = m_pRecvHead->seq;
    uint32 i = 0;
    while(nRetCount > i)
    {
        pWebMsgAck->actualnum= 0;

        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebLastMsg.id
                    <<WebLastMsg.msgnum
                    <<nRetCount
                    <<(uint8)1
                    <<pWebMsgAck->actualnum;

        for(; i < nRetCount; i ++)
        {
            len = tmp_len + strlen(pMsgInfo[i].lastMsg) + strlen(pMsgInfo[i].nickname) + sizeof(uint16)*2 + 2; 	// д���ַ�����ʱ����д���ַ�������
            if(!m_SendPacket.HasData(len))
            {
                break;
            }
            if(pMsgInfo[i].merchantID == 0xffffffff)
            {
                CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "continue userid:%d, nickname:%s",
                          pMsgInfo[i].merchantID,pMsgInfo[i].nickname);
                continue;
            }
            CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, " index %d userid:%d, nickname:%s",
                      i,pMsgInfo[i].merchantID,pMsgInfo[i].nickname);

            m_SendPacket<<pMsgInfo[i].merchantID
                        <<pMsgInfo[i].subAccountID
                        <<pMsgInfo[i].lastMsgTime
                        <<pMsgInfo[i].nickname
                        <<pMsgInfo[i].lastMsg;

            pWebMsgAck->actualnum ++;
        }
        if(i < nRetCount)
            pWebMsgAck->over = (uint8)0;
        else
            pWebMsgAck->over = (uint8)1;

        SendWebMoreLastMsgAck(m_SendPacket);
        m_SendPacket.Reset();
    }
    if(nRetCount == 0)
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebLastMsg.id
                    <<WebLastMsg.msgnum
                    <<nRetCount
                    <<(uint8)1
                    <<(uint32)0;

        SendWebMoreLastMsgAck(m_SendPacket);
    }
    return RESULT_SUCCESS;
}

CBusinessUseridMapCSid::CBusinessUseridMapCSid(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_USERID_MAP_CSID_REQ,CMD_GET_USERID_MAP_CSID_ACK)
{

}

int CBusinessUseridMapCSid::DoWork()
{
    //XT_UPDATE_NEAR_COMM_CUST_INFO_REQ* pCustInfoMod = (XT_UPDATE_NEAR_COMM_CUST_INFO_REQ*)m_RecvPacket.GetData();
    uint32 userid = 0;
    uint32 csid = 0;
    //CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();


    m_RecvPacket >> userid;
    if(RESULT_FAILED == m_pRouter->GetDataBaseMini()->Get_Userid_Map_CSid(userid,csid))
    {
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<csid;
        return RESULT_FAILED;
    }
    m_SendPacket<<(uint8)RESULT_SUCCESS
                <<csid;
    return RESULT_SUCCESS;

}



CBusinessClearWebClient::CBusinessClearWebClient(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_SET_WEBUSERINFO_REQ,0)
{

}

CBusinessClearWebClient::~CBusinessClearWebClient()
{

}


int CBusinessClearWebClient::DoWork()
{
    XT_SET_WEB_CS_REQ * lpReq = (XT_SET_WEB_CS_REQ *)m_RecvPacket.GetData();

    CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"Id:%d �յ����web�ͷ��û�%u������ auth %x IN",lpReq->id,lpReq->authData);

    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(lpReq->id);
    if(pUser == NULL)
    {
        CLog::Log("CBusinessClearWebClient", CLog::TYPE_IMPORTANT, "��ǰ������û�д��û�%u" ,lpReq->id);
        return 0;
    }
    switch(lpReq->authData)
    {
        case 0xBB:  //���web�Ӵ���Ϣ
            pUser->ClearWebClient();
            break;
        case 0xCC:  //�����û��Ӵ�
            pUser->setCsuserId(lpReq->csId);
            pUser->setCsTeamId(lpReq->teamId);
            break;
        default:
            CLog::Log("CBusinessClearWebClient", CLog::TYPE_ERROR, "auth error %u" ,lpReq->authData);

    }
    CLog::Log("CBusinessClearWebClient", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, authData:%u OUT" , m_pRecvHead->sid, m_pRecvHead->did,m_pRecvHead->flag, lpReq->authData);

	pUserManager->AddUserToRedis(pUser);

    return 0;
}


CBusinessSetCSClient::CBusinessSetCSClient(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_SET_CS_CONSULT_REQ,0)
{

}

CBusinessSetCSClient::~CBusinessSetCSClient()
{

}

/***************************************************��ȡ��Ϣ����****************************************************/
CGetMsgRemind::CGetMsgRemind(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_GETMSGREMIND_REQ,CMD_GETMSGREMIND_ACK)
{

}

int CGetMsgRemind::DoWork()
{
    char szData[1024] = {0};
    int nDataLength = m_RecvPacket.ReadData(szData,1024);
    BaseReq req;
    if(!req.ParseFromArray(szData, nDataLength))
    {
        CLog::Log("CGetMsgRemind", CLog::TYPE_ERROR, "REQ PARSE FAILD");
        return RESULT_FAILED;
    }

    // ȥ���ݿ��в���
    MsgRemindAck ack;
    m_pRouter->GetDataBaseMini()->GetMsgRemind(req.userid(), ack);
    int msg_size = ack.ByteSize();
    char *pData = new char[msg_size];
    ack.SerializeToArray(pData, msg_size);
    CLog::Log("CGetMsgRemind", CLog::TYPE_DEBUG, "BUF LENGTH:%d", msg_size);
    m_SendPacket.WriteData(pData, msg_size);
    delete[] pData;
    pData = NULL;
    return RESULT_SUCCESS;
}


/***************************************************������Ϣ����****************************************************/
CSetMsgRemind::CSetMsgRemind(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_SETMSGREMIND_REQ,CMD_SETMSGREMIND_ACK)
{

}

int CSetMsgRemind::DoWork()
{
    char szData[1024] = {0};
    int nDataLength = m_RecvPacket.ReadData(szData,1024);
    MsgRemindAck req;
    if(!req.ParseFromArray(szData, nDataLength))
    {
        CLog::Log("CSetMsgRemind", CLog::TYPE_ERROR, "REQ PARSE FAILD");
        return RESULT_FAILED;
    }

    // д�����ݿ�
    MsgRemindAck ack;
    m_pRouter->GetDataBaseMini()->SetMsgRemind(m_pRecvHead->sid, req);
    m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}


/***************************************************��ȡ������Ϣ��������****************************************************/
CGetOfflineMsgNotify::CGetOfflineMsgNotify(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_GET_OFFLINE_MSGONTIFY,0)
{

}

int CGetOfflineMsgNotify::DoWork()
{
	char szData[1024] = {0};
    int nDataLength = m_RecvPacket.ReadData(szData,1024);
    BaseReq req;
    if(!req.ParseFromArray(szData, nDataLength))
    {
        CLog::Log("CGetOfflineMsgNotify", CLog::TYPE_ERROR, "REQ PARSE FAILD");
        return RESULT_FAILED;
    }

    // ȥ���ݿ��в���������Ϣ��������
    vector<string> arrUUID;
    m_pRouter->GetDataBaseMini()->GetMsgNotifyDataOnePerson(req.userid(), arrUUID);
    /*for(int i = 0; i < (int)arrUUID.size(); ++i)
    {
    	// ����
    	CLog::Log("CGetOfflineMsgNotify", CLog::TYPE_IMPORTANT, "���� uuid:%s, userid:%u", arrUUID[i].c_str(), req.userid());
    	Buf_ClientSend ack;
        ack.set_idxpacketreq(0);
        ack.set_bufunitidx(arrUUID[i].c_str());
        ack.set_buflocation(LOCATION_DB);
        CThreadMsgNotify::SendToSeller(m_pRouter->GetDataBaseMini(), req.userid(), ack);
    }*/
    return RESULT_SUCCESS;
}



/***************************************************����δ�������buf****************************************************/
CGetRemainBuf::CGetRemainBuf(CRouterBusiness* pRouter)
    :CBusinessExIMSrv(pRouter,CMD_SEND_BUF_REQ,0)
{

}

int CGetRemainBuf::DoWork()
{
	char szData[1024] = {0};
    int nDataLength = m_RecvPacket.ReadData(szData,1024);
    Buf_ClientSend req;
    if(!req.ParseFromArray(szData, nDataLength))
    {
        CLog::Log("CGetRemainBuf", CLog::TYPE_ERROR, "REQ PARSE FAILD");
        return RESULT_FAILED;
    }

    // ȥ���ݿ��в���������Ϣ��������
    CLog::Log("CGetRemainBuf", CLog::TYPE_IMPORTANT, "����ʣ���buf. uuid[%s] idx[%d]", req.bufunitidx().c_str(), req.idxpacketreq());
    CThreadMsgNotify::SendToSeller(m_pRouter->GetGlobalBusi(), m_pRouter->GetDataBaseMini(), m_pRecvHead->sid, req);
    return RESULT_SUCCESS;
}



#if 0
void CBusinessSetCSClient::updateCsQueue(ClientService *clientService)
{
    CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"���¿ͷ���Ϣ %u has %u !",clientService->m_nID);
    uint8 count = 1;
    CSinfo_t csinfo;

    sprintf(csinfo.m_Csname,"%s",clientService->m_CSname);
    csinfo.m_CurConsult = clientService->m_CurConsult;
    csinfo.m_CurReception = clientService->m_CurReception;
    csinfo.m_MaxRecConsult = clientService->m_MaxRecConsult;
    csinfo.m_nID = clientService->m_nID;
    csinfo.m_nChattingBuyerID = clientService->GetChattingBuyerID();
    csinfo.m_nDirectorID = clientService->GetDirectorID();
    csinfo.m_State = clientService->m_State;
    csinfo.m_Customerid.clear();
    map<uint32,CSortArray<customer_t> *>::iterator mappedIter;
    for(mappedIter = clientService->m_Customerlist.begin(); mappedIter != clientService->m_Customerlist.end();
        mappedIter++)
    {
        for(int i = 0; i < (mappedIter->second)->GetSize(); ++i)
        {
            customer_t *pFriendInfo = (mappedIter->second)->FindByPos(i);
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT,"��ǰ�� %u  ���û� %u",
                      pFriendInfo->teamid,
                      pFriendInfo->userid,
                      pFriendInfo->ReceptionState);
            csinfo.m_Customerid.push_back(pFriendInfo->userid);
        }
    }

    m_pRouter->GetGlobalBusi()->AddModiCSInfo(&csinfo,count,tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_MODIFY);

}

#endif

int CBusinessSetCSClient::DoWork()
{
    XT_CLEAR_CS_CONSULT_REQ * lpReq = (XT_CLEAR_CS_CONSULT_REQ *)m_RecvPacket.GetData();

    CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"CS:%d �յ���տͷ��Ӵ���web�û�%u ��%u������ IN",
              lpReq->clientId,lpReq->csId,lpReq->teamId);
#ifndef IM_USE_IMAPI
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    ClientService * clientService  = csManager.Find(lpReq->csId);
    if(clientService)
    {
        clientService->RemoveCustomer_x(lpReq->clientId);
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"CS:%d �յ���տͷ��Ӵ���web�û�%u ��%u������ OUT",
                  lpReq->clientId,lpReq->csId,lpReq->teamId);
    }
    else
    {
        CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"CS:%d �յ���տͷ��Ӵ���web�û�%u ��%u������,���ǲ��ڱ��������� OUT",
                  lpReq->clientId,lpReq->csId,lpReq->teamId);
    }
#endif
    return 0;
}


#if 0

CBusinessUseridMapCSid::CBusinessUpdateUserInfo(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_USERID_MAP_CSID_REQ,CMD_GET_USERID_MAP_CSID_ACK)
{

}
#endif



CBusinessUpdateCsReq::CBusinessUpdateCsReq(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_UPDATE_CSINFO_REQ,0)
{

}





#ifdef IM_USE_IMAPI
int CBusinessUpdateCsReq::DoWork()
{
    char szData[1024] = {0};
    int nDataLen = m_RecvPacket.ReadData(szData,1023);

    CsUpdateOperReq req;
    if(!req.ParseFromArray(szData, nDataLen))
    {
        // �����л�ʧ��
        CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_ERROR,"�����л�ʧ��");
        return 0;
    }
	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> clientService = pUserManager->Find(req.csid());
	
    if(clientService != NULL)
    {
		if(req.updateoper() == UO_SET_MASTER)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%u��ȡ�����", clientService->m_nID);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%u������%u���", clientService->m_nID, req.custid());
        	}
        	clientService->SetDirectorID(req.custid());
			m_pRouter->GetGlobalBusi()->UpdateCsMonitor(clientService.get());
			//CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
        else if(req.updateoper() == UO_SET_CHATTING_CUST)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%uȡ��������������", clientService->m_nID);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%u����������������%u", clientService->m_nID, req.custid());
        	}
        	clientService->SetChattingBuyerID(req.custid());
			m_pRouter->GetGlobalBusi()->UpdateCsMonitor(clientService.get());
			//CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
    }
    else
    {
        CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_ERROR, "�ͷ����ڱ�������", req.csid());
    }
    return 0;
}



#else

int CBusinessUpdateCsReq::DoWork()
{
    char szData[1024] = {0};
    int nDataLen = m_RecvPacket.ReadData(szData,1023);

    CsUpdateOperReq req;
    if(!req.ParseFromArray(szData, nDataLen))
    {
        // �����л�ʧ��
        CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_ERROR,"�����л�ʧ��");
        return 0;
    }

	CSManager & csManager = CAppManager::GetInstance().GetCSManager();
	ClientService * clientService = NULL;
	clientService = csManager.Find(req.csid());

    if(clientService != NULL)
    {
        if(req.updateoper() == UO_DEL_CUST)
        {
            CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%s������%u", clientService->m_CSname, req.custid());
            clientService->RemoveCustomer_x(req.custid());

            /*�޸��û���Ϣ��memcached��*/
            CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        }
        else if(req.updateoper() == UO_ADD_CUST)
        {
            CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%s������%u", clientService->m_CSname, req.custid());

            //��ѯȫ�ַ����������web���ڷ�������Ӵ�������
            tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
            uint8 count = 1;
            uint32 custid = req.custid();
            m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&custid,&UserInfo,count);

            if(UserInfo.uid != custid)
            {
                CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_IMPORTANT,
                          "��ѯ���ȫ����Ϣʧ��! %u", custid);
            }
            else
            {
                clientService->AddCustomer_x(UserInfo.teamID, req.custid());

                /*�޸��û���Ϣ��memcached��*/
                CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
            }
        }
		else if(req.updateoper() == UO_ADD_RECPTION)
		{
			//ver�ֶ���ΪteamID�ֶ�
			clientService->ModifyCustomerState_x(req.custid());
			CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
			CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_IMPORTANT,"CS:%d ���ӿͷ��Ӵ���web�û�%u ��",
              req.csid(),req.custid());
		}		
		else if(req.updateoper() == UO_DEL_RECPTION)
		{
			clientService->RemoveCustomer_x(req.custid());
	        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
	        CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_IMPORTANT,"CS:%d ��տͷ��Ӵ���web�û�%u ��",
                  req.csid(),req.custid());
		}
        else if(req.updateoper() == UO_SET_MASTER)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%s��ȡ�����", clientService->m_CSname);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%s������%u���", clientService->m_CSname, req.custid());
        	}
        	clientService->SetDirectorID(req.custid());
			CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
        else if(req.updateoper() == UO_SET_CHATTING_CUST)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%sȡ��������������", clientService->m_CSname);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "�ͷ�%s����������������%u", clientService->m_CSname, req.custid());
        	}
        	clientService->SetChattingBuyerID(req.custid());
			CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
    }
    else
    {
        CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_ERROR, "�ͷ����ڱ�������", req.csid());
    }

    return 0;
}

#endif

//��ȡ�ͷ���������Ϣ
/*
#define CMD_CS_TEAM_INFO_REQ			0x305
#define CMD_CS_TEAM_INFO_ACK			0x306
*/
//���Ϳͷ��������Ŷ�����Ϣ
/*
#define CMD_CS_TEAM_INFO_REQ			0x307
#define CMD_CS_TEAM_INFO_ACK			0x308
*/


CBusinessCsTeamInfoReq::CBusinessCsTeamInfoReq(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_CS_TEAM_INFO_REQ,CMD_CS_TEAM_INFO_ACK)
{

}

int CBusinessCsTeamInfoReq::DoWork()
{
    char szData[1024] = {0};
    int nDataLen = m_RecvPacket.ReadData(szData,1023);

    csTeamInfoReq csTeamInforeq;
    if(!csTeamInforeq.ParseFromArray(szData, nDataLen))
    {
        // �����л�ʧ��
        CLog::Log("CBusinessCsTeamInfoReq",CLog::TYPE_ERROR,"�����л�ʧ��");
        return 0;
    }
	int userID = csTeamInforeq.csid();
	vector<uint32> ids;
    CDataBaseMini *pDatabase = m_pRouter->GetDataBaseMini();
	csTeamInfoAck csTeamInfoack;

    int ret = pDatabase->CS_Get_Groups(userID,ids);
    if(ret != DB_SUCCESS)
    {
        CLog::Log("CBusinessCsTeamInfoReq",CLog::TYPE_ERROR,"Query team %u subaccount failed!",userID);
		csTeamInfoack.set_ret(RESULT_FAILED);
		csTeamInfoack.set_csid(userID);
    }
    else
    {    	
		csTeamInfoack.set_ret(RESULT_SUCCESS);
		csTeamInfoack.set_csid(userID);

		tagGSRV_GET_ONLINE_CSQLEN_ACK::CSQinfo csQlen[32];			
		m_pRouter->GetGlobalBusi()->QueryCSQueueLength(ids,ids.size(),csQlen);
        for(int i = 0 ; i < (int)ids.size(); i++)
        {				
			// ��֪ͨ��ؿͷ�
        	vector<uint32> TeamUser;
        	CDataBaseMini *pMinibase = m_pRouter->GetDataBaseMini();
        	if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(csQlen[i].teamId,TeamUser))
        	{
            	CLog::Log("CBusinessCsTeamInfoReq", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", csQlen[i].teamId);
            	continue;
        	}
			if(std::find(TeamUser.begin(),TeamUser.end(),(uint32)userID) == TeamUser.end()) 
			{	// �û����������
				CLog::Log("CBusinessCsTeamInfoReq", CLog::TYPE_IMPORTANT, "�ͷ�[%u]���������[%u]�ķ���", (uint32)userID, csQlen[i].teamId);
				continue;
			}
        
			csTeamInfo *p = csTeamInfoack.add_teaminfo();
			p->set_csteamid(csQlen[i].teamId);
			p->set_csqueuelength(csQlen[i].queueLength);
			
            CLog::Log("CBusinessCsTeamInfoReq",
				CLog::TYPE_IMPORTANT,
				"Query user in team %u has member %u queueLength !",
				userID,csQlen[i].teamId,csQlen[i].queueLength);
        }

		
    }	
		int msg_size = csTeamInfoack.ByteSize();
		char *pData = new char[msg_size];
		csTeamInfoack.SerializeToArray(pData, msg_size);
		CLog::Log("CBusinessCsTeamInfoReq", CLog::TYPE_DEBUG, "BUF LENGTH:%d", msg_size);
		m_SendPacket.WriteData(pData, msg_size);
		delete[] pData;
		pData = NULL;
		return RESULT_SUCCESS;

}


//֪ͨ�ͷ������춯
CBusinessCSQueueInfo::CBusinessCSQueueInfo(CRouterBusiness* pRouter):
	CBusinessExIMSrv(pRouter,CMD_CS_TEAM_QINFO_REQ,CMD_CS_TEAM_QINFO_ACK)
{


}
	
int CBusinessCSQueueInfo::NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange)	  //֪ͨ����������Ա�Ŷ�״�� 0 �����Լ� 1 �������Լ�
{
	CDataBaseMini* pMinibase = m_pRouter->GetDataBaseMini();
	vector<uint32> TeamUser;

	if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(QueueChange->teamId,TeamUser))
	{
		SetErrorReturn("��ȡ��ϸ����ʧ��");
		CLog::Log("NotifyTeamQueueInfo", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", QueueChange->teamId);
		return RESULT_FAILED;
	}
	for(unsigned int i = 0 ; i < TeamUser.size(); ++i )
	{

		CLog::Log("NotifyTeamQueueInfo", CLog::TYPE_IMPORTANT,"�ڲ�����:TEAM %d ��ѯ����: TEAM %d ,���͸� %u",
				  QueueChange->teamId,QueueChange->teamId,TeamUser[i]);
		//֪ͨ�������ڵĿͷ�
		{
			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
			lpSendHead->cmd 	 = CMD_SERVERMSG_IND;
			lpSendHead->did 	 = TeamUser[i];
			lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
			lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
			lpSendHead->from	 = XT_HEAD::FROM_SERVER;
			CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
			lpSendHead->sid 	 = CAppManager::GetInstance().GetConfig().GetServerNo();

			LPXT_SERVER_TRANSMIT lpSrvTransmit	= (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
			lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			lpSrvTransmit->msgtype				= CMD_WAITQUEU_CHANGE;


			XT_WAITQUEU_CHANGE* pTeamMatesNotify = (XT_WAITQUEU_CHANGE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
			pTeamMatesNotify->bAdd	  = QueueChange->bAdd;
			pTeamMatesNotify->idBuyer = QueueChange->idBuyer;
			pTeamMatesNotify->teamId = QueueChange->teamId;
			pTeamMatesNotify->QueueLength = QueueChange->QueueLength;
			strcpy(pTeamMatesNotify->nicknameBuyer,QueueChange->nicknameBuyer);
			//pFriendStatusNotify->status = m_nStatus;

			sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);
			CLog::Log("NotifyTeamQueueInfo", CLog::TYPE_IMPORTANT,"׼������");

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);

		}
	}

	return RESULT_SUCCESS;

}

int CBusinessCSQueueInfo::DoWork()
{	
	
    XT_WAITQUEU_CHANGE *req = (XT_WAITQUEU_CHANGE*)m_RecvPacket.GetData();	 
	NotifyTeamQueueInfo(req);
	CancelReply();
	return RESULT_SUCCESS;
	 
}


//֪ͨ�ͷ������춯
CBusinessCloseReceptionByCustomer::CBusinessCloseReceptionByCustomer(CRouterBusiness* pRouter):
	CBusinessExIMSrv(pRouter,CMD_BUYER_QUIT_REQ,CMD_BUYER_QUIT_ACK)
{


}
	
int CBusinessCloseReceptionByCustomer::NotifyBuyerQuitInfo(XT_BUYERINFO_CHANGE* infoChange)	  //֪ͨ����������Ա�Ŷ�״�� 0 �����Լ� 1 �������Լ�
{
/*
    // ֪ͨ����ҵĿͷ����������
    CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = pUserManager->Find(infoChange->idBuyer);
	if(pUser == NULL)
	{
		CLog::Log("CBusinessCloseReceptionByCustomer", 
    		CLog::TYPE_ERROR, "buyer no exist. buyerid[%u] csid[%u] ip[%u]", infoChange->idBuyer, infoChange->idSeller, infoChange->loginIP);
    	return RESULT_SUCCESS;	
	}

	if(pUser->m_nIP != infoChange->loginIP)
	{
		CLog::Log("CBusinessCloseReceptionByCustomer", 
    		CLog::TYPE_ERROR, "ip error! redis ip[%u] loginip[%u]. buyerid[%u] csid[%u]", pUser->m_nIP, infoChange->loginIP, infoChange->idBuyer, infoChange->idSeller);
    	return RESULT_SUCCESS;	
	}
*/	

    CLog::Log("CBusinessCloseReceptionByCustomer", 
    	CLog::TYPE_IMPORTANT, "idBuyer[%u] idSeller[%u] teamId[%u]",
    	infoChange->idBuyer,infoChange->idSeller,infoChange->teamId);
    {
		// �޸����״̬Ϊ����
		tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser info;
    	info.id = infoChange->idBuyer;
		info.status = XTREAM_OFFLINE;
		uint8 count = 1;
		m_pRouter->GetGlobalBusi()->ModiUserStatus(&info,count,NULL);

		// ���ÿͷ�������������
		CUser::g_cs_set_chatting_buyer(infoChange->idSeller,0,m_pRouter->GetGlobalBusi());
    
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
        lpSendHead->cmd      = CMD_SERVERMSG_IND;
        lpSendHead->did      = infoChange->idSeller;
        lpSendHead->dtype    = XT_HEAD::DEST_TYPE_USER;
        lpSendHead->flag     = XT_HEAD::FLAG_SRVACTION;
        lpSendHead->from     = XT_HEAD::FROM_SERVER;
        CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
        lpSendHead->sid      = CAppManager::GetInstance().GetConfig().GetServerNo();

        LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
        lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
        lpSrvTransmit->msgtype              = CMD_BUYER_OFFLINE_IND;

        UserOfflineIND ind;
        ind.set_userid(infoChange->idBuyer);

        uint32_t msg_size = ind.ByteSize();
        char* szData = new char[msg_size];
        if (!ind.SerializeToArray(szData, msg_size))
        {
            CLog::Log("CBusinessCloseReceptionByCustomer", CLog::TYPE_IMPORTANT, "��������˳���Ϣʱ���к�ʧ��");
        }
        else
        {
            memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT), szData, msg_size);
            sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + msg_size;

            lpSendHead->len      = sizeof(XT_SERVER_TRANSMIT) + msg_size;

            CLog::Log("CBusinessCloseReceptionByCustomer", 
				CLog::TYPE_IMPORTANT,"�������[%u]�˳���Ϣ���ͷ�[%u]",
				infoChange->idBuyer, infoChange->idSeller);
            CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                sendData.data,sendData.data_len,SEND_RETRY_TIME);
        }
        delete[] szData;
        szData = NULL;
    }


	return RESULT_SUCCESS;

}

int CBusinessCloseReceptionByCustomer::DoWork()
{	
	
    XT_BUYERINFO_CHANGE *req = (XT_BUYERINFO_CHANGE*)m_RecvPacket.GetData();	 
	NotifyBuyerQuitInfo(req);
	CancelReply();
	return RESULT_SUCCESS;
	 
}



//֪ͨ�ͷ������춯
CBusinessForceOfflineCs::CBusinessForceOfflineCs(CRouterBusiness* pRouter):
	CBusinessExIMSrv(pRouter,CMD_FORCE_OFFLINE_CS_REQ,CMD_FORCE_OFFLINE_CS_ACK)
{


}
	
int CBusinessForceOfflineCs::ForceOfflineCs(XT_FORCE_OFFLINE_CS* xtOfflineCs)	
{
    CLog::Log("CBusinessForceOfflineCs", 
    	CLog::TYPE_IMPORTANT, "csId[%u]",
    	xtOfflineCs->idCs);
    {
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
        lpSendHead->cmd      = CMD_SERVERMSG_IND;
        lpSendHead->did      = xtOfflineCs->idCs;
        lpSendHead->dtype    = XT_HEAD::DEST_TYPE_USER;
        lpSendHead->flag     = XT_HEAD::FLAG_SRVACTION;
        lpSendHead->from     = XT_HEAD::FROM_SERVER;
        CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
        lpSendHead->sid      = CAppManager::GetInstance().GetConfig().GetServerNo();

        LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
        lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
        lpSrvTransmit->msgtype              = CMD_FORCE_OFFLINE_CS_IND;

        sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT);
        lpSendHead->len      = sizeof(XT_SERVER_TRANSMIT);

        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                sendData.data,sendData.data_len,SEND_RETRY_TIME);
    }

	return RESULT_SUCCESS;
}

int CBusinessForceOfflineCs::DoWork()
{	
	
    XT_FORCE_OFFLINE_CS *req = (XT_FORCE_OFFLINE_CS*)m_RecvPacket.GetData();	 
	ForceOfflineCs(req);
	CancelReply();
	return RESULT_SUCCESS;
	 
}

CBusinessGetOfflineMSG::CBusinessGetOfflineMSG(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GET_OFFLINE_MSG_REQ,0)
{

}

int CBusinessGetOfflineMSG::DoWork()
{
	GetOfflineMSGFromIMAPI(m_pRecvHead->sid);
	return RESULT_SUCCESS;
}

//��IMAPI��ȡ������Ϣ
bool CBusinessGetOfflineMSG::GetOfflineMSGFromIMAPI(const uint32 UserId)
{
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strOfflineMSGurl(CAppManager::GetInstance().GetConfig().GetUserOfflineMSGUrl());

	stringstream ssParams;
  	time_t CurrentTime;
	time(&CurrentTime);
	//uint32 sid=10078;
        //CurrentTime=1458811871;
        CUserManager *pUserManager = m_pRouter->GetUserManager();
        shared_ptr<CUser> pUser = pUserManager->Find(UserId);
	uint32 LoginTime=0;
	if(pUser)
	{
	    LoginTime=pUser->m_nLoginTime;
	}
	ssParams<<"serviceId=" << UserId;
	ssParams<<"&onlineTime=" << LoginTime;
	
	string strOfflineMSGlist;
	int httpStaus = httpClient.Post(strOfflineMSGurl, ssParams.str(), strOfflineMSGlist);
        CLog::Log("GetOfflineMSGFromIMAPI", CLog::TYPE_DEBUG, "strOfflineMSGurl=%s%s",strOfflineMSGurl.c_str(),ssParams.str().c_str());
	CLog::Log("GetOfflineMSGFromIMAPI", CLog::TYPE_DEBUG, "strOfflineMSGlist=%s",strOfflineMSGlist.c_str());
	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvRecvResponse;
		if (jReader.parse(strOfflineMSGlist, jvRecvResponse)){
			if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true"){
				if(jvRecvResponse.isMember("data")) {
					if(jvRecvResponse["data"].isMember("offlineMessage")){
						Json::Value jvMSGList = jvRecvResponse["data"]["offlineMessage"];
						for(int i = 0; i < (int)jvMSGList.size(); ++i)
						{
							Json::Value jvMSGItem = jvMSGList[i];
							SendOfflineMSGToClient(UserId,jvMSGItem);
						}
						
						return true;
					}
				}
			}
		}
	}

	return false;

}

//��ͻ��˷���������Ϣ
bool CBusinessGetOfflineMSG::SendOfflineMSGToClient(const uint32 UserId,const Json::Value& jvMSGItem)
{
    CLog::Log("SendOfflineMSGToClient", CLog::TYPE_DEBUG, "Send Offline MSG To Client UserId=%d",UserId);
    SendDataStruct sendData2Client;

    LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData2Client.data + sizeof(XT_HEAD));
    lpSrvTransmit->origin_id = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    lpSrvTransmit->msgtype = CMD_P2PMSG_SEND;

    CSendPacket sendPacket(sendData2Client.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));

    XT_MSG msg;
    msg.data_type = XT_MSG::IM_MSG;
    uint32 fromId=0;
    uint32 toId=0;
    msg.recv_flag=1;//msg from server
    std::string strMsg;

    if(jvMSGItem.isMember("fromId"))
    {
    	 fromId= jvMSGItem["fromId"].asInt();
    }
    if(jvMSGItem.isMember("toId"))
    {
         toId= jvMSGItem["toId"].asInt();
    }
    if(jvMSGItem.isMember("sendTime"))
    {
         msg.send_time= jvMSGItem["sendTime"].asInt();
    }
    if(jvMSGItem.isMember("msg"))
    {
         strMsg= jvMSGItem["msg"].asString();
    }
    if(jvMSGItem.isMember("features"))
    {
        if(jvMSGItem["features"].isMember("color"))
		{
            msg.fontColor=jvMSGItem["features"]["color"].asInt();
        }
		if(jvMSGItem["features"].isMember("size"))
        {
            msg.fontSize=jvMSGItem["features"]["size"].asInt();
        }        
		if(jvMSGItem["features"].isMember("family"))
        {
        	string strFontName = jvMSGItem["features"]["family"].asString();
        	char szFontName[50]= {0};
    		utf2gbk((char*)strFontName.c_str(),strFontName.size(), szFontName, 49);	
            strncpy(msg.fontName,szFontName,49);
        }   
		if(jvMSGItem["features"].isMember("style"))
        {
            msg.fontStyle=jvMSGItem["features"]["style"].asInt();
        }    
    }
    if(jvMSGItem.isMember("nickName"))
    {
      	string strNickName = jvMSGItem["nickName"].asString();
       	char szNickName[MAX_NICKNAME_LEN+1]= {0};
    	utf2gbk((char*)strNickName.c_str(),strNickName.size(), szNickName, MAX_NICKNAME_LEN);	
        strncpy(msg.from_nickname,szNickName,MAX_NICKNAME_LEN);
    }   
    sendPacket<<fromId
              <<toId
              <<msg.data_type
              <<msg.ver
              <<msg.send_time
              <<msg.recv_flag
              <<msg.fontSize
              <<msg.fontColor
              <<msg.fontStyle
              <<msg.fontName;

    // utf8 to gbk 
    char strMsgGBK[MAX_NETWORK_PACKET]= {0};
    int ret=utf2gbk((char*)strMsg.c_str(),strMsg.size(), strMsgGBK, MAX_NETWORK_PACKET);
    if(ret<0)
    {
        CLog::Log("SendOfflineMSGToClient", CLog::TYPE_DEBUG, "utf2gbk failed! ret=",ret);
        return false;
    }
    strcpy(msg.data, strMsgGBK);
    msg.data_len = ret+1;

    //strcpy(msg.data, strMsg.c_str());
    //msg.data_len = strMsg.length() + 1;
    CLog::Log("SendOfflineMSGToClient", CLog::TYPE_DEBUG, "###Json String:%s fromId=%d", msg.data,fromId);

    sendPacket.WriteData(msg.data,strlen(msg.data));
    sendPacket<<msg.from_nickname;

    sendData2Client.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_SERVER_P2PMSG_SEND) + sendPacket.GetSize());

    LPXT_HEAD lpSendHead2buyer = (LPXT_HEAD)sendData2Client.data;

    CAppManager::GetInstance().InitHead(lpSendHead2buyer,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) +  sendPacket.GetSize()),UserId,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_SRVACTION);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData2Client.data,sendData2Client.data_len,SEND_RETRY_TIME);
    return TRUE;
}


#if 0
/*������߼���Ӧ�ò���Ҫ����ʱ�����պ�ʹ��*/


CBusinessModiOtherUser::CBusinessModiOtherUser(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_MOD_USER_OTHER_QUERY,CMDGSRV_MOD_USER_OTHER_ACK)
{


}

int CBusinessModiOtherUser::DoWork()
{

    CLog::Log("CBusinessModiOtherUser",CLog::TYPE_ERROR,"CMDGSRV_GET_USER_OTHER_QUERY");

    CUser* pNewUser = NULL;
    nReloginCount = 0;

    //�����ʲô��������
    uint8 action;
    uint8 nUserCount;
    m_RecvPacket>>action;
    m_RecvPacket>>nUserCount;

    m_SendPacket << (uint8)V_SUCCESS    //�������
                 << (uint8) 0 ;       //���ظ�����������ٴ��޸ĸ�ֵ

    uint32 nClientID;
    bool    bIsNewUser = false;
    int count = 0 ;

    if(action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD ||
       action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY  )
    {
        for(int i = 0 ; i < nUserCount; ++i)
        {

            bIsNewUser = false;
            m_RecvPacket>>nClientID;

            if( 0 > nClientID ||
                nClientID >= MAX_139_ACCOUNT_NUM)
            {
                CLog::Log("COnlineManager",CLog::TYPE_ERROR,"�û�IDԽ��,���ܰ����ݴ���");
                return false;
            }


            //���ԭ���Ƿ��Ѿ����ڸ��û�
            pNewUser = CAppManager::GetInstance().GetUserManager()->Find(nClientID);
            if( pNewUser == NULL )
            {

                pNewUser = CAppManager::GetInstance().GetUserManager().AddHash(nClientID);
                pNewUser->Reset();
                pNewUser->m_nID = nClientID;
                pNewUser->AddToOnlineList();
                SysStat::GetInstance().addOnlineNum();

                if( NULL == pNewUser )
                {
                    CLog::Log("CBusinessModiOtherUser",CLog::TYPE_ERROR,"�ڴ����ʧ��");
                    continue;
                }
                pNewUser->m_flow = 0;
                bIsNewUser = true;
            }

            uint32 NewServerIP;
            uint32 OldUserIp;
            uint16 OldUserPort;
            uint32 OldLanIp;
            if (!bIsNewUser)
            {
                OldUserIp   = pNewUser->m_ip;
                OldUserPort = pNewUser->m_port;
                OldLanIp    = pNewUser->m_LanIp;
            }
            //�������û�����Ϣ
            pNewUser->m_UserID = nClientID;
            m_RecvPacket>>pNewUser->m_status
                        >>pNewUser->m_rule
                        >>pNewUser->m_UserName
                        >>pNewUser->m_NickName
                        >>pNewUser->m_ip
                        >>pNewUser->m_port
                        >>pNewUser->m_sexy
                        >>NewServerIP
                        >>pNewUser->m_LanIp
                        >>pNewUser->flag;

            if (pNewUser->m_status == XTREAM_OFFLINE)
                CLog::Log("AddModifyOnlineUser", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d", nClientID);

            //����Ƿ��ص�½
            if( !bIsNewUser && action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD )
            {
                if (pNewUser->m_ServerIp != NewServerIP && pNewUser->m_ip == OldUserIp && pNewUser->m_port == OldUserPort) //need del by lwl ver:0101
                    CLog::Log("COnlineManager::AddModifyOnlineUser", CLog::TYPE_ERROR, "ʹ������ͬ��IP��Port��¼��ͬ�ķ�����");
                if( pNewUser->m_ServerIp != NewServerIP || (pNewUser->m_ip != OldUserIp) || (pNewUser->m_LanIp != OldLanIp))
                {
                    //���һ���Ѿ����ڵ��û���˵�����ص�¼���������
                    //��ʱҪ֪ͨ�ɵ�IM������,���������¼����
                    pReloginUserList[nReloginCount].userid = nClientID;
                    pReloginUserList[nReloginCount].serverip = pNewUser->m_ServerIp;
                    pReloginUserList[nReloginCount].userip  = pNewUser->m_ip;
                    pReloginUserList[nReloginCount].lanip   = pNewUser->m_LanIp;

                    ++nReloginCount;
                }

            }

            pNewUser->m_ServerIp = NewServerIP;
            m_RecvPacket.ReadPacket((char*)pNewUser->m_sessionKey,sizeof(pNewUser->m_sessionKey));
            m_RecvPacket.ReadPacket((char*)pNewUser->m_p2pSessionKey,sizeof(pNewUser->m_p2pSessionKey));






        }


        m_SendPacket << pNewUser->m_UserID;
        ++count;

        if( !m_SendPacket.HasData(sizeof(pNewUser->m_UserID)))
        {
            //�ռ�洢����
            break;
        }


        //�������óɹ����û���
        GSRV_ADDMODIFY_FULLUSERINFO_ACK* pHead = (GSRV_ADDMODIFY_FULLUSERINFO_ACK*)m_SendPacket.GetData();
        pHead->count = count ;
    }
    else
    {
        CLog::Log("COnlineManager::AddModifyOnlineUser",CLog::TYPE_ERROR,"�յ�һ��δ֪action��Э���:%d",action);
        return false;
    }

    return true;
}

int CBusinessModiOtherUser::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //Ӧ��
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessModiOtherUser:SendReply",CLog::TYPE_ERROR,"��������:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(pSendData->data,
                              pSendData->data_len,
                              pSendData->retry_times,
                              pSendData->time_interval,
                              pRecvData->src_ip,
                              PORT_MEMCACHED_ONLINE,
                              m_pSendData->vlink
                             );
    return RESULT_SUCCESS;
}


CBusinessDelOtherUser::CBusinessDelOtherUser(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_DEL_USER_OTHER_QUERY,CMDGSRV_DEL_USER_OTHER_ACK)
{


}

int CBusinessDelOtherUser::DoWork()
{

    CLog::Log("CBusinessDelOtherUser",CLog::TYPE_ERROR,"CMDGSRV_DEL_USER_OTHER_QUERY");

    GSRV_GET_ONLINE_FULLINFO_QUERY* pQueryHead = (GSRV_GET_ONLINE_FULLINFO_QUERY*)m_RecvPacket.GetData();
    GSRV_GET_ONLINE_FULLINFO_ACK*    pAckHead   = (GSRV_GET_ONLINE_FULLINFO_ACK*)m_SendPacket.GetData();


    m_SendPacket << (uint8)0;       //�ɹ���־
    m_SendPacket << (uint8)0;       //���ظ���,�������ʵ��ֵ�������

    CUser* pUser = NULL;
    uint32 nClientID;
    int i;
    for(i = 0 ; i < pQueryHead->count; ++i)
    {
        nClientID = pQueryHead->userid[i];
        if( 0 > nClientID ||
            nClientID >= MAX_139_ACCOUNT_NUM)
        {
            CLog::Log("CBusinessDelOtherUser",CLog::TYPE_ERROR,"id_over:%u", nClientID);
            return false;
        }
        pUser = CAppManager::GetInstance().GetUserManager()->Find(nClientID);
        if( NULL == pUser)
        {

            CLog::Log("CBusinessDelOtherUser",CLog::TYPE_ERROR,"dont have INFO:%u", nClientID);
            //˵�����û�������
            m_SendPacket << nClientID
                         << (uint8)XTREAM_OFFLINE    //����
                         //�������������ֵ,���������
                         << (uint16)0                //version
                         << "username"
                         << "nickname"
                         << (uint32)0                //ip
                         << (uint16)0                //port
                         << (uint8)0                 //sexy
                         << (uint32)0;               //serverip

            m_SendPacket.WritePacket((const char*)pAckHead->user[0].sessionkey,sizeof(pAckHead->user[0].sessionkey));
            m_SendPacket.WritePacket((const char*)pAckHead->user[0].p2psessionkey,sizeof(pAckHead->user[0].p2psessionkey));
        }
        else
        {

            CLog::Log("COnlineManager:GetUserFullInfo",CLog::TYPE_ERROR,"HAVE USER INFO:%u", nClientID);
            m_SendPacket << nClientID
                         << pUser->m_nStatus
                         << (uint16)0    //pUser->m_version
                         << pUser->m_szUserName
                         << pUser->m_szNickName
                         << pUser->m_nIP
                         << pUser->m_nPort
                         << pUser->m_nSexy
                         << pUser->m_nLanIP;

            m_SendPacket.WritePacket((const char*)pUser->m_SessionKey,sizeof(pAckHead->user[0].sessionkey));
            m_SendPacket.WritePacket((const char*)pUser->m_P2PSessionKey,sizeof(pAckHead->user[0].p2psessionkey));
        }

        //�����һ���û�����Ϣ�Ƿ���װ�ص���
        if( !m_SendPacket.HasData(sizeof(pAckHead->user[0])) )
        {
            //�����ߴ���,�˳�
            break;
        }

    }

    pAckHead->count = i ;
    return true ;
}

int CBusinessDelOtherUser::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //Ӧ��
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessDelOtherUser:SendReply",CLog::TYPE_ERROR,"��������:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(pSendData->data,
                              pSendData->data_len,
                              pSendData->retry_times,
                              pSendData->time_interval,
                              pRecvData->src_ip,
                              PORT_MEMCACHED_ONLINE,
                              m_pSendData->vlink
                             );
    return RESULT_SUCCESS;
}




#endif
