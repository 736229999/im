// PanelBaseDlg.cpp : implementation file
//
#include "..\stdafx.h"
#include "..\HmgUploadPicture.h"
#include "PanelBaseDlg.h"

#include "UpLoadPhotoDlg.h"

#include <afxpriv.h>

// CPanelBaseDlg dialog
IMPLEMENT_DYNAMIC(CPanelBaseDlg, CDialog)

CPanelBaseDlg::CPanelBaseDlg(UINT IDD, CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{

}

CPanelBaseDlg::~CPanelBaseDlg()
{
}

void CPanelBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPanelBaseDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

/////

IMPLEMENT_DYNAMIC(CPanelDlg1, CPanelBaseDlg)

CPanelDlg1::CPanelDlg1(CWnd* pParent /*=NULL*/)
: CPanelBaseDlg(CPanelDlg1::IDD, pParent)
{
	m_pParentWnd = NULL;
	m_bEnableAlumTips = FALSE;
}

CPanelDlg1::~CPanelDlg1()
{
}

void CPanelDlg1::DoDataExchange(CDataExchange* pDX)
{
	CPanelBaseDlg::DoDataExchange(pDX);

//	DDX_Control(pDX, IDC_DLG_UPLOAD_ALBUM, m_wndEditAlbum);
	DDX_Control(pDX, IDC_DLG_UPLOAD_ALBUM_COMBO, m_wndComboAlbum);

	//	DDX_Control(pDX, IDC_DLG_UPLOAD_TAG, m_wndEditTag);
	DDX_Control(pDX, IDC_DLG_UPLOAD_TAG_COMBO, m_wndComboTAG);

	// 访问权限
	DDX_Control(pDX, IDC_DLG_UPLOAD_ACL, m_wndComboACL);

	// 限制图像的最长边
	DDX_Control(pDX, IDC_DLG_UPLOAD_IMAGE_SIZE, m_wndComboLimitSize);
}


BEGIN_MESSAGE_MAP(CPanelDlg1, CPanelBaseDlg)
	ON_CBN_KILLFOCUS(IDC_DLG_UPLOAD_TAG_COMBO, OnCbnKillfocusDlgUploadTagCombo)
	ON_CBN_SETFOCUS(IDC_DLG_UPLOAD_TAG_COMBO, OnCbnSetfocusDlgUploadTagCombo)
	ON_CBN_KILLFOCUS(IDC_DLG_UPLOAD_ALBUM_COMBO, OnCbnKillfocusDlgUploadAlbumCombo)
	ON_CBN_SETFOCUS(IDC_DLG_UPLOAD_ALBUM_COMBO, OnCbnSetfocusDlgUploadAlbumCombo)
	ON_CBN_SELENDOK(IDC_DLG_UPLOAD_IMAGE_SIZE, OnCbnSelendokDlgUploadImageSize)
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(CPanelDlg2, CPanelBaseDlg)

CPanelDlg2::CPanelDlg2(CWnd* pParent /*=NULL*/)
: CPanelBaseDlg(CPanelDlg2::IDD, pParent)
{

}

CPanelDlg2::~CPanelDlg2()
{
}

void CPanelDlg2::DoDataExchange(CDataExchange* pDX)
{
	CPanelBaseDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DLG_UPLOAD_BTN_STARTSEND, m_BeginUpLoadButton);
	DDX_Control(pDX, IDC_DLG_UPLOAD_BTN_RESETINFO, m_wndResetInfo);

	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_INFO, m_wndBmpBtnINFO);
	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_HYPERLINK, m_wndStaticHyperLink);
}


BEGIN_MESSAGE_MAP(CPanelDlg2, CPanelBaseDlg)

	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPanelBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_br.CreateSolidBrush(RGB(0xff, 0xff, 0xff));

	return TRUE;
}

BOOL CPanelDlg1::OnInitDialog()
{
	CPanelBaseDlg::OnInitDialog();

//	m_wndComboAlbum.EnableAutoCompletion();

	OnCbnKillfocusDlgUploadTagCombo();
	OnCbnKillfocusDlgUploadAlbumCombo();
	
	return TRUE;
}

BOOL CPanelDlg2::OnInitDialog()
{
	CPanelBaseDlg::OnInitDialog();

	return TRUE;
}

BOOL CPanelBaseDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CRgn rgn;
	rgn.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);

	GetRepaintRgn(rgn);

	pDC->SelectObject(&rgn);
//	pDC->FillSolidRect(&rect, RGB(255, 255, 255)); // RGB(203,235,248));

	
	
	pDC ->FillRgn(&rgn, &CBrush(RGB(255, 255, 255)));
	return FALSE;

//	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CPanelBaseDlg::PreTranslateMessage(MSG* pMsg)
{


	return CDialog::PreTranslateMessage(pMsg);
}


#include "UpLoadPhotoDlg.h"
#include "Enablewindow/EnableWindow.h"

// CUploadOperationsDlg dialog

IMPLEMENT_DYNAMIC(CUploadOperationsDlg, CPanelBaseDlg)

CUploadOperationsDlg::CUploadOperationsDlg(CWnd* pParent /*=NULL*/)
: CPanelBaseDlg(CUploadOperationsDlg::IDD, pParent)
{
	m_pParent = NULL;
}

CUploadOperationsDlg::~CUploadOperationsDlg()
{
}

void CUploadOperationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPanelBaseDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ADDUPLOAD_BUTTON, m_AddButton);
	DDX_Control(pDX, IDC_ADDALLUPLOADBUTTON, m_AddAllButton);
	DDX_Control(pDX, IDC_REMOVEUPLOADFILE_BUTTON, m_RemoveButton);
	DDX_Control(pDX, IDC_REMOVEALLUPLOADFILES_BUTTON, m_RemoveAllButton);

//	DDX_Control(pDX, IDC_DLG_UPLOAD_PROGRESSCTRL, m_wndProgressCtrl);
//	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_REMAINSIZE_1, m_wndStaticPrompt1);
//	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_REMAINSIZE_2, m_wndStaticPrompt2);


	DDX_Text(pDX, IDC_DLG_UPLOAD_STATIC_SELECTFILEINFO, m_strSelectFileINFO);

	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_SELECTFILEINFO, m_wndStaticPromptSelectInfo);
	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_SELECTFILEINFO2, m_wndStaticPromptSelectInfo2);
}


BEGIN_MESSAGE_MAP(CUploadOperationsDlg, CPanelBaseDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_ADDUPLOAD_BUTTON, OnBtnAdd)
	ON_BN_CLICKED(IDC_ADDALLUPLOADBUTTON, OnBtnAddAll)
	ON_BN_CLICKED(IDC_REMOVEUPLOADFILE_BUTTON, OnBtnRemove)
	ON_BN_CLICKED(IDC_REMOVEALLUPLOADFILES_BUTTON, OnBtnRemoveAll)

	ON_WM_SIZE()

	ON_UPDATE_COMMAND_UI(IDC_ADDUPLOAD_BUTTON, OnDlgUpdateUI)
	ON_UPDATE_COMMAND_UI(IDC_ADDALLUPLOADBUTTON, OnDlgUpdateUI)
	ON_UPDATE_COMMAND_UI(IDC_REMOVEUPLOADFILE_BUTTON, OnDlgUpdateUI)
	ON_UPDATE_COMMAND_UI(IDC_REMOVEALLUPLOADFILES_BUTTON, OnDlgUpdateUI)

	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUploadOperationsDlg message handlers

BOOL CUploadOperationsDlg::OnInitDialog()
{
	CPanelBaseDlg::OnInitDialog();

#if 0
	this->m_AddButton.SetFlatStyle(CFlatButton::FS_OFFICE);
	this->m_RemoveAllButton.SetFlatStyle(CFlatButton::FS_OFFICE);
	this->m_AddAllButton.SetFlatStyle(CFlatButton::FS_OFFICE);
	this->m_RemoveButton.SetFlatStyle(CFlatButton::FS_OFFICE);
#endif
//	m_wndProgressCtrl.Init();

	m_br.CreateSolidBrush(RGB(203,235,248));

//	m_wndStaticPrompt1.SetBkColor(RGB(0xFF,0xFF,0xFF)).SetFontBold(TRUE);//.SetBkColor(IM_SPACE_COLOR);
//	m_wndStaticPrompt2.SetBkColor(RGB(0xFF,0xFF,0xFF)).SetFontBold(TRUE);//.SetBkColor(IM_SPACE_COLOR);

	m_wndStaticPromptSelectInfo.SetBkColor(RGB(0xFF,0xFF,0xFF)).SetFontBold(TRUE);
	m_wndStaticPromptSelectInfo2.SetBkColor(RGB(0xFF,0xFF,0xFF)).SetFontBold(TRUE);

	return TRUE;
}


void CUploadOperationsDlg::SetSelectFileINFO(const CString & strText, const CString strText1)
{
	//	m_strSelectFileINFO = "";
	//	UpdateData(FALSE);

	m_strSelectFileINFO = strText;
	//	UpdateData(FALSE);

	CWnd *pWnd = GetDlgItem(IDC_DLG_UPLOAD_STATIC_SELECTFILEINFO);
	if(pWnd != NULL)
	{
		pWnd ->SetWindowText(strText);
#if 0
		CRect rc;
		pWnd ->GetWindowRect(&rc);

		pWnd ->ShowWindow(SW_HIDE);
		pWnd ->ShowWindow(SW_SHOW);
		ScreenToClient(&rc);
		InvalidateRect(&rc);
#endif
	}

	pWnd = GetDlgItem(IDC_DLG_UPLOAD_STATIC_SELECTFILEINFO2);
	if(pWnd != NULL)
	{
		pWnd ->SetWindowText(strText1);	m_strText2 = strText1;
	}

}
BOOL CUploadOperationsDlg::OnEraseBkgnd(CDC* pDC)
{
	//	pDC ->SetBkMode(TRANSPARENT);

//	CRect r;
//	GetClientRect(&r);
//	pDC->FillSolidRect(&r, RGB(255, 255, 255)); // RGB(203,235,248));

//	return TRUE;

	return CPanelBaseDlg::OnEraseBkgnd(pDC);
}

HBRUSH CUploadOperationsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPanelBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	//	if(nCtlColor == CTLCOLOR_STATIC)
	//	{
	//		pDC->SetBkMode(TRANSPARENT);
	//		pDC ->SetBkColor(RGB(255, 255, 255));
	//	}

	//	pDC->SetBkColor(RGB(0xFF,0xFF,0xFF));	
	//	return (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}

void CUploadOperationsDlg::OnBtnAdd()
{
	//	_EnableWindow _Enable(IDC_ADDUPLOAD_BUTTON, this);
	if(m_pParent != NULL && ::IsWindow(m_pParent ->GetSafeHwnd()))	m_pParent ->OnBnClickedAdduploadButton();
}

void CUploadOperationsDlg::OnBtnAddAll()
{
	//	_EnableWindow _Enable(IDC_REMOVEALLUPLOADFILES_BUTTON, this);
	if(m_pParent != NULL && ::IsWindow(m_pParent ->GetSafeHwnd()))	 m_pParent ->OnBnClickedAddalluploadbutton();
}

void CUploadOperationsDlg::OnBtnRemove()
{
	//	_EnableWindow _Enable(IDC_REMOVEUPLOADFILE_BUTTON, this);
	if(m_pParent != NULL && ::IsWindow(m_pParent ->GetSafeHwnd())) m_pParent ->OnBnClickedRemoveuploadfileButton();
}

void CUploadOperationsDlg::OnBtnRemoveAll()
{
	//	_EnableWindow _Enable(IDC_REMOVEALLUPLOADFILES_BUTTON, this);
	if(m_pParent != NULL && ::IsWindow(m_pParent ->GetSafeHwnd())) m_pParent ->OnBnClickedRemovealluploadfilesButton();
}
HBRUSH CPanelBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC && pWnd ->GetDlgCtrlID() != IDC_DLG_UPLOAD_STATIC_HYPERLINK)
	{
		pDC ->SetBkMode(TRANSPARENT);
		pDC ->SetTextColor(RGB(0, 0, 0));


		return (HBRUSH) m_br.GetSafeHandle();
	}

	return hbr;
}

LRESULT CPanelBaseDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_COMMAND )// && g_bEnableContinueOperation == TRUE)
	{
		GetParent() ->PostMessage( message, wParam, lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


void CPanelDlg1::OnCbnKillfocusDlgUploadTagCombo()
{
	CString s ;
	m_wndComboTAG.GetWindowText(s);

	s.Trim();

	if ( s == "" )
	{
		m_wndComboTAG.SetTextColor(0xAAAAAA);
		m_wndComboTAG.SetWindowText("<请在这里输入TAG,用逗号\",\"隔开,最多10个>");
	}
}

void CPanelDlg1::OnCbnSetfocusDlgUploadTagCombo()
{
	CString s ;
	m_wndComboTAG.GetWindowText(s);

	s.Trim();

	m_wndComboTAG.SetTextColor(0);

	if ( s=="<请在这里输入TAG,用逗号\",\"隔开,最多10个>" )
	{
		m_wndComboTAG.SetWindowText("");
	}
}

void CPanelDlg1::OnCbnKillfocusDlgUploadAlbumCombo()
{
// 	if(m_bEnableAlumTips == FALSE)
// 	{
// 		m_wndComboAlbum.SetTextColor(RGB(0, 0, 0));
// 		return ;
// 	}	

	CString str ;
	m_wndComboAlbum.GetWindowText(str);

	str.Trim();

	if (str.IsEmpty())
	{
		m_wndComboAlbum.SetTextColor(0xAAAAAA);
		m_wndComboAlbum.SetWindowText("<请在这里输入或者选择图集>");
	}

}

void CPanelDlg1::OnCbnSetfocusDlgUploadAlbumCombo()
{	
// 	if(m_bEnableAlumTips == FALSE)
// 	{
// 		m_wndComboAlbum.SetTextColor(0);
// 		return ;
// 	}

	CString str ;
	m_wndComboAlbum.GetWindowText(str);

	str.Trim();

	m_wndComboAlbum.SetTextColor(0);
	if (str =="<请在这里输入或者选择图集>" )
	{
		m_wndComboAlbum.SetWindowText("");
	}
}


void CPanelDlg2::OnMouseMove(UINT nFlags, CPoint point)
{

	CPanelBaseDlg::OnMouseMove(nFlags, point);
}

void CPanelBaseDlg::GetRepaintRgn(CRgn &rgn)
{

}

void CPanelDlg1::GetRepaintRgn(CRgn &rgn)
{
	RemoveRgnOfWnd(rgn, &m_wndComboAlbum);
	RemoveRgnOfWnd(rgn, &m_wndComboTAG);
	RemoveRgnOfWnd(rgn, &m_wndComboLimitSize);
	RemoveRgnOfWnd(rgn, &m_wndComboACL);
}

void CPanelDlg2::GetRepaintRgn(CRgn &rgn)
{
	RemoveRgnOfWnd(rgn, &m_wndBmpBtnINFO);
//	RemoveRgnOfWnd(rgn, &m_BeginUpLoadButton);

//	RemoveRgnOfWnd(rgn, &m_wndResetInfo);
	RemoveRgnOfWnd(rgn, &m_wndStaticHyperLink);
}

void CUploadOperationsDlg::GetRepaintRgn(CRgn &rgn)
{
/*	RemoveRgnOfWnd(rgn, &m_AddAllButton);
	RemoveRgnOfWnd(rgn, &m_AddButton);

	RemoveRgnOfWnd(rgn, &m_RemoveAllButton);
	RemoveRgnOfWnd(rgn, &m_RemoveButton)*/;
}


void CPanelBaseDlg::RemoveRgnOfWnd(CRgn &rgn, CWnd *pWnd,int iOffset)
{
	if ( pWnd && pWnd->m_hWnd )
	{
		CRgn rgnWnd;
		CRect rectWnd;
		pWnd->GetWindowRect(&rectWnd);
		ScreenToClient(&rectWnd);

		rectWnd.InflateRect(iOffset,iOffset);
		rgnWnd.CreateRectRgn(rectWnd.left,rectWnd.top,rectWnd.right,rectWnd.bottom);

		rgn.CombineRgn( &rgn, &rgnWnd, RGN_DIFF);
	}
}
void CPanelDlg1::OnCbnSelendokDlgUploadImageSize()
{
	CString strText;
	m_wndComboLimitSize.GetLBText(m_wndComboLimitSize.GetCurSel(), strText);
	const int nMaxSideSize = atoi(strText);

	m_wndComboLimitSize.EnableWindow(FALSE);
	if(m_pParentWnd != NULL && m_pParentWnd ->GetSafeHwnd())
	{
		m_pParentWnd ->RedrawUploadFileListPics(nMaxSideSize);
	}
	m_wndComboLimitSize.EnableWindow(TRUE);
}

void CPanelDlg2::OnSize(UINT nType, int cx, int cy)
{
	CPanelBaseDlg::OnSize(nType, cx, cy);
	
	if(m_wndStaticHyperLink.GetSafeHwnd() && ::IsWindow(m_wndStaticHyperLink.GetSafeHwnd()) 
		&& m_wndBmpBtnINFO.GetSafeHwnd() && ::IsWindow(m_wndBmpBtnINFO.GetSafeHwnd()))
	{
		CRect rcLink;
		m_wndStaticHyperLink.GetWindowRect(&rcLink);	ScreenToClient(&rcLink);

		CRect rcBmpBtn;	
		m_wndBmpBtnINFO.GetWindowRect(&rcBmpBtn);		ScreenToClient(&rcBmpBtn);

		CRect rc;

		rc.top = rcLink.top;
		rc.left = rcBmpBtn.left + 5;

		rc.right = rc.left + rcBmpBtn.Width();
		rc.bottom = rc.top + rcBmpBtn.Height();

		m_wndBmpBtnINFO.MoveWindow(&rc);
	}
}

void CUploadOperationsDlg::OnSize(UINT nType, int cx, int cy)
{
	CPanelBaseDlg::OnSize(nType, cx, cy);
	
	CRect rcDlgClient;
	GetClientRect(&rcDlgClient);

	if(m_wndStaticPromptSelectInfo2.GetSafeHwnd() && ::IsWindow(m_wndStaticPromptSelectInfo2.GetSafeHwnd()))
	{

		CRect rcTmp;
		m_wndStaticPromptSelectInfo2.GetWindowRect(&rcTmp);
		ScreenToClient(&rcTmp);

		int x = rcTmp.left;
		int y = rcTmp.top;

		int cx = rcDlgClient.Width() - rcTmp.left - 5;
		int cy = rcTmp.Height();

		x = rcDlgClient.right - cx;
		
	//	m_wndStaticPromptSelectInfo2.MoveWindow(x, y, cx, cy);
		m_wndStaticPromptSelectInfo2.SetWindowPos(NULL, 0, 0, cx, rcTmp.Height(),
			SWP_NOMOVE);
		m_wndStaticPromptSelectInfo2.SetWindowText(m_strText2);
	}
}

BOOL CUploadOperationsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CPanelBaseDlg::PreTranslateMessage(pMsg);
}


void CPanelBaseDlg::OnKickIdle()
{
	//UpdateDialogControls(this, FALSE);
	//SendMessageToDescendants(WM_IDLEUPDATECMDUI);
}

void CUploadOperationsDlg::OnDlgUpdateUI(CCmdUI *pCmdUI)
{
#if 1
	pCmdUI ->Enable(g_bEnableContinueOperation);
#endif
}
void CUploadOperationsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		
	}

	CPanelBaseDlg::OnTimer(nIDEvent);
}

void CUploadOperationsDlg::OnDestroy()
{
	
	CPanelBaseDlg::OnDestroy();
}
