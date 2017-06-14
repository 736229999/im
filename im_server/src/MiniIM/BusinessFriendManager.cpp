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




//处理接待请求  两种方式 一种开始接待；一种结束接待

//现有模式是处理主动请求接待模式
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


void CBusinessCSQueue::WebLogOut(uint32 fromid,uint32 toid,uint16 type)   //向WEB服务器发送退出请求
{
    // 构造转发消息结构体
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
    CLog::Log("CBusinessCSQueue::WebLogOut",CLog::TYPE_ERROR,"%u向WEB发送让用户%u退出请求",fromid,toid);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}

void CBusinessCSQueue::SetWebContext(uint32 toid,uint32 csid,uint32 teamid)
{
    //查询全局服务器并清除web所在服务器其接待上下文
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
                  "通知服务器 %u 设置web用户 %d的接待信息于客服%u 组%u !",
                  UserInfo.srvip,toid,csid,teamid);
    }
}


void CBusinessCSQueue::ClearWebContext(uint32 toid,uint32 &teamid)
{
    //查询全局服务器并清除web所在服务器其接待上下文
    CConfigInfo m_ConfigInfo;
    list < uint32 > ServerList;
    m_ConfigInfo.GetServerList(&ServerList);
    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&toid,&UserInfo,count);

    if(UserInfo.uid != toid)
    {
        teamid = 0;
        CLog::Log("CBusinessCSQueue::ClearWebContext",CLog::TYPE_IMPORTANT,"获取全局信息异常 toid %u UserInfo.id %u !",toid,UserInfo.uid);
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
        CLog::Log("CBusinessCSQueue::ClearWebContext",CLog::TYPE_IMPORTANT,"通知服务器 %u 去掉web用户 %d的全局信息 !",UserInfo.srvip,toid);
    }
}

#if 0
void CBusinessCSQueue::updateCsQueue(ClientService *clientService)
{
    CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"更新客服信息 %u has %u !",clientService->m_nID);
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
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT,"当前组 %u  有用户 %u",
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
        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "查找客服上下文失败 id: %u",m_pRecvHead->sid );
        return RESULT_FAILED;
    }
#endif
    if(req->type == XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER)   //自动接待
    {
#ifdef IM_USE_IMAPI
		//  7. 主动邀约，ServiceId和group_id必填。
		
		imapi csAskforConReq;
		string info;
		csAskforConReq.imapiReq(imapi::CSASKFORCONSULT,info,m_pRecvHead->sid);
	
	
#else
	
        //获取客服所在组


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
            groups.push_back(0);    //考虑在0组的排队
            for(int i = 0 ; i < (int)groups.size(); i++)
            {

                CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,
                          "Query CS %u's in group %u!",
                          m_pRecvHead->sid,groups[i]);
            }
        }

        //获取客服所在组排队情况

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
        //排序
        if(customersList.size() > 0)
        {
            sort(customersList.begin(),customersList.end(),SortQueueMethod);
			CheckAndCleanQueue(m_pRouter,customersList);
        }
        else
        {
            CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"No Queue");
        }
       
		
        //循环获取现在取的排队index号


        //从全局memcached删除排队队列

        if(customersList.size() == 0)
        {
            //数据长度写入数据包
            m_SendPacket << (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER
                         << (uint32)0
                         << (uint32)0;

            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "客服 %u未发生排队，无法主动接待",m_pRecvHead->sid);

            return RESULT_SUCCESS;
        }

        tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
        cSinfo.customerID = customersList[0].userid;
        cSinfo.teamID = customersList[0].teamID;
        uint8 count = 1;
        ret = -1;


        //将用户加入到接待队列中
        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "将用户加入到接待队列中 id: %u",customersList[0].userid);
        clientService->AddCustomer_x(customersList[0].teamID,customersList[0].userid);

        //CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(customersList[0].userid);
        if(pUser != NULL)
        {
            pUser->setCsuserId(m_pRecvHead->sid);
            pUser->setCsTeamId(customersList[0].teamID);
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "将用户从接待队列中删除 id: %u",m_pRecvHead->sid);

            // 用户信息回写入redis
//            m_pRouter->GetUserManager()->AddUserToRedis(pUser);
        }
        else//通知所在的用户设置WEB上下文
        {
            SetWebContext(customersList[0].userid,m_pRecvHead->sid,customersList[0].teamID);
        }
        /*修改用户信息到memcached中*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "从本地排队队列删除用户,通知其他客服 ，本地客服为id: %u",m_pRecvHead->sid);

        QueueInd.bAdd = XT_WAITQUEU_CHANGE::TYPE_DELETE;
        QueueInd.teamId = customersList[0].teamID;
        QueueInd.idBuyer = customersList[0].userid;
        clientService->NotifyTeamQueue(m_pRouter->GetGlobalBusi(), &QueueInd,1);

        CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"从全局memcached删除排队用户 %u!",cSinfo.customerID);
        ret = m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("CBusinessCSQueue",CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
        }
        PushQueueToKafka(m_pRouter,customersList[0].teamID,customersList[0].userid,m_pRecvHead->sid);

        //数据长度写入数据包
        m_SendPacket << (uint8)XT_RECEPTE_CUST_REQ::TYPE_AUTO_OFFER
                     << (uint32)customersList[0].userid
                     << (uint32)customersList[0].teamID;

        CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT, "分配客户 %u 到 客服%u",customersList[0].userid,m_pRecvHead->sid);

        return RESULT_SUCCESS;

#endif    
	}
    
    else if((req->type == XT_RECEPTE_CUST_REQ::TYPE_QUIT)) //退出接待 这里不会退出用户，只通知web，并清空本地和客服的关联
    {
        uint32 teamID = -1; //记录用户所在的组ID，便于通知web
        //通知web端将用户下线
        CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "通知web端将用户下线 id: %u",m_pRecvHead->sid );
        shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(req->idBuyer);
        if(pUser != NULL)
        {
            // pUser->SendLogoutReq(pUser);
            //不登出web用户，只修改用户和客服的关联
            CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "将用户从接待队列中删除 id: %u",m_pRecvHead->sid );
            teamID = pUser->getCsTeamId();
            uint32 curCSUserID = pUser->m_nCSuserid;
			#ifndef IM_USE_IMAPI
            clientService->RemoveCustomer_x(req->idBuyer);
			#endif
            pUser->ClearWebClient();	
            if(curCSUserID == m_pRecvHead->sid)
            {
            	 //回复web退出客服接待通知,调用传输层直接发送
            	WebLogOut(m_pRecvHead->sid,req->idBuyer);
				#ifndef IM_USE_IMAPI
            	/*修改用户信息到memcached中*/
            	CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
				#endif
            }
            else
            {
            	CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "买家[%u]的客服[%u]不为[%u]",req->idBuyer, curCSUserID, m_pRecvHead->sid );
            }
        }
        else
        {
            CLog::Log("CBusinessCSQueue::TYPE_QUIT", CLog::TYPE_IMPORTANT, "通知web端用户不在本地 %u",m_pRecvHead->sid );
            //从全局服务器获取
            ClearWebContext(req->idBuyer,teamID);
            WebLogOut(m_pRecvHead->sid,req->idBuyer);
        }
        //将用户从接待队列中删除
#ifdef IM_USE_IMAPI
        //推送用户所在组的所有排队用户信息
        //PushQueueSubAccountToKafka(m_pRouter,teamID);
 
    	// 0：客服关闭窗口，停止接待，SERVICE_ID和group_id必填。
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

    //查询用户上下文，并同步一下

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
    //接口支持id数组，目前只有一个id的请求
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
        //上传挂起接待动作
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

    //获取数据包指针
    XT_GET_USER_STATUS_REQ *req = (XT_GET_USER_STATUS_REQ*)m_RecvPacket.GetData();

    //初始化用户状态数据
//  std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(req->cnt);
    GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[MAX_SEARCH_FRINED_RETURN_NUM];
    uint8_t cnt = req->cnt;

    //获取用户状态信息

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


    //数据长度写入数据包
    m_SendPacket << (uint32_t)cnt;

    //遍历状态数组，写入数据包
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
//查用户子账号的暂不处理memcached异常情况
int CBusinessCheckSubOnline::DoWork()
{
    XT_GET_SUB_ONLINE_REQ *req = (XT_GET_SUB_ONLINE_REQ*)m_RecvPacket.GetData();

    m_SendPacket << req->cnt;

    for (uint32 i = 0; i < req->cnt; ++i)
    {
        m_SendPacket << req->id[i];
        AccountArray ids;       //所有对应的子账号

        //获取所有的子账号
        MYSQL_LIB->getAllSubAccount(req->id[i], ids);
        //ids.push_back(req->id[i]);

        uint8_t cnt = ids.size();

        //初始化用户状态信息
        std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(ids.size());
        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(&ids[0], &userStatus[0], cnt))
        {

            //写入数据包
            m_SendPacket << (uint32_t)0;
            CLog::Log("CBusinessHasSubOnline",CLog::TYPE_ERROR, "can't find %d's sub account", req->id[i]);
        }
        else
        {
            //判断子账号中是否有在线的
            auto ifind = std::find_if(userStatus.begin(), userStatus.begin() + cnt,
                                      [] (const GSRV_GET_USER_STATUS_ACK::tagUserStatus& a) ->bool
            {
                return a.status != XTREAM_OFFLINE;
            });

            //判断是否在线
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


int CBusinessGetSubAccount::NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange)    //通知组内其他成员排队状况 0 包括自己 1 不包括自己
{
    CDataBaseMini* pMinibase = m_pRouter->GetDataBaseMini();
    vector<uint32> TeamUser;

    if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(QueueChange->teamId,TeamUser))
    {
        SetErrorReturn("获取详细资料失败");
        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", QueueChange->teamId);
        return RESULT_FAILED;
    }
    for(unsigned int i = 0 ; i < TeamUser.size(); ++i )
    {

        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"内部队列:TEAM %d 查询队列: TEAM %d ,发送给 %u",
                  QueueChange->teamId,QueueChange->teamId,TeamUser[i]);
        //通知所有组内的客服
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
            pTeamMatesNotify->QueueLength = 0;  //每个客服的队列长度是不同的，推送的时候是增量更新
            //pFriendStatusNotify->status = m_nStatus;

            sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);
            CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"准备发送");

            CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);

        }
    }

    //用memcached直接写入全局服务器中，本地不存队列信息
#if 0

    //如果该好友是直连到本服务器,则直接修改其好友列表状态

    CsQueue * csQueue =  CAppManager::GetInstance().GetCSQueueManager().Find(QueueChange->teamId); //查找分组排队队列
    if(!csQueue)
    {
        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"查询排队组信息失败 :%d！", QueueChange->teamId);
    }
    if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
    {
        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"增加用户到排队队列中 :TEAM %d UID:%u",
                  QueueChange->teamId,QueueChange->idBuyer);
        csQueue->AddToCSQueueList(QueueChange->idBuyer);
    }
    else if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
    {
        CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"将排队用户从队列中移除 (被咨询) :TEAM %d UID:%u",
                  QueueChange->teamId,QueueChange->idBuyer);
        csQueue->DelFromCSQueueList(QueueChange->idBuyer);
    }
#endif


    return RESULT_SUCCESS;

}


int CBusinessGetSubAccount::AddToQueueAndNotify(CUser* User,uint32 buyerId,XT_GET_SUB_ACCOUNT_REQ *req)
{
    //加入排队队列
//  csQueue->AddToCSQueueList(buyerId);
    //在全局加入排队队列
    tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
    uint8 count = 1;
    int ret;
    cSinfo.customerID = buyerId;
    cSinfo.teamID = req->group_id;
    time_t tm;
    time(&tm);
    cSinfo.customerLevel = User->m_nLevel;
    cSinfo.customerReqTime = (uint32)tm;        // 当前排队用户等级
    User->setCsTeamId(req->group_id);           // 设置teamid,避免从memcache中删除时删不掉 sd add

    CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_ADD user %u!",cSinfo.customerID);
    ret =  m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_ADD);
    if(ret != MEMCACHED_GET_INFO_SUCCESS)
    {
        CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_ERROR,"全局加入排队队列用户 %u failed!");
    }
    //通知所有客户端
    //暂且认为可以在内存中获取客服信息
    //后面需要改成从memcached中获取所有子账号信息遍历去查
    //ClientService *clientService = CAppManager::GetInstance().GetCSManager().Find(req->merchant_id);
    // if(clientService)     //找到客服，直接推送
    // {
    XT_WAITQUEU_CHANGE notify;  //不找客服，直接全部推送
    notify.bAdd = XT_WAITQUEU_CHANGE::TYPE_ADD;
    notify.idBuyer = buyerId;
    notify.teamId = req->group_id;
    //clientService->NotifyTeamQueue(&notify);
    NotifyTeamQueueInfo(&notify);
    // }
    // else//没找到本地客服
    //  {
    //      CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "未获取子账号上下文，可能是新登录用户");
    //  }

    return 0;
}

int CBusinessGetSubAccount::DoWork()
{
    //获取接受数据包指针
    XT_GET_SUB_ACCOUNT_REQ *req = (XT_GET_SUB_ACCOUNT_REQ*)m_RecvPacket.GetData();
    uint32 id = 0;
    uint32 QueueLength = 0;     //若有排队，排队的位置
    uint32 RevisedQueueLength = 0;
    shared_ptr<CUser> pUser = NULL;
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    uint32 buyerId = m_pRecvHead->sid;
    pUser = pUserManager->Find(buyerId);


    //获取发送数据包指针
    //XT_GET_SUB_ACCOUNT_ACK *ack = (XT_GET_SUB_ACCOUNT_ACK*)m_SendPacket.GetData();
#if 1
    if(req->type == XT_GET_SUB_ACCOUNT_REQ::CS) //是客服的业务逻辑
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
            CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "当前没有客服在线");
            id = 0;
            QueueLength = 0xffffffff;
            m_SendPacket << (uint32)id
                         << (uint32)QueueLength;
            return RESULT_SUCCESS;
        }
#endif
        if(pUser == NULL)
        {
            CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "没有买家上下文");
            id = 0;
            QueueLength = 0xffffffff;
            m_SendPacket << (uint32)id
                         << (uint32)QueueLength;
            return RESULT_SUCCESS;

        }
        if((id = pUser->getCsuserId()) > 0 && ((uint32)pUser->getCsTeamId() == req->group_id))
        {
            QueueLength = 0;
            CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "已经分流过的用户 %u 到客服 %u",buyerId,id);
        }
        else
        {
            pUser->setCsTeamId(0);
            pUser->setCsuserId(0);
//            pUserManager->AddUserToRedis(pUser);

            /*
                模拟数据


                buyerId = 5000012;
                uint32 team = 100;
                uint32 userlist[]={2000000008,2000000006};
                req->group_id = team;
                req->merchant_id = 2000000008;

                ids.push_back(2000000008);
                ids.push_back(2000000006);


                end     模拟数据

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


            //问题  何时判断可以分流
#ifdef IM_USE_CACHE
            //从全局memcached获取排队队列

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
            //排序
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
            //缓存本地一下，主要是备份用
            CSQueueManager & csQueueManager = CAppManager::GetInstance().GetCSQueueManager();
            csQueue = csQueueManager.Find(req->group_id);
            if(csQueue == NULL)
            {
                CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "创建一个新的排队队列 %u" ,req->group_id);
                csQueue = csQueueManager.AddHash(req->group_id);
                csQueue->setCSID(req->group_id);
            }
#endif


            if(qinfo.m_Customerlist.size() > 0) //在排队，排队队列中有数据
            {
                vector<CSQList_t>::iterator result = find_if(qinfo.m_Customerlist.begin( ), qinfo.m_Customerlist.end(),[buyerId] (const CSQList_t& a) ->bool
                {
                    return (a.userid == (uint32)buyerId);
                }
                                                            ); //查找3
                if(( result != qinfo.m_Customerlist.end() ))    //在队列中找到
                {
                    //返回位置信息
                    //写入数据包
                    if((qinfo.m_Customerlist[0].userid) == buyerId)     //马上可接待，试试可以接待否
                    {
                        int ret = GetCSSubAccount_x(m_pRouter,req->group_id,buyerId,ids,id);
                        if(id != 0)
                        {
                            QueueLength = 0;        //分流成功，不用排队
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
                                CLog::Log("CBusinessGetSubAccount", CLog::TYPE_ERROR,"全局修改组用户: %u 失败!", buyerId);
                            }

                            //从全局memcached删除排队队列

                            tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
                            cSinfo.customerID = buyerId;
                            cSinfo.teamID = req->group_id;

                            CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"1从全局memcached删除排队用户 %u!",cSinfo.customerID);
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
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "队列中第一个就是我们，排队用户 %d 队列长度  %d  OUT", id,QueueLength);
                    }
                    else
                    {
                        QueueLength = result - qinfo.m_Customerlist.begin() + 1;
                        RevisedQueueLength = CalcQueueLength(QueueLength);
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT,
                                  "买家等待中1，排队用户 %u 队列长度  %d 调整为%d OUT", buyerId,QueueLength,RevisedQueueLength);
                        m_SendPacket << (uint32)id
                                     << (uint32)RevisedQueueLength;
                        return RESULT_SUCCESS;
                    }
                    m_SendPacket << (uint32)id
                                 << (uint32)QueueLength;

                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT,
                              "买家等待中3，排队用户 %u 队列长度  %d  OUT", buyerId,QueueLength);
                    return RESULT_SUCCESS;
                }
                else        //队列有排队人员，但是没有此用户
                {
                    QueueLength = qinfo.m_Customerlist.size() + 1;
                    RevisedQueueLength = CalcQueueLength(QueueLength);
                    AddToQueueAndNotify(pUser.get(),buyerId,req);
                    m_SendPacket << (uint32)id
                                 << (uint32)RevisedQueueLength;

                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT,
                              "买家等待中2，排队用户 %u 队列长度  %d 调整为%d OUT", buyerId,QueueLength,RevisedQueueLength);
                    return RESULT_SUCCESS;
                }

            }
            else    //没排队 ，那么开始分流
            {
                int ret = GetCSSubAccount_x(m_pRouter,req->group_id,buyerId,ids,id);
                if(id != 0)
                {
                    pUser = pUserManager->Find(buyerId);
                    if(pUser != NULL)
                    {
                        pUser->setCsuserId(id);
                        pUser->setCsTeamId(req->group_id);
                        QueueLength = 0;        //分流成功，不用排队
                        //m_SendPacket << (uint32)id
                        //      << (uint32)0;
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "买家等待中3，排队用户 %u 队列长度  %d  OUT", buyerId,QueueLength);
                        //return RESULT_SUCCESS;
                        tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID pUTeamID;
                        pUTeamID.id = buyerId;
                        pUTeamID.teamID = req->group_id;
						pUTeamID.csid = id;
                        uint8 count = 1;
                        CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
                        if(RESULT_FAILED == pGlobalBusi->ModiUserTeamID(&pUTeamID,count))
                        {
                            CLog::Log("CBusinessGetSubAccount", CLog::TYPE_ERROR,"全局修改组用户: %u 失败!", buyerId);
                        }
                    }
                    else
                    {
                        id = 0;
                        CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "获取本地买家%u失败 (买家 logout) OUT" ,buyerId);
                    }
                }
                else if(ret == NO_CS_ONLINE)
                {
                    QueueLength = -1;
                    m_SendPacket << (uint32)id
                                 << (uint32)QueueLength;
                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "买家等待中4，排队用户 %u 队列长度  %d  OUT" ,buyerId,QueueLength);
                    return RESULT_SUCCESS;
                }
                else    //没有可接待的用户---第一次开始排队
                {
                    AddToQueueAndNotify(pUser.get(),buyerId,req);
                    QueueLength = 1;
                    m_SendPacket << (uint32)id
                                 << (uint32)QueueLength;
                    CLog::Log("CBusinessGetSubAccount", CLog::TYPE_IMPORTANT, "买家等待中5，排队用户 %u 队列长度  %d  OUT" ,buyerId,QueueLength);
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

        //判断请求类型，选择使用的数据库
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


        //查找最近联系账号
        uint32 ConnAccount = 0;
        std::vector<tagGSRV_GET_USER_FLOW_ACK::tagUserFlow> userFlow(ids.size());
        uint8_t cnt = ids.size();
        if(DB_SUCCESS != pDatabase->SearchConnAccount(m_pRecvHead->sid,ids,ConnAccount))
        {
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Get Flow Mode Error,sid :%d",m_pRecvHead->sid);
        }
        else
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"id:%d ,ConnAccount:%d ",m_pRecvHead->sid,ConnAccount);

        //最近联系账号是否可分流
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
        if(i == cnt)//最近联系账号不可分流
        {
            if(online_num == 0 && req->type != 0)//分组无在线账号可分,在线分流
            {
                ids.clear();
                MYSQL_LIB->getAllSubAccount(req->merchant_id, ids);

                flow_type = 0;//在线分
            }
        }

        if(id == 0)
        {
            //获取商家分流方式
            int flow_mode = -1;
            if(DB_SUCCESS != pDatabase->GetFlowMode(req->merchant_id,flow_mode))
            {
                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"Get Flow Mode Error! merchant_id:%d",req->merchant_id);
                flow_mode = AVERAGE_FLOW;
            }

            //获取最佳分流子账号
            if(flow_mode == AVERAGE_FLOW)
            {
                //均分

                CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"AVERAGE_FLOW mode");
                if(m_pRouter->GetGlobalBusi()->GetBestAccount(req->merchant_id, req->group_id, ids, flow_type, id))
                {
                    CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"merchant_id:%d ,can't find %d's sub account",req->merchant_id,ConnAccount);
                }
            }
            else if(flow_mode == CAPACITY_FLOW)
            {
                //接待能力分
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

                //获取未回复数
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

                    //随机分配
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

    if(id == 0 && (req->type != XT_GET_SUB_ACCOUNT_REQ::CS)) //非客服分流失败，取出代理账号
    {
        CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
        if( DB_SUCCESS != pDatabase->Get_Agent_Merchant(req->merchant_id,id))
        {
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"获取代理商家失败 :%d",id);
            MYSQL_LIB->getMainAccount(req->merchant_id, id);
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"获取主商家ID :%d",id);
        }

        CLog::Log("CBusinessGetSubAccount",CLog::TYPE_IMPORTANT,"获取代理商家:%u",id);
    }

    /*返回子帐号基本信息*/
    char    strEmail[MAX_EMAIL_LEN+1] = "";
    char    strUsername[MAX_USERNAME_LEN+1] = "";   // 用户名
    char    strNickname[MAX_NICKNAME_LEN+1] = "";   // 呢称
    char    strMobile[MAX_PHONE_LEN+1] = "";            // 手机
    char    usersign[MAX_USERSIGN_LEN+1] = "";      //个性签名
    char    face_url[MAX_HEADURL_LEN+1] = "";       //头像信息

    if(id != 0)  //工作台失败是返回-1，哞哞失败返回的是0
    {
        //  CUser* pUser = NULL;
        //      CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();
        pUser = pUserManager->Find(id);
        if( NULL == pUser || (!pUser->m_bIsFullInfoValid) )
        {
            XT_SERVER_USERINFO_GET_ACK UserInfo;
            //说明该用户不在本服务器上,则到数据库中查找
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

    //写入数据包
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

    //回送一个简单的应答给用户
    SendReply();

    //通知框架层不需要再发应答了.内部已经有应答
    CancelReply();

    //下面通过通知下发好友基本资料给用户
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
                // 得到权限 // 大于2000000000表示客服工作台版本中的客服人员，需要查询主管权限给客户端
                m_pFriendInfo[nfindex].LoveFlag = 1-lpAck.m_permission;     // 主管权限通过LoveFlag字段传给客户端
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
                //包装满了,发送出去
                SendNofity();

                //为下一次做准备
                InitNotifyHead();
                AddToNotifyPacket(&FriendPacket);

            }

            FriendPacket.Reset();
        }


    }

    //最后发送一次
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

    //计算个数的位置
    m_pCount  = (uint16*)m_NotifyPacket.GetData();
    m_NotifyPacket << (uint16)0;        //返回的好友资料个数,暂时填充值
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
	CLog::Log("CBusinessGetFriendBasicInfo::SendNofity", CLog::TYPE_IMPORTANT, "好友数 %d", *m_pCount);
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
        m_SendPacket << (uint8)0;       //操作结果
        nClientID = UserInfoReq.fid;
        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT, "GetFriendFullInfo：%d, IN", nClientID);
    }
    else
    {
        pUserInfoAck->ret = 0;
        nClientID = pUserInfoReq->fid;
    }

    pUser = pUserManager->Find(nClientID);
    if (NULL == pUser && nClientID == m_pRecvHead->sid)
        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "发现未登录用户查询自己的详细信息：%d", nClientID);

    if( NULL == pUser ||(!pUser->m_bIsFullInfoValid))
    {
        XT_SERVER_USERINFO_GET_ACK UserInfo;
        //说明该用户不在本服务器上,则到数据库中查找
        CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
#ifndef _USE_MYSQL
        if( DB_SUCCESS != pDatabase->Get_User_Info(nClientID,UserInfo))
#else
        if(DB_SUCCESS != MYSQL_LIB->getUserInfo(nClientID,UserInfo))
#endif
        {
            SetErrorReturn("获取详细资料失败");
            return RESULT_FAILED;
        }

		// 如果是买家，调用http接口获取一下买家信息
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
            //向全局服务器更新该用户的新资料
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
                CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "向全局注册了一个离线用户：%d!", pUser->m_nID);

            CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
            if(RESULT_FAILED == pGlobalBusi->AddModiUserInfo(&GlobalUserInfo,count,NULL,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag))
            {
                CLog::Log("CBusinessGetFriendFullInfo->AddModiUserInfo", CLog::TYPE_ERROR,"全局注册用户: %d 失败!", pUser->m_nID);
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
                CLog::Log("CBusinessGetFriendFullInfo",CLog::TYPE_IMPORTANT,"用户 %d正在删除操作中,等待中 %d!",pUser->m_nID,LogoutCount);
                sleep(1);
            }
            */
            CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
			if(pUser->m_nID < Id_Boundary || pUser->m_nID >= cs_Boundary)
            {	// 获取买家信息时不刷redis
            	if(RESULT_FAILED == pGlobalBusi->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag))
            	{
                	CLog::Log("CBusinessGetFriendFullInfo->AddModiUserInfo", CLog::TYPE_ERROR,"全局注册用户: %d 失败!", pUser->m_nID);
                	pGlobalBusi->AddModiUserInfo(pUser.get(),0,flag);
            	}
			}
#ifdef IM_USE_IMAPI
				//3：客服上线,service_id必填。
				if(pUser->m_nUsertype == XT_LOGIN_REQ::CS)
				{
					imapi csOnlineReq;
					string info;
					csOnlineReq.imapiReq(imapi::CSONLINEREQ,info,nClientID);
				}
#else			

//通过查询数据库来增加本地以及全局服务器的客服信息
            if(pUser->m_nUsertype == XT_LOGIN_REQ::CS)
            {
                ClientService* clientServer = NULL;

                CSManager& csManager = CAppManager::GetInstance().GetCSManager();
                clientServer = csManager.Find(nClientID);
                if(clientServer == NULL)
                {
                    CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "本地没有这个客服的信息 %u!", nClientID);
                }
                else
                {

                    XT_SERVER_CS_GET_ACK CSInfo;

                    CDataBaseMini* pMinibase = m_pRouter->GetDataBaseMini();
                    if( DB_SUCCESS != pMinibase->Get_CS_Info(nClientID,CSInfo))
                    {
                        SetErrorReturn("获取客服信息失败");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "Get_CS_Info %u failed!", nClientID);
                        return RESULT_FAILED;
                    }

                    CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT,
                              "Get_CS_Info %u 用户名%s 用户状态%d 最大接待量 %d 权限 %d!",
                              nClientID,CSInfo.m_CSname,CSInfo.m_State,CSInfo.m_MaxRecConsult,CSInfo.m_permission);
                    if(CSInfo.m_CSname[0] == 0)
                    {
                        SetErrorReturn("账号不存在");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "clientServer %u m_State %d",nClientID,clientServer->m_State);
                        return RESULT_FAILED;
                    }
                    strcpy(clientServer->m_CSname,pUser->m_szUserName);
                    clientServer->m_State = (uint32)CSInfo.m_State;

                    if(clientServer->m_State == 0)
                    {
                        SetErrorReturn("账号已停用");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "clientServer %u m_State %d",nClientID,clientServer->m_State);
                        return RESULT_FAILED;
                    }

                    clientServer->m_MaxRecConsult = CSInfo.m_MaxRecConsult; //查询数据库，获取客服配置needfix duyong
                    clientServer->m_Permission = CSInfo.m_permission;
                    clientServer->m_CurReception = 0;
                    clientServer->m_CurConsult = 0;
                    clientServer->m_Customerlist.clear();
                    clientServer->m_teamMates.clear(); //查询数据库，获取客服配置needfix duyong


                    if( DB_SUCCESS != pMinibase->CS_Get_Group_Friend(nClientID,clientServer->m_teamMates))
                    {
                        SetErrorReturn("获取详细资料失败");
                        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "CS_Get_Group_Friend %u failed!", nClientID);
                        return RESULT_FAILED;
                    }


                    //查询数据库，获取客服配置needfix duyong


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
                        CLog::Log("CBusinessGetFriendFullInfo->AddModiCSInfo", CLog::TYPE_ERROR,"全局注册用户: %d 失败!", pUser->m_nID);
                        pGlobalBusi->AddModiCSInfo(&csInfo,count,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD);
                    }
#endif
                    CLog::Log("CBusinessGetFriendFullInfo->AddModiCSInfo",
                              CLog::TYPE_IMPORTANT,"全局注册客服 : %u 用户名为 %s 用户状态 %d 用户最大接待量 %d  用户当前接待量 %d  用户当前咨询量 %d !",
                              csInfo.m_nID,csInfo.m_Csname,csInfo.m_State,csInfo.m_MaxRecConsult,
                              csInfo.m_CurReception,csInfo.m_CurConsult);

                    for(int i = 0 ; i < (int)clientServer->m_teamMates.size(); i++)
                    {
                        CLog::Log("CBusinessGetFriendFullInfo->AddModiCSInfo", CLog::TYPE_IMPORTANT,"全局注册客服好友信息: %u 当前组 %u 好友 %u!",
                                  csInfo.m_nID,clientServer->m_teamMates[i].teamid,clientServer->m_teamMates[i].userid);

                    }

                    //新客服登录，看一下有没有排队尝试分流
                    {
                        //获取客服所在组，每个组都试试
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
                            groups.push_back(0);    //考虑在0组的排队
                            for(int i = 0 ; i < (int)groups.size(); i++)
                            {
                                CLog::Log("CBusinessGetFriendFullInfo",CLog::TYPE_IMPORTANT,
                                          "新客服上线，尝试主动分流，Query CS %u's in group %u!",
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

        //在服务器上，直接返回内存值
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
        // 得到权限 // 大于2000000000表示客服工作台版本中的客服人员，需要查询主管权限给客户端
        pUser->m_nIntegral = 1-lpAck.m_permission;      // 主管权限通过LoveFlag字段传给客户端
		// 主管权限同步到redis
		if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0))
        {
            CLog::Log("CBusinessGetFriendFullInfo::ReturnUserInfo", CLog::TYPE_ERROR,"客服信息写入redis失败", pUser->m_nID);
        }
        else
        {
        	CLog::Log("CBusinessGetFriendFullInfo::ReturnUserInfo", CLog::TYPE_ERROR,"客服信息写入redis成功", pUser->m_nID);
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
//  项目调整
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

            //向全局更新一下
            if (pUser->m_nStatus == XTREAM_OFFLINE)
                CLog::Log("CBusinessModifyUserInfo", CLog::TYPE_ERROR, "向全局注册了一个离线用户：%d", pUser->m_nID);

            m_pRouter->GetGlobalBusi()->ModiPartUserInfo(UserInfo.id,struPartUserinfo);
        }
        if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
            m_SendPacket << (uint8)RESULT_SUCCESS;
        else
        {
            pUserInfoModAck->ret = RESULT_SUCCESS;
            m_SendPacket.SetCursor(sizeof(XT_USERINFO_MOD_ACK));
        }

        //通知好友
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
const char MyGroupName[] = "我的团队";
#define MYGROUPPID 255
#define MYGROUPID 250


bool SortGroupListMethod(const GROUPEX &x,const GROUPEX &y)
{
    if(x.partentid != y.partentid)
        return x.partentid < y.partentid;
    else
        return x.groupid < y.groupid;
}


// 获取好友列表
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
                        << "获取好友分组失败";
        }
        else
        {
            int len;
            char* error = NULL;
            pGroupGetAck->ret = RESULT_FAILED;
            len = strlen("获取好友分组失败");
            error = (char*)pGroupGetAck + sizeof(uint8);
            strncpy(error,"获取好友分组失败",strlen("获取好友分组失败"));
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
                ///For 客服工作台
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

// 获取好友列表
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

    //从数据库中查询
    CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
    int nReturnNum = nFriendNumInOnPacket;
    if(DB_SUCCESS !=  pDatabase->Get_User_FriendList( pGetFriendListReq->id,
            pGetFriendListReq->start_pos,
            friendInfo,
            nReturnNum,
            m_pRecvHead->flag) )
    {
        SetErrorReturn("数据库繁忙,获取好友列表失败");
        return RESULT_FAILED;
    }

#if 1   //取消在这里查询卖家子账号 duyong
    //查询卖家子账号
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
				SetErrorReturn("数据库接口繁忙，获取好友列表失败");
                return RESULT_FAILED;
			}

        }
        else
        {
            if(DB_SUCCESS != MYSQL_LIB->getAllSubAccount(pGetFriendListReq->merchant_id, pGetFriendListReq->id, GroupAccount))
            {
                SetErrorReturn("数据库繁忙,获取好友列表失败");
                return RESULT_FAILED;
            }
            else
            {
                int num = 0;

                MyGroupAccount::iterator iter;

                int friend_index = nReturnNum;
                for(iter = GroupAccount.begin(); iter != GroupAccount.end(); ++ iter)
                    //获取所有用户子账号遍历
                {
                    //有subaccount用户，就增加组信息
                    //没有subaccount用户，就增加用户信息在尾部
                    for(num = 0; num < nReturnNum; num++)
                    {
                        if(friendInfo[num].friendId == iter->id)
                        {
                            friendInfo[num].groupId = MYGROUP_ID;
                            break;
                        }
                    }

                    if(num == nReturnNum)//没在好友中找到该子账户
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

    //在线状态向全局服务器查询
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
        SetErrorReturn("查询用户状态失败");
        return RESULT_FAILED;
    }
#else
    if( RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(
            idList ,
            userStatus ,
            idcount ) )
    {
        SetErrorReturn("查询用户状态失败");
        return RESULT_FAILED;
    }
#endif
    //CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"ALL %d",idcount);

    //更新用户状态
    for(int j =0; j < idcount; ++j)
    {
        if( friendInfo[j].friendId == userStatus[j].id)
        {
            friendInfo[j].status = userStatus[j].status;

            //  CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"ALL %d ,id %u , state %d",idcount,userStatus[j].id,userStatus[j].status);
        }
        else
        {
            SetErrorReturn("查询用户状态失败");
            return RESULT_FAILED;
        }
    }

    shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(pGetFriendListReq->id, TRUE);
    if( NULL == pUser)
    {
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"发现一个未登陆用户查询好友列表,id:%d",pGetFriendListReq->id);
        return RESULT_FAILED;
    }
#if 0
    else
    {
        FriendInfo* pFriendInfo;
        //pUser->m_FriendList.Clear();
        //加入用户好友列表
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
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"发现一个未登陆用户查询好友列表,id:%d",pGetFriendListReq->id);
        return RESULT_FAILED;
    }

    //组包返回数据
    {
        //FriendInfo* pFriendInfo;
		uint8 nReturnNum = pUser->m_FriendList.GetSize();
        m_SendPacket<<(uint8)RESULT_SUCCESS;    //操作结果
        m_SendPacket<<(uint16)0 ;               //下次发送请求包的起始位置,后面还要根据具体值修改
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
            //检查下一个好友的信息是否还能装载得下
            //if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
            if((i >= GET_FRIEND_NUMBER) || (!m_SendPacket.HasData(SendPacketTemp.GetSize())))
            {
                //超过尺寸了或者到达客户端接收好友数量最大值，退出
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
            //最后一个好友了
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

// 添加好友分组
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

	error = "添加分组失败";
	return false;
}

// 修改好友分组
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

	error = "修改分组失败";
	return false;
}

// 删除好友分组
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

	error = "删除分组失败";
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
            CLog::Log("CBusinessGroupInfoModify",CLog::TYPE_ERROR,"非法action类型,uid:%d, group_id:%d",req.uid,req.group_id);
            SetErrorReturn("非法组操作类型");
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

    if( m_pSearchFriendReq->search_type == XTREAM_SEARCH_ONLINE ) //查找在线用户
    {
        nResult =  FindOnlineUser();
    }
    else if( m_pSearchFriendReq->search_type == XTREAM_SEARCH_PRECISION ) //精确查找
    {
        nResult = FindPrecision() ;
    }
    else
    {
        CLog::Log("CBusinessSearchFriend",CLog::TYPE_ERROR,"非法查询类型");
        nResult = RESULT_FAILED;
    }



    return nResult;

}


int CBusinessSearchFriend::FindOnlineUser()
{
    GSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo FriendInfo[MAX_SEARCH_FRINED_RETURN_NUM];
    int nResult;
    m_SendPacket << (uint8)RESULT_SUCCESS   //操作结果
                 << (uint8)0  //返回用户信息个数,后面要根据具体值修改这个值
                 << (uint16)0    //nextpos
                 << (uint32)0;   //totalnum;

    //向全局服务器查询
    CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();

    m_nReturnCount = (m_pSearchFriendReq->count < sizeof(FriendInfo)/sizeof(FriendInfo[0])) ? m_pSearchFriendReq->count:sizeof(FriendInfo)/sizeof(FriendInfo[0]) ;

    //m_nReturnCount = sizeof(FriendInfo)/sizeof(FriendInfo[0]);

    uint32 nTotalOnlineNum = 0;
    nResult = pGlobalBusi->SearchOnlineUser(FriendInfo,m_nReturnCount,nTotalOnlineNum,m_pSearchFriendReq->startpos);
    CLog::Log("FindOnlineUser",CLog::TYPE_DEBUG,"nResult %d ",nResult);

    if( RESULT_SUCCESS == nResult )
    {
        //返回结果
        for( int i = 0 ; i < m_nReturnCount ; ++i )
        {
            m_SendPacket << FriendInfo[i].clientid
                         << FriendInfo[i].sexy
                         << FriendInfo[i].status
                         << FriendInfo[i].rule
                         << FriendInfo[i].username
                         << FriendInfo[i].nickname;
        }

        //补充上个数
        m_pSearchFriendAck->count = m_nReturnCount;
        m_pSearchFriendAck->nextpos = 0;//该项无效的，仅填充
        m_pSearchFriendAck->nTotalNum = nTotalOnlineNum;
    }
    /*	// 接入服务器不考虑redis挂掉的情况
#ifdef IM_USE_CACHE
    else if(MEMCACHED_SERVER_DOWN == nResult) //memacached 挂掉
    {

        CLog::Log("FindOnlineUser",CLog::TYPE_DEBUG,"memacached 挂掉");
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
            if( i >= m_nReturnCount)  //最多返回m_nReturnCount个用户信息
                break;
        }
        //补充上个数
        m_pSearchFriendAck->count = i;
        m_pSearchFriendAck->nextpos = 0;//该项无效的，仅填充
        m_pSearchFriendAck->nTotalNum = CUser::m_userhash.size();
    }
#endif
	*/
    else
    {
        SetErrorReturn("查询失败");
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


    //有三个查询条件，但一次查询只会使用上其中一个条件
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
            //查询结果用户的在线状态,向全局服务器查询
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
            //返回结果
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
            //查询结果用户的在线状态,向全局服务器查询
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
                //SetErrorReturn("查询用户状态失败");
                //return RESULT_FAILED;

                //查找失败,则置这些结果为离线
                for(j=0; j<m_nReturnCount; ++j)
                {
                    FriendInfo[j].status = XTREAM_OFFLINE;
                }
            }
            else
            {
                //更新用户状态
                for( j =0; j < idcount; ++j)
                {
                    if( FriendInfo[j].id == userStatus[j].id)
                    {
                        FriendInfo[j].status = userStatus[j].status;
                    }
                }
            }
            //返回结果
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
        SetErrorReturn("没有找到相关记录");
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
        //空，该用户已经不在线了，不必继续处理
        CLog::Log("CBusinessFriendModify",CLog::TYPE_DEBUG,"用户%d不在线",m_pFriendGroupReq->uid);
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
            CLog::Log("CBusinessFriendModify",CLog::TYPE_ERROR,"错误action类型:%d, uid:%d",m_pFriendGroupReq->action,m_pFriendGroupReq->uid);
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
    uint8 AllowRule;    //好友验证方式

    FriendInfo* pFriend = m_pUser->m_FriendList.Find(m_pFriendGroupReq->fid);

    //如果该对方用户还没加入好友列表，则需要先添加对方为好友,再修改所属组
    if( NULL == pFriend )
    {
        if(m_pFriendGroupReq->group_id == GROUPID_BLACKLIST)
        {
            //如果是添加到黑名单，则不需要验证
            AllowRule = FRIEND_JOIN_ANYONE;
        }
        else
        {
            //检查对方用户是否在本服务器上
            shared_ptr<CUser> pFriendUser = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->fid);
            if( pFriendUser != NULL)   //找到了
            {
                AllowRule = pFriendUser->m_AllowRule;
            }
            else
            {
                //没找到,则到数据库中查找
                if( RESULT_FAILED ==m_pRouter->GetDataBaseWeb()->Get_User_JoinFlag(m_pFriendGroupReq->fid,AllowRule) )
                {
                    SetErrorReturn("得到好友验证方式失败");
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
                //不需要进一步处理,仅返回验证信息给用户
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
        //没有该好友，不处理
//      SetErrorReturn("修改好友分组失败");
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
            if( pFriendUser != NULL)   //找到了
            {
                AllowRule = pFriendUser->m_AllowRule;
            }
            else
            {
                //没找到,则到数据库中查找
                if( RESULT_FAILED ==m_pRouter->GetDataBaseWeb()->Get_User_JoinFlag(m_pFriendGroupReq->fid,AllowRule) )
                {
                    SetErrorReturn("得到好友验证方式失败");
                    return RESULT_FAILED;
                }
            }
            if (AllowRule == FRIEND_JOIN_ANYONE)
                DoAddFriend();
        }
        else
        {
			// 修改好友分组
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
        if( pUser != NULL) //找到了
        {
            pUser->m_FriendList.Del(m_pFriendGroupReq->uid);
        }

        //给好友通知删除自己
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
	// 调用imapi接口删除好友
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

	SetErrorReturn("删除好友失败");
	return RESULT_FAILED;
}

uint8  CBusinessFriendModify::DoAddFriend()
{	
	// 得到好友状态
	uint8 status = XTREAM_OFFLINE;
	shared_ptr<CUser> pUserFriend = m_pRouter->GetUserManager()->Find(m_pFriendGroupReq->fid);
	if(pUserFriend != NULL)
	{
		status = pUserFriend->m_nStatus;
	}

	// 调用imapi接口添加好友
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

	SetErrorReturn("添加好友失败");
	return RESULT_FAILED;
/*
    //数据库操作
    int nResult = m_pRouter->GetDataBaseWeb()->Add_Friend(
                      m_pFriendGroupReq->uid,
                      m_pFriendGroupReq->fid,
                      m_pFriendGroupReq->group_id) ;
    if( nResult ==  DB_SUCCESS)
    {
        //加该好友到内存列表
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

// 调用接口修改好友分组
uint8  CBusinessFriendModify::DoModFriendGroup(int uid, int friendid, int groupid)
{	
	// 调用imapi接口添加好友
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

	SetErrorReturn("修改好友分组失败");
	return RESULT_FAILED;
}


//通知用户被某个人加为了好友
//BeAddedID --被加方
//ToAddID   --发起添加好友方
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
                        <<"您的好友数已满,不能再添加好友了！";
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


    //如果对方在当前IM服务器,直接更新数据
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


    CLog::Log("CBusinessModifyStatus",CLog::TYPE_DEBUG,"修改用户%u状态 到 %d IN",UserId,pVerifyFriendReq->status);
    if( pUser != NULL )
    {
        uint8 oldState = pUser->m_nStatus;
        pUser->m_nStatus = pVerifyFriendReq->status;

        //通知全局服务器状态更改
        GSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser UserStatus;
        UserStatus.id = UserId;
        UserStatus.status = pVerifyFriendReq->status;
        uint8 count = 1;
        if (pUser->m_nStatus == XTREAM_OFFLINE)
            CLog::Log("CBusinessModifyStatus", CLog::TYPE_ERROR, "向全局注册了一个离线用户：%d", pUser->m_nID);

        if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->ModiUserStatus(&UserStatus,count))
        {
            SetErrorReturn("网络繁忙,更改状态失败");
        }
        else
        {
            //通知该用户的在线好友状态更改
            pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());

            //通知家族更改状态
            pUser->NotifyFamilyStatus(&(CAppManager::GetInstance().GetAppNetWork()),m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

            pVerifyFriendAck->ret = RESULT_SUCCESS;
            pVerifyFriendAck->status = pVerifyFriendReq->status;

            m_SendPacket.SetCursor(sizeof(XT_STATUS_MOD_ACK));

            if(pUser->m_nUsertype == XT_LOGIN_REQ::CS)  //客服
            {
                struct tm newtime;
                time_t aclock;
                time( &aclock );
                localtime_s(&newtime, &aclock );
                using namespace LogInterface;
                using namespace std;
                if(pUser->m_nStatus == XTREAM_LEAVE)        //进入挂起状态
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
                if(oldState == XTREAM_LEAVE)        //从挂起状态退出
                {

                    //上传登录动作

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
    CLog::Log("CBusinessFriendModify",CLog::TYPE_DEBUG,"修改用户%u状态 到 %d OUT",UserId,pVerifyFriendReq->status);

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
        SendWebMoreMsgAck(m_SendPacket);//获取失败也通知
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
    CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"更新客服信息 %u has %u !",clientService->m_nID);
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
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT,"当前组 %u  有用户 %u",
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

    //服务器收到的消息的时间
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

    //接收为web设备
    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),m_pRecvHead->did) == TYPE_WEB)
        msg.to_terminal = XT_TALK::WEB_TERMINAL;

    //if(m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ || m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)
    //文字消息、评价等
    if(msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::XT_RECOMENT || msg.data_type == XT_MSG::AUTO_REPLY)
    {
        m_RecvPacket>>msg.fontSize
                    >>msg.fontColor
                    >>msg.fontStyle
                    >>msg.fontName;

        m_RecvPacket.ReadData(msg.data,sizeof(msg.data));
        m_RecvPacket>>msg.from_nickname;

        //只存web?
        if( (msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::XT_RECOMENT)
            /*&& (m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ || m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)*/
          )
        {
            CLog::Log("WebMsgSave", CLog::TYPE_DEBUG, "%s %s", msg.data, msg.from_nickname);
#if 0
//压测加入时间
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
            //这里不存，另开了一个线程来存
            uint32 logintime =0;
            CUser::m_UserListLock.Lock();
            CUser *pUser = NULL;//CUser::m_pUserListHead;
            //int m_nID = (xtTalkMsg.from_id>xtTalkMsg.to_id)?xtTalkMsg.from_id:xtTalkMsg.to_id;
            //存买家时间
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


    //文字和图片消息，另起线程保存
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
		

		//更新买家redis 超时时间
		if(m_nID == msg.from_id)
        {
	        if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->setExpireUserinfo(m_nID))
	        {
	            CLog::Log("SaveWebMsg", CLog::TYPE_ERROR,"更新用户 %u 超时时间失败!",m_nID);
	        }
			else
			{
				CLog::Log("SaveWebMsg", CLog::TYPE_IMPORTANT,"更新用户 %u 超时时间!", m_nID);
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

    //转发该消息
    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),m_pRecvHead->did) == TYPE_WEB &&
       (pP2PMsgSend->type >= XT_MSG::SF_FILE_REQ && pP2PMsgSend->type <=XT_MSG::SF_PACKET_ACK)
      )
    {
        //图片消息记录的不传输给web端，走图片服务器

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

    //web端发送消息，更新最后发送时间到定时器中
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
            CLog::Log("SendMsg1",CLog::TYPE_STATISTIC,"更新web用户%u最后发送时间%u",
                      pUser->m_nID,pUser->m_nNewlyLiveTime);

            // 更新网络层内存
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
    // 判断消息类型，如果为插入会话消息，则要将消息转给客服和客户
    XT_SERVER_P2PMSG_SEND* pP2PMsgSend  = (XT_SERVER_P2PMSG_SEND*)m_RecvPacket.GetData();
    if(pP2PMsgSend->type == XT_MSG::IM_MSG_INSERT)
    {
        // 判断聊天对象中是否存在客服，并且正在被监控，如果正在被监控，将消息转发给主管
        bool bExistMaster = CUser::g_cs_exist(pP2PMsgSend->Sender, m_pRouter->GetGlobalBusi());        // 主管
        bool bExistCS = CUser::g_cs_exist(pP2PMsgSend->Recver, m_pRouter->GetGlobalBusi());            // 客服
        if(bExistMaster && bExistCS)
        {
            // 检验
            // 判断客服是否被监控
            bool monitored = false;
            uint32 cs_master_id = (uint32)-1;
            bool success = CUser::g_cs_ismonitored(pP2PMsgSend->Recver, monitored, cs_master_id, m_pRouter->GetGlobalBusi());
            if(success && monitored && cs_master_id == pP2PMsgSend->Sender)
            {
                uint32 nBuyerID = (uint32)-1;       // 买家id			// 插入会话中最后四个字节为买家id
				BYTE *pBuyerID = (BYTE*)m_RecvPacket.GetData() + m_RecvPacket.GetSize() - sizeof(uint32);
                nBuyerID = *((uint32*)pBuyerID);
                {
                    CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"check success！master:%u, cs:%u, buyer:%u",
                              pP2PMsgSend->Sender,  pP2PMsgSend->Recver, nBuyerID);

                    // 构建转发消息
                    // 转发给客服
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

                    CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"send to cs！");


                    // 转发给买家
                    // 买家登录方式
                    //uint8 buyer_login_flag = 0;
                    // 本地查找买家
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
                    // 修改消息类型为0
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

                    CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"send to buyer！");

                    return;
                }
            }
        }

        CLog::Log("CBusinessSendMessageINSERT",CLog::TYPE_DEBUG,"check failed！master:%u, cs:%u", pP2PMsgSend->Sender,  pP2PMsgSend->Recver);
        return;
    }

    //判断是卖家发给买家的消息，而且买家处于未接待状态，则将卖家的信息置位
#ifndef IM_USE_IMAPI
    if(pP2PMsgSend->Sender > cs_Boundary && m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),pP2PMsgSend->Recver) == TYPE_WEB && pP2PMsgSend->type == 0)//先判断接收方是买家而不是客服
    {
        //uint32 team_id;
        ClientService * clientService = NULL;
        CSManager & csManager = CAppManager::GetInstance().GetCSManager();
        clientService = csManager.Find(pP2PMsgSend->Sender);
        if(clientService)
        {
            //ver字段设为teamID字段
            clientService->ModifyCustomerState_x(pP2PMsgSend->Recver);
            CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        }
        else
        {
			CsCustOper(m_pRouter,UO_ADD_RECPTION,pP2PMsgSend->Sender,pP2PMsgSend->Recver);			
            CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,"该客服并不在本服务器中:%d，发送到客服所在服务器处理！",pP2PMsgSend->Sender);
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

    CLog::Log("CBusinessServerMsg",CLog::TYPE_IMPORTANT,"用户 %u  发送消息到 %u ",pP2PMsgSend->Sender,to_id);

    uint32_t merchantID = 0;

    if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),to_id) == TYPE_WEB)//赋值商家ID信息
    {

        MYSQL_LIB->getMainMerchant(pP2PMsgSend->Sender,merchantID);
        CLog::Log("CBusinessServerMsg",CLog::TYPE_IMPORTANT,"获取卖家 %u 的商家ID %u",pP2PMsgSend->Sender,merchantID);
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
	//接收方所有在线用户都接收
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
    // 判断聊天对象中是否存在客服，并且正在被监控，如果正在被监控，将消息转发给主管
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
            // 聊天双方存在商家
            CLog::Log("CBusinessSendMessageIND",CLog::TYPE_DEBUG,"exist CS");
            uint32 cs_id = src_is_cs ? nFromID : to_id;
            uint32 cust_id = src_is_cs ? to_id : nFromID;

            // 判断是否正在被监控
            bool monitored = false;
            uint32 cs_master_id = (uint32)-1;
            bool success = CUser::g_cs_ismonitored(cs_id, monitored, cs_master_id, m_pRouter->GetGlobalBusi());
            if(success && monitored)
            {
                CLog::Log("CBusinessSendMessageIND",CLog::TYPE_DEBUG,"is Monitoring");

                // 查看主管是否在本台服务器上，不在，消息转发给其他服务器
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
                    //0.客服发送消息；1.客服收到消息；
                    uint8  action = cs_id != nFromID;

                    // 构造转发消息结构体
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

    //服务器自动回复
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

    //调换卖家和买家
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

   //调换卖家和买家  多设备发往其他发送端

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
		{	// 自营客服请求买家状态
			CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_DEBUG,"自营客服请求买家状态 userid:%u friend:%u",m_pRecvHead->sid,pStatusGetAck->id);
			vector<uint32> vecCustID;
			if(m_pRouter->GetGlobalBusi()->GetCusterList(m_pRecvHead->sid,vecCustID))
			{	// 获取咨询列表成功
				CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_IMPORTANT,"获取咨询列表成功 userid:%u",m_pRecvHead->sid);
				auto it = std::find(vecCustID.begin(), vecCustID.end(), pStatusGetAck->id);
				if(it == vecCustID.end())
				{	// 未在咨询列表中，将状态改为离线
					CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_IMPORTANT,"买家未在咨询列表中 userid:%u friend:%u",m_pRecvHead->sid, pStatusGetAck->id);
					pStatusGetAck->status = XTREAM_OFFLINE;
				}
				else
				{	// 在咨询列表中
					CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_IMPORTANT,"买家在咨询列表中 userid:%u friend:%u",m_pRecvHead->sid, pStatusGetAck->id);
					pStatusGetAck->status = XTREAM_ONLINE;
				}
			}
			else
			{
				CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_ERROR,"获取排队列表失败 userid:%u",m_pRecvHead->sid);
			}
		}
        
        CLog::Log("CBusinessGetLinkStatus::DoWork()",CLog::TYPE_DEBUG,"userid:%u friend:%u status:%d",m_pRecvHead->sid, pStatusGetAck->id,pStatusGetAck->status);
    }
    else    //该好友不在线
    {

        pStatusGetAck->ret  = RESULT_SUCCESS;
        pStatusGetAck->id   = pStatusGetReq->fid;
        pStatusGetAck->ip   = 0;        //清0表示不在线
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
//用户行为统计分析
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

//修改个人签名
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
                    <<"您的账户余额不足！";

    }
    else
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<"操作失败，请稍后再试！";


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


    m_SendPacket << (uint8)0;       //成功标志
    m_SendPacket << (uint8)0;       //返回个数,后面根据实际值重新填充
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
            //说明该用户不在线
            m_SendPacket << nClientID
                         << (uint8)XTREAM_OFFLINE    //离线
                         //下面是无意义的值,仅填充作用
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

        //检查下一个用户的信息是否还能装载得下
        if( !m_SendPacket.HasData(sizeof(pAckHead->user[0])) )
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"超过尺寸了,退出");
            //超过尺寸了,退出
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

    m_pSendData->retry_times = 0;   //应答
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //构建网络层的信息
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //构建协议头的信息
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherUser:SendReply",CLog::TYPE_IMPORTANT,"发送请求:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //发送数据
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


    m_SendPacket << (uint8)0;       //成功标志
    m_SendPacket << (uint8)0;       //返回个数,后面根据实际值重新填充
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
            //说明该用户不在线
            m_SendPacket << nClientID
                         << (uint8)XTREAM_OFFLINE;//离线
        }
        else
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_IMPORTANT,"HAVE USER INFO:%u", nClientID);
            m_SendPacket << nClientID
                         << pUser->m_nStatus ;
        }

        //检查下一个用户的信息是否还能装载得下
        if( !m_SendPacket.HasData(sizeof(pAckHead->user[0])) )
        {

            CLog::Log("CBusinessGetOtherUser",CLog::TYPE_ERROR,"超过尺寸了,退出");
            //超过尺寸了,退出
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

    m_pSendData->retry_times = 0;   //应答
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //构建网络层的信息
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //构建协议头的信息
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherUserStatus:SendReply",CLog::TYPE_IMPORTANT,"发送请求:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //发送数据
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

    m_pSendData->retry_times = 0;   //应答
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //构建网络层的信息
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //构建协议头的信息
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherTUser:SendReply",CLog::TYPE_IMPORTANT,"发送请求:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //发送数据
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

    m_SendPacket << (uint8)0;           //成功标志
    m_SendPacket << (uint8)0;           //返回个数,后面根据实际值重新填充

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
            //说明该用户不在线
            m_SendPacket << nClientID
                         << (uint32)0;   //离线

            //后面几个值都是没意义的,仅起填充作用
            m_SendPacket.WritePacket((const char*)"7788414",sizeof(pUser->m_SessionKey));
            m_SendPacket.WritePacket((const char*)"7788414",sizeof(pUser->m_SessionKey));
            //收到广播的请求，没有这个数据，直接取消返回
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

        //检查下一个用户的信息是否还能装载得下
        if( !m_SendPacket.HasData(nOneUserInfoSize) )
        {
            //超过尺寸了,退出
            break;
        }

    }

    pAckHead->count = i;

    return true;
}
//10002到请求的源端口即可
int CBusinessGetOtherCtrlUser::SendReply()
{

    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;   //应答
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //构建网络层的信息
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //构建协议头的信息
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessGetOtherCtrlUser:SendReply",CLog::TYPE_IMPORTANT,"发送请求:ip %u port %d vlink %d",
              pRecvData->src_ip,pRecvData->port, pRecvData->vlink);

    //发送数据
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
    //修改账号客服账号密码
}
int CBusinessModifyCSPWD::DoWork()
{
    //char username[100];
    //char pwd[100];
    XT_MOD_PSW_REQ modifyPwd;
    //接受到需要修改的哞哞登录名
    m_RecvPacket>>modifyPwd.uid
                >>modifyPwd.szPsw
                >>modifyPwd.random_key;
    //将修改的账号密码加密写入IM_USER表
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


/*********************************************设置客服接待的买家***************************************************/
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

    // 根据id查找客服
    bool is_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!is_cs)
    {
        // 未查找到客服
        CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // 得到当前正在聊天的买家
    uint32 chatting_buyer_id = (uint32)-1;
    bool success = CUser::g_cs_get_chatting_buyer(req.idSeller,chatting_buyer_id,m_pRouter->GetGlobalBusi());
    if(success && chatting_buyer_id == req.idBuyer)
    {
        CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "重复设置");
        m_SendPacket<<(uint8)RESULT_SUCCESS;
        return RESULT_SUCCESS;
    }

    // 设置客服正在聊天的买家
    CUser::g_cs_set_chatting_buyer(req.idSeller,req.idBuyer,m_pRouter->GetGlobalBusi());

    // 判断客服是否正在被监管
    CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "check monitor");
    bool monitored = false;
    uint32 cs_master_id = (uint32)-1;
    success = CUser::g_cs_ismonitored(req.idSeller,monitored,cs_master_id,m_pRouter->GetGlobalBusi());
    if(success && monitored)
    {
        // 被监控,发送系统中转消息
        CLog::Log("CCSerChatWithBuyer", CLog::TYPE_DEBUG, "monitoring");
        SendChatBuyerChanged(cs_master_id, req.idSeller, req.idBuyer);
    }

    // 发送回复
    m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

// 系统中转消息，通知主管他监控的客服改变了正在聊天的买家
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


/***************************************************主管监控功能****************************************************/
// 开始监控
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

    // 根据id查找客服
    bool exist_master = CUser::g_cs_exist(req.idMaster,m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller,m_pRouter->GetGlobalBusi());
    if(!exist_master || !exist_cs)
    {
        // 未查找到客服
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // 判断权限是否允许
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUserMaster = pUserManager->Find(req.idMaster);
	if(pUserMaster == NULL)
	{	// redis中获取主管全量信息失败
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "not in the server %d",req.idMaster);
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
	}
    if(pUserMaster->m_nIntegral == 0)
    {
        // 不为主管
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "not master");
        m_SendPacket<<(uint8)RESULT_FAILED;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // 获取客服正在接待的买家
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

    // 获取客服正在被谁监控
    bool monitored = false;
    uint32 cs_master_id;
    CUser::g_cs_ismonitored(req.idSeller, monitored, cs_master_id, m_pRouter->GetGlobalBusi());
    if(monitored && cs_master_id != req.idMaster)
    {
        CLog::Log("CDirectorMonitorStart", CLog::TYPE_DEBUG, "客服已经被其他主管监控");
        m_SendPacket<<(uint8)2;
        m_SendPacket<<req.idSeller;
   		m_SendPacket<<(uint32)0;
        return RESULT_FAILED;
    }

    // 开始监控
    CUser::g_cs_set_director(req.idSeller,req.idMaster,m_pRouter->GetGlobalBusi());

    // 构造回复结构体
    m_SendPacket<<(uint8)RESULT_SUCCESS;
    m_SendPacket<<req.idSeller;
    m_SendPacket<<nChattingBuyerID;

    return RESULT_SUCCESS;
}


/***************************************************结束监控功能****************************************************/
// 结束监控
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

    // 根据id查找客服
    bool exist_master = CUser::g_cs_exist(req.idMaster, m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!exist_master)
    {
        // 未查找到客服
        CLog::Log("CDirectorMonitorStop", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // 取消监控
    CLog::Log("CDirectorMonitorStop", CLog::TYPE_DEBUG, "stop success");
    if(exist_cs)
    {
        CUser::g_cs_set_director(req.idSeller,(uint32)-1,m_pRouter->GetGlobalBusi());
    }

    // 构造回复结构体
    m_SendPacket<<(uint8)RESULT_SUCCESS;
    return RESULT_SUCCESS;
}


/***************************************************主管插入会话****************************************************/
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

    // 根据id查找客服
    bool exist_master = CUser::g_cs_exist(req.idMaster, m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!exist_master || !exist_cs)
    {
        CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // 判断客服正在被监控
    bool monitored = false;
    uint32 cs_master_id = (uint32)-1;
    bool success = CUser::g_cs_ismonitored(req.idSeller,monitored,cs_master_id,m_pRouter->GetGlobalBusi());
    if(!success || !monitored)
    {
        CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "not be Monitored");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // 判断监控客服的主管是否正确
    if(cs_master_id != req.idMaster)
    {
        CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "not same monitor");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    m_SendPacket<<(uint8)RESULT_SUCCESS;

    CLog::Log("CMonitorInsertChat", CLog::TYPE_DEBUG, "Length : %d", sizeof(XT_HEAD)  + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_MONITOR_INSERT_CHAT_IND));

    // 消息中转，告之客服和买家插入会话开始
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

    // 告之买家
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


/***************************************************主管接手会话****************************************************/
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

    // 根据id查找客服
    bool exist_master = CUser::g_cs_exist(req.idMaster, m_pRouter->GetGlobalBusi());
    bool exist_cs = CUser::g_cs_exist(req.idSeller, m_pRouter->GetGlobalBusi());
    if(!exist_master || !exist_cs)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "not find");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // 判断客服正在被监控
    bool monitored = false;
    uint32 cs_master_id = (uint32)-1;
    bool success = CUser::g_cs_ismonitored(req.idSeller,monitored,cs_master_id,m_pRouter->GetGlobalBusi());
    if(!success || !monitored)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "not be Monitored");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }

    // 判断监控客服的主管是否正确
    if(cs_master_id != req.idMaster)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "not same monitor");
        m_SendPacket<<(uint8)RESULT_FAILED;
        return RESULT_FAILED;
    }


    // 买家id
    uint32 buyerID =  (uint32)-1;
    CUser::g_cs_get_chatting_buyer(req.idSeller,buyerID,m_pRouter->GetGlobalBusi());
    // 本地查找买家
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    int userteamid = -1;
    shared_ptr<CUser> pUser = pUserManager->Find(buyerID);
    if(pUser == NULL)
    {
        // 本地中不存在，在全局中查找
        uint32 lsid[1] = {buyerID};
        tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userinfo[1];
        uint8 count = 1;

        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserInfo(lsid,count,userinfo))
        {
            // 全局中查找买家失败，表示买家未登陆
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
    // 从客服中删除买家
    ClientService * clientService = NULL;
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    clientService = csManager.Find(req.idSeller);
    if(clientService != NULL)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "客服%s清空买家%u", clientService->m_CSname, req.idBuyer);
        clientService->RemoveCustomer_x(req.idBuyer);

        /*修改用户信息到memcached中*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
    }
    else
    {
        CsCustOper(m_pRouter, UO_DEL_CUST, req.idSeller, req.idBuyer);
    }
#endif
    // 修改买家上下文
    if(pUser != NULL)
    {
        pUser->m_nCSuserid = req.idMaster;

        CLog::Log("LoginSuccess", CLog::TYPE_IMPORTANT, "向全局更新一个WEB用户：%d 状态 %d", pUser->m_nID,pUser->m_nStatus);
        m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0);
    }
    else
    {
        // 如果买家不在此服务器，通告其他服务器来修改
        //查询全局服务器并清除web所在服务器其接待上下文
        tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
        uint8 count = 1;
        uint32 custid = req.idBuyer;
        m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&custid,&UserInfo,count);

        if(UserInfo.uid != custid)
        {
            CLog::Log("CMonitorTranChat",CLog::TYPE_IMPORTANT,
                      "查询买家全局信息失败! %u", custid);
        }
        else
        {
            userteamid = UserInfo.teamID;
            SetWebContext(m_pRouter, custid, req.idMaster, userteamid);
        }
    }
#ifndef IM_USE_IMAPI
    // 修复主管上下文
    clientService = csManager.Find(req.idMaster);
    if(clientService != NULL)
    {
        CLog::Log("CMonitorTranChat", CLog::TYPE_DEBUG, "客服%s添加买家%u", clientService->m_CSname, req.idBuyer);
        clientService->AddCustomer_x(userteamid, req.idBuyer);

        /*修改用户信息到memcached中*/
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
    }
    else
    {
        CsCustOper(m_pRouter, UO_ADD_CUST, req.idMaster, req.idBuyer);
    }
#endif
    // 消息中转，告之客服和买家主管接手了会话
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

    // 告知买家
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
    jvSellerInfo["switchStatus"] = 2;           // 1 表示转接开始 0 表示转接成功 2 表示主管接手会话
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

    //推送用户所在组的所有排队用户信息
    if(userteamid != -1)
    {
	#ifdef IM_USE_IMAPI
		// 9. 接手会话，ServiceId和customerId,group_id必填，toServiceID和toServiceGroup 是要转接到的客服的ID和组ID
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

    CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "请求id %u 请求个数 %d ",WebLastMsg.id,WebLastMsg.msgnum);
    ids.clear();

    //取最近联系账号以及消息
    if(m_pRouter->GetDataBaseMini()->WebLastMsgReq(&WebLastMsg,user_msgs,nRetCount) == DB_ERROR)
    {
        CLog::Log("WebLastMsgReq", CLog::TYPE_ERROR, "WebLastMsgReq Req Failed! sid:%d ",WebLastMsg.id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebLastMsg.id
                    <<WebLastMsg.msgnum;
        SendWebMoreLastMsgAck(m_SendPacket);//获取失败也通知
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

        SendWebMoreLastMsgAck(m_SendPacket);//获取失败也通知
        return RESULT_SUCCESS;
    }

//  vector<MerchantandSubAccount> merchantInfo;

    for(int i = 0 ; i < (int)nRetCount; i++)
    {
        //struct MerchantandSubAccount tmp_account;

        MYSQL_LIB->getMainMerchant((uint32_t)user_msgs[i].subAccountID, (uint32_t &)user_msgs[i].merchantID);

        CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "user_msgs[i].subAccountID %u 主账号 %u  消息 %s",
                  user_msgs[i].subAccountID,user_msgs[i].merchantID,user_msgs[i].lastMsg);
    }

    //取每个联系账号的主账号
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

    uint32 correctCount = 0;        //返回用户总数
    for(int i = 0 ; i < (int)nRetCount; i++)
    {
        if((user_msgs[i].merchantID != 0xffffffff) && (user_msgs[i].merchantID != 0))
        {

            pMsgInfo[correctCount].merchantID = user_msgs[i].merchantID;
            pMsgInfo[correctCount].subAccountID = user_msgs[i].subAccountID;
            pMsgInfo[correctCount].lastMsgTime = user_msgs[i].lastMsgTime;
            strcpy(pMsgInfo[correctCount].nickname,user_msgs[i].nickname);
            strcpy(pMsgInfo[correctCount].lastMsg,user_msgs[i].lastMsg);

            CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "正确的卖家 %u 发送时间%u 子账号 %u 消息  %s",
                      pMsgInfo[correctCount].merchantID,
                      pMsgInfo[correctCount].lastMsgTime,
                      pMsgInfo[correctCount].subAccountID,
                      pMsgInfo[correctCount].lastMsg);

            correctCount++;
        }
        CLog::Log("WebLastMsgReq", CLog::TYPE_IMPORTANT, "所有的卖家 %u 发送时间%u 子账号 %u 消息	%s",
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
        SendWebMoreLastMsgAck(m_SendPacket);//获取失败也通知
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
            len = tmp_len + strlen(pMsgInfo[i].lastMsg) + strlen(pMsgInfo[i].nickname) + sizeof(uint16)*2 + 2; 	// 写入字符串流时会先写入字符串长度
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

    CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"Id:%d 收到清空web客服用户%u的请求 auth %x IN",lpReq->id,lpReq->authData);

    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(lpReq->id);
    if(pUser == NULL)
    {
        CLog::Log("CBusinessClearWebClient", CLog::TYPE_IMPORTANT, "当前服务器没有此用户%u" ,lpReq->id);
        return 0;
    }
    switch(lpReq->authData)
    {
        case 0xBB:  //清除web接待信息
            pUser->ClearWebClient();
            break;
        case 0xCC:  //设置用户接待
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

/***************************************************获取消息提醒****************************************************/
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

    // 去数据库中查找
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


/***************************************************设置消息提醒****************************************************/
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

    // 写入数据库
    MsgRemindAck ack;
    m_pRouter->GetDataBaseMini()->SetMsgRemind(m_pRecvHead->sid, req);
    m_SendPacket<<(uint8)RESULT_SUCCESS;

    return RESULT_SUCCESS;
}


/***************************************************获取离线消息推送数据****************************************************/
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

    // 去数据库中查找离线消息推送数据
    vector<string> arrUUID;
    m_pRouter->GetDataBaseMini()->GetMsgNotifyDataOnePerson(req.userid(), arrUUID);
    /*for(int i = 0; i < (int)arrUUID.size(); ++i)
    {
    	// 发送
    	CLog::Log("CGetOfflineMsgNotify", CLog::TYPE_IMPORTANT, "推送 uuid:%s, userid:%u", arrUUID[i].c_str(), req.userid());
    	Buf_ClientSend ack;
        ack.set_idxpacketreq(0);
        ack.set_bufunitidx(arrUUID[i].c_str());
        ack.set_buflocation(LOCATION_DB);
        CThreadMsgNotify::SendToSeller(m_pRouter->GetDataBaseMini(), req.userid(), ack);
    }*/
    return RESULT_SUCCESS;
}



/***************************************************请求未接收完的buf****************************************************/
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

    // 去数据库中查找离线消息推送数据
    CLog::Log("CGetRemainBuf", CLog::TYPE_IMPORTANT, "发送剩余的buf. uuid[%s] idx[%d]", req.bufunitidx().c_str(), req.idxpacketreq());
    CThreadMsgNotify::SendToSeller(m_pRouter->GetGlobalBusi(), m_pRouter->GetDataBaseMini(), m_pRecvHead->sid, req);
    return RESULT_SUCCESS;
}



#if 0
void CBusinessSetCSClient::updateCsQueue(ClientService *clientService)
{
    CLog::Log("CBusinessCSQueue",CLog::TYPE_IMPORTANT,"更新客服信息 %u has %u !",clientService->m_nID);
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
            CLog::Log("CBusinessCSQueue", CLog::TYPE_IMPORTANT,"当前组 %u  有用户 %u",
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

    CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"CS:%d 收到清空客服接待的web用户%u 组%u的请求 IN",
              lpReq->clientId,lpReq->csId,lpReq->teamId);
#ifndef IM_USE_IMAPI
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    ClientService * clientService  = csManager.Find(lpReq->csId);
    if(clientService)
    {
        clientService->RemoveCustomer_x(lpReq->clientId);
        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"CS:%d 收到清空客服接待的web用户%u 组%u的请求 OUT",
                  lpReq->clientId,lpReq->csId,lpReq->teamId);
    }
    else
    {
        CLog::Log("CBusinessClearWebClient",CLog::TYPE_IMPORTANT,"CS:%d 收到清空客服接待的web用户%u 组%u的请求,但是不在本服务器上 OUT",
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
        // 反序列化失败
        CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_ERROR,"反序列化失败");
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
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%u被取消监控", clientService->m_nID);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%u被主管%u监控", clientService->m_nID, req.custid());
        	}
        	clientService->SetDirectorID(req.custid());
			m_pRouter->GetGlobalBusi()->UpdateCsMonitor(clientService.get());
			//CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
        else if(req.updateoper() == UO_SET_CHATTING_CUST)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%u取消正在聊天的买家", clientService->m_nID);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%u设置正在聊天的买家%u", clientService->m_nID, req.custid());
        	}
        	clientService->SetChattingBuyerID(req.custid());
			m_pRouter->GetGlobalBusi()->UpdateCsMonitor(clientService.get());
			//CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
    }
    else
    {
        CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_ERROR, "客服不在本服务器", req.csid());
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
        // 反序列化失败
        CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_ERROR,"反序列化失败");
        return 0;
    }

	CSManager & csManager = CAppManager::GetInstance().GetCSManager();
	ClientService * clientService = NULL;
	clientService = csManager.Find(req.csid());

    if(clientService != NULL)
    {
        if(req.updateoper() == UO_DEL_CUST)
        {
            CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%s清空买家%u", clientService->m_CSname, req.custid());
            clientService->RemoveCustomer_x(req.custid());

            /*修改用户信息到memcached中*/
            CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        }
        else if(req.updateoper() == UO_ADD_CUST)
        {
            CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%s添加买家%u", clientService->m_CSname, req.custid());

            //查询全局服务器并清除web所在服务器其接待上下文
            tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
            uint8 count = 1;
            uint32 custid = req.custid();
            m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&custid,&UserInfo,count);

            if(UserInfo.uid != custid)
            {
                CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_IMPORTANT,
                          "查询买家全局信息失败! %u", custid);
            }
            else
            {
                clientService->AddCustomer_x(UserInfo.teamID, req.custid());

                /*修改用户信息到memcached中*/
                CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
            }
        }
		else if(req.updateoper() == UO_ADD_RECPTION)
		{
			//ver字段设为teamID字段
			clientService->ModifyCustomerState_x(req.custid());
			CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
			CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_IMPORTANT,"CS:%d 增加客服接待的web用户%u 组",
              req.csid(),req.custid());
		}		
		else if(req.updateoper() == UO_DEL_RECPTION)
		{
			clientService->RemoveCustomer_x(req.custid());
	        CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
	        CLog::Log("CBusinessUpdateCsReq",CLog::TYPE_IMPORTANT,"CS:%d 清空客服接待的web用户%u 组",
                  req.csid(),req.custid());
		}
        else if(req.updateoper() == UO_SET_MASTER)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%s被取消监控", clientService->m_CSname);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%s被主管%u监控", clientService->m_CSname, req.custid());
        	}
        	clientService->SetDirectorID(req.custid());
			CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
        else if(req.updateoper() == UO_SET_CHATTING_CUST)
        {
        	if(req.custid() == (uint32)-1 || req.custid() == 0)
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%s取消正在聊天的买家", clientService->m_CSname);
        	}	
        	else
        	{
        		CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_DEBUG, "客服%s设置正在聊天的买家%u", clientService->m_CSname, req.custid());
        	}
        	clientService->SetChattingBuyerID(req.custid());
			CMemcacheUpdateConsultCSInfo(m_pRouter, clientService);
        }
    }
    else
    {
        CLog::Log("CBusinessUpdateCsReq", CLog::TYPE_ERROR, "客服不在本服务器", req.csid());
    }

    return 0;
}

#endif

//获取客服所在组信息
/*
#define CMD_CS_TEAM_INFO_REQ			0x305
#define CMD_CS_TEAM_INFO_ACK			0x306
*/
//推送客服所在组排队数信息
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
        // 反序列化失败
        CLog::Log("CBusinessCsTeamInfoReq",CLog::TYPE_ERROR,"反序列化失败");
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
			// 并通知相关客服
        	vector<uint32> TeamUser;
        	CDataBaseMini *pMinibase = m_pRouter->GetDataBaseMini();
        	if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(csQlen[i].teamId,TeamUser))
        	{
            	CLog::Log("CBusinessCsTeamInfoReq", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", csQlen[i].teamId);
            	continue;
        	}
			if(std::find(TeamUser.begin(),TeamUser.end(),(uint32)userID) == TeamUser.end()) 
			{	// 用户不参与分流
				CLog::Log("CBusinessCsTeamInfoReq", CLog::TYPE_IMPORTANT, "客服[%u]不参与分组[%u]的分流", (uint32)userID, csQlen[i].teamId);
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


//通知客服数据异动
CBusinessCSQueueInfo::CBusinessCSQueueInfo(CRouterBusiness* pRouter):
	CBusinessExIMSrv(pRouter,CMD_CS_TEAM_QINFO_REQ,CMD_CS_TEAM_QINFO_ACK)
{


}
	
int CBusinessCSQueueInfo::NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange)	  //通知组内其他成员排队状况 0 包括自己 1 不包括自己
{
	CDataBaseMini* pMinibase = m_pRouter->GetDataBaseMini();
	vector<uint32> TeamUser;

	if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(QueueChange->teamId,TeamUser))
	{
		SetErrorReturn("获取详细资料失败");
		CLog::Log("NotifyTeamQueueInfo", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", QueueChange->teamId);
		return RESULT_FAILED;
	}
	for(unsigned int i = 0 ; i < TeamUser.size(); ++i )
	{

		CLog::Log("NotifyTeamQueueInfo", CLog::TYPE_IMPORTANT,"内部队列:TEAM %d 查询队列: TEAM %d ,发送给 %u",
				  QueueChange->teamId,QueueChange->teamId,TeamUser[i]);
		//通知所有组内的客服
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
			CLog::Log("NotifyTeamQueueInfo", CLog::TYPE_IMPORTANT,"准备发送");

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


//通知客服数据异动
CBusinessCloseReceptionByCustomer::CBusinessCloseReceptionByCustomer(CRouterBusiness* pRouter):
	CBusinessExIMSrv(pRouter,CMD_BUYER_QUIT_REQ,CMD_BUYER_QUIT_ACK)
{


}
	
int CBusinessCloseReceptionByCustomer::NotifyBuyerQuitInfo(XT_BUYERINFO_CHANGE* infoChange)	  //通知组内其他成员排队状况 0 包括自己 1 不包括自己
{
/*
    // 通知此买家的客服，买家下线
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
		// 修改买家状态为离线
		tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser info;
    	info.id = infoChange->idBuyer;
		info.status = XTREAM_OFFLINE;
		uint8 count = 1;
		m_pRouter->GetGlobalBusi()->ModiUserStatus(&info,count,NULL);

		// 设置客服正在聊天的买家
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
            CLog::Log("CBusinessCloseReceptionByCustomer", CLog::TYPE_IMPORTANT, "发送买家退出消息时序列号失败");
        }
        else
        {
            memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT), szData, msg_size);
            sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + msg_size;

            lpSendHead->len      = sizeof(XT_SERVER_TRANSMIT) + msg_size;

            CLog::Log("CBusinessCloseReceptionByCustomer", 
				CLog::TYPE_IMPORTANT,"发送买家[%u]退出消息给客服[%u]",
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



//通知客服数据异动
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

//向IMAPI获取离线消息
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

//向客户端发送离线消息
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
/*后面的逻辑上应该不需要，暂时保留日后使用*/


CBusinessModiOtherUser::CBusinessModiOtherUser(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_MOD_USER_OTHER_QUERY,CMDGSRV_MOD_USER_OTHER_ACK)
{


}

int CBusinessModiOtherUser::DoWork()
{

    CLog::Log("CBusinessModiOtherUser",CLog::TYPE_ERROR,"CMDGSRV_GET_USER_OTHER_QUERY");

    CUser* pNewUser = NULL;
    nReloginCount = 0;

    //检查是什么请求类型
    uint8 action;
    uint8 nUserCount;
    m_RecvPacket>>action;
    m_RecvPacket>>nUserCount;

    m_SendPacket << (uint8)V_SUCCESS    //操作结果
                 << (uint8) 0 ;       //返回个数，后面会再次修改该值

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
                CLog::Log("COnlineManager",CLog::TYPE_ERROR,"用户ID越界,可能包数据错误");
                return false;
            }


            //检查原来是否已经存在该用户
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
                    CLog::Log("CBusinessModiOtherUser",CLog::TYPE_ERROR,"内存分配失败");
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
            //设置新用户的信息
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
                CLog::Log("AddModifyOnlineUser", CLog::TYPE_ERROR, "向全局注册了一个离线用户：%d", nClientID);

            //检查是否重登陆
            if( !bIsNewUser && action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD )
            {
                if (pNewUser->m_ServerIp != NewServerIP && pNewUser->m_ip == OldUserIp && pNewUser->m_port == OldUserPort) //need del by lwl ver:0101
                    CLog::Log("COnlineManager::AddModifyOnlineUser", CLog::TYPE_ERROR, "使用了相同的IP、Port登录不同的服务器");
                if( pNewUser->m_ServerIp != NewServerIP || (pNewUser->m_ip != OldUserIp) || (pNewUser->m_LanIp != OldLanIp))
                {
                    //添加一个已经存在的用户，说明有重登录情况发生，
                    //这时要通知旧的IM服务器,所以这里记录下来
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
            //空间存储已满
            break;
        }


        //返回设置成功的用户数
        GSRV_ADDMODIFY_FULLUSERINFO_ACK* pHead = (GSRV_ADDMODIFY_FULLUSERINFO_ACK*)m_SendPacket.GetData();
        pHead->count = count ;
    }
    else
    {
        CLog::Log("COnlineManager::AddModifyOnlineUser",CLog::TYPE_ERROR,"收到一个未知action的协议包:%d",action);
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

    m_pSendData->retry_times = 0;   //应答
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //构建网络层的信息
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //构建协议头的信息
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessModiOtherUser:SendReply",CLog::TYPE_ERROR,"发送请求:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //发送数据
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


    m_SendPacket << (uint8)0;       //成功标志
    m_SendPacket << (uint8)0;       //返回个数,后面根据实际值重新填充

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
            //说明该用户不在线
            m_SendPacket << nClientID
                         << (uint8)XTREAM_OFFLINE    //离线
                         //下面是无意义的值,仅填充作用
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

        //检查下一个用户的信息是否还能装载得下
        if( !m_SendPacket.HasData(sizeof(pAckHead->user[0])) )
        {
            //超过尺寸了,退出
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

    m_pSendData->retry_times = 0;   //应答
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //构建网络层的信息
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


    //构建协议头的信息
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();
    CLog::Log("CBusinessDelOtherUser:SendReply",CLog::TYPE_ERROR,"发送请求:ip %u port %d vlink %d",
              pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

    //发送数据
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
