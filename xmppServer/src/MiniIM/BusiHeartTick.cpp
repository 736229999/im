#include "stdafx.h"
#include "BusiHeartTick.h"
#include "BusiLogin.h"
#include "AppManager.h"
#include "RouterProtocol.h"
#include <time.h>
#include "hessian/hessianLogi.h"

extern map<uint32,sess_t> sessManager;

CBusiHeartTick::CBusiHeartTick(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_DIRECTORY_REQ,CMD_DIRECTORY_ACK)
{

}

CBusiHeartTick::~CBusiHeartTick(void)
{

}

int CBusiHeartTick::DoWork()
{
    LPXT_DIRECTORY_REQ lpReq = (LPXT_DIRECTORY_REQ)m_RecvPacket.GetData();

    CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
    CUser * pUser = userManager.Find(lpReq->id);
    CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"用户 %u 心跳 IN!",lpReq->id);

    if(!pUser)
    {
        CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"用户 %u pUser为空!",lpReq->id);
        return 0;
    }

    //底层的连接信息
#ifdef WEBIM_TCP
    if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
#endif
        LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(lpReq->id);
        if( !pOnlineUser )
        {
            return 0;
        }

        RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
        pOnlineUser->uport  = pRecData->port;
        pOnlineUser->uip    = pRecData->src_ip;


        pUser->m_nPort      = pRecData->port;
        pUser->m_nIP        = pRecData->src_ip;
#ifdef WEBIM_TCP
    }
#endif
    pUser->m_nNewlyLiveTime = timeGetTime();

    RESPONSE_ACK ack;
    ack.ret = 0;
    m_SendPacket<<ack.ret;

    if(sessManager.find(lpReq->id) != sessManager.end())
    {
        // 以xmpp客户端登录
        CLog::Log("CBusiHeartTick::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", m_SendPacket.GetSize()+ sizeof(XT_HEAD));
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_SendPacket.GetSize()+ sizeof(XT_HEAD));

        CancelReply();
    }

    CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"用户 %u 心跳 OUT!",lpReq->id);

    return 0;
}

/////////////////////CHeartTickCheck.cpp//////////////////////
void  MyHeartTimeProc(void* dwUser)
{
    CHeartTickCheck* pObj = (CHeartTickCheck*)dwUser;
    pObj->HandleTimer();
}

CHeartTickCheck::CHeartTickCheck()
{
    m_hMMT = NULL;

}
CHeartTickCheck::~CHeartTickCheck(void)
{
    StopWork();
}

#define TIMERPERIOD (1000 * 60 * 1) //时钟周期 1分钟
#define CNTHEARTTIMEOUT 1000 * 60 * 3  //客户端心跳超时时间

int CHeartTickCheck::StartWork()
{
    StartTimer();
    return 0;
}

void CHeartTickCheck::StopWork()
{
    StopTiemr();
}

void CHeartTickCheck::StartTimer()
{
    m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD,MyHeartTimeProc,(void*)this,TIME_PERIODIC);
    if( NULL == m_hMMT )
    {
        CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"心跳定时器设置失败!");
    }
}

void CHeartTickCheck::StopTiemr()
{
    if( m_hMMT )
    {
        ::timeKillEvent(m_hMMT);
        m_hMMT = NULL;
    }
}


void CHeartTickCheck::HandleTimer()
{
    try
    {
        CUser::m_UserListLock.Lock();

        int nCheckUserNum = 0 ;
        int nBuyerCnt = 0;
        int nSellerCnt = 0;
        int nTickOut = 0;

        CUser *pUserCurPoint = NULL;
        CUser * ptmpUser = NULL;
        list<UINT32>  lstEraseID;
        uint32 now = timeGetTime();

        for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it =  CUser::m_userhash.begin(); it != CUser::m_userhash.end(); ++ it)
        {
            pUserCurPoint = it->second;
            if (!pUserCurPoint)
                continue;
            if ( nCheckUserNum > 30000)
            {
                break;
            }
            if(pUserCurPoint->m_nID > 5000000)
            {
                nBuyerCnt++;
            }
            else
            {
                nSellerCnt++;
            }
            ++nCheckUserNum;
            if(pUserCurPoint->m_nLoginType != XT_HEAD::FLAG_MOBREQ)
            {
                if((now - pUserCurPoint->m_nNewlyLiveTime) > CNTHEARTTIMEOUT)
                {
                    //心跳超时
                    CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "User %d timeout!", pUserCurPoint->m_nID);

                    lstEraseID.push_back(pUserCurPoint->m_nID);
                    ++nTickOut;
                    continue;
                }
            }
        }
        for(list<UINT32>::iterator lit = lstEraseID.begin(); lit != lstEraseID.end(); ++lit)
        {
            UINT32 id = *lit;
            ptmpUser = CAppManager::GetInstance().GetUserManager().Find(id);
            SendLogoutReq(ptmpUser);
        }
        CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "user:%d, buyer:%u, seller:%u, logout:%u ", nCheckUserNum, nBuyerCnt, nSellerCnt, nTickOut);
        CUser::m_UserListLock.Unlock();
    }
    catch (...)
    {
        CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "catch error");
        CUser::m_UserListLock.Unlock();
    }
    //防止因连续几天登录而导致日志服务器无法统计时间，故在每日00:00查询在线用户并上传login动作日志。
    struct tm newtime;
    time_t aclock;
    time( &aclock );
    localtime_s(&newtime, &aclock );
    char tmp[6];
    //CLog::Log("HandleTimer",CLog::TYPE_IMPORTANT,"tmp:%s",tmp);
    strftime(tmp,sizeof(tmp), "%H:%M", &newtime);
    if(strcmp(tmp, "00:00") == 0)
    {
        time_t tm;
        time(&tm);
        CUser *pUser = NULL;
        CUser::m_UserListLock.Lock();;
        for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it = CUser::m_userhash.begin(); it != CUser::m_userhash.end(); ++it)
        {
            pUser = it->second;
            if(!pUser)
                continue;
            if(pUser->m_nID < Id_Boundary)
            {
                //上传登录动作
                using namespace LogInterface;
                using namespace std;
                hessianLogi hess(pUser->m_merchantid,pUser->m_nID);
                try
                {
                    SubMerchantAction info;
                    info.merchantID = pUser->m_merchantid;
                    info.subMerchantID = pUser->m_nID;
                    info.actionTime = (int)aclock;
                    info.actionType = ACTION_LOGIN;

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
        CUser::m_UserListLock.Unlock();
    }
}

//发送一个退出业务包
void CHeartTickCheck::SendLogoutReq(CUser* pUser)
{

    RecvDataStruct RecvData;
    RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
    XT_HEAD* pHead  = (XT_HEAD*)RecvData.data;
    pHead->cmd      = CMD_LOGOUT;
    pHead->sid      = pUser->m_nID;

    XT_LOGOUT* pLogOut = (XT_LOGOUT*)(RecvData.data + sizeof(XT_HEAD));
    pLogOut->id = pUser->m_nID;
    pLogOut->authData = 0x00;

    CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);
}


/////////////////////CTimeLenSave.cpp//////////////////////

void  TimeLenProc(void* dwWebMsg)
{
    CTimeLenSave* pObj = (CTimeLenSave*)dwWebMsg;
    pObj->HandleTimer();
}

#define TIMELENCHECKIOD (1000 * 60 * 1) //时钟周期 1分钟

CTimeLenSave::CTimeLenSave()
{
    m_hMMT = 0;
}


CTimeLenSave::~CTimeLenSave(void)
{
    StopWork();
}



int CTimeLenSave::StartWork()
{
    CConfigInfo     dbConfig;
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
    m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);
    if( !m_pDataBaseMini.ConnectDB() )
    {
        CLog::Log("CTimeLenSave",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return RESULT_FAILED;
    }

    StartTimer();
    return 0;
}

void CTimeLenSave::StopWork()
{
    StopTiemr();
}

void CTimeLenSave::StartTimer()
{
    m_hMMT = timeSetEvent(TIMELENCHECKIOD,TIMELENCHECKIOD,TimeLenProc,(void*)this,TIME_PERIODIC);
    if( NULL == m_hMMT )
    {
        CLog::Log("CTimeLenSave",CLog::TYPE_ERROR,"自动记录用户在线时长定时器启动失败!");
    }
}

void CTimeLenSave::StopTiemr()
{
    if( m_hMMT )
    {
        ::timeKillEvent(m_hMMT);
        m_hMMT = NULL;
    }
}


void CTimeLenSave::HandleTimer()
{
    struct tm newtime;
    time_t aclock;

    time( &aclock );

    localtime_s(&newtime, &aclock );
    char tmp[6];
    strftime(tmp,sizeof(tmp), "%H:%M", &newtime);

    if (strcmp(tmp, "23:59") == 0)
    {
        //StopTiemr();  //del by lwl ver:0101
        CUser::m_UserListLock.Lock();;

        CUser *pUser = NULL;//CUser::m_pUserListHead;
        for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it = CUser::m_userhash.begin(); it != CUser::m_userhash.end(); ++it)
        {
            pUser = it->second;
            if(!pUser)
                continue;
            m_pDataBaseMini.Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)aclock);
        }
        CUser::m_UserListLock.Unlock();
    }

}

