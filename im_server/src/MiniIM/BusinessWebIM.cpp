#include "stdafx.h"
#include "BusinessWebIM.h"
#include "AppManager.h"
#include "ErrorCode.h"
#include <curl/curl.h>
#include "ConfigInfo.h"
#include "json/json.h"
#include <string>
#include "ClientProtocol.h"
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>
#include <event.h>
#include <evhttp.h>
#include <signal.h>
#include "../DataBaseLib/MysqlLib.h"
#include "json/json.h"
#include "Function.h"
#include "BusiCacheInterface.h"
#include "kafukaProducer.h"
#include "UserManager.h"

#include <vector>
#define MAX_SEARCH_FRINED_RETURN_NUM 50



/*
void  CommonSendP2PMsg(XT_TALK xtMsg, uint32 msgDataLen)
{
    //消息发送结构体
    SendDataStruct sendData;

    //消息体
    char* pMsgData = sendData.data + sizeof(XT_HEAD);
    CSendPacket SendPacket;
    SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);


    //服务器转发消息
    XT_SERVER_TRANSMIT SrvTransmit;
    SrvTransmit.origin_id = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
    SrvTransmit.msgtype = CMD_P2PMSG_SEND;

    //msg内容
    SendPacket << SrvTransmit.origin_id << SrvTransmit.msgtype;
    SendPacket << xtMsg.from_id << xtMsg.to_id <<(uint8)xtMsg.data_type << (uint16)0 << xtMsg.send_time+1 << xtMsg.recv_flag;
    //文字消息加上样式
    if(xtMsg.data_type == XT_MSG::IM_MSG || xtMsg.data_type == XT_MSG::SF_IMG_URL)
        SendPacket << xtMsg.fontSize << xtMsg.fontColor << xtMsg.fontStyle << xtMsg.fontName;
    SendPacket.WriteData(xtMsg.data, msgDataLen);
    //文字消息加上昵称
    if(xtMsg.data_type == XT_MSG::IM_MSG || xtMsg.data_type == XT_MSG::SF_IMG_URL)
        SendPacket << xtMsg.from_nickname;

    //消息体长度
    sendData.data_len = (uint16)sizeof(XT_HEAD) + SendPacket.GetSize();

    //消息头
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
    CAppManager::GetInstance().InitHead(lpSendHead, CMD_SERVERMSG_IND,(uint16)(SendPacket.GetSize()), xtMsg.to_id, XT_HEAD::DEST_TYPE_USER);

    //发送消息
    CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data, sendData.data_len, SEND_RETRY_TIME);

}
*/

Testpacket::Testpacket()
{
    //CMemCBusi m_cMemCBusi;


}
Testpacket::~Testpacket()
{


}



void Testpacket::InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 did,uint8 dtype , uint8 flag )
{
    lpHead->cmd = cmd;
    lpHead->did = did;
    lpHead->dtype = dtype;
    lpHead->flag = flag;
    lpHead->from = 0x9999;  //版本号
    lpHead->len = len;
    lpHead->seq = 0;
    lpHead->sid = 0;
}


void Testpacket::generate_pkt(uint32 fd,int cmd,RecvDataStruct *RecvData)
{

    unsigned char digest[16];

    char password[255] = "fn123456";

    //sprintf(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",req.password);
    //sprintf(password,"%s",);

    MD5_CTX md5T;
    md5T.MD5Update ((unsigned char*)password,strlen(password) );
    md5T.MD5Final (digest);

    string sHex=ToHexString(digest,16);

    md5T.MD5Update ((unsigned char*)sHex.c_str(), sHex.length());
    md5T.MD5Final (digest);

    char str[2]= {0};
    char crypt[16]; //对一个空字串,用登录密码的MD5(MD5(password))进行tea加密
    //作为验证码,发送到服务器
    int crypt_len;

    xTEAEncryptWithKey(str,1,(char *)digest,crypt,&crypt_len);

    //Build login package

//      RecvDataStruct RecvData;
    RecvData->src_ip = 111111111;
    RecvData->port = 7788;

    RecvData->src_type = 1;
    RecvData->data_len = 1;
    RecvData->vlink = 1;

    CSendPacket sendPkt((char *)RecvData->data);

    XT_HEAD xtHead;

    InitHead(&xtHead,CMD_GET_SUB_ACCOUNT_REQ,sendPkt.GetSize(),0,XT_HEAD::DEST_TYPE_USER,XT_LOGIN_REQ::LOGINBYNAME);

    sendPkt.SetXtHead(xtHead);

#if 0

    if ((req.login_flag & 0x3) == XT_LOGIN_REQ::LOGINBYNAME)//用户名登录
        sendPacket<<req.login_flag<<req.uLoginKey.username;
    else if ((req.login_flag & 0x3) == XT_LOGIN_REQ::LOGINBYMOBILENO) //手机号登陆
        sendPacket<<req.login_flag<<req.uLoginKey.username;
    else
        sendPacket<<req.login_flag<<req.uLoginKey.id;
#endif

    sendPkt<<(uint32)2000000008 <<(uint32)100 << (uint8)2;
    //char random_key[16] = "123456";

    //  memcpy((char *)req.random_key,123456,16);
    //
//      sendPkt.WritePacket((char *)random_key,sizeof(random_key));

    //  char cryptData[MAX_SEND_BUFFERS];
//      CSendPacket cryptPacket(cryptData);
    // 写验证码
    //cryptPacket.WritePacket(crypt,16);
//      cryptPacket.WritePacket(password,sizeof(password));
//      uint32 nLocalIp = 11111111;
//      cryptPacket<<(uint32)0<<nLocalIp<<(uint16)1100;
//      xTEAEncryptWithKey(cryptPacket.GetData(),cryptPacket.GetSize(),
//          (char *)random_key,
//          cryptData,&crypt_len);

//      sendPkt.WriteData(cryptData,crypt_len);
    RecvData->data_len =sendPkt.GetSize();

    return;


}

void* Testpacket::TestCSFlow( LPVOID lpParam )  //测试memcached队列
{


    Testpacket *params =(Testpacket *)lpParam;


    shared_ptr<CUser> pUser = NULL;
    ClientService *clientService = NULL;
    ClientService *clientService1 = NULL;

    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
    CUserManager userManager(&GlobalBusi);
    CLog::Log("Testpacket",CLog::TYPE_IMPORTANT,"开始倒计时30秒!");

    //客服组是100   客服id 2000000008
    //买家id  0-9

    sleep(2);

    uint32 team_id = 100;
    uint32 csid = 2000000008;
    uint32 csFriend = 2000000006;
    uint32 member[2] = {2000000006,2000000008};
    clientService = csManager.AddHash(csid);
    clientService1 = csManager.AddHash(csFriend);

    clientService->AddTeamMatesList(team_id,csid);
    clientService->AddTeamMatesList(team_id,csFriend);

    clientService->m_MaxRecConsult = 1;
    strcpy( clientService->m_CSname,"test_1");


    clientService1->AddTeamMatesList(team_id,csid);
    clientService1->AddTeamMatesList(team_id,csFriend);

    clientService1->m_MaxRecConsult = 1;
    strcpy( clientService1->m_CSname,"test_2");

    clientService->setCSID(csid);
    clientService1->setCSID(csFriend);

    //clientService = csManager.AddHash(i);
    for(int m = 0 ; m < 2 ; m++)
    {

        CLog::Log("Testpacket",CLog::TYPE_IMPORTANT,"开始模拟登录用户!");
        pUser = userManager.CreateNew();
        pUser->m_nID = member[m];
        strcpy_s(pUser->m_szUserName,"XXX");


        //更新用户状态

        pUser->m_nStatus = 0;
        pUser->m_nIP     = 0;
        pUser->m_nPort   = 0 ;
        pUser->m_nLanIP  = 0;
        pUser->m_nLanPort= 0;
        pUser->m_nNewlyLiveTime = timeGetTime();
        pUser->m_nServerIp = 0;
        pUser->flag     = 1;
        time_t tm;
        time(&tm);
        pUser->m_nLoginTime = (uint32)tm;

        GlobalBusi.AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,0);
    }


    CSinfo_t csinfo;
    uint8 count = 1;
    for(int i = 0 ; i < 2 ; i++)
    {
        sprintf(csinfo.m_Csname,"test_%d",i);
        csinfo.m_CurConsult = 0;
        csinfo.m_CurReception = 0;
        csinfo.m_MaxRecConsult = 1;
        csinfo.m_nID = member[i];
        csinfo.m_Customerid.clear();



        GlobalBusi.AddModiCSInfo(&csinfo,count,tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_ADD);
    }


    RecvDataStruct packets;
    (params->generate_pkt)(1,1,&packets);

    (params->m_pfnUserCallback)((params->m_pUserKey),NE_RECVDATA,&packets);


    while(1)
    {
        sleep(1);
    }

    //判断是否排队
    return NULL;




}



void* Testpacket::TestThread( LPVOID lpParam )  //测试memcached队列
{
    //ClientService *clientService = NULL;
    //ClientService *clientService1 = NULL;
    //CsQueue * csQueue = NULL;

    //CUserManager & cuserManager = CAppManager::GetInstance().GetUserManager();
    //CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    //CSQueueManager & csQueueManager = CAppManager::GetInstance().GetCSQueueManager();
    CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
    CLog::Log("Testpacket",CLog::TYPE_IMPORTANT,"开始倒计时30秒!");
    tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo qinfo;
    tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
    uint8 count = 1;
    int ret;


    sleep(5);


    /***********************************************/
    /*              增加排队队列                */
    /***********************************************/

    for(int i = 1 ; i < 100 ; i++)  //增加买家信息
    {

        cSinfo.customerID = i;
        cSinfo.teamID = 185;
		cSinfo.customerLevel = 0;
		time_t tm;
		time(&tm);		
		cSinfo.customerReqTime = uint32(tm);
        CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_ADD user %u!",cSinfo.customerID);
        ret = GlobalBusi.AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_ADD);
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_ADD user %u failed!");
        }
        CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_ADD user OVER %u!",cSinfo.customerID);


    }

	
#if 0

    ret = GlobalBusi.QueryCSQueueInfo(&cSinfo.teamID,count,&qinfo);
    {
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("QueryCSQueueInfo",CLog::TYPE_IMPORTANT,"Query team %u failed!",qinfo.teamID);
        }
    }
    for(int i = 0 ; i < (int)qinfo.m_Customerlist.size(); i++)
    {

        CLog::Log("QueryCSQueueInfo",CLog::TYPE_IMPORTANT,"Query team %u has %u level %u requesttime %u!",
                  qinfo.teamID,qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);

    }


    /***********************************************/
    /*              删除排队队列元素        */
    /***********************************************/
    for(int i = 0 ; i < 100 ; i++)  //删除排队信息
    {

        cSinfo.customerID = i;
        cSinfo.teamID = 100;

        CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_DEL user %u!",cSinfo.customerID);
        ret = GlobalBusi.AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("AddModiDelCSQueueInfo",CLog::TYPE_IMPORTANT,"ACTION_DEL user %u failed!");
        }
    }

    qinfo.m_Customerlist.clear();
    qinfo.teamID = 0;
    ret = GlobalBusi.QueryCSQueueInfo(&cSinfo.teamID,count,&qinfo);
    {
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"Query team %u failed!",qinfo.teamID);
        }
    }
    for(int i = 0 ; i < (int)qinfo.m_Customerlist.size(); i++)
    {


        CLog::Log("QueryCSQueueInfo",CLog::TYPE_IMPORTANT,"Query team %u has %u level %u requesttime %u!",
                  qinfo.teamID,qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);

    }




    /***********************************************/
    /*              删除排队队列                */
    /***********************************************/

    CLog::Log("QueryCSQueueInfo",CLog::TYPE_IMPORTANT,"删除排队队列 %u has %u !",qinfo.teamID);

    ret = GlobalBusi.RemoveCSQueueInfo(&cSinfo.teamID,count);
    if(ret != MEMCACHED_GET_INFO_SUCCESS)
    {
        CLog::Log("QueryCSQueueInfo",CLog::TYPE_IMPORTANT,"删除排队队列 %u failed!",qinfo.teamID);
    }

    qinfo.m_Customerlist.clear();
    qinfo.teamID = 0;
    ret = GlobalBusi.QueryCSQueueInfo(&cSinfo.teamID,count,&qinfo);
    {
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"Query team %u failed!",qinfo.teamID);
        }
    }
    for(int i = 0 ; i < (int)qinfo.m_Customerlist.size(); i++)
    {


        CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"Query team %u has %u level %u requesttime %u!",
                  qinfo.teamID,qinfo.m_Customerlist[i].userid,qinfo.m_Customerlist[i].Level,qinfo.m_Customerlist[i].requestTime);

    }

#endif

#if 0

/***********************************************/
    /*              增加客服信息                */
    /***********************************************/

    CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"增加客服信息 %u has %u !",qinfo.teamID);
    CSinfo_t csinfo;

    for(int i = 0 ; i < 5 ; i++)
    {
        sprintf(csinfo.m_Csname,"test_%d",i);
        csinfo.m_CurConsult = i;
        csinfo.m_CurReception = i;
        csinfo.m_MaxRecConsult = 100;
        csinfo.m_nID = i;
        csinfo.m_CurReception = 1;
        csinfo.m_Customerid.clear();
        for(int j = 0 ; j < 5; j++)
        {
            csinfo.m_Customerid.push_back(j+i);
        }

        GlobalBusi.AddModiCSInfo(&csinfo,count,tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_ADD);
    }
    CSinfo_t csinfo_reply;

    for(uint32 i = 0 ; i < 5 ; i++)
    {
        csinfo_reply.m_Customerid.clear();
        GlobalBusi.QueryCSInfo(&i,count,&csinfo_reply);

        CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,
                  "查询到客服信息 %u  状态 %u 当前咨询 %u 当前接待 %u 最大接待 %u 用户名 %s !",
                  csinfo_reply.m_nID,csinfo_reply.m_State,
                  csinfo_reply.m_CurConsult,
                  csinfo_reply.m_CurReception,
                  csinfo_reply.m_MaxRecConsult,
                  csinfo_reply.m_Csname);
        for(int j = 0; j < (int)csinfo_reply.m_Customerid.size(); j++)
        {
            CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,
                      "查询到客服信息 %u  接待用户为 %u !",
                      csinfo_reply.m_nID,csinfo_reply.m_Customerid[j]);
        }
    }

#if 1
    /***********************************************/
    /*              修改客服信息                */
    /***********************************************/
    CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"修改客服信息 %u has %u !",qinfo.teamID);

    for(int i = 0 ; i < 5 ; i++)
    {
        sprintf(csinfo.m_Csname,"test_%d",i);
        csinfo.m_CurConsult = i;
        csinfo.m_CurReception = i;
        csinfo.m_MaxRecConsult = 50;
        csinfo.m_nID = i;
        csinfo.m_CurReception = 2;
        csinfo.m_Customerid.clear();
        for(int j = 0 ; j < 5; j++)
        {
            csinfo.m_Customerid.push_back(j+i);
        }

        GlobalBusi.AddModiCSInfo(&csinfo,count,tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_MODIFY);
    }
//      CSinfo_t csinfo_reply;

    for(uint32 i = 0 ; i < 5 ; i++)
    {
        csinfo_reply.m_Customerid.clear();
        GlobalBusi.QueryCSInfo(&i,count,&csinfo_reply);

        CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,
                  "查询到客服信息 %u  状态 %u 当前咨询 %u 当前接待 %u 最大接待 %u 用户名 %s !",
                  csinfo_reply.m_nID,csinfo_reply.m_State,
                  csinfo_reply.m_CurConsult,
                  csinfo_reply.m_CurReception,
                  csinfo_reply.m_MaxRecConsult,
                  csinfo_reply.m_Csname);
        for(int j = 0; j < (int)csinfo_reply.m_Customerid.size(); j++)
        {
            CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,
                      "查询到客服信息 %u  接待用户为 %u !",
                      csinfo_reply.m_nID,csinfo_reply.m_Customerid[j]);
        }
    }

    /***********************************************/
    /*              删除客服信息                */
    /***********************************************/


    CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,"删除客服信息 %u has %u !",qinfo.teamID);

    for(uint32 i = 0 ; i < 5 ; i++)
    {
        sprintf(csinfo.m_Csname,"test_%d",i);
        csinfo.m_CurConsult = i;
        csinfo.m_CurReception = i;
        csinfo.m_MaxRecConsult = 100;
        csinfo.m_nID = i;
        csinfo.m_CurReception = 1;
        csinfo.m_Customerid.clear();
        for(int j = 0 ; j < 5; j++)
        {
            csinfo.m_Customerid.push_back(j+i);
        }

        GlobalBusi.DelCSInfo(&i,count);
    }
//      CSinfo_t csinfo_reply;

    for(uint32 i = 0 ; i < 5 ; i++)
    {
        csinfo_reply.m_Customerid.clear();
        GlobalBusi.QueryCSInfo(&i,count,&csinfo_reply);

        CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,
                  "查询到客服信息 %u	状态 %u 当前咨询 %u 当前接待 %u 最大接待 %u 用户名 %s !",
                  csinfo_reply.m_nID,csinfo_reply.m_State,
                  csinfo_reply.m_CurConsult,
                  csinfo_reply.m_CurReception,
                  csinfo_reply.m_MaxRecConsult,
                  csinfo_reply.m_Csname);
        for(int j = 0; j < (int)csinfo_reply.m_Customerid.size(); j++)
        {
            CLog::Log("QueryCSQueueInfo",CLog::TYPE_ERROR,
                      "查询到客服信息 %u	接待用户为 %u !",
                      csinfo_reply.m_nID,csinfo_reply.m_Customerid[j]);
        }
    }
#endif
#endif

    while(1)
    {
        sleep(1);
    }

    //判断是否排队
    return NULL;

}


void* Testpacket::TestThread1( LPVOID lpParam ) //测试排队
{
    shared_ptr<CUser> pUser = NULL;
    ClientService *clientService = NULL;
    ClientService *clientService1 = NULL;
    //CsQueue * csQueue = NULL;

    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
    CUserManager userManager(&GlobalBusi);
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"开始倒计时30秒!");

    //客服组是100   客服id 2000000008
    //买家id  0-9

    sleep(30);

    uint32 team_id = 100;
    uint32 csid = 2000000008;
    uint32 csFriend = 2000000006;
    clientService = csManager.AddHash(csid);
    clientService1 = csManager.AddHash(csFriend);

    //csQueue = csQueueManager.AddHash(team_id);
    clientService->AddTeamMatesList(team_id,csid);
    clientService->AddTeamMatesList(team_id,csFriend);

    clientService1->AddTeamMatesList(team_id,csid);
    clientService1->AddTeamMatesList(team_id,csFriend);

    clientService->setCSID(csid);
    clientService1->setCSID(csFriend);

    for(uint32 i = 0 ; i < 10000 ; i++) //增加买家信息
    {

        CLog::Log("Testpacket",CLog::TYPE_ERROR,"开始模拟登录用户!");
        pUser = userManager.CreateNew();
        //clientService = csManager.AddHash(i);

        pUser->m_nID = i;
        strcpy_s(pUser->m_szUserName,"XXX");


        //更新用户状态

        pUser->m_nStatus = 0;
        pUser->m_nIP     = 0;
        pUser->m_nPort   = 0 ;
        pUser->m_nLanIP  = 0;
        pUser->m_nLanPort= 0;
        pUser->m_nNewlyLiveTime = timeGetTime();
        pUser->m_nServerIp = 0;
        pUser->flag     = 1;
        time_t tm;
        time(&tm);
        pUser->m_nLoginTime = (uint32)tm;


        GlobalBusi.AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,0);
        sleep(5);

        pUser->Reset();
        pUser->m_nID = i;

        //CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"begin AddToOnlineList ");
//        pUser->AddToOnlineList();
        //CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"finish AddToOnlineList ");

        SysStat::GetInstance().addOnlineNum();

        pUser->m_nUsertype = XT_LOGIN_REQ::BUYER;

        //clientService->setCSID(i);
        //clientService->AddToOnlineList();

        XT_WAITQUEU_CHANGE info;
        info.bAdd = XT_WAITQUEU_CHANGE::TYPE_ADD;
        info.idBuyer = i;
        strcpy(info.nicknameBuyer,"123");
        info.teamId = team_id;

        CLog::Log("Testpacket",CLog::TYPE_ERROR,"推送排队!");
        clientService->PushQueue(&GlobalBusi,&info,csid);
        //clientService->NotifyTeamQueue(&info);
        clientService1->PushQueue(&GlobalBusi,&info,csFriend);

        sleep(5);
        //推送排队信息

    }

    while(1)
    {
        sleep(1);
    }

    //判断是否排队
    return NULL;

}


void* Testpacket::TestMemcachedget( LPVOID lpParam )    //测试memecached
{
//  CUser* pUser = NULL;
    Testpacket *Param = (Testpacket*)lpParam;

    CConfigInfo     dbConfig;
    char *memcached_addr = dbConfig.GetMemcachedAddr();
    int memcached_port = dbConfig.GetMemcachedPort();
    ImMemCache memcached = ImMemCache();
    if(memcached.init_ImMemcached(memcached_addr,memcached_port) < 0)
    {

        CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
        return NULL;
    }
    CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),&memcached);
//  CUserManager & cuserManager = CAppManager::GetInstance().GetUserManager();
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"set user");
    uint8  count = 1;
    tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo info;

    for(uint32 i = (uint32)(10000*Param->begin) ; i < (uint32)(10000*(Param->begin+1)) ; i++)   //增加买家信息
    {
        GlobalBusi.QueryUserInfo(&i,count,&info);
    }

    while(1)
    {
        sleep(1);
    }

    //判断是否排队
    return NULL;

}


void* Testpacket::TestMemcachedset( LPVOID lpParam )    //测试排队
{
    shared_ptr<CUser> pUser = NULL;
    Testpacket *Param = (Testpacket*)lpParam;
    CConfigInfo     dbConfig;
    char *memcached_addr = dbConfig.GetMemcachedAddr();
    int memcached_port = dbConfig.GetMemcachedPort();
    ImMemCache memcached = ImMemCache();
    if(memcached.init_ImMemcached(memcached_addr,memcached_port) < 0)
    {

        CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
        return NULL;
    }
    CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),&memcached);
    CUserManager userManager(&GlobalBusi);
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"set user");

    for(uint32 i = (uint32)(1*Param->begin) ; i < (uint32)(1*(Param->begin+1)) ; i++) //增加买家信息
    {
        CLog::Log("Testpacket",CLog::TYPE_IMPORTANT,"开始模拟登录用户!");
        pUser = userManager.CreateNew();
        //clientService = csManager.AddHash(i);
        pUser->m_nID = i;
        strcpy_s(pUser->m_szUserName,"XXX");
        //更新用户状态
        pUser->m_nStatus = 0;
        pUser->m_nIP     = 0;
        pUser->m_nPort   = 0 ;
        pUser->m_nLanIP  = 0;
        pUser->m_nLanPort= 0;
        pUser->m_nNewlyLiveTime = timeGetTime();
        pUser->m_nServerIp = 0;
        pUser->flag     = 1;
		sprintf(pUser->m_SessionKey,"DY123456_%u",i);
        time_t tm;
        time(&tm);
        pUser->m_nLoginTime = (uint32)tm;
        GlobalBusi.AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,0);
#if 0
        uint8  count = 1;
        tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo info;

        //for(uint32 i = (10000*Param->begin) ; i < (10000*(Param->begin+1)) ; i++) //增加买家信息
        {
            GlobalBusi.QueryUserInfo(&i,count,&info);
        }
#endif
    }

    CLog::Log("Testpacket",CLog::TYPE_ERROR,"#######################ready to query!");

    uint8  count = 1;
    tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo info;
    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)    
    {
        GlobalBusi.QueryUserInfo(&i,count,&info);
    }

/*
	//查询用户控制信息
	int CMemCBusi::QueryUserControlInfo(uint32 userId,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo &userControlInfo)
	
	
	int CMemCBusi::GetTUserInfo(uint32 userId,tagGSRV_TUSERINFO_ACK &TUserInfo)
	
	
	
	
	//查询用户状态
	int CMemCBusi::QueryUserStatus(uint32 userId, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus &userStatus)
	
	
	//修改用户状态
	bool CMemCBusi::ModiUserStatus(tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser user)
	
	
	
	//查询分流
	int CMemCBusi::QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow)
	
	
	
	//修改分流
	bool CMemCBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow)
	
	
	//查询用户所在客服组ID
	int CMemCBusi::QueryUserTeamID(uint32 userId, tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID& userTeamID)
	
	
	//修改用户所在客服组ID
	
	bool CMemCBusi::ModiUserTeamID(tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID user)
	
*/

	/*	测试获取用户控制信息*/

	tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo info1;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   //修改用户数据测试
    {
        GlobalBusi.QueryUserControlInfo(&i,&info1,count);
		CLog::Log("QueryUserControlInfo",CLog::TYPE_ERROR,"userid %d,sessionkey %s",info1.uid,info1.sessionKey);
    }



	/*	测试获取用户传输信息*/

	
//	bool ret = true;
	//char *memcached_addr = dbConfig.GetMemcachedAddr();
	//int memcached_port = dbConfig.GetMemcachedPort();
	ImMemCache memcached1 = ImMemCache();
	if(memcached1.init_ImMemcached(memcached_addr,memcached_port) < 0)
	{			
		CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
//		ret = false;
	}		
	CMemCBusi CMemCQuery(&memcached1);

	tagGSRV_TUSERINFO_ACK info2;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
        CMemCQuery.GetTUserInfo(i,info2);
		CLog::Log("QueryUserControlInfo",CLog::TYPE_ERROR,"userid %d,sessionkey %s",info2.id,info2.sessionkey);
    }

	/*	测试用户状态*/

	tagGSRV_GET_USER_STATUS_ACK::tagUserStatus info3;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)  
    {
        GlobalBusi.QueryUserStatus(&i,&info3,count);
		CLog::Log("QueryUserStatus",CLog::TYPE_ERROR,"userid %d,status %d",info3.id,info3.status);
    }


	tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser info4;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
    	info4.id = i;
		info4.status = i*2;
        GlobalBusi.ModiUserStatus(&info4,count,NULL);
		CLog::Log("ModiUserStatus",CLog::TYPE_ERROR,"userid %d,sessionkey %s",info4.id,info4.status);
    }


	tagGSRV_GET_USER_STATUS_ACK::tagUserStatus info5;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
        GlobalBusi.QueryUserStatus(&i,&info3,count);
		CLog::Log("QueryUserStatus",CLog::TYPE_ERROR,"userid %d,status %d",info5.id,info5.status);
    }

	/*	测试用户分流*/



	tagGSRV_GET_USER_FLOW_ACK::tagUserFlow info6;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
        GlobalBusi.QueryUserFlow(&i,&info6,count);
		CLog::Log("QueryUserFlow",CLog::TYPE_ERROR,"userid %d,flow %d",info6.id,info6.flow);
    }


	tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow info7;
	tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow now;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
    	info7.id = i;
		info7.flow = i*2;
        GlobalBusi.ModiUserFlow(&info7,count,&now);
		CLog::Log("ModiUserFlow",CLog::TYPE_ERROR,"userid %d,flow %d",now.id,now.flow);
    }


	tagGSRV_GET_USER_FLOW_ACK::tagUserFlow info8;
	
	for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)	
	{
		GlobalBusi.QueryUserFlow(&i,&info8,count);
		CLog::Log("QueryUserFlow",CLog::TYPE_ERROR,"userid %d,flow %d",info8.id,info8.flow);
	}



	/*	测试用户所在客服组*/
	

	
	tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID info9;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
        GlobalBusi.QueryUserTeamID(&i,&info9,count);
		CLog::Log("QueryUserTeamID",CLog::TYPE_ERROR,"userid %d,teamID %d",info9.id,info9.teamID);
    }


	tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID info10;

    for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)   
    {
    	info10.id = i;
		info10.teamID = i*2;
        GlobalBusi.ModiUserTeamID(&info10,count);
		CLog::Log("ModiUserFlow",CLog::TYPE_ERROR,"userid %d,teamID %d",info10.id,info10.teamID);
    }


	tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID info11;
	
	for(uint32 i = (uint32)(1*Param->begin) ; i < uint32(1*(Param->begin+1)) ; i++)	
	{
		GlobalBusi.QueryUserTeamID(&i,&info11,count);
		CLog::Log("QueryUserTeamID",CLog::TYPE_ERROR,"userid %d,teamID %d",info11.id,info11.teamID);
	}

	
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"FINISHED!");





    while(1)
    {
        sleep(1);
    }

    //判断是否排队
    return NULL;

}
#include "imapi.h"

void* Testpacket::Testzsets1( LPVOID lpParam )    //测试排队
{
	imapi test;
	string info;
	test.imapiReq(0,info,1,2,3,4,5,6,7);
	
    while(1)
    {
    	test.reset();		
		test.imapiReq(0,info,1,2);
        sleep(1);
    }

    //判断是否排队
    return NULL;

}


void* Testpacket::Testzsets( LPVOID lpParam )    //测试排队
{
	using namespace global_cache;

//    CUser* pUser = NULL;
//    Testpacket *Param = (Testpacket*)lpParam;
    CConfigInfo     dbConfig;
    char *memcached_addr = dbConfig.GetMemcachedAddr();
    int memcached_port = dbConfig.GetMemcachedPort();
	global_Cache *Links = new global_Cache(memcached_addr,memcached_port);

	char key0[32] = {0};
	sprintf(key0,"key1");
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"START!");

	char oldval[32] = {0};
	char setval[32] = {0};
	
	for(int i = 0 ; i < 10 ; i++)
	{
		sprintf(setval,"hello#%d",i);
		Links->getset("yeah",setval,oldval);
		printf("getset yeah set %s getold %s\r\n",setval,oldval);
		fflush(stdout);
	}
	
	for(int i = 0 ; i < 10 ; i++)
	{

		char str_i[4] = {0};
		
		sprintf(str_i,"%d",i);
			
		Links->zadd(key0,i,str_i);
		
	}
	int count = 0;
	char value[10][ 1024 ] = {0};
	Links->zrange(key0,0,9,count,value);
	for(int m = 0 ; m < count; m++)
	{		
		CLog::Log("zrange",CLog::TYPE_ERROR,"key1 has %s!",value[m]);
		printf("zrange key1 has %s!",value[m]);
	}
	uint64_t length = 0;
	Links->zcard(key0,&length);
	
    CLog::Log("zcard",CLog::TYPE_ERROR,"TOTAL %lu!",length);
	
	printf("zcard TOTAL %lu!",length);
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"FINISHED!");
	#if 1
    for(int i = 0 ; i < 5 ; i++)
	{

		char str_i[4] = {0};
		
		sprintf(str_i,"%d",i);
			
		Links->zrem(key0,str_i);
		
		printf("zrem delete %s!",str_i);
		
	}
	Links->zrange(key0,0,9,count,value);
	for(int m = 0 ; m < count; m++)
	{		
		CLog::Log("zrange",CLog::TYPE_ERROR,"key1 has %s!",value[m]);
		printf("zrange key1 has %s!",value[m]);
	}
	length = 0;
	Links->zcard(key0,&length);
	printf("zcard TOTAL %lu!",length);

	fflush(stdout);
    CLog::Log("Testpacket",CLog::TYPE_ERROR,"FINISHED!");
#endif

    while(1)
    {
        sleep(1);
    }

    //判断是否排队
    return NULL;

}


int Testpacket::StartWork()
{

    //初始化数据库
    CConfigInfo m_Config;
    for(int i = 0 ; i < 1; i++)
    {
       // this->begin = 5;
        CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            TestThread,               // thread function
            this,                       // argument to thread function
            0,                          // use default creation flags
            NULL);                      // returns the thread identifier
    }

#if 0
    for(int i = 0 ; i < 10; i++)
    {
        this->begin = i;
        CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            TestMemcachedget,               // thread function
            this,                       // argument to thread function
            0,                          // use default creation flags
            NULL);                      // returns the thread identifier
    }
#endif
    return RESULT_SUCCESS;
}





/*

/////////////////////CWebIMMsgCheck.cpp//////////////////////
void  MyWebIMTimeProc(void* dwUser)
{
    CWebIMMsgCheck* pObj = (CWebIMMsgCheck*)dwUser;
    pObj->HandleTimer();
}


void  MyWebIMTimeProcNew(union sigval v)
{
    //CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:3");
    MyWebIMTimeProc(v.sival_ptr);
}


CWebIMMsgCheck::CWebIMMsgCheck()
{
    m_pWebIMMsg = m_WebIMMsg;

    m_hMMT = NULL;

}
CWebIMMsgCheck::~CWebIMMsgCheck(void)
{
    StopWork();
}

int CWebIMMsgCheck::StartWork()
{
    CConfigInfo m_Config;
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
    m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);
    if( !m_pDataBaseMini.ConnectDB() )
    {
        CLog::Log("CTimeLenSave",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return RESULT_FAILED;
    }

    StartTimer();
    return 0;
}

void CWebIMMsgCheck::StopWork()
{
    StopTiemr();
}

void CWebIMMsgCheck::StartTimer()
{
    if(!timeSetEventNew(m_timer, m_nTimerDelay, m_nTimerResolution, MyWebIMTimeProcNew, (void*)this) )
    {
        CLog::Log("CWebIMMsgCheck",CLog::TYPE_ERROR,"WEBIM定时器设置失败!");
    }
}

void CWebIMMsgCheck::StopTiemr()
{
	timer_delete(m_timer);
    
}


void CWebIMMsgCheck::HandleTimer()
{
    uint16 nReqCount=MAX_CHECK_NUM;
    try
    {
//      CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "扫描WEBIM消息及离线消息开始!");

        while(nReqCount>0)
        {
            nReqCount = MAX_CHECK_NUM;
            if ( DB_SUCCESS == m_pDataBaseMini.Get_WebIM_Msg(CAppManager::GetInstance().GetConfig().GetServerNo(),m_pWebIMMsg,nReqCount) )
            {
                for( int i = 0 ; i < nReqCount ; ++i )
                {
                    SendP2PMsg(&m_pWebIMMsg[i]);
                }
            }
//          CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "扫描出WEBIM消息(离线消息) %d 条", nReqCount);
        }

    }
    catch (...)
    {
        CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "发生异常!");
    }
//  CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "扫描WEBIM消息及离线消息结束!");

}

//发送WEBIM互通(离线)消息给相应用户
void CWebIMMsgCheck::SendP2PMsg(ST_WEBIM_MSG *pWebIMMsg)
{
    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    char* pMsgData = sendData.data + sizeof(XT_HEAD);

    CSendPacket SendPacket;
    SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

    if(pWebIMMsg->msglen>0)         //离线消息
    {
        SendPacket.WritePacket( pWebIMMsg->msg, pWebIMMsg->msglen );
        //对特殊离线消息设置离线标志
        //LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT) pMsgData;
        //if( (lpSrvTransmit->msgtype == CMD_CLUSTERMSG_SEND) ||
        //  (lpSrvTransmit->msgtype == CMD_P2PMSG_SEND))
        //{
        //  lpSrvTransmit->msgtype |= 0x1000;
        //}
    }
    else                            //WEBIM互通消息
    {
        XT_MSG xtmsg;
        XT_SERVER_TRANSMIT SrvTransmit;
        SrvTransmit.origin_id=XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
        SrvTransmit.msgtype=CMD_P2PMSG_SEND;
        xtmsg.data_type=0;
        xtmsg.ver=XTREAM_CLIENT_VERSION;
        xtmsg.recv_flag=0;
        xtmsg.fontSize=0x0A;
        xtmsg.fontColor=0;
        xtmsg.fontStyle=0;
        xtmsg.fontName[0]=0xCB;
        xtmsg.fontName[1]=0xCE;
        xtmsg.fontName[2]=0xCC;
        xtmsg.fontName[3]=0x05;
        xtmsg.fontName[4]=0x00;

        SendPacket << SrvTransmit.origin_id
                   << SrvTransmit.msgtype
                   << pWebIMMsg->sendID
                   << pWebIMMsg->recvID
                   << xtmsg.data_type
                   << xtmsg.ver
                   << pWebIMMsg->tm
                   << xtmsg.recv_flag
                   << xtmsg.fontSize
                   << xtmsg.fontColor
                   << xtmsg.fontStyle
                   << xtmsg.fontName;
        SendPacket.WriteData( pWebIMMsg->msgtext, strlen(pWebIMMsg->msgtext)+1 );
        SendPacket << pWebIMMsg->from_nickname;
    }

    sendData.data_len = (uint16)sizeof(XT_HEAD) + SendPacket.GetSize();

    CAppManager::GetInstance().InitHead(
        lpSendHead,
        CMD_SERVERMSG_IND,
        (uint16)(SendPacket.GetSize()),
        pWebIMMsg->recvID);

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(
        sendData.data,sendData.data_len,SEND_RETRY_TIME);
}
*/
/*
static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
}
*/

CHttpServer::CHttpServer()
{
    char *memcached_addr = m_ConfigInfo.GetMemcachedAddr();
    int memcached_port = m_ConfigInfo.GetMemcachedPort();
    memcached = new ImMemCache();
    if(memcached->init_ImMemcached(memcached_addr,memcached_port) < 0)
    {
        CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
    }
    m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),memcached);
    strcpy(m_HttpServerAddr, m_ConfigInfo.GetHttpServerAddr());
}
CHttpServer::~CHttpServer()
{
    SAFE_DELETE(memcached);
    SAFE_DELETE(m_pGlobalBusi);

}

void CHttpServer::MD5Password(char* pPassword)
{
    unsigned char digest[16];

    char password[255] = {0};
    sprintf_s(password,"%s",pPassword);

    MD5_CTX md5T;
    md5T.MD5Init();
    md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
    md5T.MD5Final (digest);

    string sHex=ToHexString(digest,16);

    strcpy(pPassword,sHex.c_str());

}

void CHttpServer::HttpServiceGetWorkGroupInfo(const char *params, Json::Value& jvReply)
{
	Json::Value recv;
	XT_SERVER_USERINFO_GET_ACK userInfo;
	int uid = 0;
	if(jReader.parse(params,recv))
	{
		//params解析
		if(recv["workId"].isInt())
		{
			uid = recv["workId"].asInt();			

			CLog::Log("HttpServiceGetWorkGroupInfo", CLog::TYPE_IMPORTANT, "uid:%d",uid);

			if(m_pDataBaseWeb.Get_User_Info(uid,userInfo) == DB_ERROR)
			{

				jvReply["result"] = false;
				jvReply["error"] = "HttpServiceGetWorkGroupInfo failed";
				CLog::Log("HttpServiceQueryCS", CLog::TYPE_ERROR, "HttpServiceGetWorkGroupInfo Failed! uid:%d ",uid);

			}
			else
			{
				jvReply["workId"] = uid;
				jvReply["username"] = userInfo.username;
				jvReply["nickname"] = userInfo.nickname;
				jvReply["groupname"] = userInfo.workGroup;	
				jvReply["result"] = true;

				CLog::Log("HttpServiceGetWorkGroupInfo", CLog::TYPE_IMPORTANT, "Success!");
			}
		}
		else
		{
			jvReply["result"] = false;
			jvReply["error"] = "json parse faild";
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
		}

	}
	else
	{
		jvReply["result"] = false;
		jvReply["error"] = "json parse faild";
		CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
	}
}



int CHttpServer::GetMerchantStatus(uint8 &cnt, uint32 *id, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus* pUStatus)
{

    for (uint8 i = 0; i < cnt; ++i)
    {
        AccountArray ids;

        //获取子账号
        MYSQL_LIB->getAllSubAccount(id[i], ids);


        //查询子账号状态
        uint8 subAccountsize = ids.size();
        std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(ids.size());
        if (RESULT_FAILED == m_pGlobalBusi->QueryUserStatus(&ids[0], &userStatus[0], subAccountsize))
        {
            CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "QueryUserStatus failed!");
        }
        else
        {
            //判断子账号中是否有在线的
            auto ifind = std::find_if(userStatus.begin(), userStatus.begin() + subAccountsize,
                                      [] (const GSRV_GET_USER_STATUS_ACK::tagUserStatus& a) ->bool
            {
                return a.status != XTREAM_OFFLINE;
            });

            //判断是否在线 0为不在线 1 为在线
            if (ifind == (userStatus.begin() + subAccountsize))
            {
                pUStatus[i].id = id[i];
                pUStatus[i].status = 0;
            }
            else
            {
                pUStatus[i].id = id[i];
                pUStatus[i].status = 1;
            }
        }
    }
    return RESULT_SUCCESS;
}

int CHttpServer::GetUserStatus(uint8 &cnt,uint32*id,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus)
{
    uint8 size = cnt;
    std::vector<GSRV_GET_USER_STATUS_ACK::tagUserStatus> userStatus(cnt);

    //获取用户状态信息
#ifdef IM_USE_CACHE
    if (MEMCACHED_NO_INFO < CMemcacheQueryUserStatusi(m_pGlobalBusi,id,cnt, &userStatus[0]))
    {
        CLog::Log(__FUNCTION__,CLog::TYPE_ERROR, "QueryUserStatus failed!");
        return RESULT_FAILED;
    }
#else
    if( RESULT_FAILED == m_pGlobalBusi->QueryUserStatus(id, &userStatus[0], cnt))
    {
        CLog::Log(__FUNCTION__,CLog::TYPE_ERROR, "QueryUserStatus failed!");
        return RESULT_FAILED;
    }
#endif
    //遍历状态数组，写入数据包
    for (uint8 i = 0; i < size; ++i)
    {
        pUStatus[i].id = id[i];
        //CLog::Log("bwwan",CLog::TYPE_ERROR,"id[i]:%ld,pUStatus[i].id:%ld",id[i],pUStatus[i].id);
        pUStatus[i].status = (userStatus[i].status == XTREAM_OFFLINE ? 0 : 1) ;
    }

    return RESULT_SUCCESS;
}
////客服工作台导入或者修改账号信息
void CHttpServer::HttpServiceIMCSInfo(const char *params, Json::Value& jvReply)
{
    XT_IMPORT_CS_REQ::TagCSInfo CSInfo[200]= {0};

    Json::Value recv;
    if(jReader.parse(params,recv))
    {
        ////params解析
        char username[MAX_USERNAME_LEN+1]= {0};
        char importname[MAX_USERNAME_LEN+1]= {0};
        char importnameutf8[MAX_USERNAME_LEN+1]= {0};
        char usernameutf8[MAX_USERNAME_LEN+1]= {0};
        char realname[MAX_USERNAME_LEN+1]= {0};
        char realnameutf8[MAX_USERNAME_LEN+1]= {0};
        char password[64]= {0};
        int receptionlimit = 0;
        uint32 userid = 0 ;
        int len = recv["importname"].asString().size();
        if (len > 0)
        {
            strncpy(importnameutf8,recv["importname"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
            utf2gbk(importnameutf8,len, importname, MAX_USERNAME_LEN);
        }
        uint8 type = recv["type"].asUInt();
        ////type =0为导入客服信息
        if(type == 0)
        {
            Json::Value jvRootArr = recv["userinfo"];   // 根分组
            int nRootSize = jvRootArr.size();       // 根分组数量
            CLog::Log("HttpServiceIMCSInfo",CLog::TYPE_DEBUG,"ImportuserCount:%d",nRootSize);
            for(int i = 0; i < nRootSize; ++i)
            {
                //解析数组
                Json::Value jvRoot = jvRootArr[i];
                len = jvRoot["username"].asString().size();
                if (len > 0)
                {
                    strncpy(usernameutf8,jvRoot["username"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
                    utf2gbk(usernameutf8,len, CSInfo[i].username, MAX_USERNAME_LEN);
                }
                CSInfo[i].permission = jvRoot["permission"].asInt();
                CSInfo[i].receptionlimit = jvRoot["receptionlimit"].asInt();

                time_t tm;
                time(&tm);
                uint32 importtime = (uint32)tm;


                if(m_pDataBaseMini.Import_CS_Req(CSInfo[i].username,CSInfo[i].permission,CSInfo[i].receptionlimit,importname,userid,importtime) == DB_ERROR)
                {
                    jvReply["result"] = false;
                    jvReply["error"] = "Import_CS_Req faild";
                    CLog::Log("Import_CS_Req", CLog::TYPE_ERROR, "Import CS Failed! name:%s, userid:%d, permission:%d, receptionlimit:%d",
                              CSInfo[i].username,userid,CSInfo[i].permission,CSInfo[i].receptionlimit);
                }
                else
                {
                    //reply赋值
                    jvReply["result"] = true;
                    CLog::Log("Import_CS_Req", CLog::TYPE_IMPORTANT, "Import CS Success!name:%s, userid:%d, permission:%d, receptionlimit:%d,importname:%s",
                              CSInfo[i].username,userid,CSInfo[i].permission,CSInfo[i].receptionlimit,importname);
                }
            }
        }
        //type=1为修改客服信息
        else if(type == 1)
        {
            Json::Value jvRootArr = recv["userinfo"];   // 根分组
            int nRootSize = jvRootArr.size();       // 根分组数量
            uint32 importtime =0;
            int permission = 1;
            int csid = 0;
            CLog::Log("HttpServiceIMCSInfo",CLog::TYPE_DEBUG,"ModifyuserCount:%d",nRootSize);
            for(int i = 0; i < nRootSize; ++i)
            {
                Json::Value jvRoot = jvRootArr[i];
                len = jvRoot["username"].asString().size();
                if (len > 0)
                {
                    strncpy(usernameutf8,jvRoot["username"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
                    utf2gbk(usernameutf8,len, username, MAX_USERNAME_LEN);
                }


                len = jvRoot["realname"].asString().size();
                if (len > 0)
                {
                    strncpy(realnameutf8,jvRoot["realname"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
                    utf2gbk(realnameutf8,len, realname, MAX_USERNAME_LEN);
                }
                receptionlimit= jvRoot["receptionlimit"].asUInt();


                len = jvRoot["password"].asString().size();
                if (len > 0)
                {
                    strncpy(password,jvRoot["password"].asCString(),len > 60 ? 60: len);
                    MD5Password(password);
                }

                csid = jvRoot["csId"].asInt();
                permission = jvRoot["permission"].asInt();//0主管，员工1

            }


            if(m_pDataBaseMini.Modify_CS_Req(username,realname,receptionlimit,password,importtime,csid,permission) == DB_ERROR)
            {
                jvReply["result"] = false;
                jvReply["error"] = "Modify CS faild";
                CLog::Log("ModifyCSReq", CLog::TYPE_ERROR, "Modify CS Failed! name:%s，csRealName:%s, receptionLimit:%d",
                          username,realname,receptionlimit);
            }
            else
            {
                jvReply["result"] = true;
                jvReply["importtime"] = importtime;
                CLog::Log("Modify_CS_Req", CLog::TYPE_IMPORTANT,
                          "Modify CSInfo Success!username:%s,realname:%s,receptionlimit:%d,password:%s,importtime:%d,csid:%d,permission:%d",
                          username,realname,receptionlimit,password,importtime,csid,permission);
            }
        }
    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }
}

//修改账号状态(type :启用1，停用0，删除2)
void CHttpServer::HttpServiceModifyStatus(const char *params, Json::Value& jvReply)
{
    Json::Value recv;
    char username[MAX_USERNAME_LEN+1]= {0};

    if(jReader.parse(params,recv))
    {
        //params解析
        //判断csstatus是否存在
        if(recv.isMember("csstatus"))
        {
            Json::Value recvArr = recv["csstatus"];
            //查看有多少id
            int size = recvArr.size();
            for(int i = 0; i < size; i++ )
            {

                Json::Value jvRoot = recvArr[i];
                int len = jvRoot["username"].asString().size();
                if (len > 0)
                {
                    char usernameutf8[MAX_USERNAME_LEN+1]= {0};
                    strncpy(usernameutf8, jvRoot["username"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
                    utf2gbk(usernameutf8,len, username, MAX_USERNAME_LEN);
                }
                int status = jvRoot["status"].asInt();

                if(m_pDataBaseMini.Modify_CS_Status(username,status) == DB_ERROR)
                {
                    jvReply["result"] = false;
                    jvReply["error"] = "Modify_CS_Status faild";
                    CLog::Log("Import_CS_Req", CLog::TYPE_ERROR, "Modify_CS_Status Failed! name:%s ",username);
                }
                else
                {
                    jvReply["result"] = true;
                    CLog::Log("Modify_CS_Status", CLog::TYPE_IMPORTANT, "Modify Status Success!");
                }
            }
        }
        else
        {
            jvReply["result"] = false;
            jvReply["error"] = "no member";
            CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "no member!");
        }
    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }
}

//查询客服信息
void CHttpServer::HttpServiceQueryCS(const char *params, Json::Value& jvReply)
{
    Json::Value recv;
    Json::Value queryinfo;
    Json::Value queryinfos;
    char username[MAX_USERNAME_LEN+1]= {0};
    char importname[MAX_USERNAME_LEN+1]= {0};
    char realname[MAX_USERNAME_LEN+1]= {0};
    int status = 888;
    XT_CS_INFO CSInfoList[200] ;
    int count = 200;
    uint32 begintime = 0;
    uint32 endtime = 0;
    if(jReader.parse(params,recv))
    {
        //params解析

        int len = recv["username"].asString().size();
        if (len > 0)
        {
            char usernameutf8[MAX_USERNAME_LEN+1]= {0};
            strncpy(usernameutf8,recv["username"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
            utf2gbk(usernameutf8,len, username, MAX_USERNAME_LEN);
        }
        len = recv["importname"].asString().size();
        if (len > 0)
        {
            char importnameutf8[MAX_USERNAME_LEN+1]= {0};
            strncpy(importnameutf8,recv["importname"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
            utf2gbk(importnameutf8,len, importname, MAX_USERNAME_LEN);
        }
        len = recv["realname"].asString().size();
        if (len > 0)
        {
            char realnameutf8[MAX_USERNAME_LEN+1]= {0};
            strncpy(realnameutf8,recv["realname"].asCString(),len > MAX_USERNAME_LEN ? MAX_USERNAME_LEN: len);
            utf2gbk(realnameutf8,len, realname, MAX_USERNAME_LEN);
        }
        status = recv["status"].asInt();
        begintime = recv["begintime"].asUInt();
        endtime = recv["endtime"].asUInt();

        CLog::Log("HttpServiceQueryCS", CLog::TYPE_IMPORTANT, "username:%s,importname:%s,realname:%s,status:%d,begintime:%d,endtime:%d"
                  ,username,importname,realname,status,begintime,endtime);

        if(m_pDataBaseMini.Qurery_CS_Info(CSInfoList,username,importname,realname,status,begintime,endtime,count) == DB_ERROR)
        {

            jvReply["result"] = false;
            jvReply["error"] = "HttpServiceQueryCS failed";
            CLog::Log("HttpServiceQueryCS", CLog::TYPE_ERROR, "HttpServiceQueryCS Failed! name:%s ",username);

        }
        else
        {
            for( int j=0; j<count; j++)
            {
                queryinfo["username"] = CSInfoList[j].username;
                queryinfo["permission"] = CSInfoList[j].permission;
                queryinfo["importname"] = CSInfoList[j].importname;
                queryinfo["importtime"] = CSInfoList[j].importtime;
                queryinfo["realname"] = CSInfoList[j].realname;
                queryinfo["status"] = CSInfoList[j].status;
                queryinfo["receptionlimit"] = CSInfoList[j].receptionlimit;
                queryinfos.append(queryinfo);
            }
            jvReply["result"] = true;
            jvReply["queryinfo"] = queryinfos;

            CLog::Log("HttpServiceQueryCS", CLog::TYPE_IMPORTANT, "HttpServiceQueryCS  Success!");
        }

    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }
}


void CHttpServer::HttpServiceGetUserStatus(const char *params, Json::Value& jvReply)
{
    uint32 id[50] = {0};
    tagGSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[50];

    Json::Value recv;
    Json::Value statuses;
    Json::Value status;

    if(jReader.parse(params,recv))
    {
        //params解析
        uint8 size = recv["ids"].size()<50 ? recv["ids"].size() : 50;
        for(int i=0; i<size; i++)
        {
            id[i] = recv["ids"][i].isInt() ? (uint32)recv["ids"][i].asInt() : 0;
        }

        //获取用户在线状态
        GetUserStatus(size, id, userStatus);

        //reply赋值
        for( int j=0; j<size; j++)
        {
            status["id"] = userStatus[j].id;
            status["status"] = userStatus[j].status;
            statuses.append(status);
        }
        jvReply["result"] = true;
        jvReply["statuses"] = statuses;
    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }

}

void CHttpServer::HttpServiceGetUserInfo(const char *params, Json::Value& jvReply)
{
	CUserManager userManager(m_pGlobalBusi);
    uint32 id[50] = {0};
	shared_ptr<CUser> pUser = NULL;
    Json::Value recv;
    Json::Value statuses;
    Json::Value status;

    if(jReader.parse(params,recv))
    {
        //params解析
        uint8 size = recv["ids"].size()<50 ? recv["ids"].size() : 50;
        for(int i=0; i<size; i++)
        {
            id[i] = recv["ids"][i].isInt() ? (uint32)recv["ids"][i].asInt() : 0;
        }

 		
        //reply赋值
        for( int j=0; j<size; j++)
        {
        	
			//获取用户信息
			pUser = userManager.Find(id[j]);
			if(pUser != NULL)
	        {
				char utf8_username[MAX_USERNAME_LEN*3+1] = {0};
				char utf8_nickname[MAX_NICKNAME_LEN*3+1] = {0};

				gbk2utf8(utf8_username,pUser->m_szUserName);
				gbk2utf8(utf8_nickname,pUser->m_szNickName);
	        	status["id"] = pUser->m_nID;
				status["info"] = true;
				status["status"] = pUser->m_nStatus;
				status["rule"] = pUser->m_AllowRule;
				status["username"] = utf8_username;
				status["nickname"] = utf8_nickname;
				status["clientip"] = pUser->m_nIP;
				status["clientport"] = pUser->m_nPort;
				status["sexy"] = pUser->m_nSexy;
				status["serverip"] = pUser->m_nServerIp;
				status["lanip"] = pUser->m_nLanIP;	
				status["flag"] = pUser->flag;		
				status["logintime"] = pUser->m_nLoginTime;		
				status["sessionkey"] = Base64Encode(pUser->m_SessionKey, MAX_SESSIONKEY_LEN).c_str();
				status["p2psessionkey"] = Base64Encode(pUser->m_P2PSessionKey, MAX_SESSIONKEY_LEN).c_str();
				status["flow"] = pUser->m_nflow;
				status["id"] = pUser->m_nID;		
				status["version"] = pUser->m_ClientVersion;
				status["headURL"] = pUser->m_szHeadURL;
				status["isFullInfoValid"] = pUser->m_bIsFullInfoValid;
				status["eMail"] = pUser->m_szEmail;
				status["birthday"] = pUser->m_szBirthday;
				status["country"] = pUser->m_nCountry;
				status["province"] = pUser->m_nProvince;
				status["city"] = pUser->m_nCity;
				status["career"] = pUser->m_nCareer;
				status["familyFlag"] = pUser->m_FamilyFlag;
				status["money"] = pUser->m_nMoney;
				status["onlineTime"] = pUser->m_nOnlineTime;
				status["linkpop"] = pUser->m_linkpop;
				status["address"] = pUser->m_szAddress;
				status["phone"] = pUser->m_szPhone;
				status["description"] = pUser->m_szDescription;		 
				status["userSign"] = pUser->m_szUserSign;                
				status["mobileStatus"] = pUser->m_nMobileStatus;    		
				status["integral"] = pUser->m_nIntegral;								
				status["u_GUID"] = pUser->m_szGUID;											
				status["newlyLiveTime"] = pUser->m_nNewlyLiveTime;      
				status["loginType"] = pUser->m_nLoginType;							
				status["lanPort"] = pUser->m_nPort;											
				status["level"] = pUser->m_nLevel;											
				status["usertype"] = pUser->m_nUsertype;
				status["csUserID"] = pUser->m_nCSuserid;
				status["csTeamID"] = pUser->m_nCSTeamid;
			}
			else
			{
				status["id"] = id[j];
				status["info"] = false;
			}
	            statuses.append(status);
        }
        jvReply["result"] = true;
        jvReply["statuses"] = statuses;
    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }

}

//将异动信息推送给客服
void CHttpServer::HttpServicePushQueuetoCs(const char *params, Json::Value& jvReply)
{

    Json::Value recv;
    Json::Value statuses;
    Json::Value status;

    if(jReader.parse(params,recv))
    {
        //params解析
		int teamId = 0;
		int queuelength = 0;
		if(recv["groupid"].isInt() && recv["queueLength"].isInt())
		{
			teamId = recv["groupid"].asInt();
			queuelength = recv["queueLength"].asInt();
		} 		
		
		RecvDataStruct RecvData;
		RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_WAITQUEU_CHANGE);
		XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
		pHead->cmd		= CMD_CS_TEAM_QINFO_REQ;
		pHead->sid		= 0;
		XT_WAITQUEU_CHANGE *pQueueInfo = (XT_WAITQUEU_CHANGE *)(RecvData.data + sizeof(XT_HEAD));
		pQueueInfo->bAdd = 2;
		pQueueInfo->idBuyer = 0;
		pQueueInfo->nicknameBuyer[0] = '\0';
		pQueueInfo->teamId = teamId;
		pQueueInfo->QueueLength = queuelength;
		
		CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);		
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "params :%s TeamId %d QueueLength %d!",params,teamId,queuelength);
        jvReply["result"] = true;

    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }
}

//向买家推送退出消息
void CHttpServer::HttpServicePushQuit(const char *params, Json::Value& jvReply)
{

    Json::Value recv;
    Json::Value statuses;
    Json::Value status;

    if(jReader.parse(params,recv))
    {
        //params解析
		int csId = 0;
		int buyerId = 0;
		//uint32 ip = 0;
		if(recv.isMember("csId") && recv.isMember("buyerId")/* && recv.isMember("loginIp")*/)
		{
			csId = recv["csId"].asInt();
			buyerId = recv["buyerId"].asInt();
			//ip = recv["loginIp"].asUInt();
		} 		
		
		RecvDataStruct RecvData;
		RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_WAITQUEU_CHANGE);
		XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
		pHead->cmd		= CMD_BUYER_QUIT_REQ;
		pHead->sid		= 0;
		XT_BUYERINFO_CHANGE *pQueueInfo = (XT_BUYERINFO_CHANGE *)(RecvData.data + sizeof(XT_HEAD));
		pQueueInfo->idBuyer = buyerId;
		pQueueInfo->idSeller = csId;
		pQueueInfo->teamId = 0;
		//pQueueInfo->loginIP = ip;

		
		CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);		
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "params :%s buyerId %d csId %d!",params,buyerId,csId);
        jvReply["result"] = true;

    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }
}

void CHttpServer::HttpServiceGetMerchantStatus(const char *params, Json::Value& jvReply)
{
    uint32 id[50] = {0};
    tagGSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[50];
    Json::Value recv;
    Json::Value statuses;
    Json::Value status;

    if(jReader.parse(params, recv))
    {
        //params解析
        uint8 size = recv["ids"].size()<50 ? recv["ids"].size() : 50;
        for(int i=0; i<size; i++)
        {
            id[i] = recv["ids"][i].isInt() ? (uint32)recv["ids"][i].asInt() : 0;
        }

        //获取商家客服在线状态
        GetMerchantStatus(size,id,userStatus);

        //reply赋值
        for(int j=0; j<size; j++)
        {
            status["id"] = userStatus[j].id;
            status["status"] = userStatus[j].status;
            statuses.append(status);
        }
        jvReply["result"] = true;
        jvReply["statuses"] = statuses;
    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }

}

void CHttpServer::HttpServiceQueryUserToken(const char *params, Json::Value& jvReply)
{
    uint32 id[50] = {0};
    Json::Value recv;
    Json::Value tokens;
    Json::Value token;
    CUserManager userManager(m_pGlobalBusi);
    shared_ptr<CUser> pUser = NULL;

    if(jReader.parse(params, recv))
    {
        //params解析
        uint8 size = recv["ids"].size()<50 ? recv["ids"].size() : 50;
        for(int i=0; i<size; i++)
        {
            id[i] = recv["ids"][i].isInt() ? (uint32)recv["ids"][i].asInt() : 0;
            pUser = userManager.Find(id[i]);
            if(pUser != NULL)
            {
                token["id"] = id[i];
                token["token"] = Base64Encode(pUser->m_SessionKey, MAX_SESSIONKEY_LEN);
                tokens.append(token);
            }
        }
        jvReply["result"] = true;
        jvReply["tokens"] = tokens;
    }
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "json parse faild";
        CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");
    }
}

// 判断主管能否插入会话
void CHttpServer::HttpServiceIsEnableInsertMsg(const char *params, Json::Value& jvReply)
{	
	CUserManager userManager(m_pGlobalBusi);

	Json::Value recv;
	Json::Value statuses;
	Json::Value status;

	if(jReader.parse(params,recv))
	{
		//params解析
		int csId = 0;
		int buyerId = 0;
		int masterId = 0;
		if(recv.isMember("csId") && recv.isMember("buyerId") && recv.isMember("masterId"))
		{
			csId = recv["csId"].asInt();
			buyerId = recv["buyerId"].asInt();
			masterId = recv["masterId"].asInt();
		} 	
		CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "csid %d  buyerid %d  masterid %d", csId, buyerId, masterId);

		bool monitored = false;
		uint32 curmasterid = 0;
		if(!CUser::g_cs_ismonitored((uint32)csId, monitored, curmasterid, m_pGlobalBusi))
		{	// 查找失败
			jvReply["result"] = false;
			jvReply["error"] = "cs not exist";
			CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "CUser::g_cs_ismonitored fail");
		}
		else
		{
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "csid[%d] curmasterid[%u] ismonitored[%d]", csId, curmasterid, monitored);
			if(!monitored)
			{
				jvReply["result"] = true;
			}
			else if((int)curmasterid == masterId)
			{
				jvReply["result"] = true;
				jvReply["info"] = "same master";
			}
			else
			{
				jvReply["result"] = false;
				jvReply["error"] = "cs has other masters";
			}
		}
	}			
	else
	{
		jvReply["result"] = false;
		jvReply["error"] = "json parse faild";
		CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!");	
	}
}

// 判断主管能否强制下线客服
void CHttpServer::HttpServiceForceOffline(const char *params, Json::Value& jvReply)
{
	CUserManager userManager(m_pGlobalBusi);
	
	Json::Value recv;
	Json::Value statuses;
	Json::Value status;
	
	if(jReader.parse(params,recv))
	{
		//params解析
		int csId = 0;
		if(recv["csId"].isInt())
		{
			csId = recv["csId"].asInt();
		}	
		CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "csid %d", csId);

		uint32 chatting_buyer_id = 0;
		if(!CUser::g_cs_get_chatting_buyer((uint32)csId, chatting_buyer_id, m_pGlobalBusi))
		{	// 查找失败
			jvReply["result"] = false;
			jvReply["error"] = "cs not exist";
			CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "CUser::g_cs_get_chatting_buyer fail");
		}
		else
		{
			jvReply["result"] = true;
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "csid[%d] buyerid[%d]", csId, (int)chatting_buyer_id);
			if(chatting_buyer_id != 0 && (int)chatting_buyer_id != -1)
			{	// 存在正在聊天的买家
				jvReply["enable"] = false;
			}
			else
			{
				jvReply["enable"] = true;
				RecvDataStruct RecvData;
				RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_FORCE_OFFLINE_CS);
				XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
				pHead->cmd		= CMD_FORCE_OFFLINE_CS_REQ;
				pHead->sid		= 0;
				XT_FORCE_OFFLINE_CS *pQueueInfo = (XT_FORCE_OFFLINE_CS *)(RecvData.data + sizeof(XT_HEAD));
				pQueueInfo->idCs = csId;

				CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);		
        		CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "params :%s csId %d!",params,csId);
			}
		}
	}			
	else
	{
		jvReply["result"] = false;
		jvReply["error"] = "json parse faild";
		CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "parse failed!"); 
	}
}



void CHttpServer::generic_handler(struct evhttp_request *req, void *arg)
{
    CHttpServer* p_httpServer = (CHttpServer*)arg;

    Json::Value jvReply;

    //u r l 解析
    char *decodeUri = strdup((char*) evhttp_request_get_uri(req));
    string strDecodeUri = decodeUri;
    CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "strDecodeUri:%s", strDecodeUri.c_str());

    //{}[]转义符替换
    map<string, string> mapReplace;
    mapReplace.insert(pair<string,string>("[","%5B"));
    mapReplace.insert(pair<string,string>("]","%5D"));
    mapReplace.insert(pair<string,string>("{","%7B"));
    mapReplace.insert(pair<string,string>("}","%7D"));
    map<string, string>::iterator   it;
    for(it=mapReplace.begin(); it!=mapReplace.end(); it++)
    {
        replace(strDecodeUri, it->first, it->second);
    }

    //参数解析
    struct evkeyvalq http_query;
    evhttp_parse_query(strDecodeUri.c_str(), &http_query);
    free(decodeUri);
    const char *version = evhttp_find_header(&http_query, "version");
    const char *method = evhttp_find_header(&http_query, "method");
    const char *params = evhttp_find_header(&http_query, "params");
    CLog::Log(__FUNCTION__, CLog::TYPE_DEBUG, "version:%s, method:%s, params:%s", version, method, params);

    //CHttpServer http;
    if(version != NULL && method != NULL && params != NULL)
    {
        //version 1.0
        if(strcmp(version,"1.0") == 0)
        {
            ////获取是否有子账号在线
            if(strcmp(method,"getMerchantStatus") == 0)
            {
                p_httpServer->HttpServiceGetMerchantStatus(params, jvReply);
            }
            ////获取子账号在线状态
            else if(strcmp(method,"getSubMerchantStatus") == 0)
            {
                p_httpServer->HttpServiceGetUserStatus(params, jvReply);
            }
            ////导入/修改客服信息
            else if(strcmp(method,"IMCSInfo") == 0)
            {
                p_httpServer->HttpServiceIMCSInfo(params, jvReply);
            }
			else if(strcmp(method,"getUserInfo") == 0)
			{
				p_httpServer->HttpServiceGetUserInfo(params,jvReply);
			}
			else if(strcmp(method,"getWorkInfo") == 0)
			{
				p_httpServer->HttpServiceGetWorkGroupInfo(params,jvReply);
			}
			else if(strcmp(method,"pushQueuetoCs") == 0)
			{
				p_httpServer->HttpServicePushQueuetoCs(params,jvReply);
			}
			else if(strcmp(method,"pushCloseReceptionByCustomer") == 0)	//推退出消息
			{
				p_httpServer->HttpServicePushQuit(params,jvReply);
			}
			////判断主管能否插入会话
			else if(strcmp(method,"getIsEnableInsertMsg") == 0)
			{
				p_httpServer->HttpServiceIsEnableInsertMsg(params,jvReply);
			}
			////主管强制下线客服
			else if(strcmp(method,"forceOfflineCs") == 0)
			{
				p_httpServer->HttpServiceForceOffline(params,jvReply);
			}
#if 1////修改客服状态
            else if(strcmp(method,"modifyStatus") == 0)
            {
                p_httpServer->HttpServiceModifyStatus(params, jvReply);

            }
#if 1
            else if(strcmp(method,"queryCSStatus") == 0)
            {
                p_httpServer->HttpServiceQueryCS(params, jvReply);
            }
            else if(strcmp(method,"queryUserToken") == 0)
            {
                p_httpServer->HttpServiceQueryUserToken(params, jvReply);
            }
#endif
#endif
            ////method 无效
            else
            {
                jvReply["result"] = false;
                jvReply["error"] = "method invalid";
                CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "methend invalid");
            }
        }
        ////version 无效
        else
        {
            jvReply["result"] = false;
            jvReply["error"] = "version invalid";
            CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "version invalid");
        }
    }
    ////参数不全
    else
    {
        jvReply["result"] = false;
        jvReply["error"] = "version or method or params is NULL";
        CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"version or method or params is NULL");
    }

    //http返回信息
    Json::FastWriter jFastWriter;
    string strReply = jFastWriter.write(jvReply);
    CLog::Log(__FUNCTION__, CLog::TYPE_DEBUG, "reply:%s", strReply.c_str());
    evbuffer_add_printf(p_httpServer->evBuf, "%s", strReply.c_str());
    evhttp_add_header(req->output_headers, "Content-Type", "text/html;charset=gbk");
    evhttp_send_reply(req, HTTP_OK, "OK", p_httpServer->evBuf);


}


int CHttpServer::StartWork()
{
    CConfigInfo m_Config;
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
    m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);

    if( !m_pDataBaseMini.ConnectDB() )
    {
        CLog::Log("CHttpServer",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return RESULT_FAILED;
    }
	
    m_pDataBaseWeb.SetConnectDB(webuid,webpwd,websid);

    if( !m_pDataBaseWeb.ConnectDB())
    {
        CLog::Log("CHttpServer",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return RESULT_FAILED;
    }
	
    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        HttpServerStart,                // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

    return RESULT_SUCCESS;
}

void* CHttpServer::HttpServerStart(LPVOID lpParam)
{
    CHttpServer* p_httpServer = (CHttpServer*)lpParam;
    char* http_addr = p_httpServer->m_ConfigInfo.GetHttpServerAddr();
    int http_port = p_httpServer->m_ConfigInfo.GetHttpServerPort();

    p_httpServer->evBuf = evbuffer_new();

    //evbuffer 申请
    if( !p_httpServer->evBuf )
    {
        CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"evBuf new failed");
        return NULL;
    }

    CLog::Log("HttpServerStart",CLog::TYPE_DEBUG,"http_addr:%s,http_port:%d",http_addr,http_port);

    struct event_base * base = event_base_new();
    struct evhttp * http_server = evhttp_new(base);
    if(!http_server)
    {
        CLog::Log("HttpServerStart",CLog::TYPE_ERROR,"http_server error!");
        return NULL;
    }

    int ret = evhttp_bind_socket(http_server,http_addr,http_port);
    if(ret!=0)
    {
        CLog::Log("HttpServerStart",CLog::TYPE_ERROR,"http_server bind error!");
        return NULL;
    }

    evhttp_set_gencb(http_server, generic_handler, p_httpServer);

    CLog::Log("HttpServerStart",CLog::TYPE_IMPORTANT,"http server start OK!");

    event_base_dispatch(base);
    evhttp_free(http_server);
    event_base_free(base);


    //不要忘记free evBuf
    evbuffer_free(p_httpServer->evBuf);
    return NULL;
}

void  MyBackendGroupTimeProc(void* dwUser)
{
    CGetBackendGroup* pObj = (CGetBackendGroup*)dwUser;
    pObj->HandleTimer(dwUser);
}


void  MyBackendGroupProcNew(union sigval v)
{
    //CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:3");
    MyBackendGroupTimeProc(v.sival_ptr);
}


//获取运营后台组信息
CGetBackendGroup::CGetBackendGroup()
{


}
CGetBackendGroup::~CGetBackendGroup()
{

    StopTiemr();
}


void CGetBackendGroup::StartTimer()
{

    if(!timeSetEventNew(m_timer, m_nTimerDelay, m_nTimerResolution, MyBackendGroupProcNew, (void*)this) )
    {
        CLog::Log("CGetBackendGroup",CLog::TYPE_ERROR,"获取运营后台组织定时器设置失败!");
    }
    CLog::Log("CGetBackendGroup::HandleTimer", CLog::TYPE_IMPORTANT, "Set backend timer success!");
}

void CGetBackendGroup::StopTiemr()
{

	timer_delete(m_timer);
}


void CGetBackendGroup::HandleTimer(LPVOID lpParam)
{
    CGetBackendGroup* pObj = (CGetBackendGroup*)lpParam;

    try
    {

        CLog::Log("CGetBackendGroup::HandleTimer", CLog::TYPE_IMPORTANT, "Get backend Group and Member Start!");
        pObj->GetBackendStart(lpParam);

    }
    catch (...)
    {
        CLog::Log("CGetBackendGroup::HandleTimer()", CLog::TYPE_IMPORTANT, "发生异常!");
    }

}


int CGetBackendGroup::StartWork()
{
    CConfigInfo m_Config;
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
    m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);

    if( !m_pDataBaseMini.ConnectDB() )
    {
        CLog::Log("CGetBackendGroup",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return RESULT_FAILED;
    }

    StartTimer();
    return RESULT_SUCCESS;


}

// 子分组解析函数，转换json子组，是个递归
void CGetBackendGroup::ConvertChildGroupJs(Json::Value &jvChildG, int nParentID, vector<GROUPEX> &vecG)
{
    int nChildGSize = jvChildG.size();          // 分组数
    for(int i = 0; i < nChildGSize; ++i)
    {
        GROUPEX gp;
        gp.partentid = nParentID;
        Json::Value jvOneChild = jvChildG[i];

        gp.groupid = jvOneChild["id"].asInt() + 100;
        char szGroupNameutf8[MAX_GROUPNAME_LEN+1] = {0};
        int len = jvOneChild["text"].asString().size();
        if (len > 0)
        {
            strncpy(szGroupNameutf8, jvOneChild["text"].asCString(),len > MAX_GROUPNAME_LEN ? MAX_GROUPNAME_LEN: len);
            utf2gbk(szGroupNameutf8,len, gp.szGroupName, MAX_GROUPNAME_LEN);
        }
        CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"gbk:gp.partentid:%d,gp.groupid:%d,gp.szGroupName:%s",
                  gp.partentid,gp.groupid,gp.szGroupName);
        vecG.push_back(gp);

        if(jvOneChild.isMember("children"))
        {
            // 存在子分组，递归解析
            ConvertChildGroupJs(jvOneChild["children"], gp.groupid, vecG);
        }
    }
}

// 查询客服分组的函数
void CGetBackendGroup::FindCSGroup(Json::Value &jvChildG, Json::Value &jvCS, BOOL &bFind)
{
    int nCSGroupID = CAppManager::GetInstance().GetConfig().GetCsGroupID();

    int nChildGSize = jvChildG.size();          // 分组数
    for(int i = 0; i < nChildGSize; ++i)
    {
        Json::Value jvOneChild = jvChildG[i];
        if(jvOneChild["id"].asInt() == nCSGroupID)
        {
            jvCS = jvOneChild;
            bFind = TRUE;
            break;
        }
        else if(jvOneChild.isMember("children"))
        {
            // 包含子组，在子组中查找
            FindCSGroup(jvOneChild["children"], jvCS, bFind);

            // 查找完后，判断是否查找到
            if(bFind)
            {
                break;
            }
        }
    }
}


void* CGetBackendGroup::GetBackendGroupStart(LPVOID lpParam)
{
    CGetBackendGroup* pGetBackendGroup = (CGetBackendGroup*)lpParam;
    pGetBackendGroup->m_vecCSPlatGroup.clear();
    //CURL *curl;
    CURLcode res;
    string strGP;

	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string getstr = CAppManager::GetInstance().GetConfig().GetCsGroupUrl();
	res = (CURLcode) httpClient.Get(getstr,strGP);
	if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
#if 0	
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL,CAppManager::GetInstance().GetConfig().GetCsGroupUrl());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strGP);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    else
    {
        CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"create curl failed!");
    }
#endif
    //解析json字符创
    Json::Reader jReader(Json::Features::strictMode());
    Json::Value jvRoot;
    string strUrl;
    int nCSGroupID = CAppManager::GetInstance().GetConfig().GetCsGroupID();
    if (jReader.parse(strGP, jvRoot))
    {
        if(jvRoot.isMember("data"))
        {
            // 包含data
            Json::Value jvRootGArr = jvRoot["data"];    // 根分组
            int nRootGSize = jvRootGArr.size();         // 根分组数量
            CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"nRootGSize:%d",nRootGSize);

            // 查找出客服分组
            Json::Value jvCS;
            BOOL bFind = FALSE;

            for(int i = 0; i < nRootGSize; ++i)
            {
                GROUPEX gp;
                gp.partentid = -1;      // 根分组的父id
                Json::Value jvRootG = jvRootGArr[i];
                if(jvRootG["id"].asInt() != nCSGroupID && !jvRootG.isMember("children"))
                    continue;

                gp.groupid = jvRootG["id"].asInt()+ 100;
                if(jvRootG["id"].asInt() == nCSGroupID)
                {
                    // 找到客服组织架构
                    jvCS = jvRootG;
                    bFind = TRUE;
                    break;
                }
                else if(jvRootG.isMember("children"))
                {
                    // 存在子分组，在子分组中查找
                    pGetBackendGroup->FindCSGroup(jvRootG["children"], jvCS, bFind);
                    if(bFind)
                    {
                        // 在子组中查询到，退出递归
                        break;
                    }
                }
            }

            // 解析出客服分组
            if(bFind)
            {
                GROUPEX gp;
                gp.partentid = -1;      // 根分组的父id
                gp.groupid = jvCS["id"].asInt()+ 100;
                int len = jvCS["text"].asString().size();

                CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"len :%d",len);
                char szGroupNameutf8[MAX_GROUPNAME_LEN+1]= {0};
                if (len > 0)
                {
                    strncpy(szGroupNameutf8, jvCS["text"].asCString(),len > MAX_GROUPNAME_LEN ? MAX_GROUPNAME_LEN: len);
                    utf2gbk(szGroupNameutf8,len, gp.szGroupName, MAX_GROUPNAME_LEN);
                }
                CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"gbk:groupid:%d,szGroupName:%s",gp.groupid,gp.szGroupName);
                pGetBackendGroup->m_vecCSPlatGroup.push_back(gp);
                if(jvCS.isMember("children"))
                {
                    // 存在子分组
                    pGetBackendGroup->ConvertChildGroupJs(jvCS["children"], gp.groupid, pGetBackendGroup->m_vecCSPlatGroup);
                }
                else
                {
                    CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"no [children] member!");
                }
            }
            else
            {
                CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"未查找到客服分组!");
            }
        }
    }
    else
    {
        CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"客服分组解析失败");
    }
    return NULL;
}


////获取客服团队成员信息
void* CGetBackendGroup::GetBackendGroupMemStart(LPVOID lpParam)
{
    //1.从数据库取出有效客服用户realname
    //2.向运营后台查询用户的详细信息(暂时为所属组织)

    CGetBackendGroup* pGetBackendGroupMem = (CGetBackendGroup*)lpParam;
    pGetBackendGroupMem->m_vecCSPlatPerson.clear();
    XT_CS_INFO CSList[200] ;
    int count = 200;
    char organName[MAX_GROUPNAME_LEN+1]= {0};
    char utf8realname[MAX_USERNAME_LEN+1]= {0};
    string urladdress;

    if(pGetBackendGroupMem->m_pDataBaseMini.Get_Vaild_CS_List(CSList,count) != DB_SUCCESS)
    {
        CLog::Log("GetVaildCS", CLog::TYPE_ERROR, "GetVaildCS Failed! ");
    }
    else
    {
        for( int j = 0 ; j < count; ++j)
        {
          //  CURL *curl;
            CURLcode res;
            string strCSMem;
			 
			CHttpClient httpClient;
			httpClient.SetDebug(true);
			char getaddress[150]= {0};
            strcpy(getaddress, CAppManager::GetInstance().GetConfig().GetCsNameUrl());
            char memaddress[200]= {0};
            gbk2utf8(utf8realname, CSList[j].realname) ;
            string realname(utf8realname);
            urladdress = urlencode(realname);
            sprintf(memaddress,"%s%s",getaddress,urladdress.c_str());
            CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"memaddress:%s",memaddress);
			string getstr = memaddress;
			res = (CURLcode)httpClient.Get(getstr,strCSMem);
			if(res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

	#if 0		
            curl = curl_easy_init();
            if(curl)
            {
                char getaddress[150]= {0};
                strcpy(getaddress, CAppManager::GetInstance().GetConfig().GetCsNameUrl());
                char memaddress[200]= {0};
                gbk2utf8(utf8realname, CSList[j].realname) ;
                string realname(utf8realname);
                urladdress = urlencode(realname);
                sprintf(memaddress,"%s%s",getaddress,urladdress.c_str());
                CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"memaddress:%s",memaddress);
                curl_easy_setopt(curl, CURLOPT_URL,memaddress);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strCSMem);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

                /* Perform the request, res will get the return code */
                res = curl_easy_perform(curl);
                /* Check for errors */
                if(res != CURLE_OK)
                {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                }

                /* always cleanup */
                curl_easy_cleanup(curl);
            }
            else
            {
                CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"create curl failed!");
            }
#endif			
            //解析json字符创
            Json::Reader jReader(Json::Features::strictMode());
            Json::Value jvRoot;
            string strUrl;
            char organNameutf8[MAX_GROUPNAME_LEN+1] = {0};
            CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"strCSMem:%s",strCSMem.c_str());
            if (jReader.parse(strCSMem, jvRoot))
            {
                if(jvRoot.isMember("data"))
                {
                    Json::Value jvRootArr = jvRoot["data"];
                    int nRootSize = jvRootArr.size();
                    CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"nRootSize:%d",nRootSize);
                    for(int i = 0; i < nRootSize; ++i)
                    {
                        Json::Value jvRootG = jvRootArr[i];
                        CLog::Log("GetBackendGroupStart",CLog::TYPE_DEBUG,"jvRootG");
                        uint32 csid = jvRootG["operatorid"].asInt();
                        if(csid != CSList[j].csid)
                        {
                        	continue;
                        }
                        
                        //CSList[j].csid = jvRootG["operatorid"].asInt();
                        int len = jvRootG["organName"].asString().size();
                        if (len > 0)
                        {
                            strncpy(organNameutf8, jvRootG["organName"].asCString(),len > MAX_GROUPNAME_LEN ? MAX_GROUPNAME_LEN: len);
                            utf2gbk(organNameutf8,len, organName, MAX_GROUPNAME_LEN);
                        }

                        CSList[j].csGroupId= jvRootG["organid"].asInt()+100;
                        ///将客服所属组织缓存
                        XT_CS_INFO CSMember;
                        strncpy(CSMember.realname, CSList[j].realname,MAX_USERNAME_LEN);
                        CSMember.id = CSList[j].id;
                        CSMember.csid = CSList[j].csid;

                        strncpy(CSMember.organName, organName,MAX_GROUPNAME_LEN);
                        CSMember.csGroupId = CSList[j].csGroupId;
                        CLog::Log("GetBackendGroupMemStart",CLog::TYPE_DEBUG,"CSMember.id:%d,CSMember.csid:%d,CSMember.realname:%s,CSMember.organName:%s,CSMember.csGroupId:%d",
                                  CSMember.id,CSMember.csid,CSMember.realname,CSMember.organName,CSMember.csGroupId);
                        pGetBackendGroupMem->m_vecCSPlatPerson.push_back(CSMember);
                    }
                }
                else
                {
                    CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"No member[data]");
                }

            }
            else
            {
                CLog::Log("GetBackendGroupStart",CLog::TYPE_ERROR,"parse faild!");

            }
        }
    }
    return NULL;
}



void* CGetBackendGroup::GetBackendStart(LPVOID lpParam)
{
    GetBackendGroupStart(lpParam);

    GetBackendGroupMemStart(lpParam);
    return NULL;
}

// 递归获取子组id
void CGetBackendGroup::GetChildGroupID(uint8 nParentGroupID, vector<uint8> &vecChildGroupID)
{
    int nCount = m_vecCSPlatGroup.size();

    // 获取第一层子组id
    vector<uint8> vecLevel1Child;
    int i = 0;
    for(i=0; i<nCount; ++i)
    {
        GROUPEX &group = m_vecCSPlatGroup[i];
        if(group.partentid == nParentGroupID)
        {
            vecLevel1Child.push_back(group.groupid);
            vecChildGroupID.push_back(group.groupid);
        }
    }

    // 获取第一层之下的子组id
    nCount = vecLevel1Child.size();
    for(i=0; i<nCount; ++i)
    {
        GetChildGroupID(vecLevel1Child[i], vecChildGroupID);
    }
}

CChatMsgSave::CChatMsgSave()
{



}

CChatMsgSave::~CChatMsgSave()
{
	if(topic != NULL) delete topic;
    if(producer != NULL) delete producer;

}




void CChatMsgSave::KafKa_init()
{
	CConfigInfo m_Config;
    mode = 'P';
    brokers = m_Config.GetKafuKaBroker();
    topic_str = m_Config.GetKafuKaTalkMsgTopic();

	CLog::Log("CChatMsgSave", CLog::TYPE_DEBUG,"Create producer: brokers :%s topics: %s" ,brokers.c_str(),topic_str.c_str());

//	brokers = "kafka01.dev1.fn:9092,kafka02.dev1.fn:9092,kafka03.dev1.fn:9092";
//	topic_str = "moumou_topic_flow";
    partition = 1;//RdKafka::Topic::PARTITION_UA;
    start_offset = RdKafka::Topic::OFFSET_BEGINNING;
    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    conf->set("metadata.broker.list", brokers, errstr);
    conf->set("event_cb", &ex_event_cb, errstr);


    /* Set delivery report callback */
    conf->set("dr_cb", &ex_dr_cb, errstr);
    /*
     * Create producer using accumulated global configuration.
     */
    producer = RdKafka::Producer::create(conf, errstr);

    if (!producer)
    {    	
		CLog::Log("CChatMsgSave", CLog::TYPE_DEBUG,"Failed to create producer: %s",errstr.c_str());
        exit(1);
    }
    /*
     * Create topic handle.
     */
    topic = RdKafka::Topic::create(producer, topic_str,
                                   tconf, errstr);
    if (!topic)
    {
    	CLog::Log("CChatMsgSave", CLog::TYPE_DEBUG,"Failed to create topic: %s",errstr.c_str());
        exit(1);
    }

}


int CChatMsgSave::StartWork()
{

	KafKa_init();

    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        ChatMsgSavePorc,                // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

    return RESULT_SUCCESS;
}



void* CChatMsgSave::ChatMsgSavePorc( LPVOID lpParam )
{
    /*
        static config
        ./rdkafka_example_cpp -P -b kafka01.dev1.fn:9092,kafka02.dev1.fn:9092,kafka03.dev1.fn:9092 -t moumou_topic_talkmsg
        ./rdkafka_example_cpp -C -b kafka01.dev1.fn:9092,kafka02.dev1.fn:9092,kafka03.dev1.fn:9092 -t moumou_topic_talkmsg -p 0
    */
    static uint32 seq = 0;
	char	 uuid[MAX_UUID_LEN+1] = {0};	//消息的唯一标示  预留

    CChatMsgSave* pCChatMsgSave = (CChatMsgSave*)lpParam;
    assert(pCChatMsgSave != NULL);
    while(true)
    {
        XT_TALK xtTalkMsg;
		
        int nMsgLen;
        CAppManager::GetInstance().GetChatMsgBuffer()->Pop((BYTE*)&xtTalkMsg, nMsgLen);
        CLog::Log("ChatMsgSavePorc", CLog::TYPE_DEBUG, "MsgBuffer %u",CAppManager::GetInstance().GetChatMsgBuffer()->GetSize());
        //有娶到消息
        if(nMsgLen == sizeof(XT_TALK))
        {
            CRecvPacket rp((char *)xtTalkMsg.data,sizeof(xtTalkMsg.data));

            if(xtTalkMsg.data_type == XT_MSG::IM_MSG|| xtTalkMsg.data_type == XT_MSG::AUTO_REPLY)
            {
                CLog::Log("MsgPic", CLog::TYPE_DEBUG, "IM_MSG:: [%u] [%u->%u %u] [%s %s ]",
                          (uint32)xtTalkMsg.recv_time,
                          xtTalkMsg.from_id, xtTalkMsg.to_id, xtTalkMsg.data_type,
                          xtTalkMsg.data,xtTalkMsg.from_nickname);
                {

					#if 0
                    uint32 logintime =0;
                    CUser::m_UserListLock.Lock();
                    CUser *pUser = NULL;//CUser::m_pUserListHead;

                    //int m_nID = (xtTalkMsg.from_id>xtTalkMsg.to_id)?xtTalkMsg.from_id:xtTalkMsg.to_id;
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
                    CUser::m_UserListLock.Unlock();
					if( logintime == 0 )
					{						
			            tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo UserInfo;
			            uint8 count = 1;
			            m_pRouter->GetGlobalBusi()->QueryUserInfo(&msg.from_id,count,&UserInfo);
			            logintime = UserInfo.logintime;
					}
					#endif
					char xtUtf8TalkMSg[1315*2] = {0};
					char xtUtf8NickName[MAX_NICKNAME_LEN*2] = {0};
					char xtUtf8fontName[100] = {0};
			        CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG, "KafKaBuffer %u",CAppManager::GetInstance().GetKafKaMsgBuffer()->GetSize());
					gbk2utf8(xtUtf8TalkMSg,xtTalkMsg.data);
					gbk2utf8(xtUtf8NickName,xtTalkMsg.from_nickname);
					gbk2utf8(xtUtf8fontName,xtTalkMsg.fontName);
					string xtUtfJsonStr;
					Json::Value jvSaveMsg;
					Json::FastWriter jFastWriter;
					jvSaveMsg["msg_seq"] = seq++;
					jvSaveMsg["from_id"] = xtTalkMsg.from_id;
					jvSaveMsg["to_id"] = xtTalkMsg.to_id;
					jvSaveMsg["data_type"] = xtTalkMsg.data_type;
					jvSaveMsg["ver"] = xtTalkMsg.ver;
					jvSaveMsg["send_time"] = xtTalkMsg.send_time;
					jvSaveMsg["recv_flag"] = xtTalkMsg.recv_flag;
					jvSaveMsg["fontSize"] = xtTalkMsg.fontSize;
					jvSaveMsg["fontColor"] = xtTalkMsg.fontColor;
					jvSaveMsg["fontStyle"] = xtTalkMsg.fontStyle;
					jvSaveMsg["fontName"] = xtUtf8fontName;
					jvSaveMsg["data"] = xtUtf8TalkMSg;
					jvSaveMsg["from_nickname"] = xtUtf8NickName;
					jvSaveMsg["recv_time"] = (uint32)xtTalkMsg.recv_time;
					jvSaveMsg["from_terminal"] = xtTalkMsg.from_terminal;
					jvSaveMsg["to_terminal"] = xtTalkMsg.to_terminal;
					jvSaveMsg["logintime"] = xtTalkMsg.logintime;					
					jvSaveMsg["uuid"] = uuid;
					
					if(seq == 0xffffffff)
					{
						seq = 0;
					}
					xtUtfJsonStr = jFastWriter.write(jvSaveMsg);
			        /*
			               * Produce message
			               */
			//		static int seq = 0;
			//		sprintf(xtJsonStr,"{\"test\":\"duyong_test\",\"seq\":%d}",seq++);
			        RdKafka::ErrorCode resp =
			            pCChatMsgSave->producer->produce(pCChatMsgSave->topic, pCChatMsgSave->partition,
			                              RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
			                              const_cast<char *>(xtUtfJsonStr.c_str()), xtUtfJsonStr.size(),
			                              NULL, NULL);
			        if (resp != RdKafka::ERR_NO_ERROR)
			            CLog::Log("CChatMsgSave", CLog::TYPE_DEBUG,"Produce failed %s", RdKafka::err2str(resp).c_str());
			        else
			            CLog::Log("CChatMsgSave", CLog::TYPE_DEBUG,"Produce  %s len %d", xtUtfJsonStr.c_str(),xtUtfJsonStr.length());

			        pCChatMsgSave->producer->poll(0);
                }
            }
        }
    }
    return RESULT_SUCCESS;
}


