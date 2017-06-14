#ifndef SYS_STATIC_H
#define SYS_STATIC_H

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


typedef struct _cpuInfo
{	
	unsigned int u, n, s, i, w, x, y, z; 	
	unsigned int u_sav, n_sav, s_sav, i_sav, w_sav, x_sav, y_sav, z_sav;  
	float cpu_use;	/* cpu use ratio */
	unsigned char id;  /* the CPU ID number */	

	_cpuInfo()
	{
		memset(this, 0, sizeof(_cpuInfo));
	}
} CpuInfo;

typedef struct _memInfo_t 
{	
	unsigned int nTotal;
	unsigned int nFree;

	_memInfo_t()
	{
		memset(this, 0, sizeof(_memInfo_t));
	}
} MemInfo;   

typedef struct _checkResult
{
	char 			CurTime[32]; 
	char  			SvrIp[32]; 
	unsigned int 	Svrport;
	float 			CpuUsed;
	unsigned int	MemAll;
	unsigned int    MemUsed;
	unsigned int 	onlinenum;
	unsigned long long    RcvPacket;
	unsigned long long    SendPacket;

	_checkResult()
	{
		memset(this, 0, sizeof(_checkResult));
	}
}CheckResult;

class SysStat
{
	public:

		static SysStat& GetInstance();
		SysStat(){onlinenum = revnum = sndnum = 0;}
		bool getCpuUseRatio();
		bool getMemInfo();
		CpuInfo& getCpu(){return cpu;}
		MemInfo& getMem(){return mem;}
		void addOnlineNum(){onlinenum ++;}
		void rmOnlineNum(){onlinenum --;}
		unsigned int getOnlineNum(){return onlinenum;}
		void setRevNum(){revnum ++;}		
		unsigned long long getRevNum(){return revnum;}
		void setSndNum(){sndnum ++;}
		unsigned long long getSndNum(){return sndnum;}
//		void sysLog(char *fmt, ...);
		

	private:

		CpuInfo cpu;
		MemInfo mem;
		unsigned int onlinenum;
		unsigned long long revnum;
		unsigned long long sndnum;
		
};
#endif

