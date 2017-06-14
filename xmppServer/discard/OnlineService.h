#pragma once
#include "ntservice.h"
#include "AppManager.h"

class COnlineService :
	public CNTService
{
public:
	COnlineService(void);
	~COnlineService(void);
	virtual BOOL OnInit();
	virtual void Run();
	virtual void OnStop();
	virtual BOOL OnUserControl(DWORD dwOpcode);

};
