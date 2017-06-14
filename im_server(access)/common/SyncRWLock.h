#pragma once

/************************************************************************ 
�ļ���������д����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-9 
************************************************************************/


#include "SyncCritical.h"
#include "SyncEvent.h"


//����ʵ�ֶ�д�����࣬����Ϊ�������������ͬʱ���У�д����һ��ʱ��ֻ��
//��һ���ڽ��У�����д����ʱ�����������ܽ��У����������ڽ���ʱ��д����
//Ҳ���ܽ��У�����ʱд��������ֹ����Ķ���������ǰ�Ķ�������󣬲ſ�ʼд����
class CSyncRWLock
{
public:
	CSyncRWLock(void);
	~CSyncRWLock(void);

	void WriteLock();
	void WriteUnlock();

	void ReadLock();
	void ReadUnlock();


private:
	CSyncCritical		m_WriteLock;
	CSyncCritical		m_ReadLock;

	CSyncEvent			m_EnableWrite;
	int					m_nReaderNum;

};
