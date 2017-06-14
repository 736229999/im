#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
class CDuiCounselNone;
class CDuiCounselNone : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CDuiCounselNone> NewDlg();
	static std::tr1::shared_ptr<CDuiCounselNone> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CDuiCounselNone> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("DuiCounselNone");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("DuiCounselNone.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();

	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);

	static void UpdateBk();
public:
	CDuiCounselNone(void);
	~CDuiCounselNone(void);
};

