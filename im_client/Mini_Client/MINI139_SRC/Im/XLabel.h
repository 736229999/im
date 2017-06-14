#pragma once


#define XY_BEG namespace xy{
#define XY_END }

// CXLabel

XY_BEG

class CXLabel : public CStatic
{
	DECLARE_DYNAMIC(CXLabel)

public:

	CXLabel();

	virtual ~CXLabel();

	void	SetTextColor(COLORREF clrText);

	void	SetBkgColor(COLORREF clrBkg);


protected:

	COLORREF	m_clrText;
	COLORREF	m_clrBkg;
	HBRUSH		m_hBkg;
	BOOL		m_bTransparent;

protected:

	DECLARE_MESSAGE_MAP()

public:

	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

};


XY_END