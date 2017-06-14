#pragma once
#include "DataBaseLib.h"
#include "ClientProtocol.h"
#include "SyncCritical.h"
#include "GlobalBusi.h"

#include "jabberd/c2s/c2s.h"
#include "jabberd/sx/sx.h"
#include "jabberd/config.h"


//应用层回调函数
typedef void (*pfnjabberdCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
typedef void (*generatepkt)(uint32 fd,char *username,char *pwd,uint32 src_ip, uint16 src_port,uint16 cmd,
		uint32 fromid,uint32 toid,RecvDataStruct *RecvData,void *input);



class JabberdIm
{

	public:		

		JabberdIm(void);		
		~JabberdIm(void);
		
		static void InitHeadSasl(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 sid,uint32 did,uint8 dtype , uint8 flag);
		static void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 sid,uint32 did,uint8 dtype , uint8 flag);
		static int InitPkt(uint32 fd,uint32 src_ip, uint16 src_port,RecvDataStruct *RecvData);
		
		static const char* TransPwd(const char* pwd);
		static bool CheckSeller(char* user, const char* pwd, uint32& uid, uint32& merchant_id, uint32& state);
		static bool CheckBuyer(const char* user, const char* password, char* guid,	char * szUserName,const char *ip);
		static void MD5Password(char* pPassword);		
		void setDataBaseWeb(CDataBaseWeb *database){ m_pDataBaseWeb = database;};
		static void generate_pkt(uint32 fd,char *username,char *pwd,uint32 src_ip, uint16 src_port,uint16 cmd,
	uint32 fromid,uint32 toid,RecvDataStruct *RecvData,void *input);
#if 1		
		void SetUserCallback(void * pUserKey,pfnjabberdCallback pfnCallback)
		{
			m_pUserKey = pUserKey;
			m_pfnUserCallback = pfnCallback;
		}
#endif
		int StartWork();
		static void *jabberd(void *params);	
		static void _c2s_signal(int signum);		
		static void _c2s_signal_hup(int signum);		
		static void _c2s_signal_usr1(int signum);		
		static void _c2s_signal_usr2(int signum);		
		/** store the process id */
		static void _c2s_pidfile(c2s_t c2s); 
		/** pull values out of the config file */
		static void _c2s_config_expand(c2s_t c2s);		
		static void _c2s_hosts_expand(c2s_t c2s);		
		static int _c2s_sx_sasl_callback(int cb, void *arg, void **res, sx_t s, void *cbarg); 
		static void _c2s_time_checks(c2s_t c2s) ;
		
		static sig_atomic_t c2s_shutdown;
		static sig_atomic_t c2s_logrotate;
		static sig_atomic_t c2s_sighup;		
//		static sig_atomic_t c2s_lost_router;
		
	private:
		void * m_pUserKey;
		pfnjabberdCallback m_pfnUserCallback;
		CDataBaseWeb*		m_pDataBaseWeb;

};



