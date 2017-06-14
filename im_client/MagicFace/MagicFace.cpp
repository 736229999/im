// MagicFace.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MagicFace.h"
#include "MagicFaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicFaceApp

BEGIN_MESSAGE_MAP(CMagicFaceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMagicFaceApp ����

CMagicFaceApp::CMagicFaceApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMagicFaceApp ����

CMagicFaceApp theApp;


// CMagicFaceApp ��ʼ��

BOOL CMagicFaceApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CString strCmdLine = GetCommandLine();

	int pos1 = strCmdLine.Find("(-");
	int pos2 = strCmdLine.Find("-)");

	if ( pos2>pos1 && pos1>0 )
	{
		g_strMagicFace = strCmdLine.Mid(pos1+2,pos2-pos1-2);
	}

	CMagicFaceDlg dlg(CWnd::GetDesktopWindow());
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
