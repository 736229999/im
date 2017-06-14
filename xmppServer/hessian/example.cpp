/*
	Create: Du-Yong
	Date: 2015-07-20
	For test hessian interface and as an example
	 

*/
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <memory>

#include "hessian/hessianLogi.h"


using namespace LogInterface;
using namespace std;
#if 0


class hessianLogi
{
	public:
		hessianLogi();
		UploadSubMerchantActionServiceProxy *GetHessiani();
		
	private:
		char m_url[288];
		hessian::HttpConnection *m_connection;
		UploadSubMerchantActionServiceProxy *m_ServiceProxy;
		CConfigInfo	m_ConfigInfo;
};


hessianLogi::hessianLogi()
{
	char hessianAddr[] = m_ConfigInfo.GetHessianAddr();	
	char hessianMethod[] = m_ConfigInfo.GetHessianMethod();
	strncpy(m_url,hessianAddr,20);	
	strncat(m_url,hessianMethod,256);
	m_connection = new hessian::HttpConnection(m_url);
	m_ServiceProxy = new UploadSubMerchantActionServiceProxy(m_connection);
}


hessianLogi::~hessianLogi()
{
	delete m_connection;
	delete m_ServiceProxy;
}

UploadSubMerchantActionServiceProxy *GetHessiani()
{
	if(m_ServiceProxy != NULL)
		return m_ServiceProxy;
}


#endif

int main(int argc, char* argv[]) {
	cout << "starting" << endl;

	// start a scope to ensure destructors are called on all objects
	{
	

		// hessian proxy, with SSL
//		const string method_name = "sayHello";
//		const string server = "10.211.240.128:18080";		
//		const string url = "http://10.211.240.128:18080/im-logger/helloService";	// note the leading slash
		

//		struct Person psons;

//		hessian::HttpConnection connection(url);
//		UploadSubMerchantActionServiceProxy Hello(connection);
		hessianLogi hess;
		

		try 
		{
			SubMerchantAction info;
			info.merchantID = 911;
			info.subMerchantID = 258;
			info.actionTime = 111;
			info.actionType = 1;

		//	ResultInfo res = Hello.uploadAction(info);
		
		ResultInfo res = hess.GetHessiani()->uploadAction(info);
		
			cout << "success: " << res.success << " : " << res.errorMessage << endl;
		}
	
		catch (std::exception& e) {
			cout << "other exception occured: " << e.what() << endl;
		}
	}
	// object scope ends

	cout << "done" << endl;
	return 0;
}

