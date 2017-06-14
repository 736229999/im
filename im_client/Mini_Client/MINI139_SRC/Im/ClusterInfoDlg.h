// ----------------------------------------------------------------
// 文件:ClusterInfoDlg.h
// 版本: 
// 作者: xy
// 日期: 2006-6-7
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once
#include "..\stdafx.h"
#include "impublic.h"
#include "..\resource.h"
#include "IMProtocol.h"
#include "afxwin.h"
#include "clustericonctrl.h"
#include "..\controls\skinctrl.h"
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


class CClusterInfoDlg :  public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	static std::tr1::shared_ptr<CClusterInfoDlg> NewDlg(XT_CLUSTER_INFO* pCluster);
	static std::tr1::shared_ptr<CClusterInfoDlg> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("ClusterInfo");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("ClusterInfo.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);

	// 消息
	void Notify(DuiLib::TNotifyUI& msg);
	void OnClusterInfoAck(const char *data,WORD wRet);
	static void UpdateBk();
	void SetCluster(XT_CLUSTER_INFO* pCluster);
public:
	HWND m_hDlgWnd;			// 保存一份HWND，以便窗口关闭或非正常关闭时能够成功注销后台交互协议
	DuiLib::CLabelUI* m_labelClusterName;
	DuiLib::CLabelUI* m_labelClusterNum;
	DuiLib::CLabelUI* m_labelclusterType;
	DuiLib::CRichEditUI* m_editFamilyDesc;

	static std::tr1::shared_ptr<CClusterInfoDlg> pInstance2;

public:
	CWndShadow m_WndShadow;
	XT_CLUSTER_INFO*	m_pCluster;		// 当前显示的群数据
};
