#pragma once
#include "afxcmn.h"
#include "emotiondlg.h"

// CEmotionManageDlg �Ի���

class CEmotionManageDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CEmotionManageDlg)

public:
	CEmotionManageDlg(int nCurGroup, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEmotionManageDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EMOTION_MANAGE };

	void		AddEmotions(const vector<string>& ems);

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnHdnItemchangedListEmotion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListEmotion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonClose();

	DECLARE_MESSAGE_MAP()

private:
	// �������Զ��������ص��ڴ�λͼ��
	void InitCustomFace();

	void RefreshEmotionList();

	CXpButton           m_btnAdd,m_btnDel,m_btnModify,m_btnClose;
	CListCtrl			m_lsEmotion          ;
	CImageList			m_imgEmotionList     ;

	CustomFaceMap		m_mapCustomFace		 ;	
	CPictureEx					m_picture			 ;
	int					m_nCurGroup;
	
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
