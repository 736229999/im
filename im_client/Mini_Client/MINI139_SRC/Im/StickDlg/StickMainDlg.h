// ----------------------------------------------------------------
// �ļ�:StickMainDlg.h
// �汾: 0.10
// ����: xy
// ����: 2006-9-20
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
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
	
	// ��ӱ��йܵ��Ӵ���
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
	// �������Ƿ���Ч
	virtual BOOL	IsVisible() const
	{ return IsWindowVisible() && IsIconic() == FALSE; };

	// ��ʱ��û��ʵ��
	// ��һ��������������������һ����,���ط�0Ϊʧ��
	int			PlaceSubWnd(CStickDlg * pSub,PLACE_STICK_WND nCmdPlace);

	// �ı��Ӵ������ʾ״̬,���ط�0Ϊʧ��
	int			ShowSubWnd(CStickDlg * pSub,SHOW_STICK_WND nCmdShow);


protected:

	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:

	// ������ȡ���������Ӵ����ճ��״̬
	void	MainDetach();

	// �Ӵ����ƶ��¼�(��̬)
	void	OnMovingSubWnd(CStickDlg * pSub,/*IN-OUT*/LPRECT pRct,CPoint& ptDown);

	// �Ӵ����С�Ķ��¼�����̬��
	void	OnSizingSubWnd(CStickDlg * pSub,LPRECT pRct);
	
	// �Ӵ����С�Ķ��¼�
	void	OnSizeSubWnd(CStickDlg * pSub);

	// �Ӵ����ƶ��¼�
	void	OnMoveSubWnd(CStickDlg * pSub);

	// �����Ӵ���
	void	OnActiveSub(CStickDlg * pSub);

	// �����Ӵ���
	void	OnActiveSub(STICK_WND * pSWnd);

	// �ı��Ӵ����С
	void	SizingSubWnd(STICK_WND * pWnd,LPRECT pRct);

	// ����һ���Ӵ����λ��
	StickDlgIt	FindWnd(CStickDlg * pWnd);

	// �������һ����������
	STICK_WND*	FindLastAttachWnd(int nDirect);

	// ���ҵ�һ����������
	StickDlgIt	FindFirstAttachWnd(int nDirect);
	
	// ���Ӵ���ճ��ͬ�����������Ե��Ӵ���֮��
	bool		AttachWnd(StickDlgIt itWnd,/*OUT*/LPRECT pRect);

	// �Զ�ճ�����嵽������
	bool		AutoAttachWnd(StickDlgIt itWnd, /*OUT*/LPRECT pRect);

	// ���Ӵ���ճ����һ���Ӵ�����
	void		AttachSubWnd(StickDlgIt itWnd,StickDlgIt itTopWnd);

	// ����ָ��λ�õ��Ӵ���
	void		DetachWnd(STICK_WND * pSWnd);

	// �ر�ָ��λ�õ��Ӵ���
	void		CloseWnd(StickDlgIt itWnd);

	// ����ָ��λ�õ��Ӵ���
	void		HideWnd(StickDlgIt itWnd);

	// ��С��ָ��λ�õ��Ӵ���
	//void		MinimizeWnd(StickDlgIt itWnd);	

	// �������崹ֱ��ˮƽ�����ƶ�(���µ���������Ҳ��֮�ƶ�)
	void		MoveWnd(STICK_WND * pSWnd,int nDeltaVert,int nDeltaHori);

	// �Ӵ���������������
	bool		SubAttachMainTest(
		/*IN*/StickDlgIt	itWnd,		// ���ⴰ��
		/*IN-OUT*/LPRECT	pRct,		// ��λ��
		/*IN*/CPoint		pt,			// ��ǰ���λ��
		/*IN-OUT*/CPoint &	ptDown		// ��갴��ʱ��λ��
	);

	// �Ӵ��������Ӵ�����
	int			SubAttachSubTest(
		/*IN*/StickDlgIt	itWnd,		// ���ⴰ��
		/*IN-OUT*/LPRECT	pRct,		// ��λ��
		/*IN*/CPoint		pt,			// ��ǰ���λ��
		/*IN-OUT*/CPoint &	ptDown		// ��갴��ʱ��λ��
	);

	// ���Ӵ�������ϲ��Ӵ����Ϸ������
	bool		SubDetachSubTest(
		/*IN*/StickDlgIt	itWnd,		// ���ⴰ��
		/*IN-OUT*/LPRECT	pRct,		// ��λ��
		/*IN*/CPoint		pt,			// ��ǰ���λ��
		/*IN-OUT*/CPoint&	ptDown		// ��갴��ʱ��λ��
	);	

	// ���Ӵ�����������Ϸ������
	bool		SubDetachMainTest(
		/*IN*/StickDlgIt	itWnd,		// ���ⴰ��
		/*IN-OUT*/LPRECT	pRct,		// ��λ��
		/*IN*/CPoint		pt,			// ��ǰ���λ��
		/*IN-OUT*/CPoint&	ptDown		// ��갴��ʱ��λ��
	);	
	
	// ��������������Ƿ�ճ�����Ӵ���
	bool		MainAttachLeftTest(LPRECT pRct);

	// �����������ұ��Ƿ�ճ�����Ӵ���
	bool		MainAttachRightTest(LPRECT pRct);

	// ɾ���������Ķ����Ӵ���
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

	// �ж�һ�������Ƿ�Ϊ�������Ķ����Ӵ���
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
	
	static BOOL			m_bDragFullWindows;	// �����϶�ʱ�����Ƿ�ˢ�£�ע�����ֵ��

protected:
	
	list<STICK_WND>		m_lstWnds;
	STICK_WND *			m_pLeftTopSub;	// ��ඥ���Ӵ���
	STICK_WND *			m_pRightTopSub;	// �Ҳඥ���Ӵ���
	CRect				m_rctLastMove;	// �ϴο�ʼ�ƶ�����ʱ������
	CRect				m_rctLastSize;	// �ϴο�ʼ�ı䴰���Сʱ������
	BOOL				m_bMoveStart;	// �Ƿ�ʼ�ƶ� 
	BOOL				m_bSizeStart;	// �Ƿ�ʼ�ı��С

	
protected:

	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
