
#include "imapi.h"
#include "ConfigInfo.h"
#include "json/json.h"
#include <sstream>



imapi::imapi()
{
	m_EventType = 0;
	m_ServiceId = 0;
	m_CustomerId = 0;
	m_GroupId = 0;
	m_requestTime = 0;
	m_userLevel = 0;
	m_toServiceID = 0;
	m_toServiceGroup = 0;
}


imapi::~imapi()
{

}

void imapi::reset()
{
	m_EventType = 0;
	m_ServiceId = 0;
	m_CustomerId = 0;
	m_GroupId = 0;
	m_requestTime = 0;
	m_userLevel = 0;
	m_toServiceID = 0;
	m_toServiceGroup = 0;
}



bool imapi::imapiReq(uint32 EventType,string &data,uint32 ServiceId,uint32 CustomerId,
	uint32 GroupId,uint32 requestTime,uint32 userLevel,uint32 toServiceID,uint32 toServiceGroup)
{
	setReuestParams(EventType,ServiceId,CustomerId,
	GroupId,requestTime,userLevel,toServiceID,toServiceGroup);
	return genHttpReq(data);
}

void imapi::setReuestParams(uint32 EventType,uint32 ServiceId,uint32 CustomerId,
	uint32 GroupId,uint32 requestTime,uint32 userLevel,uint32 toServiceID,uint32 toServiceGroup)
{
	setEventType(EventType);
	setServiceId(ServiceId);
	setCustomerId(CustomerId);
	setGroupId(GroupId);
	setrequestTime(requestTime);
	setuserLevel(userLevel);
	settoServiceID(toServiceID);
	settoServiceGroup(toServiceGroup);
	return;
}

bool imapi::genParams(string &params)
{
	uint32 type = getEventType();
	char EventTypeStr[32] = {0};
	char ServiceIdStr[32] = {0};
	char CustomerIdStr[32] = {0};
	char GroupIdStr[32] = {0};
	char requestTimeStr[32] = {0};
	char userLevelStr[32] = {0};
	char toServiceIDStr[32] = {0};
	char toServiceGroupStr[32] = {0};
	
	sprintf(EventTypeStr,"%u",getEventType());

		//增加事件类型
	params += "&EventType=";
	params += EventTypeStr;

	switch(type)
	{				
		case STOPQUEUE:		
				sprintf(CustomerIdStr,"%u",getCustomerId());
				sprintf(GroupIdStr,"%u",getGroupId());
				//增加用户ID
				params += "&CustomerId=";
				params += CustomerIdStr;
				//增加组ID
				params += "&GroupId=";
				params += GroupIdStr;
				break;
	    case CLIENTCLOSEWINDOW:
		case STOPRECPTION:
				sprintf(ServiceIdStr,"%u",getServiceId());
				sprintf(CustomerIdStr,"%u",getCustomerId());
				sprintf(GroupIdStr,"%u",getGroupId());
				//增加serviceId
				params += "&ServiceId=";
				params += ServiceIdStr;
				//增加用户ID
				params += "&CustomerId=";
				params += CustomerIdStr;
				//增加组ID
				params += "&GroupId=";
				params += GroupIdStr;
				break;
				
		case CSONLINEREQ:
		case CSOFFLINEREQ:
		case CSASKFORCONSULT:
				sprintf(ServiceIdStr,"%u",getServiceId());
				//增加serviceId
				params += "&ServiceId=";
				params += ServiceIdStr;
				break;
  		case CLIENTQUEUEREQ:
				sprintf(CustomerIdStr,"%u",getCustomerId());
				sprintf(GroupIdStr,"%u",getGroupId());
				sprintf(requestTimeStr,"%u",getrequestTime());
				sprintf(userLevelStr,"%u",getuserLevel());	
				//增加用户ID
				params += "&CustomerId=";
				params += CustomerIdStr;
				//增加组ID
				params += "&GroupId=";
				params += GroupIdStr;
				//增加请求时间	
				params += "&requestTime=";
				params += requestTimeStr;
				//增加用户等级
				params += "&userLevel=";
				params += userLevelStr;
				break;		
		case CSTRANSFER :
		case CSCONTROL:
				sprintf(ServiceIdStr,"%u",getServiceId());
				sprintf(CustomerIdStr,"%u",getCustomerId());
				sprintf(GroupIdStr,"%u",getGroupId());
				sprintf(toServiceIDStr,"%u",gettoServiceID());
				sprintf(toServiceGroupStr,"%u",gettoServiceGroup());	
				//增加serviceId
				params += "&ServiceId=";
				params += ServiceIdStr;
				//增加用户ID
				params += "&CustomerId=";
				params += CustomerIdStr;
				//增加组ID
				params += "&GroupId=";
				params += GroupIdStr;
				//转接客服ID
				params += "&toServiceId=";
				params += toServiceIDStr;
				//增加接待组
				params += "&toServiceGroup=";
				params += toServiceGroupStr;
				break;
		case CSINITSERVER:
				break;
		default :
				return false;
	}

	return true;
}



bool imapi::genHttpReq(string &data)
{		
	CHttpClient getRequest;
	string datatmp;
	string sPostParam;
	CConfigInfo config;
	sPostParam += config.GetimapiUrl();
//	sPostParam += "http://storeservice.beta.fn.com/merchantStore/getMerchantStore/";


	if(!genParams(sPostParam))
	{		
		CLog::Log("imapi",CLog::TYPE_ERROR,"生成GET请求失败 type %d",getEventType());
	}
	
	CLog::Log("imapi",CLog::TYPE_IMPORTANT,"生成GET请求为 sPostParam %s",sPostParam.c_str());
	int res = getRequest.Get(sPostParam,datatmp);

	// 0 为成功
    //判断获取是否成功
    if (res != CURLE_OK)
    {        
		CLog::Log("imapi",CLog::TYPE_ERROR,"失败，返回结果为res %d data %s",res,datatmp.c_str());
        
        return false;
    }	
	
    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("imapi",CLog::TYPE_DEBUG,"JSON:%s",datatmp.c_str());
    if (reader.parse(datatmp, recv))
    {

		//{"message":null,"success":true,"errorCode":0,"data":{"data":{"185":0}}}
		
        if(!recv.isMember("message"))
        {
            return false;
        }
        bool success = recv["success"].asBool();
        if(success == false)
        {
            //用户不存在
            CLog::Log("imapi",CLog::TYPE_ERROR,"请求失败");
            return false;
        }
        else
        {   
        	if(recv["data"].isMember("data"))
	        {
	        	if(recv["data"]["data"].isInt())
	        	{
					int datatmp = recv["data"]["data"].asInt();
					stringstream ss;			    
				    ss << datatmp;
				    ss >> data;	
				}
        	}
            //找到了子串
            return true;
        }
    }
    else
    {
        //json串解析失败
        CLog::Log("imapi", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail",getEventType(), datatmp.c_str());
        return false;
    }	
	
}
