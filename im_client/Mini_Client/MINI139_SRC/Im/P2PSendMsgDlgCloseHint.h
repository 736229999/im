#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\MainFrame.h"

class CP2PSendMsgDlg;
class CP2PSendMsgDlgCloseHint : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> NewDlg(HWND pDlg);
	static std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("P2PSendMsgDlgCloseHint");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("P2PSendMsgDlgCloseHint.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();

	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);

	static void UpdateBk();

	HWND m_parent;
	void SetParentHwnd(HWND pDlg);
	void SetWindowTop();
public:
	CP2PSendMsgDlgCloseHint(void);
	~CP2PSendMsgDlgCloseHint(void);
};

