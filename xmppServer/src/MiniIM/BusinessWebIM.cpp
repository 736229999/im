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

#include <vector>
#define TIMERPERIOD (1000 * 10 * 1) //时钟周期 10 秒钟
#define MAX_CHECK_NUM 1000  //每次获取条数

extern map<uint32,sess_t> sessManager;


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

	// uuid
	SendPacket << xtMsg.uuid;

	//消息体长度
	sendData.data_len = (uint16)sizeof(XT_HEAD) + SendPacket.GetSize();

	//消息头
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	CAppManager::GetInstance().InitHead(lpSendHead, CMD_SERVERMSG_IND,(uint16)(SendPacket.GetSize()), xtMsg.to_id, XT_HEAD::DEST_TYPE_USER);

	//发送消息
	CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data, sendData.data_len, SEND_RETRY_TIME);

}





/////////////////////CWebIMMsgCheck.cpp//////////////////////
void  MyWebIMTimeProc(void* dwUser)
{
	CWebIMMsgCheck* pObj = (CWebIMMsgCheck*)dwUser;
	pObj->HandleTimer();
}

CWebIMMsgCheck::CWebIMMsgCheck()
{
	ST_WEBIM_MSG m_WebIMMsg[MAX_CHECK_NUM];
	m_pWebIMMsg=m_WebIMMsg;
	
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
	m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD,MyWebIMTimeProc,(void*)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("CWebIMMsgCheck",CLog::TYPE_ERROR,"WEBIM定时器设置失败!");
	}
}

void CWebIMMsgCheck::StopTiemr()
{
	if( m_hMMT )
	{
		::timeKillEvent(m_hMMT);
		m_hMMT = NULL;
	}
}


void CWebIMMsgCheck::HandleTimer()
{
	uint16 nReqCount=MAX_CHECK_NUM;
	try
	{
//		CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "扫描WEBIM消息及离线消息开始!");

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
//			CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "扫描出WEBIM消息(离线消息) %d 条", nReqCount);
		}

	}
	catch (...)
	{
		CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "发生异常!");
	}
//	CLog::Log("CWebIMMsgCheck::HandleTimer()", CLog::TYPE_IMPORTANT, "扫描WEBIM消息及离线消息结束!");

}

//发送WEBIM互通(离线)消息给相应用户
void CWebIMMsgCheck::SendP2PMsg(ST_WEBIM_MSG *pWebIMMsg)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	char* pMsgData = sendData.data + sizeof(XT_HEAD);

	CSendPacket SendPacket;
	SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

	bool xmppFlag = false;
	if(pWebIMMsg->msglen>0)			//离线消息
	{
		if(sessManager.find(pWebIMMsg->recvID) == sessManager.end())
		{
			SendPacket.WritePacket( pWebIMMsg->msg, pWebIMMsg->msglen );
		}
		//对特殊离线消息设置离线标志
		//LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT) pMsgData;
		//if( (lpSrvTransmit->msgtype == CMD_CLUSTERMSG_SEND) ||
		//	(lpSrvTransmit->msgtype == CMD_P2PMSG_SEND))
		//{
		//	lpSrvTransmit->msgtype |= 0x1000;
		//}

		else
		{
		
			xmppFlag = true;			
			XT_MSG xtmsg;
			XT_SERVER_TRANSMIT SrvTransmit;
			SrvTransmit.origin_id=XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			SrvTransmit.msgtype=CMD_P2PMSG_SEND_OFFLINE;
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
			SendPacket << pWebIMMsg->uuid;
		}
		
	}
	else							//WEBIM互通消息
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
		SendPacket.WriteData( pWebIMMsg->from_nickname, strlen(pWebIMMsg->from_nickname)+1 );
		SendPacket.WriteData( pWebIMMsg->uuid, strlen(pWebIMMsg->uuid)+1 );
		SendPacket << xtmsg.uuid;
	}

	sendData.data_len = (uint16)sizeof(XT_HEAD) + SendPacket.GetSize();

	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_SERVERMSG_IND,
		(uint16)(SendPacket.GetSize()),
		pWebIMMsg->recvID);
	
    if(xmppFlag == true)
    {
        // 以xmpp客户端登录
        CLog::Log("CWebIMMsgCheck",CLog::TYPE_IMPORTANT,"send to xmpp offlinemsgs, length:%d", sendData.data_len);
        CAppManager::GetInstance().SendToJabberdIM((BYTE*)sendData.data, sendData.data_len);

        //CancelReply();
    }
	else
	{

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,SEND_RETRY_TIME);
	}
}


static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
}

/*清理图片接受失败的内含图片路径的消息*/
void  MyChatMsgTimeProc(void* dwUser)
{
	CChatMsgSave* pCChatMsgSave = (CChatMsgSave*)dwUser;
	
	/**/
	time_t timeNow;
	time(&timeNow);

	if(pCChatMsgSave == NULL)
		return;
	
	//CLog::Log("MsgPic", CLog::TYPE_DEBUG, "%u", timeNow); 
	vector<XT_TALK>::iterator iter;
	pCChatMsgSave->m_vecLock.Lock();
	
	for( iter = pCChatMsgSave->m_vecImgMsg.begin(); iter!= pCChatMsgSave->m_vecImgMsg.end(); )
	{
		XT_TALK xtMsgTmp = *iter;
		//超过60s   就erase
		
		CLog::Log("MsgPic", CLog::TYPE_DEBUG, "%u %u %d", timeNow, xtMsgTmp.recv_time, timeNow-xtMsgTmp.recv_time); 
		if( timeNow > xtMsgTmp.recv_time +  60 ||  xtMsgTmp.recv_time > timeNow)
		{
			CLog::Log("MsgPic", CLog::TYPE_ERROR, "[%u %u %d ]  [%u %u %u %u %u]  [%u %u %u %u %s] [%s %s ] OutTime!!!!", 
				timeNow, xtMsgTmp.recv_time, timeNow - xtMsgTmp.recv_time,
				xtMsgTmp.from_id, xtMsgTmp.to_id, xtMsgTmp.data_type, xtMsgTmp.ver,xtMsgTmp.send_time,
				xtMsgTmp.recv_flag, xtMsgTmp.fontSize, xtMsgTmp.fontColor, xtMsgTmp.fontStyle, xtMsgTmp.fontName,
				xtMsgTmp.data,xtMsgTmp.from_nickname);
			
			iter = pCChatMsgSave->m_vecImgMsg.erase(iter);
		}
		else
			iter++;

	}
	
	
	pCChatMsgSave->m_vecLock.Unlock();
}

CHttpServer::CHttpServer()
{
	m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	strcpy(m_HttpServerAddr, m_ConfigInfo.GetHttpServerAddr());
}
CHttpServer::~CHttpServer()
{
	SAFE_DELETE(m_pGlobalBusi);

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

			//判断是否在线
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
	if( RESULT_FAILED == m_pGlobalBusi->QueryUserStatus(id, &userStatus[0], cnt))
	{
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR, "QueryUserStatus failed!");
		return RESULT_FAILED;
	}
	//遍历状态数组，写入数据包
	for (uint8 i = 0; i < size; ++i) 
	{
		pUStatus[i].id = id[i];
		//CLog::Log("bwwan",CLog::TYPE_ERROR,"id[i]:%ld,pUStatus[i].id:%ld",id[i],pUStatus[i].id);
		pUStatus[i].status = (userStatus[i].status == XTREAM_OFFLINE ? 0 : 1) ;	
	}

	return RESULT_SUCCESS;			
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
	evhttp_send_reply(req, HTTP_OK, "OK", p_httpServer->evBuf);


}


int CHttpServer::StartWork()
{
	CreateThread( 
		NULL,						// default security attributes 
		0,							// use default stack size  
		HttpServerStart,				// thread function 
		this,						// argument to thread function 
		0,							// use default creation flags 
		NULL);						// returns the thread identifier 

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


CChatMsgSave::CChatMsgSave()
{
	
}
CChatMsgSave::~CChatMsgSave()
{
	
}



//清除线程，以免堵塞至爆
void CChatMsgSave::StartEraseTimer()
{
	m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD,MyChatMsgTimeProc,(void*)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("CChatMsgSave",CLog::TYPE_ERROR,"ChatMsg StartTimer error!!");
	}
}

int CChatMsgSave::StartWork()
{

	//初始化数据库
	CConfigInfo m_Config;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);
	
	if( !m_pDataBaseMini.ConnectDB() )
	{	
		CLog::Log("BusinessProcE",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return RESULT_FAILED;
	}
	

	CreateThread( 
		NULL,						// default security attributes 
		0,							// use default stack size  
		ChatMsgSavePorc,				// thread function 
		this,						// argument to thread function 
		0,							// use default creation flags 
		NULL);						// returns the thread identifier 

	StartEraseTimer();	
	return RESULT_SUCCESS;
}


RECV_FILE_STRUCT* CChatMsgSave::SF_NewRecvFile(uint32 file_id, uint32 sender, const char* file_name, int file_size )
{	
	RECV_FILE_STRUCT *pRecv= new RECV_FILE_STRUCT;
	pRecv->file_id = file_id;
	pRecv->from_id = sender;
	strcpy(pRecv->file_name,file_name);
	pRecv->file_size = file_size;

	char szFilePathFile[128];
	sprintf(szFilePathFile, "./ImageCache/ImageFile/%s", file_name);
	pRecv->fileObject = fopen(szFilePathFile,"a+");
	
	m_FileLock.Lock();
	
	m_vecRecvFiles.push_back(pRecv);
	
	m_FileLock.Unlock();

	return pRecv;

}



/*根据file_id 和sender查找RECV_FILE_STRUCT*/
RECV_FILE_STRUCT* CChatMsgSave::SF_FindRecvFile(uint32 file_id, uint32 sender )
{
	m_FileLock.Lock();
	for ( size_t i=0; i<m_vecRecvFiles.size(); i++ )
	{
		RECV_FILE_STRUCT *pRecv = m_vecRecvFiles[i];

		if ( pRecv->file_id == file_id && pRecv->from_id == sender )
		{
			m_FileLock.Unlock();
			return pRecv;
		}
	}
	m_FileLock.Unlock();

	return NULL;
}

/*图片接受完成后，清除相应RECV_FILE_STRUCT*/
void CChatMsgSave::SF_RemoveRecvFile(uint32 file_id, uint32 sender)
{	
	m_FileLock.Lock();
	for ( size_t i=0; i<m_vecRecvFiles.size(); i++ )
	{
		RECV_FILE_STRUCT *pRecv = m_vecRecvFiles[i];

		if ( pRecv->file_id==file_id && pRecv->from_id==sender )
		{
			if ( pRecv->fileObject )
			{
				fclose(pRecv->fileObject);
				pRecv->fileObject = NULL;
				delete pRecv;
				pRecv = NULL;
			}

			delete pRecv;
			m_vecRecvFiles.erase(m_vecRecvFiles.begin()+i);
			break;
		}
	}
	m_FileLock.Unlock();
}


 
string CChatMsgSave::upLoadPic(const char* szPicName)
{
  CURL *curl;
  CURLcode res;
  string strRet;
 
  string strPicName(szPicName);
  //图片文件保存的全路径
  string strPicPathFile("./ImageCache/ImageFile/");
  strPicPathFile += strPicName;
  
  //bwwan 获取图片文件后缀名
  string extName("jpg");
  string::size_type pos = strPicName.find(".");
  if(pos != std::string::npos)
  {
	  extName = strPicName.substr(pos + 1);
  }
  
  curl = curl_easy_init();
  if(curl) {
    struct curl_httppost *formpost = 0;
    struct curl_httppost *lastptr  = 0;
    curl_formadd(&formpost,  
                 &lastptr,  
                 CURLFORM_COPYNAME, "img",  
                 CURLFORM_FILE, strPicPathFile.c_str(),  
                 CURLFORM_END);  
    curl_formadd(&formpost,
                 &lastptr, 
                 CURLFORM_COPYNAME, "imgName",  
                 CURLFORM_COPYCONTENTS, "MouMouPic",
                 CURLFORM_END);  
    curl_formadd(&formpost,
                 &lastptr, 
                 CURLFORM_COPYNAME, "extName",  
                CURLFORM_COPYCONTENTS,extName.c_str(), 
                 CURLFORM_END);  
	/*
    curl_formadd(&formpost,
                 &lastptr, 
                 CURLFORM_COPYNAME, "uploadType",  
                 CURLFORM_COPYCONTENTS, "3",
                 CURLFORM_END);  
    */
    curl_easy_setopt(curl, CURLOPT_URL,CAppManager::GetInstance().GetConfig().GetImgServerAddr());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRet); 
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
  strRet = strRet.substr(0,128);
  CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "upLoadPic: %s ",strRet.c_str());

  //解析json字符创
  Json::Reader reader;
  Json::Value recv;
  string strUrl;
  if (reader.parse(strRet, recv))
  {
	  //获取result字段
	  bool result = recv["result"].asBool();
	  if(result)
	  {
	  	strUrl = recv["url"].asString();
		
	  }
  }
  //上传容错处理
  if(strUrl.size() > 128)
  	strUrl = "too long";
  
  CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "upLoadPic: %s ",strUrl.c_str());

  return strUrl;
}


void CChatMsgSave::ImgNameReplacebyUrl(const char* strRecvFile, const string strImgUrl)
{
	/**/
	bool bImgMsgFind = false;

	/**/
	string strImgName(strRecvFile);
	m_vecLock.Lock();
	vector<XT_TALK>::iterator iter;
	for( iter = m_vecImgMsg.begin(); iter!= m_vecImgMsg.end(); )
	{
		std::string strMsg(iter->data);
		string::size_type pos(0);
		
		pos = strMsg.find(strImgName);
		//找到了包含该图片的消息体
		if(pos != string::npos)
		{
			strMsg.replace(pos,strImgName.length(),strImgUrl);
			strcpy(iter->data,strMsg.c_str());
			
			CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "UrlReplaced [%u->%u %u] [%s %s ]", 
				iter->from_id, iter->to_id, iter->data_type, iter->data,iter->from_nickname);

			//先通知web端替换

			//通知web端替换上传成功的url
			//if(iter->to_terminal == XT_TALK::WEB_TERMINAL)
			/*
			{
				//消息体为json串
				Json::Value jvImgUrl;
				Json::FastWriter jFastWriter;
				string strJvImgUrl;
				jvImgUrl["imgName"] = strImgName;
				jvImgUrl["imgUrl"] = strImgUrl;
				strJvImgUrl = jFastWriter.write(jvImgUrl);
			
				//回url替换消息给web端
				XT_TALK xtAckMsg;
				xtAckMsg.from_id = iter->from_id;
				xtAckMsg.to_id = iter->to_id;
				xtAckMsg.data_type = XT_MSG::SF_IMG_URL;
				xtAckMsg.ver  = iter->ver;
				xtAckMsg.send_time = iter->send_time + 3; 	//随便设一个+3s
				xtAckMsg.recv_flag = iter->recv_flag;


				xtAckMsg.fontSize = iter->fontSize;
				xtAckMsg.fontColor = iter->fontColor;
				xtAckMsg.fontStyle = iter->fontStyle;
				strcpy(xtAckMsg.fontName, iter->fontName);
				//xtAckMsg.fontName = iter->fontName;
				strcpy(xtAckMsg.from_nickname,iter->from_nickname);
				//xtAckMsg.from_nickname = iter->from_nickname;
				
				CSendPacket spMsg(xtAckMsg.data);
				spMsg << strJvImgUrl.c_str();
				CommonSendP2PMsg(xtAckMsg,spMsg.GetSize());
			}
			*/
			
			
			{
				SendDataStruct sendData;
				LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

				CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,(uint16)(sizeof(XT_SERVER_TRANSMIT) + 512),iter->to_id,XT_HEAD::DEST_TYPE_USER);
				
				LPXT_SERVER_TRANSMIT lpSrvTransmit	= (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
				lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
				lpSrvTransmit->msgtype				= CMD_P2PMSG_SEND;



				char tmpBuf[1440];
				//uint32 nFileId;
				CSendPacket tmpRecvPacket(tmpBuf);
				//char msgData[1440];
		

				Json::Value jvImgUrl;
				Json::FastWriter jFastWriter;
				string strJvImgUrl;
				jvImgUrl["imgName"] = strImgName;
				jvImgUrl["imgUrl"] = strImgUrl;
				strJvImgUrl = jFastWriter.write(jvImgUrl);

				
				tmpRecvPacket<<iter->from_id<<iter->to_id<<(uint8)(XT_MSG::SF_IMG_URL)<<(uint16)0<<iter->send_time+1<<iter->recv_flag;
				tmpRecvPacket<<iter->fontSize<<iter->fontColor<<iter->fontStyle<<iter->fontName;
				tmpRecvPacket.WriteData(strJvImgUrl.c_str(), strJvImgUrl.size());
				tmpRecvPacket<<iter->from_nickname;

				BYTE* pVerifyFriendReq = (BYTE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
				memcpy(pVerifyFriendReq,tmpRecvPacket.GetData(),tmpRecvPacket.GetSize());
				sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + 512);

				CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);
			}
				uint32 logintime = 0;
				CUser::m_UserListLock.Lock();
				CUser *pUser = NULL;//CUser::m_pUserListHead;
			    int m_nID = (iter->from_id>iter->to_id)?iter->from_id:iter->to_id;
				__gnu_cxx::hash_map<uint32,CUser*>::iterator iterr = CUser::m_userhash.find(m_nID);
				if(iterr != CUser::m_userhash.end())
				{
					pUser = iterr->second;
					logintime = pUser->m_nLoginTime;
				}
				CUser::m_UserListLock.Unlock();


			//所有图片都已经上传图片服务器
			if(strMsg.find("<image:{") == string::npos )
			{
				if(m_pDataBaseMini.WebMsgSave(&(*iter),logintime) != DB_SUCCESS)
				{
					CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "WebMsg Save Failed![%u->%u]",iter->from_id, iter->to_id);	
				}
				else
				{
					CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "UrlReplaced [%u->%u %u] [%s %s ] Save Database~!!", 
						iter->from_id, iter->to_id, iter->data_type, iter->data,iter->from_nickname);
				}
				
				iter = m_vecImgMsg.erase(iter);
			}
			bImgMsgFind = true;
			
			break;
		}
		else
		{
			iter++;
		}
	
	}
	m_vecLock.Unlock();
	
	if(!bImgMsgFind)
	{
		CLog::Log("MsgPic", CLog::TYPE_ERROR, "Not find msg!Imgname:%s, ImgUrl:%s ",strImgName.c_str(),strImgUrl.c_str());
	}
}

#ifdef IM_USE_SSDB

/*Image的url存进ssdb*/
void CChatMsgSave::SaveImgUrlSSDB(const char* szKey, string strValue)
{
	string strHashName("IMG");
	string strKey(szKey);

	//建立ssdb连接
	ssdb::Status ssdbStat;
	SSDBLinks& links = SSDBLinks::GetInstance();
	ssdbClient* client = links.getLink();
	if(client == NULL)
	{
		CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb client is null");
		return;
	}

	if(client->l == NULL)
	{
		CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb link is null");
		links.releaseLink(client);
		return;
	}	

	//往ssdb中插入数据
	ssdbStat = links.hSetData(client->l,strHashName, strKey, strValue);
	if(ssdbStat.code() == "error")
	{
		SAFE_DELETE(client->l);
		CLog::Log("CChatMsgSave::SaveImgUrlSSDB",CLog::TYPE_ERROR,"ssdb set data error");
	}	
	links.releaseLink(client);

	
}


/*
ssdb中读取Image的url
*/
void CChatMsgSave::GetImgUrlSSDB(const char* szKey, string& strValue)
{
	string strHashName("IMG");
	string strKey(szKey);

	//建立ssdb连接
	ssdb::Status ssdbStat;
	SSDBLinks& links = SSDBLinks::GetInstance();
	ssdbClient* client = links.getLink();
	if(client == NULL)
	{
		CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb client is null");
		return;
	}
	
	if(client->l == NULL)
	{
		CLog::Log("CBusinessSendMessage::SaveTalkMsg",CLog::TYPE_ERROR,"ssdb link is null");
		links.releaseLink(client);
		return;
	}
	
	//从ssdb中取出数据
	ssdbStat = links.hGetData(client->l,strHashName,strKey,strValue);
	if(ssdbStat.code() == "error")
	{
		SAFE_DELETE(client->l);
		CLog::Log("CChatMsgSave::GetImgUrlSSDB",CLog::TYPE_ERROR,"ssdb get data error");	
	}
	
	links.releaseLink(client);

}
	
#endif

void CChatMsgSave::SaveImgUrlFile(const char* szKey, const string strValue)
{

	//保存url到文件(临时)
	char strUrlFileName[128];
	sprintf(strUrlFileName,"./ImageCache/ImageUrl/%s.url",szKey);
	FILE* fpUrl = fopen(strUrlFileName,"w");
	if(fpUrl != NULL)
	{
		fprintf(fpUrl, "%s",strValue.c_str());
		fclose(fpUrl);
	}

}

void CChatMsgSave::GetImgUrlFile(const char* szKey, string& strValue)
{
	//保存url到文件(临时)
	char strUrlFileName[128];
	sprintf(strUrlFileName,"./ImageCache/ImageUrl/%s.url",szKey);
	FILE* fpUrl = fopen(strUrlFileName,"r");
	
	char szUrl[128];
	if(fpUrl != NULL)
	{
		fscanf(fpUrl, "%s",szUrl);
		fclose(fpUrl);
	}
	strValue = szUrl;

}



void* CChatMsgSave::ChatMsgSavePorc( LPVOID lpParam )
{
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
			IMAGE_FILE_STRUCT stImgFile;

#if 0			
			// 文件请求req   [1]
			if(xtTalkMsg.data_type == XT_MSG::SF_FILE_REQ)
			{
				rp	>>stImgFile.file_session_id
					>>stImgFile.file_size
					>>stImgFile.file_name;
				
				CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "[%u %u %u] [%u %u %s]",
					xtTalkMsg.from_id, xtTalkMsg.to_id, xtTalkMsg.data_type, 
					stImgFile.file_session_id, stImgFile.file_size, stImgFile.file_name);
				
				//图片接受端为web, 模拟客户端回一个ack(2)
				int ret = 0;
				//if(xtTalkMsg.to_terminal == XT_TALK::WEB_TERMINAL )
				if(1)
				{
					XT_TALK xtAckMsg;
					xtAckMsg.from_id = xtTalkMsg.to_id;
					xtAckMsg.to_id = xtTalkMsg.from_id;
					xtAckMsg.data_type = XT_MSG::SF_FILE_ACK;
					xtAckMsg.ver  = xtTalkMsg.ver;
					xtAckMsg.send_time = xtTalkMsg.send_time + 3; 	//随便设一个+3s
					xtAckMsg.recv_flag = xtAckMsg.recv_flag;
					CSendPacket spMsg(xtAckMsg.data);

					//判断是否已经上传了
					string strImgUrl = "";
#ifdef IM_USE_SSDB
					pCChatMsgSave->GetImgUrlSSDB(stImgFile.file_name, strImgUrl);
#else
					pCChatMsgSave->GetImgUrlFile(stImgFile.file_name, strImgUrl);
#endif
					if(strImgUrl.find("http:") != string::npos)
					{
						ret = 2;
						pCChatMsgSave->ImgNameReplacebyUrl(stImgFile.file_name, strImgUrl);
					}
					spMsg << (uint8)ret << stImgFile.file_session_id<< strImgUrl.c_str();
					CommonSendP2PMsg(xtAckMsg,spMsg.GetSize());
				}

				//表情，过滤掉不保存
				if(stImgFile.file_name[0] != '\{')
				{
					continue;
				}

				//重复请求，丢掉
				if(pCChatMsgSave->SF_FindRecvFile(stImgFile.file_session_id, xtTalkMsg.from_id) != NULL)
				{
					continue;
				}

				//图片文件传输结构
				if(ret == 0)/*如果是传给web端，并且已经在本地存在，就不新建*/
				{
					pCChatMsgSave->SF_NewRecvFile(stImgFile.file_session_id, xtTalkMsg.from_id, stImgFile.file_name, stImgFile.file_size);
				}

			}
			// 文件请求ack  [2]
			if(xtTalkMsg.data_type == XT_MSG::SF_FILE_ACK)
			{
				rp	>>stImgFile.ret
					>>stImgFile.file_id;
				CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "[%u %u %u] [%u %u]", 
					xtTalkMsg.from_id, xtTalkMsg.to_id, xtTalkMsg.data_type, 
					stImgFile.ret, stImgFile.file_id);

				//接收方文件已经存在
				if(stImgFile.ret == 2)
				{
					RECV_FILE_STRUCT *pRecv = pCChatMsgSave->SF_FindRecvFile(stImgFile.file_id,xtTalkMsg.to_id);	
					if(pRecv != NULL && pRecv->fileObject != NULL)
					{ 
						fseek(pRecv->fileObject,0L,SEEK_END);
						int nFileSize = ftell(pRecv->fileObject);
						if(nFileSize > 0)
						{
							string strImgUrl;
						#ifdef IM_USE_SSDB
						 	pCChatMsgSave->GetImgUrlSSDB(pRecv->file_name, strImgUrl);
						#else
						 	pCChatMsgSave->GetImgUrlFile(pRecv->file_name, strImgUrl);
						#endif
						
                            if(strImgUrl.find("http:") != string::npos)
                            {	
                            	//url替换imgName
                                pCChatMsgSave->ImgNameReplacebyUrl(pRecv->file_name,strImgUrl);
                            }
                            else
                            {
                                  CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "[2] Pic upload Error [%s]! from_id:%d ",strImgUrl.c_str(),xtTalkMsg.from_id);
                            }
						}
						else
						{
							CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "The IMG size is 0! from_id:%d ",xtTalkMsg.from_id);
						}
					}
					else
					{
						CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "already recv file not find!from_id:%d ",xtTalkMsg.from_id);	
					}
					pCChatMsgSave->SF_RemoveRecvFile(stImgFile.file_id, xtTalkMsg.to_id);
							
				}
				
			}
			
			// 文件包req     [3]
			if(xtTalkMsg.data_type == XT_MSG::SF_PACKET_REQ)
			{
				int dataLen = 0;
				char data[FILE_PACKET_LEN];
				rp>>stImgFile.file_id
					>>stImgFile.last_packet_id
					>>stImgFile.last_file_pointer;
				dataLen = rp.ReadData(data,sizeof(data));
				CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "[%u %u %u] [%u %u %u]", 
					xtTalkMsg.from_id, xtTalkMsg.to_id, xtTalkMsg.data_type, 
					stImgFile.file_id, stImgFile.last_packet_id,stImgFile.last_file_pointer);
			
				RECV_FILE_STRUCT *pRecv = pCChatMsgSave->SF_FindRecvFile(stImgFile.file_id, xtTalkMsg.from_id);
				
				
				if(pRecv != NULL)
				{
					map<int,int>::iterator it =  pRecv->mapPacketRecv.find((int)stImgFile.last_packet_id);
					//编译重复的包重复写和重复计算大小
					if( it == pRecv->mapPacketRecv.end())
					{
						fseek(pRecv->fileObject,stImgFile.last_file_pointer,0);
						fwrite(data,1,dataLen,pRecv->fileObject);
						pRecv->mapPacketRecv.insert(map<int,int>::value_type((int)stImgFile.last_packet_id,
							(int)stImgFile.last_file_pointer));
						pRecv->recv_size += dataLen;
					}
					else
					{
						CLog::Log("ChatMsgSavePorc", CLog::TYPE_IMPORTANT, " repeated~");
					}
					CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "%u %u %u",dataLen,pRecv->recv_size,pRecv->file_size);


					string picUrl ="";
					if(/*dataLen < FILE_PACKET_LEN ||*/ pRecv->recv_size == pRecv->file_size)//传完了哦
					{	
						fseek(pRecv->fileObject, 0L, SEEK_END);//判断文件是否为空
						int nFileSize = ftell(pRecv->fileObject);
						if(nFileSize > 0)
						{
							string strImgUrl = pCChatMsgSave->upLoadPic(pRecv->file_name);
							//判断是否上传失败
							if(strImgUrl.find("http:") == string::npos)
							{
								CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "Pic upload Error [%s]! from_id:%d",pRecv->file_name,xtTalkMsg.from_id);	
							}
						   	else
						    {

							//保存已经上传url
						    #ifdef IM_USE_SSDB
						   		pCChatMsgSave->SaveImgUrlSSDB(pRecv->file_name,strImgUrl);
							#else
								pCChatMsgSave->SaveImgUrlFile(pRecv->file_name,strImgUrl);
							#endif
								CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "Img Recv and Upload OK! %s %s",
									pRecv->file_name,strImgUrl.c_str());
								//url替换imgName
								pCChatMsgSave->ImgNameReplacebyUrl(pRecv->file_name,strImgUrl);	
								picUrl = strImgUrl;
							}
				
							pCChatMsgSave->SF_RemoveRecvFile(stImgFile.file_id, xtTalkMsg.from_id);
						
						}
						else
						{
							CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "The IMG size is 0! from_id:%d",xtTalkMsg.from_id);
						}
				     }
					
					//图片接受端为web, 模拟客户端回一个ack(4)
					//if(xtTalkMsg.to_terminal == XT_TALK::WEB_TERMINAL)
					if (1)
					{
						XT_TALK xtAckMsg;
						xtAckMsg.from_id = xtTalkMsg.to_id;
						xtAckMsg.to_id = xtTalkMsg.from_id;
						xtAckMsg.data_type = XT_MSG::SF_PACKET_ACK;
						xtAckMsg.ver  = xtTalkMsg.ver;
						xtAckMsg.send_time = xtTalkMsg.send_time + 3; //随便设一个+3s
						xtAckMsg.recv_flag = xtAckMsg.recv_flag;
						CSendPacket spMsg(xtAckMsg.data);
						spMsg<<pRecv->file_id<<pRecv->file_size<<pRecv->file_name<<stImgFile.last_packet_id<<stImgFile.last_file_pointer<<picUrl.c_str();
						CommonSendP2PMsg(xtAckMsg,spMsg.GetSize());
						CLog::Log("ChatMsgSavePorc", CLog::TYPE_IMPORTANT, "file_id:%d,file_size:%d,file_name:%s,last_packet_id:%d,last_file_pointer:%d,strImgUrl:%s",
							pRecv->file_id,pRecv->file_size,pRecv->file_name,stImgFile.last_packet_id,stImgFile.last_file_pointer,picUrl.c_str());
						
					}

				}
				else
				{
					CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "pRecv == NULL!");
				}
				

			}
			// 文件包ack     [4]
			if(xtTalkMsg.data_type == XT_MSG::SF_PACKET_ACK)
			{
				rp>>stImgFile.file_id
					>>stImgFile.file_size
					>>stImgFile.file_name
					>>stImgFile.packet_id
					>>stImgFile.file_pointer;
				
				CLog::Log("MsgPic", CLog::TYPE_IMPORTANT, "[%u %u %u] [%u %u %s %u %u]", 
					xtTalkMsg.from_id, xtTalkMsg.to_id, xtTalkMsg.data_type, 
					stImgFile.file_id, stImgFile.file_size, stImgFile.file_name, stImgFile.packet_id, stImgFile.file_pointer);		
			}
#endif

			if(xtTalkMsg.data_type == XT_MSG::IM_MSG|| xtTalkMsg.data_type == XT_MSG::AUTO_REPLY)
			{
				CLog::Log("MsgPic", CLog::TYPE_DEBUG, "IM_MSG:: [%u] [%u->%u %u] [%s %s ]", 
					(uint32)xtTalkMsg.recv_time,
					xtTalkMsg.from_id, xtTalkMsg.to_id, xtTalkMsg.data_type,
					xtTalkMsg.data,xtTalkMsg.from_nickname);
				
				std::string strMsg(xtTalkMsg.data);
				if(strMsg.find("<image:{") != string::npos)
				{
					CLog::Log("MsgPic", CLog::TYPE_DEBUG, "Msg Exist Image");					
					pCChatMsgSave->m_vecImgMsg.push_back(xtTalkMsg);
				}
				else
				{	
					#ifndef IM_USE_SSDB

				
					uint32 logintime =0;
					CUser::m_UserListLock.Lock();
					CUser *pUser = NULL;//CUser::m_pUserListHead;
					int m_nID = (xtTalkMsg.from_id>xtTalkMsg.to_id)?xtTalkMsg.from_id:xtTalkMsg.to_id;
					__gnu_cxx::hash_map<uint32,CUser*>::iterator iter = CUser::m_userhash.find(m_nID);
					if(iter != CUser::m_userhash.end())
					{
						pUser = iter->second;
					    logintime = pUser->m_nLoginTime;
					}
					CUser::m_UserListLock.Unlock();
					if(pCChatMsgSave->m_pDataBaseMini.WebMsgSave(&xtTalkMsg,logintime) != DB_SUCCESS)
					{
						
						CLog::Log("ChatMsgSavePorc", CLog::TYPE_ERROR, "WebMsg Save Failed! from_id:%d",xtTalkMsg.from_id);	
					}
					
					#endif
				}

			}

		}
	}


	return RESULT_SUCCESS;
}


