#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\Common\WndShadow.h"

class CFriendListUI;
class CCustSerTranDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CustSerTranDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CustSerTranDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void AddGroup(LPCTSTR szText);
	void AddFriend(LPCTSTR szImgPath, LPCTSTR szText, unsigned int id, DWORD dwTextColor = 0x2E2E2E);
	void Notify(DuiLib::TNotifyUI& msg);
	void UpdateBk();

public:
	int m_nSelFriendID;
	string m_strOrderID;
	string m_strReason;

private:
	CWndShadow m_WndShadow;
	CFriendListUI *m_pFriendList;
	DuiLib::CTabLayoutUI *m_pTab;
	DuiLib::CButtonUI *m_pButtonClose1;
	DuiLib::CButtonUI *m_pButtonClose2;
	DuiLib::CButtonUI *m_pButtonOK;
	DuiLib::CButtonUI *m_pBtnBack;
	DuiLib::CLabelUI *m_pLabelTranObject;
	DuiLib::CEditUI *m_pEditOrderId;
	DuiLib::CRichEditUI *m_pEditReason;
};

class CFriendListUI : public DuiLib::CListUI
{
public:
	CFriendListUI(DuiLib::CPaintManagerUI& paint_manager);
	~CFriendListUI();
	bool AddGroup(LPCTSTR szText);
	bool AddFriend(LPCTSTR szImgPath, LPCTSTR szText, unsigned int id, DWORD dwTextColor = 0x2E2E2E);
	void CliekedGroup();

private:
	DuiLib::CPaintManagerUI& paint_manager_;
	BOOL m_bGroupExpend;
	DuiLib::CListContainerElementUI *m_pGroup;
	std::vector<DuiLib::CListContainerElementUI*> m_vecpFriendItem;

};