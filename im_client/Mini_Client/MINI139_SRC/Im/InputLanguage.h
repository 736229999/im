#pragma once

//���뷨������

class CInputLanguage
{
public:
	struct IL
	{
 		char ilID[15]; //���뷨���š�
		char szName[100];//���뷨��˵�����֡�
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
