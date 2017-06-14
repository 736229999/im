/*
 *	$Header: $
 *
 *	$History: $
 */
#pragma once

#include "iconctrl.h"

class CImageFileDialog;

// CImageFileDialog
class CImageFileDialog :	public CFileDialog
{
	class CHookShellWnd : public CWnd
	{
	public:
		void			SetOwner(CImageFileDialog *m_pOwner);

		virtual	BOOL	OnNotify(WPARAM	wParam,	LPARAM lParam, LRESULT*	pResult);
		virtual BOOL	OnCommand(WPARAM wp, LPARAM lp);

	private:
		CImageFileDialog	*m_pOwner;
	};

	DECLARE_DYNAMIC(CImageFileDialog)
public:
					CImageFileDialog(LPCTSTR	lpszDefExt = NULL, LPCTSTR lpszFileName	= NULL,	DWORD dwFlags =	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	CWnd* pParentWnd = NULL);
	virtual			~CImageFileDialog();

	virtual	void	DoDataExchange(CDataExchange *pDX);
	virtual	BOOL	OnNotify(WPARAM	wParam,	LPARAM lParam, LRESULT*	pResult);
	virtual INT_PTR	DoModal();

	void			UpdatePreview(LPCTSTR szFilename);
	DWORD           GetSelFileSize() { return ::GetFileSize(GetPathName())/1024; }//K bytes

protected:
	//CImagePreviewStatic	m_preview;
	CIconCtrl		m_picture;
	int				m_nPrevSelection;
	CHookShellWnd	m_wndHook;
	DWORD           m_dwSelFileSize ;
	char *			m_szBuf;

	virtual	BOOL	OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
