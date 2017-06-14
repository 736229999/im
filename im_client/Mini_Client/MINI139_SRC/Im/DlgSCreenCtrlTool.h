/*====================================================================  
=  �ļ���:CDlgSCreenCtrlTool��                                    
=  �޸�����:2015-5-5                                            
=  ����:wyx                  
=  ����:�����༭���ߣ����ڻ��ƾ��Ρ�Բ�Ρ���ͷ�����ֱ༭��
====================================================================*/ 
#pragma once
#include "afxwin.h"
#include "buttonext.h"
// CDlgSCreenCtrlTool �Ի���
class CScreenCtrl;
class CDlgSCreenCtrlColor;
class CDlgSCreenCtrlTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgSCreenCtrlTool)

public:
	CDlgSCreenCtrlTool(CWnd* pParent = NULL, CScreenCtrl* pScreenCtrl = NULL);   // ��׼���캯��
	virtual ~CDlgSCreenCtrlTool();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SCREEN_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CPngBtn m_btnRectangle;//���ƾ���
	CPngBtn m_btnEllipse;//����Բ��
	CPngBtn m_btnArrow;//���Ƽ�ͷ
	CPngBtn m_btnEdit;//���ֱ༭
	CPngBtn m_btnSave;//����ͼƬ
	CPngBtn m_btnDone;//��ɽ���
	CPngBtn m_btnClose;//�رս���
	CPngBtn m_btnRevoke;//�����༭
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRectangle();
	afx_msg void OnBnClickedBtnEllipse();
	afx_msg void OnBnClickedBtnArrow();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnDone();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedBtnRevoke();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void SetAllBtnUp();//�������а���̧��
	void ShowScreenColor();//���ʹ�����ʾ������༭����

private:
	CDlgSCreenCtrlColor* m_pDlgSCreenCtrlColor;//���ʹ��ߣ����ڻ���ͼ�α߿�Ŀ�Ⱥ���ɫ
	CScreenCtrl* m_pScreenCtrl;//�������ߣ������ڽ������������û���ģʽ
};
