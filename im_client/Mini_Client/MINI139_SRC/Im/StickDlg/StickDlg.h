// ----------------------------------------------------------------
// 文件:StickDlg.h
// 版本: 0.10
// 作者: xy
// 日期: 2006-9-20
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include <list>

#define _BaseStickDlg	CXSkinDlg
//#define _BaseStickDlg	CDialog

class CStickMainDlg;


enum SHOW_STICK_WND{
	SSW_ATTACH,		// 吸附
	SSW_HIDE,		// 隐藏
	SSW_CLOSE,		// 关闭
	SSW_DETACH,		// 剥离
	SSW_AUTO		// 自动吸附
};

enum PLACE_STICK_WND{
	PSW_LEFT,
	PSW_RIGHT,
	PSW_AUTO,
};

class CStickDlg : public _BaseStickDlg
{
	DECLARE_DYNAMIC(CStickDlg)

	friend class CStickMainDlg;

public:

	explicit		CStickDlg(UINT nIDTemplate,CStickMainDlg * pStickMain, CWnd * pParent = NULL);
	
	virtual			~CStickDlg();

public:
	
	// 将子窗体吸附到指定位置（未完成）
	int				PlaceWnd(PLACE_STICK_WND nCmdPlace);
	
	// 显示子窗体
	int				ShowWnd(SHOW_STICK_WND nCmdShow);
	
	// 获取size前的窗体大小
	const CRect&	GetRectBeforeSize() const
	{ return m_rctLastSize; };
	
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:

	CStickMainDlg *		m_pStickMain;
	CPoint				m_ptDown;		// 标题被点击下时的鼠标位置
	CRect				m_rctLastSize;	// 上次刚开始sizing的时候窗体的大小
	CRect				m_rctLastMove;	// 上次刚开始moving的时候窗体的大小
	BOOL				m_bSizeStart;	// 是否刚开始Size窗体
	BOOL				m_bMoveStart;	// 是否刚开始移动窗体

public:

	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg void OnClose();
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

struct STICK_WND;




struct STICK_WND{

	enum STICK_WINDOW_STATE{
		ATTACH,			// 吸附
		HIDE,			// 隐藏
		DETACH,			// 分离
		COLLAPSE,		// 折叠
		//SUB_ATTACH,		// 吸附到子窗体
	};

	enum STICK_ATTACH_DIRECT{
		//UP = 0,		// 上
		//DOWN,			// 下
		LEFT,			// 左
		RIGHT,			// 右	
		//-----------------------
		AUTO			// 自动
	};


	STICK_WND()
	{ 
		pWnd = NULL; 
		nShowState = HIDE; 
		nDirect = RIGHT; 
		pWndTop = NULL;
		pWndBottom = NULL;
	};


	CStickDlg * pWnd;

	void GetRect(CRect & rct)
	{ ASSERT(pWnd);pWnd->GetWindowRect(&rct); };

	operator CStickDlg*()
	{ return pWnd; };

	CStickDlg * operator -> ()
	{ return pWnd;};

	bool	IsHide() const
	{ return nShowState == HIDE; };

	bool    IsWindowOutOfScreen() const
	{
		CRect rcWindow;
		pWnd->GetWindowRect(&rcWindow);
		CRect rcScreen(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

		CRect rcInter;
		rcInter.IntersectRect(&rcWindow,&rcScreen);

		if ( rcInter.IsRectEmpty() )
			return true;
		else
			return false;
	}

	bool	IsAttach() const
	{ return nShowState == ATTACH; };

	bool	IsDetach() const
	{ return nShowState == DETACH; };

	int		OppsiDirect()
	{
		switch(nDirect)
		{
		case LEFT:
			return RIGHT;
		case RIGHT:
			return LEFT;
		};
		return -1;
	}

	int				nShowState;	// 显示状态

	int				nDirect;	// 吸附方向

	STICK_WND *		pWndTop;	// 上方子窗体

	STICK_WND *		pWndBottom;	// 下方子窗体

};

typedef std::list<STICK_WND>::iterator StickDlgIt;
