#include "SysStatic.h"

char SYS_LOG_FILE[] = {"Log/sysStat.log"};
char LINE[] = {"-----------------------------------------------------------------------\n"};

SysStat& SysStat::GetInstance()
{
	static SysStat SingleInstance;
	return SingleInstance;
}


bool SysStat::getMemInfo()
{
	FILE *fp = NULL;
	char buf[200] = {0};  
	bool GETMEMTOL = false;
	bool GETMEMFREE = false;
	
	fp = fopen("/proc/meminfo", "r");
	if( fp == NULL ) 
	{
    	fprintf(stdout, "getMemInfo: open /proc/meminfo error");
		return false;
	}

    while(1)
	{	
		memset(buf, 0x0, 200);
        if(!fgets(buf, 200, fp)) 
		{
			fprintf(stdout, "failed /proc/meminfo read\n");
			fclose(fp);
			return false;
		}

        if( strncmp(buf, "MemTotal", strlen("MemTotal")) == 0 ) 
		{
            sscanf(buf, "MemTotal: %u", &(mem.nTotal));
			GETMEMTOL = true;
		}
        else if( strncmp(buf, "MemFree", strlen("MemFree")) == 0 )
		{
            sscanf(buf, "MemFree: %u", &(mem.nFree));
			GETMEMFREE = true;
			break;
        }
		else
			continue;
	}

	fclose(fp);
	
	if(GETMEMTOL && GETMEMFREE)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool SysStat::getCpuUseRatio()   
{   
   	FILE *fp = NULL;   
   	char buf[200] = {0};   
	int num = 0; 
	int cpu_total = 0;
	int cpu_total_sav = 0;
	
    if(!(fp = fopen("/proc/stat", "r")))
    {
        fprintf(stdout, "Failed /proc/stat open: %s\n", strerror(errno));   
		return false;
    }  

   	if(!fgets(buf, sizeof(buf), fp)) 
	{
		fprintf(stdout, "failed /proc/stat read\n");
		fclose(fp);
		return false;
	}

	fclose(fp);
	
   	cpu.x = 0;
   	cpu.y = 0;
   	cpu.z = 0;
   	num = sscanf(buf, "cpu %u %u %u %u %u %u %u %u",   
      				&cpu.u, &cpu.n, &cpu.s, &cpu.i, &cpu.w, &cpu.x, &cpu.y, &cpu.z);
	
   	if (num < 4)
   	{	
         	fprintf(stdout, "failed /proc/stat read num[%d] < 4\n",num);  
		return false;
   	}
	
	if(cpu.i < cpu.i_sav)
	{
		cpu.cpu_use = 1.0;
	}
	else
	{
		cpu_total = 
			cpu.u + cpu.n + cpu.s + cpu.i + cpu.w + cpu.x + cpu.y + cpu.z;
		cpu_total_sav = 
			cpu.u_sav + cpu.n_sav + cpu.s_sav + cpu.i_sav + cpu.w_sav + cpu.x_sav + cpu.y_sav + cpu.z_sav;
		cpu.cpu_use = 1 - ((float)(cpu.i - cpu.i_sav))/(cpu_total - cpu_total_sav);
	}
	
	/*记录当前值，为下次计算备用*/
    cpu.u_sav = cpu.u;   
   	cpu.s_sav = cpu.s;   
   	cpu.n_sav = cpu.n;   
   	cpu.i_sav = cpu.i;   
   	cpu.w_sav = cpu.w;   
   	cpu.x_sav = cpu.x;   
   	cpu.y_sav = cpu.y;   
   	cpu.z_sav = cpu.z; 
	
   	return true;   
} 
#if 0
void SysStat::sysLog(CheckResult *result)
{
	int fd;
	char enter[] = "\n\n";
	struct stat logstat;
	char add[128] = {0};
	char sys[128] = {0};
	char online[128] = {0};
	
	if (result == NULL) return;

    stat(SYS_LOG_FILE,&logstat);
	if(logstat.st_size > 2 * 1024*1024)
		remove(SYS_LOG_FILE);
	if ((fd = open(SYS_LOG_FILE, O_CREAT | O_APPEND | O_WRONLY, 0644)) == -1) {
		fprintf(stderr, "open %s failed\n", SYS_LOG_FILE);
		return;
	}   

	printf("Current Time: %s",result.CurTime);	
	printf("Server Ip: %s, Port: %d\n",result.SvrIp, result.Svrport);
	printf("CPU USED RATIO: %f,MEMERY TOTAL: %d, MEMERY USED: %d\n",result.CpuUsed,result.MemAll,result.MemUsed);
	printf("ONLINE PEOPEL: %d,RECEIVE PACKET: %lld,SEND PACKET: %lld\n",result.onlinenum,result.RcvPacket,result.SendPacket);
	
	write(fd, LINE, strlen(LINE));
	
	write(fd, enter, 2);

	close(fd);

}
#endif

