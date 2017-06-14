/*====================================================================  
=  �ļ���:CSelCustomHeadIconDlg��                                    
=  �޸�����:2015-6-10                                            
=  ����:wyx                  
=  ����:�����Զ���ͷ���ϴ�
====================================================================*/ 
#pragma once
#include "GdiPlus.h"
#include "im/RectTrackerEx.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

// CSelCustomHeadIconDlg �Ի���

class CSelCustomHeadIconDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSelCustomHeadIconDlg)

public:
	CSelCustomHeadIconDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelCustomHeadIconDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SEL_CUSTOM_HEAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnSelectpic();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CPngBtn m_btnSelectPic;
public:
	ULONG_PTR m_pGdiToken;//gdi��ʼ��
	CString m_strFilePath;//�ļ�·��
	CStatic m_staticPic;//��ʾ�ϴ��ļ�������
	int m_nTrackerRectWidth;//Ԥ��ͷ���ȣ�88
	int m_nTrackerRectHeight;//Ԥ��ͷ��߶ȣ�88
	CRect m_rect;//�ϴ��ļ�������
	CRect m_rectPic;//�ϴ��ļ�������
	CRect m_rectLast;//ѡ������仯ǰ���������ڼ�����Ҫˢ�µ�����
	CRectTrackerEx m_rectTracker;//���ƿ�ѡ����ı߿�
	CBitmap m_bitmap;//����ͼƬ������ˢ�±���
	int m_nHandle;//�жϵ�ǰ���״̬���������״̬���������ʽ

	//����ƶ���λ��,�����϶�ѡ�������ı�ѡ������Ĵ�С
	int m_nDownX;
	int m_nDownY;
	int m_nMoveX;
	int m_nMoveY;


	
};
