#pragma once
/************************************************************************ 
�ļ���������д����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-9 
************************************************************************/



//ʹ���¼�ʵ�ֶ��̻߳�����ʵ���

class CSyncEvent
{
public:
	CSyncEvent(bool bManualReset,bool bInitialState);
	~CSyncEvent(void);

	void SetEvent();
	void ResetEvent();

	void Wait();

private:
	HANDLE m_handle;

};
