#pragma once
#include "ntservice.h"
#include "AppManager.h"

class CFamilyService :
	public CNTService
{
public:
	CFamilyService(void);
	~CFamilyService(void);
	virtual BOOL OnInit();
	virtual void Run();
	virtual void OnStop();
	virtual BOOL OnUserControl(DWORD dwOpcode);
	
};
