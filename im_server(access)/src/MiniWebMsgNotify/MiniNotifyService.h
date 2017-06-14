#pragma once
//#include "ntservice.h"

class CMiniNotifyService
//	:public CNTService
{
public:
	CMiniNotifyService(void);
	virtual ~CMiniNotifyService(void);
	virtual BOOL OnInit();
	virtual void Run();
	virtual void OnStop();
	virtual BOOL OnUserControl(DWORD dwOpcode);

};
