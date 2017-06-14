// IMClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号
#include "im\impublic.h"
#include "im\improtocol.h"
#include "Mini139_src_i.h"


class CMainFrame ;

// CIMClientApp:
// 有关此类的实现，请参阅 IMClient.cpp
//

class CIMClientApp : public CWinApp
{
public:
	CIMClientApp();
	~CIMClientApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:	
	CMainFrame * m_pMainFrame ;

private:
	void InitGolbalGdiObject();
	void InitSpaceObjects();
	void InitVideoDriver();
	bool IsBuyerLogin();   //若买家已经在线，则不启动，并发消息通知其打开相应窗口
	bool IsWorkLogin();   //若工作人员已经在线，则不启动，并发消息通知其打开相应窗口
	bool IsCsLogin();     //若客服已经在线，则不启动，并打开相应窗口
private:
	ULONG_PTR	 m_gdiplusToken;
	
public:
	
	string m_strCustomerName;		// 命令行参数-用户名
	string m_strSubMerchantName;	// 商家名字
	uint32 m_nMerchantID;           // 商家ID
	uint32 m_nSubMerchantID;        //子帐号ID
	uint32 m_nProductID;			// 商品ID

	uint32 m_nMonitorID;            //被监控的客服ID
	uint32 m_nServiceID;            //客服ID
	uint32 m_nCustomerID;           //客人ID
};

extern CIMClientApp theApp;
