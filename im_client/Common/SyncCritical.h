#pragma once


//ʹ���ٽ�����ʵ�ֶ��̻߳�����ʵ���

class CSyncCritical
{
public:
	CSyncCritical(void);
	~CSyncCritical(void);

	//��ȡ������ʣ�����б���߳��ڶ�������ᷢ������
	void Lock();
	void Unlock();

	CRITICAL_SECTION * Get()
	{ return &m_sect;};

private:
	CRITICAL_SECTION	m_sect;
};

