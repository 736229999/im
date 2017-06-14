#pragma once
//#include "ntservice.h"

class CMiniIMService 
	//:public CNTService
{
public:
	CMiniIMService(void);
	virtual ~CMiniIMService(void);
	virtual BOOL OnInit();
	virtual void Run();
	virtual void OnStop();
	virtual BOOL OnUserControl(DWORD dwOpcode);

};

