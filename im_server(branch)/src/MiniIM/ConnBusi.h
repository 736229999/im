/************************************************************************ 
 �ļ���������̬����ҵ������������ע�ᣬ����ʱ���������״̬�б�
 
 �����ˣ� Zouyf, 2006-6-29 
 
 �޸ļ�¼�� 
            
************************************************************************/

#pragma once
#include "../../include/SrvFrameProtocol.h"
class CConnBusiControl;
//����Э��ҵ����
class CConnBusi 
{
public:
	CConnBusi(CConnBusiControl * pConnBusiControl);
	~CConnBusi(void);
	//ҵ������
	void BusiRequest(const LPSERVER_REGISTER lpReq);
	void BusiRequest(const LPSERVER_UNREGISTER lpReq);
	void BusiRequest(const LPSERVER_STATUS_REPORT lpReq);
	void BusiRequest(const LPSERVER_STATUSLIST_REQ lpReq);
private:
	CConnBusiControl * m_pConnBusiControl;
};
