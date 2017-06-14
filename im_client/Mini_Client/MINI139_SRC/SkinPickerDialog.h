#ifndef SKIN_PICKER_H
#define SKIN_PICKER_H

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"
#include "SkinPickerPictureItem.h"

const TCHAR kSkinPickerPicutreItem[] = _T("SkinPictureItem");
const TCHAR kSkinTab[] =		_T("Skin_Tab");
const TCHAR kOfficialSkin[] =	_T("Skin_OptOfficial");
const TCHAR kMySkin[] =			_T("Skin_OptMy");
const TCHAR kBtnRestore[] =		_T("Skin_BtnRestore");
const TCHAR kBtnListTrans[] =	_T("Skin_BtnListTrans");
const TCHAR kOptSkinTrans[] =	_T("Skin_OptSkinTrans");
const TCHAR kBtnSkinTrans[] =	_T("Skin_BtnSkinTrans");
const TCHAR kBtnPattet[] =		_T("Skin_BtnPattet");
const TCHAR kBtnCustom[] =		_T("Skin_BtnCustom");

	

class CSkinPickerDialog : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CSkinPickerDialog> Show();

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("SkinPickerDialog");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SkinPicker.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void OnFinalMessage(HWND /*hWnd*/);

	void Notify(DuiLib::TNotifyUI& msg);
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass) ;

	static void UpdateBk();
	void   SetForeImg();
protected:
	CWndShadow		 m_WndShadow;
	static std::tr1::shared_ptr<CSkinPickerDialog> pInstance;

private:
	map<CSkinPikerPictureItemUI*,CString> m_mapDlg;
    CSkinPikerPictureItemUI  *m_pSkinPicItem;
};

#endif SKIN_PICKER_H
