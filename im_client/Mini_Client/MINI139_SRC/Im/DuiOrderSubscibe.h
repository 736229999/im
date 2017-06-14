#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "IMProtocol.h"

//��������ѡ��
class CDuiOrderSubscibe : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIOrderSubscibe");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("OrderSubscibe.xml");  }  
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	static tr1::shared_ptr<CDuiOrderSubscibe> Show();
	static void UpdateBk();
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);

public:
	void SetOrderSubscibe();//���ò����Ͷ�����������
private:
	static std::tr1::shared_ptr<CDuiOrderSubscibe> pInstance;
	DuiLib::COptionUI* m_pOptionOrderNew;
	DuiLib::COptionUI* m_pOptionOrderPay;
	DuiLib::COptionUI* m_pOptionOrderEvaluate;
	DuiLib::COptionUI* m_pOptionPopupTrading;
	DuiLib::COptionUI* m_pOptionUnPopupTrading;

	DuiLib::COptionUI* m_pOptionOrderReturn;
	DuiLib::COptionUI* m_pOptionPopupReturn;
	DuiLib::COptionUI* m_pOptionUnPopupReturn;
};

