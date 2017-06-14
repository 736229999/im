#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysSecurityMsglog �Ի���

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
	// ����
	void Save();
	afx_msg void OnBnClickedBtnMsglog();

private:
	DuiLib::COptionUI *m_pCheckLoginShowLog; ////��¼��ͬ��
	DuiLib::COptionUI *m_pCheckExitDelMsgLog;////�˳�ɾ����Ϣ
	DuiLib::CButtonUI *m_pBtnOpenMsgLog;     ////����Ϣ������
	DuiLib::CSliderUI *m_pSliderVoice;
};