#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "IMProtocol.h"

class OrderMsg;

//订单通知窗口
class CDuiOrderNotice : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIOrderNotice");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("OrderNotice.xml");  }  
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	
	static tr1::shared_ptr<CDuiOrderNotice> Show(OrderMsg* pOrderMsg = NULL);
	static void UpdateBk();
	virtual void InitWindow();
	static std::tr1::shared_ptr<CDuiOrderNotice> GetInstance()	{	return pInstance;	}
	virtual void Notify(DuiLib::TNotifyUI& msg);
public:
	void SetCurrentOrder(OrderMsg* pOrderMsg);//设置当前选中的订单
	void AddOrder(OrderMsg* pOrderMsg);//添加一个订单
	void ShowSelectOrder(OrderMsg* pOrderMsg);//显示选中的订单
	void SetOrderMsgRead(OrderMsg* pOrderMsg);//订单标记为已读，字体将粗体去掉
private:
	static std::tr1::shared_ptr<CDuiOrderNotice> pInstance;
	OrderMsg* m_pCurrentOrderMsg;//当前选中的订单
	DuiLib::CTreeViewUI* m_pTreeView;
	DuiLib::CTreeNodeUI* m_pTreeNodeOrderNew;
	DuiLib::CTreeNodeUI* m_pTreeNodeOrderPay;
	DuiLib::CTreeNodeUI* m_pTreeNodeOrderContent;
	DuiLib::CTreeNodeUI* m_pTreeNodeOrderReturn;
	DuiLib::CLabelUI* m_pLabelTotalPrice;
	DuiLib::CLabelUI* m_pLabelFreight;
	DuiLib::CLabelUI* m_pLabelTotalPriceLeft;
	DuiLib::CLabelUI* m_pLabelTotalFreightLeft;
	DuiLib::CLabelUI* m_pLabelTotalFreightRight;
};

