#pragma once

//输入法控制类

class CInputLanguage
{
public:
	struct IL
	{
 		char ilID[15]; //输入法代号。
		char szName[100];//输入法的说明文字。
		IL*  pNext;
	}; 

public:
	CInputLanguage(void);
	virtual ~CInputLanguage(void);

public:
	BOOL SelectInputLanguage(IL *pIL);
	IL * GetInputLanguageList();

private:
	IL* m_pILHead;
};
