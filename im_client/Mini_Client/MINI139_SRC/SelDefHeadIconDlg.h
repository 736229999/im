#pragma once

#include <vector>
#include <string>

// �Զ�����Ϣ
#define WM_SELDEFHEADDLG_MSG (WM_USER+741)
// ��Ϣ����
enum en_selhead_msg_type		
{
	en_selchange = 0,
};

// CSelDefHeadIconDlg �Ի���
class CRountFaceCtrl;
class CSelDefHeadIconDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelDefHeadIconDlg)

public:
	CSelDefHeadIconDlg(const std::vector<std::string> &vecHeadPath);   // ��׼���캯��
	virtual ~CSelDefHeadIconDlg();
	BOOL Create(CWnd *pParent);
	BOOL OnInitDialog();
	string GetSelHead()	{	return m_strHeadPath_CurSel;	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ͼ��ؼ����������Ϣ��Ӧ
	afx_msg void OnBnClickedHeadIcon(UINT uid);
	DECLARE_MESSAGE_MAP()

private:
	std::vector<std::string> m_vecHeadPath;
	std::vector<CRountFaceCtrl*> m_vecpFaceCtrl;
	HBRUSH m_hBkGround;
	std::string m_strHeadPath_CurSel;				// ��ǰѡ���ͷ��·��
};
