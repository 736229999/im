/*====================================================================  
=  文件名:CDuiGuidePage类                                    
=  修改日期:2015-6-10                                            
=  作者:wyx                  
=  作用:新特性引导页
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

	void Notify(DuiLib::TNotifyUI& msg);//消息响应
	virtual void InitWindow();//初始化
	void SetPage(vector<CString> vecPage);//设置新特性图片文件名
private:
	vector<CString> m_vecPage;//新特性图片文件名
	int m_nIndexPage;//当前选择图片的位置
	CWndShadow m_WndShadow;//阴影
};

