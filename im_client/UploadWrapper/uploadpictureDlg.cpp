// uploadpictureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "uploadpicture.h"
#include "uploadpictureDlg.h"
#include ".\uploadpicturedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GetSectionValue(const CString &strInfo, CString str1,CString str2, CString &strRet)
{
	int		iPos1 , iPos2, iLen;

	iPos1 = strInfo.Find(str1);
	iPos2 = strInfo.Find(str2);

	iLen  = str1.GetLength();

	if ( iPos2>iPos1 && iPos1>-1 )
	{
		strRet = strInfo.Mid(iPos1+iLen,iPos2-iPos1-iLen);
	}
	else
	{
		strRet = "";
	}
}

CuploadpictureDlg::CuploadpictureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CuploadpictureDlg::IDD, pParent)
{
	m_pUpload = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuploadpictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CuploadpictureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CuploadpictureDlg ��Ϣ�������

BOOL CuploadpictureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	::ShowWindow(m_hWnd,SW_HIDE);

	m_pUpload.CreateInstance(CLSID_HmgUploadPictureX);

	if(g_bShellExt == TRUE)
	{
		SetWindowText("uploadpicturex.v2");
		m_pUpload->UploadPicShellExt(g_strParam.AllocSysString());
	}
	else
	{
		CString strUserName;
		GetSectionValue(g_strParam,"<username>","</username>",strUserName);

		CString strTitle;
		strTitle.Format("wokuuploadpicture_platform:%s",strUserName);

		SetWindowText(strTitle);

		m_pUpload->ShowUploadDlg(g_strParam.AllocSysString());
	}

	
	m_pUpload.Release();
	m_pUpload = NULL;

	PostQuitMessage(0);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CuploadpictureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CuploadpictureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CuploadpictureDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (nState==WA_ACTIVE)
	{
		if ( m_pUpload )
			m_pUpload->BringToTop();
	}
}

void CuploadpictureDlg::OnClose()
{
	CDialog::OnClose();
}
