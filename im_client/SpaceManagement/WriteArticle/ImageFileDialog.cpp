/*
 *	$Header: $
 *
 *	$History: $
 */
#include "stdafx.h"
#include "resource.h"
#include ".\imagefiledialog.h"
#include <dlgs.h>


// CImageFileDialog
IMPLEMENT_DYNAMIC(CImageFileDialog, CFileDialog)

static char BASED_CODE szFilter[] = "图片文件(*jpg;*jpeg;*.gif,*.png,*.tif)|*jpg;*jpeg;*.png;*.tif;*.gif||";

void CImageFileDialog::CHookShellWnd::SetOwner(CImageFileDialog *pOwner)
{
	ASSERT(pOwner);
	ASSERT_KINDOF(CImageFileDialog, pOwner);

	m_pOwner = pOwner;
	
}

BOOL CImageFileDialog::CHookShellWnd::OnCommand(WPARAM /*wParam*/, LPARAM lParam)
{
//	TRACE("wp(%d), lp(%x)\n", wParam, lParam);
	return CWnd::OnCommand(0, lParam);
}

BOOL CImageFileDialog::CHookShellWnd::OnNotify(WPARAM, LPARAM lParam, LRESULT *pResult)
{
	LPNMLISTVIEW pLVHdr	= reinterpret_cast<LPNMLISTVIEW>(lParam);

	if (pLVHdr->hdr.code ==	LVN_ITEMCHANGED	&& (pLVHdr->uChanged & LVIF_STATE))
	{
		if (pLVHdr->iItem != -1)
		{
			//	It's a valid listview index	so we attach the
			//	the	handle of the window that sent the message
			//	to a local CListCtrl object	for	easy access.
			CListCtrl	  ctl;
			LPCITEMIDLIST pidl;
			TCHAR		  tszBuffer[_MAX_PATH],
						  tszFileName[_MAX_PATH],
						  tszExtension[_MAX_EXT];
			CString		  csTemp;

			ctl.Attach(pLVHdr->hdr.hwndFrom);
			pidl = (LPCITEMIDLIST) ctl.GetItemData(pLVHdr->iItem);
			SHGetPathFromIDList(pidl, tszBuffer);
			_tsplitpath(tszBuffer, NULL, NULL, tszFileName, tszExtension);
			csTemp.Format(_T("%s%s"), tszFileName, tszExtension);

			//	Update our parent window
			if (m_pOwner->m_nPrevSelection != pLVHdr->iItem)
				m_pOwner->UpdatePreview(csTemp);

			//	Be certain we detach the handle	before the CListCtrl
			//	object goes	out	of scope (else the underlying List View
			//	will be	deleted, which is NOT what we want).
			ctl.Detach();
		}
		else
			m_pOwner->UpdatePreview(_T(""));
	}

	return *pResult	= 0;
}

CImageFileDialog::CImageFileDialog(LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, CWnd* pParentWnd) : CFileDialog(TRUE, lpszDefExt, lpszFileName, dwFlags, szFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_ENABLETEMPLATE | OFN_ENABLESIZING;
	m_ofn.hInstance = AfxGetInstanceHandle();
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_IMAGEPREVIEWDLG);

	//	Provide a big buffer for returned filenames
	m_ofn.lpstrFile = new TCHAR[20000];
	m_ofn.nMaxFile  = 20000;
	memset(m_ofn.lpstrFile, 0, sizeof(m_ofn.lpstrFile));

	m_nPrevSelection = -1;
}

CImageFileDialog::~CImageFileDialog()
{
	delete m_ofn.lpstrFile;
}

BEGIN_MESSAGE_MAP(CImageFileDialog, CFileDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

INT_PTR	CImageFileDialog::DoModal()
{
	INT_PTR status = CFileDialog::DoModal();

//	m_preview.Close();
	return status;
}

BOOL CImageFileDialog::OnNotify(WPARAM, LPARAM lp, LRESULT *pResult)
{
	LPOFNOTIFY of =	(LPOFNOTIFY) lp;
	CString	   csTemp;
	TCHAR	   tszBuffer[_MAX_PATH];

	switch (of->hdr.code)
	{
	case CDN_SELCHANGE:
		if (CommDlg_OpenSave_GetSpec(GetParent()->GetSafeHwnd(), tszBuffer, sizeof(tszBuffer)) > 0)
			TRACE("%s\n", tszBuffer);

		break;

	case CDN_FOLDERCHANGE:
		//	Once we	get	this notification our old subclassing of
		//	the	SHELL window is	lost, so we	have to
		//	subclass it	again. (Changing the folder	causes a 
		//	destroy	and	recreate of	the	SHELL window).
		if (m_wndHook.GetSafeHwnd()	!= HWND(NULL))
			m_wndHook.UnsubclassWindow();

		m_wndHook.SubclassWindow(GetParent()->GetDlgItem(lst2)->GetSafeHwnd());
		UpdatePreview(_T(""));
		break;
	}

	*pResult = 0;
	return FALSE;
}

void CImageFileDialog::DoDataExchange(CDataExchange *pDX)
{
	CFileDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGEPREVIEW, m_picture);
}

// CImageFileDialog message handlers
BOOL CImageFileDialog::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	m_wndHook.SetOwner(this);

	GetParent()->CenterWindow(GetDesktopWindow());
	//	It's an import dialog so change the button text on 
	//	the open button to say "Import"
	//  GetParent()->GetDlgItem(IDOK)->SetWindowText(_T("Import"));
	//  GetParent()->SetWindowText(_T("Import Pictures"));
	return TRUE;
}

void CImageFileDialog::UpdatePreview(LPCTSTR szFilename)
{
	ASSERT(szFilename);
	ASSERT(AfxIsValidString(szFilename));

	if ( strcmp("",szFilename)==0 )
	{
		return ;
	}

	CString strFile;
	strFile.Format("%s\\%s", GetFolderPath() , szFilename);

	m_picture.SetFaceFile(strFile);

	DWORD dwFileSize = ::GetFileSize(strFile);

	CString strTemp;

	float fSize = dwFileSize/1024.0;

	if ( fSize>1024.0 )
		strTemp.Format("大小: %.2fM",fSize/1024.0);
	else
		strTemp.Format("大小: %.2fK",fSize);

	m_dwSelFileSize = fSize ;//K bytes

	SetDlgItemText(IDC_FILESIZE,strTemp);
}

void CImageFileDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFileDialog::OnShowWindow(bShow, nStatus);
}
