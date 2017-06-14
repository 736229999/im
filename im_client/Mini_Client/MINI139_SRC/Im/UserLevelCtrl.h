// ----------------------------------------------------------------
// 文件:UserLevelCtrl.h
// 版本: 
// 作者: xy
// 日期: 2006-6-7
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once
	
#define LEVEL_ICON_HEIGHT	16	// 图标高度
#define LEVEL_ICON_WIDTH	16	// 图标宽度
#define LEVEL_STEP			5	// 进制


//------------------------------------------------------
// 用户等级显示控件
//------------------------------------------------------

class CUserLevelCtrl : public CStatic
{
	DECLARE_DYNAMIC(CUserLevelCtrl)

public:

	CUserLevelCtrl();

	virtual ~CUserLevelCtrl();

public:

	//------------------------------------------------------
	// 设置用户在线时间
	//------------------------------------------------------
	void	SetOnlineTime(int nTime);
	//------------------------------------------------------
	// 显示提示信息
	//------------------------------------------------------
	void	ShowToolTip(const CString& sTip);
	//------------------------------------------------------
	// 设置用户等级
	//------------------------------------------------------
	void	SetLevel(int nLevel);


	void    SetBkColor(COLORREF clr);

	void	SetTextColor(COLORREF clr);

	// 设置字体
	void	SetFont(CFont &font);
protected:

	//------------------------------------------------------
	// 根据等级计算出太阳，月亮，星星的个数
	//------------------------------------------------------
	void	CountIcon(int nLevel);
	//------------------------------------------------------
	// 通过在线时长计算等级和达到下一级还需要的天数
	//------------------------------------------------------
	int		CountLevel(int nTime,int&nLeftTime);
	//------------------------------------------------------
	// 计算图标和文字所占的区域 
	//------------------------------------------------------
	void	RecalcLayerout(CDC* pDC);

protected:
	
	int				m_nLevel;		// 用户等级
	int				m_nSun;			// 太阳的个数
	int				m_nMoon;		// 月亮的个数
	int				m_nStar;		// 星星的个数
	int				m_nTime;		// 用户在线时长
	int				m_nLeftTime;	// 升级到下个等级剩下的时间

private:

	CImageList		m_imglistLevel;
	CRect			m_rtIcon;		// 用户等级图标所在区域
	CRect			m_rtTime;		// 文字说明所在的区域(在线时间)
	CRect			m_rtLeft;		// 文字说明所在的区域(剩余时间)
	CFont			m_font;			// 字体
	CToolTipCtrl	m_toolTip;		// 提示控件	
	CString			m_sLastTip;		// 上次的提示信息
	COLORREF		m_clrBackgournd;
	COLORREF		m_clrText;		// 文本颜色

protected:

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


