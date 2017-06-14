/************************************************************************ 
 �ļ���������ʱɨ�账��WEBIM��Ϣ(��������Ϣ)
 
 �����ˣ� Newton.Chao 2011/08/22
 
 �޸ļ�¼�� 
            
************************************************************************/
#pragma once
#include "DataBaseLib.h"
#include "ClientProtocol.h"
#include "SyncCritical.h"
#ifdef IM_USE_SSDB //bwwan
#include "../../ssdb/SSDBConn.h"
#endif
#include "GlobalBusi.h"


void CommonSendP2PMsg(XT_TALK xtMsg,uint32 msgDataLen);

class CWebIMMsgCheck
{
public:
	CWebIMMsgCheck();
	virtual ~CWebIMMsgCheck(void);
	int  StartWork();
	void StopWork();
private:
	friend void  MyWebIMTimeProc(void* dwUser);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
	void SendP2PMsg(ST_WEBIM_MSG *pWebIMMsg);

private:
	CDataBaseMini m_pDataBaseMini;
	ST_WEBIM_MSG *m_pWebIMMsg;
	PMMRESULT m_hMMT;
};


/*2���̣߳�һ���洢��һ�����������Ϣ*/
class CChatMsgSave
{
public:
	CChatMsgSave();
	virtual ~CChatMsgSave(void);

	
	int StartWork();
	void StartEraseTimer();
	string upLoadPic(const char* szPicName);	
	
	void SF_RemoveRecvFile(uint32 file_id, uint32 sender);
	RECV_FILE_STRUCT* SF_FindRecvFile(uint32 file_id, uint32 sender );
	RECV_FILE_STRUCT* SF_NewRecvFile(uint32 file_id, uint32 sender, const char* file_name, int file_size );

#ifdef IM_USE_SSDB
	void SaveImgUrlSSDB(const char* szKey, string strValue);
	void GetImgUrlSSDB(const char* szKey, string& strValue);
#endif
	
	void SaveImgUrlFile(const char* szKey, const string strValue);
	void GetImgUrlFile(const char* szKey, string& strValue);
	
	void ImgNameReplacebyUrl(const char* strRecvFile, const string strImgUrl);
	static void* ChatMsgSavePorc( LPVOID lpParam );
	
	vector<XT_TALK> m_vecImgMsg;	/*��ͼƬ��������Ϣ*/
	CSyncCritical	m_vecLock;
	CSyncCritical   m_FileLock;
	
private:
	vector<RECV_FILE_STRUCT*>m_vecRecvFiles;/*�յ���ͼƬ�ṹ��*/
	CDataBaseMini m_pDataBaseMini;
	PMMRESULT m_hMMT;
};


class CHttpServer
{
public:
	CHttpServer();
	virtual ~CHttpServer(void);

	int StartWork();
	static void generic_handler(struct evhttp_request *req, void *arg);
	static void* HttpServerStart(LPVOID lpParam);
	int GetMerchantStatus(uint8 &cnt,uint32 *id,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus);
	int GetUserStatus(uint8 &cnt,uint32 *id,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus);
	
	void HttpServiceGetMerchantStatus(const char *params, Json::Value& jvReply);
	void HttpServiceGetUserStatus(const char *params, Json::Value& jvReply);
private:
	CGlobalBusi *m_pGlobalBusi;
	CConfigInfo m_ConfigInfo;
	char m_HttpServerAddr[20];
	struct evbuffer *evBuf;
	Json::Reader jReader;

};



