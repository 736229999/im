// minidata_dll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "minidata_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// Cminidata_dllApp

BEGIN_MESSAGE_MAP(Cminidata_dllApp, CWinApp)
END_MESSAGE_MAP()


// Cminidata_dllApp ����

Cminidata_dllApp::Cminidata_dllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cminidata_dllApp ����

Cminidata_dllApp theApp;


// Cminidata_dllApp ��ʼ��

BOOL Cminidata_dllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
