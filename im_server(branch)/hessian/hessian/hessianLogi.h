#ifndef HESSIANLOGI
#define HESSIANLOGI

#include "../hessian/HttpConnection.h"
#include "../LogInterface/UploadSubMerchantActionService.h"
#include "../LogInterface/UploadSubMerchantActionServiceProxy.h"
//#include "WinCommonForLinux.h"
//#include "PublicType.h"
#include "stdafx.h"

#include "ConfigInfo.h"

using namespace LogInterface;
using namespace std;

class hessianLogi
{
	public:
		hessianLogi();
		hessianLogi(uint32 merchant_id,uint32 sub_merchant_id);
		~hessianLogi();
		void LoadHessianUrl();
		UploadSubMerchantActionServiceProxy *GetHessiani();
		
	private:
		char m_url[512];
		hessian::HttpConnection *m_connection;
		UploadSubMerchantActionServiceProxy *m_ServiceProxy;
		CConfigInfo	m_ConfigInfo;
		vector<string> hessian_url;
};

#endif
