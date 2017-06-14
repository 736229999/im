// SysCommonFastWord.cpp : 实现文件
//

#include "stdafx.h"
#include "SysCommonFastWord.h"
#include "afxdialogex.h"
#include "../im/LocalData.h"
#include "EditFastWordDlg.h"

using namespace DuiLib;

CSysCommonFastWord::CSysCommonFastWord()
{
	g_LocalData.GetFastWord(m_vecFastWord);
}

DuiLib::UILIB_RESOURCETYPE CSysCommonFastWord::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CSysCommonFastWord::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CSysCommonFastWord::AddAutoReply(const char *szAutoReply)
{
	CListLabelElementUI* pListElement = new CListLabelElementUI;  
	pListElement->SetText(szAutoReply);
	m_pList->AddAt(pListElement, 0);
	m_pList->SelectItem(-1);
}

void CSysCommonFastWord::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pList = static_cast<CListUI*>(m_PaintManager.FindControl("listAutoReply"));
	m_pBtnAdd = static_cast<CButtonUI*>(m_PaintManager.FindControl("addAutoReply"));
	m_pBtnEdit = static_cast<CButtonUI*>(m_PaintManager.FindControl("editAutoReply"));
	m_pBtnDel = static_cast<CButtonUI*>(m_PaintManager.FindControl("delAutoReply"));
	m_pBtnUp = static_cast<CButtonUI*>(m_PaintManager.FindControl("upAutoReply"));
	m_pBtnDown = static_cast<CButtonUI*>(m_PaintManager.FindControl("downAutoReply"));

	for(int i = m_vecFastWord.size()-1; i >= 0; i--)
	{
		CString strText = m_vecFastWord[i].c_str();
		AddAutoReply(strText);
	}
}

void CSysCommonFastWord::OnClick(TNotifyUI& msg)
{
	WindowImplBase::OnClick(msg);
	if(msg.pSender == m_pBtnAdd)
	{	// 添加
		OnBnClickedBtnAdd();
	}
	if(msg.pSender == m_pBtnEdit)
	{	// 编辑
		OnBnClickedBtnMod();
	}
	if(msg.pSender == m_pBtnDel)
	{	// 删除
		OnBnClickedBtnDel();
	}
	if(msg.pSender == m_pBtnUp)
	{	// 上移
		int nIndex = m_pList->GetCurSel();
		if(nIndex == -1 || nIndex == 0)
		{
			return;
		}
		CListLabelElementUI* pListElementCur = static_cast<CListLabelElementUI*>(m_pList->GetItemAt(nIndex));
		CListLabelElementUI* pListElementPre = static_cast<CListLabelElementUI*>(m_pList->GetItemAt(nIndex-1));
		string strCur = pListElementCur->GetText();
		string strPre = pListElementPre->GetText();
		pListElementPre->SetText(strCur.c_str());
		pListElementCur->SetText(strPre.c_str());
		m_pList->SelectItem(nIndex-1);
		swap(m_vecFastWord[nIndex], m_vecFastWord[nIndex-1]);
	}
	if(msg.pSender == m_pBtnDown)
	{
		int nIndex = m_pList->GetCurSel();
		if(nIndex == -1 || nIndex == m_pList->GetCount()-1)
		{
			return;
		}
		CListLabelElementUI* pListElementCur = static_cast<CListLabelElementUI*>(m_pList->GetItemAt(nIndex));
		CListLabelElementUI* pListElementNext = static_cast<CListLabelElementUI*>(m_pList->GetItemAt(nIndex+1));
		string strCur = pListElementCur->GetText();
		string strNext = pListElementNext->GetText();
		pListElementNext->SetText(strCur.c_str());
		pListElementCur->SetText(strNext.c_str());
		m_pList->SelectItem(nIndex+1);
		swap(m_vecFastWord[nIndex], m_vecFastWord[nIndex+1]);
	}
}

void CSysCommonFastWord::OnBnClickedBtnAdd()
{
	CEditFastWordDlg dlg("", CWnd::FromHandle(GetHWND()));
	dlg.Create(*this, _T("新增快捷短语"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	dlg.SetTitle(_T("新增快捷短语"));
	dlg.CenterWindow();
	if( IDOK == dlg.ShowModal() )
	{
		CString strText = dlg.GetText();
		AddAutoReply(strText);
		m_vecFastWord.insert(m_vecFastWord.begin(), (LPCTSTR)strText);
	}
}

void CSysCommonFastWord::OnBnClickedBtnMod()
{
	int nIndex = m_pList->GetCurSel();
	if(nIndex == -1)
	{
		return;
	}

	CString strText;
	CEditFastWordDlg dlg(m_vecFastWord[nIndex].c_str(), CWnd::FromHandle(GetHWND()));
	dlg.Create(*this, _T("修改快捷短语"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	dlg.SetTitle(_T("修改快捷短语"));
	dlg.m_reTextOnline.SetText(m_vecFastWord[nIndex].c_str());
	dlg.CenterWindow();
	if( IDOK == dlg.ShowModal() )
	{
		strText = dlg.GetText();
		m_vecFastWord[nIndex] = strText; 
		CListLabelElementUI* pListElement = static_cast<CListLabelElementUI*>(m_pList->GetItemAt(nIndex));
		pListElement->SetText(strText);
	}
}

void CSysCommonFastWord::OnBnClickedBtnDel()
{
	int nIndex = m_pList->GetCurSel();
	if(nIndex == -1)
	{
		return;
	}

	m_pList->RemoveAt(nIndex);
	m_vecFastWord.erase(m_vecFastWord.begin() + nIndex);
	m_pList->SelectItem(-1);
}

void CSysCommonFastWord::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == "itemactivate")
	{
		OnBnClickedBtnMod();
	}
}

// 保存
void CSysCommonFastWord::Save()
{
	g_LocalData.SetFastWord(m_vecFastWord);
}

void CSysCommonFastWord::UpdateBk()
{
	if(::IsWindow(GetHWND()) && m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,28' dest='0,0,%d,28'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,28' dest='0,0,%d,28' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}
}