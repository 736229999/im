#include "SyncEvent.h"
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

CSyncEvent::CSyncEvent(	bool bManualReset,bool bInitialState)
{
	sem_init(&m_handle,0,bInitialState?1:0);
}

CSyncEvent::~CSyncEvent(void)
{
	sem_destroy(&m_handle);
}


void CSyncEvent::SetEvent()
{
	sem_post(&m_handle);
}

void CSyncEvent::ResetEvent()
{
	int nSval = 0;
	while(1)
	{
		sem_getvalue(&m_handle,&nSval);
		if( nSval > 0 )
		{
			sem_wait(&m_handle);
		}
		else
		{
			break;
		}
	}


}

void CSyncEvent::Wait()
{
	sem_wait(&m_handle);
	int nSval = 0;
	sem_getvalue(&m_handle,&nSval);
	if( nSval == 0 )
	{
		sem_post(&m_handle);
	}
}

int CSyncEvent::GetValue()
{
	int nSval = 0;
	sem_getvalue(&m_handle,&nSval);
	return nSval;
}

DWORD WaitForSingleObject(CSyncEvent& hHandle, DWORD dwMilliseconds )
{
	DWORD dwWaitTime = dwMilliseconds ;
	do 
	{
		if( hHandle.GetValue() > 0 )
		{
			return WAIT_OBJECT_0;
		}

		Sleep(1);

		if( dwWaitTime != INFINITE)
		{
			dwWaitTime--;
		}
	} while (dwWaitTime > 0);

	return WAIT_FAILED;
}

DWORD WaitForMultipleObjects( DWORD nCount, CSyncEvent *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds )
{
	DWORD dwWaitTime = dwMilliseconds ;
	DWORD dwEventObjectIndex = 0;
	
	int nWaitNum = bWaitAll?nCount:1;
	int nTotalEventNum = 0;
	do 
	{
		nTotalEventNum = 0;
		dwEventObjectIndex = 0;
		bool bFind = false;
		for( DWORD i = 0 ; i < nCount;++i)
		{
			if( lpHandles[i].GetValue() > 0)
			{
				if( bFind == false)
				{
					bFind = true;
					dwEventObjectIndex = i;
				}
				nTotalEventNum++;
			}
		}
		if( nTotalEventNum>= nWaitNum )
		{
			return dwEventObjectIndex;
		}
		Sleep(1);

		if( dwWaitTime != INFINITE)
		{
			dwWaitTime--;
		}

	} while (dwWaitTime > 0 );

	return WAIT_FAILED;

}
