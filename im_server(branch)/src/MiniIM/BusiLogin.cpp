#include "stdafx.h"
#include <curl/curl.h>
#include "BusiLogin.h"
#include "ErrorCode.h"
#include "Function.h"
#include "RouterBusiness.h"
#include "time.h"
#include "AppManager.h"
#include "NotifyProtocol.h"
#include "ConnBusiControl.h"
#include "../DataBaseLib/MysqlLib.h"
#include "json/json.h"
#include <iconv.h>
#include "hessian/hessianLogi.h"
#include "BusiCacheInterface.h"
#include "improtocol.pb.h"
#include "ThreadMsgNotify.h"
#include "httpclient.h"
#include "imapi.h"
#include "UserManager.h"
#include "HttpInterface.h"

#define MAX_SEARCH_FRINED_RETURN_NUM 50

int CalcQueueLength(int QueueOrigin,int row = 4,int col = 4);

int HexStr2BYTEBCDArray(const char *chBuf, int len, BYTE *chBYTE)
{
    int i;

    char  chtmp;
    BYTE itmp;

    //check PN length
    if(len != (int)strlen(chBuf))
    {
        return -1;
    }

    //Transter Hex string(chBuf) to BCD BYTE Array(chBYTE)
    for(i=0 ; i<len; i++)
    {
        chtmp = chBuf[i];
        if( chtmp<='9' && chtmp >= '0')
        {
            itmp = chtmp-'0';
        }
        else
        {
            itmp = chtmp-'a' + 10;;
        }

        if(i%2 == 0)
            chBYTE[i/2] = (itmp&0x0f)<<4;
        else
            chBYTE[i/2] += (itmp&0x0f);
    }


    return 0;
}

bool GetBuyerInfo(const char* guid, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl)
{
    if(strcmp(guid, "") == 0)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid为空");
        return false;
    }

    std::string sPostParam = "type=5";

    //加入用户名
    sPostParam += "&name=";
    sPostParam += guid;
    sPostParam += "&returnType=1";
    std::string data;

    std::string url = CAppManager::GetInstance().GetConfig().GetBuyerInfoURL();
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    CURLcode res = (CURLcode)httpClient.Post(url,sPostParam,data);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid:%s curlFaild", guid);
        return false;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"JSON:%s",szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        if(!recv.isMember("msg"))
        {
            return false;
        }
        string strMsg = recv["msg"].asString();
        if(strMsg != "success")
        {
            //用户不存在
            CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid:%s, notExist", guid);
            return false;
        }
        else
        {
            string strMobileNo= recv["data"]["MEM_CELL_PHONE"].asString();
            string strEmail = recv["data"]["MEM_EMAIL"].asString();
            string strNickName =recv["data"]["NICKNAME"].asString();
            string strHeadImgFile =recv["data"]["HEAD_PORTRAIT"].asString();

            strncpy(szMobileNo, strMobileNo.c_str(), MAX_PHONE_LEN);
            strncpy(szEmail, strEmail.c_str(), MAX_EMAIL_LEN);
            strncpy(szNickName, strNickName.c_str(), 61);

            if(strHeadImgFile.length()!=0)
            {
                char szHeadImgFile[MAX_HEADURL_LEN-30]="";
                char preHeadImgURL[MAX_HEADURL_LEN-70] = {0};
                strcpy(preHeadImgURL,CAppManager::GetInstance().GetConfig().GetHeadImgURL());

                strncpy(szHeadImgFile,strHeadImgFile.c_str(),MAX_HEADURL_LEN-30 );
                CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"preHeadImgURL:%s",preHeadImgURL);

                sprintf(szHeadImgUrl,"%s%s",preHeadImgURL,szHeadImgFile);
            }
            else
            {
                strncpy(szHeadImgUrl,strHeadImgFile.c_str(),MAX_HEADURL_LEN );
            }
            CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"guid:%s, mobileNo:%s, email:%s, nickName:%s, szHeadImgFile:%s",
                      guid, szMobileNo, szEmail, szNickName, szHeadImgUrl);
            //找到了子串
            return true;
        }
    }
    else
    {
        //json串解析失败
        CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "guid:%s, data:%s ,jsonPraseFail", guid, szDataGbk);
        return false;
    }
}


#ifdef ACCESS_SERVER


/////////////////////// 版本方面的错误 //////////////////////////
#define  IMVERSION_LOW      (unsigned char)0x02
#define  IMVERSION_CANUSE   (unsigned char)0x01
#define  IMVERSION_AGREE    (unsigned char)0x00
//////////////////////////////////////////////////////////////////

CBusiLogin::CBusiLogin(CRouterBusiness* pRouter):
    CBusiness(pRouter,CMD_LOGIN_REQ,CMD_LOGIN_ACK)
{
    m_uid = 0;
    m_pAppNetwork = &(CAppManager::GetInstance().GetAppNetWork());
    m_szError[0] = '\0';

    m_IsOverLoad = false;
}

CBusiLogin::~CBusiLogin(void)
{
}


//登陆业务作为一个特殊的业务处理,是基础传输层的处理，需要应用层自行加解密
int CBusiLogin::DoWork()
{
    //初始化基本数据
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pSendData->data;
    m_szError[0] = '\0';
    m_IsOverLoad = false;
    uint32 uRet;
    XT_LOGIN_REQ req;

    if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        CRecvPacket rp(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD));
        //解包
        rp>>req.login_flag;
        switch (req.login_flag & 0x3)
        {
            case XT_LOGIN_REQ::LOGINBYID:
            {
                rp >> req.uLoginKey.id;
                CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user\'%d\'login packet",
                          req.uLoginKey.id);
                break;
            }

            case XT_LOGIN_REQ::LOGINBYNAME:
            {
                rp >> req.uLoginKey.username;
                CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user\'%s\'login packet",
                          req.uLoginKey.username);
                break;
            }
            case XT_LOGIN_REQ::LOGINBYMOBILENO:
            {
                rp >> req.uLoginKey.mobileno;
                CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user\'%s\'login packet",
                          req.uLoginKey.mobileno);
                break;
            }
            default:
                break;
        }

        if ((req.login_flag& 0xC) >> 2 == 2 )
            rp >>req.auth_session;
        rp.ReadPacket(req.random_key,sizeof(req.random_key));

        char plain[MAX_RECV_BUFFERS] = {0};
        int dwPlainLen = 0;
        dwPlainLen = rp.ReadData(plain,sizeof(plain));

        if( !xTEADecryptWithKey(plain,dwPlainLen,req.random_key, plain ,&dwPlainLen) )
        {
            CLog::Log("crashInfo", CLog::TYPE_IMPORTANT, "DecryptFailed");
            return RESULT_FAILED;   //解密失败
        }
        else
        {
            CLog::Log("crashInfo", CLog::TYPE_IMPORTANT, "DecryptSuccess");
        }

        rp.SetData(plain,dwPlainLen);

        rp.ReadPacket(req.password,sizeof(req.password));
        rp>>req.status>>req.lanip>>req.lanport;
#ifdef XMPP_BUILD
        uRet = HandleReq(&req, lpRecvHead,pRecvData->vlink);
#else
        uRet = HandleReq(&req, lpRecvHead);
#endif
#ifdef IM_USE_MULTIDEVICE

        CLog::Log("Login1", CLog::TYPE_IMPORTANT, "sid:%u, did:%u, flag:%x, deviceId %u, login_flag:%u, username:%s, status:%u, lanip:%u, lanport:%u, sessionID:%s" ,
                  lpRecvHead->sid, lpRecvHead->did, lpRecvHead->flag, lpRecvHead->deviceId,req.login_flag, req.uLoginKey.username, req.status, req.lanip, req.lanport, req.sessionID);

#else
        CLog::Log("Login1", CLog::TYPE_IMPORTANT, "sid:%u, did:%u, flag:%x, login_flag:%u, username:%s, status:%u, lanip:%u, lanport:%u, sessionID:%s" ,
                  lpRecvHead->sid, lpRecvHead->did, lpRecvHead->flag, req.login_flag, req.uLoginKey.username, req.status, req.lanip, req.lanport, req.sessionID);
#endif

    }
    else
    {
        XT_LOGIN_REQ*  req = (XT_LOGIN_REQ*)(pRecvData->data + sizeof(XT_HEAD));
#ifdef XMPP_BUILD
        uRet = HandleReq(req, lpRecvHead,pRecvData->vlink);
#else
        uRet = HandleReq(req, lpRecvHead);
#endif

        CLog::Log("Login2", CLog::TYPE_IMPORTANT, "sid:%u, did:%u, flag:%x, login_flag:%u, username:%s, status:%u, lanip:%u, lanport:%u, sessionID:%s" ,
                  lpRecvHead->sid, lpRecvHead->did, lpRecvHead->flag, req->login_flag, req->uLoginKey.username, req->status, req->lanip, req->lanport, req->sessionID);

    }


    if(uRet != RESULT_SUCCESS)
    {
        //组装返回错误原因包
        if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        {
            if(uRet == BUSIERR_INTERNAL)
                return BUSIERR_INTERNAL;
            CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));
            sp<<(uint8)1;
            if( m_szError[0] != '\0' )
            {
                sp<<m_szError;
            }
            else
            {
                sp<<"登录失败";
            }
            m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
        }
        else
        {
            int len;
            XT_LOGIN_ERROR* ack = (XT_LOGIN_ERROR*)(m_pSendData->data + sizeof(XT_HEAD));
            *(uint8*)ack = 1;
            if( m_szError[0] != '\0' )
            {
                len = strlen(m_szError);
                strncpy((char*)ack + sizeof(uint8),m_szError,len);
                *((char*)ack + sizeof(uint8) + len) = 0;
            }
            else
            {
                len = strlen("登录失败");
                strncpy((char*)ack + sizeof(uint8),"登录失败",len);
                *((char*)ack + sizeof(uint8) + len) = 0;
            }
            m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + len);
        }
    }
    if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
#ifdef XMPP_BUILD
        if(lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ || lpRecvHead->flag == XT_HEAD::FLAG_XMPPSASL)
            InitHead(XT_HEAD::FLAG_XMPPACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);
        else
#endif
            InitHead(XT_HEAD::FLAG_SRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);
    }
    else
        InitHead(XT_HEAD::FLAG_MOBSRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);

#ifdef IM_USE_MULTIDEVICE
    lpSendHead->deviceId = lpRecvHead->deviceId;
#endif


    if( m_IsOverLoad )
    {
        //暂时不用用户过载(超过最大同时在线用户数量)，zhangmin 20150425，会挂
    }

    if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
    {
        //加密，发送数据，根据ret选择加密方式,ret == 0 采用md5(md5(password)) 加密，ret == 1 采用randomkey 加密
        uRet = (*(uint8 *)(m_pSendData->data + sizeof(XT_HEAD)));
        char szKey[16] = {0};
        if(uRet == 0)
        {
            memcpy(szKey,m_digest,sizeof(szKey));
        }
        else
        {
            memcpy(szKey,req.random_key,sizeof(szKey));
        }

#ifdef XMPP_BUILD
        //根据用户登录方式判断是否将返回值发送给jabberdIM线程
        if(lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ || lpRecvHead->flag == XT_HEAD::FLAG_XMPPSASL)
        {
            CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", m_pSendData->data_len);
            CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_pSendData->data_len, pRecvData->vlink);
        }
#endif

        int tlen = 0;
        if(!xTEAEncryptWithKey(m_pSendData->data + sizeof(XT_HEAD),m_pSendData->data_len - sizeof(XT_HEAD),szKey,m_pSendData->data + sizeof(XT_HEAD),&tlen))
        {
            CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"%d xTEAEncryptWithKey failed", m_uid);
            return BUSIERR_INTERNAL;
        }

        m_pSendData->data_len = (uint16)tlen + sizeof(XT_HEAD);
    }

    //回复登陆请求,调用传输层直接发送
    m_pSendData->retry_times = 0;
    m_pSendData->dest_ip = pRecvData->src_ip;
    m_pSendData->port = pRecvData->port;

#ifdef XMPP_BUILD
    if(lpRecvHead->flag != XT_HEAD::FLAG_XMPPREQ && lpRecvHead->flag != XT_HEAD::FLAG_XMPPSASL)
    {
        CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"send to other");
        m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),*m_pSendData);
    }
#else
    m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),*m_pSendData);
#endif

    return RESULT_SUCCESS;
}

#ifdef XMPP_BUILD
uint32 CBusiLogin::HandleReq(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint32 fd)
#else
uint32 CBusiLogin::HandleReq(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead)
#endif
{
    //处理包
    uint32 uRet = 0;

    char cPass[40 + 1];
    unsigned char digest[16];

    strcpy(cPass, lpReq->password);
    MD5Password(cPass);

    //判断登陆的是卖家还是买家
    char szGuid[MAX_GUID_LEN] = "";
    char szUserName[MAX_USERNAME_LEN + 1] = "";
    char szMobileNo[MAX_PHONE_LEN + 1]="";
    char szEmail[MAX_EMAIL_LEN+1]="";
    char szNickName[61]="";
    char szHeadImgUrl[MAX_HEADURL_LEN+1] ="";
    char szPassword[MAX_PASSWORD_LEN]="";
    uint8 buyerLevel = 0;
    switch ((lpReq->login_flag & 0xC) >> 2)
    {
        case XT_LOGIN_REQ::CS:
        {
            CLog::Log("CheckCS",CLog::TYPE_IMPORTANT,"CS");
            uint8 state = 0;

            strncpy (szPassword,lpReq->password,MAX_PASSWORD_LEN);

            // toBeImApi
            if(!(m_pRouter->GetDataBaseWeb()->Get_CS_PWD(lpReq->uLoginKey.username, szPassword, m_ack.user_id, state) ))
            {
                CLog::Log("CheckCS",CLog::TYPE_IMPORTANT,"Get_CS_PWD! username:%s, id:%d, state:%d,dbPassword:***",
                          lpReq->uLoginKey.username, m_ack.user_id, state);
                if(state == 0)
                {
                    sprintf_s(m_szError, "%s", "您的帐号已停用");
                    return RESULT_FAILED;
                }

                if(strcmp(szPassword,cPass) == 0)
                {
                    CLog::Log("CheckCS",CLog::TYPE_IMPORTANT,"check cs PSW right! username:%s, user_id:%d, state:%d, dbpassword: ***, password:***",
                              lpReq->uLoginKey.username, m_ack.user_id, state);
                    m_uid = m_ack.user_id;
                }
                else
                {
                    sprintf_s(m_szError, "%s", "用户名或密码错误");
                    CLog::Log("CheckCS",CLog::TYPE_ERROR,"check cs PSW error! username:%s, user_id:%d",
                              lpReq->uLoginKey.username,  m_ack.user_id);
                    return RESULT_FAILED;
                }

            }
            else
            {
                sprintf_s(m_szError, "%s", "用户不存在");
                CLog::Log("CheckCS",CLog::TYPE_ERROR,"Get pwd fail! username:%s, id:%d",
                          lpReq->uLoginKey.username, m_ack.user_id);
                return RESULT_FAILED;
            }

            break;
        }
        case XT_LOGIN_REQ::BUYER:
        {
            //web端买家
            if(lpReq->sessionID[0] != 0 )
            {
                CLog::Log("CheckBuyer",CLog::TYPE_DEBUG,"sessionID:%s",lpReq->sessionID);
                int nlen = strlen(lpReq->sessionID);
                if(nlen >= 36)
                {
                    memcpy(szGuid, lpReq->sessionID + (nlen-36), 36);
                    strcpy(szUserName, lpReq->uLoginKey.username);
                    strcpy(szMobileNo, lpReq->mobilephoneno);
                    strcpy(szEmail,lpReq->email);
                    //strcpy(szNickname,lpReq->uLoginKey.username);
                    szGuid[36] = '\0';
                }
                else
                {
                    CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"GUID错误");
                }
            }
            //客户端买家
            else if(CheckBuyer(lpReq->uLoginKey.username, lpReq->password,szGuid, szUserName))
            {
                CLog::Log("CheckBuyer",CLog::TYPE_DEBUG,"username:%s",szUserName);
            }
            else
            {
                sprintf_s(m_szError, "%s", "用户名或密码错误");
                CLog::Log("CBusiLogin SaveAccountBuyer", CLog::TYPE_ERROR, "Buyer:%s pwsError", szUserName);
                return RESULT_FAILED;
            }
			
            buyerLevel = 0;
            if(strcmp(szNickName, "") == 0)
            {
                strncpy(szNickName,szUserName,60);
                CLog::Log("GetBuyerInfo",CLog::TYPE_IMPORTANT,"Buyer:%s, getInfoSelf",szUserName);
            }


            //内部版必须在员工系统录入
#ifdef PRE_INTERNAL
            uint32 uid = 0;
            char cPass2[60];
            uRet = m_pRouter->GetDataBaseWeb()->Get_UserID_PWD(lpReq->uLoginKey.username,(uint32 &)uid,cPass2);
            if(uRet != DB_SUCCESS)
            {
                sprintf_s(m_szError, "%s", "您的域帐号尚未在哞哞激活");
                return RESULT_FAILED;
            }
#endif


            //验证通过,存储帐号
            CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "check GUID 3:%s, length:%d", szGuid, strlen(szGuid));
            if(strlen(szGuid)== 36 )
            {
                if(strlen(szUserName) == 0)
                {
                    sprintf_s(m_szError, "%s", "用户名错误");
                    CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer: %s GetUserName Failed!",lpReq->uLoginKey.username);
                    return RESULT_FAILED;
                }
                //else if(m_pRouter->GetDataBaseWeb()->SaveAccountBuyer(szUserName, cPass, szGuid, m_uid, szMobileNo, szEmail, szNickName, szHeadImgUrl) != DB_SUCCESS)
                else if(! SaveBuyer(szUserName, cPass, szGuid, m_uid, szMobileNo, szEmail, szNickName, szHeadImgUrl))
                {
                    sprintf_s(m_szError, "%s", "您的用户名或密码错误");
                    CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s Save Account Failed!",lpReq->uLoginKey.username);
                    return RESULT_FAILED;
                }
            }
            else
            {
                sprintf_s(m_szError, "%s", "您用户名或密码错误");
                CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s, GUID:%s error!",lpReq->uLoginKey.username,szGuid);
                return RESULT_FAILED;
            }

            break;
        }
        case XT_LOGIN_REQ::SELLER:
        {
            if((lpReq->login_flag & 0x10) >> 4 == XT_LOGIN_REQ::CHECK_DIFF_PLACES)
            {
#if 0   // 先屏蔽手机验证码功能
                // 校验同地登录
                string strError;
                int nRlt = IsSameAreaLogin(lpReq->uLoginKey.username, strError);
                if(nRlt == -1)
                {
                    // 调用失败
                    sprintf_s(m_szError, "%s", strError.c_str());
                    return RESULT_FAILED;
                }
                else if(nRlt == 0)
                {
                    // 异地登录
                    nRlt = IsBindMobile(lpReq->uLoginKey.username, strError);
                    if(nRlt == -1)
                    {
                        // 调用失败
                        sprintf_s(m_szError, "%s", strError.c_str());
                        return RESULT_FAILED;
                    }

                    else if(nRlt == 1)
                    {
                        // 已绑定手机
                        sprintf_s(m_szError, "%s", "您的登录出现异常，需进行手机验证");
                        return RESULT_FAILED;
                    }
                    else
                    {
                        // 未绑定手机
                        sprintf_s(m_szError, "%s", "您的登录出现异常，为了您的账户安全，请尽快绑定手机");
                        return RESULT_FAILED;
                    }
                }
#endif
            }

            //if( CheckSellerMysql(lpReq->uLoginKey.username, lpReq->password, m_ack.user_id, m_ack.merchant_id, state) )
            int state = 0;
            string code = "";
            if( CheckSeller(lpReq->uLoginKey.username, lpReq->password, m_ack.user_id, m_ack.merchant_id, state, code) )
            {
                // 解析成功
                CLog::Log("CheckSeller",CLog::TYPE_IMPORTANT,"CheckSeller ok! username:%s, id:%d, state:%d, code:%s",
                          lpReq->uLoginKey.username, m_ack.user_id, state, code.c_str());
                if(state == 2)
                {
                    sprintf_s(m_szError, "%s", "您的帐号已停用");
                    return RESULT_FAILED;
                }

                // 解析code
                if(code != "200")
                {
                    // 登录失败
                    if(code == "205")
                    {
                        sprintf_s(m_szError, "%s", "参数错误");
                        return RESULT_FAILED;
                    }
                    else if(code == "204")
                    {
                        sprintf_s(m_szError, "%s", "参数为空");
                        return RESULT_FAILED;
                    }
                    else if(code == "210")
                    {
                        sprintf_s(m_szError, "%s", "用户名或密码错误");
                        return RESULT_FAILED;
                    }
                    else if(code == "211")
                    {
                        sprintf_s(m_szError, "%s", "此账号已被限制登录，重新设置密码，可以解除限制");
                        return RESULT_FAILED;
                    }
                }


                //if(m_pRouter->GetDataBaseWeb()->SaveAccount(lpReq->uLoginKey.username,cPass,m_ack.user_id,lpReq->uLoginKey.username) == DB_SUCCESS)
                if(SaveSeller(lpReq->uLoginKey.username,cPass,m_ack.user_id,lpReq->uLoginKey.username) )
                {
                    m_uid = m_ack.user_id;
                    if(m_ack.merchant_id == 0)
                    {
                        sprintf_s(m_szError, "%s", "您尚未入驻飞牛，请入驻后再登录哞哞");
                        return RESULT_FAILED;
                    }
                    /*
                    else if(m_ack.merchant_id != m_ack.user_id)
                    {
                        CLog::Log("CBusiLogin SaveSubAccount",CLog::TYPE_DEBUG,"Save Account %d %d",m_ack.merchant_id,m_ack.user_id);
                        //以卖家为主账号存储卖家的子账号
                        if(m_pRouter->GetDataBaseWeb()->SaveSubAccount(m_ack.merchant_id,m_ack.user_id,MYGROUP_ID,
                        cPass,lpReq->uLoginKey.username,lpReq->uLoginKey.username) != DB_SUCCESS)
                        {
                            CLog::Log("CBusiLogin SaveSubAccount",CLog::TYPE_ERROR,"Save Account Failed");
                        }
                    }
                    */
                }
                else
                {
                    sprintf_s(m_szError, "%s", "您的用户名或密码错误");
                    CLog::Log("CBusiLogin SaveAccount",CLog::TYPE_ERROR,"Seller SaveAccount Failed! username:%s, user_id:%d, merchant_id:%d",
                              lpReq->uLoginKey.username, m_ack.user_id, m_ack.merchant_id);
                    return RESULT_FAILED;
                }

                CLog::Log("crashInfo",CLog::TYPE_IMPORTANT,"SaveAccountPass");
            }
            else
            {
                // 解析失败
                sprintf_s(m_szError, "%s", "用户名或密码错误");
                CLog::Log("CBusiLogin SaveAccount",CLog::TYPE_ERROR,"Seller PSW error! username:%s, user_id:%d, merchant_id:%d",
                          lpReq->uLoginKey.username,  m_ack.user_id, m_ack.merchant_id);
                return RESULT_FAILED;
            }
            break;
        }
        case XT_LOGIN_REQ::WORK:
        {

            int code = 0;
            string email;
            string realname;
            string mobilenumber;
            string groupname;
            char groupnameStr[100] = "";
            if( CheckWork(lpReq->uLoginKey.username, cPass,code, realname,email, mobilenumber,groupname) )
            {
                // 解析成功
                CLog::Log("CheckWork",CLog::TYPE_IMPORTANT,"CheckWork ok! username:%s, realname %s,email:%s mobile %s,group %s code:%d",
                          lpReq->uLoginKey.username,realname.c_str(),email.c_str(), mobilenumber.c_str(),groupname.c_str(),code);


                // 解析code
                if(code != 200)
                {
                    // 登录失败
                    if(code == 400)
                    {
                        sprintf_s(m_szError, "%s", "用户名或密码错误");
                        return RESULT_FAILED;
                    }
                }
                if(realname.length() > 0 && realname.length() < 60)
                {
                    utf2gbk((char *) realname.c_str(),(int) realname.length(),szNickName,60);
                }
                if(groupname.length() > 0 && groupname.length() < 100)
                {
                    utf2gbk((char *) groupname.c_str(),(int) groupname.length(),groupnameStr,99);
                }
                int SaveAccountWorkRet = 0;
                //toBeImApi
                SaveAccountWorkRet = m_pRouter->GetDataBaseWeb()->SaveAccountWork(lpReq->uLoginKey.username, cPass, m_uid, (char *)mobilenumber.c_str(), (char *)email.c_str(),szNickName, szHeadImgUrl,groupnameStr);

                if(SaveAccountWorkRet != DB_SUCCESS)
                {
                    if(SaveAccountWorkRet == DB_USERNAME_EXIST)
                    {
                        sprintf_s(m_szError, "%s", "您的身份为客服，不能登录工作版");
                        CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Work:%s Save Account Failed!",lpReq->uLoginKey.username);
                    }
                    else
                    {
                        sprintf_s(m_szError, "%s", "您的用户名或密码错误");
                        CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Work:%s Save Account Failed!",lpReq->uLoginKey.username);
                    }
                    return RESULT_FAILED;
                }

                CLog::Log("crashInfo",CLog::TYPE_IMPORTANT,"SaveAccountPass,id:%d nickname %s %s",m_uid,szNickName,realname.c_str());
            }
            break;

        }

        default:
            return RESULT_FAILED;
    }
    CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"BEFORE HexStr2BYTEBCDArray");

    HexStr2BYTEBCDArray(cPass,sizeof(digest)*2,digest);
    memcpy(m_digest,digest,sizeof(m_digest));

    CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"AFTER HexStr2BYTEBCDArray");

    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
    //数据有效性校验...
    //生成用户session key
    unsigned char bRandomStr[4];
    srand( (unsigned)time( NULL ) );
    *(unsigned char*)bRandomStr = rand();
    int dwSessionLen;
    if(!xTEAEncryptWithKey((const char*)bRandomStr, sizeof(bRandomStr), (char  *)digest,(char *)m_ack.session_key,&dwSessionLen))
    {
        CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"%u xTEAEncryptWithKey failed", lpRecvHead->sid);
        return BUSIERR_INTERNAL;
    }
    CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"AFTER xTEAEncryptWithKey");

    m_ack.ret = 0;
    m_ack.user_id = m_uid;
    //检查版本
    bool bLogin = true;
    int canUseVersion = 0;
    int agreeVersion = 0;
    //int updateOption = CAppManager::GetInstance().GetUpdateOption();
    char szUpdateinfo[256] = {0}, szUpdateConfigBuyer[256] = {0}, szUpdateConfigSeller[256] = {0};

//  strcpy_s(m_ack.version_url,szUpdateUrl);
    if(m_uid < Id_Boundary) //商家
    {
        canUseVersion = CAppManager::GetInstance().GetCanUserver();
        agreeVersion = CAppManager::GetInstance().GetAgreever();
        strcpy(szUpdateConfigSeller, CAppManager::GetInstance().GetUpdateConfigSeller());
        strcpy(m_ack.update_config,szUpdateConfigSeller);
        strcpy(szUpdateinfo, CAppManager::GetInstance().GetUpdateInfo());
    }
    else if(m_uid < cs_Boundary)//买家
    {
        canUseVersion = CAppManager::GetInstance().GetCanUserver();
        agreeVersion = CAppManager::GetInstance().GetAgreever();
        strcpy(szUpdateConfigBuyer, CAppManager::GetInstance().GetUpdateConfigBuyer());
        strcpy(m_ack.update_config,szUpdateConfigBuyer);
        strcpy(szUpdateinfo, CAppManager::GetInstance().GetUpdateInfo());
    }
    else if(m_uid < work_Boundary)	//客服
    {
    	canUseVersion = CAppManager::GetInstance().GetCSCanUserver();
		agreeVersion = CAppManager::GetInstance().GetCSAgreever();
		strcpy(szUpdateConfigSeller, CAppManager::GetInstance().GetCSUpdateConfigSeller());
        strcpy(m_ack.update_config,szUpdateConfigSeller);		
		strcpy(szUpdateinfo, CAppManager::GetInstance().GetCSUpdateInfo());
    }
    else
    {
		canUseVersion = 0;
		agreeVersion = 0;
    }

    if(lpRecvHead->from >= agreeVersion)
    {
        m_ack.version_flag = (uint8)IMVERSION_AGREE;
    }
    else if(lpRecvHead->from >= canUseVersion)
    {
        if(m_uid < cs_Boundary)
        {
            m_ack.version_flag = (uint8)CAppManager::GetInstance().GetUpdateOption(lpRecvHead->from);
        }
        else
        {
            m_ack.version_flag = (uint8)CAppManager::GetInstance().GetCSUpdateOption(lpRecvHead->from);
        }
        strcpy(m_ack.update_info,szUpdateinfo);
    }
    else
    {
        bLogin = false;
        m_ack.version_flag = (uint8)IMVERSION_LOW;
        strcpy(m_ack.update_info,szUpdateinfo);
    }
    CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"AFTER UPDATE");

    if(bLogin)
    {
#ifdef XMPP_BUILD
        uRet = LoginSucess(lpReq,lphead,fd,buyerLevel);
#else
        uRet = LoginSucess(lpReq,lphead,buyerLevel);
#endif
        if(uRet != RESULT_SUCCESS)
        {
            CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"LoginSucess init fail !");
            return uRet;
        }
    }

    m_ack.publicip = pRecvData->src_ip;

    if(lphead->flag != XT_HEAD::FLAG_MOBREQ)
    {

        time_t tm;
        time(&tm);
        //组装正确的回应包
        CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));
        sp<<m_ack.ret;
        sp.WritePacket((const char *)m_ack.session_key,sizeof(m_ack.session_key));
        sp  <<m_ack.user_id
            <<m_ack.merchant_id
            <<m_ack.publicip
            <<m_ack.version_flag
            <<""
            <<m_ack.update_config
            <<m_ack.update_info
            <<uint8(0)
            <<szGuid
            <<szUserName
            <<(uint32)tm;
        CLog::Log("CBusiLogin",CLog::TYPE_DEBUG,"id:%d, LOGIN TIME %u", m_ack.user_id, tm);

        m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
    }
    else
    {
        int nlen = strlen(lpReq->sessionID);
        if(nlen >= 36)
        {
            memcpy(m_ack.sessionID,lpReq->sessionID,(nlen-36));
            m_ack.sessionID[nlen - 36] = '\0';
        }
        else
        {
            CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"GUID错误1");
        }
        CLog::Log("CBusiLogin",CLog::TYPE_DEBUG,"ack.sessionID:%s", m_ack.sessionID);
        memcpy(m_pSendData->data + sizeof(XT_HEAD), (char*)&m_ack, sizeof(XT_LOGIN_ACK));
        m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sizeof(XT_LOGIN_ACK));
    }


    CLog::Log("CBusiLogin::HandleReq", CLog::TYPE_IMPORTANT, "sid:%u, ret:%u, user_id:%u, merchant_id:%u, publicip:%u, version_flag:%u, guid:%s, username:%s, sessionid:%s",
              lpRecvHead->sid, m_ack.ret, m_ack.user_id, m_ack.merchant_id, m_ack.publicip, m_ack.version_flag, szGuid, lpReq->uLoginKey.username, m_ack.sessionID);
    return RESULT_SUCCESS;
}

void CBusiLogin::MD5Password(char* pPassword)
{
    unsigned char digest[16];

    char password[255] = {0};
    sprintf_s(password,"%s",pPassword);

    MD5_CTX_IM md5T;
    md5T.MD5Init();
    md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
    md5T.MD5Final (digest);

    string sHex=ToHexString(digest,16);

    strcpy(pPassword,sHex.c_str());

}


void CBusiLogin::ReLoginLogout(CUser * pUser,uint8 type)
{
    if(!pUser)
        return ;

    // 退出前的分流状态  
    uint8 nPreStatus = pUser->m_nStatus;		// 登录状态	
    uint8 nPreFlowStatus = pUser->m_nflow;		// 分流状态	

    pUser->m_nStatus = XTREAM_EXIT;


    //用户状态改变通知其他好友
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());
    pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();



    //保存用户在线时长

    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        time_t tm;
        time(&tm);

        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

        //上传登出动作
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(pUser->m_merchantid,pUser->m_nID);;
        try
        {
/*        
            AccountArray ids;
            ids.push_back(pUser->m_nID);
            std::vector<tagGSRV_GET_USER_FLOW_ACK::tagUserFlow> userFlow(ids.size());
            uint8_t cnt = ids.size();

            if(m_pRouter->GetGlobalBusi()->QueryUserFlow(&ids[0], &userFlow[0], cnt))
            {
                CLog::Log("ReLoginLogout",CLog::TYPE_ERROR,"id:%d ,Query User %u Flow Error",pUser->m_nID);
                cnt = 0;
            }
            else
            {
                if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //防止用户异常退出，没退出挂起状态
                {
                    SubMerchantAction info;
                    if(pUser->m_nID > cs_Boundary)
                    {
                        info.merchantID = -1;
                    }
                    else
                    {
                        info.merchantID = pUser->m_merchantid;
                    }
                    info.subMerchantID = pUser->m_nID;
                    info.actionTime = (int)tm;
                    info.actionType = ACTION_RECEPTION;

                    ResultInfo res = hess.GetHessiani()->uploadAction(info);
                    CLog::Log("ReLoginLogout",CLog::TYPE_IMPORTANT,
                              "Client Forget to change Hangup state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                              info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
                }
            }
*/           
			if(nPreStatus == XTREAM_LEAVE)
			{	// 发送离开结束
				CLog::Log("ReLoginLogout",CLog::TYPE_IMPORTANT,"商家[%u]退出时处于离开状态", pUser->m_nID);
				SubMerchantAction info;
				if(pUser->m_nID > cs_Boundary)
				{
					info.merchantID = -1;
				}
				else
				{
					info.merchantID = pUser->m_merchantid;
				}
				info.subMerchantID = pUser->m_nID;
				info.actionTime = (int)tm-3;
				info.actionType = ACTION_NOT_LEAVE;
				ResultInfo res = hess.GetHessiani()->uploadAction(info);
				CLog::Log("hessian",CLog::TYPE_IMPORTANT,
						  "Client Forget to change Leave state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
						  info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
			}
			if(nPreFlowStatus == 1)
			{	// 发送挂起结束
				CLog::Log("ReLoginLogout",CLog::TYPE_IMPORTANT,"商家[%u]退出时处于挂起状态", pUser->m_nID);
				SubMerchantAction info;
				if(pUser->m_nID > cs_Boundary)
				{
					info.merchantID = -1;
				}
				else
				{
					info.merchantID = pUser->m_merchantid;
				}
				info.subMerchantID = pUser->m_nID;
				info.actionTime = (int)tm-3;
				info.actionType = ACTION_RECEPTION;
				ResultInfo res = hess.GetHessiani()->uploadAction(info);
				CLog::Log("hessian",CLog::TYPE_IMPORTANT,
						  "Client Forget to change Hangup state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
						  info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
			}
			
            SubMerchantAction info;
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            //info.merchantID = pUser->m_merchantid;
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;

            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("ReLoginLogout",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("ReLoginLogout",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }

    }



    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

//    CUserManager *pUserManager = m_pRouter->GetUserManager();
//    pUser->DelFromOnlineList();
//    pUserManager->Del(pUser->m_nID);
#ifdef ACCESS_SERVER
    m_pAppNetwork->UserLogout(pUser->m_nID);
#endif
    CLog::Log("ReLoginLogout",CLog::TYPE_IMPORTANT,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        if(uid < Id_Boundary || uid >= cs_Boundary)
        {
            // 不是买家时才清空redis
            m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
        }
#ifdef IM_USE_MULTIDEVICE
        m_pRouter->GetGlobalBusi()->DelDeviceUserInfo(pUser->m_nID,pUser->m_nDeviceId);
#endif
    }
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();

}



#ifdef XMPP_BUILD
int CBusiLogin::LoginSucess(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint32 fd,uint8 level)
#else
int CBusiLogin::LoginSucess(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint8 level)
#endif
{
    //登陆成功处理

    CConfigInfo m_Config;
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(m_uid);
    int pUsertype = ((lpReq->login_flag & 0xC) >> 2);
    int LogoutCount=0;
    //   modify by zhangmin
    if(pUser != NULL)   //有退出线程正在退出，锁住用户
    {
        while(LogoutCount < 10)
        {
            if(! pUserManager->UserLogoutLockHandle(pUser->m_nID, CUserManager::LOGOUT_LOCK_GET))
            {
                pUser = pUserManager->Find(m_uid);  //更新一下pUser数据
                CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"2用户%d 地址%p 从等待中退出，继续处理登录操作!",m_uid,pUser.get());
                break;
            }
            LogoutCount++;
            CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"用户 %d正在删除操作中,等待中 %d!",m_uid,LogoutCount);
            sleep(1);
        }
    }

    //判断是否重登陆
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
    //    LPONLINE_USER_INFO lpOnlineUser = m_pAppNetwork->FindOnlineUser(m_uid);
    //在本服务器上的登录
    if(pUser != NULL /*&& lpOnlineUser && lpOnlineUser->isDirect*/)
    {
#ifdef IM_USE_MULTIDEVICE
        if(m_pRouter->GetGlobalBusi()->DeviceUserOnline(m_uid,pRecvData->src_ip,pRecvData->port))
#else
        if(pRecvData->src_ip == pUser->m_nIP && pRecvData->port == pUser->m_nPort)
#endif
        {
            // 如果是客服重登录，清空监控状态
            
            if(pUser != NULL)
            {
                pUser->g_cs_set_director(m_uid,(uint32)-1,m_pRouter->GetGlobalBusi());
                pUser->g_cs_set_chatting_buyer(m_uid,(uint32)-1,m_pRouter->GetGlobalBusi());
                //              pUserManager->AddUserToRedis(pUser);
            }


            //本地重登录
            CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"Id:%u relogin !",m_uid);
            memcpy(m_ack.session_key,pUser->m_SessionKey,sizeof(m_ack.session_key));
            // 重登时修改状态为在线
            if(m_uid >= Id_Boundary && m_uid < cs_Boundary)
            {
                tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser info;
                info.id = m_uid;
                info.status = lpReq->status;
                uint8 count = 1;
                m_pRouter->GetGlobalBusi()->ModiUserStatus(&info,count,NULL);
                CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"买家%u状态改为在线",m_uid);

                // 修改登录时间
                time_t tm;
                time(&tm);
                pUser->m_nLoginTime = (uint32)tm;
                m_pRouter->GetGlobalBusi()->ModiUserLoginTime(m_uid,(uint32)tm);
                CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"买家%u 更新登录时间%u",m_uid, (uint32)tm);
            }

            return RESULT_SUCCESS;
        }
        else
        {
            //异地重登录
            //登出先前的客户端

            TransitReLoginMsg(pUser->m_nID);
            //CBusiLogout busiLogout(m_pRouter);
            //busiLogout.Logout(pUser,clientService);
            //不使用上面代码避免read阻塞

            ReLoginLogout(pUser.get());

            if(pUser->m_nID < Id_Boundary || pUser->m_nID >= cs_Boundary)
            {
                pUser = NULL;
            }
        }
    }
    else if (pUser != NULL && pUser->m_bIsFullInfoValid)
    {
        CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"Id:%u server already has info !",m_uid);
        if(pUser->m_nID < Id_Boundary || pUser->m_nID >= cs_Boundary)
        {
            pUser->Reset();
        }
    }
#ifndef ACCESS_SERVER
    //广播其他服务器登出用户
    CConfigInfo m_ConfigInfo;
    list < uint32 >  ServerList;
    m_ConfigInfo.GetServerList(&ServerList);


    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    char* pMsgData = sendData.data + sizeof(XT_HEAD);

    CSendPacket SendPacket;
    SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

    SendPacket  << m_uid
                << (uint8)0xAA;

    sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

    list<uint32>::iterator it = ServerList.begin();
    for(; it !=  ServerList.end(); it++)
    {
        if(*it == (uint32)m_ConfigInfo.GetDeviceIp())
        {
            CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"跳过自己的服务器 %u 去掉用户 %d的全局信息 !",*it,m_uid);
            continue;
        }
        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_LOGOUT,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                sendData.data,sendData.data_len,0,0, *it, PORT_IM_SVRP2P);
        CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"通知服务器 %u 去掉用户 %d的全局信息 !",*it,m_uid);
    }
#endif

    // 制作该用户的P2P SESSIONKEY
    char p2pSessionKey[16] = {0};
    unsigned char digest1[16];
    m_md5.MD5Init();
    m_md5.MD5Update ((unsigned char*)m_ack.session_key,sizeof(m_ack.session_key));
    m_md5.MD5Final ((unsigned char*)digest1);
    unsigned char digest2[16];
    m_md5.MD5Init();
    m_md5.MD5Update ((unsigned char*)&m_uid,sizeof(m_uid));
    m_md5.MD5Final ((unsigned char*)digest2);
    for(int i=0; i<16; i++) p2pSessionKey[i]=digest1[i]&digest2[i];


    //重置用户信息
    if (pUser == NULL)
    {
        pUser = pUserManager->CreateNew();
        pUser->Reset();
        pUser->m_nID = m_uid;

        //CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"begin AddToOnlineList ");
        //        pUser->AddToOnlineList();
        //CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"finish AddToOnlineList ");

        SysStat::GetInstance().addOnlineNum();

        pUser->m_nUsertype = pUsertype;

    }



    pUser->m_nID = m_uid;
    strcpy_s(pUser->m_szUserName,lpReq->uLoginKey.username);


    //更新用户状态
    if (lpReq->status!= XTREAM_ONLINE && lpReq->status != XTREAM_HIDE && XTREAM_LEAVE != XTREAM_LEAVE)
        lpReq->status = XTREAM_ONLINE;

    pUser->m_nStatus = lpReq->status;
    pUser->m_nIP     = pRecvData->src_ip;
    pUser->m_nPort   = pRecvData->port ;
    pUser->m_nLanIP  = lpReq->lanip;
    pUser->m_nLanPort= lpReq->lanport;
    pUser->m_nNewlyLiveTime = timeGetTime();
    pUser->m_nServerIp = m_Config.GetDeviceIp();
    pUser->flag     = lpRecvHead->flag;
    pUser->m_nLevel = level;
#ifdef XMPP_BUILD    
    pUser->m_nXmppID = fd;
#endif    
    time_t tm;
    time(&tm);
    pUser->m_nLoginTime = (uint32)tm;
    memcpy(pUser->m_SessionKey,m_ack.session_key,MAX_SESSIONKEY_LEN);
    memcpy(pUser->m_P2PSessionKey,p2pSessionKey,MAX_SESSIONKEY_LEN);
    pUser->m_ClientVersion = lpRecvHead->from;
    pUser->m_nLoginType = lphead->flag;
    pUser->m_nflow = 0;
#ifdef IM_USE_MULTIDEVICE
    pUser->m_nDeviceId = lphead->deviceId;
    CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"id:%d,deviceID %u",pUser->m_nID, pUser->m_nDeviceId);

#endif

    CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"id:%d, Login Type %x Level %d",pUser->m_nID, pUser->m_nLoginType,pUser->m_nLevel);

    //调用的应用网络层接口
#ifdef ACCESS_SERVER
    m_pAppNetwork->UserLogin(m_pRouter->GetGlobalBusi(), m_uid,pRecvData->src_ip,
                             pRecvData->port,(const char *)m_ack.session_key,
                             p2pSessionKey,lpReq->lanip,lpReq->lanport,lpRecvHead->flag);
#endif


    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        m_pRouter->GetDataBaseMini()->Save_Merchant_Login(m_uid,(int)tm);
        //上传登录动作
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(m_ack.merchant_id,m_uid);
        pUser->m_merchantid = 0;
        {
            try
            {
                SubMerchantAction info;
                if(pUser->m_nID > cs_Boundary)
                {
                    info.merchantID = -1;
                }
                else
                {
                    info.merchantID = m_ack.merchant_id;
                }
                info.subMerchantID = m_uid;
                info.actionTime = (int)tm;
                info.actionType = ACTION_LOGIN;
                pUser->m_merchantid = m_ack.merchant_id;

                ResultInfo res = hess.GetHessiani()->uploadAction(info);


                CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                          info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());

				if(pUser->m_nStatus == XTREAM_LEAVE)
				{	// 商家已离开状态登录，上传离开动作
					SubMerchantAction info;
					if(pUser->m_nID > cs_Boundary)
					{					
						info.merchantID = -1;
					}
					else
					{
						info.merchantID = m_ack.merchant_id;
					}
					info.subMerchantID = m_uid;
					info.actionTime = (int)tm+1;
					info.actionType = ACTION_LEAVE;
					ResultInfo res = hess.GetHessiani()->uploadAction(info);
					CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
							info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
				}	
            }
            catch (std::exception& e)
            {

                CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                          m_ack.merchant_id, m_uid, e.what());
            }
        }
    }

    //将web用户向全局更新一下
    if(pUser->m_nID  > Id_Boundary && pUser->m_nID <cs_Boundary)
    {
        CLog::Log("LoginSuccess", CLog::TYPE_IMPORTANT, "向全局注册了一个WEB用户：%d 状态 %d", pUser->m_nID,pUser->m_nStatus);
        m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0);
        /*
                // 保存买家登录
                CLog::Log("LoginSuccess", CLog::TYPE_IMPORTANT, "买家登录时间记录到数据库id[%u] logintime[%u]", m_uid,(int)tm);
                m_pRouter->GetDataBaseMini()->Save_Buyer_Login(m_uid,(int)tm);
        */
    }
    else
    {
        // 商家登录也要同步到redis
        // 用户全量信息写到redis
        pUserManager->AddUserToRedis(pUser);
    }

#ifdef IM_USE_MULTIDEVICE
    m_pRouter->GetGlobalBusi()->AddModiDeviceUserInfo(pUser);   //登录的时候只在这里更新一次
#endif


    return RESULT_SUCCESS;
}

//重登录时，给先前登录的发重登陆退出提示
//做为已登陆用户的一个业务处理
int CBusiLogin::TransitReLoginMsg(uint32 udi)
{
    CNotifyProtocol::Relogin(m_pRouter->GetGlobalBusi(),udi);
    return 0;
}

int CBusiLogin::SendReply()
{
    return 0;
}


bool CBusiLogin::SaveBuyer(char* szUserName, char* cPass, char* szGuid, uint32& userId, char *szMobileNo, char *szEmail, char *szNickName, char* szHeadImgUrl)
{
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    string saveBuyerUrl(CAppManager::GetInstance().GetConfig().GetImLoggerBaseUrl());
    saveBuyerUrl += "/login/save-buyer";


    char szUserNameUtf8[MAX_USERNAME_LEN + 1] = "";
    char szNickNameUtf8[61]="";

    gbk2utf8(szUserNameUtf8,szUserName);
    gbk2utf8(szNickNameUtf8,szNickName);


    stringstream ssParams;
    ssParams<<"userName=" << szUserNameUtf8;
    ssParams<<"&password=" << cPass;
    ssParams<<"&memGuid=" << szGuid;
    ssParams<<"&mobileNo=" << szMobileNo;
    ssParams<<"&email=" << szEmail;
    ssParams<<"&nickName=" << szNickNameUtf8;
    ssParams<<"&headImgUrl=" << szHeadImgUrl;

    string saveBuyerRespose;
    int httpStaus = httpClient.Post(saveBuyerUrl, ssParams.str(), saveBuyerRespose);

    if(httpStaus == CURLE_OK )
    {
        Json::Reader jReader(Json::Features::strictMode());
        Json::Value jvRecvResponse;
        if (jReader.parse(saveBuyerRespose, jvRecvResponse))
        {
            if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true")
            {
                if(jvRecvResponse.isMember("data"))
                {
                    if(jvRecvResponse["data"].isMember("userId"))
                    {
                        userId =jvRecvResponse["data"]["userId"].asUInt();
                        return true;
                    }
                }
            }
        }
    }

    userId = 0;
    return false;

}


bool CBusiLogin::SaveSeller(char * account,char* pwd,uint32 id, char* truename)
{
    CHttpClient httpClient;

    string saveSellerUrl(CAppManager::GetInstance().GetConfig().GetImLoggerBaseUrl());



    char szUserNameUtf8[MAX_USERNAME_LEN + 1] = "";
    char szNickNameUtf8[61]="";

    gbk2utf8(szUserNameUtf8,account);
    gbk2utf8(szNickNameUtf8,truename);

    saveSellerUrl += "/login/save-seller";

    stringstream ssParams;
    ssParams<<"userName=" << szUserNameUtf8;
    ssParams<<"&password=" << pwd;
    ssParams<<"&userId=" << id;
    ssParams<<"&trueName=" << szNickNameUtf8;


    string saveBuyerRespose;

    int httpStatus = httpClient.Post(saveSellerUrl, ssParams.str(), saveBuyerRespose);

    return (httpStatus == CURLE_OK ? true : false);
}
bool CBusiLogin::CheckWork(const char* user, const char* password,int &code,
                           string &szUserName,string &szEmail,string &szMobileNumber,string &structure)
{


    CHttpClient httpclient;

    string data;
    string sPostParam;
    CConfigInfo config;

    //sPostParam += config.GetimapiUrl();
    string url;
    //url += "http://backend-merchant.beta.fn.com:8080/rest/userLoginValidateApi/userLoginValidate";
    url += config.GetWorkURL();
    sPostParam += "param={operator_name=";
    sPostParam += user;
    sPostParam += ",operator_password=";
    sPostParam += password;
    sPostParam += "}";
    int res = httpclient.Post(url,sPostParam,data);

    // 0 为成功
    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("CheckWork",CLog::TYPE_ERROR,"失败，返回结果为res %d data %s",res,data.c_str());

        return false;
    }

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("CheckWork",CLog::TYPE_DEBUG,"JSON:%s",data.c_str());
    if (reader.parse(data, recv))
    {
        if(!recv.isMember("code") && !recv["code"].isInt())
        {
            return false;
        }
        code = recv["code"].asInt();
        if(code != 200)
        {
            //用户不存在
            CLog::Log("CheckWork",CLog::TYPE_ERROR,"账号或密码错误 code: %d",code);
            return true;
        }
        else
        {
            if(recv.isMember("data"))
            {
                Json::Reader reader1(Json::Features::strictMode());
                string workInfoStr = recv["data"].asString();
                Json::Value workInfo;
                if (reader.parse(workInfoStr,workInfo))
                {
                    if(workInfo["operator_realname"].isString())
                    {
                        szUserName = workInfo["operator_realname"].asString();
                    }
                    if(workInfo["email"].isString())
                    {
                        szEmail = workInfo["email"].asString();
                    }
                    if(workInfo["mobile_number"].isString())
                    {
                        szMobileNumber = workInfo["mobile_number"].asString();
                    }
                    if(workInfo["name"].isString())
                    {
                        structure = workInfo["name"].asString();
                    }
                    return true;
                }
                else
                {
                    //json串解析失败
                    CLog::Log("CheckWork", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail", data.c_str());
                    return false;
                }
            }
            //找到了子串
            //json串解析失败
            CLog::Log("CheckWork", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail", data.c_str());
            return false;
        }
    }
    else
    {
        //json串解析失败
        CLog::Log("CheckWork", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail", data.c_str());
        return false;
    }


}

bool CBusiLogin::CheckBuyer(const char* user, const char* password, char* guid,  char * szUserName)
{
#ifdef PRE_INTERNAL
    string err_msg("");
    strcpy(guid,"123456789012345678901234567890123456");
    strcpy(szUserName,user);
    if(!AD_Auth(user,password,err_msg))
    {
        CLog::Log("CheckBuyer-AD_Auth",CLog::TYPE_DEBUG,"errormsg: %s",err_msg.c_str());
        return false;
    }
    else
    {
        CLog::Log("CheckBuyer-AD_Auth",CLog::TYPE_DEBUG,"AD_Auth success");
        return true;
    }

#endif
    //默认使用用户名登陆
    std::string sPostParam = "type=3";
    if (strlen(user) == 11 && user[0] == '1' && atoi(user) != 0)    //使用手机号登陆
    {
        sPostParam = "type=2";
    }
    else if (CheckEmail(user))      //使用Email登陆
    {
        sPostParam = "type=1";
    }

    //加入用户名
    sPostParam += "&username=";
    sPostParam += user;
    //加入密码
    sPostParam += "&password=";
    sPostParam += Base64Encode(password, strlen(password));

    std::string data;
    std::string url = CAppManager::GetInstance().GetConfig().GetBuyerURL();
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    CURLcode res = (CURLcode)httpClient.Post(url,sPostParam,data);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"Get buyer infomation error!");
        return false;
    }

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(data, recv))
    {
        //解析Header中的Status
        string status = recv["Header"]["Status"].asString();

        //判断是否有子串012002100
        if(status.find("012002100") != std::string::npos)
        {
            string strGuid = recv["Body"]["MEM_GUID"].asString();
            string strUserName = recv["Body"]["MEM_USERNAME"].asString();

            strcpy(guid,strGuid.c_str());
            strcpy(szUserName, strUserName.c_str());
            //找到了子串
            return true;
        }
    }

    //没找到子串
    CLog::Log("CheckBuyer",CLog::TYPE_ERROR," The status didn't have 012002100!");
    return false;
}

#if 1
bool CBusiLogin::CheckBuyerLevel(char* guid,  uint8 &level)
{

    Json::FastWriter writer;
    Json::Value post_val;
    post_val["memGuid"] = guid;

    string sPostParam = "data=";
    sPostParam += writer.write(post_val);

    std::string data;
    CLog::Log("CheckBuyerLevel",CLog::TYPE_IMPORTANT,"Request Level post %s!",sPostParam.c_str());

    std::string url = CAppManager::GetInstance().GetConfig().GetBuyerLevelURL();
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    CURLcode res = (CURLcode)httpClient.Post(url,sPostParam,data);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"Get buyer infomation error!");
        return false;
    }

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(data, recv))
    {
        if(recv["code"].asUInt() == 100)
        {
            char stackLevel[32] = {0};

            int len = recv["data"]["memLevel"].asString().size();
            if (len > 0)
            {
                strncpy(stackLevel,recv["data"]["memLevel"].asCString(),len > 3 ? 3: len);
            }


            if(stackLevel[0] == 'T')
            {
                switch(stackLevel[1])
                {
                    case '0':
                        level = 0;
                        break;
                    case '1':
                        level = 1;
                        break;
                    case '2':
                        level = 2;
                        break;
                    case '3':
                        level = 3;
                        break;
                }
                CLog::Log("CheckBuyerLevel",CLog::TYPE_IMPORTANT,"Get buyer level %s level %d",stackLevel,level);
                return true;
            }
            else
            {
                CLog::Log("CheckBuyerLevel",CLog::TYPE_ERROR,"Get buyer level %s level %d",stackLevel,level);
                return false;
            }

        }

    }
    CLog::Log("CheckBuyerLevel",CLog::TYPE_ERROR," get level fail!");
    return false;

}
#endif



string CBusiLogin::TransPwd(const char* pwd)
{
//  CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "PWD:%s",pwd);

    string symbol_space = "+";
    string symbol_split = "&";
    string symbol_special = "%";

    string url_space = "%2B";
    string url_split = "%26";
    string url_special = "%25";

    string UrlPwd(pwd);

    string::size_type nPos = 0;

    nPos = UrlPwd.find(symbol_special,0);
    while(nPos != string::npos)
    {
        UrlPwd.replace(nPos,symbol_special.length(),url_special);

        nPos = UrlPwd.find(symbol_special,nPos + 1);
    }

    nPos = UrlPwd.find(symbol_split,0);
    while(nPos != string::npos)
    {
        UrlPwd.replace(nPos,symbol_split.length(),url_split);

        nPos = UrlPwd.find(symbol_split,nPos + 1);
    }

    nPos = UrlPwd.find(symbol_space,0);
    while(nPos != string::npos)
    {
        UrlPwd.replace(nPos,symbol_space.length(),url_space);

        nPos = UrlPwd.find(symbol_space,nPos + 1);
    }

//  CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "Tran PWD:%s",UrlPwd.c_str());

    return UrlPwd;
}


bool CBusiLogin::CheckSellerMysql(const char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, uint32_t& state)
{
    char cPassMysql[MAX_PASSWORD_LEN];
    char cUserUtf8[MAX_USERNAME_LEN];
    gbk2utf8(cUserUtf8, user);
    if(MYSQL_LIB->getPwdByName(cUserUtf8, uid, cPassMysql, merchant_id, state) == DB_SUCCESS)
    {
        char cPass[MAX_PASSWORD_LEN];
        strncpy(cPass, pwd, MAX_PASSWORD_LEN);
        MD5Password(cPass);
        if(strcmp(cPassMysql,cPass) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}





bool CBusiLogin::CheckSeller(char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, int &state, string &code)
{
    // 客户端ip
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    sockaddr_in p;
    p.sin_addr.s_addr=pRecvData->src_ip;
    char szClientIP[64] = {0};
    strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));

    // 服务端ip
    char szServerIP[64] = {0};
    CAppManager::GetInstance().GetConfig().GetDeviceIp(szServerIP);

    //特殊符号转义
    merchant_id = 0;
    uid = 0;
    string password = TransPwd(pwd);
    if(password.empty())
    {
        CLog::Log("CBusiLogin::CheckSeller", CLog::TYPE_ERROR, "password:%s trans is NULL",pwd);
        return false;
    }

    //初始化http　get请求
    //params={"loginName":"zhangkai01","password":"pass1234","clientIp":"127.0.0.1","localIp":"10.212.192.81","channelFrom":"moumou","channel":"1","version":"v1.1"}
    char szParam[1024] = {0};
    sprintf_s(szParam, "params={\"loginName\":\"%s\",\"password\":\"%s\",\"clientIp\":\"%s\",\"localIp\":\"%s\",\"channelFrom\":\"moumou\",\"channel\":\"1\",\"version\":\"v1.1\"}",
              user, password.c_str(), szClientIP, szServerIP);
    //CLog::Log("CBusiLogin",CLog::TYPE_DEBUG, "Login param:%s", szParam);

    // 转换为utf8编码
    char szParamU[1024] = {0};
    gbk2utf8(szParamU,szParam);
    std::string sPostParam = szParamU;
    std::string data;
    std::string url = CAppManager::GetInstance().GetConfig().GetSellerURL();
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    CURLcode res =(CURLcode) httpClient.Post(url,sPostParam,data,false);

    //判断是否获取成
    if (res != CURLE_OK)
    {
        CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error!");
        return false;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json字符创
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("CheckSeller",CLog::TYPE_DEBUG,"user:%s, data:%s",user, szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        //获取code字段
        if(recv.isMember("code"))
        {
            code = recv["code"].asString();
        }
        else
        {
            CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error! no auth field");
        }

        // 继续解析
        if(!recv.isMember("auth"))
        {
            CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error! no auth field");
        }
        else
        {
            Json::Value jAuth = recv["auth"];
            //获取auth中的memberId字段
            if(jAuth.isMember("memberId"))
            {
                uid = jAuth["memberId"].asInt();
            }
            //获取auth中的merchantId字段
            if(jAuth.isMember("merchantId"))
            {
                merchant_id = jAuth["merchantId"].asInt();
            }
            //username
            if(jAuth.isMember("username"))
            {
                string username = jAuth["username"].asString();
                strncpy(user, username.c_str(), MAX_USERNAME_LEN);
            }
            // state
            if(jAuth.isMember("state"))
            {
                state = jAuth["state"].asInt();
            }
        }

        return true;
    }

    return false;
}

// 检查用户是否同地登录     返回值 1 通地登录 0 异地登录  -1 调用失败，调用失败时，会返回错误信息
int CBusiLogin::IsSameAreaLogin(const char* loginname, string &strError)
{
    strError = "用户名或密码错误";

    // 客户端ip
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    sockaddr_in p;
    p.sin_addr.s_addr=pRecvData->src_ip;
    char szClientIP[64] = {0};
    strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));

    //初始化http　get请求
    char szParam[1024] = {0};
    sprintf_s(szParam, "params={\"loginName\":\"%s\",\"clientIp\":\"%s\"}",
              loginname, szClientIP);
    CLog::Log("IsSameAreaLogin",CLog::TYPE_IMPORTANT, "param:%s", szParam);

    // 转换为utf8编码
    char szParamU[1024] = {0};
    gbk2utf8(szParamU,szParam);

    std::string data;
    std::string sPostParam = szParamU;
    std::string url = CAppManager::GetInstance().GetConfig().GetUrlIsSameAreaLogin();
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    CURLcode res = (CURLcode) httpClient.Post(url,sPostParam,data);

    //判断是否获取成
    if (res != CURLE_OK)
    {
        CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "Get seller infomation error!");
        return -1;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json字符创
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("IsSameAreaLogin",CLog::TYPE_IMPORTANT,"user:%s, data:%s",loginname, szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        //获取code字段
        string code;
        if(recv.isMember("code"))
        {
            code = recv["code"].asString();

            if(code != "200")
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }

        // 继续解析
        string data;
        if(recv.isMember("data"))
        {
            data = recv["data"].asString();
        }
        else
        {
            CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "Get seller infomation error! no data field");
            return -1;
        }

        if(data == "true")
        {
            CLog::Log("IsSameAreaLogin", CLog::TYPE_IMPORTANT, "同一地址登录");
            strError = "";
            return 1;
        }

        CLog::Log("IsSameAreaLogin", CLog::TYPE_IMPORTANT, "异地登录");
        strError = "";
        return 0;
    }

    CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "json解析失败");
    return -1;
}

// 检查用户是否绑定手机     返回值 1 绑定了手机  0 未绑定手机  -1 调用失败
int CBusiLogin::IsBindMobile(const char* loginname, string &strError)
{
    strError = "用户名或密码错误";
    //初始化http　get请求
    char szParam[1024] = {0};
    sprintf_s(szParam, "params={\"loginName\":\"%s\"}",
              loginname);
    CLog::Log("IsBindMobile",CLog::TYPE_IMPORTANT, "param:%s", szParam);

    // 转换为utf8编码
    char szParamU[1024] = {0};
    gbk2utf8(szParamU,szParam);

    std::string data;
    std::string sPostParam = szParamU;

    std::string url = CAppManager::GetInstance().GetConfig().GetUrlIsBindMobile();
    CHttpClient httpClient;
    httpClient.SetDebug(true);
    CURLcode res = (CURLcode) httpClient.Post(url,sPostParam,data);

    //判断是否获取成
    if (res != CURLE_OK)
    {
        CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "Get seller infomation error!");
        return -1;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json字符创
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("IsBindMobile",CLog::TYPE_IMPORTANT,"user:%s, data:%s",loginname, szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        //获取code字段
        string code;
        if(recv.isMember("code"))
        {
            code = recv["code"].asString();

            if(code != "200")
            {
                return -1;
            }
        }
        else
        {
            CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "Get seller infomation error! no code field");
            return -1;
        }

        // 继续解析
        string data;
        if(recv.isMember("data"))
        {
            data = recv["data"].asString();
        }
        else
        {
            CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "Get seller infomation error! no data field");
            return -1;
        }

        if(data == "true")
        {
            CLog::Log("IsBindMobile", CLog::TYPE_IMPORTANT, "绑定手机");
            strError = "";
            return 1;
        }

        CLog::Log("IsBindMobile", CLog::TYPE_IMPORTANT, "未绑定手机");
        strError = "";
        return 0;
    }

    CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "json解析失败");
    return -1;
}

// AD认证接口
bool CBusiLogin::AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo)
{
    // 初始化
    LDAP *pLdap = ldap_init((char *)AD_HOSTNAME, (int)AD_PORT);
    if(pLdap == NULL)
    {
        strErrorInfo = "连接AD认证服务器失败！";
        return FALSE;
    }

    // 连接AD认证服务器
    /*
        unsigned long  rlt = ldap_connect(pLdap, NULL);
        if(LDAP_SUCCESS != rlt)
        {
            ldap_unbind_s(pLdap);
            strErrorInfo = "连接AD认证服务器失败！";
            return FALSE;
        }
    */
    // 认证
    string strUser("FN\\");
    //strUser.Format("FN\\%s", szUserName);
    strUser += string(szUserName);
    int rlt = ldap_simple_bind_s(pLdap, (char *)strUser.c_str(), (char *)szPwd);
    if(LDAP_SUCCESS != rlt)
    {
        ldap_unbind_s(pLdap);
        strErrorInfo = "认证失败，用户名或密码错误！";
        return FALSE;
    }

    ldap_unbind_s(pLdap);
    strErrorInfo = "认证成功！";
    return TRUE;
}




/*///////////////////////////////
 BusiLogout.cpp
////////////////////////////////*/
CBusiLogout::CBusiLogout(CRouterBusiness* pRouter):
    CBusinessExIMSrv(pRouter,CMD_LOGOUT,0)
{
    m_pAppNetwork = &(CAppManager::GetInstance().GetAppNetWork());
}


CBusiLogout::~CBusiLogout(void)
{
}

int CBusiLogout::DoWork()
{
    //初始化基本数据
#if defined IM_USE_MULTIDEVICE || defined XMPP_BUILD   
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
#endif
    XT_LOGOUT * lpReq = (XT_LOGOUT *)m_RecvPacket.GetData();
#ifdef XMPP_BUILD
    LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pSendData->data;
#endif

    CLog::Log("CBusiLogout",CLog::TYPE_IMPORTANT,"Id:%d auth %#x IN",lpReq->id,lpReq->authData);

    if(lpReq->authData != 0x00  && lpReq->authData != 0xAA && lpReq->authData != 0x01 && lpReq->authData != 0x02)
    {
        CLog::Log("CBusiLogout",CLog::TYPE_ERROR,"Id:%d,接收到一个错误登出包:%d",lpReq->id,lpReq->authData);
        return 1;
    }

    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(lpReq->id, TRUE);

    if(pUser != NULL)
    {
#ifdef IM_USE_MULTIDEVICE
        //设置deviceID
        pUser->m_nDeviceId = lpRecvHead->deviceId;
#endif
        if(lpReq->authData == 0xAA)
        {
            CNotifyProtocol::Relogin(m_pRouter->GetGlobalBusi(),lpReq->id);
            CLog::Log("CBusiLogout",CLog::TYPE_IMPORTANT,"登出其他server分布式重登录用户 %u",lpReq->id);

        }
        if(m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ)
        {
            WebLogout(pUser.get(),lpReq->authData);

        }
        else
        {
            Logout(pUser.get(),lpReq->authData);

        }
#ifdef XMPP_BUILD
        //根据用户登录方式判断是否将返回值发送给jabberdIM线程
        if(pUser->m_nXmppID > 0)
        {
            m_pSendData->data_len =  sizeof(XT_HEAD);
            CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp %u, length:%d", pUser->m_nXmppID,m_pSendData->data_len);
            InitHead(XT_HEAD::FLAG_XMPPACK,lpSendHead,CMD_LOGOUT,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,pUser->m_nID);
            CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_pSendData->data_len, pRecvData->vlink);
        }
#endif
    }


    CLog::Log("CBusiLogout", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, authData:%u OUT" , m_pRecvHead->sid, m_pRecvHead->did,m_pRecvHead->flag, lpReq->authData);

    return 0;
}

int CBusiLogout::SendReply()
{
    return 0;
}


void CBusiLogout::Logout(CUser * pUser,uint8 type)
{
    if(!pUser)
        return ;

    // 退出前的分流状态  
    uint8 nPreStatus = pUser->m_nStatus;		// 登录状态	
    uint8 nPreFlowStatus = pUser->m_nflow;		// 分流状态	    

    pUser->m_nStatus = XTREAM_EXIT;



    //用户状态改变通知其他好友
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());
    pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();



    //保存用户在线时长
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        time_t tm;
        time(&tm);
        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

        //上传登出动作
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(pUser->m_merchantid,pUser->m_nID);;
        try
        {
/*        
            AccountArray ids;
            ids.push_back(pUser->m_nID);
            std::vector<tagGSRV_GET_USER_FLOW_ACK::tagUserFlow> userFlow(ids.size());
            uint8_t cnt = ids.size();

            if(m_pRouter->GetGlobalBusi()->QueryUserFlow(&ids[0], &userFlow[0], cnt))
            {
                CLog::Log("Logout",CLog::TYPE_ERROR,"id:%d ,Query User %u Flow Error",pUser->m_nID);
                cnt = 0;
            }
            else
            {
                if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //防止用户异常退出，没退出挂起状态
                {
                    SubMerchantAction info;
                    if(pUser->m_nID > cs_Boundary)
                    {
                        info.merchantID = -1;
                    }
                    else
                    {
                        info.merchantID = pUser->m_merchantid;
                    }
                    info.subMerchantID = pUser->m_nID;
                    info.actionTime = (int)tm;
                    info.actionType = ACTION_RECEPTION;

                    ResultInfo res = hess.GetHessiani()->uploadAction(info);
                    CLog::Log("hessian",CLog::TYPE_IMPORTANT,
                              "Client Forget to change Hangup state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                              info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
                }
            }
*/
			if(nPreStatus == XTREAM_LEAVE)
			{	// 发送离开结束
				CLog::Log("CBusiLogout",CLog::TYPE_IMPORTANT,"商家[%u]退出时处于离开状态", pUser->m_nID);
				SubMerchantAction info;
				if(pUser->m_nID > cs_Boundary)
				{
					info.merchantID = -1;
				}
				else
				{
					info.merchantID = pUser->m_merchantid;
				}
				info.subMerchantID = pUser->m_nID;
				info.actionTime = (int)tm-3;
				info.actionType = ACTION_NOT_LEAVE;
				ResultInfo res = hess.GetHessiani()->uploadAction(info);
				CLog::Log("hessian",CLog::TYPE_IMPORTANT,
						  "Client Forget to change Leave state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
						  info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
			}
			if(nPreFlowStatus == 1)
			{	// 发送挂起结束
				CLog::Log("CBusiLogout",CLog::TYPE_IMPORTANT,"商家[%u]退出时处于挂起状态", pUser->m_nID);
				SubMerchantAction info;
				if(pUser->m_nID > cs_Boundary)
				{
					info.merchantID = -1;
				}
				else
				{
					info.merchantID = pUser->m_merchantid;
				}
				info.subMerchantID = pUser->m_nID;
				info.actionTime = (int)tm-3;
				info.actionType = ACTION_RECEPTION;
				ResultInfo res = hess.GetHessiani()->uploadAction(info);
				CLog::Log("hessian",CLog::TYPE_IMPORTANT,
						  "Client Forget to change Hangup state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
						  info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
			}
			
            SubMerchantAction info;
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            //info.merchantID = pUser->m_merchantid;
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;

            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }

        CThreadMsgNotify::DeleteSellerData(pUser->m_nID);
		
        //  4 : 客服下线，service_id必填。
        imapi csOfflineReq;
        string info;
        csOfflineReq.imapiReq(imapi::CSOFFLINEREQ,info,pUser->m_nID);

    }


    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

//    CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
//    pUser->DelFromOnlineList();
//    userManager.DelHash(pUser->m_nID);
    m_pAppNetwork->UserLogout(pUser->m_nID);
    CLog::Log("Logout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
#ifdef IM_USE_MULTIDEVICE
        m_pRouter->GetGlobalBusi()->DelDeviceUserInfo(pUser->m_nID,pUser->m_nDeviceId);
#endif
    }
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();

}



//#define MAX_NUM 20
//判断是否需要删除全局服务器信息
//如果是其他服务器发过来的0xaa请求就不删除，只等对方服务器更新即可
//以免造成先写入，后删除的BUG
//

void CBusiLogout::ClearCSContext(uint32 selfID,uint32 teamid,uint32 toid)
{
    //查询全局服务器并清除web所在服务器其接待上下文
    CLog::Log("CBusiLogout::ClearCSContext",CLog::TYPE_DEBUG,"准备发送到指定服务器清除客服%u接待",toid);
    if(toid == 0)
    {
        return;
    }
    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&toid,&UserInfo,count);

    if(UserInfo.uid != toid)
    {
        CLog::Log("CBusiLogout::ClearCSContext",CLog::TYPE_DEBUG,"获取客服%d信息失败,清空其接待状态可能失败",toid);
        return;
    }
    else
    {
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
        char* pMsgData = sendData.data + sizeof(XT_HEAD);
        CSendPacket SendPacket;
        SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);
        SendPacket << (uint32)0
                   << selfID
                   << toid
                   << teamid;
        sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();
        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_SET_CS_CONSULT_REQ,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                sendData.data,sendData.data_len,0,0,UserInfo.srvip, PORT_IM_SVRP2P);
        CLog::Log("CBusiLogout::ClearCSContext",CLog::TYPE_IMPORTANT,"通知服务器 %u 去掉客服 %u中的接待信息 !",UserInfo.srvip,toid);
    }
}

void CBusiLogout::SendWebQueueInfoIND(CSendPacket& m_SendPacket)
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

void CBusiLogout::WebLogout(CUser * pUser,uint8 type)
{
    if(type == 0x01 || type==0x02 ) //清除WEB用户信息,只做上面的操作
    {
        return;
    }


    //modify by zhangmin
    if(type == 0x00)
    {
        CLog::Log("WebLogout",CLog::TYPE_DEBUG,"lock uid:%d,type:%d ",pUser->m_nID,type);

        CUserManager *pUserManager = m_pRouter->GetUserManager();
        pUserManager->UserLogoutLockHandle(pUser->m_nID, CUserManager::LOGOUT_LOCK_ADD);

    }

    const uint8 MAX_NUM = 20;
    if(pUser == NULL)
        return ;
    //uint32 CSuserid = pUser->getCsuserId();
    //uint32 teamid = pUser->getCsTeamId();

    pUser->m_nStatus = XTREAM_EXIT;

    // 修改redis中买家状态
    tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser info;
    info.id = pUser->m_nID;
    info.status = XTREAM_EXIT;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->ModiUserStatus(&info,count,NULL);
    CLog::Log("CBusiLogout::WebLogout",CLog::TYPE_IMPORTANT,"修改买家状态为离线 [%d] ",pUser->m_nID);

    //用户状态改变通知其他好友
    XT_WEB_BUSI_LIST_ACK::BusiList pListInfo[MAX_NUM];
    uint32 nRetCount = MAX_NUM;
    uint32 id_list[MAX_NUM];
    tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_NUM];
    HttpGetRecentSubMerchant(pUser->m_nID, MAX_NUM, pListInfo, nRetCount);

    if(nRetCount > 0)
    {
        uint8 count = nRetCount;
        for(uint32 j = 0; j < nRetCount; j ++)
        {
            id_list[j] = pListInfo[j].id;
        }
#ifdef IM_USE_CACHE
        if (MEMCACHED_NO_INFO < CMemcacheQueryUserStatusi(m_pRouter,id_list,count,UStatus))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"%d QueryUserStatus error !",pUser->m_nID);
        }
#else
        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(id_list,UStatus,count))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"%d QueryUserStatus error !",pUser->m_nID);
        }
#endif
        for(int i = 0; i < count; i ++)
        {
            if(UStatus[i].status != XTREAM_OFFLINE)
            {
                FriendInfo* friendInfo = pUser->m_FriendList.Add(UStatus[i].id);
                if(friendInfo)
                {
                    friendInfo->id = UStatus[i].id;
                    friendInfo->status = UStatus[i].status;
                }
            }
        }
    }
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();


    //保存用户在线时长
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        time_t tm;
        time(&tm);
        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(pUser->m_merchantid,pUser->m_nID);
        try
        {

            SubMerchantAction info;
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;


            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchantID:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }
    }

    // 买家退出时间记录到数据库
    time_t tm;
    time(&tm);
    CLog::Log("WebLogout",CLog::TYPE_IMPORTANT,"买家退出时间记录到数据库 id[%u] logouttime[%u]", pUser->m_nID,(int)tm);
    m_pRouter->GetDataBaseMini()->Save_Buyer_Logout(pUser->m_nID,(int)tm);

    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

//   CUserManager *pUserManager = m_pRouter->GetUserManager();
//    pUser->DelFromOnlineList();
//    pUserManager->Del(pUser->m_nID);
#ifdef ACCESS_SERVER
    m_pAppNetwork->UserLogout(pUser->m_nID);
#endif
    CLog::Log("WebLogout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
#ifdef IM_USE_MULTIDEVICE
        m_pRouter->GetGlobalBusi()->DelDeviceUserInfo(pUser->m_nID,pUser->m_nDeviceId);
#endif


    }

    // modify by zhangmin
    if(type == 0x00)
    {
        CLog::Log("WebLogout",CLog::TYPE_DEBUG,"unlock uid:%d,type:%d ",uid,type);

        CUserManager *pUserManager = m_pRouter->GetUserManager();
        pUserManager->UserLogoutLockHandle(pUser->m_nID, CUserManager::LOGOUT_LOCK_DEL);

    }

    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();

}
#endif
