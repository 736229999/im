#pragma once

#include "StdPublic.h"
#include "SyncCritical.h"
#include "DataBaseStruct.h"
#include "ErrorCode.h"
#include "ClientProtocol.h"
#include "PublicType.h"
#include "Log.h"
#include <time.h>
#include "ConfigInfo.h"


#define DEF_ORASET  struct sqlca sqlca;

#define SET_END_NULL(DB_VAR)  DB_VAR.arr[DB_VAR.len] = '\0'
#define SET_INIT_NULL(DB_VAR) {memset(DB_VAR.arr, 0, sizeof(DB_VAR.arr));DB_VAR.len = 0;}

/************************************************************************ 
文件描述：数据库网关类的声明文件 

创建人：hechang
时间：	2006-6-16 
************************************************************************/
//#pragma comment(lib,"orasql9.lib")
//#pragma comment(lib,"oraSQL10.LIB")


class CDatabaseGW
{
public:
	CDatabaseGW(void);
	~CDatabaseGW(void);
	
	//连接数据库,指定用户密码与连接
	//参数：
	//pUserName--用户名
	//pPassword--密码
	//pLinkName--连接名
	//返回值:true-成功　false-失败
	bool connect(char* pUserName,char* pPassword,char* pServerName);

	//连接mini的数据库
	bool ConnectMini();

	//连接网站的数据库
	bool ConnectWeb();

	bool ConnectFamily();

	void DisConnect();

protected:
	void sql_error(struct sqlca sqlca,struct oraca oraca);
	bool conn_error(int db_ret);
	char  m_Connect[32];						//连接标识符	

	static int		m_nConnectCount;			//当前连接数，用于生成连接标识符
	static int		m_nContextid;				//当前最大运行上下文id

	int				m_nMyContextid;				//类的对象实际使用的上下文id
	
	static CSyncCritical	m_CriticalLock;			//同步锁
 	static char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
};


