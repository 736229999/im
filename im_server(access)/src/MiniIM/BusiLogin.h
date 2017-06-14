/************************************************************************ 
文件描述：处理登陆业务

创建人：zouyf
时间：	2006-6-22
************************************************************************/

#pragma once
#include "Business.h"
#include "BusinessExIMSrv.h"
#include "md5.h"
#include "PublicType.h"
#include "SysStatic.h"
#include <ldap.h>
#ifndef IM_USE_IMAPI
#include "ClientService.h"
#endif
#define AD_HOSTNAME "fn.com"	// AD域服务地址
#define AD_PORT 389				// AD域服务端口

#ifdef ACCESS_SERVER
class CMiniAppNetwork;
class CDataBaseWeb;

class CBusiLogin : public CBusiness
{
public:
	CBusiLogin(CRouterBusiness* pRouter);
	virtual ~CBusiLogin(void);

	virtual int DoWork();
	virtual int SendReply();
	
private:
//	uint32     HandleReq(const LPXT_LOGIN_REQ lpreq);
#ifdef IM_USE_IMAPI
	void ReLoginLogout(CUser * pUser,uint8 type = 0);
#else
	void ReLoginLogout(CUser * pUser,ClientService * clientService,uint8 type = 0);

#endif
	uint32     HandleReq(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead);
	int LoginSucess(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint8 level = 0);
	int        TransitReLoginMsg(LPONLINE_USER_INFO lpPreOnlineInfo);
	void       InitHead(uint8 flag,LPXT_HEAD lpHead,uint16 cmd,uint32 seq,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_USER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = flag;
		lpHead->from = XT_HEAD::FROM_SERVER;
		lpHead->len = len;
		lpHead->seq = seq;
		lpHead->sid = 0;
	}
	void MD5Password(char* pPassword);

	bool SaveBuyer(char* szUserName, char* cPass, char* szGuid, uint32& userId, char *szMobileNo, char *szEmail, char *szNickName, char* szHeadImgUrl);
	bool SaveSeller(char * account,char* pwd,uint32 id,char* truename);


	//检验买家账号密码
	bool CheckBuyer(const char* user, const char* password, char* guid,  char * szUserName);
	bool GetBuyerInfo(const char* guid, char * szmobileno, char * szemail, char* szNickname, char*szHeadImgUrl);
	//bool GetBuyerInfo(const char* user, char * szmobileno, char * szemail, char* szNickname, char*szHeadImgUrl);
	bool CheckBuyerLevel(char* guid,  uint8 &level);
	bool CheckWork(const char* user, const char* password,int &code,
		string &szUserName,string &szEmail,string &szMobileNumber,string &structure);

	//检验卖家账号密码，并读取对应用户id和商家id
	bool CheckSeller(char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, int &state, string &code);
 	bool CheckSellerMysql(const char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, uint32_t& state);
	string TransPwd(const char* pwd);
	bool AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo);

	// 检查用户是否同地登录		返回值 1 通地登录 0 异地登录  -1 调用失败，调用失败时，会返回错误信息
	int IsSameAreaLogin(const char* loginname, std::string &strError);
	// 检查用户是否绑定手机		返回值 1 绑定了手机  0 未绑定手机  -1 调用失败
	int IsBindMobile(const char* loginname, std::string &strError);

private:
	uint32 m_uid;
	unsigned char m_digest[16]; //MD5(MD5(Password & "asdfds!@#$1234@!#$ASDFasdfQ@#4345asDF"))
	XT_LOGIN_ACK m_ack;

	CMiniAppNetwork *	m_pAppNetwork;	//网络发送层对象指针

	MD5_CTX m_md5;
	CDataBaseWeb * m_pDataBaseWeb;

	char		m_szError[MAX_ERROR_BUF];

	bool		m_IsOverLoad;		//是否过载

};

//BusiLogout.h
class CBusiLogout :public CBusinessExIMSrv
{
public:
	CBusiLogout(CRouterBusiness* pRouter);

	virtual ~CBusiLogout(void);

	virtual int DoWork();
	virtual int SendReply();
	
	void 		SendWebQueueInfoIND(CSendPacket& m_SendPacket);
	void 		ClearCSContext(uint32 selfID,uint32 teamid,uint32 toid);
	void 		WebLogout(CUser * pUser,uint8 type = 0);	
#ifdef IM_USE_IMAPI
	void Logout(CUser * pUser,uint8 type);
#else
	void Logout(CUser * pUser,ClientService * clientService,uint8 type = 0);
#endif
private:
	CMiniAppNetwork *	m_pAppNetwork;	//网络发送层对象指针
	SendDataStruct*	pData;
};
#endif
/*
class CBusiRegister : public CBusiness
{
public:
	CBusiRegister(CRouterBusiness* pRouter);
	virtual ~CBusiRegister(void);

	virtual int DoWork();
	virtual int SendReply();
	char *get_md5_password(char* password);

private:
	void       InitHead(uint8 flag,LPXT_HEAD lpHead,uint16 cmd,uint32 seq,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = flag;
		lpHead->from = XT_HEAD::FROM_SERVER;
		lpHead->len = len;
		lpHead->seq = seq;
		lpHead->sid = 0;
	}	
private:
	uint32 m_uid;
	//unsigned char m_digest[16]; //MD5(MD5(Password & "asdfds!@#$1234@!#$ASDFasdfQ@#4345asDF"))
	//XT_REGIST_ACK m_ack;

	CMiniAppNetwork *	m_pAppNetwork;	//网络发送层对象指针

	MD5_CTX m_md5;
	CDataBaseWeb * m_pDataBaseWeb;

	char		m_szError[MAX_ERROR_BUF];

};
*/

