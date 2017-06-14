// ClusterInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClusterInfo.h"
#include "afxdialogex.h"
#include "..\IMClient.h"

// CClusterInfo �Ի���

enum  MAX_LENTH
{
   CLUSTR_NAME = 20,
   CLUSTR_SIGN = 32,
   CLUSTR_INFO = 50
};
IMPLEMENT_DYNAMIC(CClusterInfo, CDialog)

CClusterInfo::CClusterInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CClusterInfo::IDD, pParent)
{
  m_dwSelName = 0;///Ⱥ��
  m_iLenName = 0;

  m_dwSelSign = 0;//Ⱥǩ��
  m_iLenSign  = 0;

  m_dwSelInfo = 0;//Ⱥ����
  m_iLenInfo  = 0;
}

CClusterInfo::~CClusterInfo()
{
}

void CClusterInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CLUSTERNAME, m_ctlClusterName);
	DDX_Control(pDX, IDC_EDIT_CLUSTERSIGN, m_ctlClusterSign);
	DDX_Control(pDX, IDC_EDIT_CLUSTERINFO, m_ctlClusterInfo);
	DDX_Control(pDX, IDC_COMBO_KINDS, m_ctlComboKinds);
	DDX_Control(pDX, IDC_STATIC_CLUSTERID, m_ctlClusterId);
	DDX_Control(pDX, IDC_STATIC_CREATOR, m_ctlCluserOwner);
}


BEGIN_MESSAGE_MAP(CClusterInfo, CDialog)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()	
	ON_EN_CHANGE(IDC_EDIT_CLUSTERSIGN, &CClusterInfo::OnEnChangeEditClustersign)
	ON_EN_CHANGE(IDC_EDIT_CLUSTERINFO, &CClusterInfo::OnEnChangeEditClusterinfo)
//	ON_EN_CHANGE(IDC_EDIT_CLUSTERNAME, &CClusterInfo::OnEnChangeEditClustername)
ON_EN_CHANGE(IDC_EDIT_CLUSTERNAME, &CClusterInfo::OnEnChangeEditClustername)
END_MESSAGE_MAP()


// CClusterInfo ��Ϣ�������
BOOL CClusterInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���������ӿؼ������ÿؼ�����
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	//m_ctlClusterName.SetLimitText(20);
	//m_ctlClusterSign.SetLimitText(32);
	//m_ctlClusterInfo.SetLimitText(50);

	m_ctlClusterName.SetWindowText(_T("������20����"));
	m_ctlClusterSign.SetWindowText(_T("������32����"));		
	m_ctlClusterInfo.SetWindowText(_T("�ø�������˽��Ⱥ��������50����"));

    SetTimer(0x01,5000,NULL); ////��ʱ����
	SetTimer(0x02,5000,NULL);
	SetTimer(0x03,5000,NULL);

	m_ctlClusterName.SetBorderColor(RGB(200, 200, 200));
	m_ctlClusterSign.SetBorderColor(RGB(200, 200, 200));
	m_ctlClusterInfo.SetBorderColor(RGB(200, 200, 200));
	m_ctlComboKinds.SetBorderColor(RGB(200, 200, 200));
	//ComboInitFamilyClass(&m_ctlComboKinds);
	InitClusterType(&m_ctlComboKinds);///Ⱥ����
	
	return TRUE;
}

void CClusterInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTxt = _T("");
	switch(nIDEvent)
	{
		case 0x01:
			m_ctlClusterName.GetWindowText(strTxt);
			if(strTxt == _T(""))
			{
				m_ctlClusterName.SetWindowText(_T("������20����"));
				m_ctlClusterName.SetSel(0,-1);
			}
		    break;
		case 0x02:
			m_ctlClusterSign.GetWindowText(strTxt);
			if(strTxt == _T(""))
			{
				m_ctlClusterSign.SetWindowText(_T("������32����"));
				m_ctlClusterSign.SetSel(0,-1);
			}
			break;
		case 0x03:
			m_ctlClusterInfo.GetWindowText(strTxt);
			if(strTxt == _T(""))
			{
				m_ctlClusterInfo.SetWindowText(_T("�ø�������˽��Ⱥ��������50����"));
				m_ctlClusterInfo.SetSel(0,-1);
			}
			break;
		default:
			break;
	}
  
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CClusterInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nId = pWnd->GetDlgCtrlID();

		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
		{
			
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = g_hWhiteBrush;
		}

		if ( nId==IDC_STATIC_TIP || nId==IDC_STATIC_TIP2 )
		{
			pDC->SetTextColor(RGB(74,123,211));
		}

		if(nId == IDC_STATIC_NAME_REQUIRED || nId == IDC_STATIC_KINDS_REQUIRED)
		{
			pDC->SetTextColor(RGB(255,0,0));
		}
	}

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}


void  CClusterInfo::UpdateComboBox(CLUSTER_INFO *pCluster)
{
    //Ⱥ����
	ComboSelFamilyClass(&m_ctlComboKinds,pCluster->nFamilyClass);
}


void CClusterInfo::OnEnChangeEditClustersign()
{
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERSIGN);
	m_dwSelSign = pEdit->GetSel();//��ȡ��ǰ���λ��
	int maxSize =CLUSTR_SIGN;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenSign =0;//���ָ���
	for (int i=0;strName[i];i++) {		
		if (strName[i] < 0){  			   
			tmp.Format("%c%c",strName[i],strName[i+1]);
			allName = allName +tmp;
			i++;   			
		}
		else{
		  tmp.Format("%c",strName[i]);
		  allName = allName +tmp;
		}
		m_iLenSign++;
		if (m_iLenSign>=maxSize){
			if (strName != allName)
			{
				pEdit->SetWindowText(allName);
				pEdit->SetSel(m_dwSelSign);//Ⱥǩ��//���������ı���β���޷��ص��ı���ͷ
			}
			break;
		}
	}	
}


void CClusterInfo::OnEnChangeEditClusterinfo()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERINFO);
    m_dwSelInfo = pEdit->GetSel();//��ȡ��ǰ���λ��
	int maxSize = CLUSTR_INFO;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenInfo =0;//���ָ���
	for (int i=0;strName[i];i++) {		
		if (strName[i] < 0){  			   
			tmp.Format("%c%c",strName[i],strName[i+1]);
			allName = allName +tmp;
			i++;   			
		}
		else{
			tmp.Format("%c",strName[i]);
			allName = allName +tmp;
		}
		m_iLenInfo++;
		if (m_iLenInfo>=maxSize){
			if (strName != allName)
			{
				pEdit->SetWindowText(allName);
				pEdit->SetSel(m_dwSelInfo);//Ⱥǩ��//���������ı���β���޷��ص��ı���ͷ
			}
			break;
		}
	}	

}

void CClusterInfo::OnEnChangeEditClustername()
{
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERNAME);
    m_dwSelName = pEdit->GetSel();//��ȡ��ǰ���λ��
	int maxSize = CLUSTR_NAME;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenName =0;//���ָ���
	for (int i=0;strName[i];i++) {		
		if (strName[i] < 0){  			   
			tmp.Format("%c%c",strName[i],strName[i+1]);
			allName = allName +tmp;
			i++;   			
		}
		else{
			tmp.Format("%c",strName[i]);
			allName = allName +tmp;
		}
		m_iLenName++;
		if (m_iLenName>=maxSize){
			if (strName != allName)
			{
				pEdit->SetWindowText(allName);
				pEdit->SetSel(m_dwSelName);//Ⱥǩ��//���������ı���β���޷��ص��ı���ͷ
			}
			break;
		}
	}	

}

BOOL CClusterInfo::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
			}
			return TRUE;
			break;
		case VK_ESCAPE:
		    return TRUE;
			break;
       
		default:

			if(m_iLenName>=CLUSTR_NAME)///Ⱥ��
			{
				CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERNAME);
				pEdit->SetSel(m_dwSelName);//���������ı���β���޷��ص��ı���ͷ
			}
			if(m_iLenSign>=CLUSTR_SIGN)//Ⱥǩ��
			{
			   CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERSIGN);
               pEdit->SetSel(m_dwSelSign);//���������ı���β���޷��ص��ı���ͷ
			}
		    if(m_iLenInfo>=CLUSTR_INFO)///Ⱥ����
			{
				CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERINFO);
				pEdit->SetSel(m_dwSelInfo);//���������ı���β���޷��ص��ı���ͷ
			}
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}