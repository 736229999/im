#pragma once

#include "..\..\..\DuiLib\UIlib.h"


class CTopBarDlg : public DuiLib::WindowImplBase
{
public:
	enum { 
		TIMER_AUTO_CLOSE=0x00,
	};

	CTopBarDlg();
	~CTopBarDlg();
	static void Show(HWND hWnd, const char *szText, POINT pt, int nWidth);
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CTopBarDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("TopBar.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	virtual void OnFinalMessage( HWND hWnd );

private:
	static std::map<int, std::tr1::shared_ptr<CTopBarDlg>> g_mapDlg;
	int m_nID;
};