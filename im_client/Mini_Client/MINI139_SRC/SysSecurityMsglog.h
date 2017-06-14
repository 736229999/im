#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysSecurityMsglog 对话框

#include "..\..\..\DuiLib\UIlib.h"

class CSysSecurityMsglog : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISecurityMsgLog");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SecurityMsgLog.xml");  } 
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// 保存
	void Save();
	afx_msg void OnBnClickedBtnMsglog();

private:
	DuiLib::COptionUI *m_pCheckLoginShowLog; ////登录后同步
	DuiLib::COptionUI *m_pCheckExitDelMsgLog;////退出删除消息
	DuiLib::CButtonUI *m_pBtnOpenMsgLog;     ////打开消息管理器
	DuiLib::CSliderUI *m_pSliderVoice;
};