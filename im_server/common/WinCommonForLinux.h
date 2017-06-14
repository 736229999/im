#ifndef WINCOMOMFOR_LINUX_H
#define WINCOMOMFOR_LINUX_H

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <arpa/inet.h>
#include <signal.h>

typedef  unsigned int	DWORD;
typedef  const char*	LPCTSTR;
typedef  char*		LPTSTR;
typedef  unsigned int   HMODULE;
typedef  int 		BOOL;
typedef	 unsigned char	byte;
typedef  void*		LPVOID;
typedef  unsigned short WORD;
typedef  char		TCHAR;
typedef unsigned int    UINT32;



typedef int SOCKET; 
typedef unsigned char BYTE; 
typedef unsigned int  DWORD; 
typedef unsigned int  UINT;
typedef   DWORD DWORD_PTR;

#define  WINAPI 
#define  APIENTRY
typedef void *HANDLE;

#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH 2
#define DLL_THREAD_DETACH 3
#define DLL_PROCESS_DETACH 0

#ifndef FALSE
#define FALSE 0 
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1) 
#endif

#define MAX_PATH	300

typedef struct _WSABUF {
	DWORD len;     /* the length of the buffer */
	char *buf; /* the pointer to the buffer */
} WSABUF, * LPWSABUF;

typedef  struct sockaddr_in SOCKADDR_IN;

#define WSAESHUTDOWN ESHUTDOWN
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif


#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

char * strlwr (char * pStr);
 
BOOL PathFileExists(const char* pFilePath);
BOOL DeleteFile ( LPCTSTR lpFileName );

int GetPrivateProfileString( const char *section, const char *key,const char *default_value,char *value, int size, const char *file);
int GetPrivateProfileInt( const char *section, const char *key,int default_value, const char *file);
BOOL WritePrivateProfileString( const char *section, const char *key,const char *value, const char *file);
int WSAGetLastError(void);
inline int closesocket (  SOCKET s)
{
	return close(s);
}
inline 	int WSACleanup(void)
{
	return 0;
}

#define  WAIT_OBJECT_0 0
pthread_t CreateThread(void* lpThreadAttributes,int dwStackSize,void *(*thread_fun)(void*),void* arg, DWORD dwCreationFlags, DWORD* lpThreadId);
DWORD WaitForSingleObject(pthread_t handle,int ntime);
DWORD WaitForMultipleObjects(DWORD dwSize,
						 pthread_t* pArray,BOOL bWaitAll,int ntime);

void TerminateThread(pthread_t handle,int nFlag);
void CloseHandle(pthread_t handle);



void Sleep(int dwMilliseconds);

typedef void(*PFN)(void*);
typedef void(*timerCallbackFn)(union sigval v);

enum{TIME_ONESHOT = true,TIME_PERIODIC = false};
typedef struct
{
	unsigned int uDelay;
	pthread_t tid;
	PFN pfn;
	void *lpParam;
	bool bOneShot;
}MMRESULT,*PMMRESULT;
PMMRESULT timeSetEvent(unsigned int uDelay,unsigned int uResolution,PFN pfn,void *lpParam,bool OneShot);
bool timeSetEventNew(timer_t& t1, uint32_t uDelay, uint32_t uResolution, timerCallbackFn fn, void *lpParam);
void timeKillEvent(PMMRESULT timer);
DWORD timeGetTime(void);
unsigned long timeGetTimeUsec(void);


typedef pthread_mutex_t CRITICAL_SECTION,*LPCRITICAL_SECTION;
void InitializeCriticalSection( LPCRITICAL_SECTION lpCriticalSection );
void DeleteCriticalSection( LPCRITICAL_SECTION lpCriticalSection );
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);


#define  _vstprintf_s vsprintf
#define  _snprintf_s  snprintf
#define  sprintf_s   sprintf
#define  strcat_s	strcat
#define  OutputDebugString(a) 

void localtime_s(struct tm * _Tm, const time_t * _Time);

DWORD GetModuleFileName(void* hModule, char* lpFilename, DWORD nSize );

typedef int errno_t;
inline errno_t strcpy_s( char* strDestination, const char *strSource )
{
	strcpy(strDestination,strSource);
	return 0;
}

inline errno_t strcpy_s( char *strDestination, size_t numberOfElements, const char *strSource )
{
	strncpy(strDestination,strSource,numberOfElements);
	return 0;
}

inline errno_t strncpy_s(char* _Dest, int nDestSize, const char * _Source, int _Count)
{
	strncpy(_Dest,_Source,nDestSize);
	return 0;
}

inline errno_t strncpy_s(char* _Dest, const char * _Source, int nDestSize)
{
	strncpy(_Dest,_Source,nDestSize);
	return 0;
}

BOOL CreateDirectory( LPCTSTR lpPathName, void* lpSecurityAttributes );


inline char * itoa(int _Val, char * _DstBuf, int _Radix)
{
	sprintf(_DstBuf,"%d",_Val);
	return _DstBuf;
}

#define  _itoa_s itoa

#endif 



