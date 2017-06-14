#include "StdAfx.h"
#include "FastWordSelectMenu.h"
#include "LocalData.h"
using namespace DuiLib;
std::tr1::shared_ptr<CFastWordSelectMenu> CFastWordSelectMenu::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CFastWordSelectMenu::CFastWordSelectMenu(void)
{
	//m_parent = NULL;
}


CFastWordSelectMenu::~CFastWordSelectMenu(void)
{
}

LRESULT CFastWordSelectMenu::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	if(WM_MOUSEWHEEL == uMsg)
	{

	}
	if(WM_KEYDOWN == uMsg)
	{
		if(wParam == VK_UP || wParam == VK_DOWN)
		{
			if(m_listFastWordSelect->GetCurSel() == -1)
			{
				m_listFastWordSelect->SelectItem(0);
				return 0;
			}
		}
		else if(wParam == VK_RETURN)
		{
			int nSelect = m_listFastWordSelect->GetCurSel();
			if(nSelect != -1)
			{
				CListContainerElementUI* pListElement = (CListContainerElementUI*)m_listFastWordSelect->GetItemAt(nSelect);
				CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("fastword")));
				CString strText = pLabelWord->GetText();
				char* pText = (LPSTR)(LPCSTR)strText;
				::SendMessage(m_parent, WM_FASTWORD_MENU_SELECT, (DWORD)pText, NULL);
			}
			::SendMessage(m_parent, WM_FASTWORD_MENU_HIDE, NULL, NULL);
		}
		else
		{
			::SendMessage(m_parent, WM_KEYDOWN, wParam, lParam);
		}
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CFastWordSelectMenu::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_parent = NULL;
	SetIcon(IDR_MAINFRAME);
	m_listFastWordSelect = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listFastWordSelect")));
}

void CFastWordSelectMenu::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("itemclick"))
	{
		int nSelect = m_listFastWordSelect->GetCurSel();
		if(nSelect != -1)
		{
			CListContainerElementUI* pListElement = (CListContainerElementUI*)m_listFastWordSelect->GetItemAt(nSelect);
			CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("fastword")));
			CString strText = pLabelWord->GetText();
			char* pText = (LPSTR)(LPCSTR)strText;
			::SendMessage(m_parent, WM_FASTWORD_MENU_SELECT, (DWORD)pText, NULL);
			::SendMessage(m_parent, WM_FASTWORD_MENU_HIDE, (DWORD)pText, NULL);
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CFastWordSelectMenu> CFastWordSelectMenu::NewDlg(HWND wnd)
{
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CFastWordSelectMenu>(new CFastWordSelectMenu());
		pInstance->Create(wnd, "快捷短语选择", UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
	}
	::BringWindowToTop(*pInstance);
	pInstance->SetParentHwnd(wnd);
	return pInstance;
}

void CFastWordSelectMenu::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

void CFastWordSelectMenu::SetFastWordSelectMenu(vector<FastWordGroupInfo> vecFastWordGroupInfo, CString strSearch)
{
	m_listFastWordSelect->RemoveAll();
	for(int nGroup = 0; nGroup < vecFastWordGroupInfo.size(); nGroup++)
	{
		FastWordGroupInfo& fastWordGroupInfo = vecFastWordGroupInfo[nGroup];
		for(int nFastWord = 0; nFastWord < fastWordGroupInfo.vecFastWordInfo.size(); nFastWord++)
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWord];
			if(strSearch != "")
			{
				BOOL bIsSame = TRUE;
				CString strNum = fastWordInfo.strNum;
				for(int nSearch = 0; nSearch < strSearch.GetLength(); nSearch++)
				{
					CString str = strSearch.GetAt(nSearch);
					if(nSearch > strNum.GetLength() || str != strNum.GetAt(nSearch))
					{
						bIsSame = FALSE;
					}
				}
				if(!bIsSame)
					continue;
			}
			DuiLib::CDialogBuilder builder;
			CListContainerElementUI* pElement = static_cast</*CListContainerElementUI*/CListContainerElementUI*>(builder.Create(_T("FastWordSelectMenuElement.xml"), (UINT)0, NULL));

			CLabelUI* pLabelNum = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pElement, _T("fastwordnum")));
			pLabelNum->SetText(fastWordInfo.strNum);
			CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pElement, _T("fastword")));
			pLabelWord->SetText(fastWordInfo.strWord);
			m_listFastWordSelect->Add(pElement);
		}
	}
	if(m_listFastWordSelect->GetCount() > 0)
		m_listFastWordSelect->SelectItem(/*-1*/0);//快捷短语默认选中第一行
	if(m_listFastWordSelect->GetCount() == 1)
	{
		CListContainerElementUI* pElement = (CListContainerElementUI*)m_listFastWordSelect->GetItemAt(0);
		CLabelUI* pLabelNum = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pElement, _T("fastwordnum")));
		CString strText = pLabelNum->GetText();
		CLabelUI* pLabelWord = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pElement, _T("fastword")));
		CString strWord = pLabelWord->GetText();
		if(strText == strSearch)
		{
			char* pText = (LPSTR)(LPCSTR)strWord;
			//如果只有一个快捷短语就直接显示出来，可以去掉
			//::SendMessage(m_parent, WM_FASTWORD_MENU_SELECT_FIRST, (DWORD)pText, 0);
		}
		m_listFastWordSelect->SelectItem(/*-1*/0);
	}
	if(m_listFastWordSelect->GetCount() == 0)
	{
		::SendMessage(m_parent, WM_FASTWORD_MENU_HIDE, NULL, NULL);
	}
}