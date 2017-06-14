#pragma once
#include "XSkinDlg.h"
#include "clientprotocol.h"
// CEvalueDlg dialog

class CEvalueDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CEvalueDlg)

public:
	CEvalueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEvalueDlg();

	// ��ȡ���۵ȼ�
	CString GetEvalueGrade();

	// ��ȡ����ȼ�
	EN_EVALUA_GRADE GetEvalueGradeEn();

	// ��ȡ����
	CString GetSuggest()	{ return m_strSuggest; }

protected:
	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnPaint();
	// ��Ӧ�ؼ���ɫ�����¼���������Ʊ���ɫ����ԭ����ɫ������
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// ����ύ��ť
	void OnBnClickedSubmit();

	DECLARE_MESSAGE_MAP()

private:
	CFlatEdit m_editSuggest;
	CBmpBtn m_btnSubmit;
	int m_nEvalueGrade;				// ���۵ȼ�
	CString m_strSuggest;			// ����	
};
