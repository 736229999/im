#pragma once
#include "resource.h"
#include "..\im\impublic.h"
#include "afxwin.h"
#include "..\im\LocalData.h"
#include "..\..\..\DuiLib\UIlib.h"


class CUIAutoReply : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIAutoReply");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("autoreply.xml");  }
	virtual void InitWindow();
	virtual void OnClick(DuiLib::TNotifyUI& msg);
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	// ±£´æ
	void CUIAutoReply::Save();
	vector<AUTO_REPLY> m_vecItem;

private:
	void AddAutoReply(const char *szAutoReply);
	void OnBnClickedBtnAdd();
	void OnBnClickedBtnMod();
	void OnBnClickedBtnDel();

private:
	DuiLib::CListUI *m_pList;
	DuiLib::CButtonUI *m_pBtnAddReply;
	DuiLib::CButtonUI *m_pBtnEditReply;
	DuiLib::CButtonUI *m_pBtnDelReply;
	DuiLib::CButtonUI *m_pBtnUpReply;
	DuiLib::CButtonUI *m_pBtnDownReply;
};