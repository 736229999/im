#pragma once

#include "SyncCritical.h"

/************************************************************************ 
�ļ��������Զ�ID������������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-7-4 
************************************************************************/


//���ڷ���һ��ΨһID

class CAutoID
{
public:
	CAutoID(void);
	~CAutoID(void);

	//�õ�һ���µ�ID
	int GetNewID();

	//ȡ��һ��ʹ�õ�ID
	void DeleteID(int nOldID);

private:
	int		  m_nMaxIDNum;			//ʹ�õ�ID��
	list<int> m_listBackup;			//���յ�ID�б�

	CSyncCritical	m_SyncLock;			//ͬ����
};

