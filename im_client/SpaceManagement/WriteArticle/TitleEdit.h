#pragma once


// CTitleEdit

class CTitleEdit : public CFlatEdit
{
	DECLARE_DYNAMIC(CTitleEdit)

public:
	CTitleEdit();
	virtual ~CTitleEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


