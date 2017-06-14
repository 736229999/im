#include "stdafx.h"

#ifdef _IM_UNIT_TEST
// DialogTest.cpp : implementation file
//


#include "../IMClient.h"
#include "TestDialog.h"
#include "afxdialogex.h"
#include <string>
#include "ImTest.h"


const char* DEFAULT_TEST_CONFIG = "d:\\test_config.txt";

// CDialogTest dialog

IMPLEMENT_DYNAMIC(CTestDialog, CDialogEx)


CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/, bool auto_test)
	: CDialogEx(CTestDialog::IDD, pParent)
	, message_length_(1000)
	, message_interval_(100)
	, auto_test_(auto_test)
{

}

CTestDialog::~CTestDialog()
{
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LENGTH, message_length_);
	DDV_MinMaxInt(pDX, message_length_, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, message_interval_);
	DDV_MinMaxInt(pDX, message_interval_, 0, 60000);
	DDX_Control(pDX, IDC_LIST_SEND_COUNT, send_cnt_list_);
}

BEGIN_MESSAGE_MAP(CTestDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CTestDialog::OnBnClickedButtonBegin)
	ON_BN_CLICKED(IDC_BUTTON_END, &CTestDialog::OnBnClickedButtonEnd)
	ON_MESSAGE(WM_FINISH_TEST, &CTestDialog::FinishTesting)
	ON_MESSAGE(WM_SEND_COUNT, &CTestDialog::SendCount)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CTestDialog::OnBnClickedButtonConfig)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDialogTest message handlers


BOOL CTestDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_EDIT_CONFIG, DEFAULT_TEST_CONFIG);

	LONG lStyle;
	lStyle = GetWindowLong(send_cnt_list_.m_hWnd, GWL_STYLE); //获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(send_cnt_list_.m_hWnd, GWL_STYLE, lStyle); //设置style

	DWORD dwStyle = send_cnt_list_.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	send_cnt_list_.SetExtendedStyle(dwStyle); //设置扩展风格
	send_cnt_list_.InsertColumn(0, "用户", LVCFMT_LEFT, 100);
	send_cnt_list_.InsertColumn(1, "个数", LVCFMT_LEFT, 100);

	EnableItems();
	
	if (auto_test_)
	{
		AutoTest();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTestDialog::OnBnClickedButtonConfig()
{
	CFileDialog dlg(TRUE);
	dlg.DoModal();
	CString config = dlg.GetPathName();
	if (!config.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_CONFIG, config);
	}
}

void CTestDialog::AutoTest()
{
	IMTEST->StartTest();
	SetTimer(UPDATE_TIMER, 500, nullptr);
	DisableItems();
}

void CTestDialog::OnBnClickedButtonBegin()
{
	if (!UpdateData(TRUE))
	{
		return;
	}

	send_cnt_list_.DeleteAllItems();
	CString config;
	GetDlgItemText(IDC_EDIT_CONFIG, config);

	ImTest::TestArgs ta;
	ta.config = (LPCTSTR)config;
	ta.hwnd = this->m_hWnd;
	ta.interval = message_interval_;
	ta.length = message_length_;
	ta.type = ImTest::P2P;

	if (!IMTEST->StartTest(ta))
	{
		return;
	}

	SetTimer(UPDATE_TIMER, 500, nullptr);

	DisableItems();
}


void CTestDialog::OnBnClickedButtonEnd()
{
	Cleanup();
}

void CTestDialog::EnableItems()
{
	GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_CONFIG)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_INTERVAL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_END)->EnableWindow(FALSE);
}

void CTestDialog::DisableItems()
{
	GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CONFIG)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_INTERVAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BEGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_END)->EnableWindow(TRUE);
}

void CTestDialog::Cleanup()
{
	IMTEST->StopTest();
	KillTimer(UPDATE_TIMER);
	EnableItems();
}

LRESULT CTestDialog::FinishTesting(WPARAM w, LPARAM l)
{
	Cleanup();
	UpdateShowTime();
	CMessageBox::Model(GetSafeHwnd(),_T("已停止"));
	return 0;
}

LRESULT CTestDialog::SendCount(WPARAM w, LPARAM l)
{
	uint32_t cnt = w;
	FRIEND_INFO* p = (FRIEND_INFO*)l;
	CString str;
	str.Format("%d", cnt);
	int id = send_cnt_list_.InsertItem(0, "");
	send_cnt_list_.SetItemText(id, 0, p->username);
	send_cnt_list_.SetItemText(id, 1, str);
	return 0;
}

void CTestDialog::UpdateShowTime()
{
	SetDlgItemInt(IDC_STATIC_TOTAL_TIME, IMTEST->GetRunTime());
	SetDlgItemInt(IDC_STATIC_TOTAL_CNT, IMTEST->GetCount());
}

void CTestDialog::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case UPDATE_TIMER:
	{
		UpdateShowTime();
		break;
	}

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTestDialog::OnClose()
{
	Cleanup();

	CDialogEx::OnClose();
}

#endif