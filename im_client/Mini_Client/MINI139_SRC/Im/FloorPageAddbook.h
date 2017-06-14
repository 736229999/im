#pragma once

#include "imaddresslistctrl.h"
#include "personinfo.h"
#include "DialogAddAddress.h"
#include "buttonext.h"
#include <map>
using namespace std;

class CFloorPageAddbook : public CDialog
{
	DECLARE_DYNAMIC(CFloorPageAddbook)
	DECLARE_HMG_SCROLLBAR()

public:
	CFloorPageAddbook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloorPageAddbook();

// 对话框数据
	enum { IDD = IDD_FLOOR_ADDBOOK };

public:
	void LoadAddressBook();
	void SaveAddressBook();

	void OnInviteAck(CString strName, CString strEmailAddress);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnGroupListNotify(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnBnClickedButtonAddAddr();
	afx_msg void OnBnClickedButtonDelAddr();
	afx_msg void OnBnClickedButtonModifyAddr();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnGroupDel();
	void OnMemberDel();
	void OnMemberModify();
	void OnMemberAdd();
	void OnSendMail();
	void OnInvite();
	void OnInviteGroup();

	LRESULT OnAddressNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnSaveCardNotify(WPARAM wParam, LPARAM lParam);

protected:
	void UpdateAddressList();
	void UpdateGroupInfo();
	void AddPerson2List(const ContactPersonInfo *pInfo,bool bSel);
	bool FindItem(CString strItem, int & nGroup, int & nItem);
	int  FindGroupData(int iData);
	void QuickSort(int nGroup, int iLo, int iHi);


private:
	CIMAddressListCtrl  m_wndAddressList;

	ContactGroupInfo    m_groupInfo;

	ADDRESS_BOOK_LIST   m_listPerson;

	CFlatButton m_wndBtnAdd;
	CFlatButton m_wndBtnModify;
	CFlatButton m_wndBtnDelete;
	CFlatButton m_wndBtnSetMyInfo;
	CFlatButton m_wndBtnMiniLink;
	
	bool			    m_bSorted;

public:
	afx_msg void OnBnClickedButtonMinilink();
	afx_msg void OnPaint();
};
