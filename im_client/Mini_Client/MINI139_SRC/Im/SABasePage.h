#pragma once

#include "..\resource.h"
// CSABasePage 对话框

class CSABasePage : public CDialog
{
	DECLARE_DYNAMIC(CSABasePage)

public:
	CSABasePage(UINT nID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSABasePage();

	virtual bool IsModified() { return m_bModified; }
	virtual void ResetModifyFlag() { 	
			m_bModified   = false;
			m_bModifyFlag = false; 
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

	bool m_bModified;
	bool m_bModifyFlag;

private:
	HBRUSH m_hBrushBackGround;
	HBRUSH m_hBrushDisable;	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
