// DlgSCreenCtrlTool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSCreenCtrlTool.h"
#include "afxdialogex.h"
#include "ScreenCtrl.h"
#include "DlgSCreenCtrlColor.h"
// CDlgSCreenCtrlTool �Ի���

IMPLEMENT_DYNAMIC(CDlgSCreenCtrlTool, CDialog)

	CDlgSCreenCtrlTool::CDlgSCreenCtrlTool(CWnd* pParent /*=NULL*/, CScreenCtrl* pScreenCtrl)
	: CDialog(CDlgSCreenCtrlTool::IDD, pParent)
{
	m_pScreenCtrl = pScreenCtrl;
	m_pDlgSCreenCtrlColor = NULL;
}

CDlgSCreenCtrlTool::~CDlgSCreenCtrlTool()
{
	if(m_pDlgSCreenCtrlColor != NULL)
	{
		delete m_pDlgSCreenCtrlColor;
		m_pDlgSCreenCtrlColor = NULL;
	}
}

void CDlgSCreenCtrlTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RECTANGLE, m_btnRectangle);
	DDX_Control(pDX, IDC_BTN_ELLIPSE, m_btnEllipse);
	DDX_Control(pDX, IDC_BTN_ARROW, m_btnArrow);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_DONE, m_btnDone);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_REVOKE, m_btnRevoke);
}


BEGIN_MESSAGE_MAP(CDlgSCreenCtrlTool, CDialog)
	ON_BN_CLICKED(IDC_BTN_RECTANGLE, &CDlgSCreenCtrlTool::OnBnClickedBtnRectangle)
	ON_BN_CLICKED(IDC_BTN_ELLIPSE, &CDlgSCreenCtrlTool::OnBnClickedBtnEllipse)
	ON_BN_CLICKED(IDC_BTN_ARROW, &CDlgSCreenCtrlTool::OnBnClickedBtnArrow)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgSCreenCtrlTool::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_DONE, &CDlgSCreenCtrlTool::OnBnClickedBtnDone)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgSCreenCtrlTool::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CDlgSCreenCtrlTool::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_REVOKE, &CDlgSCreenCtrlTool::OnBnClickedBtnRevoke)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CDlgSCreenCtrlTool ��Ϣ�������


BOOL CDlgSCreenCtrlTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(m_pDlgSCreenCtrlColor == NULL)
	{
		m_pDlgSCreenCtrlColor = new CDlgSCreenCtrlColor(this, m_pScreenCtrl);
		m_pDlgSCreenCtrlColor->Create(CDlgSCreenCtrlColor::IDD, this);
	}

	m_btnRectangle.SetPng(IDB_PNG_RECTANGLE_HOVER,IDB_PNG_RECTANGLE_NORMAL,IDB_PNG_RECTANGLE_PRESSED,IDB_PNG_RECTANGLE_NORMAL);
	m_btnRectangle.SetToolTip("����");

 	m_btnEllipse.SetPng(IDB_PNG_ELLIPSE_HOVER,IDB_PNG_ELLIPSE_NORMAL,IDB_PNG_ELLIPSE_PRESSED,IDB_PNG_ELLIPSE_NORMAL);
 	m_btnEllipse.SetToolTip("��Բ");
 
 	m_btnArrow.SetPng(IDB_PNG_ARROW_HOVER,IDB_PNG_ARROW_NORMAL,IDB_PNG_ARROW_PRESSED,IDB_PNG_ARROW_NORMAL);
 	m_btnArrow.SetToolTip("��ͷ");

	m_btnEdit.SetPng(IDB_PNG_SCREEN_FONT_HOVER,IDB_PNG_SCREEN_FONT_NORMAL,IDB_PNG_SCREEN_FONT_PRESSED,IDB_PNG_SCREEN_FONT_NORMAL);
	m_btnEdit.SetToolTip("���ֱ༭");

	m_btnRevoke.SetPng(IDB_PNG_SCREEN_UNDO_HOVER,IDB_PNG_SCREEN_UNDO_NORMAL,IDB_PNG_SCREEN_UNDO_PRESSED,IDB_PNG_SCREEN_UNDO_NORMAL);
	m_btnRevoke.SetToolTip("�����༭");

 	m_btnSave.SetPng(IDB_PNG_SAVE_HOVER,IDB_PNG_SAVE_NORMAL,IDB_PNG_SAVE_PRESSED,IDB_PNG_SAVE_NORMAL);
 	m_btnSave.SetToolTip("����ͼƬ");
 
	m_btnClose.SetPng(IDB_PNG_SCREEN_CLOSE_HOVER,IDB_PNG_SCREEN_CLOSE_NORMAL,IDB_PNG_SCREEN_CLOSE_PRESSED,IDB_PNG_SCREEN_CLOSE_NORMAL);
	m_btnClose.SetToolTip("�˳���ͼ");

	m_btnDone.SetPng(IDB_PNG_DONE_HOVER,IDB_PNG_DONE_NORMAL,IDB_PNG_DONE_PRESSED,IDB_PNG_DONE_NORMAL);
 	m_btnDone.SetToolTip("���");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgSCreenCtrlTool::OnBnClickedBtnRectangle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ƾ���ģʽ
	m_pScreenCtrl->m_bIsDrawEllipse = FALSE;
	m_pScreenCtrl->m_bIsDrawArrow = FALSE;
	m_pScreenCtrl->m_bIsDrawRectangle = TRUE;
	m_pScreenCtrl->m_bIsDrawEdit = FALSE;
	SetAllBtnUp();
	m_btnRectangle.SetDown(TRUE);
	ShowScreenColor();
}


void CDlgSCreenCtrlTool::OnBnClickedBtnEllipse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����Բ��ģʽ
	m_pScreenCtrl->m_bIsDrawRectangle = FALSE;
	m_pScreenCtrl->m_bIsDrawArrow = FALSE;
	m_pScreenCtrl->m_bIsDrawEllipse = TRUE;
	m_pScreenCtrl->m_bIsDrawEdit = FALSE;
	SetAllBtnUp();
	m_btnEllipse.SetDown(TRUE);
	ShowScreenColor();
}


void CDlgSCreenCtrlTool::OnBnClickedBtnArrow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���Ƽ�ͷģʽ
	m_pScreenCtrl->m_bIsDrawEllipse = FALSE;
	m_pScreenCtrl->m_bIsDrawRectangle = FALSE;
	m_pScreenCtrl->m_bIsDrawArrow = TRUE;
	m_pScreenCtrl->m_bIsDrawEdit = FALSE;
	SetAllBtnUp();
	m_btnArrow.SetDown(TRUE);
	ShowScreenColor();
}

void CDlgSCreenCtrlTool::OnBnClickedBtnEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ֱ༭ģʽ
	m_pScreenCtrl->m_bIsDrawEllipse = FALSE;
	m_pScreenCtrl->m_bIsDrawRectangle = FALSE;
	m_pScreenCtrl->m_bIsDrawArrow = FALSE;
	m_pScreenCtrl->m_bIsDrawEdit = TRUE;
	SetAllBtnUp();
	m_btnEdit.SetDown(TRUE);
	//ShowScreenColor();
	m_pDlgSCreenCtrlColor->ShowWindow(SW_HIDE);
}

void CDlgSCreenCtrlTool::SetAllBtnUp()
{
	//�������а�ţ̌��
	m_btnRectangle.SetDown(FALSE);
	m_btnEllipse.SetDown(FALSE);
	m_btnArrow.SetDown(FALSE);
	m_btnEdit.SetDown(FALSE);
}

void CDlgSCreenCtrlTool::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ͼƬ
	SetAllBtnUp();
	CString sFilter = "��ͼ(*.jpg)|*.jpg||";
	CFileDialog fileDlg(FALSE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, 
		sFilter,
		this);
	fileDlg.m_ofn.lpstrTitle = "����";
	char szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));
	GetCurrentDirectory(sizeof(szBuf), szBuf);
	fileDlg.m_ofn.lpstrInitialDir = szBuf;	
	if(IDOK==fileDlg.DoModal())
	{
		CString strRecordInfo;
		CString strFilePath = fileDlg.GetPathName();
		CString strFileName = fileDlg.GetFileName();
		//��������ɱ���ͼƬ
		m_pScreenCtrl->SavePic(strFilePath);

		CDialog::OnCancel();
	}
}


void CDlgSCreenCtrlTool::OnBnClickedBtnDone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ɽ������رմ���
	m_pDlgSCreenCtrlColor->ShowWindow(SW_HIDE);
	SetAllBtnUp();
	m_pScreenCtrl->DonePic();
}


void CDlgSCreenCtrlTool::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ȡ���������رմ���
	m_pDlgSCreenCtrlColor->ShowWindow(SW_HIDE);
	SetAllBtnUp();
	m_pScreenCtrl->m_bIsDrawEllipse = FALSE;
	m_pScreenCtrl->m_bIsDrawRectangle = FALSE;
	m_pScreenCtrl->m_bIsDrawArrow = FALSE;
	m_pScreenCtrl->m_bIsDrawEdit = FALSE;
	SendMessage(WM_CLOSE);
	m_pScreenCtrl->SendMessage(WM_CLOSE);
}


BOOL CDlgSCreenCtrlTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://ESC����Ӧ�����ڹرմ���
			OnBnClickedBtnClose();
			break;
		case VK_RETURN:
			OnBnClickedBtnDone();//�س�����Ӧ����ɽ���
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSCreenCtrlTool::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	//���ʹ��߸���༭����
	if(m_pDlgSCreenCtrlColor != NULL && m_pDlgSCreenCtrlColor->IsWindowVisible())
	{
		CRect rc;
		GetWindowRect(&rc);
		CRect rcScreenColor;
		m_pDlgSCreenCtrlColor->GetWindowRect(rcScreenColor);
		m_pDlgSCreenCtrlColor->MoveWindow(rc.left, rc.bottom+3, rcScreenColor.Width(), rcScreenColor.Height());
	}
	// TODO: �ڴ˴������Ϣ����������
}

void CDlgSCreenCtrlTool::ShowScreenColor()
{
	//��ʾ���ʹ���
	if(m_pDlgSCreenCtrlColor != NULL)
	{
		m_pDlgSCreenCtrlColor->ShowWindow(SW_SHOW);
		CRect rc;
		GetWindowRect(&rc);
		CRect rcScreenColor;
		m_pDlgSCreenCtrlColor->GetWindowRect(rcScreenColor);
		m_pDlgSCreenCtrlColor->MoveWindow(rc.left, rc.bottom+3, rcScreenColor.Width(), rcScreenColor.Height());
	}
}

void CDlgSCreenCtrlTool::OnBnClickedBtnRevoke()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pScreenCtrl->Revoke();//�����༭
}
