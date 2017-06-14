#pragma once

class _EnableWindow
{
public:
	_EnableWindow(HWND hWnd /* , BOOL bWaitCursor = TRUE */ );
	_EnableWindow(CWnd *pWnd);

	_EnableWindow(const UINT nCtrlID, CWnd *pParentWnd);
	virtual ~_EnableWindow();

private:
	HWND m_hWnd;
};
