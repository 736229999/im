#ifndef IMAPI_H
#define IMAPI_H

#include "stdafx.h"
#include <time.h>
#include <iconv.h>
#include <set>
#include <curl/curl.h>
#include "httpclient.h"



class imapi
{
	public:
/*

	
Event_type     0：客服关闭窗口，停止接待，SERVICE_ID和group_id必填。
			  1：web用户停止排队，该用户正在排队，customer_id和group_Id必填。
			  2：web用户停止接待，该用户正在接待，customer_id,service_id,group_Id必填。
			  3：客服上线,service_id必填。
			  4 : 客服下线，service_id必填。
			  5: 客户分流成功，CustomerId和GroupId必填。（这个应该是java服务器自己发给自己的）
			  6. 客户申请排队，CustomerId和GroupId必填。
			  7. 主动邀约，ServiceId和group_id必填。
			  8. 客服转接，ServiceId和customerId,group_id必填，ServiceId和group_id是要转接到的客服的ID和组ID
			  9. 接手会话，ServiceId和customerId,group_id必填，ServiceId和group_id是要转接到的客服的ID和组ID
			 10. 组内成员发生变化，运营后台调整组内的成员需要通知到javaserver，group_id是必填字段。

*/		
		imapi();
		~imapi();		
		void reset();
		bool imapiReq(uint32 EventType,string &data,
		uint32 ServiceId = 0,
		uint32 CustomerId = 0,
		uint32 GroupId = 0,
		uint32 requestTime = 0,
		uint32 userLevel = 0,
		uint32 toServiceID = 0,
		uint32 toServiceGroup = 0);
		enum{
				CLIENTCLOSEWINDOW = 0,
				STOPQUEUE = 1,
				STOPRECPTION = 2,
				CSONLINEREQ = 3,
				CSOFFLINEREQ = 4,
				FLOWSUCCESS = 5,
				CLIENTQUEUEREQ = 6,
				CSASKFORCONSULT = 7,
				CSTRANSFER = 8,
				CSCONTROL = 9 ,
				CSINITSERVER = 11
			};
		
	private:
		
		void setEventType(uint32 EventType){m_EventType = EventType;};
		uint32 getEventType() const {return m_EventType;};
		
		void setServiceId(uint32 ServiceId){m_ServiceId = ServiceId;};
		uint32 getServiceId() const {return m_ServiceId;};

		void setCustomerId(uint32 CustomerId){m_CustomerId = CustomerId;};
		uint32 getCustomerId() const {return m_CustomerId;};

		void setGroupId(uint32 GroupId){m_GroupId = GroupId;};
		uint32 getGroupId() const {return m_GroupId;};

		void setrequestTime(uint32 requestTime){m_requestTime = requestTime;};
		uint32 getrequestTime() const {return m_requestTime;};
		
		void setuserLevel(uint32 userLevel){m_userLevel = userLevel;};
		uint32 getuserLevel() const {return m_userLevel;};
		
		void settoServiceID(uint32 toServiceID){m_toServiceID = toServiceID;};
		uint32 gettoServiceID() const {return m_toServiceID;};
		
		void settoServiceGroup(uint32 toServiceGroup){m_toServiceGroup = toServiceGroup;};
		uint32 gettoServiceGroup() const {return m_toServiceGroup;};
		
		void setReuestParams(uint32 EventType,
		uint32 ServiceId = 0,
		uint32 CustomerId = 0,
		uint32 GroupId = 0,
		uint32 requestTime = 0,
		uint32 userLevel = 0,
		uint32 toServiceID = 0,
		uint32 toServiceGroup = 0); 		
		
		bool genHttpReq(string &data);

		
		bool genParams(string &params);
		

			
		uint32 m_EventType;
		uint32 m_ServiceId;
		uint32 m_CustomerId;
		uint32 m_GroupId;
		uint32 m_requestTime;
		uint32 m_userLevel;
		uint32 m_toServiceID;
		uint32 m_toServiceGroup;
		
};

#endif

