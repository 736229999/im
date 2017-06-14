// ----------------------------------------------------------------
// �ļ�:ClusterInfoDlg.h
// �汾: 
// ����: xy
// ����: 2006-6-7
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
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

	// ��Ϣ
	void Notify(DuiLib::TNotifyUI& msg);
	void OnClusterInfoAck(const char *data,WORD wRet);
	static void UpdateBk();
	void SetCluster(XT_CLUSTER_INFO* pCluster);
public:
	HWND m_hDlgWnd;			// ����һ��HWND���Ա㴰�ڹرջ�������ر�ʱ�ܹ��ɹ�ע����̨����Э��
	DuiLib::CLabelUI* m_labelClusterName;
	DuiLib::CLabelUI* m_labelClusterNum;
	DuiLib::CLabelUI* m_labelclusterType;
	DuiLib::CRichEditUI* m_editFamilyDesc;

	static std::tr1::shared_ptr<CClusterInfoDlg> pInstance2;

public:
	CWndShadow m_WndShadow;
	XT_CLUSTER_INFO*	m_pCluster;		// ��ǰ��ʾ��Ⱥ����
};
