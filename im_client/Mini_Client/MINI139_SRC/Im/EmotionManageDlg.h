#pragma once
#include "afxcmn.h"
#include "emotiondlg.h"

// CEmotionManageDlg 对话框

class CEmotionManageDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CEmotionManageDlg)

public:
	CEmotionManageDlg(int nCurGroup, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmotionManageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EMOTION_MANAGE };

	void		AddEmotions(const vector<string>& ems);

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
	// 将所有自定义表情加载到内存位图中
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
