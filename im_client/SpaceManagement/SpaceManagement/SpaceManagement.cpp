// SpaceManagement.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SpaceManagement.h"
#include "SpaceManagementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpaceManagementApp

BEGIN_MESSAGE_MAP(CSpaceManagementApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSpaceManagementApp ����

CSpaceManagementApp::CSpaceManagementApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSpaceManagementApp ����

CSpaceManagementApp theApp;


// CSpaceManagementApp ��ʼ��

BOOL CSpaceManagementApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	::CoInitialize(0);

	CSpaceManagementDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}


	::CoUninitialize();

	return FALSE;
}
