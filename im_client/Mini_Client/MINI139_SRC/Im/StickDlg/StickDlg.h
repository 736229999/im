// ----------------------------------------------------------------
// �ļ�:StickDlg.h
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

#include <list>

#define _BaseStickDlg	CXSkinDlg
//#define _BaseStickDlg	CDialog

class CStickMainDlg;


enum SHOW_STICK_WND{
	SSW_ATTACH,		// ����
	SSW_HIDE,		// ����
	SSW_CLOSE,		// �ر�
	SSW_DETACH,		// ����
	SSW_AUTO		// �Զ�����
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
	
	// ���Ӵ���������ָ��λ�ã�δ��ɣ�
	int				PlaceWnd(PLACE_STICK_WND nCmdPlace);
	
	// ��ʾ�Ӵ���
	int				ShowWnd(SHOW_STICK_WND nCmdShow);
	
	// ��ȡsizeǰ�Ĵ����С
	const CRect&	GetRectBeforeSize() const
	{ return m_rctLastSize; };
	
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:

	CStickMainDlg *		m_pStickMain;
	CPoint				m_ptDown;		// ���ⱻ�����ʱ�����λ��
	CRect				m_rctLastSize;	// �ϴθտ�ʼsizing��ʱ����Ĵ�С
	CRect				m_rctLastMove;	// �ϴθտ�ʼmoving��ʱ����Ĵ�С
	BOOL				m_bSizeStart;	// �Ƿ�տ�ʼSize����
	BOOL				m_bMoveStart;	// �Ƿ�տ�ʼ�ƶ�����

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
		ATTACH,			// ����
		HIDE,			// ����
		DETACH,			// ����
		COLLAPSE,		// �۵�
		//SUB_ATTACH,		// �������Ӵ���
	};

	enum STICK_ATTACH_DIRECT{
		//UP = 0,		// ��
		//DOWN,			// ��
		LEFT,			// ��
		RIGHT,			// ��	
		//-----------------------
		AUTO			// �Զ�
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

	int				nShowState;	// ��ʾ״̬

	int				nDirect;	// ��������

	STICK_WND *		pWndTop;	// �Ϸ��Ӵ���

	STICK_WND *		pWndBottom;	// �·��Ӵ���

};

typedef std::list<STICK_WND>::iterator StickDlgIt;
