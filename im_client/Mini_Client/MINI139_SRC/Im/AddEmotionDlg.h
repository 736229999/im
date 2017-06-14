#pragma once

#include "emotiondlg.h"
#include "buttonext.h"
//------------------------------------------------------
// ������ӶԻ���
//------------------------------------------------------

class CAddEmotionDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CAddEmotionDlg)

public:
	CAddEmotionDlg(int nGroup,const CString & strImageFile,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddEmotionDlg();

	void SetModifyEmotion(int nCurGroup,int iEmotion);

	void		SetAddPic()
	{ m_bPic = true;};

	int			AddEmotion(const CString & sFile);

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDEMOTION};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	CString		m_strImageFile		;
	CPictureEx  m_picture           ;
	bool        m_bModify           ;
	int         m_iEmotion          ;
	int			m_nCurGroup			;

	CFlatEdit	m_edtShortCut       ;
	CFlatEdit	m_edtTip			;
	CBmpBtn     m_btnOk,   m_btnCancel;


	CStatic     m_wndLine           ;
	bool		m_bPic;		// �Ƿ��ڳ�ʼ���Ի���ʱ�����һͼƬ
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
