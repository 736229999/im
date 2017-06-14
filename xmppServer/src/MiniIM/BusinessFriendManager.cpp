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
#include "improtocol.pb.h"
#include <set>
#include <curl/curl.h>
#include "httpclient.h"



#define MAX_QUERY_BASIC_INFO_COUNT 200

extern map<uint32,sess_t> sessManager;

static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
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

    //�ӿ�֧��id���飬Ŀǰֻ��һ��id������
    m_pRouter->GetGlobalBusi()->ModiUserFlow(&userFlowModi[0], cnt, &userFlowModiAck[0]);

    m_SendPacket<<userFlowModiAck[0].id
                <<userFlowModiAck[0].status
                <<userFlowModiAck[0].flow;

    CLog::Log("CBusinessModHangUp", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, id:%u, status:%u, flow:%u OUT" ,
              m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag,
              userFlowModiAck[0].id,
              userFlowModiAck[0].status, userFlowModiAck[0].flow);
    if(req->uUserId < Id_Boundary)
    {
        uint32 merchant_id = 0;
        CUser::m_UserListLock.Lock();
        CUser *pUser = NULL;
        __gnu_cxx::hash_map<uint32,CUser*>::iterator iter = CUser::m_userhash.find(req->uUserId);
        if(iter != CUser::m_userhash.end())
        {
            pUser = iter->second;
            merchant_id = pUser->m_merchantid;
        }
        CUser::m_UserListLock.Unlock();
        //�ϴ�����Ӵ�����
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(merchant_id,req->uUserId);
        time_t tm;
        time(&tm);
        try
        {
            SubMerchantAction info;
            info.merchantID = merchant_id;
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
    std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(req->cnt);
    uint8_t cnt = req->cnt;

    //��ȡ�û�״̬��Ϣ
    int ret = m_pRouter->GetGlobalBusi()->QueryUserStatus(req->id, &userStatus[0], cnt);

    //��ȡʧ��
    if (ret == RESULT_FAILED)
    {
        return RESULT_FAILED;
    }

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

int CBusinessGetSubAccount::DoWork()
{
    //��ȡ�������ݰ�ָ��
    XT_GET_SUB_ACCOUNT_REQ *req = (XT_GET_SUB_ACCOUNT_REQ*)m_RecvPacket.GetData();

    //��ȡ�������ݰ�ָ��
    //XT_GET_SUB_ACCOUNT_ACK *ack = (XT_GET_SUB_ACCOUNT_ACK*)m_SendPacket.GetData();

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

    uint32 id = 0;
    //���������ϵ�˺�
    uint32 ConnAccount = 0;
    std::vector<tagGSRV_GET_USER_FLOW_ACK::tagUserFlow> userFlow(ids.size());
    uint8_t cnt = ids.size();
    if(DB_SUCCESS != pDatabase->SearchConnAccount(m_pRecvHead->sid,ids,ConnAccount))
    {
        CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Get Flow Mode Error,sid :%d",m_pRecvHead->sid);
    }
    else
        CLog::Log("SearchConnAccount",CLog::TYPE_IMPORTANT,"id:%d ,ConnAccount:%d ",m_pRecvHead->sid,ConnAccount);

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
            CLog::Log("CBusinessGetSubAccount",CLog::TYPE_ERROR,"Get Flow Mode Error! merchant_id:%d",req->merchant_id);
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

    /*�������ʺŻ�����Ϣ*/
    char    strEmail[MAX_EMAIL_LEN+1] = "";
    char    strUsername[MAX_USERNAME_LEN+1] = "";   // �û���
    char    strNickname[MAX_NICKNAME_LEN+1] = "";   // �س�
    char    strMobile[MAX_PHONE_LEN+1] = "";            // �ֻ�
    char    usersign[MAX_USERSIGN_LEN+1] = "";      //����ǩ��


    if(id == 0) //����ʧ�ܣ�ȡ�������˺�
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

    if(id != 0)
    {
        CUser* pUser = NULL;
        CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();
        pUser = UserManager.Find(id);
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

                CLog::Log("CBusinessGetSubAccount1",
                          CLog::TYPE_DEBUG,
                          "get %d's sub account,id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s",
                          req->merchant_id, id , strEmail,strUsername, strMobile,strNickname,usersign);
            }

        }
        else
        {
            strcpy(strEmail, pUser->m_szEmail);
            strcpy(strUsername, pUser->m_szUserName);
            strcpy(strMobile, pUser->m_szPhone);
            strcpy(strNickname, pUser->m_szNickName);
            strcpy(usersign, pUser->m_szUserSign);

            CLog::Log("CBusinessGetSubAccount2",
                      CLog::TYPE_DEBUG,
                      "get %d's sub account,id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s",
                      req->merchant_id, id , strEmail,strUsername, strMobile,strNickname,usersign);
        }
    }
    else
    {
        CLog::Log("CBusinessGetSubAccount3",
                  CLog::TYPE_DEBUG,
                  "get %d's sub account,id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s",
                  req->merchant_id, id , strEmail,strUsername, strMobile,strNickname,usersign);
    }

    //д�����ݰ�
    m_SendPacket << (uint32)id
                 << strEmail
                 << strUsername
                 << strMobile
                 << strNickname
                 << usersign
                 << req->merchant_id;   // �ƶ�����Ҫ����merchant add by sd

    if(sessManager.find(m_pRecvHead->sid) != sessManager.end())
    {
        // ��xmpp�ͻ��˵�¼
        CLog::Log("CBusinessGetSubAccount::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", m_SendPacket.GetSize()+ sizeof(XT_HEAD));
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_SendPacket.GetSize()+ sizeof(XT_HEAD));
        CancelReply();
    }
    else
    {
        CLog::Log("CBusinessGetSubAccount::DoWork",CLog::TYPE_IMPORTANT,"not find sess");
    }

    CLog::Log("GetSubAccount", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, id:%u, email:%s, username:%s, mobile:%s, nickname:%s,usersign %s OUT" ,
              m_pRecvHead->sid, m_pRecvHead->did, m_pRecvHead->flag, id, strEmail, strUsername, strMobile, strNickname,usersign);
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

    BYTE FriendInfoBuf[sizeof(XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo)];
    CSendPacket FriendPacket((char*)FriendInfoBuf);

    InitNotifyHead();

    CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
    uint16 NextReturnPos = 0 ;
    uint16 nRetCount = 0;
    uint32 QueryId = pReq->type == pReq->TYPE_FRIEND ? m_pRecvHead->sid : 0;

    if (pReq->count > 300  )
        return RESULT_FAILED;

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
            FriendPacket<< m_pFriendInfo[nfindex].nID
                        << m_pFriendInfo[nfindex].szHeadUrl
                        << m_pFriendInfo[nfindex].nSexy
                        << m_pFriendInfo[nfindex].szNickName
                        << m_pFriendInfo[nfindex].szUserName
                        << m_pFriendInfo[nfindex].szCommentName
                        << m_pFriendInfo[nfindex].LoveFlag
                        << m_pFriendInfo[nfindex].szUserSign
                        << m_pFriendInfo[nfindex].mobileStatus
                        << m_pFriendInfo[nfindex].szGuid;
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
    if( 0 !=  *m_pCount)
    {
        //LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pNotifyData->data;

        m_pNotifyData->data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_NotifyPacket.GetSize());

        CAppManager::GetInstance().GetAppNetWork().SendDataTo(
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

    //CUser* pUser = CAppManager::GetInstance().GetUserManager().Find();

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
    XT_USERINFO_GET_REQ* pUserInfoReq = (XT_USERINFO_GET_REQ*)m_RecvPacket.GetData();
    XT_SERVER_USERINFO_GET_ACK* pUserInfoAck = (XT_SERVER_USERINFO_GET_ACK*)m_SendPacket.GetData();

    uint32  nClientID;
    CUser* pUser = NULL;
    CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();

    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        m_SendPacket << (uint8)0;       //�������
        m_RecvPacket>>nClientID;
        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_IMPORTANT, "GetFriendFullInfo��%d, IN", nClientID);
    }
    else
    {
        pUserInfoAck->ret = 0;
        nClientID = pUserInfoReq->fid;
    }

    pUser = UserManager.Find(nClientID);
    if (NULL == pUser && nClientID == m_pRecvHead->sid)
        CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "����δ��¼�û���ѯ�Լ�����ϸ��Ϣ��%d", nClientID);

    if( NULL == pUser ||(!pUser->m_bIsFullInfoValid) )
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

            if(sessManager.find(m_pRecvHead->sid) != sessManager.end())
            {
                CLog::Log("CBusinessGetFriendFullInfo::DoWork",CLog::TYPE_IMPORTANT,"��ȡ��ϸ����ʧ��, send to xmpp, length:%d", m_SendPacket.GetSize()+ sizeof(XT_HEAD));
                CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_SendPacket.GetSize()+ sizeof(XT_HEAD));
                CancelReply();
            }

            return RESULT_FAILED;
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

                if(sessManager.find(m_pRecvHead->sid) != sessManager.end())
                {
                    // ��xmpp�ͻ��˵�¼
                    CLog::Log("CBusinessGetFriendFullInfo::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", m_SendPacket.GetSize()+ sizeof(XT_HEAD));
                    CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_SendPacket.GetSize()+ sizeof(XT_HEAD));

                    CancelReply();
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
mobile:%s, description:%s, usersign:%s, avatoruri:%s, mobilestatus:%d, integral:%d, OUT",
                      nClientID, UserInfo.username, UserInfo.nickname, UserInfo.email, UserInfo.birthday, UserInfo.sex, UserInfo.province,
                      UserInfo.country, UserInfo.city, UserInfo.face_url, UserInfo.career, UserInfo.rule, UserInfo.familyflag, UserInfo.gold_money,
                      UserInfo.online_time, UserInfo.linkpop, UserInfo.address, UserInfo.mobile, UserInfo.description, UserInfo.usersign, UserInfo.avatoruri,
                      UserInfo.mobilestatus, UserInfo.integral );
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

            pUser->m_bIsFullInfoValid = true;

            //��ȫ�ַ��������¸��û���������
            uint8 flag = m_pRecvHead->flag;
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
            //GlobalUserInfo.lanport    = pUser->m_nLanPort;

            if (pUser->m_nStatus == XTREAM_OFFLINE)
                CLog::Log("CBusinessGetFriendFullInfo", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d!", pUser->m_nID);

            CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
            if(RESULT_FAILED == pGlobalBusi->AddModiUserInfo(&GlobalUserInfo,count,NULL,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag))
            {
                CLog::Log("CBusinessGetFriendFullInfo->AddModiUserInfo", CLog::TYPE_ERROR,"ȫ��ע���û�: %d ʧ��!", pUser->m_nID);
                pGlobalBusi->AddModiUserInfo(&GlobalUserInfo,count,NULL,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,flag);
            }

            ReturnUserInfo(pUser, pUserInfoAck);
        }

    }
    else
    {

        //�ڷ������ϣ�ֱ�ӷ����ڴ�ֵ
        ReturnUserInfo(pUser, pUserInfoAck);
    }


    return RESULT_SUCCESS;
}

void CBusinessGetFriendFullInfo::ReturnUserInfo(CUser* pUser, XT_SERVER_USERINFO_GET_ACK* pUserInfoAck)
{
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

        if(sessManager.find(m_pRecvHead->sid) != sessManager.end())
        {
            // ��xmpp�ͻ��˵�¼
            CLog::Log("CBusinessGetFriendFullInfo::ReturnUserInfo",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", m_SendPacket.GetSize() + sizeof(XT_HEAD));
            CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_SendPacket.GetSize() + sizeof(XT_HEAD));

            CancelReply();
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
descrption:%s, usersign:%s, avatoruri:%s, mobilestatus:%d, intergral:%d, OUT",
              pUser->m_nID, pUser->m_szUserName, pUser->m_szNickName, pUser->m_szEmail, pUser->m_szBirthday,pUser->m_nSexy,
              pUser->m_nProvince,pUser->m_nCountry, pUser->m_nCity, pUser->m_szHeadURL, pUser->m_nCareer, pUser->m_AllowRule,
              pUser->m_FamilyFlag,pUser->m_nMoney, pUser->m_nOnlineTime, pUser->m_linkpop, pUser->m_szAddress, pUser->m_szPhone,
              pUser->m_szDescription, pUser->m_szUserSign,pUser->m_szAvatoruri, pUser->m_nMobileStatus, pUser->m_nIntegral);
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

        uint8 flag = m_pRecvHead->flag;
        CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(UserInfo.id );
        if( pUser )
        {
            strcpy(pUser->m_szEmail,UserInfo.email);
            if (strcmp(UserInfo.nickname, "") !=0)
                strcpy(pUser->m_szNickName,UserInfo.nickname);
            strcpy(pUser->m_szBirthday,UserInfo.birthday);
            pUser->m_nSexy      = UserInfo.sex;
            pUser->m_nProvince  = UserInfo.province;
            pUser->m_nCountry   = UserInfo.country;
            pUser->m_nCity      = UserInfo.city;
            pUser->m_nCareer    = UserInfo.career;
            pUser->m_AllowRule  = UserInfo.rule;
            pUser->m_linkpop    = UserInfo.linkpop;
            strcpy(pUser->m_szAddress,UserInfo.address);
            strcpy(pUser->m_szPhone,UserInfo.mobile);
            strcpy(pUser->m_szDescription,UserInfo.description);
            strcpy(pUser->m_szHeadURL, UserInfo.face_url);


            //��ȫ�ָ���һ��
            if (pUser->m_nStatus == XTREAM_OFFLINE)
                CLog::Log("CBusinessModifyUserInfo", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d", pUser->m_nID);

            m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser,tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,flag);
        }
        if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
            m_SendPacket << (uint8)RESULT_SUCCESS;
        else
        {
            pUserInfoModAck->ret = RESULT_SUCCESS;
            m_SendPacket.SetCursor(sizeof(XT_USERINFO_MOD_ACK));
        }

        //֪ͨ����
        pUser->NotifyFriendInfo();
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

    MD5_CTX_IM md5T;
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
    uint32 BusinessID, iCommCount, i;
    char Account[MAX_CUST_ACCOUNT_LEN];
    char Time[MAX_CUST_TIME_LEN];
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
    CLog::Log("CBusinessModifyCustRemark", CLog::TYPE_IMPORTANT, "id:%d, CommAccount:%s, IN",
              req.uiBusinessID, req.szCommAccount);
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
    uint32 BusinessID, MaxCustCount, nRetCount;
    uint32 initCount = 0;
    uint32 i = 0;
    uint16 Start_pos, CustCount;
    uint8 Direct;
    char BTime[MAX_CUST_TIME_LEN];
    char ETime[MAX_CUST_TIME_LEN];

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

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(pData->data,pData->data_len);

}

//---------------------------------------------------------------------
const char MyGroupName[] = "�ҵ��Ŷ�";
#define MYGROUPPID 255
#define MYGROUPID 250
CBusinessGetGroupInfo::CBusinessGetGroupInfo(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GROUPINFO_GET_REQ,CMD_GROUPINFO_GET_ACK)
{

}


int CBusinessGetGroupInfo::DoWork()
{
    XT_GROUPINFO_GET_REQ* pGroupGetReq = (XT_GROUPINFO_GET_REQ*)m_RecvPacket.GetData();
    XT_GROUPINFO_GET_ACK* pGroupGetAck = (XT_GROUPINFO_GET_ACK*)m_SendPacket.GetData();

    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();
    if( 0 == pGroupGetReq->group_count)
    {
        pGroupGetReq->group_count = MAX_GROUP_NUM;
    }
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
            m_SendPacket << (uint8)RESULT_SUCCESS
                         << pGroupGetReq->group_count;
            for(int i = 0 ; i < pGroupGetReq->group_count; ++i)
            {
                m_SendPacket << m_GroupInfo[i].nGroupPid
                             << m_GroupInfo[i].nGroupID
                             << m_GroupInfo[i].szGroupName;
            }
            if(m_pRecvHead->flag == XT_HEAD::FLAG_SELLCNTREQ)
            {
                m_SendPacket<<(uint8)MYGROUPPID
                            <<(uint8)MYGROUPID
                            <<MyGroupName;
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



int CBusinessGetFriendList::DoWork()
{
    MyGroupAccount GroupAccount;
    XT_FRIENDLIST_REQ* pGetFriendListReq = (XT_FRIENDLIST_REQ*)m_RecvPacket.GetData();
    XT_FRIENDLIST_ACK* pGetFriendListAck = (XT_FRIENDLIST_ACK*)m_SendPacket.GetData();

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
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"Get_User_FriendList Error! id:%d",pGetFriendListReq->id);
        SetErrorReturn("���ݿⷱæ,��ȡ�����б�ʧ��");
        return RESULT_FAILED;
    }

    CLog::Log("CBusinessGetFriendList",CLog::TYPE_IMPORTANT,"merchant_id:%d, id:%d, start_pos:%d, IN",
              pGetFriendListReq->merchant_id,pGetFriendListReq->id,pGetFriendListReq->start_pos);
#if 1//ȡ���������ѯ�������˺� duyong
    //��ѯ�������˺�
    if(m_pRecvHead->flag == XT_HEAD::FLAG_SELLCNTREQ && pGetFriendListReq->merchant_id != 0)
    {
        if(DB_SUCCESS != MYSQL_LIB->getAllSubAccount(pGetFriendListReq->merchant_id, pGetFriendListReq->id, GroupAccount))
        {
            CLog::Log("CBusinessGetFriendList",CLog::TYPE_ERROR,"getAllSubAccount Error! id:%d",pGetFriendListReq->id);
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
#endif
    for (int i = 0 ; i < nReturnNum ; i++)
    {
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_DEBUG,"TOTAL USER:%d, userid:%d, groupid:%d",nReturnNum,friendInfo[i].friendId,friendInfo[i].groupId);
    }


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
    if( RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(
            idList ,
            userStatus ,
            idcount ) )
    {
        SetErrorReturn("��ѯ�û�״̬ʧ��");
        return RESULT_FAILED;
    }

    //�����û�״̬
    for(int j =0; j < idcount; ++j)
    {
        if( friendInfo[j].friendId == userStatus[j].id)
        {
            friendInfo[j].status = userStatus[j].status;
        }
        else
        {
            SetErrorReturn("��ѯ�û�״̬ʧ��");
            return RESULT_FAILED;
        }
    }

    CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(pGetFriendListReq->id);
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
    //�����������
    {
        FriendInfo* pFriendInfo;

        m_SendPacket<<(uint8)RESULT_SUCCESS;    //�������
        m_SendPacket<<(uint16)0 ;               //�´η������������ʼλ��,���滹Ҫ���ݾ���ֵ�޸�
        m_SendPacket<<(uint8)nReturnNum;

        char FriendInfoTempBuf[sizeof(friendInfo[0])];
        CSendPacket SendPacketTemp(FriendInfoTempBuf);

        //bool bIsLastFriend = false;
        int i;
        for( i = 0 ; i <  nReturnNum ; ++i )
        {
            SendPacketTemp<<friendInfo[i].friendId
                          <<friendInfo[i].groupId
                          <<friendInfo[i].status;

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
                pFriendInfo = pUser->m_FriendList.Add(friendInfo[i].friendId);

                if( pFriendInfo )
                {
                    pFriendInfo->id     = friendInfo[i].friendId;
                    pFriendInfo->status = friendInfo[i].status;
                    pFriendInfo->GroupID = friendInfo[i].groupId;
                }

                m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
                CLog::Log("CBusinessGetFriendList",CLog::TYPE_DEBUG,"ReturnNum:%d, friendId:%d, groupId:%d, status:%d, OUT",
                          nReturnNum, friendInfo[i].friendId, friendInfo[i].groupId, friendInfo[i].status);
                SendPacketTemp.Reset();
            }
        }

        int nNextPos =  pGetFriendListReq->start_pos + i;
        if( i >= nReturnNum)
        {
            //���һ��������
            nNextPos = 0xffff;
            pUser->NotifyFriendStatus();
        }
        pGetFriendListAck->next_pos = nNextPos;
        CLog::Log("CBusinessGetFriendList",CLog::TYPE_IMPORTANT,"ReturnNum:%d, OUT",
                  nReturnNum);

    }

    return RESULT_SUCCESS;
}


//---------------------------------------------------------------------

CBusinessGroupInfoModify::CBusinessGroupInfoModify(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_GROUPINFO_MOD_REQ,CMD_GROUPINFO_MOD_ACK)
{

}


int CBusinessGroupInfoModify::DoWork()
{
    //XT_GROUPINFO_MOD_REQ* pGroupInfoModReq = (XT_GROUPINFO_MOD_REQ*)m_RecvPacket.GetData();
    //XT_GROUPINFO_MOD_ACK* pGroupInfoModAck = (XT_GROUPINFO_MOD_ACK*)m_SendPacket.GetData();

    CDataBaseMini* pDatabase = m_pRouter->GetDataBaseMini();
    int nResult;

    XT_GROUPINFO_MOD_REQ req;
    m_RecvPacket >> req.uid
                 >> req.action
                 >> req.group_pid
                 >> req.group_id
                 >> req.name;
    switch(req.action)
    {
        case GROUP_ADD:
            //���ݿ����
            nResult = pDatabase->Add_Group(req.uid,
                                           req.group_id,
                                           req.name,
                                           req.group_pid);
            break;
        case GROUP_MODIFY:
            nResult = pDatabase->Modify_Group(req.uid,
                                              req.group_id,
                                              req.name);
            break;
        case GROUP_DEL:
            nResult = pDatabase->Delete_Group(req.uid,
                                              req.group_id);
            break;
        default:
            CLog::Log("CBusinessGroupInfoModify",CLog::TYPE_ERROR,"�Ƿ�action����,uid:%d, group_id:%d",req.uid,req.group_id);
            SetErrorReturn("�Ƿ����������");
            nResult = RESULT_FAILED;

            return nResult;
    }



    if(  DB_SUCCESS != nResult)
    {
        char szError[MAX_ERROR_BUF];
        CError::DB_Error(nResult,szError,true);
        SetErrorReturn(szError);
    }

    m_SendPacket << (uint8)RESULT_SUCCESS
                 << req.action
                 << req.group_pid
                 << req.group_id
                 << req.name;


    return RESULT_SUCCESS;
}


//---------------------------------------------------------------------

#define MAX_SEARCH_FRINED_RETURN_NUM 20

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


    m_pUser = CAppManager::GetInstance().GetUserManager().Find(m_pFriendGroupReq->uid);

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
            nResult = DelFriendGroup();
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
            CUser* pFriendUser = CAppManager::GetInstance().GetUserManager().Find(m_pFriendGroupReq->fid);
            if( pFriendUser )   //�ҵ���
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
            CUser* pFriendUser = CAppManager::GetInstance().GetUserManager().Find(m_pFriendGroupReq->fid);
            if( pFriendUser )   //�ҵ���
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
            //���ݿ����
            CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
            if( DB_SUCCESS == pDatabase->Modify_Friend_Group(
                    m_pFriendGroupReq->uid,
                    m_pFriendGroupReq->fid,
                    m_pFriendGroupReq->group_id) )
            {
                if (pFriend)
                    pFriend->GroupID = m_pFriendGroupReq->group_id;
            }
            else
            {
                SetErrorReturn("���������ݿⷱæ����ʧ��");
                return RESULT_FAILED;
            }
        }
    }

    m_pFriendGroupAck->flag = AllowRule;
    if (m_pFriendGroupReq->action == XT_FRIENDGROUP_MOD_REQ::ACTION_BLACK)
    {
        m_pRouter->GetDataBaseWeb()->Delete_Friend(m_pFriendGroupReq->fid, m_pFriendGroupReq->uid);
        CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(m_pFriendGroupReq->fid);
        if( pUser ) //�ҵ���
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

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}


uint8   CBusinessFriendModify::DelFriendGroup()
{
//  FriendInfo* pFriend = m_pUser->m_FriendList.Find(m_pFriendGroupReq->fid);
//
//  if( NULL == pFriend )
//  {
//      //û�иú��ѣ�������
//      SetErrorReturn("�����ڸú���ID");
//      return RESULT_FAILED;
//  }

    //���ݿ����
    CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
    int nDBResult = pDatabase->Delete_Friend(m_pFriendGroupReq->uid,m_pFriendGroupReq->fid);
    if( DB_SUCCESS == nDBResult )
    {
        //���ڴ��б���ɾ��
        m_pUser->m_FriendList.Del(m_pFriendGroupReq->fid);
    }
    else
    {
        char szBuf[MAX_ERROR_BUF];
        CError::DB_Error(nDBResult, szBuf, true);
        SetErrorReturn(szBuf);

        return RESULT_FAILED;
    }


    return RESULT_SUCCESS;
}

uint8  CBusinessFriendModify::DoAddFriend()
{
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

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(
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

    CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(m_pRecvHead->sid);
    if( pUser )
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
        CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(to_id);
        if( pUser )
        {
            CBusinessServerMsg ServerMsg(m_pRouter);
            RecvDataStruct      recvData;
            memcpy(recvData.data,sendData.data,sendData.data_len);
            recvData.data_len = sendData.data_len;
            ServerMsg.SetRowPacket((BYTE*)&recvData,recvData.data_len);
            ServerMsg.DoWork();
        }
    }




    CAppManager::GetInstance().GetAppNetWork().SendDataTo(
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
    CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(UserId);
    if( pUser)
    {
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
            pUser->NotifyFriendStatus();

            //֪ͨ�������״̬
            pUser->NotifyFamilyStatus(&(CAppManager::GetInstance().GetAppNetWork()),m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

            pVerifyFriendAck->ret = RESULT_SUCCESS;
            pVerifyFriendAck->status = pVerifyFriendReq->status;

            m_SendPacket.SetCursor(sizeof(XT_STATUS_MOD_ACK));

        }


    }

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
    CLog::Log("CBusinessWebBusiList",CLog::TYPE_IMPORTANT,"QueryUseriD:%d, msgnum:%d,IN",WebBusiList.id,WebBusiList.msgnum);

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

        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(id_list,UStatus,count))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"QueryUserStatus error! id:%d ",WebBusiList.id);
        }
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

    if(sessManager.find(m_pRecvHead->sid) != sessManager.end())
    {
        // ��xmpp�ͻ��˵�¼
        CLog::Log("CBusinessWebBusiList",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", pData->data_len);
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)pData->data, pData->data_len);

        CancelReply();
    }
    else
    {
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(pData->data,pData->data_len,SEND_RETRY_TIME);
    }
}


#define MAX_LIST_NUM 20
CBusinessXmppBusiList::CBusinessXmppBusiList(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_XMPP_BUSI_LIST_REQ,CMD_XMPP_BUSI_LIST_ACK)
{
    pListInfo = new XT_XMPP_BUSI_LIST_ACK::BusiList[MAX_LIST_NUM];
    pData = new SendDataStruct;
    if(pListInfo == NULL || pData == NULL)
        CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"New failed!");
}

CBusinessXmppBusiList::~CBusinessXmppBusiList()
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
//��ʱ�併������
bool SortXmppTimeMethod(const XT_XMPP_BUSI_LIST_ACK::BusiList &x,const XT_XMPP_BUSI_LIST_ACK::BusiList &y)
{
    if(x.time != y.time)
    {
    	return x.time>y.time;
    }
}
bool CBusinessXmppBusiList::getMerchantInfo(uint32 merchantid,char *storename)
{
	  //Ĭ��ʹ���û�����½
    std::string sPostParam ;
   	if(merchantid == 0)
   	{
   		return false;
	}
	/*
	
	http://storeservice.beta.fn.com/merchantStore/getMerchantStore/71
	
	*/
	CHttpClient getRequest;
	string data;
	sPostParam += CAppManager::GetInstance().GetConfig().GetStoreNameURL();
//	sPostParam += "http://storeservice.beta.fn.com/merchantStore/getMerchantStore/";
	char str_id[32]  = {0};
	sprintf(str_id,"%u",merchantid);	
	sPostParam += str_id;	
	int res = getRequest.Get(sPostParam,data);


#if 0	
    	
    //ͨ��http�ӿڻ�ȡ�����Ϣ
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL,sPostParam.c_str());
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    //curl_easy_setopt(curl, CURLOPT_POST, 1);
    //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);    
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    CURLcode res = curl_easy_perform(curl);

    //�����ӿ�
    curl_easy_cleanup(curl);
#endif

    //�жϻ�ȡ�Ƿ�ɹ�
    if (res != CURLE_OK)
    {
        CLog::Log("getMerchantInfo",CLog::TYPE_ERROR,"Get buyer infomation error!");
        return false;
    }


	    //����json
    Json::Reader reader;
    Json::Value recv;
    if (reader.parse(data, recv))
    {
        //����Header�е�Status
        string flag = recv["flag"].asString();
		
        if(0 == strncmp(flag.c_str(),"1",1))
        {
			uint32 getMerchantid = recv["datas"]["merchantId"].asUInt();
			if(getMerchantid == merchantid)
			{
				string mctname = recv["datas"]["storeName"].asString();
				strcpy(storename,mctname.c_str());				
				CLog::Log("getMerchantInfo",CLog::TYPE_IMPORTANT,"id %u storeName %s",getMerchantid,storename);
				return true;
			}
		}        
    }
    //û�ҵ��Ӵ�
    CLog::Log("getMerchantInfo",CLog::TYPE_ERROR," The status didn't have not right!");
    return false;
}


int CBusinessXmppBusiList::DoWork()
{
    XT_XMPP_BUSI_LIST_REQ XmppBusiList;
	set<uint32> merchantDuplication;
    uint32 nRetCount;
    uint32 len = 0;
    uint32 i = 0;

    uint32 id_list[MAX_LIST_NUM];
//    tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_LIST_NUM];

    XT_XMPP_BUSI_LIST_ACK* pWebBusiListAck = (XT_XMPP_BUSI_LIST_ACK*)m_SendPacket.GetData();

    CancelReply();

    m_RecvPacket>>XmppBusiList.id
                >>XmppBusiList.msgnum;
    CLog::Log("CBusinessXmppBusiList",CLog::TYPE_IMPORTANT,"QueryUseriD:%d, msgnum:%d,IN",XmppBusiList.id,XmppBusiList.msgnum);

    nRetCount = XmppBusiList.msgnum;
    if(nRetCount > MAX_LIST_NUM)
        nRetCount = MAX_LIST_NUM;
    if(m_pRouter->GetDataBaseMini()->XmppBusiListReq(&XmppBusiList,pListInfo,nRetCount) == DB_ERROR)
    {
        CLog::Log("CBusinessXmppBusiList", CLog::TYPE_ERROR, "WebBusiList Req Failed! id:%d ",XmppBusiList.id);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<XmppBusiList.id
                    <<XmppBusiList.msgnum;
        return RESULT_SUCCESS;
    }
	//��pListInfo ����ʱ������
	if(nRetCount > 0)
	{
		sort(pListInfo,pListInfo + nRetCount - 1,SortXmppTimeMethod);
	}
	for(uint32 i = 0 ; i < nRetCount; i++)
    {
        CLog::Log("CBusinessXmppBusiList",CLog::TYPE_IMPORTANT,"after Query  has %u  sendtime %u!",
                  pListInfo[i].id,pListInfo[i].time);
    }
	
    seq = m_pRecvHead->seq;

    while(nRetCount > i)
    {
        pWebBusiListAck->actualnum = 0;

        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<XmppBusiList.id
                    <<XmppBusiList.msgnum
                    <<pWebBusiListAck->actualnum;
		 
        for(; i < nRetCount; i ++)
        {		
        	AccountArray ids;
			ids.clear();
			MYSQL_LIB->getMainMerchant( pListInfo[i].id,pListInfo[i].merchantId);
			if(pListInfo[i].merchantId == 0)
			{
				pListInfo[i].merchantId = pListInfo[i].id;
				ids.push_back(pListInfo[i].merchantId);
			}
			else	
			{
				//�޳����˺���ͬ���û�
				set<uint32>::iterator it_merchant;
				it_merchant = merchantDuplication.find(pListInfo[i].merchantId);
				if(it_merchant != merchantDuplication.end())//���ظ�
				{					
					CLog::Log("CBusinessXmppBusiList",CLog::TYPE_IMPORTANT,"�����ظ��̼�ID %u ",(pListInfo[i].merchantId));
					continue;
				}
				else
				{
					merchantDuplication.insert(pListInfo[i].merchantId);				
					if(DB_SUCCESS != MYSQL_LIB->getAllSubAccount(pListInfo[i].merchantId, ids))
					{	// ��ȡ���˺�ʧ��
						CLog::Log("CBusinessXmppBusiList",CLog::TYPE_IMPORTANT,"�̼�ID %u ��ȡ���˺�ʧ��",(pListInfo[i].merchantId));
						continue;
					}
					if(!getMerchantInfo(pListInfo[i].merchantId,pListInfo[i].storename))
					{	// ����merchantid��ȡ������ʧ�ܣ�����ѭ��
						CLog::Log("CBusinessXmppBusiList",CLog::TYPE_IMPORTANT,"�̼�ID %u ���ҵ�����ʧ��",(pListInfo[i].merchantId));
						continue;
					}
				}
			}

			//��ȡ���˺ŵ�״̬��Ϊ�̼ҵ��̵�״̬
			{				

				//��ѯ���˺�״̬
				uint8 subAccountsize = ids.size();
				std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(ids.size());
				userStatus.clear();
				if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(&ids[0], &userStatus[0], subAccountsize))
				{
					CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "QueryUserStatus failed!");
				}
				else
				{
					//�ж����˺����Ƿ������ߵ�
						auto ifind = std::find_if(userStatus.begin(), userStatus.begin() + subAccountsize,
					                          [] (const GSRV_GET_USER_STATUS_ACK::tagUserStatus& a) ->bool
					{
					    return a.status != XTREAM_OFFLINE;
					});

					//�ж��Ƿ����� 1Ϊ������ 0 Ϊ����
					if (ifind == (userStatus.begin() + subAccountsize))
					{				    
					    pListInfo[i].state = XTREAM_OFFLINE;
					}
					else
					{
					    pListInfo[i].state = 0;
					}
				}
			}
			pListInfo[i].subCount = ids.size();
			for(int subCount = 0;subCount < pListInfo[i].subCount; subCount++)
			{
				pListInfo[i].subMerchants[subCount] = ids[subCount];
			}			
            len = sizeof(pListInfo[i].id)+strlen(pListInfo[i].nickname)+strlen(pListInfo[i].storename)+sizeof(pListInfo[i].time)
                  +sizeof(pListInfo[i].state)+strlen(pListInfo[i].headImgUrl)+strlen(pListInfo[i].usersign)
                  +sizeof(pListInfo[i].merchantId) + sizeof(pListInfo[i].subCount) + pListInfo[i].subCount*sizeof(uint32) ;
            if(!m_SendPacket.HasData(len))
            {
                break;
            }
            m_SendPacket<<pListInfo[i].id
						<<pListInfo[i].merchantId
                        <<pListInfo[i].nickname
                        <<pListInfo[i].time
                        <<pListInfo[i].state
                        <<pListInfo[i].headImgUrl
                        <<pListInfo[i].usersign
                        <<pListInfo[i].storename
                        <<pListInfo[i].subCount;

			for(int subCount = 0;subCount < pListInfo[i].subCount; subCount++)
			{
				m_SendPacket << ids[subCount];
			}			
            pWebBusiListAck->actualnum ++;

            CLog::Log("CBusinessXmppBusiList",CLog::TYPE_DEBUG,
            "pListInfo id:%d, storename: %s, nickname:%s, time:%u, status:%d,headImgUrl:%s usersign %s,count:%u,merchantId %u subCount %d",
                      pListInfo[i].id,pListInfo[i].storename, pListInfo[i].nickname, pListInfo[i].time,
                      pListInfo[i].state, pListInfo[i].headImgUrl,pListInfo[i].usersign,
                      pWebBusiListAck->actualnum,pListInfo[i].merchantId,pListInfo[i].subCount);
        }
        SendXmppBusiListAck(m_SendPacket);
        m_SendPacket.Reset();
    }
    if(nRetCount == 0)
    {
        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<XmppBusiList.id
                    <<XmppBusiList.msgnum
                    <<(uint32)0;
        SendXmppBusiListAck(m_SendPacket);
    }
    return RESULT_SUCCESS;
}

void CBusinessXmppBusiList::SendXmppBusiListAck(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = CAppManager::GetInstance().GetSendSeq();
    pHead->cmd  = CMD_XMPP_BUSI_LIST_ACK;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    if(sessManager.find(m_pRecvHead->sid) != sessManager.end())
    {
        // ��xmpp�ͻ��˵�¼
        CLog::Log("CBusinessXmppBusiList",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", pData->data_len);
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)pData->data, pData->data_len);

        CancelReply();
    }
    else
    {
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(pData->data,pData->data_len,SEND_RETRY_TIME);
    }
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

    XT_WEB_MORE_MSG_ACK* pWebMsgAck = (XT_WEB_MORE_MSG_ACK*)m_SendPacket.GetData();

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

    CLog::Log("WebMoreMsg", CLog::TYPE_IMPORTANT, "sid:%d, did:%d, msgtime:%d, msgnum:%d, msgid:%d, uuid:%s, isMerchant:%d, nRetCount:%d, IN",
              WebMoreMsg.sid, WebMoreMsg.did, WebMoreMsg.msgtime, WebMoreMsg.msgnum, WebMoreMsg.msgid, WebMoreMsg.uuid,WebMoreMsg.isMerchant,nRetCount);
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
        //CLog::Log("WebMoreMsg", CLog::TYPE_ERROR, "WebMoreMsg Req Failed! sid:%d ",WebMoreMsg.sid);
        m_SendPacket<<(uint8)RESULT_FAILED
                    <<WebMoreMsg.sid
                    <<WebMoreMsg.did
                    <<WebMoreMsg.msgtime
                    <<WebMoreMsg.msgnum
                    <<WebMoreMsg.msgid
                    <<WebMoreMsg.uuid;
        CLog::Log("CBusinessWebMoreMsg",CLog::TYPE_ERROR,"WebMoreMsg Req Failed! sid:%d, did:%d, msgtime:%d, msgnum:%d, msgid:%d, uuid:%s, OUT",
                  WebMoreMsg.sid, WebMoreMsg.did, WebMoreMsg.msgtime, WebMoreMsg.msgnum, WebMoreMsg.msgid, WebMoreMsg.uuid);
        SendWebMoreMsgAck(m_SendPacket);//��ȡʧ��Ҳ֪ͨ
        return RESULT_SUCCESS;
    }

    tmp_len = sizeof(uint32)+sizeof(uint32)+sizeof(uint8)+sizeof(uint32)+sizeof(uint16);
    seq = m_pRecvHead->seq;

    while(nRetCount > i)
    {
        pWebMsgAck->actualnum= 0;

        m_SendPacket<<(uint8)RESULT_SUCCESS
                    <<WebMoreMsg.sid
                    <<WebMoreMsg.did
                    <<WebMoreMsg.msgtime
                    <<WebMoreMsg.msgnum
                    <<nRetCount
                    <<(uint8)1
                    <<pWebMsgAck->actualnum
                    <<WebMoreMsg.uuid;
        for(; i < nRetCount; i ++)
        {
            len = tmp_len + strlen(pMsgInfo[i].fontName)+strlen(pMsgInfo[i].nickName)+strlen(pMsgInfo[i].data);
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
            pWebMsgAck->actualnum ++;
        }
        if(i < nRetCount)
            pWebMsgAck->over = 0;
        else
            pWebMsgAck->over = 1;

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

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
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

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(pData->data,pData->data_len,SEND_RETRY_TIME);

}


//---------------------------------------------------------------------

CBusinessSendMessage::CBusinessSendMessage(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_P2PMSG_SEND,CMD_P2PMSG_ACK)
{
#ifdef PRESS_TEST
    LoadMerchantId();
#endif
}

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

    if(msg.data_type == XT_MSG::SF_FILE_REQ || msg.data_type == XT_MSG::SF_PACKET_REQ
       || msg.data_type == XT_MSG::SF_FILE_ACK /*|| msg.data_type == XT_MSG::SF_PACKET_ACK*/)
        m_RecvPacket.ReadData(msg.data,sizeof(msg.data));

    //����Ϊweb�豸
    if(m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)
        msg.to_terminal = XT_TALK::WEB_TERMINAL;

    //if(m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ || m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)
    //������Ϣ�����۵�
    if(msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::XT_RECOMENT || msg.data_type == XT_MSG::XT_SERVICE|| msg.data_type == XT_MSG::AUTO_REPLY)
    {
        m_RecvPacket>>msg.fontSize
                    >>msg.fontColor
                    >>msg.fontStyle
                    >>msg.fontName;

        m_RecvPacket.ReadData(msg.data,sizeof(msg.data));
        m_RecvPacket>>msg.from_nickname;

        CLog::Log("CHATmsg", CLog::TYPE_ERROR, "########## DID %d  msgs  %s ",
                  msg.to_id,msg.data);

        //ֻ��web?
        if( (msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::XT_RECOMENT)
            /*&& (m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ || m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB)*/
          )
        {
            //ѹ�����ʱ��
            /*
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
            */
#ifndef IMG_SAVE
            //���ﲻ�棬����һ���߳�����
            uint32 logintime =0;
            CUser::m_UserListLock.Lock();
            CUser *pUser = NULL;//CUser::m_pUserListHead;
            int m_nID = (xtTalkMsg.from_id>xtTalkMsg.to_id)?xtTalkMsg.from_id:xtTalkMsg.to_id;
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
        }
    }

    if(msg.data_type == XT_MSG::XT_SERVICE)
    {
        uint32 seller_id = msg.from_id;
        uint32 buyer_id = msg.to_id;
        uint16 recomm = 0;

        if(m_pRouter->GetDataBaseMini()->WebServiceSave(buyer_id,seller_id,recomm,msg.send_time) == DB_ERROR)
        {
            CLog::Log("WebRecomentSave", CLog::TYPE_ERROR, "WebRecoment Save Failed! buyer_id:%d, seller_id:%d", buyer_id, seller_id);
        }
    }

    else if(msg.data_type == XT_MSG::XT_RECOMENT)
    {
        if(m_pRouter->GetDataBaseMini()->WebRecommSave(msg.from_id,msg.to_id,msg.ver,msg.send_time) == DB_ERROR)
        {
            CLog::Log("WebRecomentSave", CLog::TYPE_ERROR, "WebRecoment Save Failed! from_id:%d, to_id:%d ,ver:%d, send_time:%d",
                      msg.from_id,msg.to_id,msg.ver,msg.send_time);
        }

        XT_TALK* talk = (XT_TALK*)m_RecvPacket.GetData();
        talk->data_type = XT_MSG::IM_MSG;
    }

    m_RecvPacket>>msg.uuid;

    //���ֺ�ͼƬ��Ϣ�������̱߳���
#ifdef IMG_SAVE
    if(msg.data_type == XT_MSG::IM_MSG || msg.data_type == XT_MSG::SF_FILE_REQ
       || msg.data_type == XT_MSG::SF_PACKET_REQ || msg.data_type == XT_MSG::SF_FILE_ACK || msg.data_type == XT_MSG::AUTO_REPLY/*|| msg.data_type == XT_MSG::SF_PACKET_ACK*/)
    {
        CAppManager::GetInstance().GetChatMsgBuffer0()->Push((BYTE*)&msg,sizeof(XT_TALK));
    }
#endif
}
#ifdef IM_USE_SSDB
void CBusinessSendMessage::SaveTalkMsg()
{
    XT_TALK msg;
    string id_name,id_key;
    string key,value,hash_name;
    ssdb::Status stat;

    SSDBLinks& links = SSDBLinks::GetInstance();

    ssdbClient* client = links.getLink();
    if(client == NULL)
    {
        CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb client is null");
        return;
    }

    if(client->l == NULL)
    {
        CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb link is null");
        links.releaseLink(client);
        return;
    }

    m_RecvPacket>>msg.from_id
                >>msg.to_id
                >>msg.data_type
                >>msg.ver
                >>msg.send_time
                >>msg.recv_flag
                >>msg.fontSize
                >>msg.fontColor
                >>msg.fontStyle
                >>msg.fontName;

    if((strlen(msg.fontName) == 0 )&& (msg.data_type != XT_MSG::SF_FILE_REQ || msg.data_type != XT_MSG::SF_PACKET_REQ || msg.data_type != XT_MSG::SF_FILE_ACK))
    {
        links.releaseLink(client);
        return;
    }
    else
    {
        m_RecvPacket.ReadData(msg.data,sizeof(msg.data));
        m_RecvPacket>>msg.from_nickname;

        if(msg.data_type == XT_MSG::IM_MSG)
        {
            std::string strMsg(msg.data);
            //����Ϣ�����ͼƬ
            if(strMsg.find("<image:{") != string::npos)
            {
                CLog::Log("CBusinessSendMessage::SaveTalkMsg", CLog::TYPE_DEBUG, "Msg Exist Image");
                CAppManager::GetInstance().GetChatMsgBuffer()->Push((BYTE*)&msg,sizeof(XT_TALK));
            }
            //����Ϣ�ﲻ����ͼƬ����ֱ�Ӵ�ssdb
            else
            {

                links.genhName(msg.from_id,id_name);
                links.genhKey(msg.to_id,id_key);

                //��ȡhash name
                stat = links.hGetData(client->l,id_name,id_key,hash_name);
                if(stat.not_found())
                {
                    //���뷢�ͷ�id
                    links.genhValue(msg.from_id,msg.to_id,hash_name);
                    links.hSetData(client->l,id_name,id_key,hash_name);

                    //������շ�id
                    links.genhName(msg.to_id,id_name);
                    links.genhKey(msg.from_id,id_key);
                    links.hSetData(client->l,id_name,id_key,hash_name);
                }
                else if(stat.code() == "error")
                {
                    if(client->l)
                    {
                        delete client->l;
                        client->l = NULL;
                    }
                    CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb get data error");

                    links.releaseLink(client);
                    return;
                }

                //������Ϣkey��value
                links.genhKey(msg.send_time,key);
                links.genhValue(&msg,value);

                stat = links.hSetData(client->l,hash_name,key,value);
                if(stat.code() == "error")
                {
                    if(client->l)
                    {
                        delete client->l;
                        client->l = NULL;
                    }
                    CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb set data error");

                    links.releaseLink(client);
                    return;
                }
                uint64 flag =atoi(hash_name.c_str());
                std::string name = "hash_name";
                switch( flag % 5)
                {
                    case 0:
                        name = "hash_name_1";
                        break;
                    case 1:
                        name = "hash_name_2";
                        break;
                    case 2:
                        name = "hash_name_3";
                        break;
                    case 3:
                        name = "hash_name_4";
                        break;
                    case 4:
                        name = "hash_name_5";
                        break;
                }
                stat = links.hSetData(client->l,name,hash_name,key);
                if(stat.code() == "error")
                {
                    SAFE_DELETE(client->l);
                    CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb set data error");
                    l       inks.releaseLink(client);
                    return;
                }
            }
        }
#if 0
        string key_start = "";
        string key_end = "";
        vector<string> values;
        stat = links.hScanData(client->l,hash_name,key_start,key_end,100,&values);
        if(stat.code() == "error")
        {
            if(client->l)
            {
                delete client->l;
                client->l = NULL;
            }
            CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb scan data error");

            links.releaseLink(client);
            return;

        }
        if(!values.empty())
        {
            vector<string>::iterator iter;
            vector<string> str;
            XT_TALK msg;
            int i = 0;
            for(iter = values.begin(); iter != values.end(); ++ iter,++ i)
            {
                if(i%2 != 0)
                {
                    cout<<*iter<<endl;
                    links.genMsg(*iter,str,&msg);
                    cout<<msg.data_type<<"&"<<msg.recv_flag<<"&"<<msg.fontSize<<"&"<<msg.data<<"&"<<msg.from_nickname<<endl;
                }
            }
        }
#endif
    }
    links.releaseLink(client);
    return;
}
#endif





int CBusinessSendMessage::DoWork()
{
    XT_SERVER_P2PMSG_SEND* pP2PMsgSend  = (XT_SERVER_P2PMSG_SEND*)m_RecvPacket.GetData();
    XT_SERVER_P2PMSG_ACK* pP2PMsgAck    = (XT_SERVER_P2PMSG_ACK*)m_SendPacket.GetData();

    pP2PMsgAck->Sender  = pP2PMsgSend->Sender;
    pP2PMsgAck->Receiver= pP2PMsgSend->Recver;
    pP2PMsgAck->type    = pP2PMsgSend->type;



    m_SendPacket.SetCursor(sizeof(XT_SERVER_P2PMSG_ACK));
#ifdef PRESS_TEST
    DWORD nBeginTime = timeGetTime();
#endif

#ifdef PRE_INTERNAL
#else
#ifdef IM_USE_SSDB
    SaveTalkMsg();
#else
    SaveWebMsg();
#endif
#endif

#ifdef PRESS_TEST
    CLog::Log("SendMsg1",CLog::TYPE_STATISTIC,"TimeStatistic: %u %u %u %u",
              pP2PMsgAck->Sender, pP2PMsgAck->Receiver,pP2PMsgAck->type,timeGetTime() - nBeginTime);

    nBeginTime = timeGetTime();
#endif

    //ת������Ϣ
    if(m_pAppNetwork->UserType(m_pRecvHead->did) == TYPE_WEB &&
       (pP2PMsgSend->type >= XT_MSG::SF_FILE_REQ && pP2PMsgSend->type <=XT_MSG::SF_PACKET_ACK)
      )
    {
        //ͼƬ��Ϣ��¼�Ĳ������web�ˣ���ͼƬ������

    }
    else
    {
        RouterMsgNotify(pP2PMsgSend->Recver);
    }
#ifdef PRESS_TEST
    CLog::Log("SendMsg2",CLog::TYPE_STATISTIC,"TimeStatistic: %u %u %u %u",
              pP2PMsgAck->Sender, pP2PMsgAck->Receiver,pP2PMsgAck->type,timeGetTime() - nBeginTime);
#endif
    return RESULT_SUCCESS;
}

void CBusinessSendMessage::RouterMsgNotify(uint32 to_id)
{
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;

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

    sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize());

    //�����û���¼��ʽ�ж��Ƿ񽫷���ֵ���͸�jabberdIM�߳�
    if(/*lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ ||*/ sessManager.find(to_id) != sessManager.end() )
    {
        CLog::Log("CBusinessSendMessage::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", sendData.data_len);
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)sendData.data, sendData.data_len, pRecvData->vlink);
        sleep(1);
    }
    else
    {

        CLog::Log("CBusinessSendMessage::DoWork",CLog::TYPE_IMPORTANT,"send to other, length:%d", sendData.data_len);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);
    }


//�������Զ��ظ�

#if 0

    uint32 from_id = (uint32)(*(uint32*)pVerifyFriendReq);

    //�������Һ����
    * ((uint32*)pVerifyFriendReq ) = to_id;
    * ((uint32*)(pVerifyFriendReq + 4)) = from_id;

    CLog::Log("SendMsg2",CLog::TYPE_DEBUG,"ServerAutoReply: %u %u",*(uint32*)pVerifyFriendReq, *((uint32*)(pVerifyFriendReq + 4)));
    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),from_id,XT_HEAD::DEST_TYPE_USER);
    else
        CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize()),from_id,XT_HEAD::DEST_TYPE_USER,XT_HEAD::FLAG_MOBSRVACTION);

    if( sessManager.find(from_id) != sessManager.end() )
    {
        CLog::Log("SendMsg2",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", sendData.data_len);
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)sendData.data, sendData.data_len, pRecvData->vlink);
    }


    //ppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);
#endif



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

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);

#endif
}
#ifdef PRESS_TEST
void CBusinessSendMessage::LoadMerchantId()
{
    Json::Reader jReader;
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



//�񶯺��Ѵ���---------------------------------------------------------------------

CBusinessShakeFriendWin::CBusinessShakeFriendWin(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_WIN_SHAKE_REQ,CMD_WIN_SHAKE_ACK)
{

}

int CBusinessShakeFriendWin::DoWork()
{
    XT_SHAKE_WIN_REQ* pShakeWinReq  = (XT_SHAKE_WIN_REQ*)m_RecvPacket.GetData();

    //ת������Ϣ
    RouterShakeNotify(pShakeWinReq->Recver);

    this->CancelReply();

    return RESULT_SUCCESS;
}

void CBusinessShakeFriendWin::RouterShakeNotify(uint32 to_id)
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
    lpSrvTransmit->msgtype              = CMD_WIN_SHAKE_REQ;

    BYTE* pShakeFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
    memcpy(pShakeFriendReq,m_RecvPacket.GetData(),m_RecvPacket.GetSize());

    sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + m_RecvPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}

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

    CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(pStatusGetReq->fid);
    if( pUser )
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
        CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"friend: %d status: %d",pStatusGetAck->id,pStatusGetAck->status);
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
        CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"not found friend: %d status: %d",pStatusGetAck->id,pStatusGetAck->status);
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

CBusinessReloginNotify::CBusinessReloginNotify(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMDGSRV_USERRELOGIN_NOTIFY,CMDGSRV_USERRELOGIN_ACK)
{

}


int CBusinessReloginNotify::DoWork()
{
    SERVER_RELOGIN_NOTIFY*  pReq = (SERVER_RELOGIN_NOTIFY*)m_RecvPacket.GetData();
    SERVER_RELOGIN_ACK*     pAck = (SERVER_RELOGIN_ACK*)m_SendPacket.GetData();

    pAck->userid = pReq->userid;
    CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(pReq->userid);
    if( pUser && pReq->userip && pReq->lanip)
    {
        if (pUser->m_nIP != pReq->userip || (pUser->m_nIP == pReq->userip && pUser->m_nLanIP != pReq->lanip))
            CNotifyProtocol::Relogin(pReq->userid);
    }


    m_SendPacket.SetCursor(sizeof(SERVER_RELOGIN_ACK));

    LogoutLocalIm(pReq->userid);
    if (pReq->chgpwd != 0)
    {
        m_pRouter->GetDataBaseWeb()->Change_User_PWD(pReq->userid);
    }

    return RESULT_SUCCESS;
}

//���ǻ�Ӧ��ȫ�������û���������
int CBusinessReloginNotify::SendReply()
{
    RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    SendDataStruct* pSendData = m_pSendData;

    m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
    m_pSendHead->from = XT_HEAD::FROM_SERVER;
    m_pSendHead->sid  = m_pRecvHead->did;
    m_pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
    m_pSendHead->did    = m_pRecvHead->sid;

    m_pSendData->retry_times = 0;
    m_pSendData->time_interval = 0;
    m_pSendData->vlink       = pRecvData->vlink;

    //������������Ϣ
    pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    //����Э��ͷ����Ϣ
    XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
    pSendHead->len  = m_SendPacket.GetSize();

    //��������
    assert(m_pAppNetwork);
    m_pAppNetwork->SendDataTo(
        pSendData->data,
        pSendData->data_len,
        m_pSendData->retry_times,
        m_pSendData->time_interval,
        pRecvData->src_ip,pRecvData->port );

    return 0;
}

void CBusinessReloginNotify::LogoutLocalIm(uint32& userid)
{
    CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
    CUser* pUser = userManager.Find(userid);

    if(!pUser)
        return ;

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //����̶�Ⱥ��Ϣid
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();

    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

    //�����û�����ʱ��
    int money;
    time_t tm;
    time(&tm);
    m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
    if (money > 0  && pUser->m_nID != 0)
    {
        if (money > 5)
            money = 5;
        m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
        //CLog::Log("Money", CLog::TYPE_IMPORTANT, "�û�:%d���ͻ���:%d", pUser->m_nID, money*5);
//      SendDataStruct sendData;
//
//      char packet[MAX_NETWORK_PACKET];
//      char content[255];
//      sprintf(content,"Ϊ�˹�����ʹ��Mini���Ѿ�����������%d���֣�",money*5);
//
//      CSendPacket pSendPacket(packet);
//      pSendPacket <<(uint32)0
//          <<(uint16)XT_SERVER_TRANSMIT::SRVMSGTYPE::TYPE_SYS_MESSAGE
//          <<(uint32)tm
//          <<(uint8)2
//          <<content;
//
//      m_pRouter->GetDataBaseMini()->Save_User_Msg(pUser->m_nID, 0,0,0,(uint32)tm,XT_SERVER_TRANSMIT::SRVMSGTYPE::TYPE_SYS_MESSAGE,pSendPacket.GetData(),(uint16)pSendPacket.GetSize());
    }


    //�����û��ڴ�
    pUser->DelFromOnlineList();
    userManager.DelHash(pUser->m_nID);
    m_pAppNetwork->UserLogout(pUser->m_nID);
}


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

    CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();

    CUser *pUser = UserManager.Find(m_pRecvHead->sid);
    if (pUser)
        strncpy(pUser->m_szUserSign, szUnderWrite,MAX_USERSIGN_LEN);


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
    uint8 type = 0;
    uint8 uploadFinish = 0;
    uint8 ret = 0;
    uint32 from_id = 0;
    int dataLen = 0;
    string strImgUrl;
    char data[FILE_PACKET_LEN];
    char face_url[MAX_HEADURL_LEN+1]="";
    IMAGE_FILE_STRUCT headImgFile;
    static CChatMsgSave HeadFileSave;

    m_RecvPacket>>from_id
                >>type;
    //typeΪ1ʱ������ͻ����ϴ�ͷ������
    if(type == 1)
    {
        m_RecvPacket>>headImgFile.file_session_id
                    >>headImgFile.file_size
                    >>headImgFile.file_name;
        CLog::Log("CBusinessSaveHeadFile",CLog::TYPE_IMPORTANT,"id:%d, type:%d, file_session_id:%d, file_size:%d, file_name:%s, IN",
                  from_id, type, headImgFile.file_session_id, headImgFile.file_size,headImgFile.file_name);
        RECV_FILE_STRUCT *pRecv = HeadFileSave.SF_NewRecvFile(headImgFile.file_session_id,from_id, headImgFile.file_name, headImgFile.file_size);
        if (pRecv == NULL)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }

    }
    //typeΪ2ʱ������ͻ����ϴ�ͷ���ļ�
    else if(type == 2)
    {
        m_RecvPacket>>headImgFile.file_id
                    >>headImgFile.last_packet_id
                    >>headImgFile.last_file_pointer;
        dataLen = m_RecvPacket.ReadData(data,sizeof(data));
        CLog::Log("CBusinessSaveHeadFile",CLog::TYPE_IMPORTANT,"id:%d, type:%d, file_id:%d, last_packet_id:%d, last_file_pointer:%d, IN",
                  from_id, type, headImgFile.file_id, headImgFile.last_packet_id, headImgFile.last_file_pointer);
        RECV_FILE_STRUCT *pRecv = HeadFileSave.SF_FindRecvFile(headImgFile.file_id,from_id);

        if(pRecv != NULL)
        {
            map<int,int>::iterator it =  pRecv->mapPacketRecv.find((int)headImgFile.last_packet_id);
            //�����ظ��İ��ظ�д���ظ������С
            if( it == pRecv->mapPacketRecv.end())
            {
                fseek(pRecv->fileObject,headImgFile.last_file_pointer,0);
                fwrite(data,1,dataLen,pRecv->fileObject);
                pRecv->mapPacketRecv.insert(map<int,int>::value_type((int)headImgFile.last_packet_id,
                                            (int)headImgFile.last_file_pointer));
                pRecv->recv_size += dataLen;
            }
            else
            {
                CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_IMPORTANT, "id:%d, repeated~", from_id);
            }
            CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_IMPORTANT, "id:%d, datalen:%u, recv_size:%u, file_size:%u",
                      from_id, dataLen, pRecv->recv_size, pRecv->file_size);
        }
        else
        {
            ret = 1;
            m_SendPacket<<ret;
            CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_ERROR, "id:%d, ret:%d, pRecv == NULL!, OUT", from_id, ret);
            return RESULT_FAILED;
        }
        if(/*dataLen < FILE_PACKET_LEN ||*/ pRecv->recv_size == pRecv->file_size)
        {
            fseek(pRecv->fileObject, 0L, SEEK_END);
            int nFileSize = ftell(pRecv->fileObject);
            if(nFileSize > 0)
            {
                strImgUrl = HeadFileSave.upLoadPic(pRecv->file_name);
                strcpy(face_url,strImgUrl.c_str());
                if(strImgUrl.find("http:") == string::npos)
                {
                    CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_ERROR, "id:%d, Pic upload Error! file_name:%s",from_id, pRecv->file_name);
                }
                else
                {
                    CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_IMPORTANT, "id:%d, Img Recv and Upload OK! file_name:%s, ImgUrl:%s",
                              from_id,pRecv->file_name,strImgUrl.c_str());
                }
                HeadFileSave.SF_RemoveRecvFile(headImgFile.file_id,from_id);
                uploadFinish = 1;
            }
            else
            {
                CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_ERROR, "The IMG size is 0! id:%d ",from_id);
            }
        }

    }
    else
    {
        CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_ERROR, "The type is wrong!!id:%d, type:%d ",from_id, type);
    }

    m_SendPacket<<ret
                <<type
                <<uploadFinish
                <<face_url;
    CLog::Log("CBusinessSaveHeadFile", CLog::TYPE_IMPORTANT,"id:%d, ret:%d, uploadFinish:%d, face_url:%s, OUT",from_id, type, uploadFinish, face_url);
    return RESULT_SUCCESS;
}

CBusinessHistoryMsg::CBusinessHistoryMsg(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_HISTORY_MSG_REQ,CMD_HISTORY_MSG_ACK)
{
    pData = new SendDataStruct;
    if(pData == NULL)
        CLog::Log("CBusinessHistoryMsg",CLog::TYPE_ERROR,"New failed");
}

CBusinessHistoryMsg::~CBusinessHistoryMsg()
{
    if(pData)
    {
        delete pData;
        pData = NULL;
    }
}

int CBusinessHistoryMsg::DoWork()
{
	MsgListReq req;
	char szData[1024] = {0};
	int nLen = m_RecvPacket.ReadData(szData,1024);

	if(!req.ParseFromArray(szData, nLen))
	{	// �����кųɹ�
		CLog::Log("CBusinessHistoryMsg",CLog::TYPE_ERROR,"�����к�ʧ��, len:%d", nLen);
		return RESULT_FAILED;	
	}

	CLog::Log("CBusinessHistoryMsg",CLog::TYPE_DEBUG,"�����кųɹ�");

	MsgList ack;
	ack.set_userid(req.toid());
	uint32 ret;
    if(m_pRouter->GetDataBaseMini()->WebHistoryMsg(req.fromid(), req.toid(), req.bgnmsgid(), req.bgntime(), req.endtime(), req.maxmsg(), ret, ack) == DB_ERROR)
    {
        CLog::Log("CBusinessHistoryMsg",CLog::TYPE_ERROR,"��ѯ��ʷ��¼ʧ��");
    }
    CLog::Log("CBusinessHistoryMsg",CLog::TYPE_DEBUG,"��ѯ��ʷ��¼�ɹ�");

     //�����û���¼��ʽ�ж��Ƿ񽫷���ֵ���͸�jabberdIM�߳�
    if(/*lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ ||*/ sessManager.find(m_pRecvHead->sid) != sessManager.end() )
    {
		uint32_t msg_size = ack.ByteSize();
    	char* szData = new char[sizeof(XT_HEAD)+msg_size];
    	memcpy(szData, m_pSendData->data, sizeof(XT_HEAD));
    	if (!ack.SerializeToArray(szData+sizeof(XT_HEAD), msg_size))
    	{
        	CLog::Log("CBusinessHistoryMsg",CLog::TYPE_DEBUG,"���к�ʧ��");
        	return RESULT_FAILED;	
    	}
    	
        CLog::Log("CBusinessHistoryMsg::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", sizeof(XT_HEAD)+msg_size);
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)szData, sizeof(XT_HEAD)+msg_size, 0);

        delete[] szData;
        szData = NULL;
        sleep(1);
    }

    return RESULT_SUCCESS;
}

void CBusinessHistoryMsg::SendWebMoreMsgAck(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
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

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(pData->data,pData->data_len,SEND_RETRY_TIME);

}

