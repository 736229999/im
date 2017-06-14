#include "StdAfx.h"
#include "DuiMsgNotice.h"
#include "MsgDlg.h"

CDuiMsgNotice::CDuiMsgNotice(void)
{
}


CDuiMsgNotice::~CDuiMsgNotice(void)
{
}

std::tr1::shared_ptr<CDuiMsgNotice> CDuiMsgNotice::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

tr1::shared_ptr<CDuiMsgNotice> CDuiMsgNotice::Show(TranMsg* tranMsgInfo)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->AddMsg(tranMsgInfo);
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiMsgNotice>(new CDuiMsgNotice());
		pInstance->Create(GetDesktopWindow(), _T("消息通知"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->AddMsg(tranMsgInfo);
		pInstance->ShowSelectOrder(tranMsgInfo);
	}
	
	::FlashWindow(pInstance->GetHWND(), TRUE);
	return pInstance;
}

void CDuiMsgNotice::UpdateBk()
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

void CDuiMsgNotice::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();

	//m_pLabelTotalPrice = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalPrice"));
	//m_pLabelFreight = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelFreight"));

	//m_pLabelTotalPriceLeft = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalPriceLeft"));
	//m_pLabelTotalFreightLeft = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalFreightLeft"));
	//m_pLabelTotalFreightRight = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("labelTotalFreightRight"));

	//m_pLabelTotalPrice->SetFont(2);
	//m_pLabelFreight->SetFont(2);

	m_pTreeView = static_cast<DuiLib::CTreeViewUI*>(m_PaintManager.FindControl(_T("treeMsgType")));

	for(int i = 0; i < 1; i++)
	{
		DuiLib::CDialogBuilder builder;
		DuiLib::CTreeNodeUI* pTreeNode = static_cast<DuiLib::CTreeNodeUI*>(builder.Create(_T("OrederNoticeTreeElement.xml"), (UINT)0, NULL));

		m_pTreeView->Add(pTreeNode);
		CString strOrderType;
		switch (i)
		{
		case 0:
			strOrderType = "系统消息";
			m_pTreeNodeMsgSystem = pTreeNode;
			m_pTreeNodeMsgSystem->SetVisible(true);
			break;
		}
		pTreeNode->GetFolderButton()->SetText(strOrderType);

		m_pTreeView->SetItemExpand(true, pTreeNode);
	}
	m_pTreeView->SetAttribute("visiblefolderbtn", "true");//visiblefolderbtn设置为true放在最后
}

void CDuiMsgNotice::Notify(DuiLib::TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnMoreMsg")
		{
			CDuiMsgDlg::Show();
			if(CDuiMsgDlg::GetInstance() != NULL && CDuiMsgDlg::GetInstance()->GetHWND() != NULL)
			{
				if(m_pCurrentTranMsg != NULL)
				{
					CDuiMsgDlg::GetInstance()->ShowSystemMsgPage();
				}

			}
		}
		if(msg.pSender == m_pBtnAccept)
		{
			AcceptTranMsg();
			DeleteSelectTranMsg();
		}
		else if(msg.pSender == m_pBtnRefuse)
		{
			RefuseTranMsg();
			DeleteSelectTranMsg();
		}
	}
	else if(msg.sType == _T("itemclick"))
	{
		if(msg.pSender->GetParent()->GetParent() == m_pTreeView)
		{
			DuiLib::CTreeNodeUI* pNode = (DuiLib::CTreeNodeUI*)msg.pSender;
			TranMsg* pTranMsg = (TranMsg*)pNode->GetTag();
			ShowSelectOrder(pTranMsg);
			//AfxBeginThread(ShowSelectOrderThread,pOrderMsg);
			//设置当前订单
			//SetCurrentOrder(pOrderMsg);
			//标记为已读
			DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pNode, _T("labelOrderName")));
			pLabelOrderName->SetFont(0);
		}
	}
}

//void CDuiMsgNotice::SetCurrentOrder(OrderMsg* pOrderMsg)
//{
//	m_pCurrentOrderMsg = pOrderMsg;
//}
//
void CDuiMsgNotice::AddMsg(TranMsg* pTranMsg)
{
	DuiLib::CTreeNodeUI* pTreeNode =  m_pTreeNodeMsgSystem;

	for(int nNode = 0; nNode < m_pTreeNodeMsgSystem->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeMsgSystem->GetChildNode(nNode);
		TranMsg* pTranMsgNode = (TranMsg*)pNode->GetTag();
		if(pTranMsgNode != NULL && pTranMsgNode->nCusterId == pTranMsg->nCusterId)
		{
			return;
		}
	}

	std::string strTranReq;
	switch (pTranMsg->nTranState)
	{
	case 1:
		//pTreeNode = m_pTreeNodeMsgTranReq;
		strTranReq = "转接请求";
		break;
	case 2:
		//pTreeNode = m_pTreeNodeMsgTranRefuse;
		strTranReq = "转接拒绝";
		break;
	}
	if(pTreeNode == NULL)
		return;
	if(!pTreeNode->IsVisible())
		pTreeNode->SetVisible();
	DuiLib::CDialogBuilder builder;
	DuiLib::CTreeNodeUI* pTreeNodeChild = static_cast<DuiLib::CTreeNodeUI*>(builder.Create(_T("OrederNoticeTreeNodeElement.xml"), (UINT)0, NULL));

	DuiLib::CLabelUI* pLabelOrderName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pTreeNodeChild, _T("labelOrderName")));
	pLabelOrderName->SetText(strTranReq.c_str());
	pLabelOrderName->SetFont(2);

	pTreeNodeChild->SetTag((DWORD)pTranMsg);

	pTreeNode->AddAt(pTreeNodeChild, 0);
	pTreeNode->GetFolderButton()->SetCheck(true);
}

void CDuiMsgNotice::ShowSelectOrder(TranMsg* pOrderMsg)
{
	m_pCurrentTranMsg = pOrderMsg;
	DuiLib::CVerticalLayoutUI* pVlOrder = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl("VLMsg"));
	pVlOrder->RemoveAll();

	DuiLib::CHorizontalLayoutUI* pHLOrderNum = new DuiLib::CHorizontalLayoutUI();

	pHLOrderNum->SetFixedHeight(30);
	DuiLib::CLabelUI* pLabelOrderNum = new DuiLib::CLabelUI();
	pLabelOrderNum->SetAttribute("padding", "10,0,0,0");

	CString strOrderNum;
	strOrderNum.Format("来自\"%s\"", pOrderMsg->strFromName.c_str());
	pLabelOrderNum->SetAttribute("text", strOrderNum);
	pHLOrderNum->Add(pLabelOrderNum);

	DuiLib::CHorizontalLayoutUI* pHLMid = new DuiLib::CHorizontalLayoutUI();
	pHLOrderNum->Add(pHLMid);

	DuiLib::CLabelUI* pLabelOrderTime = new DuiLib::CLabelUI();
	pLabelOrderTime->SetFixedWidth(130);

	CString strTimeAll,strDate, strTime;
	CTime  tm(pOrderMsg->time);
	strTime=tm.Format("%H:%M:%S");
	strDate=tm.Format("%Y.%m.%d");
	strTimeAll.Format("%s %s", strDate,strTime);
	pLabelOrderTime->SetAttribute("text", strTimeAll);
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
	if(pOrderMsg->nTranState == 1)
		strTitle.Format("买家 <b>%s</b> 将转接给你服务，请确认", pOrderMsg->strCusterName.c_str());
	else if(pOrderMsg->nTranState == 2)
		strTitle.Format("买家 <b>%s</b> 转接失败，%s拒绝该次转接任务", pOrderMsg->strCusterName.c_str(), pOrderMsg->strFromName.c_str());

	pLabelOrderTitle->SetAttribute("text", strTitle);
	pLabelOrderTitle->SetAttribute("showhtml", "true");
	pHLOrderTitle->Add(pLabelOrderTitle);
	pVlOrder->Add(pHLOrderTitle);
	if(pOrderMsg->nTranState == 1)
	{
		DuiLib::CHorizontalLayoutUI* pHLBtn = new DuiLib::CHorizontalLayoutUI();
		pHLBtn->SetFixedHeight(40);

		m_pBtnAccept = new DuiLib::CButtonUI();
		m_pBtnAccept->SetAttribute("padding", "20,10,0,0");
		m_pBtnAccept->SetAttribute("text", "接受");
		m_pBtnAccept->SetAttribute("width", "60");
		m_pBtnAccept->SetAttribute("height", "24");
		m_pBtnAccept->SetAttribute("normalimage", "file='SysBtn\\blue_button_normal.png'");
		m_pBtnAccept->SetAttribute("hotimage", "file='SysBtn\\blue_button_hover.png'");
		m_pBtnAccept->SetAttribute("pushedimage", "file='SysBtn\\blue_button_pressed.png'");
		m_pBtnRefuse = new DuiLib::CButtonUI();
		m_pBtnRefuse->SetAttribute("padding", "20,10,0,0");
		m_pBtnRefuse->SetAttribute("text", "拒绝");
		m_pBtnRefuse->SetAttribute("width", "60");
		m_pBtnRefuse->SetAttribute("height", "24");
		m_pBtnRefuse->SetAttribute("normalimage", "file='SysBtn\\button_normal.png'");
		m_pBtnRefuse->SetAttribute("hotimage", "file='SysBtn\\button_hover.png'");
		m_pBtnRefuse->SetAttribute("pushedimage", "file='SysBtn\\button_pressed.png'");

		pHLBtn->Add(m_pBtnAccept);
		pHLBtn->Add(m_pBtnRefuse);
		pVlOrder->Add(pHLBtn);
	}
}

void CDuiMsgNotice::AcceptTranMsg()
{
	XT_MSG msgTran;
	memset(&msgTran,0,sizeof(msgTran));
	msgTran.ver         = XTREAM_CLIENT_VERSION;
	msgTran.from_id     = g_ImUserInfo.GetId();
	msgTran.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
	msgTran.to_id		= m_pCurrentTranMsg->nFromId;
	msgTran.data_type   = XT_MSG::CUST_SER_TRAN_REQ;

	CString text = "";
	if(1)
	text.Format("tranAck:%d:1", m_pCurrentTranMsg->nCusterId);//接收

	strcpy(msgTran.data, text);
	msgTran.data_len=strlen(text)+1;
	g_pProtocol->SendP2PMsg(msgTran,CIMProtocol::SMT_SERVER);
}

void CDuiMsgNotice::RefuseTranMsg()
{
	XT_MSG msgTran;
	memset(&msgTran,0,sizeof(msgTran));
	msgTran.ver         = XTREAM_CLIENT_VERSION;
	msgTran.from_id     = g_ImUserInfo.GetId();
	msgTran.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
	msgTran.to_id		= m_pCurrentTranMsg->nFromId;
	msgTran.data_type   = XT_MSG::CUST_SER_TRAN_REQ;

	CString text;
	text.Format("tranAck:%d:2", m_pCurrentTranMsg->nCusterId);//拒绝

	strcpy(msgTran.data, text);
	msgTran.data_len=strlen(text)+1;
	g_pProtocol->SendP2PMsg(msgTran,CIMProtocol::SMT_SERVER);
}

void CDuiMsgNotice::DeleteSelectTranMsg()
{
	for(int nNode = 0; nNode < m_pTreeNodeMsgSystem->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeMsgSystem->GetChildNode(nNode);
		TranMsg* pTranMsgNode = (TranMsg*)pNode->GetTag();
		if(pTranMsgNode != NULL && pTranMsgNode == m_pCurrentTranMsg)
		{
			m_pTreeNodeMsgSystem->Remove(pNode);
			break;
		}
	}
	if(m_pTreeNodeMsgSystem->GetCountChild() == 0)
		Close(IDOK);
}

void CDuiMsgNotice::DeleteTranMsg(int nCusterId)
{
	for(int nNode = 0; nNode < m_pTreeNodeMsgSystem->GetCountChild(); nNode++)
	{
		DuiLib::CTreeNodeUI* pNode = m_pTreeNodeMsgSystem->GetChildNode(nNode);
		TranMsg* pTranMsgNode = (TranMsg*)pNode->GetTag();
		if(pTranMsgNode != NULL && pTranMsgNode->nCusterId == nCusterId)
		{
			m_pTreeNodeMsgSystem->Remove(pNode);
			break;
		}
	}
	if(m_pTreeNodeMsgSystem->GetCountChild() == 0)
		Close(IDOK);
}