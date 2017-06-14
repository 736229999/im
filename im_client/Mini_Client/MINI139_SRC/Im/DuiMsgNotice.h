#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "IMProtocol.h"
#include "LocalData.h"
class CDuiMsgNotice : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()
public:
	CDuiMsgNotice(void);
	~CDuiMsgNotice(void);
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIMsgNotice");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MsgNotice.xml");  }  
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	static tr1::shared_ptr<CDuiMsgNotice> Show(TranMsg* tranMsgInfo);
	static void UpdateBk();
	virtual void InitWindow();
	static std::tr1::shared_ptr<CDuiMsgNotice> GetInstance()	{	return pInstance;	}
	virtual void Notify(DuiLib::TNotifyUI& msg);

	void AddMsg(TranMsg* pTranMsg);
	void ShowSelectOrder(TranMsg* pOrderMsg);

	void AcceptTranMsg();
	void RefuseTranMsg();
	void DeleteSelectTranMsg();
	void DeleteTranMsg(int nCusterId);
private:
	static std::tr1::shared_ptr<CDuiMsgNotice> pInstance;
	TranMsg* m_pCurrentTranMsg;
	DuiLib::CTreeViewUI* m_pTreeView;
	DuiLib::CTreeNodeUI* m_pTreeNodeMsgSystem;
	DuiLib::CTreeNodeUI* m_pTreeNodeMsgTranReq;
	DuiLib::CTreeNodeUI* m_pTreeNodeMsgTranRefuse;

	DuiLib::CButtonUI* m_pBtnAccept;
	DuiLib::CButtonUI* m_pBtnRefuse;
};

