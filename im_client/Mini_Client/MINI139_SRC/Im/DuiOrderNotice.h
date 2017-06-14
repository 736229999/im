#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "IMProtocol.h"

class OrderMsg;

//����֪ͨ����
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
	void SetCurrentOrder(OrderMsg* pOrderMsg);//���õ�ǰѡ�еĶ���
	void AddOrder(OrderMsg* pOrderMsg);//���һ������
	void ShowSelectOrder(OrderMsg* pOrderMsg);//��ʾѡ�еĶ���
	void SetOrderMsgRead(OrderMsg* pOrderMsg);//�������Ϊ�Ѷ������彫����ȥ��
private:
	static std::tr1::shared_ptr<CDuiOrderNotice> pInstance;
	OrderMsg* m_pCurrentOrderMsg;//��ǰѡ�еĶ���
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

