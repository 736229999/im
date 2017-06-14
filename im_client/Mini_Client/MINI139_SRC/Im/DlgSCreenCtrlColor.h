/*====================================================================  
=  �ļ���:CDlgSCreenCtrlTool��                                    
=  �޸�����:2015-5-5                                            
=  ����:wyx                  
=  ����:�������ʹ��ߣ����ڻ���ͼ�εı߿��С����ɫ
====================================================================*/ 
#pragma once

// CDlgSCreenCtrlColor �Ի���
class CScreenCtrl;
class CDlgSCreenCtrlColor : public CDialog
{
	DECLARE_DYNAMIC(CDlgSCreenCtrlColor)

public:
	CDlgSCreenCtrlColor(CWnd* pParent = NULL, CScreenCtrl* pDlgSCreenCtrl = NULL);   // ��׼���캯��
	virtual ~CDlgSCreenCtrlColor();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCREEN_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CStatic m_staticColor;//��ʾѡ����ɫ��picture�ؼ�
	CPngBtn m_btnSmall;//�߿��ȵĴ�СΪС
	CPngBtn m_btnNormal;//�߿��ȵĴ�СΪ��
	CPngBtn m_btnBig;//�߿��ȵĴ�СΪ��
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnSmall();
	afx_msg void OnBnClickedBtnNormal();
	afx_msg void OnBnClickedBtnBig();
	
public:
	void SetRect(int nPicRect, BOOL bIn);//��������Ƿ���ĳ��picture�ؼ��ϣ������ؼ�����Ϊδѡ��
	void DrawAllRectColor();//��������picture�ؼ��ϵ���ɫ
	void DrawRectColor(DWORD dwColor);//������ʾѡ����ɫ��picture�ؼ�
	void DrawGrayLine();//����һ����ɫ�����ߣ�����QQ

private:
	CScreenCtrl* m_pDlgSCreenCtrl;

	UINT m_vecPicId[16];//picture�ؼ�ID
	DWORD m_vecColor[16];//16����ɫ�����ڻ���picture�ؼ�
	BOOL m_bIsInRect[16];//�ж�������ĸ�picture�ؼ�
	BOOL m_bIsInRectOld[16];//�ж�����ϴ����ĸ�picture�ؼ�
	int m_nColor;//���ѡ�е���ɫ���
	int m_nSize;//���ѡ�еĴ�С���
};
