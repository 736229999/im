
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <dirent.h>	
#include "mfcstr.h"
#include "WinCommonForLinux.h"	
	
 char * strlwr (char * pStr)
 {
 	char* pBegin = pStr;
	unsigned char cChar;
	while( (cChar = *pStr) != 0 )
	{
		if( cChar >= 'A' && cChar <= 'Z' )
		{
			*pStr = 'a' + cChar - 'A';
		}
		pStr++;
	}
	return pBegin;
 }
 
 
 BOOL PathFileExists(const char* pFilePath)
 {
	return 0 == access(pFilePath,F_OK); 
 }
 
 BOOL DeleteFile ( LPCTSTR lpFileName )
 {
 	return 0 ==unlink(lpFileName);
 }


int GetPrivateProfileString( const char *section, const char *key,const char *default_value,char *value, int size, const char *file)
{
	BOOL bSetting = FALSE;
	FILE* pf = fopen(file,"r");
	if( pf)
	{
		BOOL bMatchSection = FALSE;
		char szLinebuf[10000];
		while (!feof(pf))
		{
			fgets(szLinebuf,sizeof(szLinebuf),pf);

			CString strLine = szLinebuf;
			strLine.Trim();
			
			if( strLine.GetLength() == 0  )
			{
				continue;
			}
			
			if( strLine[0] == ';')
			{
				continue;
			}

			if( strLine[0] == '[' && strLine[strLine.GetLength()-1] == ']' )
			{
				if( bMatchSection )
				{
					break;
				}

				if( strncmp(section,((LPCTSTR)strLine) +1,strLine.GetLength() -2  ) == 0 )
				{
					bMatchSection = TRUE;
				}

			}
			else
			{
				if( bMatchSection)
				{
					int nFindPos = strLine.Find('=');
					if( -1 == nFindPos)
					{
						continue;
					}
	
					CString strLeft;
					strLeft = strLine.Left(nFindPos);
					strLeft.Trim();

					if( strLeft == key )
					{
						CString strRight;
						strRight = strLine.Mid(nFindPos+1);
						strRight.Trim();

						strncpy(value,(LPCTSTR)strRight,size);
			
						bSetting = TRUE;

					}
				}
			}

		}

		fclose(pf);
	}
	else
	{
		//IBDEBUG("open ini file failed");	
		return 0;
	}

	if( !bSetting)
	{
		strncpy(value,default_value,size);
	}

	return size;
}

int GetPrivateProfileInt( const char *section, const char *key,int default_value, const char *file)
{
	char szValue[32] = "";
	if( GetPrivateProfileString(section,key,"",szValue,sizeof(szValue),file) > 0 )
	{
		if( szValue[0] > 0 )
		{
			return atoi(szValue);
		}
	}
	return default_value;
}
BOOL WritePrivateProfileString( const char *section, const char *key,const char *value, const char *file)
{


	CString strNewConfig;
	FILE* pf = fopen(file,"r");
	if( pf)
	{
		BOOL bMatchSection = FALSE;
		BOOL bMatchKey = FALSE;
		char szLinebuf[10000];
		while (!feof(pf))
		{
			if( NULL == fgets(szLinebuf,sizeof(szLinebuf),pf) )
			{
				continue;
			}

			CString strLine = szLinebuf;
			strLine.Trim();
			
			if( strLine.GetLength() == 0  )
			{
				strNewConfig+=szLinebuf;
				continue;
			}

			if( strLine[0] == ';')
			{
				strNewConfig+=szLinebuf;
				continue;
			}

			if( bMatchKey)
			{
				strNewConfig+=szLinebuf;
				continue;
			}

			if( strLine[0] == '[' && strLine[strLine.GetLength()-1] == ']' )
			{
				if( bMatchSection )
				{
					if( !bMatchKey)
					{
						CString strAddKey;
						strAddKey.Format("%s=%s\n",key,value);
						strNewConfig += strAddKey;

						bMatchKey = TRUE;
					}
				}
				else if( strncmp(section,((LPCTSTR)strLine) +1,strLine.GetLength() -2  ) == 0 )
				{
					bMatchSection = TRUE;
				}

			}
			else
			{
				if( bMatchSection)
				{
					int nFindPos = strLine.Find('=');
					if( -1 == nFindPos)
					{
						strNewConfig+=szLinebuf;
						continue;
					}

					CString strLeft;
					strLeft = strLine.Left(nFindPos);
					strLeft.Trim();

					if( strLeft == key )
					{
						char* pNewData = strchr(szLinebuf,'=');
						if( pNewData)
						{
							strcpy(pNewData+1,value);
							strcat(szLinebuf,"\r\n");

							bMatchKey = TRUE;
						}
					}
				}
			}

			strNewConfig+=szLinebuf;

		}

		fclose(pf);

		if( !bMatchKey)
		{
			CString strAddKey;
			if( bMatchSection)
			{
				strAddKey.Format("%s=%s\n",key,value);	
			}
			else
			{
				strAddKey.Format("[%s]\n%s=%s\n",section,key,value);				
			}
			strNewConfig += strAddKey;

		}

		FILE* pf = fopen(file,"w");
		if( pf)
		{
			fwrite((LPCTSTR)strNewConfig,strNewConfig.GetLength(),1,pf);
			fclose(pf);
		}
		else
		{
			return FALSE;
		}

	}
	else
	{
		return FALSE;;
	}

//	strncpy(value,default_value,size);

//	return size;
	return TRUE;
}


int WSAGetLastError(void)
{
	return errno;
}

pthread_t CreateThread(void* lpThreadAttributes,int dwStackSize,void *(*thread_fun)(void*),void* arg, DWORD dwCreationFlags, DWORD* lpThreadId)
{
	pthread_t _thread = 0;
	pthread_create(&_thread, NULL, thread_fun, arg);

	return _thread;
}
DWORD WaitForSingleObject(pthread_t handle,int ntime)
{
	pthread_join(handle, NULL);
	return WAIT_OBJECT_0;
}

DWORD WaitForMultipleObjects(DWORD dwSize,
							 pthread_t* pArray,BOOL bWaitAll,int ntime)
{
	for( DWORD i = 0 ; i < dwSize;++i )
	{
		pthread_join(pArray[i], NULL);
	}
	return WAIT_OBJECT_0;
}

void TerminateThread(pthread_t handle,int nFlag)
{
	
}
void CloseHandle(pthread_t handle)
{

}
/*
int closesocket (  SOCKET s)
{
	closeSocket(s);
}*/

void Sleep(int dwMilliseconds)
{
/*
	struct timeval tv;
	tv.tv_sec = dwMilliseconds/1000;
	tv.tv_usec= dwMilliseconds%1000;
	select(0, NULL, NULL, NULL ,&tv);
*/
	usleep(dwMilliseconds*1000);
}

//////////////////////////////////////////////////////////////////////////
//实时器


static void* TimerThreadEntry(void* lpParam)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);



	PMMRESULT p = (PMMRESULT)lpParam;
	PFN fn = p->pfn;

	do
	{
		pthread_testcancel();

		Sleep(p->uDelay);

		if(fn)
			fn(p->lpParam);
	}while(!(p->bOneShot));

	return NULL;
}

PMMRESULT timeSetEvent(unsigned int uDelay,unsigned int uResolution,PFN pfn,void *lpParam,bool OneShot)
{
	printf("timeSetEvent\n");

	PMMRESULT pmm = (PMMRESULT)malloc(sizeof(MMRESULT));
	if(NULL == pmm)
		return NULL;

	pmm->uDelay = uDelay;
	pmm->bOneShot = OneShot;
	pmm->pfn = pfn;
	pmm->lpParam = lpParam;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	int n = pthread_create(&pmm->tid,&attr,TimerThreadEntry,(void*)pmm);
	pthread_attr_destroy(&attr);

	if(n ==-1)
	{
		free(pmm);
		return NULL;
	}
	return pmm;
}

void timeKillEvent(PMMRESULT timer)
{
	if(timer == NULL)
		return;
	if(timer->bOneShot)
		return;

	timer->bOneShot = true;
	if(timer->tid)
		pthread_cancel(timer->tid);

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//实时器设置event
bool timeSetEventNew(timer_t& t1, uint32_t uDelay, uint32_t uResolution, timerCallbackFn fn, void *lpParam)
{
	//信号event
	struct sigevent se;
	memset(&se, 0, sizeof(struct sigevent));
	se.sigev_value.sival_ptr = lpParam;
    se.sigev_notify = SIGEV_THREAD;
    se.sigev_notify_function = fn;

	//创建定时器
	if(timer_create(CLOCK_REALTIME, &se, &t1) < 0) {
		return false;
	}

	//设置延迟时间和定时间隔
	//it_interval指定间隔时间,it_value指定初始定时时间
	struct itimerspec it;
	it.it_interval.tv_sec = uResolution / 1000;     /* seconds */  
	it.it_interval.tv_nsec = uResolution % 1000 * 1000000;       /*nanoseconds */ 
	it.it_value.tv_sec = uDelay / 1000;
	it.it_value.tv_nsec = uDelay % 1000 * 1000000;

	//设置定时器
	if(timer_settime(t1, 0, &it, 0) < 0) {
		timer_delete(t1);
		return false;
	}
	
	return true;
}

/*
void timeKillEventNew(time_t& t1)
{
	timer_delete(t1);
}
*/



DWORD timeGetTime(void)
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec/1000);
}

unsigned long timeGetTimeUsec(void)
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec/1000);
}


//////////////////////////////////////////////////////////////////////////
void InitializeCriticalSection( LPCRITICAL_SECTION lpCriticalSection )
{
	::pthread_mutex_init(lpCriticalSection,NULL);
}

void DeleteCriticalSection( LPCRITICAL_SECTION lpCriticalSection )
{
	::pthread_mutex_destroy(lpCriticalSection);
}
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	::pthread_mutex_lock(lpCriticalSection);
}

void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	::pthread_mutex_unlock(lpCriticalSection);
}

void localtime_s(struct tm * _Tm, const time_t * _Time)
{
	struct tm*  pTempTime = NULL;
	pTempTime = localtime(_Time);
	*_Tm = *pTempTime;
}

DWORD GetModuleFileName(void* hModule, char* lpFilename, DWORD nSize )
{
	char szProcFilePath[80];
	snprintf(szProcFilePath, sizeof(szProcFilePath), "/proc/%d/exe", (int)getpid());

	memset(lpFilename, 0, nSize);
	readlink(szProcFilePath, lpFilename, nSize-1);

	return 0;
}

BOOL CreateDirectory( LPCTSTR lpPathName, void* lpSecurityAttributes )
{
	char command[1024];
	snprintf(command, sizeof(command), "mkdir %s",lpPathName);
	system(command);
	return TRUE;
}

