/*====================================================================  
=  �ļ���:CScreenCtrl��                                    
=  �޸�����:2015-5-5                                            
=  ����:wyx                  
=  ����:���ڽ�����Ч������QQ
=  ʹ�÷���:
=  CScreenCtrl dlg(GetDesktopWindow());
=  dlg.m_strImageFileCachePath = g_LocalData.GetImageFileCachePath();
=  if ( IDOK==dlg.DoModal() )
=  {
=      CString strCacheFileName;
=      g_LocalData.SaveImageFileToCache(dlg.m_strImageFile,strCacheFileName);
=      CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

=      DeleteFile(dlg.m_strImageFile);

=      this->m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());//���͵������
}
====================================================================*/ 
#pragma once
#include <vector>
#include "RectTrackerEx.h"
#include "EditScreen.h"
// CScreenCtrl �Ի���
class CRectTrackerEx;
class CDlgSCreenCtrlTool;
#include "GdiPlus.h"
using namespace Gdiplus;

#define WM_EDIT_PIC_SAVE WM_USER+1
#define WM_DRAW_RECTTRACKER WM_USER+2

typedef struct tARROWSTRUCT {
	int nWidth;
	float fTheta;
	bool bFill;
} ARROWSTRUCT;

typedef CArray< CBitmap*, CBitmap* > CBitmapArray;

class CScreenCtrl : public CDialog
{
	DECLARE_DYNAMIC(CScreenCtrl)

public:
	CScreenCtrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenCtrl();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	LRESULT OnEditPicSave(WPARAM,LPARAM);
	LRESULT DrawRectTracker(WPARAM,LPARAM);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMove(int x, int y);

public:
	HBITMAP CopyBitmap(HBITMAP hSourceHbitmap);//����bmpͼƬ
	void SetDrawColor(DWORD dwColor);//���û���ͼ�ε���ɫ
	void SetDrawSize(int nSize);//���û���ͼ�εĿ��
	void SetFontSize(int nSize);//�������ֱ༭�Ĵ�С
	void DonePic();//��ɽ���
	void SavePic(CString strFilePath);//����ͼƬ
	void Revoke();//�����༭
public:
	CString m_strImageFile;//������ļ���
	CString m_strImageFileCachePath;//������ļ�·��

	BOOL m_bIsDrawRectangle;//�ж��Ƿ�Ϊ���ƾ���״̬
	BOOL m_bIsDrawEllipse;//�ж��Ƿ�Ϊ����Բ��״̬
	BOOL m_bIsDrawArrow;//�ж��Ƿ�Ϊ���Ƽ�ͷ״̬
	BOOL m_bIsDrawEdit;//�ж��Ƿ�Ϊ�༭����״̬
	BOOL m_bEditFlag;//�ж��Ƿ���Ҫ�������ֱ༭���������ֱ༭״̬ʱѡ���������༭����
private:
	unsigned long m_pGdiToken;//GDI+���߳�ʼ��

	CEditScreen m_edit;//���ֱ༭�ı༭��

	CDlgSCreenCtrlTool* m_pDlgSCreenCtrlTool;//��ͼ�༭����

	CRectTrackerEx m_rectTracker;//���ƿ�ѡ����ı߿�
	BOOL m_bIsDrawBack;//�ж��Ƿ��Ѿ������˱���
	CSize      m_szScreen;//��ǰ��Ļ�Ĵ�С

	CBitmap    m_bitmapScreen;//��ǰ��ĻͼƬ
	CBitmap    m_bitmapBack;//��ǰ��Ļ����ͼƬ������˫����
	CBitmap    m_bitmapLast;//���һ�α����ͼƬ�����ڱ���ͼƬʱȥ���߿�

	std::vector<CRect> m_vecRc;//���㼶˳��洢��ǰϵͳ���еĴ���
	CRect m_rect;//ѡ������ı߿�
	CRect m_rectLast;//ѡ������仯ǰ���������ڼ�����Ҫˢ�µ�����
	CRect m_rectOld;//�ж���굱ǰλ�õĴ����Ƿ�Ϊ��һ�εĴ��ڣ�����ˢ�´��ڲ��񣬷�ֹ����

	CRect m_rectTool;
	CRect m_rectToolLast;

	BOOL m_bIsBtnDown;//�ж�����Ƿ��£������ж��Ƿ�ѡ��������
	BOOL m_bIsBtnDownFirst;//�ж�����Ƿ��һ�ΰ��£������ж����϶�������ѡ������
	BOOL m_bIsMouseUp;//�ж�����Ƿ�̧���������»���ѡ�������

	//��갴�µ�λ��,�����϶�ѡ�������ı�ѡ������Ĵ�С
	int m_nDownX;
	int m_nDownY;
	//����ƶ���λ��,�����϶�ѡ�������ı�ѡ������Ĵ�С
	int m_nMoveX;
	int m_nMoveY;

	//ѡ������仯ʱ����������ˢ�£���ֹ����
	CRect m_rectRefresh1;
	CRect m_rectRefresh2;
	CRect m_rectRefresh3;

	//ѡ������仯ʱ����������ˢ�£���ֹ����
	CRect m_rectRefresh4;
	CRect m_rectRefresh5;
	CRect m_rectRefresh6;

	int m_nHandle;//�жϵ�ǰ���״̬���������״̬���������ʽ

	CPoint m_ptStart;//����ͼ��ʱ����ʼλ��
	CPoint m_ptOld;//��һ�λ���ͼ�ε�λ�ã����ڲ�����һ�λ��Ƶ�ͼ��
	BOOL m_bMouseDown;//�ж��Ƿ�Ϊ����ͼ��ʱ��갴��

	CBitmapArray m_arrayBitmap;//�洢��Ҫ���Ƶ�ͼƬ�����ڳ����༭
	CBitmapArray m_arrayBitmapAll;//�洢���л��Ƶ�ͼƬ�������ͷ��ڴ�

	//��һ�λ��Ƽ�ͷ���������㣬���ڲ�����һ�λ��Ƶļ�ͷ
	CPoint m_pt1Old;
	CPoint m_pt2Old;

	DWORD m_dwDrawColor;//������ɫ
	int m_nDrawSize;//���ƴ�С(���)
	int m_nFontSize;//���ִ�С

};