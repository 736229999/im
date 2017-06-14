#include "StdAfx.h"
#include "DuiOrderSubscibe.h"
#include "IMProtocol.h"
#include "localdata.h"

std::tr1::shared_ptr<CDuiOrderSubscibe> CDuiOrderSubscibe::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

tr1::shared_ptr<CDuiOrderSubscibe> CDuiOrderSubscibe::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiOrderSubscibe>(new CDuiOrderSubscibe());
		pInstance->Create(GetDesktopWindow(), _T("��Ϣ������ - ��������"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);	
	}
	return pInstance;
}

void CDuiOrderSubscibe::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

void CDuiOrderSubscibe::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();

	OrderSubscibe orderSubscibe = g_LocalData.GetOrderSubscibe();
	m_pOptionOrderNew = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("orderNew"));
	m_pOptionOrderPay = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("orderPay"));
	m_pOptionOrderEvaluate = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("orderEvaluate"));
	m_pOptionPopupTrading = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("optPopupTrading"));
	m_pOptionUnPopupTrading = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("optUnPopupTrading"));

	m_pOptionOrderReturn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("orderReturn"));
	m_pOptionPopupReturn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("optPopupReturn"));
	m_pOptionUnPopupReturn = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("optUnPopupReturn"));

	m_pOptionOrderNew->Selected(orderSubscibe.orderNew);
	m_pOptionOrderPay->Selected(orderSubscibe.orderPaid);
	m_pOptionOrderEvaluate->Selected(orderSubscibe.orderEvaluate);
	m_pOptionPopupTrading->Selected(orderSubscibe.orderTradePopup);
	m_pOptionUnPopupTrading->Selected(!orderSubscibe.orderTradePopup);

	m_pOptionOrderReturn->Selected(orderSubscibe.orderReturn);
	m_pOptionPopupReturn->Selected(orderSubscibe.orderReturnPopup);
	m_pOptionUnPopupReturn->Selected(!orderSubscibe.orderReturnPopup);
}

void CDuiOrderSubscibe::Notify(DuiLib::TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);

	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnOK")
		{
			SetOrderSubscibe();
			Close();
		}
		else if(msg.pSender->GetName() == "btnCencel")
		{
			Close();
		}
	}
}

void CDuiOrderSubscibe::SetOrderSubscibe()
{
	OrderSubscibe orderSubscibe;
	
	REMIND_TYPE remindTypeNeworder = RT_RECEIVE_AND_POPMSG;//�¶���
	REMIND_TYPE remindTypePayorder = RT_RECEIVE_AND_POPMSG;//����Ѹ���
	REMIND_TYPE remindTypeGoodsComment = RT_RECEIVE_AND_POPMSG;//���������
	REMIND_TYPE remindTypeReturnOrder = RT_RECEIVE_AND_POPMSG;//�˻���

	if(m_pOptionPopupTrading->IsSelected())//������Ϣ������ʾ
	{
		remindTypeNeworder = RT_RECEIVE_AND_POPMSG;
		remindTypePayorder = RT_RECEIVE_AND_POPMSG;
		remindTypeGoodsComment = RT_RECEIVE_AND_POPMSG;

		orderSubscibe.orderTradePopup = true;
	}
	else if(m_pOptionUnPopupTrading->IsSelected())//��������ʾ
	{
		remindTypeNeworder = RT_RECEIVE_NOT_POPMSG;
		remindTypePayorder = RT_RECEIVE_NOT_POPMSG;
		remindTypeGoodsComment = RT_RECEIVE_NOT_POPMSG;

		orderSubscibe.orderTradePopup = false;
	}

	if(!m_pOptionOrderNew->IsSelected())//�¶�����ѡ
	{
		remindTypeNeworder = RT_OFF;
		orderSubscibe.orderNew = false;
	}
	else
		orderSubscibe.orderNew = true;

	if(!m_pOptionOrderPay->IsSelected())//�Ѹ��ѡ
	{
		remindTypePayorder = RT_OFF;
		orderSubscibe.orderPaid = false;
	}
	else
		orderSubscibe.orderPaid = true;

	if(!m_pOptionOrderEvaluate->IsSelected())//�����۹�ѡ
	{
		remindTypeGoodsComment = RT_OFF;
		orderSubscibe.orderEvaluate = false;
	}
	else
		orderSubscibe.orderEvaluate = true;
		
	if(m_pOptionPopupReturn->IsSelected())//�˻���������ʾ
	{
		remindTypeReturnOrder = RT_RECEIVE_AND_POPMSG;//�¶���
		orderSubscibe.orderReturnPopup = true;
	}
	else if(m_pOptionUnPopupReturn->IsSelected())//�Ƿ񵯴���ʾ������Ϣ
	{
		remindTypeReturnOrder = RT_RECEIVE_NOT_POPMSG;
		orderSubscibe.orderReturnPopup = false;
	}

	if(!m_pOptionOrderReturn->IsSelected())//�˻�����ѡ
	{
		remindTypeReturnOrder = RT_OFF;
		orderSubscibe.orderReturn = false;
	}
	else
		orderSubscibe.orderReturn = true;
	
	g_LocalData.SetOrderSubscibe(orderSubscibe);
	//���Ͷ�����������
	MsgRemindAck msgRemindAck;
	msgRemindAck.set_neworder(remindTypeNeworder);
	msgRemindAck.set_payorder(remindTypePayorder);
	msgRemindAck.set_goodscomment(remindTypeGoodsComment);
	msgRemindAck.set_returnorder(remindTypeReturnOrder);
	g_pProtocol->SendReq(msgRemindAck);
}