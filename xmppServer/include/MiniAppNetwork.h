/************************************************************************ 
 文件描述：应用网络层
     对业务服务器的集群实现请求包的透明路由，管理维护服务器的各通讯链路
 
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

// 下列 ifdef 块是创建使从 DLL 导出更简单的
//宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MINIAPPNETWORK_EXPORTS
// 符号编译的。在使用此 DLL 的
//任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将 
// MINIAPPNETWORK_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once

#ifdef MINIAPPNETWORK_EXPORTS
	#define MINIAPPNETWORK_API 
#else
	#define MINIAPPNETWORK_API 
#endif

#include "PublicType.h"
#include "MiniNetTrans.h"

typedef struct tagONLINE_USER_INFO
{
	uint32 uid;
	uint32 uip;
	uint16 uport;
	char sessionKey[16];
	char p2pSessionKey[16];
	uint32 srvip;
	uint8  isDirect;
	uint32 joinedTime;  //已加入时间
	uint32 lanip;
	uint16 lanport;
	uint8 flag;//判断来自PC端、WEB端或移动端
	tagONLINE_USER_INFO()
	{
		memset(this,0,sizeof(tagONLINE_USER_INFO));
	}
}ONLINE_USER_INFO, * LPONLINE_USER_INFO;

typedef struct tagONLINE_FAMILY_INFO
{
	uint32 fid;
	uint8  ftype; //PERM_FAMILY TEMP_FAMILY
	uint32 srvip;
	uint8  isDirect;
	uint32 joinedTime;
	tagONLINE_FAMILY_INFO()
	{
		memset(this,0,sizeof(tagONLINE_FAMILY_INFO));
	}
}ONLINE_FAMILY_INFO, * LPONLINE_FAMILY_INFO;

//应用层回调函数
typedef void (*pfnAppNetCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);

// 此类是从 MiniAppNetwork.dll 导出的
class MINIAPPNETWORK_API CMiniAppNetwork
{
public:
	CMiniAppNetwork(void);
	~CMiniAppNetwork(void);

	/*
	功能描述: 设置用户回调参数和回调函数
	必要参数说明：pUserKey：用户回调参数
				  pfnCallback：用户回调函数
	返 回 值：
	*/ 
	void SetUserCallback(void * pUserKey,pfnAppNetCallback pfnCallback);

	/*
	功能描述: 启动应用网络层服务
	必要参数说明：addr：IP 地址
				  prot：udp监听端口
	返 回 值：0 成功，< 0 失败
	*/ 
	int StartWork(const char * addr,uint16 port = 0);

	/*
	功能描述: 查找用户在线信息
	必要参数说明：uid：用户ID
	返 回 值： 用户在线信息
	*/ 
	LPONLINE_USER_INFO FindOnlineUser(uint32 uid);

	/*
	功能描述: 用户登陆
	必要参数说明：uid：用户ID
	返 回 值： 0 正常登陆， 1 本机重登陆 ，2 异地重登录
	*/ 
	int UserLogin(uint32 uid,uint32 uip,uint16 uport,const char * pSessionKey = NULL,const char * pP2PSessionKey = NULL,uint32 lanip = 0,uint16 lanport = 0,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	void UserLogout(uint32 uid);

	/*
	功能描述: 家族登陆
	必要参数说明：fid：家族ID
	返 回 值：
	*/ 
	void FamilyLogin(uint32 fid,uint8 ftype = PERM_FAMILY);
	void FamilyLogout(uint32 fid,uint8 ftype = PERM_FAMILY);

	/*
	功能描述: 通过应用网络层发送数据，自动加密
	必要参数说明：uid ：目标用户
				  data：发送的数据
				  size：数据长度
				  retrys：重发次数
				  interval：重发间隔
	返 回 值：0 成功，< 0 失败
	*/ 
	int SendDataTo(const char * data,int size,int retrys = 0,uint16 interval = 0,uint32 ip = 0,uint16 port = 0,uint32 vlink = 0);

	//直接通过传输层发送数据
	int SendDataTo(SendDataStruct & sendData);

	void StopWork();

	uint16 GetPort();
	uint32 GetLocalIP();

	CUdpNetTrans*  GetSrvNetTrans();
	CUdpNetTrans*  GetCntNetTrans();

	void AddImServer(uint32 srvid,uint32 srvip);
	void DelImServer(uint32 srvid);

	//检查用户类型
	int UserType(uint32 id);
	
private:
	void * m_pAppNetwork;
};
