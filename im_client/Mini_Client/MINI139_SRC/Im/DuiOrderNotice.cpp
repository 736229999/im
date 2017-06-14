#include "StdAfx.h"
#include "DuiOrderNotice.h"
#include "LocalData.h"
#include "WebProcess.h"
#include "impublic.h"
#include "GdiPlus.h"
#include "../MainFrame.h"
#include "MsgDlg.h"

std::tr1::shared_ptr<CDuiOrderNotice> CDuiOrderNotice::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

extern string CutRectImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);

//显示订单数据使用多线程
UINT ShowSelectOrderThread( LPVOID pParam )
{
	OrderMsg* p = (OrderMsg*)pParam;
	if(CDuiOrderNotice::GetInstance() != NULL && CDuiOrderNotice::GetInstance()->GetHWND() != NULL)
	{
		CDuiOrderNotice::GetInstance()->ShowSelectOrder(p);
		CDuiOrderNotice::GetInstance()->SetCurrentOrder(p);
	}
	return 0;
};

tr1::shared_ptr<CDuiOrderNotice> CDuiOrderNotice::Show(OrderMsg* pOrderMsg)
{
	if(pOrderMsg->orderType == -1)//没有订单类型则不添加订单
		return NULL;
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
		if(pOrderMsg != NULL)
		{
			pInstance->AddOrder(pOrderMsg);
		}
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiOrderNotice>(new CDuiOrderNotice());
		pInstance->Create(GetDesktopWindow(), _T("订单消息"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
		::BringWindowToTop(pInstance->GetHWND());
		if(pOrderMsg != NULL)
		{
			pInstance->AddOrder(pOrderMsg);
			//pInstance->ShowSelectOrder(pOrderMsg);
			AfxBeginThread(ShowSelectOrderThread,pOrderMsg);
			//pInstance->ShowSelectOrder(pOrderMsg);
		}
	}
	::FlashWindow(pInstance->GetHWND(), TRUE);
	return pInstance;
}

void CDuiOrderNotice::UpdateBk()
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

void CDuiOrderNotice::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();

	m_pLabelTotalPrice = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalPrice"));
	m_pLabelFreight = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelFreight"));

	m_pLabelTotalPriceLeft = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalPriceLeft"));
	m_pLabelTotalFreightLeft = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalFreightLeft"));
	m_pLabelTotalFreightRight = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalFreightRight"));
	
	m_pLabelTotalPrice->SetFont(2);
	m_pLabelFreight->SetFont(2);
	
	m_pTreeView = static_cast<DuiLib::CTreeViewUI*>(m_PaintManager.FindControl(_T("treeOrederType")));
	
	for(int i = 0; i < 4; i++)
	{
		DuiLib::CDialogBuilder builder;
		DuiLib::CTreeNodeUI* pTreeNode = static_cast<DuiLib::CTreeNodeUI*>(builder.Create(_T("OrederNoticeTreeElement.xml"), (UINT)0, NULL));

		m_pTreeView->Add(pTreeNode);
		CString strOrderType;
		switch (i)
		{
		case 0:
			strOrderType = "新订单";
			m_pTreeNodeOrderNew = pTreeNode;
			m_pTreeNodeOrderNew->SetVisible(false);
			break;
		case 1:
			strOrderType = "买家已付款";
			m_pTreeNodeOrderPay = pTreeNode;
			m_pTreeNodeOrderPay->SetVisible(false);
			break;
		case 2:
			strOrderType = "买家已评价";
			m_pTreeNodeOrderContent = pTreeNode;
			m_pTreeNodeOrderContent->SetVisible(false);
			break;
		case 3:
			strOrderType = "退货消息";
			m_pTreeNodeOrderReturn = pTreeNode;
			m_pTreeNodeOrderReturn->SetVisible(false);
			break;
		}
		pTreeNode->GetFolderButton()->SetText(strOrderType);

		m_pTreeView->SetItemExpand(true, pTreeNode);
	}
	m_pTreeView->SetAttribute("visiblefolderbtn", "true");//visiblefolderbtn设置为true放在最后
}

void CDuiOrderNotice::Notify(DuiLib::TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnDetails")
		{
			OrderMsg* pOrderMsg = m_pCurrentOrderMsg;
			if(pOrderMsg != NULL)
			{
				if(pOrderMsg->orderType == 0 || pOrderMsg->orderType == 1)
				{
					string orderPackNum = pOrderMsg->orderPackNum;
					CString strDetailUrl;
					strDetailUrl.Format("%sshowStatus=delivery&id=%s",ORDER_TRADE_URL, orderPackNum.c_str());
					OpenHtmlPage(strDetailUrl);
				}
				else if(pOrderMsg->orderType == 2)
				{
					OpenHtmlPage(ORDER_COMMENT_URL);
				}
				else if(pOrderMsg->orderType == 3)
				{
					string orderPackNum = pOrderMsg->orderPackNum;
					CString strDetailUrl;
					strDetailUrl.Format("%sopt=audit&id=%s", ORDER_RETURN_URL, orderPackNum.c_str());
					OpenHtmlPage(strDetailUrl);
				}
			}
		}
		else if(msg.pSender->GetName() == "btnMoreMsg")
		{
			CDuiMsgDlg::Show();
			if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL)
			{
				if(m_pCurrentOrderMsg != NULL)
				{
					CDuiMsgDlg::GetInstance()->ShowOrderPage(m_pCurrentOrderMsg->orderType);
				}
				
			}
		}
	}
	else if(msg.sType == _T("itemclick"))
	{
		if(msg.pSender->GetParent()->GetParent() == m_pTreeView)
		{
			DuiLib::CTreeNodeUI* pNode = (DuiLib::CTreeNodeUI*)msg.pSender;
			OrderMsg* pOrderMsg = (OrderMsg*)pNode->GetTag();
			//ShowSelectOrder(pOrderMsg);
			AfxBeginThread(ShowSelectOrderThread,pOrderMsg);
			//设置当前订单
			SetCurrentOrder(pOrderMsg);
			//标记为已读
			pOrderMsg->bIsRead = true;
			DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pNode, _T("labelOrderName")));
			pLabelOrderName->SetFont(0);
			g_LocalData.SetOrderMsgRead(pOrderMsg->orderUuid);

			//订单消息订阅窗口
			if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL && IsWindowVisible(CDuiMsgDlg::GetInstance()->GetHWND()))
			{
				CDuiMsgDlg::GetInstance()->SetOrderMsgRead(pOrderMsg);
			}
			
			//如果未读订单为空则取消红点
			BOOL bIsAllRead = TRUE;
			for(int nOrder = 0; nOrder < g_LocalData.GetOrderMsgTrade().size(); nOrder++)
			{
				OrderMsg* pOrderMsgTrade = g_LocalData.GetOrderMsgTrade()[nOrder];
				if(!pOrderMsgTrade->bIsRead)
				{
					bIsAllRead = FALSE;
					break;
				}
			}
			for(int nOrder = 0; nOrder < g_LocalData.GetOrderMsgReturn().size(); nOrder++)
			{
				OrderMsg* pOrderMsgTrade = g_LocalData.GetOrderMsgReturn()[nOrder];
				if(!pOrderMsgTrade->bIsRead)
				{
					bIsAllRead = FALSE;
					break;
				}
			}
			if(bIsAllRead)
				CMainFrame::GetInstance()->SetLabelOrderMsgUnRead(false);
		}
	}
}

void CDuiOrderNotice::SetCurrentOrder(OrderMsg* pOrderMsg)
{
	m_pCurrentOrderMsg = pOrderMsg;
}

void CDuiOrderNotice::AddOrder(OrderMsg* pOrderMsg)
{
	DuiLib::CTreeNodeUI* pTreeNode =  NULL;
	switch (pOrderMsg->orderType)
	{
	case 0:
		pTreeNode = m_pTreeNodeOrderNew;
		break;
	case 1:
		pTreeNode = m_pTreeNodeOrderPay;
		break;
	case 2:
		pTreeNode = m_pTreeNodeOrderContent;
		break;
	case 3:
		pTreeNode = m_pTreeNodeOrderReturn;
		break;
	}
	if(pTreeNode == NULL)
		return;
	if(!pTreeNode->IsVisible())
		pTreeNode->SetVisible();
	DuiLib::CDialogBuilder builder;
	DuiLib::CTreeNodeUI* pTreeNodeChild = static_cast<DuiLib::CTreeNodeUI*>(builder.Create(_T("OrederNoticeTreeNodeElement.xml"), (UINT)0, NULL));

	DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pTreeNodeChild, _T("labelOrderName")));
	pLabelOrderName->SetText(pOrderMsg->buyerName.c_str());
	pLabelOrderName->SetFont(2);

	pTreeNodeChild->SetTag((DWORD)pOrderMsg);

	pTreeNode->AddAt(pTreeNodeChild, 0);
	pTreeNode->GetFolderButton()->SetCheck(true);
}

void CDuiOrderNotice::ShowSelectOrder(OrderMsg* pOrderMsg)
{
	DuiLib::CVerticalLayoutUI* pVlOrder = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl("VLOrder"));
	pVlOrder->RemoveAll();

	DuiLib::CHorizontalLayoutUI* pHLOrderNum = new DuiLib::CHorizontalLayoutUI();

	pHLOrderNum->SetFixedHeight(30);
	DuiLib::CLabelUI* pLabelOrderNum = new DuiLib::CLabelUI();
	pLabelOrderNum->SetAttribute("padding", "10,0,0,0");

	CString strOrderNum;
	strOrderNum.Format("订单号：%s", pOrderMsg->orderNum.c_str());
	pLabelOrderNum->SetAttribute("text", strOrderNum);
	pHLOrderNum->Add(pLabelOrderNum);

	DuiLib::CHorizontalLayoutUI* pHLMid = new DuiLib::CHorizontalLayoutUI();
	pHLOrderNum->Add(pHLMid);

	DuiLib::CLabelUI* pLabelOrderTime = new DuiLib::CLabelUI();
	pLabelOrderTime->SetFixedWidth(130);
	pLabelOrderTime->SetAttribute("text", pOrderMsg->orderTime.c_str());
	pHLOrderNum->Add(pLabelOrderTime);

	pVlOrder->Add(pHLOrderNum);

	DuiLib::CHorizontalLayoutUI* pHLOrderTitle = new DuiLib::CHorizontalLayoutUI();
	pHLOrderTitle->SetFixedHeight(30);
	DuiLib::CLabelUI* pLabelOrderTitle = new DuiLib::CLabelUI();
	pLabelOrderTitle->SetAttribute("padding", "10,0,10,0");
	pLabelOrderTitle->SetAttribute("bordersize", "1");
	pLabelOrderTitle->SetAttribute("bordercolor", "#FFDCDCDC");
	pLabelOrderTitle->SetAttribute("bkcolor", "#FFFFF3DD");
	pLabelOrderTitle->SetAttribute("textpadding", "5,0,0,0");
	CString strTitle;
	if(pOrderMsg->orderType == 0)
		strTitle.Format("买家 %s 购买了 %d 件商品，等待买家付款", pOrderMsg->buyerName.c_str(), pOrderMsg->vecOrderItem.size());
	else if(pOrderMsg->orderType == 1)
		strTitle.Format("买家 %s 已经付款，请尽快发货", pOrderMsg->buyerName.c_str(), pOrderMsg->vecOrderItem.size());
	else if(pOrderMsg->orderType == 2)
		strTitle.Format("买家 %s 已经评价商品", pOrderMsg->buyerName.c_str(), pOrderMsg->vecOrderItem.size());
	else if(pOrderMsg->orderType == 3)
		strTitle.Format("买家 %s 已经申请以下商品退货", pOrderMsg->buyerName.c_str(), pOrderMsg->vecOrderItem.size());
	pLabelOrderTitle->SetAttribute("text", /*"买家 熊宝宝 购买了2个商品，等待买家付款"*/strTitle);
	pHLOrderTitle->Add(pLabelOrderTitle);

	pVlOrder->Add(pHLOrderTitle);

	for(int nItem = 0; nItem < pOrderMsg->vecOrderItem.size(); nItem++)
	{
		OrderItem* pOrderItem = pOrderMsg->vecOrderItem[nItem];
		pOrderItem->itemPicUrl;
		DuiLib::CHorizontalLayoutUI* pHLOrderContent = new DuiLib::CHorizontalLayoutUI();
		pHLOrderContent->SetFixedHeight(80);
		DuiLib::CLabelUI* pLabelItemPic = new DuiLib::CLabelUI();
		pLabelItemPic->SetAttribute("padding", "10,2,0,0");
		pLabelItemPic->SetFixedWidth(80);
		pLabelItemPic->SetFixedHeight(80);

		CString strUrl = pOrderItem->itemPicUrl.c_str();
		int nPoint = strUrl.ReverseFind('.');
		strUrl = strUrl.Left(nPoint) + "_80x80" + strUrl.Right(strUrl.GetLength() - nPoint);
		string strPicUrl = strUrl;
		string strFilePath = g_LocalData.GetUrlFile(strPicUrl);
		if(strFilePath != "")
		{
			pLabelItemPic->SetAttribute("bkimage", strFilePath.c_str());
		}
		else
		{
			if(strUrl.Find("http://") != -1)
			{
				strUrl = strUrl.Right(strUrl.GetLength() - 7);
				int nIndex = strUrl.Find("/");
				strUrl = strUrl.Right(strUrl.GetLength() - nIndex);
			}
			strUrl = GetPicRandomUrl() + strUrl;
			CString strLocalPath = strUrl.Right(strUrl.GetLength() - strUrl.ReverseFind('/') - 1);
			CString strDownloadFile = g_LocalData.GetImageFileCachePath() + strLocalPath;
			if(DownLoadWebFile(/*strStoreLogoUrl*/strUrl,strDownloadFile))
			{
				//string strFile = CutRectImg(strDownloadFile, 80, 80);
				pLabelItemPic->SetAttribute("bkimage", strDownloadFile);
				string strLocalFile = strDownloadFile;
				g_LocalData.SetUrlFile(strPicUrl, strLocalFile);
			}
			else
			{
				pLabelItemPic->SetAttribute("bordersize", "1");
				pLabelItemPic->SetAttribute("bordercolor", "#FFDCDCDC");
			}
		}

		pHLOrderContent->Add(pLabelItemPic);

		DuiLib::CVerticalLayoutUI* pVLItemContent = new DuiLib::CVerticalLayoutUI();
		DuiLib::CLabelUI* pLabelItemName = new DuiLib::CLabelUI();
		pLabelItemName->SetAttribute("padding", "10,0,0,0");
		CString strOrderItemTitle;
		strOrderItemTitle.Format("%s", pOrderItem->itemName.c_str());
		pLabelItemName->SetAttribute("text", strOrderItemTitle);
		pVLItemContent->Add(pLabelItemName);

		DuiLib::CLabelUI* pLabelStandard = new DuiLib::CLabelUI();
		pLabelStandard->SetAttribute("padding", "10,0,0,0");
		CString strOrderItemStandard;
		strOrderItemStandard.Format("商品规格：%s", pOrderItem->itemStandard.c_str());
		pLabelStandard->SetAttribute("text", strOrderItemStandard);
		pVLItemContent->Add(pLabelStandard);

		DuiLib::CLabelUI* pLabelItemCount = new DuiLib::CLabelUI();
		pLabelItemCount->SetAttribute("padding", "10,0,0,0");
		CString strOrderItemCount;
		if(pOrderMsg->orderType == 3)
			strOrderItemCount.Format("退货数量：%d", pOrderItem->itemCount);
		else
			strOrderItemCount.Format("商品数量：%d", pOrderItem->itemCount);
		pLabelItemCount->SetAttribute("text", strOrderItemCount);
		pVLItemContent->Add(pLabelItemCount);

		DuiLib::CHorizontalLayoutUI* pHLOrderItemPrice = new DuiLib::CHorizontalLayoutUI();
		pHLOrderItemPrice->SetFixedHeight(20);
		DuiLib::CLabelUI* pLabelItemPriceLeft = new DuiLib::CLabelUI();
		pLabelItemPriceLeft->SetAttribute("padding", "10,0,0,0");
		if(pOrderMsg->orderType == 3)
			pLabelItemPriceLeft->SetAttribute("text", "退货金额：");
		else
			pLabelItemPriceLeft->SetAttribute("text", "商品金额：");
		pLabelItemPriceLeft->SetAttribute("autocalcwidth", "true");
		pHLOrderItemPrice->Add(pLabelItemPriceLeft);
		DuiLib::CLabelUI* pLabelItemPriceMid = new DuiLib::CLabelUI();
		pLabelItemPriceMid->SetAttribute("padding", "0,0,0,0");
		pLabelItemPriceMid->SetAttribute("textcolor", "#FFDB384C");
		pLabelItemPriceMid->SetAttribute("autocalcwidth", "true");
		CString strOrderItemPrice;
		strOrderItemPrice.Format("%lg", pOrderItem->itemPrice);
		pLabelItemPriceMid->SetAttribute("text", strOrderItemPrice);
		pHLOrderItemPrice->Add(pLabelItemPriceMid);
		DuiLib::CLabelUI* pLabelItemPriceRight = new DuiLib::CLabelUI();
		pLabelItemPriceRight->SetAttribute("padding", "0,0,0,0");
		pLabelItemPriceRight->SetAttribute("text", "元");
		pLabelItemPriceRight->SetAttribute("autocalcwidth", "true");
		pHLOrderItemPrice->Add(pLabelItemPriceRight);

		pVLItemContent->Add(pHLOrderItemPrice);

		pHLOrderContent->Add(pVLItemContent);
		if(nItem != pOrderMsg->vecOrderItem.size() - 1)
		{
			DuiLib::CVerticalLayoutUI* pVLLine = new DuiLib::CVerticalLayoutUI();
			pVLLine->SetFixedHeight(3);
			DuiLib::CHorizontalLayoutUI* pHLLineleft = new DuiLib::CHorizontalLayoutUI();
			pVLLine->Add(pHLLineleft);
			DuiLib::CHorizontalLayoutUI* pHLLineMid = new DuiLib::CHorizontalLayoutUI();
			pHLLineMid->SetAttribute("padding", "10,0,10,0");
			pHLLineMid->SetFixedHeight(1);
			pHLLineMid->SetAttribute("bkcolor", "#FFDCDCDC");
			pVLLine->Add(pHLLineMid);
			DuiLib::CHorizontalLayoutUI* pHLLineRight = new DuiLib::CHorizontalLayoutUI();
			pVLLine->Add(pHLLineRight);
			pHLOrderContent->Add(pVLLine);
		}


		pVlOrder->Add(pHLOrderContent);
	}
	
	if(pOrderMsg->orderType == 0 || pOrderMsg->orderType == 1)
	{
		m_pLabelTotalPrice->SetVisible(true);
		m_pLabelFreight->SetVisible(true);
		m_pLabelTotalPriceLeft->SetVisible(true);
		m_pLabelTotalFreightLeft->SetVisible(true);
		m_pLabelTotalFreightRight->SetVisible(true);

		CString strTotalPrice;
		strTotalPrice.Format("%lg", pOrderMsg->orderPrice);
		m_pLabelTotalPrice->SetText(strTotalPrice);
		m_pLabelTotalPrice->SetFixedWidth(20);//多线程里设置一次宽度才能自动设置大小
		CString strFreight;
		strFreight.Format("%lg", pOrderMsg->orderFreight);
		m_pLabelFreight->SetText(strFreight);
		m_pLabelFreight->SetFixedWidth(20);
	}
	else
	{
		m_pLabelTotalPrice->SetVisible(false);
		m_pLabelFreight->SetVisible(false);
		m_pLabelTotalPriceLeft->SetVisible(false);
		m_pLabelTotalFreightLeft->SetVisible(false);
		m_pLabelTotalFreightRight->SetVisible(false);
	}
}

void CDuiOrderNotice::SetOrderMsgRead(OrderMsg* pOrderMsg)
{
	for(int nNode = 0; nNode < m_pTreeNodeOrderNew->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeOrderNew->GetChildNode(nNode);
		OrderMsg* pOrderMsgNode = (OrderMsg*)pNode->GetTag();
		if(pOrderMsgNode != NULL && pOrderMsgNode == pOrderMsg)
		{
			DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pNode, _T("labelOrderName")));
			pLabelOrderName->SetFont(0);
			return;
		}
	}
	for(int nNode = 0; nNode < m_pTreeNodeOrderPay->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeOrderPay->GetChildNode(nNode);
		OrderMsg* pOrderMsgNode = (OrderMsg*)pNode->GetTag();
		if(pOrderMsgNode != NULL && pOrderMsgNode == pOrderMsg)
		{
			DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pNode, _T("labelOrderName")));
			pLabelOrderName->SetFont(0);
			return;
		}
	}
	for(int nNode = 0; nNode < m_pTreeNodeOrderContent->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeOrderContent->GetChildNode(nNode);
		OrderMsg* pOrderMsgNode = (OrderMsg*)pNode->GetTag();
		if(pOrderMsgNode != NULL && pOrderMsgNode == pOrderMsg)
		{
			DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pNode, _T("labelOrderName")));
			pLabelOrderName->SetFont(0);
			return;
		}
	}
	for(int nNode = 0; nNode < m_pTreeNodeOrderReturn->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeOrderReturn->GetChildNode(nNode);
		OrderMsg* pOrderMsgNode = (OrderMsg*)pNode->GetTag();
		if(pOrderMsgNode != NULL && pOrderMsgNode == pOrderMsg)
		{
			DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pNode, _T("labelOrderName")));
			pLabelOrderName->SetFont(0);
			return;
		}
	}
}