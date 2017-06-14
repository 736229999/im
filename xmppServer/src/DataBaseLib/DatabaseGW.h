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
�ļ����������ݿ�������������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-16 
************************************************************************/
//#pragma comment(lib,"orasql9.lib")
//#pragma comment(lib,"oraSQL10.LIB")


class CDatabaseGW
{
public:
	CDatabaseGW(void);
	~CDatabaseGW(void);
	
	//�������ݿ�,ָ���û�����������
	//������
	//pUserName--�û���
	//pPassword--����
	//pLinkName--������
	//����ֵ:true-�ɹ���false-ʧ��
	bool connect(char* pUserName,char* pPassword,char* pServerName);

	//����mini�����ݿ�
	bool ConnectMini();

	//������վ�����ݿ�
	bool ConnectWeb();

	bool ConnectFamily();

	void DisConnect();

protected:
	void sql_error(struct sqlca sqlca,struct oraca oraca);
	bool conn_error(int db_ret);
	char  m_Connect[32];						//���ӱ�ʶ��	

	static int		m_nConnectCount;			//��ǰ�������������������ӱ�ʶ��
	static int		m_nContextid;				//��ǰ�������������id

	int				m_nMyContextid;				//��Ķ���ʵ��ʹ�õ�������id
	
	static CSyncCritical	m_CriticalLock;			//ͬ����
 	static char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
};


