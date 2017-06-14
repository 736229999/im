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

	
Event_type     0���ͷ��رմ��ڣ�ֹͣ�Ӵ���SERVICE_ID��group_id���
			  1��web�û�ֹͣ�Ŷӣ����û������Ŷӣ�customer_id��group_Id���
			  2��web�û�ֹͣ�Ӵ������û����ڽӴ���customer_id,service_id,group_Id���
			  3���ͷ�����,service_id���
			  4 : �ͷ����ߣ�service_id���
			  5: �ͻ������ɹ���CustomerId��GroupId��������Ӧ����java�������Լ������Լ��ģ�
			  6. �ͻ������Ŷӣ�CustomerId��GroupId���
			  7. ������Լ��ServiceId��group_id���
			  8. �ͷ�ת�ӣ�ServiceId��customerId,group_id���ServiceId��group_id��Ҫת�ӵ��Ŀͷ���ID����ID
			  9. ���ֻỰ��ServiceId��customerId,group_id���ServiceId��group_id��Ҫת�ӵ��Ŀͷ���ID����ID
			 10. ���ڳ�Ա�����仯����Ӫ��̨�������ڵĳ�Ա��Ҫ֪ͨ��javaserver��group_id�Ǳ����ֶΡ�

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

