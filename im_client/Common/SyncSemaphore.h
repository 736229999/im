#pragma once

//ʹ���ź�����ʵ�ֶ��̻߳�����ʵ���

class CSyncSemaphore
{
public:
	CSyncSemaphore(int nInitValue,int nMaxCount);
	~CSyncSemaphore(void);

	//��ȡ�ź������ʣ�������ܻᷢ���������û�п����ź�����ʱ��
	void Lock();
	void Unlock();

private:
	HANDLE m_handle;
};
