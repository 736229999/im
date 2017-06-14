#include "HttpInterface.h"
#include "httpclient.h"
#include "json/json.h"
#include "Log.h"
#include "AppManager.h"
#include <string>
#include "curl/curl.h"
#include "Function.h"


using namespace std;

// 获取最近联系商家
void HttpGetRecentSubMerchant(uint32 nUserID, int nMaxCount, XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount)
{
	nRetCount = 0;
	CHttpClient httpClient;
	
	string strGetRecentSubMerchant(CAppManager::GetInstance().GetConfig().GetRecentSubMerchantUrl());

	stringstream ssParams;
	ssParams<<"customerId=" << nUserID;
	ssParams<<"&maxNumber=" << nMaxCount;
	
	string strResult;

	int httpStatus = httpClient.Post(strGetRecentSubMerchant, ssParams.str(), strResult);
	if(httpStatus != CURLE_OK)
	{
		return;
	}

	// utf8转gbk
    int nLength = strResult.length()*1.5;
    char *pGbkReturn = new char[nLength];
    memset(pGbkReturn, 0, nLength);
    utf2gbk((char*)strResult.c_str(),strResult.length(), pGbkReturn, nLength);

	//解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("HttpGetRecentSubMerchant",CLog::TYPE_DEBUG,"JSON:%s",strResult.c_str());
    if (reader.parse(pGbkReturn, recv))
    {
        if(recv.isMember("data"))
		{
			Json::Value jvData = recv["data"];
			if(jvData.isMember("recentChatInfoList"))
			{
				Json::Value jvListArray = jvData["recentChatInfoList"];
				for(int i = 0; i < (int)jvListArray.size(); ++i)
				{
					Json::Value jvList = jvListArray[i];
					if(jvList.isMember("id") && jvList.isMember("nickname") 
						&& jvList.isMember("headImgUrl") && jvList.isMember("time")
						&& jvList.isMember("userSign"))
					{
						if(!jvList["id"].isNull())
							pListInfo[nRetCount].id = jvList["id"].asUInt();
						if(!jvList["time"].isNull())	
	        				pListInfo[nRetCount].time = jvList["time"].asUInt();;

						if(jvList["nickname"].isString())
    				    	strncpy(pListInfo[nRetCount].nickname,jvList["nickname"].asCString(),MAX_NICKNAME_LEN);
    				    if(jvList["headImgUrl"].isString())
        					strncpy(pListInfo[nRetCount].headImgUrl,jvList["headImgUrl"].asCString(),MAX_HEADURL_LEN);
        				if(jvList["userSign"].isString())
        					strncpy(pListInfo[nRetCount].usersign,jvList["userSign"].asCString(),MAX_USERSIGN_LEN);
					
						++nRetCount;
					}
				}
			}
		}
	}

	delete[] pGbkReturn;
	pGbkReturn = NULL;
}

#ifdef XMPP_BUILD
// 商家分流
void HttpSellerFlow(uint32 nUserID, int nUserLevel, uint32 nMerchantID, int nGroupID, int nRequestTime, XT_GET_SUB_ACCOUNT_ACK &ack)
{
	CHttpClient httpClient;
	
	string strSellerFlow(CAppManager::GetInstance().GetConfig().GetSellerFlowUrl());

	stringstream ssParams;
	ssParams<<"userLevel=" << nUserLevel;
	ssParams<<"&groupId=" << nGroupID;
	ssParams<<"&merchantId=" << nMerchantID;
	ssParams<<"&customerId=" << nUserID;
	ssParams<<"&requestTime=" << nRequestTime;
	ssParams<<"&fromDevice=" << (int)1;
	ssParams<<"&fromPage=" << (int)-1;
	ssParams<<"&type=" << (int)0;
	
	string strResult;

	int httpStatus = httpClient.Post(strSellerFlow, ssParams.str(), strResult);
	if(httpStatus != CURLE_OK)
	{
		return;
	}

	// utf8转gbk
    int nLength = strResult.length()*1.5;
    char *pGbkReturn = new char[nLength];
    memset(pGbkReturn, 0, nLength);
    utf2gbk((char*)strResult.c_str(),strResult.length(), pGbkReturn, nLength);

    ack.QueueLength = -1;

	//解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("HttpSellerFlow",CLog::TYPE_DEBUG,"JSON:%s",strResult.c_str());
    if (reader.parse(pGbkReturn, recv))
    {
        if(recv.isMember("data"))
		{
			Json::Value jvData = recv["data"];
			if(jvData.isMember("queueResponse"))
			{
				Json::Value jvResponse = jvData["queueResponse"];

				if(jvResponse.isMember("id") && !jvResponse["id"].isNull())
				{
					ack.id = jvResponse["id"].asUInt();
				}
				if(jvResponse.isMember("queueLength") && !jvResponse["queueLength"].isNull())
				{
					ack.QueueLength = jvResponse["queueLength"].asInt();
				}
				if(jvResponse.isMember("username") && jvResponse["username"].isString())
				{
					strncpy(ack.strUsername,jvResponse["username"].asCString(),MAX_USERNAME_LEN);
					strncpy(ack.strNickname,jvResponse["username"].asCString(),MAX_NICKNAME_LEN);
				}
				if(jvResponse.isMember("usersign") && jvResponse["usersign"].isString())
				{
					strncpy(ack.usersign,jvResponse["usersign"].asCString(),MAX_USERSIGN_LEN);
				}
				if(jvResponse.isMember("headImgUrl") && jvResponse["headImgUrl"].isString())
				{
					strncpy(ack.face_url,jvResponse["headImgUrl"].asCString(),MAX_HEADURL_LEN);
				}
				ack.merchant_id = nMerchantID;
			}
		}
	}

	delete[] pGbkReturn;
	pGbkReturn = NULL;
}
#endif
