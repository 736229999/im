// E:\hechang\MiniApollo\Mini_Client\Mini_Client\MINI_CLIENT\Mini139_Src\Im\DialogShowHelp.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "Mini139_src.h"
#include "DialogShowHelp.h"
#include ".\dialogshowhelp.h"
#include "LocalData.h"
#include "..\IMClient.h"

extern CIMClientApp theApp;
// CDialogShowHelp �Ի���

CDialogShowHelp* CDialogShowHelp::m_pInstance = NULL;
IMPLEMENT_DYNAMIC(CDialogShowHelp, CXSkinDlg)
CDialogShowHelp::CDialogShowHelp(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CDialogShowHelp::IDD, pParent)
{
	m_pInstance = this;

}

CDialogShowHelp::~CDialogShowHelp()
{

}

void CDialogShowHelp::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_HELP, m_ieHelp);
	DDX_Control(pDX, IDC_CHECK_NEED_RUN, m_wndCheckNeedRun);
}


BEGIN_MESSAGE_MAP(CDialogShowHelp, CXSkinDlg)
	ON_WM_GETMINMAXINFO()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_NEED_RUN, OnBnClickedCheckNeedRun)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogShowHelp ��Ϣ�������

void CDialogShowHelp::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI->ptMinTrackSize.x = 585;
	lpMMI->ptMinTrackSize.y = 385;


	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

BOOL CDialogShowHelp::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	SetWindowText("������ʾ");
	
	CString strHelpPath;
	strHelpPath =  GetModulePath();
	strHelpPath += "MiniDemo.swf";
	m_ieHelp.Navigate(strHelpPath,NULL,NULL,NULL,NULL);

	m_wndCheckNeedRun.SetCheck(BST_CHECKED);


	CRect rtMain;
	GetWindowRect(rtMain);
	//ClientToScreen(rtMain);
	//m_ieHelp.ModifyStyle(WS_BORDER ,NULL );

	rtMain.right = rtMain.left + 585;
	rtMain.bottom= rtMain.top  + 385 + 50;
	MoveWindow(rtMain);

	CenterWindow();
		

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CDialogShowHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if( CTLCOLOR_STATIC == nCtlColor )
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);//ʹ����͸��
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CDialogShowHelp::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CXSkinDlg::PostNcDestroy();


}

void CDialogShowHelp::Show()
{
	if( m_pInstance )
	{
		//delete m_pInstance;
		m_pInstance->ShowWindow(SW_SHOW);
		m_pInstance->BringWindowToTop();
	}
	else
	{
		m_pInstance = new CDialogShowHelp;
		m_pInstance->Create(CDialogShowHelp::IDD,NULL);
		m_pInstance->ShowWindow(SW_SHOW);
	}
}

void CDialogShowHelp::Exit()
{
	if( m_pInstance )
	{

		m_pInstance->DestroyWindow();
	//	delete m_pInstance;
	//	m_pInstance = NULL;
	}
}

void CDialogShowHelp::OnBnClickedCheckNeedRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if( BST_CHECKED == m_wndCheckNeedRun.GetCheck())
	{
		g_LocalData.SetShowHelpDemo(true);
	}
	else
	{
		g_LocalData.SetShowHelpDemo(false);
	}
}


void CDialogShowHelp::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();

	// TODO: �ڴ˴������Ϣ����������
	Exit();
}

void CDialogShowHelp::OnDestroy()
{
	//this->m_ieHelp.DestroyWindow();
	CXSkinDlg::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

}

void CDialogShowHelp::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rtCheck;
	

	if( m_wndCheckNeedRun.GetSafeHwnd())
	{
		m_wndCheckNeedRun.GetWindowRect(rtCheck);
		m_wndCheckNeedRun.MoveWindow(16,cy-rtCheck.Height()-8,rtCheck.Width(),rtCheck.Height());
		
		if( m_ieHelp.GetSafeHwnd() )
		{
			int nTop = 24;
			m_ieHelp.MoveWindow(0,nTop,cx,cy - nTop - (rtCheck.Height()+16));
		}
	}


}
