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

#define AD_HOSTNAME "fn.com"	// AD域服务地址
#define AD_PORT 389				// AD域服务端口


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
	uint32     HandleReq(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint32 fd);
	int LoginSucess(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint32 fd);
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

	//检验买家账号密码
	bool CheckBuyer(const char* user, const char* password, char* guid,  char * szUserName);
	bool GetBuyerInfo(const char* user, char * szmobileno, char * szemail, char* szNickname, char*szHeadImgUrl);

	//检验卖家账号密码，并读取对应用户id和商家id
	bool CheckSeller(char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, uint32_t& state);
 	bool CheckSellerMysql(const char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, uint32_t& state);
	const char* TransPwd(const char* pwd);
	bool AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo);

private:
	uint32 m_uid;
	unsigned char m_digest[16]; //MD5(MD5(Password & "asdfds!@#$1234@!#$ASDFasdfQ@#4345asDF"))
	XT_LOGIN_ACK m_ack;

	CMiniAppNetwork *	m_pAppNetwork;	//网络发送层对象指针

	MD5_CTX_IM m_md5;
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
	
	void        Logout(CUser * pUser);
	void        WebLogout(CUser * pUser);
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
private:
	
	CConfigInfo 			m_ConfigInfo;
	CMiniAppNetwork *	m_pAppNetwork;	//网络发送层对象指针
};

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

	MD5_CTX_IM m_md5;
	CDataBaseWeb * m_pDataBaseWeb;

	char		m_szError[MAX_ERROR_BUF];

};

