#ifndef SSDBCONNCHECK_H
#define SSDBCONNCHECK_H

#include "../common/WinCommonForLinux.h"

class SSDBConnCheck
{
public:
	SSDBConnCheck(char* addr);
	~SSDBConnCheck();
	int StartWork();
	void StopWork();
	
private:
	friend void  ConnProc(void* dwUser);
	void HandleTimer();

	void StartTimer();
	void StopTimer();

	char* ssdbAddr;
private:
	PMMRESULT m_hMMT;
};

#endif
