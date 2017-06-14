#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "personinfo.h"

#define WM_MODIFY_NOTIFY  WM_USER+1
// CFormViewCtrl 窗体视图
class CFormViewCtrl : public CFormView
{
	DECLARE_DYNCREATE(CFormViewCtrl)
	DECLARE_HMG_SCROLLBAR()

	friend class CDialogAddAddress;

public:
	CFormViewCtrl();           // 动态创建所使用的受保护的构造函数
	virtual ~CFormViewCtrl();

public:
	enum { IDD = IDD_DIALOG_ADD_ADDRESS_CROLL_PART };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//这个函数通过对话框上的控件创建一个CFormViewCtrl控件，目的是在对话框设计的时候便于布局：
	BOOL CreateFromCtrl(UINT nID, CWnd* pParent);

	void ResetModifyFlag() { 	
		m_bModified   = false;
		m_bModifyFlag = false; 
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDtnDatetimechangeDatetimepickerBirthday(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	void Reset();
	
	BOOL SetContactInfo(const ContactPersonInfo& personInfo,const ContactGroupInfo &groupInfo);
	BOOL GetContactInfo(ContactPersonInfo& personInfo,ContactGroupInfo& groupInfo);	

	bool IsModified() { return m_bModified; }

private:
	CString			m_strName;
	CString			m_strCompany;
	CString			m_strCareer;
	CString			m_strAddress;
	CString			m_strEmail;
	CString			m_strPhone;
	CString			m_strMSNNum;
	CString			m_strBlog;
	CString			m_strNote;
	CString			m_strQQNum;
	CString			m_strSchool;
	CString			m_strBirthday;
	int				m_nSex;
	int				m_nType;//0 mini139 contact, 1 outlook contact
	int				m_nInvited;
	
	CGraphComboBox		m_wndComboType;
	CGraphComboBox		m_wndComboSex;

	CFlatEdit	m_wndEditName;
	CFlatEdit	m_wndEditSchool;
	CFlatEdit	m_wndEditCompany;
	CFlatEdit	m_wndEditAddress;
	CFlatEdit	m_wndEditEmail;
	CFlatEdit	m_wndEditPhone;
	CFlatEdit	m_wndEditQQNum;
	CFlatEdit	m_wndEditMSNNum;
	CFlatEdit	m_wndEditBlog;
	CFlatEdit	m_wndEditNote;
	CFlatEdit	m_wndEditCareer;
	CFlatEdit	m_wndEditBirthday;

	bool		m_bModified;
	bool		m_bModifyFlag;

	ContactGroupInfo   m_groupInfo;
};


