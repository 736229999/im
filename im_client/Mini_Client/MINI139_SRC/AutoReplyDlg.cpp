// AutoReplyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoReplyDlg.h"
#include "afxdialogex.h"
#include "EditAutoReplyDlg.h"
#include "../im/LocalData.h"

using namespace DuiLib;

DuiLib::UILIB_RESOURCETYPE CUIAutoReply::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CUIAutoReply::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CUIAutoReply::AddAutoReply(const char *szAutoReply)
{
	CListLabelElementUI* pListElement = new CListLabelElementUI;  
	pListElement->SetText(szAutoReply);
	m_pList->AddAt(pListElement, 0);
	m_pList->SelectItem(-1);
}

void CUIAutoReply::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pList = static_cast<CListUI*>(m_PaintManager.FindControl("listAutoReply"));
	m_pBtnAddReply = static_cast<CButtonUI*>(m_PaintManager.FindControl("addAutoReply"));
	m_pBtnEditReply = static_cast<CButtonUI*>(m_PaintManager.FindControl("editAutoReply"));
	m_pBtnDelReply = static_cast<CButtonUI*>(m_PaintManager.FindControl("delAutoReply"));
	m_pBtnUpReply = static_cast<CButtonUI*>(m_PaintManager.FindControl("upAutoReply"));
	m_pBtnDownReply = static_cast<CButtonUI*>(m_PaintManager.FindControl("downAutoReply"));

	for(int i = m_vecItem.size()-1; i >= 0; i--)
	{
		CString strText = m_vecItem[i].strText;
		AddAutoReply(strText);
	}
}

void CUIAutoReply::OnClick(TNotifyUI& msg)
{
	WindowImplBase::OnClick(msg);
	if(msg.pSender == m_pBtnAddReply)
	{	// 添加
		OnBnClickedBtnAdd();
	}
	if(msg.pSender == m_pBtnEditReply)
	{	// 编辑
		OnBnClickedBtnMod();
	}
	if(msg.pSender == m_pBtnDelReply)
	{	// 删除
		OnBnClickedBtnDel();
	}
	if(msg.pSender == m_pBtnUpReply)
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
		swap(m_vecItem[nIndex], m_vecItem[nIndex-1]);
	}
	if(msg.pSender == m_pBtnDownReply)
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
		swap(m_vecItem[nIndex], m_vecItem[nIndex+1]);
	}
}

void CUIAutoReply::OnBnClickedBtnAdd()
{
	CEditAutoReplyDlg dlg("", g_ImFont, CWnd::FromHandle(GetHWND()));
	dlg.Create(*this, _T("新增自动回复"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	dlg.SetTitle(_T("新增自动回复"));
	dlg.CenterWindow();
	if(IDOK == dlg.ShowModal())
	{
		AUTO_REPLY item;
		item.strText = dlg.GetText();
		item.font = dlg.GetFont();
		AddAutoReply(item.strText);
		m_vecItem.insert(m_vecItem.begin(), item);
	}
}

void CUIAutoReply::OnBnClickedBtnMod()
{
	int nIndex = m_pList->GetCurSel();
	if(nIndex == -1)
	{
		return;
	}

	AUTO_REPLY item = m_vecItem[nIndex];
	CEditAutoReplyDlg dlg(item.strText, item.font, CWnd::FromHandle(GetHWND()));
	dlg.Create(*this, _T("修改自动回复"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	dlg.SetTitle(_T("修改自动回复"));
	dlg.m_reTextOnline.SetXFont(item.font);
	dlg.m_reTextOnline.SetText(item.strText);
	dlg.CenterWindow();
	if(IDOK == dlg.ShowModal())
	{
		item.strText = dlg.GetText();
		item.font = dlg.GetFont();
		CListLabelElementUI* pListElement = static_cast<CListLabelElementUI*>(m_pList->GetItemAt(nIndex));
		pListElement->SetText(item.strText);
		m_vecItem[nIndex] = item;
	}
}

void CUIAutoReply::OnBnClickedBtnDel()
{
	int nIndex = m_pList->GetCurSel();
	if(nIndex == -1)
	{
		return;
	}

	m_pList->RemoveAt(nIndex);
	m_vecItem.erase(m_vecItem.begin() + nIndex);
	m_pList->SelectItem(-1);
}

void CUIAutoReply::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	if(msg.sType == "itemactivate")
	{
		OnBnClickedBtnMod();
	}
}

// 保存
void CUIAutoReply::Save()
{
	g_LocalData.SetAllAutoReplyItem(m_vecItem);
}