
#ifdef IM_USE_CACHE
#include "BusinessExIMSrv.h"
#include "BusiCacheInterface.h"
#include "MiniAppNetwork.h"
#include <typeinfo>
#include "RouterBusiness.h"
#include "httpclient.h"
#include "curl/curl.h"
#include "UserManager.h"

/*
��������

#define MEMCACHED_GET_INFO_SUCCESS 0

#define MEMCACHED_SERVER_DOWN 1

#define MEMCACHED_NO_INFO 2

#define MEMCACHED_JASON_ERROR 3


*/
int CMemcacheQueryUserStatusi(  CRouterBusiness*        m_pRouter,
                                const uint32 *idList,uint8 &m_nReturnCount,
                                XT_XTREAM_SEARCH_ACK::tagFriendInfo  *FriendInfo)
{
    //��ѯ����û�������״̬,��ȫ�ַ�������ѯ
    uint8   idcount=0;
    int j =0;
    int ret = -1;
    idcount = m_nReturnCount;
    GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[MAX_SEARCH_FRINED_RETURN_NUM];
    ret = CMemcacheQueryUserStatusi(m_pRouter,
                                    idList,m_nReturnCount,
                                    userStatus);

    //�����û�״̬
    for( j =0; j < idcount; ++j)
    {
        if( FriendInfo[j].id == userStatus[j].id)
        {
            FriendInfo[j].status = userStatus[j].status;
        }
        CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"userid %u status %u",userStatus[j].id,FriendInfo[j].status);
    }

    //���ؽ��
    return ret;
}

int CMemcacheQueryUserStatusi( CGlobalBusi* m_pGlobalBusi,
                               const uint32 *idList,uint8 &m_nReturnCount,
                               GSRV_GET_USER_STATUS_ACK::tagUserStatus *userStatus)
{
	CUserManager userManager(m_pGlobalBusi);
	
    //��ѯ����û�������״̬,��ȫ�ַ�������ѯ
    uint8   idcount=0;
    int j =0;
    idcount = m_nReturnCount;

    int ret = m_pGlobalBusi->QueryUserStatus(
                  idList ,
                  userStatus,
                  idcount );

    if( MEMCACHED_SERVER_DOWN == ret )
    {

        //ʧ��֮��鱾��
        int localReturnCount = 0;
        uint8 localFailCount = 0;
        shared_ptr<CUser> pUser = NULL;
        bool successflag[MAX_SEARCH_FRINED_RETURN_NUM] = {0};
        tagGSRV_GET_USER_STATUS_ACK::tagUserStatus pUserInfo[MAX_SEARCH_FRINED_RETURN_NUM];
        //Ĭ�����е��û���ʱ��������
        for(int i = 0 ; i < MAX_SEARCH_FRINED_RETURN_NUM; i++ )
        {
            pUserInfo[i].status = XTREAM_OFFLINE;
        }
        //uint32 idMultiList[MAX_SEARCH_FRINED_RETURN_NUM];
        for( j = 0 ; j < m_nReturnCount; ++j)
        {
            pUser = userManager.Find(idList[j]);
            if( NULL == pUser)
            {
                CLog::Log("CBusinessSearchFriend",CLog::TYPE_DEBUG,
                          "����һ��δ��½�û���ѯ�����б�,id:%d",idList[j]);
                //idMultiList[localFailCount] = idList[j];
                successflag[j] = false;
                localFailCount++;
                continue;
            }
            //ʹ��FriendInfo ��0��ʼ��¼�ɹ��û���Ϣ

            userStatus[j].status = pUser->m_nStatus;
            successflag[j] = true;
            CLog::Log("CBusinessSearchFriend",CLog::TYPE_DEBUG,
                      "��ѯ�����û�id:%d state:%d",idList[j],pUser->m_nStatus);
            localReturnCount++;
        }
        if(localReturnCount < m_nReturnCount ) //�鱾���û�����������Ŀ�����ͶಥREQ����
        {
            CLog::Log("CBusinessSearchFriend",CLog::TYPE_DEBUG,
                      "��ʼ�㲥��ѯ�����û� idList[0]:%d",idList[0]);
            if( RESULT_FAILED == m_pGlobalBusi->QueryOtherUserStatus(
                    idList ,
                    pUserInfo,
                    m_nReturnCount) )
            {
                //����ʧ��,����ʣ����Ϊ����
                for( j = 0 ; j < m_nReturnCount; ++j)
                {
                    if(userStatus[j].id == idList[j] && successflag[j] == false)
                    {
                        userStatus[j].status = XTREAM_OFFLINE;
                    }
                }

            }
            else
            {
                //����REQ�ɹ����ϲ�
                for( j = 0 ; j < m_nReturnCount; ++j)
                {
                    if(userStatus[j].id == idList[j] && successflag[j] == false)
                    {
                        successflag[j] = true;
                        userStatus[j].status = pUserInfo[j].status;
                    }
                }
            }

        }
        else
        {
            //���ز��ҳɹ���������û�����ʣ����Ϊ����
            for( j = 0 ; j < m_nReturnCount; ++j)
            {
                if(userStatus[j].id == idList[j] && successflag[j] == false)
                {
                    successflag[j] = true;
                    userStatus[j].status = XTREAM_OFFLINE;
                }
            }
        }
    }

    //���ؽ��
    return ret; //MEMCACHED_NO_INFO ��errorһ��
}


int CMemcacheQueryUserStatusi( CRouterBusiness* m_pRouter,
                               const uint32 *idList,uint8 &m_nReturnCount,
                               GSRV_GET_USER_STATUS_ACK::tagUserStatus *userStatus)
{
    return CMemcacheQueryUserStatusi( m_pRouter->GetGlobalBusi(),
                                      idList,m_nReturnCount,userStatus);
}

int CalcQueueLength(int QueueOrigin,int row = 4,int col = 4)
{
    if(QueueOrigin == 0)
        return -1;

    if(QueueOrigin < (row * col))
    {
        return (--QueueOrigin / col) + 1;
    }
    else
    {
        return QueueOrigin - (row * col) + row;
    }
}

int GetCSSubAccount_x(CRouterBusiness* m_pRouter,uint32 TeamID,uint32 buyer,vector<uint32> ids,uint32 &id)
{

#if 1
    //��ȡ�������˺�
    //��ȫ�ַ�������ѡ������ҿ��ԽӴ����û�

    //������ڶ��׵��ŶӶ���(������ŶӵĻ�)

    //�����쳣

    //AccountArray ids;

    //��ͷ�����״̬
    CLog::Log("GetCSSubAccount",CLog::TYPE_IMPORTANT,"CS mode AVERAGE_FLOW mode");
    int ret = -1;
    if((ret = m_pRouter->GetGlobalBusi()->GetBestCSAccount(0, TeamID, ids, 1, id)) != CS_SUCCESS)
    {
        CLog::Log("GetCSSubAccount",CLog::TYPE_ERROR,"merchant_id:%d ,�޷��ҵ�����˺� (�뿪���߽Ӵ���)",TeamID);
        id = 0;
        return ret;
    }
    else
    {
    
        //��ȡ���˺ųɹ�
        ClientService *clientService = CAppManager::GetInstance().GetCSManager().Find(id);
        if(clientService) //�ڱ���
            /*�޸��û���Ϣ��memcached��*/
        {
            clientService->AddCustomer_x(TeamID,buyer);
        }
        else//��memcached �ϻ�ȡ�˺�
        {
            ClientService Otherclientservice;
            CSinfo_t csinfo_reply;
            uint8 count = 1;
            if(m_pRouter->GetGlobalBusi()->QueryCSInfo(&id,count,&csinfo_reply) != MEMCACHED_GET_INFO_SUCCESS)
            {
                id = 0;            //û��ȡ���ͷ���Ϣ�������쳣��֧

                CLog::Log("GetCSSubAccount",CLog::TYPE_IMPORTANT,"1��ǰ��%uû�п��ÿͷ�����",TeamID);
            }
            else
            {
                //clientService->AddCustomer(req->group_id,buyer); ���ڱ��أ�����Ҫ���������ģ�ֱ��ͬ����memcached
				//sinfo_reply.m_CurConsult++;
				//csinfo_reply.m_Customerid.push_back(buyer);
				//CMemcacheUpdateConsultCSInfo(m_pRouter,csinfo_reply);				
		       CsCustOper(m_pRouter,UO_ADD_CUST,id,buyer);				
            }
        }
        return 0;
    }


#if 0
    else //��ʼ�Ŷӽ��ͻ��˼������
    {
        CSQueueManager & csQueueManager = CAppManager::GetInstance().GetCSQueueManager();
        CsQueue = csQueueManager.Find(100);
        if(CsQueue == NULL)
        {
            CSQueueManager& csQueueManager = CAppManager::GetInstance().GetCSQueueManager();
            CsQueue = csQueueManager.AddHash(100);
        }
        CsQueue->AddToOnlineList(1);//����Ҽ��뵽�ŶӶ�����

    }
#endif
#endif
    return 0;


}



void GetSubAccountInfo(CRouterBusiness* m_pRouter,uint32 id,SubAccountInf_t &Subinfo)
{
    /*�������ʺŻ�����Ϣ*/
	CUserManager userManager(m_pRouter->GetGlobalBusi());

    if(id != 0)  //����̨ʧ���Ƿ���-1������ʧ�ܷ��ص���0
    {
        shared_ptr<CUser> pUser = NULL;
        pUser = userManager.Find(id);
        if( NULL == pUser || (!pUser->m_bIsFullInfoValid) )
        {
            XT_SERVER_USERINFO_GET_ACK UserInfo;
            //˵�����û����ڱ���������,�����ݿ��в���
            CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
            if( DB_SUCCESS != pDatabase->Get_User_Info(id,UserInfo))
            {
                CLog::Log("GetSubAccountInfo",CLog::TYPE_ERROR,"Get_User_Info Error:%d",id);
            }
            else
            {
                strcpy(Subinfo.strEmail, UserInfo.email);
                strcpy(Subinfo.strUsername, UserInfo.username);
                strcpy(Subinfo.strMobile, UserInfo.mobile);
                strcpy(Subinfo.strNickname, UserInfo.nickname);
                strcpy(Subinfo.usersign, UserInfo.usersign);
                strcpy(Subinfo.face_url, UserInfo.face_url);

                CLog::Log("GetSubAccountInfo",
                          CLog::TYPE_DEBUG,
                          "id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s,face_url %s",
                          id ,
                          Subinfo.strEmail,Subinfo.strUsername, Subinfo.strMobile,Subinfo.strNickname,Subinfo.usersign,Subinfo.face_url);
            }

        }
        else
        {
            strcpy(Subinfo.strEmail, pUser->m_szEmail);
            strcpy(Subinfo.strUsername, pUser->m_szUserName);
            strcpy(Subinfo.strMobile, pUser->m_szPhone);
            strcpy(Subinfo.strNickname, pUser->m_szNickName);
            strcpy(Subinfo.usersign, pUser->m_szUserSign);
            strcpy(Subinfo.face_url,pUser->m_szHeadURL);
            CLog::Log("GetSubAccountInfo",
                      CLog::TYPE_DEBUG,
                      "id:%d, Email:%s, Username:%s, Mobile:%s, Nickname:%s Usersign %s,face_url %s",
                      id , Subinfo.strEmail,Subinfo.strUsername, Subinfo.strMobile,Subinfo.strNickname,Subinfo.usersign,Subinfo.face_url);
        }

    }

    Subinfo.id = id;

    CLog::Log("GetSubAccountInfo", CLog::TYPE_IMPORTANT,
              "id:%u, email:%s, mobile:%s, nickname:%s,usersign %s face_url %s OUT" ,
              id, Subinfo.strEmail, Subinfo.strUsername, Subinfo.strMobile, Subinfo.strNickname,Subinfo.usersign,Subinfo.face_url);
}

bool SortQueueMethod(const CSQList_t &x,const CSQList_t &y)
{
    if(x.Level != y.Level)
        return x.Level>y.Level;
    else
        return x.requestTime<y.requestTime;
}

int PushQueueSubAccountToKafka(CRouterBusiness* m_pRouter,uint32 teamID)
{
	CUserManager userManager(m_pRouter->GetGlobalBusi());

#if 0
    if(teamID == -1)
    {
        CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_ERROR,"1Query team %u fail!",teamID);
        return RESULT_SUCCESS;
    }
#endif
    vector<uint32> ids;
    CDataBaseMini *pDatabase = m_pRouter->GetDataBaseMini();

    int ret = pDatabase->CS_Get_Group_Account(teamID,ids);
    if(ret != DB_SUCCESS)
    {
        CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_ERROR,"Query team %u subaccount failed!",teamID);
    }
    else
    {
        for(int i = 0 ; i < (int)ids.size(); i++)
        {

            CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_IMPORTANT,"Query team %u has member %u !",teamID,ids[i]);
        }
    }

    tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo qinfo;
    int count_Q = 1;
    ret = m_pRouter->GetGlobalBusi()->QueryCSQueueInfo((const uint32 *)&teamID,(uint8)count_Q,(tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo *)&qinfo);
    {
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            if(ret == MEMCACHED_NO_INFO)
            {
                CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_IMPORTANT,"����%dδ�����Ŷ�",qinfo.teamID);
                return RESULT_SUCCESS;
            }
            else
            {
                CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_ERROR,"Query team %u fail!",qinfo.teamID);
                return RESULT_SUCCESS;
            }
        }
    }
		
    if(qinfo.m_Customerlist.size() <= 0)
    {
        return RESULT_SUCCESS;
    }

    for(uint32 i = 0 ; i < qinfo.m_Customerlist.size(); i++)
    {

        CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_IMPORTANT,"1before Query  has %u in team %u level %u requesttime %u!",
                  qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].teamID,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);
    }
    //����
    if(qinfo.m_Customerlist.size() > 0)
    {
        sort(qinfo.m_Customerlist.begin(),qinfo.m_Customerlist.end(),SortQueueMethod);
		 CheckAndCleanQueue(m_pRouter,qinfo);
    }


	
    uint32 buyerId = 0;
    uint32 pos = 0;
    uint32 id;
    Json::Value node_queue;
    Json::Value json_queue;
    Json::Value node_subaccount;
    Json::Value json_subaccount;
    Json::Value json_write;
    Json::FastWriter json_writer;
    for(pos = 0; pos < qinfo.m_Customerlist.size(); pos++)
    {
        buyerId = qinfo.m_Customerlist[pos].userid;
        int ret = GetCSSubAccount_x(m_pRouter,teamID,buyerId,ids,id);
        if(id != 0)
        {
            //�����ɹ��������Ŷ�

            shared_ptr<CUser> pUser = userManager.Find(buyerId);
            if(pUser != NULL)
            {
                pUser->setCsuserId(id);
                pUser->setCsTeamId(teamID);
                tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID pUTeamID;
                pUTeamID.id = buyerId;
                pUTeamID.teamID = teamID;
				pUTeamID.csid = id;
                uint8 count = 1;
                CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
                if(RESULT_FAILED == pGlobalBusi->ModiUserTeamID(&pUTeamID,count))
                {
                    CLog::Log("PushQueueSubAccountToKafka", CLog::TYPE_ERROR,"ȫ���޸����û�: %u ʧ��!", buyerId);
                }
                //��ȫ��memcachedɾ���ŶӶ���

                tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
                cSinfo.customerID = buyerId;
                cSinfo.teamID = teamID;

                CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"1��ȫ��memcachedɾ���Ŷ��û� %u!",cSinfo.customerID);
                ret = m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
                if(ret != MEMCACHED_GET_INFO_SUCCESS)
                {
                    CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
                }
                SubAccountInf_t Subinfo;

                GetSubAccountInfo(m_pRouter,id,Subinfo);
                node_subaccount["BuyerId"] = buyerId;
                node_subaccount["MerchantId"]=Subinfo.id;
                node_subaccount["Email"]=Subinfo.strEmail;
                node_subaccount["UserName"]=Subinfo.strUsername;
                node_subaccount["NickName"]=Subinfo.strNickname;
                node_subaccount["Mobile"]=Subinfo.strMobile;
                node_subaccount["UserSign"]=Subinfo.usersign;
                node_subaccount["FaceUrl"]=Subinfo.face_url;
                json_subaccount.append(node_subaccount);
            }
            else
            {
                break;
            }
        }
        else if(ret == NO_CS_ONLINE)
        {
            break;
        }
        else if(ret == NO_USABLE_CS)
        {
            break;
        }
    }

    //��ȫ��memcachedɾ���ŶӶ���
    int first_queue_pos = pos;
    time_t tm;
    time(&tm);

    json_write["SubAccount"] = json_subaccount;
	int everytime = 1;	
    for(; pos < qinfo.m_Customerlist.size() ; pos++,everytime++)
    {    	
        node_queue["Id"] = qinfo.m_Customerlist[pos].userid;
        node_queue["Pos"] = CalcQueueLength(pos+1-first_queue_pos);
		json_queue.append(node_queue);
		if(everytime % 10 == 0)
        {	
			json_write["GroupId"] = teamID;
   		    json_write["TimeStamp"]=(uint32)tm;
        	json_write["Queue"] = json_queue;
			json_write["isFinish"] = false;	

			string json_str = json_writer.write(json_write);
		    uint32 len = strlen(json_str.c_str());
		    if(len > MAX_JSON_BUFFER-1)
		        len = MAX_JSON_BUFFER-1;
		    char w_str[MAX_JSON_BUFFER] = {0};
		    strncpy(w_str,json_str.c_str(),len);
		    w_str[len] = 0;

			CAppManager::GetInstance().GetKafKaMsgBuffer()->Push((BYTE*)w_str,len);
			json_write.clear();
			json_queue.clear();	
			json_write["SubAccount"] = "";
		}
    }

	json_write["GroupId"] = teamID;
	json_write["TimeStamp"]=(uint32)tm;
	json_write["Queue"] = json_queue;
	json_write["isFinish"] = true;	

    string json_str = json_writer.write(json_write);
    uint32 len = strlen(json_str.c_str());
    if(len > MAX_JSON_BUFFER-1)
        len = MAX_JSON_BUFFER-1;
    char w_str[MAX_JSON_BUFFER] = {0};
    strncpy(w_str,json_str.c_str(),len);
    w_str[len] = 0;

	CAppManager::GetInstance().GetKafKaMsgBuffer()->Push((BYTE*)w_str,len);


    //SendWebQueueInfoIND(m_SendPacket);


    CLog::Log("PushQueueSubAccountToKafka", CLog::TYPE_IMPORTANT, "1�ӱ����ŶӶ���ɾ���û�,֪ͨ�����ͷ� �����ؿͷ�Ϊid: %u",id);
    XT_WAITQUEU_CHANGE QueueInd;
    QueueInd.idBuyer = buyerId;
    QueueInd.teamId = teamID;
    ClientService *clientService = CAppManager::GetInstance().GetCSManager().Find(id);
    if(clientService)
    {
        CLog::Log("PushQueueSubAccountToKafka", CLog::TYPE_IMPORTANT, "���ҵ��ͷ������� id: %u",id );
        //�ӱ����ŶӶ���ɾ���û�,֪ͨ�����ͷ�
        QueueInd.bAdd = XT_WAITQUEU_CHANGE::TYPE_DELETE;
        clientService->NotifyTeamQueue(m_pRouter->GetGlobalBusi(), &QueueInd,1);
    }


    return RESULT_SUCCESS;
}

void SetWebContext(CRouterBusiness* m_pRouter,uint32 toid,uint32 csid,uint32 teamid)
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
        CLog::Log("SetWebContext",CLog::TYPE_IMPORTANT,
                  "֪ͨ������ %u ����web�û� %d�ĽӴ���Ϣ�ڿͷ�%u ��%u !",
                  UserInfo.srvip,toid,csid,teamid);
    }
}

void CsCustOper(CRouterBusiness* m_pRouter, CS_UPDATE_OPER op, uint32 csid, uint32 custid)
{
    // �������û�ڴ˷�������֪ͨ�������������޸�
    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&csid,&UserInfo,count);

    if(UserInfo.uid != csid)
    {
        CLog::Log("CsCustOper",CLog::TYPE_IMPORTANT,
                  "��ѯ�ͷ�ȫ����Ϣʧ��! %u", csid);
    }
    else
    {
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

        char* pMsgData = sendData.data + sizeof(XT_HEAD);

        CSendPacket SendPacket;
        SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

        CsUpdateOperReq req;
        req.set_updateoper(op);
        req.set_custid(custid);
        req.set_csid(csid);
        char szData[1024] = {0};
        int nDataLength = req.ByteSize();
        req.SerializeToArray(szData, nDataLength);
        SendPacket.WriteData(szData,nDataLength);

        sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_UPDATE_CSINFO_REQ,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0,0,UserInfo.srvip, PORT_IM_SVRP2P);
    }
}


//�ͷ�������Լ����ҿ��ܲ���ͬһ����������
int PushQueueToKafka(CRouterBusiness* m_pRouter,uint32 teamID,uint32 buyerId,uint32 id)
{
	CUserManager userManager(m_pRouter->GetGlobalBusi());
    vector<uint32> ids;
    CDataBaseMini *pDatabase = m_pRouter->GetDataBaseMini();

    int ret = pDatabase->CS_Get_Group_Account(teamID,ids);
    if(ret != DB_SUCCESS)
    {
        CLog::Log("PushQueueToKafka",CLog::TYPE_ERROR,"Query team %u subaccount failed!",teamID);
    }
    else
    {
        for(int i = 0 ; i < (int)ids.size(); i++)
        {

            CLog::Log("PushQueueToKafka",CLog::TYPE_IMPORTANT,"Query team %u has member %u !",teamID,ids[i]);
        }
    }

    tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo qinfo;
    int count_Q = 1;
    ret = m_pRouter->GetGlobalBusi()->QueryCSQueueInfo((const uint32 *)&teamID,(uint8)count_Q,(tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo *)&qinfo);
    {
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            if(ret == MEMCACHED_NO_INFO)
            {
                CLog::Log("PushQueueToKafka",CLog::TYPE_IMPORTANT,"����%dδ�����Ŷ�",qinfo.teamID);
                //return RESULT_SUCCESS;
            }
            else
            {
                CLog::Log("PushQueueToKafka",CLog::TYPE_ERROR,"Query team %u fail!",qinfo.teamID);
                //return RESULT_SUCCESS;
            }
        }
    }
    if(qinfo.m_Customerlist.size() > 0)
    {
        //return RESULT_SUCCESS;


        for(uint32 i = 0 ; i < qinfo.m_Customerlist.size(); i++)
        {

            CLog::Log("PushQueueToKafka",CLog::TYPE_IMPORTANT,"1before Query  has %u in team %u level %u requesttime %u!",
                      qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].teamID,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);
        }
        //����
        if(qinfo.m_Customerlist.size() > 0)
        {
            sort(qinfo.m_Customerlist.begin(),qinfo.m_Customerlist.end(),SortQueueMethod);
			CheckAndCleanQueue(m_pRouter,qinfo);
        }



    }
    uint32 pos = 0;
    Json::Value node_queue;
    Json::Value json_queue;
    Json::Value node_subaccount;
    Json::Value json_subaccount;
    Json::Value json_write;
    Json::FastWriter json_writer;
    if(id != 0)
    {
        //�����ɹ��������Ŷ�

        shared_ptr<CUser> pUser = userManager.Find(buyerId);
        if(pUser != NULL)
        {
            pUser->setCsuserId(id);
            pUser->setCsTeamId(teamID);
        }
        else
        {
            SetWebContext(m_pRouter,buyerId,id,teamID);
        }
        tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID pUTeamID;
        pUTeamID.id = buyerId;
        pUTeamID.teamID = teamID;
		pUTeamID.csid = id;
        uint8 count = 1;
        CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
        if(RESULT_FAILED == pGlobalBusi->ModiUserTeamID(&pUTeamID,count))
        {
            CLog::Log("PushQueueToKafka", CLog::TYPE_ERROR,"ȫ���޸����û�: %u ʧ��!", buyerId);
        }

        SubAccountInf_t Subinfo;

        GetSubAccountInfo(m_pRouter,id,Subinfo);

        node_subaccount["BuyerId"] = buyerId;
        node_subaccount["MerchantId"]=Subinfo.id;
        node_subaccount["Email"]=Subinfo.strEmail;
        node_subaccount["UserName"]=Subinfo.strUsername;
        node_subaccount["NickName"]=Subinfo.strNickname;
        node_subaccount["Mobile"]=Subinfo.strMobile;
        node_subaccount["UserSign"]=Subinfo.usersign;
        node_subaccount["FaceUrl"]=Subinfo.face_url;
        json_subaccount.append(node_subaccount);
    }

    //��ȫ��memcachedɾ���ŶӶ���
    int first_queue_pos = pos;
    time_t tm;
    time(&tm);

    json_write["SubAccount"] = json_subaccount;
	int everytime = 1;	
    for(; pos < qinfo.m_Customerlist.size() ; pos++,everytime++)
    {    	
        node_queue["Id"] = qinfo.m_Customerlist[pos].userid;
        node_queue["Pos"] = CalcQueueLength(pos+1-first_queue_pos);
		json_queue.append(node_queue);
		if(everytime % 10 == 0)
        {	
			json_write["GroupId"] = teamID;
   		    json_write["TimeStamp"]=(uint32)tm;
        	json_write["Queue"] = json_queue;
			json_write["isFinish"] = false;	

			string json_str = json_writer.write(json_write);
		    uint32 len = strlen(json_str.c_str());
		    if(len > MAX_JSON_BUFFER-1)
		        len = MAX_JSON_BUFFER-1;
		    char w_str[MAX_JSON_BUFFER] = {0};
		    strncpy(w_str,json_str.c_str(),len);
		    w_str[len] = 0;

			CAppManager::GetInstance().GetKafKaMsgBuffer()->Push((BYTE*)w_str,len);
			json_write.clear();
			json_queue.clear();
			json_write["SubAccount"] = "";
		}
    }

	json_write["GroupId"] = teamID;
	json_write["TimeStamp"]=(uint32)tm;
	json_write["Queue"] = json_queue;
	json_write["isFinish"] = true;	

    string json_str = json_writer.write(json_write);
    uint32 len = strlen(json_str.c_str());
    if(len > MAX_JSON_BUFFER-1)
        len = MAX_JSON_BUFFER-1;
    char w_str[MAX_JSON_BUFFER] = {0};
    strncpy(w_str,json_str.c_str(),len);
    w_str[len] = 0;

	CAppManager::GetInstance().GetKafKaMsgBuffer()->Push((BYTE*)w_str,len);
    return RESULT_SUCCESS;
}




void CheckAndCleanQueue(CRouterBusiness* m_pRouter,vector<CSQList_t> &m_Customerlist)
{
	for(vector<CSQList_t>::iterator iter = m_Customerlist.begin();iter != m_Customerlist.end();)
	{

		//CLog::Log("PushQueueSubAccountToKafka",CLog::TYPE_IMPORTANT,"1after Query  has %u in team %u level %u requesttime %u!",
		//		  qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].teamID,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);
		bool state = false;
		int ret = 0;
		if( CheckQueueUserState(m_pRouter,iter->userid,state)) //http�ӿڳɹ�
		{
			if(state == true) //����
			{
				break;
			}
			else  //���û������ߣ�ɾ�����û��Ŷ���Ϣ
			{
				//��ȫ��memcachedɾ���ŶӶ���
				tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
				cSinfo.customerID = iter->userid;
				cSinfo.teamID = iter->teamID;
				uint8 count = 1;
				CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"��� %u �Ѿ������ߣ��Ӷ���%u �� ɾ��!",cSinfo.customerID,cSinfo.teamID);
				ret = m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
				if(ret != MEMCACHED_GET_INFO_SUCCESS)
				{
					CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
				}
				iter = m_Customerlist.erase(iter);
				continue;
			}
			iter++;
		}
		else
		{
			CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
			break;
		}
		
	}
	return;
}


void CheckAndCleanQueue(CRouterBusiness* m_pRouter,tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo &qinfo)
{
	CheckAndCleanQueue(m_pRouter,qinfo.m_Customerlist);
	return;
}

bool CheckQueueUserState(CRouterBusiness* m_pRouter,uint32 userid,bool &state)
{
	CHttpClient getRequest;
	string datatmp;
	string sPostParam;
	CConfigInfo config;
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userIP;
	uint8 count = 1;
	int ret = 0; 
	ret = m_pRouter->GetGlobalBusi()->QueryUserInfo((const uint32 *)&userid,count,&userIP);
	{
		if(ret != MEMCACHED_GET_INFO_SUCCESS)
		{
			if(ret == MEMCACHED_NO_INFO)	//�û������ߣ�ֱ��ɾ��
			{
				CLog::Log("CheckQueueUserState",CLog::TYPE_IMPORTANT,"No user %d info",userid);
				state = false;
				return true;
				//return RESULT_SUCCESS;
			}
			else		//״̬�쳣�˳�
			{
				CLog::Log("CheckQueueUserState",CLog::TYPE_ERROR,"Query user %u fail!",userid);
				state = false;
				return false;
				//return RESULT_SUCCESS;
			}
		}
		else
		{
			if(userIP.clientip == 0)
			{
				CLog::Log("CheckQueueUserState",CLog::TYPE_IMPORTANT,"No user %d info and ip",userid);
				state = false;
				return true;
			}
		}
	}
	sockaddr_in p;	
	char szClientIP[64] = {0};
	char userid_ch[32] = {0};
	p.sin_addr.s_addr=userIP.clientip;
	strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));	
	sprintf(userid_ch,"%u",userid);
	
	char QueueUsercmd[] = ":8080/webim/vender?VENDER_INFO_TYPE=QUERY_USER_LOGIN_INFO&userIDList=";
	//sPostParam += config.GetimapiUrl();
//	sPostParam += "http://storeservice.beta.fn.com/merchantStore/getMerchantStore/";
//	sPostParam += "http://10.211.240.128/webim-emerge/vender?VENDER_INFO_TYPE=QUERY_USER_LOGIN_INFO&userIDList=";
	sPostParam += "http://";
	sPostParam += szClientIP;
	sPostParam += QueueUsercmd;
	sPostParam += userid_ch;
	
	CLog::Log("CheckQueueUserState",CLog::TYPE_IMPORTANT,"����GET����Ϊ sPostParam %s",sPostParam.c_str());
	int res = getRequest.Get(sPostParam,datatmp);
	/*
	{"success":true,"data":{"userLoginInfo":{"5002714":false,"1":false,"2":false,"3":false}},"errorMessage":null,"needLogin":false,"errorCode":200}
	*/
	// 0 Ϊ�ɹ�
    //�жϻ�ȡ�Ƿ�ɹ�
    if (res != CURLE_OK)
    {        
		CLog::Log("CheckQueueUserState",CLog::TYPE_ERROR,"ʧ�ܣ����ؽ��Ϊres %d data %s",res,datatmp.c_str());
        
        return false;
    }	
	
    //����json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("CheckQueueUserState",CLog::TYPE_DEBUG,"JSON:%s",datatmp.c_str());
    if (reader.parse(datatmp, recv))
    {

		//{"message":null,"success":true,"errorCode":0,"data":{"data":{"185":0}}}
		
        if(!recv.isMember("errorMessage"))
        {
            return false;
        }
        bool success = recv["success"].asBool();
        if(success == false)
        {
            //�û�������
            CLog::Log("CheckQueueUserState",CLog::TYPE_ERROR,"����ʧ��");
            return false;
        }
        else
        {   
        	if(recv["data"].isMember("userLoginInfo"))
	        {
	        	if(recv["data"]["userLoginInfo"].isMember(userid_ch))
	        	{
	        		state = recv["data"]["userLoginInfo"][userid_ch].asBool();
					return true; 
				}
        	}
            //�ҵ����Ӵ�
            return true;
        }
    }
    else
    {
        //json������ʧ��
        CLog::Log("CheckQueueUserState", CLog::TYPE_ERROR, "���� data:%s ,jsonPraseFail", datatmp.c_str());
        return false;
    }	

}


#endif

