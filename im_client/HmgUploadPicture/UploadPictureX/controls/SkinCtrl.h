#ifndef _SKIN_CTRL_
#define _SKIN_CTRL_


class CStaticBase : public CStatic
{
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

class CTransparentLabel : public CStaticBase
{
public:
	CTransparentLabel()
	{
		m_clrText= 0;
	}

	void SetTextColor(COLORREF clr) { m_clrText=clr; }
	void SetWindowText(CString sEndWithDot, CString sFullStr)
	{
		m_strName   = sEndWithDot;
		m_strStatus = sFullStr;
		
		CWnd *pParent = GetParent();

		if ( pParent && pParent->m_hWnd )
		{
			CRect rc;
			GetClientRect(&rc);
			ClientToScreen(&rc);

			pParent->ScreenToClient(&rc);
			pParent->InvalidateRect(&rc);
		}
	}

protected:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_clrText;
	CString  m_strName, m_strStatus;
};

#endif 