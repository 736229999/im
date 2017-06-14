#pragma once

#include "SpaceData.h"
#include "WebInterfaceWrapper.h"
#include "Resource.h"
#include "stdafx.h"
// CDisplayPictures �Ի��� 
static    int  l_nInit=0;
class CDisplayPictures : public CDialog
{
	DECLARE_DYNAMIC(CDisplayPictures)

public:
	CDisplayPictures(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDisplayPictures();

// �Ի�������
	enum { IDD = IDD_DIALOG_PICTURES_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	int m_cx,m_cy;
	CRect m_rcClose,m_rcPopMenu,m_rcMoveUp,m_rcMoveDown,m_rcClient;
	
	int   m_nFirstPictureTop;   //��¼��һ��ͼƬ��ʾ�Ķ���λ��;
	vector <int>  m_vecPicturesTop;
	int		m_nPictureScrollSpace;
	int    m_nTimer;
	int    m_nUpDownTimer;	

	HCURSOR   m_hCursor;
	BOOL       m_bLButtonDown;
	BOOL       m_bInitPicsTopSuccess;
	int       m_nPicScrollSpeed;
	int       m_nPicturesUpState,m_nPicturesDownState;    //0 Leave 1  Enter  2 Down
	int       m_nMenuButtonState,m_nCloseButtonState;		//0 Leave 1  Enter  2 Down
	//BOOL      m_bPtInRcMenu,m_bPtInRcClose;             //0��m_rcMenu����   1��m_rcClose����
	CRect     m_rcPicturesUp,m_rcPicturesDown;
	int      m_nCurBtn;    
	CToolTipCtrl   m_ToolTip;
	int		 m_nIndex;             //��һ����ʾͼƬ�����;
	int   m_nOffset;              //��һ��ͼƬ��ƫ����;   ��Χ-(BMP_HEIGHT+1)��0;
	

public:
	virtual BOOL OnInitDialog();
protected:
	void		InitDlgInfo();
	int			HitTest(CPoint  point);
	void       DrawFrame(CDC   *pDC);
	void       DrawPicture(CDC  *pDC);
	int        ShowIndexOfFirstPicture();
//	void       InitPicturesTop();
//	void       ReInitPicturesTop();
	void       OpenWebPicture(int nIndex);
	void       InitDlgPosition();
	void		ShowToolTip(CString strTip);
	void		InitToolTip();
	int        GetPictureIndex(CPoint pt);

protected:

	afx_msg LRESULT OnNcHitTest(CPoint point);	
	
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPicturesUp();
	afx_msg void OnPicturesDown();
	afx_msg void OnPicturesStop();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
protected :
	BOOL   NotifyMainDlg();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSpeedLow();
	afx_msg void OnSpeedMedium();
	afx_msg void OnSpeedFast();
	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
public:
	afx_msg void OnDestroy();
};
