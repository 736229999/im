#pragma once
#include "shockwaveflash.h"
#include "explorer1.h"

// CAdvertisePanel 对话框

class CAdvertisePanel : public CDialog
{
	DECLARE_DYNAMIC(CAdvertisePanel)

public:
	CAdvertisePanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdvertisePanel();

// 对话框数据
	enum { IDD = IDD_MAIN_AD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel() {}
	DECLARE_MESSAGE_MAP()


protected:
	class CAdvStatic : public CStatic
	{
	public:
		CAdvStatic()
		{
			m_bEnableDraw = true;
			m_hCursor = ::LoadCursor(NULL,IDC_HAND);
		}

	public:
		void EnableDraw(bool bEnable)
		{
			m_bEnableDraw=bEnable;
		}
	protected:
		void OnPaint();
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		DECLARE_MESSAGE_MAP()
	protected:
		HCURSOR m_hCursor;
		BOOL    m_bEnableDraw;

	};

	CFlatButton  m_btnSearch;
	CFlatEdit    m_edtContent;
	CGraphComboBox m_combSearchType;

	CAdvStatic      m_wndFrame;

	CFont		 m_font;
	CExplorer1	 m_wndExplore;

	bool		 m_bNavigated;
	bool		 m_bNavigateError;
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticFrame();
	DECLARE_EVENTSINK_MAP()
	void NavigateErrorExplorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel);
	void NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL);
	afx_msg void OnEnSetfocusEditContent();
	afx_msg void OnEnKillfocusEditContent();
	afx_msg void OnBnClickedButtonSearch();
};
