#include "UserManager.h"
#include "Log.h"
#include "AppManager.h"
#include "ConfigInfo.h"
#include "SyncCritical.h"
#include "GlobalBusi.h"
#include "httpclient.h"
#include <string>
#include <curl/curl.h>
#include "Function.h"


using namespace std;

CUserManager::CUserManager(CGlobalBusi *pGlobalBusi)
{
	m_pGlobalBusi = pGlobalBusi;
}

CUserManager::~CUserManager()
{

}


// 查找在线用户
// 直接从redis中查找
shared_ptr<CUser> CUserManager::Find(uint32 userid, BOOL bGetFriendList /*= FALSE*/)
{
	if(m_pGlobalBusi == NULL)
	{
		return NULL;
	}

	shared_ptr<CUser> pUser = shared_ptr<CUser>(new CUser);
	// redis中查找
	CUser redisUser;
	int rlt = m_pGlobalBusi->GetFUserInfo(userid, redisUser);

	if(rlt == MEMCACHED_GET_INFO_SUCCESS)
	{	// redis中有用户信息	
		*pUser = redisUser;
		// 获取好友列表
		if(bGetFriendList)
		{
			if(!GetFriendList(redisUser.m_nID, redisUser.m_merchantid, pUser->m_FriendList))
			{
				CLog::Log("CUserManager::Find", CLog::TYPE_ERROR, "获取好友列表失败");
			}
			else
			{
				CLog::Log("CUserManager::Find", CLog::TYPE_DEBUG, "获取好友列表成功");
			}
		}
		return pUser;
	}
	else
	{	// redis中没有用户信息
		return NULL;
	}
}

// 用户信息回写入redis
bool CUserManager::AddUserToRedis(const shared_ptr<CUser> &pUser)
{
	if(m_pGlobalBusi == NULL)
	{
		return false;
	}

	return m_pGlobalBusi->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0);
}

// 从redis中删除用户
bool CUserManager::Del(uint32 userid)
{
	if(m_pGlobalBusi == NULL)
	{
		return false;
	}

	uint8 count = 1;
	return m_pGlobalBusi->DelUserInfo(&userid,count);
}

// 创建一个新用户内存
shared_ptr<CUser> CUserManager::CreateNew()
{
	shared_ptr<CUser> pUser = shared_ptr<CUser>(new CUser);
	return pUser;
}

// 获取好友列表
bool CUserManager::GetFriendList(int id, int merchantId, CSortArray<FriendInfo> &friendlist)
{
	friendlist.Clear();

	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strfriendlisturl(CAppManager::GetInstance().GetConfig().GetFriendListUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << id;
	ssParams<<"&merchantId=" << merchantId;
	
	string strFriendlist;
	int httpStaus = httpClient.Post(strfriendlisturl, ssParams.str(), strFriendlist);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strFriendlist, jvRecvResponse)){
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true"){
				if(jvRecvResponse.isMember("data")) {
					if(jvRecvResponse["data"].isMember("friendItemList")){
						Json::Value jvFriendList = jvRecvResponse["data"]["friendItemList"];
						for(int i = 0; i < (int)jvFriendList.size(); ++i)
						{
							Json::Value jvFriendItem = jvFriendList[i];
							if(!jvFriendItem.isMember("friendId"))
							{
								continue;
							}
							
							int nId = jvFriendItem["friendId"].asInt();
							FriendInfo *pFriendInfo = friendlist.Add(nId);
							if(pFriendInfo == NULL)
							{
								continue;
							}
							pFriendInfo->id = nId;
							if(jvFriendItem.isMember("status"))
							{
								pFriendInfo->status = jvFriendItem["status"].asInt();
							}
							else
							{
								pFriendInfo->status = 19;
							}
							if(jvFriendItem.isMember("groupId"))
							{
								pFriendInfo->GroupID = jvFriendItem["groupId"].asInt();
							}
						}
						
						return true;
					}
				}
			}
	    }
	}

	return false;
}

// 获取好友分组信息(暂时只获取客服分组信息)
bool CUserManager::GetGroupList(uint32 uid, vector<XT_GROUPINFO_GET_ACK::tagGroupInfo> &vecGroupInfo)
{
	vecGroupInfo.clear();

	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string strgrouplisturl(CAppManager::GetInstance().GetConfig().GetGroupListUrl());

	stringstream ssParams;
	ssParams<<"serviceId=" << uid;
	
	string strGrouplist;
	int httpStaus = httpClient.Post(strgrouplisturl, ssParams.str(), strGrouplist);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(strGrouplist, jvRecvResponse)){
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true"){
				if(jvRecvResponse.isMember("data")) {
					if(jvRecvResponse["data"].isMember("organNameInfos")){
						Json::Value jvGroupList = jvRecvResponse["data"]["organNameInfos"];
						XT_GROUPINFO_GET_ACK::tagGroupInfo groupitem;
						for(int i = 0; i < (int)jvGroupList.size(); ++i)
						{
							Json::Value jvGroupItem = jvGroupList[i];
							if(!jvGroupItem.isMember("id"))
							{
								continue;
							}
							groupitem.nGroupID = jvGroupItem["id"].asInt();

							if(!jvGroupItem.isMember("parentId"))
							{
								continue;
							}
							groupitem.nGroupPid = jvGroupItem["parentId"].asInt();

							if(!jvGroupItem.isMember("orgnName"))
							{
								continue;
							}
							string strGroupName = jvGroupItem["orgnName"].asString();
							char szGroupName[1024] = {0};
    						utf2gbk((char*)strGroupName.c_str(),strGroupName.length(), szGroupName, 1023);
    						strGroupName = szGroupName;
							string::size_type pos = strGroupName.find_last_of('\\');
							if(pos == string::npos || pos+1 == strGroupName.length())
							{
								continue;
							}
							memset(groupitem.szGroupName, 0, sizeof(groupitem.szGroupName));
							string strRealGroupName = strGroupName.substr(pos+1, strGroupName.length()-1-pos);
							strncpy(groupitem.szGroupName, strRealGroupName.c_str(), strRealGroupName.length() > MAX_GROUPNAME_LEN ? MAX_GROUPNAME_LEN : strRealGroupName.length());

							vecGroupInfo.push_back(groupitem);
						}
						
						return true;
					}
				}
			}
	    }
	}

	return false;
}