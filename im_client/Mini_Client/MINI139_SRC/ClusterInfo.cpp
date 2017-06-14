// ClusterInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "ClusterInfo.h"
#include "afxdialogex.h"
#include "..\IMClient.h"

// CClusterInfo 对话框

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
  m_dwSelName = 0;///群名
  m_iLenName = 0;

  m_dwSelSign = 0;//群签名
  m_iLenSign  = 0;

  m_dwSelInfo = 0;//群介绍
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


// CClusterInfo 消息处理程序
BOOL CClusterInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 遍历所有子控件，设置控件字体
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	//m_ctlClusterName.SetLimitText(20);
	//m_ctlClusterSign.SetLimitText(32);
	//m_ctlClusterInfo.SetLimitText(50);

	m_ctlClusterName.SetWindowText(_T("不超过20个字"));
	m_ctlClusterSign.SetWindowText(_T("不超过32个字"));		
	m_ctlClusterInfo.SetWindowText(_T("让更多的人了解此群，不超过50个字"));

    SetTimer(0x01,5000,NULL); ////延时增加
	SetTimer(0x02,5000,NULL);
	SetTimer(0x03,5000,NULL);

	m_ctlClusterName.SetBorderColor(RGB(200, 200, 200));
	m_ctlClusterSign.SetBorderColor(RGB(200, 200, 200));
	m_ctlClusterInfo.SetBorderColor(RGB(200, 200, 200));
	m_ctlComboKinds.SetBorderColor(RGB(200, 200, 200));
	//ComboInitFamilyClass(&m_ctlComboKinds);
	InitClusterType(&m_ctlComboKinds);///群分类
	
	return TRUE;
}

void CClusterInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTxt = _T("");
	switch(nIDEvent)
	{
		case 0x01:
			m_ctlClusterName.GetWindowText(strTxt);
			if(strTxt == _T(""))
			{
				m_ctlClusterName.SetWindowText(_T("不超过20个字"));
				m_ctlClusterName.SetSel(0,-1);
			}
		    break;
		case 0x02:
			m_ctlClusterSign.GetWindowText(strTxt);
			if(strTxt == _T(""))
			{
				m_ctlClusterSign.SetWindowText(_T("不超过32个字"));
				m_ctlClusterSign.SetSel(0,-1);
			}
			break;
		case 0x03:
			m_ctlClusterInfo.GetWindowText(strTxt);
			if(strTxt == _T(""))
			{
				m_ctlClusterInfo.SetWindowText(_T("让更多的人了解此群，不超过50个字"));
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
    //群分类
	ComboSelFamilyClass(&m_ctlComboKinds,pCluster->nFamilyClass);
}


void CClusterInfo::OnEnChangeEditClustersign()
{
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERSIGN);
	m_dwSelSign = pEdit->GetSel();//获取当前光标位置
	int maxSize =CLUSTR_SIGN;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenSign =0;//文字个数
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
				pEdit->SetSel(m_dwSelSign);//群签名//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}	
}


void CClusterInfo::OnEnChangeEditClusterinfo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERINFO);
    m_dwSelInfo = pEdit->GetSel();//获取当前光标位置
	int maxSize = CLUSTR_INFO;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenInfo =0;//文字个数
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
				pEdit->SetSel(m_dwSelInfo);//群签名//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}	

}

void CClusterInfo::OnEnChangeEditClustername()
{
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERNAME);
    m_dwSelName = pEdit->GetSel();//获取当前光标位置
	int maxSize = CLUSTR_NAME;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	m_iLenName =0;//文字个数
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
				pEdit->SetSel(m_dwSelName);//群签名//如果光标在文本结尾则无法回到文本开头
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

			if(m_iLenName>=CLUSTR_NAME)///群名
			{
				CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERNAME);
				pEdit->SetSel(m_dwSelName);//如果光标在文本结尾则无法回到文本开头
			}
			if(m_iLenSign>=CLUSTR_SIGN)//群签名
			{
			   CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERSIGN);
               pEdit->SetSel(m_dwSelSign);//如果光标在文本结尾则无法回到文本开头
			}
		    if(m_iLenInfo>=CLUSTR_INFO)///群介绍
			{
				CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CLUSTERINFO);
				pEdit->SetSel(m_dwSelInfo);//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}