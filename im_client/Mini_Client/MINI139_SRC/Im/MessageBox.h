// ----------------------------------------------------------------
// �ļ�:MessageBox.h
// �汾: 
// ����: xy
// ����: 2006-6-27
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "..\..\..\..\DuiLib\UIlib.h"
#include <map>
#define WM_MSGBOX (WM_USER+888)		// ֻ�е��Ƿ�ģ̫�Ի���ʱ�ŷ��ʹ���Ϣ��������

class CMessageBox : public DuiLib::WindowImplBase
{
public:
	// ֻ����ģ̬�Ի���ʱ����ֵ����Ч
	static int Prompt(LPCTSTR lpszText, 
		LPCTSTR lpszCaption = NULL,
		UINT nType = MB_OK,
		HWND hParent = NULL,
		BOOL bModel = FALSE);

	// ����һ��ģ̬�Ի���
	static int Model(HWND hWnd, LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

	int GetDlgID() {	return m_nID;	}

	// �������б���
	static void UpdateAllBk();

private:
	CMessageBox(const char *pTitle, const char *pText, UINT nType, HWND hParent, BOOL bModel);
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("MessageBox");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MessageBox.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	// ����һ����ť
	DuiLib::CButtonUI* NewBtn(const char *pName, const char *pText, BOOL bBkBlue=TRUE);
	// ����һ��ռλ��
	DuiLib::CLabelUI* NewLabel(int nWidth=10);
	void Notify(DuiLib::TNotifyUI& msg);
	// ˢ�±���
	void UpdateBk();
	
private:
	static std::map<int, std::tr1::shared_ptr<CMessageBox>> g_mapDlg;
	int m_nID;
	string m_strTitle;
	string m_strText;
	UINT m_nType;
	HWND m_hParent;
	BOOL m_bModel;
};