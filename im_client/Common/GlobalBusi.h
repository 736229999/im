/************************************************************************ 
 文件描述：全局客户端业务类,对于每一个调用线程建立一个类对象
 
 创建人： Zouyf, 2006-7-7 
 
 修改记录： 
            
************************************************************************/
#pragma once
#include "BaseUdpTrans.h"
#include "GlobalBusiControl.h"
class CUser;

class CGlobalBusi
{
public:
	CGlobalBusi(GlobalBusiControl * pBusiControl);
	~CGlobalBusi(void);
	//在线用户全局查询
	int SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum);

	//查询在线用户控制信息
	int QueryUserControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count);
	//查询用户状态
	int QueryUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count);
	//修改用户状态
	int ModiUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow = NULL);
	//查询在线用户全局资料
	int QueryUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo);
	//向全局添加/修改用户资料
	int AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID = NULL,uint8 action = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD);
	int AddModiUserInfo(CUser* pUser,uint8 action = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD);
	//删除用户全局资料
	int DelUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID = NULL);

	/************ 家族业务*******************/
	//查询全局家族资料
	int FindFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
	//添加全局家族资料
	int AddFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID = NULL,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
	//删除全局家族资料
	int DelFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID = NULL,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
private:
	void SetEvent(const RecvDataStruct * pRecvData)
	{
		memcpy(m_pRecvData,pRecvData,sizeof(RecvDataStruct) - MAX_NETWORK_PACKET + pRecvData->data_len);
		::SetEvent(m_hEvent);
	}
private:
	friend class GlobalBusiControl;
	uint32 m_vLink;
	HANDLE m_hEvent;

	GlobalBusiControl * m_pBusiControl;
	RecvDataStruct * m_pRecvData;
	SendDataStruct m_sendData;
};
