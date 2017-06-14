// ----------------------------------------------------------------
// �ļ�:UserLevelCtrl.h
// �汾: 
// ����: xy
// ����: 2006-6-7
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once
	
#define LEVEL_ICON_HEIGHT	16	// ͼ��߶�
#define LEVEL_ICON_WIDTH	16	// ͼ����
#define LEVEL_STEP			5	// ����


//------------------------------------------------------
// �û��ȼ���ʾ�ؼ�
//------------------------------------------------------

class CUserLevelCtrl : public CStatic
{
	DECLARE_DYNAMIC(CUserLevelCtrl)

public:

	CUserLevelCtrl();

	virtual ~CUserLevelCtrl();

public:

	//------------------------------------------------------
	// �����û�����ʱ��
	//------------------------------------------------------
	void	SetOnlineTime(int nTime);
	//------------------------------------------------------
	// ��ʾ��ʾ��Ϣ
	//------------------------------------------------------
	void	ShowToolTip(const CString& sTip);
	//------------------------------------------------------
	// �����û��ȼ�
	//------------------------------------------------------
	void	SetLevel(int nLevel);


	void    SetBkColor(COLORREF clr);

	void	SetTextColor(COLORREF clr);

	// ��������
	void	SetFont(CFont &font);
protected:

	//------------------------------------------------------
	// ���ݵȼ������̫�������������ǵĸ���
	//------------------------------------------------------
	void	CountIcon(int nLevel);
	//------------------------------------------------------
	// ͨ������ʱ������ȼ��ʹﵽ��һ������Ҫ������
	//------------------------------------------------------
	int		CountLevel(int nTime,int&nLeftTime);
	//------------------------------------------------------
	// ����ͼ���������ռ������ 
	//------------------------------------------------------
	void	RecalcLayerout(CDC* pDC);

protected:
	
	int				m_nLevel;		// �û��ȼ�
	int				m_nSun;			// ̫���ĸ���
	int				m_nMoon;		// �����ĸ���
	int				m_nStar;		// ���ǵĸ���
	int				m_nTime;		// �û�����ʱ��
	int				m_nLeftTime;	// �������¸��ȼ�ʣ�µ�ʱ��

private:

	CImageList		m_imglistLevel;
	CRect			m_rtIcon;		// �û��ȼ�ͼ����������
	CRect			m_rtTime;		// ����˵�����ڵ�����(����ʱ��)
	CRect			m_rtLeft;		// ����˵�����ڵ�����(ʣ��ʱ��)
	CFont			m_font;			// ����
	CToolTipCtrl	m_toolTip;		// ��ʾ�ؼ�	
	CString			m_sLastTip;		// �ϴε���ʾ��Ϣ
	COLORREF		m_clrBackgournd;
	COLORREF		m_clrText;		// �ı���ɫ

protected:

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


