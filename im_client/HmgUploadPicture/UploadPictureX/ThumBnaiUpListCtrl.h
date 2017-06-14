#ifndef THUMBNAIUPLIST_CTRL_H
#define THUMBNAIUPLIST_CTRL_H

#pragma once

#include "thumbnailistctrl.h"

class CThumBnaiUpListCtrl :	public CThumBnaiListCtrl
{
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	CThumBnaiUpListCtrl(void);
	~CThumBnaiUpListCtrl(void);
public:
	CString m_strOldItemText;
};

#endif