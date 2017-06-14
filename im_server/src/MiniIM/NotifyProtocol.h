#pragma once
class CGlobalBusi;
class CNotifyProtocol
{
public:
	CNotifyProtocol(void);
	~CNotifyProtocol(void);

	static void Relogin(CGlobalBusi *pGlobalBusi,uint32 uid);
};


