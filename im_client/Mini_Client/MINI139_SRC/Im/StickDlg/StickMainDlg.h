// ----------------------------------------------------------------
// 文件:StickMainDlg.h
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

#include "StickDlg.h"
#include <list>
using namespace std;

#define _BaseStickMainDlg	CXSkinDlg
//#define _BaseStickMainDlg	CDialog


class CStickMainDlg : public _BaseStickMainDlg
{
	DECLARE_DYNAMIC(CStickMainDlg)
	friend class CStickDlg;
public:

	explicit CStickMainDlg(UINT nIDTemplate, CWnd * pParent = NULL);

	virtual ~CStickMainDlg();

public:

	enum SUBATTACHSUB_TYPE{ATTACH_TOP,ATTACH_BOTTOM,ATTACH_NONE};
	
	// 添加被托管的子窗体
	void			AddStickDlg(CStickDlg * pDlg)
	{
		ASSERT(pDlg);
		STICK_WND	wnd;
		StickDlgIt	it;
		if(FindWnd(pDlg) == m_lstWnds.end())
		{
			wnd.pWnd = pDlg;
			wnd.nShowState = STICK_WND::HIDE;
			m_lstWnds.push_back(wnd);
		}
	};
	// 主窗体是否有效
	virtual BOOL	IsVisible() const
	{ return IsWindowVisible() && IsIconic() == FALSE; };

	// 暂时还没有实现
	// 将一个窗体吸附到主窗体任一方向,返回非0为失败
	int			PlaceSubWnd(CStickDlg * pSub,PLACE_STICK_WND nCmdPlace);

	// 改变子窗体的显示状态,返回非0为失败
	int			ShowSubWnd(CStickDlg * pSub,SHOW_STICK_WND nCmdShow);


protected:

	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:

	// 主窗体取消和其他子窗体的粘附状态
	void	MainDetach();

	// 子窗体移动事件(动态)
	void	OnMovingSubWnd(CStickDlg * pSub,/*IN-OUT*/LPRECT pRct,CPoint& ptDown);

	// 子窗体大小改动事件（动态）
	void	OnSizingSubWnd(CStickDlg * pSub,LPRECT pRct);
	
	// 子窗体大小改动事件
	void	OnSizeSubWnd(CStickDlg * pSub);

	// 子窗体移动事件
	void	OnMoveSubWnd(CStickDlg * pSub);

	// 激活子窗体
	void	OnActiveSub(CStickDlg * pSub);

	// 激活子窗体
	void	OnActiveSub(STICK_WND * pSWnd);

	// 改变子窗体大小
	void	SizingSubWnd(STICK_WND * pWnd,LPRECT pRct);

	// 查找一个子窗体的位置
	StickDlgIt	FindWnd(CStickDlg * pWnd);

	// 查找最后一个吸附窗体
	STICK_WND*	FindLastAttachWnd(int nDirect);

	// 查找第一个吸附窗体
	StickDlgIt	FindFirstAttachWnd(int nDirect);
	
	// 将子窗体粘到同方向主窗体旁的子窗体之下
	bool		AttachWnd(StickDlgIt itWnd,/*OUT*/LPRECT pRect);

	// 自动粘贴窗体到主窗口
	bool		AutoAttachWnd(StickDlgIt itWnd, /*OUT*/LPRECT pRect);

	// 将子窗体粘到另一个子窗体下
	void		AttachSubWnd(StickDlgIt itWnd,StickDlgIt itTopWnd);

	// 分离指定位置的子窗体
	void		DetachWnd(STICK_WND * pSWnd);

	// 关闭指定位置的子窗体
	void		CloseWnd(StickDlgIt itWnd);

	// 隐藏指定位置的子窗体
	void		HideWnd(StickDlgIt itWnd);

	// 最小化指定位置的子窗体
	//void		MinimizeWnd(StickDlgIt itWnd);	

	// 吸附窗体垂直和水平方向移动(其下的其他窗体也随之移动)
	void		MoveWnd(STICK_WND * pSWnd,int nDeltaVert,int nDeltaHori);

	// 子窗体吸附主窗体检测
	bool		SubAttachMainTest(
		/*IN*/StickDlgIt	itWnd,		// 待测窗体
		/*IN-OUT*/LPRECT	pRct,		// 新位置
		/*IN*/CPoint		pt,			// 当前鼠标位置
		/*IN-OUT*/CPoint &	ptDown		// 鼠标按下时的位置
	);

	// 子窗体吸附子窗体检测
	int			SubAttachSubTest(
		/*IN*/StickDlgIt	itWnd,		// 待测窗体
		/*IN-OUT*/LPRECT	pRct,		// 新位置
		/*IN*/CPoint		pt,			// 当前鼠标位置
		/*IN-OUT*/CPoint &	ptDown		// 鼠标按下时的位置
	);

	// 将子窗体从它上层子窗体上分离测试
	bool		SubDetachSubTest(
		/*IN*/StickDlgIt	itWnd,		// 待测窗体
		/*IN-OUT*/LPRECT	pRct,		// 新位置
		/*IN*/CPoint		pt,			// 当前鼠标位置
		/*IN-OUT*/CPoint&	ptDown		// 鼠标按下时的位置
	);	

	// 将子窗体从主窗体上分离测试
	bool		SubDetachMainTest(
		/*IN*/StickDlgIt	itWnd,		// 待测窗体
		/*IN-OUT*/LPRECT	pRct,		// 新位置
		/*IN*/CPoint		pt,			// 当前鼠标位置
		/*IN-OUT*/CPoint&	ptDown		// 鼠标按下时的位置
	);	
	
	// 测试主窗体左边是否粘上了子窗体
	bool		MainAttachLeftTest(LPRECT pRct);

	// 测试主窗体右边是否粘上了子窗体
	bool		MainAttachRightTest(LPRECT pRct);

	// 删除主窗体侧的顶部子窗体
	void		DelTopSub(STICK_WND * pSWnd)
	{
		ASSERT(pSWnd);
		if(pSWnd == m_pLeftTopSub)
		{
			m_pLeftTopSub = NULL;
		}
		else if(m_pRightTopSub)
		{
			m_pRightTopSub = NULL;
		}
	};	

	// 判断一个窗体是否为主窗体侧的顶层子窗体
	STICK_WND * IsTopSub(STICK_WND * pSWnd)
	{
		ASSERT(pSWnd);
		if(pSWnd == NULL)
			return NULL;
		bool bFind = false;
		if(pSWnd->nDirect == STICK_WND::LEFT)
			bFind = (pSWnd == m_pLeftTopSub);
		else if(pSWnd->nDirect == STICK_WND::RIGHT)
			bFind = (pSWnd == m_pRightTopSub);
		if(bFind)
			return pSWnd;
		else
			return NULL;
	};
	
	

public:
	
	static BOOL			m_bDragFullWindows;	// 窗体拖动时内容是否刷新（注册表项值）

protected:
	
	list<STICK_WND>		m_lstWnds;
	STICK_WND *			m_pLeftTopSub;	// 左侧顶层子窗口
	STICK_WND *			m_pRightTopSub;	// 右侧顶层子窗口
	CRect				m_rctLastMove;	// 上次开始移动窗体时的区域
	CRect				m_rctLastSize;	// 上次开始改变窗体大小时的区域
	BOOL				m_bMoveStart;	// 是否开始移动 
	BOOL				m_bSizeStart;	// 是否开始改变大小

	
protected:

	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
