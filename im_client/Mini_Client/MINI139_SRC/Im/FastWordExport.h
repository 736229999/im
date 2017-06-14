#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"

//快捷短语导出
class CFastWordExport;
class CFastWordExport : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CFastWordExport> NewDlg(HWND pDlg);
	static std::tr1::shared_ptr<CFastWordExport> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CFastWordExport> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("FastWordExport");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FastWordExport.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void SetParentHwnd(HWND pDlg);
	void ExportExcel(CString strFileName);//导出Excel
	void ExportCsv(CString strFileName);//导出Csv
private:
	HWND m_parent;
	DuiLib::COptionUI *m_pRedioExcel;
	DuiLib::COptionUI *m_pRedioCsv;
public:
	CFastWordExport(void);
	~CFastWordExport(void);
};

