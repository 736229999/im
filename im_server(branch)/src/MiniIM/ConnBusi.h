/************************************************************************ 
 文件描述：动态负载业务，向接入服务器注册，并定时请求服务器状态列表
 
 创建人： Zouyf, 2006-6-29 
 
 修改记录： 
            
************************************************************************/

#pragma once
#include "../../include/SrvFrameProtocol.h"
class CConnBusiControl;
//具体协议业务处理
class CConnBusi 
{
public:
	CConnBusi(CConnBusiControl * pConnBusiControl);
	~CConnBusi(void);
	//业务请求
	void BusiRequest(const LPSERVER_REGISTER lpReq);
	void BusiRequest(const LPSERVER_UNREGISTER lpReq);
	void BusiRequest(const LPSERVER_STATUS_REPORT lpReq);
	void BusiRequest(const LPSERVER_STATUSLIST_REQ lpReq);
private:
	CConnBusiControl * m_pConnBusiControl;
};
