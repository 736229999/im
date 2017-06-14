/*====================================================================  
=  �ļ���:CDuiGuidePage��                                    
=  �޸�����:2015-6-10                                            
=  ����:wyx                  
=  ����:����������ҳ
====================================================================*/ 
#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

class CDuiGuidePage :
	public DuiLib::WindowImplBase
{
public:
	CDuiGuidePage(void);
	~CDuiGuidePage(void);

public:
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("DuiGuidePage.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;

	void Notify(DuiLib::TNotifyUI& msg);//��Ϣ��Ӧ
	virtual void InitWindow();//��ʼ��
	void SetPage(vector<CString> vecPage);//����������ͼƬ�ļ���
private:
	vector<CString> m_vecPage;//������ͼƬ�ļ���
	int m_nIndexPage;//��ǰѡ��ͼƬ��λ��
	CWndShadow m_WndShadow;//��Ӱ
};

