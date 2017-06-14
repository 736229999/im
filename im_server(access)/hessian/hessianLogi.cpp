
#include "hessianLogi.h"
#include "../include/json/json.h"


using namespace LogInterface;
using namespace std;


hessianLogi::hessianLogi()
{
	LoadHessianUrl();	
	memset(m_url,0,sizeof(m_url));
	strncpy(m_url,hessian_url[0].c_str(),hessian_url[0].length());
	m_connection = new hessian::HttpConnection(m_url);
	m_ServiceProxy = new UploadSubMerchantActionServiceProxy(*m_connection);
}

hessianLogi::hessianLogi(uint32 merchant_id,uint32 sub_merchant_id)
{
	if(sub_merchant_id == 0)
		sub_merchant_id = merchant_id;
	LoadHessianUrl();
	memset(m_url,0,sizeof(m_url));
	if(hessian_url.size() > 1 )
	{
		strcpy(m_url,hessian_url[sub_merchant_id % 2].c_str());	
	}
	else if(hessian_url.size() == 1)
	{		
		strcpy(m_url,hessian_url[0].c_str());	
	}
	else
	{		
		CLog::Log("hessianLogi",CLog::TYPE_DEBUG,"Hessian LOAD FAIL");
	}
	CLog::Log("hessianLogi",CLog::TYPE_DEBUG,"Hessian connect: merchant_id %d url:%s",sub_merchant_id,m_url);
	m_connection = new hessian::HttpConnection(m_url);
	m_ServiceProxy = new UploadSubMerchantActionServiceProxy(*m_connection);
}


hessianLogi::~hessianLogi()
{
	delete m_connection;
	delete m_ServiceProxy;
}

void hessianLogi::LoadHessianUrl()
{
	Json::Reader jReader(Json::Features::strictMode());
	Json::Value jValue;
	char *szHessianConfig = m_ConfigInfo.GetHessianUrl();
	hessian_url.clear();
	if(szHessianConfig == NULL)
	{
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"GetHessianUrl fail\r\n");
		return;
	}
	if(szHessianConfig[0] == '{')
	{
		if(jReader.parse(szHessianConfig, jValue))
		{
			int total = jValue["total"].asInt();
			for(int count = 1; count <= total ; count++)
			{
				char var1[32] = {0};
				sprintf(var1,"url%d",count);
				string hessian_url_one = jValue[var1].asString();
				if(hessian_url_one.length() == 0)
					break;
				hessian_url.push_back(hessian_url_one);				
				CLog::Log("LogConfig",CLog::TYPE_DEBUG,"Hessian config: url%d --- %s",count,hessian_url_one.c_str());
			}
		}
			

	}

}
UploadSubMerchantActionServiceProxy *hessianLogi::GetHessiani()
{
	return m_ServiceProxy;
}

