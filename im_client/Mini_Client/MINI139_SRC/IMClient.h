// IMClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
#include "im\impublic.h"
#include "im\improtocol.h"
#include "Mini139_src_i.h"


class CMainFrame ;

// CIMClientApp:
// �йش����ʵ�֣������ IMClient.cpp
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
	bool IsBuyerLogin();   //������Ѿ����ߣ���������������Ϣ֪ͨ�����Ӧ����
	bool IsWorkLogin();   //��������Ա�Ѿ����ߣ���������������Ϣ֪ͨ�����Ӧ����
	bool IsCsLogin();     //���ͷ��Ѿ����ߣ���������������Ӧ����
private:
	ULONG_PTR	 m_gdiplusToken;
	
public:
	
	string m_strCustomerName;		// �����в���-�û���
	string m_strSubMerchantName;	// �̼�����
	uint32 m_nMerchantID;           // �̼�ID
	uint32 m_nSubMerchantID;        //���ʺ�ID
	uint32 m_nProductID;			// ��ƷID

	uint32 m_nMonitorID;            //����صĿͷ�ID
	uint32 m_nServiceID;            //�ͷ�ID
	uint32 m_nCustomerID;           //����ID
};

extern CIMClientApp theApp;
